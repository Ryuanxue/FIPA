/* HTTP support.
   Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
   2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2014, 2015 Free
   Software Foundation, Inc.

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
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <locale.h>

#include "hash.h"
#include "http.h"
#include "hsts.h"
#include "utils.h"
#include "url.h"
#include "host.h"
#include "retr.h"
#include "connect.h"
#include "netrc.h"
#ifdef HAVE_SSL
# include "ssl.h"
#endif
#ifdef ENABLE_NTLM
# include "http-ntlm.h"
#endif
#include "cookies.h"
#include "md5.h"
#include "convert.h"
#include "spider.h"
#include "warc.h"
#include "c-strcase.h"
#include "version.h"
#ifdef HAVE_METALINK
# include "metalink.h"
# include "xstrndup.h"
#endif

#ifdef TESTING
#include "test.h"

#include "wget_rpc_wrapper.h"

#endif

#ifdef __VMS
# include "vms.h"
#endif /* def __VMS */


/* Forward decls. */
struct http_stat;
static char *create_authorization_line (const char *, const char *,
                                        const char *, const char *,
                                        const char *, bool *, uerr_t *);
static char *basic_authentication_encode (const char *, const char *);
static bool known_authentication_scheme_p (const char *, const char *);
static void ensure_extension (struct http_stat *, const char *, int *);
static void load_cookies (void);

static bool cookies_loaded_p;
static struct cookie_jar *wget_cookie_jar;

#define TEXTHTML_S "text/html"
#define TEXTXHTML_S "application/xhtml+xml"
#define TEXTCSS_S "text/css"

/* Some status code validation macros: */
#define H_10X(x)        (((x) >= 100) && ((x) < 200))
#define H_20X(x)        (((x) >= 200) && ((x) < 300))
#define H_PARTIAL(x)    ((x) == HTTP_STATUS_PARTIAL_CONTENTS)
#define H_REDIRECTED(x) ((x) == HTTP_STATUS_MOVED_PERMANENTLY          \
                         || (x) == HTTP_STATUS_MOVED_TEMPORARILY       \
                         || (x) == HTTP_STATUS_SEE_OTHER               \
                         || (x) == HTTP_STATUS_TEMPORARY_REDIRECT)

/* HTTP/1.0 status codes from RFC1945, provided for reference.  */
/* Successful 2xx.  */
#define HTTP_STATUS_OK                    200
#define HTTP_STATUS_CREATED               201
#define HTTP_STATUS_ACCEPTED              202
#define HTTP_STATUS_NO_CONTENT            204
#define HTTP_STATUS_PARTIAL_CONTENTS      206

/* Redirection 3xx.  */
#define HTTP_STATUS_MULTIPLE_CHOICES      300
#define HTTP_STATUS_MOVED_PERMANENTLY     301
#define HTTP_STATUS_MOVED_TEMPORARILY     302
#define HTTP_STATUS_SEE_OTHER             303 /* from HTTP/1.1 */
#define HTTP_STATUS_NOT_MODIFIED          304
#define HTTP_STATUS_TEMPORARY_REDIRECT    307 /* from HTTP/1.1 */

/* Client error 4xx.  */
#define HTTP_STATUS_BAD_REQUEST           400
#define HTTP_STATUS_UNAUTHORIZED          401
#define HTTP_STATUS_FORBIDDEN             403
#define HTTP_STATUS_NOT_FOUND             404
#define HTTP_STATUS_RANGE_NOT_SATISFIABLE 416

/* Server errors 5xx.  */
#define HTTP_STATUS_INTERNAL              500
#define HTTP_STATUS_NOT_IMPLEMENTED       501
#define HTTP_STATUS_BAD_GATEWAY           502
#define HTTP_STATUS_UNAVAILABLE           503
#define HTTP_STATUS_GATEWAY_TIMEOUT       504

enum rp {
  rel_none, rel_name, rel_value, rel_both
};

struct request {
  const char *method;
  char *arg;

  struct request_header {
    char *name, *value;
    enum rp release_policy;
  } *headers;
  int hcount, hcapacity;
};


/* Create a new, empty request. Set the request's method and its
   arguments.  METHOD should be a literal string (or it should outlive
   the request) because it will not be freed.  ARG will be freed by
   request_free.  */

static struct request *
request_new (const char *method, char *arg)
{
  struct request *req = xnew0 (struct request);
  req->hcapacity = 8;
  req->headers = xnew_array (struct request_header, req->hcapacity);
  req->method = method;
  req->arg = arg;
  return req;
}

/* Return the method string passed with the last call to
   request_set_method.  */

static const char *
request_method (const struct request *req)
{
  return req->method;
}

/* Free one header according to the release policy specified with
   request_set_header.  */

static void
release_header (struct request_header *hdr)
{
  switch (hdr->release_policy)
    {
    case rel_none:
      break;
    case rel_name:
      xfree (hdr->name);
      break;
    case rel_value:
      xfree (hdr->value);
      break;
    case rel_both:
      xfree (hdr->name);
      xfree (hdr->value);
      break;
    }
}

/* Set the request named NAME to VALUE.  Specifically, this means that
   a "NAME: VALUE\r\n" header line will be used in the request.  If a
   header with the same name previously existed in the request, its
   value will be replaced by this one.  A NULL value means do nothing.

   RELEASE_POLICY determines whether NAME and VALUE should be released
   (freed) with request_free.  Allowed values are:

    - rel_none     - don't free NAME or VALUE
    - rel_name     - free NAME when done
    - rel_value    - free VALUE when done
    - rel_both     - free both NAME and VALUE when done

   Setting release policy is useful when arguments come from different
   sources.  For example:

     // Don't free literal strings!
     request_set_header (req, "Pragma", "no-cache", rel_none);

     // Don't free a global variable, we'll need it later.
     request_set_header (req, "Referer", opt.referer, rel_none);

     // Value freshly allocated, free it when done.
     request_set_header (req, "Range",
                         aprintf ("bytes=%s-", number_to_static_string (hs->restval)),
                         rel_value);
   */

static void
request_set_header (struct request *req, const char *name, const char *value,
                    enum rp release_policy)
{
  struct request_header *hdr;
  int i;

  if (!value)
    {
      /* A NULL value is a no-op; if freeing the name is requested,
         free it now to avoid leaks.  */
      if (release_policy == rel_name || release_policy == rel_both)
        xfree (name);
      return;
    }

  for (i = 0; i < req->hcount; i++)
    {
      hdr = &req->headers[i];
      if (0 == c_strcasecmp (name, hdr->name))
        {
          /* Replace existing header. */
          release_header (hdr);
          hdr->name = (void *)name;
          hdr->value = (void *)value;
          hdr->release_policy = release_policy;
          return;
        }
    }

  /* Install new header. */

  if (req->hcount >= req->hcapacity)
    {
      req->hcapacity <<= 1;
      req->headers = xrealloc (req->headers, req->hcapacity * sizeof (*hdr));
    }
  hdr = &req->headers[req->hcount++];
  hdr->name = (void *)name;
  hdr->value = (void *)value;
  hdr->release_policy = release_policy;
}

/* Like request_set_header, but sets the whole header line, as
   provided by the user using the `--header' option.  For example,
   request_set_user_header (req, "Foo: bar") works just like
   request_set_header (req, "Foo", "bar").  */

static void
request_set_user_header (struct request *req, const char *header)
{
  char *name;
  const char *p = strchr (header, ':');
  if (!p)
    return;
  BOUNDED_TO_ALLOCA (header, p, name);
  ++p;
  while (c_isspace (*p))
    ++p;
  request_set_header (req, xstrdup (name), (char *) p, rel_name);
}

/* Remove the header with specified name from REQ.  Returns true if
   the header was actually removed, false otherwise.  */

static bool
request_remove_header (struct request *req, const char *name)
{
  int i;
  for (i = 0; i < req->hcount; i++)
    {
      struct request_header *hdr = &req->headers[i];
      if (0 == c_strcasecmp (name, hdr->name))
        {
          release_header (hdr);
          /* Move the remaining headers by one. */
          if (i < req->hcount - 1)
            memmove (hdr, hdr + 1, (req->hcount - i - 1) * sizeof (*hdr));
          --req->hcount;
          return true;
        }
    }
  return false;
}

#define APPEND(p, str) do {                     \
  int A_len = strlen (str);                     \
  memcpy (p, str, A_len);                       \
  p += A_len;                                   \
} while (0)

/* Construct the request and write it to FD using fd_write.
   If warc_tmp is set to a file pointer, the request string will
   also be written to that file. */

static int request_send(const struct request *req, int fd, FILE *warc_tmp)
{
  char *request_string;
  char *p;
  int i;
  int size;
  int write_error;
  size = 0;
  size += ((((strlen(req->method) + 1) + strlen(req->arg)) + 1) + 8) + 2;
  for (i = 0; i < req->hcount; i++)
  {
    struct request_header *hdr = &req->headers[i];
    size += ((strlen(hdr->name) + 2) + strlen(hdr->value)) + 2;
  }

  size += 3;
  p = (request_string = (char *) alloca(size * (sizeof(char))));
  do
  {
    int A_len = strlen(req->method);
    memcpy(p, req->method, A_len);
    p += A_len;
  }
  while (0);
  *(p++) = ' ';
  do
  {
    int A_len = strlen(req->arg);
    memcpy(p, req->arg, A_len);
    p += A_len;
  }
  while (0);
  *(p++) = ' ';
  memcpy(p, "HTTP/1.1\r\n", 10);
  p += 10;
  for (i = 0; i < req->hcount; i++)
  {
    struct request_header *hdr = &req->headers[i];
    do
    {
      int A_len = strlen(hdr->name);
      memcpy(p, hdr->name, A_len);
      p += A_len;
    }
    while (0);
    *(p++) = ':', *(p++) = ' ';
    do
    {
      int A_len = strlen(hdr->value);
      memcpy(p, hdr->value, A_len);
      p += A_len;
    }
    while (0);
    *(p++) = '\r', *(p++) = '\n';
  }

  *(p++) = '\r', *(p++) = '\n', *(p++) = '\0';
  assert((p - request_string) == size);
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("\n---request begin---\n%s---request end---\n", request_string);
    }
  }
  while (0);
  write_error = fd_write(fd, request_string, size - 1, -1);
  if (write_error < 0)
    logprintf(LOG_VERBOSE, gettext("Failed writing HTTP request: %s.\n"), fd_errstr(fd));
  else
    if (warc_tmp != 0)
  {
    int warc_tmp_written = fwrite(request_string, 1, size - 1, warc_tmp);
    if (warc_tmp_written != (size - 1))
      return -2;
  }
  return write_error;
}



/* Release the resources used by REQ.
   It is safe to call it with a vaild pointer to a NULL pointer.
   It is not safe to call it with an invalid or NULL pointer.  */

static void
request_free (struct request **req_ref)
{
  int i;
  struct request *req = *req_ref;

  if (!req)
    return;

  xfree (req->arg);
  for (i = 0; i < req->hcount; i++)
    release_header (&req->headers[i]);
  xfree (req->headers);
  xfree (req);
  *req_ref = NULL;
}

static struct hash_table *basic_authed_hosts;

/* Find out if this host has issued a Basic challenge yet; if so, give
 * it the username, password. A temporary measure until we can get
 * proper authentication in place. */

static bool maybe_send_basic_creds(const char *hostname, const char *user, const char *passwd, struct request *req)
{
  bool do_challenge = 0;
  if (get_opt_auth_without_challenge_wrapper())
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Auth-without-challenge set, sending Basic credentials.\n");
      }
    }
    while (0);
    do_challenge = 1;
  }
  else
    if (get_basic_authed_hosts_wrapper() && hash_table_contains(get_basic_authed_hosts_wrapper(), hostname))
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Found %s in basic_authed_hosts.\n", quote(hostname));
      }
    }
    while (0);
    do_challenge = 1;
  }
  else
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Host %s has not issued a general basic challenge.\n", quote(hostname));
      }
    }
    while (0);
  }
  if (do_challenge)
  {
    request_set_header(req, "Authorization", basic_authentication_encode(user, passwd), rel_value);
  }
  return do_challenge;
}



static void register_basic_auth_host(const char *hostname)
{
  if (!get_basic_authed_hosts_wrapper())
  {
    set_basic_authed_hosts_wrapper(make_nocase_string_hash_table(1));
  }
  if (!hash_table_contains(get_basic_authed_hosts_wrapper(), hostname))
  {
    hash_table_put(get_basic_authed_hosts_wrapper(), xstrdup(hostname), 0);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Inserted %s into basic_authed_hosts\n", quote(hostname));
      }
    }
    while (0);
  }
}




/* Send the contents of FILE_NAME to SOCK.  Make sure that exactly
   PROMISED_SIZE bytes are sent over the wire -- if the file is
   longer, read only that much; if the file is shorter, report an error.
   If warc_tmp is set to a file pointer, the post data will
   also be written to that file.  */

static int body_file_send(int sock, const char *file_name, wgint promised_size, FILE *warc_tmp)
{
  static char chunk[8192];
  wgint written = 0;
  int write_error;
  FILE *fp;
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("[writing BODY file %s ... ", file_name);
    }
  }
  while (0);
  fp = fopen(file_name, "rb");
  if (!fp)
    return -1;
  while ((!feof(fp)) && (written < promised_size))
  {
    int towrite;
    int length = fread(chunk, 1, sizeof(chunk), fp);
    if (length == 0)
      break;
    towrite = ((promised_size - written) <= length) ? (promised_size - written) : (length);
    write_error = fd_write(sock, chunk, towrite, -1);
    if (write_error < 0)
    {
      fclose(fp);
      return -1;
    }
    if (warc_tmp != 0)
    {
      int warc_tmp_written = fwrite(chunk, 1, towrite, warc_tmp);
      if (warc_tmp_written != towrite)
      {
        fclose(fp);
        return -2;
      }
    }
    written += towrite;
  }

  fclose(fp);
  if (written < promised_size)
  {
    errno = EINVAL;
    return -1;
  }
  assert(written == promised_size);
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("done]\n");
    }
  }
  while (0);
  return 0;
}



/* Determine whether [START, PEEKED + PEEKLEN) contains an empty line.
   If so, return the pointer to the position after the line, otherwise
   return NULL.  This is used as callback to fd_read_hunk.  The data
   between START and PEEKED has been read and cannot be "unread"; the
   data after PEEKED has only been peeked.  */

static const char *
response_head_terminator (const char *start, const char *peeked, int peeklen)
{
  const char *p, *end;

  /* If at first peek, verify whether HUNK starts with "HTTP".  If
     not, this is a HTTP/0.9 request and we must bail out without
     reading anything.  */
  if (start == peeked && 0 != memcmp (start, "HTTP", MIN (peeklen, 4)))
    return start;

  /* Look for "\n[\r]\n", and return the following position if found.
     Start two chars before the current to cover the possibility that
     part of the terminator (e.g. "\n\r") arrived in the previous
     batch.  */
  p = peeked - start < 2 ? start : peeked - 2;
  end = peeked + peeklen;

  /* Check for \n\r\n or \n\n anywhere in [p, end-2). */
  for (; p < end - 2; p++)
    if (*p == '\n')
      {
        if (p[1] == '\r' && p[2] == '\n')
          return p + 3;
        else if (p[1] == '\n')
          return p + 2;
      }
  /* p==end-2: check for \n\n directly preceding END. */
  if (p[0] == '\n' && p[1] == '\n')
    return p + 2;

  return NULL;
}

/* The maximum size of a single HTTP response we care to read.  Rather
   than being a limit of the reader implementation, this limit
   prevents Wget from slurping all available memory upon encountering
   malicious or buggy server output, thus protecting the user.  Define
   it to 0 to remove the limit.  */

#define HTTP_RESPONSE_MAX_SIZE 65536

/* Read the HTTP request head from FD and return it.  The error
   conditions are the same as with fd_read_hunk.

   To support HTTP/0.9 responses, this function tries to make sure
   that the data begins with "HTTP".  If this is not the case, no data
   is read and an empty request is returned, so that the remaining
   data can be treated as body.  */

static char *
read_http_response_head (int fd)
{
  return fd_read_hunk (fd, response_head_terminator, 512,
                       HTTP_RESPONSE_MAX_SIZE);
}

struct response {
  /* The response data. */
  const char *data;

  /* The array of pointers that indicate where each header starts.
     For example, given this HTTP response:

       HTTP/1.0 200 Ok
       Description: some
        text
       Etag: x

     The headers are located like this:

     "HTTP/1.0 200 Ok\r\nDescription: some\r\n text\r\nEtag: x\r\n\r\n"
     ^                   ^                             ^          ^
     headers[0]          headers[1]                    headers[2] headers[3]

     I.e. headers[0] points to the beginning of the request,
     headers[1] points to the end of the first header and the
     beginning of the second one, etc.  */

  const char **headers;
};

/* Create a new response object from the text of the HTTP response,
   available in HEAD.  That text is automatically split into
   constituent header lines for fast retrieval using
   resp_header_*.  */

static struct response *
resp_new (const char *head)
{
  const char *hdr;
  int count, size;

  struct response *resp = xnew0 (struct response);
  resp->data = head;

  if (*head == '\0')
    {
      /* Empty head means that we're dealing with a headerless
         (HTTP/0.9) response.  In that case, don't set HEADERS at
         all.  */
      return resp;
    }

  /* Split HEAD into header lines, so that resp_header_* functions
     don't need to do this over and over again.  */

  size = count = 0;
  hdr = head;
  while (1)
    {
      DO_REALLOC (resp->headers, size, count + 1, const char *);
      resp->headers[count++] = hdr;

      /* Break upon encountering an empty line. */
      if (!hdr[0] || (hdr[0] == '\r' && hdr[1] == '\n') || hdr[0] == '\n')
        break;

      /* Find the end of HDR, including continuations. */
      do
        {
          const char *end = strchr (hdr, '\n');
          if (end)
            hdr = end + 1;
          else
            hdr += strlen (hdr);
        }
      while (*hdr == ' ' || *hdr == '\t');
    }
  DO_REALLOC (resp->headers, size, count + 1, const char *);
  resp->headers[count] = NULL;

  return resp;
}

