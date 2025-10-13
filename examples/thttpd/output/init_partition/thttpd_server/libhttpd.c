/* libhttpd.c - HTTP protocol library
**
** Copyright  1995,1998,1999,2000,2001,2015 by
** Jef Poskanzer <jef@mail.acme.com>. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/


#include "config.h"
#include "version.h"

#ifdef SHOW_SERVER_VERSION
#define EXPOSED_SERVER_SOFTWARE SERVER_SOFTWARE
#else /* SHOW_SERVER_VERSION */
#define EXPOSED_SERVER_SOFTWARE "thttpd"
#endif /* SHOW_SERVER_VERSION */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif /* HAVE_MEMORY_H */
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <stdarg.h>

#ifdef HAVE_OSRELDATE_H
#include <osreldate.h>
#endif /* HAVE_OSRELDATE_H */

#ifdef HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# ifdef HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# ifdef HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# ifdef HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#include "libhttpd.h"
#include "mmc.h"
#include "timers.h"
#include "match.h"
#include "tdate_parse.h"

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

#ifndef SHUT_WR
#define SHUT_WR 1
#endif

#ifndef HAVE_INT64T
typedef long long int64_t;
#endif

#ifndef HAVE_SOCKLENT
typedef int socklen_t;
#endif

#ifdef __CYGWIN__
#define timezone  _timezone
#endif

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif


/* Forwards. */
static void check_options( void );
static void free_httpd_server( httpd_server* hs );
static int initialize_listen_socket( httpd_sockaddr* saP );
static void add_response( httpd_conn* hc, char* str );
static void send_mime( httpd_conn* hc, int status, char* title, char* encodings, char* extraheads, char* type, off_t length, time_t mod );
static void send_response( httpd_conn* hc, int status, char* title, char* extraheads, char* form, char* arg );
static void send_response_tail( httpd_conn* hc );
static void defang( char* str, char* dfstr, int dfsize );
#ifdef ERR_DIR
static int send_err_file( httpd_conn* hc, int status, char* title, char* extraheads, char* filename );
#endif /* ERR_DIR */
#ifdef AUTH_FILE
static void send_authenticate( httpd_conn* hc, char* realm );
static int b64_decode( const char* str, unsigned char* space, int size );
static int auth_check( httpd_conn* hc, char* dirname  );
static int auth_check2( httpd_conn* hc, char* dirname  );
#endif /* AUTH_FILE */
static void send_dirredirect( httpd_conn* hc );
static int hexit( char c );
static void strdecode( char* to, char* from );
#ifdef GENERATE_INDEXES
static void strencode( char* to, int tosize, char* from );
#endif /* GENERATE_INDEXES */
#ifdef TILDE_MAP_1
static int tilde_map_1( httpd_conn* hc );
#endif /* TILDE_MAP_1 */
#ifdef TILDE_MAP_2
static int tilde_map_2( httpd_conn* hc );
#endif /* TILDE_MAP_2 */
static int vhost_map( httpd_conn* hc );
static char* expand_symlinks( char* path, char** restP, int no_symlink_check, int tildemapped );
static char* bufgets( httpd_conn* hc );
static void de_dotdot( char* file );
static void init_mime( void );
static void figure_mime( httpd_conn* hc );
#ifdef CGI_TIMELIMIT
static void cgi_kill2( ClientData client_data, struct timeval* nowP );
static void cgi_kill( ClientData client_data, struct timeval* nowP );
#endif /* CGI_TIMELIMIT */
#ifdef GENERATE_INDEXES
static int ls( httpd_conn* hc );
#endif /* GENERATE_INDEXES */
static char* build_env( char* fmt, char* arg );
#ifdef SERVER_NAME_LIST
static char* hostname_map( char* hostname );
#endif /* SERVER_NAME_LIST */
static char** make_envp( httpd_conn* hc );
static char** make_argp( httpd_conn* hc );
static void cgi_interpose_input( httpd_conn* hc, int wfd );
static void post_post_garbage_hack( httpd_conn* hc );
static void cgi_interpose_output( httpd_conn* hc, int rfd );
static void cgi_child( httpd_conn* hc );
static int cgi( httpd_conn* hc );
static int really_start_request( httpd_conn* hc, struct timeval* nowP );
static void make_log_entry( httpd_conn* hc, struct timeval* nowP );
static int check_referrer( httpd_conn* hc );
static int really_check_referrer( httpd_conn* hc );
static int sockaddr_check( httpd_sockaddr* saP );
static size_t sockaddr_len( httpd_sockaddr* saP );
static int my_snprintf( char* str, size_t size, const char* format, ... );
#ifndef HAVE_ATOLL
static long long atoll( const char* str );
#endif /* HAVE_ATOLL */


