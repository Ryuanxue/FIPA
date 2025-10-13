/* Handling of recursive HTTP retrieving.
   Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
   2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2015 Free Software
   Foundation, Inc.

This file is part of GNU Wget.

GNU Wget is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

GNU Wget is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Wget.  If not, see <http://www.gnu.org/licenses/>.

Additional permission under GNU GPL version 3 section 7

If you modify this program, or any covered work, by linking or
combining it with the OpenSSL project's OpenSSL library (or a
modified version of that library), containing parts covered by the
terms of the OpenSSL or SSLeay licenses, the Free Software Foundation
grants you additional permission to convey the resulting work.
Corresponding Source for a non-source form of such a combination
shall include the source code for the parts of OpenSSL used as well
as that of the covered work.  */

#include "wget.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "url.h"
#include "recur.h"
#include "utils.h"
#include "retr.h"
#include "ftp.h"
#include "host.h"
#include "hash.h"
#include "res.h"
#include "convert.h"
#include "html-url.h"
#include "css-url.h"
#include "spider.h"
#include "exits.h"

#include "wget_rpc_wrapper.h"

/* Functions for maintaining the URL queue.  */

struct queue_element {
  const char *url;              /* the URL to download */
  const char *referer;          /* the referring document */
  int depth;                    /* the depth */
  bool html_allowed;            /* whether the document is allowed to
                                   be treated as HTML. */
  struct iri *iri;                /* sXXXav */
  bool css_allowed;             /* whether the document is allowed to
                                   be treated as CSS. */
  struct queue_element *next;   /* next element in queue */
};

struct url_queue {
  struct queue_element *head;
  struct queue_element *tail;
  int count, maxcount;
};

/* Create a URL queue. */

static struct url_queue *
url_queue_new (void)
{
  struct url_queue *queue = xnew0 (struct url_queue);
  return queue;
}

/* Delete a URL queue. */

static void
url_queue_delete (struct url_queue *queue)
{
  xfree (queue);
}

/* Enqueue a URL in the queue.  The queue is FIFO: the items will be
   retrieved ("dequeued") from the queue in the order they were placed
   into it.  */

static void url_enqueue(struct url_queue *queue, struct iri *i, const char *url, const char *referer, int depth, bool html_allowed, bool css_allowed)
{
  struct queue_element *qel = xmalloc(sizeof(struct queue_element));
  qel->iri = i;
  qel->url = url;
  qel->referer = referer;
  qel->depth = depth;
  qel->html_allowed = html_allowed;
  qel->css_allowed = css_allowed;
  qel->next = 0;
  ++queue->count;
  if (queue->count > queue->maxcount)
    queue->maxcount = queue->count;
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Enqueuing %s at depth %d\n", quotearg_n_style(0, escape_quoting_style, url), depth);
    }
  }
  while (0);
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Queue count %d, maxcount %d.\n", queue->count, queue->maxcount);
    }
  }
  while (0);
  if (i)
    do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("[IRI Enqueuing %s with %s\n", quote_n(0, url), (i->uri_encoding) ? (quote_n(1, i->uri_encoding)) : ("None"));
    }
  }
  while (0);
  if (queue->tail)
    queue->tail->next = qel;
  queue->tail = qel;
  if (!queue->head)
    queue->head = queue->tail;
}



/* Take a URL out of the queue.  Return true if this operation
   succeeded, or false if the queue is empty.  */

static bool url_dequeue(struct url_queue *queue, struct iri **i, const char **url, const char **referer, int *depth, bool *html_allowed, bool *css_allowed)
{
  struct queue_element *qel = queue->head;
  if (!qel)
    return 0;
  queue->head = queue->head->next;
  if (!queue->head)
    queue->tail = 0;
  *i = qel->iri;
  *url = qel->url;
  *referer = qel->referer;
  *depth = qel->depth;
  *html_allowed = qel->html_allowed;
  *css_allowed = qel->css_allowed;
  --queue->count;
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Dequeuing %s at depth %d\n", quotearg_n_style(0, escape_quoting_style, qel->url), qel->depth);
    }
  }
  while (0);
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Queue count %d, maxcount %d.\n", queue->count, queue->maxcount);
    }
  }
  while (0);
  do
  {
    free((void *) qel);
    qel = 0;
  }
  while (0);
  return 1;
}



