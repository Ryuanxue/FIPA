/* File Transfer Protocol support.
   Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
   2005, 2006, 2007, 2008, 2009, 2010, 2011, 2014, 2015 Free Software
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
#include <strings.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

#include "utils.h"
#include "url.h"
#include "retr.h"
#include "ftp.h"
#include "ssl.h"
#include "connect.h"
#include "host.h"
#include "netrc.h"
#include "convert.h"            /* for downloaded_file */
#include "recur.h"              /* for INFINITE_RECURSION */
#include "warc.h"
#include "c-strcase.h"

#include "wget_rpc_wrapper.h"

#ifdef __VMS
# include "vms.h"
#endif /* def __VMS */


/* File where the "ls -al" listing will be saved.  */
#ifdef MSDOS
#define LIST_FILENAME "_listing"
#else
#define LIST_FILENAME ".listing"
#endif

typedef struct
{
  int st;                       /* connection status */
  int cmd;                      /* command code */
  int csock;                    /* control connection socket */
  double dltime;                /* time of the download in msecs */
  enum stype rs;                /* remote system reported by ftp server */
  enum ustype rsu;              /* when rs is ST_UNIX, here there are more details */
  char *id;                     /* initial directory */
  char *target;                 /* target file name */
  struct url *proxy;            /* FTWK-style proxy */
} ccon;


/* Look for regexp "( *[0-9]+ *byte" (literal parenthesis) anywhere in
   the string S, and return the number converted to wgint, if found, 0
   otherwise.  */
static wgint
ftp_expected_bytes (const char *s)
{
  wgint res;

  while (1)
    {
      while (*s && *s != '(')
        ++s;
      if (!*s)
        return 0;
      ++s;                      /* skip the '(' */
      res = str_to_wgint (s, (char **) &s, 10);
      if (!*s)
        return 0;
      while (*s && c_isspace (*s))
        ++s;
      if (!*s)
        return 0;
      if (c_tolower (*s) != 'b')
        continue;
      if (c_strncasecmp (s, "byte", 4))
        continue;
      else
        break;
    }
  return res;
}

#ifdef ENABLE_IPV6
/*
 * This function sets up a passive data connection with the FTP server.
 * It is merely a wrapper around ftp_epsv, ftp_lpsv and ftp_pasv.
 */
static uerr_t ftp_do_pasv(int csock, ip_address *addr, int *port)
{
  uerr_t err;
  if (!socket_ip_address(csock, addr, ENDPOINT_PEER))
    abort();
  switch (addr->family)
  {
    case AF_INET:
      if (!get_opt_server_response_wrapper())
      logputs(LOG_VERBOSE, "==> PASV ... ");
      err = ftp_pasv(csock, addr, port);
      break;

    case AF_INET6:
      if (!get_opt_server_response_wrapper())
      logputs(LOG_VERBOSE, "==> EPSV ... ");
      err = ftp_epsv(csock, addr, port);
      if (err == FTPNOPASV)
    {
      if (!get_opt_server_response_wrapper())
        logputs(LOG_VERBOSE, "==> LPSV ... ");
      err = ftp_lpsv(csock, addr, port);
    }
      break;

    default:
      abort();

  }

  return err;
}



/*
 * This function sets up an active data connection with the FTP server.
 * It is merely a wrapper around ftp_eprt, ftp_lprt and ftp_port.
 */
static uerr_t ftp_do_port(int csock, int *local_sock)
{
  uerr_t err;
  ip_address cip;
  if (!socket_ip_address(csock, &cip, ENDPOINT_PEER))
    abort();
  switch (cip.family)
  {
    case AF_INET:
      if (!get_opt_server_response_wrapper())
      logputs(LOG_VERBOSE, "==> PORT ... ");
      err = ftp_port(csock, local_sock);
      break;

    case AF_INET6:
      if (!get_opt_server_response_wrapper())
      logputs(LOG_VERBOSE, "==> EPRT ... ");
      err = ftp_eprt(csock, local_sock);
      if (err == FTPPORTERR)
    {
      if (!get_opt_server_response_wrapper())
        logputs(LOG_VERBOSE, "==> LPRT ... ");
      err = ftp_lprt(csock, local_sock);
    }
      break;

    default:
      abort();

  }

  return err;
}


#else

static uerr_t
ftp_do_pasv (int csock, ip_address *addr, int *port)
{
  if (!opt.server_response)
    logputs (LOG_VERBOSE, "==> PASV ... ");
  return ftp_pasv (csock, addr, port);
}

static uerr_t
ftp_do_port (int csock, int *local_sock)
{
  if (!opt.server_response)
    logputs (LOG_VERBOSE, "==> PORT ... ");
  return ftp_port (csock, local_sock);
}
#endif

static void
print_length (wgint size, wgint start, bool authoritative)
{
  logprintf (LOG_VERBOSE, _("Length: %s"), number_to_static_string (size));
  if (size >= 1024)
    logprintf (LOG_VERBOSE, " (%s)", human_readable (size, 10, 1));
  if (start > 0)
    {
      if (size - start >= 1024)
        logprintf (LOG_VERBOSE, _(", %s (%s) remaining"),
                   number_to_static_string (size - start),
                   human_readable (size - start, 10, 1));
      else
        logprintf (LOG_VERBOSE, _(", %s remaining"),
                   number_to_static_string (size - start));
    }
  logputs (LOG_VERBOSE, !authoritative ? _(" (unauthoritative)\n") : "\n");
}

static uerr_t ftp_get_listing (struct url *, struct url *, ccon *, struct fileinfo **);

static uerr_t
get_ftp_greeting(int csock, ccon *con)
{
  uerr_t err = 0;

  /* Get the server's greeting */
  err = ftp_greeting (csock);
  if (err != FTPOK)
    {
      logputs (LOG_NOTQUIET, "Error in server response. Closing.\n");
      fd_close (csock);
      con->csock = -1;
    }

  return err;
}

#ifdef HAVE_SSL
static uerr_t init_control_ssl_connection(int csock, struct url *u, bool *using_control_security)
{
  bool using_security = 0;
  if ((!get_opt_ftps_implicit_wrapper()) && (!get_opt_server_response_wrapper()))
    logputs(LOG_VERBOSE, "==> AUTH TLS ... ");
  if (get_opt_ftps_implicit_wrapper() || (ftp_auth(csock, SCHEME_FTPS) == FTPOK))
  {
    if (!ssl_connect_wget(csock, u->host, 0))
    {
      fd_close(csock);
      return CONSSLERR;
    }
    else
      if (!ssl_check_certificate(csock, u->host))
    {
      fd_close(csock);
      return VERIFCERTERR;
    }
    if ((!get_opt_ftps_implicit_wrapper()) && (!get_opt_server_response_wrapper()))
      logputs(LOG_VERBOSE, " done.\n");
    using_security = 1;
  }
  else
  {
    if (get_opt_ftps_fallback_to_ftp_wrapper())
    {
      logputs(LOG_NOTQUIET, "Server does not support AUTH TLS. Falling back to FTP.\n");
      using_security = 0;
    }
    else
    {
      fd_close(csock);
      return FTPNOAUTH;
    }
  }
  *using_control_security = using_security;
  return NOCONERROR;
}


#endif

/* Retrieves a file with denoted parameters through opening an FTP
   connection to the server.  It always closes the data connection,
   and closes the control connection in case of error.  If warc_tmp
   is non-NULL, the downloaded data will be written there as well.  */
