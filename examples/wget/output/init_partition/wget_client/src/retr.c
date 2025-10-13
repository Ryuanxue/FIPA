/* File retrieval.
   Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
   2005, 2006, 2007, 2008, 2009, 2010, 2011, 2014, 2015 Free Software
   Foundation, Inc.

This file is part of GNU Wget.

GNU Wget is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

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
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#ifdef VMS
# include <unixio.h>            /* For delete(). */
#endif

#include "exits.h"
#include "utils.h"
#include "retr.h"
#include "progress.h"
#include "url.h"
#include "recur.h"
#include "ftp.h"
#include "http.h"
#include "host.h"
#include "connect.h"
#include "hash.h"
#include "convert.h"
#include "ptimer.h"
#include "html-url.h"
#include "iri.h"
#include "hsts.h"

#include "wget_rpc_wrapper.h"

/* Total size of downloaded files.  Used to enforce quota.  */
SUM_SIZE_INT total_downloaded_bytes;

/* Total download time in seconds. */
double total_download_time;

/* If non-NULL, the stream to which output should be written.  This
   stream is initialized when `-O' is used.  */
FILE *output_stream;

/* Whether output_document is a regular file we can manipulate,
   i.e. not `-' or a device file. */
bool output_stream_regular;

static struct {
  wgint chunk_bytes;
  double chunk_start;
  double sleep_adjust;
} limit_data;

static void limit_bandwidth_reset(void)
{
  {
    new_limit_data temp_limit_data = get_limit_data_wrapper();
    memset(&temp_limit_data, '\0', sizeof(limit_data));
    set_limit_data_wrapper(temp_limit_data);
  }
}



/* Limit the bandwidth by pausing the download for an amount of time.
   BYTES is the number of bytes received from the network, and TIMER
   is the timer that started at the beginning of download.  */

static void limit_bandwidth(wgint bytes, struct ptimer *timer)
{
  double delta_t = ptimer_read(timer) - get_limit_data_chunk_start_wrapper();
  double expected;
  set_limit_data_chunk_bytes_wrapper(bytes);
  expected = ((double) get_limit_data_chunk_bytes_wrapper()) / get_opt_limit_rate_wrapper();
  if (expected > delta_t)
  {
    double slp = (expected - delta_t) + get_limit_data_sleep_adjust_wrapper();
    double t0;
    double t1;
    if (slp < 0.2)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("deferring a %.2f ms sleep (%s/%.2f).\n", slp * 1000, number_to_static_string(get_limit_data_chunk_bytes_wrapper()), delta_t);
        }
      }
      while (0);
      return;
    }
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("\nsleeping %.2f ms for %s bytes, adjust %.2f ms\n", slp * 1000, number_to_static_string(get_limit_data_chunk_bytes_wrapper()), get_limit_data_sleep_adjust_wrapper());
      }
    }
    while (0);
    t0 = ptimer_read(timer);
    xsleep(slp);
    t1 = ptimer_measure(timer);
    set_limit_data_sleep_adjust_wrapper(slp - (t1 - t0));
    if (get_limit_data_sleep_adjust_wrapper() > 0.5)
      set_limit_data_sleep_adjust_wrapper(0.5);
    else
      if (get_limit_data_sleep_adjust_wrapper() < (-0.5))
      set_limit_data_sleep_adjust_wrapper(-0.5);
  }
  set_limit_data_chunk_bytes_wrapper(0);
  set_limit_data_chunk_start_wrapper(ptimer_read(timer));
}



/* Write data in BUF to OUT.  However, if *SKIP is non-zero, skip that
   amount of data and decrease SKIP.  Increment *TOTAL by the amount
   of data written.  If OUT2 is not NULL, also write BUF to OUT2.
   In case of error writing to OUT, -1 is returned.  In case of error
   writing to OUT2, -2 is returned.  Return 1 if the whole BUF was
   skipped.  */

static int
write_data (FILE *out, FILE *out2, const char *buf, int bufsize,
            wgint *skip, wgint *written)
{
  if (out == NULL && out2 == NULL)
    return 1;
  if (*skip > bufsize)
    {
      *skip -= bufsize;
      return 1;
    }
  if (*skip)
    {
      buf += *skip;
      bufsize -= *skip;
      *skip = 0;
      if (bufsize == 0)
        return 1;
    }

  if (out != NULL)
    fwrite (buf, 1, bufsize, out);
  if (out2 != NULL)
    fwrite (buf, 1, bufsize, out2);
  *written += bufsize;

  /* Immediately flush the downloaded data.  This should not hinder
     performance: fast downloads will arrive in large 16K chunks
     (which stdio would write out immediately anyway), and slow
     downloads wouldn't be limited by disk speed.  */

  /* 2005-04-20 SMS.
     Perhaps it shouldn't hinder performance, but it sure does, at least
     on VMS (more than 2X).  Rather than speculate on what it should or
     shouldn't do, it might make more sense to test it.  Even better, it
     might be nice to explain what possible benefit it could offer, as
     it appears to be a clear invitation to poor performance with no
     actual justification.  (Also, why 16K?  Anyone test other values?)
  */
#ifndef __VMS
  if (out != NULL)
    fflush (out);
  if (out2 != NULL)
    fflush (out2);
#endif /* ndef __VMS */
  if (out != NULL && ferror (out))
    return -1;
  else if (out2 != NULL && ferror (out2))
    return -2;
  else
    return 0;
}

