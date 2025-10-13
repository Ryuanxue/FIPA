/* Conversion of links to local files.
   Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011,
   2014, 2015 Free Software Foundation, Inc.

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
#include "convert.h"
#include "url.h"
#include "recur.h"
#include "utils.h"
#include "hash.h"
#include "ptimer.h"
#include "res.h"
#include "html-url.h"
#include "css-url.h"
#include "iri.h"
#include "xstrndup.h"

#include "wget_rpc_wrapper.h"

static struct hash_table *dl_file_url_map;
struct hash_table *dl_url_file_map;

/* Set of HTML/CSS files downloaded in this Wget run, used for link
   conversion after Wget is done.  */
struct hash_table *downloaded_html_set;
struct hash_table *downloaded_css_set;

static void convert_links (const char *, struct urlpos *);


static void convert_links_in_hashtable(struct hash_table *downloaded_set, int is_css, int *file_count)
{
  int i;
  int cnt;
  char **file_array;
  cnt = 0;
  if (downloaded_set)
    cnt = hash_table_count(downloaded_set);
  if (cnt == 0)
    return;
  file_array = (char **) alloca(cnt * (sizeof(char *)));
  string_set_to_array(downloaded_set, file_array);
  for (i = 0; i < cnt; i++)
  {
    struct urlpos *urls;
    struct urlpos *cur_url;
    char *url;
    char *file = file_array[i];
    url = hash_table_get(get_dl_file_url_map_wrapper(), file);
    if (!url)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Apparently %s has been removed.\n", file);
        }
      }
      while (0);
      continue;
    }
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Scanning %s (from %s)\n", file, url);
      }
    }
    while (0);
    urls = (is_css) ? (get_urls_css_file(file, url)) : (get_urls_html(file, url, 0, 0));
    for (cur_url = urls; cur_url; cur_url = cur_url->next)
    {
      char *local_name;
      struct url *u;
      struct iri *pi;
      if (cur_url->link_base_p)
      {
        cur_url->convert = CO_NULLIFY_BASE;
        continue;
      }
      pi = &get_dummy_iri_wrapper();
      ;
      u = url_parse(cur_url->url->url, 0, pi, 1);
      if (!u)
        continue;
      local_name = hash_table_get(get_dl_url_file_map_wrapper(), u->url);
      if (local_name)
      {
        cur_url->convert = (get_opt_convert_file_only_wrapper()) ? (CO_CONVERT_BASENAME_ONLY) : (CO_CONVERT_TO_RELATIVE);
        cur_url->local_name = xstrdup(local_name);
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("will convert url %s to local %s\n", u->url, local_name);
          }
        }
        while (0);
      }
      else
      {
        if (!cur_url->link_complete_p)
          cur_url->convert = CO_CONVERT_TO_COMPLETE;
        cur_url->local_name = 0;
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("will convert url %s to complete\n", u->url);
          }
        }
        while (0);
      }
      url_free(u);
      ;
    }

    convert_links(file, urls);
    ++(*file_count);
    free_urlpos(urls);
  }

}



/* This function is called when the retrieval is done to convert the
   links that have been downloaded.  It has to be called at the end of
   the retrieval, because only then does Wget know conclusively which
   URLs have been downloaded, and which not, so it can tell which
   direction to convert to.

   The "direction" means that the URLs to the files that have been
   downloaded get converted to the relative URL which will point to
   that file.  And the other URLs get converted to the remote URL on
   the server.

   All the downloaded HTMLs are kept in downloaded_html_files, and
   downloaded URLs in urls_downloaded.  All the information is
   extracted from these two lists.  */

void convert_all_links(void)
{
  double secs;
  int file_count = 0;
  struct ptimer *timer = ptimer_new();
  convert_links_in_hashtable(get_downloaded_html_set_wrapper(), 0, &file_count);
  convert_links_in_hashtable(get_downloaded_css_set_wrapper(), 1, &file_count);
  secs = ptimer_measure(timer);
  logprintf(LOG_VERBOSE, gettext("Converted links in %d files in %s seconds.\n"), file_count, print_decimal(secs));
  ptimer_destroy(timer);
}



