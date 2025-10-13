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

#if defined unix || defined __unix || defined __unix__
# include <signal.h>
/* By the way, we need to include curses.h before telnet.h since,
 * among other things, telnet.h #defines 'DO', which is a variable
 * declared in curses.h.
 */
#endif /* unix || __unix || __unix__ */

#include <arpa/telnet.h>

#include <ctype.h>
#include <stdlib.h>

#include <libinetutils.h>

#include "ring.h"

#include "defines.h"
#include "externs.h"
#include "types.h"
#include "general.h"

#include "telnet_rpc_wrapper.h"

#ifdef HAVE_TERMCAP_TGETENT
# include <termcap.h>
#elif defined HAVE_CURSES_TGETENT
# include <curses.h>
# include <term.h>
#endif

#ifdef AUTHENTICATION
# include <libtelnet/auth.h>
#endif
#ifdef ENCRYPTION
# include <libtelnet/encrypt.h>
#endif
#if defined AUTHENTICATION || defined ENCRYPTION
# include <libtelnet/misc.h>
#endif


#define strip(x) ((my_want_state_is_wont(TELOPT_BINARY)) ? ((x)&0x7f) : (x))

static unsigned char subbuffer[SUBBUFSIZE], *subpointer, *subend;	/* buffer for sub-options */
#define SB_CLEAR()	subpointer = subbuffer;
#define SB_TERM()	{ subend = subpointer; SB_CLEAR(); }
#define SB_ACCUM(c)	if (subpointer < (subbuffer+sizeof subbuffer)) { \
				*subpointer++ = (c); \
			}

#define SB_GET()	((*subpointer++)&0xff)
#define SB_PEEK()	((*subpointer)&0xff)
#define SB_EOF()	(subpointer >= subend)
#define SB_LEN()	(subend - subpointer)

char options[256] = { 0 };		/* The combined options */
char do_dont_resp[256] = { 0 };
char will_wont_resp[256] = { 0 };

int eight = 0, autologin = 0,	/* Autologin anyone? */
  skiprc = 0, connected, showoptions, In3270,	/* Are we in 3270 mode? */
  ISend,			/* trying to send network data in */
  debug = 0, crmod, netdata,	/* Print out network data flow */
  crlf,				/* Should '\r' be mapped to <CR><LF> (or <CR><NUL>)? */
#if defined TN3270
  noasynchtty = 0,		/* User specified "-noasynch" on command line */
  noasynchnet = 0,		/* User specified "-noasynch" on command line */
  askedSGA = 0,			/* We have talked about suppress go ahead */
#endif
  /* defined(TN3270) */
  telnetport, SYNCHing,		/* we are in TELNET SYNCH mode */
  flushout,			/* flush output */
  autoflush = 0,		/* flush output when interrupting? */
  autosynch,			/* send interrupt characters with SYNCH? */
  localflow,			/* we handle flow control locally */
  restartany,			/* if flow control enabled, restart on any character */
  localchars,			/* we recognize interrupt/quit */
  donelclchars,			/* the user has set "localchars" */
  donebinarytoggle,		/* the user has put us in binary */
  dontlecho,			/* do we suppress local echoing right now? */
  globalmode;

char *prompt = 0;

cc_t escape;
cc_t rlogin;
#ifdef	KLUDGELINEMODE
cc_t echoc;
#endif

/*
 * Telnet receiver states for fsm
 */
#define TS_DATA		0
#define TS_IAC		1
#define TS_WILL		2
#define TS_WONT		3
#define TS_DO		4
#define TS_DONT		5
#define TS_CR		6
#define TS_SB		7	/* sub-option collection */
#define TS_SE		8	/* looking for sub-option end */

static int telrcv_state;
#ifdef	OLD_ENVIRON
unsigned char telopt_environ = TELOPT_NEW_ENVIRON;
#else
# define telopt_environ TELOPT_NEW_ENVIRON
#endif

jmp_buf toplevel;
jmp_buf peerdied;

int flushline;
int linemode;

#ifdef	KLUDGELINEMODE
int kludgelinemode = 1;
#endif

/*
 * The following are some clocks used to decide how to interpret
 * the relationship between various variables.
 */

Clocks clocks;



/*
 * Initialize telnet environment.
 */

void init_telnet(void)
{
  env_init();
  set_subpointer_wrapper(subbuffer);
  ;
  set_connected_wrapper(In3270 = set_ISend_wrapper(localflow = set_donebinarytoggle_wrapper(0)));
  set_restartany_wrapper(-1);
  set_SYNCHing_wrapper(0);
  set_escape_wrapper(']' & 0x1f);
  set_rlogin_wrapper((cc_t) '\377');
  set_echoc_wrapper('E' & 0x1f);
  set_flushline_wrapper(1);
  set_telrcv_state_wrapper(0);
}



/*
 * These routines are in charge of sending option negotiations
 * to the other side.
 *
 * The basic idea is that we send the negotiation if either side
 * is in disagreement as to what the current state should be.
 */

void send_do(register int c, register int init)
{
  if (init)
  {
    if (((get_do_dont_resp_wrapper()[c] == 0) && (get_options_wrapper()[c] & 0x04)) || (get_options_wrapper()[c] & 0x08))
      return;
    {
      get_options_wrapper()[c] |= 0x08;
    }
    ;
    do_dont_resp[c]++;
  }
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 253;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  {
    *get_netoring_supply_wrapper() = c;
    {
      Ring temp_netoring = get_netoring_wrapper();
      ring_supplied(&temp_netoring, 1);
      set_netoring_wrapper(temp_netoring);
    }
  }
  ;
  printoption("SENT", 253, c);
}



void send_dont(register int c, register int init)
{
  if (init)
  {
    if (((get_do_dont_resp_wrapper()[c] == 0) && (!(get_options_wrapper()[c] & 0x04))) || (!(get_options_wrapper()[c] & 0x08)))
      return;
    {
      get_options_wrapper()[c] &= ~0x08;
    }
    ;
    do_dont_resp[c]++;
  }
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 254;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  {
    *get_netoring_supply_wrapper() = c;
    {
      Ring temp_netoring = get_netoring_wrapper();
      ring_supplied(&temp_netoring, 1);
      set_netoring_wrapper(temp_netoring);
    }
  }
  ;
  printoption("SENT", 254, c);
}



void send_will(register int c, register int init)
{
  if (init)
  {
    if (((get_will_wont_resp_wrapper()[c] == 0) && (get_options_wrapper()[c] & 0x01)) || (get_options_wrapper()[c] & 0x02))
      return;
    {
      get_options_wrapper()[c] |= 0x02;
    }
    ;
    will_wont_resp[c]++;
  }
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 251;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  {
    *get_netoring_supply_wrapper() = c;
    {
      Ring temp_netoring = get_netoring_wrapper();
      ring_supplied(&temp_netoring, 1);
      set_netoring_wrapper(temp_netoring);
    }
  }
  ;
  printoption("SENT", 251, c);
}



void send_wont(register int c, register int init)
{
  if (init)
  {
    if (((get_will_wont_resp_wrapper()[c] == 0) && (!(get_options_wrapper()[c] & 0x01))) || (!(get_options_wrapper()[c] & 0x02)))
      return;
    {
      get_options_wrapper()[c] &= ~0x02;
    }
    ;
    will_wont_resp[c]++;
  }
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 252;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  {
    *get_netoring_supply_wrapper() = c;
    {
      Ring temp_netoring = get_netoring_wrapper();
      ring_supplied(&temp_netoring, 1);
      set_netoring_wrapper(temp_netoring);
    }
  }
  ;
  printoption("SENT", 252, c);
}




void willoption(int option)
{
  int new_state_ok = 0;
  if (get_do_dont_resp_wrapper()[option])
  {
    --do_dont_resp[option];
    if (get_do_dont_resp_wrapper()[option] && (get_options_wrapper()[option] & 0x04))
      --do_dont_resp[option];
  }
  if ((get_do_dont_resp_wrapper()[option] == 0) && (!(get_options_wrapper()[option] & 0x08)))
  {
    switch (option)
    {
      case 1:

      case 0:

      case 3:
        set_clocks_modenegotiated_wrapper(clocks.system++);

      case 5:
        new_state_ok = 1;
        break;

      case 6:
        if (get_flushout_wrapper())
        set_flushout_wrapper(0);
      {
        get_options_wrapper()[option] &= ~0x08;
      }
        ;
      {
        get_options_wrapper()[option] &= ~0x04;
      }
        ;
        return;

      case 34:

      default:
        break;

    }

    if (new_state_ok)
    {
      {
        get_options_wrapper()[option] |= 0x08;
      }
      ;
      send_do(option, 0);
      setconnmode(0);
    }
    else
    {
      do_dont_resp[option]++;
      send_dont(option, 0);
    }
  }
  {
    get_options_wrapper()[option] |= 0x04;
  }
  ;
}