/* Read the contents of file descriptor FD until it the connection
   terminates or a read error occurs.  The data is read in portions of
   up to 16K and written to OUT as it arrives.  If opt.verbose is set,
   the progress is shown.

   TOREAD is the amount of data expected to arrive, normally only used
   by the progress gauge.

   STARTPOS is the position from which the download starts, used by
   the progress gauge.  If QTYREAD is non-NULL, the value it points to
   is incremented by the amount of data read from the network.  If
   QTYWRITTEN is non-NULL, the value it points to is incremented by
   the amount of data written to disk.  The time it took to download
   the data is stored to ELAPSED.

   If OUT2 is non-NULL, the contents is also written to OUT2.
   OUT2 will get an exact copy of the response: if this is a chunked
   response, everything -- including the chunk headers -- is written
   to OUT2.  (OUT will only get the unchunked response.)

   The function exits and returns the amount of data read.  In case of
   error while reading data, -1 is returned.  In case of error while
   writing data to OUT, -2 is returned.  In case of error while writing
   data to OUT2, -3 is returned.  */

int fd_read_body(const char *downloaded_filename, int fd, FILE *out, wgint toread, wgint startpos, wgint *qtyread, wgint *qtywritten, double *elapsed, int flags, FILE *out2)
{
  int ret = 0;
  int dlbufsize = (1024 > (8 * 1024)) ? (1024) : (8 * 1024);
  char *dlbuf = xmalloc(dlbufsize);
  struct ptimer *timer = 0;
  double last_successful_read_tm = 0;
  void *progress = 0;
  bool progress_interactive = 0;
  bool exact = !(!(flags & rb_read_exactly));
  bool chunked = flags & rb_chunked_transfer_encoding;
  wgint skip = 0;
  wgint sum_read = 0;
  wgint sum_written = 0;
  wgint remaining_chunk_size = 0;
  if (flags & rb_skip_startpos)
    skip = startpos;
  if (get_opt_show_progress_wrapper())
  {
    const char *filename_progress;
    wgint start = (skip) ? (0) : (startpos);
    if (get_opt_dir_prefix_wrapper())
      filename_progress = (downloaded_filename + strlen(get_opt_dir_prefix_wrapper())) + 1;
    else
      filename_progress = downloaded_filename;
    progress = progress_create(filename_progress, start, start + toread);
    progress_interactive = progress_interactive_p(progress);
  }
  if (get_opt_limit_rate_wrapper())
    limit_bandwidth_reset();
  if ((progress || get_opt_limit_rate_wrapper()) || elapsed)
  {
    timer = ptimer_new();
    last_successful_read_tm = 0;
  }
  if (get_opt_limit_rate_wrapper() && (get_opt_limit_rate_wrapper() < dlbufsize))
    dlbufsize = get_opt_limit_rate_wrapper();
  while ((!exact) || (sum_read < toread))
  {
    int rdsize;
    double tmout = get_opt_read_timeout_wrapper();
    if (chunked)
    {
      if (remaining_chunk_size == 0)
      {
        char *line = fd_read_line(fd);
        char *endl;
        if (line == 0)
        {
          ret = -1;
          break;
        }
        else
          if (out2 != 0)
          fwrite(line, 1, strlen(line), out2);
        remaining_chunk_size = strtol(line, &endl, 16);
        do
        {
          free((void *) line);
          line = 0;
        }
        while (0);
        if (remaining_chunk_size == 0)
        {
          ret = 0;
          line = fd_read_line(fd);
          if (line == 0)
            ret = -1;
          else
          {
            if (out2 != 0)
              fwrite(line, 1, strlen(line), out2);
            do
            {
              free((void *) line);
              line = 0;
            }
            while (0);
          }
          break;
        }
      }
      rdsize = (remaining_chunk_size <= dlbufsize) ? (remaining_chunk_size) : (dlbufsize);
    }
    else
      rdsize = (exact) ? (((toread - sum_read) <= dlbufsize) ? (toread - sum_read) : (dlbufsize)) : (dlbufsize);
    if (progress_interactive)
    {
      tmout = 0.95;
      if (get_opt_read_timeout_wrapper())
      {
        double waittm;
        waittm = ptimer_read(timer) - last_successful_read_tm;
        if ((waittm + tmout) > get_opt_read_timeout_wrapper())
        {
          tmout = get_opt_read_timeout_wrapper() - waittm;
          if (tmout < 0)
          {
            ret = -1, errno = ETIMEDOUT;
            break;
          }
        }
      }
    }
    ret = fd_read(fd, dlbuf, rdsize, tmout);
    if ((progress_interactive && (ret < 0)) && (errno == ETIMEDOUT))
      ret = 0;
    else
      if (ret <= 0)
      break;
    if ((progress || get_opt_limit_rate_wrapper()) || elapsed)
    {
      ptimer_measure(timer);
      if (ret > 0)
        last_successful_read_tm = ptimer_read(timer);
    }
    if (ret > 0)
    {
      int write_res;
      sum_read += ret;
      write_res = write_data(out, out2, dlbuf, ret, &skip, &sum_written);
      if (write_res < 0)
      {
        ret = (write_res == (-3)) ? (-3) : (-2);
        goto out;
      }
      if (chunked)
      {
        remaining_chunk_size -= ret;
        if (remaining_chunk_size == 0)
        {
          char *line = fd_read_line(fd);
          if (line == 0)
          {
            ret = -1;
            break;
          }
          else
          {
            if (out2 != 0)
              fwrite(line, 1, strlen(line), out2);
            do
            {
              free((void *) line);
              line = 0;
            }
            while (0);
          }
        }
      }
    }
    if (get_opt_limit_rate_wrapper())
      limit_bandwidth(ret, timer);
    if (progress)
      progress_update(progress, ret, ptimer_read(timer));
  }

  if (ret < (-1))
    ret = -1;
  out:
  if (progress)
    progress_finish(progress, ptimer_read(timer));

  if (elapsed)
    *elapsed = ptimer_read(timer);
  if (timer)
    ptimer_destroy(timer);
  if (qtyread)
    *qtyread += sum_read;
  if (qtywritten)
    *qtywritten += sum_written;
  do
  {
    free((void *) dlbuf);
    dlbuf = 0;
  }
  while (0);
  return ret;
}