/* Locate the header named NAME in the request data, starting with
   position START.  This allows the code to loop through the request
   data, filtering for all requests of a given name.  Returns the
   found position, or -1 for failure.  The code that uses this
   function typically looks like this:

     for (pos = 0; (pos = resp_header_locate (...)) != -1; pos++)
       ... do something with header ...

   If you only care about one header, use resp_header_get instead of
   this function.  */

static int
resp_header_locate (const struct response *resp, const char *name, int start,
                    const char **begptr, const char **endptr)
{
  int i;
  const char **headers = resp->headers;
  int name_len;

  if (!headers || !headers[1])
    return -1;

  name_len = strlen (name);
  if (start > 0)
    i = start;
  else
    i = 1;

  for (; headers[i + 1]; i++)
    {
      const char *b = headers[i];
      const char *e = headers[i + 1];
      if (e - b > name_len
          && b[name_len] == ':'
          && 0 == c_strncasecmp (b, name, name_len))
        {
          b += name_len + 1;
          while (b < e && c_isspace (*b))
            ++b;
          while (b < e && c_isspace (e[-1]))
            --e;
          *begptr = b;
          *endptr = e;
          return i;
        }
    }
  return -1;
}

/* Find and retrieve the header named NAME in the request data.  If
   found, set *BEGPTR to its starting, and *ENDPTR to its ending
   position, and return true.  Otherwise return false.

   This function is used as a building block for resp_header_copy
   and resp_header_strdup.  */

static bool
resp_header_get (const struct response *resp, const char *name,
                 const char **begptr, const char **endptr)
{
  int pos = resp_header_locate (resp, name, 0, begptr, endptr);
  return pos != -1;
}

/* Copy the response header named NAME to buffer BUF, no longer than
   BUFSIZE (BUFSIZE includes the terminating 0).  If the header
   exists, true is returned, false otherwise.  If there should be no
   limit on the size of the header, use resp_header_strdup instead.

   If BUFSIZE is 0, no data is copied, but the boolean indication of
   whether the header is present is still returned.  */

static bool
resp_header_copy (const struct response *resp, const char *name,
                  char *buf, int bufsize)
{
  const char *b, *e;
  if (!resp_header_get (resp, name, &b, &e))
    return false;
  if (bufsize)
    {
      int len = MIN (e - b, bufsize - 1);
      memcpy (buf, b, len);
      buf[len] = '\0';
    }
  return true;
}

/* Return the value of header named NAME in RESP, allocated with
   malloc.  If such a header does not exist in RESP, return NULL.  */

static char *
resp_header_strdup (const struct response *resp, const char *name)
{
  const char *b, *e;
  if (!resp_header_get (resp, name, &b, &e))
    return NULL;
  return strdupdelim (b, e);
}

/* Parse the HTTP status line, which is of format:

   HTTP-Version SP Status-Code SP Reason-Phrase

   The function returns the status-code, or -1 if the status line
   appears malformed.  The pointer to "reason-phrase" message is
   returned in *MESSAGE.  */

static int
resp_status (const struct response *resp, char **message)
{
  int status;
  const char *p, *end;

  if (!resp->headers)
    {
      /* For a HTTP/0.9 response, assume status 200. */
      if (message)
        *message = xstrdup (_("No headers, assuming HTTP/0.9"));
      return 200;
    }

  p = resp->headers[0];
  end = resp->headers[1];

  if (!end)
    return -1;

  /* "HTTP" */
  if (end - p < 4 || 0 != strncmp (p, "HTTP", 4))
    return -1;
  p += 4;

  /* Match the HTTP version.  This is optional because Gnutella
     servers have been reported to not specify HTTP version.  */
  if (p < end && *p == '/')
    {
      ++p;
      while (p < end && c_isdigit (*p))
        ++p;
      if (p < end && *p == '.')
        ++p;
      while (p < end && c_isdigit (*p))
        ++p;
    }

  while (p < end && c_isspace (*p))
    ++p;
  if (end - p < 3 || !c_isdigit (p[0]) || !c_isdigit (p[1]) || !c_isdigit (p[2]))
    return -1;

  status = 100 * (p[0] - '0') + 10 * (p[1] - '0') + (p[2] - '0');
  p += 3;

  if (message)
    {
      while (p < end && c_isspace (*p))
        ++p;
      while (p < end && c_isspace (end[-1]))
        --end;
      *message = strdupdelim (p, end);
    }

  return status;
}

/* Release the resources used by RESP.
   It is safe to call it with a valid pointer to a NULL pointer.
   It is not safe to call it with a invalid or NULL pointer.  */

static void
resp_free (struct response **resp_ref)
{
  struct response *resp = *resp_ref;

  if (!resp)
    return;

  xfree (resp->headers);
  xfree (resp);

  *resp_ref = NULL;
}

/* Print a single line of response, the characters [b, e).  We tried
   getting away with
      logprintf (LOG_VERBOSE, "%s%.*s\n", prefix, (int) (e - b), b);
   but that failed to escape the non-printable characters and, in fact,
   caused crashes in UTF-8 locales.  */

static void
print_response_line(const char *prefix, const char *b, const char *e)
{
  char *copy;
  BOUNDED_TO_ALLOCA(b, e, copy);
  logprintf (LOG_ALWAYS, "%s%s\n", prefix,
             quotearg_style (escape_quoting_style, copy));
}

/* Print the server response, line by line, omitting the trailing CRLF
   from individual header lines, and prefixed with PREFIX.  */

static void
print_server_response (const struct response *resp, const char *prefix)
{
  int i;
  if (!resp->headers)
    return;
  for (i = 0; resp->headers[i + 1]; i++)
    {
      const char *b = resp->headers[i];
      const char *e = resp->headers[i + 1];
      /* Skip CRLF */
      if (b < e && e[-1] == '\n')
        --e;
      if (b < e && e[-1] == '\r')
        --e;
      print_response_line(prefix, b, e);
    }
}

/* Parse the `Content-Range' header and extract the information it
   contains.  Returns true if successful, false otherwise.  */
static bool
parse_content_range (const char *hdr, wgint *first_byte_ptr,
                     wgint *last_byte_ptr, wgint *entity_length_ptr)
{
  wgint num;

  /* Ancient versions of Netscape proxy server, presumably predating
     rfc2068, sent out `Content-Range' without the "bytes"
     specifier.  */
  if (0 == strncasecmp (hdr, "bytes", 5))
    {
      hdr += 5;
      /* "JavaWebServer/1.1.1" sends "bytes: x-y/z", contrary to the
         HTTP spec. */
      if (*hdr == ':')
        ++hdr;
      while (c_isspace (*hdr))
        ++hdr;
      if (!*hdr)
        return false;
    }
  if (!c_isdigit (*hdr))
    return false;
  for (num = 0; c_isdigit (*hdr); hdr++)
    num = 10 * num + (*hdr - '0');
  if (*hdr != '-' || !c_isdigit (*(hdr + 1)))
    return false;
  *first_byte_ptr = num;
  ++hdr;
  for (num = 0; c_isdigit (*hdr); hdr++)
    num = 10 * num + (*hdr - '0');
  if (*hdr != '/')
    return false;
  *last_byte_ptr = num;
  if (!(c_isdigit (*(hdr + 1)) || *(hdr + 1) == '*'))
    return false;
  if (*last_byte_ptr < *first_byte_ptr)
    return false;
  ++hdr;
  if (*hdr == '*')
    num = -1;
  else
    for (num = 0; c_isdigit (*hdr); hdr++)
      num = 10 * num + (*hdr - '0');
  *entity_length_ptr = num;
  if ((*entity_length_ptr <= *last_byte_ptr) && *entity_length_ptr != -1)
    return false;
  return true;
}

/* Read the body of the request, but don't store it anywhere and don't
   display a progress gauge.  This is useful for reading the bodies of
   administrative responses to which we will soon issue another
   request.  The response is not useful to the user, but reading it
   allows us to continue using the same connection to the server.

   If reading fails, false is returned, true otherwise.  In debug
   mode, the body is displayed for debugging purposes.  */

static bool skip_short_body(int fd, wgint contlen, bool chunked)
{
  enum 
  {
    SKIP_SIZE = 512,
    SKIP_THRESHOLD = 4096
  };
  wgint remaining_chunk_size = 0;
  char dlbuf[SKIP_SIZE + 1];
  dlbuf[SKIP_SIZE] = '\0';
  if (contlen > SKIP_THRESHOLD)
    return 0;
  while ((contlen > 0) || chunked)
  {
    int ret;
    if (chunked)
    {
      if (remaining_chunk_size == 0)
      {
        char *line = fd_read_line(fd);
        char *endl;
        if (line == 0)
          break;
        remaining_chunk_size = strtol(line, &endl, 16);
        do
        {
          free((void *) line);
          line = 0;
        }
        while (0);
        if (remaining_chunk_size == 0)
        {
          line = fd_read_line(fd);
          do
          {
            free((void *) line);
            line = 0;
          }
          while (0);
          break;
        }
      }
      contlen = (remaining_chunk_size <= SKIP_SIZE) ? (remaining_chunk_size) : (SKIP_SIZE);
    }
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Skipping %s bytes of body: [", number_to_static_string(contlen));
      }
    }
    while (0);
    ret = fd_read(fd, dlbuf, (contlen <= SKIP_SIZE) ? (contlen) : (SKIP_SIZE), -1);
    if (ret <= 0)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("] aborting (%s).\n", (ret < 0) ? (fd_errstr(fd)) : ("EOF received"));
        }
      }
      while (0);
      return 0;
    }
    contlen -= ret;
    if (chunked)
    {
      remaining_chunk_size -= ret;
      if (remaining_chunk_size == 0)
      {
        char *line = fd_read_line(fd);
        if (line == 0)
          return 0;
        else
          do
        {
          free((void *) line);
          line = 0;
        }
        while (0);
      }
    }
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("%.*s", ret, dlbuf);
      }
    }
    while (0);
  }

  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("] done.\n");
    }
  }
  while (0);
  return 1;
}



#define NOT_RFC2231 0
#define RFC2231_NOENCODING 1
#define RFC2231_ENCODING 2

/* extract_param extracts the parameter name into NAME.
   However, if the parameter name is in RFC2231 format then
   this function adjusts NAME by stripping of the trailing
   characters that are not part of the name but are present to
   indicate the presence of encoding information in the value
   or a fragment of a long parameter value
*/
static int
modify_param_name(param_token *name)
{
  const char *delim1 = memchr (name->b, '*', name->e - name->b);
  const char *delim2 = memrchr (name->b, '*', name->e - name->b);

  int result;

  if(delim1 == NULL)
    {
      result = NOT_RFC2231;
    }
  else if(delim1 == delim2)
    {
      if ((name->e - 1) == delim1)
        {
          result = RFC2231_ENCODING;
        }
      else
        {
          result = RFC2231_NOENCODING;
        }
      name->e = delim1;
    }
  else
    {
      name->e = delim1;
      result = RFC2231_ENCODING;
    }
  return result;
}

/* extract_param extract the paramater value into VALUE.
   Like modify_param_name this function modifies VALUE by
   stripping off the encoding information from the actual value
*/
static void
modify_param_value (param_token *value, int encoding_type )
{
  if (encoding_type == RFC2231_ENCODING)
    {
      const char *delim = memrchr (value->b, '\'', value->e - value->b);
      if (delim != NULL)
        {
          value->b = (delim+1);
        }
    }
}

/* Extract a parameter from the string (typically an HTTP header) at
   **SOURCE and advance SOURCE to the next parameter.  Return false
   when there are no more parameters to extract.  The name of the
   parameter is returned in NAME, and the value in VALUE.  If the
   parameter has no value, the token's value is zeroed out.

   For example, if *SOURCE points to the string "attachment;
   filename=\"foo bar\"", the first call to this function will return
   the token named "attachment" and no value, and the second call will
   return the token named "filename" and value "foo bar".  The third
   call will return false, indicating no more valid tokens.

   is_url_encoded is an out parameter. If not NULL, a boolean value will be
   stored into it, letting the caller know whether or not the extracted value is
   URL-encoded. The caller can then decode it with url_unescape(), which however
   performs decoding in-place. URL-encoding is used by RFC 2231 to support
   non-US-ASCII characters in HTTP header values.  */

bool
extract_param (const char **source, param_token *name, param_token *value,
               char separator, bool *is_url_encoded)
{
  const char *p = *source;
  int param_type;
  if (is_url_encoded)
    *is_url_encoded = false;   /* initializing the out parameter */

  while (c_isspace (*p)) ++p;
  if (!*p)
    {
      *source = p;
      return false;             /* no error; nothing more to extract */
    }

  /* Extract name. */
  name->b = p;
  while (*p && !c_isspace (*p) && *p != '=' && *p != separator) ++p;
  name->e = p;
  if (name->b == name->e)
    return false;               /* empty name: error */
  while (c_isspace (*p)) ++p;
  if (*p == separator || !*p)           /* no value */
    {
      xzero (*value);
      if (*p == separator) ++p;
      *source = p;
      return true;
    }
  if (*p != '=')
    return false;               /* error */

  /* *p is '=', extract value */
  ++p;
  while (c_isspace (*p)) ++p;
  if (*p == '"')                /* quoted */
    {
      value->b = ++p;
      while (*p && *p != '"') ++p;
      if (!*p)
        return false;
      value->e = p++;
      /* Currently at closing quote; find the end of param. */
      while (c_isspace (*p)) ++p;
      while (*p && *p != separator) ++p;
      if (*p == separator)
        ++p;
      else if (*p)
        /* garbage after closed quote, e.g. foo="bar"baz */
        return false;
    }
  else                          /* unquoted */
    {
      value->b = p;
      while (*p && *p != separator) ++p;
      value->e = p;
      while (value->e != value->b && c_isspace (value->e[-1]))
        --value->e;
      if (*p == separator) ++p;
    }
  *source = p;

  param_type = modify_param_name(name);
  if (param_type != NOT_RFC2231)
    {
      if (param_type == RFC2231_ENCODING && is_url_encoded)
        *is_url_encoded = true;
      modify_param_value(value, param_type);
    }
  return true;
}

#undef NOT_RFC2231
#undef RFC2231_NOENCODING
#undef RFC2231_ENCODING

/* Appends the string represented by VALUE to FILENAME */

static void
append_value_to_filename (char **filename, param_token const * const value,
                          bool is_url_encoded)
{
  int original_length = strlen(*filename);
  int new_length = strlen(*filename) + (value->e - value->b);
  *filename = xrealloc (*filename, new_length+1);
  memcpy (*filename + original_length, value->b, (value->e - value->b));
  (*filename)[new_length] = '\0';
  if (is_url_encoded)
    url_unescape (*filename + original_length);
}

/* Parse the contents of the `Content-Disposition' header, extracting
   the information useful to Wget.  Content-Disposition is a header
   borrowed from MIME; when used in HTTP, it typically serves for
   specifying the desired file name of the resource.  For example:

       Content-Disposition: attachment; filename="flora.jpg"

   Wget will skip the tokens it doesn't care about, such as
   "attachment" in the previous example; it will also skip other
   unrecognized params.  If the header is syntactically correct and
   contains a file name, a copy of the file name is stored in
   *filename and true is returned.  Otherwise, the function returns
   false.

   The file name is stripped of directory components and must not be
   empty.

   Historically, this function returned filename prefixed with opt.dir_prefix,
   now that logic is handled by the caller, new code should pay attention,
   changed by crq, Sep 2010.

*/
static bool
parse_content_disposition (const char *hdr, char **filename)
{
  param_token name, value;
  bool is_url_encoded = false;

  char *encodedFilename = NULL;
  char *unencodedFilename = NULL;
  for ( ; extract_param (&hdr, &name, &value, ';', &is_url_encoded);
        is_url_encoded = false)
    {
      int isFilename = BOUNDED_EQUAL_NO_CASE (name.b, name.e, "filename");
      if ( isFilename && value.b != NULL)
        {
          /* Make the file name begin at the last slash or backslash. */
          bool isEncodedFilename;
          char **outFilename;
          const char *last_slash = memrchr (value.b, '/', value.e - value.b);
          const char *last_bs = memrchr (value.b, '\\', value.e - value.b);
          if (last_slash && last_bs)
            value.b = 1 + MAX (last_slash, last_bs);
          else if (last_slash || last_bs)
            value.b = 1 + (last_slash ? last_slash : last_bs);
          if (value.b == value.e)
            continue;

          /* Check if the name is "filename*" as specified in RFC 6266.
           * Since "filename" could be broken up as "filename*N" (RFC 2231),
           * a check is needed to make sure this is not the case */
          isEncodedFilename = *name.e == '*' && !c_isdigit (*(name.e + 1));
          outFilename = isEncodedFilename ? &encodedFilename
            : &unencodedFilename;
          if (*outFilename)
            append_value_to_filename (outFilename, &value, is_url_encoded);
          else
            {
              *outFilename = strdupdelim (value.b, value.e);
              if (is_url_encoded)
                url_unescape (*outFilename);
            }
        }
    }
  if (encodedFilename)
    {
      xfree (unencodedFilename);
      *filename = encodedFilename;
    }
  else
    {
      xfree (encodedFilename);
      *filename = unencodedFilename;
    }
  if (*filename)
    return true;
  else
    return false;
}

#ifdef HAVE_HSTS
static bool parse_strict_transport_security(const char *header, time_t *max_age, bool *include_subdomains)
{
  param_token name;
  param_token value;
  const char *c_max_age = 0;
  bool is = 0;
  bool is_url_encoded = 0;
  bool success = 0;
  if (header)
  {
    for (; extract_param(&header, &name, &value, ';', &is_url_encoded); is_url_encoded = 0)
    {
      if (((name.e - name.b) == ((sizeof("max-age")) - 1)) && (!c_strncasecmp(name.b, "max-age", (sizeof("max-age")) - 1)))
      {
        do
        {
          free((void *) c_max_age);
          c_max_age = 0;
        }
        while (0);
        c_max_age = strdupdelim(value.b, value.e);
      }
      else
        if (((name.e - name.b) == ((sizeof("includeSubDomains")) - 1)) && (!c_strncasecmp(name.b, "includeSubDomains", (sizeof("includeSubDomains")) - 1)))
        is = 1;
    }

    if (c_max_age)
    {
      if (max_age)
        *max_age = (time_t) strtol(c_max_age, 0, 10);
      if (include_subdomains)
        *include_subdomains = is;
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Parsed Strict-Transport-Security max-age = %s, includeSubDomains = %s\n", c_max_age, (is) ? ("true") : ("false"));
        }
      }
      while (0);
      do
      {
        free((void *) c_max_age);
        c_max_age = 0;
      }
      while (0);
      success = 1;
    }
    else
    {
      logprintf(LOG_VERBOSE, "Could not parse String-Transport-Security header\n");
      success = 0;
    }
  }
  return success;
}


