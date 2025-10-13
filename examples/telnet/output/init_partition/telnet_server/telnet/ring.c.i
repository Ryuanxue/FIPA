# 1 "ring.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "ring.c"
# 50 "ring.c"
# 1 "../config.h" 1
# 3128 "../config.h"
# 1 "../confpaths.h" 1



# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/paths.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/paths.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1



typedef int size_t;
typedef int __builtin_va_list;
typedef int __gnuc_va_list;
typedef int va_list;
typedef int __int8_t;
typedef int __uint8_t;
typedef int __int16_t;
typedef int __uint16_t;
typedef int __int_least16_t;
typedef int __uint_least16_t;
typedef int __int32_t;
typedef int __uint32_t;
typedef int __int64_t;
typedef int __uint64_t;
typedef int __int_least32_t;
typedef int __uint_least32_t;
typedef int __s8;
typedef int __u8;
typedef int __s16;
typedef int __u16;
typedef int __s32;
typedef int __u32;
typedef int __s64;
typedef int __u64;
typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;
typedef int _off_t;
typedef int __dev_t;
typedef int __uid_t;
typedef int __gid_t;
typedef int _off64_t;
typedef int _fpos_t;
typedef int _ssize_t;
typedef int wint_t;
typedef int _mbstate_t;
typedef int _flock_t;
typedef int _iconv_t;
typedef int __ULong;
typedef int __FILE;
typedef int ptrdiff_t;
typedef int wchar_t;
typedef int __off_t;
typedef int __pid_t;
typedef int __loff_t;
typedef int u_char;
typedef int u_short;
typedef int u_int;
typedef int u_long;
typedef int ushort;
typedef int uint;
typedef int clock_t;
typedef int time_t;
typedef int daddr_t;
typedef int caddr_t;
typedef int ino_t;
typedef int off_t;
typedef int dev_t;
typedef int uid_t;
typedef int gid_t;
typedef int pid_t;
typedef int key_t;
typedef int ssize_t;
typedef int mode_t;
typedef int nlink_t;
typedef int fd_mask;
typedef int _types_fd_set;
typedef int clockid_t;
typedef int timer_t;
typedef int useconds_t;
typedef int suseconds_t;
typedef int FILE;
typedef int fpos_t;
typedef int cookie_read_function_t;
typedef int cookie_write_function_t;
typedef int cookie_seek_function_t;
typedef int cookie_close_function_t;
typedef int cookie_io_functions_t;
typedef int div_t;
typedef int ldiv_t;
typedef int lldiv_t;
typedef int sigset_t;
typedef int __sigset_t;
typedef int _sig_func_ptr;
typedef int sig_atomic_t;
typedef int __tzrule_type;
typedef int __tzinfo_type;
typedef int mbstate_t;
typedef int sem_t;
typedef int pthread_t;
typedef int pthread_attr_t;
typedef int pthread_mutex_t;
typedef int pthread_mutexattr_t;
typedef int pthread_cond_t;
typedef int pthread_condattr_t;
typedef int pthread_key_t;
typedef int pthread_once_t;
typedef int pthread_rwlock_t;
typedef int pthread_rwlockattr_t;
typedef int pthread_spinlock_t;
typedef int pthread_barrier_t;
typedef int pthread_barrierattr_t;
typedef int jmp_buf;
typedef int rlim_t;
typedef int sa_family_t;
typedef int sigjmp_buf;
typedef int stack_t;
typedef int siginfo_t;
typedef int z_stream;


typedef int int8_t;
typedef int uint8_t;
typedef int int16_t;
typedef int uint16_t;
typedef int int32_t;
typedef int uint32_t;
typedef int int64_t;
typedef int uint64_t;


typedef int int_least8_t;
typedef int uint_least8_t;
typedef int int_least16_t;
typedef int uint_least16_t;
typedef int int_least32_t;
typedef int uint_least32_t;
typedef int int_least64_t;
typedef int uint_least64_t;


typedef int int_fast8_t;
typedef int uint_fast8_t;
typedef int int_fast16_t;
typedef int uint_fast16_t;
typedef int int_fast32_t;
typedef int uint_fast32_t;
typedef int int_fast64_t;
typedef int uint_fast64_t;


typedef int intptr_t;
typedef int uintptr_t;


typedef int intmax_t;
typedef int uintmax_t;


typedef _Bool bool;


typedef void* MirEGLNativeWindowType;
typedef void* MirEGLNativeDisplayType;
typedef struct MirConnection MirConnection;
typedef struct MirSurface MirSurface;
typedef struct MirSurfaceSpec MirSurfaceSpec;
typedef struct MirScreencast MirScreencast;
typedef struct MirPromptSession MirPromptSession;
typedef struct MirBufferStream MirBufferStream;
typedef struct MirPersistentId MirPersistentId;
typedef struct MirBlob MirBlob;
typedef struct MirDisplayConfig MirDisplayConfig;
typedef unsigned char cc_t;
typedef void (*__sighandler_t) (int);
typedef __sighandler_t sighandler_t;


