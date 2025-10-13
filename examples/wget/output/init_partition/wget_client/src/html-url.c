/* Collect URLs from HTML source.
   Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
   2007, 2008, 2009, 2010, 2011, 2012, 2015 Free Software Foundation,
   Inc.

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
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include "exits.h"
#include "html-parse.h"
#include "url.h"
#include "utils.h"
#include "hash.h"
#include "convert.h"
#include "recur.h"
#include "html-url.h"
#include "css-url.h"
#include "c-strcase.h"

#include "wget_rpc_wrapper.h"

typedef void (*tag_handler_t) (int, struct taginfo *, struct map_context *);

#define DECLARE_TAG_HANDLER(fun)                                \
  static void fun (int, struct taginfo *, struct map_context *)

DECLARE_TAG_HANDLER (tag_find_urls);
DECLARE_TAG_HANDLER (tag_handle_base);
DECLARE_TAG_HANDLER (tag_handle_form);
DECLARE_TAG_HANDLER (tag_handle_img);
DECLARE_TAG_HANDLER (tag_handle_link);
DECLARE_TAG_HANDLER (tag_handle_meta);

enum {
  TAG_A,
  TAG_APPLET,
  TAG_AREA,
  TAG_BASE,
  TAG_BGSOUND,
  TAG_BODY,
  TAG_EMBED,
  TAG_FIG,
  TAG_FORM,
  TAG_FRAME,
  TAG_IFRAME,
  TAG_IMG,
  TAG_INPUT,
  TAG_LAYER,
  TAG_LINK,
  TAG_META,
  TAG_OBJECT,
  TAG_OVERLAY,
  TAG_SCRIPT,
  TAG_TABLE,
  TAG_TD,
  TAG_TH,
  TAG_VIDEO,
  TAG_AUDIO,
  TAG_SOURCE
};

/* The list of known tags and functions used for handling them.  Most
   tags are simply harvested for URLs. */
static struct known_tag {
  int tagid;
  const char *name;
  tag_handler_t handler;
} known_tags[] = {
  { TAG_A,       "a",           tag_find_urls },
  { TAG_APPLET,  "applet",      tag_find_urls },
  { TAG_AREA,    "area",        tag_find_urls },
  { TAG_BASE,    "base",        tag_handle_base },
  { TAG_BGSOUND, "bgsound",     tag_find_urls },
  { TAG_BODY,    "body",        tag_find_urls },
  { TAG_EMBED,   "embed",       tag_find_urls },
  { TAG_FIG,     "fig",         tag_find_urls },
  { TAG_FORM,    "form",        tag_handle_form },
  { TAG_FRAME,   "frame",       tag_find_urls },
  { TAG_IFRAME,  "iframe",      tag_find_urls },
  { TAG_IMG,     "img",         tag_handle_img },
  { TAG_INPUT,   "input",       tag_find_urls },
  { TAG_LAYER,   "layer",       tag_find_urls },
  { TAG_LINK,    "link",        tag_handle_link },
  { TAG_META,    "meta",        tag_handle_meta },
  { TAG_OBJECT,  "object",      tag_find_urls },
  { TAG_OVERLAY, "overlay",     tag_find_urls },
  { TAG_SCRIPT,  "script",      tag_find_urls },
  { TAG_TABLE,   "table",       tag_find_urls },
  { TAG_TD,      "td",          tag_find_urls },
  { TAG_TH,      "th",          tag_find_urls },
  { TAG_VIDEO,   "video",       tag_find_urls },
  { TAG_AUDIO,   "audio",       tag_find_urls },
  { TAG_SOURCE,  "source",      tag_find_urls }
};

/* tag_url_attributes documents which attributes of which tags contain
   URLs to harvest.  It is used by tag_find_urls.  */

/* Defines for the FLAGS. */

/* The link is "inline", i.e. needs to be retrieved for this document
   to be correctly rendered.  Inline links include inlined images,
   stylesheets, children frames, etc.  */
#define ATTR_INLINE     1

