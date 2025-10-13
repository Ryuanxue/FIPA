/* Command line parsing.
   Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
   2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015 Free
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
#include <unistd.h>
#include <string.h>
#include <signal.h>
#ifdef ENABLE_NLS
# include <locale.h>
#endif
#include <assert.h>
#include <errno.h>
#include <time.h>

#include "exits.h"
#include "utils.h"
#include "init.h"
#include "retr.h"
#include "recur.h"
#include "host.h"
#include "url.h"
#include "progress.h"           /* for progress_handle_sigwinch */
#include "convert.h"
#include "spider.h"
#include "http.h"               /* for save_cookies */
#include "hsts.h"               /* for initializing hsts_store to NULL */
#include "ptimer.h"
#include "warc.h"
#include "version.h"
#include "c-strcase.h"
#include "dirname.h"
#include <getopt.h>
#include <getpass.h>
#include <quote.h>

#ifdef HAVE_METALINK
# include <metalink/metalink_parser.h>
# include "metalink.h"
#endif

#ifdef WINDOWS
# include <io.h>
# include <fcntl.h>
#endif

#ifdef __VMS
# include "vms.h"
#endif /* __VMS */

#ifndef PATH_SEPARATOR
# define PATH_SEPARATOR '/'
#endif

#ifndef ENABLE_IRI
struct iri dummy_iri;
#endif

#ifdef HAVE_LIBCARES
#include <ares.h>
ares_channel ares;
#else
void *ares;
#endif

struct options opt;

/* defined in version.c */
extern char *system_getrc;
/* Used for --version output in print_version */
#define MAX_CHARS_PER_LINE      72
#define TABULATION              4

const char *exec_name;

/* Number of successfully downloaded URLs */
int numurls = 0;

#ifndef TESTING
/* Initialize I18N/L10N.  That amounts to invoking setlocale, and
   setting up gettext's message catalog using bindtextdomain and
   textdomain.  Does nothing if NLS is disabled or missing.  */

#if defined(SIGHUP) || defined(SIGUSR1)
/* Hangup signal handler.  When wget receives SIGHUP or SIGUSR1, it
   will proceed operation as usual, trying to write into a log file.
   If that is impossible, the output will be turned off.  */


#endif /* defined(SIGHUP) || defined(SIGUSR1) */



#ifdef HAVE_HSTS
/* make the HSTS store global */
hsts_store_t hsts_store;

static char*
get_hsts_database (void)
{
  char *home;

  if (opt.hsts_file)
    return xstrdup (opt.hsts_file);

  home = home_dir ();
  if (home)
    {
      char *dir = aprintf ("%s/.wget-hsts", home);
      xfree(home);
      return dir;
    }

  return NULL;
}

static void
load_hsts (void)
{
  if (!hsts_store)
    {
      char *filename = get_hsts_database ();

      if (filename)
        {
          DEBUGP (("Reading HSTS entries from %s\n", filename));

          hsts_store = hsts_store_open (filename);

          if (!hsts_store)
            logprintf (LOG_NOTQUIET, "ERROR: could not open HSTS store at '%s'. "
                       "HSTS will be disabled.\n",
                       filename);
        }
      else
        logprintf (LOG_NOTQUIET, "ERROR: could not open HSTS store. HSTS will be disabled.\n");

      xfree (filename);
    }
}

static void
save_hsts (void)
{
  if (hsts_store)
    {
      char *filename = get_hsts_database ();

      if (filename && hsts_store_has_changed (hsts_store))
        {
          DEBUGP (("Saving HSTS entries to %s\n", filename));
          hsts_store_save (hsts_store, filename);
        }

      hsts_store_close (hsts_store);

      xfree (filename);
    }
}
#endif

/* Definition of command-line options. */

static void _Noreturn print_help (void);
static void _Noreturn print_version (void);

#ifdef HAVE_SSL
# define IF_SSL(x) x
#else
# define IF_SSL(x) NULL
#endif

