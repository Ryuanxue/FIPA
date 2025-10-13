/* Basic FTP routines.
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include "utils.h"
#include "connect.h"
#include "host.h"
#include "ftp.h"
#include "retr.h"
#include "c-strcase.h"

#include "wget_rpc_wrapper.h"


/* Get the response of FTP server and allocate enough room to handle
   it.  <CR> and <LF> characters are stripped from the line, and the
   line is 0-terminated.  All the response lines but the last one are
   skipped.  The last line is determined as described in RFC959.

   If the line is successfully read, FTPOK is returned, and *ret_line
   is assigned a freshly allocated line.  Otherwise, FTPRERR is
   returned, and the value of *ret_line should be ignored.  */

uerr_t ftp_response(int fd, char **ret_line)
{
  while (1)
  {
    char *p;
    char *line = fd_read_line(fd);
    if (!line)
      return FTPRERR;
    p = strchr(line, '\0');
    if ((p > line) && (p[-1] == '\n'))
      *(--p) = '\0';
    if ((p > line) && (p[-1] == '\r'))
      *(--p) = '\0';
    if (get_opt_server_response_wrapper())
      logprintf(LOG_NOTQUIET, "%s\n", quotearg_style(escape_quoting_style, line));
    else
      do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("%s\n", quotearg_style(escape_quoting_style, line));
      }
    }
    while (0);
    if (((c_isdigit(line[0]) && c_isdigit(line[1])) && c_isdigit(line[2])) && (line[3] == ' '))
    {
      *ret_line = line;
      return FTPOK;
    }
    do
    {
      free((void *) line);
      line = 0;
    }
    while (0);
  }

}



/* Returns the malloc-ed FTP request, ending with <CR><LF>, printing
   it if printing is required.  If VALUE is NULL, just use
   command<CR><LF>.  */
static char *ftp_request(const char *command, const char *value)
{
  char *res;
  if (value)
  {
    if (strpbrk(value, "\r\n"))
    {
      char *defanged;
      char *p;
      do
      {
        char **SA_dest = &defanged;
        const char *SA_src = value;
        *SA_dest = (char *) alloca(strlen(SA_src) + 1);
        strcpy(*SA_dest, SA_src);
      }
      while (0);
      for (p = defanged; *p; p++)
        if (((*p) == '\r') || ((*p) == '\n'))
        *p = ' ';

      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("\nDetected newlines in %s \"%s\"; changing to %s \"%s\"\n", command, quotearg_style(escape_quoting_style, value), command, quotearg_style(escape_quoting_style, defanged));
        }
      }
      while (0);
      value = defanged;
    }
    res = concat_strings(command, " ", value, "\r\n", (char *) 0);
  }
  else
    res = concat_strings(command, "\r\n", (char *) 0);
  if (get_opt_server_response_wrapper())
  {
    if (strncmp(res, "PASS", 4) != 0)
      logprintf(LOG_ALWAYS, "--> %s\n", res);
    else
      logputs(LOG_ALWAYS, "--> PASS Turtle Power!\n\n");
  }
  else
    do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("\n--> %s\n", res);
    }
  }
  while (0);
  return res;
}



uerr_t
ftp_greeting (int csock)
{
  uerr_t err = FTPOK;
  char *response = NULL;

  err = ftp_response (csock, &response);
  if (err != FTPOK)
    goto bail;
  if (*response != '2')
    err = FTPSRVERR;

bail:
  if (response)
    xfree (response);
  return err;
}
/* Sends the USER and PASS commands to the server, to control
   connection socket csock.  */
uerr_t
ftp_login (int csock, const char *acc, const char *pass)
{
  uerr_t err;
  char *request, *respline;
  int nwritten;

  /* Send USER username.  */
  request = ftp_request ("USER", acc);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      return WRITEFAILED;
    }
  xfree (request);
  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    return err;
  /* An unprobable possibility of logging without a password.  */
  if (*respline == '2')
    {
      xfree (respline);
      return FTPOK;
    }
  /* Else, only response 3 is appropriate.  */
  if (*respline != '3')
    {
      xfree (respline);
      return FTPLOGREFUSED;
    }
