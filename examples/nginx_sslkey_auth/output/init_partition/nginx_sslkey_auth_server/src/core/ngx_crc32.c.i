# 1 "src/core/ngx_crc32.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "src/core/ngx_crc32.c"







# 1 "src/core/ngx_config.h" 1
# 12 "src/core/ngx_config.h"
# 1 "objs/ngx_auto_headers.h" 1
# 13 "src/core/ngx_config.h" 2
# 26 "src/core/ngx_config.h"
# 1 "src/os/unix/ngx_linux_config.h" 1
# 18 "src/os/unix/ngx_linux_config.h"
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/types.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/types.h" 2
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




typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/types.h" 2
# 19 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/time.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/time.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/time.h" 2
# 20 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/unistd.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/unistd.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/unistd.h" 2
# 21 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdarg.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdarg.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdarg.h" 2
# 22 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stddef.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stddef.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stddef.h" 2
# 23 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdio.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdio.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdio.h" 2
# 24 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdlib.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdlib.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/stdlib.h" 2
# 25 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/ctype.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/ctype.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/ctype.h" 2
# 26 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/errno.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/errno.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/errno.h" 2
# 27 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/string.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/string.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/string.h" 2
# 28 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/signal.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/signal.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/signal.h" 2
# 29 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/pwd.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/pwd.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 3 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/pwd.h" 2
struct passwd
{
  char *pw_name;
  char *pw_passwd;

  __uid_t pw_uid;
  __gid_t pw_gid;
  char *pw_gecos;
  char *pw_dir;
  char *pw_shell;
};
# 30 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/grp.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/grp.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/grp.h" 2
# 31 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/dirent.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/dirent.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/dirent.h" 2
# 32 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/glob.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/glob.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/glob.h" 2
# 33 "src/os/unix/ngx_linux_config.h" 2
# 1 "/usr/include/x86_64-linux-gnu/sys/vfs.h" 1 3 4



# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/statfs.h" 1 3 4
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1 3 4
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/statfs.h" 2 3 4
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1 3 4
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 3 4

# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 3 4
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/statfs.h" 2 3 4
# 4 "/usr/include/x86_64-linux-gnu/sys/vfs.h" 2 3 4
# 34 "src/os/unix/ngx_linux_config.h" 2

# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/uio.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/uio.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"

# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/uio.h" 2
# 36 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/stat.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/stat.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/stat.h" 2
# 37 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/fcntl.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/fcntl.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/fcntl.h" 2
# 38 "src/os/unix/ngx_linux_config.h" 2

# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/wait.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/wait.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/wait.h" 2
# 40 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/mman.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/mman.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/mman.h" 2
# 41 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/resource.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/resource.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/resource.h" 2
# 42 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sched.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sched.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sched.h" 2
# 43 "src/os/unix/ngx_linux_config.h" 2

# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/socket.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/socket.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/socket.h" 2
# 45 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/netinet/in.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/netinet/in.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/netinet/in.h" 2
# 46 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/netinet/tcp.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/netinet/tcp.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/netinet/tcp.h" 2
# 47 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/arpa/inet.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/arpa/inet.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/arpa/inet.h" 2
# 48 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/netdb.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/netdb.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/netdb.h" 2
# 49 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/un.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/un.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/un.h" 2
# 50 "src/os/unix/ngx_linux_config.h" 2

# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/time.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/time.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/time.h" 2
# 52 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/malloc.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/malloc.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/malloc.h" 2
# 53 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/limits.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/limits.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/limits.h" 2
# 54 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/ioctl.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/ioctl.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/ioctl.h" 2
# 55 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/crypt.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/crypt.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/crypt.h" 2
# 56 "src/os/unix/ngx_linux_config.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/utsname.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/utsname.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/utsname.h" 2
# 57 "src/os/unix/ngx_linux_config.h" 2

# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/dlfcn.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/dlfcn.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/dlfcn.h" 2
# 59 "src/os/unix/ngx_linux_config.h" 2


# 1 "objs/ngx_auto_config.h" 1
# 62 "src/os/unix/ngx_linux_config.h" 2



# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/semaphore.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/semaphore.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/semaphore.h" 2
# 66 "src/os/unix/ngx_linux_config.h" 2




# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/prctl.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/prctl.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/prctl.h" 2
# 71 "src/os/unix/ngx_linux_config.h" 2




# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/sendfile.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/sendfile.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/sendfile.h" 2
# 76 "src/os/unix/ngx_linux_config.h" 2
# 88 "src/os/unix/ngx_linux_config.h"
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/epoll.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/epoll.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/epoll.h" 2
# 89 "src/os/unix/ngx_linux_config.h" 2




# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/eventfd.h" 1
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/eventfd.h" 2
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h"
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/sys/eventfd.h" 2
# 94 "src/os/unix/ngx_linux_config.h" 2

# 1 "/usr/include/x86_64-linux-gnu/sys/syscall.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/sys/syscall.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/unistd.h" 1 3 4
# 20 "/usr/include/x86_64-linux-gnu/asm/unistd.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/asm/unistd_64.h" 1 3 4
# 21 "/usr/include/x86_64-linux-gnu/asm/unistd.h" 2 3 4
# 25 "/usr/include/x86_64-linux-gnu/sys/syscall.h" 2 3 4




# 1 "/usr/include/x86_64-linux-gnu/bits/syscall.h" 1 3 4
# 30 "/usr/include/x86_64-linux-gnu/sys/syscall.h" 2 3 4
# 96 "src/os/unix/ngx_linux_config.h" 2







# 1 "/usr/include/linux/capability.h" 1 3 4
# 17 "/usr/include/linux/capability.h" 3 4
# 1 "/usr/include/linux/types.h" 1 3 4




# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/asm/types.h" 1 3 4
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_defines.h" 1 3 4
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/asm/types.h" 2 3 4
# 1 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 1 3 4
# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 3 4

# 307 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/_fake_typedefs.h" 3 4
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_session_st SSL_SESSION;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct rsa_st RSA;
typedef struct asn1_string_st ASN1_INTEGER;
typedef struct asn1_string_st ASN1_ENUMERATED;
typedef struct asn1_string_st ASN1_BIT_STRING;
typedef struct asn1_string_st ASN1_OCTET_STRING;
typedef struct asn1_string_st ASN1_PRINTABLESTRING;
typedef struct asn1_string_st ASN1_T61STRING;
typedef struct asn1_string_st ASN1_IA5STRING;
typedef struct asn1_string_st ASN1_GENERALSTRING;
typedef struct asn1_string_st ASN1_UNIVERSALSTRING;
typedef struct asn1_string_st ASN1_BMPSTRING;
typedef struct asn1_string_st ASN1_UTCTIME;
typedef struct asn1_string_st ASN1_TIME;
typedef struct asn1_string_st ASN1_GENERALIZEDTIME;
typedef struct asn1_string_st ASN1_VISIBLESTRING;
typedef struct asn1_string_st ASN1_UTF8STRING;
typedef struct asn1_string_st ASN1_STRING;
typedef int ASN1_BOOLEAN;
typedef int ASN1_NULL;
# 2 "/home/raoxue/Desktop/IF-driver-partition/FIPA/scripts/fake_libc_include/asm/types.h" 2 3 4
# 6 "/usr/include/linux/types.h" 2 3 4



# 1 "/usr/include/linux/posix_types.h" 1 3 4




# 1 "/usr/include/linux/stddef.h" 1 3 4
# 6 "/usr/include/linux/posix_types.h" 2 3 4
# 25 "/usr/include/linux/posix_types.h" 3 4
typedef struct {
 unsigned long fds_bits[1024 / (8 * sizeof(long))];
} __kernel_fd_set;


typedef void (*__kernel_sighandler_t)(int);


typedef int __kernel_key_t;
typedef int __kernel_mqd_t;

# 1 "/usr/include/x86_64-linux-gnu/asm/posix_types.h" 1 3 4






# 1 "/usr/include/x86_64-linux-gnu/asm/posix_types_64.h" 1 3 4
# 11 "/usr/include/x86_64-linux-gnu/asm/posix_types_64.h" 3 4
typedef unsigned short __kernel_old_uid_t;
typedef unsigned short __kernel_old_gid_t;


typedef unsigned long __kernel_old_dev_t;


# 1 "/usr/include/asm-generic/posix_types.h" 1 3 4




# 1 "/usr/include/x86_64-linux-gnu/asm/bitsperlong.h" 1 3 4
# 11 "/usr/include/x86_64-linux-gnu/asm/bitsperlong.h" 3 4
# 1 "/usr/include/asm-generic/bitsperlong.h" 1 3 4
# 12 "/usr/include/x86_64-linux-gnu/asm/bitsperlong.h" 2 3 4
# 6 "/usr/include/asm-generic/posix_types.h" 2 3 4
# 15 "/usr/include/asm-generic/posix_types.h" 3 4
typedef long __kernel_long_t;
typedef unsigned long __kernel_ulong_t;



typedef __kernel_ulong_t __kernel_ino_t;



typedef unsigned int __kernel_mode_t;



typedef int __kernel_pid_t;



typedef int __kernel_ipc_pid_t;