static void write_backup_file (const char *, downloaded_file_t);
static const char *replace_plain (const char*, int, FILE*, const char *);
static const char *replace_attr (const char *, int, FILE *, const char *);
static const char *replace_attr_refresh_hack (const char *, int, FILE *,
                                              const char *, int);
static char *local_quote_string (const char *, bool);
static char *construct_relative (const char *, const char *);
static char *convert_basename (const char *, const struct urlpos *);

/* Change the links in one file.  LINKS is a list of links in the
   document, along with their positions and the desired direction of
   the conversion.  */
static void convert_links(const char *file, struct urlpos *links)
{
  struct file_memory *fm;
  FILE *fp;
  const char *p;
  downloaded_file_t downloaded_file_return;
  struct urlpos *link;
  int to_url_count = 0;
  int to_file_count = 0;
  logprintf(LOG_VERBOSE, gettext("Converting links in %s... "), file);
  {
    int dry_count = 0;
    struct urlpos *dry;
    for (dry = links; dry; dry = dry->next)
      if (dry->convert != CO_NOCONVERT)
      ++dry_count;

    if (!dry_count)
    {
      logputs(LOG_VERBOSE, gettext("nothing to do.\n"));
      return;
    }
  }
  fm = wget_read_file(file);
  if (!fm)
  {
    logprintf(LOG_NOTQUIET, gettext("Cannot convert links in %s: %s\n"), file, strerror(errno));
    return;
  }
  downloaded_file_return = downloaded_file(CHECK_FOR_FILE, file);
  if (get_opt_backup_converted_wrapper() && downloaded_file_return)
    write_backup_file(file, downloaded_file_return);
  if ((unlink(file) < 0) && (errno != ENOENT))
  {
    logprintf(LOG_NOTQUIET, gettext("Unable to delete %s: %s\n"), quote(file), strerror(errno));
    wget_read_file_free(fm);
    return;
  }
  fp = fopen(file, "wb");
  if (!fp)
  {
    logprintf(LOG_NOTQUIET, gettext("Cannot convert links in %s: %s\n"), file, strerror(errno));
    wget_read_file_free(fm);
    return;
  }
  p = fm->content;
  for (link = links; link; link = link->next)
  {
    char *url_start = fm->content + link->pos;
    if (link->pos >= fm->length)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Something strange is going on.  Please investigate.");
        }
      }
      while (0);
      break;
    }
    if (link->convert == CO_NOCONVERT)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Skipping %s at position %d.\n", link->url->url, link->pos);
        }
      }
      while (0);
      continue;
    }
    fwrite(p, 1, url_start - p, fp);
    p = url_start;
    switch (link->convert)
    {
      case CO_CONVERT_TO_RELATIVE:
      {
        char *newname = construct_relative(file, link->local_name);
        char *quoted_newname = local_quote_string(newname, link->link_css_p);
        if (link->link_css_p || link->link_noquote_html_p)
          p = replace_plain(p, link->size, fp, quoted_newname);
        else
          if (!link->link_refresh_p)
          p = replace_attr(p, link->size, fp, quoted_newname);
        else
          p = replace_attr_refresh_hack(p, link->size, fp, quoted_newname, link->refresh_timeout);
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("TO_RELATIVE: %s to %s at position %d in %s.\n", link->url->url, newname, link->pos, file);
          }
        }
        while (0);
        do
        {
          free((void *) newname);
          newname = 0;
        }
        while (0);
        do
        {
          free((void *) quoted_newname);
          quoted_newname = 0;
        }
        while (0);
        ++to_file_count;
        break;
      }

      case CO_CONVERT_BASENAME_ONLY:
      {
        char *newname = convert_basename(p, link);
        char *quoted_newname = local_quote_string(newname, link->link_css_p);
        if (link->link_css_p || link->link_noquote_html_p)
          p = replace_plain(p, link->size, fp, quoted_newname);
        else
          if (!link->link_refresh_p)
          p = replace_attr(p, link->size, fp, quoted_newname);
        else
          p = replace_attr_refresh_hack(p, link->size, fp, quoted_newname, link->refresh_timeout);
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Converted file part only: %s to %s at position %d in %s.\n", link->url->url, newname, link->pos, file);
          }
        }
        while (0);
        do
        {
          free((void *) newname);
          newname = 0;
        }
        while (0);
        do
        {
          free((void *) quoted_newname);
          quoted_newname = 0;
        }
        while (0);
        ++to_file_count;
        break;
      }

      case CO_CONVERT_TO_COMPLETE:
      {
        char *newlink = link->url->url;
        char *quoted_newlink = html_quote_string(newlink);
        if (link->link_css_p || link->link_noquote_html_p)
          p = replace_plain(p, link->size, fp, newlink);
        else
          if (!link->link_refresh_p)
          p = replace_attr(p, link->size, fp, quoted_newlink);
        else
          p = replace_attr_refresh_hack(p, link->size, fp, quoted_newlink, link->refresh_timeout);
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("TO_COMPLETE: <something> to %s at position %d in %s.\n", newlink, link->pos, file);
          }
        }
        while (0);
        do
        {
          free((void *) quoted_newlink);
          quoted_newlink = 0;
        }
        while (0);
        ++to_url_count;
        break;
      }

      case CO_NULLIFY_BASE:
        p = replace_attr(p, link->size, fp, "");
        break;

      case CO_NOCONVERT:
        abort();
        break;

    }

  }

  if ((p - fm->content) < fm->length)
    fwrite(p, 1, fm->length - (p - fm->content), fp);
  fclose(fp);
  wget_read_file_free(fm);
  logprintf(LOG_VERBOSE, "%d-%d\n", to_file_count, to_url_count);
}