static uerr_t getftp(struct url *u, struct url *original_url, wgint passed_expected_bytes, wgint *qtyread, wgint restval, ccon *con, int count, wgint *last_expected_bytes, FILE *warc_tmp)
{
  int csock;
  int dtsock;
  int local_sock;
  int res;
  uerr_t err = RETROK;
  FILE *fp = 0;
  char *respline;
  char *tms;
  const char *user;
  const char *passwd;
  const char *tmrate;
  int cmd = con->cmd;
  wgint expected_bytes = 0;
  bool got_expected_bytes = 0;
  bool rest_failed = 0;
  int flags;
  wgint rd_size;
  wgint previous_rd_size = 0;
  char type_char;
  bool try_again;
  bool list_a_used = 0;
  enum prot_level prot = (get_opt_ftps_clear_data_connection_wrapper()) ? (PROT_CLEAR) : (PROT_PRIVATE);
  bool using_control_security = 0;
  bool using_data_security = 0;
  assert(con != 0);
  assert(con->target != 0);
  assert(!((cmd & DO_LIST) && (cmd & DO_RETR)));
  assert((cmd & (((DO_LIST | DO_CWD) | DO_RETR) | DO_LOGIN)) != 0);
  *qtyread = restval;
  user = u->user;
  passwd = u->passwd;
  search_netrc(u->host, (const char **) (&user), (const char **) (&passwd), 1);
  user = (user) ? (user) : ((get_opt_ftp_user_wrapper()) ? (get_opt_ftp_user_wrapper()) : (get_opt_user_wrapper()));
  if (!user)
    user = "anonymous";
  passwd = (passwd) ? (passwd) : ((get_opt_ftp_passwd_wrapper()) ? (get_opt_ftp_passwd_wrapper()) : (get_opt_passwd_wrapper()));
  if (!passwd)
    passwd = "-wget@";
  dtsock = -1;
  local_sock = -1;
  con->dltime = 0;
  if (u->scheme == SCHEME_FTPS)
  {
    if (!ssl_init())
    {
      scheme_disable(SCHEME_FTPS);
      logprintf(LOG_NOTQUIET, gettext("Could not initialize SSL. It will be disabled."));
      err = SSLINITFAILED;
      return err;
    }
    if (get_opt_ftps_implicit_wrapper() && (u->port == 21))
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Implicit FTPS was specified. Rewriting default port to %d.\n", 990);
        }
      }
      while (0);
      u->port = 990;
    }
  }
  if (!(cmd & DO_LOGIN))
  {
    csock = con->csock;
    using_data_security = con->st & DATA_CHANNEL_SECURITY;
  }
  else
  {
    char *host = (con->proxy) ? (con->proxy->host) : (u->host);
    int port = (con->proxy) ? (con->proxy->port) : (u->port);
    csock = connect_to_host(host, port);
    if (csock == E_HOST)
      return HOSTERR;
    else
      if (csock < 0)
      return (retryable_socket_connect_error(errno)) ? (CONERROR) : (CONIMPOSSIBLE);
    if (cmd & LEAVE_PENDING)
      con->csock = csock;
    else
      con->csock = -1;
    if (u->scheme == SCHEME_FTPS)
    {
      if (get_opt_ftps_implicit_wrapper())
      {
        err = init_control_ssl_connection(csock, u, &using_control_security);
        if (err != NOCONERROR)
          return err;
        err = get_ftp_greeting(csock, con);
        if (err != FTPOK)
          return err;
      }
      else
      {
        err = get_ftp_greeting(csock, con);
        if (err != FTPOK)
          return err;
        err = init_control_ssl_connection(csock, u, &using_control_security);
        if (err != NOCONERROR)
          return err;
      }
    }
    else
    {
      err = get_ftp_greeting(csock, con);
      if (err != FTPOK)
        return err;
    }
    logprintf(LOG_VERBOSE, gettext("Logging in as %s ... "), quotearg_style(escape_quoting_style, user));
    if (get_opt_server_response_wrapper())
      logputs(LOG_ALWAYS, "\n");
    if (con->proxy)
    {
      char *logname = concat_strings(user, "@", u->host, (char *) 0);
      err = ftp_login(csock, logname, passwd);
      do
      {
        free((void *) logname);
        logname = 0;
      }
      while (0);
    }
    else
      err = ftp_login(csock, user, passwd);
    switch (err)
    {
      case FTPRERR:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
        fd_close(csock);
        con->csock = -1;
        return err;

      case FTPSRVERR:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("Error in server greeting.\n"));
        fd_close(csock);
        con->csock = -1;
        return err;

      case WRITEFAILED:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("Write failed, closing control connection.\n"));
        fd_close(csock);
        con->csock = -1;
        return err;

      case FTPLOGREFUSED:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("The server refuses login.\n"));
        fd_close(csock);
        con->csock = -1;
        return FTPLOGREFUSED;

      case FTPLOGINC:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("Login incorrect.\n"));
        fd_close(csock);
        con->csock = -1;
        return FTPLOGINC;

      case FTPOK:
        if (!get_opt_server_response_wrapper())
        logputs(LOG_VERBOSE, gettext("Logged in!\n"));
        break;

      default:
        abort();

    }

    if (using_control_security)
    {
      if (u->scheme == SCHEME_FTPS)
      {
        if (!get_opt_server_response_wrapper())
          logputs(LOG_VERBOSE, "==> PBSZ 0 ... ");
        if ((err = ftp_pbsz(csock, 0)) == FTPNOPBSZ)
        {
          logputs(LOG_NOTQUIET, gettext("Server did not accept the 'PBSZ 0' command.\n"));
          return err;
        }
        if (!get_opt_server_response_wrapper())
          logputs(LOG_VERBOSE, "done.");
        if (!get_opt_server_response_wrapper())
          logprintf(LOG_VERBOSE, "  ==> PROT %c ... ", prot);
        if ((err = ftp_prot(csock, prot)) == FTPNOPROT)
        {
          logprintf(LOG_NOTQUIET, gettext("Server did not accept the 'PROT %c' command.\n"), prot);
          return err;
        }
        if (!get_opt_server_response_wrapper())
          logputs(LOG_VERBOSE, "done.\n");
        if (prot != PROT_CLEAR)
        {
          using_data_security = 1;
          con->st |= DATA_CHANNEL_SECURITY;
        }
      }
    }
    if (!get_opt_server_response_wrapper())
      logprintf(LOG_VERBOSE, "==> SYST ... ");
    err = ftp_syst(csock, &con->rs, &con->rsu);
    switch (err)
    {
      case FTPRERR:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
        fd_close(csock);
        con->csock = -1;
        return err;

      case FTPSRVERR:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("Server error, can't determine system type.\n"));
        break;

      case FTPOK:
        break;

      default:
        abort();

    }

    if ((!get_opt_server_response_wrapper()) && (err != FTPSRVERR))
      logputs(LOG_VERBOSE, gettext("done.    "));
    switch (con->rs)
    {
      case ST_VMS:
        do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("\nVMS: I know it and I will use \"LIST\" as standard list command\n");
        }
      }
      while (0);
        con->st |= LIST_AFTER_LIST_A_CHECK_DONE;
        con->st |= AVOID_LIST_A;
        break;

      case ST_UNIX:
        if (con->rsu == UST_MULTINET)
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("\nUNIX MultiNet: I know it and I will use \"LIST\" as standard list command\n");
          }
        }
        while (0);
        con->st |= LIST_AFTER_LIST_A_CHECK_DONE;
        con->st |= AVOID_LIST_A;
      }
      else
        if (con->rsu == UST_TYPE_L8)
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("\nUNIX TYPE L8: I know it and I will use \"LIST -a\" as standard list command\n");
          }
        }
        while (0);
        con->st |= LIST_AFTER_LIST_A_CHECK_DONE;
        con->st |= AVOID_LIST;
      }
        break;

      default:
        break;

    }

    if (!get_opt_server_response_wrapper())
      logprintf(LOG_VERBOSE, "==> PWD ... ");
    err = ftp_pwd(csock, &con->id);
    switch (err)
    {
      case FTPRERR:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
        fd_close(csock);
        con->csock = -1;
        return err;

      case FTPSRVERR:
        do
      {
        free((void *) con->id);
        con->id = 0;
      }
      while (0);
        con->id = xstrdup("/");
        break;

      case FTPOK:
        break;

      default:
        abort();

    }

    if (!get_opt_server_response_wrapper())
      logputs(LOG_VERBOSE, gettext("done.\n"));
    type_char = ftp_process_type(u->params);
    if (!get_opt_server_response_wrapper())
      logprintf(LOG_VERBOSE, "==> TYPE %c ... ", type_char);
    err = ftp_type(csock, type_char);
    switch (err)
    {
      case FTPRERR:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
        fd_close(csock);
        con->csock = -1;
        return err;

      case WRITEFAILED:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("Write failed, closing control connection.\n"));
        fd_close(csock);
        con->csock = -1;
        return err;

      case FTPUNKNOWNTYPE:
        logputs(LOG_VERBOSE, "\n");
        logprintf(LOG_NOTQUIET, gettext("Unknown type `%c', closing control connection.\n"), type_char);
        fd_close(csock);
        con->csock = -1;
        return err;

      case FTPOK:
        break;

      default:
        abort();

    }

    if (!get_opt_server_response_wrapper())
      logputs(LOG_VERBOSE, gettext("done.  "));
  }
  if (cmd & DO_CWD)
  {
    if (!(*u->dir))
      logputs(LOG_VERBOSE, gettext("==> CWD not needed.\n"));
    else
    {
      const char *targ = 0;
      int cwd_count;
      int cwd_end;
      int cwd_start;
      char *target = u->dir;
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("changing working directory\n");
        }
      }
      while (0);
      if ((((target[0] != '/') && (!(((con->rs != ST_UNIX) && c_isalpha(target[0])) && (target[1] == ':')))) && (con->rs != ST_OS400)) && (con->rs != ST_VMS))
      {
        int idlen = strlen(con->id);
        char *ntarget;
        char *p;
        while ((idlen > 0) && (con->id[idlen - 1] == '/'))
          --idlen;

        p = (ntarget = (char *) alloca(((idlen + 1) + strlen(u->dir)) + 1));
        memcpy(p, con->id, idlen);
        p += idlen;
        *(p++) = '/';
        strcpy(p, target);
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Prepended initial PWD to relative path:\n");
          }
        }
        while (0);
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("   pwd: '%s'\n   old: '%s'\n  new: '%s'\n", con->id, target, ntarget);
          }
        }
        while (0);
        target = ntarget;
      }
      if ((con->rs == ST_VMS) && (target[0] != '/'))
      {
        cwd_start = 0;
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Using two-step CWD for relative path.\n");
          }
        }
        while (0);
      }
      else
      {
        cwd_start = 1;
      }
      if ((con->rs == ST_VMS) && (strchr(target, '/') != 0))
      {
        cwd_end = 3;
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Using extra \"CWD []\" step for VMS server.\n");
          }
        }
        while (0);
      }
      else
      {
        cwd_end = 2;
      }
      for (cwd_count = cwd_start; cwd_count < cwd_end; cwd_count++)
      {
        switch (cwd_count)
        {
          case 0:
            targ = con->id;
            break;

          case 1:
            targ = target;
            break;

          case 2:
            targ = "[]";
            break;

          default:
            logprintf(LOG_ALWAYS, gettext("Logically impossible section reached in getftp()"));
            logprintf(LOG_ALWAYS, gettext("cwd_count: %d\ncwd_start: %d\ncwd_end: %d\n"), cwd_count, cwd_start, cwd_end);
            abort();

        }

        if (!get_opt_server_response_wrapper())
          logprintf(LOG_VERBOSE, "==> CWD (%d) %s ... ", cwd_count, quotearg_style(escape_quoting_style, target));
        err = ftp_cwd(csock, targ);
        switch (err)
        {
          case FTPRERR:
            logputs(LOG_VERBOSE, "\n");
            logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
            fd_close(csock);
            con->csock = -1;
            return err;

          case WRITEFAILED:
            logputs(LOG_VERBOSE, "\n");
            logputs(LOG_NOTQUIET, gettext("Write failed, closing control connection.\n"));
            fd_close(csock);
            con->csock = -1;
            return err;

          case FTPNSFOD:
            logputs(LOG_VERBOSE, "\n");
            logprintf(LOG_NOTQUIET, gettext("No such directory %s.\n\n"), quote(u->dir));
            fd_close(csock);
            con->csock = -1;
            return err;

          case FTPOK:
            break;

          default:
            abort();

        }

        if (!get_opt_server_response_wrapper())
          logputs(LOG_VERBOSE, gettext("done.\n"));
      }

    }
  }
  else
    logputs(LOG_VERBOSE, gettext("==> CWD not required.\n"));
  if ((cmd & DO_RETR) && (passed_expected_bytes == 0))
  {
    if (get_opt_verbose_wrapper())
    {
      if (!get_opt_server_response_wrapper())
        logprintf(LOG_VERBOSE, "==> SIZE %s ... ", quotearg_style(escape_quoting_style, u->file));
    }
    err = ftp_size(csock, u->file, &expected_bytes);
    switch (err)
    {
      case FTPRERR:

      case FTPSRVERR:
        logputs(LOG_VERBOSE, "\n");
        logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
        fd_close(csock);
        con->csock = -1;
        return err;

      case FTPOK:
        got_expected_bytes = 1;
        break;

      default:
        abort();

    }

    if (!get_opt_server_response_wrapper())
    {
      logprintf(LOG_VERBOSE, "%s\n", (expected_bytes) ? (number_to_static_string(expected_bytes)) : (gettext("done.\n")));
    }
  }
  if (((cmd & DO_RETR) && (restval > 0)) && (restval == expected_bytes))
  {
    logputs(LOG_VERBOSE, gettext("File has already been retrieved.\n"));
    fd_close(csock);
    con->csock = -1;
    return RETRFINISHED;
  }
  do
  {
    try_again = 0;
    if (cmd & (DO_LIST | DO_RETR))
    {
      if (get_opt_ftp_pasv_wrapper())
      {
        ip_address passive_addr;
        int passive_port;
        err = ftp_do_pasv(csock, &passive_addr, &passive_port);
        switch (err)
        {
          case FTPRERR:
            logputs(LOG_VERBOSE, "\n");
            logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
            fd_close(csock);
            con->csock = -1;
            return err;

          case WRITEFAILED:
            logputs(LOG_VERBOSE, "\n");
            logputs(LOG_NOTQUIET, gettext("Write failed, closing control connection.\n"));
            fd_close(csock);
            con->csock = -1;
            return err;

          case FTPNOPASV:
            logputs(LOG_VERBOSE, "\n");
            logputs(LOG_NOTQUIET, gettext("Cannot initiate PASV transfer.\n"));
            break;

          case FTPINVPASV:
            logputs(LOG_VERBOSE, "\n");
            logputs(LOG_NOTQUIET, gettext("Cannot parse PASV response.\n"));
            break;

          case FTPOK:
            break;

          default:
            abort();

        }

        if (err == FTPOK)
        {
          do
          {
            if (__builtin_expect(get_opt_debug_wrapper(), 0))
            {
              debug_logprintf("trying to connect to %s port %d\n", print_address(&passive_addr), passive_port);
            }
          }
          while (0);
          dtsock = connect_to_ip(&passive_addr, passive_port, 0);
          if (dtsock < 0)
          {
            int save_errno = errno;
            fd_close(csock);
            con->csock = -1;
            logprintf(LOG_VERBOSE, gettext("couldn't connect to %s port %d: %s\n"), print_address(&passive_addr), passive_port, strerror(save_errno));
            return (retryable_socket_connect_error(save_errno)) ? (CONERROR) : (CONIMPOSSIBLE);
          }
          if (!get_opt_server_response_wrapper())
            logputs(LOG_VERBOSE, gettext("done.    "));
        }
        else
          return err;
      }
      else
      {
        err = ftp_do_port(csock, &local_sock);
        switch (err)
        {
          case FTPRERR:
            logputs(LOG_VERBOSE, "\n");
            logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
            fd_close(csock);
            con->csock = -1;
            fd_close(dtsock);
            fd_close(local_sock);
            return err;

          case WRITEFAILED:
            logputs(LOG_VERBOSE, "\n");
            logputs(LOG_NOTQUIET, gettext("Write failed, closing control connection.\n"));
            fd_close(csock);
            con->csock = -1;
            fd_close(dtsock);
            fd_close(local_sock);
            return err;

          case CONSOCKERR:
            logputs(LOG_VERBOSE, "\n");
            logprintf(LOG_NOTQUIET, "socket: %s\n", strerror(errno));
            fd_close(csock);
            con->csock = -1;
            fd_close(dtsock);
            fd_close(local_sock);
            return err;

          case FTPSYSERR:
            logputs(LOG_VERBOSE, "\n");
            logprintf(LOG_NOTQUIET, gettext("Bind error (%s).\n"), strerror(errno));
            fd_close(dtsock);
            return err;

          case FTPPORTERR:
            logputs(LOG_VERBOSE, "\n");
            logputs(LOG_NOTQUIET, gettext("Invalid PORT.\n"));
            fd_close(csock);
            con->csock = -1;
            fd_close(dtsock);
            fd_close(local_sock);
            return err;

          case FTPOK:
            break;

          default:
            abort();

        }

        if (!get_opt_server_response_wrapper())
          logputs(LOG_VERBOSE, gettext("done.    "));
      }
    }
    if (restval && (cmd & DO_RETR))
    {
      if (!get_opt_server_response_wrapper())
        logprintf(LOG_VERBOSE, "==> REST %s ... ", number_to_static_string(restval));
      err = ftp_rest(csock, restval);
      switch (err)
      {
        case FTPRERR:
          logputs(LOG_VERBOSE, "\n");
          logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
          fd_close(csock);
          con->csock = -1;
          fd_close(dtsock);
          fd_close(local_sock);
          return err;

        case WRITEFAILED:
          logputs(LOG_VERBOSE, "\n");
          logputs(LOG_NOTQUIET, gettext("Write failed, closing control connection.\n"));
          fd_close(csock);
          con->csock = -1;
          fd_close(dtsock);
          fd_close(local_sock);
          return err;

        case FTPRESTFAIL:
          logputs(LOG_VERBOSE, gettext("\nREST failed, starting from scratch.\n"));
          rest_failed = 1;
          break;

        case FTPOK:
          break;

        default:
          abort();

      }

      if ((err != FTPRESTFAIL) && (!get_opt_server_response_wrapper()))
        logputs(LOG_VERBOSE, gettext("done.    "));
    }
    if (cmd & DO_RETR)
    {
      if (get_opt_spider_wrapper())
      {
        bool exists = 0;
        struct fileinfo *f;
        uerr_t _res = ftp_get_listing(u, original_url, con, &f);
        con->cmd |= DO_RETR;
        if (_res == RETROK)
        {
          while (f)
          {
            if (!strcmp(f->name, u->file))
            {
              exists = 1;
              break;
            }
            f = f->next;
          }

          if (exists)
          {
            logputs(LOG_VERBOSE, "\n");
            logprintf(LOG_NOTQUIET, gettext("File %s exists.\n"), quote(u->file));
          }
          else
          {
            logputs(LOG_VERBOSE, "\n");
            logprintf(LOG_NOTQUIET, gettext("No such file %s.\n"), quote(u->file));
          }
        }
        fd_close(csock);
        con->csock = -1;
        fd_close(dtsock);
        fd_close(local_sock);
        return RETRFINISHED;
      }
      if (get_opt_verbose_wrapper())
      {
        if (!get_opt_server_response_wrapper())
        {
          if (restval)
            logputs(LOG_VERBOSE, "\n");
          logprintf(LOG_VERBOSE, "==> RETR %s ... ", quotearg_style(escape_quoting_style, u->file));
        }
      }
      err = ftp_retr(csock, u->file);
      switch (err)
      {
        case FTPRERR:
          logputs(LOG_VERBOSE, "\n");
          logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
          fd_close(csock);
          con->csock = -1;
          fd_close(dtsock);
          fd_close(local_sock);
          return err;

        case WRITEFAILED:
          logputs(LOG_VERBOSE, "\n");
          logputs(LOG_NOTQUIET, gettext("Write failed, closing control connection.\n"));
          fd_close(csock);
          con->csock = -1;
          fd_close(dtsock);
          fd_close(local_sock);
          return err;

        case FTPNSFOD:
          logputs(LOG_VERBOSE, "\n");
          logprintf(LOG_NOTQUIET, gettext("No such file %s.\n\n"), quote(u->file));
          fd_close(dtsock);
          fd_close(local_sock);
          return err;

        case FTPOK:
          break;

        default:
          abort();

      }

      if (!get_opt_server_response_wrapper())
        logputs(LOG_VERBOSE, gettext("done.\n"));
      if (!got_expected_bytes)
        expected_bytes = *last_expected_bytes;
    }
    if (cmd & DO_LIST)
    {
      if (!get_opt_server_response_wrapper())
        logputs(LOG_VERBOSE, "==> LIST ... ");
      err = ftp_list(csock, 0, con->st & AVOID_LIST_A, con->st & AVOID_LIST, &list_a_used);
      switch (err)
      {
        case FTPRERR:
          logputs(LOG_VERBOSE, "\n");
          logputs(LOG_NOTQUIET, gettext("Error in server response, closing control connection.\n"));
          fd_close(csock);
          con->csock = -1;
          fd_close(dtsock);
          fd_close(local_sock);
          return err;

        case WRITEFAILED:
          logputs(LOG_VERBOSE, "\n");
          logputs(LOG_NOTQUIET, gettext("Write failed, closing control connection.\n"));
          fd_close(csock);
          con->csock = -1;
          fd_close(dtsock);
          fd_close(local_sock);
          return err;

        case FTPNSFOD:
          logputs(LOG_VERBOSE, "\n");
          logprintf(LOG_NOTQUIET, gettext("No such file or directory %s.\n\n"), quote("."));
          fd_close(dtsock);
          fd_close(local_sock);
          return err;

        case FTPOK:
          break;

        default:
          abort();

      }

      if (!get_opt_server_response_wrapper())
        logputs(LOG_VERBOSE, gettext("done.\n"));
      if (!got_expected_bytes)
        expected_bytes = *last_expected_bytes;
    }
    if ((!(cmd & (DO_LIST | DO_RETR))) || (get_opt_spider_wrapper() && (!(cmd & DO_LIST))))
      return RETRFINISHED;
    if (((passed_expected_bytes && restval) && expected_bytes) && (expected_bytes == (passed_expected_bytes - restval)))
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Lying FTP server found, adjusting.\n");
        }
      }
      while (0);
      expected_bytes = passed_expected_bytes;
    }
    if (!get_opt_ftp_pasv_wrapper())
    {
      dtsock = accept_connection(local_sock);
      if (dtsock < 0)
      {
        logprintf(LOG_NOTQUIET, "accept: %s\n", strerror(errno));
        return CONERROR;
      }
    }
    if ((!get_output_stream_wrapper()) || (con->cmd & DO_LIST))
    {
      mkalldirs(con->target);
      if (get_opt_backups_wrapper())
        rotate_backups(con->target);
      if (restval && (!(con->cmd & DO_LIST)))
      {
        fp = fopen(con->target, "ab");
      }
      else
        if (((((get_opt_noclobber_wrapper() || get_opt_always_rest_wrapper()) || get_opt_timestamping_wrapper()) || get_opt_dirstruct_wrapper()) || get_opt_output_document_wrapper()) || (count > 0))
      {
        if (get_opt_unlink_wrapper() && file_exists_p(con->target))
        {
          if (unlink(con->target) < 0)
          {
            logprintf(LOG_NOTQUIET, "%s: %s\n", con->target, strerror(errno));
            fd_close(csock);
            con->csock = -1;
            fd_close(dtsock);
            fd_close(local_sock);
            return UNLINKERR;
          }
        }
        fp = fopen(con->target, "wb");
      }
      else
      {
        fp = fopen_excl(con->target, 1);
        if ((!fp) && (errno == EEXIST))
        {
          logprintf(LOG_NOTQUIET, gettext("%s has sprung into existence.\n"), con->target);
          fd_close(csock);
          con->csock = -1;
          fd_close(dtsock);
          fd_close(local_sock);
          return FOPEN_EXCL_ERR;
        }
      }
      if (!fp)
      {
        logprintf(LOG_NOTQUIET, "%s: %s\n", con->target, strerror(errno));
        fd_close(csock);
        con->csock = -1;
        fd_close(dtsock);
        fd_close(local_sock);
        return FOPENERR;
      }
    }
    else
      fp = get_output_stream_wrapper();
    if (passed_expected_bytes)
    {
      print_length(passed_expected_bytes, restval, 1);
      expected_bytes = passed_expected_bytes;
    }
    else
      if (expected_bytes)
      print_length(expected_bytes, restval, 0);
    if ((u->scheme == SCHEME_FTPS) && using_data_security)
    {
      if ((!get_opt_ftps_resume_ssl_wrapper()) || (!ssl_connect_wget(dtsock, u->host, &csock)))
      {
        if (get_opt_ftps_resume_ssl_wrapper())
          logputs(LOG_NOTQUIET, "Server does not want to resume the SSL session. Trying with a new one.\n");
        if (!ssl_connect_wget(dtsock, u->host, 0))
        {
          fd_close(csock);
          fd_close(dtsock);
          err = CONERROR;
          logputs(LOG_NOTQUIET, "Could not perform SSL handshake.\n");
          goto exit_error;
        }
      }
      else
        logputs(LOG_NOTQUIET, "Resuming SSL session in data connection.\n");
      if (!ssl_check_certificate(dtsock, u->host))
      {
        fd_close(csock);
        fd_close(dtsock);
        err = CONERROR;
        goto exit_error;
      }
    }
    flags = 0;
    if (restval && rest_failed)
      flags |= rb_skip_startpos;
    rd_size = 0;
    res = fd_read_body(con->target, dtsock, fp, (expected_bytes) ? (expected_bytes - restval) : (0), restval, &rd_size, qtyread, &con->dltime, flags, warc_tmp);
    tms = datetime_str(time(0));
    tmrate = retr_rate(rd_size, con->dltime);
    set_total_download_time_wrapper(con->dltime);
    fd_close(local_sock);
    if ((!get_output_stream_wrapper()) || (con->cmd & DO_LIST))
      fclose(fp);
    if ((res == (-2)) || ((warc_tmp != 0) && (res == (-3))))
    {
      logprintf(LOG_NOTQUIET, gettext("%s: %s, closing control connection.\n"), con->target, strerror(errno));
      fd_close(csock);
      con->csock = -1;
      fd_close(dtsock);
      if (res == (-2))
        return FWRITEERR;
      else
        if (res == (-3))
        return WARC_TMP_FWRITEERR;
    }
    else
      if (res == (-1))
    {
      logprintf(LOG_NOTQUIET, gettext("%s (%s) - Data connection: %s; "), tms, tmrate, fd_errstr(dtsock));
      if (get_opt_server_response_wrapper())
        logputs(LOG_ALWAYS, "\n");
    }
    fd_close(dtsock);
    err = ftp_response(csock, &respline);
    if (err != FTPOK)
    {
      if (res != (-1))
        logprintf(LOG_NOTQUIET, "%s (%s) - ", tms, tmrate);
      logputs(LOG_NOTQUIET, gettext("Control connection closed.\n"));
      fd_close(csock);
      con->csock = -1;
      return FTPRETRINT;
    }
    *last_expected_bytes = ftp_expected_bytes(respline);
    if ((*respline) != '2')
    {
      if (res != (-1))
        logprintf(LOG_NOTQUIET, "%s (%s) - ", tms, tmrate);
      logputs(LOG_NOTQUIET, gettext("Data transfer aborted.\n"));
      if ((!c_strncasecmp(respline, "425", 3)) && (u->scheme == SCHEME_FTPS))
      {
        logputs(LOG_NOTQUIET, "FTPS server rejects new SSL sessions in the data connection.\n");
        do
        {
          free((void *) respline);
          respline = 0;
        }
        while (0);
        return FTPRESTFAIL;
      }
      do
      {
        free((void *) respline);
        respline = 0;
      }
      while (0);
      return FTPRETRINT;
    }
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    if (res == (-1))
    {
      return FTPRETRINT;
    }
    if (!(cmd & LEAVE_PENDING))
    {
      fd_close(csock);
      con->csock = -1;
    }
    if (con->cmd & DO_LIST)
    {
      if (get_opt_server_response_wrapper())
      {
        mkalldirs(con->target);
        fp = fopen(con->target, "r");
        if (!fp)
          logprintf(LOG_ALWAYS, "%s: %s\n", con->target, strerror(errno));
        else
        {
          char *line = 0;
          size_t bufsize = 0;
          ssize_t len;
          while ((len = getline(&line, &bufsize, fp)) > 0)
          {
            while ((len > 0) && ((line[len - 1] == '\n') || (line[len - 1] == '\r')))
              line[--len] = '\0';

            logprintf(LOG_ALWAYS, "%s\n", quotearg_style(escape_quoting_style, line));
          }

          do
          {
            free((void *) line);
            line = 0;
          }
          while (0);
          fclose(fp);
        }
      }
      if (!(con->st & LIST_AFTER_LIST_A_CHECK_DONE))
      {
        if (con->st & AVOID_LIST_A)
        {
          if (rd_size > previous_rd_size)
          {
            con->st |= LIST_AFTER_LIST_A_CHECK_DONE;
            do
            {
              if (__builtin_expect(get_opt_debug_wrapper(), 0))
              {
                debug_logprintf("LIST returned more data than \"LIST -a\": I will use \"LIST\" as standard list command\n");
              }
            }
            while (0);
          }
          else
            if (previous_rd_size > rd_size)
          {
            con->st |= LIST_AFTER_LIST_A_CHECK_DONE;
            con->st |= AVOID_LIST;
            con->st &= ~AVOID_LIST_A;
            try_again = 1;
            do
            {
              if (__builtin_expect(get_opt_debug_wrapper(), 0))
              {
                debug_logprintf("LIST returned less data than \"LIST -a\": I will use \"LIST -a\" as standard list command\n");
              }
            }
            while (0);
          }
          else
          {
            if (rd_size == 0)
            {
              con->st &= ~AVOID_LIST_A;
            }
            else
            {
              con->st |= LIST_AFTER_LIST_A_CHECK_DONE;
              con->st |= AVOID_LIST;
              con->st &= ~AVOID_LIST_A;
              do
              {
                if (__builtin_expect(get_opt_debug_wrapper(), 0))
                {
                  debug_logprintf("LIST returned the same amount of data of \"LIST -a\": I will use \"LIST -a\" as standard list command\n");
                }
              }
              while (0);
            }
          }
        }
        else
        {
          if (list_a_used)
          {
            previous_rd_size = rd_size;
            try_again = 1;
            con->st |= AVOID_LIST_A;
          }
          else
          {
            con->st |= LIST_AFTER_LIST_A_CHECK_DONE;
            con->st |= AVOID_LIST_A;
            do
            {
              if (__builtin_expect(get_opt_debug_wrapper(), 0))
              {
                debug_logprintf("\"LIST -a\" failed: I will use \"LIST\" as standard list command\n");
              }
            }
            while (0);
          }
        }
      }
    }
  }
  while (try_again);
  return RETRFINISHED;
  exit_error:
  if (fp && (!get_output_stream_wrapper()))
    fclose(fp);

  return err;
}



