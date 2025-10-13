/* Download progress.
   Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
   2010, 2011, 2015 Free Software Foundation, Inc.

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
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <wchar.h>
#include <mbiter.h>

#include "progress.h"
#include "utils.h"
#include "retr.h"
#include "c-strcase.h"

#include "wget_rpc_wrapper.h"

struct progress_implementation {
  const char *name;
  bool interactive;
  void *(*create) (const char *, wgint, wgint);
  void (*update) (void *, wgint, double);
  void (*draw) (void *);
  void (*finish) (void *, double);
  void (*set_params) (char *);
};

/* Necessary forward declarations. */

static void *dot_create (const char *, wgint, wgint);
static void dot_update (void *, wgint, double);
static void dot_finish (void *, double);
static void dot_draw (void *);
static void dot_set_params (char *);

static void *bar_create (const char *, wgint, wgint);
static void bar_update (void *, wgint, double);
static void bar_draw (void *);
static void bar_finish (void *, double);
static void bar_set_params (char *);

static struct progress_implementation implementations[] = {
  { "dot", 0, dot_create, dot_update, dot_draw, dot_finish, dot_set_params },
  { "bar", 1, bar_create, bar_update, bar_draw, bar_finish, bar_set_params }
};
static struct progress_implementation *current_impl;
static int current_impl_locked;

/* Progress implementation used by default.  Can be overriden in
   wgetrc or by the fallback one.  */

#define DEFAULT_PROGRESS_IMPLEMENTATION "bar"

/* Fallback progress implementation should be something that works
   under all display types.  If you put something other than "dot"
   here, remember that bar_set_params tries to switch to this if we're
   not running on a TTY.  So changing this to "bar" could cause
   infloop.  */

#define FALLBACK_PROGRESS_IMPLEMENTATION "dot"

/* Return true if NAME names a valid progress bar implementation.  The
   characters after the first : will be ignored.  */

bool
valid_progress_implementation_p (const char *name)
{
  size_t i;
  struct progress_implementation *pi = implementations;
  char *colon = strchr (name, ':');
  size_t namelen = colon ? (size_t) (colon - name) : strlen (name);

  for (i = 0; i < countof (implementations); i++, pi++)
    if (!strncmp (pi->name, name, namelen))
      return true;
  return false;
}

/* Set the progress implementation to NAME.  */

void set_progress_implementation(const char *name)
{
  size_t i;
  size_t namelen;
  struct progress_implementation *pi = implementations;
  char *colon;
  if (!name)
    name = "bar";
  colon = strchr(name, ':');
  namelen = (colon) ? ((size_t) (colon - name)) : (strlen(name));
  for (i = 0; i < ((sizeof(implementations)) / (sizeof(implementations[0]))); i++, pi++)
    if (!strncmp(pi->name, name, namelen))
  {
    set_current_impl_wrapper(pi);
    set_current_impl_locked_wrapper(0);
    if (colon)
      ++colon;
    if (pi->set_params)
      pi->set_params(colon);
    return;
  }

  abort();
}



static int output_redirected;

void progress_schedule_redirect(void)
{
  set_output_redirected_wrapper(1);
}



/* Create a progress gauge.  INITIAL is the number of bytes the
   download starts from (zero if the download starts from scratch).
   TOTAL is the expected total number of bytes in this download.  If
   TOTAL is zero, it means that the download size is not known in
   advance.  */

void *progress_create(const char *f_download, wgint initial, wgint total)
{
  if (get_output_redirected_wrapper())
  {
    if (!get_current_impl_locked_wrapper())
      set_progress_implementation("dot");
    set_output_redirected_wrapper(0);
  }
  return get_current_impl_create_wrapper()(f_download, initial, total);
}



/* Return true if the progress gauge is "interactive", i.e. if it can
   profit from being called regularly even in absence of data.  The
   progress bar is interactive because it regularly updates the ETA
   and current update.  */

bool progress_interactive_p(void *progress)
{
  return get_current_impl_interactive_wrapper();
}



/* Inform the progress gauge of newly received bytes.  DLTIME is the
   time since the beginning of the download.  */

void progress_update(void *progress, wgint howmuch, double dltime)
{
  get_current_impl_update_wrapper()(progress, howmuch, dltime);
  get_current_impl_draw_wrapper()(progress);
}



/* Tell the progress gauge to clean up.  Calling this will free the
   PROGRESS object, the further use of which is not allowed.  */

