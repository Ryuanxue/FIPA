/* Parsing FTP `ls' output.
   Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
   2005, 2006, 2007, 2008, 2009, 2010, 2011, 2015 Free Software
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
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "utils.h"
#include "ftp.h"
#include "url.h"
#include "convert.h"            /* for html_quote_string prototype */
#include "retr.h"               /* for output_stream */
#include "c-strcase.h"

#include "wget_rpc_wrapper.h"

/* Converts symbolic permissions to number-style ones, e.g. string
   rwxr-xr-x to 755.  For now, it knows nothing of
   setuid/setgid/sticky.  ACLs are ignored.  */
static int
symperms (const char *s)
{
  int perms = 0, i;

  if (strlen (s) < 9)
    return 0;
  for (i = 0; i < 3; i++, s += 3)
    {
      perms <<= 3;
      perms += (((s[0] == 'r') << 2) + ((s[1] == 'w') << 1) +
                (s[2] == 'x' || s[2] == 's'));
    }
  return perms;
}


/* Cleans a line of text so that it can be consistently parsed. Destroys
   <CR> and <LF> in case that thay occur at the end of the line and
   replaces all <TAB> character with <SPACE>. Returns the length of the
   modified line. */
static int
clean_line (char *line, int len)
{
  if (len <= 0) return 0;

  while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
    line[--len] = '\0';

  if (!len) return 0;

  for ( ; *line ; line++ ) if (*line == '\t') *line = ' ';

  return len;
}

/* Convert the Un*x-ish style directory listing stored in FILE to a
   linked list of fileinfo (system-independent) entries.  The contents
   of FILE are considered to be produced by the standard Unix `ls -la'
   output (whatever that might be).  BSD (no group) and SYSV (with
   group) listings are handled.

   The time stamps are stored in a separate variable, time_t
   compatible (I hope).  The timezones are ignored.  */
static struct fileinfo *ftp_parse_unix_ls(const char *file, int ignore_perms)
{
  FILE *fp;
  static const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  int next;
  int len;
  int i;
  int error;
  int ignore;
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec;
  int ptype;
  struct tm timestruct;
  struct tm *tnow;
  time_t timenow;
  size_t bufsize = 0;
  char *line = 0;
  char *tok;
  char *ptok;
  struct fileinfo *dir;
  struct fileinfo *l;
  struct fileinfo cur;
  fp = fopen(file, "rb");
  if (!fp)
  {
    logprintf(LOG_NOTQUIET, "%s: %s\n", file, strerror(errno));
    return 0;
  }
  dir = (l = 0);
  while ((len = getline(&line, &bufsize, fp)) > 0)
  {
    len = clean_line(line, len);
    if (!c_strncasecmp(line, "total", 5))
      continue;
    tok = strtok(line, " ");
    if (!tok)
      continue;
    cur.name = 0;
    cur.linkto = 0;
    switch (*tok)
    {
      case '-':
        cur.type = FT_PLAINFILE;
        do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("PLAINFILE; ");
        }
      }
      while (0);
        break;