void wontoption(int option)
{
  if (get_do_dont_resp_wrapper()[option])
  {
    --do_dont_resp[option];
    if (get_do_dont_resp_wrapper()[option] && (!(get_options_wrapper()[option] & 0x04)))
      --do_dont_resp[option];
  }
  if ((get_do_dont_resp_wrapper()[option] == 0) && (get_options_wrapper()[option] & 0x08))
  {
    switch (option)
    {
      case 3:
        if (!get_kludgelinemode_wrapper())
        break;

      case 1:
        set_clocks_modenegotiated_wrapper(clocks.system++);
        break;

      case 6:
        if (get_flushout_wrapper())
        set_flushout_wrapper(0);
      {
        get_options_wrapper()[option] &= ~0x08;
      }
        ;
      {
        get_options_wrapper()[option] &= ~0x04;
      }
        ;
        return;

      default:
        break;

    }

    {
      get_options_wrapper()[option] &= ~0x08;
    }
    ;
    if (get_options_wrapper()[option] & 0x04)
      send_dont(option, 0);
    setconnmode(0);
  }
  else
    if (option == 6)
  {
    if (get_flushout_wrapper())
      set_flushout_wrapper(0);
    {
      get_options_wrapper()[option] &= ~0x08;
    }
    ;
  }
  {
    get_options_wrapper()[option] &= ~0x04;
  }
  ;
}



static void dooption(int option)
{
  int new_state_ok = 0;
  if (get_will_wont_resp_wrapper()[option])
  {
    --will_wont_resp[option];
    if (get_will_wont_resp_wrapper()[option] && (get_options_wrapper()[option] & 0x01))
      --will_wont_resp[option];
  }
  if (get_will_wont_resp_wrapper()[option] == 0)
  {
    if (!(get_options_wrapper()[option] & 0x02))
    {
      switch (option)
      {
        case 6:
          send_will(option, 0);
        {
          get_options_wrapper()[6] &= ~0x02;
        }
          ;
        {
          get_options_wrapper()[6] &= ~0x01;
        }
          ;
          return;

        case 0:

        case 31:

        case 32:

        case 33:

        case 24:

        case 3:
          new_state_ok = 1;
          break;

        case 39:
          new_state_ok = 1;
          break;

        case 35:
          if (env_getvalue("DISPLAY"))
          new_state_ok = 1;
          break;

        case 34:
          set_kludgelinemode_wrapper(0);
          send_do(3, 1);
        {
          get_options_wrapper()[34] |= 0x02;
        }
          ;
          send_will(option, 0);
        {
          get_options_wrapper()[34] |= 0x01;
        }
          ;
          slc_init();
          return;

        case 1:

        default:
          break;

      }

      if (new_state_ok)
      {
        {
          get_options_wrapper()[option] |= 0x02;
        }
        ;
        send_will(option, 0);
        setconnmode(0);
      }
      else
      {
        will_wont_resp[option]++;
        send_wont(option, 0);
      }
    }
    else
    {
      switch (option)
      {
        case 34:
          set_kludgelinemode_wrapper(0);
          send_do(3, 1);
        {
          get_options_wrapper()[option] |= 0x01;
        }
          ;
          slc_init();
          send_do(3, 0);
          return;

      }

    }
  }
  {
    get_options_wrapper()[option] |= 0x01;
  }
  ;
}



static void dontoption(int option)
{
  if (get_will_wont_resp_wrapper()[option])
  {
    --will_wont_resp[option];
    if (get_will_wont_resp_wrapper()[option] && (!(get_options_wrapper()[option] & 0x01)))
      --will_wont_resp[option];
  }
  if ((get_will_wont_resp_wrapper()[option] == 0) && (get_options_wrapper()[option] & 0x02))
  {
    switch (option)
    {
      case 34:
        set_linemode_wrapper(0);
        break;

    }

    {
      get_options_wrapper()[option] &= ~0x02;
    }
    ;
    if (get_options_wrapper()[option] & 0x01)
      send_wont(option, 0);
    setconnmode(0);
  }
  {
    get_options_wrapper()[option] &= ~0x01;
  }
  ;
}



int
is_unique (register char *name, register char **as, register char **ae)
{
  register char **ap;
  register int n;

  n = strlen (name) + 1;
  for (ap = as; ap < ae; ap++)
    if (strncasecmp (*ap, name, n) == 0)
      return (0);
  return (1);
}

/*
 * Given a buffer returned by tgetent(), this routine will turn
 * the pipe separated list of names in the buffer into an array
 * of pointers to null terminated names.  We toss out any bad,
 * duplicate, or verbose names (names with spaces).
 */

static char *name_unknown = "UNKNOWN";
static char *unknown[] = { 0, 0 };

char **
mklist (char *buf, char *name)
{
  register int n;
  register char c, *cp, **argvp, *cp2, **argv, **avt;

  if (name)
    {
      if ((int) strlen (name) > 40)
	{
	  name = 0;
	  unknown[0] = name_unknown;
	}
      else
	{
	  unknown[0] = name;
	  upcase (name);
	}
    }
  else
    unknown[0] = name_unknown;

  /*
   * An empty capability buffer finishes the analysis.
   */
  if (!buf || !*buf)
    return (unknown);

  /*
   * Count the number of alias names.
   * Stop at the first field separator, a colon.
   */
  for (n = 1, cp = buf; *cp && *cp != ':'; cp++)
    {
      if (*cp == '|')
	n++;
    }

  /*
   * Allocate an array to hold the name pointers.
   */
  argv = (char **) malloc ((n + 3) * sizeof (char *));
  if (argv == 0)
    return (unknown);

  /*
   * Fill the array of pointers with the established aliases.
   * Reserve the first slot for the preferred name.
   */
  argvp = argv + 1;
  *argv = *argvp = 0;
  n = 0;		/* Positive: name uses white space.  */

  for (cp = cp2 = buf; (c = *cp); cp++)
    {
      if (c == '|' || c == ':')		/* Delimiters */
	{
	  *cp++ = '\0';
	  /*
	   * Skip entries that have spaces or are over 40
	   * characters long.  If this is our environment
	   * name, then put it up front.  Otherwise, as
	   * long as this is not a duplicate name (case
	   * insensitive) add it to the list.
	   */
	  if (n || (cp - cp2 > 41))
	    ;			/* Ignore the just scanned name.  */
	  else if (name && (strncasecmp (name, cp2, cp - cp2) == 0))
	    *argv = cp2;	/* Preferred name, exact match.  */
	  else if (is_unique (cp2, argv + 1, argvp))
	    {
	      *argvp++ = cp2;
	      *argvp = 0;	/* Prevent looking forward.  */
	    }

	  /* Abort parsing at first field delimiter.  */
	  if (c == ':')
	    break;

	  /*
	   * Skip multiple delimiters. Reset CP2 to
	   * the beginning of the next name. Reset N,
	   * the flag for names with spaces.
	   */
	  while ((c = *cp) == '|')
	    cp++;

	  cp2 = cp;		/* Proceed to next alias name.  */
	  n = 0;
	}
      /*
       * Skip entries with spaces or non-ascii values.
       * Convert lower case letters to upper case.
       */
      if ((c == ' ') || !isascii (c))
	n = 1;
      else if (islower (c))
	*cp = toupper (c);
    }

  /*
   * Check for an old V6 2 character name.  If the second
   * name points to the beginning of the buffer, and is
   * only 2 characters long, move it to the end of the array.
   */
  if ((argv[1] == buf) && (strlen (argv[1]) == 2))
    {
      --argvp;
      for (avt = &argv[1]; avt < argvp; avt++)
	*avt = *(avt + 1);
      *argvp++ = buf;
    }

  /*
   * Duplicate last name, for TTYPE option, and null
   * terminate the array.  If we didn't find a match on
   * our terminal name, put that name at the beginning.
   */
  cp = *(argvp - 1);
  *argvp++ = cp;
  *argvp = 0;

  if (*argv == 0)
    {
      if (name)
	*argv = name;
      else
	{
	  --argvp;
	  for (avt = argv; avt < argvp; avt++)
	    *avt = *(avt + 1);
	}
    }
  if (*argv)
    return (argv);
  else
    return (unknown);
}

/* Claimed to be ignored by contemporary implementations,
 * but still modified by FreeBSD and NetBSD.
 * mklist will examine this buffer, so erase it
 * to cover corner cases.
 */
char termbuf[2048] = { 0 };

static int init_term(char *tname, int *errp)
{
  int err = -1;
  err = tgetent(get_termbuf_wrapper(), tname);
  if (err == 1)
  {
    get_termbuf_wrapper()[(sizeof(get_termbuf_wrapper())) - 1] = '\0';
    if (errp)
      *errp = 1;
    return 0;
  }
  if (errp)
    *errp = 0;
  return -1;
}



int resettermname = 1;

char *gettermname(void)
{
  char *tname;
  static char **tnamep = 0;
  static char **next;
  int err;
  if (get_resettermname_wrapper())
  {
    set_resettermname_wrapper(0);
    if (tnamep && (tnamep != unknown))
      free(tnamep);
    if ((tname = (char *) env_getvalue("TERM")) && (init_term(tname, &err) == 0))
    {
      tnamep = mklist(get_termbuf_wrapper(), tname);
    }
    else
    {
      if (tname && (((int) strlen(tname)) <= 40))
      {
        unknown[0] = tname;
        upcase(tname);
      }
      else
        unknown[0] = name_unknown;
      tnamep = unknown;
    }
    next = tnamep;
  }
  if ((*next) == 0)
    next = tnamep;
  return *(next++);
}



/*
 * suboption()
 *
 *	Look at the sub-option buffer, and try to be helpful to the other
 * side.
 *
 *	Currently we recognize:
 *
 *		Terminal type, send request.
 *		Terminal speed (send request).
 *		Local flow control (is request).
 *		Linemode
 */