/* Read a hunk of data from FD, up until a terminator.  The hunk is
   limited by whatever the TERMINATOR callback chooses as its
   terminator.  For example, if terminator stops at newline, the hunk
   will consist of a line of data; if terminator stops at two
   newlines, it can be used to read the head of an HTTP response.
   Upon determining the boundary, the function returns the data (up to
   the terminator) in malloc-allocated storage.

   In case of read error, NULL is returned.  In case of EOF and no
   data read, NULL is returned and errno set to 0.  In case of having
   read some data, but encountering EOF before seeing the terminator,
   the data that has been read is returned, but it will (obviously)
   not contain the terminator.

   The TERMINATOR function is called with three arguments: the
   beginning of the data read so far, the beginning of the current
   block of peeked-at data, and the length of the current block.
   Depending on its needs, the function is free to choose whether to
   analyze all data or just the newly arrived data.  If TERMINATOR
   returns NULL, it means that the terminator has not been seen.
   Otherwise it should return a pointer to the charactre immediately
   following the terminator.

   The idea is to be able to read a line of input, or otherwise a hunk
   of text, such as the head of an HTTP request, without crossing the
   boundary, so that the next call to fd_read etc. reads the data
   after the hunk.  To achieve that, this function does the following:

   1. Peek at incoming data.

   2. Determine whether the peeked data, along with the previously
      read data, includes the terminator.

      2a. If yes, read the data until the end of the terminator, and
          exit.

      2b. If no, read the peeked data and goto 1.

   The function is careful to assume as little as possible about the
   implementation of peeking.  For example, every peek is followed by
   a read.  If the read returns a different amount of data, the
   process is retried until all data arrives safely.

   SIZEHINT is the buffer size sufficient to hold all the data in the
   typical case (it is used as the initial buffer size).  MAXSIZE is
   the maximum amount of memory this function is allowed to allocate,
   or 0 if no upper limit is to be enforced.

   This function should be used as a building block for other
   functions -- see fd_read_line as a simple example.  */