typedef unsigned int __kernel_uid_t;
typedef unsigned int __kernel_gid_t;



typedef __kernel_long_t __kernel_suseconds_t;



typedef int __kernel_daddr_t;



typedef unsigned int __kernel_uid32_t;
typedef unsigned int __kernel_gid32_t;
# 72 "/usr/include/asm-generic/posix_types.h" 3 4
typedef __kernel_ulong_t __kernel_size_t;
typedef __kernel_long_t __kernel_ssize_t;
typedef __kernel_long_t __kernel_ptrdiff_t;




typedef struct {
 int val[2];
} __kernel_fsid_t;





typedef __kernel_long_t __kernel_off_t;
typedef long long __kernel_loff_t;
typedef __kernel_long_t __kernel_time_t;
typedef long long __kernel_time64_t;
typedef __kernel_long_t __kernel_clock_t;
typedef int __kernel_timer_t;
typedef int __kernel_clockid_t;
typedef char * __kernel_caddr_t;
typedef unsigned short __kernel_uid16_t;
typedef unsigned short __kernel_gid16_t;
# 19 "/usr/include/x86_64-linux-gnu/asm/posix_types_64.h" 2 3 4
# 8 "/usr/include/x86_64-linux-gnu/asm/posix_types.h" 2 3 4
# 37 "/usr/include/linux/posix_types.h" 2 3 4
# 10 "/usr/include/linux/types.h" 2 3 4
# 24 "/usr/include/linux/types.h" 3 4
typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __le32;
typedef __u32 __be32;
typedef __u64 __le64;
typedef __u64 __be64;

typedef __u16 __sum16;
typedef __u32 __wsum;
# 47 "/usr/include/linux/types.h" 3 4
typedef unsigned __poll_t;
# 18 "/usr/include/linux/capability.h" 2 3 4
# 39 "/usr/include/linux/capability.h" 3 4
typedef struct __user_cap_header_struct {
 __u32 version;
 int pid;
} *cap_user_header_t;

typedef struct __user_cap_data_struct {
        __u32 effective;
        __u32 permitted;
        __u32 inheritable;
} *cap_user_data_t;
# 72 "/usr/include/linux/capability.h" 3 4
struct vfs_cap_data {
 __le32 magic_etc;
 struct {
  __le32 permitted;
  __le32 inheritable;
 } data[2];
};




struct vfs_ns_cap_data {
 __le32 magic_etc;
 struct {
  __le32 permitted;
  __le32 inheritable;
 } data[2];
 __le32 rootid;
};
# 104 "src/os/unix/ngx_linux_config.h" 2
# 125 "src/os/unix/ngx_linux_config.h"

# 125 "src/os/unix/ngx_linux_config.h"
extern char **environ;
# 27 "src/core/ngx_config.h" 2
# 78 "src/core/ngx_config.h"
typedef intptr_t ngx_int_t;
typedef uintptr_t ngx_uint_t;
typedef intptr_t ngx_flag_t;
# 9 "src/core/ngx_crc32.c" 2
# 1 "src/core/ngx_core.h" 1
# 15 "src/core/ngx_core.h"
typedef struct ngx_module_s ngx_module_t;
typedef struct ngx_conf_s ngx_conf_t;
typedef struct ngx_cycle_s ngx_cycle_t;
typedef struct ngx_pool_s ngx_pool_t;
typedef struct ngx_chain_s ngx_chain_t;
typedef struct ngx_log_s ngx_log_t;
typedef struct ngx_open_file_s ngx_open_file_t;
typedef struct ngx_command_s ngx_command_t;
typedef struct ngx_file_s ngx_file_t;
typedef struct ngx_event_s ngx_event_t;
typedef struct ngx_event_aio_s ngx_event_aio_t;
typedef struct ngx_connection_s ngx_connection_t;
typedef struct ngx_thread_task_s ngx_thread_task_t;
typedef struct ngx_ssl_s ngx_ssl_t;
typedef struct ngx_ssl_connection_s ngx_ssl_connection_t;
typedef struct ngx_udp_connection_s ngx_udp_connection_t;

typedef void (*ngx_event_handler_pt)(ngx_event_t *ev);
typedef void (*ngx_connection_handler_pt)(ngx_connection_t *c);
# 45 "src/core/ngx_core.h"
# 1 "src/os/unix/ngx_errno.h" 1
# 13 "src/os/unix/ngx_errno.h"
# 1 "src/core/ngx_core.h" 1
# 14 "src/os/unix/ngx_errno.h" 2


typedef int ngx_err_t;
# 75 "src/os/unix/ngx_errno.h"
u_char *ngx_strerror(ngx_err_t err, u_char *errstr, size_t size);
ngx_int_t ngx_strerror_init(void);
# 46 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_atomic.h" 1
# 97 "src/os/unix/ngx_atomic.h"
typedef long ngx_atomic_int_t;
typedef unsigned long ngx_atomic_uint_t;







typedef volatile ngx_atomic_uint_t ngx_atomic_t;
# 307 "src/os/unix/ngx_atomic.h"
void ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin);
# 47 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_thread.h" 1
# 48 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_rbtree.h" 1
# 16 "src/core/ngx_rbtree.h"
typedef ngx_uint_t ngx_rbtree_key_t;
typedef ngx_int_t ngx_rbtree_key_int_t;


typedef struct ngx_rbtree_node_s ngx_rbtree_node_t;

struct ngx_rbtree_node_s {
    ngx_rbtree_key_t key;
    ngx_rbtree_node_t *left;
    ngx_rbtree_node_t *right;
    ngx_rbtree_node_t *parent;
    u_char color;
    u_char data;
};


typedef struct ngx_rbtree_s ngx_rbtree_t;

typedef void (*ngx_rbtree_insert_pt) (ngx_rbtree_node_t *root,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);

struct ngx_rbtree_s {
    ngx_rbtree_node_t *root;
    ngx_rbtree_node_t *sentinel;
    ngx_rbtree_insert_pt insert;
};
# 51 "src/core/ngx_rbtree.h"
void ngx_rbtree_insert(ngx_rbtree_t *tree, ngx_rbtree_node_t *node);
void ngx_rbtree_delete(ngx_rbtree_t *tree, ngx_rbtree_node_t *node);
void ngx_rbtree_insert_value(ngx_rbtree_node_t *root, ngx_rbtree_node_t *node,
    ngx_rbtree_node_t *sentinel);
void ngx_rbtree_insert_timer_value(ngx_rbtree_node_t *root,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
ngx_rbtree_node_t *ngx_rbtree_next(ngx_rbtree_t *tree,
    ngx_rbtree_node_t *node);
# 73 "src/core/ngx_rbtree.h"
static inline ngx_rbtree_node_t *
ngx_rbtree_min(ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}
# 49 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_time.h" 1
# 16 "src/os/unix/ngx_time.h"
typedef ngx_rbtree_key_t ngx_msec_t;
typedef ngx_rbtree_key_int_t ngx_msec_int_t;

typedef struct tm ngx_tm_t;
# 56 "src/os/unix/ngx_time.h"
void ngx_timezone_update(void);
void ngx_localtime(time_t s, ngx_tm_t *tm);
void ngx_libc_localtime(time_t s, struct tm *tm);
void ngx_libc_gmtime(time_t s, struct tm *tm);
# 50 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_socket.h" 1
# 17 "src/os/unix/ngx_socket.h"
typedef int ngx_socket_t;







int ngx_nonblocking(ngx_socket_t s);
int ngx_blocking(ngx_socket_t s);
# 41 "src/os/unix/ngx_socket.h"
int ngx_tcp_nopush(ngx_socket_t s);
int ngx_tcp_push(ngx_socket_t s);
# 51 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_string.h" 1
# 16 "src/core/ngx_string.h"
typedef struct {
    size_t len;
    u_char *data;
} ngx_str_t;


typedef struct {
    ngx_str_t key;
    ngx_str_t value;
} ngx_keyval_t;


typedef struct {
    unsigned len:28;

    unsigned valid:1;
    unsigned no_cacheable:1;
    unsigned not_found:1;
    unsigned escape:1;

    u_char *data;
} ngx_variable_value_t;
# 50 "src/core/ngx_string.h"
void ngx_strlow(u_char *dst, u_char *src, size_t n);
# 63 "src/core/ngx_string.h"
size_t ngx_strnlen(u_char *p, size_t n);



static inline u_char *
ngx_strlchr(u_char *p, u_char *last, u_char c)
{
    while (p < last) {

        if (*p == c) {
            return p;
        }

        p++;
    }

    return 0;
}
# 149 "src/core/ngx_string.h"
u_char *ngx_cpystrn(u_char *dst, u_char *src, size_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_sprintf(u_char *buf, const char *fmt, ...);
u_char * ngx_snprintf(u_char *buf, size_t max, const char *fmt, ...);
u_char * ngx_slprintf(u_char *buf, u_char *last, const char *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *last, const char *fmt, va_list args);



ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, size_t n);

u_char *ngx_strnstr(u_char *s1, char *s2, size_t n);

u_char *ngx_strstrn(u_char *s1, char *s2, size_t n);
u_char *ngx_strcasestrn(u_char *s1, char *s2, size_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, size_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, size_t n);

ngx_int_t ngx_atoi(u_char *line, size_t n);
ngx_int_t ngx_atofp(u_char *line, size_t n, size_t point);
ssize_t ngx_atosz(u_char *line, size_t n);
off_t ngx_atoof(u_char *line, size_t n);
time_t ngx_atotm(u_char *line, size_t n);
ngx_int_t ngx_hextoi(u_char *line, size_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, size_t len);





void ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
void ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, size_t n);
size_t ngx_utf8_length(u_char *p, size_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len);
# 208 "src/core/ngx_string.h"
uintptr_t ngx_escape_uri(u_char *dst, u_char *src, size_t size,
    ngx_uint_t type);
void ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, size_t size);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, size_t size);


typedef struct {
    ngx_rbtree_node_t node;
    ngx_str_t str;
} ngx_str_node_t;


void ngx_str_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
ngx_str_node_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


void ngx_sort(void *base, size_t n, size_t size,
    ngx_int_t (*cmp)(const void *, const void *));
# 52 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_files.h" 1
# 16 "src/os/unix/ngx_files.h"
typedef int ngx_fd_t;
typedef struct stat ngx_file_info_t;
typedef ino_t ngx_file_uniq_t;


typedef struct {
    u_char *name;
    size_t size;
    void *addr;
    ngx_fd_t fd;
    ngx_log_t *log;
} ngx_file_mapping_t;


typedef struct {
    DIR *dir;
    struct dirent *de;
    struct stat info;

    unsigned type:8;
    unsigned valid_info:1;
} ngx_dir_t;


typedef struct {
    size_t n;
    glob_t pglob;
    u_char *pattern;
    ngx_log_t *log;
    ngx_uint_t test;
} ngx_glob_t;
# 117 "src/os/unix/ngx_files.h"
ngx_fd_t ngx_open_tempfile(u_char *name, ngx_uint_t persistent,
    ngx_uint_t access);



ssize_t ngx_read_file(ngx_file_t *file, u_char *buf, size_t size, off_t offset);






ssize_t ngx_write_file(ngx_file_t *file, u_char *buf, size_t size,
    off_t offset);

ssize_t ngx_write_chain_to_file(ngx_file_t *file, ngx_chain_t *ce,
    off_t offset, ngx_pool_t *pool);
# 144 "src/os/unix/ngx_files.h"
static inline ssize_t
ngx_write_fd(ngx_fd_t fd, void *buf, size_t n)
{
    return write(fd, buf, n);
}
# 169 "src/os/unix/ngx_files.h"
ngx_int_t ngx_set_file_time(u_char *name, ngx_fd_t fd, time_t s);
# 193 "src/os/unix/ngx_files.h"
ngx_int_t ngx_create_file_mapping(ngx_file_mapping_t *fm);
void ngx_close_file_mapping(ngx_file_mapping_t *fm);
# 219 "src/os/unix/ngx_files.h"
ngx_int_t ngx_open_dir(ngx_str_t *name, ngx_dir_t *dir);







ngx_int_t ngx_read_dir(ngx_dir_t *dir);
# 249 "src/os/unix/ngx_files.h"
static inline ngx_int_t
ngx_de_info(u_char *name, ngx_dir_t *dir)
{
    dir->type = 0;
    return stat((const char *) name, &dir->info);
}
# 289 "src/os/unix/ngx_files.h"
ngx_int_t ngx_open_glob(ngx_glob_t *gl);

ngx_int_t ngx_read_glob(ngx_glob_t *gl, ngx_str_t *name);
void ngx_close_glob(ngx_glob_t *gl);


ngx_err_t ngx_trylock_fd(ngx_fd_t fd);
ngx_err_t ngx_lock_fd(ngx_fd_t fd);
ngx_err_t ngx_unlock_fd(ngx_fd_t fd);
# 315 "src/os/unix/ngx_files.h"
ngx_int_t ngx_read_ahead(ngx_fd_t fd, size_t n);
# 328 "src/os/unix/ngx_files.h"
ngx_int_t ngx_directio_on(ngx_fd_t fd);


ngx_int_t ngx_directio_off(ngx_fd_t fd);
# 351 "src/os/unix/ngx_files.h"
size_t ngx_fs_bsize(u_char *name);
# 53 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_shmem.h" 1
# 16 "src/os/unix/ngx_shmem.h"
typedef struct {
    u_char *addr;
    size_t size;
    ngx_str_t name;
    ngx_log_t *log;
    ngx_uint_t exists;
} ngx_shm_t;


ngx_int_t ngx_shm_alloc(ngx_shm_t *shm);
void ngx_shm_free(ngx_shm_t *shm);
# 54 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_process.h" 1
# 12 "src/os/unix/ngx_process.h"
# 1 "src/os/unix/ngx_setaffinity.h" 1
# 16 "src/os/unix/ngx_setaffinity.h"
typedef cpu_set_t ngx_cpuset_t;
# 26 "src/os/unix/ngx_setaffinity.h"
void ngx_setaffinity(ngx_cpuset_t *cpu_affinity, ngx_log_t *log);
# 13 "src/os/unix/ngx_process.h" 2
# 1 "src/os/unix/ngx_setproctitle.h" 1
# 37 "src/os/unix/ngx_setproctitle.h"
ngx_int_t ngx_init_setproctitle(ngx_log_t *log);
void ngx_setproctitle(char *title);
# 14 "src/os/unix/ngx_process.h" 2


typedef pid_t ngx_pid_t;



typedef void (*ngx_spawn_proc_pt) (ngx_cycle_t *cycle, void *data);

typedef struct {
    ngx_pid_t pid;
    int status;
    ngx_socket_t channel[2];

    ngx_spawn_proc_pt proc;
    void *data;
    char *name;

    unsigned respawn:1;
    unsigned just_spawn:1;
    unsigned detached:1;
    unsigned exiting:1;
    unsigned exited:1;
} ngx_process_t;


typedef struct {
    char *path;
    char *name;
    char *const *argv;
    char *const *envp;
} ngx_exec_ctx_t;
# 64 "src/os/unix/ngx_process.h"
ngx_pid_t ngx_spawn_process(ngx_cycle_t *cycle,
    ngx_spawn_proc_pt proc, void *data, char *name, ngx_int_t respawn);
ngx_pid_t ngx_execute(ngx_cycle_t *cycle, ngx_exec_ctx_t *ctx);
ngx_int_t ngx_init_signals(ngx_log_t *log);
void ngx_debug_point(void);
# 78 "src/os/unix/ngx_process.h"
extern int ngx_argc;
extern char **ngx_argv;
extern char **ngx_os_argv;

extern ngx_pid_t ngx_pid;
extern ngx_pid_t ngx_parent;
extern ngx_socket_t ngx_channel;
extern ngx_int_t ngx_process_slot;
extern ngx_int_t ngx_last_process;
extern ngx_process_t ngx_processes[1024];
# 55 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_user.h" 1
# 16 "src/os/unix/ngx_user.h"
typedef uid_t ngx_uid_t;
typedef gid_t ngx_gid_t;


ngx_int_t ngx_libc_crypt(ngx_pool_t *pool, u_char *key, u_char *salt,
    u_char **encrypted);
# 56 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_dlopen.h" 1
# 27 "src/os/unix/ngx_dlopen.h"
char *ngx_dlerror(void);
# 57 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_parse.h" 1
# 16 "src/core/ngx_parse.h"
ssize_t ngx_parse_size(ngx_str_t *line);
off_t ngx_parse_offset(ngx_str_t *line);
ngx_int_t ngx_parse_time(ngx_str_t *line, ngx_uint_t is_sec);
# 58 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_parse_time.h" 1
# 16 "src/core/ngx_parse_time.h"
time_t ngx_parse_http_time(u_char *value, size_t len);
# 59 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_log.h" 1
# 45 "src/core/ngx_log.h"
typedef u_char *(*ngx_log_handler_pt) (ngx_log_t *log, u_char *buf, size_t len);
typedef void (*ngx_log_writer_pt) (ngx_log_t *log, ngx_uint_t level,
    u_char *buf, size_t len);


struct ngx_log_s {
    ngx_uint_t log_level;
    ngx_open_file_t *file;

    ngx_atomic_uint_t connection;

    time_t disk_full_time;

    ngx_log_handler_pt handler;
    void *data;

    ngx_log_writer_pt writer;
    void *wdata;







    char *action;

    ngx_log_t *next;
};
# 88 "src/core/ngx_log.h"
void ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    const char *fmt, ...);
# 231 "src/core/ngx_log.h"
ngx_log_t *ngx_log_init(u_char *prefix);
void ngx_log_abort(ngx_err_t err, const char *fmt, ...);
void ngx_log_stderr(ngx_err_t err, const char *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err);
ngx_int_t ngx_log_open_default(ngx_cycle_t *cycle);
ngx_int_t ngx_log_redirect_stderr(ngx_cycle_t *cycle);
ngx_log_t *ngx_log_get_file_log(ngx_log_t *head);
char *ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head);
# 250 "src/core/ngx_log.h"
static inline void
ngx_write_stderr(char *text)
{
    (void) ngx_write_fd(STDERR_FILENO, text, strlen((const char *) text));
}