/* A one-file FTP loop.  This is the part where FTP retrieval is
   retried, and retried, and retried, and...

   This loop either gets commands from con, or (if ON_YOUR_OWN is
   set), makes them up to retrieve the file given by the URL.  */
static uerr_t ftp_loop_internal(struct url *u, struct url *original_url, struct fileinfo *f, ccon *con, char **local_file, bool force_full_retrieve)
{
  int count;
  int orig_lp;
  wgint restval;
  wgint len = 0;
  wgint qtyread = 0;
  char *tms;
  char *locf;
  const char *tmrate = 0;
  uerr_t err;
  struct stat st;
  bool warc_enabled = get_opt_warc_filename_wrapper() != 0;
  FILE *warc_tmp = 0;
  ip_address *warc_ip = 0;
  wgint last_expected_bytes = 0;
  if ((f == 0) && con->target)
  {
    locf = con->target;
  }
  else
  {
    do
    {
      free((void *) con->target);
      con->target = 0;
    }
    while (0);
    con->target = url_file_name((get_opt_trustservernames_wrapper() || (!original_url)) ? (u) : (original_url), 0);
    if (!get_opt_output_document_wrapper())
      locf = con->target;
    else
      locf = get_opt_output_document_wrapper();
  }
  if (((get_opt_noclobber_wrapper() && (!get_opt_output_document_wrapper())) && file_exists_p(con->target)) && (!((con->cmd & DO_LIST) && (!(con->cmd & DO_RETR)))))
  {
    logprintf(LOG_VERBOSE, gettext("File %s already there; not retrieving.\n"), quote(con->target));
    return RETROK;
  }
  remove_link(con->target);
  count = 0;
  if (con->st & ON_YOUR_OWN)
    con->st = ON_YOUR_OWN;
  orig_lp = (con->cmd & LEAVE_PENDING) ? (1) : (0);
  do
  {
    ++count;
    sleep_between_retrievals(count);
    if (con->st & ON_YOUR_OWN)
    {
      con->cmd = 0;
      con->cmd |= DO_RETR | LEAVE_PENDING;
      if (con->csock != (-1))
        con->cmd &= ~(DO_LOGIN | DO_CWD);
      else
        con->cmd |= DO_LOGIN | DO_CWD;
    }
    else
    {
      if (con->csock != (-1))
        con->cmd &= ~DO_LOGIN;
      else
        con->cmd |= DO_LOGIN;
      if (con->st & DONE_CWD)
        con->cmd &= ~DO_CWD;
      else
        con->cmd |= DO_CWD;
    }
    if ((warc_enabled && (con->cmd & DO_RETR)) && (warc_tmp == 0))
    {
      warc_tmp = warc_tempfile();
      if (warc_tmp == 0)
        return WARC_TMP_FOPENERR;
      if ((!con->proxy) && (con->csock != (-1)))
      {
        warc_ip = (ip_address *) alloca(sizeof(ip_address));
        socket_ip_address(con->csock, warc_ip, ENDPOINT_PEER);
      }
    }
    if (con->cmd & DO_LIST)
      restval = 0;
    else
      if (force_full_retrieve)
      restval = 0;
    else
      if (get_opt_start_pos_wrapper() >= 0)
      restval = get_opt_start_pos_wrapper();
    else
      if ((get_opt_always_rest_wrapper() && (stat(locf, &st) == 0)) && S_ISREG(st.st_mode))
      restval = st.st_size;
    else
      if (count > 1)
      restval = qtyread;
    else
      restval = 0;
    tms = datetime_str(time(0));
    if (get_opt_verbose_wrapper())
    {
      char *hurl = url_string(u, URL_AUTH_HIDE_PASSWD);
      char tmp[256];
      strcpy(tmp, "        ");
      if (count > 1)
        sprintf(tmp, gettext("(try:%2d)"), count);
      logprintf(LOG_VERBOSE, "--%s--  %s\n  %s => %s\n", tms, hurl, tmp, quote(locf));
      do
      {
        free((void *) hurl);
        hurl = 0;
      }
      while (0);
    }
    if (f && (f->type != FT_SYMLINK))
      len = f->size;
    else
      len = 0;
    err = getftp(u, original_url, len, &qtyread, restval, con, count, &last_expected_bytes, warc_tmp);
    if (con->csock == (-1))
      con->st &= ~DONE_CWD;
    else
      con->st |= DONE_CWD;
    switch (err)
    {
      case HOSTERR:

      case CONIMPOSSIBLE:

      case FWRITEERR:

      case FOPENERR:

      case FTPNSFOD:

      case FTPLOGINC:

      case FTPNOPASV:

      case FTPNOAUTH:

      case FTPNOPBSZ:

      case FTPNOPROT:

      case UNLINKERR:

      case WARC_TMP_FWRITEERR:

      case CONSSLERR:

      case CONTNOTSUPPORTED:
        if (err == FTPNOAUTH)
        logputs(LOG_NOTQUIET, "Server does not support AUTH TLS.\n");
        if (get_opt_ftps_implicit_wrapper())
        logputs(LOG_NOTQUIET, "Server does not like implicit FTPS connections.\n");
        if (warc_tmp != 0)
        fclose(warc_tmp);
        return err;

      case CONSOCKERR:

      case CONERROR:

      case FTPSRVERR:

      case FTPRERR:

      case WRITEFAILED:

      case FTPUNKNOWNTYPE:

      case FTPSYSERR:

      case FTPPORTERR:

      case FTPLOGREFUSED:

      case FTPINVPASV:

      case FOPEN_EXCL_ERR:
        printwhat(count, get_opt_ntry_wrapper());
        if (err == FOPEN_EXCL_ERR)
      {
        do
        {
          free((void *) con->target);
          con->target = 0;
        }
        while (0);
        con->target = url_file_name(u, 0);
        locf = con->target;
      }
        continue;

      case FTPRETRINT:
        if ((!f) || (qtyread != f->size))
      {
        printwhat(count, get_opt_ntry_wrapper());
        continue;
      }
        break;

      case RETRFINISHED:
        break;

      default:
        abort();

    }

    tms = datetime_str(time(0));
    if (!get_opt_spider_wrapper())
      tmrate = retr_rate(qtyread - restval, con->dltime);
    downloaded_file(FILE_DOWNLOADED_NORMALLY, locf);
    if (con->st & ON_YOUR_OWN)
    {
      fd_close(con->csock);
      con->csock = -1;
    }
    if (!get_opt_spider_wrapper())
    {
      bool write_to_stdout = get_opt_output_document_wrapper() && (((*get_opt_output_document_wrapper()) == '-') && (!(*(get_opt_output_document_wrapper() + 1))));
      logprintf(LOG_VERBOSE, (write_to_stdout) ? (gettext("%s (%s) - written to stdout %s[%s]\n\n")) : (gettext("%s (%s) - %s saved [%s]\n\n")), tms, tmrate, (write_to_stdout) ? ("") : (quote(locf)), number_to_static_string(qtyread));
    }
    if ((!get_opt_verbose_wrapper()) && (!get_opt_quiet_wrapper()))
    {
      char *hurl = url_string(u, URL_AUTH_HIDE_PASSWD);
      logprintf(LOG_NONVERBOSE, "%s URL: %s [%s] -> \"%s\" [%d]\n", tms, hurl, number_to_static_string(qtyread), locf, count);
      do
      {
        free((void *) hurl);
        hurl = 0;
      }
      while (0);
    }
    if (warc_enabled && (con->cmd & DO_RETR))
    {
      bool warc_res;
      warc_res = warc_write_resource_record(0, u->url, 0, 0, warc_ip, 0, warc_tmp, -1);
      if (!warc_res)
        return WARC_ERR;
      warc_tmp = 0;
    }
    if (con->cmd & DO_LIST)
    {
      if (!get_opt_remove_listing_wrapper())
      {
        set_total_downloaded_bytes_wrapper(qtyread);
        gsetter_numurls_postfix_wrapper();
      }
    }
    else
      if (!get_opt_spider_wrapper())
    {
      set_total_downloaded_bytes_wrapper(qtyread);
      gsetter_numurls_postfix_wrapper();
      if (get_opt_delete_after_wrapper() && (!input_file_url(get_opt_input_filename_wrapper())))
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Removing file due to --delete-after in ftp_loop_internal():\n");
          }
        }
        while (0);
        logprintf(LOG_VERBOSE, gettext("Removing %s.\n"), locf);
        if (unlink(locf))
          logprintf(LOG_NOTQUIET, "unlink: %s\n", strerror(errno));
      }
    }
    if (orig_lp)
      con->cmd |= LEAVE_PENDING;
    else
      con->cmd &= ~LEAVE_PENDING;
    if (local_file)
      *local_file = xstrdup(locf);
    if (warc_tmp != 0)
      fclose(warc_tmp);
    return RETROK;
  }
  while ((!get_opt_ntry_wrapper()) || (count < get_opt_ntry_wrapper()));
  if ((con->csock != (-1)) && (con->st & ON_YOUR_OWN))
  {
    fd_close(con->csock);
    con->csock = -1;
  }
  if (warc_tmp != 0)
    fclose(warc_tmp);
  return TRYLIMEXC;
}



