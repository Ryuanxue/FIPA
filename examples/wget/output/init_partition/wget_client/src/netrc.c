/* Read and parse the .netrc file to get hosts, accounts, and passwords.
   Copyright (C) 1996, 2007, 2008, 2009, 2010, 2011, 2015 Free Software
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

/* This file used to be kept in synch with the code in Fetchmail, but
   the latter has diverged since.  */

#include "wget.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "netrc.h"
#include "init.h"

#define NETRC_FILE_NAME ".netrc"

static acc_t *netrc_list;

static acc_t *parse_netrc (const char *);

void netrc_cleanup(void)
{
  free_netrc(get_netrc_list_wrapper());
}



/* Return the correct user and password, given the host, user (as
   given in the URL), and password (as given in the URL).  May return
   NULL.

   If SLACK_DEFAULT is set, allow looking for a "default" account.
   You will typically turn it off for HTTP.  */
void search_netrc(const char *host, const char **acc, const char **passwd, int slack_default)
{
  acc_t *l;
  static int processed_netrc;
  if (!get_opt_netrc_wrapper())
    return;
  if (!processed_netrc)
  {
    char *home = home_dir();
    set_netrc_list_wrapper(0);
    processed_netrc = 1;
    if (home)
    {
      int err;
      struct stat buf;
      char *path = (char *) alloca(((strlen(home) + 1) + strlen(".netrc")) + 1);
      sprintf(path, "%s/%s", home, ".netrc");
      do
      {
        free((void *) home);
        home = 0;
      }
      while (0);
      err = stat(path, &buf);
      if (err == 0)
        set_netrc_list_wrapper(parse_netrc(path));
    }
  }
  if (!get_netrc_list_wrapper())
    return;
  if ((*acc) && (*passwd))
    return;
  for (l = get_netrc_list_wrapper(); l; l = l->next)
  {
    if (!l->host)
      continue;
    else
      if (!strcasecmp(l->host, host))
      break;
  }

  if (l)
  {
    if (*acc)
    {
      if (!strcmp(l->acc, *acc))
        *passwd = l->passwd;
      else
        *passwd = 0;
    }
    else
    {
      *acc = l->acc;
      if (l->passwd)
        *passwd = l->passwd;
    }
    return;
  }
  else
  {
    if (!slack_default)
      return;
    if (*acc)
      return;
    for (l = get_netrc_list_wrapper(); l; l = l->next)
      if (!l->host)
      break;

    if (!l)
      return;
    *acc = l->acc;
    if (!(*passwd))
      *passwd = l->passwd;
    return;
  }
}




#ifdef STANDALONE

/* Normally, these functions would be defined by your package.  */
# define xmalloc malloc
# define xfree(p) do { free ((void *) (p)); p = NULL; } while (0)
# define xstrdup strdup

# define xrealloc realloc

#endif /* STANDALONE */

/* Maybe add NEWENTRY to the account information list, LIST.  NEWENTRY is
   set to a ready-to-use acc_t, in any event.  */
static void
maybe_add_to_list (acc_t **newentry, acc_t **list)
{
  acc_t *a, *l;
  a = *newentry;
  l = *list;

  /* We need an account name in order to add the entry to the list.  */
  if (a && ! a->acc)
    {
      /* Free any allocated space.  */
      xfree (a->host);
      xfree (a->acc);
      xfree (a->passwd);
    }
  else
    {
      if (a)
        {
          /* Add the current machine into our list.  */
          a->next = l;
          l = a;
        }

      /* Allocate a new acc_t structure.  */
      a = xmalloc (sizeof (acc_t));
    }

  /* Zero the structure, so that it is ready to use.  */
  memset (a, 0, sizeof(*a));

  /* Return the new pointers.  */
  *newentry = a;
  *list = l;
  return;
}

/* Helper function for the parser, shifts contents of
   null-terminated string once character to the left.
   Used in processing \ and " constructs in the netrc file */
static void
shift_left(char *string)
{
  char *p;

  for (p=string; *p; ++p)
    *p = *(p+1);
}