static inline void
ngx_write_stdout(char *text)
{
    (void) ngx_write_fd(STDOUT_FILENO, text, strlen((const char *) text));
}


extern ngx_module_t ngx_errlog_module;
extern ngx_uint_t ngx_use_stderr;
# 60 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_alloc.h" 1
# 16 "src/os/unix/ngx_alloc.h"
void *ngx_alloc(size_t size, ngx_log_t *log);
void *ngx_calloc(size_t size, ngx_log_t *log);
# 31 "src/os/unix/ngx_alloc.h"
void *ngx_memalign(size_t alignment, size_t size, ngx_log_t *log);
# 40 "src/os/unix/ngx_alloc.h"
extern ngx_uint_t ngx_pagesize;
extern ngx_uint_t ngx_pagesize_shift;
extern ngx_uint_t ngx_cacheline_size;
# 61 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_palloc.h" 1
# 30 "src/core/ngx_palloc.h"
typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s ngx_pool_cleanup_t;

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt handler;
    void *data;
    ngx_pool_cleanup_t *next;
};


typedef struct ngx_pool_large_s ngx_pool_large_t;

struct ngx_pool_large_s {
    ngx_pool_large_t *next;
    void *alloc;
};


typedef struct {
    u_char *last;
    u_char *end;
    ngx_pool_t *next;
    ngx_uint_t failed;
} ngx_pool_data_t;


struct ngx_pool_s {
    ngx_pool_data_t d;
    size_t max;
    ngx_pool_t *current;
    ngx_chain_t *chain;
    ngx_pool_large_t *large;
    ngx_pool_cleanup_t *cleanup;
    ngx_log_t *log;
};


typedef struct {
    ngx_fd_t fd;
    u_char *name;
    ngx_log_t *log;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
void ngx_destroy_pool(ngx_pool_t *pool);
void ngx_reset_pool(ngx_pool_t *pool);

void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);
# 62 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_buf.h" 1
# 16 "src/core/ngx_buf.h"
typedef void * ngx_buf_tag_t;

typedef struct ngx_buf_s ngx_buf_t;

struct ngx_buf_s {
    u_char *pos;
    u_char *last;
    off_t file_pos;
    off_t file_last;

    u_char *start;
    u_char *end;
    ngx_buf_tag_t tag;
    ngx_file_t *file;
    ngx_buf_t *shadow;



    unsigned temporary:1;





    unsigned memory:1;


    unsigned mmap:1;

    unsigned recycled:1;
    unsigned in_file:1;
    unsigned flush:1;
    unsigned sync:1;
    unsigned last_buf:1;
    unsigned last_in_chain:1;

    unsigned last_shadow:1;
    unsigned temp_file:1;

               int num;
};


struct ngx_chain_s {
    ngx_buf_t *buf;
    ngx_chain_t *next;
};


typedef struct {
    ngx_int_t num;
    size_t size;
} ngx_bufs_t;


typedef struct ngx_output_chain_ctx_s ngx_output_chain_ctx_t;

typedef ngx_int_t (*ngx_output_chain_filter_pt)(void *ctx, ngx_chain_t *in);

typedef void (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);

struct ngx_output_chain_ctx_s {
    ngx_buf_t *buf;
    ngx_chain_t *in;
    ngx_chain_t *free;
    ngx_chain_t *busy;

    unsigned sendfile:1;
    unsigned directio:1;
    unsigned unaligned:1;
    unsigned need_in_memory:1;
    unsigned need_in_temp:1;
    unsigned aio:1;
# 104 "src/core/ngx_buf.h"
    off_t alignment;

    ngx_pool_t *pool;
    ngx_int_t allocated;
    ngx_bufs_t bufs;
    ngx_buf_tag_t tag;

    ngx_output_chain_filter_pt output_filter;
    void *filter_ctx;
};


typedef struct {
    ngx_chain_t *out;
    ngx_chain_t **last;
    ngx_connection_t *connection;
    ngx_pool_t *pool;
    off_t limit;
} ngx_chain_writer_ctx_t;
# 143 "src/core/ngx_buf.h"
ngx_buf_t *ngx_create_temp_buf(ngx_pool_t *pool, size_t size);
ngx_chain_t *ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);





ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);






ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in);
ngx_int_t ngx_chain_writer(void *ctx, ngx_chain_t *in);

ngx_int_t ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **free);
void ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **free,
    ngx_chain_t **busy, ngx_chain_t **out, ngx_buf_tag_t tag);

off_t ngx_chain_coalesce_file(ngx_chain_t **in, off_t limit);

ngx_chain_t *ngx_chain_update_sent(ngx_chain_t *in, off_t sent);
# 63 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_queue.h" 1
# 16 "src/core/ngx_queue.h"
typedef struct ngx_queue_s ngx_queue_t;

struct ngx_queue_s {
    ngx_queue_t *prev;
    ngx_queue_t *next;
};
# 107 "src/core/ngx_queue.h"
ngx_queue_t *ngx_queue_middle(ngx_queue_t *queue);
void ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *));
# 64 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_array.h" 1
# 16 "src/core/ngx_array.h"
typedef struct {
    void *elts;
    ngx_uint_t nelts;
    size_t size;
    ngx_uint_t nalloc;
    ngx_pool_t *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
void ngx_array_destroy(ngx_array_t *a);
void *ngx_array_push(ngx_array_t *a);
void *ngx_array_push_n(ngx_array_t *a, ngx_uint_t n);


static inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{





    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * size);
    if (array->elts == 0) {
        return -1;
    }

    return 0;
}
# 65 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_list.h" 1
# 16 "src/core/ngx_list.h"
typedef struct ngx_list_part_s ngx_list_part_t;

struct ngx_list_part_s {
    void *elts;
    ngx_uint_t nelts;
    ngx_list_part_t *next;
};


typedef struct {
    ngx_list_part_t *last;
    ngx_list_part_t part;
    size_t size;
    ngx_uint_t nalloc;
    ngx_pool_t *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

static inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);
    if (list->part.elts == 0) {
        return -1;
    }

    list->part.nelts = 0;
    list->part.next = 0;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return 0;
}
# 80 "src/core/ngx_list.h"
void *ngx_list_push(ngx_list_t *list);
# 66 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_hash.h" 1
# 16 "src/core/ngx_hash.h"
typedef struct {
    void *value;
    u_short len;
    u_char name[1];
} ngx_hash_elt_t;


typedef struct {
    ngx_hash_elt_t **buckets;
    ngx_uint_t size;
} ngx_hash_t;


typedef struct {
    ngx_hash_t hash;
    void *value;
} ngx_hash_wildcard_t;


typedef struct {
    ngx_str_t key;
    ngx_uint_t key_hash;
    void *value;
} ngx_hash_key_t;


typedef ngx_uint_t (*ngx_hash_key_pt) (u_char *data, size_t len);


typedef struct {
    ngx_hash_t hash;
    ngx_hash_wildcard_t *wc_head;
    ngx_hash_wildcard_t *wc_tail;
} ngx_hash_combined_t;


typedef struct {
    ngx_hash_t *hash;
    ngx_hash_key_pt key;

    ngx_uint_t max_size;
    ngx_uint_t bucket_size;

    char *name;
    ngx_pool_t *pool;
    ngx_pool_t *temp_pool;
} ngx_hash_init_t;
# 75 "src/core/ngx_hash.h"
typedef struct {
    ngx_uint_t hsize;

    ngx_pool_t *pool;
    ngx_pool_t *temp_pool;

    ngx_array_t keys;
    ngx_array_t *keys_hash;

    ngx_array_t dns_wc_head;
    ngx_array_t *dns_wc_head_hash;

    ngx_array_t dns_wc_tail;
    ngx_array_t *dns_wc_tail_hash;
} ngx_hash_keys_arrays_t;


typedef struct {
    ngx_uint_t hash;
    ngx_str_t key;
    ngx_str_t value;
    u_char *lowcase_key;
} ngx_table_elt_t;


void *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t key, u_char *name, size_t len);
void *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t key,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts);