/* This global keeps track of whether we are in the main process or a
** sub-process.  The reason is that httpd_write_response() can get called
** in either context; when it is called from the main process it must use
** non-blocking I/O to avoid stalling the server, but when it is called
** from a sub-process it wants to use blocking I/O so that the whole
** response definitely gets written.  So, it checks this variable.  A bit
** of a hack but it seems to do the right thing.
*/
static int sub_process = 0;


s

s

h

s

v

v

v

/* Conditional macro to allow two alternate forms for use in the built-in
** error pages.  If EXPLICIT_ERROR_PAGES is defined, the second and more
** explicit error form is used; otherwise, the first and more generic
** form is used.
*/
#ifdef EXPLICIT_ERROR_PAGES
#define ERROR_FORM(a,b) b
#else /* EXPLICIT_ERROR_PAGES */
#define ERROR_FORM(a,b) a
#endif /* EXPLICIT_ERROR_PAGES */


static char* ok200title = "OK";
static char* ok206title = "Partial Content";

static char* err302title = "Found";
static char* err302form = "The actual URL is '%.80s'.\n";

static char* err304title = "Not Modified";

char* httpd_err400title = "Bad Request";
char* httpd_err400form =
    "Your request has bad syntax or is inherently impossible to satisfy.\n";

#ifdef AUTH_FILE
static char* err401title = "Unauthorized";
static char* err401form =
    "Authorization required for the URL '%.80s'.\n";
#endif /* AUTH_FILE */

static char* err403title = "Forbidden";
#ifndef EXPLICIT_ERROR_PAGES
static char* err403form =
    "You do not have permission to get URL '%.80s' from this server.\n";
#endif /* !EXPLICIT_ERROR_PAGES */

static char* err404title = "Not Found";
static char* err404form =
    "The requested URL '%.80s' was not found on this server.\n";

char* httpd_err408title = "Request Timeout";
char* httpd_err408form =
    "No request appeared within a reasonable time period.\n";

static char* err451title = "Unavailable For Legal Reasons";
static char* err451form =
    "You do not have legal permission to get URL '%.80s' from this server.\n";

static char* err500title = "Internal Error";
static char* err500form =
    "There was an unusual problem serving the requested URL '%.80s'.\n";

static char* err501title = "Not Implemented";
static char* err501form =
    "The requested method '%.80s' is not implemented by this server.\n";

char* httpd_err503title = "Service Temporarily Overloaded";
char* httpd_err503form =
    "The requested URL '%.80s' is temporarily overloaded.  Please try again later.\n";


/* Append a string to the buffer waiting to be sent as response. */
static void
add_response( httpd_conn* hc, char* str )
    {
    size_t len;

    len = strlen( str );
    httpd_realloc_str( &hc->response, &hc->maxresponse, hc->responselen + len );
    (void) memmove( &(hc->response[hc->responselen]), str, len );
    hc->responselen += len;
    }

/* Send the buffered response. */
v

/* Set no-delay / non-blocking mode on a socket. */
v

/* Clear no-delay / non-blocking mode on a socket. */
v