char *
fd_read_hunk (int fd, hunk_terminator_t terminator, long sizehint, long maxsize)
{
  long bufsize = sizehint;
  char *hunk = xmalloc (bufsize);
  int tail = 0;                 /* tail position in HUNK */

  assert (!maxsize || maxsize >= bufsize);

  while (1)
    {
      const char *end;
      int pklen, rdlen, remain;

      /* First, peek at the available data. */

      pklen = fd_peek (fd, hunk + tail, bufsize - 1 - tail, -1);
      if (pklen < 0)
        {
          xfree (hunk);
          return NULL;
        }
      end = terminator (hunk, hunk + tail, pklen);
      if (end)
        {
          /* The data contains the terminator: we'll drain the data up
             to the end of the terminator.  */
          remain = end - (hunk + tail);
          assert (remain >= 0);
          if (remain == 0)
            {
              /* No more data needs to be read. */
              hunk[tail] = '\0';
              return hunk;
            }
          if (bufsize - 1 < tail + remain)
            {
              bufsize = tail + remain + 1;
              hunk = xrealloc (hunk, bufsize);
            }
        }
      else
        /* No terminator: simply read the data we know is (or should
           be) available.  */
        remain = pklen;

      /* Now, read the data.  Note that we make no assumptions about
         how much data we'll get.  (Some TCP stacks are notorious for
         read returning less data than the previous MSG_PEEK.)  */

      rdlen = fd_read (fd, hunk + tail, remain, 0);
      if (rdlen < 0)
        {
          xfree (hunk);
          return NULL;
        }
      tail += rdlen;
      hunk[tail] = '\0';

      if (rdlen == 0)
        {
          if (tail == 0)
            {
              /* EOF without anything having been read */
              xfree (hunk);
              errno = 0;
              return NULL;
            }
          else
            /* EOF seen: return the data we've read. */
            return hunk;
        }
      if (end && rdlen == remain)
        /* The terminator was seen and the remaining data drained --
           we got what we came for.  */
        return hunk;

      /* Keep looping until all the data arrives. */

      if (tail == bufsize - 1)
        {
          /* Double the buffer size, but refuse to allocate more than
             MAXSIZE bytes.  */
          if (maxsize && bufsize >= maxsize)
            {
              xfree (hunk);
              errno = ENOMEM;
              return NULL;
            }
          bufsize <<= 1;
          if (maxsize && bufsize > maxsize)
            bufsize = maxsize;
          hunk = xrealloc (hunk, bufsize);
        }
    }
}

static const char *
line_terminator (const char *start _GL_UNUSED, const char *peeked, int peeklen)
{
  const char *p = memchr (peeked, '\n', peeklen);
  if (p)
    /* p+1 because the line must include '\n' */
    return p + 1;
  return NULL;
}

/* The maximum size of the single line we agree to accept.  This is
   not meant to impose an arbitrary limit, but to protect the user
   from Wget slurping up available memory upon encountering malicious
   or buggy server output.  Define it to 0 to remove the limit.  */
#define FD_READ_LINE_MAX 4096

/* Read one line from FD and return it.  The line is allocated using
   malloc, but is never larger than FD_READ_LINE_MAX.

   If an error occurs, or if no data can be read, NULL is returned.
   In the former case errno indicates the error condition, and in the
   latter case, errno is NULL.  */

char *
fd_read_line (int fd)
{
  return fd_read_hunk (fd, line_terminator, 128, FD_READ_LINE_MAX);
}

/* Return a printed representation of the download rate, along with
   the units appropriate for the download speed.  */

const char *retr_rate(wgint bytes, double secs)
{
  static char res[20];
  static const char *rate_names[] = {"B/s", "KB/s", "MB/s", "GB/s"};
  static const char *rate_names_bits[] = {"b/s", "Kb/s", "Mb/s", "Gb/s"};
  int units;
  double dlrate = calc_rate(bytes, secs, &units);
  sprintf(res, "%.*f %s", (dlrate >= 99.95) ? (0) : ((dlrate >= 9.995) ? (1) : (2)), dlrate, (!get_opt_report_bps_wrapper()) ? (rate_names[units]) : (rate_names_bits[units]));
  return res;
}



/* Calculate the download rate and trim it as appropriate for the
   speed.  Appropriate means that if rate is greater than 1K/s,
   kilobytes are used, and if rate is greater than 1MB/s, megabytes
   are used.

   UNITS is zero for B/s, one for KB/s, two for MB/s, and three for
   GB/s.  */

double calc_rate(wgint bytes, double secs, int *units)
{
  double dlrate;
  double bibyte = 1000.0;
  if (!get_opt_report_bps_wrapper())
    bibyte = 1024.0;
  assert(secs >= 0);
  assert(bytes >= 0);
  if (secs == 0)
    secs = ptimer_resolution() / 2.0;
  dlrate = convert_to_bits(bytes) / secs;
  if (dlrate < bibyte)
    *units = 0;
  else
    if (dlrate < (bibyte * bibyte))
    *units = 1, dlrate /= bibyte;
  else
    if (dlrate < ((bibyte * bibyte) * bibyte))
    *units = 2, dlrate /= bibyte * bibyte;
  else
    *units = 3, dlrate /= (bibyte * bibyte) * bibyte;
  return dlrate;
}




#define SUSPEND_METHOD do {                     \
  method_suspended = true;                      \
  saved_body_data = opt.body_data;              \
  saved_body_file_name = opt.body_file;         \
  saved_method = opt.method;                    \
  opt.body_data = NULL;                         \
  opt.body_file = NULL;                         \
  opt.method = NULL;                            \
} while (0)