/* Construct and return a link that points from BASEFILE to LINKFILE.
   Both files should be local file names, BASEFILE of the referrering
   file, and LINKFILE of the referred file.

   Examples:

   cr("foo", "bar")         -> "bar"
   cr("A/foo", "A/bar")     -> "bar"
   cr("A/foo", "A/B/bar")   -> "B/bar"
   cr("A/X/foo", "A/Y/bar") -> "../Y/bar"
   cr("X/", "Y/bar")        -> "../Y/bar" (trailing slash does matter in BASE)

   Both files should be absolute or relative, otherwise strange
   results might ensue.  The function makes no special efforts to
   handle "." and ".." in links, so make sure they're not there
   (e.g. using path_simplify).  */

static char *
construct_relative (const char *basefile, const char *linkfile)
{
  char *link;
  int basedirs;
  const char *b, *l;
  int i, start;

  /* First, skip the initial directory components common to both
     files.  */
  start = 0;
  for (b = basefile, l = linkfile; *b == *l && *b != '\0'; ++b, ++l)
    {
      if (*b == '/')
        start = (b - basefile) + 1;
    }
  basefile += start;
  linkfile += start;

  /* With common directories out of the way, the situation we have is
     as follows:
         b - b1/b2/[...]/bfile
         l - l1/l2/[...]/lfile

     The link we're constructing needs to be:
       lnk - ../../l1/l2/[...]/lfile

     Where the number of ".."'s equals the number of bN directory
     components in B.  */

  /* Count the directory components in B. */
  basedirs = 0;
  for (b = basefile; *b; b++)
    {
      if (*b == '/')
        ++basedirs;
    }

  if (!basedirs && (b = strpbrk (linkfile, "/:")) && *b == ':')
    {
      link = xmalloc (2 + strlen (linkfile) + 1);
      memcpy (link, "./", 2);
      strcpy (link + 2, linkfile);
    }
  else
    {
      /* Construct LINK as explained above. */
      link = xmalloc (3 * basedirs + strlen (linkfile) + 1);
      for (i = 0; i < basedirs; i++)
        memcpy (link + 3 * i, "../", 3);
      strcpy (link + 3 * i, linkfile);
    }

  return link;
}