static void
send_mime( httpd_conn* hc, int status, char* title, char* encodings, char* extraheads, char* type, off_t length, time_t mod )
    {
    time_t now, expires;
    const char* rfc1123fmt = "%a, %d %b %Y %H:%M:%S GMT";
    char nowbuf[100];
    char modbuf[100];
    char expbuf[100];
    char fixed_type[500];
    char buf[1000];
    int partial_content;
    int s100;

    hc->status = status;
    hc->bytes_to_send = length;
    if ( hc->mime_flag )
	{
	if ( status == 200 && hc->got_range &&
	     ( hc->last_byte_index >= hc->first_byte_index ) &&
	     ( ( hc->last_byte_index != length - 1 ) ||
	       ( hc->first_byte_index != 0 ) ) &&
	     ( hc->range_if == (time_t) -1 ||
	       hc->range_if == hc->sb.st_mtime ) )
	    {
	    partial_content = 1;
	    hc->status = status = 206;
	    title = ok206title;
	    }
	else
	    {
	    partial_content = 0;
	    hc->got_range = 0;
	    }

	now = time( (time_t*) 0 );
	if ( mod == (time_t) 0 )
	    mod = now;
	(void) strftime( nowbuf, sizeof(nowbuf), rfc1123fmt, gmtime( &now ) );
	(void) strftime( modbuf, sizeof(modbuf), rfc1123fmt, gmtime( &mod ) );
	(void) my_snprintf(
	    fixed_type, sizeof(fixed_type), type, hc->hs->charset );
	(void) my_snprintf( buf, sizeof(buf),
	    "%.20s %d %s\015\012Server: %s\015\012Content-Type: %s\015\012Date: %s\015\012Last-Modified: %s\015\012Accept-Ranges: bytes\015\012Connection: close\015\012",
	    hc->protocol, status, title, EXPOSED_SERVER_SOFTWARE, fixed_type,
	    nowbuf, modbuf );
	add_response( hc, buf );
	s100 = status / 100;
	if ( s100 != 2 && s100 != 3 )
	    {
	    (void) my_snprintf( buf, sizeof(buf),
		"Cache-Control: no-cache,no-store\015\012" );
	    add_response( hc, buf );
	    }
	if ( encodings[0] != '\0' )
	    {
	    (void) my_snprintf( buf, sizeof(buf),
		"Content-Encoding: %s\015\012", encodings );
	    add_response( hc, buf );
	    }
	if ( partial_content )
	    {
	    (void) my_snprintf( buf, sizeof(buf),
		"Content-Range: bytes %lld-%lld/%lld\015\012Content-Length: %lld\015\012",
		(long long) hc->first_byte_index,
		(long long) hc->last_byte_index,
		(long long) length,
		(long long) ( hc->last_byte_index - hc->first_byte_index + 1 ) );
	    add_response( hc, buf );
	    }
	else if ( length >= 0 )
	    {
	    (void) my_snprintf( buf, sizeof(buf),
		"Content-Length: %lld\015\012", (long long) length );
	    add_response( hc, buf );
	    }
	if ( hc->hs->p3p[0] != '\0' )
	    {
	    (void) my_snprintf( buf, sizeof(buf), "P3P: %s\015\012", hc->hs->p3p );
	    add_response( hc, buf );
	    }
	if ( hc->hs->max_age >= 0 )
	    {
	    expires = now + hc->hs->max_age;
	    (void) strftime(
		expbuf, sizeof(expbuf), rfc1123fmt, gmtime( &expires ) );
	    (void) my_snprintf( buf, sizeof(buf),
		"Cache-Control: max-age=%d\015\012Expires: %s\015\012",
		hc->hs->max_age, expbuf );
	    add_response( hc, buf );
	    }
	if ( extraheads[0] != '\0' )
	    add_response( hc, extraheads );
	add_response( hc, "\015\012" );
	}
    }


static int str_alloc_count = 0;
static size_t str_alloc_size = 0;

void
httpd_realloc_str( char** strP, size_t* maxsizeP, size_t size )
    {
    if ( *maxsizeP == 0 )
	{
	*maxsizeP = MAX( 200, size + 100 );
	*strP = NEW( char, *maxsizeP + 1 );
	++str_alloc_count;
	str_alloc_size += *maxsizeP;
	}
    else if ( size > *maxsizeP )
	{
	str_alloc_size -= *maxsizeP;
	*maxsizeP = MAX( *maxsizeP * 2, size * 5 / 4 );
	*strP = RENEW( *strP, char, *maxsizeP + 1 );
	str_alloc_size += *maxsizeP;
	}
    else
	return;
    if ( *strP == (char*) 0 )
	{
	syslog(
	    LOG_ERR, "out of memory reallocating a string to %ld bytes",
	    (long) *maxsizeP );
	exit( 1 );
	}
    }


static void
send_response( httpd_conn* hc, int status, char* title, char* extraheads, char* form, char* arg )
    {
    char defanged_arg[1000], buf[2000];

    send_mime(
	hc, status, title, "", extraheads, "text/html; charset=%s", (off_t) -1,
	(time_t) 0 );
    (void) my_snprintf( buf, sizeof(buf), "\
<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n\
\n\
<html>\n\
\n\
  <head>\n\
    <meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\">\n\
    <title>%d %s</title>\n\
  </head>\n\
\n\
  <body bgcolor=\"#cc9999\" text=\"#000000\" link=\"#2020ff\" vlink=\"#4040cc\">\n\
\n\
    <h2>%d %s</h2>\n",
	status, title, status, title );
    add_response( hc, buf );
    defang( arg, defanged_arg, sizeof(defanged_arg) );
    (void) my_snprintf( buf, sizeof(buf), form, defanged_arg );
    add_response( hc, buf );
    if ( match( "**MSIE**", hc->useragent ) )
	{
	int n;
	add_response( hc, "<!--\n" );
	for ( n = 0; n < 6; ++n )
	    add_response( hc, "Padding so that MSIE deigns to show this error instead of its own canned one.\n");
	add_response( hc, "-->\n" );
	}
    send_response_tail( hc );
    }


