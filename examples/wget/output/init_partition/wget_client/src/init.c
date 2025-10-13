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

#include "wget_rpc_wrapper.h"

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

static int
command_by_name (const char *cmdname)
{
  /* Use binary search for speed.  Wget has ~100 commands, which
     guarantees a worst case performance of 7 string comparisons.  */
  int lo = 0, hi = countof (commands) - 1;

  while (lo <= hi)
    {
      int mid = (lo + hi) >> 1;
      int cmp = c_strcasecmp (cmdname, commands[mid].name);
      if (cmp < 0)
        hi = mid - 1;
      else if (cmp > 0)
        lo = mid + 1;
      else
        return mid;
    }
  return -1;
}

/* Reset the variables to default values.  */
void defaults(void)
{
  char *tmp;
  {
    options temp_opt = get_opt_wrapper();
    memset(&temp_opt, '\0', sizeof(opt));
    set_opt_wrapper(temp_opt);
  }
  set_opt_cookies_wrapper(1);
  set_opt_verbose_wrapper(-1);
  set_opt_ntry_wrapper(20);
  set_opt_reclevel_wrapper(5);
  set_opt_add_hostdir_wrapper(1);
  set_opt_netrc_wrapper(1);
  set_opt_ftp_glob_wrapper(1);
  set_opt_htmlify_wrapper(1);
  set_opt_http_keep_alive_wrapper(1);
  set_opt_use_proxy_wrapper(1);
  set_opt_convert_file_only_wrapper(0);
  tmp = getenv("no_proxy");
  if (tmp)
    set_opt_no_proxy_wrapper(sepstring(tmp));
  set_opt_prefer_family_wrapper(prefer_none);
  set_opt_allow_cache_wrapper(1);
  set_opt_if_modified_since_wrapper(1);
  set_opt_read_timeout_wrapper(900);
  set_opt_use_robots_wrapper(1);
  set_opt_remove_listing_wrapper(1);
  set_opt_dot_bytes_wrapper(1024);
  set_opt_dot_spacing_wrapper(10);
  set_opt_dots_in_line_wrapper(50);
  set_opt_dns_cache_wrapper(1);
  set_opt_ftp_pasv_wrapper(1);
  set_opt_retr_symlinks_wrapper(1);
  set_opt_check_cert_wrapper(CHECK_CERT_ON);
  set_opt_ftps_resume_ssl_wrapper(1);
  set_opt_ftps_fallback_to_ftp_wrapper(0);
  set_opt_ftps_implicit_wrapper(0);
  set_opt_ftps_clear_data_connection_wrapper(0);
  set_opt_restrict_files_os_wrapper(restrict_unix);
  set_opt_restrict_files_ctrl_wrapper(1);
  set_opt_restrict_files_nonascii_wrapper(0);
  set_opt_restrict_files_case_wrapper(restrict_no_case_restriction);
  set_opt_regex_type_wrapper(regex_type_posix);
  set_opt_max_redirect_wrapper(20);
  set_opt_waitretry_wrapper(10);
  set_opt_enable_iri_wrapper(0);
  set_opt_locale_wrapper(0);
  set_opt_encoding_remote_wrapper(0);
  set_opt_useservertimestamps_wrapper(1);
  set_opt_show_all_dns_entries_wrapper(0);
  set_opt_warc_maxsize_wrapper(0);
  set_opt_warc_compression_enabled_wrapper(1);
  set_opt_warc_digests_enabled_wrapper(1);
  set_opt_warc_cdx_enabled_wrapper(0);
  set_opt_warc_cdx_dedup_filename_wrapper(0);
  set_opt_warc_tempdir_wrapper(0);
  set_opt_warc_keep_log_wrapper(1);
  set_opt_start_pos_wrapper(-1);
  set_opt_show_progress_wrapper(-1);
  set_opt_noscroll_wrapper(0);
  set_opt_hsts_wrapper(1);
}



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
char *wgetrc_env_file_name(void)
{
  char *env = getenv("WGETRC");
  if (env && (*env))
  {
    if (!file_exists_p(env))
    {
      fprintf(stderr, gettext("%s: WGETRC points to %s, which doesn't exist.\n"), get_exec_name_wrapper(), env);
      exit(WGET_EXIT_GENERIC_ERROR);
    }
    return xstrdup(env);
  }
  return 0;
}



/* Check for the existance of '$HOME/.wgetrc' and return its path
   if it exists and is set.  */
char *
wgetrc_user_file_name (void)
{
  char *home;
  char *file = NULL;
  /* If that failed, try $HOME/.wgetrc (or equivalent).  */

#ifdef __VMS
  file = "SYS$LOGIN:.wgetrc";
#else /* def __VMS */
  home = home_dir ();
  if (home)
    file = aprintf ("%s/.wgetrc", home);
  xfree (home);
#endif /* def __VMS [else] */

  if (!file)
    return NULL;
  if (!file_exists_p (file))
    {
      xfree (file);
      return NULL;
    }
  return file;
}