#endif

/* Persistent connections.  Currently, we cache the most recently used
   connection as persistent, provided that the HTTP server agrees to
   make it such.  The persistence data is stored in the variables
   below.  Ideally, it should be possible to cache an arbitrary fixed
   number of these connections.  */

/* Whether a persistent connection is active. */
static bool pconn_active;

static struct {
  /* The socket of the connection.  */
  int socket;

  /* Host and port of the currently active persistent connection. */
  char *host;
  int port;

  /* Whether a ssl handshake has occoured on this connection.  */
  bool ssl;

  /* Whether the connection was authorized.  This is only done by
     NTLM, which authorizes *connections* rather than individual
     requests.  (That practice is peculiar for HTTP, but it is a
     useful optimization.)  */
  bool authorized;

#ifdef ENABLE_NTLM
  /* NTLM data of the current connection.  */
  struct ntlmdata ntlm;
#endif
} pconn;

/* Mark the persistent connection as invalid and free the resources it
   uses.  This is used by the CLOSE_* macros after they forcefully
   close a registered persistent connection.  */

static void invalidate_persistent(void)
{
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Disabling further reuse of socket %d.\n", get_pconn_socket_wrapper());
    }
  }
  while (0);
  set_pconn_active_wrapper(0);
  fd_close(get_pconn_socket_wrapper());
  do
  {
    free((void *) get_pconn_host_wrapper());
    set_pconn_host_wrapper(0);
  }
  while (0);
  {
    new_pconn temp_pconn = get_pconn_wrapper();
    memset(&temp_pconn, '\0', sizeof(pconn));
    set_pconn_wrapper(temp_pconn);
  }
}



/* Register FD, which should be a TCP/IP connection to HOST:PORT, as
   persistent.  This will enable someone to use the same connection
   later.  In the context of HTTP, this must be called only AFTER the
   response has been received and the server has promised that the
   connection will remain alive.

   If a previous connection was persistent, it is closed. */

static void register_persistent(const char *host, int port, int fd, bool ssl)
{
  if (get_pconn_active_wrapper())
  {
    if (get_pconn_socket_wrapper() == fd)
    {
      return;
    }
    else
    {
      invalidate_persistent();
    }
  }
  set_pconn_active_wrapper(1);
  set_pconn_socket_wrapper(fd);
  set_pconn_host_wrapper(xstrdup(host));
  set_pconn_port_wrapper(port);
  set_pconn_ssl_wrapper(ssl);
  set_pconn_authorized_wrapper(0);
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Registered socket %d for persistent reuse.\n", fd);
    }
  }
  while (0);
}



/* Return true if a persistent connection is available for connecting
   to HOST:PORT.  */

static bool persistent_available_p(const char *host, int port, bool ssl, bool *host_lookup_failed)
{
  if (!get_pconn_active_wrapper())
    return 0;
  if (ssl != get_pconn_ssl_wrapper())
    return 0;
  if (port != get_pconn_port_wrapper())
    return 0;
  if (0 != strcasecmp(host, get_pconn_host_wrapper()))
  {
    bool found;
    ip_address ip;
    struct address_list *al;
    if (ssl)
      return 0;
    if (!socket_ip_address(get_pconn_socket_wrapper(), &ip, ENDPOINT_PEER))
    {
      invalidate_persistent();
      return 0;
    }
    al = lookup_host(host, 0);
    if (!al)
    {
      *host_lookup_failed = 1;
      return 0;
    }
    found = address_list_contains(al, &ip);
    address_list_release(al);
    if (!found)
      return 0;
  }
  if (!test_socket_open(get_pconn_socket_wrapper()))
  {
    invalidate_persistent();
    return 0;
  }
  return 1;
}



/* The idea behind these two CLOSE macros is to distinguish between
   two cases: one when the job we've been doing is finished, and we
   want to close the connection and leave, and two when something is
   seriously wrong and we're closing the connection as part of
   cleanup.

   In case of keep_alive, CLOSE_FINISH should leave the connection
   open, while CLOSE_INVALIDATE should still close it.

   Note that the semantics of the flag `keep_alive' is "this
   connection *will* be reused (the server has promised not to close
   the connection once we're done)", while the semantics of
   `pc_active_p && (fd) == pc_last_fd' is "we're *now* using an
   active, registered connection".  */

#define CLOSE_FINISH(fd) do {                   \
  if (!keep_alive)                              \
    {                                           \
      if (pconn_active && (fd) == pconn.socket) \
        invalidate_persistent ();               \
      else                                      \
          fd_close (fd);                        \
      fd = -1;                                  \
    }                                           \
} while (0)

#define CLOSE_INVALIDATE(fd) do {               \
  if (pconn_active && (fd) == pconn.socket)     \
    invalidate_persistent ();                   \
  else                                          \
    fd_close (fd);                              \
  fd = -1;                                      \
} while (0)

struct http_stat
{
  wgint len;                    /* received length */
  wgint contlen;                /* expected length */
  wgint restval;                /* the restart value */
  int res;                      /* the result of last read */
  char *rderrmsg;               /* error message from read error */
  char *newloc;                 /* new location (redirection) */
  char *remote_time;            /* remote time-stamp string */
  char *error;                  /* textual HTTP error */
  int statcode;                 /* status code */
  char *message;                /* status message */
  wgint rd_size;                /* amount of data read from socket */
  double dltime;                /* time it took to download the data */
  const char *referer;          /* value of the referer header. */
  char *local_file;             /* local file name. */
  bool existence_checked;       /* true if we already checked for a file's
                                   existence after having begun to download
                                   (needed in gethttp for when connection is
                                   interrupted/restarted. */
  bool timestamp_checked;       /* true if pre-download time-stamping checks
                                 * have already been performed */
  char *orig_file_name;         /* name of file to compare for time-stamping
                                 * (might be != local_file if -K is set) */
  wgint orig_file_size;         /* size of file to compare for time-stamping */
  time_t orig_file_tstamp;      /* time-stamp of file to compare for
                                 * time-stamping */
#ifdef HAVE_METALINK
  metalink_t *metalink;
#endif
};

static void
free_hstat (struct http_stat *hs)
{
  xfree (hs->newloc);
  xfree (hs->remote_time);
  xfree (hs->error);
  xfree (hs->rderrmsg);
  xfree (hs->local_file);
  xfree (hs->orig_file_name);
  xfree (hs->message);
#ifdef HAVE_METALINK
  metalink_delete (hs->metalink);
  hs->metalink = NULL;
#endif
}

static void
get_file_flags (const char *filename, int *dt)
{
  logprintf (LOG_VERBOSE, _("\
File %s already there; not retrieving.\n\n"), quote (filename));
  /* If the file is there, we suppose it's retrieved OK.  */
  *dt |= RETROKF;

  /* #### Bogusness alert.  */
  /* If its suffix is "html" or "htm" or similar, assume text/html.  */
  if (has_html_suffix_p (filename))
    *dt |= TEXTHTML;
}

/* Download the response body from the socket and writes it to
   an output file.  The headers have already been read from the
   socket.  If WARC is enabled, the response body will also be
   written to a WARC response record.

   hs, contlen, contrange, chunked_transfer_encoding and url are
   parameters from the gethttp method.  fp is a pointer to the
   output file.

   url, warc_timestamp_str, warc_request_uuid, warc_ip, type
   and statcode will be saved in the headers of the WARC record.
   The head parameter contains the HTTP headers of the response.

   If fp is NULL and WARC is enabled, the response body will be
   written only to the WARC file.  If WARC is disabled and fp
   is a file pointer, the data will be written to the file.
   If fp is a file pointer and WARC is enabled, the body will
   be written to both destinations.

   Returns the error code.   */
static int read_response_body(struct http_stat *hs, int sock, FILE *fp, wgint contlen, wgint contrange, bool chunked_transfer_encoding, char *url, char *warc_timestamp_str, char *warc_request_uuid, ip_address *warc_ip, char *type, int statcode, char *head)
{
  int warc_payload_offset = 0;
  FILE *warc_tmp = 0;
  int warcerr = 0;
  int flags = 0;
  if (get_opt_warc_filename_wrapper() != 0)
  {
    warc_tmp = warc_tempfile();
    if (warc_tmp == 0)
      warcerr = WARC_TMP_FOPENERR;
    if (warcerr == 0)
    {
      int head_len = strlen(head);
      int warc_tmp_written = fwrite(head, 1, head_len, warc_tmp);
      if (warc_tmp_written != head_len)
        warcerr = WARC_TMP_FWRITEERR;
      warc_payload_offset = head_len;
    }
    if (warcerr != 0)
    {
      if (warc_tmp != 0)
        fclose(warc_tmp);
      return warcerr;
    }
  }
  if (fp != 0)
  {
    if (get_opt_save_headers_wrapper() && (hs->restval == 0))
      fwrite(head, 1, strlen(head), fp);
  }
  if (contlen != (-1))
    flags |= rb_read_exactly;
  if (((fp != 0) && (hs->restval > 0)) && (contrange == 0))
    flags |= rb_skip_startpos;
  if (chunked_transfer_encoding)
    flags |= rb_chunked_transfer_encoding;
  hs->len = hs->restval;
  hs->rd_size = 0;
  hs->res = fd_read_body(hs->local_file, sock, fp, (contlen != (-1)) ? (contlen) : (0), hs->restval, &hs->rd_size, &hs->len, &hs->dltime, flags, warc_tmp);
  if (hs->res >= 0)
  {
    if (warc_tmp != 0)
    {
      bool r = warc_write_response_record(url, warc_timestamp_str, warc_request_uuid, warc_ip, warc_tmp, warc_payload_offset, type, statcode, hs->newloc);
      if (!r)
        return WARC_ERR;
    }
    return RETRFINISHED;
  }
  if (warc_tmp != 0)
    fclose(warc_tmp);
  if (hs->res == (-2))
  {
    return FWRITEERR;
  }
  else
    if (hs->res == (-3))
  {
    return WARC_TMP_FWRITEERR;
  }
  else
  {
    hs->rderrmsg = xstrdup(fd_errstr(sock));
    return RETRFINISHED;
  }
}



#define BEGINS_WITH(line, string_constant)                               \
  (!c_strncasecmp (line, string_constant, sizeof (string_constant) - 1)    \
   && (c_isspace (line[sizeof (string_constant) - 1])                      \
       || !line[sizeof (string_constant) - 1]))

#ifdef __VMS
#define SET_USER_AGENT(req) do {                                         \
  if (!opt.useragent)                                                    \
    request_set_header (req, "User-Agent",                               \
                        aprintf ("Wget/%s (VMS %s %s)",                  \
                        version_string, vms_arch(), vms_vers()),         \
                        rel_value);                                      \
  else if (*opt.useragent)                                               \
    request_set_header (req, "User-Agent", opt.useragent, rel_none);     \
} while (0)
#else /* def __VMS */
#define SET_USER_AGENT(req) do {                                         \
  if (!opt.useragent)                                                    \
    request_set_header (req, "User-Agent",                               \
                        aprintf ("Wget/%s (%s)",                         \
                        version_string, OS_TYPE),                        \
                        rel_value);                                      \
  else if (*opt.useragent)                                               \
    request_set_header (req, "User-Agent", opt.useragent, rel_none);     \
} while (0)
#endif /* def __VMS [else] */

/*
   Convert time_t to one of valid HTTP date formats
   ie. rfc1123-date.

   HTTP-date    = rfc1123-date | rfc850-date | asctime-date
   rfc1123-date = wkday "," SP date1 SP time SP "GMT"
   rfc850-date  = weekday "," SP date2 SP time SP "GMT"
   asctime-date = wkday SP date3 SP time SP 4DIGIT
   date1        = 2DIGIT SP month SP 4DIGIT
                  ; day month year (e.g., 02 Jun 1982)
   date2        = 2DIGIT "-" month "-" 2DIGIT
                  ; day-month-year (e.g., 02-Jun-82)
   date3        = month SP ( 2DIGIT | ( SP 1DIGIT ))
                  ; month day (e.g., Jun  2)
   time         = 2DIGIT ":" 2DIGIT ":" 2DIGIT
                  ; 00:00:00 - 23:59:59
   wkday        = "Mon" | "Tue" | "Wed"
                | "Thu" | "Fri" | "Sat" | "Sun"
   weekday      = "Monday" | "Tuesday" | "Wednesday"
                | "Thursday" | "Friday" | "Saturday" | "Sunday"
   month        = "Jan" | "Feb" | "Mar" | "Apr"
                | "May" | "Jun" | "Jul" | "Aug"
                | "Sep" | "Oct" | "Nov" | "Dec"

   source: RFC2616  */
static uerr_t
time_to_rfc1123 (time_t time, char *buf, size_t bufsize)
{
  static const char *wkday[] = { "Sun", "Mon", "Tue", "Wed",
                                 "Thu", "Fri", "Sat" };
  static const char *month[] = { "Jan", "Feb", "Mar", "Apr",
                                 "May", "Jun", "Jul", "Aug",
                                 "Sep", "Oct", "Nov", "Dec" };
  /* rfc1123 example: Thu, 01 Jan 1998 22:12:57 GMT  */
  static const char *time_format = "%s, %02d %s %04d %02d:%02d:%02d GMT";

  struct tm *gtm = gmtime (&time);
  if (!gtm)
    {
      logprintf (LOG_NOTQUIET,
                 _("gmtime failed. This is probably a bug.\n"));
      return TIMECONV_ERR;
    }

  snprintf (buf, bufsize, time_format, wkday[gtm->tm_wday],
            gtm->tm_mday, month[gtm->tm_mon],
            gtm->tm_year + 1900, gtm->tm_hour,
            gtm->tm_min, gtm->tm_sec);

  return RETROK;
}

static struct request *initialize_request(struct url *u, struct http_stat *hs, int *dt, struct url *proxy, bool inhibit_keep_alive, bool *basic_auth_finished, wgint *body_data_size, char **user, char **passwd, uerr_t *ret)
{
  bool head_only = !(!((*dt) & HEAD_ONLY));
  struct request *req;
  {
    char *meth_arg;
    const char *meth = "GET";
    if (head_only)
      meth = "HEAD";
    else
      if (get_opt_method_wrapper())
      meth = get_opt_method_wrapper();
    if (proxy && (u->scheme != SCHEME_HTTPS))
      meth_arg = xstrdup(u->url);
    else
      meth_arg = url_full_path(u);
    req = request_new(meth, meth_arg);
  }
  request_set_header(req, "Referer", (char *) hs->referer, rel_none);
  if ((*dt) & SEND_NOCACHE)
  {
    request_set_header(req, "Cache-Control", "no-cache, must-revalidate", rel_none);
    request_set_header(req, "Pragma", "no-cache", rel_none);
  }
  if ((*dt) & IF_MODIFIED_SINCE)
  {
    char strtime[32];
    uerr_t err = time_to_rfc1123(hs->orig_file_tstamp, strtime, (sizeof(strtime)) / (sizeof(strtime[0])));
    if (err != RETROK)
    {
      logputs(LOG_VERBOSE, gettext("Cannot convert timestamp to http format. Falling back to time 0 as last modification time.\n"));
      strcpy(strtime, "Thu, 01 Jan 1970 00:00:00 GMT");
    }
    request_set_header(req, "If-Modified-Since", xstrdup(strtime), rel_value);
  }
  if (hs->restval)
    request_set_header(req, "Range", aprintf("bytes=%s-", number_to_static_string(hs->restval)), rel_value);
  do
  {
    if (!get_opt_useragent_wrapper())
      request_set_header(req, "User-Agent", aprintf("Wget/%s (%s)", version_string, "linux-gnu"), rel_value);
    else
      if (*get_opt_useragent_wrapper())
      request_set_header(req, "User-Agent", get_opt_useragent_wrapper(), rel_none);
  }
  while (0);
  request_set_header(req, "Accept", "*/*", rel_none);
  request_set_header(req, "Accept-Encoding", "identity", rel_none);
  *user = u->user;
  *passwd = u->passwd;
  search_netrc(u->host, (const char **) user, (const char **) passwd, 0);
  *user = (*user) ? (*user) : ((get_opt_http_user_wrapper()) ? (get_opt_http_user_wrapper()) : (get_opt_user_wrapper()));
  *passwd = (*passwd) ? (*passwd) : ((get_opt_http_passwd_wrapper()) ? (get_opt_http_passwd_wrapper()) : (get_opt_passwd_wrapper()));
  if ((user && (*passwd)) && ((!u->user) || get_opt_auth_without_challenge_wrapper()))
  {
    *basic_auth_finished = maybe_send_basic_creds(u->host, *user, *passwd, req);
  }
  {
    static const char *hfmt[][2] = {{"%s", "[%s]"}, {"%s:%d", "[%s]:%d"}};
    int add_port = u->port != scheme_default_port(u->scheme);
    int add_squares = strchr(u->host, ':') != 0;
    request_set_header(req, "Host", aprintf(hfmt[add_port][add_squares], u->host, u->port), rel_value);
  }
  if (inhibit_keep_alive)
    request_set_header(req, "Connection", "Close", rel_none);
  else
  {
    request_set_header(req, "Connection", "Keep-Alive", rel_none);
    if (proxy)
      request_set_header(req, "Proxy-Connection", "Keep-Alive", rel_none);
  }
  if (get_opt_method_wrapper())
  {
    if (get_opt_body_data_wrapper() || get_opt_body_file_wrapper())
    {
      request_set_header(req, "Content-Type", "application/x-www-form-urlencoded", rel_none);
      if (get_opt_body_data_wrapper())
        *body_data_size = strlen(get_opt_body_data_wrapper());
      else
      {
        *body_data_size = file_size(get_opt_body_file_wrapper());
        if ((*body_data_size) == (-1))
        {
          logprintf(LOG_NOTQUIET, gettext("BODY data file %s missing: %s\n"), quote(get_opt_body_file_wrapper()), strerror(errno));
          request_free(&req);
          *ret = FILEBADFILE;
          return 0;
        }
      }
      request_set_header(req, "Content-Length", xstrdup(number_to_static_string(*body_data_size)), rel_value);
    }
    else
      if (((c_strcasecmp(get_opt_method_wrapper(), "post") == 0) || (c_strcasecmp(get_opt_method_wrapper(), "put") == 0)) || (c_strcasecmp(get_opt_method_wrapper(), "patch") == 0))
      request_set_header(req, "Content-Length", "0", rel_none);
  }
  return req;
}



