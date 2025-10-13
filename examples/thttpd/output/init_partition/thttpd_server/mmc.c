/* mmc.c - mmap cache
**
** Copyright  1998,2001,2014 by Jef Poskanzer <jef@mail.acme.com>.
** All rights reserved.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>

#ifdef HAVE_MMAP
#include <sys/mman.h>
#endif /* HAVE_MMAP */

#include "mmc.h"
#include "libhttpd.h"

#ifndef HAVE_INT64T
typedef long long int64_t;
#endif


/* Defines. */
#ifndef DEFAULT_EXPIRE_AGE
#define DEFAULT_EXPIRE_AGE 600
#endif
#ifndef DESIRED_FREE_COUNT
#define DESIRED_FREE_COUNT 100
#endif
#ifndef DESIRED_MAX_MAPPED_FILES
#define DESIRED_MAX_MAPPED_FILES 2000
#endif
#ifndef DESIRED_MAX_MAPPED_BYTES
#define DESIRED_MAX_MAPPED_BYTES 1000000000
#endif
#ifndef INITIAL_HASH_SIZE
#define INITIAL_HASH_SIZE (1 << 10)
#endif

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif


/* The Map struct. */
typedef struct MapStruct {
    ino_t ino;
    dev_t dev;
    off_t size;
    time_t ct;
    int refcount;
    time_t reftime;
    void* addr;
    unsigned int hash;
    int hash_idx;
    struct MapStruct* next;
    } Map;


/* Globals. */
static Map* maps = (Map*) 0;
static Map* free_maps = (Map*) 0;
static int alloc_count = 0, map_count = 0, free_count = 0;
static Map** hash_table = (Map**) 0;
static int hash_size;
static unsigned int hash_mask;
static time_t expire_age = DEFAULT_EXPIRE_AGE;
static off_t mapped_bytes = 0;



/* Forwards. */
static void panic( void );
static void really_unmap( Map** mm );
static int check_hash_size( void );
static int add_hash( Map* m );
static Map* find_hash( ino_t ino, dev_t dev, off_t size, time_t ct );
static unsigned int hash( ino_t ino, dev_t dev, off_t size, time_t ct );


v

v

v

s

s

v

/* Make sure the hash table is big enough. */
s

s

s

s

/* Generate debugging statistics syslog message. */
v