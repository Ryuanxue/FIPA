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

/*
 * The following routines try to encapsulate what is system dependent
 * (at least between 4.x and dos) which is used in telnet.c.
 */

#include <config.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>
#include <arpa/telnet.h>
#include <sys/select.h>
#include <unused-parameter.h>

#include "ring.h"

#include "defines.h"
#include "externs.h"
#include "types.h"

#include "telnet_rpc_wrapper.h"

#ifdef	SIGINFO
extern void ayt_status ();
extern void sendayt ();
#endif

int tout,			/* Output file descriptor */
  tin,				/* Input file descriptor */
  net;

#ifndef USE_TERMIO
struct tchars otc = { 0 }, ntc = { 0 };
struct ltchars oltc = { 0 }, nltc = { 0 };
struct sgttyb ottyb = { 0 }, nttyb = { 0 };
int olmode = 0;
# define cfgetispeed(ptr)	(ptr)->sg_ispeed
# define cfgetospeed(ptr)	(ptr)->sg_ospeed
# define old_tc ottyb

#else /* USE_TERMIO */
struct termio old_tc = { 0 };
extern struct termio new_tc;

# ifndef TCSANOW
#  ifdef TCSETS
#   define TCSANOW		TCSETS
#   define TCSADRAIN	TCSETSW
#   define tcgetattr(f, t) ioctl(f, TCGETS, (char *)t)
#  else
#   ifdef TCSETA
#    define TCSANOW		TCSETA
#    define TCSADRAIN	TCSETAW
#    define tcgetattr(f, t) ioctl(f, TCGETA, (char *)t)
#   else
#    define TCSANOW		TIOCSETA
#    define TCSADRAIN	TIOCSETAW
#    define tcgetattr(f, t) ioctl(f, TIOCGETA, (char *)t)
#   endif
#  endif
#  define tcsetattr(f, a, t) ioctl(f, a, (char *)t)
#  define cfgetospeed(ptr)	((ptr)->c_cflag&CBAUD)
#  ifdef CIBAUD
#   define cfgetispeed(ptr)	(((ptr)->c_cflag&CIBAUD) >> IBSHIFT)
#  else
#   define cfgetispeed(ptr)	cfgetospeed(ptr)
#  endif
# endif	/* TCSANOW */
# ifdef	sysV88
#  define TIOCFLUSH TC_PX_DRAIN
# endif
#endif /* USE_TERMIO */

static fd_set ibits, obits, xbits;


void init_sys(void)
{
  set_tout_wrapper(fileno(stdout));
  set_tin_wrapper(fileno(stdin));
  {
    fd_set temp_ibits = get_ibits_wrapper();
    FD_ZERO(&temp_ibits);
    set_ibits_wrapper(temp_ibits);
  }
  {
    fd_set temp_obits = get_obits_wrapper();
    FD_ZERO(&temp_obits);
    set_obits_wrapper(temp_obits);
  }
  {
    fd_set temp_xbits = get_xbits_wrapper();
    FD_ZERO(&temp_xbits);
    set_xbits_wrapper(temp_xbits);
  }
  errno = 0;
}




int TerminalWrite(char *buf, int n)
{
  return write(get_tout_wrapper(), buf, n);
}





/*
 *
 */

int
TerminalAutoFlush (void)
{
#if defined LNOFLSH
  int flush;

  ioctl (0, TIOCLGET, (char *) &flush);
  return !(flush & LNOFLSH);	/* if LNOFLSH, no autoflush */
#else /* LNOFLSH */
  return 1;
#endif /* LNOFLSH */
}

#ifdef	KLUDGELINEMODE
extern int kludgelinemode;
#endif
/*
 * TerminalSpecialChars()
 *
 * Look at an input character to see if it is a special character
 * and decide what to do.
 *
 * Output:
 *
 *	0	Don't add this character.
 *	1	Do add this character
 */

extern void xmitAO (void), xmitEL (void), xmitEC (void), intp (void), sendbrk (void);