static void
send_response_tail( httpd_conn* hc )
    {
    char buf[1000];

    (void) my_snprintf( buf, sizeof(buf), "\
    <hr>\n\
\n\
    <address><a href=\"%s\">%s</a></address>\n\
\n\
  </body>\n\
\n\
</html>\n",
	SERVER_ADDRESS, EXPOSED_SERVER_SOFTWARE );
    add_response( hc, buf );
    }


static void
defang( char* str, char* dfstr, int dfsize )
    {
    char* cp1;
    char* cp2;

    for ( cp1 = str, cp2 = dfstr;
	  *cp1 != '\0' && cp2 - dfstr < dfsize - 5;
	  ++cp1, ++cp2 )
	{
	switch ( *cp1 )
	    {
	    case '<':
	    *cp2++ = '&';
	    *cp2++ = 'l';
	    *cp2++ = 't';
	    *cp2 = ';';
	    break;
	    case '>':
	    *cp2++ = '&';
	    *cp2++ = 'g';
	    *cp2++ = 't';
	    *cp2 = ';';
	    break;
	    default:
	    *cp2 = *cp1;
	    break;
	    }
	}
    *cp2 = '\0';
    }


void
httpd_send_err( httpd_conn* hc, int status, char* title, char* extraheads, char* form, char* arg )
    {
#ifdef ERR_DIR

    char filename[1000];

    /* Try virtual host error page. */
    if ( hc->hs->vhost && hc->hostdir[0] != '\0' )
	{
	(void) my_snprintf( filename, sizeof(filename),
	    "%s/%s/err%d.html", hc->hostdir, ERR_DIR, status );
	if ( send_err_file( hc, status, title, extraheads, filename ) )
	    return;
	}

    /* Try server-wide error page. */
    (void) my_snprintf( filename, sizeof(filename),
	"%s/err%d.html", ERR_DIR, status );
    if ( send_err_file( hc, status, title, extraheads, filename ) )
	return;

    /* Fall back on built-in error page. */
    send_response( hc, status, title, extraheads, form, arg );

#else /* ERR_DIR */

    send_response( hc, status, title, extraheads, form, arg );

#endif /* ERR_DIR */
    }


#ifdef ERR_DIR
static int
send_err_file( httpd_conn* hc, int status, char* title, char* extraheads, char* filename )
    {
    FILE* fp;
    char buf[1000];
    size_t r;

    fp = fopen( filename, "r" );
    if ( fp == (FILE*) 0 )
	return 0;
    send_mime(
	hc, status, title, "", extraheads, "text/html; charset=%s", (off_t) -1,
	(time_t) 0 );
    for (;;)
	{
	r = fread( buf, 1, sizeof(buf) - 1, fp );
	if ( r == 0 )
	    break;
	buf[r] = '\0';
	add_response( hc, buf );
	}
    (void) fclose( fp );

#ifdef ERR_APPEND_SERVER_INFO
    send_response_tail( hc );
#endif /* ERR_APPEND_SERVER_INFO */

    return 1;
    }
#endif /* ERR_DIR */


#ifdef AUTH_FILE

static void
send_authenticate( httpd_conn* hc, char* realm )
    {
    static char* header;
    static size_t maxheader = 0;
    static char headstr[] = "WWW-Authenticate: Basic realm=\"";

    httpd_realloc_str(
	&header, &maxheader, sizeof(headstr) + strlen( realm ) + 3 );
    (void) my_snprintf( header, maxheader, "%s%s\"\015\012", headstr, realm );
    httpd_send_err( hc, 401, err401title, header, err401form, hc->encodedurl );
    /* If the request was a POST then there might still be data to be read,
    ** so we need to do a lingering close.
    */
    if ( hc->method == METHOD_POST )
	hc->should_linger = 1;
    }


/* Base-64 decoding.  This represents binary data as printable ASCII
** characters.  Three 8-bit binary bytes are turned into four 6-bit
** values, like so:
**
**   [11111111]  [22222222]  [33333333]
**
**   [111111] [112222] [222233] [333333]
**
** Then the 6-bit values are represented using the characters "A-Za-z0-9+/".
*/

static int b64_decode_table[256] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
    52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
    15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
    -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
    41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
    };

