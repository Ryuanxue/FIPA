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

#define TELOPTS
#define TELCMDS
#define SLC_NAMES
#include <arpa/telnet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>

#include "general.h"
#include "ring.h"
#include "defines.h"
#include "externs.h"

#include "telnet_rpc_wrapper.h"

#ifdef AUTHENTICATION
# include <libtelnet/auth.h>
#endif
#ifdef ENCRYPTION
# include <libtelnet/encrypt.h>
#endif

FILE *NetTrace = 0;		/* Not in bss, since needs to stay */
int prettydump;

/*
 * upcase()
 *
 *	Upcase (in place) the argument.
 */

void
upcase (register char *argument)
{
  register int c;

  while ((c = *argument) != 0)
    {
      if (islower (c))
	{
	  *argument = toupper (c);
	}
      argument++;
    }
}

/*
 * SetSockOpt()
 *
 * Compensate for differences in 4.2 and 4.3 systems.
 */

int
SetSockOpt (int fd, int level, int option, int yesno)
{
#ifndef NOT43
  return setsockopt (fd, level, option, (char *) &yesno, sizeof yesno);
#else /* NOT43 */
  if (yesno == 0)
    {				/* Can't do that in 4.2! */
      fprintf (stderr, "Error: attempt to turn off an option 0x%x.\n",
	       option);
      return -1;
    }
  return setsockopt (fd, level, option, 0, 0);
#endif /* NOT43 */
}

/*
 * The following are routines used to print out debugging information.
 */

unsigned char NetTraceFile[256] = "(standard output)";

void SetNetTrace(register char *file)
{
  if (get_NetTrace_wrapper() && (get_NetTrace_wrapper() != stdout))
    fclose(get_NetTrace_wrapper());
  if (file && (strcmp(file, "-") != 0))
  {
    set_NetTrace_wrapper(fopen(file, "w"));
    if (get_NetTrace_wrapper())
    {
      strncpy((char *) NetTraceFile, file, sizeof(NetTraceFile));
      NetTraceFile[(sizeof(NetTraceFile)) - 1] = 0;
      return;
    }
    fprintf(stderr, "Cannot open %s.\n", file);
  }
  set_NetTrace_wrapper(stdout);
  strcpy((char *) NetTraceFile, "(standard output)");
}



void Dump(char direction, unsigned char *buffer, int length)
{
  unsigned char *pThis;
  int offset;
  offset = 0;
  while (length)
  {
    fprintf(get_NetTrace_wrapper(), "%c 0x%x\t", direction, offset);
    pThis = buffer;
    if (prettydump)
    {
      buffer = buffer + ((length < (32 / 2)) ? (length) : (32 / 2));
      while (pThis < buffer)
      {
        fprintf(get_NetTrace_wrapper(), "%c%.2x", (((*pThis) & 0xff) == 0xff) ? ('*') : (' '), (*pThis) & 0xff);
        pThis++;
      }

      length -= 32 / 2;
      offset += 32 / 2;
    }
    else
    {
      buffer = buffer + ((length < 32) ? (length) : (32));
      while (pThis < buffer)
      {
        fprintf(get_NetTrace_wrapper(), "%.2x", (*pThis) & 0xff);
        pThis++;
      }

      length -= 32;
      offset += 32;
    }
    if (get_NetTrace_wrapper() == stdout)
    {
      fprintf(get_NetTrace_wrapper(), "\r\n");
    }
    else
    {
      fprintf(get_NetTrace_wrapper(), "\n");
    }
    if (length < 0)
    {
      fflush(get_NetTrace_wrapper());
      return;
    }
  }

  fflush(get_NetTrace_wrapper());
}