static void initialize_proxy_configuration(struct url *u, struct request *req, struct url *proxy, char **proxyauth)
{
  char *proxy_user;
  char *proxy_passwd;
  if (get_opt_proxy_user_wrapper() && get_opt_proxy_passwd_wrapper())
  {
    proxy_user = get_opt_proxy_user_wrapper();
    proxy_passwd = get_opt_proxy_passwd_wrapper();
  }
  else
  {
    proxy_user = proxy->user;
    proxy_passwd = proxy->passwd;
  }
  if (proxy_user && proxy_passwd)
    *proxyauth = basic_authentication_encode(proxy_user, proxy_passwd);
  if (u->scheme != SCHEME_HTTPS)
    request_set_header(req, "Proxy-Authorization", *proxyauth, rel_value);
}



static uerr_t establish_connection(struct url *u, struct url **conn_ref, struct http_stat *hs, struct url *proxy, char **proxyauth, struct request **req_ref, bool *using_ssl, bool inhibit_keep_alive, int *sock_ref)
{
  bool host_lookup_failed = 0;
  int sock = *sock_ref;
  struct request *req = *req_ref;
  struct url *conn = *conn_ref;
  struct response *resp;
  int write_error;
  int statcode;
  if (!inhibit_keep_alive)
  {
    struct url *relevant = conn;
    if (u->scheme == SCHEME_HTTPS)
      relevant = u;
    if (persistent_available_p(relevant->host, relevant->port, relevant->scheme == SCHEME_HTTPS, &host_lookup_failed))
    {
      int family = socket_family(get_pconn_socket_wrapper(), ENDPOINT_PEER);
      sock = get_pconn_socket_wrapper();
      *using_ssl = get_pconn_ssl_wrapper();
      if (family == AF_INET6)
        logprintf(LOG_VERBOSE, gettext("Reusing existing connection to [%s]:%d.\n"), quotearg_style(escape_quoting_style, get_pconn_host_wrapper()), get_pconn_port_wrapper());
      else
        logprintf(LOG_VERBOSE, gettext("Reusing existing connection to %s:%d.\n"), quotearg_style(escape_quoting_style, get_pconn_host_wrapper()), get_pconn_port_wrapper());
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Reusing fd %d.\n", sock);
        }
      }
      while (0);
      if (get_pconn_authorized_wrapper())
        request_remove_header(req, "Authorization");
    }
    else
      if (host_lookup_failed)
    {
      logprintf(LOG_NOTQUIET, gettext("%s: unable to resolve host address %s\n"), get_exec_name_wrapper(), quote(relevant->host));
      return HOSTERR;
    }
    else
      if (sock != (-1))
    {
      sock = -1;
    }
  }
  if (sock < 0)
  {
    sock = connect_to_host(conn->host, conn->port);
    if (sock == E_HOST)
      return HOSTERR;
    else
      if (sock < 0)
      return (retryable_socket_connect_error(errno)) ? (CONERROR) : (CONIMPOSSIBLE);
    if (proxy && (u->scheme == SCHEME_HTTPS))
    {
      char *head;
      char *message;
      struct request *connreq = request_new("CONNECT", aprintf("%s:%d", u->host, u->port));
      do
      {
        if (!get_opt_useragent_wrapper())
          request_set_header(connreq, "User-Agent", aprintf("Wget/%s (%s)", version_string, "linux-gnu"), rel_value);
        else
          if (*get_opt_useragent_wrapper())
          request_set_header(connreq, "User-Agent", get_opt_useragent_wrapper(), rel_none);
      }
      while (0);
      if (proxyauth)
      {
        request_set_header(connreq, "Proxy-Authorization", *proxyauth, rel_value);
        *proxyauth = 0;
      }
      request_set_header(connreq, "Host", aprintf("%s:%d", u->host, u->port), rel_value);
      write_error = request_send(connreq, sock, 0);
      request_free(&connreq);
      if (write_error < 0)
      {
        do
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
        while (0);
        return WRITEFAILED;
      }
      head = read_http_response_head(sock);
      if (!head)
      {
        logprintf(LOG_VERBOSE, gettext("Failed reading proxy response: %s\n"), fd_errstr(sock));
        do
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
        while (0);
        return HERR;
      }
      message = 0;
      if (!(*head))
      {
        do
        {
          free((void *) head);
          head = 0;
        }
        while (0);
        goto failed_tunnel;
      }
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("proxy responded with: [%s]\n", head);
        }
      }
      while (0);
      resp = resp_new(head);
      statcode = resp_status(resp, &message);
      if (statcode < 0)
      {
        char *tms = datetime_str(time(0));
        logprintf(LOG_VERBOSE, "%d\n", statcode);
        logprintf(LOG_NOTQUIET, gettext("%s ERROR %d: %s.\n"), tms, statcode, quotearg_style(escape_quoting_style, gettext("Malformed status line")));
        do
        {
          free((void *) head);
          head = 0;
        }
        while (0);
        return HERR;
      }
      do
      {
        free((void *) hs->message);
        hs->message = 0;
      }
      while (0);
      hs->message = xstrdup(message);
      resp_free(&resp);
      do
      {
        free((void *) head);
        head = 0;
      }
      while (0);
      if (statcode != 200)
      {
        failed_tunnel:
        logprintf(LOG_NOTQUIET, gettext("Proxy tunneling failed: %s"), (message) ? (quotearg_style(escape_quoting_style, message)) : ("?"));

        do
        {
          free((void *) message);
          message = 0;
        }
        while (0);
        return CONSSLERR;
      }
      do
      {
        free((void *) message);
        message = 0;
      }
      while (0);
      conn = u;
    }
    if (conn->scheme == SCHEME_HTTPS)
    {
      if (!ssl_connect_wget(sock, u->host, 0))
      {
        do
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
        while (0);
        return CONSSLERR;
      }
      else
        if (!ssl_check_certificate(sock, u->host))
      {
        do
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
        while (0);
        return VERIFCERTERR;
      }
      *using_ssl = 1;
    }
  }
  *conn_ref = conn;
  *req_ref = req;
  *sock_ref = sock;
  return RETROK;
}



static uerr_t set_file_timestamp(struct http_stat *hs)
{
  size_t filename_len = strlen(hs->local_file);
  char *filename_plus_orig_suffix = alloca(filename_len + (sizeof(".orig")));
  bool local_dot_orig_file_exists = 0;
  char *local_filename = 0;
  struct stat st;
  if (get_opt_backup_converted_wrapper())
  {
    memcpy(filename_plus_orig_suffix, hs->local_file, filename_len);
    memcpy(filename_plus_orig_suffix + filename_len, ".orig", sizeof(".orig"));
    if (stat(filename_plus_orig_suffix, &st) == 0)
    {
      local_dot_orig_file_exists = 1;
      local_filename = filename_plus_orig_suffix;
    }
  }
  if (!local_dot_orig_file_exists)
    if (stat(hs->local_file, &st) == 0)
    local_filename = hs->local_file;
  if (local_filename != 0)
  {
    hs->orig_file_name = xstrdup(local_filename);
    hs->orig_file_size = st.st_size;
    hs->orig_file_tstamp = st.st_mtime;
    hs->timestamp_checked = 1;
  }
  return RETROK;
}



static uerr_t check_file_output(struct url *u, struct http_stat *hs, struct response *resp, char *hdrval, size_t hdrsize)
{
  if (!hs->local_file)
  {
    char *local_file = 0;
    if (((!get_opt_content_disposition_wrapper()) || (!resp_header_copy(resp, "Content-Disposition", hdrval, hdrsize))) || (!parse_content_disposition(hdrval, &local_file)))
    {
      hs->local_file = url_file_name(u, 0);
    }
    else
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Parsed filename from Content-Disposition: %s\n", local_file);
        }
      }
      while (0);
      hs->local_file = url_file_name(u, local_file);
    }
    do
    {
      free((void *) local_file);
      local_file = 0;
    }
    while (0);
  }
  if ((!hs->existence_checked) && file_exists_p(hs->local_file))
  {
    if (get_opt_noclobber_wrapper() && (!get_opt_output_document_wrapper()))
    {
      return RETRUNNEEDED;
    }
    else
      if (!(((((get_opt_noclobber_wrapper() || get_opt_always_rest_wrapper()) || get_opt_timestamping_wrapper()) || get_opt_dirstruct_wrapper()) || get_opt_output_document_wrapper()) || (get_opt_backups_wrapper() > 0)))
    {
      char *unique = unique_name(hs->local_file, 1);
      if (unique != hs->local_file)
        do
      {
        free((void *) hs->local_file);
        hs->local_file = 0;
      }
      while (0);
      hs->local_file = unique;
    }
  }
  hs->existence_checked = 1;
  if (get_opt_timestamping_wrapper() && (!hs->timestamp_checked))
  {
    uerr_t timestamp_err = set_file_timestamp(hs);
    if (timestamp_err != RETROK)
      return timestamp_err;
  }
  return RETROK;
}



static uerr_t check_auth(struct url *u, char *user, char *passwd, struct response *resp, struct request *req, bool *ntlm_seen_ref, bool *retry, bool *basic_auth_finished_ref, bool *auth_finished_ref)
{
  uerr_t auth_err = RETROK;
  bool basic_auth_finished = *basic_auth_finished_ref;
  bool auth_finished = *auth_finished_ref;
  bool ntlm_seen = *ntlm_seen_ref;
  *retry = 0;
  if ((!auth_finished) && (user && passwd))
  {
    int wapos;
    char *buf;
    const char *www_authenticate = 0;
    const char *wabeg;
    const char *waend;
    const char *digest = 0;
    const char *basic = 0;
    const char *ntlm = 0;
    for (wapos = 0; (!ntlm) && ((wapos = resp_header_locate(resp, "WWW-Authenticate", wapos, &wabeg, &waend)) != (-1)); ++wapos)
    {
      param_token name;
      param_token value;
      do
      {
        const char *BTA_beg = wabeg;
        int BTA_len = waend - BTA_beg;
        char **BTA_dest = &buf;
        *BTA_dest = alloca(BTA_len + 1);
        memcpy(*BTA_dest, BTA_beg, BTA_len);
        (*BTA_dest)[BTA_len] = '\0';
      }
      while (0);
      www_authenticate = buf;
      for (; !ntlm;)
      {
        while (c_isspace(*www_authenticate))
          www_authenticate++;

        name.e = (name.b = www_authenticate);
        while ((*name.e) && (!c_isspace(*name.e)))
          name.e++;

        if (name.b == name.e)
          break;
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Auth scheme found '%.*s'\n", (int) (name.e - name.b), name.b);
          }
        }
        while (0);
        if (known_authentication_scheme_p(name.b, name.e))
        {
          if ((!c_strncasecmp(name.b, "NTLM", (sizeof("NTLM")) - 1)) && (c_isspace(name.b[(sizeof("NTLM")) - 1]) || (!name.b[(sizeof("NTLM")) - 1])))
          {
            ntlm = name.b;
            break;
          }
          else
            if ((!digest) && ((!c_strncasecmp(name.b, "Digest", (sizeof("Digest")) - 1)) && (c_isspace(name.b[(sizeof("Digest")) - 1]) || (!name.b[(sizeof("Digest")) - 1]))))
            digest = name.b;
          else
            if ((!basic) && ((!c_strncasecmp(name.b, "Basic", (sizeof("Basic")) - 1)) && (c_isspace(name.b[(sizeof("Basic")) - 1]) || (!name.b[(sizeof("Basic")) - 1]))))
            basic = name.b;
        }
        www_authenticate = name.e;
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Auth param list '%s'\n", www_authenticate);
          }
        }
        while (0);
        while ((extract_param(&www_authenticate, &name, &value, ',', 0) && name.b) && value.b)
        {
          do
          {
            if (__builtin_expect(get_opt_debug_wrapper(), 0))
            {
              debug_logprintf("Auth param %.*s=%.*s\n", (int) (name.e - name.b), name.b, (int) (value.e - value.b), value.b);
            }
          }
          while (0);
        }

      }

    }

    if (((!basic) && (!digest)) && (!ntlm))
    {
      logputs(LOG_NOTQUIET, gettext("Unknown authentication scheme.\n"));
    }
    else
      if ((!basic_auth_finished) || (!basic))
    {
      char *pth = url_full_path(u);
      const char *value;
      uerr_t *auth_stat;
      auth_stat = xmalloc(sizeof(uerr_t));
      *auth_stat = RETROK;
      if (ntlm)
        www_authenticate = ntlm;
      else
        if (digest)
        www_authenticate = digest;
      else
        www_authenticate = basic;
      logprintf(LOG_NOTQUIET, gettext("Authentication selected: %s\n"), www_authenticate);
      value = create_authorization_line(www_authenticate, user, passwd, request_method(req), pth, &auth_finished, auth_stat);
      auth_err = *auth_stat;
      if (auth_err == RETROK)
      {
        request_set_header(req, "Authorization", value, rel_value);
        if ((!c_strncasecmp(www_authenticate, "NTLM", (sizeof("NTLM")) - 1)) && (c_isspace(www_authenticate[(sizeof("NTLM")) - 1]) || (!www_authenticate[(sizeof("NTLM")) - 1])))
          ntlm_seen = 1;
        else
          if ((!u->user) && ((!c_strncasecmp(www_authenticate, "Basic", (sizeof("Basic")) - 1)) && (c_isspace(www_authenticate[(sizeof("Basic")) - 1]) || (!www_authenticate[(sizeof("Basic")) - 1]))))
        {
          register_basic_auth_host(u->host);
        }
        do
        {
          free((void *) pth);
          pth = 0;
        }
        while (0);
        do
        {
          free((void *) auth_stat);
          auth_stat = 0;
        }
        while (0);
        *retry = 1;
        goto cleanup;
      }
      else
      {
      }
    }
    else
    {
    }
  }
  cleanup:
  *ntlm_seen_ref = ntlm_seen;

  *basic_auth_finished_ref = basic_auth_finished;
  *auth_finished_ref = auth_finished;
  return auth_err;
}



static uerr_t open_output_stream(struct http_stat *hs, int count, FILE **fp)
{
  if (!get_output_stream_wrapper())
  {
    mkalldirs(hs->local_file);
    if (get_opt_backups_wrapper())
      rotate_backups(hs->local_file);
    if (hs->restval)
    {
      *fp = fopen(hs->local_file, "ab");
    }
    else
      if ((((((get_opt_noclobber_wrapper() || get_opt_always_rest_wrapper()) || get_opt_timestamping_wrapper()) || get_opt_dirstruct_wrapper()) || get_opt_output_document_wrapper()) || (get_opt_backups_wrapper() > 0)) || (count > 0))
    {
      if (get_opt_unlink_wrapper() && file_exists_p(hs->local_file))
      {
        if (unlink(hs->local_file) < 0)
        {
          logprintf(LOG_NOTQUIET, "%s: %s\n", hs->local_file, strerror(errno));
          return UNLINKERR;
        }
      }
      *fp = fopen(hs->local_file, "wb");
    }
    else
    {
      *fp = fopen_excl(hs->local_file, 1);
      if ((!(*fp)) && (errno == EEXIST))
      {
        logprintf(LOG_NOTQUIET, gettext("%s has sprung into existence.\n"), hs->local_file);
        return FOPEN_EXCL_ERR;
      }
    }
    if (!(*fp))
    {
      logprintf(LOG_NOTQUIET, "%s: %s\n", hs->local_file, strerror(errno));
      return FOPENERR;
    }
  }
  else
    *fp = get_output_stream_wrapper();
  logprintf(LOG_VERBOSE, gettext("Saving to: %s\n"), (((*hs->local_file) == '-') && (!(*(hs->local_file + 1)))) ? (quote("STDOUT")) : (quote(hs->local_file)));
  return RETROK;
}



/* Set proper type flags based on type string.  */
static void
set_content_type (int *dt, const char *type)
{
  /* If content-type is not given, assume text/html.  This is because
     of the multitude of broken CGI's that "forget" to generate the
     content-type.  */
  if (!type ||
      0 == strncasecmp (type, TEXTHTML_S, strlen (TEXTHTML_S)) ||
      0 == strncasecmp (type, TEXTXHTML_S, strlen (TEXTXHTML_S)))
    *dt |= TEXTHTML;
  else
    *dt &= ~TEXTHTML;

  if (type &&
      0 == strncasecmp (type, TEXTCSS_S, strlen (TEXTCSS_S)))
    *dt |= TEXTCSS;
  else
    *dt &= ~TEXTCSS;
}

#ifdef HAVE_METALINK
/* Will return proper metalink_t structure if enough data was found in
   http response resp. Otherwise returns NULL.
   Two exit points: one for success and one for failure.  */