/* The link is expected to yield HTML contents.  It's important not to
   try to follow HTML obtained by following e.g. <img src="...">
   regardless of content-type.  Doing this causes infinite loops for
   "images" that return non-404 error pages with links to the same
   image.  */
#define ATTR_HTML       2

/* For tags handled by tag_find_urls: attributes that contain URLs to
   download. */
static struct {
  int tagid;
  const char *attr_name;
  int flags;
} tag_url_attributes[] = {
  { TAG_A,              "href",         ATTR_HTML },
  { TAG_APPLET,         "code",         ATTR_INLINE },
  { TAG_AREA,           "href",         ATTR_HTML },
  { TAG_BGSOUND,        "src",          ATTR_INLINE },
  { TAG_BODY,           "background",   ATTR_INLINE },
  { TAG_EMBED,          "href",         ATTR_HTML },
  { TAG_EMBED,          "src",          ATTR_INLINE | ATTR_HTML },
  { TAG_FIG,            "src",          ATTR_INLINE },
  { TAG_FRAME,          "src",          ATTR_INLINE | ATTR_HTML },
  { TAG_IFRAME,         "src",          ATTR_INLINE | ATTR_HTML },
  { TAG_IMG,            "href",         ATTR_INLINE },
  { TAG_IMG,            "lowsrc",       ATTR_INLINE },
  { TAG_IMG,            "src",          ATTR_INLINE },
  { TAG_INPUT,          "src",          ATTR_INLINE },
  { TAG_LAYER,          "src",          ATTR_INLINE | ATTR_HTML },
  { TAG_OBJECT,         "data",         ATTR_INLINE },
  { TAG_OVERLAY,        "src",          ATTR_INLINE | ATTR_HTML },
  { TAG_SCRIPT,         "src",          ATTR_INLINE },
  { TAG_TABLE,          "background",   ATTR_INLINE },
  { TAG_TD,             "background",   ATTR_INLINE },
  { TAG_TH,             "background",   ATTR_INLINE },
  { TAG_VIDEO,          "src",          ATTR_INLINE },
  { TAG_VIDEO,          "poster",       ATTR_INLINE },
  { TAG_AUDIO,          "src",          ATTR_INLINE },
  { TAG_AUDIO,          "poster",       ATTR_INLINE },
  { TAG_SOURCE,         "src",          ATTR_INLINE }
};

/* The lists of interesting tags and attributes are built dynamically,
   from the information above.  However, some places in the code refer
   to the attributes not mentioned here.  We add them manually.  */
static const char *additional_attributes[] = {
  "rel",                        /* used by tag_handle_link  */
  "type",                       /* used by tag_handle_link  */
  "http-equiv",                 /* used by tag_handle_meta  */
  "name",                       /* used by tag_handle_meta  */
  "content",                    /* used by tag_handle_meta  */
  "action",                     /* used by tag_handle_form  */
  "style",                      /* used by check_style_attr */
  "srcset",                     /* used by tag_handle_img */
};

static struct hash_table *interesting_tags;
static struct hash_table *interesting_attributes;

/* Will contains the (last) charset found in 'http-equiv=content-type'
   meta tags  */
static char *meta_charset;

static void init_interesting(void)
{
  size_t i;
  set_interesting_tags_wrapper(make_nocase_string_hash_table((sizeof(known_tags)) / (sizeof(known_tags[0]))));
  for (i = 0; i < ((sizeof(known_tags)) / (sizeof(known_tags[0]))); i++)
    hash_table_put(get_interesting_tags_wrapper(), known_tags[i].name, known_tags + i);

  if (get_opt_ignore_tags_wrapper())
  {
    char **ignored;
    for (ignored = get_opt_ignore_tags_wrapper(); *ignored; ignored++)
      hash_table_remove(get_interesting_tags_wrapper(), *ignored);

  }
  if (get_opt_follow_tags_wrapper())
  {
    struct hash_table *intersect = make_nocase_string_hash_table(0);
    char **followed;
    for (followed = get_opt_follow_tags_wrapper(); *followed; followed++)
    {
      struct known_tag *t = hash_table_get(get_interesting_tags_wrapper(), *followed);
      if (!t)
        continue;
      hash_table_put(intersect, *followed, t);
    }

    hash_table_destroy(get_interesting_tags_wrapper());
    set_interesting_tags_wrapper(intersect);
  }
  set_interesting_attributes_wrapper(make_nocase_string_hash_table(10));
  for (i = 0; i < ((sizeof(additional_attributes)) / (sizeof(additional_attributes[0]))); i++)
    hash_table_put(get_interesting_attributes_wrapper(), additional_attributes[i], "1");

  for (i = 0; i < ((sizeof(tag_url_attributes)) / (sizeof(tag_url_attributes[0]))); i++)
    hash_table_put(get_interesting_attributes_wrapper(), tag_url_attributes[i].attr_name, "1");

}