struct cmdline_option {
  const char *long_name;
  char short_name;
  enum {
    OPT_VALUE,
    OPT_BOOLEAN,
    OPT_FUNCALL,
    /* Non-standard options that have to be handled specially in
       main().  */
    OPT__APPEND_OUTPUT,
    OPT__CLOBBER,
    OPT__DONT_REMOVE_LISTING,
    OPT__EXECUTE,
    OPT__NO,
    OPT__PARENT
  } type;
  const void *data;             /* for standard options */
  int argtype;                  /* for non-standard options */
};

static struct cmdline_option option_data[] =
  {
    { "accept", 'A', OPT_VALUE, "accept", -1 },
    { "accept-regex", 0, OPT_VALUE, "acceptregex", -1 },
    { "adjust-extension", 'E', OPT_BOOLEAN, "adjustextension", -1 },
    { "append-output", 'a', OPT__APPEND_OUTPUT, NULL, required_argument },
    { "ask-password", 0, OPT_BOOLEAN, "askpassword", -1 },
    { "auth-no-challenge", 0, OPT_BOOLEAN, "authnochallenge", -1 },
    { "background", 'b', OPT_BOOLEAN, "background", -1 },
    { "backup-converted", 'K', OPT_BOOLEAN, "backupconverted", -1 },
    { "backups", 0, OPT_BOOLEAN, "backups", -1 },
    { "base", 'B', OPT_VALUE, "base", -1 },
    { "bind-address", 0, OPT_VALUE, "bindaddress", -1 },
#ifdef HAVE_LIBCARES
    { "bind-dns-address", 0, OPT_VALUE, "binddnsaddress", -1 },
#endif
    { "body-data", 0, OPT_VALUE, "bodydata", -1 },
    { "body-file", 0, OPT_VALUE, "bodyfile", -1 },
    { IF_SSL ("ca-certificate"), 0, OPT_VALUE, "cacertificate", -1 },
    { IF_SSL ("ca-directory"), 0, OPT_VALUE, "cadirectory", -1 },
    { "cache", 0, OPT_BOOLEAN, "cache", -1 },
    { IF_SSL ("certificate"), 0, OPT_VALUE, "certificate", -1 },
    { IF_SSL ("certificate-type"), 0, OPT_VALUE, "certificatetype", -1 },
    { IF_SSL ("check-certificate"), 0, OPT_BOOLEAN, "checkcertificate", -1 },
    { "clobber", 0, OPT__CLOBBER, NULL, optional_argument },
    { "config", 0, OPT_VALUE, "chooseconfig", -1 },
    { "connect-timeout", 0, OPT_VALUE, "connecttimeout", -1 },
    { "continue", 'c', OPT_BOOLEAN, "continue", -1 },
    { "convert-file-only", 0, OPT_BOOLEAN, "convertfileonly", -1 },
    { "convert-links", 'k', OPT_BOOLEAN, "convertlinks", -1 },
    { "content-disposition", 0, OPT_BOOLEAN, "contentdisposition", -1 },
    { "content-on-error", 0, OPT_BOOLEAN, "contentonerror", -1 },
    { "cookies", 0, OPT_BOOLEAN, "cookies", -1 },
    { IF_SSL ("crl-file"), 0, OPT_VALUE, "crlfile", -1 },
    { "cut-dirs", 0, OPT_VALUE, "cutdirs", -1 },
    { "debug", 'd', OPT_BOOLEAN, "debug", -1 },
    { "default-page", 0, OPT_VALUE, "defaultpage", -1 },
    { "delete-after", 0, OPT_BOOLEAN, "deleteafter", -1 },
    { "directories", 0, OPT_BOOLEAN, "dirstruct", -1 },
    { "directory-prefix", 'P', OPT_VALUE, "dirprefix", -1 },
    { "dns-cache", 0, OPT_BOOLEAN, "dnscache", -1 },
#ifdef HAVE_LIBCARES
    { "dns-servers", 0, OPT_VALUE, "dnsservers", -1 },
#endif
    { "dns-timeout", 0, OPT_VALUE, "dnstimeout", -1 },
    { "domains", 'D', OPT_VALUE, "domains", -1 },
    { "dont-remove-listing", 0, OPT__DONT_REMOVE_LISTING, NULL, no_argument },
    { "dot-style", 0, OPT_VALUE, "dotstyle", -1 }, /* deprecated */
    { "egd-file", 0, OPT_VALUE, "egdfile", -1 },
    { "exclude-directories", 'X', OPT_VALUE, "excludedirectories", -1 },
    { "exclude-domains", 0, OPT_VALUE, "excludedomains", -1 },
    { "execute", 'e', OPT__EXECUTE, NULL, required_argument },
    { "follow-ftp", 0, OPT_BOOLEAN, "followftp", -1 },
    { "follow-tags", 0, OPT_VALUE, "followtags", -1 },
    { "force-directories", 'x', OPT_BOOLEAN, "dirstruct", -1 },
    { "force-html", 'F', OPT_BOOLEAN, "forcehtml", -1 },
    { "ftp-password", 0, OPT_VALUE, "ftppassword", -1 },
#ifdef __VMS
    { "ftp-stmlf", 0, OPT_BOOLEAN, "ftpstmlf", -1 },
#endif /* def __VMS */
    { "ftp-user", 0, OPT_VALUE, "ftpuser", -1 },
#ifdef HAVE_SSL
    { "ftps-clear-data-connection", 0, OPT_BOOLEAN, "ftpscleardataconnection", -1 },
    { "ftps-fallback-to-ftp", 0, OPT_BOOLEAN, "ftpsfallbacktoftp", -1 },
    { "ftps-implicit", 0, OPT_BOOLEAN, "ftpsimplicit", -1 },
    { "ftps-resume-ssl", 0, OPT_BOOLEAN, "ftpsresumessl", -1 },
#endif
    { "glob", 0, OPT_BOOLEAN, "glob", -1 },
    { "header", 0, OPT_VALUE, "header", -1 },
    { "help", 'h', OPT_FUNCALL, (void *)print_help, no_argument },
    { "host-directories", 0, OPT_BOOLEAN, "addhostdir", -1 },
#ifdef HAVE_HSTS
    { "hsts", 0, OPT_BOOLEAN, "hsts", -1},
    { "hsts-file", 0, OPT_VALUE, "hsts-file", -1 },
#endif
    { "html-extension", 'E', OPT_BOOLEAN, "adjustextension", -1 }, /* deprecated */
    { "htmlify", 0, OPT_BOOLEAN, "htmlify", -1 },
    { "http-keep-alive", 0, OPT_BOOLEAN, "httpkeepalive", -1 },
    { "http-passwd", 0, OPT_VALUE, "httppassword", -1 }, /* deprecated */
    { "http-password", 0, OPT_VALUE, "httppassword", -1 },
    { "http-user", 0, OPT_VALUE, "httpuser", -1 },
    { IF_SSL ("https-only"), 0, OPT_BOOLEAN, "httpsonly", -1 },
    { "ignore-case", 0, OPT_BOOLEAN, "ignorecase", -1 },
    { "ignore-length", 0, OPT_BOOLEAN, "ignorelength", -1 },
    { "ignore-tags", 0, OPT_VALUE, "ignoretags", -1 },
    { "include-directories", 'I', OPT_VALUE, "includedirectories", -1 },
#ifdef ENABLE_IPV6
    { "inet4-only", '4', OPT_BOOLEAN, "inet4only", -1 },
    { "inet6-only", '6', OPT_BOOLEAN, "inet6only", -1 },
#endif
    { "input-file", 'i', OPT_VALUE, "input", -1 },
#ifdef HAVE_METALINK
    { "input-metalink", 0, OPT_VALUE, "input-metalink", -1 },
#endif
    { "iri", 0, OPT_BOOLEAN, "iri", -1 },
    { "keep-session-cookies", 0, OPT_BOOLEAN, "keepsessioncookies", -1 },
    { "level", 'l', OPT_VALUE, "reclevel", -1 },
    { "limit-rate", 0, OPT_VALUE, "limitrate", -1 },
    { "load-cookies", 0, OPT_VALUE, "loadcookies", -1 },
    { "local-encoding", 0, OPT_VALUE, "localencoding", -1 },
    { "rejected-log", 0, OPT_VALUE, "rejectedlog", -1 },
    { "max-redirect", 0, OPT_VALUE, "maxredirect", -1 },
#ifdef HAVE_METALINK
    { "metalink-over-http", 0, OPT_BOOLEAN, "metalink-over-http", -1 },
#endif
    { "method", 0, OPT_VALUE, "method", -1 },
    { "mirror", 'm', OPT_BOOLEAN, "mirror", -1 },
    { "no", 'n', OPT__NO, NULL, required_argument },
    { "no-clobber", 0, OPT_BOOLEAN, "noclobber", -1 },
    { "no-config", 0, OPT_BOOLEAN, "noconfig", -1},
    { "no-parent", 0, OPT_BOOLEAN, "noparent", -1 },
    { "output-document", 'O', OPT_VALUE, "outputdocument", -1 },
    { "output-file", 'o', OPT_VALUE, "logfile", -1 },
    { "page-requisites", 'p', OPT_BOOLEAN, "pagerequisites", -1 },
    { "parent", 0, OPT__PARENT, NULL, optional_argument },
    { "passive-ftp", 0, OPT_BOOLEAN, "passiveftp", -1 },
    { "password", 0, OPT_VALUE, "password", -1 },
    { IF_SSL ("pinnedpubkey"), 0, OPT_VALUE, "pinnedpubkey", -1 },
    { "post-data", 0, OPT_VALUE, "postdata", -1 },
    { "post-file", 0, OPT_VALUE, "postfile", -1 },
    { "prefer-family", 0, OPT_VALUE, "preferfamily", -1 },
#ifdef HAVE_METALINK
    { "preferred-location", 0, OPT_VALUE, "preferred-location", -1 },
#endif
    { "preserve-permissions", 0, OPT_BOOLEAN, "preservepermissions", -1 },
    { IF_SSL ("private-key"), 0, OPT_VALUE, "privatekey", -1 },
    { IF_SSL ("private-key-type"), 0, OPT_VALUE, "privatekeytype", -1 },
    { "progress", 0, OPT_VALUE, "progress", -1 },
    { "show-progress", 0, OPT_BOOLEAN, "showprogress", -1 },
    { "protocol-directories", 0, OPT_BOOLEAN, "protocoldirectories", -1 },
    { "proxy", 0, OPT_BOOLEAN, "useproxy", -1 },
    { "proxy__compat", 'Y', OPT_VALUE, "useproxy", -1 }, /* back-compatible */
    { "proxy-passwd", 0, OPT_VALUE, "proxypassword", -1 }, /* deprecated */
    { "proxy-password", 0, OPT_VALUE, "proxypassword", -1 },
    { "proxy-user", 0, OPT_VALUE, "proxyuser", -1 },
    { "quiet", 'q', OPT_BOOLEAN, "quiet", -1 },
    { "quota", 'Q', OPT_VALUE, "quota", -1 },
    { "random-file", 0, OPT_VALUE, "randomfile", -1 },
    { "random-wait", 0, OPT_BOOLEAN, "randomwait", -1 },
    { "read-timeout", 0, OPT_VALUE, "readtimeout", -1 },
    { "recursive", 'r', OPT_BOOLEAN, "recursive", -1 },
    { "referer", 0, OPT_VALUE, "referer", -1 },
    { "regex-type", 0, OPT_VALUE, "regextype", -1 },
    { "reject", 'R', OPT_VALUE, "reject", -1 },
    { "reject-regex", 0, OPT_VALUE, "rejectregex", -1 },
    { "relative", 'L', OPT_BOOLEAN, "relativeonly", -1 },
    { "remote-encoding", 0, OPT_VALUE, "remoteencoding", -1 },
    { "remove-listing", 0, OPT_BOOLEAN, "removelisting", -1 },
    { "report-speed", 0, OPT_BOOLEAN, "reportspeed", -1 },
    { "restrict-file-names", 0, OPT_BOOLEAN, "restrictfilenames", -1 },
    { "retr-symlinks", 0, OPT_BOOLEAN, "retrsymlinks", -1 },
    { "retry-connrefused", 0, OPT_BOOLEAN, "retryconnrefused", -1 },
    { "save-cookies", 0, OPT_VALUE, "savecookies", -1 },
    { "save-headers", 0, OPT_BOOLEAN, "saveheaders", -1 },
    { IF_SSL ("secure-protocol"), 0, OPT_VALUE, "secureprotocol", -1 },
    { "server-response", 'S', OPT_BOOLEAN, "serverresponse", -1 },
    { "span-hosts", 'H', OPT_BOOLEAN, "spanhosts", -1 },
    { "spider", 0, OPT_BOOLEAN, "spider", -1 },
    { "start-pos", 0, OPT_VALUE, "startpos", -1 },
    { "strict-comments", 0, OPT_BOOLEAN, "strictcomments", -1 },
    { "timeout", 'T', OPT_VALUE, "timeout", -1 },
    { "timestamping", 'N', OPT_BOOLEAN, "timestamping", -1 },
    { "if-modified-since", 0, OPT_BOOLEAN, "if-modified-since", -1 },
    { "tries", 't', OPT_VALUE, "tries", -1 },
    { "unlink", 0, OPT_BOOLEAN, "unlink", -1 },
    { "trust-server-names", 0, OPT_BOOLEAN, "trustservernames", -1 },
    { "use-server-timestamps", 0, OPT_BOOLEAN, "useservertimestamps", -1 },
    { "user", 0, OPT_VALUE, "user", -1 },
    { "user-agent", 'U', OPT_VALUE, "useragent", -1 },
    { "verbose", 'v', OPT_BOOLEAN, "verbose", -1 },
    { "verbose", 0, OPT_BOOLEAN, "verbose", -1 },
    { "version", 'V', OPT_FUNCALL, (void *) print_version, no_argument },
    { "wait", 'w', OPT_VALUE, "wait", -1 },
    { "waitretry", 0, OPT_VALUE, "waitretry", -1 },
    { "warc-cdx", 0, OPT_BOOLEAN, "warccdx", -1 },
#ifdef HAVE_LIBZ
    { "warc-compression", 0, OPT_BOOLEAN, "warccompression", -1 },
#endif
    { "warc-dedup", 0, OPT_VALUE, "warccdxdedup", -1 },
    { "warc-digests", 0, OPT_BOOLEAN, "warcdigests", -1 },
    { "warc-file", 0, OPT_VALUE, "warcfile", -1 },
    { "warc-header", 0, OPT_VALUE, "warcheader", -1 },
    { "warc-keep-log", 0, OPT_BOOLEAN, "warckeeplog", -1 },
    { "warc-max-size", 0, OPT_VALUE, "warcmaxsize", -1 },
    { "warc-tempdir", 0, OPT_VALUE, "warctempdir", -1 },
#ifdef USE_WATT32
    { "wdebug", 0, OPT_BOOLEAN, "wdebug", -1 },
#endif
  };

