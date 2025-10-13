/* Reading/parsing the initialization file.
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
#include "exits.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
/* not all systems provide PATH_MAX in limits.h */
#ifndef PATH_MAX
# include <sys/param.h>
# ifndef PATH_MAX
#  define PATH_MAX MAXPATHLEN
# endif
#endif

#include <regex.h>
#ifdef HAVE_LIBPCRE
# include <pcre.h>
#endif

#ifdef HAVE_PWD_H
# include <pwd.h>
#endif
#include <assert.h>

#include "utils.h"
#include "init.h"
#include "host.h"
#include "netrc.h"
#include "progress.h"
#include "recur.h"              /* for INFINITE_RECURSION */
#include "convert.h"            /* for convert_cleanup */
#include "res.h"                /* for res_cleanup */
#include "http.h"               /* for http_cleanup */
#include "retr.h"               /* for output_stream */
#include "warc.h"               /* for warc_close */
#include "spider.h"             /* for spider_cleanup */
#include "html-url.h"           /* for cleanup_html_url */
#include "c-strcase.h"

#ifdef TESTING
#include "test.h"
#endif



#define CMD_DECLARE(func) static bool func (const char *, const char *, void *)

CMD_DECLARE (cmd_boolean);
CMD_DECLARE (cmd_bytes);
CMD_DECLARE (cmd_bytes_sum);
#ifdef HAVE_SSL
CMD_DECLARE (cmd_cert_type);
#endif
CMD_DECLARE (cmd_directory_vector);
CMD_DECLARE (cmd_number);
CMD_DECLARE (cmd_number_inf);
CMD_DECLARE (cmd_string);
CMD_DECLARE (cmd_string_uppercase);
CMD_DECLARE (cmd_file);
CMD_DECLARE (cmd_file_once);
CMD_DECLARE (cmd_directory);
CMD_DECLARE (cmd_time);
CMD_DECLARE (cmd_vector);

CMD_DECLARE (cmd_spec_dirstruct);
CMD_DECLARE (cmd_spec_header);
CMD_DECLARE (cmd_spec_warc_header);
CMD_DECLARE (cmd_spec_htmlify);
CMD_DECLARE (cmd_spec_mirror);
CMD_DECLARE (cmd_spec_prefer_family);
CMD_DECLARE (cmd_spec_progress);
CMD_DECLARE (cmd_spec_progressdisp);
CMD_DECLARE (cmd_spec_recursive);
CMD_DECLARE (cmd_spec_regex_type);
CMD_DECLARE (cmd_spec_restrict_file_names);
CMD_DECLARE (cmd_spec_report_speed);
#ifdef HAVE_SSL
CMD_DECLARE (cmd_spec_secure_protocol);
#endif
CMD_DECLARE (cmd_spec_timeout);
CMD_DECLARE (cmd_spec_useragent);
CMD_DECLARE (cmd_spec_verbose);
CMD_DECLARE (cmd_check_cert);

/* List of recognized commands, each consisting of name, place and
   function.  When adding a new command, simply add it to the list,
   but be sure to keep the list sorted alphabetically, as
   command_by_name's binary search depends on it.  Also, be sure to
   add any entries that allocate memory (e.g. cmd_string and
   cmd_vector) to the cleanup() function below. */