/* Construct and return a "transparent proxy" URL
   reflecting changes made by --adjust-extension to the file component
   (i.e., "basename") of the original URL, but leaving the "dirname"
   of the URL (protocol://hostname... portion) untouched.

   Think: populating a squid cache via a recursive wget scrape, where
   changing URLs to work locally with "file://..." is NOT desirable.

   Example:

   if
                     p = "//foo.com/bar.cgi?xyz"
   and
      link->local_name = "docroot/foo.com/bar.cgi?xyz.css"
   then

      new_construct_func(p, link);
   will return
      "//foo.com/bar.cgi?xyz.css"

   Essentially, we do s/$(basename orig_url)/$(basename link->local_name)/
*/
static char *
convert_basename (const char *p, const struct urlpos *link)
{
  int len = link->size;
  char *url = NULL;
  char *org_basename = NULL, *local_basename = NULL;
  char *result = NULL;

  if (*p == '"' || *p == '\'')
    {
      len -= 2;
      p++;
    }

  url = xstrndup (p, len);

  org_basename = strrchr (url, '/');
  if (org_basename)
    org_basename++;
  else
    org_basename = url;

  local_basename = strrchr (link->local_name, '/');
  if (local_basename)
    local_basename++;
  else
    local_basename = url;

  /*
   * If the basenames differ, graft the adjusted basename (local_basename)
   * onto the original URL.
   */
  if (strcmp (org_basename, local_basename) == 0)
    result = url;
  else
    {
      result = uri_merge (url, local_basename);
      xfree (url);
    }

  return result;
}

/* Used by write_backup_file to remember which files have been
   written. */
static struct hash_table *converted_files;

static void write_backup_file(const char *file, downloaded_file_t downloaded_file_return)
{
  size_t filename_len = strlen(file);
  char *filename_plus_orig_suffix;
  if (downloaded_file_return == FILE_DOWNLOADED_AND_HTML_EXTENSION_ADDED)
  {
    filename_plus_orig_suffix = alloca(filename_len + 1);
    strcpy(filename_plus_orig_suffix, file);
    strcpy((filename_plus_orig_suffix + filename_len) - 4, "orig");
  }
  else
  {
    filename_plus_orig_suffix = alloca(filename_len + (sizeof(".orig")));
    strcpy(filename_plus_orig_suffix, file);
    strcpy(filename_plus_orig_suffix + filename_len, ".orig");
  }
  if (!get_converted_files_wrapper())
    set_converted_files_wrapper(make_string_hash_table(0));
  if (!string_set_contains(get_converted_files_wrapper(), file))
  {
    if (rename(file, filename_plus_orig_suffix) != 0)
      logprintf(LOG_NOTQUIET, gettext("Cannot back up %s as %s: %s\n"), file, filename_plus_orig_suffix, strerror(errno));
    string_set_add(get_converted_files_wrapper(), file);
  }
}



static bool find_fragment (const char *, int, const char **, const char **);

/* Replace a string with NEW_TEXT.  Ignore quoting. */
static const char *
replace_plain (const char *p, int size, FILE *fp, const char *new_text)
{
  fputs (new_text, fp);
  p += size;
  return p;
}

/* Replace an attribute's original text with NEW_TEXT. */

static const char *
replace_attr (const char *p, int size, FILE *fp, const char *new_text)
{
  bool quote_flag = false;
  char quote_char = '\"';       /* use "..." for quoting, unless the
                                   original value is quoted, in which
                                   case reuse its quoting char. */
  const char *frag_beg, *frag_end;

  /* Structure of our string is:
       "...old-contents..."
       <---    size    --->  (with quotes)
     OR:
       ...old-contents...
       <---    size   -->    (no quotes)   */

  if (*p == '\"' || *p == '\'')
    {
      quote_char = *p;
      quote_flag = true;
      ++p;
      size -= 2;                /* disregard opening and closing quote */
    }
  putc (quote_char, fp);
  fputs (new_text, fp);

  /* Look for fragment identifier, if any. */
  if (find_fragment (p, size, &frag_beg, &frag_end))
    fwrite (frag_beg, 1, frag_end - frag_beg, fp);
  p += size;
  if (quote_flag)
    ++p;
  putc (quote_char, fp);

  return p;
}