void progress_finish(void *progress, double dltime)
{
  get_current_impl_finish_wrapper()(progress, dltime);
}



/* Dot-printing. */

struct dot_progress {
  wgint initial_length;         /* how many bytes have been downloaded
                                   previously. */
  wgint total_length;           /* expected total byte count when the
                                   download finishes */

  int accumulated;              /* number of bytes accumulated after
                                   the last printed dot */

  double dltime;                /* download time so far */
  int rows;                     /* number of rows printed so far */
  int dots;                     /* number of dots printed in this row */

  double last_timer_value;
};

/* Dot-progress backend for progress_create. */

static void *dot_create(const char *f_download, wgint initial, wgint total)
{
  struct dot_progress *dp = xcalloc(1, sizeof(struct dot_progress));
  dp->initial_length = initial;
  dp->total_length = total;
  if (dp->initial_length)
  {
    int dot_bytes = get_opt_dot_bytes_wrapper();
    const wgint ROW_BYTES = get_opt_dot_bytes_wrapper() * get_opt_dots_in_line_wrapper();
    int remainder = dp->initial_length % ROW_BYTES;
    wgint skipped = dp->initial_length - remainder;
    if (skipped)
    {
      wgint skipped_k = skipped / 1024;
      int skipped_k_len = numdigit(skipped_k);
      if (skipped_k_len < 6)
        skipped_k_len = 6;
      logprintf(LOG_PROGRESS, gettext("\n%*s[ skipping %sK ]"), 2 + skipped_k_len, "", number_to_static_string(skipped_k));
    }
    logprintf(LOG_PROGRESS, "\n%6sK", number_to_static_string(skipped / 1024));
    for (; remainder >= dot_bytes; remainder -= dot_bytes)
    {
      if ((dp->dots % get_opt_dot_spacing_wrapper()) == 0)
        logputs(LOG_PROGRESS, " ");
      logputs(LOG_PROGRESS, ",");
      ++dp->dots;
    }

    assert(dp->dots < get_opt_dots_in_line_wrapper());
    dp->accumulated = remainder;
    dp->rows = skipped / ROW_BYTES;
  }
  return dp;
}



static const char *eta_to_human_short (int, bool);

/* Prints the stats (percentage of completion, speed, ETA) for current
   row.  DLTIME is the time spent downloading the data in current
   row.

   #### This function is somewhat uglified by the fact that current
   row and last row have somewhat different stats requirements.  It
   might be worthwhile to split it to two different functions.  */

static void print_row_stats(struct dot_progress *dp, double dltime, bool last)
{
  const wgint ROW_BYTES = get_opt_dot_bytes_wrapper() * get_opt_dots_in_line_wrapper();
  wgint bytes_displayed = (dp->rows * ROW_BYTES) + (dp->dots * get_opt_dot_bytes_wrapper());
  if (last)
    bytes_displayed += dp->accumulated;
  if (dp->total_length)
  {
    int percentage = (100.0 * bytes_displayed) / dp->total_length;
    logprintf(LOG_PROGRESS, "%3d%%", percentage);
  }
  {
    static char names[] = {' ', 'K', 'M', 'G'};
    int units;
    double rate;
    wgint bytes_this_row;
    if (!last)
      bytes_this_row = ROW_BYTES;
    else
      bytes_this_row = (dp->dots * get_opt_dot_bytes_wrapper()) + dp->accumulated;
    if (dp->rows == (dp->initial_length / ROW_BYTES))
      bytes_this_row -= dp->initial_length % ROW_BYTES;
    rate = calc_rate(bytes_this_row, dltime - dp->last_timer_value, &units);
    logprintf(LOG_PROGRESS, " %4.*f%c", (rate >= 99.95) ? (0) : ((rate >= 9.995) ? (1) : (2)), rate, names[units]);
    dp->last_timer_value = dltime;
  }
  if (!last)
  {
    if (dp->total_length)
    {
      wgint bytes_remaining = dp->total_length - bytes_displayed;
      wgint bytes_sofar = bytes_displayed - dp->initial_length;
      double eta = (dltime * bytes_remaining) / bytes_sofar;
      if (eta < (2147483647 - 1))
        logprintf(LOG_PROGRESS, " %s", eta_to_human_short((int) (eta + 0.5), 1));
    }
  }
  else
  {
    if (dltime >= 10)
      logprintf(LOG_PROGRESS, "=%s", eta_to_human_short((int) (dltime + 0.5), 1));
    else
      logprintf(LOG_PROGRESS, "=%ss", print_decimal(dltime));
  }
}