/* Do base-64 decoding on a string.  Ignore any non-base64 bytes.
** Return the actual number of bytes generated.  The decoded size will
** be at most 3/4 the size of the encoded, and may be smaller if there
** are padding characters (blanks, newlines).
*/
static int
b64_decode( const char* str, unsigned char* space, int size )
    {
    const char* cp;
    int space_idx, phase;
    int d, prev_d = 0;
    unsigned char c;

    space_idx = 0;
    phase = 0;
    for ( cp = str; *cp != '\0'; ++cp )
	{
	d = b64_decode_table[(int) ((unsigned char) *cp)];
	if ( d != -1 )
	    {
	    switch ( phase )
		{
		case 0:
		++phase;
		break;
		case 1:
		c = ( ( prev_d << 2 ) | ( ( d & 0x30 ) >> 4 ) );
		if ( space_idx < size )
		    space[space_idx++] = c;
		++phase;
		break;
		case 2:
		c = ( ( ( prev_d & 0xf ) << 4 ) | ( ( d & 0x3c ) >> 2 ) );
		if ( space_idx < size )
		    space[space_idx++] = c;
		++phase;
		break;
		case 3:
		c = ( ( ( prev_d & 0x03 ) << 6 ) | d );
		if ( space_idx < size )
		    space[space_idx++] = c;
		phase = 0;
		break;
		}
	    prev_d = d;
	    }
	}
    return space_idx;
    }


/* Returns -1 == unauthorized, 0 == no auth file, 1 = authorized. */
s

/* Returns -1 == unauthorized, 0 == no auth file, 1 = authorized. */
sint auth_check2_sense_1(int *auth_check2_sense_1_ret, httpd_conn *hc, char *dirname)
{
  static char *authpath;
  static size_t maxauthpath = 0;
  struct stat sb;
  char authinfo[500];
  char *authpass;
  char *colon;
  int l;
  FILE *fp;
  char line[500];
  char *cryp;
  static char *prevauthpath;
  static size_t maxprevauthpath = 0;
  static time_t prevmtime;
  static char *prevuser;
  static size_t maxprevuser = 0;
  static char *prevcryp;
  static size_t maxprevcryp = 0;
  httpd_realloc_str(&authpath, &maxauthpath, (strlen(dirname) + 1) + (sizeof(".htpasswd")));
  (void) my_snprintf(authpath, maxauthpath, "%s/%s", dirname, ".htpasswd");
  if (stat(authpath, &sb) < 0)
  {
    *auth_check2_sense_1_ret = 1;
    return 0;
  }
  if ((hc->authorization[0] == '\0') || (strncmp(hc->authorization, "Basic ", 6) != 0))
  {
    send_authenticate(hc, dirname);
    {
      *auth_check2_sense_1_ret = 1;
      return -1;
    }
  }
  l = b64_decode(&hc->authorization[6], (unsigned char *) authinfo, (sizeof(authinfo)) - 1);
  authinfo[l] = '\0';
  authpass = strchr(authinfo, ':');
  if (authpass == ((char *) 0))
  {
    send_authenticate(hc, dirname);
    {
      *auth_check2_sense_1_ret = 1;
      return -1;
    }
  }
  *(authpass++) = '\0';
  colon = strchr(authpass, ':');
  if (colon != ((char *) 0))
    *colon = '\0';
  if ((((maxprevauthpath != 0) && (strcmp(authpath, prevauthpath) == 0)) && (sb.st_mtime == prevmtime)) && (strcmp(authinfo, prevuser) == 0))
  {
    if (strcmp(crypt(authpass, prevcryp), prevcryp) == 0)
    {
      httpd_realloc_str(&hc->remoteuser, &hc->maxremoteuser, strlen(authinfo));
      (void) strcpy(hc->remoteuser, authinfo);
      {
        *auth_check2_sense_1_ret = 1;
        return 1;
      }
    }
    else
    {
      send_authenticate(hc, dirname);
      {
        *auth_check2_sense_1_ret = 1;
        return -1;
      }
    }
  }
  fp = fopen(authpath, "r");
  if (fp == ((FILE *) 0))
  {
    syslog(LOG_ERR, "%.80s auth file %.80s could not be opened - %m", httpd_ntoa(&hc->client_addr), authpath);
    httpd_send_err(hc, 403, err403title, "", "The requested URL '%.80s' is protected by an authentication file, but the authentication file cannot be opened.\n", hc->encodedurl);
    {
      *auth_check2_sense_1_ret = 1;
      return -1;
    }
  }
  while (fgets(line, sizeof(line), fp) != ((char *) 0))
  {
    l = strlen(line);
    if (line[l - 1] == '\n')
      line[l - 1] = '\0';
    cryp = strchr(line, ':');
    if (cryp == ((char *) 0))
      continue;
    *(cryp++) = '\0';
    if (strcmp(line, authinfo) == 0)
    {
      (void) fclose(fp);
      if (strcmp(crypt(authpass, cryp), cryp) == 0)
      {
        httpd_realloc_str(&hc->remoteuser, &hc->maxremoteuser, strlen(line));
        (void) strcpy(hc->remoteuser, line);
        httpd_realloc_str(&prevauthpath, &maxprevauthpath, strlen(authpath));
        (void) strcpy(prevauthpath, authpath);
        prevmtime = sb.st_mtime;
        httpd_realloc_str(&prevuser, &maxprevuser, strlen(authinfo));
        (void) strcpy(prevuser, authinfo);
        httpd_realloc_str(&prevcryp, &maxprevcryp, strlen(cryp));
        (void) strcpy(prevcryp, cryp);
        {
          *auth_check2_sense_1_ret = 1;
          return 1;
        }
      }
      else
      {
        send_authenticate(hc, dirname);
        {
          *auth_check2_sense_1_ret = 1;
          return -1;
        }
      }
    }
  }

  (void) fclose(fp);
  send_authenticate(hc, dirname);
  {
    *auth_check2_sense_1_ret = 1;
    return -1;
  }
}