      case 'd':
        cur.type = FT_DIRECTORY;
        do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("DIRECTORY; ");
        }
      }
      while (0);
        break;

      case 'l':
        cur.type = FT_SYMLINK;
        do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("SYMLINK; ");
        }
      }
      while (0);
        break;

      default:
        cur.type = FT_UNKNOWN;
        do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("UNKNOWN; ");
        }
      }
      while (0);
        break;

    }

    if (ignore_perms)
    {
      switch (cur.type)
      {
        case FT_PLAINFILE:
          cur.perms = 0644;
          break;

        case FT_DIRECTORY:
          cur.perms = 0755;
          break;

        default:
          cur.perms = 0644;

      }

      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("implicit perms %0o; ", cur.perms);
        }
      }
      while (0);
    }
    else
    {
      cur.perms = symperms(tok + 1);
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("perms %0o; ", cur.perms);
        }
      }
      while (0);
    }
    error = (ignore = 0);
    year = (hour = (min = (sec = 0)));
    month = (day = 0);
    ptype = TT_DAY;
    next = -1;
    tok = line;
    while (ptok = tok, (tok = strtok(0, " ")) != 0)
    {
      --next;
      if (next < 0)
      {
        for (i = 0; i < 12; i++)
          if (!c_strcasecmp(tok, months[i]))
          break;

        if (i != 12)
        {
          wgint size;
          if (ptok == line)
          {
            error = 1;
            break;
          }
          errno = 0;
          size = strtol(ptok, 0, 10);
          if ((size == ((wgint) ((!(!(((wgint) 0) < ((wgint) (-1))))) ? ((wgint) (-1)) : ((((((wgint) 1) << (((sizeof(wgint)) * 8) - 2)) - 1) * 2) + 1)))) && (errno == ERANGE))
            cur.size = 0;
          else
            cur.size = size;
          do
          {
            if (__builtin_expect(get_opt_debug_wrapper(), 0))
            {
              debug_logprintf("size: %s; ", number_to_static_string(cur.size));
            }
          }
          while (0);
          month = i;
          next = 5;
          do
          {
            if (__builtin_expect(get_opt_debug_wrapper(), 0))
            {
              debug_logprintf("month: %s; ", months[month]);
            }
          }
          while (0);
        }
      }
      else
        if (next == 4)
      {
        if (tok[1])
          day = ((10 * ((*tok) - '0')) + tok[1]) - '0';
        else
          day = (*tok) - '0';
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("day: %d; ", day);
          }
        }
        while (0);
      }
      else
        if (next == 3)
      {
        year = 0;
        min = (hour = (sec = 0));
        if (c_isdigit(*tok))
        {
          for (; c_isdigit(*tok); tok++)
            year = ((*tok) - '0') + (10 * year);

          if ((*tok) == ':')
          {
            hour = year;
            year = 0;
            ptype = TT_HOUR_MIN;
            ++tok;
            for (; c_isdigit(*tok); tok++)
              min = ((*tok) - '0') + (10 * min);

            if ((*tok) == ':')
            {
              ++tok;
              for (; c_isdigit(*tok); tok++)
                sec = ((*tok) - '0') + (10 * sec);

            }
          }
        }
        if (year)
          do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("year: %d (no tm); ", year);
          }
        }
        while (0);
        else
          do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("time: %02d:%02d:%02d (no yr); ", hour, min, sec);
          }
        }
        while (0);
      }
      else
        if (next == 2)
      {
        int fnlen;
        char *p;
        fnlen = strlen(tok);
        if (fnlen < (len - (tok - line)))
        {
          tok[fnlen] = ' ';
          if (cur.type == FT_SYMLINK)
          {
            p = strstr(tok, " -> ");
            if (!p)
            {
              error = 1;
              break;
            }
            cur.linkto = xstrdup(p + 4);
            do
            {
              if (__builtin_expect(get_opt_debug_wrapper(), 0))
              {
                debug_logprintf("link to: %s\n", cur.linkto);
              }
            }
            while (0);
            *p = '\0';
          }
        }
        if ((!strcmp(tok, ".")) || (!strcmp(tok, "..")))
        {
          do
          {
            if (__builtin_expect(get_opt_debug_wrapper(), 0))
            {
              debug_logprintf("\nIgnoring `.' and `..'; ");
            }
          }
          while (0);
          ignore = 1;
          break;
        }
        fnlen = strlen(tok);
        cur.name = xmalloc(fnlen + 1);
        memcpy(cur.name, tok, fnlen + 1);
        if (fnlen)
        {
          if ((cur.type == FT_DIRECTORY) && (cur.name[fnlen - 1] == '/'))
          {
            cur.name[fnlen - 1] = '\0';
            do
            {
              if (__builtin_expect(get_opt_debug_wrapper(), 0))
              {
                debug_logprintf("trailing `/' on dir.\n");
              }
            }
            while (0);
          }
          else
            if ((cur.type == FT_SYMLINK) && (cur.name[fnlen - 1] == '@'))
          {
            cur.name[fnlen - 1] = '\0';
            do
            {
              if (__builtin_expect(get_opt_debug_wrapper(), 0))
              {
                debug_logprintf("trailing `@' on link.\n");
              }
            }
            while (0);
          }
          else
            if (((cur.type == FT_PLAINFILE) && (cur.perms & 0111)) && (cur.name[fnlen - 1] == '*'))
          {
            cur.name[fnlen - 1] = '\0';
            do
            {
              if (__builtin_expect(get_opt_debug_wrapper(), 0))
              {
                debug_logprintf("trailing `*' on exec.\n");
              }
            }
            while (0);
          }
        }
        else
          error = 1;
        break;
      }
      else
        abort();
    }

    if ((!cur.name) || ((cur.type == FT_SYMLINK) && (!cur.linkto)))
      error = 1;
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("%s\n", (cur.name) ? (cur.name) : (""));
      }
    }
    while (0);
    if (error || ignore)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Skipping.\n");
        }
      }
      while (0);
      do
      {
        free((void *) cur.name);
        cur.name = 0;
      }
      while (0);
      do
      {
        free((void *) cur.linkto);
        cur.linkto = 0;
      }
      while (0);
      continue;
    }
    if (!dir)
    {
      l = (dir = xmalloc(sizeof(struct fileinfo)));
      memcpy(l, &cur, sizeof(cur));
      l->prev = (l->next = 0);
    }
    else
    {
      cur.prev = l;
      l->next = xmalloc(sizeof(struct fileinfo));
      l = l->next;
      memcpy(l, &cur, sizeof(cur));
      l->next = 0;
    }
    timenow = time(0);
    tnow = localtime(&timenow);
    timestruct.tm_sec = sec;
    timestruct.tm_min = min;
    timestruct.tm_hour = hour;
    timestruct.tm_mday = day;
    timestruct.tm_mon = month;
    if (year == 0)
    {
      if (month > tnow->tm_mon)
        timestruct.tm_year = tnow->tm_year - 1;
      else
        timestruct.tm_year = tnow->tm_year;
    }
    else
      timestruct.tm_year = year;
    if (timestruct.tm_year >= 1900)
      timestruct.tm_year -= 1900;
    timestruct.tm_wday = 0;
    timestruct.tm_yday = 0;
    timestruct.tm_isdst = -1;
    l->tstamp = mktime(&timestruct);
    l->ptype = ptype;
  }

  do
  {
    free((void *) line);
    line = 0;
  }
  while (0);
  fclose(fp);
  return dir;
}