/* The same as REPLACE_ATTR, but used when replacing
   <meta http-equiv=refresh content="new_text"> because we need to
   append "timeout_value; URL=" before the next_text.  */

static const char *
replace_attr_refresh_hack (const char *p, int size, FILE *fp,
                           const char *new_text, int timeout)
{
  /* "0; URL=..." */
  char *new_with_timeout = (char *)alloca (numdigit (timeout)
                                           + 6 /* "; URL=" */
                                           + strlen (new_text)
                                           + 1);
  sprintf (new_with_timeout, "%d; URL=%s", timeout, new_text);

  return replace_attr (p, size, fp, new_with_timeout);
}

/* Find the first occurrence of '#' in [BEG, BEG+SIZE) that is not
   preceded by '&'.  If the character is not found, return zero.  If
   the character is found, return true and set BP and EP to point to
   the beginning and end of the region.

   This is used for finding the fragment indentifiers in URLs.  */

static bool
find_fragment (const char *beg, int size, const char **bp, const char **ep)
{
  const char *end = beg + size;
  bool saw_amp = false;
  for (; beg < end; beg++)
    {
      switch (*beg)
        {
        case '&':
          saw_amp = true;
          break;
        case '#':
          if (!saw_amp)
            {
              *bp = beg;
              *ep = end;
              return true;
            }
          /* fallthrough */
        default:
          saw_amp = false;
        }
    }
  return false;
}

/* Quote FILE for use as local reference to an HTML file.

   We quote ? as %3F to avoid passing part of the file name as the
   parameter when browsing the converted file through HTTP.  However,
   it is safe to do this only when `--adjust-extension' is turned on.
   This is because converting "index.html?foo=bar" to
   "index.html%3Ffoo=bar" would break local browsing, as the latter
   isn't even recognized as an HTML file!  However, converting
   "index.html?foo=bar.html" to "index.html%3Ffoo=bar.html" should be
   safe for both local and HTTP-served browsing.

   We always quote "#" as "%23", "%" as "%25" and ";" as "%3B"
   because those characters have special meanings in URLs.  */

static char *local_quote_string(const char *file, bool no_html_quote)
{
  const char *from;
  char *newname;
  char *to;
  char *any = strpbrk(file, "?#%;");
  if (!any)
    return (no_html_quote) ? (strdup(file)) : (html_quote_string(file));
  to = (newname = (char *) alloca((3 * strlen(file)) + 1));
  newname[0] = '\0';
  for (from = file; *from; from++)
    switch (*from)
  {
    case '%':
      *(to++) = '%';
      *(to++) = '2';
      *(to++) = '5';
      break;

    case '#':
      *(to++) = '%';
      *(to++) = '2';
      *(to++) = '3';
      break;

    case ';':
      *(to++) = '%';
      *(to++) = '3';
      *(to++) = 'B';
      break;

    case '?':
      if (get_opt_adjust_extension_wrapper())
    {
      *(to++) = '%';
      *(to++) = '3';
      *(to++) = 'F';
      break;
    }

    default:
      *(to++) = *from;

  }


  *to = '\0';
  return (no_html_quote) ? (strdup(newname)) : (html_quote_string(newname));
}



/* Book-keeping code for dl_file_url_map, dl_url_file_map,
   downloaded_html_list, and downloaded_html_set.  Other code calls
   these functions to let us know that a file has been downloaded.  */

#define ENSURE_TABLES_EXIST do {                        \
  if (!dl_file_url_map)                                 \
    dl_file_url_map = make_string_hash_table (0);       \
  if (!dl_url_file_map)                                 \
    dl_url_file_map = make_string_hash_table (0);       \
} while (0)

/* Return true if S1 and S2 are the same, except for "/index.html".
   The three cases in which it returns one are (substitute any
   substring for "foo"):

   m("foo/index.html", "foo/")  ==> 1
   m("foo/", "foo/index.html")  ==> 1
   m("foo", "foo/index.html")   ==> 1
   m("foo", "foo/"              ==> 1
   m("foo", "foo")              ==> 1  */