/* Dot-progress backend for progress_update. */

static void
dot_update (void *progress, wgint howmuch, double dltime)
{
  struct dot_progress *dp = progress;
  dp->accumulated += howmuch;
  dp->dltime = dltime;
}

static void dot_draw(void *progress)
{
  struct dot_progress *dp = progress;
  int dot_bytes = get_opt_dot_bytes_wrapper();
  wgint ROW_BYTES = get_opt_dot_bytes_wrapper() * get_opt_dots_in_line_wrapper();
  log_set_flush(0);
  for (; dp->accumulated >= dot_bytes; dp->accumulated -= dot_bytes)
  {
    if (dp->dots == 0)
      logprintf(LOG_PROGRESS, "\n%6sK", number_to_static_string((dp->rows * ROW_BYTES) / 1024));
    if ((dp->dots % get_opt_dot_spacing_wrapper()) == 0)
      logputs(LOG_PROGRESS, " ");
    logputs(LOG_PROGRESS, ".");
    ++dp->dots;
    if (dp->dots >= get_opt_dots_in_line_wrapper())
    {
      ++dp->rows;
      dp->dots = 0;
      print_row_stats(dp, dp->dltime, 0);
    }
  }

  log_set_flush(1);
}



/* Dot-progress backend for progress_finish. */

static void dot_finish(void *progress, double dltime)
{
  struct dot_progress *dp = progress;
  wgint ROW_BYTES = get_opt_dot_bytes_wrapper() * get_opt_dots_in_line_wrapper();
  int i;
  log_set_flush(0);
  if (dp->dots == 0)
    logprintf(LOG_PROGRESS, "\n%6sK", number_to_static_string((dp->rows * ROW_BYTES) / 1024));
  for (i = dp->dots; i < get_opt_dots_in_line_wrapper(); i++)
  {
    if ((i % get_opt_dot_spacing_wrapper()) == 0)
      logputs(LOG_PROGRESS, " ");
    logputs(LOG_PROGRESS, " ");
  }

  print_row_stats(dp, dltime, 1);
  logputs(LOG_VERBOSE, "\n\n");
  log_set_flush(0);
  do
  {
    free((void *) dp);
    dp = 0;
  }
  while (0);
}



/* This function interprets the progress "parameters".  For example,
   if Wget is invoked with --progress=dot:mega, it will set the
   "dot-style" to "mega".  Valid styles are default, binary, mega, and
   giga.  */

static void dot_set_params(char *params)
{
  if ((!params) || (!(*params)))
    params = get_opt_dot_style_wrapper();
  if (!params)
    return;
  if (!c_strcasecmp(params, "default"))
  {
    set_opt_dot_bytes_wrapper(1024);
    set_opt_dot_spacing_wrapper(10);
    set_opt_dots_in_line_wrapper(50);
  }
  else
    if (!c_strcasecmp(params, "binary"))
  {
    set_opt_dot_bytes_wrapper(8192);
    set_opt_dot_spacing_wrapper(16);
    set_opt_dots_in_line_wrapper(48);
  }
  else
    if (!c_strcasecmp(params, "mega"))
  {
    set_opt_dot_bytes_wrapper(65536L);
    set_opt_dot_spacing_wrapper(8);
    set_opt_dots_in_line_wrapper(48);
  }
  else
    if (!c_strcasecmp(params, "giga"))
  {
    set_opt_dot_bytes_wrapper(1L << 20);
    set_opt_dot_spacing_wrapper(8);
    set_opt_dots_in_line_wrapper(32);
  }
  else
    fprintf(stderr, gettext("Invalid dot style specification %s; leaving unchanged.\n"), quote(params));
}



/* "Thermometer" (bar) progress. */

/* Assumed screen width if we can't find the real value.  */
#define DEFAULT_SCREEN_WIDTH 80

/* Minimum screen width we'll try to work with.  If this is too small,
   create_image will overflow the buffer.  */
#define MINIMUM_SCREEN_WIDTH 45

/* The last known screen width.  This can be updated by the code that
   detects that SIGWINCH was received (but it's never updated from the
   signal handler).  */
static int screen_width;

/* A flag that, when set, means SIGWINCH was received.  */
static volatile sig_atomic_t received_sigwinch;

/* Size of the download speed history ring. */
#define DLSPEED_HISTORY_SIZE 20