static struct fileinfo *ftp_parse_winnt_ls(const char *file)
{
  FILE *fp;
  int len;
  int year;
  int month;
  int day;
  int hour;
  int min;
  size_t bufsize = 0;
  struct tm timestruct;
  char *line = 0;
  char *tok;
  char *filename;
  struct fileinfo *dir;
  struct fileinfo *l;
  struct fileinfo cur;
  fp = fopen(file, "rb");
  if (!fp)
  {
    logprintf(LOG_NOTQUIET, "%s: %s\n", file, strerror(errno));
    return 0;
  }
  dir = (l = 0);
  while ((len = getline(&line, &bufsize, fp)) > 0)
  {
    len = clean_line(line, len);
    if (len < 40)
      continue;
    filename = line + 39;
    tok = strtok(line, "-");
    if (tok == 0)
      continue;
    month = atoi(tok) - 1;
    if (month < 0)
      month = 0;
    tok = strtok(0, "-");
    if (tok == 0)
      continue;
    day = atoi(tok);
    tok = strtok(0, " ");
    if (tok == 0)
      continue;
    year = atoi(tok);
    if (year <= 70)
    {
      year += 100;
    }
    else
      if (year >= 1900)
    {
      year -= 1900;
      filename += 2;
    }
    cur.name = xstrdup(filename);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Name: '%s'\n", cur.name);
      }
    }
    while (0);
    tok = strtok(0, ":");
    if (tok == 0)
      continue;
    hour = atoi(tok);
    tok = strtok(0, "M");
    if (tok == 0)
      continue;
    min = atoi(tok);
    tok += 2;
    if (hour == 12)
      hour = 0;
    if ((*tok) == 'P')
      hour += 12;
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("YYYY/MM/DD HH:MM - %d/%02d/%02d %02d:%02d\n", year + 1900, month, day, hour, min);
      }
    }
    while (0);
    timestruct.tm_sec = 0;
    timestruct.tm_min = min;
    timestruct.tm_hour = hour;
    timestruct.tm_mday = day;
    timestruct.tm_mon = month;
    timestruct.tm_year = year;
    timestruct.tm_wday = 0;
    timestruct.tm_yday = 0;
    timestruct.tm_isdst = -1;
    cur.tstamp = mktime(&timestruct);
    cur.ptype = TT_HOUR_MIN;
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Timestamp: %ld\n", cur.tstamp);
      }
    }
    while (0);
    tok = strtok(0, " ");
    if (tok == 0)
      continue;
    while ((tok != 0) && ((*tok) == '\0'))
      tok = strtok(0, " ");

    if (tok == 0)
      continue;
    if ((*tok) == '<')
    {
      cur.type = FT_DIRECTORY;
      cur.size = 0;
      cur.perms = 0755;
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Directory\n");
        }
      }
      while (0);
    }
    else
    {
      wgint size;
      cur.type = FT_PLAINFILE;
      errno = 0;
      size = strtol(tok, 0, 10);
      if ((size == ((wgint) ((!(!(((wgint) 0) < ((wgint) (-1))))) ? ((wgint) (-1)) : ((((((wgint) 1) << (((sizeof(wgint)) * 8) - 2)) - 1) * 2) + 1)))) && (errno == ERANGE))
        cur.size = 0;
      else
        cur.size = size;
      cur.perms = 0644;
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("File, size %s bytes\n", number_to_static_string(cur.size));
        }
      }
      while (0);
    }
    cur.linkto = 0;
    if (!dir)
    {
      l = (dir = xmalloc(sizeof(struct fileinfo)));
      memcpy(l, &cur, sizeof(cur));
      l->prev = (l->next = 0);
    }
    else
    {
      cur.prev = l;
      l->next = xmalloc(sizeof(struct fileinfo));
      l = l->next;
      memcpy(l, &cur, sizeof(cur));
      l->next = 0;
    }
  }

  do
  {
    free((void *) line);
    line = 0;
  }
  while (0);
  fclose(fp);
  return dir;
}