/* Return the directory listing in a reusable format.  The directory
   is specifed in u->dir.  */
static uerr_t ftp_get_listing(struct url *u, struct url *original_url, ccon *con, struct fileinfo **f)
{
  uerr_t err;
  char *uf;
  char *lf;
  char *old_target = con->target;
  con->st &= ~ON_YOUR_OWN;
  con->cmd |= DO_LIST | LEAVE_PENDING;
  con->cmd &= ~DO_RETR;
  uf = url_file_name(u, 0);
  lf = file_merge(uf, ".listing");
  do
  {
    free((void *) uf);
    uf = 0;
  }
  while (0);
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf(gettext("Using %s as listing tmp file.\n"), quote(lf));
    }
  }
  while (0);
  con->target = xstrdup(lf);
  do
  {
    free((void *) lf);
    lf = 0;
  }
  while (0);
  err = ftp_loop_internal(u, original_url, 0, con, 0, 0);
  lf = xstrdup(con->target);
  do
  {
    free((void *) con->target);
    con->target = 0;
  }
  while (0);
  con->target = old_target;
  if (err == RETROK)
  {
    *f = ftp_parse_ls(lf, con->rs);
    if (get_opt_remove_listing_wrapper())
    {
      if (unlink(lf))
        logprintf(LOG_NOTQUIET, "unlink: %s\n", strerror(errno));
      else
        logprintf(LOG_VERBOSE, gettext("Removed %s.\n"), quote(lf));
    }
  }
  else
    *f = 0;
  do
  {
    free((void *) lf);
    lf = 0;
  }
  while (0);
  con->cmd &= ~DO_LIST;
  return err;
}