/* Return the path to the user's .wgetrc.  This is either the value of
   `WGETRC' environment variable, or `$HOME/.wgetrc'.

   Additionally, for windows, look in the directory where wget.exe
   resides.  */
char *
wgetrc_file_name (void)
{
  char *file = wgetrc_env_file_name ();
  if (file && *file)
    return file;

  file = wgetrc_user_file_name ();

#ifdef WINDOWS
  /* Under Windows, if we still haven't found .wgetrc, look for the file
     `wget.ini' in the directory where `wget.exe' resides; we do this for
     backward compatibility with previous versions of Wget.
     SYSTEM_WGETRC should not be defined under WINDOWS.  */
  if (!file)
    {
      char *home = ws_mypath ();
      if (home)
        {
          file = aprintf ("%s/wget.ini", home);
          if (!file_exists_p (file))
            {
              xfree (file);
            }
          xfree (home);
        }
    }
#endif /* WINDOWS */

  return file;
}

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

bool run_wgetrc(const char *file)
{
  FILE *fp;
  char *line = 0;
  size_t bufsize = 0;
  int ln;
  int errcnt = 0;
  fp = fopen(file, "r");
  if (!fp)
  {
    fprintf(stderr, gettext("%s: Cannot read %s (%s).\n"), get_exec_name_wrapper(), file, strerror(errno));
    return 1;
  }
  ln = 1;
  while (getline(&line, &bufsize, fp) > 0)
  {
    char *com = 0;
    char *val = 0;
    int comind;
    switch (parse_line(line, &com, &val, &comind))
    {
      case line_ok:
        if (!setval_internal_tilde(comind, com, val))
      {
        fprintf(stderr, gettext("%s: Error in %s at line %d.\n"), get_exec_name_wrapper(), file, ln);
        ++errcnt;
      }
        break;

      case line_syntax_error:
        fprintf(stderr, gettext("%s: Syntax error in %s at line %d.\n"), get_exec_name_wrapper(), file, ln);
        ++errcnt;
        break;

      case line_unknown_command:
        fprintf(stderr, gettext("%s: Unknown command %s in %s at line %d.\n"), get_exec_name_wrapper(), quote(com), file, ln);
        ++errcnt;
        break;

      case line_empty:
        break;

      default:
        abort();

    }

    do
    {
      free((void *) com);
      com = 0;
    }
    while (0);
    do
    {
      free((void *) val);
      val = 0;
    }
    while (0);
    ++ln;
  }

  do
  {
    free((void *) line);
    line = 0;
  }
  while (0);
  fclose(fp);
  return errcnt == 0;
}



/* Initialize the defaults and run the system wgetrc and user's own
   wgetrc.  */
void initialize(void)
{
  char *file;
  char *env_sysrc;
  bool ok = 1;
  env_sysrc = getenv("SYSTEM_WGETRC");
  if (env_sysrc && file_exists_p(env_sysrc))
  {
    ok &= run_wgetrc(env_sysrc);
    if (!ok)
    {
      fprintf(stderr, gettext("Parsing system wgetrc file (env SYSTEM_WGETRC) failed.  Please check\n'%s',\nor specify a different file using --config.\n"), env_sysrc);
      exit(WGET_EXIT_PARSE_ERROR);
    }
  }
  else
    if (file_exists_p("/usr/local/etc/wgetrc"))
    ok &= run_wgetrc("/usr/local/etc/wgetrc");
  if (!ok)
  {
    fprintf(stderr, gettext("Parsing system wgetrc file failed.  Please check\n'%s',\nor specify a different file using --config.\n"), "/usr/local/etc/wgetrc");
    exit(WGET_EXIT_PARSE_ERROR);
  }
  file = wgetrc_file_name();
  if (!file)
    return;
  if (!strcmp(file, "/usr/local/etc/wgetrc"))
  {
    fprintf(stderr, gettext("%s: Warning: Both system and user wgetrc point to %s.\n"), get_exec_name_wrapper(), quote(file));
  }
  else
    ok &= run_wgetrc(file);
  if (!ok)
    exit(WGET_EXIT_PARSE_ERROR);
  do
  {
    free((void *) file);
    file = 0;
  }
  while (0);
  return;
}



/* Remove dashes and underscores from S, modifying S in the
   process. */

static void
dehyphen (char *s)
{
  char *t = s;                  /* t - tortoise */
  char *h = s;                  /* h - hare     */
  while (*h)
    if (*h == '_' || *h == '-')
      ++h;
    else
      *t++ = *h++;
  *t = '\0';
}