static const struct {
  const char *name;
  void *place;
  bool (*action) (const char *, const char *, void *);
} commands[] = {
  /* KEEP THIS LIST ALPHABETICALLY SORTED */
  { "accept",           &opt.accepts,           cmd_vector },
  { "acceptregex",      &opt.acceptregex_s,     cmd_string },
  { "addhostdir",       &opt.add_hostdir,       cmd_boolean },
  { "adjustextension",  &opt.adjust_extension,  cmd_boolean },
  { "alwaysrest",       &opt.always_rest,       cmd_boolean }, /* deprecated */
  { "askpassword",      &opt.ask_passwd,        cmd_boolean },
  { "authnochallenge",  &opt.auth_without_challenge,
                                                cmd_boolean },
  { "background",       &opt.background,        cmd_boolean },
  { "backupconverted",  &opt.backup_converted,  cmd_boolean },
  { "backups",          &opt.backups,           cmd_number },
  { "base",             &opt.base_href,         cmd_string },
  { "bindaddress",      &opt.bind_address,      cmd_string },
#ifdef HAVE_LIBCARES
  { "binddnsaddress",   &opt.bind_dns_address,  cmd_string },
#endif
  { "bodydata",         &opt.body_data,         cmd_string },
  { "bodyfile",         &opt.body_file,         cmd_string },
#ifdef HAVE_SSL
  { "cacertificate",    &opt.ca_cert,           cmd_file },
#endif
  { "cache",            &opt.allow_cache,       cmd_boolean },
#ifdef HAVE_SSL
  { "cadirectory",      &opt.ca_directory,      cmd_directory },
  { "certificate",      &opt.cert_file,         cmd_file },
  { "certificatetype",  &opt.cert_type,         cmd_cert_type },
  { "checkcertificate", &opt.check_cert,        cmd_check_cert },
#endif
  { "chooseconfig",     &opt.choose_config,     cmd_file },
  { "connecttimeout",   &opt.connect_timeout,   cmd_time },
  { "contentdisposition", &opt.content_disposition, cmd_boolean },
  { "contentonerror",   &opt.content_on_error,  cmd_boolean },
  { "continue",         &opt.always_rest,       cmd_boolean },
  { "convertfileonly",  &opt.convert_file_only, cmd_boolean },
  { "convertlinks",     &opt.convert_links,     cmd_boolean },
  { "cookies",          &opt.cookies,           cmd_boolean },
#ifdef HAVE_SSL
  { "crlfile",          &opt.crl_file,          cmd_file_once },
#endif
  { "cutdirs",          &opt.cut_dirs,          cmd_number },
  { "debug",            &opt.debug,             cmd_boolean },
  { "defaultpage",      &opt.default_page,      cmd_string },
  { "deleteafter",      &opt.delete_after,      cmd_boolean },
  { "dirprefix",        &opt.dir_prefix,        cmd_directory },
  { "dirstruct",        NULL,                   cmd_spec_dirstruct },
  { "dnscache",         &opt.dns_cache,         cmd_boolean },
#ifdef HAVE_LIBCARES
  { "dnsservers",       &opt.dns_servers,       cmd_string },
#endif
  { "dnstimeout",       &opt.dns_timeout,       cmd_time },
  { "domains",          &opt.domains,           cmd_vector },
  { "dotbytes",         &opt.dot_bytes,         cmd_bytes },
  { "dotsinline",       &opt.dots_in_line,      cmd_number },
  { "dotspacing",       &opt.dot_spacing,       cmd_number },
  { "dotstyle",         &opt.dot_style,         cmd_string }, /* deprecated */
#ifdef HAVE_SSL
  { "egdfile",          &opt.egd_file,          cmd_file },
#endif
  { "excludedirectories", &opt.excludes,        cmd_directory_vector },
  { "excludedomains",   &opt.exclude_domains,   cmd_vector },
  { "followftp",        &opt.follow_ftp,        cmd_boolean },
  { "followtags",       &opt.follow_tags,       cmd_vector },
  { "forcehtml",        &opt.force_html,        cmd_boolean },
  { "ftppasswd",        &opt.ftp_passwd,        cmd_string }, /* deprecated */
  { "ftppassword",      &opt.ftp_passwd,        cmd_string },
  { "ftpproxy",         &opt.ftp_proxy,         cmd_string },
#ifdef HAVE_SSL
  { "ftpscleardataconnection", &opt.ftps_clear_data_connection, cmd_boolean },
  { "ftpsfallbacktoftp", &opt.ftps_fallback_to_ftp, cmd_boolean },
  { "ftpsimplicit",     &opt.ftps_implicit,     cmd_boolean },
  { "ftpsresumessl",    &opt.ftps_resume_ssl,   cmd_boolean },
#endif
#ifdef __VMS
  { "ftpstmlf",         &opt.ftp_stmlf,         cmd_boolean },
#endif /* def __VMS */
  { "ftpuser",          &opt.ftp_user,          cmd_string },
  { "glob",             &opt.ftp_glob,          cmd_boolean },
  { "header",           NULL,                   cmd_spec_header },
#ifdef HAVE_HSTS
  { "hsts",             &opt.hsts,              cmd_boolean },
  { "hsts-file",        &opt.hsts_file,         cmd_file },
#endif
  { "htmlextension",    &opt.adjust_extension,  cmd_boolean }, /* deprecated */
  { "htmlify",          NULL,                   cmd_spec_htmlify },
  { "httpkeepalive",    &opt.http_keep_alive,   cmd_boolean },
  { "httppasswd",       &opt.http_passwd,       cmd_string }, /* deprecated */
  { "httppassword",     &opt.http_passwd,       cmd_string },
  { "httpproxy",        &opt.http_proxy,        cmd_string },
#ifdef HAVE_SSL
  { "httpsonly",        &opt.https_only,        cmd_boolean },
#endif
  { "httpsproxy",       &opt.https_proxy,       cmd_string },
  { "httpuser",         &opt.http_user,         cmd_string },
  { "if-modified-since", &opt.if_modified_since, cmd_boolean },
  { "ignorecase",       &opt.ignore_case,       cmd_boolean },
  { "ignorelength",     &opt.ignore_length,     cmd_boolean },
  { "ignoretags",       &opt.ignore_tags,       cmd_vector },
  { "includedirectories", &opt.includes,        cmd_directory_vector },
#ifdef ENABLE_IPV6
  { "inet4only",        &opt.ipv4_only,         cmd_boolean },
  { "inet6only",        &opt.ipv6_only,         cmd_boolean },
#endif
  { "input",            &opt.input_filename,    cmd_file },
#ifdef HAVE_METALINK
  { "input-metalink",   &opt.input_metalink,    cmd_file },
#endif
  { "iri",              &opt.enable_iri,        cmd_boolean },
  { "keepsessioncookies", &opt.keep_session_cookies, cmd_boolean },
  { "limitrate",        &opt.limit_rate,        cmd_bytes },
  { "loadcookies",      &opt.cookies_input,     cmd_file },
  { "localencoding",    &opt.locale,            cmd_string },
  { "logfile",          &opt.lfilename,         cmd_file },
  { "login",            &opt.ftp_user,          cmd_string },/* deprecated*/
  { "maxredirect",      &opt.max_redirect,      cmd_number },
#ifdef HAVE_METALINK
  { "metalink-over-http", &opt.metalink_over_http, cmd_boolean },
#endif
  { "method",           &opt.method,            cmd_string_uppercase },
  { "mirror",           NULL,                   cmd_spec_mirror },
  { "netrc",            &opt.netrc,             cmd_boolean },
  { "noclobber",        &opt.noclobber,         cmd_boolean },
  { "noconfig",         &opt.noconfig,          cmd_boolean },
  { "noparent",         &opt.no_parent,         cmd_boolean },
  { "noproxy",          &opt.no_proxy,          cmd_vector },
  { "numtries",         &opt.ntry,              cmd_number_inf },/* deprecated*/
  { "outputdocument",   &opt.output_document,   cmd_file },
  { "pagerequisites",   &opt.page_requisites,   cmd_boolean },
  { "passiveftp",       &opt.ftp_pasv,          cmd_boolean },
  { "passwd",           &opt.ftp_passwd,        cmd_string },/* deprecated*/
  { "password",         &opt.passwd,            cmd_string },
#ifdef HAVE_SSL
  { "pinnedpubkey",     &opt.pinnedpubkey,      cmd_string },
#endif
  { "postdata",         &opt.post_data,         cmd_string },
  { "postfile",         &opt.post_file_name,    cmd_file },
  { "preferfamily",     NULL,                   cmd_spec_prefer_family },
#ifdef HAVE_METALINK
  { "preferred-location", &opt.preferred_location, cmd_string },
#endif
  { "preservepermissions", &opt.preserve_perm,  cmd_boolean },
#ifdef HAVE_SSL
  { "privatekey",       &opt.private_key,       cmd_file },
  { "privatekeytype",   &opt.private_key_type,  cmd_cert_type },
#endif
  { "progress",         &opt.progress_type,     cmd_spec_progress },
  { "protocoldirectories", &opt.protocol_directories, cmd_boolean },
  { "proxypasswd",      &opt.proxy_passwd,      cmd_string }, /* deprecated */
  { "proxypassword",    &opt.proxy_passwd,      cmd_string },
  { "proxyuser",        &opt.proxy_user,        cmd_string },
  { "quiet",            &opt.quiet,             cmd_boolean },
  { "quota",            &opt.quota,             cmd_bytes_sum },
#ifdef HAVE_SSL
  { "randomfile",       &opt.random_file,       cmd_file },
#endif
  { "randomwait",       &opt.random_wait,       cmd_boolean },
  { "readtimeout",      &opt.read_timeout,      cmd_time },
  { "reclevel",         &opt.reclevel,          cmd_number_inf },
  { "recursive",        NULL,                   cmd_spec_recursive },
  { "referer",          &opt.referer,           cmd_string },
  { "regextype",        &opt.regex_type,        cmd_spec_regex_type },
  { "reject",           &opt.rejects,           cmd_vector },
  { "rejectedlog",      &opt.rejected_log,      cmd_file },
  { "rejectregex",      &opt.rejectregex_s,     cmd_string },
  { "relativeonly",     &opt.relative_only,     cmd_boolean },
  { "remoteencoding",   &opt.encoding_remote,   cmd_string },
  { "removelisting",    &opt.remove_listing,    cmd_boolean },
  { "reportspeed",             &opt.report_bps, cmd_spec_report_speed},
  { "restrictfilenames", NULL,                  cmd_spec_restrict_file_names },
  { "retrsymlinks",     &opt.retr_symlinks,     cmd_boolean },
  { "retryconnrefused", &opt.retry_connrefused, cmd_boolean },
  { "robots",           &opt.use_robots,        cmd_boolean },
  { "savecookies",      &opt.cookies_output,    cmd_file },
  { "saveheaders",      &opt.save_headers,      cmd_boolean },
#ifdef HAVE_SSL
  { "secureprotocol",   &opt.secure_protocol,   cmd_spec_secure_protocol },
#endif
  { "serverresponse",   &opt.server_response,   cmd_boolean },
  { "showalldnsentries", &opt.show_all_dns_entries, cmd_boolean },
  { "showprogress",     &opt.show_progress,     cmd_spec_progressdisp },
  { "spanhosts",        &opt.spanhost,          cmd_boolean },
  { "spider",           &opt.spider,            cmd_boolean },
  { "startpos",         &opt.start_pos,         cmd_bytes },
  { "strictcomments",   &opt.strict_comments,   cmd_boolean },
  { "timeout",          NULL,                   cmd_spec_timeout },
  { "timestamping",     &opt.timestamping,      cmd_boolean },
  { "tries",            &opt.ntry,              cmd_number_inf },
  { "trustservernames", &opt.trustservernames,  cmd_boolean },
  { "unlink",           &opt.unlink,            cmd_boolean },
  { "useproxy",         &opt.use_proxy,         cmd_boolean },
  { "user",             &opt.user,              cmd_string },
  { "useragent",        NULL,                   cmd_spec_useragent },
  { "useservertimestamps", &opt.useservertimestamps, cmd_boolean },
  { "verbose",          NULL,                   cmd_spec_verbose },
  { "wait",             &opt.wait,              cmd_time },
  { "waitretry",        &opt.waitretry,         cmd_time },
  { "warccdx",          &opt.warc_cdx_enabled,  cmd_boolean },
  { "warccdxdedup",     &opt.warc_cdx_dedup_filename,  cmd_file },
#ifdef HAVE_LIBZ
  { "warccompression",  &opt.warc_compression_enabled, cmd_boolean },
#endif
  { "warcdigests",      &opt.warc_digests_enabled, cmd_boolean },
  { "warcfile",         &opt.warc_filename,     cmd_file },
  { "warcheader",       NULL,                   cmd_spec_warc_header },
  { "warckeeplog",      &opt.warc_keep_log,     cmd_boolean },
  { "warcmaxsize",      &opt.warc_maxsize,      cmd_bytes },
  { "warctempdir",      &opt.warc_tempdir,      cmd_directory },
#ifdef USE_WATT32
  { "wdebug",           &opt.wdebug,            cmd_boolean },
#endif
};