static uerr_t ftp_retrieve_dirs (struct url *, struct url *,
                                 struct fileinfo *, ccon *);
static uerr_t ftp_retrieve_glob (struct url *, struct url *, ccon *, int);
static struct fileinfo *delelement (struct fileinfo *, struct fileinfo **);
static void freefileinfo (struct fileinfo *f);

/* Retrieve a list of files given in struct fileinfo linked list.  If
   a file is a symbolic link, do not retrieve it, but rather try to
   set up a similar link on the local disk, if the symlinks are
   supported.

   If opt.recursive is set, after all files have been retrieved,
   ftp_retrieve_dirs will be called to retrieve the directories.  */
static uerr_t ftp_retrieve_list(struct url *u, struct url *original_url, struct fileinfo *f, ccon *con)
{
  static int depth = 0;
  uerr_t err;
  struct fileinfo *orig;
  wgint local_size;
  time_t tml;
  bool dlthis;
  const char *actual_target = 0;
  bool force_full_retrieve = 0;
  ++depth;
  if ((get_opt_reclevel_wrapper() != (-1)) && (depth > get_opt_reclevel_wrapper()))
  {
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf(gettext("Recursion depth %d exceeded max. depth %d.\n"), depth, get_opt_reclevel_wrapper());
      }
    }
    while (0);
    --depth;
    return RECLEVELEXC;
  }
  assert(f != 0);
  orig = f;
  con->st &= ~ON_YOUR_OWN;
  if (!(con->st & DONE_CWD))
    con->cmd |= DO_CWD;
  else
    con->cmd &= ~DO_CWD;
  con->cmd |= DO_RETR | LEAVE_PENDING;
  if (con->csock < 0)
    con->cmd |= DO_LOGIN;
  else
    con->cmd &= ~DO_LOGIN;
  err = RETROK;
  while (f)
  {
    char *old_target;
    char *ofile;
    if (get_opt_quota_wrapper() && (get_total_downloaded_bytes_wrapper() > get_opt_quota_wrapper()))
    {
      --depth;
      return QUOTEXC;
    }
    old_target = con->target;
    ofile = xstrdup(u->file);
    url_set_file(u, f->name);
    con->target = url_file_name(u, 0);
    err = RETROK;
    dlthis = 1;
    if (get_opt_timestamping_wrapper() && (f->type == FT_PLAINFILE))
    {
      struct stat st;
      if (!stat(con->target, &st))
      {
        bool eq_size;
        bool cor_val;
        local_size = st.st_size;
        tml = st.st_mtime;
        cor_val = (con->rs == ST_UNIX) || (con->rs == ST_WINNT);
        eq_size = (cor_val) ? (local_size == f->size) : (1);
        if ((f->tstamp <= tml) && eq_size)
        {
          logprintf(LOG_VERBOSE, gettext("Remote file no newer than local file %s -- not retrieving.\n"), quote(con->target));
          dlthis = 0;
        }
        else
          if (f->tstamp > tml)
        {
          force_full_retrieve = 1;
          logprintf(LOG_VERBOSE, gettext("Remote file is newer than local file %s -- retrieving.\n\n"), quote(con->target));
        }
        else
        {
          logprintf(LOG_VERBOSE, gettext("The sizes do not match (local %s) -- retrieving.\n\n"), number_to_static_string(local_size));
        }
      }
    }
    switch (f->type)
    {
      case FT_SYMLINK:
        if (!get_opt_retr_symlinks_wrapper())
      {
        if (!f->linkto)
          logputs(LOG_NOTQUIET, gettext("Invalid name of the symlink, skipping.\n"));
        else
        {
          struct stat st;
          int rc = lstat(con->target, &st);
          if (rc == 0)
          {
            size_t len = strlen(f->linkto) + 1;
            if (S_ISLNK(st.st_mode))
            {
              char *link_target = (char *) alloca(len);
              size_t n = readlink(con->target, link_target, len);
              if ((n == (len - 1)) && (memcmp(link_target, f->linkto, n) == 0))
              {
                logprintf(LOG_VERBOSE, gettext("Already have correct symlink %s -> %s\n\n"), quote(con->target), quote(f->linkto));
                dlthis = 0;
                break;
              }
            }
          }
          logprintf(LOG_VERBOSE, gettext("Creating symlink %s -> %s\n"), quote(con->target), quote(f->linkto));
          unlink(con->target);
          if (symlink(f->linkto, con->target) == (-1))
            logprintf(LOG_NOTQUIET, "symlink: %s\n", strerror(errno));
          logputs(LOG_VERBOSE, "\n");
        }
      }
      else
      {
        if (dlthis)
        {
          err = ftp_loop_internal(u, original_url, f, con, 0, force_full_retrieve);
        }
      }
        break;

      case FT_DIRECTORY:
        if (!get_opt_recursive_wrapper())
        logprintf(LOG_NOTQUIET, gettext("Skipping directory %s.\n"), quote(f->name));
        break;

      case FT_PLAINFILE:
        if (dlthis)
      {
        err = ftp_loop_internal(u, original_url, f, con, 0, force_full_retrieve);
      }
        break;

      case FT_UNKNOWN:
        logprintf(LOG_NOTQUIET, gettext("%s: unknown/unsupported file type.\n"), quote(f->name));
        break;

    }

    set_local_file(&actual_target, con->target);
    if (((dlthis && (actual_target != 0)) && (f->type == FT_PLAINFILE)) && get_opt_preserve_perm_wrapper())
    {
      if (f->perms)
        chmod(actual_target, f->perms);
      else
        do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Unrecognized permissions for %s.\n", actual_target);
        }
      }
      while (0);
    }
    if (actual_target != 0)
    {
      if ((((get_opt_useservertimestamps_wrapper() && (!((f->type == FT_SYMLINK) && (!get_opt_retr_symlinks_wrapper())))) && (f->tstamp != (-1))) && dlthis) && file_exists_p(con->target))
      {
        touch(actual_target, f->tstamp);
      }
      else
        if (f->tstamp == (-1))
        logprintf(LOG_NOTQUIET, gettext("%s: corrupt time-stamp.\n"), actual_target);
    }
    do
    {
      free((void *) con->target);
      con->target = 0;
    }
    while (0);
    con->target = old_target;
    url_set_file(u, ofile);
    do
    {
      free((void *) ofile);
      ofile = 0;
    }
    while (0);
    if ((((((err == QUOTEXC) || (err == HOSTERR)) || (err == FWRITEERR)) || (err == WARC_ERR)) || (err == WARC_TMP_FOPENERR)) || (err == WARC_TMP_FWRITEERR))
      break;
    con->cmd &= ~(DO_CWD | DO_LOGIN);
    f = f->next;
  }

  if (get_opt_recursive_wrapper() && (!((get_opt_reclevel_wrapper() != (-1)) && (depth >= get_opt_reclevel_wrapper()))))
    err = ftp_retrieve_dirs(u, original_url, orig, con);
  else
    if (get_opt_recursive_wrapper())
    do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf(gettext("Will not retrieve dirs since depth is %d (max %d).\n"), depth, get_opt_reclevel_wrapper());
    }
  }
  while (0);
  --depth;
  return err;
}