/* Parse the line pointed by line, with the syntax:
   <sp>* command <sp>* = <sp>* value <sp>*
   Uses malloc to allocate space for command and value.

   Returns one of line_ok, line_empty, line_syntax_error, or
   line_unknown_command.

   In case of line_ok, *COM and *VAL point to freshly allocated
   strings, and *COMIND points to com's index.  In case of error or
   empty line, their values are unmodified.  */

static enum parse_line
parse_line (const char *line, char **com, char **val, int *comind)
{
  const char *p;
  const char *end = line + strlen (line);
  const char *cmdstart, *cmdend;
  const char *valstart, *valend;

  char *cmdcopy;
  int ind;

  /* Skip leading and trailing whitespace.  */
  while (*line && c_isspace (*line))
    ++line;
  while (end > line && c_isspace (end[-1]))
    --end;

  /* Skip empty lines and comments.  */
  if (!*line || *line == '#')
    return line_empty;

  p = line;

  cmdstart = p;
  while (p < end && (c_isalnum (*p) || *p == '_' || *p == '-'))
    ++p;
  cmdend = p;

  /* Skip '=', as well as any space before or after it. */
  while (p < end && c_isspace (*p))
    ++p;
  if (p == end || *p != '=')
    return line_syntax_error;
  ++p;
  while (p < end && c_isspace (*p))
    ++p;

  valstart = p;
  valend   = end;

  /* The syntax is valid (even though the command might not be).  Fill
     in the command name and value.  */
  *com = strdupdelim (cmdstart, cmdend);
  *val = strdupdelim (valstart, valend);

  /* The line now known to be syntactically correct.  Check whether
     the command is valid.  */
  BOUNDED_TO_ALLOCA (cmdstart, cmdend, cmdcopy);
  dehyphen (cmdcopy);
  ind = command_by_name (cmdcopy);
  if (ind == -1)
    return line_unknown_command;

  /* Report success to the caller. */
  *comind = ind;
  return line_ok;
}

#if defined(WINDOWS) || defined(MSDOS)
# define ISSEP(c) ((c) == '/' || (c) == '\\')
#else
# define ISSEP(c) ((c) == '/')
#endif

/* Run commands[comind].action. */

static bool setval_internal(int comind, const char *com, const char *val)
{
  assert((0 <= comind) && (((size_t) comind) < ((sizeof(commands)) / (sizeof(commands[0])))));
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("Setting %s (%s) to %s\n", com, commands[comind].name, val);
    }
  }
  while (0);
  return commands[comind].action(com, val, commands[comind].place);
}



static bool
setval_internal_tilde (int comind, const char *com, const char *val)
{
  bool ret;
  int homelen;
  char *home;
  char **pstring;
  ret = setval_internal (comind, com, val);

  /* We make tilde expansion for cmd_file and cmd_directory */
  if (((commands[comind].action == cmd_file) ||
       (commands[comind].action == cmd_directory))
      && ret && (*val == '~' && ISSEP (val[1])))
    {
      pstring = commands[comind].place;
      home = home_dir ();
      if (home)
        {
          homelen = strlen (home);
          while (homelen && ISSEP (home[homelen - 1]))
                 home[--homelen] = '\0';

          /* Skip the leading "~/". */
          for (++val; ISSEP (*val); val++)
            ;
          *pstring = concat_strings (home, "/", val, (char *)0);
        }
    }
  return ret;
}

/* Run command COM with value VAL.  If running the command produces an
   error, report the error and exit.

   This is intended to be called from main to modify Wget's behavior
   through command-line switches.  Since COM is hard-coded in main,
   it is not canonicalized, and this aborts when COM is not found.

   If COMIND's are exported to init.h, this function will be changed
   to accept COMIND directly.  */

void
setoptval (const char *com, const char *val, const char *optname)
{
  /* Prepend "--" to OPTNAME. */
  char *dd_optname = (char *) alloca (2 + strlen (optname) + 1);
  dd_optname[0] = '-';
  dd_optname[1] = '-';
  strcpy (dd_optname + 2, optname);

  assert (val != NULL);
  if (!setval_internal (command_by_name (com), dd_optname, val))
    exit (WGET_EXIT_PARSE_ERROR);
}

/* Parse OPT into command and value and run it.  For example,
   run_command("foo=bar") is equivalent to setoptval("foo", "bar").
   This is used by the `--execute' flag in main.c.  */