/* The minimum time length of a history sample.  By default, each
   sample is at least 150ms long, which means that, over the course of
   20 samples, "current" download speed spans at least 3s into the
   past.  */
#define DLSPEED_SAMPLE_MIN 0.15

/* The time after which the download starts to be considered
   "stalled", i.e. the current bandwidth is not printed and the recent
   download speeds are scratched.  */
#define STALL_START_TIME 5

/* Time between screen refreshes will not be shorter than this, so
   that Wget doesn't swamp the TTY with output.  */
#define REFRESH_INTERVAL 0.2

/* Don't refresh the ETA too often to avoid jerkiness in predictions.
   This allows ETA to change approximately once per second.  */
#define ETA_REFRESH_INTERVAL 0.99

struct bar_progress {
  const char *f_download;       /* Filename of the downloaded file */
  wgint initial_length;         /* how many bytes have been downloaded
                                   previously. */
  wgint total_length;           /* expected total byte count when the
                                   download finishes */
  wgint count;                  /* bytes downloaded so far */

  double last_screen_update;    /* time of the last screen update,
                                   measured since the beginning of
                                   download. */

  double dltime;                /* download time so far */
  int width;                    /* screen width we're using at the
                                   time the progress gauge was
                                   created.  this is different from
                                   the screen_width global variable in
                                   that the latter can be changed by a
                                   signal. */
  char *buffer;                 /* buffer where the bar "image" is
                                   stored. */
  int tick;                     /* counter used for drawing the
                                   progress bar where the total size
                                   is not known. */

  /* The following variables (kept in a struct for namespace reasons)
     keep track of recent download speeds.  See bar_update() for
     details.  */
  struct bar_progress_hist {
    int pos;
    double times[DLSPEED_HISTORY_SIZE];
    wgint bytes[DLSPEED_HISTORY_SIZE];

    /* The sum of times and bytes respectively, maintained for
       efficiency. */
    double total_time;
    wgint total_bytes;
  } hist;

  double recent_start;          /* timestamp of beginning of current
                                   position. */
  wgint recent_bytes;           /* bytes downloaded so far. */

  bool stalled;                 /* set when no data arrives for longer
                                   than STALL_START_TIME, then reset
                                   when new data arrives. */

  /* create_image() uses these to make sure that ETA information
     doesn't flicker. */
  double last_eta_time;         /* time of the last update to download
                                   speed and ETA, measured since the
                                   beginning of download. */
  int last_eta_value;
};

static void create_image (struct bar_progress *, double, bool);
static void display_image (char *);

static void *bar_create(const char *f_download, wgint initial, wgint total)
{
  struct bar_progress *bp = xcalloc(1, sizeof(struct bar_progress));
  if (initial > total)
    total = initial;
  bp->initial_length = initial;
  bp->total_length = total;
  bp->f_download = f_download;
  if ((!get_screen_width_wrapper()) || get_received_sigwinch_wrapper())
  {
    set_screen_width_wrapper(determine_screen_width());
    if (!get_screen_width_wrapper())
      set_screen_width_wrapper(80);
    else
      if (get_screen_width_wrapper() < 45)
      set_screen_width_wrapper(45);
    set_received_sigwinch_wrapper(0);
  }
  bp->width = get_screen_width_wrapper() - 1;
  bp->buffer = xmalloc(bp->width + 100);
  logputs(LOG_VERBOSE, "\n");
  create_image(bp, 0, 0);
  display_image(bp->buffer);
  return bp;
}



static void update_speed_ring (struct bar_progress *, wgint, double);

static void
bar_update (void *progress, wgint howmuch, double dltime)
{
  struct bar_progress *bp = progress;

  bp->dltime = dltime;
  bp->count += howmuch;
  if (bp->total_length > 0
      && bp->count + bp->initial_length > bp->total_length)
    /* We could be downloading more than total_length, e.g. when the
       server sends an incorrect Content-Length header.  In that case,
       adjust bp->total_length to the new reality, so that the code in
       create_image() that depends on total size being smaller or
       equal to the expected size doesn't abort.  */
    bp->total_length = bp->initial_length + bp->count;

  update_speed_ring (bp, howmuch, dltime);
}