int TerminalSpecialChars(int c)
{
  if (c == get_new_tc_c_cc_wrapper()[VINTR])
  {
    intp();
    return 0;
  }
  else
    if (c == get_new_tc_c_cc_wrapper()[VQUIT])
  {
    if (get_kludgelinemode_wrapper())
      sendbrk();
    else
      sendabort();
    return 0;
  }
  else
    if (c == get_new_tc_c_cc_wrapper()[VEOF])
  {
    if (get_options_wrapper()[34] & 0x02)
    {
      sendeof();
      return 0;
    }
    return 1;
  }
  else
    if (c == termSuspChar)
  {
    sendsusp();
    return 0;
  }
  else
    if (c == termFlushChar)
  {
    xmitAO();
    return 0;
  }
  else
    if (!(get_globalmode_wrapper() & (0x01 | 0x02)))
  {
    if (c == get_new_tc_c_cc_wrapper()[VKILL])
    {
      xmitEL();
      return 0;
    }
    else
      if (c == get_new_tc_c_cc_wrapper()[VERASE])
    {
      xmitEC();
      return 0;
    }
  }
  return 1;
}




/*
 * Flush output to the terminal
 */

void
TerminalFlushOutput (void)
{
  int flags = 0;
#ifdef	TIOCFLUSH
  ioctl (fileno (stdout), TIOCFLUSH, &flags);
#else
  ioctl (fileno (stdout), TCFLSH, &flags);
#endif
}

void TerminalSaveState(void)
{
  ioctl(0, TIOCGETA, (char *) (&get_old_tc_wrapper()));
  set_new_tc_wrapper(old_tc);
  termFlushChar = 'O' & 0x1f;
  termWerasChar = 'W' & 0x1f;
  termRprntChar = 'R' & 0x1f;
  termLiteralNextChar = 'V' & 0x1f;
  termStartChar = 'Q' & 0x1f;
  termStopChar = 'S' & 0x1f;
  set_termAytChar_wrapper('T' & 0x1f);
}



cc_t *tcval(register int func)
{
  switch (func)
  {
    case 3:
      return &get_new_tc_c_cc_wrapper()[VINTR];

    case 7:
      return &get_new_tc_c_cc_wrapper()[VQUIT];

    case 8:
      return &get_new_tc_c_cc_wrapper()[VEOF];

    case 10:
      return &get_new_tc_c_cc_wrapper()[VERASE];

    case 11:
      return &get_new_tc_c_cc_wrapper()[VKILL];

    case 15:
      return &termStartChar;

    case 16:
      return &termStopChar;

    case 17:
      return &termForw1Char;

    case 18:
      return &termForw2Char;

    case 1:

    case 2:

    case 6:

    default:
      return (cc_t *) 0;

  }

}



void TerminalDefaultChars(void)
{
  memmove(get_new_tc_c_cc_wrapper(), get_old_tc_c_cc_wrapper(), sizeof(get_old_tc_c_cc_wrapper()));
  termFlushChar = 'O' & 0x1f;
  termWerasChar = 'W' & 0x1f;
  termRprntChar = 'R' & 0x1f;
  termLiteralNextChar = 'V' & 0x1f;
  termStartChar = 'Q' & 0x1f;
  termStopChar = 'S' & 0x1f;
  set_termAytChar_wrapper('T' & 0x1f);
}



/*
 * TerminalNewMode - set up terminal to a specific mode.
 *	MODE_ECHO: do local terminal echo
 *	MODE_FLOW: do local flow control
 *	MODE_TRAPSIG: do local mapping to TELNET IAC sequences
 *	MODE_EDIT: do local line editing
 *
 *	Command mode:
 *		MODE_ECHO|MODE_EDIT|MODE_FLOW|MODE_TRAPSIG
 *		local echo
 *		local editing
 *		local xon/xoff
 *		local signal mapping
 *
 *	Linemode:
 *		local/no editing
 *	Both Linemode and Single Character mode:
 *		local/remote echo
 *		local/no xon/xoff
 *		local/no signal mapping
 */