#ifdef ENABLE_OPIE
  {
    static const char *skey_head[] = {
      "331 s/key ",
      "331 opiekey "
    };
    size_t i;
    const char *seed = NULL;

    for (i = 0; i < countof (skey_head); i++)
      {
        int l = strlen (skey_head[i]);
        if (0 == c_strncasecmp (skey_head[i], respline, l))
          {
            seed = respline + l;
            break;
          }
      }
    if (seed)
      {
        int skey_sequence = 0;

        /* Extract the sequence from SEED.  */
        for (; c_isdigit (*seed); seed++)
          skey_sequence = 10 * skey_sequence + *seed - '0';
        if (*seed == ' ')
          ++seed;
        else
          {
            xfree (respline);
            return FTPLOGREFUSED;
          }
        /* Replace the password with the SKEY response to the
           challenge.  */
        pass = skey_response (skey_sequence, seed, pass);
      }
  }
#endif /* ENABLE_OPIE */
  xfree (respline);
  /* Send PASS password.  */
  request = ftp_request ("PASS", pass);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      return WRITEFAILED;
    }
  xfree (request);
  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    return err;
  if (*respline != '2')
    {
      xfree (respline);
      return FTPLOGINC;
    }
  xfree (respline);
  /* All OK.  */
  return FTPOK;
}

static void
ip_address_to_port_repr (const ip_address *addr, int port, char *buf,
                         size_t buflen)
{
  unsigned char *ptr;

  assert (addr->family == AF_INET);
  /* buf must contain the argument of PORT (of the form a,b,c,d,e,f). */
  assert (buflen >= 6 * 4);

  ptr = IP_INADDR_DATA (addr);
  snprintf (buf, buflen, "%d,%d,%d,%d,%d,%d", ptr[0], ptr[1],
            ptr[2], ptr[3], (port & 0xff00) >> 8, port & 0xff);
  buf[buflen - 1] = '\0';
}

/* Bind a port and send the appropriate PORT command to the FTP
   server.  Use acceptport after RETR, to get the socket of data
   connection.  */
uerr_t
ftp_port (int csock, int *local_sock)
{
  uerr_t err;
  char *request, *respline;
  ip_address addr;
  int nwritten;
  int port;
  /* Must contain the argument of PORT (of the form a,b,c,d,e,f). */
  char bytes[6 * 4 + 1];

  /* Get the address of this side of the connection. */
  if (!socket_ip_address (csock, &addr, ENDPOINT_LOCAL))
    return FTPSYSERR;

  assert (addr.family == AF_INET);

  /* Setting port to 0 lets the system choose a free port.  */
  port = 0;

  /* Bind the port.  */
  *local_sock = bind_local (&addr, &port);
  if (*local_sock < 0)
    return FTPSYSERR;

  /* Construct the argument of PORT (of the form a,b,c,d,e,f). */
  ip_address_to_port_repr (&addr, port, bytes, sizeof (bytes));

  /* Send PORT request.  */
  request = ftp_request ("PORT", bytes);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      fd_close (*local_sock);
      return WRITEFAILED;
    }
  xfree (request);

  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    {
      fd_close (*local_sock);
      return err;
    }
  if (*respline != '2')
    {
      xfree (respline);
      fd_close (*local_sock);
      return FTPPORTERR;
    }
  xfree (respline);
  return FTPOK;
}