/* Find the value of attribute named NAME in the taginfo TAG.  If the
   attribute is not present, return NULL.  If ATTRIND is non-NULL, the
   index of the attribute in TAG will be stored there.  */

static char *
find_attr (struct taginfo *tag, const char *name, int *attrind)
{
  int i;
  for (i = 0; i < tag->nattrs; i++)
    if (!c_strcasecmp (tag->attrs[i].name, name))
      {
        if (attrind)
          *attrind = i;
        return tag->attrs[i].value;
      }
  return NULL;
}

/* used for calls to append_url */
#define ATTR_POS(tag, attrind, ctx) \
 (tag->attrs[attrind].value_raw_beginning - ctx->text)
#define ATTR_SIZE(tag, attrind) \
 (tag->attrs[attrind].value_raw_size)

/* Append LINK_URI to the urlpos structure that is being built.

   LINK_URI will be merged with the current document base.
*/

struct urlpos *append_url(const char *link_uri, int position, int size, struct map_context *ctx)
{
  int link_has_scheme = url_has_scheme(link_uri);
  struct urlpos *newel;
  const char *base = (ctx->base) ? (ctx->base) : (ctx->parent_base);
  struct url *url;
  struct iri *iri = &get_dummy_iri_wrapper();
  ;
  iri->utf8_encode = 1;
  if (!base)
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("%s: no base, merge will use \"%s\".\n", ctx->document_file, link_uri);
      }
    }
    while (0);
    if (!link_has_scheme)
    {
      logprintf(LOG_NOTQUIET, gettext("%s: Cannot resolve incomplete link %s.\n"), ctx->document_file, link_uri);
      ;
      return 0;
    }
    url = url_parse(link_uri, 0, iri, 0);
    if (!url)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("%s: link \"%s\" doesn't parse.\n", ctx->document_file, link_uri);
        }
      }
      while (0);
      ;
      return 0;
    }
  }
  else
  {
    char *complete_uri = uri_merge(base, link_uri);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("%s: merge(%s, %s) -> %s\n", quotearg_n_style(0, escape_quoting_style, ctx->document_file), quote_n(1, base), quote_n(2, link_uri), quotearg_n_style(3, escape_quoting_style, complete_uri));
      }
    }
    while (0);
    url = url_parse(complete_uri, 0, iri, 0);
    if (!url)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("%s: merged link \"%s\" doesn't parse.\n", ctx->document_file, complete_uri);
        }
      }
      while (0);
      do
      {
        free((void *) complete_uri);
        complete_uri = 0;
      }
      while (0);
      ;
      return 0;
    }
    do
    {
      free((void *) complete_uri);
      complete_uri = 0;
    }
    while (0);
  }
  ;
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("appending %s to urlpos.\n", quote(url->url));
    }
  }
  while (0);
  newel = xcalloc(1, sizeof(struct urlpos));
  newel->url = url;
  newel->pos = position;
  newel->size = size;
  if ((!link_has_scheme) && ((*link_uri) != '/'))
    newel->link_relative_p = 1;
  else
    if (link_has_scheme)
    newel->link_complete_p = 1;
  if (ctx->head == 0)
    ctx->head = newel;
  else
  {
    struct urlpos *it;
    struct urlpos *prev = 0;
    it = ctx->head;
    while (it && (position > it->pos))
    {
      prev = it;
      it = it->next;
    }

    newel->next = it;
    if (prev)
      prev->next = newel;
    else
      ctx->head = newel;
  }
  return newel;
}