void TerminalNewMode(register int f)
{
  static int prevmode = 0;
  struct termios tmp_tc;
  int onoff;
  int old;
  cc_t esc;
  set_globalmode_wrapper(f & (~0x1000));
  if (prevmode == f)
    return;
  old = ttyflush(get_SYNCHing_wrapper() | get_flushout_wrapper());
  if ((old < 0) || (old > 1))
  {
    ioctl(get_tin_wrapper(), TIOCGETA, (char *) (&tmp_tc));
    do
    {
      ioctl(get_tin_wrapper(), TIOCSETAW, (char *) (&tmp_tc));
      old = ttyflush(get_SYNCHing_wrapper() | get_flushout_wrapper());
    }
    while ((old < 0) || (old > 1));
  }
  old = prevmode;
  prevmode = f & (~0x1000);
  tmp_tc = get_new_tc_wrapper();
  if (f & 0x0200)
  {
    tmp_tc.c_lflag |= ECHO;
    tmp_tc.c_oflag |= ONLCR;
    if (crlf)
      tmp_tc.c_iflag |= ICRNL;
  }
  else
  {
    tmp_tc.c_lflag &= ~ECHO;
    tmp_tc.c_oflag &= ~ONLCR;
  }
  if ((f & 0x0100) == 0)
  {
    tmp_tc.c_iflag &= ~(IXOFF | IXON);
  }
  else
  {
    if (get_restartany_wrapper() < 0)
    {
      tmp_tc.c_iflag |= IXOFF | IXON;
    }
    else
      if (get_restartany_wrapper() > 0)
    {
      tmp_tc.c_iflag |= (IXOFF | IXON) | IXANY;
    }
    else
    {
      tmp_tc.c_iflag |= IXOFF | IXON;
      tmp_tc.c_iflag &= ~IXANY;
    }
  }
  if ((f & 0x02) == 0)
  {
    tmp_tc.c_lflag &= ~ISIG;
    set_localchars_wrapper(0);
  }
  else
  {
    tmp_tc.c_lflag |= ISIG;
    set_localchars_wrapper(1);
  }
  if (f & 0x01)
  {
    tmp_tc.c_lflag |= ICANON;
  }
  else
  {
    tmp_tc.c_lflag &= ~ICANON;
    tmp_tc.c_iflag &= ~ICRNL;
    tmp_tc.c_cc[VMIN] = 1;
    tmp_tc.c_cc[VTIME] = 0;
  }
  if ((f & (0x01 | 0x02)) == 0)
  {
  }
  if (f & 0x08)
  {
  }
  else
  {
  }
  if (f & 0x10)
  {
  }
  else
  {
  }
  if (f == (-1))
  {
    onoff = 0;
  }
  else
  {
    if (f & 0x0400)
      tmp_tc.c_iflag &= ~ISTRIP;
    if (f & 0x0800)
    {
      tmp_tc.c_cflag &= ~(CSIZE | PARENB);
      tmp_tc.c_cflag |= CS8;
      tmp_tc.c_oflag &= ~OPOST;
    }
    else
    {
      tmp_tc.c_cflag &= ~(CSIZE | PARENB);
      tmp_tc.c_cflag |= get_old_tc_c_cflag_wrapper() & (CSIZE | PARENB);
      tmp_tc.c_oflag |= OPOST;
    }
    onoff = 1;
  }
  if (f != (-1))
  {
    esc = (get_rlogin_wrapper() != ((cc_t) '\377')) ? (get_rlogin_wrapper()) : (get_escape_wrapper());
    if (tmp_tc.c_cc[VEOL] != esc)
    {
      if (tmp_tc.c_cc[VEOL] == ((cc_t) ((cc_t) '\377')))
        tmp_tc.c_cc[VEOL] = esc;
    }
  }
  else
  {
    tmp_tc = get_old_tc_wrapper();
  }
  if (ioctl(get_tin_wrapper(), TIOCSETAW, (char *) (&tmp_tc)) < 0)
    ioctl(get_tin_wrapper(), TIOCSETA, (char *) (&tmp_tc));
  ioctl(get_tin_wrapper(), FIONBIO, (char *) (&onoff));
  ioctl(get_tout_wrapper(), FIONBIO, (char *) (&onoff));
}



/*
 * Try to guess whether speeds are "encoded" (4.2BSD) or just numeric (4.4BSD).
 */
#if B4800 != 4800
# define DECODE_BAUD
#endif

#ifdef	DECODE_BAUD
# ifndef B7200
#  define B7200   B4800
# endif

# ifndef B14400
#  define B14400  B9600
# endif