#ifdef ENABLE_IPV6
static void
ip_address_to_lprt_repr (const ip_address *addr, int port, char *buf,
                         size_t buflen)
{
  unsigned char *ptr = IP_INADDR_DATA (addr);

  /* buf must contain the argument of LPRT (of the form af,n,h1,h2,...,hn,p1,p2). */
  assert (buflen >= 21 * 4);

  /* Construct the argument of LPRT (of the form af,n,h1,h2,...,hn,p1,p2). */
  switch (addr->family)
    {
    case AF_INET:
      snprintf (buf, buflen, "%d,%d,%d,%d,%d,%d,%d,%d,%d", 4, 4,
                ptr[0], ptr[1], ptr[2], ptr[3], 2,
                (port & 0xff00) >> 8, port & 0xff);
      break;
    case AF_INET6:
      snprintf (buf, buflen,
                "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                6, 16,
                ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7],
                ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15],
                2, (port & 0xff00) >> 8, port & 0xff);
      break;
    default:
      abort ();
    }
}

/* Bind a port and send the appropriate PORT command to the FTP
   server.  Use acceptport after RETR, to get the socket of data
   connection.  */
uerr_t
ftp_lprt (int csock, int *local_sock)
{
  uerr_t err;
  char *request, *respline;
  ip_address addr;
  int nwritten;
  int port;
  /* Must contain the argument of LPRT (of the form af,n,h1,h2,...,hn,p1,p2). */
  char bytes[21 * 4 + 1];

  /* Get the address of this side of the connection. */
  if (!socket_ip_address (csock, &addr, ENDPOINT_LOCAL))
    return FTPSYSERR;

  assert (addr.family == AF_INET || addr.family == AF_INET6);

  /* Setting port to 0 lets the system choose a free port.  */
  port = 0;

  /* Bind the port.  */
  *local_sock = bind_local (&addr, &port);
  if (*local_sock < 0)
    return FTPSYSERR;

  /* Construct the argument of LPRT (of the form af,n,h1,h2,...,hn,p1,p2). */
  ip_address_to_lprt_repr (&addr, port, bytes, sizeof (bytes));

  /* Send PORT request.  */
  request = ftp_request ("LPRT", bytes);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      fd_close (*local_sock);
      return WRITEFAILED;
    }
  xfree (request);
  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    {
      fd_close (*local_sock);
      return err;
    }
  if (*respline != '2')
    {
      xfree (respline);
      fd_close (*local_sock);
      return FTPPORTERR;
    }
  xfree (respline);
  return FTPOK;
}

static void
ip_address_to_eprt_repr (const ip_address *addr, int port, char *buf,
                         size_t buflen)
{
  int afnum;

  /* buf must contain the argument of EPRT (of the form |af|addr|port|).
   * 4 chars for the | separators, INET6_ADDRSTRLEN chars for addr
   * 1 char for af (1-2) and 5 chars for port (0-65535) */
  assert (buflen >= 4 + INET6_ADDRSTRLEN + 1 + 5);

  /* Construct the argument of EPRT (of the form |af|addr|port|). */
  afnum = (addr->family == AF_INET ? 1 : 2);
  snprintf (buf, buflen, "|%d|%s|%d|", afnum, print_address (addr), port);
  buf[buflen - 1] = '\0';
}

/* Bind a port and send the appropriate PORT command to the FTP
   server.  Use acceptport after RETR, to get the socket of data
   connection.  */
uerr_t
ftp_eprt (int csock, int *local_sock)
{
  uerr_t err;
  char *request, *respline;
  ip_address addr;
  int nwritten;
  int port;
  /* Must contain the argument of EPRT (of the form |af|addr|port|).
   * 4 chars for the | separators, INET6_ADDRSTRLEN chars for addr
   * 1 char for af (1-2) and 5 chars for port (0-65535) */
  char bytes[4 + INET6_ADDRSTRLEN + 1 + 5 + 1];

  /* Get the address of this side of the connection. */
  if (!socket_ip_address (csock, &addr, ENDPOINT_LOCAL))
    return FTPSYSERR;

  /* Setting port to 0 lets the system choose a free port.  */
  port = 0;

  /* Bind the port.  */
  *local_sock = bind_local (&addr, &port);
  if (*local_sock < 0)
    return FTPSYSERR;

  /* Construct the argument of EPRT (of the form |af|addr|port|). */
  ip_address_to_eprt_repr (&addr, port, bytes, sizeof (bytes));

  /* Send PORT request.  */
  request = ftp_request ("EPRT", bytes);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      fd_close (*local_sock);
      return WRITEFAILED;
    }
  xfree (request);
  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    {
      fd_close (*local_sock);
      return err;
    }
  if (*respline != '2')
    {
      xfree (respline);
      fd_close (*local_sock);
      return FTPPORTERR;
    }
  xfree (respline);
  return FTPOK;
}
#endif