static void bar_draw(void *progress)
{
  bool force_screen_update = 0;
  struct bar_progress *bp = progress;
  if (get_received_sigwinch_wrapper())
  {
    int old_width = get_screen_width_wrapper();
    set_screen_width_wrapper(determine_screen_width());
    if (!get_screen_width_wrapper())
      set_screen_width_wrapper(80);
    else
      if (get_screen_width_wrapper() < 45)
      set_screen_width_wrapper(45);
    if (get_screen_width_wrapper() != old_width)
    {
      bp->width = get_screen_width_wrapper() - 1;
      bp->buffer = xrealloc(bp->buffer, bp->width + 100);
      force_screen_update = 1;
    }
    set_received_sigwinch_wrapper(0);
  }
  if (((bp->dltime - bp->last_screen_update) < 0.2) && (!force_screen_update))
    return;
  create_image(bp, bp->dltime, 0);
  display_image(bp->buffer);
  bp->last_screen_update = bp->dltime;
}



static void
bar_finish (void *progress, double dltime)
{
  struct bar_progress *bp = progress;

  if (bp->total_length > 0
      && bp->count + bp->initial_length > bp->total_length)
    /* See bar_update() for explanation. */
    bp->total_length = bp->initial_length + bp->count;

  create_image (bp, dltime, true);
  display_image (bp->buffer);

  logputs (LOG_VERBOSE, "\n");
  logputs (LOG_PROGRESS, "\n");

  xfree (bp->buffer);
  xfree (bp);
}

/* This code attempts to maintain the notion of a "current" download
   speed, over the course of no less than 3s.  (Shorter intervals
   produce very erratic results.)

   To do so, it samples the speed in 150ms intervals and stores the
   recorded samples in a FIFO history ring.  The ring stores no more
   than 20 intervals, hence the history covers the period of at least
   three seconds and at most 20 reads into the past.  This method
   should produce reasonable results for downloads ranging from very
   slow to very fast.

   The idea is that for fast downloads, we get the speed over exactly
   the last three seconds.  For slow downloads (where a network read
   takes more than 150ms to complete), we get the speed over a larger
   time period, as large as it takes to complete twenty reads.  This
   is good because slow downloads tend to fluctuate more and a
   3-second average would be too erratic.  */

static void
update_speed_ring (struct bar_progress *bp, wgint howmuch, double dltime)
{
  struct bar_progress_hist *hist = &bp->hist;
  double recent_age = dltime - bp->recent_start;

  /* Update the download count. */
  bp->recent_bytes += howmuch;

  /* For very small time intervals, we return after having updated the
     "recent" download count.  When its age reaches or exceeds minimum
     sample time, it will be recorded in the history ring.  */
  if (recent_age < DLSPEED_SAMPLE_MIN)
    return;

  if (howmuch == 0)
    {
      /* If we're not downloading anything, we might be stalling,
         i.e. not downloading anything for an extended period of time.
         Since 0-reads do not enter the history ring, recent_age
         effectively measures the time since last read.  */
      if (recent_age >= STALL_START_TIME)
        {
          /* If we're stalling, reset the ring contents because it's
             stale and because it will make bar_update stop printing
             the (bogus) current bandwidth.  */
          bp->stalled = true;
          xzero (*hist);
          bp->recent_bytes = 0;
        }
      return;
    }

  /* We now have a non-zero amount of to store to the speed ring.  */

  /* If the stall status was acquired, reset it. */
  if (bp->stalled)
    {
      bp->stalled = false;
      /* "recent_age" includes the entired stalled period, which
         could be very long.  Don't update the speed ring with that
         value because the current bandwidth would start too small.
         Start with an arbitrary (but more reasonable) time value and
         let it level out.  */
      recent_age = 1;
    }

  /* Store "recent" bytes and download time to history ring at the
     position POS.  */

  /* To correctly maintain the totals, first invalidate existing data
     (least recent in time) at this position. */
  hist->total_time  -= hist->times[hist->pos];
  hist->total_bytes -= hist->bytes[hist->pos];

  /* Now store the new data and update the totals. */
  hist->times[hist->pos] = recent_age;
  hist->bytes[hist->pos] = bp->recent_bytes;
  hist->total_time  += recent_age;
  hist->total_bytes += bp->recent_bytes;

  /* Start a new "recent" period. */
  bp->recent_start = dltime;
  bp->recent_bytes = 0;

  /* Advance the current ring position. */
  if (++hist->pos == DLSPEED_HISTORY_SIZE)
    hist->pos = 0;

#if 0
  /* Sledgehammer check to verify that the totals are accurate. */
  {
    int i;
    double sumt = 0, sumb = 0;
    for (i = 0; i < DLSPEED_HISTORY_SIZE; i++)
      {
        sumt += hist->times[i];
        sumb += hist->bytes[i];
      }
    assert (sumb == hist->total_bytes);
    /* We can't use assert(sumt==hist->total_time) because some
       precision is lost by adding and subtracting floating-point
       numbers.  But during a download this precision should not be
       detectable, i.e. no larger than 1ns.  */
    double diff = sumt - hist->total_time;
    if (diff < 0) diff = -diff;
    assert (diff < 1e-9);
  }
#endif
}

