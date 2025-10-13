# 1 "terminal.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "terminal.c"
# 50 "terminal.c"
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
# 51 "terminal.c" 2

# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdlib.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdlib.h" 2
# 53 "terminal.c" 2

# 1 "/usr/include/arpa/telnet.h" 1 3 4
# 68 "/usr/include/arpa/telnet.h" 3 4

# 68 "/usr/include/arpa/telnet.h" 3 4
extern char *telcmds[];
# 210 "/usr/include/arpa/telnet.h" 3 4
extern char *slc_names[];
# 270 "/usr/include/arpa/telnet.h" 3 4
extern char *authtype_names[];
# 305 "/usr/include/arpa/telnet.h" 3 4
extern char *encrypt_names[];
extern char *enctype_names[];
# 55 "terminal.c" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/types.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/types.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/types.h" 2
# 56 "terminal.c" 2

# 1 "ring.h" 1
# 60 "ring.h"

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
# 58 "terminal.c" 2

# 1 "externs.h" 1
# 50 "externs.h"
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/unistd.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/unistd.h" 2
# 51 "externs.h" 2
# 72 "externs.h"
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdio.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdio.h" 2
# 73 "externs.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/setjmp.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/setjmp.h" 2
# 74 "externs.h" 2



# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/ioctl.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/ioctl.h" 2
# 78 "externs.h" 2



# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/errno.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/errno.h" 2
# 82 "externs.h" 2





# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/termios.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/termios.h" 2
# 88 "externs.h" 2
# 104 "externs.h"
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/string.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/string.h" 2
# 105 "externs.h" 2
# 122 "externs.h"
extern int autologin,
  skiprc,
  eight,
  flushout,
  connected,
  globalmode,
  In3270,
  telnetport,
  localflow,
  restartany,
  localchars,
  donelclchars,
  showoptions, net,
  tin,
  tout,
  crlf,
  autoflush,
  autosynch,
  SYNCHing,
  donebinarytoggle,
  dontlecho,
  crmod, netdata,
  prettydump,





  termdata,
  debug;

extern cc_t escape;
extern cc_t rlogin;

extern cc_t echoc;


extern char *prompt;

extern char doopt[], dont[], will[], wont[], options[],
 *hostname;
# 231 "externs.h"
extern FILE *NetTrace;
extern unsigned char NetTraceFile[];
extern void SetNetTrace (char *);

extern jmp_buf peerdied, toplevel;

extern void
command (int, char *, int),
Dump (char, unsigned char *, int),
init_3270 (void),
printoption (char *, int, int),
printsub (char, unsigned char *, int),
sendnaws (void),
setconnmode (int),
setcommandmode (void),
setneturg (void),
sys_telnet_init (void),
telnet (char *),
tel_enter_binary (int),
tel_leave_binary (int),
TerminalFlushOutput (void),
TerminalNewMode (int),
TerminalRestoreState (void),
TerminalSaveState (void),
tninit (void), upcase (char *), willoption (int), wontoption (int);

extern int TerminalWindowSize (long *rows, long *cols);
extern void TerminalSpeeds (long *ispeed, long *ospeed);
extern void TerminalDefaultChars (void);
extern int TerminalSpecialChars (int c);
extern int TerminalAutoFlush (void);
extern int TerminalWrite (char *buf, int n);

extern int telrcv (void);
extern int getconnmode (void);
extern void optionstatus (void);
extern int NetClose (int);
extern void Exit (int);
extern void ExitString (char *, int);
extern int netflush (void);
extern int opt_welldefined (char *);
extern int stilloob (void);
extern int process_rings (int, int, int, int, int, int);

extern void init_terminal (void);
extern void init_network (void);
extern void init_telnet (void);
extern void init_sys (void);

extern void set_escape_char (char*);
extern int tn (int argc, char **argv);

extern void
send_do (int, int),
send_dont (int, int), send_will (int, int), send_wont (int, int);

extern void sendabort (void);
extern void sendeof (void);
extern void sendsusp (void);
extern int ttyflush (int);
extern int rlogin_susp (void);
extern void env_init (void);


int SetSockOpt (int fd, int level, int option, int yesno);

extern int quit (void);

extern void
lm_will (unsigned char *, int),
lm_wont (unsigned char *, int),
lm_do (unsigned char *, int),
lm_dont (unsigned char *, int), lm_mode (unsigned char *, int, int);

extern void
slc_init (void),
slcstate (void),
slc_mode_export (void),
slc_mode_import (int),
slc_import (int),
slc_export (void),
slc (unsigned char *, int),
slc_check (void),
slc_start_reply (void),
slc_add_reply (unsigned char, unsigned char, cc_t), slc_end_reply (void);
extern int slc_update (void);

extern void
env_opt (unsigned char *, int),
env_opt_start (void),
env_opt_start_info (void), env_opt_add (unsigned char *), env_opt_end (int);

extern unsigned char *env_default (int, int), *env_getvalue (const char *);

extern int get_status (void), dosynch (void);