/* Look up CMDNAME in the commands[] and return its position in the
   array.  If CMDNAME is not found, return -1.  */



/* Reset the variables to default values.  */


/* Return the user's home directory (strdup-ed), or NULL if none is
   found.  */
char *
home_dir (void)
{
  static char *buf = NULL;
  static char *home, *ret;

  if (!home)
    {
      home = getenv ("HOME");
      if (!home)
        {
#if defined(MSDOS)
          int len;

          /* Under MSDOS, if $HOME isn't defined, use the directory where
             `wget.exe' resides.  */
          const char *_w32_get_argv0 (void); /* in libwatt.a/pcconfig.c */
          char *p;

          buff = _w32_get_argv0 ();

          p = strrchr (buf, '/');            /* djgpp */
          if (!p)
            p = strrchr (buf, '\\');          /* others */
          assert (p);

          len = p - buff + 1;
          buff = strdup (_w32_get_argv0 ());

          home = buf;
#elif !defined(WINDOWS)
          /* If HOME is not defined, try getting it from the password
             file.  */
          struct passwd *pwd = getpwuid (getuid ());
          if (!pwd || !pwd->pw_dir)
            return NULL;
          home = pwd->pw_dir;
#else  /* !WINDOWS */
          /* Under Windows, if $HOME isn't defined, use the directory where
             `wget.exe' resides.  */
          home = ws_mypath ();
#endif /* WINDOWS */
        }
    }

  ret = home ? xstrdup (home) : NULL;
  xfree (buf);

  return ret;
}