ngx_uint_t ngx_hash_key(u_char *data, size_t len);
ngx_uint_t ngx_hash_key_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_keys_array_init(ngx_hash_keys_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_key(ngx_hash_keys_arrays_t *ha, ngx_str_t *key,
    void *value, ngx_uint_t flags);
# 67 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_file.h" 1
# 16 "src/core/ngx_file.h"
struct ngx_file_s {
    ngx_fd_t fd;
    ngx_str_t name;
    ngx_file_info_t info;

    off_t offset;
    off_t sys_offset;

    ngx_log_t *log;
# 37 "src/core/ngx_file.h"
    unsigned valid_info:1;
    unsigned directio:1;
};





typedef ngx_msec_t (*ngx_path_manager_pt) (void *data);
typedef ngx_msec_t (*ngx_path_purger_pt) (void *data);
typedef void (*ngx_path_loader_pt) (void *data);


typedef struct {
    ngx_str_t name;
    size_t len;
    size_t level[3];

    ngx_path_manager_pt manager;
    ngx_path_purger_pt purger;
    ngx_path_loader_pt loader;
    void *data;

    u_char *conf_file;
    ngx_uint_t line;
} ngx_path_t;


typedef struct {
    ngx_str_t name;
    size_t level[3];
} ngx_path_init_t;


typedef struct {
    ngx_file_t file;
    off_t offset;
    ngx_path_t *path;
    ngx_pool_t *pool;
    char *warn;

    ngx_uint_t access;

    unsigned log_level:8;
    unsigned persistent:1;
    unsigned clean:1;
    unsigned thread_write:1;
} ngx_temp_file_t;


typedef struct {
    ngx_uint_t access;
    ngx_uint_t path_access;
    time_t time;
    ngx_fd_t fd;

    unsigned create_path:1;
    unsigned delete_file:1;

    ngx_log_t *log;
} ngx_ext_rename_file_t;


typedef struct {
    off_t size;
    size_t buf_size;

    ngx_uint_t access;
    time_t time;

    ngx_log_t *log;
} ngx_copy_file_t;


typedef struct ngx_tree_ctx_s ngx_tree_ctx_t;

typedef ngx_int_t (*ngx_tree_init_handler_pt) (void *ctx, void *prev);
typedef ngx_int_t (*ngx_tree_handler_pt) (ngx_tree_ctx_t *ctx, ngx_str_t *name);

struct ngx_tree_ctx_s {
    off_t size;
    off_t fs_size;
    ngx_uint_t access;
    time_t mtime;

    ngx_tree_init_handler_pt init_handler;
    ngx_tree_handler_pt file_handler;
    ngx_tree_handler_pt pre_tree_handler;
    ngx_tree_handler_pt post_tree_handler;
    ngx_tree_handler_pt spec_handler;

    void *data;
    size_t alloc;

    ngx_log_t *log;
};


ngx_int_t ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix,
    ngx_str_t *name);

ssize_t ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain);
ngx_int_t ngx_create_temp_file(ngx_file_t *file, ngx_path_t *path,
    ngx_pool_t *pool, ngx_uint_t persistent, ngx_uint_t clean,
    ngx_uint_t access);
void ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len);
ngx_int_t ngx_create_path(ngx_file_t *file, ngx_path_t *path);
ngx_err_t ngx_create_full_path(u_char *dir, ngx_uint_t access);
ngx_int_t ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot);
ngx_int_t ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user);
ngx_int_t ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to,
    ngx_ext_rename_file_t *ext);
ngx_int_t ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf);
ngx_int_t ngx_walk_tree(ngx_tree_ctx_t *ctx, ngx_str_t *tree);

ngx_atomic_uint_t ngx_next_temp_number(ngx_uint_t collision);

char *ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_merge_path_value(ngx_conf_t *cf, ngx_path_t **path,
    ngx_path_t *prev, ngx_path_init_t *init);
char *ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


extern ngx_atomic_t *ngx_temp_number;
extern ngx_atomic_int_t ngx_random_number;
# 68 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_crc.h" 1
# 18 "src/core/ngx_crc.h"
static inline uint32_t
ngx_crc(u_char *data, size_t len)
{
    uint32_t sum;

    for (sum = 0; len; len--) {






        sum = sum >> 1 | sum << 31;

        sum += *data++;
    }

    return sum;
}
# 69 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_crc32.h" 1
# 16 "src/core/ngx_crc32.h"
extern uint32_t *ngx_crc32_table_short;
extern uint32_t ngx_crc32_table256[];


static inline uint32_t
ngx_crc32_short(u_char *p, size_t len)
{
    u_char c;
    uint32_t crc;

    crc = 0xffffffff;

    while (len--) {
        c = *p++;
        crc = ngx_crc32_table_short[(crc ^ (c & 0xf)) & 0xf] ^ (crc >> 4);
        crc = ngx_crc32_table_short[(crc ^ (c >> 4)) & 0xf] ^ (crc >> 4);
    }

    return crc ^ 0xffffffff;
}


static inline uint32_t
ngx_crc32_long(u_char *p, size_t len)
{
    uint32_t crc;

    crc = 0xffffffff;

    while (len--) {
        crc = ngx_crc32_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    return crc ^ 0xffffffff;
}






static inline void
ngx_crc32_update(uint32_t *crc, u_char *p, size_t len)
{
    uint32_t c;

    c = *crc;

    while (len--) {
        c = ngx_crc32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}






ngx_int_t ngx_crc32_table_init(void);
# 70 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_murmurhash.h" 1
# 16 "src/core/ngx_murmurhash.h"
uint32_t ngx_murmur_hash2(u_char *data, size_t len);
# 71 "src/core/ngx_core.h" 2



# 1 "src/core/ngx_radix_tree.h" 1
# 18 "src/core/ngx_radix_tree.h"
typedef struct ngx_radix_node_s ngx_radix_node_t;

struct ngx_radix_node_s {
    ngx_radix_node_t *right;
    ngx_radix_node_t *left;
    ngx_radix_node_t *parent;
    uintptr_t value;
};


typedef struct {
    ngx_radix_node_t *root;
    ngx_pool_t *pool;
    ngx_radix_node_t *free;
    char *start;
    size_t size;
} ngx_radix_tree_t;


ngx_radix_tree_t *ngx_radix_tree_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_insert(ngx_radix_tree_t *tree,
    uint32_t key, uint32_t mask, uintptr_t value);
ngx_int_t ngx_radix32tree_delete(ngx_radix_tree_t *tree,
    uint32_t key, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_tree_t *tree, uint32_t key);


ngx_int_t ngx_radix128tree_insert(ngx_radix_tree_t *tree,
    u_char *key, u_char *mask, uintptr_t value);
ngx_int_t ngx_radix128tree_delete(ngx_radix_tree_t *tree,
    u_char *key, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_tree_t *tree, u_char *key);
# 75 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_times.h" 1
# 16 "src/core/ngx_times.h"
typedef struct {
    time_t sec;
    ngx_uint_t msec;
    ngx_int_t gmtoff;
} ngx_time_t;


void ngx_time_init(void);
void ngx_time_update(void);
void ngx_time_sigsafe_update(void);
u_char *ngx_http_time(u_char *buf, time_t t);
u_char *ngx_http_cookie_time(u_char *buf, time_t t);
void ngx_gmtime(time_t t, ngx_tm_t *tp);

time_t ngx_next_time(time_t when);



extern volatile ngx_time_t *ngx_cached_time;




extern volatile ngx_str_t ngx_cached_err_log_time;
extern volatile ngx_str_t ngx_cached_http_time;
extern volatile ngx_str_t ngx_cached_http_log_time;
extern volatile ngx_str_t ngx_cached_http_log_iso8601;
extern volatile ngx_str_t ngx_cached_syslog_time;





extern volatile ngx_msec_t ngx_current_msec;
# 76 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_rwlock.h" 1
# 16 "src/core/ngx_rwlock.h"
void ngx_rwlock_wlock(ngx_atomic_t *lock);
void ngx_rwlock_rlock(ngx_atomic_t *lock);
void ngx_rwlock_unlock(ngx_atomic_t *lock);
void ngx_rwlock_downgrade(ngx_atomic_t *lock);
# 77 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_shmtx.h" 1
# 16 "src/core/ngx_shmtx.h"
typedef struct {
    ngx_atomic_t lock;

    ngx_atomic_t wait;

} ngx_shmtx_sh_t;


typedef struct {

    ngx_atomic_t *lock;

    ngx_atomic_t *wait;
    ngx_uint_t semaphore;
    sem_t sem;





    ngx_uint_t spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
void ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
void ngx_shmtx_lock(ngx_shmtx_t *mtx);
void ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);
# 78 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_slab.h" 1
# 16 "src/core/ngx_slab.h"
typedef struct ngx_slab_page_s ngx_slab_page_t;

struct ngx_slab_page_s {
    uintptr_t slab;
    ngx_slab_page_t *next;
    uintptr_t prev;
};


typedef struct {
    ngx_uint_t total;
    ngx_uint_t used;

    ngx_uint_t reqs;
    ngx_uint_t fails;
} ngx_slab_stat_t;


typedef struct {
    ngx_shmtx_sh_t lock;

    size_t min_size;
    size_t min_shift;

    ngx_slab_page_t *pages;
    ngx_slab_page_t *last;
    ngx_slab_page_t free;

    ngx_slab_stat_t *stats;
    ngx_uint_t pfree;

    u_char *start;
    u_char *end;

    ngx_shmtx_t mutex;

    u_char *log_ctx;
    u_char zero;

    unsigned log_nomem:1;

    void *data;
    void *addr;
} ngx_slab_pool_t;


void ngx_slab_sizes_init(void);
void ngx_slab_init(ngx_slab_pool_t *pool);
void *ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size);
void *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size);
void *ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size);
void *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size);
void ngx_slab_free(ngx_slab_pool_t *pool, void *p);
void ngx_slab_free_locked(ngx_slab_pool_t *pool, void *p);
# 79 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_inet.h" 1
# 35 "src/core/ngx_inet.h"
typedef union {
    struct sockaddr sockaddr;
    struct sockaddr_in sockaddr_in;

    struct sockaddr_in6 sockaddr_in6;


    struct sockaddr_un sockaddr_un;

} ngx_sockaddr_t;