/* Parse a .netrc file (as described in the ftp(1) manual page).  */
static acc_t *parse_netrc(const char *path)
{
  FILE *fp;
  char *line = 0;
  char *p;
  char *tok;
  const char *premature_token;
  acc_t *current;
  acc_t *retval;
  int ln;
  int qmark;
  size_t bufsize = 0;
  enum 
  {
    tok_nothing,
    tok_account,
    tok_login,
    tok_macdef,
    tok_machine,
    tok_password
  } last_token = tok_nothing;
  current = (retval = 0);
  fp = fopen(path, "r");
  if (!fp)
  {
    fprintf(stderr, gettext("%s: Cannot read %s (%s).\n"), get_exec_name_wrapper(), path, strerror(errno));
    return retval;
  }
  ln = 0;
  premature_token = 0;
  while (getline(&line, &bufsize, fp) > 0)
  {
    ln++;
    p = line;
    qmark = 0;
    while ((*p) && c_isspace(*p))
      p++;

    if ((last_token == tok_macdef) && (!(*p)))
      last_token = tok_nothing;
    while ((*p) && (last_token != tok_macdef))
    {
      while ((*p) && c_isspace(*p))
        p++;

      if (((*p) == '#') || (!(*p)))
        break;
      if ((*p) == '"')
      {
        qmark = 1;
        shift_left(p);
      }
      tok = p;
      while ((*p) && ((qmark) ? ((*p) != '"') : (!c_isspace(*p))))
      {
        if ((*p) == '\\')
          shift_left(p);
        p++;
      }

      if (qmark)
      {
        shift_left(p);
        qmark = 0;
      }
      if (*p)
        *(p++) = '\0';
      switch (last_token)
      {
        case tok_login:
          if (current)
          current->acc = xstrdup(tok);
        else
          premature_token = "login";
          break;

        case tok_machine:
          maybe_add_to_list(&current, &retval);
          current->host = xstrdup(tok);
          break;

        case tok_password:
          if (current)
          current->passwd = xstrdup(tok);
        else
          premature_token = "password";
          break;

        case tok_macdef:
          if (!current)
          premature_token = "macdef";
          break;

        case tok_account:
          if (!current)
          premature_token = "account";
          break;

        case tok_nothing:
          break;

      }

      if (premature_token)
      {
        fprintf(stderr, gettext("%s: %s:%d: warning: %s token appears before any machine name\n"), get_exec_name_wrapper(), path, ln, quote(premature_token));
        premature_token = 0;
      }
      if (last_token != tok_nothing)
        last_token = tok_nothing;
      else
      {
        if (!strcmp(tok, "account"))
          last_token = tok_account;
        else
          if (!strcmp(tok, "default"))
        {
          maybe_add_to_list(&current, &retval);
        }
        else
          if (!strcmp(tok, "login"))
          last_token = tok_login;
        else
          if (!strcmp(tok, "macdef"))
          last_token = tok_macdef;
        else
          if (!strcmp(tok, "machine"))
          last_token = tok_machine;
        else
          if (!strcmp(tok, "password"))
          last_token = tok_password;
        else
          fprintf(stderr, gettext("%s: %s:%d: unknown token \"%s\"\n"), get_exec_name_wrapper(), path, ln, tok);
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
  maybe_add_to_list(&current, &retval);
  do
  {
    free((void *) current);
    current = 0;
  }
  while (0);
  current = retval;
  retval = 0;
  while (current)
  {
    acc_t *saved_reference;
    saved_reference = current->next;
    current->next = retval;
    retval = current;
    current = saved_reference;
  }

  return retval;
}




/* Free a netrc list.  */
void
free_netrc(acc_t *l)
{
  acc_t *t;

  while (l)
    {
      t = l->next;
      xfree (l->acc);
      xfree (l->passwd);
      xfree (l->host);
      xfree (l);
      l = t;
    }
}

#ifdef STANDALONE
#include <sys/types.h>
#include <sys/stat.h>

#include "wget_rpc_wrapper.h"

int
main (int argc, char **argv)
{
  struct_stat sb;
  char *program_name, *file, *target;
  acc_t *head, *a;

  if (argc < 2 || argc > 3)
    {
      fprintf (stderr, _("Usage: %s NETRC [HOSTNAME]\n"), argv[0]);
      exit (WGET_EXIT_GENERIC_ERROR);
    }

  program_name = argv[0];
  file = argv[1];
  target = argv[2];

#ifdef ENABLE_NLS
  /* Set the current locale.  */
  setlocale (LC_ALL, "");
  /* Set the text message domain.  */
  bindtextdomain ("wget", LOCALEDIR);
  textdomain ("wget");
#endif /* ENABLE_NLS */

  if (stat (file, &sb))
    {
      fprintf (stderr, _("%s: cannot stat %s: %s\n"), argv[0], file,
               strerror (errno));
      exit (WGET_EXIT_GENERIC_ERROR);
    }

  head = parse_netrc (file);
  a = head;
  while (a)
    {
      /* Skip if we have a target and this isn't it.  */
      if (target && a->host && strcmp (target, a->host))
        {
          a = a->next;
          continue;
        }

      if (!target)
        {
          /* Print the host name if we have no target.  */
          if (a->host)
            fputs (a->host, stdout);
          else
            fputs ("DEFAULT", stdout);

          fputc (' ', stdout);
        }

      /* Print the account name.  */
      fputs (a->acc, stdout);

      if (a->passwd)
        {
          /* Print the password, if there is any.  */
          fputc (' ', stdout);
          fputs (a->passwd, stdout);
        }

      fputc ('\n', stdout);

      /* Exit if we found the target.  */
      if (target)
        exit (WGET_EXIT_SUCCESS);
      a = a->next;
    }

  /* Exit with failure if we had a target, success otherwise.  */
  if (target)
    exit (WGET_EXIT_GENERIC_ERROR);

  exit (WGET_EXIT_SUCCESS);
}
#endif /* STANDALONE */