typedef struct xcb_connection_t xcb_connection_t;
typedef uint32_t xcb_window_t;
typedef uint32_t xcb_visualid_t;
typedef long int __fd_mask;
typedef __fd_mask fd_mask;
typedef uint32_t in_addr_t;
typedef struct pcap pcap_t;
typedef struct pcap_dumper pcap_dumper_t;
typedef u_int bpf_u_int32;
typedef void (*pcap_handler)(u_char *, const struct pcap_pkthdr *,
        const u_char *);


typedef uint16_t in_port_t;

typedef struct
  {






    long int __fds_bits[16];


  } fd_set;


typedef unsigned long __cpu_mask;


typedef struct
{
  __cpu_mask __bits[__CPU_SETSIZE / __NCPUBITS];
} cpu_set_t;



typedef struct __dirstream DIR;



typedef int socklen_t;
typedef long z_off64_t;
typedef void *iconv_t;
struct gzFile_s {
    unsigned have;
    unsigned char *next;
    z_off64_t pos;
};
typedef struct gzFile_s *gzFile;

struct sockaddr
  {
    sa_family_t sa_family;
    char sa_data[14];
  };

struct addrinfo
{
  int ai_flags;
  int ai_family;
  int ai_socktype;
  int ai_protocol;
  socklen_t ai_addrlen;
  struct sockaddr *ai_addr;
  char *ai_canonname;
  struct addrinfo *ai_next;
};

struct termios
  {
    unsigned int c_iflag;
    unsigned int c_oflag;
    unsigned int c_cflag;
    unsigned int c_lflag;
    unsigned char c_line;
    unsigned char c_cc[32];
    unsigned int c_ispeed;
    unsigned int c_ospeed;
  };

  typedef struct
{
  int __allocated;
  int __used;
  struct posix_spawn_file_actions_t *__actions;
  int __pad[16];
} posix_spawn_file_actions_t;

typedef struct re_pattern_buffer regex_t;
struct gcry_mpi;
typedef struct gcry_mpi *gcry_mpi_t;
typedef unsigned int gpg_error_t;
typedef gpg_error_t gcry_error_t;
typedef struct gcry_md_handle
{

  struct gcry_md_context *ctx;


  int bufpos;
  int bufsize;
  unsigned char buf[1];
} *gcry_md_hd_t;
struct gcry_cipher_handle;
typedef struct gcry_cipher_handle *gcry_cipher_hd_t;
typedef struct
  {
    size_t gl_pathc;
    char **gl_pathv;
    size_t gl_offs;
    int gl_flags;



    void (*gl_closedir) (void *);



    void *(*gl_readdir) (void *);

    void *(*gl_opendir) (const char *);




    int (*gl_lstat) (const char *, void *);
    int (*gl_stat) (const char *, void *);

  } glob_t;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/paths.h" 2
# 5 "../confpaths.h" 2
# 3128 "../config.h" 2
# 51 "ring.c" 2

# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/string.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/string.h" 2
# 53 "ring.c" 2
# 65 "ring.c"
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdio.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdio.h" 2
# 66 "ring.c" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/errno.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/errno.h" 2
# 67 "ring.c" 2





# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/types.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/types.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/types.h" 2
# 73 "ring.c" 2

# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/ioctl.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/ioctl.h" 2
# 75 "ring.c" 2

# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/socket.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/socket.h" 2
# 77 "ring.c" 2

# 1 "ring.h" 1
# 60 "ring.h"
typedef struct
{
  unsigned char *consume,
   *supply,
   *bottom,
   *top,
   *mark;




  int size;
  unsigned long consumetime,
    supplytime;
} Ring;




extern int ring_init (Ring * ring, unsigned char *buffer, int count);


extern void ring_supply_data (Ring * ring, unsigned char *buffer, int count);


extern void
ring_supplied (Ring * ring, int count),
ring_consumed (Ring * ring, int count);


extern int
ring_empty_count (Ring * ring),
ring_empty_consecutive (Ring * ring),
ring_full_count (Ring * ring), ring_full_consecutive (Ring * ring);






extern void ring_clear_mark (Ring *);
extern void ring_mark (Ring *);
extern int ring_at_mark (Ring *);
# 79 "ring.c" 2
# 1 "general.h" 1
# 80 "ring.c" 2
# 104 "ring.c"
static unsigned long ring_clock = 0;
# 118 "ring.c"
int
ring_init (Ring * ring, unsigned char *buffer, int count)
{
  memset ((char *) ring, 0, sizeof *ring);

  ring->size = count;

  ring->supply = ring->consume = ring->bottom = buffer;

  ring->top = ring->bottom + ring->size;





  return 1;
}