void printoption(char *direction, int cmd, int option)
{
  if (!showoptions)
    return;
  if (cmd == 255)
  {
    if ((((unsigned int) option) <= 255) && (((unsigned int) option) >= 236))
      fprintf(get_NetTrace_wrapper(), "%s IAC %s", direction, telcmds[option - 236]);
    else
      fprintf(get_NetTrace_wrapper(), "%s IAC %d", direction, option);
  }
  else
  {
    register char *fmt;
    fmt = (cmd == 251) ? ("WILL") : ((cmd == 252) ? ("WONT") : ((cmd == 253) ? ("DO") : ((cmd == 254) ? ("DONT") : (0))));
    if (fmt)
    {
      fprintf(get_NetTrace_wrapper(), "%s %s ", direction, fmt);
      if (((unsigned int) option) <= 39)
        fprintf(get_NetTrace_wrapper(), "%s", get_telopts_wrapper()[option - 0]);
      else
        if (option == 255)
        fprintf(get_NetTrace_wrapper(), "EXOPL");
      else
        fprintf(get_NetTrace_wrapper(), "%d", option);
    }
    else
      fprintf(get_NetTrace_wrapper(), "%s %d %d", direction, cmd, option);
  }
  if (get_NetTrace_wrapper() == stdout)
  {
    fprintf(get_NetTrace_wrapper(), "\r\n");
    fflush(get_NetTrace_wrapper());
  }
  else
  {
    fprintf(get_NetTrace_wrapper(), "\n");
  }
  return;
}



void optionstatus(void)
{
  register int i;
  extern char will_wont_resp[];
  extern char do_dont_resp[];
  for (i = 0; i < 256; i++)
  {
    if (get_do_dont_resp_wrapper()[i])
    {
      if (((unsigned int) i) <= 39)
        printf("resp DO_DONT %s: %d\n", get_telopts_wrapper()[i - 0], get_do_dont_resp_wrapper()[i]);
      else
        if ((((unsigned int) i) <= 255) && (((unsigned int) i) >= 236))
        printf("resp DO_DONT %s: %d\n", telcmds[i - 236], get_do_dont_resp_wrapper()[i]);
      else
        printf("resp DO_DONT %d: %d\n", i, get_do_dont_resp_wrapper()[i]);
      if (get_options_wrapper()[i] & 0x08)
      {
        if (((unsigned int) i) <= 39)
          printf("want DO   %s\n", get_telopts_wrapper()[i - 0]);
        else
          if ((((unsigned int) i) <= 255) && (((unsigned int) i) >= 236))
          printf("want DO   %s\n", telcmds[i - 236]);
        else
          printf("want DO   %d\n", i);
      }
      else
      {
        if (((unsigned int) i) <= 39)
          printf("want DONT %s\n", get_telopts_wrapper()[i - 0]);
        else
          if ((((unsigned int) i) <= 255) && (((unsigned int) i) >= 236))
          printf("want DONT %s\n", telcmds[i - 236]);
        else
          printf("want DONT %d\n", i);
      }
    }
    else
    {
      if (get_options_wrapper()[i] & 0x04)
      {
        if (((unsigned int) i) <= 39)
          printf("     DO   %s\n", get_telopts_wrapper()[i - 0]);
        else
          if ((((unsigned int) i) <= 255) && (((unsigned int) i) >= 236))
          printf("     DO   %s\n", telcmds[i - 236]);
        else
          printf("     DO   %d\n", i);
      }
    }
    if (get_will_wont_resp_wrapper()[i])
    {
      if (((unsigned int) i) <= 39)
        printf("resp WILL_WONT %s: %d\n", get_telopts_wrapper()[i - 0], get_will_wont_resp_wrapper()[i]);
      else
        if ((((unsigned int) i) <= 255) && (((unsigned int) i) >= 236))
        printf("resp WILL_WONT %s: %d\n", telcmds[i - 236], get_will_wont_resp_wrapper()[i]);
      else
        printf("resp WILL_WONT %d: %d\n", i, get_will_wont_resp_wrapper()[i]);
      if (get_options_wrapper()[i] & 0x02)
      {
        if (((unsigned int) i) <= 39)
          printf("want WILL %s\n", get_telopts_wrapper()[i - 0]);
        else
          if ((((unsigned int) i) <= 255) && (((unsigned int) i) >= 236))
          printf("want WILL %s\n", telcmds[i - 236]);
        else
          printf("want WILL %d\n", i);
      }
      else
      {
        if (((unsigned int) i) <= 39)
          printf("want WONT %s\n", get_telopts_wrapper()[i - 0]);
        else
          if ((((unsigned int) i) <= 255) && (((unsigned int) i) >= 236))
          printf("want WONT %s\n", telcmds[i - 236]);
        else
          printf("want WONT %d\n", i);
      }
    }
    else
    {
      if (get_options_wrapper()[i] & 0x01)
      {
        if (((unsigned int) i) <= 39)
          printf("     WILL %s\n", get_telopts_wrapper()[i - 0]);
        else
          if ((((unsigned int) i) <= 255) && (((unsigned int) i) >= 236))
          printf("     WILL %s\n", telcmds[i - 236]);
        else
          printf("     WILL %d\n", i);
      }
    }
  }

}