# ifndef B19200
#  define B19200 B14400
# endif

# ifndef B28800
#  define B28800  B19200
# endif

# ifndef B38400
#  define B38400 B28800
# endif

# ifndef B57600
#  define B57600  B38400
# endif

# ifndef B76800
#  define B76800  B57600
# endif

# ifndef B115200
#  define B115200 B76800
# endif

# ifndef B230400
#  define B230400 B115200
# endif


/*
 * This code assumes that the values B0, B50, B75...
 * are in ascending order.  They do not have to be
 * contiguous.
 */
struct termspeeds
{
  long speed;
  long value;
} termspeeds[] =
  {
    {0, B0},
    {50, B50},
    {75, B75},
    {110, B110},
    {134, B134},
    {150, B150},
    {200, B200},
    {300, B300},
    {600, B600},
    {1200, B1200},
    {1800, B1800},
    {2400, B2400},
    {4800, B4800},
    {7200, B7200},
    {9600, B9600},
    {14400, B14400},
    {19200, B19200},
    {28800, B28800},
    {38400, B38400},
    {57600, B57600},
    {115200, B115200},
    {230400, B230400},
    {-1, B230400}
  };
#endif /* DECODE_BAUD */

void TerminalSpeeds(long *ispeed, long *ospeed)
{
  register struct termspeeds *tp;
  register long in;
  register long out;
  out = (&get_old_tc_wrapper())->c_cflag & CBAUD;
  in = (&get_old_tc_wrapper())->c_cflag & CBAUD;
  if (in == 0)
    in = out;
  tp = termspeeds;
  while ((tp->speed != (-1)) && (tp->value < in))
    tp++;

  *ispeed = tp->speed;
  tp = termspeeds;
  while ((tp->speed != (-1)) && (tp->value < out))
    tp++;

  *ospeed = tp->speed;
}



int
TerminalWindowSize (long *rows, long *cols)
{
#ifdef	TIOCGWINSZ
  struct winsize ws;

  if (ioctl (fileno (stdin), TIOCGWINSZ, (char *) &ws) >= 0)
    {
      *rows = ws.ws_row;
      *cols = ws.ws_col;
      return 1;
    }
#endif /* TIOCGWINSZ */
  return 0;
}

int
NetClose (int fd)
{
  return close (fd);
}


void
NetNonblockingIO (int fd, int onoff)
{
  ioctl (fd, FIONBIO, (char *) &onoff);
}

#if defined TN3270
void
NetSigIO (int fd, int onoff)
{
  ioctl (fd, FIOASYNC, (char *) &onoff);	/* hear about input */
}

void
NetSetPgrp (int fd)
{
  int myPid;

  myPid = getpid ();
  fcntl (fd, F_SETOWN, myPid);
}
#endif /*defined(TN3270) */

/*
 * Various signal handling routines.
 */

void deadpeer(int sig)
{
  setcommandmode();
  longjmp(get_peerdied_wrapper(), -1);
}




void intr(int sig)
{
  if (get_localchars_wrapper())
  {
    intp();
    return;
  }
  setcommandmode();
  longjmp(toplevel, -1);
}




void intr2(int sig)
{
  if (get_localchars_wrapper())
  {
    if (get_kludgelinemode_wrapper())
      sendbrk();
    else
      sendabort();
    return;
  }
}



#ifdef	SIGTSTP

#endif

#ifdef	SIGWINCH
void sendwin(int sig)
{
  if (get_connected_wrapper())
  {
    sendnaws();
  }
}


#endif

#ifdef	SIGINFO
void
ayt (int sig _GL_UNUSED_PARAMETER)
{
  if (connected)
    sendayt ();
  else
    ayt_status ();
}
#endif


void sys_telnet_init(void)
{
  struct sigaction sa;
  sa.sa_flags = SA_RESTART;
  sigemptyset(&sa.sa_mask);
  signal(SIGINT, intr);
  signal(SIGQUIT, intr2);
  signal(SIGPIPE, deadpeer);
  sa.sa_handler = sendwin;
  (void) sigaction(28, &sa, 0);
  setconnmode(0);
  NetNonblockingIO(get_net_wrapper(), 1);
}