/* Check the 'WGETRC' environment variable and return the file name
   if  'WGETRC' is set and is a valid file.
   If the `WGETRC' variable exists but the file does not exist, the
   function will exit().  */


/* Check for the existance of '$HOME/.wgetrc' and return its path
   if it exists and is set.  */


/* Return the path to the user's .wgetrc.  This is either the value of
   `WGETRC' environment variable, or `$HOME/.wgetrc'.

   Additionally, for windows, look in the directory where wget.exe
   resides.  */


/* Return values of parse_line. */
enum parse_line {
  line_ok,
  line_empty,
  line_syntax_error,
  line_unknown_command
};

static enum parse_line parse_line (const char *, char **, char **, int *);
static bool setval_internal (int, const char *, const char *);
static bool setval_internal_tilde (int, const char *, const char *);

/* Initialize variables from a wgetrc file.  Returns zero (failure) if
   there were errors in the file.  */



/* Initialize the defaults and run the system wgetrc and user's own
   wgetrc.  */


/* Remove dashes and underscores from S, modifying S in the
   process. */



/* Parse the line pointed by line, with the syntax:
   <sp>* command <sp>* = <sp>* value <sp>*
   Uses malloc to allocate space for command and value.

   Returns one of line_ok, line_empty, line_syntax_error, or
   line_unknown_command.

   In case of line_ok, *COM and *VAL point to freshly allocated
   strings, and *COMIND points to com's index.  In case of error or
   empty line, their values are unmodified.  */