void run_command(const char *cmdopt)
{
  char *com;
  char *val;
  int comind;
  switch (parse_line(cmdopt, &com, &val, &comind))
  {
    case line_ok:
      if (!setval_internal(comind, com, val))
      exit(WGET_EXIT_PARSE_ERROR);
      do
    {
      free((void *) com);
      com = 0;
    }
    while (0);
      do
    {
      free((void *) val);
      val = 0;
    }
    while (0);
      break;

    default:
      fprintf(stderr, gettext("%s: Invalid --execute command %s\n"), get_exec_name_wrapper(), quote(cmdopt));
      exit(WGET_EXIT_PARSE_ERROR);

  }

}



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
static bool cmd_boolean(const char *com, const char *val, void *place)
{
  bool value;
  switch (cmd_boolean_internal(com, val, place))
  {
    case 0:
      value = 0;
      break;

    case 1:
      value = 1;
      break;

    default:
    {
      fprintf(stderr, gettext("%s: %s: Invalid boolean %s; use `on' or `off'.\n"), get_exec_name_wrapper(), com, quote(val));
      return 0;
    }

  }

  *((bool *) place) = value;
  return 1;
}



/* Store the check_cert value from VAL to PLACE.  COM is ignored,
   except for error messages.  */
static bool cmd_check_cert(const char *com, const char *val, void *place)
{
  int value;
  switch (cmd_boolean_internal(com, val, place))
  {
    case 0:
      value = CHECK_CERT_OFF;
      break;

    case 1:
      value = CHECK_CERT_ON;
      break;

    default:
    {
      if (!c_strcasecmp(val, "quiet"))
        value = CHECK_CERT_QUIET;
      else
      {
        fprintf(stderr, gettext("%s: %s: Invalid %s; use `on', `off' or `quiet'.\n"), get_exec_name_wrapper(), com, quote(val));
        return 0;
      }
    }

  }

  *((int *) place) = value;
  return 1;
}



/* Set the non-negative integer value from VAL to PLACE.  With
   incorrect specification, the number remains unchanged.  */
static bool cmd_number(const char *com, const char *val, void *place)
{
  if ((!simple_atoi(val, val + strlen(val), place)) || ((*((int *) place)) < 0))
  {
    fprintf(stderr, gettext("%s: %s: Invalid number %s.\n"), get_exec_name_wrapper(), com, quote(val));
    return 0;
  }
  return 1;
}



/* Similar to cmd_number(), only accepts `inf' as a synonym for 0.  */
static bool
cmd_number_inf (const char *com, const char *val, void *place)
{
  if (!c_strcasecmp (val, "inf"))
    {
      *(int *) place = 0;
      return true;
    }
  return cmd_number (com, val, place);
}

/* Copy (strdup) the string at COM to a new location and place a
   pointer to *PLACE.  */
static bool
cmd_string (const char *com _GL_UNUSED, const char *val, void *place)
{
  char **pstring = (char **)place;

  xfree (*pstring);
  *pstring = xstrdup (val);
  return true;
}

/* Like cmd_string but ensure the string is upper case.  */
static bool
cmd_string_uppercase (const char *com _GL_UNUSED, const char *val, void *place)
{
  char *q, **pstring;
  pstring = (char **)place;
  xfree (*pstring);

  *pstring = xmalloc (strlen (val) + 1);

  for (q = *pstring; *val; val++, q++)
    *q = c_toupper (*val);

  *q = '\0';
  return true;
}


/* Like cmd_string, but handles tilde-expansion when reading a user's
   `.wgetrc'.  In that case, and if VAL begins with `~', the tilde
   gets expanded to the user's home directory.  */
static bool
cmd_file (const char *com _GL_UNUSED, const char *val, void *place)
{
  char **pstring = (char **)place;

  xfree (*pstring);

  /* #### If VAL is empty, perhaps should set *PLACE to NULL.  */

  *pstring = xstrdup (val);

#if defined(WINDOWS) || defined(MSDOS)
  /* Convert "\" to "/". */
  {
    char *s;
    for (s = *pstring; *s; s++)
      if (*s == '\\')
        *s = '/';
  }
#endif
  return true;
}

/* like cmd_file, but insist on just a single option usage */
static bool cmd_file_once(const char *com, const char *val, void *place)
{
  if (*((char **) place))
  {
    fprintf(stderr, gettext("%s: %s must only be used once\n"), get_exec_name_wrapper(), com);
    return 0;
  }
  return cmd_file(com, val, place);
}



/* Like cmd_file, but strips trailing '/' characters.  */
static bool
cmd_directory (const char *com, const char *val, void *place)
{
  char *s, *t;

  /* Call cmd_file() for tilde expansion and separator
     canonicalization (backslash -> slash under Windows).  These
     things should perhaps be in a separate function.  */
  if (!cmd_file (com, val, place))
    return false;

  s = *(char **)place;
  t = s + strlen (s);
  while (t > s && *--t == '/')
    *t = '\0';

  return true;
}

/* Split VAL by space to a vector of values, and append those values
   to vector pointed to by the PLACE argument.  If VAL is empty, the
   PLACE vector is cleared instead.  */