#endif /* AUTH_FILE */


s

char*
httpd_method_str( int method )
    {
    switch ( method )
	{
	case METHOD_GET: return "GET";
	case METHOD_HEAD: return "HEAD";
	case METHOD_POST: return "POST";
	default: return "UNKNOWN";
	}
    }


s

/* Copies and decodes a string.  It's ok for from and to to be the
** same string.
*/
s

#ifdef GENERATE_INDEXES
/* Copies and encodes a string. */
s#endif /* GENERATE_INDEXES */


#ifdef TILDE_MAP_1
/* Map a ~username/whatever URL into <prefix>/username. */
static int
tilde_map_1( httpd_conn* hc )
    {
    static char* temp;
    static size_t maxtemp = 0;
    int len;
    static char* prefix = TILDE_MAP_1;

    len = strlen( hc->expnfilename ) - 1;
    httpd_realloc_str( &temp, &maxtemp, len );
    (void) strcpy( temp, &hc->expnfilename[1] );
    httpd_realloc_str(
	&hc->expnfilename, &hc->maxexpnfilename, strlen( prefix ) + 1 + len );
    (void) strcpy( hc->expnfilename, prefix );
    if ( prefix[0] != '\0' )
	(void) strcat( hc->expnfilename, "/" );
    (void) strcat( hc->expnfilename, temp );
    return 1;
    }
#endif /* TILDE_MAP_1 */

#ifdef TILDE_MAP_2
/* Map a ~username/whatever URL into <user's homedir>/<postfix>. */
static int
tilde_map_2( httpd_conn* hc )
    {
    static char* temp;
    static size_t maxtemp = 0;
    static char* postfix = TILDE_MAP_2;
    char* cp;
    struct passwd* pw;
    char* alt;
    char* rest;

    /* Get the username. */
    httpd_realloc_str( &temp, &maxtemp, strlen( hc->expnfilename ) - 1 );
    (void) strcpy( temp, &hc->expnfilename[1] );
    cp = strchr( temp, '/' );
    if ( cp != (char*) 0 )
	*cp++ = '\0';
    else
	cp = "";

    /* Get the passwd entry. */
    pw = getpwnam( temp );
    if ( pw == (struct passwd*) 0 )
	return 0;

    /* Set up altdir. */
    httpd_realloc_str(
	&hc->altdir, &hc->maxaltdir,
	strlen( pw->pw_dir ) + 1 + strlen( postfix ) );
    (void) strcpy( hc->altdir, pw->pw_dir );
    if ( postfix[0] != '\0' )
	{
	(void) strcat( hc->altdir, "/" );
	(void) strcat( hc->altdir, postfix );
	}
    alt = expand_symlinks( hc->altdir, &rest, 0, 1 );
    if ( rest[0] != '\0' )
	return 0;
    httpd_realloc_str( &hc->altdir, &hc->maxaltdir, strlen( alt ) );
    (void) strcpy( hc->altdir, alt );

    /* And the filename becomes altdir plus the post-~ part of the original. */
    httpd_realloc_str(
	&hc->expnfilename, &hc->maxexpnfilename,
	strlen( hc->altdir ) + 1 + strlen( cp ) );
    (void) my_snprintf( hc->expnfilename, hc->maxexpnfilename,
	"%s/%s", hc->altdir, cp );

    /* For this type of tilde mapping, we want to defeat vhost mapping. */
    hc->tildemapped = 1;

    return 1;
    }