/* Convert the VMS-style directory listing stored in "file" to a
   linked list of fileinfo (system-independent) entries.  The contents
   of FILE are considered to be produced by the standard VMS
   "DIRECTORY [/SIZE [= ALL]] /DATE [/OWNER] [/PROTECTION]" command,
   more or less.  (Different VMS FTP servers may have different headers,
   and may not supply the same data, but all should be subsets of this.)

   VMS normally provides local (server) time and date information.
   Define the logical name or environment variable
   "WGET_TIMEZONE_DIFFERENTIAL" (seconds) to adjust the receiving local
   times if different from the remote local times.

   2005-02-23 SMS.
   Added code to eliminate "^" escape characters from ODS5 extended file
   names.  The TCPIP FTP server (V5.4) seems to prefer requests which do
   not use the escaped names which it provides.
*/

#define VMS_DEFAULT_PROT_FILE 0644
#define VMS_DEFAULT_PROT_DIR 0755

/* 2005-02-23 SMS.
   eat_carets().

   Delete ODS5 extended file name escape characters ("^") in the
   original buffer.
   Note that the current scheme does not handle all EFN cases, but it
   could be made more complicated.
*/

static void eat_carets( char *str)
/* char *str;      Source pointer. */
{
  char *strd;   /* Destination pointer. */
  char hdgt;
  unsigned char uchr;
  unsigned char prop;

  /* Skip ahead to the first "^", if any. */
  while ((*str != '\0') && (*str != '^'))
     str++;

  /* If no caret was found, quit early. */
  if (*str != '\0')
  {
    /* Shift characters leftward as carets are found. */
    strd = str;
    while (*str != '\0')
    {
      uchr = *str;
      if (uchr == '^')
      {
        /* Found a caret.  Skip it, and check the next character. */
        uchr = *(++str);
        prop = char_prop[ uchr];
        if (prop& 64)
        {
          /* Hex digit.  Get char code from this and next hex digit. */
          if (uchr <= '9')
          {
            hdgt = uchr- '0';           /* '0' - '9' -> 0 - 9. */
          }
          else
          {
            hdgt = ((uchr- 'A')& 7)+ 10;    /* [Aa] - [Ff] -> 10 - 15. */
          }
          hdgt <<= 4;                   /* X16. */
          uchr = *(++str);              /* Next char must be hex digit. */
          if (uchr <= '9')
          {
            uchr = hdgt+ uchr- '0';
          }
          else
          {
            uchr = hdgt+ ((uchr- 'A')& 15)+ 10;
          }
        }
        else if (uchr == '_')
        {
          /* Convert escaped "_" to " ". */
          uchr = ' ';
        }
        else if (uchr == '/')
        {
          /* Convert escaped "/" (invalid Zip) to "?" (invalid VMS). */
          /* Note that this is a left-over from Info-ZIP code, and is
             probably of little value here, except perhaps to avoid
             directory confusion which an unconverted slash might cause.
          */
          uchr = '?';
        }
        /* Else, not a hex digit.  Must be a simple escaped character
           (or Unicode, which is not yet handled here).
        */
      }
      /* Else, not a caret.  Use as-is. */
      *strd = uchr;

      /* Advance destination and source pointers. */
      strd++;
      str++;
    }
    /* Terminate the destination string. */
    *strd = '\0';
  }
}