/* char direction; '<' or '>' */
/* unsigned char *pointer; where suboption data sits */
/* int		  length; length of suboption data */
void printsub(char direction, unsigned char *pointer, int length)
{
  register int i;
  extern int want_status_response;
  if ((showoptions || (direction == 0)) || (get_want_status_response_wrapper() && (pointer[0] == 5)))
  {
    if (direction)
    {
      fprintf(get_NetTrace_wrapper(), "%s IAC SB ", (direction == '<') ? ("RCVD") : ("SENT"));
      if (length >= 3)
      {
        register int j;
        i = pointer[length - 2];
        j = pointer[length - 1];
        if ((i != 255) || (j != 240))
        {
          fprintf(get_NetTrace_wrapper(), "(terminated by ");
          if (((unsigned int) i) <= 39)
            fprintf(get_NetTrace_wrapper(), "%s ", get_telopts_wrapper()[i - 0]);
          else
            if ((((unsigned int) i) <= 255) && (((unsigned int) i) >= 236))
            fprintf(get_NetTrace_wrapper(), "%s ", telcmds[i - 236]);
          else
            fprintf(get_NetTrace_wrapper(), "%d ", i);
          if (((unsigned int) j) <= 39)
            fprintf(get_NetTrace_wrapper(), "%s", get_telopts_wrapper()[j - 0]);
          else
            if ((((unsigned int) j) <= 255) && (((unsigned int) j) >= 236))
            fprintf(get_NetTrace_wrapper(), "%s", telcmds[j - 236]);
          else
            fprintf(get_NetTrace_wrapper(), "%d", j);
          fprintf(get_NetTrace_wrapper(), ", not IAC SE!) ");
        }
      }
      length -= 2;
    }
    if (length < 1)
    {
      fprintf(get_NetTrace_wrapper(), "(Empty suboption??\?)");
      if (get_NetTrace_wrapper() == stdout)
        fflush(get_NetTrace_wrapper());
      return;
    }
    switch (pointer[0])
    {
      case 24:
        fprintf(get_NetTrace_wrapper(), "TERMINAL-TYPE ");
        switch (pointer[1])
      {
        case 0:
          fprintf(get_NetTrace_wrapper(), "IS \"%.*s\"", length - 2, ((char *) pointer) + 2);
          break;

        case 1:
          fprintf(get_NetTrace_wrapper(), "SEND");
          break;

        default:
          fprintf(get_NetTrace_wrapper(), "- unknown qualifier %d (0x%x).", pointer[1], pointer[1]);

      }

        break;

      case 32:
        fprintf(get_NetTrace_wrapper(), "TERMINAL-SPEED");
        if (length < 2)
      {
        fprintf(get_NetTrace_wrapper(), " (empty suboption??\?)");
        break;
      }
        switch (pointer[1])
      {
        case 0:
          fprintf(get_NetTrace_wrapper(), " IS ");
          fprintf(get_NetTrace_wrapper(), "%.*s", length - 2, ((char *) pointer) + 2);
          break;

        default:
          if (pointer[1] == 1)
          fprintf(get_NetTrace_wrapper(), " SEND");
        else
          fprintf(get_NetTrace_wrapper(), " %d (unknown)", pointer[1]);
          for (i = 2; i < length; i++)
          fprintf(get_NetTrace_wrapper(), " ?%d?", pointer[i]);

          break;

      }

        break;

      case 33:
        fprintf(get_NetTrace_wrapper(), "TOGGLE-FLOW-CONTROL");
        if (length < 2)
      {
        fprintf(get_NetTrace_wrapper(), " (empty suboption??\?)");
        break;
      }
        switch (pointer[1])
      {
        case 0:
          fprintf(get_NetTrace_wrapper(), " OFF");
          break;

        case 1:
          fprintf(get_NetTrace_wrapper(), " ON");
          break;

        case 2:
          fprintf(get_NetTrace_wrapper(), " RESTART-ANY");
          break;

        case 3:
          fprintf(get_NetTrace_wrapper(), " RESTART-XON");
          break;

        default:
          fprintf(get_NetTrace_wrapper(), " %d (unknown)", pointer[1]);

      }

        for (i = 2; i < length; i++)
        fprintf(get_NetTrace_wrapper(), " ?%d?", pointer[i]);

        break;

      case 31:
        fprintf(get_NetTrace_wrapper(), "NAWS");
        if (length < 2)
      {
        fprintf(get_NetTrace_wrapper(), " (empty suboption??\?)");
        break;
      }
        if (length == 2)
      {
        fprintf(get_NetTrace_wrapper(), " ?%d?", pointer[1]);
        break;
      }
        fprintf(get_NetTrace_wrapper(), " %d %d (%d)", pointer[1], pointer[2], (int) ((((unsigned int) pointer[1]) << 8) | ((unsigned int) pointer[2])));
        if (length == 4)
      {
        fprintf(get_NetTrace_wrapper(), " ?%d?", pointer[3]);
        break;
      }
        fprintf(get_NetTrace_wrapper(), " %d %d (%d)", pointer[3], pointer[4], (int) ((((unsigned int) pointer[3]) << 8) | ((unsigned int) pointer[4])));
        for (i = 5; i < length; i++)
        fprintf(get_NetTrace_wrapper(), " ?%d?", pointer[i]);

        break;

      case 34:
        fprintf(get_NetTrace_wrapper(), "LINEMODE ");
        if (length < 2)
      {
        fprintf(get_NetTrace_wrapper(), " (empty suboption??\?)");
        break;
      }
        switch (pointer[1])
      {
        case 251:
          fprintf(get_NetTrace_wrapper(), "WILL ");
          goto common;

        case 252:
          fprintf(get_NetTrace_wrapper(), "WONT ");
          goto common;

        case 253:
          fprintf(get_NetTrace_wrapper(), "DO ");
          goto common;

        case 254:
          fprintf(get_NetTrace_wrapper(), "DONT ");
          common:
        if (length < 3)
        {
          fprintf(get_NetTrace_wrapper(), "(no option??\?)");
          break;
        }

          switch (pointer[2])
        {
          case 2:
            fprintf(get_NetTrace_wrapper(), "Forward Mask");
            for (i = 3; i < length; i++)
            fprintf(get_NetTrace_wrapper(), " %x", pointer[i]);

            break;

          default:
            fprintf(get_NetTrace_wrapper(), "%d (unknown)", pointer[2]);
            for (i = 3; i < length; i++)
            fprintf(get_NetTrace_wrapper(), " %d", pointer[i]);

            break;

        }

          break;

        case 3:
          fprintf(get_NetTrace_wrapper(), "SLC");
          for (i = 2; i < (length - 2); i += 3)
        {
          if (((unsigned int) pointer[i + 0]) <= 18)
            fprintf(get_NetTrace_wrapper(), " %s", slc_names[pointer[i + 0]]);
          else
            fprintf(get_NetTrace_wrapper(), " %d", pointer[i + 0]);
          switch (pointer[i + 1] & 0x03)
          {
            case 0:
              fprintf(get_NetTrace_wrapper(), " NOSUPPORT");
              break;

            case 1:
              fprintf(get_NetTrace_wrapper(), " CANTCHANGE");
              break;

            case 2:
              fprintf(get_NetTrace_wrapper(), " VARIABLE");
              break;

            case 3:
              fprintf(get_NetTrace_wrapper(), " DEFAULT");
              break;

          }

          fprintf(get_NetTrace_wrapper(), "%s%s%s", (pointer[i + 1] & 0x80) ? ("|ACK") : (""), (pointer[i + 1] & 0x40) ? ("|FLUSHIN") : (""), (pointer[i + 1] & 0x20) ? ("|FLUSHOUT") : (""));
          if (pointer[i + 1] & (~(((0x80 | 0x40) | 0x20) | 0x03)))
            fprintf(get_NetTrace_wrapper(), "(0x%x)", pointer[i + 1]);
          fprintf(get_NetTrace_wrapper(), " %d;", pointer[i + 2]);
          if ((pointer[i + 2] == 255) && (pointer[(i + 2) + 1] == 255))
            i++;
        }

          for (; i < length; i++)
          fprintf(get_NetTrace_wrapper(), " ?%d?", pointer[i]);

          break;

        case 1:
          fprintf(get_NetTrace_wrapper(), "MODE ");
          if (length < 3)
        {
          fprintf(get_NetTrace_wrapper(), "(no mode??\?)");
          break;
        }
        {
          char tbuf[64];
          sprintf(tbuf, "%s%s%s%s%s", (pointer[2] & 0x01) ? ("|EDIT") : (""), (pointer[2] & 0x02) ? ("|TRAPSIG") : (""), (pointer[2] & 0x08) ? ("|SOFT_TAB") : (""), (pointer[2] & 0x10) ? ("|LIT_ECHO") : (""), (pointer[2] & 0x04) ? ("|ACK") : (""));
          fprintf(get_NetTrace_wrapper(), "%s", (tbuf[0]) ? (&tbuf[1]) : ("0"));
        }
          if (pointer[2] & (~0x1f))
          fprintf(get_NetTrace_wrapper(), " (0x%x)", pointer[2]);
          for (i = 3; i < length; i++)
          fprintf(get_NetTrace_wrapper(), " ?0x%x?", pointer[i]);

          break;

        default:
          fprintf(get_NetTrace_wrapper(), "%d (unknown)", pointer[1]);
          for (i = 2; i < length; i++)
          fprintf(get_NetTrace_wrapper(), " %d", pointer[i]);


      }

        break;

      case 5:
      {
        register char *cp;
        register int j;
        register int k;
        fprintf(get_NetTrace_wrapper(), "STATUS");
        switch (pointer[1])
        {
          default:
            if (pointer[1] == 1)
            fprintf(get_NetTrace_wrapper(), " SEND");
          else
            fprintf(get_NetTrace_wrapper(), " %d (unknown)", pointer[1]);
            for (i = 2; i < length; i++)
            fprintf(get_NetTrace_wrapper(), " ?%d?", pointer[i]);

            break;

          case 0:
            if (gsetter_want_status_response_prefix_wrapper() < 0)
            set_want_status_response_wrapper(0);
            if (get_NetTrace_wrapper() == stdout)
            fprintf(get_NetTrace_wrapper(), " IS\r\n");
          else
            fprintf(get_NetTrace_wrapper(), " IS\n");
            for (i = 2; i < length; i++)
          {
            switch (pointer[i])
            {
              case 253:
                cp = "DO";
                goto common2;

              case 254:
                cp = "DONT";
                goto common2;

              case 251:
                cp = "WILL";
                goto common2;

              case 252:
                cp = "WONT";
                goto common2;
                common2:
              i++;

                if (((unsigned int) ((int) pointer[i])) <= 39)
                fprintf(get_NetTrace_wrapper(), " %s %s", cp, get_telopts_wrapper()[pointer[i] - 0]);
              else
                fprintf(get_NetTrace_wrapper(), " %s %d", cp, pointer[i]);
                if (get_NetTrace_wrapper() == stdout)
                fprintf(get_NetTrace_wrapper(), "\r\n");
              else
                fprintf(get_NetTrace_wrapper(), "\n");
                break;

              case 250:
                fprintf(get_NetTrace_wrapper(), " SB ");
                i++;
                j = (k = i);
                while (j < length)
              {
                if (pointer[j] == 240)
                {
                  if ((j + 1) == length)
                    break;
                  if (pointer[j + 1] == 240)
                    j++;
                  else
                    break;
                }
                pointer[k++] = pointer[j++];
              }

                printsub(0, &pointer[i], k - i);
                if (i < length)
              {
                fprintf(get_NetTrace_wrapper(), " SE");
                i = j;
              }
              else
                i = j - 1;
                if (get_NetTrace_wrapper() == stdout)
                fprintf(get_NetTrace_wrapper(), "\r\n");
              else
                fprintf(get_NetTrace_wrapper(), "\n");
                break;

              default:
                fprintf(get_NetTrace_wrapper(), " %d", pointer[i]);
                break;

            }

          }

            break;

        }

        break;
      }

      case 35:
        fprintf(get_NetTrace_wrapper(), "X-DISPLAY-LOCATION ");
        switch (pointer[1])
      {
        case 0:
          fprintf(get_NetTrace_wrapper(), "IS \"%.*s\"", length - 2, ((char *) pointer) + 2);
          break;

        case 1:
          fprintf(get_NetTrace_wrapper(), "SEND");
          break;

        default:
          fprintf(get_NetTrace_wrapper(), "- unknown qualifier %d (0x%x).", pointer[1], pointer[1]);

      }

        break;

      case 39:
        fprintf(get_NetTrace_wrapper(), "NEW-ENVIRON ");
        switch (pointer[1])
      {
        case 0:
          fprintf(get_NetTrace_wrapper(), "IS ");
          goto env_common;

        case 1:
          fprintf(get_NetTrace_wrapper(), "SEND ");
          goto env_common;

        case 2:
          fprintf(get_NetTrace_wrapper(), "INFO ");
          env_common:
        {
          const char *quote = "";
          for (i = 2; i < length; i++)
          {
            switch (pointer[i])
            {
              case 1:
                fprintf(get_NetTrace_wrapper(), "%sVALUE ", quote);
                quote = "";
                break;

              case 0:
                fprintf(get_NetTrace_wrapper(), "%sVAR ", quote);
                quote = "";
                break;

              case 2:
                fprintf(get_NetTrace_wrapper(), "%sESC ", quote);
                quote = "";
                break;

              case 3:
                fprintf(get_NetTrace_wrapper(), "%sUSERVAR ", quote);
                quote = "";
                break;

              default:
                if (isprint(pointer[i]) && (pointer[i] != '"'))
              {
                if (quote[0] == '\0')
                {
                  putc('"', get_NetTrace_wrapper());
                  quote = "\" ";
                }
                putc(pointer[i], get_NetTrace_wrapper());
              }
              else
              {
                fprintf(get_NetTrace_wrapper(), "%s%03o ", quote, pointer[i]);
                quote = "";
              }
                break;

            }

          }

          if (quote[0] != '\0')
            putc('"', get_NetTrace_wrapper());
          break;
        }


      }

        break;

      default:
        if (((unsigned int) pointer[0]) <= 39)
        fprintf(get_NetTrace_wrapper(), "%s (unknown)", get_telopts_wrapper()[pointer[0] - 0]);
      else
        fprintf(get_NetTrace_wrapper(), "%d (unknown)", pointer[0]);
        for (i = 1; i < length; i++)
        fprintf(get_NetTrace_wrapper(), " %d", pointer[i]);

        break;

    }

    if (direction)
    {
      if (get_NetTrace_wrapper() == stdout)
        fprintf(get_NetTrace_wrapper(), "\r\n");
      else
        fprintf(get_NetTrace_wrapper(), "\n");
    }
    if (get_NetTrace_wrapper() == stdout)
      fflush(get_NetTrace_wrapper());
  }
}