static bool
cmd_vector (const char *com _GL_UNUSED, const char *val, void *place)
{
  char ***pvec = (char ***)place;

  if (*val)
    *pvec = merge_vecs (*pvec, sepstring (val));
  else
    {
      free_vec (*pvec);
      *pvec = NULL;
    }
  return true;
}

static bool
cmd_directory_vector (const char *com _GL_UNUSED, const char *val, void *place)
{
  char ***pvec = (char ***)place;

  if (*val)
    {
      /* Strip the trailing slashes from directories.  */
      char **t, **seps;

      seps = sepstring (val);
      for (t = seps; t && *t; t++)
        {
          int len = strlen (*t);
          /* Skip degenerate case of root directory.  */
          if (len > 1)
            {
              if ((*t)[len - 1] == '/')
                (*t)[len - 1] = '\0';
            }
        }
      *pvec = merge_vecs (*pvec, seps);
    }
  else
    {
      free_vec (*pvec);
      *pvec = NULL;
    }
  return true;
}

/* Engine for cmd_bytes and cmd_bytes_sum: converts a string such as
   "100k" or "2.5G" to a floating point number.  */

static bool
parse_bytes_helper (const char *val, double *result)
{
  double number, mult;
  const char *end = val + strlen (val);

  /* Check for "inf".  */
  if (0 == strcmp (val, "inf"))
    {
      *result = 0;
      return true;
    }

  /* Strip trailing whitespace.  */
  while (val < end && c_isspace (end[-1]))
    --end;
  if (val == end)
    return false;

  switch (c_tolower (end[-1]))
    {
    case 'k':
      --end, mult = 1024.0;
      break;
    case 'm':
      --end, mult = 1048576.0;
      break;
    case 'g':
      --end, mult = 1073741824.0;
      break;
    case 't':
      --end, mult = 1099511627776.0;
      break;
    default:
      /* Not a recognized suffix: assume it's a digit.  (If not,
         simple_atof will raise an error.)  */
      mult = 1;
    }

  /* Skip leading and trailing whitespace. */
  while (val < end && c_isspace (*val))
    ++val;
  while (val < end && c_isspace (end[-1]))
    --end;
  if (val == end)
    return false;

  if (!simple_atof (val, end, &number) || number < 0)
    return false;

  *result = number * mult;
  return true;
}

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

static bool cmd_bytes(const char *com, const char *val, void *place)
{
  double byte_value;
  if (!parse_bytes_helper(val, &byte_value))
  {
    fprintf(stderr, gettext("%s: %s: Invalid byte value %s\n"), get_exec_name_wrapper(), com, quote(val));
    return 0;
  }
  *((wgint *) place) = (wgint) byte_value;
  return 1;
}



/* Like cmd_bytes, but PLACE is interpreted as a pointer to
   SIZE_SUM.  It works by converting the string to double, therefore
   working with values up to 2^53-1 without loss of precision.  This
   value (8192 TB) is large enough to serve for a while.  */

static bool cmd_bytes_sum(const char *com, const char *val, void *place)
{
  double byte_value;
  if (!parse_bytes_helper(val, &byte_value))
  {
    fprintf(stderr, gettext("%s: %s: Invalid byte value %s\n"), get_exec_name_wrapper(), com, quote(val));
    return 0;
  }
  *((SUM_SIZE_INT *) place) = (SUM_SIZE_INT) byte_value;
  return 1;
}



/* Store the value of VAL to *OUT.  The value is a time period, by
   default expressed in seconds, but also accepting suffixes "m", "h",
   "d", and "w" for minutes, hours, days, and weeks respectively.  */

static bool cmd_time(const char *com, const char *val, void *place)
{
  double number;
  double mult;
  const char *end = val + strlen(val);
  while ((val < end) && c_isspace(end[-1]))
    --end;

  if (val == end)
  {
    err:
    fprintf(stderr, gettext("%s: %s: Invalid time period %s\n"), get_exec_name_wrapper(), com, quote(val));

    return 0;
  }
  switch (c_tolower(end[-1]))
  {
    case 's':
      --end, mult = 1;
      break;

    case 'm':
      --end, mult = 60;
      break;

    case 'h':
      --end, mult = 3600;
      break;

    case 'd':
      --end, mult = 86400.0;
      break;

    case 'w':
      --end, mult = 604800.0;
      break;

    default:
      mult = 1;

  }

  while ((val < end) && c_isspace(*val))
    ++val;

  while ((val < end) && c_isspace(end[-1]))
    --end;

  if (val == end)
    goto err;
  if (!simple_atof(val, end, &number))
    goto err;
  *((double *) place) = number * mult;
  return 1;
}



#ifdef HAVE_SSL
static bool cmd_cert_type(const char *com, const char *val, void *place)
{
  static const struct decode_item choices[] = {{"pem", keyfile_pem}, {"der", keyfile_asn1}, {"asn1", keyfile_asn1}};
  int ok = decode_string(val, choices, (sizeof(choices)) / (sizeof(choices[0])), place);
  if (!ok)
    fprintf(stderr, gettext("%s: %s: Invalid value %s.\n"), get_exec_name_wrapper(), com, quote(val));
  return ok;
}