#if USE_NLS_PROGRESS_BAR
static int
count_cols (const char *mbs)
{
  wchar_t wc;
  int     bytes;
  int     remaining = strlen(mbs);
  int     cols = 0;
  int     wccols;

  while (*mbs != '\0')
    {
      bytes = mbtowc (&wc, mbs, remaining);
      assert (bytes != 0);  /* Only happens when *mbs == '\0' */
      if (bytes == -1)
        {
          /* Invalid sequence. We'll just have to fudge it. */
          return cols + remaining;
        }
      mbs += bytes;
      remaining -= bytes;
      wccols = wcwidth(wc);
      cols += (wccols == -1? 1 : wccols);
    }
  return cols;
}

static int
cols_to_bytes (const char *mbs, const int cols, int *ncols)
{
  int p_cols = 0, bytes = 0;
  mbchar_t mbc;
  mbi_iterator_t iter;
  mbi_init (iter, mbs, strlen(mbs));
  while (p_cols < cols && mbi_avail (iter))
    {
      mbc = mbi_cur (iter);
      p_cols += mb_width (mbc);
      /* The multibyte character has exceeded the total number of columns we
       * have available. The remaining bytes will be padded with a space. */
      if (p_cols > cols)
        {
          p_cols -= mb_width (mbc);
          break;
        }
      bytes += mb_len (mbc);
      mbi_advance (iter);
    }
  *ncols = p_cols;
  return bytes;
}
#else
static int count_cols (const char *mbs) { return (int) strlen(mbs); }
static int
cols_to_bytes (const char *mbs _GL_UNUSED, const int cols, int *ncols)
{
  *ncols = cols;
  return cols;
}
#endif

static const char *
get_eta (int *bcd)
{
  /* TRANSLATORS: "ETA" is English-centric, but this must
     be short, ideally 3 chars.  Abbreviate if necessary.  */
  static const char eta_str[] = N_("    eta %s");
  static const char *eta_trans;
  static int bytes_cols_diff;
  if (eta_trans == NULL)
    {
      int nbytes;
      int ncols;

#if USE_NLS_PROGRESS_BAR
      eta_trans = _(eta_str);
#else
      eta_trans = eta_str;
#endif

      /* Determine the number of bytes used in the translated string,
       * versus the number of columns used. This is to figure out how
       * many spaces to add at the end to pad to the full line width.
       *
       * We'll store the difference between the number of bytes and
       * number of columns, so that removing this from the string length
       * will reveal the total number of columns in the progress bar. */
      nbytes = strlen (eta_trans);
      ncols = count_cols (eta_trans);
      bytes_cols_diff = nbytes - ncols;
    }

  if (bcd != NULL)
    *bcd = bytes_cols_diff;

  return eta_trans;
}

#define APPEND_LITERAL(s) do {                  \
  memcpy (p, s, sizeof (s) - 1);                \
  p += sizeof (s) - 1;                          \
} while (0)

/* Use move_to_end (s) to get S to point the end of the string (the
   terminating \0).  This is faster than s+=strlen(s), but some people
   are confused when they see strchr (s, '\0') in the code.  */
#define move_to_end(s) s = strchr (s, '\0');