static bool
match_except_index (const char *s1, const char *s2)
{
  int i;
  const char *lng;

  /* Skip common substring. */
  for (i = 0; *s1 && *s2 && *s1 == *s2; s1++, s2++, i++)
    ;
  if (i == 0)
    /* Strings differ at the very beginning -- bail out.  We need to
       check this explicitly to avoid `lng - 1' reading outside the
       array.  */
    return false;

  if (!*s1 && !*s2)
    /* Both strings hit EOF -- strings are equal. */
    return true;
  else if (*s1 && *s2)
    /* Strings are randomly different, e.g. "/foo/bar" and "/foo/qux". */
    return false;
  else if (*s1)
    /* S1 is the longer one. */
    lng = s1;
  else
    /* S2 is the longer one. */
    lng = s2;

  /* foo            */            /* foo/           */
  /* foo/index.html */  /* or */  /* foo/index.html */
  /*    ^           */            /*     ^          */

  if (*lng != '/')
    /* The right-hand case. */
    --lng;

  if (*lng == '/' && *(lng + 1) == '\0')
    /* foo  */
    /* foo/ */
    return true;

  return 0 == strcmp (lng, "/index.html");
}

static int dissociate_urls_from_file_mapper(void *key, void *value, void *arg)
{
  char *mapping_url = (char *) key;
  char *mapping_file = (char *) value;
  char *file = (char *) arg;
  if (0 == strcmp(mapping_file, file))
  {
    hash_table_remove(get_dl_url_file_map_wrapper(), mapping_url);
    do
    {
      free((void *) mapping_url);
      mapping_url = 0;
    }
    while (0);
    do
    {
      free((void *) mapping_file);
      mapping_file = 0;
    }
    while (0);
  }
  return 0;
}



/* Remove all associations from various URLs to FILE from dl_url_file_map. */

static void dissociate_urls_from_file(const char *file)
{
  hash_table_for_each(get_dl_url_file_map_wrapper(), dissociate_urls_from_file_mapper, (char *) file);
}



/* Register that URL has been successfully downloaded to FILE.  This
   is used by the link conversion code to convert references to URLs
   to references to local files.  It is also being used to check if a
   URL has already been downloaded.  */

void register_download(const char *url, const char *file)
{
  char *old_file;
  char *old_url;
  do
  {
    if (!get_dl_file_url_map_wrapper())
      set_dl_file_url_map_wrapper(make_string_hash_table(0));
    if (!get_dl_url_file_map_wrapper())
      set_dl_url_file_map_wrapper(make_string_hash_table(0));
  }
  while (0);
  if (hash_table_get_pair(get_dl_file_url_map_wrapper(), file, &old_file, &old_url))
  {
    if (0 == strcmp(url, old_url))
      return;
    if (match_except_index(url, old_url) && (!hash_table_contains(get_dl_url_file_map_wrapper(), url)))
      goto url_only;
    hash_table_remove(get_dl_file_url_map_wrapper(), file);
    do
    {
      free((void *) old_file);
      old_file = 0;
    }
    while (0);
    do
    {
      free((void *) old_url);
      old_url = 0;
    }
    while (0);
    dissociate_urls_from_file(file);
  }
  hash_table_put(get_dl_file_url_map_wrapper(), xstrdup(file), xstrdup(url));
  url_only:
  if (hash_table_get_pair(get_dl_url_file_map_wrapper(), url, &old_url, &old_file))
  {
    hash_table_remove(get_dl_url_file_map_wrapper(), url);
    do
    {
      free((void *) old_url);
      old_url = 0;
    }
    while (0);
    do
    {
      free((void *) old_file);
      old_file = 0;
    }
    while (0);
  }

  hash_table_put(get_dl_url_file_map_wrapper(), xstrdup(url), xstrdup(file));
}



/* Register that FROM has been redirected to "TO".  This assumes that TO
   is successfully downloaded and already registered using
   register_download() above.  */