static void
check_style_attr (struct taginfo *tag, struct map_context *ctx)
{
  int attrind;
  int raw_start;
  int raw_len;
  char *style = find_attr (tag, "style", &attrind);
  if (!style)
    return;

  /* raw pos and raw size include the quotes, skip them when they are
     present.  */
  raw_start = ATTR_POS (tag, attrind, ctx);
  raw_len  = ATTR_SIZE (tag, attrind);
  if( *(char *)(ctx->text + raw_start) == '\''
      || *(char *)(ctx->text + raw_start) == '"')
    {
      raw_start += 1;
      raw_len -= 2;
    }

  if(raw_len <= 0)
       return;

  get_urls_css (ctx, raw_start, raw_len);
}

/* All the tag_* functions are called from collect_tags_mapper, as
   specified by KNOWN_TAGS.  */

/* Default tag handler: collect URLs from attributes specified for
   this tag by tag_url_attributes.  */

static void
tag_find_urls (int tagid, struct taginfo *tag, struct map_context *ctx)
{
  size_t i;
  int attrind;
  int first = -1;

  for (i = 0; i < countof (tag_url_attributes); i++)
    if (tag_url_attributes[i].tagid == tagid)
      {
        /* We've found the index of tag_url_attributes where the
           attributes of our tag begin.  */
        first = i;
        break;
      }
  assert (first != -1);

  /* Loop over the "interesting" attributes of this tag.  In this
     example, it will loop over "src" and "lowsrc".

       <img src="foo.png" lowsrc="bar.png">

     This has to be done in the outer loop so that the attributes are
     processed in the same order in which they appear in the page.
     This is required when converting links.  */

  for (attrind = 0; attrind < tag->nattrs; attrind++)
    {
      /* Find whether TAG/ATTRIND is a combination that contains a
         URL. */
      char *link = tag->attrs[attrind].value;
      const size_t size = countof (tag_url_attributes);

      /* If you're cringing at the inefficiency of the nested loops,
         remember that they both iterate over a very small number of
         items.  The worst-case inner loop is for the IMG tag, which
         has three attributes.  */
      for (i = first; i < size && tag_url_attributes[i].tagid == tagid; i++)
        {
          if (0 == strcasecmp (tag->attrs[attrind].name,
                               tag_url_attributes[i].attr_name))
            {
              struct urlpos *up = append_url (link, ATTR_POS(tag,attrind,ctx),
                                              ATTR_SIZE(tag,attrind), ctx);
              if (up)
                {
                  int flags = tag_url_attributes[i].flags;
                  if (flags & ATTR_INLINE)
                    up->link_inline_p = 1;
                  if (flags & ATTR_HTML)
                    up->link_expect_html = 1;
                }
            }
        }
    }
}

/* Handle the BASE tag, for <base href=...>. */

static void
tag_handle_base (int tagid _GL_UNUSED, struct taginfo *tag, struct map_context *ctx)
{
  struct urlpos *base_urlpos;
  int attrind;
  char *newbase = find_attr (tag, "href", &attrind);
  if (!newbase)
    return;

  base_urlpos = append_url (newbase, ATTR_POS(tag,attrind,ctx),
                            ATTR_SIZE(tag,attrind), ctx);
  if (!base_urlpos)
    return;
  base_urlpos->ignore_when_downloading = 1;
  base_urlpos->link_base_p = 1;

  xfree (ctx->base);
  if (ctx->parent_base)
    ctx->base = uri_merge (ctx->parent_base, newbase);
  else
    ctx->base = xstrdup (newbase);
}

/* Mark the URL found in <form action=...> for conversion. */