static struct fileinfo *ftp_parse_vms_ls(const char *file)
{
  FILE *fp;
  int dt;
  int i;
  int j;
  int len;
  int perms;
  size_t bufsize = 0;
  time_t timenow;
  struct tm *timestruct;
  char date_str[32];
  char *line = 0;
  char *tok;
  struct fileinfo *dir;
  struct fileinfo *l;
  struct fileinfo cur;
  fp = fopen(file, "r");
  if (!fp)
  {
    logprintf(LOG_NOTQUIET, "%s: %s\n", file, strerror(errno));
    return 0;
  }
  dir = (l = 0);
  for (j = 0; (i = getline(&line, &bufsize, fp)) > 0;)
  {
    i = clean_line(line, i);
    if (i <= 0)
      continue;
    if ((j == 0) && (line[i - 1] == ']'))
    {
      j = 1;
    }
    else
      if (!strncmp(line, "Total of ", 9))
    {
      i = 0;
      break;
    }
    else
    {
      break;
    }
  }

  while (i > 0)
  {
    char *p;
    tok = strtok(line, " ");
    if (tok == 0)
      tok = line;
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("file name:   '%s'\n", tok);
      }
    }
    while (0);
    for (p = tok + strlen(tok); ((--p) > tok) && c_isdigit(*p);)
      ;

    if (((*p) == ';') && ((*(p - 1)) != '^'))
    {
      *p = '\0';
    }
    eat_carets(tok);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("file name-^: '%s'\n", tok);
      }
    }
    while (0);
    len = strlen(tok);
    if (!c_strncasecmp(tok + (len - 4), ".DIR", 4))
    {
      *(tok + (len - 4)) = '\0';
      cur.type = FT_DIRECTORY;
      cur.perms = 0755;
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Directory (nv)\n");
        }
      }
      while (0);
    }
    else
      if (!c_strncasecmp(tok + (len - 6), ".DIR;1", 6))
    {
      *(tok + (len - 6)) = '\0';
      cur.type = FT_DIRECTORY;
      cur.perms = 0755;
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Directory (v)\n");
        }
      }
      while (0);
    }
    else
    {
      cur.type = FT_PLAINFILE;
      cur.perms = 0644;
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("File\n");
        }
      }
      while (0);
    }
    cur.name = xstrdup(tok);
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Name: '%s'\n", cur.name);
      }
    }
    while (0);
    *date_str = '\0';
    cur.linkto = 0;
    cur.size = 0;
    tok = strtok(0, " ");
    if (tok == 0)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Getting additional line.\n");
        }
      }
      while (0);
      i = getline(&line, &bufsize, fp);
      if (i <= 0)
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("EOF.  Leaving listing parser.\n");
          }
        }
        while (0);
        break;
      }
      i = clean_line(line, i);
      if (i <= 0)
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Blank line.  Leaving listing parser.\n");
          }
        }
        while (0);
        break;
      }
      else
        if (line[0] != ' ')
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Non-blank in column 1.  Must be a new file name?\n");
          }
        }
        while (0);
        continue;
      }
      else
      {
        tok = strtok(line, " ");
        if (tok == 0)
        {
          do
          {
            if (__builtin_expect(get_opt_debug_wrapper(), 0))
            {
              debug_logprintf("Null token.  Leaving listing parser.\n");
            }
          }
          while (0);
          break;
        }
      }
    }
    while (tok != 0)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Token: >%s<: ", tok);
        }
      }
      while (0);
      if ((strlen(tok) < 12) && (strchr(tok, '-') != 0))
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Date.\n");
          }
        }
        while (0);
        strcpy(date_str, tok);
        strcat(date_str, " ");
      }
      else
        if ((strlen(tok) < 12) && (strchr(tok, ':') != 0))
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Time. ");
          }
        }
        while (0);
        strncat(date_str, tok, ((sizeof(date_str)) - strlen(date_str)) - 1);
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Date time: >%s<\n", date_str);
          }
        }
        while (0);
      }
      else
        if (strchr(tok, '[') != 0)
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Owner.\n");
          }
        }
        while (0);
      }
      else
        if (strchr(tok, '(') != 0)
      {
        perms = 0;
        j = 0;
        for (i = 0; i < ((int) strlen(tok)); i++)
        {
          switch (tok[i])
          {
            case '(':
              break;

            case ')':
              break;

            case ',':
              if (j == 0)
            {
              perms = 0;
              j = 1;
            }
            else
            {
              perms <<= 3;
            }
              break;

            case 'R':
              perms |= 4;
              break;

            case 'W':
              perms |= 2;
              break;

            case 'E':
              perms |= 1;
              break;

            case 'D':
              perms |= 2;
              break;

          }

        }

        cur.perms = perms;
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Prot.  perms = %0o.\n", cur.perms);
          }
        }
        while (0);
      }
      else
      {
        do
        {
          if (__builtin_expect(get_opt_debug_wrapper(), 0))
          {
            debug_logprintf("Ignored (size?).\n");
          }
        }
        while (0);
      }
      tok = strtok(0, " ");
    }

    timenow = time(0);
    timestruct = localtime(&timenow);
    strptime(date_str, "%d-%b-%Y %H:%M:%S", timestruct);
    timenow = mktime(timestruct);
    if ((tok = getenv("WGET_TIMEZONE_DIFFERENTIAL")) != 0)
    {
      dt = atoi(tok);
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Time differential = %d.\n", dt);
        }
      }
      while (0);
    }
    else
      dt = 0;
    if (dt >= 0)
      timenow += dt;
    else
      timenow -= -dt;
    cur.tstamp = timenow;
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Timestamp: %ld\n", cur.tstamp);
      }
    }
    while (0);
    cur.ptype = TT_HOUR_MIN;
    if (!dir)
    {
      l = (dir = (struct fileinfo *) xmalloc(sizeof(struct fileinfo)));
      memcpy(l, &cur, sizeof(cur));
      l->prev = (l->next = 0);
    }
    else
    {
      cur.prev = l;
      l->next = (struct fileinfo *) xmalloc(sizeof(struct fileinfo));
      l = l->next;
      memcpy(l, &cur, sizeof(cur));
      l->next = 0;
    }
    i = getline(&line, &bufsize, fp);
    if (i > 0)
    {
      i = clean_line(line, i);
      if (i <= 0)
      {
        break;
      }
    }
  }

  do
  {
    free((void *) line);
    line = 0;
  }
  while (0);
  fclose(fp);
  return dir;
}




