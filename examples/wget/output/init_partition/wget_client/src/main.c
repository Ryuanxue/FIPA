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

#include "wget_rpc_wrapper.h"

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

static void
i18n_initialize (void)
{
  /* ENABLE_NLS implies existence of functions invoked here.  */
#ifdef ENABLE_NLS
  /* Set the current locale.  */
  setlocale (LC_ALL, "");
  /* Set the text message domain.  */
  bindtextdomain ("wget", LOCALEDIR);
  textdomain ("wget");
#endif /* ENABLE_NLS */
}

#ifdef HAVE_HSTS
/* make the HSTS store global */
hsts_store_t hsts_store;

static char *get_hsts_database(void)
{
  char *home;
  if (get_opt_hsts_file_wrapper())
    return xstrdup(get_opt_hsts_file_wrapper());
  home = home_dir();
  if (home)
  {
    char *dir = aprintf("%s/.wget-hsts", home);
    do
    {
      free((void *) home);
      home = 0;
    }
    while (0);
    return dir;
  }
  return 0;
}



static void load_hsts(void)
{
  if (!get_hsts_store_wrapper())
  {
    char *filename = get_hsts_database();
    if (filename)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Reading HSTS entries from %s\n", filename);
        }
      }
      while (0);
      set_hsts_store_wrapper(hsts_store_open(filename));
      if (!get_hsts_store_wrapper())
        logprintf(LOG_NOTQUIET, "ERROR: could not open HSTS store at '%s'. HSTS will be disabled.\n", filename);
    }
    else
      logprintf(LOG_NOTQUIET, "ERROR: could not open HSTS store. HSTS will be disabled.\n");
    do
    {
      free((void *) filename);
      filename = 0;
    }
    while (0);
  }
}



static void save_hsts(void)
{
  if (get_hsts_store_wrapper())
  {
    char *filename = get_hsts_database();
    if (filename && hsts_store_has_changed(get_hsts_store_wrapper()))
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Saving HSTS entries to %s\n", filename);
        }
      }
      while (0);
      hsts_store_save(get_hsts_store_wrapper(), filename);
    }
    hsts_store_close(get_hsts_store_wrapper());
    do
    {
      free((void *) filename);
      filename = 0;
    }
    while (0);
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

static char *
no_prefix (const char *s)
{
  static char buffer[2048];
  static char *p = buffer;

  char *cp = p;
  int size = 3 + strlen (s) + 1;  /* "no-STRING\0" */
  assert(p + size <= buffer + sizeof (buffer));

  cp[0] = 'n', cp[1] = 'o', cp[2] = '-';
  strcpy (cp + 3, s);
  p += size;
  return cp;
}

/* The arguments that that main passes to getopt_long. */
static struct option long_options[2 * countof (option_data) + 1];
static char short_options[128];

/* Mapping between short option chars and option_data indices. */
static unsigned char optmap[96];

/* Marker for `--no-FOO' values in long_options.  */
#define BOOLEAN_NEG_MARKER 1024

/* Initialize the long_options array used by getopt_long from the data
   in option_data.  */

static void
init_switches (void)
{
  char *p = short_options;
  size_t i, o = 0;
  for (i = 0; i < countof (option_data); i++)
    {
      struct cmdline_option *cmdopt = &option_data[i];
      struct option *longopt;

      if (!cmdopt->long_name)
        /* The option is disabled. */
        continue;

      longopt = &long_options[o++];
      longopt->name = cmdopt->long_name;
      longopt->val = i;
      if (cmdopt->short_name)
        {
          *p++ = cmdopt->short_name;
          optmap[cmdopt->short_name - 32] = longopt - long_options;
        }
      switch (cmdopt->type)
        {
        case OPT_VALUE:
          longopt->has_arg = required_argument;
          if (cmdopt->short_name)
            *p++ = ':';
          break;
        case OPT_BOOLEAN:
          /* Specify an optional argument for long options, so that
             --option=off works the same as --no-option, for
             compatibility with pre-1.10 Wget.  However, don't specify
             optional arguments short-option booleans because they
             prevent combining of short options.  */
          longopt->has_arg = optional_argument;
          /* For Boolean options, add the "--no-FOO" variant, which is
             identical to "--foo", except it has opposite meaning and
             it doesn't allow an argument.  */
          longopt = &long_options[o++];
          longopt->name = no_prefix (cmdopt->long_name);
          longopt->has_arg = no_argument;
          /* Mask the value so we'll be able to recognize that we're
             dealing with the false value.  */
          longopt->val = i | BOOLEAN_NEG_MARKER;
          break;
        default:
          assert (cmdopt->argtype != -1);
          longopt->has_arg = cmdopt->argtype;
          if (cmdopt->short_name)
            {
              if (longopt->has_arg == required_argument)
                *p++ = ':';
              /* Don't handle optional_argument */
            }
        }
    }
  /* Terminate short_options. */
  *p = '\0';
  /* No need for xzero(long_options[o]) because its storage is static
     and it will be zeroed by default.  */
  assert (o <= countof (long_options));
}

/* Print the usage message.  */
static int print_usage(int error)
{
  return fprintf((error) ? (stderr) : (stdout), gettext("Usage: %s [OPTION]... [URL]...\n"), get_exec_name_wrapper());
}



/* Print the help message, describing all the available options.  If
   you add an option, be sure to update this list.  */