static metalink_t *
metalink_from_http (const struct response *resp, const struct http_stat *hs,
                    const struct url *u)
{
  metalink_t *metalink = NULL;
  metalink_file_t *mfile = xnew0 (metalink_file_t);
  const char *val_beg, *val_end;
  int res_count = 0, hash_count = 0, sig_count = 0, i;

  DEBUGP (("Checking for Metalink in HTTP response\n"));

  /* Initialize metalink file for our simple use case.  */
  if (hs->local_file)
    mfile->name = xstrdup (hs->local_file);
  else
    mfile->name = url_file_name (u, NULL);

  /* Begin with 1-element array (for 0-termination). */
  mfile->checksums = xnew0 (metalink_checksum_t *);
  mfile->resources = xnew0 (metalink_resource_t *);

  /* Find all Link headers.  */
  for (i = 0;
       (i = resp_header_locate (resp, "Link", i, &val_beg, &val_end)) != -1;
       i++)
    {
      char *rel = NULL, *reltype = NULL;
      char *urlstr = NULL;
      const char *url_beg, *url_end, *attrs_beg;
      size_t url_len;

      /* Sample Metalink Link headers:

           Link: <http://www2.example.com/dir1/dir2/dir3/dir4/dir5/example.ext>;
           rel=duplicate; pri=1; pref; geo=gb; depth=4

           Link: <http://example.com/example.ext.asc>; rel=describedby;
           type="application/pgp-signature"
       */

      /* Find beginning of URL.  */
      url_beg = val_beg;
      while (url_beg < val_end - 1 && c_isspace (*url_beg))
        url_beg++;

      /* Find end of URL.  */
      /* The convention here is that end ptr points to one element after
         end of string. In this case, it should be pointing to the '>', which
         is one element after end of actual URL. Therefore, it should never point
         to val_end, which is one element after entire header value string.  */
      url_end = url_beg + 1;
      while (url_end < val_end - 1 && *url_end != '>')
        url_end++;

      if (url_beg >= val_end || url_end >= val_end ||
          *url_beg != '<' || *url_end != '>')
        {
          DEBUGP (("This is not a valid Link header. Ignoring.\n"));
          continue;
        }

      /* Skip <.  */
      url_beg++;
      url_len = url_end - url_beg;

      /* URL found. Now handle the attributes.  */
      attrs_beg = url_end + 1;

      /* First we need to find out what type of link it is. Currently, we
         support rel=duplicate and rel=describedby.  */
      if (!find_key_value (attrs_beg, val_end, "rel", &rel))
        {
          DEBUGP (("No rel value in Link header, skipping.\n"));
          continue;
        }

      urlstr = xstrndup (url_beg, url_len);
      DEBUGP (("URL=%s\n", urlstr));
      DEBUGP (("rel=%s\n", rel));

      /* Handle signatures.
         Libmetalink only supports one signature per file. Therefore we stop
         as soon as we successfully get first supported signature.  */
      if (sig_count == 0 &&
          !strcmp (rel, "describedby") &&
          find_key_value (attrs_beg, val_end, "type", &reltype) &&
          !strcmp (reltype, "application/pgp-signature")
          )
        {
          /* Download the signature to a temporary file.  */
          FILE *_output_stream = output_stream;
          bool _output_stream_regular = output_stream_regular;

          output_stream = tmpfile ();
          if (output_stream)
            {
              struct iri *iri = iri_new ();
              struct url *url;
              int url_err;

              set_uri_encoding (iri, opt.locale, true);
              url = url_parse (urlstr, &url_err, iri, false);

              if (!url)
                {
                  char *error = url_error (urlstr, url_err);
                  logprintf (LOG_NOTQUIET, _("When downloading signature:\n"
                                             "%s: %s.\n"), urlstr, error);
                  xfree (error);
                  iri_free (iri);
                }
              else
                {
                  /* Avoid recursive Metalink from HTTP headers.  */
                  bool _metalink_http = opt.metalink_over_http;
                  uerr_t retr_err;

                  opt.metalink_over_http = false;
                  retr_err = retrieve_url (url, urlstr, NULL, NULL,
                                           NULL, NULL, false, iri, false);
                  opt.metalink_over_http = _metalink_http;

                  url_free (url);
                  iri_free (iri);

                  if (retr_err == RETROK)
                    {
                      /* Signature is in the temporary file. Read it into
                         metalink resource structure.  */
                      metalink_signature_t msig;
                      size_t siglen;

                      fseek (output_stream, 0, SEEK_END);
                      siglen = ftell (output_stream);
                      fseek (output_stream, 0, SEEK_SET);

                      DEBUGP (("siglen=%lu\n", siglen));

                      msig.signature = xmalloc (siglen + 1);
                      if (fread (msig.signature, siglen, 1, output_stream) != 1)
                        {
                          logputs (LOG_NOTQUIET,
                                   _("Unable to read signature content from "
                                     "temporary file. Skipping.\n"));
                          xfree (msig.signature);
                        }
                      else
                        {
                          msig.signature[siglen] = '\0'; /* Just in case.  */
                          msig.mediatype = xstrdup ("application/pgp-signature");

                          DEBUGP (("Signature (%s):\n%s\n",
                                   msig.mediatype, msig.signature));

                          mfile->signature = xnew (metalink_signature_t);
                          *mfile->signature = msig;

                          sig_count++;
                        }
                    }
                }
              fclose (output_stream);
            }
          else
            {
              logputs (LOG_NOTQUIET, _("Could not create temporary file. "
                                       "Skipping signature download.\n"));
            }
          output_stream_regular = _output_stream_regular;
          output_stream = _output_stream;
        } /* Iterate over signatures.  */

        /* Handle Metalink resources.  */
      else if (!strcmp (rel, "duplicate"))
        {
          metalink_resource_t mres = {0};
          char *pristr;

          /*
             Valid ranges for the "pri" attribute are from
             1 to 999999.  Mirror servers with a lower value of the "pri"
             attribute have a higher priority, while mirrors with an undefined
             "pri" attribute are considered to have a value of 999999, which is
             the lowest priority.

             rfc6249 section 3.1
           */
          mres.priority = DEFAULT_PRI;
          if (find_key_value (url_end, val_end, "pri", &pristr))
            {
              long pri;
              char *end_pristr;
              /* Do not care for errno since 0 is error in this case.  */
              pri = strtol (pristr, &end_pristr, 10);
              if (end_pristr != pristr + strlen (pristr) ||
                  !VALID_PRI_RANGE (pri))
                {
                  /* This is against the specification, so let's inform the user.  */
                  logprintf (LOG_NOTQUIET,
                             _("Invalid pri value. Assuming %d.\n"),
                             DEFAULT_PRI);
                }
              else
                mres.priority = pri;
              xfree (pristr);
            }

          switch (url_scheme (urlstr))
            {
            case SCHEME_HTTP:
              mres.type = xstrdup ("http");
              break;
#ifdef HAVE_SSL
            case SCHEME_HTTPS:
              mres.type = xstrdup ("https");
              break;
            case SCHEME_FTPS:
              mres.type = xstrdup ("ftps");
              break;
#endif
            case SCHEME_FTP:
              mres.type = xstrdup ("ftp");
              break;
            default:
              DEBUGP (("Unsupported url scheme in %s. Skipping resource.\n", urlstr));
            }

          if (mres.type)
            {
              DEBUGP (("TYPE=%s\n", mres.type));

              /* At this point we have validated the new resource.  */

              find_key_value (url_end, val_end, "geo", &mres.location);

              mres.url = urlstr;
              urlstr = NULL;

              mres.preference = 0;
              if (has_key (url_end, val_end, "pref"))
                {
                  DEBUGP (("This resource has preference\n"));
                  mres.preference = 1;
                }

              /* 1 slot from new resource, 1 slot for null-termination.  */
              mfile->resources = xrealloc (mfile->resources,
                                           sizeof (metalink_resource_t *) * (res_count + 2));
              mfile->resources[res_count] = xnew0 (metalink_resource_t);
              *mfile->resources[res_count] = mres;
              res_count++;
            }
        } /* Handle resource link (rel=duplicate).  */
      else
        DEBUGP (("This link header was not used for Metalink\n"));

      xfree (urlstr);
      xfree (reltype);
      xfree (rel);
    } /* Iterate over link headers.  */

  /* Null-terminate resources array.  */
  mfile->resources[res_count] = 0;

  if (res_count == 0)
    {
      DEBUGP (("No valid metalink references found.\n"));
      goto fail;
    }

  /* Find all Digest headers.  */
  for (i = 0;
       (i = resp_header_locate (resp, "Digest", i, &val_beg, &val_end)) != -1;
       i++)
    {
      const char *dig_pos;
      char *dig_type, *dig_hash;

      /* Each Digest header can include multiple hashes. Example:
           Digest: SHA=thvDyvhfIqlvFe+A9MYgxAfm1q5=,unixsum=30637
           Digest: md5=HUXZLQLMuI/KZ5KDcJPcOA==
       */
      for (dig_pos = val_beg;
           (dig_pos = find_key_values (dig_pos, val_end, &dig_type, &dig_hash));
           dig_pos++)
        {
          /* The hash here is assumed to be base64. We need the hash in hex.
             Therefore we convert: base64 -> binary -> hex.  */
          const size_t dig_hash_str_len = strlen (dig_hash);
          char *bin_hash = alloca (dig_hash_str_len * 3 / 4 + 1);
          size_t hash_bin_len;

          hash_bin_len = base64_decode (dig_hash, bin_hash);

          /* One slot for me, one for zero-termination.  */
          mfile->checksums =
                  xrealloc (mfile->checksums,
                            sizeof (metalink_checksum_t *) * (hash_count + 2));
          mfile->checksums[hash_count] = xnew (metalink_checksum_t);
          mfile->checksums[hash_count]->type = dig_type;

          mfile->checksums[hash_count]->hash = xmalloc (hash_bin_len * 2 + 1);
          wg_hex_to_string (mfile->checksums[hash_count]->hash, bin_hash, hash_bin_len);

          xfree (dig_hash);

          hash_count++;
        }
    }

  /* Zero-terminate checksums array.  */
  mfile->checksums[hash_count] = 0;

  /*
    If Instance Digests are not provided by the Metalink servers, the
    Link header fields pertaining to this specification MUST be ignored.

    rfc6249 section 6
   */
  if (hash_count == 0)
    {
      logputs (LOG_VERBOSE,
               _("Could not find acceptable digest for Metalink resources.\n"
                 "Ignoring them.\n"));
      goto fail;
    }

  /* Metalink data is OK. Now we just need to sort the resources based
     on their priorities, preference, and perhaps location.  */
  stable_sort (mfile->resources, res_count, sizeof (metalink_resource_t *), metalink_res_cmp);

  /* Restore sensible preference values (in case someone cares to look).  */
  for (i = 0; i < res_count; ++i)
    mfile->resources[i]->preference = 1000000 - mfile->resources[i]->priority;

  metalink = xnew0 (metalink_t);
  metalink->files = xmalloc (sizeof (metalink_file_t *) * 2);
  metalink->files[0] = mfile;
  metalink->files[1] = 0;
  metalink->origin = xstrdup (u->url);
  metalink->version = METALINK_VERSION_4;
  /* Leave other fields set to 0.  */

  return metalink;

fail:
  /* Free all allocated memory.  */
  if (metalink)
    metalink_delete (metalink);
  else
    metalink_file_delete (mfile);
  return NULL;
}
#endif /* HAVE_METALINK */

/* Retrieve a document through HTTP protocol.  It recognizes status
   code, and correctly handles redirections.  It closes the network
   socket.  If it receives an error from the functions below it, it
   will print it if there is enough information to do so (almost
   always), returning the error to the caller (i.e. http_loop).

   Various HTTP parameters are stored to hs.

   If PROXY is non-NULL, the connection will be made to the proxy
   server, and u->url will be requested.  */