static void blacklist_add (struct hash_table *blacklist, const char *url)
{
  char *url_unescaped = xstrdup (url);

  url_unescape (url_unescaped);
  string_set_add (blacklist, url_unescaped);
  xfree (url_unescaped);
}

static int blacklist_contains (struct hash_table *blacklist, const char *url)
{
  char *url_unescaped = xstrdup(url);
  int ret;

  url_unescape (url_unescaped);
  ret = string_set_contains (blacklist, url_unescaped);
  xfree (url_unescaped);

  return ret;
}

typedef enum
{
  WG_RR_SUCCESS, WG_RR_BLACKLIST, WG_RR_NOTHTTPS, WG_RR_NONHTTP, WG_RR_ABSOLUTE,
  WG_RR_DOMAIN, WG_RR_PARENT, WG_RR_LIST, WG_RR_REGEX, WG_RR_RULES,
  WG_RR_SPANNEDHOST, WG_RR_ROBOTS
} reject_reason;

static reject_reason download_child (const struct urlpos *, struct url *, int,
                              struct url *, struct hash_table *, struct iri *);
static reject_reason descend_redirect (const char *, struct url *, int,
                              struct url *, struct hash_table *, struct iri *);
static void write_reject_log_header (FILE *);
static void write_reject_log_reason (FILE *, reject_reason,
                              const struct url *, const struct url *);

/* Retrieve a part of the web beginning with START_URL.  This used to
   be called "recursive retrieval", because the old function was
   recursive and implemented depth-first search.  retrieve_tree on the
   other hand implements breadth-search traversal of the tree, which
   results in much nicer ordering of downloads.

   The algorithm this function uses is simple:

   1. put START_URL in the queue.
   2. while there are URLs in the queue:

     3. get next URL from the queue.
     4. download it.
     5. if the URL is HTML and its depth does not exceed maximum depth,
        get the list of URLs embedded therein.
     6. for each of those URLs do the following:

       7. if the URL is not one of those downloaded before, and if it
          satisfies the criteria specified by the various command-line
          options, add it to the queue. */