static void _Noreturn
print_help (void)
{
  /* We split the help text this way to ease translation of individual
     entries.  */
  static const char *help[] = {
    "\n",
    N_("\
Mandatory arguments to long options are mandatory for short options too.\n\n"),
    N_("\
Startup:\n"),
    N_("\
  -V,  --version                   display the version of Wget and exit\n"),
    N_("\
  -h,  --help                      print this help\n"),
    N_("\
  -b,  --background                go to background after startup\n"),
    N_("\
  -e,  --execute=COMMAND           execute a `.wgetrc'-style command\n"),
    "\n",

    N_("\
Logging and input file:\n"),
    N_("\
  -o,  --output-file=FILE          log messages to FILE\n"),
    N_("\
  -a,  --append-output=FILE        append messages to FILE\n"),
#ifdef ENABLE_DEBUG
    N_("\
  -d,  --debug                     print lots of debugging information\n"),
#endif
#ifdef USE_WATT32
    N_("\
       --wdebug                    print Watt-32 debug output\n"),
#endif
    N_("\
  -q,  --quiet                     quiet (no output)\n"),
    N_("\
  -v,  --verbose                   be verbose (this is the default)\n"),
    N_("\
  -nv, --no-verbose                turn off verboseness, without being quiet\n"),
    N_("\
       --report-speed=TYPE         output bandwidth as TYPE.  TYPE can be bits\n"),
    N_("\
  -i,  --input-file=FILE           download URLs found in local or external FILE\n"),
#ifdef HAVE_METALINK
    N_("\
       --input-metalink=FILE       download files covered in local Metalink FILE\n"),
#endif
    N_("\
  -F,  --force-html                treat input file as HTML\n"),
    N_("\
  -B,  --base=URL                  resolves HTML input-file links (-i -F)\n\
                                     relative to URL\n"),
    N_("\
       --config=FILE               specify config file to use\n"),
    N_("\
       --no-config                 do not read any config file\n"),
    N_("\
       --rejected-log=FILE         log reasons for URL rejection to FILE\n"),
    "\n",

    N_("\
Download:\n"),
    N_("\
  -t,  --tries=NUMBER              set number of retries to NUMBER (0 unlimits)\n"),
    N_("\
       --retry-connrefused         retry even if connection is refused\n"),
    N_("\
  -O,  --output-document=FILE      write documents to FILE\n"),
    N_("\
  -nc, --no-clobber                skip downloads that would download to\n\
                                     existing files (overwriting them)\n"),
    N_("\
  -c,  --continue                  resume getting a partially-downloaded file\n"),
    N_("\
       --start-pos=OFFSET          start downloading from zero-based position OFFSET\n"),
    N_("\
       --progress=TYPE             select progress gauge type\n"),
    N_("\
       --show-progress             display the progress bar in any verbosity mode\n"),
    N_("\
  -N,  --timestamping              don't re-retrieve files unless newer than\n\
                                     local\n"),
    N_("\
       --no-if-modified-since      don't use conditional if-modified-since get\n\
                                     requests in timestamping mode\n"),
    N_("\
       --no-use-server-timestamps  don't set the local file's timestamp by\n\
                                     the one on the server\n"),
    N_("\
  -S,  --server-response           print server response\n"),
    N_("\
       --spider                    don't download anything\n"),
    N_("\
  -T,  --timeout=SECONDS           set all timeout values to SECONDS\n"),
#ifdef HAVE_LIBCARES
    N_("\
       --dns-servers=ADDRESSES     list of DNS servers to query (comma separated)\n"),
    N_("\
       --bind-dns-address=ADDRESS  bind DNS resolver to ADDRESS (hostname or IP) on local host\n"),
#endif
    N_("\
       --dns-timeout=SECS          set the DNS lookup timeout to SECS\n"),
    N_("\
       --connect-timeout=SECS      set the connect timeout to SECS\n"),
    N_("\
       --read-timeout=SECS         set the read timeout to SECS\n"),
    N_("\
  -w,  --wait=SECONDS              wait SECONDS between retrievals\n"),
    N_("\
       --waitretry=SECONDS         wait 1..SECONDS between retries of a retrieval\n"),
    N_("\
       --random-wait               wait from 0.5*WAIT...1.5*WAIT secs between retrievals\n"),
    N_("\
       --no-proxy                  explicitly turn off proxy\n"),
    N_("\
  -Q,  --quota=NUMBER              set retrieval quota to NUMBER\n"),
    N_("\
       --bind-address=ADDRESS      bind to ADDRESS (hostname or IP) on local host\n"),
    N_("\
       --limit-rate=RATE           limit download rate to RATE\n"),
    N_("\
       --no-dns-cache              disable caching DNS lookups\n"),
    N_("\
       --restrict-file-names=OS    restrict chars in file names to ones OS allows\n"),
    N_("\
       --ignore-case               ignore case when matching files/directories\n"),
#ifdef ENABLE_IPV6
    N_("\
  -4,  --inet4-only                connect only to IPv4 addresses\n"),
    N_("\
  -6,  --inet6-only                connect only to IPv6 addresses\n"),
    N_("\
       --prefer-family=FAMILY      connect first to addresses of specified family,\n\
                                     one of IPv6, IPv4, or none\n"),
#endif
    N_("\
       --user=USER                 set both ftp and http user to USER\n"),
    N_("\
       --password=PASS             set both ftp and http password to PASS\n"),
    N_("\
       --ask-password              prompt for passwords\n"),
    N_("\
       --no-iri                    turn off IRI support\n"),
    N_("\
       --local-encoding=ENC        use ENC as the local encoding for IRIs\n"),
    N_("\
       --remote-encoding=ENC       use ENC as the default remote encoding\n"),
    N_("\
       --unlink                    remove file before clobber\n"),
#ifdef HAVE_METALINK
    N_("\
       --metalink-over-http        use Metalink metadata from HTTP response headers\n"),
    N_("\
       --preferred-location        preferred location for Metalink resources\n"),
#endif
    "\n",

    N_("\
Directories:\n"),
    N_("\
  -nd, --no-directories            don't create directories\n"),
    N_("\
  -x,  --force-directories         force creation of directories\n"),
    N_("\
  -nH, --no-host-directories       don't create host directories\n"),
    N_("\
       --protocol-directories      use protocol name in directories\n"),
    N_("\
  -P,  --directory-prefix=PREFIX   save files to PREFIX/..\n"),
    N_("\
       --cut-dirs=NUMBER           ignore NUMBER remote directory components\n"),
    "\n",

    N_("\
HTTP options:\n"),
    N_("\
       --http-user=USER            set http user to USER\n"),
    N_("\
       --http-password=PASS        set http password to PASS\n"),
    N_("\
       --no-cache                  disallow server-cached data\n"),
    N_ ("\
       --default-page=NAME         change the default page name (normally\n\
                                     this is 'index.html'.)\n"),
    N_("\
  -E,  --adjust-extension          save HTML/CSS documents with proper extensions\n"),
    N_("\
       --ignore-length             ignore 'Content-Length' header field\n"),
    N_("\
       --header=STRING             insert STRING among the headers\n"),
    N_("\
       --max-redirect              maximum redirections allowed per page\n"),
    N_("\
       --proxy-user=USER           set USER as proxy username\n"),
    N_("\
       --proxy-password=PASS       set PASS as proxy password\n"),
    N_("\
       --referer=URL               include 'Referer: URL' header in HTTP request\n"),
    N_("\
       --save-headers              save the HTTP headers to file\n"),
    N_("\
  -U,  --user-agent=AGENT          identify as AGENT instead of Wget/VERSION\n"),
    N_("\
       --no-http-keep-alive        disable HTTP keep-alive (persistent connections)\n"),
    N_("\
       --no-cookies                don't use cookies\n"),
    N_("\
       --load-cookies=FILE         load cookies from FILE before session\n"),
    N_("\
       --save-cookies=FILE         save cookies to FILE after session\n"),
    N_("\
       --keep-session-cookies      load and save session (non-permanent) cookies\n"),
    N_("\
       --post-data=STRING          use the POST method; send STRING as the data\n"),
    N_("\
       --post-file=FILE            use the POST method; send contents of FILE\n"),
    N_("\
       --method=HTTPMethod         use method \"HTTPMethod\" in the request\n"),
    N_("\
       --body-data=STRING          send STRING as data. --method MUST be set\n"),
    N_("\
       --body-file=FILE            send contents of FILE. --method MUST be set\n"),
    N_("\
       --content-disposition       honor the Content-Disposition header when\n\
                                     choosing local file names (EXPERIMENTAL)\n"),
    N_("\
       --content-on-error          output the received content on server errors\n"),
    N_("\
       --auth-no-challenge         send Basic HTTP authentication information\n\
                                     without first waiting for the server's\n\
                                     challenge\n"),
    "\n",

#ifdef HAVE_SSL
    N_("\
HTTPS (SSL/TLS) options:\n"),
    N_("\
       --secure-protocol=PR        choose secure protocol, one of auto, SSLv2,\n\
                                     SSLv3, TLSv1 and PFS\n"),
    N_("\
       --https-only                only follow secure HTTPS links\n"),
    N_("\
       --no-check-certificate      don't validate the server's certificate\n"),
    N_("\
       --certificate=FILE          client certificate file\n"),
    N_("\
       --certificate-type=TYPE     client certificate type, PEM or DER\n"),
    N_("\
       --private-key=FILE          private key file\n"),
    N_("\
       --private-key-type=TYPE     private key type, PEM or DER\n"),
    N_("\
       --ca-certificate=FILE       file with the bundle of CAs\n"),
    N_("\
       --ca-directory=DIR          directory where hash list of CAs is stored\n"),
    N_("\
       --crl-file=FILE             file with bundle of CRLs\n"),
    N_("\
       --pinnedpubkey=FILE/HASHES  Public key (PEM/DER) file, or any number\n\
                                   of base64 encoded sha256 hashes preceded by\n\
                                   \'sha256//\' and seperated by \';\', to verify\n\
                                   peer against\n"),
#if defined(HAVE_LIBSSL) || defined(HAVE_LIBSSL32)
    N_("\
       --random-file=FILE          file with random data for seeding the SSL PRNG\n"),
#endif
#if (defined(HAVE_LIBSSL) || defined(HAVE_LIBSSL32)) && defined(HAVE_RAND_EGD)
    N_("\
       --egd-file=FILE             file naming the EGD socket with random data\n"),
#endif
    "\n",
#endif /* HAVE_SSL */

#ifdef HAVE_HSTS
    N_("\
HSTS options:\n"),
    N_("\
       --no-hsts                   disable HSTS\n"),
    N_("\
       --hsts-file                 path of HSTS database (will override default)\n"),
    "\n",
#endif

    N_("\
FTP options:\n"),
#ifdef __VMS
    N_("\
       --ftp-stmlf                 use Stream_LF format for all binary FTP files\n"),
#endif /* def __VMS */
    N_("\
       --ftp-user=USER             set ftp user to USER\n"),
    N_("\
       --ftp-password=PASS         set ftp password to PASS\n"),
    N_("\
       --no-remove-listing         don't remove '.listing' files\n"),
    N_("\
       --no-glob                   turn off FTP file name globbing\n"),
    N_("\
       --no-passive-ftp            disable the \"passive\" transfer mode\n"),
    N_("\
       --preserve-permissions      preserve remote file permissions\n"),
    N_("\
       --retr-symlinks             when recursing, get linked-to files (not dir)\n"),
    "\n",

#ifdef HAVE_SSL
    N_("\
FTPS options:\n"),
    N_("\
       --ftps-implicit                 use implicit FTPS (default port is 990)\n"),
    N_("\
       --ftps-resume-ssl               resume the SSL/TLS session started in the control connection when\n"
        "                                         opening a data connection\n"),
    N_("\
       --ftps-clear-data-connection    cipher the control channel only; all the data will be in plaintext\n"),
    N_("\
       --ftps-fallback-to-ftp          fall back to FTP if FTPS is not supported in the target server\n"),
#endif

    N_("\
WARC options:\n"),
    N_("\
       --warc-file=FILENAME        save request/response data to a .warc.gz file\n"),
    N_("\
       --warc-header=STRING        insert STRING into the warcinfo record\n"),
    N_("\
       --warc-max-size=NUMBER      set maximum size of WARC files to NUMBER\n"),
    N_("\
       --warc-cdx                  write CDX index files\n"),
    N_("\
       --warc-dedup=FILENAME       do not store records listed in this CDX file\n"),
#ifdef HAVE_LIBZ
    N_("\
       --no-warc-compression       do not compress WARC files with GZIP\n"),
#endif
    N_("\
       --no-warc-digests           do not calculate SHA1 digests\n"),
    N_("\
       --no-warc-keep-log          do not store the log file in a WARC record\n"),
    N_("\
       --warc-tempdir=DIRECTORY    location for temporary files created by the\n\
                                     WARC writer\n"),
    "\n",

    N_("\
Recursive download:\n"),
    N_("\
  -r,  --recursive                 specify recursive download\n"),
    N_("\
  -l,  --level=NUMBER              maximum recursion depth (inf or 0 for infinite)\n"),
    N_("\
       --delete-after              delete files locally after downloading them\n"),
    N_("\
  -k,  --convert-links             make links in downloaded HTML or CSS point to\n\
                                     local files\n"),
    N_("\
       --convert-file-only         convert the file part of the URLs only (usually known as the basename)\n"),
    N_("\
       --backups=N                 before writing file X, rotate up to N backup files\n"),

#ifdef __VMS
    N_("\
  -K,  --backup-converted          before converting file X, back up as X_orig\n"),
#else /* def __VMS */
    N_("\
  -K,  --backup-converted          before converting file X, back up as X.orig\n"),
#endif /* def __VMS [else] */
    N_("\
  -m,  --mirror                    shortcut for -N -r -l inf --no-remove-listing\n"),
    N_("\
  -p,  --page-requisites           get all images, etc. needed to display HTML page\n"),
    N_("\
       --strict-comments           turn on strict (SGML) handling of HTML comments\n"),
    "\n",

    N_("\
Recursive accept/reject:\n"),
    N_("\
  -A,  --accept=LIST               comma-separated list of accepted extensions\n"),
    N_("\
  -R,  --reject=LIST               comma-separated list of rejected extensions\n"),
    N_("\
       --accept-regex=REGEX        regex matching accepted URLs\n"),
    N_("\
       --reject-regex=REGEX        regex matching rejected URLs\n"),
#ifdef HAVE_LIBPCRE
    N_("\
       --regex-type=TYPE           regex type (posix|pcre)\n"),
#else
    N_("\
       --regex-type=TYPE           regex type (posix)\n"),
#endif
    N_("\
  -D,  --domains=LIST              comma-separated list of accepted domains\n"),
    N_("\
       --exclude-domains=LIST      comma-separated list of rejected domains\n"),
    N_("\
       --follow-ftp                follow FTP links from HTML documents\n"),
    N_("\
       --follow-tags=LIST          comma-separated list of followed HTML tags\n"),
    N_("\
       --ignore-tags=LIST          comma-separated list of ignored HTML tags\n"),
    N_("\
  -H,  --span-hosts                go to foreign hosts when recursive\n"),
    N_("\
  -L,  --relative                  follow relative links only\n"),
    N_("\
  -I,  --include-directories=LIST  list of allowed directories\n"),
    N_("\
       --trust-server-names        use the name specified by the redirection\n\
                                     URL's last component\n"),
    N_("\
  -X,  --exclude-directories=LIST  list of excluded directories\n"),
    N_("\
  -np, --no-parent                 don't ascend to the parent directory\n"),
    "\n",
    N_("Mail bug reports and suggestions to <bug-wget@gnu.org>\n")
  };

  size_t i;

  if (printf (_("GNU Wget %s, a non-interactive network retriever.\n"),
              version_string) < 0)
    exit (WGET_EXIT_IO_FAIL);
  if (print_usage (0) < 0)
    exit (WGET_EXIT_IO_FAIL);

  for (i = 0; i < countof (help); i++)
    if (fputs (_(help[i]), stdout) < 0)
      exit (WGET_EXIT_IO_FAIL);

  exit (WGET_EXIT_SUCCESS);
}

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