static void suboption(void)
{
  unsigned char subchar;
  printsub('<', get_subbuffer_wrapper(), (get_subend_wrapper() - get_subpointer_wrapper()) + 2);
  switch (subchar = (*gsetter_subpointer_postfix_wrapper()) & 0xff)
  {
    case 24:
      if (!(get_options_wrapper()[24] & 0x02))
      return;
      if ((get_subpointer_wrapper() >= get_subend_wrapper()) || (((*gsetter_subpointer_postfix_wrapper()) & 0xff) != 1))
    {
      return;
    }
    else
    {
      char *name;
      unsigned char temp[50];
      int len;
      name = gettermname();
      len = (strlen(name) + 4) + 2;
      {
        Ring temp_netoring = get_netoring_wrapper();
        int temp_result_3 = ring_empty_count(&temp_netoring);
        set_netoring_wrapper(temp_netoring);
        if (len < temp_result_3)
        {
          sprintf((char *) temp, "%c%c%c%c%s%c%c", 255, 250, 24, 0, name, 255, 240);
          {
            Ring temp_netoring = get_netoring_wrapper();
            ring_supply_data(&temp_netoring, temp, len);
            set_netoring_wrapper(temp_netoring);
          }
          printsub('>', &temp[2], len - 2);
        }
        else
        {
          ExitString("No room in buffer for terminal type.\n", 1);
        }
      }
    }
      break;

    case 32:
      if (!(get_options_wrapper()[32] & 0x02))
      return;
      if (get_subpointer_wrapper() >= get_subend_wrapper())
      return;
      if (((*gsetter_subpointer_postfix_wrapper()) & 0xff) == 1)
    {
      long ospeed;
      long ispeed;
      unsigned char temp[50];
      int len;
      TerminalSpeeds(&ispeed, &ospeed);
      sprintf((char *) temp, "%c%c%c%c%d,%d%c%c", 255, 250, 32, 0, (int) ospeed, (int) ispeed, 255, 240);
      len = strlen(((char *) temp) + 4) + 4;
      {
        Ring temp_netoring = get_netoring_wrapper();
        int temp_result_4 = ring_empty_count(&temp_netoring);
        set_netoring_wrapper(temp_netoring);
        if (len < temp_result_4)
        {
          {
            Ring temp_netoring = get_netoring_wrapper();
            ring_supply_data(&temp_netoring, temp, len);
            set_netoring_wrapper(temp_netoring);
          }
          printsub('>', temp + 2, len - 2);
        }
        else
          printf("lm_will: not enough room in buffer\n");
      }
    }
      break;

    case 33:
      if (!(get_options_wrapper()[33] & 0x02))
      return;
      if (get_subpointer_wrapper() >= get_subend_wrapper())
      return;
      switch ((*gsetter_subpointer_postfix_wrapper()) & 0xff)
    {
      case 2:
        set_restartany_wrapper(1);
        break;

      case 3:
        set_restartany_wrapper(0);
        break;

      case 1:
        set_localflow_wrapper(1);
        break;

      case 0:
        set_localflow_wrapper(0);
        break;

      default:
        return;

    }

      setcommandmode();
      setconnmode(0);
      break;

    case 34:
      if (!(get_options_wrapper()[34] & 0x02))
      return;
      if (get_subpointer_wrapper() >= get_subend_wrapper())
      return;
      switch ((*gsetter_subpointer_postfix_wrapper()) & 0xff)
    {
      case 251:
        lm_will(get_subpointer_wrapper(), get_subend_wrapper() - get_subpointer_wrapper());
        break;

      case 252:
        lm_wont(get_subpointer_wrapper(), get_subend_wrapper() - get_subpointer_wrapper());
        break;

      case 253:
        lm_do(get_subpointer_wrapper(), get_subend_wrapper() - get_subpointer_wrapper());
        break;

      case 254:
        lm_dont(get_subpointer_wrapper(), get_subend_wrapper() - get_subpointer_wrapper());
        break;

      case 3:
        slc(get_subpointer_wrapper(), get_subend_wrapper() - get_subpointer_wrapper());
        break;

      case 1:
        lm_mode(get_subpointer_wrapper(), get_subend_wrapper() - get_subpointer_wrapper(), 0);
        break;

      default:
        break;

    }

      break;

    case 39:
      if (get_subpointer_wrapper() >= get_subend_wrapper())
      return;
      switch ((*get_subpointer_wrapper()) & 0xff)
    {
      case 0:

      case 2:
        if (!(get_options_wrapper()[subchar] & 0x08))
        return;
        break;

      case 1:
        if (!(get_options_wrapper()[subchar] & 0x02))
      {
        return;
      }
        break;

      default:
        return;

    }

      env_opt(get_subpointer_wrapper(), get_subend_wrapper() - get_subpointer_wrapper());
      break;

    case 35:
      if (!(get_options_wrapper()[35] & 0x02))
      return;
      if (get_subpointer_wrapper() >= get_subend_wrapper())
      return;
      if (((*gsetter_subpointer_postfix_wrapper()) & 0xff) == 1)
    {
      unsigned char temp[50];
      unsigned char *dp;
      int len;
      if ((dp = env_getvalue("DISPLAY")) == 0)
      {
        send_wont(35, 1);
        break;
      }
      sprintf((char *) temp, "%c%c%c%c%s%c%c", 255, 250, 35, 0, dp, 255, 240);
      len = strlen(((char *) temp) + 4) + 4;
      {
        Ring temp_netoring = get_netoring_wrapper();
        int temp_result_5 = ring_empty_count(&temp_netoring);
        set_netoring_wrapper(temp_netoring);
        if (len < temp_result_5)
        {
          {
            Ring temp_netoring = get_netoring_wrapper();
            ring_supply_data(&temp_netoring, temp, len);
            set_netoring_wrapper(temp_netoring);
          }
          printsub('>', temp + 2, len - 2);
        }
        else
          printf("lm_will: not enough room in buffer\n");
      }
    }
      break;

    default:
      break;

  }

}



static unsigned char str_lm[] = { IAC, SB, TELOPT_LINEMODE, 0, 0, IAC, SE };

void lm_will(unsigned char *cmd, int len)
{
  if (len < 1)
  {
    printf("lm_will: no command!!!\n");
    return;
  }
  switch (cmd[0])
  {
    case 2:

    default:
      get_str_lm_wrapper()[3] = 254;
      get_str_lm_wrapper()[4] = cmd[0];
    {
      Ring temp_netoring = get_netoring_wrapper();
      int temp_result_6 = ring_empty_count(&temp_netoring);
      set_netoring_wrapper(temp_netoring);
      if (temp_result_6 > ((int) (sizeof(str_lm))))
      {
        {
          Ring temp_netoring = get_netoring_wrapper();
          ring_supply_data(&temp_netoring, str_lm, sizeof(str_lm));
          set_netoring_wrapper(temp_netoring);
        }
        printsub('>', &get_str_lm_wrapper()[2], (sizeof(get_str_lm_wrapper())) - 2);
      }
      else
        printf("lm_will: not enough room in buffer\n");
    }
      break;

  }

}



void
lm_wont (unsigned char *cmd, int len)
{
  if (len < 1)
    {
      /*@ */ printf ("lm_wont: no command!!!\n");
      /* Should not happen... */
      return;
    }
  switch (cmd[0])
    {
    case LM_FORWARDMASK:	/* We shouldn't ever get this... */
    default:
      /* We are always DONT, so don't respond */
      return;
    }
}

void lm_do(unsigned char *cmd, int len)
{
  if (len < 1)
  {
    printf("lm_do: no command!!!\n");
    return;
  }
  switch (cmd[0])
  {
    case 2:

    default:
      get_str_lm_wrapper()[3] = 252;
      get_str_lm_wrapper()[4] = cmd[0];
    {
      Ring temp_netoring = get_netoring_wrapper();
      int temp_result_7 = ring_empty_count(&temp_netoring);
      set_netoring_wrapper(temp_netoring);
      if (temp_result_7 > ((int) (sizeof(str_lm))))
      {
        {
          Ring temp_netoring = get_netoring_wrapper();
          ring_supply_data(&temp_netoring, str_lm, sizeof(str_lm));
          set_netoring_wrapper(temp_netoring);
        }
        printsub('>', &get_str_lm_wrapper()[2], (sizeof(get_str_lm_wrapper())) - 2);
      }
      else
        printf("lm_do: not enough room in buffer\n");
    }
      break;

  }

}



void
lm_dont (unsigned char *cmd, int len)
{
  if (len < 1)
    {
      /*@ */ printf ("lm_dont: no command!!!\n");
      /* Should not happen... */
      return;
    }
  switch (cmd[0])
    {
    case LM_FORWARDMASK:
    default:
      /* we are always WONT, so don't respond */
      break;
    }
}

static unsigned char str_lm_mode[] = {
  IAC, SB, TELOPT_LINEMODE, LM_MODE, 0, IAC, SE
};