typedef struct {
    in_addr_t addr;
    in_addr_t mask;
} ngx_in_cidr_t;




typedef struct {
    struct in6_addr addr;
    struct in6_addr mask;
} ngx_in6_cidr_t;




typedef struct {
    ngx_uint_t family;
    union {
        ngx_in_cidr_t in;

        ngx_in6_cidr_t in6;

    } u;
} ngx_cidr_t;


typedef struct {
    struct sockaddr *sockaddr;
    socklen_t socklen;
    ngx_str_t name;
} ngx_addr_t;


typedef struct {
    ngx_str_t url;
    ngx_str_t host;
    ngx_str_t port_text;
    ngx_str_t uri;

    in_port_t port;
    in_port_t default_port;
    int family;

    unsigned listen:1;
    unsigned uri_part:1;
    unsigned no_resolve:1;

    unsigned no_port:1;
    unsigned wildcard:1;

    socklen_t socklen;
    ngx_sockaddr_t sockaddr;

    ngx_addr_t *addrs;
    ngx_uint_t naddrs;

    char *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, size_t len);

ngx_int_t ngx_inet6_addr(u_char *p, size_t len, u_char *addr);
size_t ngx_inet6_ntop(u_char *p, u_char *text, size_t len);

size_t ngx_sock_ntop(struct sockaddr *sa, socklen_t socklen, u_char *text,
    size_t len, ngx_uint_t port);
size_t ngx_inet_ntop(int family, void *addr, u_char *text, size_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(struct sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, size_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(struct sockaddr *sa1, socklen_t slen1,
    struct sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(struct sockaddr *sa);
void ngx_inet_set_port(struct sockaddr *sa, in_port_t port);
# 80 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_cycle.h" 1
# 25 "src/core/ngx_cycle.h"
typedef struct ngx_shm_zone_s ngx_shm_zone_t;

typedef ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, void *data);

struct ngx_shm_zone_s {
    void *data;
    ngx_shm_t shm;
    ngx_shm_zone_init_pt init;
    void *tag;
    void *sync;
    ngx_uint_t noreuse;
};


struct ngx_cycle_s {
    void ****conf_ctx;
    ngx_pool_t *pool;

    ngx_log_t *log;
    ngx_log_t new_log;

    ngx_uint_t log_use_stderr;

    ngx_connection_t **files;
    ngx_connection_t *free_connections;
    ngx_uint_t free_connection_n;

    ngx_module_t **modules;
    ngx_uint_t modules_n;
    ngx_uint_t modules_used;

    ngx_queue_t reusable_connections_queue;
    ngx_uint_t reusable_connections_n;

    ngx_array_t listening;
    ngx_array_t paths;

    ngx_array_t config_dump;
    ngx_rbtree_t config_dump_rbtree;
    ngx_rbtree_node_t config_dump_sentinel;

    ngx_list_t open_files;
    ngx_list_t shared_memory;

    ngx_uint_t connection_n;
    ngx_uint_t files_n;

    ngx_connection_t *connections;
    ngx_event_t *read_events;
    ngx_event_t *write_events;

    ngx_cycle_t *old_cycle;

    ngx_str_t conf_file;
    ngx_str_t conf_param;
    ngx_str_t conf_prefix;
    ngx_str_t prefix;
    ngx_str_t lock_file;
    ngx_str_t hostname;
};


typedef struct {
    ngx_flag_t daemon;
    ngx_flag_t master;

    ngx_msec_t timer_resolution;
    ngx_msec_t shutdown_timeout;

    ngx_int_t worker_processes;
    ngx_int_t debug_points;

    ngx_int_t rlimit_nofile;
    off_t rlimit_core;

    int priority;

    ngx_uint_t cpu_affinity_auto;
    ngx_uint_t cpu_affinity_n;
    ngx_cpuset_t *cpu_affinity;

    char *username;
    ngx_uid_t user;
    ngx_gid_t group;

    ngx_str_t working_directory;
    ngx_str_t lock_file;

    ngx_str_t pid;
    ngx_str_t oldpid;

    ngx_array_t env;
    char **environment;

    ngx_uint_t transparent;
} ngx_core_conf_t;





ngx_cycle_t *ngx_init_cycle(ngx_cycle_t *old_cycle);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log);
void ngx_delete_pidfile(ngx_cycle_t *cycle);
ngx_int_t ngx_signal_process(ngx_cycle_t *cycle, char *sig);
void ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user);
char **ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last);
ngx_pid_t ngx_exec_new_binary(ngx_cycle_t *cycle, char *const *argv);
ngx_cpuset_t *ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name,
    size_t size, void *tag);
void ngx_set_shutdown_timer(ngx_cycle_t *cycle);


extern volatile ngx_cycle_t *ngx_cycle;
extern ngx_array_t ngx_old_cycles;
extern ngx_module_t ngx_core_module;
extern ngx_uint_t ngx_test_config;
extern ngx_uint_t ngx_dump_config;
extern ngx_uint_t ngx_quiet_mode;
# 81 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_resolver.h" 1
# 40 "src/core/ngx_resolver.h"
typedef struct ngx_resolver_s ngx_resolver_t;


typedef struct {
    ngx_connection_t *udp;
    ngx_connection_t *tcp;
    struct sockaddr *sockaddr;
    socklen_t socklen;
    ngx_str_t server;
    ngx_log_t log;
    ngx_buf_t *read_buf;
    ngx_buf_t *write_buf;
    ngx_resolver_t *resolver;
} ngx_resolver_connection_t;


typedef struct ngx_resolver_ctx_s ngx_resolver_ctx_t;

typedef void (*ngx_resolver_handler_pt)(ngx_resolver_ctx_t *ctx);


typedef struct {
    struct sockaddr *sockaddr;
    socklen_t socklen;
    ngx_str_t name;
    u_short priority;
    u_short weight;
} ngx_resolver_addr_t;


typedef struct {
    ngx_str_t name;
    u_short priority;
    u_short weight;
    u_short port;
} ngx_resolver_srv_t;


typedef struct {
    ngx_str_t name;
    u_short priority;
    u_short weight;
    u_short port;

    ngx_resolver_ctx_t *ctx;
    ngx_int_t state;

    ngx_uint_t naddrs;
    ngx_addr_t *addrs;
} ngx_resolver_srv_name_t;


typedef struct {
    ngx_rbtree_node_t node;
    ngx_queue_t queue;


    u_char *name;



    struct in6_addr addr6;


    u_short nlen;
    u_short qlen;

    u_char *query;

    u_char *query6;


    union {
        in_addr_t addr;
        in_addr_t *addrs;
        u_char *cname;
        ngx_resolver_srv_t *srvs;
    } u;

    u_char code;
    u_short naddrs;
    u_short nsrvs;
    u_short cnlen;


    union {
        struct in6_addr addr6;
        struct in6_addr *addrs6;
    } u6;

    u_short naddrs6;


    time_t expire;
    time_t valid;
    uint32_t ttl;

    unsigned tcp:1;

    unsigned tcp6:1;


    ngx_uint_t last_connection;

    ngx_resolver_ctx_t *waiting;
} ngx_resolver_node_t;


struct ngx_resolver_s {

    ngx_event_t *event;
    void *dummy;
    ngx_log_t *log;


    ngx_int_t ident;


    ngx_array_t connections;
    ngx_uint_t last_connection;

    ngx_rbtree_t name_rbtree;
    ngx_rbtree_node_t name_sentinel;

    ngx_rbtree_t srv_rbtree;
    ngx_rbtree_node_t srv_sentinel;

    ngx_rbtree_t addr_rbtree;
    ngx_rbtree_node_t addr_sentinel;

    ngx_queue_t name_resend_queue;
    ngx_queue_t srv_resend_queue;
    ngx_queue_t addr_resend_queue;

    ngx_queue_t name_expire_queue;
    ngx_queue_t srv_expire_queue;
    ngx_queue_t addr_expire_queue;


    ngx_uint_t ipv6;
    ngx_rbtree_t addr6_rbtree;
    ngx_rbtree_node_t addr6_sentinel;
    ngx_queue_t addr6_resend_queue;
    ngx_queue_t addr6_expire_queue;


    time_t resend_timeout;
    time_t tcp_timeout;
    time_t expire;
    time_t valid;

    ngx_uint_t log_level;
};


struct ngx_resolver_ctx_s {
    ngx_resolver_ctx_t *next;
    ngx_resolver_t *resolver;
    ngx_resolver_node_t *node;


    ngx_int_t ident;

    ngx_int_t state;
    ngx_str_t name;
    ngx_str_t service;

    time_t valid;
    ngx_uint_t naddrs;
    ngx_resolver_addr_t *addrs;
    ngx_resolver_addr_t addr;
    struct sockaddr_in sin;

    ngx_uint_t count;
    ngx_uint_t nsrvs;
    ngx_resolver_srv_name_t *srvs;

    ngx_resolver_handler_pt handler;
    void *data;
    ngx_msec_t timeout;

    unsigned quick:1;
    unsigned async:1;
    unsigned cancelable:1;
    ngx_uint_t recursion;
    ngx_event_t *event;
};