#endif

/* Specialized helper functions, used by `commands' to handle some
   options specially.  */

static bool check_user_specified_header (const char *);

static bool cmd_spec_dirstruct(const char *com, const char *val, void *place_ignored)
{
  if (!cmd_boolean(com, val, &get_opt_dirstruct_wrapper()))
    return 0;
  if (get_opt_dirstruct_wrapper())
    set_opt_no_dirstruct_wrapper(0);
  else
    set_opt_no_dirstruct_wrapper(1);
  return 1;
}



static bool cmd_spec_header(const char *com, const char *val, void *place_ignored)
{
  if ((*val) == '\0')
  {
    free_vec(get_opt_user_headers_wrapper());
    set_opt_user_headers_wrapper(0);
    return 1;
  }
  if (!check_user_specified_header(val))
  {
    fprintf(stderr, gettext("%s: %s: Invalid header %s.\n"), get_exec_name_wrapper(), com, quote(val));
    return 0;
  }
  set_opt_user_headers_wrapper(vec_append(opt.user_headers, val));
  return 1;
}



static bool cmd_spec_warc_header(const char *com, const char *val, void *place_ignored)
{
  if ((*val) == '\0')
  {
    free_vec(get_opt_warc_user_headers_wrapper());
    set_opt_warc_user_headers_wrapper(0);
    return 1;
  }
  if (!check_user_specified_header(val))
  {
    fprintf(stderr, gettext("%s: %s: Invalid WARC header %s.\n"), get_exec_name_wrapper(), com, quote(val));
    return 0;
  }
  set_opt_warc_user_headers_wrapper(vec_append(opt.warc_user_headers, val));
  return 1;
}



static bool cmd_spec_htmlify(const char *com, const char *val, void *place_ignored)
{
  int flag = cmd_boolean(com, val, &get_opt_htmlify_wrapper());
  if (flag && (!get_opt_htmlify_wrapper()))
    set_opt_remove_listing_wrapper(0);
  return flag;
}



/* Set the "mirror" mode.  It means: recursive download, timestamping,
   no limit on max. recursion depth, and don't remove listings.  */

static bool cmd_spec_mirror(const char *com, const char *val, void *place_ignored)
{
  int mirror;
  if (!cmd_boolean(com, val, &mirror))
    return 0;
  if (mirror)
  {
    set_opt_recursive_wrapper(1);
    if (!get_opt_no_dirstruct_wrapper())
      set_opt_dirstruct_wrapper(1);
    set_opt_timestamping_wrapper(1);
    set_opt_reclevel_wrapper(-1);
    set_opt_remove_listing_wrapper(0);
  }
  return 1;
}



/* Validate --prefer-family and set the choice.  Allowed values are
   "IPv4", "IPv6", and "none".  */

static bool cmd_spec_prefer_family(const char *com, const char *val, void *place_ignored)
{
  static const struct decode_item choices[] = {{"IPv4", prefer_ipv4}, {"IPv6", prefer_ipv6}, {"none", prefer_none}};
  int prefer_family = prefer_none;
  int ok = decode_string(val, choices, (sizeof(choices)) / (sizeof(choices[0])), &prefer_family);
  if (!ok)
    fprintf(stderr, gettext("%s: %s: Invalid value %s.\n"), get_exec_name_wrapper(), com, quote(val));
  set_opt_prefer_family_wrapper(prefer_family);
  return ok;
}



/* Set progress.type to VAL, but verify that it's a valid progress
   implementation before that.  */

static bool cmd_spec_progress(const char *com, const char *val, void *place_ignored)
{
  if (!valid_progress_implementation_p(val))
  {
    fprintf(stderr, gettext("%s: %s: Invalid progress type %s.\n"), get_exec_name_wrapper(), com, quote(val));
    return 0;
  }
  do
  {
    free((void *) get_opt_progress_type_wrapper());
    set_opt_progress_type_wrapper(0);
  }
  while (0);
  set_opt_progress_type_wrapper(xstrdup(val));
  return 1;
}



/* Set opt.recursive to VAL as with cmd_boolean.  If opt.recursive is
   set to true, also set opt.dirstruct to true, unless opt.no_dirstruct
   is specified.  */

static bool cmd_spec_recursive(const char *com, const char *val, void *place_ignored)
{
  if (!cmd_boolean(com, val, &get_opt_recursive_wrapper()))
    return 0;
  else
  {
    if (get_opt_recursive_wrapper() && (!get_opt_no_dirstruct_wrapper()))
      set_opt_dirstruct_wrapper(1);
  }
  return 1;
}