void lm_mode(unsigned char *cmd, int len, int init)
{
  if (len != 1)
    return;
  if (((get_linemode_wrapper() & 0x1f) & (~0x04)) == (*cmd))
    return;
  if ((*cmd) & 0x04)
    return;
  set_linemode_wrapper((*cmd) & (0x1f & (~0x04)));
  get_str_lm_mode_wrapper()[4] = get_linemode_wrapper();
  if (!init)
    get_str_lm_mode_wrapper()[4] |= 0x04;
  {
    Ring temp_netoring = get_netoring_wrapper();
    int temp_result_8 = ring_empty_count(&temp_netoring);
    set_netoring_wrapper(temp_netoring);
    if (temp_result_8 > ((int) (sizeof(str_lm_mode))))
    {
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supply_data(&temp_netoring, str_lm_mode, sizeof(str_lm_mode));
        set_netoring_wrapper(temp_netoring);
      }
      printsub('>', &get_str_lm_mode_wrapper()[2], (sizeof(get_str_lm_mode_wrapper())) - 2);
    }
    else
      printf("lm_mode: not enough room in buffer\n");
  }
  setconnmode(0);
}





/*
 * slc()
 * Handle special character suboption of LINEMODE.
 */

struct spc
{
  cc_t val;
  cc_t *valp;
  char flags;			/* Current flags & level */
  char mylevel;			/* Maximum level & flags */
} spc_data[NSLC + 1];

#define SLC_IMPORT	0
#define SLC_EXPORT	1
#define SLC_RVALUE	2
static int slc_mode = SLC_EXPORT;

void slc_init(void)
{
  register struct spc *spcp;
  set_localchars_wrapper(1);
  for (spcp = spc_data; spcp < (&spc_data[18 + 1]); spcp++)
  {
    spcp->val = 0;
    spcp->valp = 0;
    spcp->flags = (spcp->mylevel = 0);
  }

  {
    spcp = &spc_data[1];
    if (spcp->valp = tcval(1))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[4];
    if (spcp->valp = tcval(4))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[5];
    if (spcp->valp = tcval(5))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[7];
    if (spcp->valp = tcval(7))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = (2 | 0x40) | 0x20;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[8];
    if (spcp->valp = tcval(8))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[9];
    if (spcp->valp = tcval(9))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0x40;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[10];
    if (spcp->valp = tcval(10))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[11];
    if (spcp->valp = tcval(11))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[12];
    if (spcp->valp = tcval(12))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[13];
    if (spcp->valp = tcval(13))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[14];
    if (spcp->valp = tcval(14))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[15];
    if (spcp->valp = tcval(15))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[16];
    if (spcp->valp = tcval(16))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[17];
    if (spcp->valp = tcval(17))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[18];
    if (spcp->valp = tcval(18))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = 2 | 0;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  {
    spcp = &spc_data[3];
    if (spcp->valp = tcval(3))
    {
      spcp->val = *spcp->valp;
      spcp->mylevel = (2 | 0x40) | 0x20;
    }
    else
    {
      spcp->val = 0;
      spcp->mylevel = 3;
    }
  }
  ;
  if (get_slc_mode_wrapper() == 1)
    slc_export();
  else
    slc_import(1);
}



void slcstate(void)
{
  printf("Special characters are %s values\n", (get_slc_mode_wrapper() == 0) ? ("remote default") : ((get_slc_mode_wrapper() == 1) ? ("local") : ("remote")));
}



void slc_mode_export(void)
{
  set_slc_mode_wrapper(1);
  if (get_options_wrapper()[34] & 0x01)
    slc_export();
}



void slc_mode_import(int def)
{
  set_slc_mode_wrapper((def) ? (0) : (2));
  if (get_options_wrapper()[34] & 0x01)
    slc_import(def);
}



unsigned char slc_import_val[] = {
  IAC, SB, TELOPT_LINEMODE, LM_SLC, 0, SLC_VARIABLE, 0, IAC, SE
};
unsigned char slc_import_def[] = {
  IAC, SB, TELOPT_LINEMODE, LM_SLC, 0, SLC_DEFAULT, 0, IAC, SE
};

void slc_import(int def)
{
  {
    Ring temp_netoring = get_netoring_wrapper();
    int temp_result_9 = ring_empty_count(&temp_netoring);
    set_netoring_wrapper(temp_netoring);
    if (temp_result_9 > ((int) (sizeof(slc_import_val))))
    {
      if (def)
      {
        {
          Ring temp_netoring = get_netoring_wrapper();
          ring_supply_data(&temp_netoring, slc_import_def, sizeof(slc_import_def));
          set_netoring_wrapper(temp_netoring);
        }
        printsub('>', &get_slc_import_def_wrapper()[2], (sizeof(get_slc_import_def_wrapper())) - 2);
      }
      else
      {
        {
          Ring temp_netoring = get_netoring_wrapper();
          ring_supply_data(&temp_netoring, slc_import_val, sizeof(slc_import_val));
          set_netoring_wrapper(temp_netoring);
        }
        printsub('>', &get_slc_import_val_wrapper()[2], (sizeof(get_slc_import_val_wrapper())) - 2);
      }
    }
    else
      printf("slc_import: not enough room\n");
  }
}



void
slc_export (void)
{
  register struct spc *spcp;

  TerminalDefaultChars ();

  slc_start_reply ();
  for (spcp = &spc_data[1]; spcp < &spc_data[NSLC + 1]; spcp++)
    {
      if (spcp->mylevel != SLC_NOSUPPORT)
	{
	  if (spcp->val == (cc_t) (_POSIX_VDISABLE))
	    spcp->flags = SLC_NOSUPPORT;
	  else
	    spcp->flags = spcp->mylevel;
	  if (spcp->valp)
	    spcp->val = *spcp->valp;
	  slc_add_reply (spcp - spc_data, spcp->flags, spcp->val);
	}
    }
  slc_end_reply ();
  slc_update ();
  setconnmode (1);		/* Make sure the character values are set */
}

void
slc (register unsigned char *cp, int len)
{
  register struct spc *spcp;
  register int func, level;

  slc_start_reply ();

  for (; len >= 3; len -= 3, cp += 3)
    {

      func = cp[SLC_FUNC];

      if (func == 0)
	{
	  /*
	   * Client side: always ignore 0 function.
	   */
	  continue;
	}
      if (func > NSLC)
	{
	  if ((cp[SLC_FLAGS] & SLC_LEVELBITS) != SLC_NOSUPPORT)
	    slc_add_reply (func, SLC_NOSUPPORT, 0);
	  continue;
	}

      spcp = &spc_data[func];

      level = cp[SLC_FLAGS] & (SLC_LEVELBITS | SLC_ACK);

      if ((cp[SLC_VALUE] == (unsigned char) spcp->val) &&
	  ((level & SLC_LEVELBITS) == (spcp->flags & SLC_LEVELBITS)))
	{
	  continue;
	}

      if (level == (SLC_DEFAULT | SLC_ACK))
	{
	  /*
	   * This is an error condition, the SLC_ACK
	   * bit should never be set for the SLC_DEFAULT
	   * level.  Our best guess to recover is to
	   * ignore the SLC_ACK bit.
	   */
	  cp[SLC_FLAGS] &= ~SLC_ACK;
	}

      if (level == ((spcp->flags & SLC_LEVELBITS) | SLC_ACK))
	{
	  spcp->val = (cc_t) cp[SLC_VALUE];
	  spcp->flags = cp[SLC_FLAGS];	/* include SLC_ACK */
	  continue;
	}

      level &= ~SLC_ACK;

      if (level <= (spcp->mylevel & SLC_LEVELBITS))
	{
	  spcp->flags = cp[SLC_FLAGS] | SLC_ACK;
	  spcp->val = (cc_t) cp[SLC_VALUE];
	}
      if (level == SLC_DEFAULT)
	{
	  if ((spcp->mylevel & SLC_LEVELBITS) != SLC_DEFAULT)
	    spcp->flags = spcp->mylevel;
	  else
	    spcp->flags = SLC_NOSUPPORT;
	}
      slc_add_reply (func, spcp->flags, spcp->val);
    }
  slc_end_reply ();
  if (slc_update ())
    setconnmode (1);		/* set the  new character values */
}

void
slc_check (void)
{
  register struct spc *spcp;

  slc_start_reply ();
  for (spcp = &spc_data[1]; spcp < &spc_data[NSLC + 1]; spcp++)
    {
      if (spcp->valp && spcp->val != *spcp->valp)
	{
	  spcp->val = *spcp->valp;
	  if (spcp->val == (cc_t) (_POSIX_VDISABLE))
	    spcp->flags = SLC_NOSUPPORT;
	  else
	    spcp->flags = spcp->mylevel;
	  slc_add_reply (spcp - spc_data, spcp->flags, spcp->val);
	}
    }
  slc_end_reply ();
  setconnmode (1);
}


unsigned char slc_reply[128];
unsigned char *slc_replyp;

void slc_start_reply(void)
{
  set_slc_replyp_wrapper(slc_reply);
  *gsetter_slc_replyp_postfix_wrapper() = 255;
  *gsetter_slc_replyp_postfix_wrapper() = 250;
  *gsetter_slc_replyp_postfix_wrapper() = 34;
  *gsetter_slc_replyp_postfix_wrapper() = 3;
}



void slc_add_reply(unsigned char func, unsigned char flags, cc_t value)
{
  if ((*gsetter_slc_replyp_postfix_wrapper() = func) == 255)
    *gsetter_slc_replyp_postfix_wrapper() = 255;
  if ((*gsetter_slc_replyp_postfix_wrapper() = flags) == 255)
    *gsetter_slc_replyp_postfix_wrapper() = 255;
  if ((*gsetter_slc_replyp_postfix_wrapper() = (unsigned char) value) == 255)
    *gsetter_slc_replyp_postfix_wrapper() = 255;
}



void slc_end_reply(void)
{
  register int len;
  *gsetter_slc_replyp_postfix_wrapper() = 255;
  *gsetter_slc_replyp_postfix_wrapper() = 240;
  len = get_slc_replyp_wrapper() - get_slc_reply_wrapper();
  if (len <= 6)
    return;
  {
    Ring temp_netoring = get_netoring_wrapper();
    int temp_result_10 = ring_empty_count(&temp_netoring);
    set_netoring_wrapper(temp_netoring);
    if (temp_result_10 > len)
    {
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supply_data(&temp_netoring, slc_reply, slc_replyp - slc_reply);
        set_netoring_wrapper(temp_netoring);
      }
      printsub('>', &get_slc_reply_wrapper()[2], (get_slc_replyp_wrapper() - get_slc_reply_wrapper()) - 2);
    }
    else
      printf("slc_end_reply: not enough room\n");
  }
}