#define RESTORE_METHOD do {                             \
  if (method_suspended)                                 \
    {                                                   \
      opt.body_data = saved_body_data;                  \
      opt.body_file = saved_body_file_name;             \
      opt.method = saved_method;                        \
      method_suspended = false;                         \
    }                                                   \
} while (0)

static char *getproxy (struct url *);

/* Retrieve the given URL.  Decides which loop to call -- HTTP, FTP,
   FTP, proxy, etc.  */

/* #### This function should be rewritten so it doesn't return from
   multiple points. */

uerr_t retrieve_url(struct url *orig_parsed, const char *origurl, char **file, char **newloc, const char *refurl, int *dt, bool recursive, struct iri *iri, bool register_status)
{
  uerr_t result;
  char *url;
  bool location_changed;
  bool iri_fallbacked = 0;
  int dummy;
  char *mynewloc;
  char *proxy;
  struct url *u = orig_parsed;
  struct url *proxy_url;
  int up_error_code;
  char *local_file = 0;
  int redirection_count = 0;
  bool method_suspended = 0;
  char *saved_body_data = 0;
  char *saved_method = 0;
  char *saved_body_file_name = 0;
  if (!dt)
  {
    dt = &dummy;
    dummy = 0;
  }
  url = xstrdup(origurl);
  if (newloc)
    *newloc = 0;
  if (file)
    *file = 0;
  if (!refurl)
    refurl = get_opt_referer_wrapper();
  redirected:
  result = NOCONERROR;

  mynewloc = 0;
  do
  {
    free((void *) local_file);
    local_file = 0;
  }
  while (0);
  proxy_url = 0;
  proxy = getproxy(u);
  if (proxy)
  {
    struct iri *pi = &get_dummy_iri_wrapper();
    ;
    pi->utf8_encode = 0;
    proxy_url = url_parse(proxy, &up_error_code, pi, 1);
    if (!proxy_url)
    {
      char *error = url_error(proxy, up_error_code);
      logprintf(LOG_NOTQUIET, gettext("Error parsing proxy URL %s: %s.\n"), proxy, error);
      do
      {
        free((void *) url);
        url = 0;
      }
      while (0);
      do
      {
        free((void *) error);
        error = 0;
      }
      while (0);
      do
      {
        free((void *) proxy);
        proxy = 0;
      }
      while (0);
      ;
      do
      {
        if (method_suspended)
        {
          set_opt_body_data_wrapper(saved_body_data);
          set_opt_body_file_wrapper(saved_body_file_name);
          set_opt_method_wrapper(saved_method);
          method_suspended = 0;
        }
      }
      while (0);
      result = PROXERR;
      goto bail;
    }
    if ((proxy_url->scheme != SCHEME_HTTP) && (proxy_url->scheme != u->scheme))
    {
      logprintf(LOG_NOTQUIET, gettext("Error in proxy URL %s: Must be HTTP.\n"), proxy);
      url_free(proxy_url);
      do
      {
        free((void *) url);
        url = 0;
      }
      while (0);
      do
      {
        free((void *) proxy);
        proxy = 0;
      }
      while (0);
      ;
      do
      {
        if (method_suspended)
        {
          set_opt_body_data_wrapper(saved_body_data);
          set_opt_body_file_wrapper(saved_body_file_name);
          set_opt_method_wrapper(saved_method);
          method_suspended = 0;
        }
      }
      while (0);
      result = PROXERR;
      goto bail;
    }
    ;
    do
    {
      free((void *) proxy);
      proxy = 0;
    }
    while (0);
  }
  if (((u->scheme == SCHEME_HTTP) || (u->scheme == SCHEME_HTTPS)) || (proxy_url && (proxy_url->scheme == SCHEME_HTTP)))
  {
    extern hsts_store_t hsts_store;
    if (get_opt_hsts_wrapper() && get_hsts_store_wrapper())
    {
      if (hsts_match(get_hsts_store_wrapper(), u))
        logprintf(LOG_VERBOSE, "URL transformed to HTTPS due to an HSTS policy\n");
    }
    result = http_loop(u, orig_parsed, &mynewloc, &local_file, refurl, dt, proxy_url, iri);
  }
  else
    if ((u->scheme == SCHEME_FTP) || (u->scheme == SCHEME_FTPS))
  {
    bool oldrec = recursive;
    bool glob = get_opt_ftp_glob_wrapper();
    if (redirection_count)
      oldrec = (glob = 0);
    result = ftp_loop(u, orig_parsed, &local_file, dt, proxy_url, recursive, glob);
    recursive = oldrec;
    if ((redirection_count && local_file) && ((u->scheme == SCHEME_FTP) || (u->scheme == SCHEME_FTPS)))
    {
      if (has_html_suffix_p(local_file))
        *dt |= TEXTHTML;
    }
  }
  if (proxy_url)
  {
    url_free(proxy_url);
    proxy_url = 0;
  }
  location_changed = (result == NEWLOCATION) || (result == NEWLOCATION_KEEP_POST);
  if (location_changed)
  {
    char *construced_newloc;
    struct url *newloc_parsed;
    assert(mynewloc != 0);
    do
    {
      free((void *) local_file);
      local_file = 0;
    }
    while (0);
    construced_newloc = uri_merge(url, mynewloc);
    do
    {
      free((void *) mynewloc);
      mynewloc = 0;
    }
    while (0);
    mynewloc = construced_newloc;
    newloc_parsed = url_parse(mynewloc, &up_error_code, iri, 1);
    if (!newloc_parsed)
    {
      char *error = url_error(mynewloc, up_error_code);
      logprintf(LOG_NOTQUIET, "%s: %s.\n", escnonprint_uri(mynewloc), error);
      if (orig_parsed != u)
      {
        url_free(u);
      }
      do
      {
        free((void *) url);
        url = 0;
      }
      while (0);
      do
      {
        free((void *) mynewloc);
        mynewloc = 0;
      }
      while (0);
      do
      {
        free((void *) error);
        error = 0;
      }
      while (0);
      do
      {
        if (method_suspended)
        {
          set_opt_body_data_wrapper(saved_body_data);
          set_opt_body_file_wrapper(saved_body_file_name);
          set_opt_method_wrapper(saved_method);
          method_suspended = 0;
        }
      }
      while (0);
      goto bail;
    }
    do
    {
      free((void *) mynewloc);
      mynewloc = 0;
    }
    while (0);
    mynewloc = xstrdup(newloc_parsed->url);
    if ((++redirection_count) > get_opt_max_redirect_wrapper())
    {
      logprintf(LOG_NOTQUIET, gettext("%d redirections exceeded.\n"), get_opt_max_redirect_wrapper());
      url_free(newloc_parsed);
      if (orig_parsed != u)
      {
        url_free(u);
      }
      do
      {
        free((void *) url);
        url = 0;
      }
      while (0);
      do
      {
        free((void *) mynewloc);
        mynewloc = 0;
      }
      while (0);
      do
      {
        if (method_suspended)
        {
          set_opt_body_data_wrapper(saved_body_data);
          set_opt_body_file_wrapper(saved_body_file_name);
          set_opt_method_wrapper(saved_method);
          method_suspended = 0;
        }
      }
      while (0);
      result = WRONGCODE;
      goto bail;
    }
    do
    {
      free((void *) url);
      url = 0;
    }
    while (0);
    url = mynewloc;
    if (orig_parsed != u)
    {
      url_free(u);
    }
    u = newloc_parsed;
    if ((result != NEWLOCATION_KEEP_POST) && (!method_suspended))
      do
    {
      method_suspended = 1;
      saved_body_data = get_opt_body_data_wrapper();
      saved_body_file_name = get_opt_body_file_wrapper();
      saved_method = get_opt_method_wrapper();
      set_opt_body_data_wrapper(0);
      set_opt_body_file_wrapper(0);
      set_opt_method_wrapper(0);
    }
    while (0);
    goto redirected;
  }
  else
  {
    do
    {
      free((void *) mynewloc);
      mynewloc = 0;
    }
    while (0);
  }
  if ((!((*dt) & RETROKF)) && iri->utf8_encode)
  {
    iri->utf8_encode = 0;
    if (orig_parsed != u)
    {
      url_free(u);
    }
    u = url_parse(origurl, 0, iri, 1);
    if (u)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("[IRI fallbacking to non-utf8 for %s\n", quote(url));
        }
      }
      while (0);
      do
      {
        free((void *) url);
        url = 0;
      }
      while (0);
      url = xstrdup(u->url);
      iri_fallbacked = 1;
      goto redirected;
    }
    else
      do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("[Couldn't fallback to non-utf8 for %s\n", quote(url));
      }
    }
    while (0);
  }
  if ((local_file && u) && (((*dt) & RETROKF) || get_opt_content_on_error_wrapper()))
  {
    register_download(u->url, local_file);
    if (((!get_opt_spider_wrapper()) && redirection_count) && (0 != strcmp(origurl, u->url)))
      register_redirection(origurl, u->url);
    if ((*dt) & TEXTHTML)
      register_html(local_file);
    if ((*dt) & TEXTCSS)
      register_css(local_file);
  }
  if (file)
    *file = (local_file) ? (local_file) : (0);
  else
    do
  {
    free((void *) local_file);
    local_file = 0;
  }
  while (0);
  if (orig_parsed != u)
  {
    url_free(u);
  }
  if (redirection_count || iri_fallbacked)
  {
    if (newloc)
      *newloc = url;
    else
      do
    {
      free((void *) url);
      url = 0;
    }
    while (0);
  }
  else
  {
    if (newloc)
      *newloc = 0;
    do
    {
      free((void *) url);
      url = 0;
    }
    while (0);
  }
  do
  {
    if (method_suspended)
    {
      set_opt_body_data_wrapper(saved_body_data);
      set_opt_body_file_wrapper(saved_body_file_name);
      set_opt_method_wrapper(saved_method);
      method_suspended = 0;
    }
  }
  while (0);
  bail:
  if (register_status)
    inform_exit_status(result);

  return result;
}