static char *prompt_for_password(void)
{
  if (get_opt_user_wrapper())
    fprintf(stderr, gettext("Password for user %s: "), quote(get_opt_user_wrapper()));
  else
    fprintf(stderr, gettext("Password: "));
  return gnu_getpass("");
}



/* Function that prints the line argument while limiting it
   to at most line_length. prefix is printed on the first line
   and an appropriate number of spaces are added on subsequent
   lines.*/
static int
format_and_print_line (const char *prefix, const char *line,
                       int line_length)
{
  int remaining_chars;
  char *line_dup, *token;

  assert (prefix != NULL);
  assert (line != NULL);
  assert (line_length > TABULATION);

  line_dup = xstrdup (line);

  if (printf ("%s", prefix) < 0)
    {
      xfree (line_dup);
      return -1;
    }

  /* Wrap to new line after prefix. */
  remaining_chars = 0;

  /* We break on spaces. */
  token = strtok (line_dup, " ");
  while (token != NULL)
    {
      /* If however a token is much larger than the maximum
         line length, all bets are off and we simply print the
         token on the next line. */
      if (remaining_chars <= (int) strlen (token))
        {
          if (printf ("\n%*c", TABULATION, ' ') < 0)
            {
              xfree (line_dup);
              return -1;
            }
          remaining_chars = line_length - TABULATION;
        }
      if (printf ("%s ", token) < 0)
        {
          xfree (line_dup);
          return -1;
        }
      remaining_chars -= strlen (token) + 1;  /* account for " " */
      token = strtok (NULL, " ");
    }

  if (printf ("\n") < 0)
    {
      xfree (line_dup);
      return -1;
    }

  xfree (line_dup);
  return 0;
}