int
slc_update (void)
{
  register struct spc *spcp;
  int need_update = 0;

  for (spcp = &spc_data[1]; spcp < &spc_data[NSLC + 1]; spcp++)
    {
      if (!(spcp->flags & SLC_ACK))
	continue;
      spcp->flags &= ~SLC_ACK;
      if (spcp->valp && (*spcp->valp != spcp->val))
	{
	  *spcp->valp = spcp->val;
	  need_update = 1;
	}
    }
  return (need_update);
}

#ifdef	OLD_ENVIRON
# ifdef	ENV_HACK
/*
 * Earlier version of telnet/telnetd from the BSD code had
 * the definitions of VALUE and VAR reversed.  To ensure
 * maximum interoperability, we assume that the server is
 * an older BSD server, until proven otherwise.  The newer
 * BSD servers should be able to handle either definition,
 * so it is better to use the wrong values if we don't
 * know what type of server it is.
 */
int env_auto = 1;
int old_env_var = OLD_ENV_VAR;
int old_env_value = OLD_ENV_VALUE;
# else
#  define old_env_var OLD_ENV_VAR
#  define old_env_value OLD_ENV_VALUE
# endif
#endif

void
env_opt (register unsigned char *buf, register int len)
{
  register unsigned char *ep = 0, *epc = 0;
  register int i;

  switch (buf[0] & 0xff)
    {
    case TELQUAL_SEND:
      env_opt_start ();
      if (len == 1)
	{
	  env_opt_add (NULL);
	}
      else
	for (i = 1; i < len; i++)
	  {
	    switch (buf[i] & 0xff)
	      {
#ifdef	OLD_ENVIRON
	      case OLD_ENV_VAR:
# ifdef	ENV_HACK
		if (telopt_environ == TELOPT_OLD_ENVIRON && env_auto)
		  {
		    /* Server has the same definitions */
		    old_env_var = OLD_ENV_VAR;
		    old_env_value = OLD_ENV_VALUE;
		  }
		/* FALL THROUGH */
# endif
	      case OLD_ENV_VALUE:
		/*
		 * Although OLD_ENV_VALUE is not legal, we will
		 * still recognize it, just in case it is an
		 * old server that has VAR & VALUE mixed up...
		 */
		/* FALL THROUGH */
#else
	      case NEW_ENV_VAR:
#endif
	      case ENV_USERVAR:
		if (ep)
		  {
		    *epc = 0;
		    env_opt_add (ep);
		  }
		ep = epc = &buf[i + 1];
		break;
	      case ENV_ESC:
		i++;
		/*FALL THROUGH */
	      default:
		if (epc)
		  *epc++ = buf[i];
		break;
	      }
	  }
      if (ep)
	{
	  *epc = 0;
	  env_opt_add (ep);
	}
      env_opt_end (1);
      break;

    case TELQUAL_IS:
    case TELQUAL_INFO:
      /* Ignore for now.  We shouldn't get it anyway. */
      break;

    default:
      break;
    }
}

#define OPT_REPLY_SIZE	256
unsigned char *opt_reply;
unsigned char *opt_replyp;
unsigned char *opt_replyend;

void env_opt_start(void)
{
  if (get_opt_reply_wrapper())
    set_opt_reply_wrapper((unsigned char *) realloc(get_opt_reply_wrapper(), 256));
  else
    set_opt_reply_wrapper((unsigned char *) malloc(256));
  if (get_opt_reply_wrapper() == 0)
  {
    printf("env_opt_start: malloc()/realloc() failed!!!\n");
    set_opt_reply_wrapper(opt_replyp = set_opt_replyend_wrapper(0));
    return;
  }
  set_opt_replyp_wrapper(opt_reply);
  set_opt_replyend_wrapper(get_opt_reply_wrapper() + 256);
  *gsetter_opt_replyp_postfix_wrapper() = 255;
  *gsetter_opt_replyp_postfix_wrapper() = 250;
  *gsetter_opt_replyp_postfix_wrapper() = 39;
  *gsetter_opt_replyp_postfix_wrapper() = 0;
}



void env_opt_start_info(void)
{
  env_opt_start();
  if (get_opt_replyp_wrapper())
    get_opt_replyp_wrapper()[-1] = 2;
}



void env_opt_add(register unsigned char *ep)
{
  register unsigned char *vp;
  register unsigned char c;
  if (get_opt_reply_wrapper() == 0)
    return;
  if ((ep == 0) || ((*ep) == '\0'))
  {
    env_default(1, 0);
    while (ep = env_default(0, 0))
      env_opt_add(ep);

    env_default(1, 1);
    while (ep = env_default(0, 1))
      env_opt_add(ep);

    return;
  }
  vp = env_getvalue((char *) ep);
  if ((((get_opt_replyp_wrapper() + ((vp) ? (strlen((char *) vp)) : (0))) + strlen((char *) ep)) + 6) > get_opt_replyend_wrapper())
  {
    register int len;
    set_opt_replyend_wrapper(256);
    len = get_opt_replyend_wrapper() - get_opt_reply_wrapper();
    set_opt_reply_wrapper((unsigned char *) realloc(get_opt_reply_wrapper(), len));
    if (get_opt_reply_wrapper() == 0)
    {
      printf("env_opt_add: realloc() failed!!!\n");
      set_opt_reply_wrapper(opt_replyp = set_opt_replyend_wrapper(0));
      return;
    }
    set_opt_replyp_wrapper((get_opt_reply_wrapper() + len) - (get_opt_replyend_wrapper() - get_opt_replyp_wrapper()));
    set_opt_replyend_wrapper(get_opt_reply_wrapper() + len);
  }
  if (opt_welldefined((char *) ep))
    *gsetter_opt_replyp_postfix_wrapper() = 0;
  else
    *gsetter_opt_replyp_postfix_wrapper() = 3;
  for (;;)
  {
    while (c = *(ep++))
    {
      switch (c & 0xff)
      {
        case 255:
          *gsetter_opt_replyp_postfix_wrapper() = 255;
          break;

        case 0:

        case 1:

        case 2:

        case 3:
          *gsetter_opt_replyp_postfix_wrapper() = 2;
          break;

      }

      *gsetter_opt_replyp_postfix_wrapper() = c;
    }

    if (ep = vp)
    {
      *gsetter_opt_replyp_postfix_wrapper() = 1;
      vp = 0;
    }
    else
      break;
  }

}



int
opt_welldefined (char *ep)
{
  if ((strcmp (ep, "USER") == 0) ||
      (strcmp (ep, "DISPLAY") == 0) ||
      (strcmp (ep, "PRINTER") == 0) ||
      (strcmp (ep, "SYSTEMTYPE") == 0) ||
      (strcmp (ep, "JOB") == 0) || (strcmp (ep, "ACCT") == 0))
    return (1);
  return (0);
}

void env_opt_end(register int emptyok)
{
  register int len;
  len = (get_opt_replyp_wrapper() - get_opt_reply_wrapper()) + 2;
  if (emptyok || (len > 6))
  {
    *gsetter_opt_replyp_postfix_wrapper() = 255;
    *gsetter_opt_replyp_postfix_wrapper() = 240;
    {
      Ring temp_netoring = get_netoring_wrapper();
      int temp_result_11 = ring_empty_count(&temp_netoring);
      set_netoring_wrapper(temp_netoring);
      if (temp_result_11 > len)
      {
        {
          Ring temp_netoring = get_netoring_wrapper();
          ring_supply_data(&temp_netoring, opt_reply, len);
          set_netoring_wrapper(temp_netoring);
        }
        printsub('>', &get_opt_reply_wrapper()[2], len - 2);
      }
      else
        printf("slc_end_reply: not enough room\n");
    }
  }
  if (get_opt_reply_wrapper())
  {
    free(get_opt_reply_wrapper());
    set_opt_reply_wrapper(opt_replyp = set_opt_replyend_wrapper(0));
  }
}