static void
tag_handle_form (int tagid _GL_UNUSED, struct taginfo *tag, struct map_context *ctx)
{
  int attrind;
  char *action = find_attr (tag, "action", &attrind);

  if (action)
    {
      struct urlpos *up = append_url (action, ATTR_POS(tag,attrind,ctx),
                                      ATTR_SIZE(tag,attrind), ctx);
      if (up)
        up->ignore_when_downloading = 1;
    }
}

/* Handle the LINK tag.  It requires special handling because how its
   links will be followed in -p mode depends on the REL attribute.  */

static void
tag_handle_link (int tagid _GL_UNUSED, struct taginfo *tag, struct map_context *ctx)
{
  int attrind;
  char *href = find_attr (tag, "href", &attrind);

  /* All <link href="..."> link references are external, except those
     known not to be, such as style sheet and shortcut icon:

     <link rel="stylesheet" href="...">
     <link rel="shortcut icon" href="...">
  */
  if (href)
    {
      struct urlpos *up = append_url (href, ATTR_POS(tag,attrind,ctx),
                                      ATTR_SIZE(tag,attrind), ctx);
      if (up)
        {
          char *rel = find_attr (tag, "rel", NULL);
          if (rel)
            {
              if (0 == c_strcasecmp (rel, "stylesheet"))
                {
                  up->link_inline_p = 1;
                  up->link_expect_css = 1;
                }
              else if (0 == c_strcasecmp (rel, "shortcut icon"))
                {
                  up->link_inline_p = 1;
                }
              else
                {
                  /* The external ones usually point to HTML pages, such as
                     <link rel="next" href="...">
                     except when the type attribute says otherwise:
                     <link rel="alternate" type="application/rss+xml" href=".../?feed=rss2" />
                  */
                  char *type = find_attr (tag, "type", NULL);
                  if (!type || c_strcasecmp (type, "text/html") == 0)
                    up->link_expect_html = 1;
                }
            }
        }
    }
}

/* Handle the META tag.  This requires special handling because of the
   refresh feature and because of robot exclusion.  */

static void tag_handle_meta(int tagid, struct taginfo *tag, struct map_context *ctx)
{
  char *name = find_attr(tag, "name", 0);
  char *http_equiv = find_attr(tag, "http-equiv", 0);
  if (http_equiv && (0 == c_strcasecmp(http_equiv, "refresh")))
  {
    struct urlpos *entry;
    int attrind;
    int timeout = 0;
    char *p;
    char *refresh = find_attr(tag, "content", &attrind);
    if (!refresh)
      return;
    for (p = refresh; c_isdigit(*p); p++)
      timeout = ((10 * timeout) + (*p)) - '0';

    if ((*(p++)) != ';')
      return;
    while (c_isspace(*p))
      ++p;

    if (!((((c_toupper(*p) == 'U') && (c_toupper(*(p + 1)) == 'R')) && (c_toupper(*(p + 2)) == 'L')) && ((*(p + 3)) == '=')))
      return;
    p += 4;
    while (c_isspace(*p))
      ++p;

    entry = append_url(p, tag->attrs[attrind].value_raw_beginning - ctx->text, tag->attrs[attrind].value_raw_size, ctx);
    if (entry)
    {
      entry->link_refresh_p = 1;
      entry->refresh_timeout = timeout;
      entry->link_expect_html = 1;
    }
  }
  else
    if (http_equiv && (0 == c_strcasecmp(http_equiv, "content-type")))
  {
    char *mcharset;
    char *content = find_attr(tag, "content", 0);
    if (!content)
      return;
    mcharset = 0;
    if (!mcharset)
      return;
    do
    {
      free((void *) get_meta_charset_wrapper());
      set_meta_charset_wrapper(0);
    }
    while (0);
    set_meta_charset_wrapper(mcharset);
  }
  else
    if (name && (0 == c_strcasecmp(name, "robots")))
  {
    char *content = find_attr(tag, "content", 0);
    if (!content)
      return;
    if (!c_strcasecmp(content, "none"))
      ctx->nofollow = 1;
    else
    {
      while (*content)
      {
        char *end;
        content += strspn(content, " \f\n\r\t\v");
        end = content + strcspn(content, ", \f\n\r\t\v");
        if (!c_strncasecmp(content, "nofollow", end - content))
          ctx->nofollow = 1;
        if ((*end) == ',')
          ++end;
        else
        {
          end = strchr(end, ',');
          if (end)
            ++end;
          else
            end = content + strlen(content);
        }
        content = end;
      }

    }
  }
}