uerr_t retrieve_tree(struct url *start_url_parsed, struct iri *pi)
{
  uerr_t status = RETROK;
  struct url_queue *queue;
  struct hash_table *blacklist;
  struct iri *i = &get_dummy_iri_wrapper();
  FILE *rejectedlog = 0;
  if (pi)
  {
    i->uri_encoding = (pi->uri_encoding) ? (xstrdup(pi->uri_encoding)) : (0);
    ;
    i->content_encoding = (pi->content_encoding) ? (xstrdup(pi->content_encoding)) : (0);
    ;
    i->utf8_encode = pi->utf8_encode;
  }
  queue = url_queue_new();
  blacklist = make_string_hash_table(0);
  url_enqueue(queue, i, xstrdup(start_url_parsed->url), 0, 0, 1, 0);
  blacklist_add(blacklist, start_url_parsed->url);
  if (get_opt_rejected_log_wrapper())
  {
    rejectedlog = fopen(get_opt_rejected_log_wrapper(), "w");
    write_reject_log_header(rejectedlog);
    if (!rejectedlog)
      logprintf(LOG_NOTQUIET, "%s: %s\n", get_opt_rejected_log_wrapper(), strerror(errno));
  }
  while (1)
  {
    bool descend = 0;
    char *url;
    char *referer;
    char *file = 0;
    int depth;
    bool html_allowed;
    bool css_allowed;
    bool is_css = 0;
    bool dash_p_leaf_HTML = 0;
    if (get_opt_quota_wrapper() && (get_total_downloaded_bytes_wrapper() > get_opt_quota_wrapper()))
      break;
    if (status == FWRITEERR)
      break;
    if (!url_dequeue(queue, (struct iri **) (&i), (const char **) (&url), (const char **) (&referer), &depth, &html_allowed, &css_allowed))
      break;
    if (get_dl_url_file_map_wrapper() && hash_table_contains(get_dl_url_file_map_wrapper(), url))
    {
      bool is_css_bool;
      file = xstrdup(hash_table_get(get_dl_url_file_map_wrapper(), url));
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Already downloaded \"%s\", reusing it from \"%s\".\n", url, file);
        }
      }
      while (0);
      if ((is_css_bool = (css_allowed && get_downloaded_css_set_wrapper()) && string_set_contains(get_downloaded_css_set_wrapper(), file)) || ((html_allowed && get_downloaded_html_set_wrapper()) && string_set_contains(get_downloaded_html_set_wrapper(), file)))
      {
        descend = 1;
        is_css = is_css_bool;
      }
    }
    else
    {
      int dt = 0;
      int url_err;
      char *redirected = 0;
      struct url *url_parsed = url_parse(url, &url_err, i, 1);
      if (!url_parsed)
      {
        char *error = url_error(url, url_err);
        logprintf(LOG_NOTQUIET, "%s: %s.\n", url, error);
        do
        {
          free((void *) error);
          error = 0;
        }
        while (0);
        inform_exit_status(URLERROR);
      }
      else
      {
        status = retrieve_url(url_parsed, url, &file, &redirected, referer, &dt, 0, i, 1);
        if ((((html_allowed && file) && (status == RETROK)) && (dt & RETROKF)) && (dt & TEXTHTML))
        {
          descend = 1;
          is_css = 0;
        }
        if (((file && (status == RETROK)) && (dt & RETROKF)) && ((dt & TEXTCSS) || css_allowed))
        {
          descend = 1;
          is_css = 1;
        }
        if (redirected)
        {
          if (descend)
          {
            reject_reason r = descend_redirect(redirected, url_parsed, depth, start_url_parsed, blacklist, i);
            if (r == WG_RR_SUCCESS)
            {
              blacklist_add(blacklist, url);
            }
            else
            {
              write_reject_log_reason(rejectedlog, r, url_parsed, start_url_parsed);
              descend = 0;
            }
          }
          do
          {
            free((void *) url);
            url = 0;
          }
          while (0);
          url = redirected;
        }
        else
        {
          do
          {
            free((void *) url);
            url = 0;
          }
          while (0);
          url = xstrdup(url_parsed->url);
        }
        url_free(url_parsed);
      }
    }
    if (get_opt_spider_wrapper())
    {
      ;
    }
    if ((descend && (depth >= get_opt_reclevel_wrapper())) && (get_opt_reclevel_wrapper() != (-1)))
    {
      if (get_opt_page_requisites_wrapper() && ((depth == get_opt_reclevel_wrapper()) || (depth == (get_opt_reclevel_wrapper() + 1))))
      {
        dash_p_leaf_HTML = 1;
      }
      else
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Not descending further; at depth %d, max. %d.\n", depth, get_opt_reclevel_wrapper());
          }
        }
        while (0);
        descend = 0;
      }
    }
    if (descend)
    {
      bool meta_disallow_follow = 0;
      struct urlpos *children = (is_css) ? (get_urls_css_file(file, url)) : (get_urls_html(file, url, &meta_disallow_follow, i));
      if (get_opt_use_robots_wrapper() && meta_disallow_follow)
      {
        free_urlpos(children);
        children = 0;
      }
      if (children)
      {
        struct urlpos *child = children;
        struct url *url_parsed = url_parse(url, 0, i, 1);
        struct iri *ci;
        char *referer_url = url;
        bool strip_auth = (url_parsed != 0) && (url_parsed->user != 0);
        assert(url_parsed != 0);
        if (strip_auth)
          referer_url = url_string(url_parsed, URL_AUTH_HIDE);
        for (; child; child = child->next)
        {
          reject_reason r;
          if (child->ignore_when_downloading)
            continue;
          if (dash_p_leaf_HTML && (!child->link_inline_p))
            continue;
          r = download_child(child, url_parsed, depth, start_url_parsed, blacklist, i);
          if (r == WG_RR_SUCCESS)
          {
            ci = &get_dummy_iri_wrapper();
            ;
            url_enqueue(queue, ci, xstrdup(child->url->url), xstrdup(referer_url), depth + 1, child->link_expect_html, child->link_expect_css);
            blacklist_add(blacklist, child->url->url);
          }
          else
          {
            write_reject_log_reason(rejectedlog, r, child->url, url_parsed);
          }
        }

        if (strip_auth)
          do
        {
          free((void *) referer_url);
          referer_url = 0;
        }
        while (0);
        url_free(url_parsed);
        free_urlpos(children);
      }
    }
    if (file && ((get_opt_delete_after_wrapper() || get_opt_spider_wrapper()) || (!acceptable(file))))
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Removing file due to %s in recursive_retrieve():\n", (get_opt_delete_after_wrapper()) ? ("--delete-after") : ((get_opt_spider_wrapper()) ? ("--spider") : ("recursive rejection criteria")));
        }
      }
      while (0);
      logprintf(LOG_VERBOSE, (get_opt_delete_after_wrapper() || get_opt_spider_wrapper()) ? (gettext("Removing %s.\n")) : (gettext("Removing %s since it should be rejected.\n")), file);
      if (unlink(file))
        logprintf(LOG_NOTQUIET, "unlink: %s\n", strerror(errno));
      logputs(LOG_VERBOSE, "\n");
      register_delete_file(file);
    }
    do
    {
      free((void *) url);
      url = 0;
    }
    while (0);
    do
    {
      free((void *) referer);
      referer = 0;
    }
    while (0);
    do
    {
      free((void *) file);
      file = 0;
    }
    while (0);
    ;
  }

  if (rejectedlog)
    fclose(rejectedlog);
  {
    char *d1;
    char *d2;
    int d3;
    bool d4;
    bool d5;
    struct iri *d6;
    while (url_dequeue(queue, (struct iri **) (&d6), (const char **) (&d1), (const char **) (&d2), &d3, &d4, &d5))
    {
      ;
      do
      {
        free((void *) d1);
        d1 = 0;
      }
      while (0);
      do
      {
        free((void *) d2);
        d2 = 0;
      }
      while (0);
    }

  }
  url_queue_delete(queue);
  string_set_free(blacklist);
  if (get_opt_quota_wrapper() && (get_total_downloaded_bytes_wrapper() > get_opt_quota_wrapper()))
    return QUOTEXC;
  else
    if (status == FWRITEERR)
    return FWRITEERR;
  else
    return RETROK;
}