static uerr_t gethttp(struct url *u, struct http_stat *hs, int *dt, struct url *proxy, struct iri *iri, int count)
{
  struct request *req = 0;
  char *type = 0;
  char *user;
  char *passwd;
  char *proxyauth;
  int statcode;
  int write_error;
  wgint contlen;
  wgint contrange;
  struct url *conn;
  FILE *fp;
  int err;
  uerr_t retval;
  extern hsts_store_t hsts_store;
  const char *hsts_params;
  time_t max_age;
  bool include_subdomains;
  int sock = -1;
  bool auth_finished = 0;
  bool basic_auth_finished = 0;
  bool ntlm_seen = 0;
  bool using_ssl = 0;
  bool head_only = !(!((*dt) & HEAD_ONLY));
  bool cond_get = !(!((*dt) & IF_MODIFIED_SINCE));
  char *head = 0;
  struct response *resp = 0;
  char hdrval[512];
  char *message = 0;
  bool warc_enabled = get_opt_warc_filename_wrapper() != 0;
  FILE *warc_tmp = 0;
  char warc_timestamp_str[21];
  char warc_request_uuid[48];
  ip_address *warc_ip = 0;
  off_t warc_payload_offset = -1;
  bool keep_alive;
  bool chunked_transfer_encoding = 0;
  bool inhibit_keep_alive = (!get_opt_http_keep_alive_wrapper()) || get_opt_ignore_length_wrapper();
  wgint body_data_size = 0;
  if (u->scheme == SCHEME_HTTPS)
  {
    if (!ssl_init())
    {
      scheme_disable(SCHEME_HTTPS);
      logprintf(LOG_NOTQUIET, gettext("Disabling SSL due to encountered errors.\n"));
      retval = SSLINITFAILED;
      goto cleanup;
    }
  }
  hs->len = 0;
  hs->contlen = -1;
  hs->res = -1;
  hs->rderrmsg = 0;
  hs->newloc = 0;
  do
  {
    free((void *) hs->remote_time);
    hs->remote_time = 0;
  }
  while (0);
  hs->error = 0;
  hs->message = 0;
  conn = u;
  {
    uerr_t ret;
    req = initialize_request(u, hs, dt, proxy, inhibit_keep_alive, &basic_auth_finished, &body_data_size, &user, &passwd, &ret);
    if (req == 0)
    {
      retval = ret;
      goto cleanup;
    }
  }
  retry_with_auth:
  if (get_opt_cookies_wrapper())
    request_set_header(req, "Cookie", cookie_header(get_wget_cookie_jar_wrapper(), u->host, u->port, u->path, u->scheme == SCHEME_HTTPS), rel_value);

  if (get_opt_user_headers_wrapper())
  {
    int i;
    for (i = 0; get_opt_user_headers_wrapper()[i]; i++)
      request_set_user_header(req, get_opt_user_headers_wrapper()[i]);

  }
  proxyauth = 0;
  if (proxy)
  {
    conn = proxy;
    initialize_proxy_configuration(u, req, proxy, &proxyauth);
  }
  keep_alive = 1;
  if (inhibit_keep_alive)
    keep_alive = 0;
  {
    uerr_t conn_err = establish_connection(u, &conn, hs, proxy, &proxyauth, &req, &using_ssl, inhibit_keep_alive, &sock);
    if (conn_err != RETROK)
    {
      retval = conn_err;
      goto cleanup;
    }
  }
  if (warc_enabled)
  {
    warc_tmp = warc_tempfile();
    if (warc_tmp == 0)
    {
      do
      {
        if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
          invalidate_persistent();
        else
          fd_close(sock);
        sock = -1;
      }
      while (0);
      retval = WARC_TMP_FOPENERR;
      goto cleanup;
    }
    if (!proxy)
    {
      warc_ip = (ip_address *) alloca(sizeof(ip_address));
      socket_ip_address(sock, warc_ip, ENDPOINT_PEER);
    }
  }
  write_error = request_send(req, sock, warc_tmp);
  if (write_error >= 0)
  {
    if (get_opt_body_data_wrapper())
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("[BODY data: %s]\n", get_opt_body_data_wrapper());
        }
      }
      while (0);
      write_error = fd_write(sock, get_opt_body_data_wrapper(), body_data_size, -1);
      if ((write_error >= 0) && (warc_tmp != 0))
      {
        int warc_tmp_written;
        warc_payload_offset = ftello(warc_tmp);
        warc_tmp_written = fwrite(get_opt_body_data_wrapper(), 1, body_data_size, warc_tmp);
        if (warc_tmp_written != body_data_size)
          write_error = -2;
      }
    }
    else
      if (get_opt_body_file_wrapper() && (body_data_size != 0))
    {
      if (warc_tmp != 0)
        warc_payload_offset = ftello(warc_tmp);
      write_error = body_file_send(sock, get_opt_body_file_wrapper(), body_data_size, warc_tmp);
    }
  }
  if (write_error < 0)
  {
    do
    {
      if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
        invalidate_persistent();
      else
        fd_close(sock);
      sock = -1;
    }
    while (0);
    if (warc_tmp != 0)
      fclose(warc_tmp);
    if (write_error == (-2))
      retval = WARC_TMP_FWRITEERR;
    else
      retval = WRITEFAILED;
    goto cleanup;
  }
  logprintf(LOG_VERBOSE, gettext("%s request sent, awaiting response... "), (proxy) ? ("Proxy") : ("HTTP"));
  contlen = -1;
  contrange = 0;
  *dt &= ~RETROKF;
  if (warc_enabled)
  {
    bool warc_result;
    warc_timestamp(warc_timestamp_str, sizeof(warc_timestamp_str));
    warc_uuid_str(warc_request_uuid);
    warc_result = warc_write_request_record(u->url, warc_timestamp_str, warc_request_uuid, warc_ip, warc_tmp, warc_payload_offset);
    if (!warc_result)
    {
      do
      {
        if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
          invalidate_persistent();
        else
          fd_close(sock);
        sock = -1;
      }
      while (0);
      retval = WARC_ERR;
      goto cleanup;
    }
  }
  {
    bool _repeat;
    do
    {
      head = read_http_response_head(sock);
      if (!head)
      {
        if (errno == 0)
        {
          logputs(LOG_NOTQUIET, gettext("No data received.\n"));
          do
          {
            if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
              invalidate_persistent();
            else
              fd_close(sock);
            sock = -1;
          }
          while (0);
          retval = HEOF;
        }
        else
        {
          logprintf(LOG_NOTQUIET, gettext("Read error (%s) in headers.\n"), fd_errstr(sock));
          do
          {
            if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
              invalidate_persistent();
            else
              fd_close(sock);
            sock = -1;
          }
          while (0);
          retval = HERR;
        }
        goto cleanup;
      }
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("\n---response begin---\n%s---response end---\n", head);
        }
      }
      while (0);
      resp = resp_new(head);
      do
      {
        free((void *) message);
        message = 0;
      }
      while (0);
      statcode = resp_status(resp, &message);
      if (statcode < 0)
      {
        char *tms = datetime_str(time(0));
        logprintf(LOG_VERBOSE, "%d\n", statcode);
        logprintf(LOG_NOTQUIET, gettext("%s ERROR %d: %s.\n"), tms, statcode, quotearg_style(escape_quoting_style, gettext("Malformed status line")));
        do
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
        while (0);
        retval = HERR;
        goto cleanup;
      }
      if ((statcode >= 100) && (statcode < 200))
      {
        do
        {
          free((void *) head);
          head = 0;
        }
        while (0);
        resp_free(&resp);
        _repeat = 1;
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Ignoring response\n");
          }
        }
        while (0);
      }
      else
      {
        _repeat = 0;
      }
    }
    while (_repeat);
  }
  do
  {
    free((void *) hs->message);
    hs->message = 0;
  }
  while (0);
  hs->message = xstrdup(message);
  if (!get_opt_server_response_wrapper())
    logprintf(LOG_VERBOSE, "%2d %s\n", statcode, (message) ? (quotearg_style(escape_quoting_style, message)) : (""));
  else
  {
    logprintf(LOG_VERBOSE, "\n");
    print_server_response(resp, "  ");
  }
  if ((!get_opt_ignore_length_wrapper()) && resp_header_copy(resp, "Content-Length", hdrval, sizeof(hdrval)))
  {
    wgint parsed;
    errno = 0;
    parsed = strtol(hdrval, 0, 10);
    if ((parsed == ((wgint) ((!(!(((wgint) 0) < ((wgint) (-1))))) ? ((wgint) (-1)) : ((((((wgint) 1) << (((sizeof(wgint)) * 8) - 2)) - 1) * 2) + 1)))) && (errno == ERANGE))
    {
      contlen = -1;
    }
    else
      if (parsed < 0)
    {
      contlen = -1;
    }
    else
      contlen = parsed;
  }
  if (!inhibit_keep_alive)
  {
    if (resp_header_copy(resp, "Connection", hdrval, sizeof(hdrval)))
    {
      if (0 == c_strcasecmp(hdrval, "Close"))
        keep_alive = 0;
    }
  }
  chunked_transfer_encoding = 0;
  if (resp_header_copy(resp, "Transfer-Encoding", hdrval, sizeof(hdrval)) && (0 == c_strcasecmp(hdrval, "chunked")))
    chunked_transfer_encoding = 1;
  if (get_opt_cookies_wrapper())
  {
    int scpos;
    const char *scbeg;
    const char *scend;
    assert(get_wget_cookie_jar_wrapper() != 0);
    for (scpos = 0; (scpos = resp_header_locate(resp, "Set-Cookie", scpos, &scbeg, &scend)) != (-1); ++scpos)
    {
      char *set_cookie;
      do
      {
        const char *BTA_beg = scbeg;
        int BTA_len = scend - BTA_beg;
        char **BTA_dest = &set_cookie;
        *BTA_dest = alloca(BTA_len + 1);
        memcpy(*BTA_dest, BTA_beg, BTA_len);
        (*BTA_dest)[BTA_len] = '\0';
      }
      while (0);
      cookie_handle_set_cookie(get_wget_cookie_jar_wrapper(), u->host, u->port, u->path, set_cookie);
    }

  }
  if (keep_alive)
    register_persistent(conn->host, conn->port, sock, using_ssl);
  if (statcode == 401)
  {
    uerr_t auth_err = RETROK;
    bool retry;
    if (warc_enabled)
    {
      int _err;
      type = resp_header_strdup(resp, "Content-Type");
      _err = read_response_body(hs, sock, 0, contlen, 0, chunked_transfer_encoding, u->url, warc_timestamp_str, warc_request_uuid, warc_ip, type, statcode, head);
      do
      {
        free((void *) type);
        type = 0;
      }
      while (0);
      if ((_err != RETRFINISHED) || (hs->res < 0))
      {
        do
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
        while (0);
        retval = _err;
        goto cleanup;
      }
      else
        do
      {
        if (!keep_alive)
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
      }
      while (0);
    }
    else
    {
      if ((keep_alive && (!head_only)) && skip_short_body(sock, contlen, chunked_transfer_encoding))
        do
      {
        if (!keep_alive)
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
      }
      while (0);
      else
        do
      {
        if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
          invalidate_persistent();
        else
          fd_close(sock);
        sock = -1;
      }
      while (0);
    }
    set_pconn_authorized_wrapper(0);
    {
      auth_err = check_auth(u, user, passwd, resp, req, &ntlm_seen, &retry, &basic_auth_finished, &auth_finished);
      if ((auth_err == RETROK) && retry)
      {
        do
        {
          free((void *) hs->message);
          hs->message = 0;
        }
        while (0);
        resp_free(&resp);
        do
        {
          free((void *) message);
          message = 0;
        }
        while (0);
        do
        {
          free((void *) head);
          head = 0;
        }
        while (0);
        goto retry_with_auth;
      }
    }
    if (auth_err == RETROK)
      retval = AUTHFAILED;
    else
      retval = auth_err;
    goto cleanup;
  }
  else
  {
    if (ntlm_seen)
      set_pconn_authorized_wrapper(1);
  }
  if (statcode == 504)
  {
    hs->len = 0;
    hs->res = 0;
    hs->restval = 0;
    do
    {
      if (!keep_alive)
      {
        if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
          invalidate_persistent();
        else
          fd_close(sock);
        sock = -1;
      }
    }
    while (0);
    do
    {
      free((void *) hs->message);
      hs->message = 0;
    }
    while (0);
    retval = GATEWAYTIMEOUT;
    goto cleanup;
  }
  {
    uerr_t ret = check_file_output(u, hs, resp, hdrval, sizeof(hdrval));
    if (ret != RETROK)
    {
      retval = ret;
      goto cleanup;
    }
  }
  hs->statcode = statcode;
  if (statcode == (-1))
    hs->error = xstrdup(gettext("Malformed status line"));
  else
    if (!(*message))
    hs->error = xstrdup(gettext("(no description)"));
  else
    hs->error = xstrdup(message);
  if (get_opt_hsts_wrapper() && get_hsts_store_wrapper())
  {
    hsts_params = resp_header_strdup(resp, "Strict-Transport-Security");
    if (parse_strict_transport_security(hsts_params, &max_age, &include_subdomains))
    {
      if (hsts_store_entry(get_hsts_store_wrapper(), u->scheme, u->host, u->port, max_age, include_subdomains))
        do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Added new HSTS host: %s:%u (max-age: %u, includeSubdomains: %s)\n", u->host, u->port, (unsigned int) max_age, (include_subdomains) ? ("true") : ("false"));
        }
      }
      while (0);
      else
        do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Updated HSTS host: %s:%u (max-age: %u, includeSubdomains: %s)\n", u->host, u->port, (unsigned int) max_age, (include_subdomains) ? ("true") : ("false"));
        }
      }
      while (0);
    }
  }
  type = resp_header_strdup(resp, "Content-Type");
  if (type)
  {
    char *tmp = strchr(type, ';');
    if (tmp)
    {
      while ((tmp > type) && c_isspace(tmp[-1]))
        --tmp;

      *tmp = '\0';
    }
  }
  hs->newloc = resp_header_strdup(resp, "Location");
  hs->remote_time = resp_header_strdup(resp, "Last-Modified");
  if (resp_header_copy(resp, "Content-Range", hdrval, sizeof(hdrval)))
  {
    wgint first_byte_pos;
    wgint last_byte_pos;
    wgint entity_length;
    if (parse_content_range(hdrval, &first_byte_pos, &last_byte_pos, &entity_length))
    {
      contrange = first_byte_pos;
      contlen = (last_byte_pos - first_byte_pos) + 1;
    }
  }
  if ((statcode >= 200) && (statcode < 300))
    *dt |= RETROKF;
  if (statcode == 204)
  {
    hs->len = 0;
    hs->res = 0;
    hs->restval = 0;
    do
    {
      if (!keep_alive)
      {
        if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
          invalidate_persistent();
        else
          fd_close(sock);
        sock = -1;
      }
    }
    while (0);
    retval = RETRFINISHED;
    goto cleanup;
  }
  if (((((statcode == 301) || (statcode == 302)) || (statcode == 303)) || (statcode == 307)) || (statcode == 300))
  {
    if ((statcode == 300) && (!hs->newloc))
      *dt |= RETROKF;
    else
    {
      logprintf(LOG_VERBOSE, gettext("Location: %s%s\n"), (hs->newloc) ? (escnonprint_uri(hs->newloc)) : (gettext("unspecified")), (hs->newloc) ? (gettext(" [following]")) : (""));
      hs->len = 0;
      hs->res = 0;
      hs->restval = 0;
      if (warc_enabled)
      {
        int _err = read_response_body(hs, sock, 0, contlen, 0, chunked_transfer_encoding, u->url, warc_timestamp_str, warc_request_uuid, warc_ip, type, statcode, head);
        if ((_err != RETRFINISHED) || (hs->res < 0))
        {
          do
          {
            if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
              invalidate_persistent();
            else
              fd_close(sock);
            sock = -1;
          }
          while (0);
          retval = _err;
          goto cleanup;
        }
        else
          do
        {
          if (!keep_alive)
          {
            if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
              invalidate_persistent();
            else
              fd_close(sock);
            sock = -1;
          }
        }
        while (0);
      }
      else
      {
        if ((keep_alive && (!head_only)) && skip_short_body(sock, contlen, chunked_transfer_encoding))
          do
        {
          if (!keep_alive)
          {
            if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
              invalidate_persistent();
            else
              fd_close(sock);
            sock = -1;
          }
        }
        while (0);
        else
          do
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
        while (0);
      }
      switch (statcode)
      {
        case 307:
          retval = NEWLOCATION_KEEP_POST;
          goto cleanup;

        case 301:
          if (get_opt_method_wrapper() && (c_strcasecmp(get_opt_method_wrapper(), "post") != 0))
        {
          retval = NEWLOCATION_KEEP_POST;
          goto cleanup;
        }
          break;

        case 302:
          if (get_opt_method_wrapper() && (c_strcasecmp(get_opt_method_wrapper(), "post") != 0))
        {
          retval = NEWLOCATION_KEEP_POST;
          goto cleanup;
        }
          break;

      }

      retval = NEWLOCATION;
      goto cleanup;
    }
  }
  set_content_type(dt, type);
  if (get_opt_adjust_extension_wrapper())
  {
    if ((*dt) & TEXTHTML)
    {
      ensure_extension(hs, ".html", dt);
    }
    else
      if ((*dt) & TEXTCSS)
    {
      ensure_extension(hs, ".css", dt);
    }
  }
  if (cond_get)
  {
    if (statcode == 304)
    {
      logprintf(LOG_VERBOSE, gettext("File %s not modified on server. Omitting download.\n\n"), quote(hs->local_file));
      *dt |= RETROKF;
      do
      {
        if (!keep_alive)
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
      }
      while (0);
      retval = RETRUNNEEDED;
      goto cleanup;
    }
    else
      if ((statcode == 200) && hs->remote_time)
    {
      time_t tmr = http_atotm(hs->remote_time);
      if (((tmr != ((time_t) (-1))) && (tmr <= hs->orig_file_tstamp)) && ((contlen == (-1)) || (contlen == hs->orig_file_size)))
      {
        logprintf(LOG_VERBOSE, gettext("Server ignored If-Modified-Since header for file %s.\nYou might want to add --no-if-modified-since option.\n\n"), quote(hs->local_file));
        *dt |= RETROKF;
        do
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
        while (0);
        retval = RETRUNNEEDED;
        goto cleanup;
      }
    }
  }
  if ((statcode == 416) || ((((((!get_opt_timestamping_wrapper()) && (hs->restval > 0)) && (statcode == 200)) && (contrange == 0)) && (contlen >= 0)) && (hs->restval >= contlen)))
  {
    logputs(LOG_VERBOSE, gettext("\n    The file is already fully retrieved; nothing to do.\n\n"));
    hs->len = contlen;
    hs->res = 0;
    *dt |= RETROKF;
    if (statcode == 416)
      do
    {
      if (!keep_alive)
      {
        if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
          invalidate_persistent();
        else
          fd_close(sock);
        sock = -1;
      }
    }
    while (0);
    else
      do
    {
      if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
        invalidate_persistent();
      else
        fd_close(sock);
      sock = -1;
    }
    while (0);
    retval = RETRUNNEEDED;
    goto cleanup;
  }
  if (((contrange != 0) && (contrange != hs->restval)) || ((statcode == 206) && (!contrange)))
  {
    do
    {
      if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
        invalidate_persistent();
      else
        fd_close(sock);
      sock = -1;
    }
    while (0);
    retval = RANGEERR;
    goto cleanup;
  }
  if (contlen == (-1))
    hs->contlen = -1;
  else
    hs->contlen = contlen + contrange;
  if (get_opt_verbose_wrapper())
  {
    if ((*dt) & RETROKF)
    {
      logputs(LOG_VERBOSE, gettext("Length: "));
      if (contlen != (-1))
      {
        logputs(LOG_VERBOSE, number_to_static_string(contlen + contrange));
        if ((contlen + contrange) >= 1024)
          logprintf(LOG_VERBOSE, " (%s)", human_readable(contlen + contrange, 10, 1));
        if (contrange)
        {
          if (contlen >= 1024)
            logprintf(LOG_VERBOSE, gettext(", %s (%s) remaining"), number_to_static_string(contlen), human_readable(contlen, 10, 1));
          else
            logprintf(LOG_VERBOSE, gettext(", %s remaining"), number_to_static_string(contlen));
        }
      }
      else
        logputs(LOG_VERBOSE, (get_opt_ignore_length_wrapper()) ? (gettext("ignored")) : (gettext("unspecified")));
      if (type)
        logprintf(LOG_VERBOSE, " [%s]\n", quotearg_style(escape_quoting_style, type));
      else
        logputs(LOG_VERBOSE, "\n");
    }
  }
  if ((((!((*dt) & RETROKF)) && (!get_opt_content_on_error_wrapper())) || head_only) || (get_opt_spider_wrapper() && (!get_opt_recursive_wrapper())))
  {
    hs->len = 0;
    hs->res = 0;
    hs->restval = 0;
    if (warc_enabled)
    {
      int _err = read_response_body(hs, sock, 0, contlen, 0, chunked_transfer_encoding, u->url, warc_timestamp_str, warc_request_uuid, warc_ip, type, statcode, head);
      if ((_err != RETRFINISHED) || (hs->res < 0))
      {
        do
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
        while (0);
        retval = _err;
        goto cleanup;
      }
      else
        do
      {
        if (!keep_alive)
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
      }
      while (0);
    }
    else
    {
      if (head_only)
        do
      {
        if (!keep_alive)
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
      }
      while (0);
      else
        if (get_opt_spider_wrapper() && (!get_opt_recursive_wrapper()))
        do
      {
        if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
          invalidate_persistent();
        else
          fd_close(sock);
        sock = -1;
      }
      while (0);
      else
        if (keep_alive && skip_short_body(sock, contlen, chunked_transfer_encoding))
        do
      {
        if (!keep_alive)
        {
          if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
            invalidate_persistent();
          else
            fd_close(sock);
          sock = -1;
        }
      }
      while (0);
      else
        do
      {
        if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
          invalidate_persistent();
        else
          fd_close(sock);
        sock = -1;
      }
      while (0);
    }
    retval = RETRFINISHED;
    goto cleanup;
  }
  err = open_output_stream(hs, count, &fp);
  if (err != RETROK)
  {
    do
    {
      if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
        invalidate_persistent();
      else
        fd_close(sock);
      sock = -1;
    }
    while (0);
    retval = err;
    goto cleanup;
  }
  err = read_response_body(hs, sock, fp, contlen, contrange, chunked_transfer_encoding, u->url, warc_timestamp_str, warc_request_uuid, warc_ip, type, statcode, head);
  if (hs->res >= 0)
    do
  {
    if (!keep_alive)
    {
      if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
        invalidate_persistent();
      else
        fd_close(sock);
      sock = -1;
    }
  }
  while (0);
  else
    do
  {
    if (get_pconn_active_wrapper() && (sock == get_pconn_socket_wrapper()))
      invalidate_persistent();
    else
      fd_close(sock);
    sock = -1;
  }
  while (0);
  if (!get_output_stream_wrapper())
    fclose(fp);
  retval = err;
  cleanup:
  do
  {
    free((void *) head);
    head = 0;
  }
  while (0);

  do
  {
    free((void *) type);
    type = 0;
  }
  while (0);
  do
  {
    free((void *) message);
    message = 0;
  }
  while (0);
  resp_free(&resp);
  request_free(&req);
  return retval;
}



/* The genuine HTTP loop!  This is the part where the retrieval is
   retried, and retried, and retried, and...  */