#ifdef HAVE_SSL
/*
 * The following three functions defined into this #ifdef block
 * wrap the extended FTP commands defined in RFC 2228 (FTP Security Extensions).
 * Currently, only FTPS is supported, so these functions are only compiled when SSL
 * support is available, because there's no point in using FTPS when there's no SSL.
 * Shall someone add new secure FTP protocols in the future, feel free to remove this
 * #ifdef, or add new constants to it.
 */

/*
 * Sends an AUTH command as defined by RFC 2228,
 * deriving its argument from the scheme. For example, if the provided scheme
 * is SCHEME_FTPS, the command sent will be "AUTH TLS". Currently, this is the only
 * scheme supported, so this function will return FTPNOAUTH when supplied a different
 * one. It will also return FTPNOAUTH if the target server does not support FTPS.
 */
uerr_t
ftp_auth (int csock, enum url_scheme scheme)
{
  uerr_t err = 0;
  int written = 0;
  char *request = NULL, *response = NULL;

  if (scheme == SCHEME_FTPS)
    {
      request = ftp_request ("AUTH", "TLS");
      written = fd_write (csock, request, strlen (request), -1);
      if (written < 0)
        {
          err = WRITEFAILED;
          goto bail;
        }
      err = ftp_response (csock, &response);
      if (err != FTPOK)
        goto bail;
      if (*response != '2')
        err = FTPNOAUTH;
    }
  else
    err = FTPNOAUTH;

bail:
  xfree (request);
  xfree (response);

  return err;
}

uerr_t
ftp_pbsz (int csock, int pbsz)
{
  uerr_t err = 0;
  int written = 0;
  char spbsz[5];
  char *request = NULL, *response = NULL;

  snprintf (spbsz, 5, "%d", pbsz);
  request = ftp_request ("PBSZ", spbsz);
  written = fd_write (csock, request, strlen (request), -1);
  if (written < 0)
    {
      err = WRITEFAILED;
      goto bail;
    }

  err = ftp_response (csock, &response);
  if (err != FTPOK)
    goto bail;
  if (*response != '2')
    err = FTPNOPBSZ;

bail:
  xfree (request);
  xfree (response);

  return err;
}

uerr_t
ftp_prot (int csock, enum prot_level prot)
{
  uerr_t err = 0;
  int written = 0;
  char *request = NULL, *response = NULL;
  /* value must be a single character value */
  char value[2];

  value[0] = prot;
  value[1] = '\0';

  request = ftp_request ("PROT", value);
  written = fd_write (csock, request, strlen (request), -1);
  if (written < 0)
    {
      err = WRITEFAILED;
      goto bail;
    }

  err = ftp_response (csock, &response);
  if (err != FTPOK)
    goto bail;
  if (*response != '2')
    err = FTPNOPROT;

bail:
  xfree (request);
  xfree (response);

  return err;
}
#endif /* HAVE_SSL */

/* Similar to ftp_port, but uses `PASV' to initiate the passive FTP
   transfer.  Reads the response from server and parses it.  Reads the
   host and port addresses and returns them.  */
