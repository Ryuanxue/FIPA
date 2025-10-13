/*
  Copyright (C) 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003,
  2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014,
  2015 Free Software Foundation, Inc.

  This file is part of GNU Inetutils.

  GNU Inetutils is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at
  your option) any later version.

  GNU Inetutils is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see `http://www.gnu.org/licenses/'. */

/*
 * Copyright (c) 1988, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <config.h>

#include <sys/types.h>

#include <stdlib.h>
#ifdef HAVE_LOCALE_H
# include <locale.h>
#endif

#include "ring.h"
#include "defines.h"
#include "externs.h"

#include <progname.h>
#include <error.h>
#include <argp.h>
#include <unused-parameter.h>
#include <libinetutils.h>

#include <arpa/telnet.h>

#include "telnet_rpc_wrapper.h"

#ifdef	AUTHENTICATION
# include <libtelnet/auth.h>
#endif
#ifdef	ENCRYPTION
# include <libtelnet/encrypt.h>
#endif

/* These values need to be the same as defined in libtelnet/kerberos5.c */
/* Either define them in both places, or put in some common header file. */
#define OPTS_FORWARD_CREDS           0x00000002
#define OPTS_FORWARDABLE_CREDS       0x00000001

#if 0
# define FORWARD
#endif

/*
 * Initialize variables.
 */
void
tninit (void)
{
  init_terminal ();

  init_network ();

  init_telnet ();

  init_sys ();

#if defined TN3270
  init_3270 ();
#endif
}

int family = 0;
char *user;
#ifdef	FORWARD
extern int forward_flags;
#endif /* FORWARD */

enum {
  OPTION_NOASYNCH = 256,
  OPTION_NOASYNCTTY,
  OPTION_NOASYNCNET
};

#if defined KERBEROS || defined SHISHI
extern char *dest_realm;
#endif

static struct argp_option argp_options[] = {
#define GRID 10
  { NULL, 0, NULL, 0,
    "General options:", GRID },