/* Retrieve the directories given in a file list.  This function works
   by simply going through the linked list and calling
   ftp_retrieve_glob on each directory entry.  The function knows
   about excluded directories.  */
static uerr_t ftp_retrieve_dirs(struct url *u, struct url *original_url, struct fileinfo *f, ccon *con)
{
  char *container = 0;
  int container_size = 0;
  for (; f; f = f->next)
  {
    int size;
    char *odir;
    char *newdir;
    if (get_opt_quota_wrapper() && (get_total_downloaded_bytes_wrapper() > get_opt_quota_wrapper()))
      break;
    if (f->type != FT_DIRECTORY)
      continue;
    size = ((strlen(u->dir) + 1) + strlen(f->name)) + 1;
    if (size > container_size)
      container = (char *) alloca(size);
    newdir = container;
    odir = u->dir;
    if (((*odir) == '\0') || (((*odir) == '/') && ((*(odir + 1)) == '\0')))
      sprintf(newdir, "%s%s", odir, f->name);
    else
      sprintf(newdir, "%s/%s", odir, f->name);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Composing new CWD relative to the initial directory.\n");
      }
    }
    while (0);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("  odir = '%s'\n  f->name = '%s'\n  newdir = '%s'\n\n", odir, f->name, newdir);
      }
    }
    while (0);
    if (!accdir(newdir))
    {
      logprintf(LOG_VERBOSE, gettext("Not descending to %s as it is excluded/not-included.\n"), quote(newdir));
      continue;
    }
    con->st &= ~DONE_CWD;
    odir = xstrdup(u->dir);
    url_set_dir(u, newdir);
    ftp_retrieve_glob(u, original_url, con, GLOB_GETALL);
    url_set_dir(u, odir);
    do
    {
      free((void *) odir);
      odir = 0;
    }
    while (0);
  }

  if (get_opt_quota_wrapper() && (get_total_downloaded_bytes_wrapper() > get_opt_quota_wrapper()))
    return QUOTEXC;
  else
    return RETROK;
}