uerr_t http_loop(struct url *u, struct url *original_url, char **newloc, char **local_file, const char *referer, int *dt, struct url *proxy, struct iri *iri)
{
  int count;
  bool got_head = 0;
  bool time_came_from_head = 0;
  bool got_name = 0;
  char *tms;
  const char *tmrate;
  uerr_t err;
  uerr_t ret = TRYLIMEXC;
  time_t tmr = -1;
  struct http_stat hstat;
  struct stat st;
  bool send_head_first = 1;
  bool force_full_retrieve = 0;
  if (get_opt_warc_filename_wrapper() != 0)
    force_full_retrieve = 1;
  assert((local_file == 0) || ((*local_file) == 0));
  if (local_file && get_opt_output_document_wrapper())
    *local_file = (((*get_opt_output_document_wrapper()) == '-') && (!(*(get_opt_output_document_wrapper() + 1)))) ? (0) : (xstrdup(get_opt_output_document_wrapper()));
  *newloc = 0;
  if (get_opt_cookies_wrapper())
    load_cookies();
  if (get_opt_ftp_glob_wrapper() && has_wildcards_p(u->path))
    logputs(LOG_VERBOSE, gettext("Warning: wildcards not supported in HTTP.\n"));
  memset(&hstat, '\0', sizeof(hstat));
  hstat.referer = referer;
  if (get_opt_output_document_wrapper())
  {
    hstat.local_file = xstrdup(get_opt_output_document_wrapper());
    got_name = 1;
  }
  else
    if (!get_opt_content_disposition_wrapper())
  {
    hstat.local_file = url_file_name((get_opt_trustservernames_wrapper()) ? (u) : (original_url), 0);
    got_name = 1;
  }
  if (((got_name && file_exists_p(hstat.local_file)) && get_opt_noclobber_wrapper()) && (!get_opt_output_document_wrapper()))
  {
    get_file_flags(hstat.local_file, dt);
    ret = RETROK;
    goto exit;
  }
  count = 0;
  *dt = 0;
  if (!get_opt_spider_wrapper())
    send_head_first = 0;
  if (get_opt_content_disposition_wrapper() && get_opt_always_rest_wrapper())
    send_head_first = 1;
  if (get_opt_timestamping_wrapper())
  {
    if (((get_opt_if_modified_since_wrapper() && (!send_head_first)) && got_name) && file_exists_p(hstat.local_file))
    {
      *dt |= IF_MODIFIED_SINCE;
      {
        uerr_t timestamp_err = set_file_timestamp(&hstat);
        if (timestamp_err != RETROK)
          return timestamp_err;
      }
    }
    else
      if (get_opt_content_disposition_wrapper() || file_exists_p(hstat.local_file))
      send_head_first = 1;
  }
  do
  {
    ++count;
    sleep_between_retrievals(count);
    tms = datetime_str(time(0));
    if (get_opt_spider_wrapper() && (!got_head))
      logprintf(LOG_VERBOSE, gettext("Spider mode enabled. Check if remote file exists.\n"));
    if (get_opt_verbose_wrapper())
    {
      char *hurl = url_string(u, URL_AUTH_HIDE_PASSWD);
      if (count > 1)
      {
        char tmp[256];
        sprintf(tmp, gettext("(try:%2d)"), count);
        logprintf(LOG_NOTQUIET, "--%s--  %s  %s\n", tms, tmp, hurl);
      }
      else
      {
        logprintf(LOG_NOTQUIET, "--%s--  %s\n", tms, hurl);
      }
      do
      {
        free((void *) hurl);
        hurl = 0;
      }
      while (0);
    }
    if (send_head_first && (!got_head))
      *dt |= HEAD_ONLY;
    else
      *dt &= ~HEAD_ONLY;
    if (force_full_retrieve)
      hstat.restval = hstat.len;
    else
      if (get_opt_start_pos_wrapper() >= 0)
      hstat.restval = get_opt_start_pos_wrapper();
    else
      if (((get_opt_always_rest_wrapper() && got_name) && (stat(hstat.local_file, &st) == 0)) && S_ISREG(st.st_mode))
      hstat.restval = st.st_size;
    else
      if (count > 1)
      hstat.restval = hstat.len;
    else
      hstat.restval = 0;
    if ((proxy && (count > 1)) || (!get_opt_allow_cache_wrapper()))
      *dt |= SEND_NOCACHE;
    else
      *dt &= ~SEND_NOCACHE;
    err = gethttp(u, &hstat, dt, proxy, iri, count);
    tms = datetime_str(time(0));
    if (hstat.newloc)
      *newloc = xstrdup(hstat.newloc);
    switch (err)
    {
      case HERR:

      case HEOF:

      case CONSOCKERR:

      case CONERROR:

      case READERR:

      case WRITEFAILED:

      case RANGEERR:

      case FOPEN_EXCL_ERR:

      case GATEWAYTIMEOUT:
        printwhat(count, get_opt_ntry_wrapper());
        continue;

      case FWRITEERR:

      case FOPENERR:
        logputs(LOG_VERBOSE, "\n");
        logprintf(LOG_NOTQUIET, gettext("Cannot write to %s (%s).\n"), quote(hstat.local_file), strerror(errno));

      case HOSTERR:

      case CONIMPOSSIBLE:

      case PROXERR:

      case SSLINITFAILED:

      case CONTNOTSUPPORTED:

      case VERIFCERTERR:

      case FILEBADFILE:

      case UNKNOWNATTR:
        ret = err;
        goto exit;

      case ATTRMISSING:
        logputs(LOG_VERBOSE, "\n");
        logprintf(LOG_NOTQUIET, gettext("Required attribute missing from Header received.\n"));
        ret = err;
        goto exit;

      case AUTHFAILED:
        logputs(LOG_VERBOSE, "\n");
        logprintf(LOG_NOTQUIET, gettext("Username/Password Authentication Failed.\n"));
        ret = err;
        goto exit;

      case WARC_ERR:
        logputs(LOG_VERBOSE, "\n");
        logprintf(LOG_NOTQUIET, gettext("Cannot write to WARC file.\n"));
        ret = err;
        goto exit;

      case WARC_TMP_FOPENERR:

      case WARC_TMP_FWRITEERR:
        logputs(LOG_VERBOSE, "\n");
        logprintf(LOG_NOTQUIET, gettext("Cannot write to temporary WARC file.\n"));
        ret = err;
        goto exit;

      case CONSSLERR:
        logprintf(LOG_NOTQUIET, gettext("Unable to establish SSL connection.\n"));
        ret = err;
        goto exit;

      case UNLINKERR:
        logputs(LOG_VERBOSE, "\n");
        logprintf(LOG_NOTQUIET, gettext("Cannot unlink %s (%s).\n"), quote(hstat.local_file), strerror(errno));
        ret = err;
        goto exit;

      case NEWLOCATION:

      case NEWLOCATION_KEEP_POST:
        if (!(*newloc))
      {
        logprintf(LOG_NOTQUIET, gettext("ERROR: Redirection (%d) without location.\n"), hstat.statcode);
        ret = WRONGCODE;
      }
      else
      {
        ret = err;
      }
        goto exit;

      case RETRUNNEEDED:
        ret = RETROK;
        goto exit;

      case RETRFINISHED:
        break;

      default:
        abort();

    }

    if (!((*dt) & RETROKF))
    {
      char *hurl = 0;
      if (!get_opt_verbose_wrapper())
      {
        hurl = url_string(u, URL_AUTH_HIDE_PASSWD);
        logprintf(LOG_NONVERBOSE, "%s:\n", hurl);
      }
      if (((*dt) & HEAD_ONLY) && ((hstat.statcode == 500) || (hstat.statcode == 501)))
      {
        got_head = 1;
        continue;
      }
      else
        if (get_opt_spider_wrapper() && (!iri->utf8_encode))
      {
        if (!hurl)
          hurl = url_string(u, URL_AUTH_HIDE_PASSWD);
        nonexisting_url(hurl);
        logprintf(LOG_NOTQUIET, gettext("Remote file does not exist -- broken link!!!\n"));
      }
      else
      {
        logprintf(LOG_NOTQUIET, gettext("%s ERROR %d: %s.\n"), tms, hstat.statcode, quotearg_style(escape_quoting_style, hstat.error));
      }
      logputs(LOG_VERBOSE, "\n");
      ret = WRONGCODE;
      do
      {
        free((void *) hurl);
        hurl = 0;
      }
      while (0);
      goto exit;
    }
    if ((!got_head) || (get_opt_spider_wrapper() && (!get_opt_recursive_wrapper())))
    {
      got_head = 1;
      if (get_opt_timestamping_wrapper() && (!hstat.remote_time))
      {
        logputs(LOG_NOTQUIET, gettext("Last-modified header missing -- time-stamps turned off.\n"));
      }
      else
        if (hstat.remote_time)
      {
        tmr = http_atotm(hstat.remote_time);
        if (tmr == ((time_t) (-1)))
          logputs(LOG_VERBOSE, gettext("Last-modified header invalid -- time-stamp ignored.\n"));
        if ((*dt) & HEAD_ONLY)
          time_came_from_head = 1;
      }
      if (send_head_first)
      {
        if (get_opt_timestamping_wrapper())
        {
          if (hstat.orig_file_name)
          {
            if (hstat.remote_time && (tmr != ((time_t) (-1))))
            {
              if (hstat.orig_file_tstamp >= tmr)
              {
                if ((hstat.contlen == (-1)) || (hstat.orig_file_size == hstat.contlen))
                {
                  logprintf(LOG_VERBOSE, gettext("Server file no newer than local file %s -- not retrieving.\n\n"), quote(hstat.orig_file_name));
                  ret = RETROK;
                  goto exit;
                }
                else
                {
                  logprintf(LOG_VERBOSE, gettext("The sizes do not match (local %s) -- retrieving.\n"), number_to_static_string(hstat.orig_file_size));
                }
              }
              else
              {
                force_full_retrieve = 1;
                logputs(LOG_VERBOSE, gettext("Remote file is newer, retrieving.\n"));
              }
              logputs(LOG_VERBOSE, "\n");
            }
          }
          hstat.timestamp_checked = 1;
        }
        if (get_opt_spider_wrapper())
        {
          bool finished = 1;
          if (get_opt_recursive_wrapper())
          {
            if ((*dt) & TEXTHTML)
            {
              logputs(LOG_VERBOSE, gettext("Remote file exists and could contain links to other resources -- retrieving.\n\n"));
              finished = 0;
            }
            else
            {
              logprintf(LOG_VERBOSE, gettext("Remote file exists but does not contain any link -- not retrieving.\n\n"));
              ret = RETROK;
            }
          }
          else
          {
            if ((*dt) & TEXTHTML)
            {
              logprintf(LOG_VERBOSE, gettext("Remote file exists and could contain further links,\nbut recursion is disabled -- not retrieving.\n\n"));
            }
            else
            {
              logprintf(LOG_VERBOSE, gettext("Remote file exists.\n\n"));
            }
            ret = RETROK;
          }
          if (finished)
          {
            logprintf(LOG_NONVERBOSE, gettext("%s URL: %s %2d %s\n"), tms, u->url, hstat.statcode, (hstat.message) ? (quotearg_style(escape_quoting_style, hstat.message)) : (""));
            goto exit;
          }
        }
        got_name = 1;
        *dt &= ~HEAD_ONLY;
        count = 0;
        do
        {
          free((void *) hstat.message);
          hstat.message = 0;
        }
        while (0);
        do
        {
          free((void *) hstat.error);
          hstat.error = 0;
        }
        while (0);
        continue;
      }
    }
    if ((get_opt_useservertimestamps_wrapper() && (tmr != ((time_t) (-1)))) && ((hstat.len == hstat.contlen) || ((hstat.res == 0) && (hstat.contlen == (-1)))))
    {
      const char *fl = 0;
      set_local_file(&fl, hstat.local_file);
      if (fl)
      {
        time_t newtmr = -1;
        if ((time_came_from_head && hstat.remote_time) && hstat.remote_time[0])
        {
          newtmr = http_atotm(hstat.remote_time);
          if (newtmr != ((time_t) (-1)))
            tmr = newtmr;
        }
        touch(fl, tmr);
      }
    }
    tmrate = retr_rate(hstat.rd_size, hstat.dltime);
    set_total_download_time_wrapper(hstat.dltime);
    if (hstat.len == hstat.contlen)
    {
      if (((*dt) & RETROKF) || get_opt_content_on_error_wrapper())
      {
        bool write_to_stdout = get_opt_output_document_wrapper() && (((*get_opt_output_document_wrapper()) == '-') && (!(*(get_opt_output_document_wrapper() + 1))));
        logprintf(LOG_VERBOSE, (write_to_stdout) ? (gettext("%s (%s) - written to stdout %s[%s/%s]\n\n")) : (gettext("%s (%s) - %s saved [%s/%s]\n\n")), tms, tmrate, (write_to_stdout) ? ("") : (quote(hstat.local_file)), number_to_static_string(hstat.len), number_to_static_string(hstat.contlen));
        logprintf(LOG_NONVERBOSE, "%s URL:%s [%s/%s] -> \"%s\" [%d]\n", tms, u->url, number_to_static_string(hstat.len), number_to_static_string(hstat.contlen), hstat.local_file, count);
      }
      gsetter_numurls_prefix_wrapper();
      set_total_downloaded_bytes_wrapper(hstat.rd_size);
      if ((*dt) & ADDED_HTML_EXTENSION)
        downloaded_file(FILE_DOWNLOADED_AND_HTML_EXTENSION_ADDED, hstat.local_file);
      else
        downloaded_file(FILE_DOWNLOADED_NORMALLY, hstat.local_file);
      ret = RETROK;
      goto exit;
    }
    else
      if (hstat.res == 0)
    {
      if (hstat.contlen == (-1))
      {
        if (((*dt) & RETROKF) || get_opt_content_on_error_wrapper())
        {
          bool write_to_stdout = get_opt_output_document_wrapper() && (((*get_opt_output_document_wrapper()) == '-') && (!(*(get_opt_output_document_wrapper() + 1))));
          logprintf(LOG_VERBOSE, (write_to_stdout) ? (gettext("%s (%s) - written to stdout %s[%s]\n\n")) : (gettext("%s (%s) - %s saved [%s]\n\n")), tms, tmrate, (write_to_stdout) ? ("") : (quote(hstat.local_file)), number_to_static_string(hstat.len));
          logprintf(LOG_NONVERBOSE, "%s URL:%s [%s] -> \"%s\" [%d]\n", tms, u->url, number_to_static_string(hstat.len), hstat.local_file, count);
        }
        gsetter_numurls_prefix_wrapper();
        set_total_downloaded_bytes_wrapper(hstat.rd_size);
        if ((*dt) & ADDED_HTML_EXTENSION)
          downloaded_file(FILE_DOWNLOADED_AND_HTML_EXTENSION_ADDED, hstat.local_file);
        else
          downloaded_file(FILE_DOWNLOADED_NORMALLY, hstat.local_file);
        ret = RETROK;
        goto exit;
      }
      else
        if (hstat.len < hstat.contlen)
      {
        logprintf(LOG_VERBOSE, gettext("%s (%s) - Connection closed at byte %s. "), tms, tmrate, number_to_static_string(hstat.len));
        printwhat(count, get_opt_ntry_wrapper());
        continue;
      }
      else
        if (hstat.len != hstat.restval)
        abort();
      else
      {
        ret = RETROK;
        goto exit;
      }
    }
    else
    {
      if (hstat.contlen == (-1))
      {
        logprintf(LOG_VERBOSE, gettext("%s (%s) - Read error at byte %s (%s)."), tms, tmrate, number_to_static_string(hstat.len), hstat.rderrmsg);
        printwhat(count, get_opt_ntry_wrapper());
        continue;
      }
      else
      {
        logprintf(LOG_VERBOSE, gettext("%s (%s) - Read error at byte %s/%s (%s). "), tms, tmrate, number_to_static_string(hstat.len), number_to_static_string(hstat.contlen), hstat.rderrmsg);
        printwhat(count, get_opt_ntry_wrapper());
        continue;
      }
    }
  }
  while ((!get_opt_ntry_wrapper()) || (count < get_opt_ntry_wrapper()));
  exit:
  if (((ret == RETROK) || get_opt_content_on_error_wrapper()) && local_file)
  {
    do
    {
      free((void *) (*local_file));
      *local_file = 0;
    }
    while (0);
    *local_file = xstrdup(hstat.local_file);
  }

  free_hstat(&hstat);
  return ret;
}



/* Check whether the result of strptime() indicates success.
   strptime() returns the pointer to how far it got to in the string.
   The processing has been successful if the string is at `GMT' or
   `+X', or at the end of the string.

   In extended regexp parlance, the function returns 1 if P matches
   "^ *(GMT|[+-][0-9]|$)", 0 otherwise.  P being NULL (which strptime
   can return) is considered a failure and 0 is returned.  */
static bool
check_end (const char *p)
{
  if (!p)
    return false;
  while (c_isspace (*p))
    ++p;
  if (!*p
      || (p[0] == 'G' && p[1] == 'M' && p[2] == 'T')
      || ((p[0] == '+' || p[0] == '-') && c_isdigit (p[1])))
    return true;
  else
    return false;
}

/* Convert the textual specification of time in TIME_STRING to the
   number of seconds since the Epoch.

   TIME_STRING can be in any of the three formats RFC2616 allows the
   HTTP servers to emit -- RFC1123-date, RFC850-date or asctime-date,
   as well as the time format used in the Set-Cookie header.
   Timezones are ignored, and should be GMT.

   Return the computed time_t representation, or -1 if the conversion
   fails.

   This function uses strptime with various string formats for parsing
   TIME_STRING.  This results in a parser that is not as lenient in
   interpreting TIME_STRING as I would like it to be.  Being based on
   strptime, it always allows shortened months, one-digit days, etc.,
   but due to the multitude of formats in which time can be
   represented, an ideal HTTP time parser would be even more
   forgiving.  It should completely ignore things like week days and
   concentrate only on the various forms of representing years,
   months, days, hours, minutes, and seconds.  For example, it would
   be nice if it accepted ISO 8601 out of the box.

   I've investigated free and PD code for this purpose, but none was
   usable.  getdate was big and unwieldy, and had potential copyright
   issues, or so I was informed.  Dr. Marcus Hennecke's atotm(),
   distributed with phttpd, is excellent, but we cannot use it because
   it is not assigned to the FSF.  So I stuck it with strptime.  */

time_t
http_atotm (const char *time_string)
{
  /* NOTE: Solaris strptime man page claims that %n and %t match white
     space, but that's not universally available.  Instead, we simply
     use ` ' to mean "skip all WS", which works under all strptime
     implementations I've tested.  */

  static const char *time_formats[] = {
    "%a, %d %b %Y %T",          /* rfc1123: Thu, 29 Jan 1998 22:12:57 */
    "%A, %d-%b-%y %T",          /* rfc850:  Thursday, 29-Jan-98 22:12:57 */
    "%a %b %d %T %Y",           /* asctime: Thu Jan 29 22:12:57 1998 */
    "%a, %d-%b-%Y %T"           /* cookies: Thu, 29-Jan-1998 22:12:57
                                   (used in Set-Cookie, defined in the
                                   Netscape cookie specification.) */
  };
  const char *oldlocale;
  char savedlocale[256];
  size_t i;
  time_t ret = (time_t) -1;

  /* Solaris strptime fails to recognize English month names in
     non-English locales, which we work around by temporarily setting
     locale to C before invoking strptime.  */
  oldlocale = setlocale (LC_TIME, NULL);
  if (oldlocale)
    {
      size_t l = strlen (oldlocale) + 1;
      if (l >= sizeof savedlocale)
        savedlocale[0] = '\0';
      else
        memcpy (savedlocale, oldlocale, l);
    }
  else savedlocale[0] = '\0';

  setlocale (LC_TIME, "C");

  for (i = 0; i < countof (time_formats); i++)
    {
      struct tm t;

      /* Some versions of strptime use the existing contents of struct
         tm to recalculate the date according to format.  Zero it out
         to prevent stack garbage from influencing strptime.  */
      xzero (t);

      if (check_end (strptime (time_string, time_formats[i], &t)))
        {
          ret = timegm (&t);
          break;
        }
    }

  /* Restore the previous locale. */
  if (savedlocale[0])
    setlocale (LC_TIME, savedlocale);

  return ret;
}

