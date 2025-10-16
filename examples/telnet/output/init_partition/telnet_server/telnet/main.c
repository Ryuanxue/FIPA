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
};
}

  { argp_options, parse_opt, args_doc, doc, NULL, NULL, NULL };



/*
  /* NOT REACHED */
}