/* Based on the context provided by retrieve_tree, decide whether a
   URL is to be descended to.  This is only ever called from
   retrieve_tree, but is in a separate function for clarity.

   The most expensive checks (such as those for robots) are memoized
   by storing these URLs to BLACKLIST.  This may or may not help.  It
   will help if those URLs are encountered many times.  */

static reject_reason download_child(const struct urlpos *upos, struct url *parent, int depth, struct url *start_url_parsed, struct hash_table *blacklist, struct iri *iri)
{
  struct url *u = upos->url;
  const char *url = u->url;
  bool u_scheme_like_http;
  reject_reason reason = WG_RR_SUCCESS;
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Deciding whether to enqueue \"%s\".\n", url);
    }
  }
  while (0);
  if (blacklist_contains(blacklist, url))
  {
    if (get_opt_spider_wrapper())
    {
      char *referrer = url_string(parent, URL_AUTH_HIDE_PASSWD);
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("download_child: parent->url is: %s\n", quote(parent->url));
        }
      }
      while (0);
      ;
      do
      {
        free((void *) referrer);
        referrer = 0;
      }
      while (0);
    }
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Already on the black list.\n");
      }
    }
    while (0);
    reason = WG_RR_BLACKLIST;
    goto out;
  }
  if (get_opt_https_only_wrapper() && (u->scheme != SCHEME_HTTPS))
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Not following non-HTTPS links.\n");
      }
    }
    while (0);
    reason = WG_RR_NOTHTTPS;
    goto out;
  }
  u_scheme_like_http = schemes_are_similar_p(u->scheme, SCHEME_HTTP);
  if ((!u_scheme_like_http) && (!(((u->scheme == SCHEME_FTP) || (u->scheme == SCHEME_FTPS)) && get_opt_follow_ftp_wrapper())))
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Not following non-HTTP schemes.\n");
      }
    }
    while (0);
    reason = WG_RR_NONHTTP;
    goto out;
  }
  if (u_scheme_like_http)
    if (get_opt_relative_only_wrapper() && (!upos->link_relative_p))
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("It doesn't really look like a relative link.\n");
      }
    }
    while (0);
    reason = WG_RR_ABSOLUTE;
    goto out;
  }
  if (!accept_domain(u))
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("The domain was not accepted.\n");
      }
    }
    while (0);
    reason = WG_RR_DOMAIN;
    goto out;
  }
  if ((((get_opt_no_parent_wrapper() && schemes_are_similar_p(u->scheme, start_url_parsed->scheme)) && (0 == strcasecmp(u->host, start_url_parsed->host))) && ((u->scheme != start_url_parsed->scheme) || (u->port == start_url_parsed->port))) && (!(get_opt_page_requisites_wrapper() && upos->link_inline_p)))
  {
    if (!subdir_p(start_url_parsed->dir, u->dir))
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Going to \"%s\" would escape \"%s\" with no_parent on.\n", u->dir, start_url_parsed->dir);
        }
      }
      while (0);
      reason = WG_RR_PARENT;
      goto out;
    }
  }
  if (get_opt_includes_wrapper() || get_opt_excludes_wrapper())
  {
    if (!accdir(u->dir))
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("%s (%s) is excluded/not-included.\n", url, u->dir);
        }
      }
      while (0);
      reason = WG_RR_LIST;
      goto out;
    }
  }
  if (!accept_url(url))
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("%s is excluded/not-included through regex.\n", url);
      }
    }
    while (0);
    reason = WG_RR_REGEX;
    goto out;
  }
  if ((u->file[0] != '\0') && (!(has_html_suffix_p(u->file) && (((get_opt_reclevel_wrapper() == (-1)) || (depth < (get_opt_reclevel_wrapper() - 1))) || get_opt_page_requisites_wrapper()))))
  {
    if (!acceptable(u->file))
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("%s (%s) does not match acc/rej rules.\n", url, u->file);
        }
      }
      while (0);
      reason = WG_RR_RULES;
      goto out;
    }
  }
  if (schemes_are_similar_p(u->scheme, parent->scheme))
    if ((!get_opt_spanhost_wrapper()) && (0 != strcasecmp(parent->host, u->host)))
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("This is not the same hostname as the parent's (%s and %s).\n", u->host, parent->host);
      }
    }
    while (0);
    reason = WG_RR_SPANNEDHOST;
    goto out;
  }
  if (get_opt_use_robots_wrapper() && u_scheme_like_http)
  {
    struct robot_specs *specs = res_get_specs(u->host, u->port);
    if (!specs)
    {
      char *rfile;
      if (res_retrieve_file(url, &rfile, iri))
      {
        specs = res_parse_from_file(rfile);
        if (get_opt_delete_after_wrapper() || get_opt_spider_wrapper())
        {
          logprintf(LOG_VERBOSE, gettext("Removing %s.\n"), rfile);
          if (unlink(rfile))
            logprintf(LOG_NOTQUIET, "unlink: %s\n", strerror(errno));
        }
        do
        {
          free((void *) rfile);
          rfile = 0;
        }
        while (0);
      }
      else
      {
        specs = res_parse("", 0);
      }
      res_register_specs(u->host, u->port, specs);
    }
    if (!res_match_path(specs, u->path))
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Not following %s because robots.txt forbids it.\n", url);
        }
      }
      while (0);
      blacklist_add(blacklist, url);
      reason = WG_RR_ROBOTS;
      goto out;
    }
  }
  out:
  if (reason == WG_RR_SUCCESS)
    do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Decided to load it.\n");
    }
  }
  while (0);
  else
    do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Decided NOT to load it.\n");
    }
  }
  while (0);

  return reason;
}