#if defined(WINDOWS) || defined(MSDOS)
# define ISSEP(c) ((c) == '/' || (c) == '\\')
#else
# define ISSEP(c) ((c) == '/')
#endif

/* Run commands[comind].action. */





/* Run command COM with value VAL.  If running the command produces an
   error, report the error and exit.

   This is intended to be called from main to modify Wget's behavior
   through command-line switches.  Since COM is hard-coded in main,
   it is not canonicalized, and this aborts when COM is not found.

   If COMIND's are exported to init.h, this function will be changed
   to accept COMIND directly.  */



/* Parse OPT into command and value and run it.  For example,
   run_command("foo=bar") is equivalent to setoptval("foo", "bar").
   This is used by the `--execute' flag in main.c.  */



/* Generic helper functions, for use with `commands'. */

/* Forward declarations: */
struct decode_item {
  const char *name;
  int code;
};
static bool decode_string (const char *, const struct decode_item *, int, int *);
static bool simple_atoi (const char *, const char *, int *);
static bool simple_atof (const char *, const char *, double *);

#define CMP1(p, c0) (c_tolower((p)[0]) == (c0) && (p)[1] == '\0')

#define CMP2(p, c0, c1) (c_tolower((p)[0]) == (c0)        \
                         && c_tolower((p)[1]) == (c1)     \
                         && (p)[2] == '\0')

#define CMP3(p, c0, c1, c2) (c_tolower((p)[0]) == (c0)    \
                     && c_tolower((p)[1]) == (c1)         \
                     && c_tolower((p)[2]) == (c2)         \
                     && (p)[3] == '\0')


static int
cmd_boolean_internal (const char *com _GL_UNUSED, const char *val, void *place _GL_UNUSED)
{
  if (CMP2 (val, 'o', 'n') || CMP3 (val, 'y', 'e', 's') || CMP1 (val, '1'))
    /* "on", "yes" and "1" mean true. */
    return 1;
  else if (CMP3 (val, 'o', 'f', 'f') || CMP2 (val, 'n', 'o') || CMP1 (val, '0'))
    /* "off", "no" and "0" mean false. */
    return 0;
  return -1;
}

/* Store the boolean value from VAL to PLACE.  COM is ignored,
   except for error messages.  */


/* Store the check_cert value from VAL to PLACE.  COM is ignored,
   except for error messages.  */