  { "ipv4", '4', NULL, 0,
    "use only IPv4", GRID+1 },
  { "ipv6", '6', NULL, 0,
    "use only IPv6", GRID+1 },
  /* FIXME: Called "8bit" in r* utils */
  { "binary", '8', NULL, 0,
    "use an 8-bit data transmission", GRID+1 },
  { "login", 'a', NULL, 0,
    "attempt automatic login", GRID+1 },
  { "no-rc", 'c', NULL, 0,
    "do not read the user's .telnetrc file", GRID+1 },
  { "debug", 'd', NULL, 0,
    "turn on debugging", GRID+1 },
  { "escape", 'e', "CHAR", 0,
    "use CHAR as an escape character", GRID+1 },
  { "no-escape", 'E', NULL, 0,
    "use no escape character", GRID+1 },
  { "no-login", 'K', NULL, 0,
    "do not automatically login to the remote system", GRID+1 },
  { "user", 'l', "USER", 0,
    "attempt automatic login as USER", GRID+1 },
  { "binary-output", 'L', NULL, 0, /* FIXME: Why L?? */
    "use an 8-bit data transmission for output only", GRID+1 },
  { "trace", 'n', "FILE", 0,
    "record trace information into FILE", GRID+1 },
  { "rlogin", 'r', NULL, 0,
    "use a user-interface similar to rlogin", GRID+1 },
#undef GRID

#ifdef ENCRYPTION
# define GRID 20
  { NULL, 0, NULL, 0,
    "Encryption control:", GRID },
  { "encrypt", 'x', NULL, 0,
    "encrypt the data stream, if possible", GRID+1 },
# undef GRID
#endif

#ifdef AUTHENTICATION
# define GRID 30
  { NULL, 0, NULL, 0,
    "Authentication and Kerberos options:", GRID },
  { "disable-auth", 'X', "ATYPE", 0,
    "disable type ATYPE authentication", GRID+1 },
# if defined KERBEROS || defined SHISHI
  { "realm", 'k', "REALM", 0,
    "obtain tickets for the remote host in REALM "
    "instead of the remote host's realm", GRID+1 },
# endif
# if defined KRB5 && defined FORWARD
  { "fwd-credentials", 'f', NULL, 0,
    "allow the local credentials to be forwarded", GRID+1 },
  { NULL, 'F', NULL, 0,
    "forward a forwardable copy of the local credentials "
    "to the remote system", GRID+1 },
# endif
# undef GRID
#endif

#if defined TN3270 && (defined unix || defined __unix || defined __unix__)
# define GRID 40
  { NULL, 0, NULL, 0,
    "TN3270 support:", GRID },
  /* FIXME: Do we need it? */
  { "transcom", 't', "ARG", 0, "", GRID+1 },
  { "noasynch", OPTION_NOASYNCH, NULL, 0, "", GRID+1 },
  { "noasynctty", OPTION_NOASYNCTTY, NULL, 0, "", GRID+1 },
  { "noasyncnet", OPTION_NOASYNCNET, NULL, 0, "", GRID+1 },
# undef GRID
#endif /* TN3270 && (unix || __unix || __unix__) */
  { NULL, 0, NULL, 0, NULL, 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  switch (key)
  {
    case '4':
      family = 4;
      break;

    case '6':
      family = 6;
      break;

    case '8':
      set_eight_wrapper(3);
      break;

    case 'E':
      set_rlogin_wrapper(escape = (cc_t) '\377');
      break;

    case 'K':
      break;

    case 'L':
      set_eight_wrapper(2);
      break;

    case 'a':
      set_autologin_wrapper(1);
      break;

    case 'c':
      set_skiprc_wrapper(1);
      break;

    case 'd':
      debug = 1;
      break;

    case 'e':
      set_escape_char(arg);
      break;

    case 'l':
      set_autologin_wrapper(1);
      set_user_wrapper(arg);
      break;

    case 'n':
      SetNetTrace(arg);
      break;

    case 'r':
      set_rlogin_wrapper('~');
      break;

    default:
      return E2BIG;

  }

  return 0;
}




const char args_doc[] = "[HOST [PORT]]";
const char doc[] = "Login to remote system HOST "
                   "(optionally, on service port PORT)";
static struct argp argp =
  { argp_options, parse_opt, args_doc, doc, NULL, NULL, NULL };



/*
 * main.  Parse arguments, invoke the protocol or command parser.
 */
int main(int argc, char *argv[])
{
  int index;
  set_program_name(argv[0]);
  setlocale(LC_ALL, "");
  tninit();
  TerminalSaveState();
  if (set_prompt_wrapper(strrchr(argv[0], '/')))
    gsetter_prompt_prefix_wrapper();
  else
    set_prompt_wrapper(argv[0]);
  set_user_wrapper(0);
  set_rlogin_wrapper((strncmp(get_prompt_wrapper(), "rlog", 4) == 0) ? ('~') : ((cc_t) '\377'));
  set_autologin_wrapper(-1);
  argp_program_bug_address = "<bug-inetutils@gnu.org>";
  argp_version_setup("telnet", default_program_authors);
  ;
  argp_parse(&argp, argc, argv, 0, &index, 0);
  if (get_autologin_wrapper() == (-1))
    set_autologin_wrapper((get_rlogin_wrapper() == ((cc_t) '\377')) ? (0) : (1));
  argc -= index;
  argv += index;
  if (argc)
  {
    char *args[8];
    char **argp = args;
    if (argc > 2)
      error(1, 0, "too many arguments");
    *(argp++) = get_prompt_wrapper();
    if (get_user_wrapper())
    {
      *(argp++) = "-l";
      *(argp++) = get_user_wrapper();
    }
    if (family == 4)
      *(argp++) = "-4";
    else
      if (family == 6)
      *(argp++) = "-6";
    *(argp++) = argv[0];
    if (argc > 1)
      *(argp++) = argv[1];
    *argp = 0;
    if (setjmp(toplevel) != 0)
      Exit(0);
    if (tn(argp - args, args) == 1)
      return 0;
    else
      return 1;
  }
  setjmp(toplevel);
  for (;;)
  {
    command(1, 0, 0);
  }

}