static void create_image(struct bar_progress *bp, double dl_total_time, bool done)
{
  const int MAX_FILENAME_COLS = bp->width / 4;
  char *p = bp->buffer;
  wgint size = bp->initial_length + bp->count;
  struct bar_progress_hist *hist = &bp->hist;
  int orig_filename_cols = count_cols(bp->f_download);
  int padding;
  int progress_size = bp->width - ((((((((MAX_FILENAME_COLS + 1) + 4) + 2) + 7) + 1) + 8) + 2) + 15);
  int bytes_cols_diff = 0;
  int cols_diff;
  const char *down_size;
  memset(bp->buffer, '\0', bp->width + 100);
  if (progress_size < 5)
    progress_size = 0;
  if (orig_filename_cols <= MAX_FILENAME_COLS)
  {
    padding = MAX_FILENAME_COLS - orig_filename_cols;
    p += sprintf(p, "%s ", bp->f_download);
    memset(p, ' ', padding);
    p += padding;
  }
  else
  {
    int offset_cols;
    int bytes_in_filename;
    int offset_bytes;
    int col;
    int *cols_ret = &col;
    if (((orig_filename_cols > (MAX_FILENAME_COLS + 5)) && (!get_opt_noscroll_wrapper())) && (!done))
    {
      offset_cols = ((((int) bp->tick) + orig_filename_cols) + (MAX_FILENAME_COLS / 2)) % (orig_filename_cols + MAX_FILENAME_COLS);
      if (offset_cols > orig_filename_cols)
      {
        padding = MAX_FILENAME_COLS - (offset_cols - orig_filename_cols);
        memset(p, ' ', padding);
        p += padding;
        offset_cols = 0;
      }
      else
        padding = 0;
    }
    else
    {
      padding = 0;
      offset_cols = 0;
    }
    offset_bytes = cols_to_bytes(bp->f_download, offset_cols, cols_ret);
    bytes_in_filename = cols_to_bytes(bp->f_download + offset_bytes, MAX_FILENAME_COLS - padding, cols_ret);
    memcpy(p, bp->f_download + offset_bytes, bytes_in_filename);
    p += bytes_in_filename;
    padding = MAX_FILENAME_COLS - (padding + (*cols_ret));
    memset(p, ' ', padding + 1);
    p += padding + 1;
  }
  if (bp->total_length > 0)
  {
    int percentage = (100.0 * size) / bp->total_length;
    assert(percentage <= 100);
    p += sprintf(p, "%3d%%", percentage);
  }
  else
  {
    memset(p, ' ', 4);
    p += 4;
  }
  if (progress_size && (bp->total_length > 0))
  {
    int insz = (((double) bp->initial_length) / bp->total_length) * progress_size;
    int dlsz = (((double) size) / bp->total_length) * progress_size;
    char *begin;
    assert(dlsz <= progress_size);
    assert(insz <= dlsz);
    *(p++) = '[';
    begin = p;
    memset(p, '+', insz);
    p += insz;
    dlsz -= insz;
    if (dlsz > 0)
    {
      memset(p, '=', dlsz - 1);
      p += dlsz - 1;
      *(p++) = '>';
    }
    memset(p, ' ', progress_size - (p - begin));
    p += progress_size - (p - begin);
    *(p++) = ']';
  }
  else
    if (progress_size)
  {
    int ind = bp->tick % ((progress_size * 2) - 6);
    int i;
    int pos;
    if (ind < (progress_size - 2))
      pos = ind + 1;
    else
      pos = progress_size - ((ind - progress_size) + 5);
    *(p++) = '[';
    for (i = 0; i < progress_size; i++)
    {
      if (i == (pos - 1))
        *(p++) = '<';
      else
        if (i == pos)
        *(p++) = '=';
      else
        if (i == (pos + 1))
        *(p++) = '>';
      else
        *(p++) = ' ';
    }

    *(p++) = ']';
  }
  ++bp->tick;
  down_size = human_readable(size, 1000, 2);
  cols_diff = (7 + 1) - count_cols(down_size);
  memset(p, ' ', cols_diff);
  p += cols_diff;
  p += sprintf(p, "%s", down_size);
  if ((hist->total_time > 0) && hist->total_bytes)
  {
    static const char *short_units[] = {" B/s", "KB/s", "MB/s", "GB/s"};
    static const char *short_units_bits[] = {" b/s", "Kb/s", "Mb/s", "Gb/s"};
    int units = 0;
    wgint dlquant = hist->total_bytes + bp->recent_bytes;
    double dltime = hist->total_time + (dl_total_time - bp->recent_start);
    double dlspeed = calc_rate(dlquant, dltime, &units);
    p += sprintf(p, "  %4.*f%s", (dlspeed >= 99.95) ? (0) : ((dlspeed >= 9.995) ? (1) : (2)), dlspeed, (!get_opt_report_bps_wrapper()) ? (short_units[units]) : (short_units_bits[units]));
  }
  else
    do
  {
    memcpy(p, "  --.-KB/s", (sizeof("  --.-KB/s")) - 1);
    p += (sizeof("  --.-KB/s")) - 1;
  }
  while (0);
  if (!done)
  {
    if (((bp->total_length > 0) && (bp->count > 0)) && (dl_total_time > 3))
    {
      int eta;
      if (((bp->total_length != size) && (bp->last_eta_value != 0)) && ((dl_total_time - bp->last_eta_time) < 0.99))
        eta = bp->last_eta_value;
      else
      {
        wgint bytes_remaining = bp->total_length - size;
        double eta_ = (dl_total_time * bytes_remaining) / bp->count;
        if (eta_ >= (2147483647 - 1))
          goto skip_eta;
        eta = (int) (eta_ + 0.5);
        bp->last_eta_value = eta;
        bp->last_eta_time = dl_total_time;
      }
      p += sprintf(p, get_eta(&bytes_cols_diff), eta_to_human_short(eta, 0));
    }
    else
      if (bp->total_length > 0)
    {
      skip_eta:
      memset(p, ' ', 15);

      p += 15;
    }
  }
  else
  {
    int nbytes;
    int ncols;
    strcpy(p, gettext("    in "));
    nbytes = strlen(p);
    ncols = count_cols(p);
    bytes_cols_diff = nbytes - ncols;
    if (dl_total_time >= 10)
      ncols += sprintf(p + nbytes, "%s", eta_to_human_short((int) (dl_total_time + 0.5), 0));
    else
      ncols += sprintf(p + nbytes, "%ss", print_decimal(dl_total_time));
    p += ncols + bytes_cols_diff;
    memset(p, ' ', 15 - ncols);
    p += 15 - ncols;
  }
  padding = bp->width - count_cols(bp->buffer);
  assert((padding >= 0) && "Padding length became non-positive!");
  padding = (padding > 0) ? (padding) : (0);
  memset(p, ' ', padding);
  p += padding;
  *p = '\0';
  assert(count_cols(bp->buffer) == bp->width);
}