/* Set the non-negative integer value from VAL to PLACE.  With
   incorrect specification, the number remains unchanged.  */


/* Similar to cmd_number(), only accepts `inf' as a synonym for 0.  */


/* Copy (strdup) the string at COM to a new location and place a
   pointer to *PLACE.  */


/* Like cmd_string but ensure the string is upper case.  */



/* Like cmd_string, but handles tilde-expansion when reading a user's
   `.wgetrc'.  In that case, and if VAL begins with `~', the tilde
   gets expanded to the user's home directory.  */


/* like cmd_file, but insist on just a single option usage */


/* Like cmd_file, but strips trailing '/' characters.  */


/* Split VAL by space to a vector of values, and append those values
   to vector pointed to by the PLACE argument.  If VAL is empty, the
   PLACE vector is cleared instead.  */





/* Engine for cmd_bytes and cmd_bytes_sum: converts a string such as
   "100k" or "2.5G" to a floating point number.  */



/* Parse VAL as a number and set its value to PLACE (which should
   point to a wgint).

   By default, the value is assumed to be in bytes.  If "K", "M", or
   "G" are appended, the value is multiplied with 1<<10, 1<<20, or
   1<<30, respectively.  Floating point values are allowed and are
   cast to integer before use.  The idea is to be able to use things
   like 1.5k instead of "1536".

   The string "inf" is returned as 0.

   In case of error, false is returned and memory pointed to by PLACE
   remains unmodified.  */



/* Like cmd_bytes, but PLACE is interpreted as a pointer to
   SIZE_SUM.  It works by converting the string to double, therefore
   working with values up to 2^53-1 without loss of precision.  This
   value (8192 TB) is large enough to serve for a while.  */



/* Store the value of VAL to *OUT.  The value is a time period, by
   default expressed in seconds, but also accepting suffixes "m", "h",
   "d", and "w" for minutes, hours, days, and weeks respectively.  */



#ifdef HAVE_SSL

#endif

/* Specialized helper functions, used by `commands' to handle some
   options specially.  */

static bool check_user_specified_header (const char *);









/* Set the "mirror" mode.  It means: recursive download, timestamping,
   no limit on max. recursion depth, and don't remove listings.  */



/* Validate --prefer-family and set the choice.  Allowed values are
   "IPv4", "IPv6", and "none".  */



/* Set progress.type to VAL, but verify that it's a valid progress
   implementation before that.  */



/* Set opt.recursive to VAL as with cmd_boolean.  If opt.recursive is
   set to true, also set opt.dirstruct to true, unless opt.no_dirstruct
   is specified.  */



/* Validate --regex-type and set the choice.  */







#ifdef HAVE_SSL

#endif

/* Set all three timeout values. */





/* The --show-progress option is not a cmd_boolean since we need to keep track
 * of whether the user explicitly requested the option or not. -1 means
 * uninitialized. */



/* The "verbose" option cannot be cmd_boolean because the variable is
   not bool -- it's of type int (-1 means uninitialized because of
   some random hackery for disallowing -q -v).  */



/* Miscellaneous useful routines.  */

/* A very simple atoi clone, more useful than atoi because it works on
   delimited strings, and has error reportage.  Returns true on success,
   false on failure.  If successful, stores result to *DEST.  */



/* Trivial atof, with error reporting.  Handles "<digits>[.<digits>]",
   doesn't handle exponential notation.  Returns true on success,
   false on failure.  In case of success, stores its result to
   *DEST.  */



/* Verify that the user-specified header in S is valid.  It must
   contain a colon preceded by non-white-space characters and must not
   contain newlines.  */



/* Decode VAL into a number, according to ITEMS. */