static void _Noreturn
print_version (void)
{
  const char *wgetrc_title  = _("Wgetrc: ");
  const char *locale_title  = _("Locale: ");
  const char *compile_title = _("Compile: ");
  const char *link_title    = _("Link: ");
  char *env_wgetrc, *user_wgetrc;
  int i;

  if (printf (_("GNU Wget %s built on %s.\n\n"), version_string, OS_TYPE) < 0)
    exit (WGET_EXIT_IO_FAIL);

  for (i = 0; compiled_features[i] != NULL; )
    {
      int line_length = MAX_CHARS_PER_LINE;
      while ((line_length > 0) && (compiled_features[i] != NULL))
        {
          if (printf ("%s ", compiled_features[i]) < 0)
            exit (WGET_EXIT_IO_FAIL);
          line_length -= strlen (compiled_features[i]) + 2;
          i++;
        }
      if (printf ("\n") < 0)
        exit (WGET_EXIT_IO_FAIL);
    }
  if (printf ("\n") < 0)
    exit (WGET_EXIT_IO_FAIL);

  /* Handle the case when $WGETRC is unset and $HOME/.wgetrc is
     absent. */
  if (printf ("%s\n", wgetrc_title) < 0)
    exit (WGET_EXIT_IO_FAIL);

  env_wgetrc = wgetrc_env_file_name ();
  if (env_wgetrc && *env_wgetrc)
    {
      if (printf (_("    %s (env)\n"), env_wgetrc) < 0)
        exit (WGET_EXIT_IO_FAIL);
      xfree (env_wgetrc);
    }
  user_wgetrc = wgetrc_user_file_name ();
  if (user_wgetrc)
    {
      if (printf (_("    %s (user)\n"), user_wgetrc) < 0)
        exit (WGET_EXIT_IO_FAIL);
      xfree (user_wgetrc);
    }
#ifdef SYSTEM_WGETRC
  if (printf (_("    %s (system)\n"), SYSTEM_WGETRC) < 0)
    exit (WGET_EXIT_IO_FAIL);
#endif

#ifdef ENABLE_NLS
  if (format_and_print_line (locale_title,
                             LOCALEDIR,
                             MAX_CHARS_PER_LINE) < 0)
    exit (WGET_EXIT_IO_FAIL);
#endif /* def ENABLE_NLS */

  if (compilation_string != NULL)
    if (format_and_print_line (compile_title,
                               compilation_string,
                               MAX_CHARS_PER_LINE) < 0)
      exit (WGET_EXIT_IO_FAIL);

  if (link_string != NULL)
    if (format_and_print_line (link_title,
                               link_string,
                               MAX_CHARS_PER_LINE) < 0)
      exit (WGET_EXIT_IO_FAIL);

  if (printf ("\n") < 0)
    exit (WGET_EXIT_IO_FAIL);

  /* TRANSLATORS: When available, an actual copyright character
     (circle-c) should be used in preference to "(C)". */
  if (printf (_("\
Copyright (C) %s Free Software Foundation, Inc.\n"), "2015") < 0)
    exit (WGET_EXIT_IO_FAIL);
  if (fputs (_("\
License GPLv3+: GNU GPL version 3 or later\n\
<http://www.gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n"), stdout) < 0)
    exit (WGET_EXIT_IO_FAIL);
  /* TRANSLATORS: When available, please use the proper diacritics for
     names such as this one. See en_US.po for reference. */
  if (fputs (_("\nOriginally written by Hrvoje Niksic <hniksic@xemacs.org>.\n"),
             stdout) < 0)
    exit (WGET_EXIT_IO_FAIL);
  if (fputs (_("Please send bug reports and questions to <bug-wget@gnu.org>.\n"),
             stdout) < 0)
    exit (WGET_EXIT_IO_FAIL);

  exit (WGET_EXIT_SUCCESS);
}