void register_redirection(const char *from, const char *to)
{
  char *file;
  do
  {
    if (!get_dl_file_url_map_wrapper())
      set_dl_file_url_map_wrapper(make_string_hash_table(0));
    if (!get_dl_url_file_map_wrapper())
      set_dl_url_file_map_wrapper(make_string_hash_table(0));
  }
  while (0);
  file = hash_table_get(get_dl_url_file_map_wrapper(), to);
  assert(file != 0);
  if (!hash_table_contains(get_dl_url_file_map_wrapper(), from))
    hash_table_put(get_dl_url_file_map_wrapper(), xstrdup(from), xstrdup(file));
}



/* Register that the file has been deleted. */

void register_delete_file(const char *file)
{
  char *old_url;
  char *old_file;
  do
  {
    if (!get_dl_file_url_map_wrapper())
      set_dl_file_url_map_wrapper(make_string_hash_table(0));
    if (!get_dl_url_file_map_wrapper())
      set_dl_url_file_map_wrapper(make_string_hash_table(0));
  }
  while (0);
  if (!hash_table_get_pair(get_dl_file_url_map_wrapper(), file, &old_file, &old_url))
    return;
  hash_table_remove(get_dl_file_url_map_wrapper(), file);
  do
  {
    free((void *) old_file);
    old_file = 0;
  }
  while (0);
  do
  {
    free((void *) old_url);
    old_url = 0;
  }
  while (0);
  dissociate_urls_from_file(file);
}



/* Register that FILE is an HTML file that has been downloaded. */

void register_html(const char *file)
{
  if (!get_downloaded_html_set_wrapper())
    set_downloaded_html_set_wrapper(make_string_hash_table(0));
  string_set_add(get_downloaded_html_set_wrapper(), file);
}



/* Register that FILE is a CSS file that has been downloaded. */

void register_css(const char *file)
{
  if (!get_downloaded_css_set_wrapper())
    set_downloaded_css_set_wrapper(make_string_hash_table(0));
  string_set_add(get_downloaded_css_set_wrapper(), file);
}



static void downloaded_files_free (void);

/* Cleanup the data structures associated with this file.  */

void convert_cleanup(void)
{
  if (get_dl_file_url_map_wrapper())
  {
    free_keys_and_values(get_dl_file_url_map_wrapper());
    hash_table_destroy(get_dl_file_url_map_wrapper());
    set_dl_file_url_map_wrapper(0);
  }
  if (get_dl_url_file_map_wrapper())
  {
    free_keys_and_values(get_dl_url_file_map_wrapper());
    hash_table_destroy(get_dl_url_file_map_wrapper());
    set_dl_url_file_map_wrapper(0);
  }
  if (get_downloaded_html_set_wrapper())
    string_set_free(get_downloaded_html_set_wrapper());
  downloaded_files_free();
  if (get_converted_files_wrapper())
    string_set_free(get_converted_files_wrapper());
}



/* Book-keeping code for downloaded files that enables extension
   hacks.  */

/* This table should really be merged with dl_file_url_map and
   downloaded_html_files.  This was originally a list, but I changed
   it to a hash table beause it was actually taking a lot of time to
   find things in it.  */

static struct hash_table *downloaded_files_hash;

/* We're storing "modes" of type downloaded_file_t in the hash table.
   However, our hash tables only accept pointers for keys and values.
   So when we need a pointer, we use the address of a
   downloaded_file_t variable of static storage.  */

static downloaded_file_t *
downloaded_mode_to_ptr (downloaded_file_t mode)
{
  static downloaded_file_t
    v1 = FILE_NOT_ALREADY_DOWNLOADED,
    v2 = FILE_DOWNLOADED_NORMALLY,
    v3 = FILE_DOWNLOADED_AND_HTML_EXTENSION_ADDED,
    v4 = CHECK_FOR_FILE;

  switch (mode)
    {
    case FILE_NOT_ALREADY_DOWNLOADED:
      return &v1;
    case FILE_DOWNLOADED_NORMALLY:
      return &v2;
    case FILE_DOWNLOADED_AND_HTML_EXTENSION_ADDED:
      return &v3;
    case CHECK_FOR_FILE:
      return &v4;
    }
  return NULL;
}