/* Find the URLs in the file and call retrieve_url() for each of them.
   If HTML is true, treat the file as HTML, and construct the URLs
   accordingly.

   If opt.recursive is set, call retrieve_tree() for each file.  */

uerr_t retrieve_from_file(const char *file, bool html, int *count)
{
  uerr_t status;
  struct urlpos *url_list;
  struct urlpos *cur_url;
  struct iri *iri = &get_dummy_iri_wrapper();
  char *input_file;
  char *url_file = 0;
  const char *url = file;
  status = RETROK;
  *count = 0;
  ;
  ;
  if (url_valid_scheme(url))
  {
    int dt;
    int url_err;
    struct url *url_parsed = url_parse(url, &url_err, iri, 1);
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
      ;
      return URLERROR;
    }
    if (!get_opt_base_href_wrapper())
      set_opt_base_href_wrapper(xstrdup(url));
    status = retrieve_url(url_parsed, url, &url_file, 0, 0, &dt, 0, iri, 1);
    url_free(url_parsed);
    if ((!url_file) || (status != RETROK))
      return status;
    if (dt & TEXTHTML)
      html = 1;
    iri->utf8_encode = get_opt_enable_iri_wrapper();
    do
    {
      free((void *) iri->orig_url);
      iri->orig_url = 0;
    }
    while (0);
    input_file = url_file;
  }
  else
    input_file = (char *) file;
  url_list = (html) ? (get_urls_html(input_file, 0, 0, iri)) : (get_urls_file(input_file));
  do
  {
    free((void *) url_file);
    url_file = 0;
  }
  while (0);
  for (cur_url = url_list; cur_url; cur_url = cur_url->next, ++(*count))
  {
    char *filename = 0;
    char *new_file = 0;
    char *proxy;
    int dt = 0;
    struct iri *tmpiri = &get_dummy_iri_wrapper();
    struct url *parsed_url = 0;
    if (cur_url->ignore_when_downloading)
      continue;
    if (get_opt_quota_wrapper() && (get_total_downloaded_bytes_wrapper() > get_opt_quota_wrapper()))
    {
      status = QUOTEXC;
      break;
    }
    parsed_url = url_parse(cur_url->url->url, 0, tmpiri, 1);
    proxy = getproxy(cur_url->url);
    if ((get_opt_recursive_wrapper() || get_opt_page_requisites_wrapper()) && (((cur_url->url->scheme != SCHEME_FTP) && (cur_url->url->scheme != SCHEME_FTPS)) || proxy))
    {
      int old_follow_ftp = get_opt_follow_ftp_wrapper();
      if ((cur_url->url->scheme == SCHEME_FTP) || (cur_url->url->scheme == SCHEME_FTPS))
        set_opt_follow_ftp_wrapper(1);
      status = retrieve_tree((parsed_url) ? (parsed_url) : (cur_url->url), tmpiri);
      set_opt_follow_ftp_wrapper(old_follow_ftp);
    }
    else
      status = retrieve_url((parsed_url) ? (parsed_url) : (cur_url->url), cur_url->url->url, &filename, &new_file, 0, &dt, get_opt_recursive_wrapper(), tmpiri, 1);
    do
    {
      free((void *) proxy);
      proxy = 0;
    }
    while (0);
    if (parsed_url)
      url_free(parsed_url);
    if ((filename && get_opt_delete_after_wrapper()) && file_exists_p(filename))
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Removing file due to --delete-after in retrieve_from_file():\n");
        }
      }
      while (0);
      logprintf(LOG_VERBOSE, gettext("Removing %s.\n"), filename);
      if (unlink(filename))
        logprintf(LOG_NOTQUIET, "unlink: %s\n", strerror(errno));
      dt &= ~RETROKF;
    }
    do
    {
      free((void *) new_file);
      new_file = 0;
    }
    while (0);
    do
    {
      free((void *) filename);
      filename = 0;
    }
    while (0);
    ;
  }

  free_urlpos(url_list);
  ;
  return status;
}