uerr_t
ftp_pasv (int csock, ip_address *addr, int *port)
{
  char *request, *respline, *s;
  int nwritten, i;
  uerr_t err;
  unsigned char tmp[6];

  assert (addr != NULL);
  assert (port != NULL);

  xzero (*addr);

  /* Form the request.  */
  request = ftp_request ("PASV", NULL);
  /* And send it.  */
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      return WRITEFAILED;
    }
  xfree (request);
  /* Get the server response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    return err;
  if (*respline != '2')
    {
      xfree (respline);
      return FTPNOPASV;
    }
  /* Parse the request.  */
  s = respline;
  for (s += 4; *s && !c_isdigit (*s); s++)
    ;
  if (!*s)
    {
      xfree (respline);
      return FTPINVPASV;
    }
  for (i = 0; i < 6; i++)
    {
      tmp[i] = 0;
      for (; c_isdigit (*s); s++)
        tmp[i] = (*s - '0') + 10 * tmp[i];
      if (*s == ',')
        s++;
      else if (i < 5)
        {
          /* When on the last number, anything can be a terminator.  */
          xfree (respline);
          return FTPINVPASV;
        }
    }
  xfree (respline);

  addr->family = AF_INET;
  memcpy (IP_INADDR_DATA (addr), tmp, 4);
  *port = ((tmp[4] << 8) & 0xff00) + tmp[5];

  return FTPOK;
}

#ifdef ENABLE_IPV6
/* Similar to ftp_lprt, but uses `LPSV' to initiate the passive FTP
   transfer.  Reads the response from server and parses it.  Reads the
   host and port addresses and returns them.  */
uerr_t ftp_lpsv(int csock, ip_address *addr, int *port)
{
  char *request;
  char *respline;
  char *s;
  int nwritten;
  int i;
  int af;
  int addrlen;
  int portlen;
  uerr_t err;
  unsigned char tmp[16];
  unsigned char tmpprt[2];
  assert(addr != 0);
  assert(port != 0);
  memset(&(*addr), '\0', sizeof(*addr));
  request = ftp_request("LPSV", 0);
  nwritten = fd_write(csock, request, strlen(request), -1);
  if (nwritten < 0)
  {
    do
    {
      free((void *) request);
      request = 0;
    }
    while (0);
    return WRITEFAILED;
  }
  do
  {
    free((void *) request);
    request = 0;
  }
  while (0);
  err = ftp_response(csock, &respline);
  if (err != FTPOK)
    return err;
  if ((*respline) != '2')
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPNOPASV;
  }
  s = respline;
  for (s += 4; (*s) && (!c_isdigit(*s)); s++)
    ;

  if (!(*s))
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  af = 0;
  for (; c_isdigit(*s); s++)
    af = ((*s) - '0') + (10 * af);

  if ((af != 4) && (af != 6))
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  if ((!(*s)) || ((*(s++)) != ','))
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  addrlen = 0;
  for (; c_isdigit(*s); s++)
    addrlen = ((*s) - '0') + (10 * addrlen);

  if ((!(*s)) || ((*(s++)) != ','))
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  if (addrlen > 16)
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  if (((af == 4) && (addrlen != 4)) || ((af == 6) && (addrlen != 16)))
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  for (i = 0; i < addrlen; i++)
  {
    tmp[i] = 0;
    for (; c_isdigit(*s); s++)
      tmp[i] = ((*s) - '0') + (10 * tmp[i]);

    if ((*s) == ',')
      s++;
    else
    {
      do
      {
        free((void *) respline);
        respline = 0;
      }
      while (0);
      return FTPINVPASV;
    }
  }

  portlen = 0;
  for (; c_isdigit(*s); s++)
    portlen = ((*s) - '0') + (10 * portlen);

  if ((!(*s)) || ((*(s++)) != ','))
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  if (portlen > 2)
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  tmpprt[0] = 0;
  for (; c_isdigit(*s); s++)
    tmpprt[0] = ((*s) - '0') + (10 * tmpprt[0]);

  if ((!(*s)) || ((*(s++)) != ','))
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  tmpprt[1] = 0;
  for (; c_isdigit(*s); s++)
    tmpprt[1] = ((*s) - '0') + (10 * tmpprt[1]);

  assert(s != 0);
  if (af == 4)
  {
    addr->family = AF_INET;
    memcpy((void *) (&addr->data), tmp, 4);
    *port = ((tmpprt[0] << 8) & 0xff00) + tmpprt[1];
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("lpsv addr is: %s\n", print_address(addr));
      }
    }
    while (0);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("tmpprt[0] is: %d\n", tmpprt[0]);
      }
    }
    while (0);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("tmpprt[1] is: %d\n", tmpprt[1]);
      }
    }
    while (0);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("*port is: %d\n", *port);
      }
    }
    while (0);
  }
  else
  {
    assert(af == 6);
    addr->family = AF_INET6;
    memcpy((void *) (&addr->data), tmp, 16);
    *port = ((tmpprt[0] << 8) & 0xff00) + tmpprt[1];
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("lpsv addr is: %s\n", print_address(addr));
      }
    }
    while (0);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("tmpprt[0] is: %d\n", tmpprt[0]);
      }
    }
    while (0);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("tmpprt[1] is: %d\n", tmpprt[1]);
      }
    }
    while (0);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("*port is: %d\n", *port);
      }
    }
    while (0);
  }
  do
  {
    free((void *) respline);
    respline = 0;
  }
  while (0);
  return FTPOK;
}