/* Return true if S has a leading '/'  or contains '../' */
static bool
has_insecure_name_p (const char *s)
{
  if (*s == '/')
    return true;

  if (strstr (s, "../") != 0)
    return true;

  return false;
}

/* Test if the file node is invalid. This can occur due to malformed or
 * maliciously crafted listing files being returned by the server.
 *
 * Currently, this function only tests if there are multiple entries in the
 * listing file by the same name. However this function can be expanded as more
 * such illegal listing formats are discovered. */
static bool
is_invalid_entry (struct fileinfo *f)
{
  struct fileinfo *cur = f;
  char *f_name = f->name;

  /* If the node we're currently checking has a duplicate later, we eliminate
   * the current node and leave the next one intact. */
  while (cur->next)
    {
      cur = cur->next;
      if (strcmp(f_name, cur->name) == 0)
          return true;
    }
  return false;
}

/* A near-top-level function to retrieve the files in a directory.
   The function calls ftp_get_listing, to get a linked list of files.
   Then it weeds out the file names that do not match the pattern.
   ftp_retrieve_list is called with this updated list as an argument.

   If the argument ACTION is GLOB_GETONE, just download the file (but
   first get the listing, so that the time-stamp is heeded); if it's
   GLOB_GLOBALL, use globbing; if it's GLOB_GETALL, download the whole
   directory.  */