extern cc_t *tcval (int);
# 369 "externs.h"
extern struct termios new_tc;
# 378 "externs.h"
extern cc_t termSuspChar;







extern cc_t termFlushChar;




extern cc_t termWerasChar;




extern cc_t termRprntChar;




extern cc_t termLiteralNextChar;




extern cc_t termStartChar;




extern cc_t termStopChar;




extern cc_t termForw1Char;




extern cc_t termForw2Char;




extern cc_t termAytChar;
# 470 "externs.h"
extern Ring netoring, netiring, ttyoring, ttyiring;
# 60 "terminal.c" 2
# 1 "types.h" 1
# 50 "types.h"
typedef struct
{
  char *modedescriptions;
  char modetype;
} Modelist;

extern Modelist modelist[];

typedef struct
{
  int system,
    echotoggle,
    modenegotiated,
    didnetreceive,
    gotDM;
} Clocks;

extern Clocks clocks;
# 61 "terminal.c" 2





Ring ttyoring, ttyiring;
unsigned char ttyobuf[2 * 1024], ttyibuf[1024];

int termdata;



cc_t termFlushChar;


cc_t termLiteralNextChar;


cc_t termSuspChar;


cc_t termWerasChar;


cc_t termRprntChar;


cc_t termStartChar;


cc_t termStopChar;


cc_t termForw1Char;


cc_t termForw2Char;


cc_t termAytChar;
# 111 "terminal.c"
void
init_terminal (void)
{
  if (ring_init (&ttyoring, ttyobuf, sizeof ttyobuf) != 1)
    {
      exit (1);
    }
  if (ring_init (&ttyiring, ttyibuf, sizeof ttyibuf) != 1)
    {
      exit (1);
    }
  autoflush = TerminalAutoFlush ();
}
# 137 "terminal.c"
int
ttyflush (int drop)
{
  register int n, n0, n1;

  n0 = ring_full_count (&ttyoring);
  if ((n1 = n = ring_full_consecutive (&ttyoring)) > 0)
    {
      if (drop)
 {
   TerminalFlushOutput ();

 }
      else
 {
   n = TerminalWrite ((char *) ttyoring.consume, n);
 }
    }
  if (n > 0)
    {
      if (termdata && n)
 {
   Dump ('>', ttyoring.consume, n);
 }





      if (n1 == n && n0 > n)
 {
   n1 = n0 - n;
   if (!drop)
     n1 = TerminalWrite ((char *) ttyoring.bottom, n1);
   if (n1 > 0)
     n += n1;
 }
      ring_consumed (&ttyoring, n);
    }
  if (n < 0)
    return -1;
  if (n == n0)
    {
      if (n0)
 return -1;
      return 0;
    }
  return n0 - n + 1;
}
# 194 "terminal.c"
int
getconnmode (void)
{
  extern int linemode;
  int mode = 0;

  extern int kludgelinemode;


  if (In3270)
    return (
# 204 "terminal.c" 3 4
           0x0100
# 204 "terminal.c"
                    );

  if ((!(options[
# 206 "terminal.c" 3 4
     1
# 206 "terminal.c"
     ]&0x08)))
    mode |= 
# 207 "terminal.c" 3 4
           0x0200
# 207 "terminal.c"
                    ;

  if (localflow)
    mode |= 
# 210 "terminal.c" 3 4
           0x0100
# 210 "terminal.c"
                    ;

  if ((options[
# 212 "terminal.c" 3 4
     0
# 212 "terminal.c"
     ]&0x02))
    mode |= 
# 213 "terminal.c" 3 4
           0x0400
# 213 "terminal.c"
                     ;

  if ((options[
# 215 "terminal.c" 3 4
     0
# 215 "terminal.c"
     ]&0x08))
    mode |= 
# 216 "terminal.c" 3 4
           0x0800
# 216 "terminal.c"
                      ;


  if (kludgelinemode)
    {
      if ((!(options[
# 221 "terminal.c" 3 4
         3
# 221 "terminal.c"
         ]&0x08)))
 {
   mode |= (
# 223 "terminal.c" 3 4
           0x02 
# 223 "terminal.c"
                        | 
# 223 "terminal.c" 3 4
                          0x01
# 223 "terminal.c"
                                   );
   if (dontlecho && (clocks.echotoggle > clocks.modenegotiated))
     {
       mode &= ~
# 226 "terminal.c" 3 4
               0x0200
# 226 "terminal.c"
                        ;
     }
 }
      return (mode);
    }

  if ((options[
# 232 "terminal.c" 3 4
     34
# 232 "terminal.c"
     ]&0x02))
    mode |= linemode;
  return (mode);
}

void
setconnmode (int force)
{



  register int newmode;

  newmode = getconnmode () | (force ? 
# 245 "terminal.c" 3 4
                                     0x1000 
# 245 "terminal.c"
                                                : 0);

  TerminalNewMode (newmode);
# 269 "terminal.c"
}


void
setcommandmode (void)
{
  TerminalNewMode (-1);
}