/* Remembers which files have been downloaded.  In the standard case,
   should be called with mode == FILE_DOWNLOADED_NORMALLY for each
   file we actually download successfully (i.e. not for ones we have
   failures on or that we skip due to -N).

   When we've downloaded a file and tacked on a ".html" extension due
   to -E, call this function with
   FILE_DOWNLOADED_AND_HTML_EXTENSION_ADDED rather than
   FILE_DOWNLOADED_NORMALLY.

   If you just want to check if a file has been previously added
   without adding it, call with mode == CHECK_FOR_FILE.  Please be
   sure to call this function with local filenames, not remote
   URLs.  */

downloaded_file_t downloaded_file(downloaded_file_t mode, const char *file)
{
  downloaded_file_t *ptr;
  if (mode == CHECK_FOR_FILE)
  {
    if (!get_downloaded_files_hash_wrapper())
      return FILE_NOT_ALREADY_DOWNLOADED;
    ptr = hash_table_get(get_downloaded_files_hash_wrapper(), file);
    if (!ptr)
      return FILE_NOT_ALREADY_DOWNLOADED;
    return *ptr;
  }
  if (!get_downloaded_files_hash_wrapper())
    set_downloaded_files_hash_wrapper(make_string_hash_table(0));
  ptr = hash_table_get(get_downloaded_files_hash_wrapper(), file);
  if (ptr)
    return *ptr;
  ptr = downloaded_mode_to_ptr(mode);
  hash_table_put(get_downloaded_files_hash_wrapper(), xstrdup(file), ptr);
  return FILE_NOT_ALREADY_DOWNLOADED;
}



static void downloaded_files_free(void)
{
  if (get_downloaded_files_hash_wrapper())
  {
    hash_table_iterator iter;
    for (hash_table_iterate(get_downloaded_files_hash_wrapper(), &iter); hash_table_iter_next(&iter);)
      do
    {
      free((void *) iter.key);
      iter.key = 0;
    }
    while (0);

    hash_table_destroy(get_downloaded_files_hash_wrapper());
    set_downloaded_files_hash_wrapper(0);
  }
}



/* The function returns the pointer to the malloc-ed quoted version of
   string s.  It will recognize and quote numeric and special graphic
   entities, as per RFC1866:

   `&' -> `&amp;'
   `<' -> `&lt;'
   `>' -> `&gt;'
   `"' -> `&quot;'
   SP  -> `&#32;'

   No other entities are recognized or replaced.  */
char *
html_quote_string (const char *s)
{
  const char *b = s;
  char *p, *res;
  int i;

  /* Pass through the string, and count the new size.  */
  for (i = 0; *s; s++, i++)
    {
      if (*s == '&')
        i += 4;                 /* `amp;' */
      else if (*s == '<' || *s == '>')
        i += 3;                 /* `lt;' and `gt;' */
      else if (*s == '\"')
        i += 5;                 /* `quot;' */
      else if (*s == ' ')
        i += 4;                 /* #32; */
    }
  res = xmalloc (i + 1);
  s = b;
  for (p = res; *s; s++)
    {
      switch (*s)
        {
        case '&':
          *p++ = '&';
          *p++ = 'a';
          *p++ = 'm';
          *p++ = 'p';
          *p++ = ';';
          break;
        case '<': case '>':
          *p++ = '&';
          *p++ = (*s == '<' ? 'l' : 'g');
          *p++ = 't';
          *p++ = ';';
          break;
        case '\"':
          *p++ = '&';
          *p++ = 'q';
          *p++ = 'u';
          *p++ = 'o';
          *p++ = 't';
          *p++ = ';';
          break;
        case ' ':
          *p++ = '&';
          *p++ = '#';
          *p++ = '3';
          *p++ = '2';
          *p++ = ';';
          break;
        default:
          *p++ = *s;
        }
    }
  *p = '\0';
  return res;
}

/*
 * vim: et ts=2 sw=2
 */