int telrcv(void)
{
  register int c;
  register int scc;
  register unsigned char *sbp;
  int count;
  int returnValue = 0;
  scc = 0;
  count = 0;
  while ((  {
    Ring temp_ttyoring = get_ttyoring_wrapper();
    ring_empty_count(&temp_ttyoring);
    set_ttyoring_wrapper(temp_ttyoring);
  }
) > 2)
  {
    if (scc == 0)
    {
      if (count)
      {
        {
          Ring temp_netiring = get_netiring_wrapper();
          ring_consumed(&temp_netiring, count);
          set_netiring_wrapper(temp_netiring);
        }
        returnValue = 1;
        count = 0;
      }
      sbp = get_netiring_consume_wrapper();
      {
        Ring temp_netiring = get_netiring_wrapper();
        int temp_result_12 = ring_full_consecutive(&temp_netiring);
        set_netiring_wrapper(temp_netiring);
        scc = temp_result_12;
      }
      if (scc == 0)
      {
        break;
      }
    }
    c = (*(sbp++)) & 0xff, scc--;
    count++;
    switch (get_telrcv_state_wrapper())
    {
      case 6:
        set_telrcv_state_wrapper(0);
        if (c == '\0')
      {
        break;
      }
      else
        if (((c == '\n') && (!(get_options_wrapper()[1] & 0x08))) && (!get_crmod_wrapper()))
      {
        if (!(get_SYNCHing_wrapper() || get_flushout_wrapper()))
        {
          *get_ttyoring_supply_wrapper() = c;
          {
            Ring temp_ttyoring = get_ttyoring_wrapper();
            ring_supplied(&temp_ttyoring, 1);
            set_ttyoring_wrapper(temp_ttyoring);
          }
        }
        ;
        break;
      }

      case 0:
        if (c == 255)
      {
        set_telrcv_state_wrapper(1);
        break;
      }
        if ((c == '\r') && (!(get_options_wrapper()[0] & 0x08)))
      {
        if (scc > 0)
        {
          c = (*sbp) & 0xff;
          if (c == 0)
          {
            sbp++, scc--;
            count++;
            if (!(get_SYNCHing_wrapper() || get_flushout_wrapper()))
            {
              *get_ttyoring_supply_wrapper() = '\r';
              {
                Ring temp_ttyoring = get_ttyoring_wrapper();
                ring_supplied(&temp_ttyoring, 1);
                set_ttyoring_wrapper(temp_ttyoring);
              }
            }
            ;
          }
          else
            if ((!(get_options_wrapper()[1] & 0x08)) && (c == '\n'))
          {
            sbp++, scc--;
            count++;
            if (!(get_SYNCHing_wrapper() || get_flushout_wrapper()))
            {
              *get_ttyoring_supply_wrapper() = '\n';
              {
                Ring temp_ttyoring = get_ttyoring_wrapper();
                ring_supplied(&temp_ttyoring, 1);
                set_ttyoring_wrapper(temp_ttyoring);
              }
            }
            ;
          }
          else
          {
            if (!(get_SYNCHing_wrapper() || get_flushout_wrapper()))
            {
              *get_ttyoring_supply_wrapper() = '\r';
              {
                Ring temp_ttyoring = get_ttyoring_wrapper();
                ring_supplied(&temp_ttyoring, 1);
                set_ttyoring_wrapper(temp_ttyoring);
              }
            }
            ;
            if (get_crmod_wrapper())
            {
              if (!(get_SYNCHing_wrapper() || get_flushout_wrapper()))
              {
                *get_ttyoring_supply_wrapper() = '\n';
                {
                  Ring temp_ttyoring = get_ttyoring_wrapper();
                  ring_supplied(&temp_ttyoring, 1);
                  set_ttyoring_wrapper(temp_ttyoring);
                }
              }
              ;
            }
          }
        }
        else
        {
          set_telrcv_state_wrapper(6);
          if (!(get_SYNCHing_wrapper() || get_flushout_wrapper()))
          {
            *get_ttyoring_supply_wrapper() = '\r';
            {
              Ring temp_ttyoring = get_ttyoring_wrapper();
              ring_supplied(&temp_ttyoring, 1);
              set_ttyoring_wrapper(temp_ttyoring);
            }
          }
          ;
          if (get_crmod_wrapper())
          {
            if (!(get_SYNCHing_wrapper() || get_flushout_wrapper()))
            {
              *get_ttyoring_supply_wrapper() = '\n';
              {
                Ring temp_ttyoring = get_ttyoring_wrapper();
                ring_supplied(&temp_ttyoring, 1);
                set_ttyoring_wrapper(temp_ttyoring);
              }
            }
            ;
          }
        }
      }
      else
      {
        if (!(get_SYNCHing_wrapper() || get_flushout_wrapper()))
        {
          *get_ttyoring_supply_wrapper() = c;
          {
            Ring temp_ttyoring = get_ttyoring_wrapper();
            ring_supplied(&temp_ttyoring, 1);
            set_ttyoring_wrapper(temp_ttyoring);
          }
        }
        ;
      }
        continue;

      case 1:
        process_iac:
      switch (c)
      {
        case 251:
          set_telrcv_state_wrapper(2);
          continue;

        case 252:
          set_telrcv_state_wrapper(3);
          continue;

        case 253:
          set_telrcv_state_wrapper(4);
          continue;

        case 254:
          set_telrcv_state_wrapper(5);
          continue;

        case 242:
          printoption("RCVD", 255, 242);
          set_SYNCHing_wrapper(1);
          ttyflush(1);
          set_SYNCHing_wrapper(stilloob());
          set_clocks_gotDM_wrapper(clocks.system++);
          break;

        case 250:
          set_subpointer_wrapper(subbuffer);
          ;
          set_telrcv_state_wrapper(7);
          continue;

        case 255:
          if (!(get_SYNCHing_wrapper() || get_flushout_wrapper()))
        {
          *get_ttyoring_supply_wrapper() = 255;
          {
            Ring temp_ttyoring = get_ttyoring_wrapper();
            ring_supplied(&temp_ttyoring, 1);
            set_ttyoring_wrapper(temp_ttyoring);
          }
        }
          ;
          break;

        case 241:

        case 249:

        default:
          printoption("RCVD", 255, c);
          break;

      }


        set_telrcv_state_wrapper(0);
        continue;

      case 2:
        printoption("RCVD", 251, c);
        willoption(c);
        ;
        set_telrcv_state_wrapper(0);
        continue;

      case 3:
        printoption("RCVD", 252, c);
        wontoption(c);
        ;
        set_telrcv_state_wrapper(0);
        continue;

      case 4:
        printoption("RCVD", 253, c);
        dooption(c);
        ;
        if (c == 31)
      {
        sendnaws();
      }
      else
        if (c == 33)
      {
        set_localflow_wrapper(1);
        setcommandmode();
        setconnmode(0);
      }
        set_telrcv_state_wrapper(0);
        continue;

      case 5:
        printoption("RCVD", 254, c);
        dontoption(c);
        set_flushline_wrapper(1);
        setconnmode(0);
        ;
        set_telrcv_state_wrapper(0);
        continue;

      case 7:
        if (c == 255)
      {
        set_telrcv_state_wrapper(8);
      }
      else
      {
        if (get_subpointer_wrapper() < (get_subbuffer_wrapper() + (sizeof(get_subbuffer_wrapper()))))
        {
          *gsetter_subpointer_postfix_wrapper() = c;
        }
        ;
      }
        continue;

      case 8:
        if (c != 240)
      {
        if (c != 255)
        {
          if (get_subpointer_wrapper() < (get_subbuffer_wrapper() + (sizeof(get_subbuffer_wrapper()))))
          {
            *gsetter_subpointer_postfix_wrapper() = 255;
          }
          ;
          if (get_subpointer_wrapper() < (get_subbuffer_wrapper() + (sizeof(get_subbuffer_wrapper()))))
          {
            *gsetter_subpointer_postfix_wrapper() = c;
          }
          ;
          set_subpointer_wrapper(2);
          {
            set_subend_wrapper(subpointer);
            set_subpointer_wrapper(subbuffer);
            ;
          }
          ;
          printoption("In SUBOPTION processing, RCVD", 255, c);
          suboption();
          ;
          set_telrcv_state_wrapper(1);
          goto process_iac;
        }
        if (get_subpointer_wrapper() < (get_subbuffer_wrapper() + (sizeof(get_subbuffer_wrapper()))))
        {
          *gsetter_subpointer_postfix_wrapper() = c;
        }
        ;
        set_telrcv_state_wrapper(7);
      }
      else
      {
        if (get_subpointer_wrapper() < (get_subbuffer_wrapper() + (sizeof(get_subbuffer_wrapper()))))
        {
          *gsetter_subpointer_postfix_wrapper() = 255;
        }
        ;
        if (get_subpointer_wrapper() < (get_subbuffer_wrapper() + (sizeof(get_subbuffer_wrapper()))))
        {
          *gsetter_subpointer_postfix_wrapper() = 240;
        }
        ;
        set_subpointer_wrapper(2);
        {
          set_subend_wrapper(subpointer);
          set_subpointer_wrapper(subbuffer);
          ;
        }
        ;
        suboption();
        ;
        set_telrcv_state_wrapper(0);
      }

    }

  }

  if (count)
  {
    Ring temp_netiring = get_netiring_wrapper();
    ring_consumed(&temp_netiring, count);
    set_netiring_wrapper(temp_netiring);
  }
  return returnValue || count;
}



static int bol = 1, local = 0;

int rlogin_susp(void)
{
  if (get_local_wrapper())
  {
    set_local_wrapper(0);
    set_bol_wrapper(1);
    command(0, "z\n", 2);
    return 1;
  }
  return 0;
}