/* EmptyTerminal - called to make sure that the terminal buffer is empty.
 *			Note that we consider the buffer to run all the
 *			way to the kernel (thus the select).
 */

void EmptyTerminal(void)
{
  fd_set o;
  FD_ZERO(&o);
  {
    Ring temp_ttyoring = get_ttyoring_wrapper();
    int temp_result_18 = ring_full_count(&temp_ttyoring);
    set_ttyoring_wrapper(temp_ttyoring);
    if (temp_result_18 == 0)
    {
      FD_SET(get_tout_wrapper(), &o);
      select(get_tout_wrapper() + 1, (fd_set *) 0, &o, (fd_set *) 0, (struct timeval *) 0);
    }
    else
    {
      {
        Ring temp_ttyoring = get_ttyoring_wrapper();
        int temp_result_19 = ring_full_count(&temp_ttyoring);
        set_ttyoring_wrapper(temp_ttyoring);
        while (temp_result_19)
        {
          ttyflush(0);
          FD_SET(get_tout_wrapper(), &o);
          select(get_tout_wrapper() + 1, (fd_set *) 0, &o, (fd_set *) 0, (struct timeval *) 0);
          temp_ttyoring = get_ttyoring_wrapper();
          temp_result_19 = ring_full_count(&temp_ttyoring);
          set_ttyoring_wrapper(temp_ttyoring);
        }

      }
    }
  }
}



void SetForExit(void)
{
  setconnmode(0);
  {
    Ring temp_netiring;
    int temp_result_20;
    do
    {
      telrcv();
      EmptyTerminal();
      temp_netiring = get_netiring_wrapper();
      temp_result_20 = ring_full_count(&temp_netiring);
      set_netiring_wrapper(temp_netiring);
    }
    while (temp_result_20);
  }
  setcommandmode();
  fflush(stdout);
  fflush(stderr);
  setconnmode(0);
  EmptyTerminal();
  setcommandmode();
}



void
Exit (int returnCode)
{
  SetForExit ();
  exit (returnCode);
}

void
ExitString (char *string, int returnCode)
{
  SetForExit ();
  fwrite (string, 1, strlen (string), stderr);
  exit (returnCode);
}