/* This function determines whether we will consider downloading the
   children of a URL whose download resulted in a redirection,
   possibly to another host, etc.  It is needed very rarely, and thus
   it is merely a simple-minded wrapper around download_child.  */

static reject_reason descend_redirect(const char *redirected, struct url *orig_parsed, int depth, struct url *start_url_parsed, struct hash_table *blacklist, struct iri *iri)
{
  struct url *new_parsed;
  struct urlpos *upos;
  reject_reason reason;
  assert(orig_parsed != 0);
  new_parsed = url_parse(redirected, 0, 0, 0);
  assert(new_parsed != 0);
  upos = xcalloc(1, sizeof(struct urlpos));
  upos->url = new_parsed;
  reason = download_child(upos, orig_parsed, depth, start_url_parsed, blacklist, iri);
  if (reason == WG_RR_SUCCESS)
    blacklist_add(blacklist, upos->url->url);
  else
    do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Redirection \"%s\" failed the test.\n", redirected);
    }
  }
  while (0);
  url_free(new_parsed);
  do
  {
    free((void *) upos);
    upos = 0;
  }
  while (0);
  return reason;
}




/* This function writes the rejected log header. */
static void
write_reject_log_header (FILE *f)
{
  if (!f)
    return;

  /* Note: Update this header when columns change in any way. */
  fprintf (f, "REASON\t"
    "U_URL\tU_SCHEME\tU_HOST\tU_PORT\tU_PATH\tU_PARAMS\tU_QUERY\tU_FRAGMENT\t"
    "P_URL\tP_SCHEME\tP_HOST\tP_PORT\tP_PATH\tP_PARAMS\tP_QUERY\tP_FRAGMENT\n");
}