/* Handle the IMG tag.  This requires special handling for the srcset attr,
   while the traditional src/lowsrc/href attributes can be handled generically.
*/

static void
tag_handle_img (int tagid, struct taginfo *tag, struct map_context *ctx) {
  int attrind;
  char *srcset;

  /* Use the generic approach for the attributes without special syntax. */
  tag_find_urls(tagid, tag, ctx);

  srcset = find_attr (tag, "srcset", &attrind);
  if (srcset)
    {
      /* These are relative to the input text. */
      int base_ind = ATTR_POS (tag,attrind,ctx);
      int size = strlen (srcset);

      /* These are relative to srcset. */
      int offset, url_start, url_end;

      /* Make sure to line up base_ind with srcset[0], not outside quotes. */
      if (ctx->text[base_ind] == '"' || ctx->text[base_ind] == '\'')
        ++base_ind;

      offset = 0;
      while (offset < size)
        {
          bool has_descriptor = true;

          /* Skip over initial whitespace and commas. Note there is no \v
            in HTML5 whitespace. */
          url_start = offset + strspn (srcset + offset, " \f\n\r\t,");

          if (url_start == size)
            return;

          /* URL is any non-whitespace chars (including commas) - but with
             trailing commas removed. */
          url_end = url_start + strcspn (srcset + url_start, " \f\n\r\t");
          while ((url_end - 1) > url_start && srcset[url_end - 1] == ',')
            {
              has_descriptor = false;
              --url_end;
            }

          if (url_end > url_start)
            {
              char *url_text = strdupdelim (srcset + url_start,
                                            srcset + url_end);
              struct urlpos *up = append_url (url_text, base_ind + url_start,
                                              url_end - url_start, ctx);
              up->link_inline_p = 1;
              up->link_noquote_html_p = 1;
              xfree (url_text);
            }

          /* If the URL wasn't terminated by a , there may also be a descriptor
             which we just skip. */
          if (has_descriptor)
            {
              /* This is comma-terminated, except there may be one level of
                 parentheses escaping that. */
              bool in_paren = false;
              for (offset = url_end; offset < size; ++offset)
                {
                  char c = srcset[offset];
                  if (c == '(')
                    in_paren = true;
                  else if (c == ')' && in_paren)
                    in_paren = false;
                  else if (c == ',' && !in_paren)
                    break;
                }
            }
          else
            offset = url_end;
        }
    }
}

/* Dispatch the tag handler appropriate for the tag we're mapping
   over.  See known_tags[] for definition of tag handlers.  */

static void collect_tags_mapper(struct taginfo *tag, void *arg)
{
  struct map_context *ctx = (struct map_context *) arg;
  struct known_tag *t = hash_table_get(get_interesting_tags_wrapper(), tag->name);
  if (t != 0)
    t->handler(t->tagid, tag, ctx);
  check_style_attr(tag, ctx);
  if ((((tag->end_tag_p && (0 == c_strcasecmp(tag->name, "style"))) && tag->contents_begin) && tag->contents_end) && (tag->contents_begin <= tag->contents_end))
  {
    get_urls_css(ctx, tag->contents_begin - ctx->text, tag->contents_end - tag->contents_begin);
  }
}



/* Analyze HTML tags FILE and construct a list of URLs referenced from
   it.  It merges relative links in FILE with URL.  It is aware of
   <base href=...> and does the right thing.  */