/* Free the memory allocated by global variables.  */
void
cleanup (void)
{
  /* Free external resources, close files, etc. */

  /* Close WARC file. */
  if (opt.warc_filename != 0)
    warc_close ();

  log_close ();

  if (output_stream)
    if (fclose (output_stream) == EOF)
      inform_exit_status (CLOSEFAILED);

  /* No need to check for error because Wget flushes its output (and
     checks for errors) after any data arrives.  */

  /* We're exiting anyway so there's no real need to call free()
     hundreds of times.  Skipping the frees will make Wget exit
     faster.

     However, when detecting leaks, it's crucial to free() everything
     because then you can find the real leaks, i.e. the allocated
     memory which grows with the size of the program.  */

#ifdef DEBUG_MALLOC
  convert_cleanup ();
  res_cleanup ();
  http_cleanup ();
  cleanup_html_url ();
  spider_cleanup ();
  host_cleanup ();
  log_cleanup ();
  netrc_cleanup ();

  xfree (opt.choose_config);
  xfree (opt.lfilename);
  xfree (opt.dir_prefix);
  xfree (opt.input_filename);
#ifdef HAVE_METALINK
  xfree (opt.input_metalink);
  xfree (opt.preferred_location);
#endif
  xfree (opt.output_document);
  free_vec (opt.accepts);
  free_vec (opt.rejects);
  free_vec ((char **)opt.excludes);
  free_vec ((char **)opt.includes);
  free_vec (opt.domains);
  free_vec (opt.follow_tags);
  free_vec (opt.ignore_tags);
  xfree (opt.progress_type);
  xfree (opt.ftp_user);
  xfree (opt.ftp_passwd);
  xfree (opt.ftp_proxy);
  xfree (opt.https_proxy);
  xfree (opt.http_proxy);
  free_vec (opt.no_proxy);
  xfree (opt.useragent);
  xfree (opt.referer);
  xfree (opt.http_user);
  xfree (opt.http_passwd);
  free_vec (opt.user_headers);
  free_vec (opt.warc_user_headers);
# ifdef HAVE_SSL
  xfree (opt.cert_file);
  xfree (opt.private_key);
  xfree (opt.ca_directory);
  xfree (opt.ca_cert);
  xfree (opt.crl_file);
  xfree (opt.random_file);
  xfree (opt.egd_file);
# endif
  xfree (opt.bind_address);
  xfree (opt.cookies_input);
  xfree (opt.cookies_output);
  xfree (opt.user);
  xfree (opt.passwd);
  xfree (opt.base_href);
  xfree (opt.method);
  xfree (opt.post_file_name);
  xfree (opt.post_data);
  xfree (opt.body_data);
  xfree (opt.body_file);
  xfree (opt.rejected_log);

#ifdef HAVE_LIBCARES
#include <ares.h>
  {
    extern ares_channel ares;

    xfree (opt.bind_dns_address);
    xfree (opt.dns_servers);
    ares_destroy (ares);
    ares_library_cleanup ();
  }
#endif

#endif /* DEBUG_MALLOC */
}

/* Unit testing routines.  */

#ifdef TESTING

const char *
test_commands_sorted(void)
{
  unsigned i;

  for (i = 1; i < countof(commands); ++i)
    {
      if (c_strcasecmp (commands[i - 1].name, commands[i].name) > 0)
        {
          mu_assert ("FAILED", false);
          break;
        }
    }
  return NULL;
}

const char *
test_cmd_spec_restrict_file_names(void)
{
  unsigned i;
  static const struct {
    const char *val;
    int expected_restrict_files_os;
    bool expected_restrict_files_ctrl;
    int expected_restrict_files_case;
    bool result;
  } test_array[] = {
    { "windows", restrict_windows, true, restrict_no_case_restriction, true },
    { "windows,", restrict_windows, true, restrict_no_case_restriction, true },
    { "windows,lowercase", restrict_windows, true, restrict_lowercase, true },
    { "unix,nocontrol,lowercase,", restrict_unix, false, restrict_lowercase, true },
  };

  for (i = 0; i < countof(test_array); ++i)
    {
      bool res;

      defaults();
      res = cmd_spec_restrict_file_names ("dummy", test_array[i].val, NULL);

      /*
      fprintf (stderr, "test_cmd_spec_restrict_file_names: TEST %d\n", i); fflush (stderr);
      fprintf (stderr, "opt.restrict_files_os: %d\n",   opt.restrict_files_os); fflush (stderr);
      fprintf (stderr, "opt.restrict_files_ctrl: %d\n", opt.restrict_files_ctrl); fflush (stderr);
      fprintf (stderr, "opt.restrict_files_case: %d\n", opt.restrict_files_case); fflush (stderr);
      */
      mu_assert ("test_cmd_spec_restrict_file_names: wrong result",
                 res == test_array[i].result
                 && (int) opt.restrict_files_os   == test_array[i].expected_restrict_files_os
                 && opt.restrict_files_ctrl == test_array[i].expected_restrict_files_ctrl
                 && (int) opt.restrict_files_case == test_array[i].expected_restrict_files_case);
    }

  return NULL;
}

#endif /* TESTING */