#undef IF_SSL

/* Return a string that contains S with "no-" prepended.  The string
   is NUL-terminated and allocated off static storage at Wget
   startup.  */



/* The arguments that that main passes to getopt_long. */
static struct option long_options[2 * countof (option_data) + 1];
static char short_options[128];

/* Mapping between short option chars and option_data indices. */
static unsigned char optmap[96];

/* Marker for `--no-FOO' values in long_options.  */
#define BOOLEAN_NEG_MARKER 1024

/* Initialize the long_options array used by getopt_long from the data
   in option_data.  */



/* Print the usage message.  */


/* Print the help message, describing all the available options.  If
   you add an option, be sure to update this list.  */


/* Return a human-readable printed representation of INTERVAL,
   measured in seconds.  */

static char *
secs_to_human_time (double interval)
{
  static char buf[32];
  int secs = (int) (interval + 0.5);
  int hours, mins, days;

  days = secs / 86400, secs %= 86400;
  hours = secs / 3600, secs %= 3600;
  mins = secs / 60, secs %= 60;

  if (days)
    sprintf (buf, "%dd %dh %dm %ds", days, hours, mins, secs);
  else if (hours)
    sprintf (buf, "%dh %dm %ds", hours, mins, secs);
  else if (mins)
    sprintf (buf, "%dm %ds", mins, secs);
  else
    sprintf (buf, "%ss", print_decimal (interval));

  return buf;
}