/* This function switches between the correct parsing routine depending on
   the SYSTEM_TYPE. The system type should be based on the result of the
   "SYST" response of the FTP server. According to this repsonse we will
   use on of the three different listing parsers that cover the most of FTP
   servers used nowadays.  */

struct fileinfo *
ftp_parse_ls (const char *file, const enum stype system_type)
{
  switch (system_type)
    {
    case ST_UNIX:
      return ftp_parse_unix_ls (file, 0);
    case ST_WINNT:
      {
        /* Detect whether the listing is simulating the UNIX format */
        FILE *fp;
        int   c;
        fp = fopen (file, "rb");
        if (!fp)
        {
          logprintf (LOG_NOTQUIET, "%s: %s\n", file, strerror (errno));
          return NULL;
        }
        c = fgetc(fp);
        fclose(fp);
        /* If the first character of the file is '0'-'9', it's WINNT
           format. */
        if (c >= '0' && c <='9')
          return ftp_parse_winnt_ls (file);
        else
          return ftp_parse_unix_ls (file, 1);
      }
    case ST_VMS:
      return ftp_parse_vms_ls (file);
    case ST_MACOS:
      return ftp_parse_unix_ls (file, 1);
    default:
      logprintf (LOG_NOTQUIET, _("\
Unsupported listing type, trying Unix listing parser.\n"));
      return ftp_parse_unix_ls (file, 0);
    }
}

/* Stuff for creating FTP index. */

