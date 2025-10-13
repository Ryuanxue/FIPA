/* thttpd.c - tiny/turbo/throttling HTTP server
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

#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/uio.h>

#include <errno.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#include <pwd.h>
#ifdef HAVE_GRP_H
#include <grp.h>
#endif
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#ifdef TIME_WITH_SYS_TIME
#include <time.h>
#endif
#include <unistd.h>

#include "fdwatch.h"
#include "libhttpd.h"
#include "mmc.h"
#include "timers.h"
#include "match.h"

#ifndef SHUT_WR
#define SHUT_WR 1
#endif

#ifndef HAVE_INT64T
typedef long long int64_t;
#endif


static char* argv0;
static int debug;
static unsigned short port;
static char* dir;
static char* data_dir;
static int do_chroot, no_log, no_symlink_check, do_vhost, do_global_passwd;
static char* cgi_pattern;
static int cgi_limit;
static char* url_pattern;
static int no_empty_referrers;
static char* local_pattern;
static char* logfile;
static char* throttlefile;
static char* hostname;
static char* pidfile;
static char* user;
static char* charset;
static char* p3p;
static int max_age;


typedef struct {
    char* pattern;
    long max_limit, min_limit;
    long rate;
    off_t bytes_since_avg;
    int num_sending;
    } throttletab;
static throttletab* throttles;
static int numthrottles, maxthrottles;

#define THROTTLE_NOLIMIT -1


typedef struct {
    int conn_state;
    int next_free_connect;
    httpd_conn* hc;
    int tnums[MAXTHROTTLENUMS];         /* throttle indexes */
    int numtnums;
    long max_limit, min_limit;
    time_t started_at, active_at;
    Timer* wakeup_timer;
    Timer* linger_timer;
    long wouldblock_delay;
    off_t bytes;
    off_t end_byte_index;
    off_t next_byte_index;
    } connecttab;
static connecttab* connects;
static int num_connects, max_connects, first_free_connect;
static int httpd_conn_count;

/* The connection states. */
#define CNST_FREE 0
#define CNST_READING 1
#define CNST_SENDING 2
#define CNST_PAUSING 3
#define CNST_LINGERING 4


static httpd_server* hs = (httpd_server*) 0;
int terminate = 0;
time_t start_time, stats_time;
long stats_connections;
off_t stats_bytes;
int stats_simultaneous;

static volatile int got_hup, got_usr1, watchdog_flag;


/* Forwards. */
static void parse_args( int argc, char** argv );
static void usage( void );
static void read_config( char* filename );
static void value_required( char* name, char* value );
static void no_value_required( char* name, char* value );
static char* e_strdup( char* oldstr );
static void lookup_hostname( httpd_sockaddr* sa4P, size_t sa4_len, int* gotv4P, httpd_sockaddr* sa6P, size_t sa6_len, int* gotv6P );
static void read_throttlefile( char* tf );
static void shut_down( void );
static int handle_newconnect( struct timeval* tvP, int listen_fd );
static void handle_read( connecttab* c, struct timeval* tvP );
static void handle_send( connecttab* c, struct timeval* tvP );
static void handle_linger( connecttab* c, struct timeval* tvP );
static int check_throttles( connecttab* c );
static void clear_throttles( connecttab* c, struct timeval* tvP );
static void update_throttles( ClientData client_data, struct timeval* nowP );
static void finish_connection( connecttab* c, struct timeval* tvP );
static void clear_connection( connecttab* c, struct timeval* tvP );
static void really_clear_connection( connecttab* c, struct timeval* tvP );
static void idle( ClientData client_data, struct timeval* nowP );
static void wakeup_connection( ClientData client_data, struct timeval* nowP );
static void linger_clear_connection( ClientData client_data, struct timeval* nowP );
static void occasional( ClientData client_data, struct timeval* nowP );
#ifdef STATS_TIME
static void show_stats( ClientData client_data, struct timeval* nowP );
#endif /* STATS_TIME */
static void logstats( struct timeval* nowP );
static void thttpd_logstats( long secs );


/* SIGTERM and SIGINT say to exit immediately. */
s

/* SIGCHLD - a chile process exitted, so we need to reap the zombie */
s

/* SIGHUP says to re-open the log file. */
s

/* SIGUSR1 says to exit as soon as all current connections are done. */
s

/* SIGUSR2 says to generate the stats syslogs immediately. */
s

/* SIGALRM is used as a watchdog. */
s

s

i

s

s

s

s

s

s

s

s

s

s

s

s

s

s

s

s

s

s

s

s

s
s

s

#ifdef STATS_TIME
s#endif /* STATS_TIME */


/* Generate debugging statistics syslog messages for all packages. */
s

/* Generate debugging statistics syslog message. */
s