/* Function that prints the line argument while limiting it
   to at most line_length. prefix is printed on the first line
   and an appropriate number of spaces are added on subsequent
   lines.*/




const char *program_name; /* Needed by lib/error.c. */
const char *program_argstring; /* Needed by wget_warc.c. */

void main_sense_1(char **url, char **t, int nurl, struct ptimer *timer, double start_time)
{
  signal(28, progress_handle_sigwinch);
  if (opt.hsts)
    load_hsts();
  for (t = url; *t; t++)
  {
    char *filename = 0;
    char *redirected_URL = 0;
    int dt;
    int url_err;
    struct iri *iri = &dummy_iri;
    struct url *url_parsed;
    ;
    url_parsed = url_parse(*t, &url_err, iri, 1);
    if (!url_parsed)
    {
      char *error = url_error(*t, url_err);
      logprintf(LOG_NOTQUIET, "%s: %s.\n", *t, error);
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
      if ((opt.recursive || opt.page_requisites) && (((url_scheme(*t) != SCHEME_FTP) && (url_scheme(*t) != SCHEME_FTPS)) || url_uses_proxy(url_parsed)))
      {
        int old_follow_ftp = opt.follow_ftp;
        if ((url_scheme(*t) == SCHEME_FTP) || (url_scheme(*t) == SCHEME_FTPS))
          opt.follow_ftp = 1;
        retrieve_tree(url_parsed, 0);
        opt.follow_ftp = old_follow_ftp;
      }
      else
      {
        retrieve_url(url_parsed, *t, &filename, &redirected_URL, 0, &dt, opt.recursive, iri, 1);
      }
      if ((opt.delete_after && (filename != 0)) && file_exists_p(filename))
      {
        do
        {
          if (__builtin_expect(opt.debug, 0))
          {
            debug_logprintf("Removing file due to --delete-after in main():\n");
          }
        }
        while (0);
        logprintf(LOG_VERBOSE, gettext("Removing %s.\n"), filename);
        if (unlink(filename))
          logprintf(LOG_NOTQUIET, "unlink: %s\n", strerror(errno));
      }
      do
      {
        free((void *) redirected_URL);
        redirected_URL = 0;
      }
      while (0);
      do
      {
        free((void *) filename);
        filename = 0;
      }
      while (0);
      url_free(url_parsed);
    }
    ;
  }

  if (opt.input_filename)
  {
    int count;
    int status;
    status = retrieve_from_file(opt.input_filename, opt.force_html, &count);
    inform_exit_status(status);
    if (!count)
      logprintf(LOG_NOTQUIET, gettext("No URLs found in %s.\n"), opt.input_filename);
  }
  if (opt.recursive && opt.spider)
    print_broken_links();
  if ((((opt.recursive || opt.page_requisites) || (nurl > 1)) || (opt.input_filename && (total_downloaded_bytes != 0))) && (total_downloaded_bytes != 0))
  {
    double end_time = ptimer_measure(timer);
    char *wall_time = xstrdup(secs_to_human_time(end_time - start_time));
    char *download_time = xstrdup(secs_to_human_time(total_download_time));
    ptimer_destroy(timer);
    logprintf(LOG_NOTQUIET, gettext("FINISHED --%s--\nTotal wall clock time: %s\nDownloaded: %d files, %s in %s (%s)\n"), datetime_str(time(0)), wall_time, numurls, human_readable(total_downloaded_bytes, 10, 1), download_time, retr_rate(total_downloaded_bytes, total_download_time));
    do
    {
      free((void *) wall_time);
      wall_time = 0;
    }
    while (0);
    do
    {
      free((void *) download_time);
      download_time = 0;
    }
    while (0);
    if (opt.quota && (total_downloaded_bytes > opt.quota))
      logprintf(LOG_NOTQUIET, gettext("Download quota of %s EXCEEDED!\n"), human_readable(opt.quota, 10, 1));
  }
  if (opt.cookies_output)
    save_cookies();
  if (opt.hsts && hsts_store)
    save_hsts();
  if ((opt.convert_links || opt.convert_file_only) && (!opt.delete_after))
    convert_all_links();
  cleanup();
  exit(get_exit_status());
}



#endif /* TESTING */

/*
 * vim: et ts=2 sw=2
 */