/* Print `giving up', or `retrying', depending on the impending
   action.  N1 and N2 are the attempt number and the attempt limit.  */
void
printwhat (int n1, int n2)
{
  logputs (LOG_VERBOSE, (n1 == n2) ? _("Giving up.\n\n") : _("Retrying.\n\n"));
}

/* If opt.wait or opt.waitretry are specified, and if certain
   conditions are met, sleep the appropriate number of seconds.  See
   the documentation of --wait and --waitretry for more information.

   COUNT is the count of current retrieval, beginning with 1. */

void sleep_between_retrievals(int count)
{
  static bool first_retrieval = 1;
  if (first_retrieval)
  {
    first_retrieval = 0;
    return;
  }
  if (get_opt_waitretry_wrapper() && (count > 1))
  {
    if (count <= get_opt_waitretry_wrapper())
      xsleep(count - 1);
    else
      xsleep(get_opt_waitretry_wrapper());
  }
  else
    if (get_opt_wait_wrapper())
  {
    if ((!get_opt_random_wait_wrapper()) || (count > 1))
      xsleep(get_opt_wait_wrapper());
    else
    {
      double waitsecs = (0.5 + random_float()) * get_opt_wait_wrapper();
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("sleep_between_retrievals: avg=%f,sleep=%f\n", get_opt_wait_wrapper(), waitsecs);
        }
      }
      while (0);
      xsleep(waitsecs);
    }
  }
}