/* Similar to ftp_eprt, but uses `EPSV' to initiate the passive FTP
   transfer.  Reads the response from server and parses it.  Reads the
   host and port addresses and returns them.  */
uerr_t ftp_epsv(int csock, ip_address *ip, int *port)
{
  char *request;
  char *respline;
  char *start;
  char delim;
  char *s;
  int nwritten;
  int i;
  uerr_t err;
  int tport;
  assert(ip != 0);
  assert(port != 0);
  request = ftp_request("EPSV", (ip->family == AF_INET) ? ("1") : ("2"));
  nwritten = fd_write(csock, request, strlen(request), -1);
  if (nwritten < 0)
  {
    do
    {
      free((void *) request);
      request = 0;
    }
    while (0);
    return WRITEFAILED;
  }
  do
  {
    free((void *) request);
    request = 0;
  }
  while (0);
  err = ftp_response(csock, &respline);
  if (err != FTPOK)
    return err;
  if ((*respline) != '2')
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPNOPASV;
  }
  assert(respline != 0);
  do
  {
    if (__builtin_expect(get_opt_debug_wrapper(), 0))
    {
      debug_logprintf("respline is %s\n", respline);
    }
  }
  while (0);
  start = strchr(respline, '(');
  if (start == 0)
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  s = start + 1;
  delim = *(s++);
  if ((delim < 33) || (delim > 126))
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  for (i = 0; i < 2; i++)
  {
    if ((*(s++)) != delim)
    {
      do
      {
        free((void *) respline);
        respline = 0;
      }
      while (0);
      return FTPINVPASV;
    }
  }

  for (tport = 0, i = 0; (i < 5) && c_isdigit(*s); i++, s++)
    tport = ((*s) - '0') + (10 * tport);

  if ((*(s++)) != delim)
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  if ((*s) != ')')
  {
    do
    {
      free((void *) respline);
      respline = 0;
    }
    while (0);
    return FTPINVPASV;
  }
  *port = tport;
  do
  {
    free((void *) respline);
    respline = 0;
  }
  while (0);
  return FTPOK;
}


#endif

/* Sends the TYPE request to the server.  */
uerr_t
ftp_type (int csock, int type)
{
  char *request, *respline;
  int nwritten;
  uerr_t err;
  char stype[2];

  /* Construct argument.  */
  stype[0] = type;
  stype[1] = 0;
  /* Send TYPE request.  */
  request = ftp_request ("TYPE", stype);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      return WRITEFAILED;
    }
  xfree (request);
  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    return err;
  if (*respline != '2')
    {
      xfree (respline);
      return FTPUNKNOWNTYPE;
    }
  xfree (respline);
  /* All OK.  */
  return FTPOK;
}

/* Changes the working directory by issuing a CWD command to the
   server.  */