/* Validate --regex-type and set the choice.  */

static bool cmd_spec_regex_type(const char *com, const char *val, void *place_ignored)
{
  static const struct decode_item choices[] = {{"posix", regex_type_posix}, {"pcre", regex_type_pcre}};
  int regex_type = regex_type_posix;
  int ok = decode_string(val, choices, (sizeof(choices)) / (sizeof(choices[0])), &regex_type);
  if (!ok)
    fprintf(stderr, gettext("%s: %s: Invalid value %s.\n"), get_exec_name_wrapper(), com, quote(val));
  set_opt_regex_type_wrapper(regex_type);
  return ok;
}



static bool cmd_spec_restrict_file_names(const char *com, const char *val, void *place_ignored)
{
  int restrict_os = get_opt_restrict_files_os_wrapper();
  int restrict_ctrl = get_opt_restrict_files_ctrl_wrapper();
  int restrict_case = get_opt_restrict_files_case_wrapper();
  int restrict_nonascii = get_opt_restrict_files_nonascii_wrapper();
  const char *end;
  do
  {
    end = strchr(val, ',');
    if (!end)
      end = val + strlen(val);
    if (((end - val) == ((sizeof("unix")) - 1)) && (!memcmp(val, "unix", (sizeof("unix")) - 1)))
      restrict_os = restrict_unix;
    else
      if (((end - val) == ((sizeof("vms")) - 1)) && (!memcmp(val, "vms", (sizeof("vms")) - 1)))
      restrict_os = restrict_vms;
    else
      if (((end - val) == ((sizeof("windows")) - 1)) && (!memcmp(val, "windows", (sizeof("windows")) - 1)))
      restrict_os = restrict_windows;
    else
      if (((end - val) == ((sizeof("lowercase")) - 1)) && (!memcmp(val, "lowercase", (sizeof("lowercase")) - 1)))
      restrict_case = restrict_lowercase;
    else
      if (((end - val) == ((sizeof("uppercase")) - 1)) && (!memcmp(val, "uppercase", (sizeof("uppercase")) - 1)))
      restrict_case = restrict_uppercase;
    else
      if (((end - val) == ((sizeof("nocontrol")) - 1)) && (!memcmp(val, "nocontrol", (sizeof("nocontrol")) - 1)))
      restrict_ctrl = 0;
    else
      if (((end - val) == ((sizeof("ascii")) - 1)) && (!memcmp(val, "ascii", (sizeof("ascii")) - 1)))
      restrict_nonascii = 1;
    else
    {
      fprintf(stderr, gettext("%s: %s: Invalid restriction %s,\n    use [unix|vms|windows],[lowercase|uppercase],[nocontrol],[ascii].\n"), get_exec_name_wrapper(), com, quote(val));
      return 0;
    }
    if (*end)
      val = end + 1;
  }
  while ((*val) && (*end));
  set_opt_restrict_files_os_wrapper(restrict_os);
  set_opt_restrict_files_ctrl_wrapper(restrict_ctrl);
  set_opt_restrict_files_case_wrapper(restrict_case);
  set_opt_restrict_files_nonascii_wrapper(restrict_nonascii);
  return 1;
}



static bool cmd_spec_report_speed(const char *com, const char *val, void *place_ignored)
{
  set_opt_report_bps_wrapper(c_strcasecmp(val, "bits") == 0);
  if (!get_opt_report_bps_wrapper())
    fprintf(stderr, gettext("%s: %s: Invalid value %s.\n"), get_exec_name_wrapper(), com, quote(val));
  return get_opt_report_bps_wrapper();
}



#ifdef HAVE_SSL
static bool cmd_spec_secure_protocol(const char *com, const char *val, void *place)
{
  static const struct decode_item choices[] = {{"auto", secure_protocol_auto}, {"sslv2", secure_protocol_sslv2}, {"sslv3", secure_protocol_sslv3}, {"tlsv1", secure_protocol_tlsv1}, {"tlsv1_1", secure_protocol_tlsv1_1}, {"tlsv1_2", secure_protocol_tlsv1_2}, {"pfs", secure_protocol_pfs}};
  int ok = decode_string(val, choices, (sizeof(choices)) / (sizeof(choices[0])), place);
  if (!ok)
    fprintf(stderr, gettext("%s: %s: Invalid value %s.\n"), get_exec_name_wrapper(), com, quote(val));
  return ok;
}


#endif

/* Set all three timeout values. */

static bool cmd_spec_timeout(const char *com, const char *val, void *place_ignored)
{
  double value;
  if (!cmd_time(com, val, &value))
    return 0;
  set_opt_read_timeout_wrapper(value);
  set_opt_connect_timeout_wrapper(value);
  set_opt_dns_timeout_wrapper(value);
  return 1;
}