static uerr_t ftp_retrieve_glob(struct url *u, struct url *original_url, ccon *con, int action)
{
  struct fileinfo *f;
  struct fileinfo *start;
  uerr_t res;
  con->cmd |= LEAVE_PENDING;
  res = ftp_get_listing(u, original_url, con, &start);
  if (res != RETROK)
    return res;
  if (get_opt_accepts_wrapper() || get_opt_rejects_wrapper())
  {
    f = start;
    while (f)
    {
      if ((f->type != FT_DIRECTORY) && (!acceptable(f->name)))
      {
        logprintf(LOG_VERBOSE, gettext("Rejecting %s.\n"), quote(f->name));
        f = delelement(f, &start);
      }
      else
        f = f->next;
    }

  }
  f = start;
  while (f)
  {
    if (has_insecure_name_p(f->name) || is_invalid_entry(f))
    {
      logprintf(LOG_VERBOSE, gettext("Rejecting %s.\n"), quote(f->name));
      f = delelement(f, &start);
    }
    else
      f = f->next;
  }

  if (*u->file)
  {
    if (action == GLOB_GLOBALL)
    {
      int (*matcher)(const char *, const char *, int) = (get_opt_ignore_case_wrapper()) ? (fnmatch_nocase) : (fnmatch);
      int matchres = 0;
      f = start;
      while (f)
      {
        matchres = matcher(u->file, f->name, 0);
        if (matchres == (-1))
        {
          logprintf(LOG_NOTQUIET, gettext("Error matching %s against %s: %s\n"), u->file, quotearg_style(escape_quoting_style, f->name), strerror(errno));
          break;
        }
        if (matchres == FNM_NOMATCH)
          f = delelement(f, &start);
        else
          f = f->next;
      }

      if (matchres == (-1))
      {
        freefileinfo(start);
        return RETRBADPATTERN;
      }
    }
    else
      if (action == GLOB_GETONE)
    {
      int (*cmp)(const char *, const char *) = (get_opt_ignore_case_wrapper()) ? (strcasecmp) : (strcmp);
      f = start;
      while (f)
      {
        if (0 != cmp(u->file, f->name))
          f = delelement(f, &start);
        else
          f = f->next;
      }

    }
  }
  if (start)
  {
    res = ftp_retrieve_list(u, original_url, start, con);
  }
  else
  {
    if (action == GLOB_GLOBALL)
    {
      logprintf(LOG_VERBOSE, gettext("No matches on pattern %s.\n"), quote(u->file));
    }
    else
      if (action == GLOB_GETONE)
    {
      con->st |= ON_YOUR_OWN;
      res = ftp_loop_internal(u, original_url, 0, con, 0, 0);
      return res;
    }
  }
  freefileinfo(start);
  if (get_opt_quota_wrapper() && (get_total_downloaded_bytes_wrapper() > get_opt_quota_wrapper()))
    return QUOTEXC;
  else
    return res;
}



/* The wrapper that calls an appropriate routine according to contents
   of URL.  Inherently, its capabilities are limited on what can be
   encoded into a URL.  */
uerr_t ftp_loop(struct url *u, struct url *original_url, char **local_file, int *dt, struct url *proxy, bool recursive, bool glob)
{
  ccon con;
  uerr_t res;
  *dt = 0;
  memset(&con, '\0', sizeof(con));
  con.csock = -1;
  con.st = ON_YOUR_OWN;
  con.rs = ST_UNIX;
  con.id = 0;
  con.proxy = proxy;
  if ((!(*u->file)) && (!recursive))
  {
    struct fileinfo *f;
    res = ftp_get_listing(u, original_url, &con, &f);
    if (res == RETROK)
    {
      if (get_opt_htmlify_wrapper() && (!get_opt_spider_wrapper()))
      {
        struct url *url_file = (get_opt_trustservernames_wrapper()) ? (u) : (original_url);
        char *filename = (get_opt_output_document_wrapper()) ? (xstrdup(get_opt_output_document_wrapper())) : ((con.target) ? (xstrdup(con.target)) : (url_file_name(url_file, 0)));
        res = ftp_index(filename, u, f);
        if ((res == FTPOK) && get_opt_verbose_wrapper())
        {
          if (!get_opt_output_document_wrapper())
          {
            struct stat st;
            wgint sz;
            if (stat(filename, &st) == 0)
              sz = st.st_size;
            else
              sz = -1;
            logprintf(LOG_NOTQUIET, gettext("Wrote HTML-ized index to %s [%s].\n"), quote(filename), number_to_static_string(sz));
          }
          else
            logprintf(LOG_NOTQUIET, gettext("Wrote HTML-ized index to %s.\n"), quote(filename));
        }
        do
        {
          free((void *) filename);
          filename = 0;
        }
        while (0);
      }
      freefileinfo(f);
    }
  }
  else
  {
    bool ispattern = 0;
    if (glob)
    {
      char *file_part = strrchr(u->path, '/');
      if (!file_part)
        file_part = u->path;
      ispattern = has_wildcards_p(file_part);
    }
    if (((ispattern || recursive) || get_opt_timestamping_wrapper()) || get_opt_preserve_perm_wrapper())
    {
      res = ftp_retrieve_glob(u, original_url, &con, (ispattern) ? (GLOB_GLOBALL) : (GLOB_GETONE));
    }
    else
    {
      res = ftp_loop_internal(u, original_url, 0, &con, local_file, 0);
    }
  }
  if (res == FTPOK)
    res = RETROK;
  if (res == RETROK)
    *dt |= RETROKF;
  if (con.csock != (-1))
    fd_close(con.csock);
  do
  {
    free((void *) con.id);
    con.id = 0;
  }
  while (0);
  do
  {
    free((void *) con.target);
    con.target = 0;
  }
  while (0);
  return res;
}



/* Delete an element from the fileinfo linked list.  Returns the
   address of the next element, or NULL if the list is exhausted.  It
   can modify the start of the list.  */
static struct fileinfo *
delelement (struct fileinfo *f, struct fileinfo **start)
{
  struct fileinfo *prev = f->prev;
  struct fileinfo *next = f->next;

  xfree (f->name);
  xfree (f->linkto);
  xfree (f);

  if (next)
    next->prev = prev;
  if (prev)
    prev->next = next;
  else
    *start = next;
  return next;
}

/* Free the fileinfo linked list of files.  */
static void
freefileinfo (struct fileinfo *f)
{
  while (f)
    {
      struct fileinfo *next = f->next;
      xfree (f->name);
      if (f->linkto)
        xfree (f->linkto);
      xfree (f);
      f = next;
    }
}