uerr_t
ftp_cwd (int csock, const char *dir)
{
  char *request, *respline;
  int nwritten;
  uerr_t err;

  /* Send CWD request.  */
  request = ftp_request ("CWD", dir);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      return WRITEFAILED;
    }
  xfree (request);
  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    return err;
  if (*respline == '5')
    {
      xfree (respline);
      return FTPNSFOD;
    }
  if (*respline != '2')
    {
      xfree (respline);
      return FTPRERR;
    }
  xfree (respline);
  /* All OK.  */
  return FTPOK;
}

/* Sends REST command to the FTP server.  */
uerr_t
ftp_rest (int csock, wgint offset)
{
  char *request, *respline;
  int nwritten;
  uerr_t err;

  request = ftp_request ("REST", number_to_static_string (offset));
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      return WRITEFAILED;
    }
  xfree (request);
  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    return err;
  if (*respline != '3')
    {
      xfree (respline);
      return FTPRESTFAIL;
    }
  xfree (respline);
  /* All OK.  */
  return FTPOK;
}

/* Sends RETR command to the FTP server.  */
uerr_t
ftp_retr (int csock, const char *file)
{
  char *request, *respline;
  int nwritten;
  uerr_t err;

  /* Send RETR request.  */
  request = ftp_request ("RETR", file);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      return WRITEFAILED;
    }
  xfree (request);
  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    return err;
  if (*respline == '5')
    {
      xfree (respline);
      return FTPNSFOD;
    }
  if (*respline != '1')
    {
      xfree (respline);
      return FTPRERR;
    }
  xfree (respline);
  /* All OK.  */
  return FTPOK;
}

/* Sends the LIST command to the server.  If FILE is NULL, send just
   `LIST' (no space).  */
uerr_t ftp_list(int csock, const char *file, bool avoid_list_a, bool avoid_list, bool *list_a_used)
{
  char *request;
  char *respline;
  int nwritten;
  uerr_t err;
  bool ok = 0;
  size_t i = 0;
  static const char *list_commands[] = {"LIST -a", "LIST"};
  *list_a_used = 0;
  if (avoid_list_a)
  {
    i = ((sizeof(list_commands)) / (sizeof(list_commands[0]))) - 1;
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("(skipping \"LIST -a\")");
      }
    }
    while (0);
  }
  do
  {
    request = ftp_request(list_commands[i], file);
    nwritten = fd_write(csock, request, strlen(request), -1);
    if (nwritten < 0)
    {
      do
      {
        free((void *) request);
        request = 0;
      }
      while (0);
      return WRITEFAILED;
    }
    do
    {
      free((void *) request);
      request = 0;
    }
    while (0);
    err = ftp_response(csock, &respline);
    if (err == FTPOK)
    {
      if ((*respline) == '5')
      {
        err = FTPNSFOD;
      }
      else
        if ((*respline) == '1')
      {
        err = FTPOK;
        ok = 1;
        *list_a_used = i == 0;
      }
      else
      {
        err = FTPRERR;
      }
      do
      {
        free((void *) respline);
        respline = 0;
      }
      while (0);
    }
    ++i;
    if (avoid_list && (i == 1))
    {
      ++i;
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("(skipping \"LIST\")");
        }
      }
      while (0);
    }
  }
  while ((i < ((sizeof(list_commands)) / (sizeof(list_commands[0])))) && (!ok));
  return err;
}