/* Print the contents of the buffer as a one-line ASCII "image" so
   that it can be overwritten next time.  */

static void
display_image (char *buf)
{
  bool old = log_set_save_context (false);
  logputs (LOG_PROGRESS, "\r");
  logputs (LOG_PROGRESS, buf);
  log_set_save_context (old);
}

static void bar_set_params(char *params)
{
  if (params)
  {
    char *param = strtok(params, ":");
    do
    {
      if (0 == strcmp(param, "force"))
        set_current_impl_locked_wrapper(1);
      else
        if (0 == strcmp(param, "noscroll"))
        set_opt_noscroll_wrapper(1);
    }
    while ((param = strtok(0, ":")) != 0);
  }
  if (((get_opt_lfilename_wrapper() && (get_opt_show_progress_wrapper() != 1)) || (!isatty(fileno(stderr)))) && (!get_current_impl_locked_wrapper()))
  {
    set_progress_implementation("dot");
    return;
  }
}



#ifdef SIGWINCH
void progress_handle_sigwinch(int sig)
{
  set_received_sigwinch_wrapper(1);
  signal(28, progress_handle_sigwinch);
}


#endif

/* Provide a short human-readable rendition of the ETA.  This is like
   secs_to_human_time in main.c, except the output doesn't include
   fractions (which would look silly in by nature imprecise ETA) and
   takes less room.  If the time is measured in hours, hours and
   minutes (but not seconds) are shown; if measured in days, then days
   and hours are shown.  This ensures brevity while still displaying
   as much as possible.

   If CONDENSED is true, the separator between minutes and seconds
   (and hours and minutes, etc.) is not included, shortening the
   display by one additional character.  This is used for dot
   progress.

   The display never occupies more than 7 characters of screen
   space.  */

static const char *
eta_to_human_short (int secs, bool condensed)
{
  static char buf[10];          /* 8 should be enough, but just in case */
  static int last = -1;
  const char *space = condensed ? "" : " ";

  /* Trivial optimization.  create_image can call us every 200 msecs
     (see bar_update) for fast downloads, but ETA will only change
     once per 900 msecs.  */
  if (secs == last)
    return buf;
  last = secs;

  if (secs < 100)
    sprintf (buf, "%ds", secs);
  else if (secs < 100 * 60)
    sprintf (buf, "%dm%s%ds", secs / 60, space, secs % 60);
  else if (secs < 48 * 3600)
    sprintf (buf, "%dh%s%dm", secs / 3600, space, (secs / 60) % 60);
  else if (secs < 100 * 86400)
    sprintf (buf, "%dd%s%dh", secs / 86400, space, (secs / 3600) % 24);
  else
    /* even (2^31-1)/86400 doesn't overflow BUF. */
    sprintf (buf, "%dd", secs / 86400);

  return buf;
}