ngx_resolver_t *ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *names,
    ngx_uint_t n);
ngx_resolver_ctx_t *ngx_resolve_start(ngx_resolver_t *r,
    ngx_resolver_ctx_t *temp);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
void ngx_resolve_name_done(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
void ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx);
char *ngx_resolver_strerror(ngx_int_t err);
# 82 "src/core/ngx_core.h" 2



# 1 "src/os/unix/ngx_process_cycle.h" 1
# 30 "src/os/unix/ngx_process_cycle.h"
typedef struct {
    ngx_event_handler_pt handler;
    char *name;
    ngx_msec_t delay;
} ngx_cache_manager_ctx_t;


void ngx_master_process_cycle(ngx_cycle_t *cycle);
void ngx_single_process_cycle(ngx_cycle_t *cycle);


extern ngx_uint_t ngx_process;
extern ngx_uint_t ngx_worker;
extern ngx_pid_t ngx_pid;
extern ngx_pid_t ngx_new_binary;
extern ngx_uint_t ngx_inherited;
extern ngx_uint_t ngx_daemonized;
extern ngx_uint_t ngx_exiting;

extern sig_atomic_t ngx_reap;
extern sig_atomic_t ngx_sigio;
extern sig_atomic_t ngx_sigalrm;
extern sig_atomic_t ngx_quit;
extern sig_atomic_t ngx_debug_quit;
extern sig_atomic_t ngx_terminate;
extern sig_atomic_t ngx_noaccept;
extern sig_atomic_t ngx_reconfigure;
extern sig_atomic_t ngx_reopen;
extern sig_atomic_t ngx_change_binary;
# 86 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_conf_file.h" 1
# 77 "src/core/ngx_conf_file.h"
struct ngx_command_s {
    ngx_str_t name;
    ngx_uint_t type;
    char *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
    ngx_uint_t conf;
    ngx_uint_t offset;
    void *post;
};




struct ngx_open_file_s {
    ngx_fd_t fd;
    ngx_str_t name;

    void (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    void *data;
};


typedef struct {
    ngx_file_t file;
    ngx_buf_t *buffer;
    ngx_buf_t *dump;
    ngx_uint_t line;
} ngx_conf_file_t;


typedef struct {
    ngx_str_t name;
    ngx_buf_t *buffer;
} ngx_conf_dump_t;


typedef char *(*ngx_conf_handler_pt)(ngx_conf_t *cf,
    ngx_command_t *dummy, void *conf);


struct ngx_conf_s {
    char *name;
    ngx_array_t *args;

    ngx_cycle_t *cycle;
    ngx_pool_t *pool;
    ngx_pool_t *temp_pool;
    ngx_conf_file_t *conf_file;
    ngx_log_t *log;

    void *ctx;
    ngx_uint_t module_type;
    ngx_uint_t cmd_type;

    ngx_conf_handler_pt handler;
    void *handler_conf;
};


typedef char *(*ngx_conf_post_handler_pt) (ngx_conf_t *cf,
    void *data, void *conf);

typedef struct {
    ngx_conf_post_handler_pt post_handler;
} ngx_conf_post_t;


typedef struct {
    ngx_conf_post_handler_pt post_handler;
    char *old_name;
    char *new_name;
} ngx_conf_deprecated_t;


typedef struct {
    ngx_conf_post_handler_pt post_handler;
    ngx_int_t low;
    ngx_int_t high;
} ngx_conf_num_bounds_t;


typedef struct {
    ngx_str_t name;
    ngx_uint_t value;
} ngx_conf_enum_t;




typedef struct {
    ngx_str_t name;
    ngx_uint_t mask;
} ngx_conf_bitmask_t;



char * ngx_conf_deprecated(ngx_conf_t *cf, void *post, void *data);
char *ngx_conf_check_num_bounds(ngx_conf_t *cf, void *post, void *data);
# 268 "src/core/ngx_conf_file.h"
char *ngx_conf_param(ngx_conf_t *cf);
char *ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename);
char *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


ngx_int_t ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name,
    ngx_uint_t conf_prefix);
ngx_open_file_t *ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name);
void ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf,
    ngx_err_t err, const char *fmt, ...);


char *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
char *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
# 87 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_module.h" 1
# 15 "src/core/ngx_module.h"
# 1 "src/core/nginx.h" 1
# 16 "src/core/ngx_module.h" 2
# 222 "src/core/ngx_module.h"
struct ngx_module_s {
    ngx_uint_t ctx_index;
    ngx_uint_t index;

    char *name;

    ngx_uint_t spare0;
    ngx_uint_t spare1;

    ngx_uint_t version;
    const char *signature;

    void *ctx;
    ngx_command_t *commands;
    ngx_uint_t type;

    ngx_int_t (*init_master)(ngx_log_t *log);

    ngx_int_t (*init_module)(ngx_cycle_t *cycle);

    ngx_int_t (*init_process)(ngx_cycle_t *cycle);
    ngx_int_t (*init_thread)(ngx_cycle_t *cycle);
    void (*exit_thread)(ngx_cycle_t *cycle);
    void (*exit_process)(ngx_cycle_t *cycle);

    void (*exit_master)(ngx_cycle_t *cycle);

    uintptr_t spare_hook0;
    uintptr_t spare_hook1;
    uintptr_t spare_hook2;
    uintptr_t spare_hook3;
    uintptr_t spare_hook4;
    uintptr_t spare_hook5;
    uintptr_t spare_hook6;
    uintptr_t spare_hook7;
};


typedef struct {
    ngx_str_t name;
    void *(*create_conf)(ngx_cycle_t *cycle);
    char *(*init_conf)(ngx_cycle_t *cycle, void *conf);
} ngx_core_module_t;


ngx_int_t ngx_preinit_modules(void);
ngx_int_t ngx_cycle_modules(ngx_cycle_t *cycle);
ngx_int_t ngx_init_modules(ngx_cycle_t *cycle);
ngx_int_t ngx_count_modules(ngx_cycle_t *cycle, ngx_uint_t type);


ngx_int_t ngx_add_module(ngx_conf_t *cf, ngx_str_t *file,
    ngx_module_t *module, char **order);


extern ngx_module_t *ngx_modules[];
extern ngx_uint_t ngx_max_module;

extern char *ngx_module_names[];
# 88 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_open_file_cache.h" 1
# 19 "src/core/ngx_open_file_cache.h"
typedef struct {
    ngx_fd_t fd;
    ngx_file_uniq_t uniq;
    time_t mtime;
    off_t size;
    off_t fs_size;
    off_t directio;
    size_t read_ahead;

    ngx_err_t err;
    char *failed;

    time_t valid;

    ngx_uint_t min_uses;


    size_t disable_symlinks_from;
    unsigned disable_symlinks:2;


    unsigned test_dir:1;
    unsigned test_only:1;
    unsigned log:1;
    unsigned errors:1;
    unsigned events:1;

    unsigned is_dir:1;
    unsigned is_file:1;
    unsigned is_link:1;
    unsigned is_exec:1;
    unsigned is_directio:1;
} ngx_open_file_info_t;


typedef struct ngx_cached_open_file_s ngx_cached_open_file_t;

struct ngx_cached_open_file_s {
    ngx_rbtree_node_t node;
    ngx_queue_t queue;

    u_char *name;
    time_t created;
    time_t accessed;

    ngx_fd_t fd;
    ngx_file_uniq_t uniq;
    time_t mtime;
    off_t size;
    ngx_err_t err;

    uint32_t uses;


    size_t disable_symlinks_from;
    unsigned disable_symlinks:2;


    unsigned count:24;
    unsigned close:1;
    unsigned use_event:1;

    unsigned is_dir:1;
    unsigned is_file:1;
    unsigned is_link:1;
    unsigned is_exec:1;
    unsigned is_directio:1;

    ngx_event_t *event;
};


typedef struct {
    ngx_rbtree_t rbtree;
    ngx_rbtree_node_t sentinel;
    ngx_queue_t expire_queue;

    ngx_uint_t current;
    ngx_uint_t max;
    time_t inactive;
} ngx_open_file_cache_t;


typedef struct {
    ngx_open_file_cache_t *cache;
    ngx_cached_open_file_t *file;
    ngx_uint_t min_uses;
    ngx_log_t *log;
} ngx_open_file_cache_cleanup_t;


typedef struct {


    void *data;
    ngx_event_t *read;
    ngx_event_t *write;
    ngx_fd_t fd;

    ngx_cached_open_file_t *file;
    ngx_open_file_cache_t *cache;
} ngx_open_file_cache_event_t;


ngx_open_file_cache_t *ngx_open_file_cache_init(ngx_pool_t *pool,
    ngx_uint_t max, time_t inactive);
ngx_int_t ngx_open_cached_file(ngx_open_file_cache_t *cache, ngx_str_t *name,
    ngx_open_file_info_t *of, ngx_pool_t *pool);
# 89 "src/core/ngx_core.h" 2
# 1 "src/os/unix/ngx_os.h" 1
# 19 "src/os/unix/ngx_os.h"
typedef ssize_t (*ngx_recv_pt)(ngx_connection_t *c, u_char *buf, size_t size);
typedef ssize_t (*ngx_recv_chain_pt)(ngx_connection_t *c, ngx_chain_t *in,
    off_t limit);