const char *program_name; /* Needed by lib/error.c. */
const char *program_argstring; /* Needed by wget_warc.c. */

int main(int argc, char **argv)
{
  char **url;
  char **t;
  char *p;
  int i;
  int ret;
  int longindex;
  int nurl;
  int retconf;
  int argstring_length;
  bool use_userconfig = 0;
  bool noconfig = 0;
  bool append_to_log = 0;
  struct ptimer *timer = ptimer_new();
  double start_time = ptimer_measure(timer);
  set_total_downloaded_bytes_wrapper(0);
  program_name = argv[0];
  i18n_initialize();
  set_exec_name_wrapper(base_name(argv[0]));
  for (argstring_length = 1, i = 1; i < argc; i++)
    argstring_length += (strlen(argv[i]) + 3) + 1;

  set_program_argstring_wrapper(p = malloc(argstring_length));
  if (p == 0)
  {
    fprintf(stderr, gettext("Memory allocation problem\n"));
    exit(WGET_EXIT_PARSE_ERROR);
  }
  for (i = 1; i < argc; i++)
  {
    int arglen;
    *(p++) = '"';
    arglen = strlen(argv[i]);
    memcpy(p, argv[i], arglen);
    p += arglen;
    *(p++) = '"';
    *(p++) = ' ';
  }

  *p = '\0';
  defaults();
  init_switches();
  longindex = -1;
  while ((retconf = getopt_long(argc, argv, short_options, long_options, &longindex)) != (-1))
  {
    int confval;
    struct cmdline_option *config_opt;
    if (longindex >= 0)
    {
      confval = long_options[longindex].val;
      config_opt = &option_data[confval & (~1024)];
      if (strcmp(config_opt->long_name, "no-config") == 0)
      {
        noconfig = 1;
        break;
      }
      else
        if (strcmp(config_opt->long_name, "config") == 0)
      {
        bool userrc_ret = 1;
        userrc_ret &= run_wgetrc(optarg);
        use_userconfig = 1;
        if (userrc_ret)
          break;
        else
        {
          fprintf(stderr, gettext("Exiting due to error in %s\n"), optarg);
          exit(WGET_EXIT_PARSE_ERROR);
        }
      }
    }
  }

  if ((noconfig == 0) && (use_userconfig == 0))
    initialize();
  opterr = 0;
  optind = 0;
  longindex = -1;
  while ((ret = getopt_long(argc, argv, short_options, long_options, &longindex)) != (-1))
  {
    int val;
    struct cmdline_option *cmdopt;
    if (longindex == (-1))
    {
      if (ret == '?')
      {
        print_usage(1);
        fprintf(stderr, "\n");
        fprintf(stderr, gettext("Try `%s --help' for more options.\n"), get_exec_name_wrapper());
        exit(WGET_EXIT_PARSE_ERROR);
      }
      longindex = optmap[ret - 32];
    }
    val = long_options[longindex].val;
    cmdopt = &option_data[val & (~1024)];
    switch (cmdopt->type)
    {
      case OPT_VALUE:
        setoptval(cmdopt->data, optarg, cmdopt->long_name);
        break;

      case OPT_BOOLEAN:
        if (optarg)
        setoptval(cmdopt->data, optarg, cmdopt->long_name);
      else
      {
        bool neg = !(!(val & 1024));
        setoptval(cmdopt->data, (neg) ? ("0") : ("1"), cmdopt->long_name);
      }
        break;

      case OPT_FUNCALL:
      {
        void (*func)(void) = (void (*)(void)) cmdopt->data;
        func();
      }
        break;

      case OPT__APPEND_OUTPUT:
        setoptval("logfile", optarg, cmdopt->long_name);
        append_to_log = 1;
        break;

      case OPT__EXECUTE:
        run_command(optarg);
        break;

      case OPT__NO:
      {
        for (p = optarg; p && (*p); p++)
          switch (*p)
        {
          case 'v':
            setoptval("verbose", "0", cmdopt->long_name);
            break;

          case 'H':
            setoptval("addhostdir", "0", cmdopt->long_name);
            break;

          case 'd':
            setoptval("dirstruct", "0", cmdopt->long_name);
            break;

          case 'c':
            setoptval("noclobber", "1", cmdopt->long_name);
            break;

          case 'p':
            setoptval("noparent", "1", cmdopt->long_name);
            break;

          default:
            fprintf(stderr, gettext("%s: illegal option -- `-n%c'\n"), get_exec_name_wrapper(), *p);
            print_usage(1);
            fprintf(stderr, "\n");
            fprintf(stderr, gettext("Try `%s --help' for more options.\n"), get_exec_name_wrapper());
            exit(WGET_EXIT_GENERIC_ERROR);

        }


        break;
      }

      case OPT__PARENT:

      case OPT__CLOBBER:
      {
        bool flag = 1;
        if (optarg)
          flag = (((*optarg) == '1') || (c_tolower(*optarg) == 'y')) || ((c_tolower(optarg[0]) == 'o') && (c_tolower(optarg[1]) == 'n'));
        setoptval((cmdopt->type == OPT__PARENT) ? ("noparent") : ("noclobber"), (flag) ? ("0") : ("1"), cmdopt->long_name);
        break;
      }

      case OPT__DONT_REMOVE_LISTING:
        setoptval("removelisting", "0", cmdopt->long_name);
        break;

    }

    longindex = -1;
  }

  nurl = argc - optind;
  if (get_opt_noclobber_wrapper() && (get_opt_convert_links_wrapper() || get_opt_convert_file_only_wrapper()))
  {
    fprintf(stderr, (get_opt_convert_links_wrapper()) ? (gettext("Both --no-clobber and --convert-links were specified, only --convert-links will be used.\n")) : (gettext("Both --no-clobber and --convert-file-only were specified, only --convert-file-only will be used.\n")));
    set_opt_noclobber_wrapper(0);
  }
  if (get_opt_reclevel_wrapper() == 0)
    set_opt_reclevel_wrapper(-1);
  if (get_opt_spider_wrapper() || get_opt_delete_after_wrapper())
    set_opt_no_dirstruct_wrapper(1);
  if (get_opt_page_requisites_wrapper() && (!get_opt_recursive_wrapper()))
  {
    set_opt_reclevel_wrapper(0);
    if (!get_opt_no_dirstruct_wrapper())
      set_opt_dirstruct_wrapper(1);
  }
  if (get_opt_verbose_wrapper() == (-1))
    set_opt_verbose_wrapper(!opt.quiet);
  if ((!get_opt_verbose_wrapper()) && (get_opt_show_progress_wrapper() == (-1)))
    set_opt_show_progress_wrapper(0);
  if (get_opt_quiet_wrapper() && (get_opt_show_progress_wrapper() == (-1)))
    set_opt_show_progress_wrapper(0);
  if (get_opt_verbose_wrapper() && get_opt_quiet_wrapper())
  {
    fprintf(stderr, gettext("Can't be verbose and quiet at the same time.\n"));
    print_usage(1);
    exit(WGET_EXIT_GENERIC_ERROR);
  }
  if (get_opt_timestamping_wrapper() && get_opt_noclobber_wrapper())
  {
    fprintf(stderr, gettext("Can't timestamp and not clobber old files at the same time.\n"));
    print_usage(1);
    exit(WGET_EXIT_GENERIC_ERROR);
  }
  if (get_opt_ipv4_only_wrapper() && get_opt_ipv6_only_wrapper())
  {
    fprintf(stderr, gettext("Cannot specify both --inet4-only and --inet6-only.\n"));
    print_usage(1);
    exit(WGET_EXIT_GENERIC_ERROR);
  }
  if (get_opt_output_document_wrapper())
  {
    if ((get_opt_convert_links_wrapper() || get_opt_convert_file_only_wrapper()) && (((nurl > 1) || get_opt_page_requisites_wrapper()) || get_opt_recursive_wrapper()))
    {
      fputs(gettext("Cannot specify both -k or --convert-file-only and -O if multiple URLs are given, or in combination\nwith -p or -r. See the manual for details.\n\n"), stderr);
      print_usage(1);
      exit(WGET_EXIT_GENERIC_ERROR);
    }
    if (get_opt_page_requisites_wrapper() || get_opt_recursive_wrapper())
    {
      logprintf(LOG_NOTQUIET, "%s", gettext("WARNING: combining -O with -r or -p will mean that all downloaded content\nwill be placed in the single file you specified.\n\n"));
    }
    if (get_opt_timestamping_wrapper())
    {
      logprintf(LOG_NOTQUIET, "%s", gettext("WARNING: timestamping does nothing in combination with -O. See the manual\nfor details.\n\n"));
      set_opt_timestamping_wrapper(0);
    }
    if (get_opt_noclobber_wrapper() && file_exists_p(get_opt_output_document_wrapper()))
    {
      logprintf(LOG_VERBOSE, gettext("File `%s' already there; not retrieving.\n"), get_opt_output_document_wrapper());
      exit(WGET_EXIT_GENERIC_ERROR);
    }
  }
  if (get_opt_warc_filename_wrapper() != 0)
  {
    if (get_opt_noclobber_wrapper())
    {
      fprintf(stderr, gettext("WARC output does not work with --no-clobber, --no-clobber will be disabled.\n"));
      set_opt_noclobber_wrapper(0);
    }
    if (get_opt_timestamping_wrapper())
    {
      fprintf(stderr, gettext("WARC output does not work with timestamping, timestamping will be disabled.\n"));
      set_opt_timestamping_wrapper(0);
    }
    if (get_opt_spider_wrapper())
    {
      fprintf(stderr, gettext("WARC output does not work with --spider.\n"));
      exit(WGET_EXIT_GENERIC_ERROR);
    }
    if (get_opt_always_rest_wrapper() || (get_opt_start_pos_wrapper() >= 0))
    {
      fprintf(stderr, gettext("WARC output does not work with --continue or --start-pos, they will be disabled.\n"));
      set_opt_always_rest_wrapper(0);
      set_opt_start_pos_wrapper(-1);
    }
    if ((get_opt_warc_cdx_dedup_filename_wrapper() != 0) && (!get_opt_warc_digests_enabled_wrapper()))
    {
      fprintf(stderr, gettext("Digests are disabled; WARC deduplication will not find duplicate records.\n"));
    }
    if (get_opt_warc_keep_log_wrapper())
    {
      set_opt_progress_type_wrapper(xstrdup("dot"));
    }
  }
  if (get_opt_ask_passwd_wrapper() && get_opt_passwd_wrapper())
  {
    fprintf(stderr, gettext("Cannot specify both --ask-password and --password.\n"));
    print_usage(1);
    exit(WGET_EXIT_GENERIC_ERROR);
  }
  if ((get_opt_start_pos_wrapper() >= 0) && get_opt_always_rest_wrapper())
  {
    fprintf(stderr, gettext("Specifying both --start-pos and --continue is not recommended; --continue will be disabled.\n"));
    set_opt_always_rest_wrapper(0);
  }
  if ((!nurl) && (!get_opt_input_filename_wrapper()))
  {
    fprintf(stderr, gettext("%s: missing URL\n"), get_exec_name_wrapper());
    print_usage(1);
    fprintf(stderr, "\n");
    fprintf(stderr, gettext("Try `%s --help' for more options.\n"), get_exec_name_wrapper());
    exit(WGET_EXIT_GENERIC_ERROR);
  }
  switch (get_opt_regex_type_wrapper())
  {
    case regex_type_pcre:
      set_opt_regex_compile_fun_wrapper(compile_pcre_regex);
      set_opt_regex_match_fun_wrapper(match_pcre_regex);
      break;

    case regex_type_posix:

    default:
      set_opt_regex_compile_fun_wrapper(compile_posix_regex);
      set_opt_regex_match_fun_wrapper(match_posix_regex);
      break;

  }

  if (get_opt_acceptregex_s_wrapper())
  {
    set_opt_acceptregex_wrapper(opt.regex_compile_fun(opt.acceptregex_s));
    if (!get_opt_acceptregex_wrapper())
      exit(WGET_EXIT_GENERIC_ERROR);
  }
  if (get_opt_rejectregex_s_wrapper())
  {
    set_opt_rejectregex_wrapper(opt.regex_compile_fun(opt.rejectregex_s));
    if (!get_opt_rejectregex_wrapper())
      exit(WGET_EXIT_GENERIC_ERROR);
  }
  if (get_opt_post_data_wrapper() || get_opt_post_file_name_wrapper())
  {
    if (get_opt_post_data_wrapper() && get_opt_post_file_name_wrapper())
    {
      fprintf(stderr, gettext("You cannot specify both --post-data and --post-file.\n"));
      exit(WGET_EXIT_GENERIC_ERROR);
    }
    else
      if (get_opt_method_wrapper())
    {
      fprintf(stderr, gettext("You cannot use --post-data or --post-file along with --method. --method expects data through --body-data and --body-file options"));
      exit(WGET_EXIT_GENERIC_ERROR);
    }
  }
  if (get_opt_body_data_wrapper() || get_opt_body_file_wrapper())
  {
    if (!get_opt_method_wrapper())
    {
      fprintf(stderr, gettext("You must specify a method through --method=HTTPMethod to use with --body-data or --body-file.\n"));
      exit(WGET_EXIT_GENERIC_ERROR);
    }
    else
      if (get_opt_body_data_wrapper() && get_opt_body_file_wrapper())
    {
      fprintf(stderr, gettext("You cannot specify both --body-data and --body-file.\n"));
      exit(WGET_EXIT_GENERIC_ERROR);
    }
  }
  if (get_opt_method_wrapper() && (c_strcasecmp(get_opt_method_wrapper(), "HEAD") == 0))
    setoptval("spider", "1", "spider");
  if (get_opt_post_data_wrapper() || get_opt_post_file_name_wrapper())
  {
    setoptval("method", "POST", "method");
    if (get_opt_post_data_wrapper())
    {
      setoptval("bodydata", get_opt_post_data_wrapper(), "body-data");
      do
      {
        free((void *) get_opt_post_data_wrapper());
        set_opt_post_data_wrapper(0);
      }
      while (0);
    }
    else
    {
      setoptval("bodyfile", get_opt_post_file_name_wrapper(), "body-file");
      do
      {
        free((void *) get_opt_post_file_name_wrapper());
        set_opt_post_file_name_wrapper(0);
      }
      while (0);
    }
  }
  {
    iri temp_dummy_iri = get_dummy_iri_wrapper();
    memset(&temp_dummy_iri, 0, sizeof(dummy_iri));
    set_dummy_iri_wrapper(temp_dummy_iri);
  }
  if ((get_opt_enable_iri_wrapper() || get_opt_locale_wrapper()) || get_opt_encoding_remote_wrapper())
  {
    fprintf(stderr, gettext("This version does not have support for IRIs\n"));
    exit(WGET_EXIT_GENERIC_ERROR);
  }
  if (get_opt_ask_passwd_wrapper())
  {
    set_opt_passwd_wrapper(prompt_for_password());
    if ((get_opt_passwd_wrapper() == 0) || (get_opt_passwd_wrapper()[0] == '\0'))
      exit(WGET_EXIT_GENERIC_ERROR);
  }
  if (get_opt_background_wrapper())
    fork_to_background();
  if (get_opt_show_progress_wrapper())
    set_progress_implementation(get_opt_progress_type_wrapper());
  url = (char **) alloca((nurl + 1) * (sizeof(char *)));
  if (url == 0)
  {
    fprintf(stderr, gettext("Memory allocation problem\n"));
    exit(WGET_EXIT_PARSE_ERROR);
  }
  for (i = 0; i < nurl; i++, optind++)
  {
    char *rewritten = rewrite_shorthand_url(argv[optind]);
    if (rewritten)
      url[i] = rewritten;
    else
      url[i] = xstrdup(argv[optind]);
  }

  url[i] = 0;
  log_init(get_opt_lfilename_wrapper(), append_to_log);
  if (get_opt_warc_filename_wrapper() != 0)
    warc_init();
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("DEBUG output created by Wget %s on %s.\n\n", version_string, "linux-gnu");
    }
  }
  while (0);
  if (get_opt_output_document_wrapper())
  {
    if (((*get_opt_output_document_wrapper()) == '-') && (!(*(get_opt_output_document_wrapper() + 1))))
    {
      set_output_stream_wrapper(stdout);
    }
    else
    {
      struct stat st;
      set_output_stream_wrapper(fopen(get_opt_output_document_wrapper(), (get_opt_always_rest_wrapper()) ? ("ab") : ("wb")));
      if (get_output_stream_wrapper() == 0)
      {
        perror(get_opt_output_document_wrapper());
        exit(WGET_EXIT_GENERIC_ERROR);
      }
      if ((fstat(fileno(get_output_stream_wrapper()), &st) == 0) && S_ISREG(st.st_mode))
        set_output_stream_regular_wrapper(1);
    }
    if ((!get_output_stream_regular_wrapper()) && (get_opt_convert_links_wrapper() || get_opt_recursive_wrapper()))
    {
      fprintf(stderr, gettext("-k or -r can be used together with -O only if outputting to a regular file.\n"));
      exit(WGET_EXIT_GENERIC_ERROR);
    }
    if ((!get_output_stream_regular_wrapper()) && (get_opt_convert_links_wrapper() || get_opt_convert_file_only_wrapper()))
    {
      fprintf(stderr, gettext("--convert-links or --convert-file-only can be used together only if outputting to a regular file.\n"));
      exit(WGET_EXIT_GENERIC_ERROR);
    }
  }
  {
    main_sense_1_wrapper(url, t, nurl, timer, start_time);
  }
}



#endif /* TESTING */

/*
 * vim: et ts=2 sw=2
 */