/* Sends the SYST command to the server. */
uerr_t
ftp_syst (int csock, enum stype *server_type, enum ustype *unix_type)
{
  char *request, *respline;
  int nwritten;
  uerr_t err;
  char *ftp_last_respline;

  /* Send SYST request.  */
  request = ftp_request ("SYST", NULL);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      return WRITEFAILED;
    }
  xfree (request);

  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    return err;
  if (*respline == '5')
    {
      xfree (respline);
      return FTPSRVERR;
    }

  ftp_last_respline = strdup (respline);

  /* Skip the number (215, but 200 (!!!) in case of VMS) */
  strtok (respline, " ");

  /* Which system type has been reported (we are interested just in the
     first word of the server response)?  */
  request = strtok (NULL, " ");

  *unix_type = UST_OTHER;

  if (request == NULL)
    *server_type = ST_OTHER;
  else if (!c_strcasecmp (request, "VMS"))
    *server_type = ST_VMS;
  else if (!c_strcasecmp (request, "UNIX"))
    {
      *server_type = ST_UNIX;
      /* 2013-10-17 Andrea Urbani (matfanjol)
         I check more in depth the system type */
      if (!c_strncasecmp (ftp_last_respline, "215 UNIX Type: L8", 17))
        *unix_type = UST_TYPE_L8;
      else if (!c_strncasecmp (ftp_last_respline,
                             "215 UNIX MultiNet Unix Emulation V5.3(93)", 41))
        *unix_type = UST_MULTINET;
    }
  else if (!c_strcasecmp (request, "WINDOWS_NT")
           || !c_strcasecmp (request, "WINDOWS2000"))
    *server_type = ST_WINNT;
  else if (!c_strcasecmp (request, "MACOS"))
    *server_type = ST_MACOS;
  else if (!c_strcasecmp (request, "OS/400"))
    *server_type = ST_OS400;
  else
    *server_type = ST_OTHER;

  xfree (ftp_last_respline);
  xfree (respline);
  /* All OK.  */
  return FTPOK;
}

/* Sends the PWD command to the server. */
uerr_t
ftp_pwd (int csock, char **pwd)
{
  char *request, *respline;
  int nwritten;
  uerr_t err;

  /* Send PWD request.  */
  request = ftp_request ("PWD", NULL);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      return WRITEFAILED;
    }
  xfree (request);
  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    return err;
  if (*respline == '5')
    {
    err:
      xfree (respline);
      return FTPSRVERR;
    }

  /* Skip the number (257), leading citation mark, trailing citation mark
     and everything following it. */
  strtok (respline, "\"");
  request = strtok (NULL, "\"");
  if (!request)
    /* Treat the malformed response as an error, which the caller has
       to handle gracefully anyway.  */
    goto err;

  /* Has the `pwd' been already allocated?  Free! */
  xfree (*pwd);

  *pwd = xstrdup (request);

  xfree (respline);
  /* All OK.  */
  return FTPOK;
}

/* Sends the SIZE command to the server, and returns the value in 'size'.
 * If an error occurs, size is set to zero. */
uerr_t
ftp_size (int csock, const char *file, wgint *size)
{
  char *request, *respline;
  int nwritten;
  uerr_t err;

  /* Send PWD request.  */
  request = ftp_request ("SIZE", file);
  nwritten = fd_write (csock, request, strlen (request), -1);
  if (nwritten < 0)
    {
      xfree (request);
      *size = 0;
      return WRITEFAILED;
    }
  xfree (request);
  /* Get appropriate response.  */
  err = ftp_response (csock, &respline);
  if (err != FTPOK)
    {
      *size = 0;
      return err;
    }
  if (*respline == '5')
    {
      /*
       * Probably means SIZE isn't supported on this server.
       * Error is nonfatal since SIZE isn't in RFC 959
       */
      xfree (respline);
      *size = 0;
      return FTPOK;
    }

  errno = 0;
  *size = str_to_wgint (respline + 4, NULL, 10);
  if (errno)
    {
      /*
       * Couldn't parse the response for some reason.  On the (few)
       * tests I've done, the response is 213 <SIZE> with nothing else -
       * maybe something a bit more resilient is necessary.  It's not a
       * fatal error, however.
       */
      xfree (respline);
      *size = 0;
      return FTPOK;
    }

  xfree (respline);
  /* All OK.  */
  return FTPOK;
}

/* If URL's params are of the form "type=X", return character X.
   Otherwise, return 'I' (the default type).  */
char
ftp_process_type (const char *params)
{
  if (params
      && 0 == strncasecmp (params, "type=", 5)
      && params[5] != '\0')
    return c_toupper (params[5]);
  else
    return 'I';
}