#endif /* TILDE_MAP_2 */


/* Virtual host mapping. */
s

/* Expands all symlinks in the given filename, eliding ..'s and leading /'s.
** Returns the expanded path (pointer to static string), or (char*) 0 on
** errors.  Also returns, in the string pointed to by restP, any trailing
** parts of the path that don't exist.
**
** This is a fairly nice little routine.  It handles any size filenames
** without excessive mallocs.
*/
s

i

/* Checks hc->read_buf to see whether a complete request has been read so far;
** either the first line has two words (an HTTP/0.9 request), or the first
** line has three words and there's a blank line present.
**
** hc->read_idx is how much has been read in; hc->checked_idx is how much we
** have checked so far; and hc->checked_state is the current state of the
** finite state machine.
*/
i

i

s

s

v
void
httpd_destroy_conn( httpd_conn* hc )
    {
    if ( hc->initialized )
	{
	free( (void*) hc->read_buf );
	free( (void*) hc->decodedurl );
	free( (void*) hc->origfilename );
	free( (void*) hc->expnfilename );
	free( (void*) hc->encodings );
	free( (void*) hc->pathinfo );
	free( (void*) hc->query );
	free( (void*) hc->accept );
	free( (void*) hc->accepte );
	free( (void*) hc->reqhost );
	free( (void*) hc->hostdir );
	free( (void*) hc->remoteuser );
	free( (void*) hc->response );
#ifdef TILDE_MAP_2
	free( (void*) hc->altdir );
#endif /* TILDE_MAP_2 */
	hc->initialized = 0;
	}
    }


struct mime_entry {
    char* ext;
    size_t ext_len;
    char* val;
    size_t val_len;
    };
static struct mime_entry enc_tab[] = {
#include "mime_encodings.h"
    };
static const int n_enc_tab = sizeof(enc_tab) / sizeof(*enc_tab);
static struct mime_entry typ_tab[] = {
#include "mime_types.h"
    };
static const int n_typ_tab = sizeof(typ_tab) / sizeof(*typ_tab);


/* qsort comparison routine */
s

s

/* Figure out MIME encodings and type based on the filename.  Multiple
** encodings are separated by commas, and are listed in the order in
** which they were applied to the file.
*/
s

#ifdef CGI_TIMELIMIT
s
s#endif /* CGI_TIMELIMIT */


#ifdef GENERATE_INDEXES

/* qsort comparison routine */
s

s
#endif /* GENERATE_INDEXES */


s

#ifdef SERVER_NAME_LIST
static char*
hostname_map( char* hostname )
    {
    int len, n;
    static char* list[] = { SERVER_NAME_LIST };

    len = strlen( hostname );
    for ( n = sizeof(list) / sizeof(*list) - 1; n >= 0; --n )
	if ( strncasecmp( hostname, list[n], len ) == 0 )
	    if ( list[n][len] == '/' )  /* check in case of a substring match */
		return &list[n][len + 1];
    return (char*) 0;
    }
#endif /* SERVER_NAME_LIST */


/* Set up environment variables. Be real careful here to avoid
** letting malicious clients overrun a buffer.  We don't have
** to worry about freeing stuff since we're a sub-process.
*/
s

/* Set up argument vector.  Again, we don't have to worry about freeing stuff
** since we're a sub-process.  This gets done after make_envp() because we
** scribble on hc->query.
*/
s

/* This routine is used only for POST requests.  It reads the data
** from the request and sends it to the child process.  The only reason
** we need to do it this way instead of just letting the child read
** directly is that we have already read part of the data into our
** buffer.
*/
s

/* Special hack to deal with broken browsers that send a LF or CRLF
** after POST data, causing TCP resets - we just read and discard up
** to 2 bytes.  Unfortunately this doesn't fix the problem for CGIs
** which avoid the interposer process due to their POST data being
** short.  Creating an interposer process for all POST CGIs is
** unacceptably expensive.  The eventual fix will come when interposing
** gets integrated into the main loop as a tasklet instead of a process.
*/
s

/* This routine is used for parsed-header CGIs.  The idea here is that the
** CGI can return special headers such as "Status:" and "Location:" which
** change the return status of the response.  Since the return status has to
** be the very first line written out, we have to accumulate all the headers
** and check for the special ones before writing the status.  Then we write
** out the saved headers and proceed to echo the rest of the response.
*/
s

/* CGI child process. */
s

s

s

i