void
ring_mark (Ring * ring)
{
  ring->mark = (((ring->supply)-(1) >= (ring)->bottom)? (ring->supply)-(1) : (((ring->supply)-(1))-(ring)->size));
}





int
ring_at_mark (Ring * ring)
{
  if (ring->mark == ring->consume)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}





void
ring_clear_mark (Ring * ring)
{
  ring->mark = 0;
}




void
ring_supplied (Ring * ring, int count)
{
  ring->supply = (((ring->supply)+(count) < (ring)->top)? (ring->supply)+(count) : (((ring->supply)+(count))-(ring)->size));
  ring->supplytime = ++ring_clock;
}




void
ring_consumed (Ring * ring, int count)
{
  if (count == 0)
    return;

  if (ring->mark && ((((ring->mark)-(ring->consume) >= 0)? (ring->mark)-(ring->consume): (((ring->mark)-(ring->consume))+(ring)->size)) < count))
    {
      ring->mark = 0;
    }
# 206 "ring.c"
  ring->consume = (((ring->consume)+(count) < (ring)->top)? (ring->consume)+(count) : (((ring->consume)+(count))-(ring)->size));
  ring->consumetime = ++ring_clock;



  if ((((ring)->consume == (ring)->supply) && ((ring)->consumetime >= (ring)->supplytime)))
    {
      ring->consume = ring->supply = ring->bottom;
    }
}







int
ring_empty_count (Ring * ring)
{
  if ((((ring)->consume == (ring)->supply) && ((ring)->consumetime >= (ring)->supplytime)))
    {
      return ring->size;
    }
  else
    {
      return (((ring->consume)-(ring->supply) >= 0)? (ring->consume)-(ring->supply): (((ring->consume)-(ring->supply))+(ring)->size));
    }
}


int
ring_empty_consecutive (Ring * ring)
{
  if ((ring->consume < ring->supply) || (((ring)->consume == (ring)->supply) && ((ring)->consumetime >= (ring)->supplytime)))
    {




      return (((ring->top)-(ring->supply) >= 0)? (ring->top)-(ring->supply): (((ring->top)-(ring->supply))+(ring)->size));
    }
  else
    {



      return (((ring->consume)-(ring->supply) >= 0)? (ring->consume)-(ring->supply): (((ring->consume)-(ring->supply))+(ring)->size));
    }
}





int
ring_full_count (Ring * ring)
{
  if ((ring->mark == 0) || (ring->mark == ring->consume))
    {
      if ((((ring)->supply == (ring)->consume) && ((ring)->supplytime > (ring)->consumetime)))
 {
   return ring->size;
 }
      else
 {
   return (((ring->supply)-(ring->consume) >= 0)? (ring->supply)-(ring->consume): (((ring->supply)-(ring->consume))+(ring)->size));
 }
    }
  else
    {
      return (((ring->mark)-(ring->consume) >= 0)? (ring->mark)-(ring->consume): (((ring->mark)-(ring->consume))+(ring)->size));
    }
}





int
ring_full_consecutive (Ring * ring)
{
  if ((ring->mark == 0) || (ring->mark == ring->consume))
    {
      if ((ring->supply < ring->consume) || (((ring)->supply == (ring)->consume) && ((ring)->supplytime > (ring)->consumetime)))
 {
   return (((ring->top)-(ring->consume) >= 0)? (ring->top)-(ring->consume): (((ring->top)-(ring->consume))+(ring)->size));
 }
      else
 {
   return (((ring->supply)-(ring->consume) >= 0)? (ring->supply)-(ring->consume): (((ring->supply)-(ring->consume))+(ring)->size));
 }
    }
  else
    {
      if (ring->mark < ring->consume)
 {
   return (((ring->top)-(ring->consume) >= 0)? (ring->top)-(ring->consume): (((ring->top)-(ring->consume))+(ring)->size));
 }
      else
 {
   return (((ring->mark)-(ring->consume) >= 0)? (ring->mark)-(ring->consume): (((ring->mark)-(ring->consume))+(ring)->size));
 }
    }
}




void
ring_supply_data (Ring * ring, unsigned char *buffer, int count)
{
  int i;

  while (count)
    {
      i = (((count)<(ring_empty_consecutive (ring)))? (count):(ring_empty_consecutive (ring)));
      memmove (ring->supply, buffer, i);
      ring_supplied (ring, i);
      count -= i;
      buffer += i;
    }
}