/* This function writes a URL to the reject log. Internal use only. */
static void
write_reject_log_url (FILE *fp, const struct url *url)
{
  const char *escaped_str;
  const char *scheme_str;

  if (!fp)
    return;

  escaped_str = url_escape (url->url);

  switch (url->scheme)
    {
      case SCHEME_HTTP:  scheme_str = "SCHEME_HTTP";    break;
#ifdef HAVE_SSL
      case SCHEME_HTTPS: scheme_str = "SCHEME_HTTPS";   break;
      case SCHEME_FTPS:  scheme_str = "SCHEME_FTPS";    break;
#endif
      case SCHEME_FTP:   scheme_str = "SCHEME_FTP";     break;
      default:           scheme_str = "SCHEME_INVALID"; break;
    }

  fprintf (fp, "%s\t%s\t%s\t%i\t%s\t%s\t%s\t%s",
    escaped_str,
    scheme_str,
    url->host,
    url->port,
    url->path,
    url->params ? url->params : "",
    url->query ? url->query : "",
    url->fragment ? url->fragment : "");

  xfree (escaped_str);
}

/* This function writes out information on why a URL was rejected and its
   context from download_child such as the URL being rejected and it's
   parent's URL. The format it uses is comma separated values but with tabs. */
static void
write_reject_log_reason (FILE *fp, reject_reason reason,
                         const struct url *url, const struct url *parent)
{
  const char *reason_str;

  if (!fp)
    return;

  switch (reason)
    {
      case WG_RR_SUCCESS:     reason_str = "SUCCESS";     break;
      case WG_RR_BLACKLIST:   reason_str = "BLACKLIST";   break;
      case WG_RR_NOTHTTPS:    reason_str = "NOTHTTPS";    break;
      case WG_RR_NONHTTP:     reason_str = "NONHTTP";     break;
      case WG_RR_ABSOLUTE:    reason_str = "ABSOLUTE";    break;
      case WG_RR_DOMAIN:      reason_str = "DOMAIN";      break;
      case WG_RR_PARENT:      reason_str = "PARENT";      break;
      case WG_RR_LIST:        reason_str = "LIST";        break;
      case WG_RR_REGEX:       reason_str = "REGEX";       break;
      case WG_RR_RULES:       reason_str = "RULES";       break;
      case WG_RR_SPANNEDHOST: reason_str = "SPANNEDHOST"; break;
      case WG_RR_ROBOTS:      reason_str = "ROBOTS";      break;
      default:                reason_str = "UNKNOWN";     break;
    }

  fprintf (fp, "%s\t", reason_str);
  write_reject_log_url (fp, url);
  fprintf (fp, "\t");
  write_reject_log_url (fp, parent);
  fprintf (fp, "\n");
}

/* vim:set sts=2 sw=2 cino+={s: */
