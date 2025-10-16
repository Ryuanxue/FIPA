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

#include "telnet_rpc_wrapper.h"

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
#include "telnet_rpc_wrapper.h"

#include <stdlib.h>

#include <arpa/telnet.h>
#include <sys/types.h>

#include "ring.h"

#include "externs.h"
#include "types.h"

#ifdef  ENCRYPTION
# include <libtelnet/encrypt.h>
#endif


#ifdef	USE_TERMIO
# ifndef VDISCARD
cc_t termFlushChar;
# endif
# ifndef VLNEXT
cc_t termLiteralNextChar;
# endif
# ifndef VSUSP
cc_t termSuspChar;
# endif
# ifndef VWERASE
cc_t termWerasChar;
# endif
# ifndef VREPRINT
cc_t termRprntChar;
# endif
# ifndef VSTART
cc_t termStartChar;
# endif
# ifndef VSTOP
cc_t termStopChar;
# endif
# ifndef VEOL
cc_t termForw1Char;
# endif
# ifndef VEOL2
cc_t termForw2Char;
# endif
# ifndef VSTATUS
# endif
#else
cc_t termForw2Char;
cc_t termAytChar;
#endif

/*
 * initialize the terminal data structures.
 */

void init_terminal(void)
{
  {
    Ring temp_ttyoring = get_ttyoring_wrapper();
    int temp_result_0 = ring_init(&temp_ttyoring, ttyobuf, sizeof(ttyobuf));
    set_ttyoring_wrapper(temp_ttyoring);
    if (temp_result_0 != 1)
    {
      exit(1);
    }
  }
  {
    Ring temp_ttyiring = get_ttyiring_wrapper();
    int temp_result_1 = ring_init(&temp_ttyiring, ttyibuf, sizeof(ttyibuf));
    set_ttyiring_wrapper(temp_ttyiring);
    if (temp_result_1 != 1)
    {
      exit(1);
    }
  }
  set_autoflush_wrapper(TerminalAutoFlush());
}



/*
 *		Send as much data as possible to the terminal.
 *
 *		Return value:
 *			-1: No useful work done, data waiting to go out.
 *			 0: No data was waiting, so nothing was done.
 *			 1: All waiting data was written out.
 *			 n: All data - n was written out.
 */


int ttyflush(int drop)
{
  register int n;
  register int n0;
  register int n1;
  {
    Ring temp_ttyoring = get_ttyoring_wrapper();
    int temp_result_2 = ring_full_count(&temp_ttyoring);
    set_ttyoring_wrapper(temp_ttyoring);
    n0 = temp_result_2;
  }
  if ((n1 =   {
    Ring temp_ttyoring = get_ttyoring_wrapper();
    int temp_result_2 = ring_full_consecutive(&temp_ttyoring);
    set_ttyoring_wrapper(temp_ttyoring);
    n = temp_result_2;
  }
) > 0)
  {
    if (drop)
    {
      TerminalFlushOutput();
    }
    else
    {
      n = TerminalWrite((char *) get_ttyoring_consume_wrapper(), n);
    }
  }
  if (n > 0)
  {
    if (termdata && n)
    {
      Dump('>', get_ttyoring_consume_wrapper(), n);
    }
    if ((n1 == n) && (n0 > n))
    {
      n1 = n0 - n;
      if (!drop)
        n1 = TerminalWrite((char *) get_ttyoring_bottom_wrapper(), n1);
      if (n1 > 0)
        n += n1;
    }
    {
      Ring temp_ttyoring = get_ttyoring_wrapper();
      ring_consumed(&temp_ttyoring, n);
      set_ttyoring_wrapper(temp_ttyoring);
    }
  }
  if (n < 0)
    return -1;
  if (n == n0)
  {
    if (n0)
      return -1;
    return 0;
  }
  return (n0 - n) + 1;
}



/*
 * These routines decides on what the mode should be (based on the values
 * of various global variables).
 */

int getconnmode(void)
{
  extern int linemode;
  int mode = 0;
  extern int kludgelinemode;
  if (get_In3270_wrapper())
    return 0x0100;
  if (!(get_options_wrapper()[1] & 0x08))
    mode |= 0x0200;
  if (get_localflow_wrapper())
    mode |= 0x0100;
  if (get_options_wrapper()[0] & 0x02)
    mode |= 0x0400;
  if (get_options_wrapper()[0] & 0x08)
    mode |= 0x0800;
  if (get_kludgelinemode_wrapper())
  {
    if (!(get_options_wrapper()[3] & 0x08))
    {
      mode |= 0x02 | 0x01;
      if (get_dontlecho_wrapper() && (get_clocks_echotoggle_wrapper() > get_clocks_modenegotiated_wrapper()))
      {
        mode &= ~0x0200;
      }
    }
    return mode;
  }
  if (get_options_wrapper()[34] & 0x02)
    mode |= get_linemode_wrapper();
  return mode;
}

}

void
setconnmode (int force)
{
#ifdef	ENCRYPTION
  static int enc_passwd = 0;
#endif /* ENCRYPTION */
  register int newmode;

  newmode = getconnmode () | (force ? MODE_FORCE : 0);

  TerminalNewMode (newmode);

#ifdef  ENCRYPTION
  if ((newmode & (MODE_ECHO | MODE_EDIT)) == MODE_EDIT)
    {
      if (my_want_state_is_will (TELOPT_ENCRYPT)
	  && (enc_passwd == 0) && !encrypt_output)
	{
	  encrypt_request_start (0, 0);
	  enc_passwd = 1;
	}
    }
  else
    {
      if (enc_passwd)
	{
	  encrypt_request_end ();
	  enc_passwd = 0;
	}
    }
#endif /* ENCRYPTION */

}


void
setcommandmode (void)
{
  TerminalNewMode (-1);
}