static int telsnd(void)
{
  int tcc;
  int count;
  int returnValue = 0;
  unsigned char *tbp;
  tcc = 0;
  count = 0;
  while ((  {
    Ring temp_netoring = get_netoring_wrapper();
    ring_empty_count(&temp_netoring);
    set_netoring_wrapper(temp_netoring);
  }
) > 2)
  {
    register int sc;
    register int c;
    if (tcc == 0)
    {
      if (count)
      {
        {
          Ring temp_ttyiring = get_ttyiring_wrapper();
          ring_consumed(&temp_ttyiring, count);
          set_ttyiring_wrapper(temp_ttyiring);
        }
        returnValue = 1;
        count = 0;
      }
      tbp = get_ttyiring_consume_wrapper();
      {
        Ring temp_ttyiring = get_ttyiring_wrapper();
        int temp_result_12 = ring_full_consecutive(&temp_ttyiring);
        set_ttyiring_wrapper(temp_ttyiring);
        tcc = temp_result_12;
      }
      if (tcc == 0)
      {
        break;
      }
    }
    c = (*(tbp++)) & 0xff, sc = (!(get_options_wrapper()[0] & 0x02)) ? (c & 0x7f) : (c), tcc--;
    count++;
    if (get_rlogin_wrapper() != ((cc_t) '\377'))
    {
      if (get_bol_wrapper())
      {
        set_bol_wrapper(0);
        if (sc == get_rlogin_wrapper())
        {
          set_local_wrapper(1);
          continue;
        }
      }
      else
        if (get_local_wrapper())
      {
        set_local_wrapper(0);
        if ((sc == '.') || (c == get_new_tc_c_cc_wrapper()[VEOF]))
        {
          set_bol_wrapper(1);
          command(0, "close\n", 6);
          continue;
        }
        if (sc == termSuspChar)
        {
          set_bol_wrapper(1);
          command(0, "z\n", 2);
          continue;
        }
        if (sc == get_escape_wrapper())
        {
          command(0, (char *) tbp, tcc);
          set_bol_wrapper(1);
          count += tcc;
          tcc = 0;
          set_flushline_wrapper(1);
          break;
        }
        if (sc != get_rlogin_wrapper())
        {
          ++tcc;
          --tbp;
          --count;
          c = (sc = get_rlogin_wrapper());
        }
      }
      if ((sc == '\n') || (sc == '\r'))
        set_bol_wrapper(1);
    }
    else
      if (sc == get_escape_wrapper())
    {
      if (tcc && (((!(get_options_wrapper()[0] & 0x02)) ? ((*tbp) & 0x7f) : (*tbp)) == get_escape_wrapper()))
      {
        tbp++;
        tcc--;
        count++;
        set_bol_wrapper(0);
      }
      else
      {
        command(0, (char *) tbp, tcc);
        set_bol_wrapper(1);
        count += tcc;
        tcc = 0;
        set_flushline_wrapper(1);
        break;
      }
    }
    else
      set_bol_wrapper(0);
    if ((get_kludgelinemode_wrapper() && (get_globalmode_wrapper() & 0x01)) && (sc == get_echoc_wrapper()))
    {
      if ((tcc > 0) && (((!(get_options_wrapper()[0] & 0x02)) ? ((*tbp) & 0x7f) : (*tbp)) == get_echoc_wrapper()))
      {
        tcc--;
        tbp++;
        count++;
      }
      else
      {
        set_dontlecho_wrapper(!get_dontlecho_wrapper());
        set_clocks_echotoggle_wrapper(clocks.system++);
        setconnmode(0);
        set_flushline_wrapper(1);
        break;
      }
    }
    if (get_globalmode_wrapper() & (0x01 | 0x02))
    {
      if (TerminalSpecialChars(sc) == 0)
      {
        set_bol_wrapper(1);
        break;
      }
    }
    if (!(get_options_wrapper()[0] & 0x02))
    {
      switch (c)
      {
        case '\n':
          if (get_globalmode_wrapper() & (0x01 | 0x02))
        {
          {
            *get_netoring_supply_wrapper() = '\r';
            {
              Ring temp_netoring = get_netoring_wrapper();
              ring_supplied(&temp_netoring, 1);
              set_netoring_wrapper(temp_netoring);
            }
          }
          ;
        }
        {
          *get_netoring_supply_wrapper() = '\n';
          {
            Ring temp_netoring = get_netoring_wrapper();
            ring_supplied(&temp_netoring, 1);
            set_netoring_wrapper(temp_netoring);
          }
        }
          ;
          set_bol_wrapper(flushline = 1);
          break;

        case '\r':
          if (!crlf)
        {
          {
            {
              *get_netoring_supply_wrapper() = '\r';
              {
                Ring temp_netoring = get_netoring_wrapper();
                ring_supplied(&temp_netoring, 1);
                set_netoring_wrapper(temp_netoring);
              }
            }
            ;
            {
              *get_netoring_supply_wrapper() = '\0';
              {
                Ring temp_netoring = get_netoring_wrapper();
                ring_supplied(&temp_netoring, 1);
                set_netoring_wrapper(temp_netoring);
              }
            }
            ;
          }
          ;
        }
        else
        {
          {
            {
              *get_netoring_supply_wrapper() = '\r';
              {
                Ring temp_netoring = get_netoring_wrapper();
                ring_supplied(&temp_netoring, 1);
                set_netoring_wrapper(temp_netoring);
              }
            }
            ;
            {
              *get_netoring_supply_wrapper() = '\n';
              {
                Ring temp_netoring = get_netoring_wrapper();
                ring_supplied(&temp_netoring, 1);
                set_netoring_wrapper(temp_netoring);
              }
            }
            ;
          }
          ;
        }
          set_bol_wrapper(flushline = 1);
          break;

        case 255:
        {
          {
            *get_netoring_supply_wrapper() = 255;
            {
              Ring temp_netoring = get_netoring_wrapper();
              ring_supplied(&temp_netoring, 1);
              set_netoring_wrapper(temp_netoring);
            }
          }
          ;
          {
            *get_netoring_supply_wrapper() = 255;
            {
              Ring temp_netoring = get_netoring_wrapper();
              ring_supplied(&temp_netoring, 1);
              set_netoring_wrapper(temp_netoring);
            }
          }
          ;
        }
          ;
          break;

        default:
        {
          *get_netoring_supply_wrapper() = c;
          {
            Ring temp_netoring = get_netoring_wrapper();
            ring_supplied(&temp_netoring, 1);
            set_netoring_wrapper(temp_netoring);
          }
        }
          ;
          break;

      }

    }
    else
      if (c == 255)
    {
      {
        {
          *get_netoring_supply_wrapper() = 255;
          {
            Ring temp_netoring = get_netoring_wrapper();
            ring_supplied(&temp_netoring, 1);
            set_netoring_wrapper(temp_netoring);
          }
        }
        ;
        {
          *get_netoring_supply_wrapper() = 255;
          {
            Ring temp_netoring = get_netoring_wrapper();
            ring_supplied(&temp_netoring, 1);
            set_netoring_wrapper(temp_netoring);
          }
        }
        ;
      }
      ;
    }
    else
    {
      {
        *get_netoring_supply_wrapper() = c;
        {
          Ring temp_netoring = get_netoring_wrapper();
          ring_supplied(&temp_netoring, 1);
          set_netoring_wrapper(temp_netoring);
        }
      }
      ;
    }
  }

  if (count)
  {
    Ring temp_ttyiring = get_ttyiring_wrapper();
    ring_consumed(&temp_ttyiring, count);
    set_ttyiring_wrapper(temp_ttyiring);
  }
  return returnValue || count;
}



/*
 * Scheduler()
 *
 * Try to do something.
 *
 * If we do something useful, return 1; else return 0.
 *
 */


/* block; should we block in the select ? */
int Scheduler(int block)
{
  int returnValue;
  int netin;
  int netout;
  int netex;
  int ttyin;
  int ttyout;
  {
    Ring temp_netoring = get_netoring_wrapper();
    int temp_result_12 = ring_full_count(&temp_netoring);
    set_netoring_wrapper(temp_netoring);
    netout = temp_result_12 && ((flushline || ((!(options[34] & 0x02)) && ((!kludgelinemode) || (options[3] & 0x08)))) || (options[0] & 0x02));
  }
  {
    Ring temp_ttyoring = get_ttyoring_wrapper();
    int temp_result_12 = ring_full_count(&temp_ttyoring);
    set_ttyoring_wrapper(temp_ttyoring);
    ttyout = temp_result_12;
  }
  {
    Ring temp_ttyiring = get_ttyiring_wrapper();
    int temp_result_12 = ring_empty_count(&temp_ttyiring);
    set_ttyiring_wrapper(temp_ttyiring);
    ttyin = temp_result_12;
  }
  {
    Ring temp_netiring = get_netiring_wrapper();
    int temp_result_12 = ring_empty_count(&temp_netiring);
    set_netiring_wrapper(temp_netiring);
    netin = (!ISend) && temp_result_12;
  }
  netex = !get_SYNCHing_wrapper();
  returnValue = process_rings(netin, netout, netex, ttyin, ttyout, !block);
  if (  {
    Ring temp_ttyiring = get_ttyiring_wrapper();
    ring_full_count(&temp_ttyiring);
    set_ttyiring_wrapper(temp_ttyiring);
  }
)
  {
    returnValue |= telsnd();
  }
  if (  {
    Ring temp_netiring = get_netiring_wrapper();
    ring_full_count(&temp_netiring);
    set_netiring_wrapper(temp_netiring);
  }
)
  {
    returnValue |= telrcv();
  }
  return returnValue;
}