static bool cmd_spec_useragent(const char *com, const char *val, void *place_ignored)
{
  if (strchr(val, '\n'))
  {
    fprintf(stderr, gettext("%s: %s: Invalid value %s.\n"), get_exec_name_wrapper(), com, quote(val));
    return 0;
  }
  do
  {
    free((void *) get_opt_useragent_wrapper());
    set_opt_useragent_wrapper(0);
  }
  while (0);
  set_opt_useragent_wrapper(xstrdup(val));
  return 1;
}



/* The --show-progress option is not a cmd_boolean since we need to keep track
 * of whether the user explicitly requested the option or not. -1 means
 * uninitialized. */
static bool cmd_spec_progressdisp(const char *com, const char *val, void *place)
{
  bool flag;
  if (cmd_boolean(com, val, &flag))
  {
    set_opt_show_progress_wrapper(flag);
    return 1;
  }
  return 0;
}




/* The "verbose" option cannot be cmd_boolean because the variable is
   not bool -- it's of type int (-1 means uninitialized because of
   some random hackery for disallowing -q -v).  */

static bool cmd_spec_verbose(const char *com, const char *val, void *place_ignored)
{
  bool flag;
  if (cmd_boolean(com, val, &flag))
  {
    set_opt_verbose_wrapper(flag);
    set_opt_show_progress_wrapper(-1);
    return 1;
  }
  return 0;
}



/* Miscellaneous useful routines.  */

/* A very simple atoi clone, more useful than atoi because it works on
   delimited strings, and has error reportage.  Returns true on success,
   false on failure.  If successful, stores result to *DEST.  */

static bool
simple_atoi (const char *beg, const char *end, int *dest)
{
  int result = 0;
  bool negative = false;
  const char *p = beg;

  while (p < end && c_isspace (*p))
    ++p;
  if (p < end && (*p == '-' || *p == '+'))
    {
      negative = (*p == '-');
      ++p;
    }
  if (p == end)
    return false;

  /* Read negative numbers in a separate loop because the most
     negative integer cannot be represented as a positive number.  */

  if (!negative)
    for (; p < end && c_isdigit (*p); p++)
      {
        int next = (10 * result) + (*p - '0');
        if (next < result)
          return false;         /* overflow */
        result = next;
      }
  else
    for (; p < end && c_isdigit (*p); p++)
      {
        int next = (10 * result) - (*p - '0');
        if (next > result)
          return false;         /* underflow */
        result = next;
      }

  if (p != end)
    return false;

  *dest = result;
  return true;
}

/* Trivial atof, with error reporting.  Handles "<digits>[.<digits>]",
   doesn't handle exponential notation.  Returns true on success,
   false on failure.  In case of success, stores its result to
   *DEST.  */

static bool
simple_atof (const char *beg, const char *end, double *dest)
{
  double result = 0;

  bool negative = false;
  bool seen_dot = false;
  bool seen_digit = false;
  double divider = 1;

  const char *p = beg;

  while (p < end && c_isspace (*p))
    ++p;
  if (p < end && (*p == '-' || *p == '+'))
    {
      negative = (*p == '-');
      ++p;
    }

  for (; p < end; p++)
    {
      char ch = *p;
      if (c_isdigit (ch))
        {
          if (!seen_dot)
            result = (10 * result) + (ch - '0');
          else
            result += (ch - '0') / (divider *= 10);
          seen_digit = true;
        }
      else if (ch == '.')
        {
          if (!seen_dot)
            seen_dot = true;
          else
            return false;
        }
      else
        return false;
    }
  if (!seen_digit)
    return false;
  if (negative)
    result = -result;

  *dest = result;
  return true;
}

/* Verify that the user-specified header in S is valid.  It must
   contain a colon preceded by non-white-space characters and must not
   contain newlines.  */

static bool
check_user_specified_header (const char *s)
{
  const char *p;

  for (p = s; *p && *p != ':' && !c_isspace (*p); p++)
    ;
  /* The header MUST contain `:' preceded by at least one
     non-whitespace character.  */
  if (*p != ':' || p == s)
    return false;
  /* The header MUST NOT contain newlines.  */
  if (strchr (s, '\n'))
    return false;
  return true;
}

/* Decode VAL into a number, according to ITEMS. */

static bool
decode_string (const char *val, const struct decode_item *items, int itemcount,
               int *place)
{
  int i;
  for (i = 0; i < itemcount; i++)
    if (0 == c_strcasecmp (val, items[i].name))
      {
        *place = items[i].code;
        return true;
      }
  return false;
}

/* Free the memory allocated by global variables.  */
void cleanup(void)
{
  if (get_opt_warc_filename_wrapper() != 0)
    warc_close();
  log_close();
  if (get_output_stream_wrapper())
    if (fclose(get_output_stream_wrapper()) == (-1))
    inform_exit_status(CLOSEFAILED);
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