struct urlpos *get_urls_html(const char *file, const char *url, bool *meta_disallow_follow, struct iri *iri)
{
  struct file_memory *fm;
  struct map_context ctx;
  int flags;
  fm = wget_read_file(file);
  if (!fm)
  {
    logprintf(LOG_NOTQUIET, "%s: %s\n", file, strerror(errno));
    return 0;
  }
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Loaded %s (size %s).\n", file, number_to_static_string(fm->length));
    }
  }
  while (0);
  ctx.text = fm->content;
  ctx.head = 0;
  ctx.base = 0;
  ctx.parent_base = (url) ? (url) : (get_opt_base_href_wrapper());
  ctx.document_file = file;
  ctx.nofollow = 0;
  if (!get_interesting_tags_wrapper())
    init_interesting();
  flags = 2;
  if (get_opt_strict_comments_wrapper())
    flags |= 1;
  map_html_tags(fm->content, fm->length, collect_tags_mapper, &ctx, flags, 0, get_interesting_attributes_wrapper());
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("no-follow in %s: %d\n", file, ctx.nofollow);
    }
  }
  while (0);
  if (meta_disallow_follow)
    *meta_disallow_follow = ctx.nofollow;
  do
  {
    free((void *) ctx.base);
    ctx.base = 0;
  }
  while (0);
  wget_read_file_free(fm);
  return ctx.head;
}



/* This doesn't really have anything to do with HTML, but it's similar
   to get_urls_html, so we put it here.  */

struct urlpos *get_urls_file(const char *file)
{
  struct file_memory *fm;
  struct urlpos *head;
  struct urlpos *tail;
  const char *text;
  const char *text_end;
  fm = wget_read_file(file);
  if (!fm)
  {
    logprintf(LOG_NOTQUIET, "%s: %s\n", file, strerror(errno));
    return 0;
  }
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Loaded %s (size %s).\n", file, number_to_static_string(fm->length));
    }
  }
  while (0);
  head = (tail = 0);
  text = fm->content;
  text_end = fm->content + fm->length;
  while (text < text_end)
  {
    int up_error_code;
    char *url_text;
    char *new_url;
    struct urlpos *entry;
    struct url *url;
    const char *line_beg = text;
    const char *line_end = memchr(text, '\n', text_end - text);
    if (!line_end)
      line_end = text_end;
    else
      ++line_end;
    text = line_end;
    while ((line_beg < line_end) && c_isspace(*line_beg))
      ++line_beg;

    while ((line_end > line_beg) && c_isspace(*(line_end - 1)))
      --line_end;

    if (line_beg == line_end)
      continue;
    url_text = strdupdelim(line_beg, line_end);
    if (get_opt_base_href_wrapper())
    {
      char *merged = uri_merge(get_opt_base_href_wrapper(), url_text);
      do
      {
        free((void *) url_text);
        url_text = 0;
      }
      while (0);
      url_text = merged;
    }
    new_url = rewrite_shorthand_url(url_text);
    if (new_url)
    {
      do
      {
        free((void *) url_text);
        url_text = 0;
      }
      while (0);
      url_text = new_url;
    }
    url = url_parse(url_text, &up_error_code, 0, 0);
    if (!url)
    {
      char *error = url_error(url_text, up_error_code);
      logprintf(LOG_NOTQUIET, gettext("%s: Invalid URL %s: %s\n"), file, url_text, error);
      do
      {
        free((void *) url_text);
        url_text = 0;
      }
      while (0);
      do
      {
        free((void *) error);
        error = 0;
      }
      while (0);
      inform_exit_status(URLERROR);
      continue;
    }
    do
    {
      free((void *) url_text);
      url_text = 0;
    }
    while (0);
    entry = xcalloc(1, sizeof(struct urlpos));
    entry->url = url;
    if (!head)
      head = entry;
    else
      tail->next = entry;
    tail = entry;
  }

  wget_read_file_free(fm);
  return head;
}



void cleanup_html_url(void)
{
  if (get_interesting_tags_wrapper())
    hash_table_destroy(get_interesting_tags_wrapper());
  if (get_interesting_attributes_wrapper())
    hash_table_destroy(get_interesting_attributes_wrapper());
}