typedef ssize_t (*ngx_send_pt)(ngx_connection_t *c, u_char *buf, size_t size);
typedef ngx_chain_t *(*ngx_send_chain_pt)(ngx_connection_t *c, ngx_chain_t *in,
    off_t limit);

typedef struct {
    ngx_recv_pt recv;
    ngx_recv_chain_pt recv_chain;
    ngx_recv_pt udp_recv;
    ngx_send_pt send;
    ngx_send_pt udp_send;
    ngx_send_chain_pt udp_send_chain;
    ngx_send_chain_pt send_chain;
    ngx_uint_t flags;
} ngx_os_io_t;


ngx_int_t ngx_os_init(ngx_log_t *log);
void ngx_os_status(ngx_log_t *log);
ngx_int_t ngx_os_specific_init(ngx_log_t *log);
void ngx_os_specific_status(ngx_log_t *log);
ngx_int_t ngx_daemon(ngx_log_t *log);
ngx_int_t ngx_os_signal_process(ngx_cycle_t *cycle, char *sig, ngx_pid_t pid);


ssize_t ngx_unix_recv(ngx_connection_t *c, u_char *buf, size_t size);
ssize_t ngx_readv_chain(ngx_connection_t *c, ngx_chain_t *entry, off_t limit);
ssize_t ngx_udp_unix_recv(ngx_connection_t *c, u_char *buf, size_t size);
ssize_t ngx_unix_send(ngx_connection_t *c, u_char *buf, size_t size);
ngx_chain_t *ngx_writev_chain(ngx_connection_t *c, ngx_chain_t *in,
    off_t limit);
ssize_t ngx_udp_unix_send(ngx_connection_t *c, u_char *buf, size_t size);
ngx_chain_t *ngx_udp_unix_sendmsg_chain(ngx_connection_t *c, ngx_chain_t *in,
    off_t limit);
# 64 "src/os/unix/ngx_os.h"
typedef struct {
    struct iovec *iovs;
    ngx_uint_t count;
    size_t size;
    ngx_uint_t nalloc;
} ngx_iovec_t;

ngx_chain_t *ngx_output_chain_to_iovec(ngx_iovec_t *vec, ngx_chain_t *in,
    size_t limit, ngx_log_t *log);


ssize_t ngx_writev(ngx_connection_t *c, ngx_iovec_t *vec);


extern ngx_os_io_t ngx_os_io;
extern ngx_int_t ngx_ncpu;
extern ngx_int_t ngx_max_sockets;
extern ngx_uint_t ngx_inherited_nonblocking;
extern ngx_uint_t ngx_tcp_nodelay_and_tcp_nopush;







# 1 "src/os/unix/ngx_linux.h" 1
# 12 "src/os/unix/ngx_linux.h"
ngx_chain_t *ngx_linux_sendfile_chain(ngx_connection_t *c, ngx_chain_t *in,
    off_t limit);
# 91 "src/os/unix/ngx_os.h" 2
# 90 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_connection.h" 1
# 16 "src/core/ngx_connection.h"
typedef struct ngx_listening_s ngx_listening_t;

struct ngx_listening_s {
    ngx_socket_t fd;

    struct sockaddr *sockaddr;
    socklen_t socklen;
    size_t addr_text_max_len;
    ngx_str_t addr_text;

    int type;

    int backlog;
    int rcvbuf;
    int sndbuf;

    int keepidle;
    int keepintvl;
    int keepcnt;



    ngx_connection_handler_pt handler;

    void *servers;

    ngx_log_t log;
    ngx_log_t *logp;

    size_t pool_size;

    size_t post_accept_buffer_size;

    ngx_msec_t post_accept_timeout;

    ngx_listening_t *previous;
    ngx_connection_t *connection;

    ngx_rbtree_t rbtree;
    ngx_rbtree_node_t sentinel;

    ngx_uint_t worker;

    unsigned open:1;
    unsigned remain:1;
    unsigned ignore:1;

    unsigned bound:1;
    unsigned inherited:1;
    unsigned nonblocking_accept:1;
    unsigned listen:1;
    unsigned nonblocking:1;
    unsigned shared:1;
    unsigned addr_ntop:1;
    unsigned wildcard:1;


    unsigned ipv6only:1;

    unsigned reuseport:1;
    unsigned add_reuseport:1;
    unsigned keepalive:2;

    unsigned deferred_accept:1;
    unsigned delete_deferred:1;
    unsigned add_deferred:1;
# 90 "src/core/ngx_connection.h"
    int fastopen;


};


typedef enum {
    NGX_ERROR_ALERT = 0,
    NGX_ERROR_ERR,
    NGX_ERROR_INFO,
    NGX_ERROR_IGNORE_ECONNRESET,
    NGX_ERROR_IGNORE_EINVAL
} ngx_connection_log_error_e;


typedef enum {
    NGX_TCP_NODELAY_UNSET = 0,
    NGX_TCP_NODELAY_SET,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


typedef enum {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_SET,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;







struct ngx_connection_s {
    void *data;
    ngx_event_t *read;
    ngx_event_t *write;

    ngx_socket_t fd;

    ngx_recv_pt recv;
    ngx_send_pt send;
    ngx_recv_chain_pt recv_chain;
    ngx_send_chain_pt send_chain;

    ngx_listening_t *listening;

    off_t sent;

    ngx_log_t *log;

    ngx_pool_t *pool;

    int type;

    struct sockaddr *sockaddr;
    socklen_t socklen;
    ngx_str_t addr_text;

    ngx_str_t proxy_protocol_addr;
    in_port_t proxy_protocol_port;





    ngx_udp_connection_t *udp;

    struct sockaddr *local_sockaddr;
    socklen_t local_socklen;

    ngx_buf_t *buffer;

    ngx_queue_t queue;

    ngx_atomic_uint_t number;

    ngx_uint_t requests;

    unsigned buffered:8;

    unsigned log_error:3;

    unsigned timedout:1;
    unsigned error:1;
    unsigned destroyed:1;

    unsigned idle:1;
    unsigned reusable:1;
    unsigned close:1;
    unsigned shared:1;

    unsigned sendfile:1;
    unsigned sndlowat:1;
    unsigned tcp_nodelay:2;
    unsigned tcp_nopush:2;

    unsigned need_last_buf:1;
# 197 "src/core/ngx_connection.h"
};
# 211 "src/core/ngx_connection.h"
ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, struct sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls);
ngx_int_t ngx_set_inherited_sockets(ngx_cycle_t *cycle);
ngx_int_t ngx_open_listening_sockets(ngx_cycle_t *cycle);
void ngx_configure_listening_sockets(ngx_cycle_t *cycle);
void ngx_close_listening_sockets(ngx_cycle_t *cycle);
void ngx_close_connection(ngx_connection_t *c);
void ngx_close_idle_connections(ngx_cycle_t *cycle);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t port);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, char *text);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *log);
void ngx_free_connection(ngx_connection_t *c);

void ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);
# 91 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_syslog.h" 1
# 11 "src/core/ngx_syslog.h"
typedef struct {
    ngx_uint_t facility;
    ngx_uint_t severity;
    ngx_str_t tag;

    ngx_addr_t server;
    ngx_connection_t conn;
    unsigned busy:1;
    unsigned nohostname:1;
} ngx_syslog_peer_t;


char *ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
u_char *ngx_syslog_add_header(ngx_syslog_peer_t *peer, u_char *buf);
void ngx_syslog_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len);
ssize_t ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, size_t len);
# 92 "src/core/ngx_core.h" 2
# 1 "src/core/ngx_proxy_protocol.h" 1
# 19 "src/core/ngx_proxy_protocol.h"
u_char *ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
u_char *ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf,
    u_char *last);
# 93 "src/core/ngx_core.h" 2
# 104 "src/core/ngx_core.h"
void ngx_cpuinfo(void);
# 10 "src/core/ngx_crc32.c" 2
# 26 "src/core/ngx_crc32.c"
static uint32_t ngx_crc32_table16[] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};


uint32_t ngx_crc32_table256[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
    0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
    0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
    0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
    0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
    0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
    0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
    0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
    0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
    0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
    0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
    0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
    0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
    0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
    0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
    0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
    0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
    0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
    0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
    0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
    0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};


uint32_t *ngx_crc32_table_short = ngx_crc32_table16;


ngx_int_t
ngx_crc32_table_init(void)
{
    void *p;

    if (((uintptr_t) ngx_crc32_table_short
          & ~((uintptr_t) ngx_cacheline_size - 1))
        == (uintptr_t) ngx_crc32_table_short)
    {
        return 0;
    }

    p = ngx_alloc(16 * sizeof(uint32_t) + ngx_cacheline_size, ngx_cycle->log);
    if (p == 0) {
        return -1;
    }

    p = (u_char *) (((uintptr_t) (p) + ((uintptr_t) ngx_cacheline_size - 1)) & ~((uintptr_t) ngx_cacheline_size - 1));

    (void) memcpy(p, ngx_crc32_table16, 16 * sizeof(uint32_t));

    ngx_crc32_table_short = p;

    return 0;
}