/* Free the linked list of urlpos.  */
void
free_urlpos (struct urlpos *l)
{
  while (l)
    {
      struct urlpos *next = l->next;
      if (l->url)
        url_free (l->url);
      xfree (l->local_name);
      xfree (l);
      l = next;
    }
}

/* Rotate FNAME opt.backups times */
void rotate_backups(const char *fname)
{
  int maxlen = ((strlen(fname) + (sizeof("."))) + numdigit(get_opt_backups_wrapper())) + 0;
  char *from = (char *) alloca(maxlen);
  char *to = (char *) alloca(maxlen);
  struct stat sb;
  int i;
  if (stat(fname, &sb) == 0)
    if (S_ISREG(sb.st_mode) == 0)
    return;
  for (i = get_opt_backups_wrapper(); i > 1; i--)
  {
    sprintf(to, "%s%s%d", fname, ".", i);
    sprintf(from, "%s%s%d", fname, ".", i - 1);
    rename(from, to);
  }

  sprintf(to, "%s%s%d", fname, ".", 1);
  rename(fname, to);
}



static bool no_proxy_match (const char *, const char **);

/* Return the URL of the proxy appropriate for url U.  */

static char *getproxy(struct url *u)
{
  char *proxy = 0;
  char *rewritten_url;
  if (!get_opt_use_proxy_wrapper())
    return 0;
  if (no_proxy_match(u->host, (const char **) get_opt_no_proxy_wrapper()))
    return 0;
  switch (u->scheme)
  {
    case SCHEME_HTTP:
      proxy = (get_opt_http_proxy_wrapper()) ? (get_opt_http_proxy_wrapper()) : (getenv("http_proxy"));
      break;

    case SCHEME_HTTPS:
      proxy = (get_opt_https_proxy_wrapper()) ? (get_opt_https_proxy_wrapper()) : (getenv("https_proxy"));
      break;

    case SCHEME_FTPS:
      proxy = (get_opt_ftp_proxy_wrapper()) ? (get_opt_ftp_proxy_wrapper()) : (getenv("ftps_proxy"));
      break;

    case SCHEME_FTP:
      proxy = (get_opt_ftp_proxy_wrapper()) ? (get_opt_ftp_proxy_wrapper()) : (getenv("ftp_proxy"));
      break;

    case SCHEME_INVALID:
      break;

  }

  if ((!proxy) || (!(*proxy)))
    return 0;
  rewritten_url = rewrite_shorthand_url(proxy);
  if (rewritten_url)
    return rewritten_url;
  return strdup(proxy);
}



/* Returns true if URL would be downloaded through a proxy. */

bool
url_uses_proxy (struct url * u)
{
  bool ret;
  char *proxy;

  if (!u)
    return false;
  proxy = getproxy (u);
  ret = proxy != NULL;
  xfree (proxy);
  return ret;
}

/* Should a host be accessed through proxy, concerning no_proxy?  */
static bool
no_proxy_match (const char *host, const char **no_proxy)
{
  if (!no_proxy)
    return false;
  else
    return sufmatch (no_proxy, host);
}

/* Set the file parameter to point to the local file string.  */
void set_local_file(const char **file, const char *default_file)
{
  if (get_opt_output_document_wrapper())
  {
    if (get_output_stream_regular_wrapper())
      *file = get_opt_output_document_wrapper();
  }
  else
    *file = default_file;
}



/* Return true for an input file's own URL, false otherwise.  */
bool
input_file_url (const char *input_file)
{
  static bool first = true;

  if (input_file
      && url_has_scheme (input_file)
      && first)
    {
      first = false;
      return true;
    }
  else
    return false;
}