/* Authorization support: We support three authorization schemes:

   * `Basic' scheme, consisting of base64-ing USER:PASSWORD string;

   * `Digest' scheme, added by Junio Hamano <junio@twinsun.com>,
   consisting of answering to the server's challenge with the proper
   MD5 digests.

   * `NTLM' ("NT Lan Manager") scheme, based on code written by Daniel
   Stenberg for libcurl.  Like digest, NTLM is based on a
   challenge-response mechanism, but unlike digest, it is non-standard
   (authenticates TCP connections rather than requests), undocumented
   and Microsoft-specific.  */

/* Create the authentication header contents for the `Basic' scheme.
   This is done by encoding the string "USER:PASS" to base64 and
   prepending the string "Basic " in front of it.  */

static char *
basic_authentication_encode (const char *user, const char *passwd)
{
  char *t1, *t2;
  int len1 = strlen (user) + 1 + strlen (passwd);

  t1 = (char *)alloca (len1 + 1);
  sprintf (t1, "%s:%s", user, passwd);

  t2 = (char *)alloca (BASE64_LENGTH (len1) + 1);
  base64_encode (t1, len1, t2);

  return concat_strings ("Basic ", t2, (char *) 0);
}

#define SKIP_WS(x) do {                         \
  while (c_isspace (*(x)))                        \
    ++(x);                                      \
} while (0)

#ifdef ENABLE_DIGEST
/* Dump the hexadecimal representation of HASH to BUF.  HASH should be
   an array of 16 bytes containing the hash keys, and BUF should be a
   buffer of 33 writable characters (32 for hex digits plus one for
   zero termination).  */
static void
dump_hash (char *buf, const unsigned char *hash)
{
  int i;

  for (i = 0; i < MD5_DIGEST_SIZE; i++, hash++)
    {
      *buf++ = XNUM_TO_digit (*hash >> 4);
      *buf++ = XNUM_TO_digit (*hash & 0xf);
    }
  *buf = '\0';
}

/* Take the line apart to find the challenge, and compose a digest
   authorization header.  See RFC2069 section 2.1.2.  */
static char *
digest_authentication_encode (const char *au, const char *user,
                              const char *passwd, const char *method,
                              const char *path, uerr_t *auth_err)
{
  static char *realm, *opaque, *nonce, *qop, *algorithm;
  static struct {
    const char *name;
    char **variable;
  } options[] = {
    { "realm", &realm },
    { "opaque", &opaque },
    { "nonce", &nonce },
    { "qop", &qop },
    { "algorithm", &algorithm }
  };
  char cnonce[16] = "";
  char *res;
  int res_len;
  size_t res_size;
  param_token name, value;


  realm = opaque = nonce = algorithm = qop = NULL;

  au += 6;                      /* skip over `Digest' */
  while (extract_param (&au, &name, &value, ',', NULL))
    {
      size_t i;
      size_t namelen = name.e - name.b;
      for (i = 0; i < countof (options); i++)
        if (namelen == strlen (options[i].name)
            && 0 == strncmp (name.b, options[i].name,
                             namelen))
          {
            *options[i].variable = strdupdelim (value.b, value.e);
            break;
          }
    }

  if (qop != NULL && strcmp(qop,"auth"))
    {
      logprintf (LOG_NOTQUIET, _("Unsupported quality of protection '%s'.\n"), qop);
      xfree (qop); /* force freeing mem and return */
    }
  else if (algorithm != NULL && strcmp (algorithm,"MD5") && strcmp (algorithm,"MD5-sess"))
    {
      logprintf (LOG_NOTQUIET, _("Unsupported algorithm '%s'.\n"), algorithm);
      xfree (qop); /* force freeing mem and return */
    }

  if (!realm || !nonce || !user || !passwd || !path || !method)
    {
      *auth_err = ATTRMISSING;

      xfree (realm);
      xfree (opaque);
      xfree (nonce);
      xfree (qop);
      xfree (algorithm);

      return NULL;
    }

  /* Calculate the digest value.  */
  {
    struct md5_ctx ctx;
    unsigned char hash[MD5_DIGEST_SIZE];
    char a1buf[MD5_DIGEST_SIZE * 2 + 1], a2buf[MD5_DIGEST_SIZE * 2 + 1];
    char response_digest[MD5_DIGEST_SIZE * 2 + 1];

    /* A1BUF = H(user ":" realm ":" password) */
    md5_init_ctx (&ctx);
    md5_process_bytes ((unsigned char *)user, strlen (user), &ctx);
    md5_process_bytes ((unsigned char *)":", 1, &ctx);
    md5_process_bytes ((unsigned char *)realm, strlen (realm), &ctx);
    md5_process_bytes ((unsigned char *)":", 1, &ctx);
    md5_process_bytes ((unsigned char *)passwd, strlen (passwd), &ctx);
    md5_finish_ctx (&ctx, hash);

    dump_hash (a1buf, hash);

    if (algorithm && !strcmp (algorithm, "MD5-sess"))
      {
        /* A1BUF = H( H(user ":" realm ":" password) ":" nonce ":" cnonce ) */
        snprintf (cnonce, sizeof (cnonce), "%08x", random_number(INT_MAX));

        md5_init_ctx (&ctx);
        /* md5_process_bytes (hash, MD5_DIGEST_SIZE, &ctx); */
        md5_process_bytes (a1buf, MD5_DIGEST_SIZE * 2, &ctx);
        md5_process_bytes ((unsigned char *)":", 1, &ctx);
        md5_process_bytes ((unsigned char *)nonce, strlen (nonce), &ctx);
        md5_process_bytes ((unsigned char *)":", 1, &ctx);
        md5_process_bytes ((unsigned char *)cnonce, strlen (cnonce), &ctx);
        md5_finish_ctx (&ctx, hash);

        dump_hash (a1buf, hash);
      }

    /* A2BUF = H(method ":" path) */
    md5_init_ctx (&ctx);
    md5_process_bytes ((unsigned char *)method, strlen (method), &ctx);
    md5_process_bytes ((unsigned char *)":", 1, &ctx);
    md5_process_bytes ((unsigned char *)path, strlen (path), &ctx);
    md5_finish_ctx (&ctx, hash);
    dump_hash (a2buf, hash);

    if (qop && !strcmp(qop, "auth"))
      {
        /* RFC 2617 Digest Access Authentication */
        /* generate random hex string */
        if (!*cnonce)
          snprintf(cnonce, sizeof(cnonce), "%08x", random_number(INT_MAX));

        /* RESPONSE_DIGEST = H(A1BUF ":" nonce ":" noncecount ":" clientnonce ":" qop ": " A2BUF) */
        md5_init_ctx (&ctx);
        md5_process_bytes ((unsigned char *)a1buf, MD5_DIGEST_SIZE * 2, &ctx);
        md5_process_bytes ((unsigned char *)":", 1, &ctx);
        md5_process_bytes ((unsigned char *)nonce, strlen (nonce), &ctx);
        md5_process_bytes ((unsigned char *)":", 1, &ctx);
        md5_process_bytes ((unsigned char *)"00000001", 8, &ctx); /* TODO: keep track of server nonce values */
        md5_process_bytes ((unsigned char *)":", 1, &ctx);
        md5_process_bytes ((unsigned char *)cnonce, strlen(cnonce), &ctx);
        md5_process_bytes ((unsigned char *)":", 1, &ctx);
        md5_process_bytes ((unsigned char *)qop, strlen(qop), &ctx);
        md5_process_bytes ((unsigned char *)":", 1, &ctx);
        md5_process_bytes ((unsigned char *)a2buf, MD5_DIGEST_SIZE * 2, &ctx);
        md5_finish_ctx (&ctx, hash);
      }
    else
      {
        /* RFC 2069 Digest Access Authentication */
        /* RESPONSE_DIGEST = H(A1BUF ":" nonce ":" A2BUF) */
        md5_init_ctx (&ctx);
        md5_process_bytes ((unsigned char *)a1buf, MD5_DIGEST_SIZE * 2, &ctx);
        md5_process_bytes ((unsigned char *)":", 1, &ctx);
        md5_process_bytes ((unsigned char *)nonce, strlen (nonce), &ctx);
        md5_process_bytes ((unsigned char *)":", 1, &ctx);
        md5_process_bytes ((unsigned char *)a2buf, MD5_DIGEST_SIZE * 2, &ctx);
        md5_finish_ctx (&ctx, hash);
      }

    dump_hash (response_digest, hash);

    res_size = strlen (user)
             + strlen (realm)
             + strlen (nonce)
             + strlen (path)
             + 2 * MD5_DIGEST_SIZE /*strlen (response_digest)*/
             + (opaque ? strlen (opaque) : 0)
             + (algorithm ? strlen (algorithm) : 0)
             + (qop ? 128: 0)
             + strlen (cnonce)
             + 128;

    res = xmalloc (res_size);

    if (qop && !strcmp (qop, "auth"))
      {
        res_len = snprintf (res, res_size, "Digest "\
                "username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\""\
                ", qop=auth, nc=00000001, cnonce=\"%s\"",
                  user, realm, nonce, path, response_digest, cnonce);

      }
    else
      {
        res_len = snprintf (res, res_size, "Digest "\
                "username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",
                  user, realm, nonce, path, response_digest);
      }

    if (opaque)
      {
        res_len += snprintf(res + res_len, res_size - res_len, ", opaque=\"%s\"", opaque);
      }

    if (algorithm)
      {
        snprintf(res + res_len, res_size - res_len, ", algorithm=\"%s\"", algorithm);
      }
  }

  xfree (realm);
  xfree (opaque);
  xfree (nonce);
  xfree (qop);
  xfree (algorithm);

  return res;
}
#endif /* ENABLE_DIGEST */

/* Computing the size of a string literal must take into account that
   value returned by sizeof includes the terminating \0.  */
#define STRSIZE(literal) (sizeof (literal) - 1)

/* Whether chars in [b, e) begin with the literal string provided as
   first argument and are followed by whitespace or terminating \0.
   The comparison is case-insensitive.  */
#define STARTS(literal, b, e)                           \
  ((e > b) \
   && ((size_t) ((e) - (b))) >= STRSIZE (literal)   \
   && 0 == c_strncasecmp (b, literal, STRSIZE (literal))  \
   && ((size_t) ((e) - (b)) == STRSIZE (literal)          \
       || c_isspace (b[STRSIZE (literal)])))

static bool
known_authentication_scheme_p (const char *hdrbeg, const char *hdrend)
{
  return STARTS ("Basic", hdrbeg, hdrend)
#ifdef ENABLE_DIGEST
    || STARTS ("Digest", hdrbeg, hdrend)
#endif
#ifdef ENABLE_NTLM
    || STARTS ("NTLM", hdrbeg, hdrend)
#endif
    ;
}

#undef STARTS

/* Create the HTTP authorization request header.  When the
   `WWW-Authenticate' response header is seen, according to the
   authorization scheme specified in that header (`Basic' and `Digest'
   are supported by the current implementation), produce an
   appropriate HTTP authorization request header.  */
static char *create_authorization_line(const char *au, const char *user, const char *passwd, const char *method, const char *path, bool *finished, uerr_t *auth_err)
{
  switch (c_toupper(*au))
  {
    case 'B':
      *finished = 1;
      return basic_authentication_encode(user, passwd);

    case 'D':
      *finished = 1;
      return digest_authentication_encode(au, user, passwd, method, path, auth_err);

    case 'N':
      if (!ntlm_input(&get_pconn_ntlm_wrapper(), au))
    {
      *finished = 1;
      return 0;
    }
      return ntlm_output(&get_pconn_ntlm_wrapper(), user, passwd, finished);

    default:
      abort();

  }

}



static void load_cookies(void)
{
  if (!get_wget_cookie_jar_wrapper())
    set_wget_cookie_jar_wrapper(cookie_jar_new());
  if (get_opt_cookies_input_wrapper() && (!get_cookies_loaded_p_wrapper()))
  {
    cookie_jar_load(get_wget_cookie_jar_wrapper(), get_opt_cookies_input_wrapper());
    set_cookies_loaded_p_wrapper(1);
  }
}



void save_cookies(void)
{
  if (get_wget_cookie_jar_wrapper())
    cookie_jar_save(get_wget_cookie_jar_wrapper(), get_opt_cookies_output_wrapper());
}



void http_cleanup(void)
{
  do
  {
    free((void *) get_pconn_host_wrapper());
    set_pconn_host_wrapper(0);
  }
  while (0);
  if (get_wget_cookie_jar_wrapper())
    cookie_jar_delete(get_wget_cookie_jar_wrapper());
}



void ensure_extension(struct http_stat *hs, const char *ext, int *dt)
{
  char *last_period_in_local_filename = strrchr(hs->local_file, '.');
  char shortext[8];
  int len;
  shortext[0] = '\0';
  len = strlen(ext);
  if (len == 5)
  {
    memcpy(shortext, ext, len - 1);
    shortext[len - 1] = '\0';
  }
  if ((last_period_in_local_filename == 0) || (!((0 == strcasecmp(last_period_in_local_filename, shortext)) || (0 == strcasecmp(last_period_in_local_filename, ext)))))
  {
    int local_filename_len = strlen(hs->local_file);
    hs->local_file = xrealloc(hs->local_file, (local_filename_len + 24) + len);
    strcpy(hs->local_file + local_filename_len, ext);
    if ((!(((((get_opt_noclobber_wrapper() || get_opt_always_rest_wrapper()) || get_opt_timestamping_wrapper()) || get_opt_dirstruct_wrapper()) || get_opt_output_document_wrapper()) || (get_opt_backups_wrapper() > 0))) && file_exists_p(hs->local_file))
    {
      int ext_num = 1;
      do
        sprintf(hs->local_file + local_filename_len, ".%d%s", ext_num++, ext);
      while (file_exists_p(hs->local_file));
    }
    *dt |= ADDED_HTML_EXTENSION;
  }
}



#ifdef TESTING

const char *
test_parse_range_header(void)
{
  unsigned i;
  static const struct {
    const char * rangehdr;
    const wgint firstbyte;
    const wgint lastbyte;
    const wgint length;
    const bool shouldPass;
  } test_array[] = {
      { "bytes 0-1000/1000", 0, 1000, 1000, false },
      { "bytes 0-999/1000", 0, 999, 1000, true },
      { "bytes 100-99/1000", 100, 99, 1000, false },
      { "bytes 100-100/1000", 100, 100, 1000, true },
      { "bytes 0-1000/100000000", 0, 1000, 100000000, true },
      { "bytes 1-999/1000", 1, 999, 1000, true },
      { "bytes 42-1233/1234", 42, 1233, 1234, true },
      { "bytes 42-1233/*", 42, 1233, -1, true },
      { "bytes 0-2147483648/2147483649", 0, 2147483648, 2147483649, true },
      { "bytes 2147483648-4294967296/4294967297", 2147483648, 4294967296, 4294967297, true }
  };

  wgint firstbyteptr[sizeof(wgint)];
  wgint lastbyteptr[sizeof(wgint)];
  wgint lengthptr[sizeof(wgint)];
  bool result;
  for (i = 0; i < countof (test_array); i++)
    {
      result = parse_content_range (test_array[i].rangehdr, firstbyteptr, lastbyteptr, lengthptr);
#if 0
      printf ("%ld %ld\n", test_array[i].firstbyte, *firstbyteptr);
      printf ("%ld %ld\n", test_array[i].lastbyte, *lastbyteptr);
      printf ("%ld %ld\n", test_array[i].length, *lengthptr);
      printf ("\n");
#endif
      mu_assert ("test_parse_range_header: False Negative", result == test_array[i].shouldPass);
      mu_assert ("test_parse_range_header: Bad parse", test_array[i].firstbyte == *firstbyteptr &&
                                                       test_array[i].lastbyte == *lastbyteptr &&
                                                       test_array[i].length == *lengthptr);
    }

  return NULL;
}

const char *
test_parse_content_disposition(void)
{
  unsigned i;
  static const struct {
    const char *hdrval;
    const char *filename;
    bool result;
  } test_array[] = {
    { "filename=\"file.ext\"", "file.ext", true },
    { "attachment; filename=\"file.ext\"", "file.ext", true },
    { "attachment; filename=\"file.ext\"; dummy", "file.ext", true },
    { "attachment", NULL, false },
    { "attachement; filename*=UTF-8'en-US'hello.txt", "hello.txt", true },
    { "attachement; filename*0=\"hello\"; filename*1=\"world.txt\"",
      "helloworld.txt", true },
    { "attachment; filename=\"A.ext\"; filename*=\"B.ext\"", "B.ext", true },
    { "attachment; filename*=\"A.ext\"; filename*0=\"B\"; filename*1=\"B.ext\"",
      "A.ext", true },
    { "filename**0=\"A\"; filename**1=\"A.ext\"; filename*0=\"B\";\
filename*1=\"B\"", "AA.ext", true },
  };

  for (i = 0; i < countof(test_array); ++i)
    {
      char *filename;
      bool res;

      res = parse_content_disposition (test_array[i].hdrval, &filename);

      mu_assert ("test_parse_content_disposition: wrong result",
                 res == test_array[i].result
                 && (res == false
                     || 0 == strcmp (test_array[i].filename, filename)));
      xfree (filename);
    }

  return NULL;
}

#endif /* TESTING */

/*
 * vim: et sts=2 sw=2 cino+={s
 */