svoid make_log_entry_sense_1(char *ru, char url[305], char bytes[40], httpd_conn *hc, struct timeval *nowP)
{
  if (hc->remoteuser[0] != '\0')
    ru = hc->remoteuser;
  else
    ru = "-";
  if (hc->hs->vhost && (!hc->tildemapped))
    (void) my_snprintf(url, sizeof(url), "/%.100s%.200s", (hc->hostname == ((char *) 0)) ? (hc->hs->server_hostname) : (hc->hostname), hc->encodedurl);
  else
    (void) my_snprintf(url, sizeof(url), "%.200s", hc->encodedurl);
  if (hc->bytes_sent >= 0)
    (void) my_snprintf(bytes, sizeof(bytes), "%lld", (long long) hc->bytes_sent);
  else
    (void) strcpy(bytes, "-");
  if (hc->hs->logfp != ((FILE *) 0))
  {
    time_t now;
    struct tm *t;
    const char *cernfmt_nozone = "%d/%b/%Y:%H:%M:%S";
    char date_nozone[100];
    int zone;
    char sign;
    char date[100];
    if (nowP != ((struct timeval *) 0))
      now = nowP->tv_sec;
    else
      now = time((time_t *) 0);
    t = localtime(&now);
    (void) strftime(date_nozone, sizeof(date_nozone), cernfmt_nozone, t);
    zone = t->tm_gmtoff / 60L;
    if (zone >= 0)
      sign = '+';
    else
    {
      sign = '-';
      zone = -zone;
    }
    zone = ((zone / 60) * 100) + (zone % 60);
    (void) my_snprintf(date, sizeof(date), "%s %c%04d", date_nozone, sign, zone);
    (void) fprintf(hc->hs->logfp, "%.80s - %.80s [%s] \"%.80s %.300s %.80s\" %d %s \"%.200s\" \"%.200s\"\n", httpd_ntoa(&hc->client_addr), ru, date, httpd_method_str(hc->method), url, hc->protocol, hc->status, bytes, hc->referrer, hc->useragent);
    (void) fflush(hc->hs->logfp);
  }
  else
    syslog(LOG_INFO, "%.80s - %.80s \"%.80s %.200s %.80s\" %d %s \"%.200s\" \"%.200s\"", httpd_ntoa(&hc->client_addr), ru, httpd_method_str(hc->method), url, hc->protocol, hc->status, bytes, hc->referrer, hc->useragent);
}



/* Returns 1 if ok to serve the url, 0 if not. */
s

/* Returns 1 if ok to serve the url, 0 if not. */
s

char*
httpd_ntoa( httpd_sockaddr* saP )
    {
#ifdef USE_IPV6
    static char str[200];

    if ( getnameinfo( &saP->sa, sockaddr_len( saP ), str, sizeof(str), 0, 0, NI_NUMERICHOST ) != 0 )
	{
	str[0] = '?';
	str[1] = '\0';
	}
    else if ( IN6_IS_ADDR_V4MAPPED( &saP->sa_in6.sin6_addr ) && strncmp( str, "::ffff:", 7 ) == 0 )
	/* Elide IPv6ish prefix for IPv4 addresses. */
	(void) ol_strcpy( str, &str[7] );

    return str;

#else /* USE_IPV6 */

    return inet_ntoa( saP->sa_in.sin_addr );

#endif /* USE_IPV6 */
    }


s

static size_t
sockaddr_len( httpd_sockaddr* saP )
    {
    switch ( saP->sa.sa_family )
	{
	case AF_INET: return sizeof(struct sockaddr_in);
#ifdef USE_IPV6
	case AF_INET6: return sizeof(struct sockaddr_in6);
#endif /* USE_IPV6 */
	default:
	return 0;	/* shouldn't happen */
	}
    }


/* Some systems don't have snprintf(), so we make our own that uses
** either vsnprintf() or vsprintf().  If your system doesn't have
** vsnprintf(), it is probably vulnerable to buffer overruns.
** Upgrade!
*/
static int
my_snprintf( char* str, size_t size, const char* format, ... )
    {
    va_list ap;
    int r;

    va_start( ap, format );
#ifdef HAVE_VSNPRINTF
    r = vsnprintf( str, size, format, ap );
#else /* HAVE_VSNPRINTF */
    r = vsprintf( str, format, ap );
#endif /* HAVE_VSNPRINTF */
    va_end( ap );
    return r;
    }


#ifndef HAVE_ATOLL
s#endif /* HAVE_ATOLL */


/* Read the requested buffer completely, accounting for interruptions. */
i

/* Write the requested buffer completely, accounting for interruptions. */
i

/* Generate debugging statistics syslog message. */
v