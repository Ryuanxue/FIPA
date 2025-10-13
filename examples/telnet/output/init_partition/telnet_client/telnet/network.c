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
 * Copyright (c) 1988, 1993
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

#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#include <arpa/telnet.h>
#include <sys/select.h>

#include "ring.h"

#include "defines.h"
#include "externs.h"

#include "telnet_rpc_wrapper.h"

Ring netoring, netiring;
unsigned char netobuf[2 * BUFSIZ], netibuf[BUFSIZ];

/*
 * Initialize internal network data structures.
 */

void init_network(void)
{
  {
    Ring temp_netoring = get_netoring_wrapper();
    int temp_result_14 = ring_init(&temp_netoring, netobuf, sizeof(netobuf));
    set_netoring_wrapper(temp_netoring);
    if (temp_result_14 != 1)
    {
      exit(1);
    }
  }
  {
    Ring temp_netiring = get_netiring_wrapper();
    int temp_result_15 = ring_init(&temp_netiring, netibuf, sizeof(netibuf));
    set_netiring_wrapper(temp_netiring);
    if (temp_result_15 != 1)
    {
      exit(1);
    }
  }
  set_NetTrace_wrapper(stdout);
}




/*
 * Check to see if any out-of-band data exists on a socket (for
 * Telnet "synch" processing).
 */

int stilloob(void)
{
  static struct timeval timeout = {0, 0};
  fd_set excepts;
  int value;
  do
  {
    FD_ZERO(&excepts);
    FD_SET(get_net_wrapper(), &excepts);
    value = select(get_net_wrapper() + 1, (fd_set *) 0, (fd_set *) 0, &excepts, &timeout);
  }
  while ((value == (-1)) && (errno == EINTR));
  if (value < 0)
  {
    perror("select");
    (void) quit();
  }
  if (FD_ISSET(get_net_wrapper(), &excepts))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}




/*
 *  setneturg()
 *
 *	Sets "neturg" to the current location.
 */

void setneturg(void)
{
  {
    Ring temp_netoring = get_netoring_wrapper();
    ring_mark(&temp_netoring);
    set_netoring_wrapper(temp_netoring);
  }
}




/*
 *  netflush
 *		Send as much data as possible to the network,
 *	handling requests for urgent data.
 *
 *		The return value indicates whether we did any
 *	useful work.
 */


int netflush(void)
{
  register int n;
  register int n1;
  if ((n1 =   {
    Ring temp_netoring = get_netoring_wrapper();
    int temp_result_16 = ring_full_consecutive(&temp_netoring);
    set_netoring_wrapper(temp_netoring);
    n = temp_result_16;
  }
) > 0)
  {
    {
      Ring temp_netoring = get_netoring_wrapper();
      int temp_result_16 = ring_at_mark(&temp_netoring);
      set_netoring_wrapper(temp_netoring);
      if (!temp_result_16)
      {
        n = send(get_net_wrapper(), (char *) get_netoring_consume_wrapper(), n, 0);
      }
      else
      {
        n = send(get_net_wrapper(), (char *) get_netoring_consume_wrapper(), 1, MSG_OOB);
      }
    }
  }
  if (n < 0)
  {
    if ((errno != ENOBUFS) && (errno != EWOULDBLOCK))
    {
      setcommandmode();
      perror(get_hostname_wrapper());
      (void) NetClose(get_net_wrapper());
      {
        Ring temp_netoring = get_netoring_wrapper();
        ring_clear_mark(&temp_netoring);
        set_netoring_wrapper(temp_netoring);
      }
      longjmp(get_peerdied_wrapper(), -1);
    }
    n = 0;
  }
  if (netdata && n)
  {
    Dump('>', get_netoring_consume_wrapper(), n);
  }
  if (n)
  {
    {
      Ring temp_netoring = get_netoring_wrapper();
      ring_consumed(&temp_netoring, n);
      set_netoring_wrapper(temp_netoring);
    }
    {
      Ring temp_netoring = get_netoring_wrapper();
      int temp_result_17 = ring_full_consecutive(&temp_netoring);
      set_netoring_wrapper(temp_netoring);
      if ((n1 == n) && temp_result_17)
      {
        (void) netflush();
      }
    }
    return 1;
  }
  else
  {
    return 0;
  }
}