/* The function creates an HTML index containing references to given
   directories and files on the appropriate host.  The references are
   FTP.  */
uerr_t ftp_index(const char *file, struct url *u, struct fileinfo *f)
{
  FILE *fp;
  char *upwd;
  char *htcldir;
  char *htclfile;
  char *urlclfile;
  if (!get_output_stream_wrapper())
  {
    fp = fopen(file, "wb");
    if (!fp)
    {
      logprintf(LOG_NOTQUIET, "%s: %s\n", file, strerror(errno));
      return FOPENERR;
    }
  }
  else
    fp = get_output_stream_wrapper();
  if (u->user)
  {
    char *tmpu;
    char *tmpp;
    tmpu = url_escape(u->user);
    tmpp = (u->passwd) ? (url_escape(u->passwd)) : (0);
    if (tmpp)
      upwd = concat_strings(tmpu, ":", tmpp, "@", (char *) 0);
    else
      upwd = concat_strings(tmpu, "@", (char *) 0);
    do
    {
      free((void *) tmpu);
      tmpu = 0;
    }
    while (0);
    do
    {
      free((void *) tmpp);
      tmpp = 0;
    }
    while (0);
  }
  else
    upwd = xstrdup("");
  htcldir = html_quote_string(u->dir);
  fprintf(fp, "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n");
  fprintf(fp, "<html>\n<head>\n<title>");
  fprintf(fp, gettext("Index of /%s on %s:%d"), htcldir, u->host, u->port);
  fprintf(fp, "</title>\n</head>\n<body>\n<h1>");
  fprintf(fp, gettext("Index of /%s on %s:%d"), htcldir, u->host, u->port);
  fprintf(fp, "</h1>\n<hr>\n<pre>\n");
  while (f)
  {
    fprintf(fp, "  ");
    if (f->tstamp != (-1))
    {
      static const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
      time_t tstamp = f->tstamp;
      struct tm *ptm = localtime(&tstamp);
      fprintf(fp, "%d %s %02d ", ptm->tm_year + 1900, months[ptm->tm_mon], ptm->tm_mday);
      if (f->ptype == TT_HOUR_MIN)
        fprintf(fp, "%02d:%02d  ", ptm->tm_hour, ptm->tm_min);
      else
        fprintf(fp, "       ");
    }
    else
      fprintf(fp, gettext("time unknown       "));
    switch (f->type)
    {
      case FT_PLAINFILE:
        fprintf(fp, gettext("File        "));
        break;

      case FT_DIRECTORY:
        fprintf(fp, gettext("Directory   "));
        break;

      case FT_SYMLINK:
        fprintf(fp, gettext("Link        "));
        break;

      default:
        fprintf(fp, gettext("Not sure    "));
        break;

    }

    htclfile = html_quote_string(f->name);
    urlclfile = url_escape_unsafe_and_reserved(f->name);
    fprintf(fp, "<a href=\"ftp://%s%s:%d", upwd, u->host, u->port);
    if ((*u->dir) != '/')
      putc('/', fp);
    fprintf(fp, "%s", htcldir);
    if (*u->dir)
      putc('/', fp);
    fprintf(fp, "%s", urlclfile);
    if (f->type == FT_DIRECTORY)
      putc('/', fp);
    fprintf(fp, "\">%s", htclfile);
    if (f->type == FT_DIRECTORY)
      putc('/', fp);
    fprintf(fp, "</a> ");
    if (f->type == FT_PLAINFILE)
      fprintf(fp, gettext(" (%s bytes)"), number_to_static_string(f->size));
    else
      if (f->type == FT_SYMLINK)
      fprintf(fp, "-> %s", (f->linkto) ? (f->linkto) : ("(nil)"));
    putc('\n', fp);
    do
    {
      free((void *) htclfile);
      htclfile = 0;
    }
    while (0);
    do
    {
      free((void *) urlclfile);
      urlclfile = 0;
    }
    while (0);
    f = f->next;
  }

  fprintf(fp, "</pre>\n</body>\n</html>\n");
  do
  {
    free((void *) htcldir);
    htcldir = 0;
  }
  while (0);
  do
  {
    free((void *) upwd);
    upwd = 0;
  }
  while (0);
  if (!get_output_stream_wrapper())
    fclose(fp);
  else
    fflush(fp);
  return FTPOK;
}