/*
 * Process rings -
 *
 *	This routine tries to fill up/empty our various rings.
 *
 *	The parameter specifies whether this is a poll operation,
 *	or a block-until-something-happens operation.
 *
 *	The return value is 1 if something happened, 0 if not.
 */

/* poll; If 0, then block until something to do */
int process_rings(int netin, int netout, int netex, int ttyin, int ttyout, int poll)
{
  register int c;
  int returnValue = 0;
  static struct timeval TimeValue = {0, 0};
  int nfds = 0;
  if (netout)
  {
    {
      fd_set temp_obits = get_obits_wrapper();
      FD_SET(net, &temp_obits);
      set_obits_wrapper(temp_obits);
    }
    if (get_net_wrapper() > nfds)
      nfds = get_net_wrapper();
  }
  if (ttyout)
  {
    {
      fd_set temp_obits = get_obits_wrapper();
      FD_SET(tout, &temp_obits);
      set_obits_wrapper(temp_obits);
    }
    if (get_tout_wrapper() > nfds)
      nfds = get_tout_wrapper();
  }
  if (ttyin)
  {
    {
      fd_set temp_ibits = get_ibits_wrapper();
      FD_SET(tin, &temp_ibits);
      set_ibits_wrapper(temp_ibits);
    }
    if (get_tin_wrapper() > nfds)
      nfds = get_tin_wrapper();
  }
  if (netin)
  {
    {
      fd_set temp_ibits = get_ibits_wrapper();
      FD_SET(net, &temp_ibits);
      set_ibits_wrapper(temp_ibits);
    }
    if (get_net_wrapper() > nfds)
      nfds = get_net_wrapper();
  }
  if (netex)
  {
    {
      fd_set temp_xbits = get_xbits_wrapper();
      FD_SET(net, &temp_xbits);
      set_xbits_wrapper(temp_xbits);
    }
    if (get_net_wrapper() > nfds)
      nfds = get_net_wrapper();
  }
  if ((  {
    fd_set temp_ibits = get_ibits_wrapper();
    fd_set temp_obits = get_obits_wrapper();
    fd_set temp_xbits = get_xbits_wrapper();
    int temp_result_3 = select(nfds + 1, &temp_ibits, &temp_obits, &temp_xbits, (poll == 0) ? ((struct timeval *) 0) : (&TimeValue));
    set_ibits_wrapper(temp_ibits);
    set_obits_wrapper(temp_obits);
    set_xbits_wrapper(temp_xbits);
    c = temp_result_3;
  }
) < 0)
  {
    if (c == (-1))
    {
      if (errno == EINTR)
      {
        return 0;
      }
      printf("sleep(5) from telnet, after select\r\n");
      sleep(5);
    }
    return 0;
  }
  if (  {
    fd_set temp_xbits = get_xbits_wrapper();
    FD_ISSET(net, &temp_xbits);
    set_xbits_wrapper(temp_xbits);
  }
)
  {
    {
      fd_set temp_xbits = get_xbits_wrapper();
      FD_CLR(net, &temp_xbits);
      set_xbits_wrapper(temp_xbits);
    }
    set_SYNCHing_wrapper(1);
    ttyflush(1);
  }
  if (  {
    fd_set temp_ibits = get_ibits_wrapper();
    FD_ISSET(net, &temp_ibits);
    set_ibits_wrapper(temp_ibits);
  }
)
  {
    int canread;
    {
      fd_set temp_ibits = get_ibits_wrapper();
      FD_CLR(net, &temp_ibits);
      set_ibits_wrapper(temp_ibits);
    }
    {
      Ring temp_netiring = get_netiring_wrapper();
      int temp_result_3 = ring_empty_consecutive(&temp_netiring);
      set_netiring_wrapper(temp_netiring);
      canread = temp_result_3;
    }
    if (get_SYNCHing_wrapper())
    {
      int atmark;
      static int bogus_oob = 0;
      static int first = 1;
      ioctl(get_net_wrapper(), SIOCATMARK, (char *) (&atmark));
      if (atmark)
      {
        c = recv(get_net_wrapper(), get_netiring_supply_wrapper(), canread, MSG_OOB);
        if ((c == (-1)) && (errno == EINVAL))
        {
          c = recv(get_net_wrapper(), get_netiring_supply_wrapper(), canread, 0);
          if (get_clocks_didnetreceive_wrapper() < get_clocks_gotDM_wrapper())
          {
            set_SYNCHing_wrapper(stilloob(get_net_wrapper()));
          }
        }
        else
          if (first && (c > 0))
        {
          int i;
          i = recv(get_net_wrapper(), get_netiring_supply_wrapper() + c, canread - c, MSG_OOB);
          if ((i == c) && (memcmp(get_netiring_supply_wrapper(), get_netiring_supply_wrapper() + c, i) == 0))
          {
            bogus_oob = 1;
            first = 0;
          }
          else
            if (i < 0)
          {
            bogus_oob = 0;
            first = 0;
          }
          else
            c += i;
        }
        if (bogus_oob && (c > 0))
        {
          int i;
          i = read(get_net_wrapper(), get_netiring_supply_wrapper() + c, canread - c);
          if (i > 0)
            c += i;
        }
      }
      else
      {
        c = recv(get_net_wrapper(), get_netiring_supply_wrapper(), canread, 0);
      }
    }
    else
    {
      c = recv(get_net_wrapper(), get_netiring_supply_wrapper(), canread, 0);
    }
    set_clocks_didnetreceive_wrapper(clocks.system++);
    if ((c < 0) && (errno == EWOULDBLOCK))
    {
      c = 0;
    }
    else
      if (c <= 0)
    {
      return -1;
    }
    if (netdata)
    {
      Dump('<', get_netiring_supply_wrapper(), c);
    }
    if (c)
    {
      Ring temp_netiring = get_netiring_wrapper();
      ring_supplied(&temp_netiring, c);
      set_netiring_wrapper(temp_netiring);
    }
    returnValue = 1;
  }
  if (  {
    fd_set temp_ibits = get_ibits_wrapper();
    FD_ISSET(tin, &temp_ibits);
    set_ibits_wrapper(temp_ibits);
  }
)
  {
    {
      fd_set temp_ibits = get_ibits_wrapper();
      FD_CLR(tin, &temp_ibits);
      set_ibits_wrapper(temp_ibits);
    }
    c = TerminalRead((char *) get_ttyiring_supply_wrapper(),     {
      Ring temp_ttyiring = get_ttyiring_wrapper();
      ring_empty_consecutive(&temp_ttyiring);
      set_ttyiring_wrapper(temp_ttyiring);
    }
);
    if ((c < 0) && (errno == EIO))
      c = 0;
    if ((c < 0) && (errno == EWOULDBLOCK))
    {
      c = 0;
    }
    else
    {
      if (((c == 0) && (get_globalmode_wrapper() & (0x01 | 0x02))) && isatty(get_tin_wrapper()))
      {
        *get_ttyiring_supply_wrapper() = get_new_tc_c_cc_wrapper()[VEOF];
        c = 1;
      }
      if (c <= 0)
      {
        return -1;
      }
      if (termdata)
      {
        Dump('<', get_ttyiring_supply_wrapper(), c);
      }
      {
        Ring temp_ttyiring = get_ttyiring_wrapper();
        ring_supplied(&temp_ttyiring, c);
        set_ttyiring_wrapper(temp_ttyiring);
      }
    }
    returnValue = 1;
  }
  if (  {
    fd_set temp_obits = get_obits_wrapper();
    FD_ISSET(net, &temp_obits);
    set_obits_wrapper(temp_obits);
  }
)
  {
    {
      fd_set temp_obits = get_obits_wrapper();
      FD_CLR(net, &temp_obits);
      set_obits_wrapper(temp_obits);
    }
    returnValue |= netflush();
  }
  if (  {
    fd_set temp_obits = get_obits_wrapper();
    FD_ISSET(tout, &temp_obits);
    set_obits_wrapper(temp_obits);
  }
)
  {
    {
      fd_set temp_obits = get_obits_wrapper();
      FD_CLR(tout, &temp_obits);
      set_obits_wrapper(temp_obits);
    }
    returnValue |= ttyflush(get_SYNCHing_wrapper() | get_flushout_wrapper()) > 0;
  }
  return returnValue;
}