/*
 * Select from tty and network...
 */
void telnet(char *user)
{
  sys_telnet_init();
  (void) get_user_wrapper();
  if (get_telnetport_wrapper())
  {
    send_do(3, 1);
    send_will(24, 1);
    send_will(31, 1);
    send_will(32, 1);
    send_will(33, 1);
    send_will(34, 1);
    send_will(39, 1);
    send_do(5, 1);
    if (env_getvalue("DISPLAY"))
      send_will(35, 1);
    if (get_eight_wrapper())
      tel_enter_binary(get_eight_wrapper());
  }
  for (;;)
  {
    int schedValue;
    while ((schedValue = Scheduler(0)) != 0)
    {
      if (schedValue == (-1))
      {
        setcommandmode();
        return;
      }
    }

    if (Scheduler(1) == (-1))
    {
      setcommandmode();
      return;
    }
  }

}



#if 0				/* XXX - this not being in is a bug */
/*
 * nextitem()
 *
 *	Return the address of the next "item" in the TELNET data
 * stream.  This will be the address of the next character if
 * the current address is a user data character, or it will
 * be the address of the character following the TELNET command
 * if the current address is a TELNET IAC ("I Am a Command")
 * character.
 */

static char *
nextitem (char *current)
{
  if ((*current & 0xff) != IAC)
    {
      return current + 1;
    }
  switch (*(current + 1) & 0xff)
    {
    case DO:
    case DONT:
    case WILL:
    case WONT:
      return current + 3;
    case SB:			/* loop forever looking for the SE */
      {
	register char *look = current + 2;

	for (;;)
	  {
	    if ((*look++ & 0xff) == IAC)
	      {
		if ((*look++ & 0xff) == SE)
		  {
		    return look;
		  }
	      }
	  }
      }
    default:
      return current + 2;
    }
}
#endif /* 0 */

/*
 * netclear()
 *
 *	We are about to do a TELNET SYNCH operation.  Clear
 * the path to the network.
 *
 *	Things are a bit tricky since we may have sent the first
 * byte or so of a previous TELNET command into the network.
 * So, we have to scan the network buffer from the beginning
 * until we are up to where we want to be.
 *
 *	A side effect of what we do, just to keep things
 * simple, is to clear the urgent data pointer.  The principal
 * caller should be setting the urgent data pointer AFTER calling
 * us in any case.
 */

static void
netclear (void)
{
#if 0				/* XXX */
  register char *thisitem, *next;
  char *good;
# define wewant(p)	((nfrontp > p) && ((*p&0xff) == IAC) && \
				((*(p+1)&0xff) != EC) && ((*(p+1)&0xff) != EL))

  thisitem = netobuf;

  while ((next = nextitem (thisitem)) <= netobuf.send)
    {
      thisitem = next;
    }

  /* Now, thisitem is first before/at boundary. */

  good = netobuf;		/* where the good bytes go */

  while (netoring.add > thisitem)
    {
      if (wewant (thisitem))
	{
	  int length;

	  next = thisitem;
	  do
	    {
	      next = nextitem (next);
	    }
	  while (wewant (next) && (nfrontp > next));
	  length = next - thisitem;
	  memmove (good, thisitem, length);
	  good += length;
	  thisitem = next;
	}
      else
	{
	  thisitem = nextitem (thisitem);
	}
    }

#endif /* 0 */
}

/*
 * These routines add various telnet commands to the data stream.
 */

static void doflush(void)
{
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 253;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  {
    *get_netoring_supply_wrapper() = 6;
    {
      Ring temp_netoring = get_netoring_wrapper();
      ring_supplied(&temp_netoring, 1);
      set_netoring_wrapper(temp_netoring);
    }
  }
  ;
  set_flushline_wrapper(1);
  set_flushout_wrapper(1);
  ttyflush(1);
  printoption("SENT", 253, 6);
}



void xmitAO(void)
{
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 245;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  printoption("SENT", 255, 245);
  if (get_autoflush_wrapper())
  {
    doflush();
  }
}




void xmitEL(void)
{
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 248;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  printoption("SENT", 255, 248);
}



void xmitEC(void)
{
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 247;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  printoption("SENT", 255, 247);
}




int dosynch(void)
{
  netclear();
  {
    *get_netoring_supply_wrapper() = 255;
    {
      Ring temp_netoring = get_netoring_wrapper();
      ring_supplied(&temp_netoring, 1);
      set_netoring_wrapper(temp_netoring);
    }
  }
  ;
  setneturg();
  {
    *get_netoring_supply_wrapper() = 242;
    {
      Ring temp_netoring = get_netoring_wrapper();
      ring_supplied(&temp_netoring, 1);
      set_netoring_wrapper(temp_netoring);
    }
  }
  ;
  printoption("SENT", 255, 242);
  return 1;
}



int want_status_response = 0;

int get_status(void)
{
  unsigned char tmp[16];
  register unsigned char *cp;
  if (!(get_options_wrapper()[5] & 0x08))
  {
    printf("Remote side does not support STATUS option\n");
    return 0;
  }
  cp = tmp;
  *(cp++) = 255;
  *(cp++) = 250;
  *(cp++) = 5;
  *(cp++) = 1;
  *(cp++) = 255;
  *(cp++) = 240;
  {
    Ring temp_netoring = get_netoring_wrapper();
    int temp_result_12 = ring_empty_count(&temp_netoring);
    set_netoring_wrapper(temp_netoring);
    if (temp_result_12 >= (cp - tmp))
    {
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supply_data(&temp_netoring, tmp, cp - tmp);
        set_netoring_wrapper(temp_netoring);
      }
      printsub('>', tmp + 2, (cp - tmp) - 2);
    }
  }
  gsetter_want_status_response_prefix_wrapper();
  return 1;
}



void intp(void)
{
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 244;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  printoption("SENT", 255, 244);
  set_flushline_wrapper(1);
  if (get_autoflush_wrapper())
  {
    doflush();
  }
  if (autosynch)
  {
    dosynch();
  }
}



void sendbrk(void)
{
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 243;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  printoption("SENT", 255, 243);
  set_flushline_wrapper(1);
  if (get_autoflush_wrapper())
  {
    doflush();
  }
  if (autosynch)
  {
    dosynch();
  }
}



void sendabort(void)
{
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 238;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  printoption("SENT", 255, 238);
  set_flushline_wrapper(1);
  if (get_autoflush_wrapper())
  {
    doflush();
  }
  if (autosynch)
  {
    dosynch();
  }
}



void sendsusp(void)
{
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 237;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  printoption("SENT", 255, 237);
  set_flushline_wrapper(1);
  if (get_autoflush_wrapper())
  {
    doflush();
  }
  if (autosynch)
  {
    dosynch();
  }
}



void sendeof(void)
{
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 236;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  printoption("SENT", 255, 236);
}



void sendayt(void)
{
  {
    {
      *get_netoring_supply_wrapper() = 255;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
    {
      *get_netoring_supply_wrapper() = 246;
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supplied(&temp_netoring, 1);
        set_netoring_wrapper(temp_netoring);
      }
    }
    ;
  }
  ;
  printoption("SENT", 255, 246);
}



/*
 * Send a window size update to the remote system.
 */

void sendnaws(void)
{
  long rows;
  long cols;
  unsigned char tmp[16];
  register unsigned char *cp;
  if (!(get_options_wrapper()[31] & 0x01))
    return;
  if (TerminalWindowSize(&rows, &cols) == 0)
  {
    return;
  }
  cp = tmp;
  *(cp++) = 255;
  *(cp++) = 250;
  *(cp++) = 31;
  {
    if ((*(cp++) = (cols >> 8) & 0xff) == 255)
      *(cp++) = 255;
    if ((*(cp++) = cols & 0xff) == 255)
      *(cp++) = 255;
  }
  ;
  {
    if ((*(cp++) = (rows >> 8) & 0xff) == 255)
      *(cp++) = 255;
    if ((*(cp++) = rows & 0xff) == 255)
      *(cp++) = 255;
  }
  ;
  *(cp++) = 255;
  *(cp++) = 240;
  {
    Ring temp_netoring = get_netoring_wrapper();
    int temp_result_13 = ring_empty_count(&temp_netoring);
    set_netoring_wrapper(temp_netoring);
    if (temp_result_13 >= (cp - tmp))
    {
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_supply_data(&temp_netoring, tmp, cp - tmp);
        set_netoring_wrapper(temp_netoring);
      }
      printsub('>', tmp + 2, (cp - tmp) - 2);
    }
  }
}



void
tel_enter_binary (int rw)
{
  if (rw & 1)
    send_do (TELOPT_BINARY, 1);
  if (rw & 2)
    send_will (TELOPT_BINARY, 1);
}

void
tel_leave_binary (int rw)
{
  if (rw & 1)
    send_dont (TELOPT_BINARY, 1);
  if (rw & 2)
    send_wont (TELOPT_BINARY, 1);
}
