# 1 "src/http/ngx_http_upstream.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "src/http/ngx_http_upstream.c"







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
# 9 "src/http/ngx_http_upstream.c" 2
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
# 10 "src/http/ngx_http_upstream.c" 2
# 1 "src/http/ngx_http.h" 1
# 16 "src/http/ngx_http.h"
typedef struct ngx_http_request_s ngx_http_request_t;
typedef struct ngx_http_upstream_s ngx_http_upstream_t;
typedef struct ngx_http_cache_s ngx_http_cache_t;
typedef struct ngx_http_file_cache_s ngx_http_file_cache_t;
typedef struct ngx_http_log_ctx_s ngx_http_log_ctx_t;
typedef struct ngx_http_chunked_s ngx_http_chunked_t;
typedef struct ngx_http_v2_stream_s ngx_http_v2_stream_t;

typedef ngx_int_t (*ngx_http_header_handler_pt)(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
typedef u_char *(*ngx_http_log_handler_pt)(ngx_http_request_t *r,
    ngx_http_request_t *sr, u_char *buf, size_t len);


# 1 "src/http/ngx_http_variables.h" 1
# 14 "src/http/ngx_http_variables.h"
# 1 "src/http/ngx_http.h" 1
# 15 "src/http/ngx_http_variables.h" 2


typedef ngx_variable_value_t ngx_http_variable_value_t;



typedef struct ngx_http_variable_s ngx_http_variable_t;

typedef void (*ngx_http_set_variable_pt) (ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
typedef ngx_int_t (*ngx_http_get_variable_pt) (ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
# 37 "src/http/ngx_http_variables.h"
struct ngx_http_variable_s {
    ngx_str_t name;
    ngx_http_set_variable_pt set_handler;
    ngx_http_get_variable_pt get_handler;
    uintptr_t data;
    ngx_uint_t flags;
    ngx_uint_t index;
};




ngx_http_variable_t *ngx_http_add_variable(ngx_conf_t *cf, ngx_str_t *name,
    ngx_uint_t flags);
ngx_int_t ngx_http_get_variable_index(ngx_conf_t *cf, ngx_str_t *name);
ngx_http_variable_value_t *ngx_http_get_indexed_variable(ngx_http_request_t *r,
    ngx_uint_t index);
ngx_http_variable_value_t *ngx_http_get_flushed_variable(ngx_http_request_t *r,
    ngx_uint_t index);

ngx_http_variable_value_t *ngx_http_get_variable(ngx_http_request_t *r,
    ngx_str_t *name, ngx_uint_t key);

ngx_int_t ngx_http_variable_unknown_header(ngx_http_variable_value_t *v,
    ngx_str_t *var, ngx_list_part_t *part, size_t prefix);
# 95 "src/http/ngx_http_variables.h"
typedef struct {
    ngx_hash_combined_t hash;




} ngx_http_map_t;


void *ngx_http_map_find(ngx_http_request_t *r, ngx_http_map_t *map,
    ngx_str_t *match);


ngx_int_t ngx_http_variables_add_core_vars(ngx_conf_t *cf);
ngx_int_t ngx_http_variables_init_vars(ngx_conf_t *cf);


extern ngx_http_variable_value_t ngx_http_variable_null_value;
extern ngx_http_variable_value_t ngx_http_variable_true_value;
# 31 "src/http/ngx_http.h" 2
# 1 "src/http/ngx_http_config.h" 1
# 17 "src/http/ngx_http_config.h"
typedef struct {
    void **main_conf;
    void **srv_conf;
    void **loc_conf;
} ngx_http_conf_ctx_t;


typedef struct {
    ngx_int_t (*preconfiguration)(ngx_conf_t *cf);
    ngx_int_t (*postconfiguration)(ngx_conf_t *cf);

    void *(*create_main_conf)(ngx_conf_t *cf);
    char *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    void *(*create_srv_conf)(ngx_conf_t *cf);
    char *(*merge_srv_conf)(ngx_conf_t *cf, void *prev, void *conf);

    void *(*create_loc_conf)(ngx_conf_t *cf);
    char *(*merge_loc_conf)(ngx_conf_t *cf, void *prev, void *conf);
} ngx_http_module_t;
# 32 "src/http/ngx_http.h" 2
# 1 "src/http/ngx_http_request.h" 1
# 153 "src/http/ngx_http_request.h"
typedef enum {
    NGX_HTTP_INITING_REQUEST_STATE = 0,
    NGX_HTTP_READING_REQUEST_STATE,
    NGX_HTTP_PROCESS_REQUEST_STATE,

    NGX_HTTP_CONNECT_UPSTREAM_STATE,
    NGX_HTTP_WRITING_UPSTREAM_STATE,
    NGX_HTTP_READING_UPSTREAM_STATE,

    NGX_HTTP_WRITING_REQUEST_STATE,
    NGX_HTTP_LINGERING_CLOSE_STATE,
    NGX_HTTP_KEEPALIVE_STATE
} ngx_http_state_e;


typedef struct {
    ngx_str_t name;
    ngx_uint_t offset;
    ngx_http_header_handler_pt handler;
} ngx_http_header_t;


typedef struct {
    ngx_str_t name;
    ngx_uint_t offset;
} ngx_http_header_out_t;


typedef struct {
    ngx_list_t headers;

    ngx_table_elt_t *host;
    ngx_table_elt_t *connection;
    ngx_table_elt_t *if_modified_since;
    ngx_table_elt_t *if_unmodified_since;
    ngx_table_elt_t *if_match;
    ngx_table_elt_t *if_none_match;
    ngx_table_elt_t *user_agent;
    ngx_table_elt_t *referer;
    ngx_table_elt_t *content_length;
    ngx_table_elt_t *content_range;
    ngx_table_elt_t *content_type;

    ngx_table_elt_t *range;
    ngx_table_elt_t *if_range;

    ngx_table_elt_t *transfer_encoding;
    ngx_table_elt_t *te;
    ngx_table_elt_t *expect;
    ngx_table_elt_t *upgrade;






    ngx_table_elt_t *authorization;

    ngx_table_elt_t *keep_alive;
# 233 "src/http/ngx_http_request.h"
    ngx_str_t user;
    ngx_str_t passwd;

    ngx_array_t cookies;

    ngx_str_t server;
    off_t content_length_n;
    time_t keep_alive_n;

    unsigned connection_type:2;
    unsigned chunked:1;
    unsigned msie:1;
    unsigned msie6:1;
    unsigned opera:1;
    unsigned gecko:1;
    unsigned chrome:1;
    unsigned safari:1;
    unsigned konqueror:1;
} ngx_http_headers_in_t;


typedef struct {
    ngx_list_t headers;
    ngx_list_t trailers;

    ngx_uint_t status;
    ngx_str_t status_line;

    ngx_table_elt_t *server;
    ngx_table_elt_t *date;
    ngx_table_elt_t *content_length;
    ngx_table_elt_t *content_encoding;
    ngx_table_elt_t *location;
    ngx_table_elt_t *refresh;
    ngx_table_elt_t *last_modified;
    ngx_table_elt_t *content_range;
    ngx_table_elt_t *accept_ranges;
    ngx_table_elt_t *www_authenticate;
    ngx_table_elt_t *expires;
    ngx_table_elt_t *etag;

    ngx_str_t *override_charset;

    size_t content_type_len;
    ngx_str_t content_type;
    ngx_str_t charset;
    u_char *content_type_lowcase;
    ngx_uint_t content_type_hash;

    ngx_array_t cache_control;
    ngx_array_t link;

    off_t content_length_n;
    off_t content_offset;
    time_t date_time;
    time_t last_modified_time;
} ngx_http_headers_out_t;


typedef void (*ngx_http_client_body_handler_pt)(ngx_http_request_t *r);

typedef struct {
    ngx_temp_file_t *temp_file;
    ngx_chain_t *bufs;
    ngx_buf_t *buf;
    off_t rest;
    off_t received;
    ngx_chain_t *free;
    ngx_chain_t *busy;
    ngx_http_chunked_t *chunked;
    ngx_http_client_body_handler_pt post_handler;
} ngx_http_request_body_t;


typedef struct ngx_http_addr_conf_s ngx_http_addr_conf_t;

typedef struct {
    ngx_http_addr_conf_t *addr_conf;
    ngx_http_conf_ctx_t *conf_ctx;
# 320 "src/http/ngx_http_request.h"
    ngx_chain_t *busy;
    ngx_int_t nbusy;

    ngx_chain_t *free;

    unsigned ssl:1;
    unsigned proxy_protocol:1;
} ngx_http_connection_t;


typedef void (*ngx_http_cleanup_pt)(void *data);

typedef struct ngx_http_cleanup_s ngx_http_cleanup_t;

struct ngx_http_cleanup_s {
    ngx_http_cleanup_pt handler;
    void *data;
    ngx_http_cleanup_t *next;
};


typedef ngx_int_t (*ngx_http_post_subrequest_pt)(ngx_http_request_t *r,
    void *data, ngx_int_t rc);

typedef struct {
    ngx_http_post_subrequest_pt handler;
    void *data;
} ngx_http_post_subrequest_t;


typedef struct ngx_http_postponed_request_s ngx_http_postponed_request_t;

struct ngx_http_postponed_request_s {
    ngx_http_request_t *request;
    ngx_chain_t *out;
    ngx_http_postponed_request_t *next;
};


typedef struct ngx_http_posted_request_s ngx_http_posted_request_t;

struct ngx_http_posted_request_s {
    ngx_http_request_t *request;
    ngx_http_posted_request_t *next;
};


typedef ngx_int_t (*ngx_http_handler_pt)(ngx_http_request_t *r);
typedef void (*ngx_http_event_handler_pt)(ngx_http_request_t *r);


struct ngx_http_request_s {
    uint32_t signature;

    ngx_connection_t *connection;

    void **ctx;
    void **main_conf;
    void **srv_conf;
    void **loc_conf;

    ngx_http_event_handler_pt read_event_handler;
    ngx_http_event_handler_pt write_event_handler;


    ngx_http_cache_t *cache;


    ngx_http_upstream_t *upstream;
    ngx_array_t *upstream_states;


    ngx_pool_t *pool;
    ngx_buf_t *header_in;

    ngx_http_headers_in_t headers_in;
    ngx_http_headers_out_t headers_out;

    ngx_http_request_body_t *request_body;

    time_t lingering_time;
    time_t start_sec;
    ngx_msec_t start_msec;

    ngx_uint_t method;
    ngx_uint_t http_version;

    ngx_str_t request_line;
    ngx_str_t uri;
    ngx_str_t args;
    ngx_str_t exten;
    ngx_str_t unparsed_uri;

    ngx_str_t method_name;
    ngx_str_t http_protocol;
    ngx_str_t schema;

    ngx_chain_t *out;
    ngx_http_request_t *main;
    ngx_http_request_t *parent;
    ngx_http_postponed_request_t *postponed;
    ngx_http_post_subrequest_t *post_subrequest;
    ngx_http_posted_request_t *posted_requests;

    ngx_int_t phase_handler;
    ngx_http_handler_pt content_handler;
    ngx_uint_t access_code;

    ngx_http_variable_value_t *variables;







    size_t limit_rate;
    size_t limit_rate_after;


    size_t header_size;

    off_t request_length;

    ngx_uint_t err_status;

    ngx_http_connection_t *http_connection;
    ngx_http_v2_stream_t *stream;

    ngx_http_log_handler_pt log_handler;

    ngx_http_cleanup_t *cleanup;

    unsigned count:16;
    unsigned subrequests:8;
    unsigned blocked:8;

    unsigned aio:1;

    unsigned http_state:4;


    unsigned complex_uri:1;


    unsigned quoted_uri:1;


    unsigned plus_in_uri:1;


    unsigned space_in_uri:1;

    unsigned invalid_header:1;

    unsigned add_uri_to_alias:1;
    unsigned valid_location:1;
    unsigned valid_unparsed_uri:1;
    unsigned uri_changed:1;
    unsigned uri_changes:4;

    unsigned request_body_in_single_buf:1;
    unsigned request_body_in_file_only:1;
    unsigned request_body_in_persistent_file:1;
    unsigned request_body_in_clean_file:1;
    unsigned request_body_file_group_access:1;
    unsigned request_body_file_log_level:3;
    unsigned request_body_no_buffering:1;

    unsigned subrequest_in_memory:1;
    unsigned waited:1;


    unsigned cached:1;
# 502 "src/http/ngx_http_request.h"
    unsigned proxy:1;
    unsigned bypass_cache:1;
    unsigned no_cache:1;






    unsigned limit_conn_set:1;
    unsigned limit_req_set:1;





    unsigned pipeline:1;
    unsigned chunked:1;
    unsigned header_only:1;
    unsigned expect_trailers:1;
    unsigned keepalive:1;
    unsigned lingering_close:1;
    unsigned discard_body:1;
    unsigned reading_body:1;
    unsigned internal:1;
    unsigned error_page:1;
    unsigned filter_finalize:1;
    unsigned post_action:1;
    unsigned request_complete:1;
    unsigned request_output:1;
    unsigned header_sent:1;
    unsigned expect_tested:1;
    unsigned root_tested:1;
    unsigned done:1;
    unsigned logged:1;

    unsigned buffered:4;

    unsigned main_filter_need_in_memory:1;
    unsigned filter_need_in_memory:1;
    unsigned filter_need_temporary:1;
    unsigned preserve_body:1;
    unsigned allow_ranges:1;
    unsigned subrequest_ranges:1;
    unsigned single_range:1;
    unsigned disable_not_modified:1;
    unsigned stat_reading:1;
    unsigned stat_writing:1;
    unsigned stat_processing:1;

    unsigned background:1;
    unsigned health_check:1;



    ngx_uint_t state;

    ngx_uint_t header_hash;
    ngx_uint_t lowcase_index;
    u_char lowcase_header[32];

    u_char *header_name_start;
    u_char *header_name_end;
    u_char *header_start;
    u_char *header_end;






    u_char *uri_start;
    u_char *uri_end;
    u_char *uri_ext;
    u_char *args_start;
    u_char *request_start;
    u_char *request_end;
    u_char *method_end;
    u_char *schema_start;
    u_char *schema_end;
    u_char *host_start;
    u_char *host_end;
    u_char *port_start;
    u_char *port_end;

    unsigned http_minor:16;
    unsigned http_major:16;
};


typedef struct {
    ngx_http_posted_request_t terminal_posted_request;
} ngx_http_ephemeral_t;





extern ngx_http_header_t ngx_http_headers_in[];
extern ngx_http_header_out_t ngx_http_headers_out[];
# 33 "src/http/ngx_http.h" 2
# 1 "src/http/ngx_http_script.h" 1
# 17 "src/http/ngx_http_script.h"
typedef struct {
    u_char *ip;
    u_char *pos;
    ngx_http_variable_value_t *sp;

    ngx_str_t buf;
    ngx_str_t line;


    u_char *args;

    unsigned flushed:1;
    unsigned skip:1;
    unsigned quote:1;
    unsigned is_args:1;
    unsigned log:1;

    ngx_int_t status;
    ngx_http_request_t *request;
} ngx_http_script_engine_t;


typedef struct {
    ngx_conf_t *cf;
    ngx_str_t *source;

    ngx_array_t **flushes;
    ngx_array_t **lengths;
    ngx_array_t **values;

    ngx_uint_t variables;
    ngx_uint_t ncaptures;
    ngx_uint_t captures_mask;
    ngx_uint_t size;

    void *main;

    unsigned compile_args:1;
    unsigned complete_lengths:1;
    unsigned complete_values:1;
    unsigned zero:1;
    unsigned conf_prefix:1;
    unsigned root_prefix:1;

    unsigned dup_capture:1;
    unsigned args:1;
} ngx_http_script_compile_t;


typedef struct {
    ngx_str_t value;
    ngx_uint_t *flushes;
    void *lengths;
    void *values;
} ngx_http_complex_value_t;


typedef struct {
    ngx_conf_t *cf;
    ngx_str_t *value;
    ngx_http_complex_value_t *complex_value;

    unsigned zero:1;
    unsigned conf_prefix:1;
    unsigned root_prefix:1;
} ngx_http_compile_complex_value_t;


typedef void (*ngx_http_script_code_pt) (ngx_http_script_engine_t *e);
typedef size_t (*ngx_http_script_len_code_pt) (ngx_http_script_engine_t *e);


typedef struct {
    ngx_http_script_code_pt code;
    uintptr_t len;
} ngx_http_script_copy_code_t;


typedef struct {
    ngx_http_script_code_pt code;
    uintptr_t index;
} ngx_http_script_var_code_t;


typedef struct {
    ngx_http_script_code_pt code;
    ngx_http_set_variable_pt handler;
    uintptr_t data;
} ngx_http_script_var_handler_code_t;


typedef struct {
    ngx_http_script_code_pt code;
    uintptr_t n;
} ngx_http_script_copy_capture_code_t;
# 154 "src/http/ngx_http_script.h"
typedef struct {
    ngx_http_script_code_pt code;
    uintptr_t conf_prefix;
} ngx_http_script_full_name_code_t;


typedef struct {
    ngx_http_script_code_pt code;
    uintptr_t status;
    ngx_http_complex_value_t text;
} ngx_http_script_return_code_t;


typedef enum {
    ngx_http_script_file_plain = 0,
    ngx_http_script_file_not_plain,
    ngx_http_script_file_dir,
    ngx_http_script_file_not_dir,
    ngx_http_script_file_exists,
    ngx_http_script_file_not_exists,
    ngx_http_script_file_exec,
    ngx_http_script_file_not_exec
} ngx_http_script_file_op_e;


typedef struct {
    ngx_http_script_code_pt code;
    uintptr_t op;
} ngx_http_script_file_code_t;


typedef struct {
    ngx_http_script_code_pt code;
    uintptr_t next;
    void **loc_conf;
} ngx_http_script_if_code_t;


typedef struct {
    ngx_http_script_code_pt code;
    ngx_array_t *lengths;
} ngx_http_script_complex_value_code_t;


typedef struct {
    ngx_http_script_code_pt code;
    uintptr_t value;
    uintptr_t text_len;
    uintptr_t text_data;
} ngx_http_script_value_code_t;


void ngx_http_script_flush_complex_value(ngx_http_request_t *r,
    ngx_http_complex_value_t *val);
ngx_int_t ngx_http_complex_value(ngx_http_request_t *r,
    ngx_http_complex_value_t *val, ngx_str_t *value);
ngx_int_t ngx_http_compile_complex_value(ngx_http_compile_complex_value_t *ccv);
char *ngx_http_set_complex_value_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


ngx_int_t ngx_http_test_predicates(ngx_http_request_t *r,
    ngx_array_t *predicates);
char *ngx_http_set_predicate_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

ngx_uint_t ngx_http_script_variables_count(ngx_str_t *value);
ngx_int_t ngx_http_script_compile(ngx_http_script_compile_t *sc);
u_char *ngx_http_script_run(ngx_http_request_t *r, ngx_str_t *value,
    void *code_lengths, size_t reserved, void *code_values);
void ngx_http_script_flush_no_cacheable_variables(ngx_http_request_t *r,
    ngx_array_t *indices);

void *ngx_http_script_start_code(ngx_pool_t *pool, ngx_array_t **codes,
    size_t size);
void *ngx_http_script_add_code(ngx_array_t *codes, size_t size, void *code);

size_t ngx_http_script_copy_len_code(ngx_http_script_engine_t *e);
void ngx_http_script_copy_code(ngx_http_script_engine_t *e);
size_t ngx_http_script_copy_var_len_code(ngx_http_script_engine_t *e);
void ngx_http_script_copy_var_code(ngx_http_script_engine_t *e);
size_t ngx_http_script_copy_capture_len_code(ngx_http_script_engine_t *e);
void ngx_http_script_copy_capture_code(ngx_http_script_engine_t *e);
size_t ngx_http_script_mark_args_code(ngx_http_script_engine_t *e);
void ngx_http_script_start_args_code(ngx_http_script_engine_t *e);




void ngx_http_script_return_code(ngx_http_script_engine_t *e);
void ngx_http_script_break_code(ngx_http_script_engine_t *e);
void ngx_http_script_if_code(ngx_http_script_engine_t *e);
void ngx_http_script_equal_code(ngx_http_script_engine_t *e);
void ngx_http_script_not_equal_code(ngx_http_script_engine_t *e);
void ngx_http_script_file_code(ngx_http_script_engine_t *e);
void ngx_http_script_complex_value_code(ngx_http_script_engine_t *e);
void ngx_http_script_value_code(ngx_http_script_engine_t *e);
void ngx_http_script_set_var_code(ngx_http_script_engine_t *e);
void ngx_http_script_var_set_handler_code(ngx_http_script_engine_t *e);
void ngx_http_script_var_code(ngx_http_script_engine_t *e);
void ngx_http_script_nop_code(ngx_http_script_engine_t *e);
# 34 "src/http/ngx_http.h" 2
# 1 "src/http/ngx_http_upstream.h" 1
# 14 "src/http/ngx_http_upstream.h"
# 1 "src/event/ngx_event.h" 1
# 30 "src/event/ngx_event.h"
struct ngx_event_s {
    void *data;

    unsigned write:1;

    unsigned accept:1;


    unsigned instance:1;





    unsigned active:1;

    unsigned disabled:1;


    unsigned ready:1;

    unsigned oneshot:1;


    unsigned complete:1;

    unsigned eof:1;
    unsigned error:1;

    unsigned timedout:1;
    unsigned timer_set:1;

    unsigned delayed:1;

    unsigned deferred_accept:1;


    unsigned pending_eof:1;

    unsigned posted:1;

    unsigned closed:1;


    unsigned channel:1;
    unsigned resolver:1;

    unsigned cancelable:1;
# 107 "src/event/ngx_event.h"
    unsigned available:1;


    ngx_event_handler_pt handler;






    ngx_uint_t index;

    ngx_log_t *log;

    ngx_rbtree_node_t timer;


    ngx_queue_t queue;
# 145 "src/event/ngx_event.h"
};
# 177 "src/event/ngx_event.h"
typedef struct {
    ngx_int_t (*add)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    ngx_int_t (*del)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);

    ngx_int_t (*enable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    ngx_int_t (*disable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);

    ngx_int_t (*add_conn)(ngx_connection_t *c);
    ngx_int_t (*del_conn)(ngx_connection_t *c, ngx_uint_t flags);

    ngx_int_t (*notify)(ngx_event_handler_pt handler);

    ngx_int_t (*process_events)(ngx_cycle_t *cycle, ngx_msec_t timer,
                                 ngx_uint_t flags);

    ngx_int_t (*init)(ngx_cycle_t *cycle, ngx_msec_t timer);
    void (*done)(ngx_cycle_t *cycle);
} ngx_event_actions_t;


extern ngx_event_actions_t ngx_event_actions;

extern ngx_uint_t ngx_use_epoll_rdhup;
# 425 "src/event/ngx_event.h"
extern ngx_os_io_t ngx_io;
# 440 "src/event/ngx_event.h"
typedef struct {
    ngx_uint_t connections;
    ngx_uint_t use;

    ngx_flag_t multi_accept;
    ngx_flag_t accept_mutex;

    ngx_msec_t accept_mutex_delay;

    u_char *name;




} ngx_event_conf_t;


typedef struct {
    ngx_str_t *name;

    void *(*create_conf)(ngx_cycle_t *cycle);
    char *(*init_conf)(ngx_cycle_t *cycle, void *conf);

    ngx_event_actions_t actions;
} ngx_event_module_t;


extern ngx_atomic_t *ngx_connection_counter;

extern ngx_atomic_t *ngx_accept_mutex_ptr;
extern ngx_shmtx_t ngx_accept_mutex;
extern ngx_uint_t ngx_use_accept_mutex;
extern ngx_uint_t ngx_accept_events;
extern ngx_uint_t ngx_accept_mutex_held;
extern ngx_msec_t ngx_accept_mutex_delay;
extern ngx_int_t ngx_accept_disabled;
# 495 "src/event/ngx_event.h"
extern sig_atomic_t ngx_event_timer_alarm;
extern ngx_uint_t ngx_event_flags;
extern ngx_module_t ngx_events_module;
extern ngx_module_t ngx_event_core_module;







void ngx_event_accept(ngx_event_t *ev);

void ngx_event_recvmsg(ngx_event_t *ev);
void ngx_udp_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);

ngx_int_t ngx_trylock_accept_mutex(ngx_cycle_t *cycle);
ngx_int_t ngx_enable_accept_events(ngx_cycle_t *cycle);
u_char *ngx_accept_log_error(ngx_log_t *log, u_char *buf, size_t len);





void ngx_process_events_and_timers(ngx_cycle_t *cycle);
ngx_int_t ngx_handle_read_event(ngx_event_t *rev, ngx_uint_t flags);
ngx_int_t ngx_handle_write_event(ngx_event_t *wev, size_t lowat);
# 532 "src/event/ngx_event.h"
ngx_int_t ngx_send_lowat(ngx_connection_t *c, size_t lowat);






# 1 "src/event/ngx_event_timer.h" 1
# 14 "src/event/ngx_event_timer.h"
# 1 "src/event/ngx_event.h" 1
# 15 "src/event/ngx_event_timer.h" 2







ngx_int_t ngx_event_timer_init(ngx_log_t *log);
ngx_msec_t ngx_event_find_timer(void);
void ngx_event_expire_timers(void);
ngx_int_t ngx_event_no_timers_left(void);


extern ngx_rbtree_t ngx_event_timer_rbtree;


static inline void
ngx_event_del_timer(ngx_event_t *ev)
{
   

                                                             ;

    ngx_rbtree_delete(&ngx_event_timer_rbtree, &ev->timer);







    ev->timer_set = 0;
}


static inline void
ngx_event_add_timer(ngx_event_t *ev, ngx_msec_t timer)
{
    ngx_msec_t key;
    ngx_msec_int_t diff;

    key = ngx_current_msec + timer;

    if (ev->timer_set) {







        diff = (ngx_msec_int_t) (key - ev->timer.key);

        if ((((diff) >= 0) ? (diff) : - (diff)) < 300) {
           

                                                                          ;
            return;
        }

        ngx_event_del_timer(ev);
    }

    ev->timer.key = key;

   

                                                                    ;

    ngx_rbtree_insert(&ngx_event_timer_rbtree, &ev->timer);

    ev->timer_set = 1;
}
# 540 "src/event/ngx_event.h" 2
# 1 "src/event/ngx_event_posted.h" 1
# 41 "src/event/ngx_event_posted.h"
void ngx_event_process_posted(ngx_cycle_t *cycle, ngx_queue_t *posted);


extern ngx_queue_t ngx_posted_accept_events;
extern ngx_queue_t ngx_posted_events;
# 541 "src/event/ngx_event.h" 2
# 15 "src/http/ngx_http_upstream.h" 2
# 1 "src/event/ngx_event_connect.h" 1
# 22 "src/event/ngx_event_connect.h"
typedef struct ngx_peer_connection_s ngx_peer_connection_t;

typedef ngx_int_t (*ngx_event_get_peer_pt)(ngx_peer_connection_t *pc,
    void *data);
typedef void (*ngx_event_free_peer_pt)(ngx_peer_connection_t *pc, void *data,
    ngx_uint_t state);
typedef void (*ngx_event_notify_peer_pt)(ngx_peer_connection_t *pc,
    void *data, ngx_uint_t type);
typedef ngx_int_t (*ngx_event_set_peer_session_pt)(ngx_peer_connection_t *pc,
    void *data);
typedef void (*ngx_event_save_peer_session_pt)(ngx_peer_connection_t *pc,
    void *data);


struct ngx_peer_connection_s {
    ngx_connection_t *connection;

    struct sockaddr *sockaddr;
    socklen_t socklen;
    ngx_str_t *name;

    ngx_uint_t tries;
    ngx_msec_t start_time;

    ngx_event_get_peer_pt get;
    ngx_event_free_peer_pt free;
    ngx_event_notify_peer_pt notify;
    void *data;






    ngx_addr_t *local;

    int type;
    int rcvbuf;

    ngx_log_t *log;

    unsigned cached:1;
    unsigned transparent:1;


    unsigned log_error:2;

   
   
};


ngx_int_t ngx_event_connect_peer(ngx_peer_connection_t *pc);
ngx_int_t ngx_event_get_peer(ngx_peer_connection_t *pc, void *data);
# 16 "src/http/ngx_http_upstream.h" 2
# 1 "src/event/ngx_event_pipe.h" 1
# 17 "src/event/ngx_event_pipe.h"
typedef struct ngx_event_pipe_s ngx_event_pipe_t;

typedef ngx_int_t (*ngx_event_pipe_input_filter_pt)(ngx_event_pipe_t *p,
                                                    ngx_buf_t *buf);
typedef ngx_int_t (*ngx_event_pipe_output_filter_pt)(void *data,
                                                     ngx_chain_t *chain);


struct ngx_event_pipe_s {
    ngx_connection_t *upstream;
    ngx_connection_t *downstream;

    ngx_chain_t *free_raw_bufs;
    ngx_chain_t *in;
    ngx_chain_t **last_in;

    ngx_chain_t *writing;

    ngx_chain_t *out;
    ngx_chain_t *free;
    ngx_chain_t *busy;






    ngx_event_pipe_input_filter_pt input_filter;
    void *input_ctx;

    ngx_event_pipe_output_filter_pt output_filter;
    void *output_ctx;
# 57 "src/event/ngx_event_pipe.h"
    unsigned read:1;
    unsigned cacheable:1;
    unsigned single_buf:1;
    unsigned free_bufs:1;
    unsigned upstream_done:1;
    unsigned upstream_error:1;
    unsigned upstream_eof:1;
    unsigned upstream_blocked:1;
    unsigned downstream_done:1;
    unsigned downstream_error:1;
    unsigned cyclic_temp_file:1;
    unsigned aio:1;

    ngx_int_t allocated;
    ngx_bufs_t bufs;
    ngx_buf_tag_t tag;

    ssize_t busy_size;

    off_t read_length;
    off_t length;

    off_t max_temp_file_size;
    ssize_t temp_file_write_size;

    ngx_msec_t read_timeout;
    ngx_msec_t send_timeout;
    ssize_t send_lowat;

    ngx_pool_t *pool;
    ngx_log_t *log;

    ngx_chain_t *preread_bufs;
    size_t preread_size;
    ngx_buf_t *buf_to_file;

    size_t limit_rate;
    time_t start_sec;

    ngx_temp_file_t *temp_file;

               int num;
};


ngx_int_t ngx_event_pipe(ngx_event_pipe_t *p, ngx_int_t do_write);
ngx_int_t ngx_event_pipe_copy_input_filter(ngx_event_pipe_t *p, ngx_buf_t *buf);
ngx_int_t ngx_event_pipe_add_free_buf(ngx_event_pipe_t *p, ngx_buf_t *b);
# 17 "src/http/ngx_http_upstream.h" 2
# 59 "src/http/ngx_http_upstream.h"
typedef struct {
    ngx_uint_t status;
    ngx_msec_t response_time;
    ngx_msec_t connect_time;
    ngx_msec_t header_time;
    ngx_msec_t queue_time;
    off_t response_length;
    off_t bytes_received;

    ngx_str_t *peer;
} ngx_http_upstream_state_t;


typedef struct {
    ngx_hash_t headers_in_hash;
    ngx_array_t upstreams;

} ngx_http_upstream_main_conf_t;

typedef struct ngx_http_upstream_srv_conf_s ngx_http_upstream_srv_conf_t;

typedef ngx_int_t (*ngx_http_upstream_init_pt)(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
typedef ngx_int_t (*ngx_http_upstream_init_peer_pt)(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);


typedef struct {
    ngx_http_upstream_init_pt init_upstream;
    ngx_http_upstream_init_peer_pt init;
    void *data;
} ngx_http_upstream_peer_t;


typedef struct {
    ngx_str_t name;
    ngx_addr_t *addrs;
    ngx_uint_t naddrs;
    ngx_uint_t weight;
    ngx_uint_t max_conns;
    ngx_uint_t max_fails;
    time_t fail_timeout;
    ngx_msec_t slow_start;
    ngx_uint_t down;

    unsigned backup:1;

   
   
} ngx_http_upstream_server_t;
# 120 "src/http/ngx_http_upstream.h"
struct ngx_http_upstream_srv_conf_s {
    ngx_http_upstream_peer_t peer;
    void **srv_conf;

    ngx_array_t *servers;

    ngx_uint_t flags;
    ngx_str_t host;
    u_char *file_name;
    ngx_uint_t line;
    in_port_t port;
    ngx_uint_t no_port;


    ngx_shm_zone_t *shm_zone;

};


typedef struct {
    ngx_addr_t *addr;
    ngx_http_complex_value_t *value;

    ngx_uint_t transparent;

} ngx_http_upstream_local_t;


typedef struct {
    ngx_http_upstream_srv_conf_t *upstream;

    ngx_msec_t connect_timeout;
    ngx_msec_t send_timeout;
    ngx_msec_t read_timeout;
    ngx_msec_t next_upstream_timeout;

    size_t send_lowat;
    size_t buffer_size;
    size_t limit_rate;

    size_t busy_buffers_size;
    size_t max_temp_file_size;
    size_t temp_file_write_size;

    size_t busy_buffers_size_conf;
    size_t max_temp_file_size_conf;
    size_t temp_file_write_size_conf;

    ngx_bufs_t bufs;

    ngx_uint_t ignore_headers;
    ngx_uint_t next_upstream;
    ngx_uint_t store_access;
    ngx_uint_t next_upstream_tries;
    ngx_flag_t buffering;
    ngx_flag_t request_buffering;
    ngx_flag_t pass_request_headers;
    ngx_flag_t pass_request_body;

    ngx_flag_t ignore_client_abort;
    ngx_flag_t intercept_errors;
    ngx_flag_t cyclic_temp_file;
    ngx_flag_t force_ranges;

    ngx_path_t *temp_path;

    ngx_hash_t hide_headers_hash;
    ngx_array_t *hide_headers;
    ngx_array_t *pass_headers;

    ngx_http_upstream_local_t *local;


    ngx_shm_zone_t *cache_zone;
    ngx_http_complex_value_t *cache_value;

    ngx_uint_t cache_min_uses;
    ngx_uint_t cache_use_stale;
    ngx_uint_t cache_methods;

    off_t cache_max_range_offset;

    ngx_flag_t cache_lock;
    ngx_msec_t cache_lock_timeout;
    ngx_msec_t cache_lock_age;

    ngx_flag_t cache_revalidate;
    ngx_flag_t cache_convert_head;
    ngx_flag_t cache_background_update;

    ngx_array_t *cache_valid;
    ngx_array_t *cache_bypass;
    ngx_array_t *cache_purge;
    ngx_array_t *no_cache;


    ngx_array_t *store_lengths;
    ngx_array_t *store_values;


    signed cache:2;

    signed store:2;
    unsigned intercept_404:1;
    unsigned change_buffering:1;
    unsigned pass_trailers:1;
    unsigned preserve_output:1;
# 237 "src/http/ngx_http_upstream.h"
    ngx_str_t module;

   
   
} ngx_http_upstream_conf_t;


typedef struct {
    ngx_str_t name;
    ngx_http_header_handler_pt handler;
    ngx_uint_t offset;
    ngx_http_header_handler_pt copy_handler;
    ngx_uint_t conf;
    ngx_uint_t redirect;
} ngx_http_upstream_header_t;


typedef struct {
    ngx_list_t headers;
    ngx_list_t trailers;

    ngx_uint_t status_n;
    ngx_str_t status_line;

    ngx_table_elt_t *status;
    ngx_table_elt_t *date;
    ngx_table_elt_t *server;
    ngx_table_elt_t *connection;

    ngx_table_elt_t *expires;
    ngx_table_elt_t *etag;
    ngx_table_elt_t *x_accel_expires;
    ngx_table_elt_t *x_accel_redirect;
    ngx_table_elt_t *x_accel_limit_rate;

    ngx_table_elt_t *content_type;
    ngx_table_elt_t *content_length;

    ngx_table_elt_t *last_modified;
    ngx_table_elt_t *location;
    ngx_table_elt_t *accept_ranges;
    ngx_table_elt_t *www_authenticate;
    ngx_table_elt_t *transfer_encoding;
    ngx_table_elt_t *vary;





    ngx_array_t cache_control;
    ngx_array_t cookies;

    off_t content_length_n;
    time_t last_modified_time;

    unsigned connection_close:1;
    unsigned chunked:1;
} ngx_http_upstream_headers_in_t;


typedef struct {
    ngx_str_t host;
    in_port_t port;
    ngx_uint_t no_port;

    ngx_uint_t naddrs;
    ngx_resolver_addr_t *addrs;

    struct sockaddr *sockaddr;
    socklen_t socklen;
    ngx_str_t name;

    ngx_resolver_ctx_t *ctx;
} ngx_http_upstream_resolved_t;


typedef void (*ngx_http_upstream_handler_pt)(ngx_http_request_t *r,
    ngx_http_upstream_t *u);


struct ngx_http_upstream_s {
    ngx_http_upstream_handler_pt read_event_handler;
    ngx_http_upstream_handler_pt write_event_handler;

    ngx_peer_connection_t peer;

    ngx_event_pipe_t *pipe;

    ngx_chain_t *request_bufs;

    ngx_output_chain_ctx_t output;
    ngx_chain_writer_ctx_t writer;

    ngx_http_upstream_conf_t *conf;
    ngx_http_upstream_srv_conf_t *upstream;

    ngx_array_t *caches;


    ngx_http_upstream_headers_in_t headers_in;

    ngx_http_upstream_resolved_t *resolved;

    ngx_buf_t from_client;

    ngx_buf_t buffer;
    off_t length;

    ngx_chain_t *out_bufs;
    ngx_chain_t *busy_bufs;
    ngx_chain_t *free_bufs;

    ngx_int_t (*input_filter_init)(void *data);
    ngx_int_t (*input_filter)(void *data, ssize_t bytes);
    void *input_filter_ctx;


    ngx_int_t (*create_key)(ngx_http_request_t *r);

    ngx_int_t (*create_request)(ngx_http_request_t *r);
    ngx_int_t (*reinit_request)(ngx_http_request_t *r);
    ngx_int_t (*process_header)(ngx_http_request_t *r);
    void (*abort_request)(ngx_http_request_t *r);
    void (*finalize_request)(ngx_http_request_t *r,
                                         ngx_int_t rc);
    ngx_int_t (*rewrite_redirect)(ngx_http_request_t *r,
                                         ngx_table_elt_t *h, size_t prefix);
    ngx_int_t (*rewrite_cookie)(ngx_http_request_t *r,
                                         ngx_table_elt_t *h);

    ngx_msec_t timeout;

    ngx_http_upstream_state_t *state;

    ngx_str_t method;
    ngx_str_t schema;
    ngx_str_t uri;





    ngx_http_cleanup_pt *cleanup;

    unsigned store:1;
    unsigned cacheable:1;
    unsigned accel:1;
    unsigned ssl:1;

    unsigned cache_status:3;


    unsigned buffering:1;
    unsigned keepalive:1;
    unsigned upgrade:1;

    unsigned request_sent:1;
    unsigned request_body_sent:1;
    unsigned request_body_blocked:1;
    unsigned header_sent:1;
};


typedef struct {
    ngx_uint_t status;
    ngx_uint_t mask;
} ngx_http_upstream_next_t;


typedef struct {
    ngx_str_t key;
    ngx_str_t value;
    ngx_uint_t skip_empty;
} ngx_http_upstream_param_t;


ngx_int_t ngx_http_upstream_create(ngx_http_request_t *r);
void ngx_http_upstream_init(ngx_http_request_t *r);
ngx_http_upstream_srv_conf_t *ngx_http_upstream_add(ngx_conf_t *cf,
    ngx_url_t *u, ngx_uint_t flags);
char *ngx_http_upstream_bind_set_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
char *ngx_http_upstream_param_set_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
ngx_int_t ngx_http_upstream_hide_headers_hash(ngx_conf_t *cf,
    ngx_http_upstream_conf_t *conf, ngx_http_upstream_conf_t *prev,
    ngx_str_t *default_hide_headers, ngx_hash_init_t *hash);






extern ngx_module_t ngx_http_upstream_module;
extern ngx_conf_bitmask_t ngx_http_upstream_cache_method_mask[];
extern ngx_conf_bitmask_t ngx_http_upstream_ignore_headers_masks[];
# 35 "src/http/ngx_http.h" 2
# 1 "src/http/ngx_http_upstream_round_robin.h" 1
# 17 "src/http/ngx_http_upstream_round_robin.h"
typedef struct ngx_http_upstream_rr_peer_s ngx_http_upstream_rr_peer_t;

struct ngx_http_upstream_rr_peer_s {
    struct sockaddr *sockaddr;
    socklen_t socklen;
    ngx_str_t name;
    ngx_str_t server;

    ngx_int_t current_weight;
    ngx_int_t effective_weight;
    ngx_int_t weight;

    ngx_uint_t conns;
    ngx_uint_t max_conns;

    ngx_uint_t fails;
    time_t accessed;
    time_t checked;

    ngx_uint_t max_fails;
    time_t fail_timeout;
    ngx_msec_t slow_start;
    ngx_msec_t start_time;

    ngx_uint_t down;







    ngx_atomic_t lock;


    ngx_http_upstream_rr_peer_t *next;

   
   
};


typedef struct ngx_http_upstream_rr_peers_s ngx_http_upstream_rr_peers_t;

struct ngx_http_upstream_rr_peers_s {
    ngx_uint_t number;


    ngx_slab_pool_t *shpool;
    ngx_atomic_t rwlock;
    ngx_http_upstream_rr_peers_t *zone_next;


    ngx_uint_t total_weight;

    unsigned single:1;
    unsigned weighted:1;

    ngx_str_t *name;

    ngx_http_upstream_rr_peers_t *next;

    ngx_http_upstream_rr_peer_t *peer;
};
# 127 "src/http/ngx_http_upstream_round_robin.h"
typedef struct {
    ngx_uint_t config;
    ngx_http_upstream_rr_peers_t *peers;
    ngx_http_upstream_rr_peer_t *current;
    uintptr_t *tried;
    uintptr_t data;
} ngx_http_upstream_rr_peer_data_t;


ngx_int_t ngx_http_upstream_init_round_robin(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
ngx_int_t ngx_http_upstream_init_round_robin_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
ngx_int_t ngx_http_upstream_create_round_robin_peer(ngx_http_request_t *r,
    ngx_http_upstream_resolved_t *ur);
ngx_int_t ngx_http_upstream_get_round_robin_peer(ngx_peer_connection_t *pc,
    void *data);
void ngx_http_upstream_free_round_robin_peer(ngx_peer_connection_t *pc,
    void *data, ngx_uint_t state);
# 36 "src/http/ngx_http.h" 2
# 1 "src/http/ngx_http_core_module.h" 1
# 63 "src/http/ngx_http_core_module.h"
typedef struct ngx_http_location_tree_node_s ngx_http_location_tree_node_t;
typedef struct ngx_http_core_loc_conf_s ngx_http_core_loc_conf_t;


typedef struct {
    ngx_sockaddr_t sockaddr;
    socklen_t socklen;

    unsigned set:1;
    unsigned default_server:1;
    unsigned bind:1;
    unsigned wildcard:1;
    unsigned ssl:1;
    unsigned http2:1;

    unsigned ipv6only:1;

    unsigned deferred_accept:1;
    unsigned reuseport:1;
    unsigned so_keepalive:2;
    unsigned proxy_protocol:1;

    int backlog;
    int rcvbuf;
    int sndbuf;




    int fastopen;


    int tcp_keepidle;
    int tcp_keepintvl;
    int tcp_keepcnt;






    u_char addr[114];
} ngx_http_listen_opt_t;


typedef enum {
    NGX_HTTP_POST_READ_PHASE = 0,

    NGX_HTTP_SERVER_REWRITE_PHASE,

    NGX_HTTP_FIND_CONFIG_PHASE,
    NGX_HTTP_REWRITE_PHASE,
    NGX_HTTP_POST_REWRITE_PHASE,

    NGX_HTTP_PREACCESS_PHASE,

    NGX_HTTP_ACCESS_PHASE,
    NGX_HTTP_POST_ACCESS_PHASE,

    NGX_HTTP_PRECONTENT_PHASE,

    NGX_HTTP_CONTENT_PHASE,

    NGX_HTTP_LOG_PHASE
} ngx_http_phases;

typedef struct ngx_http_phase_handler_s ngx_http_phase_handler_t;

typedef ngx_int_t (*ngx_http_phase_handler_pt)(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph);

struct ngx_http_phase_handler_s {
    ngx_http_phase_handler_pt checker;
    ngx_http_handler_pt handler;
    ngx_uint_t next;
};


typedef struct {
    ngx_http_phase_handler_t *handlers;
    ngx_uint_t server_rewrite_index;
    ngx_uint_t location_rewrite_index;
} ngx_http_phase_engine_t;


typedef struct {
    ngx_array_t handlers;
} ngx_http_phase_t;


typedef struct {
    ngx_array_t servers;

    ngx_http_phase_engine_t phase_engine;

    ngx_hash_t headers_in_hash;

    ngx_hash_t variables_hash;

    ngx_array_t variables;
    ngx_array_t prefix_variables;
    ngx_uint_t ncaptures;

    ngx_uint_t server_names_hash_max_size;
    ngx_uint_t server_names_hash_bucket_size;

    ngx_uint_t variables_hash_max_size;
    ngx_uint_t variables_hash_bucket_size;

    ngx_hash_keys_arrays_t *variables_keys;

    ngx_array_t *ports;

    ngx_http_phase_t phases[NGX_HTTP_LOG_PHASE + 1];
} ngx_http_core_main_conf_t;


typedef struct {

    ngx_array_t server_names;


    ngx_http_conf_ctx_t *ctx;

    u_char *file_name;
    ngx_uint_t line;

    ngx_str_t server_name;

    size_t connection_pool_size;
    size_t request_pool_size;
    size_t client_header_buffer_size;

    ngx_bufs_t large_client_header_buffers;

    ngx_msec_t client_header_timeout;

    ngx_flag_t ignore_invalid_headers;
    ngx_flag_t merge_slashes;
    ngx_flag_t underscores_in_headers;

    unsigned listen:1;




    ngx_http_core_loc_conf_t **named_locations;
} ngx_http_core_srv_conf_t;





typedef struct {



    ngx_http_core_srv_conf_t *server;
    ngx_str_t name;
} ngx_http_server_name_t;


typedef struct {
    ngx_hash_combined_t names;

    ngx_uint_t nregex;
    ngx_http_server_name_t *regex;
} ngx_http_virtual_names_t;


struct ngx_http_addr_conf_s {

    ngx_http_core_srv_conf_t *default_server;

    ngx_http_virtual_names_t *virtual_names;

    unsigned ssl:1;
    unsigned http2:1;
    unsigned proxy_protocol:1;
};


typedef struct {
    in_addr_t addr;
    ngx_http_addr_conf_t conf;
} ngx_http_in_addr_t;




typedef struct {
    struct in6_addr addr6;
    ngx_http_addr_conf_t conf;
} ngx_http_in6_addr_t;




typedef struct {

    void *addrs;
    ngx_uint_t naddrs;
} ngx_http_port_t;


typedef struct {
    ngx_int_t family;
    in_port_t port;
    ngx_array_t addrs;
} ngx_http_conf_port_t;


typedef struct {
    ngx_http_listen_opt_t opt;

    ngx_hash_t hash;
    ngx_hash_wildcard_t *wc_head;
    ngx_hash_wildcard_t *wc_tail;







    ngx_http_core_srv_conf_t *default_server;
    ngx_array_t servers;
} ngx_http_conf_addr_t;


typedef struct {
    ngx_int_t status;
    ngx_int_t overwrite;
    ngx_http_complex_value_t value;
    ngx_str_t args;
} ngx_http_err_page_t;


struct ngx_http_core_loc_conf_s {
    ngx_str_t name;





    unsigned noname:1;
    unsigned lmt_excpt:1;
    unsigned named:1;

    unsigned exact_match:1;
    unsigned noregex:1;

    unsigned auto_redirect:1;





    ngx_http_location_tree_node_t *static_locations;





    void **loc_conf;

    uint32_t limit_except;
    void **limit_except_loc_conf;

    ngx_http_handler_pt handler;


    size_t alias;
    ngx_str_t root;
    ngx_str_t post_action;

    ngx_array_t *root_lengths;
    ngx_array_t *root_values;

    ngx_array_t *types;
    ngx_hash_t types_hash;
    ngx_str_t default_type;

    off_t client_max_body_size;
    off_t directio;
    off_t directio_alignment;

    size_t client_body_buffer_size;
    size_t send_lowat;
    size_t postpone_output;
    size_t limit_rate;
    size_t limit_rate_after;
    size_t sendfile_max_chunk;
    size_t read_ahead;
    size_t subrequest_output_buffer_size;


    ngx_msec_t client_body_timeout;
    ngx_msec_t send_timeout;
    ngx_msec_t keepalive_timeout;
    ngx_msec_t lingering_time;
    ngx_msec_t lingering_timeout;
    ngx_msec_t resolver_timeout;

    ngx_resolver_t *resolver;

    time_t keepalive_header;

    ngx_uint_t keepalive_requests;
    ngx_uint_t keepalive_disable;
    ngx_uint_t satisfy;
    ngx_uint_t lingering_close;
    ngx_uint_t if_modified_since;
    ngx_uint_t max_ranges;
    ngx_uint_t client_body_in_file_only;

    ngx_flag_t client_body_in_single_buffer;

    ngx_flag_t internal;
    ngx_flag_t sendfile;
    ngx_flag_t aio;
    ngx_flag_t aio_write;
    ngx_flag_t tcp_nopush;
    ngx_flag_t tcp_nodelay;
    ngx_flag_t reset_timedout_connection;
    ngx_flag_t absolute_redirect;
    ngx_flag_t server_name_in_redirect;
    ngx_flag_t port_in_redirect;
    ngx_flag_t msie_padding;
    ngx_flag_t msie_refresh;
    ngx_flag_t log_not_found;
    ngx_flag_t log_subrequest;
    ngx_flag_t recursive_error_pages;
    ngx_uint_t server_tokens;
    ngx_flag_t chunked_transfer_encoding;
    ngx_flag_t etag;
# 417 "src/http/ngx_http_core_module.h"
    ngx_uint_t disable_symlinks;
    ngx_http_complex_value_t *disable_symlinks_from;


    ngx_array_t *error_pages;

    ngx_path_t *client_body_temp_path;

    ngx_open_file_cache_t *open_file_cache;
    time_t open_file_cache_valid;
    ngx_uint_t open_file_cache_min_uses;
    ngx_flag_t open_file_cache_errors;
    ngx_flag_t open_file_cache_events;

    ngx_log_t *error_log;

    ngx_uint_t types_hash_max_size;
    ngx_uint_t types_hash_bucket_size;

    ngx_queue_t *locations;




};


typedef struct {
    ngx_queue_t queue;
    ngx_http_core_loc_conf_t *exact;
    ngx_http_core_loc_conf_t *inclusive;
    ngx_str_t *name;
    u_char *file_name;
    ngx_uint_t line;
    ngx_queue_t list;
} ngx_http_location_queue_t;


struct ngx_http_location_tree_node_s {
    ngx_http_location_tree_node_t *left;
    ngx_http_location_tree_node_t *right;
    ngx_http_location_tree_node_t *tree;

    ngx_http_core_loc_conf_t *exact;
    ngx_http_core_loc_conf_t *inclusive;

    u_char auto_redirect;
    u_char len;
    u_char name[1];
};


void ngx_http_core_run_phases(ngx_http_request_t *r);
ngx_int_t ngx_http_core_generic_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph);
ngx_int_t ngx_http_core_rewrite_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph);
ngx_int_t ngx_http_core_find_config_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph);
ngx_int_t ngx_http_core_post_rewrite_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph);
ngx_int_t ngx_http_core_access_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph);
ngx_int_t ngx_http_core_post_access_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph);
ngx_int_t ngx_http_core_content_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph);


void *ngx_http_test_content_type(ngx_http_request_t *r, ngx_hash_t *types_hash);
ngx_int_t ngx_http_set_content_type(ngx_http_request_t *r);
void ngx_http_set_exten(ngx_http_request_t *r);
ngx_int_t ngx_http_set_etag(ngx_http_request_t *r);
void ngx_http_weak_etag(ngx_http_request_t *r);
ngx_int_t ngx_http_send_response(ngx_http_request_t *r, ngx_uint_t status,
    ngx_str_t *ct, ngx_http_complex_value_t *cv);
u_char *ngx_http_map_uri_to_path(ngx_http_request_t *r, ngx_str_t *name,
    size_t *root_length, size_t reserved);
ngx_int_t ngx_http_auth_basic_user(ngx_http_request_t *r);





ngx_int_t ngx_http_subrequest(ngx_http_request_t *r,
    ngx_str_t *uri, ngx_str_t *args, ngx_http_request_t **sr,
    ngx_http_post_subrequest_t *psr, ngx_uint_t flags);
ngx_int_t ngx_http_internal_redirect(ngx_http_request_t *r,
    ngx_str_t *uri, ngx_str_t *args);
ngx_int_t ngx_http_named_location(ngx_http_request_t *r, ngx_str_t *name);


ngx_http_cleanup_t *ngx_http_cleanup_add(ngx_http_request_t *r, size_t size);


typedef ngx_int_t (*ngx_http_output_header_filter_pt)(ngx_http_request_t *r);
typedef ngx_int_t (*ngx_http_output_body_filter_pt)
    (ngx_http_request_t *r, ngx_chain_t *chain);
typedef ngx_int_t (*ngx_http_request_body_filter_pt)
    (ngx_http_request_t *r, ngx_chain_t *chain);


ngx_int_t ngx_http_output_filter(ngx_http_request_t *r, ngx_chain_t *chain);
ngx_int_t ngx_http_write_filter(ngx_http_request_t *r, ngx_chain_t *chain);
ngx_int_t ngx_http_request_body_save_filter(ngx_http_request_t *r,
    ngx_chain_t *chain);


ngx_int_t ngx_http_set_disable_symlinks(ngx_http_request_t *r,
    ngx_http_core_loc_conf_t *clcf, ngx_str_t *path, ngx_open_file_info_t *of);

ngx_int_t ngx_http_get_forwarded_addr(ngx_http_request_t *r, ngx_addr_t *addr,
    ngx_array_t *headers, ngx_str_t *value, ngx_array_t *proxies,
    int recursive);


extern ngx_module_t ngx_http_core_module;

extern ngx_uint_t ngx_http_max_module;

extern ngx_str_t ngx_http_core_get_method;
# 37 "src/http/ngx_http.h" 2





# 1 "src/http/ngx_http_cache.h" 1
# 33 "src/http/ngx_http_cache.h"
typedef struct {
    ngx_uint_t status;
    time_t valid;
} ngx_http_cache_valid_t;


typedef struct {
    ngx_rbtree_node_t node;
    ngx_queue_t queue;

    u_char key[16
                                         - sizeof(ngx_rbtree_key_t)];

    unsigned count:20;
    unsigned uses:10;
    unsigned valid_msec:10;
    unsigned error:10;
    unsigned exists:1;
    unsigned updating:1;
    unsigned deleting:1;
    unsigned purged:1;


    ngx_file_uniq_t uniq;
    time_t expire;
    time_t valid_sec;
    size_t body_start;
    off_t fs_size;
    ngx_msec_t lock_time;
} ngx_http_file_cache_node_t;


struct ngx_http_cache_s {
    ngx_file_t file;
    ngx_array_t keys;
    uint32_t crc32;
    u_char key[16];
    u_char main[16];

    ngx_file_uniq_t uniq;
    time_t valid_sec;
    time_t updating_sec;
    time_t error_sec;
    time_t last_modified;
    time_t date;

    ngx_str_t etag;
    ngx_str_t vary;
    u_char variant[16];

    size_t header_start;
    size_t body_start;
    off_t length;
    off_t fs_size;

    ngx_uint_t min_uses;
    ngx_uint_t error;
    ngx_uint_t valid_msec;
    ngx_uint_t vary_tag;

    ngx_buf_t *buf;

    ngx_http_file_cache_t *file_cache;
    ngx_http_file_cache_node_t *node;





    ngx_msec_t lock_timeout;
    ngx_msec_t lock_age;
    ngx_msec_t lock_time;
    ngx_msec_t wait_time;

    ngx_event_t wait_event;

    unsigned lock:1;
    unsigned waiting:1;

    unsigned updated:1;
    unsigned updating:1;
    unsigned exists:1;
    unsigned temp_file:1;
    unsigned purged:1;
    unsigned reading:1;
    unsigned secondary:1;
    unsigned background:1;

    unsigned stale_updating:1;
    unsigned stale_error:1;
};


typedef struct {
    ngx_uint_t version;
    time_t valid_sec;
    time_t updating_sec;
    time_t error_sec;
    time_t last_modified;
    time_t date;
    uint32_t crc32;
    u_short valid_msec;
    u_short header_start;
    u_short body_start;
    u_char etag_len;
    u_char etag[128];
    u_char vary_len;
    u_char vary[128];
    u_char variant[16];
} ngx_http_file_cache_header_t;


typedef struct {
    ngx_rbtree_t rbtree;
    ngx_rbtree_node_t sentinel;
    ngx_queue_t queue;
    ngx_atomic_t cold;
    ngx_atomic_t loading;
    off_t size;
    ngx_uint_t count;
    ngx_uint_t watermark;
} ngx_http_file_cache_sh_t;


struct ngx_http_file_cache_s {
    ngx_http_file_cache_sh_t *sh;
    ngx_slab_pool_t *shpool;

    ngx_path_t *path;

    off_t max_size;
    size_t bsize;

    time_t inactive;

    time_t fail_time;

    ngx_uint_t files;
    ngx_uint_t loader_files;
    ngx_msec_t last;
    ngx_msec_t loader_sleep;
    ngx_msec_t loader_threshold;

    ngx_uint_t manager_files;
    ngx_msec_t manager_sleep;
    ngx_msec_t manager_threshold;

    ngx_shm_zone_t *shm_zone;

    ngx_uint_t use_temp_path;

};


ngx_int_t ngx_http_file_cache_new(ngx_http_request_t *r);
ngx_int_t ngx_http_file_cache_create(ngx_http_request_t *r);
void ngx_http_file_cache_create_key(ngx_http_request_t *r);
ngx_int_t ngx_http_file_cache_open(ngx_http_request_t *r);
ngx_int_t ngx_http_file_cache_set_header(ngx_http_request_t *r, u_char *buf);
void ngx_http_file_cache_update(ngx_http_request_t *r, ngx_temp_file_t *tf);
void ngx_http_file_cache_update_header(ngx_http_request_t *r);
ngx_int_t ngx_http_cache_send(ngx_http_request_t *);
void ngx_http_file_cache_free(ngx_http_cache_t *c, ngx_temp_file_t *tf);
time_t ngx_http_file_cache_valid(ngx_array_t *cache_valid, ngx_uint_t status);

char *ngx_http_file_cache_set_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
char *ngx_http_file_cache_valid_set_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


extern ngx_str_t ngx_http_cache_status[];
# 43 "src/http/ngx_http.h" 2
# 52 "src/http/ngx_http.h"
struct ngx_http_log_ctx_s {
    ngx_connection_t *connection;
    ngx_http_request_t *request;
    ngx_http_request_t *current_request;
};


struct ngx_http_chunked_s {
    ngx_uint_t state;
    off_t size;
    off_t length;
};


typedef struct {
    ngx_uint_t http_version;
    ngx_uint_t code;
    ngx_uint_t count;
    u_char *start;
    u_char *end;
} ngx_http_status_t;






ngx_int_t ngx_http_add_location(ngx_conf_t *cf, ngx_queue_t **locations,
    ngx_http_core_loc_conf_t *clcf);
ngx_int_t ngx_http_add_listen(ngx_conf_t *cf, ngx_http_core_srv_conf_t *cscf,
    ngx_http_listen_opt_t *lsopt);


void ngx_http_init_connection(ngx_connection_t *c);
void ngx_http_close_connection(ngx_connection_t *c);





ngx_int_t ngx_http_parse_request_line(ngx_http_request_t *r, ngx_buf_t *b);
ngx_int_t ngx_http_parse_uri(ngx_http_request_t *r);
ngx_int_t ngx_http_parse_complex_uri(ngx_http_request_t *r,
    ngx_uint_t merge_slashes);
ngx_int_t ngx_http_parse_status_line(ngx_http_request_t *r, ngx_buf_t *b,
    ngx_http_status_t *status);
ngx_int_t ngx_http_parse_unsafe_uri(ngx_http_request_t *r, ngx_str_t *uri,
    ngx_str_t *args, ngx_uint_t *flags);
ngx_int_t ngx_http_parse_header_line(ngx_http_request_t *r, ngx_buf_t *b,
    ngx_uint_t allow_underscores);
ngx_int_t ngx_http_parse_multi_header_lines(ngx_array_t *headers,
    ngx_str_t *name, ngx_str_t *value);
ngx_int_t ngx_http_parse_set_cookie_lines(ngx_array_t *headers,
    ngx_str_t *name, ngx_str_t *value);
ngx_int_t ngx_http_arg(ngx_http_request_t *r, u_char *name, size_t len,
    ngx_str_t *value);
void ngx_http_split_args(ngx_http_request_t *r, ngx_str_t *uri,
    ngx_str_t *args);
ngx_int_t ngx_http_parse_chunked(ngx_http_request_t *r, ngx_buf_t *b,
    ngx_http_chunked_t *ctx);


ngx_http_request_t *ngx_http_create_request(ngx_connection_t *c);
ngx_int_t ngx_http_process_request_uri(ngx_http_request_t *r);
ngx_int_t ngx_http_process_request_header(ngx_http_request_t *r);
void ngx_http_process_request(ngx_http_request_t *r);
void ngx_http_update_location_config(ngx_http_request_t *r);
void ngx_http_handler(ngx_http_request_t *r);
void ngx_http_run_posted_requests(ngx_connection_t *c);
ngx_int_t ngx_http_post_request(ngx_http_request_t *r,
    ngx_http_posted_request_t *pr);
void ngx_http_finalize_request(ngx_http_request_t *r, ngx_int_t rc);
void ngx_http_free_request(ngx_http_request_t *r, ngx_int_t rc);

void ngx_http_empty_handler(ngx_event_t *wev);
void ngx_http_request_empty_handler(ngx_http_request_t *r);





ngx_int_t ngx_http_send_special(ngx_http_request_t *r, ngx_uint_t flags);


ngx_int_t ngx_http_read_client_request_body(ngx_http_request_t *r,
    ngx_http_client_body_handler_pt post_handler);
ngx_int_t ngx_http_read_unbuffered_request_body(ngx_http_request_t *r);

ngx_int_t ngx_http_send_header(ngx_http_request_t *r);
ngx_int_t ngx_http_special_response_handler(ngx_http_request_t *r,
    ngx_int_t error);
ngx_int_t ngx_http_filter_finalize_request(ngx_http_request_t *r,
    ngx_module_t *m, ngx_int_t error);
void ngx_http_clean_header(ngx_http_request_t *r);


ngx_int_t ngx_http_discard_request_body(ngx_http_request_t *r);
void ngx_http_discarded_request_body_handler(ngx_http_request_t *r);
void ngx_http_block_reading(ngx_http_request_t *r);
void ngx_http_test_reading(ngx_http_request_t *r);


char *ngx_http_types_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_http_merge_types(ngx_conf_t *cf, ngx_array_t **keys,
    ngx_hash_t *types_hash, ngx_array_t **prev_keys,
    ngx_hash_t *prev_types_hash, ngx_str_t *default_types);
ngx_int_t ngx_http_set_default_types(ngx_conf_t *cf, ngx_array_t **types,
    ngx_str_t *default_type);






extern ngx_module_t ngx_http_module;

extern ngx_str_t ngx_http_html_default_types[];


extern ngx_http_output_header_filter_pt ngx_http_top_header_filter;
extern ngx_http_output_body_filter_pt ngx_http_top_body_filter;
extern ngx_http_request_body_filter_pt ngx_http_top_request_body_filter;
# 11 "src/http/ngx_http_upstream.c" 2



static ngx_int_t ngx_http_upstream_cache(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static ngx_int_t ngx_http_upstream_cache_get(ngx_http_request_t *r,
    ngx_http_upstream_t *u, ngx_http_file_cache_t **cache);
static ngx_int_t ngx_http_upstream_cache_send(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static ngx_int_t ngx_http_upstream_cache_background_update(
    ngx_http_request_t *r, ngx_http_upstream_t *u);
static ngx_int_t ngx_http_upstream_cache_check_range(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static ngx_int_t ngx_http_upstream_cache_status(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_upstream_cache_last_modified(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_upstream_cache_etag(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);


static void ngx_http_upstream_init_request(ngx_http_request_t *r);
static void ngx_http_upstream_resolve_handler(ngx_resolver_ctx_t *ctx);
static void ngx_http_upstream_rd_check_broken_connection(ngx_http_request_t *r);
static void ngx_http_upstream_wr_check_broken_connection(ngx_http_request_t *r);
static void ngx_http_upstream_check_broken_connection(ngx_http_request_t *r,
    ngx_event_t *ev);
static void ngx_http_upstream_connect(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static ngx_int_t ngx_http_upstream_reinit(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_send_request(ngx_http_request_t *r,
    ngx_http_upstream_t *u, ngx_uint_t do_write);
static ngx_int_t ngx_http_upstream_send_request_body(ngx_http_request_t *r,
    ngx_http_upstream_t *u, ngx_uint_t do_write);
static void ngx_http_upstream_send_request_handler(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_read_request_handler(ngx_http_request_t *r);
static void ngx_http_upstream_process_header(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static ngx_int_t ngx_http_upstream_test_next(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static ngx_int_t ngx_http_upstream_intercept_errors(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static ngx_int_t ngx_http_upstream_test_connect(ngx_connection_t *c);
static ngx_int_t ngx_http_upstream_process_headers(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static ngx_int_t ngx_http_upstream_process_trailers(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_send_response(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_upgrade(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_upgraded_read_downstream(ngx_http_request_t *r);
static void ngx_http_upstream_upgraded_write_downstream(ngx_http_request_t *r);
static void ngx_http_upstream_upgraded_read_upstream(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_upgraded_write_upstream(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_process_upgraded(ngx_http_request_t *r,
    ngx_uint_t from_upstream, ngx_uint_t do_write);
static void
    ngx_http_upstream_process_non_buffered_downstream(ngx_http_request_t *r);
static void
    ngx_http_upstream_process_non_buffered_upstream(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void
    ngx_http_upstream_process_non_buffered_request(ngx_http_request_t *r,
    ngx_uint_t do_write);
static ngx_int_t ngx_http_upstream_non_buffered_filter_init(void *data);
static ngx_int_t ngx_http_upstream_non_buffered_filter(void *data,
    ssize_t bytes);





static ngx_int_t ngx_http_upstream_output_filter(void *data,
    ngx_chain_t *chain);
static void ngx_http_upstream_process_downstream(ngx_http_request_t *r);
static void ngx_http_upstream_process_upstream(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_process_request(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_store(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_dummy_handler(ngx_http_request_t *r,
    ngx_http_upstream_t *u);
static void ngx_http_upstream_next(ngx_http_request_t *r,
    ngx_http_upstream_t *u, ngx_uint_t ft_type);
static void ngx_http_upstream_cleanup(void *data);
static void ngx_http_upstream_finalize_request(ngx_http_request_t *r,
    ngx_http_upstream_t *u, ngx_int_t rc);

static ngx_int_t ngx_http_upstream_process_header_line(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_process_content_length(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_process_last_modified(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_process_set_cookie(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t
    ngx_http_upstream_process_cache_control(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_ignore_header_line(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_process_expires(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_process_accel_expires(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_process_limit_rate(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_process_buffering(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_process_charset(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_process_connection(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t
    ngx_http_upstream_process_transfer_encoding(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_process_vary(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_copy_header_line(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t
    ngx_http_upstream_copy_multi_header_lines(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_copy_content_type(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_copy_last_modified(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_rewrite_location(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_rewrite_refresh(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_rewrite_set_cookie(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);
static ngx_int_t ngx_http_upstream_copy_allow_ranges(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset);






static ngx_int_t ngx_http_upstream_add_variables(ngx_conf_t *cf);
static ngx_int_t ngx_http_upstream_addr_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_upstream_status_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_upstream_response_time_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_upstream_response_length_variable(
    ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_upstream_header_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_upstream_trailer_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_upstream_cookie_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static char *ngx_http_upstream(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy);
static char *ngx_http_upstream_server(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_upstream_set_local(ngx_http_request_t *r,
  ngx_http_upstream_t *u, ngx_http_upstream_local_t *local);

static void *ngx_http_upstream_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_upstream_init_main_conf(ngx_conf_t *cf, void *conf);
# 196 "src/http/ngx_http_upstream.c"
static ngx_http_upstream_header_t ngx_http_upstream_headers_in[] = {

    { { sizeof("Status") - 1, (u_char *) "Status" },
                 ngx_http_upstream_process_header_line,
                 
# 200 "src/http/ngx_http_upstream.c" 3 4
                0
# 200 "src/http/ngx_http_upstream.c"
                                                                ,
                 ngx_http_upstream_copy_header_line, 0, 0 },

    { { sizeof("Content-Type") - 1, (u_char *) "Content-Type" },
                 ngx_http_upstream_process_header_line,
                 
# 205 "src/http/ngx_http_upstream.c" 3 4
                0
# 205 "src/http/ngx_http_upstream.c"
                                                                      ,
                 ngx_http_upstream_copy_content_type, 0, 1 },

    { { sizeof("Content-Length") - 1, (u_char *) "Content-Length" },
                 ngx_http_upstream_process_content_length, 0,
                 ngx_http_upstream_ignore_header_line, 0, 0 },

    { { sizeof("Date") - 1, (u_char *) "Date" },
                 ngx_http_upstream_process_header_line,
                 
# 214 "src/http/ngx_http_upstream.c" 3 4
                0
# 214 "src/http/ngx_http_upstream.c"
                                                              ,
                 ngx_http_upstream_copy_header_line,
                 
# 216 "src/http/ngx_http_upstream.c" 3 4
                0
# 216 "src/http/ngx_http_upstream.c"
                                                      , 0 },

    { { sizeof("Last-Modified") - 1, (u_char *) "Last-Modified" },
                 ngx_http_upstream_process_last_modified, 0,
                 ngx_http_upstream_copy_last_modified, 0, 0 },

    { { sizeof("ETag") - 1, (u_char *) "ETag" },
                 ngx_http_upstream_process_header_line,
                 
# 224 "src/http/ngx_http_upstream.c" 3 4
                0
# 224 "src/http/ngx_http_upstream.c"
                                                              ,
                 ngx_http_upstream_copy_header_line,
                 
# 226 "src/http/ngx_http_upstream.c" 3 4
                0
# 226 "src/http/ngx_http_upstream.c"
                                                      , 0 },

    { { sizeof("Server") - 1, (u_char *) "Server" },
                 ngx_http_upstream_process_header_line,
                 
# 230 "src/http/ngx_http_upstream.c" 3 4
                0
# 230 "src/http/ngx_http_upstream.c"
                                                                ,
                 ngx_http_upstream_copy_header_line,
                 
# 232 "src/http/ngx_http_upstream.c" 3 4
                0
# 232 "src/http/ngx_http_upstream.c"
                                                        , 0 },

    { { sizeof("WWW-Authenticate") - 1, (u_char *) "WWW-Authenticate" },
                 ngx_http_upstream_process_header_line,
                 
# 236 "src/http/ngx_http_upstream.c" 3 4
                0
# 236 "src/http/ngx_http_upstream.c"
                                                                          ,
                 ngx_http_upstream_copy_header_line, 0, 0 },

    { { sizeof("Location") - 1, (u_char *) "Location" },
                 ngx_http_upstream_process_header_line,
                 
# 241 "src/http/ngx_http_upstream.c" 3 4
                0
# 241 "src/http/ngx_http_upstream.c"
                                                                  ,
                 ngx_http_upstream_rewrite_location, 0, 0 },

    { { sizeof("Refresh") - 1, (u_char *) "Refresh" },
                 ngx_http_upstream_ignore_header_line, 0,
                 ngx_http_upstream_rewrite_refresh, 0, 0 },

    { { sizeof("Set-Cookie") - 1, (u_char *) "Set-Cookie" },
                 ngx_http_upstream_process_set_cookie,
                 
# 250 "src/http/ngx_http_upstream.c" 3 4
                0
# 250 "src/http/ngx_http_upstream.c"
                                                                 ,
                 ngx_http_upstream_rewrite_set_cookie, 0, 1 },

    { { sizeof("Content-Disposition") - 1, (u_char *) "Content-Disposition" },
                 ngx_http_upstream_ignore_header_line, 0,
                 ngx_http_upstream_copy_header_line, 0, 1 },

    { { sizeof("Cache-Control") - 1, (u_char *) "Cache-Control" },
                 ngx_http_upstream_process_cache_control, 0,
                 ngx_http_upstream_copy_multi_header_lines,
                 
# 260 "src/http/ngx_http_upstream.c" 3 4
                0
# 260 "src/http/ngx_http_upstream.c"
                                                               , 1 },

    { { sizeof("Expires") - 1, (u_char *) "Expires" },
                 ngx_http_upstream_process_expires, 0,
                 ngx_http_upstream_copy_header_line,
                 
# 265 "src/http/ngx_http_upstream.c" 3 4
                0
# 265 "src/http/ngx_http_upstream.c"
                                                         , 1 },

    { { sizeof("Accept-Ranges") - 1, (u_char *) "Accept-Ranges" },
                 ngx_http_upstream_process_header_line,
                 
# 269 "src/http/ngx_http_upstream.c" 3 4
                0
# 269 "src/http/ngx_http_upstream.c"
                                                                       ,
                 ngx_http_upstream_copy_allow_ranges,
                 
# 271 "src/http/ngx_http_upstream.c" 3 4
                0
# 271 "src/http/ngx_http_upstream.c"
                                                               , 1 },

    { { sizeof("Content-Range") - 1, (u_char *) "Content-Range" },
                 ngx_http_upstream_ignore_header_line, 0,
                 ngx_http_upstream_copy_header_line,
                 
# 276 "src/http/ngx_http_upstream.c" 3 4
                0
# 276 "src/http/ngx_http_upstream.c"
                                                               , 0 },

    { { sizeof("Connection") - 1, (u_char *) "Connection" },
                 ngx_http_upstream_process_connection, 0,
                 ngx_http_upstream_ignore_header_line, 0, 0 },

    { { sizeof("Keep-Alive") - 1, (u_char *) "Keep-Alive" },
                 ngx_http_upstream_ignore_header_line, 0,
                 ngx_http_upstream_ignore_header_line, 0, 0 },

    { { sizeof("Vary") - 1, (u_char *) "Vary" },
                 ngx_http_upstream_process_vary, 0,
                 ngx_http_upstream_copy_header_line, 0, 0 },

    { { sizeof("Link") - 1, (u_char *) "Link" },
                 ngx_http_upstream_ignore_header_line, 0,
                 ngx_http_upstream_copy_multi_header_lines,
                 
# 293 "src/http/ngx_http_upstream.c" 3 4
                0
# 293 "src/http/ngx_http_upstream.c"
                                                      , 0 },

    { { sizeof("X-Accel-Expires") - 1, (u_char *) "X-Accel-Expires" },
                 ngx_http_upstream_process_accel_expires, 0,
                 ngx_http_upstream_copy_header_line, 0, 0 },

    { { sizeof("X-Accel-Redirect") - 1, (u_char *) "X-Accel-Redirect" },
                 ngx_http_upstream_process_header_line,
                 
# 301 "src/http/ngx_http_upstream.c" 3 4
                0
# 301 "src/http/ngx_http_upstream.c"
                                                                          ,
                 ngx_http_upstream_copy_header_line, 0, 0 },

    { { sizeof("X-Accel-Limit-Rate") - 1, (u_char *) "X-Accel-Limit-Rate" },
                 ngx_http_upstream_process_limit_rate, 0,
                 ngx_http_upstream_copy_header_line, 0, 0 },

    { { sizeof("X-Accel-Buffering") - 1, (u_char *) "X-Accel-Buffering" },
                 ngx_http_upstream_process_buffering, 0,
                 ngx_http_upstream_copy_header_line, 0, 0 },

    { { sizeof("X-Accel-Charset") - 1, (u_char *) "X-Accel-Charset" },
                 ngx_http_upstream_process_charset, 0,
                 ngx_http_upstream_copy_header_line, 0, 0 },

    { { sizeof("Transfer-Encoding") - 1, (u_char *) "Transfer-Encoding" },
                 ngx_http_upstream_process_transfer_encoding, 0,
                 ngx_http_upstream_ignore_header_line, 0, 0 },
# 327 "src/http/ngx_http_upstream.c"
    { { 0, 0 }, 0, 0, 0, 0, 0 }
};


static ngx_command_t ngx_http_upstream_commands[] = {

    { { sizeof("upstream") - 1, (u_char *) "upstream" },
      0x02000000|0x00000100|0x00000002,
      ngx_http_upstream,
      0,
      0,
      0 },

    { { sizeof("server") - 1, (u_char *) "server" },
      0x10000000|0x00000800,
      ngx_http_upstream_server,
      
# 343 "src/http/ngx_http_upstream.c" 3 4
     0
# 343 "src/http/ngx_http_upstream.c"
                             ,
      0,
      0 },

      { { 0, 0 }, 0, 0, 0, 0, 0 }
};


static ngx_http_module_t ngx_http_upstream_module_ctx = {
    ngx_http_upstream_add_variables,
    0,

    ngx_http_upstream_create_main_conf,
    ngx_http_upstream_init_main_conf,

    0,
    0,

    0,
    0
};


ngx_module_t ngx_http_upstream_module = {
    (ngx_uint_t) -1, (ngx_uint_t) -1, 0, 0, 0, 1015005, "8" "," "4" "," "8" "," "0" "0" "0" "0" "1" "1" "1" "1" "1" "1" "0" "1" "0" "1" "1" "1" "0" "0" "1" "1" "1" "0" "0" "0" "1" "0" "1" "0" "0" "0" "0" "1" "1" "0",
    &ngx_http_upstream_module_ctx,
    ngx_http_upstream_commands,
    0x50545448,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, 0, 0, 0, 0, 0, 0, 0
};


static ngx_http_variable_t ngx_http_upstream_vars[] = {

    { { sizeof("upstream_addr") - 1, (u_char *) "upstream_addr" }, 0,
      ngx_http_upstream_addr_variable, 0,
      2, 0 },

    { { sizeof("upstream_status") - 1, (u_char *) "upstream_status" }, 0,
      ngx_http_upstream_status_variable, 0,
      2, 0 },

    { { sizeof("upstream_connect_time") - 1, (u_char *) "upstream_connect_time" }, 0,
      ngx_http_upstream_response_time_variable, 2,
      2, 0 },

    { { sizeof("upstream_header_time") - 1, (u_char *) "upstream_header_time" }, 0,
      ngx_http_upstream_response_time_variable, 1,
      2, 0 },

    { { sizeof("upstream_response_time") - 1, (u_char *) "upstream_response_time" }, 0,
      ngx_http_upstream_response_time_variable, 0,
      2, 0 },

    { { sizeof("upstream_response_length") - 1, (u_char *) "upstream_response_length" }, 0,
      ngx_http_upstream_response_length_variable, 0,
      2, 0 },

    { { sizeof("upstream_bytes_received") - 1, (u_char *) "upstream_bytes_received" }, 0,
      ngx_http_upstream_response_length_variable, 1,
      2, 0 },



    { { sizeof("upstream_cache_status") - 1, (u_char *) "upstream_cache_status" }, 0,
      ngx_http_upstream_cache_status, 0,
      2, 0 },

    { { sizeof("upstream_cache_last_modified") - 1, (u_char *) "upstream_cache_last_modified" }, 0,
      ngx_http_upstream_cache_last_modified, 0,
      2|8, 0 },

    { { sizeof("upstream_cache_etag") - 1, (u_char *) "upstream_cache_etag" }, 0,
      ngx_http_upstream_cache_etag, 0,
      2|8, 0 },



    { { sizeof("upstream_http_") - 1, (u_char *) "upstream_http_" }, 0, ngx_http_upstream_header_variable,
      0, 2|32, 0 },

    { { sizeof("upstream_trailer_") - 1, (u_char *) "upstream_trailer_" }, 0, ngx_http_upstream_trailer_variable,
      0, 2|32, 0 },

    { { sizeof("upstream_cookie_") - 1, (u_char *) "upstream_cookie_" }, 0, ngx_http_upstream_cookie_variable,
      0, 2|32, 0 },

      { { 0, 0 }, 0, 0, 0, 0, 0 }
};


static ngx_http_upstream_next_t ngx_http_upstream_next_errors[] = {
    { 500, 0x00000010 },
    { 502, 0x00000020 },
    { 503, 0x00000040 },
    { 504, 0x00000080 },
    { 403, 0x00000100 },
    { 404, 0x00000200 },
    { 429, 0x00000400 },
    { 0, 0 }
};


ngx_conf_bitmask_t ngx_http_upstream_cache_method_mask[] = {
    { { sizeof("GET") - 1, (u_char *) "GET" }, 0x0002 },
    { { sizeof("HEAD") - 1, (u_char *) "HEAD" }, 0x0004 },
    { { sizeof("POST") - 1, (u_char *) "POST" }, 0x0008 },
    { { 0, 0 }, 0 }
};


ngx_conf_bitmask_t ngx_http_upstream_ignore_headers_masks[] = {
    { { sizeof("X-Accel-Redirect") - 1, (u_char *) "X-Accel-Redirect" }, 0x00000002 },
    { { sizeof("X-Accel-Expires") - 1, (u_char *) "X-Accel-Expires" }, 0x00000004 },
    { { sizeof("X-Accel-Limit-Rate") - 1, (u_char *) "X-Accel-Limit-Rate" }, 0x00000040 },
    { { sizeof("X-Accel-Buffering") - 1, (u_char *) "X-Accel-Buffering" }, 0x00000080 },
    { { sizeof("X-Accel-Charset") - 1, (u_char *) "X-Accel-Charset" }, 0x00000100 },
    { { sizeof("Expires") - 1, (u_char *) "Expires" }, 0x00000008 },
    { { sizeof("Cache-Control") - 1, (u_char *) "Cache-Control" }, 0x00000010 },
    { { sizeof("Set-Cookie") - 1, (u_char *) "Set-Cookie" }, 0x00000020 },
    { { sizeof("Vary") - 1, (u_char *) "Vary" }, 0x00000200 },
    { { 0, 0 }, 0 }
};


ngx_int_t
ngx_http_upstream_create(ngx_http_request_t *r)
{
    ngx_http_upstream_t *u;

    u = r->upstream;

    if (u && u->cleanup) {
        r->main->count++;
        ngx_http_upstream_cleanup(r);
    }

    u = ngx_pcalloc(r->pool, sizeof(ngx_http_upstream_t));
    if (u == 0) {
        return -1;
    }

    r->upstream = u;

    u->peer.log = r->connection->log;
    u->peer.log_error = NGX_ERROR_ERR;


    r->cache = 0;


    u->headers_in.content_length_n = -1;
    u->headers_in.last_modified_time = -1;

    return 0;
}


void
ngx_http_upstream_init(ngx_http_request_t *r)
{
    ngx_connection_t *c;

    c = r->connection;

   
                                                                              ;
# 525 "src/http/ngx_http_upstream.c"
    if (c->read->timer_set) {
        ngx_event_del_timer(c->read);
    }

    if (ngx_event_flags & 0x00000004) {

        if (!c->write->active) {
            if (ngx_event_actions.add(c->write, EPOLLOUT, EPOLLET)
                == -1)
            {
                ngx_http_finalize_request(r, 500);
                return;
            }
        }
    }

    ngx_http_upstream_init_request(r);
}


static void
ngx_http_upstream_init_request(ngx_http_request_t *r)
{
    ngx_str_t *host;
    ngx_uint_t i;
    ngx_resolver_ctx_t *ctx, temp;
    ngx_http_cleanup_t *cln;
    ngx_http_upstream_t *u;
    ngx_http_core_loc_conf_t *clcf;
    ngx_http_upstream_srv_conf_t *uscf, **uscfp;
    ngx_http_upstream_main_conf_t *umcf;

    if (r->aio) {
        return;
    }

    u = r->upstream;



    if (u->conf->cache) {
        ngx_int_t rc;

        rc = ngx_http_upstream_cache(r, u);

        if (rc == -3) {
            r->write_event_handler = ngx_http_upstream_init_request;
            return;
        }

        r->write_event_handler = ngx_http_request_empty_handler;

        if (rc == -1) {
            ngx_http_finalize_request(r, 500);
            return;
        }

        if (rc == 0) {
            rc = ngx_http_upstream_cache_send(r, u);

            if (rc == -4) {
                return;
            }

            if (rc == 40) {
                rc = -5;
                r->cached = 0;
                u->buffer.start = 0;
                u->cache_status = 1;
                u->request_sent = 1;
            }

            if (ngx_http_upstream_cache_background_update(r, u) != 0) {
                rc = -1;
            }
        }

        if (rc != -5) {
            ngx_http_finalize_request(r, rc);
            return;
        }
    }



    u->store = u->conf->store;

    if (!u->store && !r->post_action && !u->conf->ignore_client_abort) {
        r->read_event_handler = ngx_http_upstream_rd_check_broken_connection;
        r->write_event_handler = ngx_http_upstream_wr_check_broken_connection;
    }

    if (r->request_body) {
        u->request_bufs = r->request_body->bufs;
    }

    if (u->create_request(r) != 0) {
        ngx_http_finalize_request(r, 500);
        return;
    }

    if (ngx_http_upstream_set_local(r, u, u->conf->local) != 0) {
        ngx_http_finalize_request(r, 500);
        return;
    }

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    u->output.alignment = clcf->directio_alignment;
    u->output.pool = r->pool;
    u->output.bufs.num = 1;
    u->output.bufs.size = clcf->client_body_buffer_size;

    if (u->output.output_filter == 0) {
        u->output.output_filter = ngx_chain_writer;
        u->output.filter_ctx = &u->writer;
    }

    u->writer.pool = r->pool;

    if (r->upstream_states == 0) {

        r->upstream_states = ngx_array_create(r->pool, 1,
                                            sizeof(ngx_http_upstream_state_t));
        if (r->upstream_states == 0) {
            ngx_http_finalize_request(r, 500);
            return;
        }

    } else {

        u->state = ngx_array_push(r->upstream_states);
        if (u->state == 0) {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }

        (void) memset(u->state, 0, sizeof(ngx_http_upstream_state_t));
    }

    cln = ngx_http_cleanup_add(r, 0);
    if (cln == 0) {
        ngx_http_finalize_request(r, 500);
        return;
    }

    cln->handler = ngx_http_upstream_cleanup;
    cln->data = r;
    u->cleanup = &cln->handler;

    if (u->resolved == 0) {

        uscf = u->conf->upstream;

    } else {





        host = &u->resolved->host;

        umcf = (r)->main_conf[ngx_http_upstream_module.ctx_index];

        uscfp = umcf->upstreams.elts;

        for (i = 0; i < umcf->upstreams.nelts; i++) {

            uscf = uscfp[i];

            if (uscf->host.len == host->len
                && ((uscf->port == 0 && u->resolved->no_port)
                     || uscf->port == u->resolved->port)
                && ngx_strncasecmp(uscf->host.data, host->data, host->len) == 0)
            {
                goto found;
            }
        }

        if (u->resolved->sockaddr) {

            if (u->resolved->port == 0
                && u->resolved->sockaddr->sa_family != AF_UNIX)
            {
                if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "no port in upstream \"%V\"", host)
                                                                 ;
                ngx_http_upstream_finalize_request(r, u,
                                               500);
                return;
            }

            if (ngx_http_upstream_create_round_robin_peer(r, u->resolved)
                != 0)
            {
                ngx_http_upstream_finalize_request(r, u,
                                               500);
                return;
            }

            ngx_http_upstream_connect(r, u);

            return;
        }

        if (u->resolved->port == 0) {
            if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "no port in upstream \"%V\"", host)
                                                             ;
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }

        temp.name = *host;

        ctx = ngx_resolve_start(clcf->resolver, &temp);
        if (ctx == 0) {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }

        if (ctx == (void *) -1) {
            if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "no resolver defined to resolve %V", host)
                                                                    ;

            ngx_http_upstream_finalize_request(r, u, 502);
            return;
        }

        ctx->name = *host;
        ctx->handler = ngx_http_upstream_resolve_handler;
        ctx->data = r;
        ctx->timeout = clcf->resolver_timeout;

        u->resolved->ctx = ctx;

        if (ngx_resolve_name(ctx) != 0) {
            u->resolved->ctx = 0;
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }

        return;
    }

found:

    if (uscf == 0) {
        if ((r->connection->log)->log_level >= 2) ngx_log_error_core(2, r->connection->log, 0, "no upstream configuration")
                                                  ;
        ngx_http_upstream_finalize_request(r, u,
                                           500);
        return;
    }

    u->upstream = uscf;





    if (uscf->peer.init(r, uscf) != 0) {
        ngx_http_upstream_finalize_request(r, u,
                                           500);
        return;
    }

    u->peer.start_time = ngx_current_msec;

    if (u->conf->next_upstream_tries
        && u->peer.tries > u->conf->next_upstream_tries)
    {
        u->peer.tries = u->conf->next_upstream_tries;
    }

    ngx_http_upstream_connect(r, u);
}




static ngx_int_t
ngx_http_upstream_cache(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
    ngx_int_t rc;
    ngx_http_cache_t *c;
    ngx_http_file_cache_t *cache;

    c = r->cache;

    if (c == 0) {

        if (!(r->method & u->conf->cache_methods)) {
            return -5;
        }

        rc = ngx_http_upstream_cache_get(r, u, &cache);

        if (rc != 0) {
            return rc;
        }

        if (r->method == 0x0004 && u->conf->cache_convert_head) {
            u->method = ngx_http_core_get_method;
        }

        if (ngx_http_file_cache_new(r) != 0) {
            return -1;
        }

        if (u->create_key(r) != 0) {
            return -1;
        }



        ngx_http_file_cache_create_key(r);

        if (r->cache->header_start + 256 >= u->conf->buffer_size) {
            if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "%V_buffer_size %uz is not enough for cache key, " "it should be increased to at least %uz", &u->conf->module, u->conf->buffer_size, (((r->cache->header_start + 256) + (1024 - 1)) & ~(1024 - 1)))



                                                                        ;

            r->cache = 0;
            return -5;
        }

        u->cacheable = 1;

        c = r->cache;

        c->body_start = u->conf->buffer_size;
        c->min_uses = u->conf->cache_min_uses;
        c->file_cache = cache;

        switch (ngx_http_test_predicates(r, u->conf->cache_bypass)) {

        case -1:
            return -1;

        case -5:
            u->cache_status = 2;
            return -5;

        default:
            break;
        }

        c->lock = u->conf->cache_lock;
        c->lock_timeout = u->conf->cache_lock_timeout;
        c->lock_age = u->conf->cache_lock_age;

        u->cache_status = 1;
    }

    rc = ngx_http_file_cache_open(r);

   
                                                 ;

    switch (rc) {

    case 4:

        if (((u->conf->cache_use_stale & 0x00000800)
             || c->stale_updating) && !r->background
            && u->conf->cache_background_update)
        {
            r->cache->background = 1;
            u->cache_status = rc;
            rc = 0;
        }

        break;

    case 5:

        if (((u->conf->cache_use_stale & 0x00000800)
             || c->stale_updating) && !r->background)
        {
            u->cache_status = rc;
            rc = 0;

        } else {
            rc = 4;
        }

        break;

    case 0:
        u->cache_status = 7;
    }

    switch (rc) {

    case 0:

        return 0;

    case 4:

        c->valid_sec = 0;
        c->updating_sec = 0;
        c->error_sec = 0;

        u->buffer.start = 0;
        u->cache_status = 3;

        break;

    case -5:

        if ((size_t) (u->buffer.end - u->buffer.start) < u->conf->buffer_size) {
            u->buffer.start = 0;

        } else {
            u->buffer.pos = u->buffer.start + c->header_start;
            u->buffer.last = u->buffer.pos;
        }

        break;

    case 8:

        u->cacheable = 0;

        break;

    case -2:

        return -3;

    case -1:

        return -1;

    default:



        u->cache_status = 7;

        return rc;
    }

    if (ngx_http_upstream_cache_check_range(r, u) == -5) {
        u->cacheable = 0;
    }

    r->cached = 0;

    return -5;
}


static ngx_int_t
ngx_http_upstream_cache_get(ngx_http_request_t *r, ngx_http_upstream_t *u,
    ngx_http_file_cache_t **cache)
{
    ngx_str_t *name, val;
    ngx_uint_t i;
    ngx_http_file_cache_t **caches;

    if (u->conf->cache_zone) {
        *cache = u->conf->cache_zone->data;
        return 0;
    }

    if (ngx_http_complex_value(r, u->conf->cache_value, &val) != 0) {
        return -1;
    }

    if (val.len == 0
        || (val.len == 3 && strncmp((const char *) val.data, (const char *) "off", 3) == 0))
    {
        return -5;
    }

    caches = u->caches->elts;

    for (i = 0; i < u->caches->nelts; i++) {
        name = &caches[i]->shm_zone->shm.name;

        if (name->len == val.len
            && strncmp((const char *) name->data, (const char *) val.data, val.len) == 0)
        {
            *cache = caches[i];
            return 0;
        }
    }

    if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "cache \"%V\" not found", &val)
                                                 ;

    return -1;
}


static ngx_int_t
ngx_http_upstream_cache_send(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
    ngx_int_t rc;
    ngx_http_cache_t *c;

    r->cached = 1;
    c = r->cache;

    if (c->header_start == c->body_start) {
        r->http_version = 9;
        return ngx_http_cache_send(r);
    }



    u->buffer = *c->buf;
    u->buffer.pos += c->header_start;

    (void) memset(&u->headers_in, 0, sizeof(ngx_http_upstream_headers_in_t));
    u->headers_in.content_length_n = -1;
    u->headers_in.last_modified_time = -1;

    if (ngx_list_init(&u->headers_in.headers, r->pool, 8,
                      sizeof(ngx_table_elt_t))
        != 0)
    {
        return -1;
    }

    if (ngx_list_init(&u->headers_in.trailers, r->pool, 2,
                      sizeof(ngx_table_elt_t))
        != 0)
    {
        return -1;
    }

    rc = u->process_header(r);

    if (rc == 0) {

        if (ngx_http_upstream_process_headers(r, u) != 0) {
            return -4;
        }

        return ngx_http_cache_send(r);
    }

    if (rc == -1) {
        return -1;
    }

    if (rc == -2) {
        rc = 40;
    }



    if ((r->connection->log)->log_level >= 3) ngx_log_error_core(3, r->connection->log, 0, "cache file \"%s\" contains invalid header", c->file.name.data)

                                    ;



    return rc;
}


static ngx_int_t
ngx_http_upstream_cache_background_update(ngx_http_request_t *r,
    ngx_http_upstream_t *u)
{
    ngx_http_request_t *sr;

    if (!r->cached || !r->cache->background) {
        return 0;
    }

    if (r == r->main) {
        r->preserve_body = 1;
    }

    if (ngx_http_subrequest(r, &r->uri, &r->args, &sr, 0,
                            8
                            |16)
        != 0)
    {
        return -1;
    }

    sr->header_only = 1;

    return 0;
}


static ngx_int_t
ngx_http_upstream_cache_check_range(ngx_http_request_t *r,
    ngx_http_upstream_t *u)
{
    off_t offset;
    u_char *p, *start;
    ngx_table_elt_t *h;

    h = r->headers_in.range;

    if (h == 0
        || !u->cacheable
        || u->conf->cache_max_range_offset == 9223372036854775807LL)
    {
        return 0;
    }

    if (u->conf->cache_max_range_offset == 0) {
        return -5;
    }

    if (h->value.len < 7
        || ngx_strncasecmp(h->value.data, (u_char *) "bytes=", 6) != 0)
    {
        return 0;
    }

    p = h->value.data + 6;

    while (*p == ' ') { p++; }

    if (*p == '-') {
        return -5;
    }

    start = p;

    while (*p >= '0' && *p <= '9') { p++; }

    offset = ngx_atoof(start, p - start);

    if (offset >= u->conf->cache_max_range_offset) {
        return -5;
    }

    return 0;
}




static void
ngx_http_upstream_resolve_handler(ngx_resolver_ctx_t *ctx)
{
    ngx_uint_t run_posted;
    ngx_connection_t *c;
    ngx_http_request_t *r;
    ngx_http_upstream_t *u;
    ngx_http_upstream_resolved_t *ur;

    run_posted = ctx->async;

    r = ctx->data;
    c = r->connection;

    u = r->upstream;
    ur = u->resolved;

    ((ngx_http_log_ctx_t *) c->log->data)->current_request = r;

   
                                                                         ;

    if (ctx->state) {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "%V could not be resolved (%i: %s)", &ctx->name, ctx->state, ngx_resolver_strerror(ctx->state))


                                                        ;

        ngx_http_upstream_finalize_request(r, u, 502);
        goto failed;
    }

    ur->naddrs = ctx->naddrs;
    ur->addrs = ctx->addrs;
# 1227 "src/http/ngx_http_upstream.c"
    if (ngx_http_upstream_create_round_robin_peer(r, ur) != 0) {
        ngx_http_upstream_finalize_request(r, u,
                                           500);
        goto failed;
    }

    ngx_resolve_name_done(ctx);
    ur->ctx = 0;

    u->peer.start_time = ngx_current_msec;

    if (u->conf->next_upstream_tries
        && u->peer.tries > u->conf->next_upstream_tries)
    {
        u->peer.tries = u->conf->next_upstream_tries;
    }

    ngx_http_upstream_connect(r, u);

failed:

    if (run_posted) {
        ngx_http_run_posted_requests(c);
    }
}


static void
ngx_http_upstream_handler(ngx_event_t *ev)
{
    ngx_connection_t *c;
    ngx_http_request_t *r;
    ngx_http_upstream_t *u;

    c = ev->data;
    r = c->data;

    u = r->upstream;
    c = r->connection;

    ((ngx_http_log_ctx_t *) c->log->data)->current_request = r;

   
                                                                         ;

    if (ev->delayed && ev->timedout) {
        ev->delayed = 0;
        ev->timedout = 0;
    }

    if (ev->write) {
        u->write_event_handler(r, u);

    } else {
        u->read_event_handler(r, u);
    }

    ngx_http_run_posted_requests(c);
}


static void
ngx_http_upstream_rd_check_broken_connection(ngx_http_request_t *r)
{
    ngx_http_upstream_check_broken_connection(r, r->connection->read);
}


static void
ngx_http_upstream_wr_check_broken_connection(ngx_http_request_t *r)
{
    ngx_http_upstream_check_broken_connection(r, r->connection->write);
}


static void
ngx_http_upstream_check_broken_connection(ngx_http_request_t *r,
    ngx_event_t *ev)
{
    int n;
    char buf[1];
    ngx_err_t err;
    ngx_int_t event;
    ngx_connection_t *c;
    ngx_http_upstream_t *u;

   

                                      ;

    c = r->connection;
    u = r->upstream;

    if (c->error) {
        if ((ngx_event_flags & 0x00000001) && ev->active) {

            event = ev->write ? EPOLLOUT : (EPOLLIN|EPOLLRDHUP);

            if (ngx_event_actions.del(ev, event, 0) != 0) {
                ngx_http_upstream_finalize_request(r, u,
                                               500);
                return;
            }
        }

        if (!u->cacheable) {
            ngx_http_upstream_finalize_request(r, u,
                                               499);
        }

        return;
    }
# 1386 "src/http/ngx_http_upstream.c"
    if ((ngx_event_flags & 0x00000040) && ngx_use_epoll_rdhup) {
        socklen_t len;

        if (!ev->pending_eof) {
            return;
        }

        ev->eof = 1;
        c->error = 1;

        err = 0;
        len = sizeof(ngx_err_t);






        if (getsockopt(c->fd, SOL_SOCKET, SO_ERROR, (void *) &err, &len)
            == -1)
        {
            err = errno;
        }

        if (err) {
            ev->error = 1;
        }

        if (!u->cacheable && u->peer.connection) {
            if ((ev->log)->log_level >= 7) ngx_log_error_core(7, ev->log, err, "epoll_wait() reported that client prematurely closed " "connection, so upstream connection is closed too")

                                                                           ;
            ngx_http_upstream_finalize_request(r, u,
                                               499);
            return;
        }

        if ((ev->log)->log_level >= 7) ngx_log_error_core(7, ev->log, err, "epoll_wait() reported that client prematurely closed " "connection")

                                   ;

        if (u->peer.connection == 0) {
            ngx_http_upstream_finalize_request(r, u,
                                               499);
        }

        return;
    }



    n = recv(c->fd, buf, 1, MSG_PEEK);

    err = errno;

   
                                                 ;

    if (ev->write && (n >= 0 || err == EAGAIN)) {
        return;
    }

    if ((ngx_event_flags & 0x00000001) && ev->active) {

        event = ev->write ? EPOLLOUT : (EPOLLIN|EPOLLRDHUP);

        if (ngx_event_actions.del(ev, event, 0) != 0) {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }
    }

    if (n > 0) {
        return;
    }

    if (n == -1) {
        if (err == EAGAIN) {
            return;
        }

        ev->error = 1;

    } else {
        err = 0;
    }

    ev->eof = 1;
    c->error = 1;

    if (!u->cacheable && u->peer.connection) {
        if ((ev->log)->log_level >= 7) ngx_log_error_core(7, ev->log, err, "client prematurely closed connection, " "so upstream connection is closed too")

                                                             ;
        ngx_http_upstream_finalize_request(r, u,
                                           499);
        return;
    }

    if ((ev->log)->log_level >= 7) ngx_log_error_core(7, ev->log, err, "client prematurely closed connection")
                                                         ;

    if (u->peer.connection == 0) {
        ngx_http_upstream_finalize_request(r, u,
                                           499);
    }
}


static void
ngx_http_upstream_connect(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
    ngx_int_t rc;
    ngx_connection_t *c;

    r->connection->log->action = "connecting to upstream";

    if (u->state && u->state->response_time) {
        u->state->response_time = ngx_current_msec - u->state->response_time;
    }

    u->state = ngx_array_push(r->upstream_states);
    if (u->state == 0) {
        ngx_http_upstream_finalize_request(r, u,
                                           500);
        return;
    }

    (void) memset(u->state, 0, sizeof(ngx_http_upstream_state_t));

    u->state->response_time = ngx_current_msec;
    u->state->connect_time = (ngx_msec_t) -1;
    u->state->header_time = (ngx_msec_t) -1;

    rc = ngx_event_connect_peer(&u->peer);

   
                                                   ;

    if (rc == -1) {
        ngx_http_upstream_finalize_request(r, u,
                                           500);
        return;
    }

    u->state->peer = u->peer.name;

    if (rc == -3) {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "no live upstreams");
        ngx_http_upstream_next(r, u, 0x40000000);
        return;
    }

    if (rc == -5) {
        ngx_http_upstream_next(r, u, 0x00000002);
        return;
    }



    c = u->peer.connection;

    c->requests++;

    c->data = r;

    c->write->handler = ngx_http_upstream_handler;
    c->read->handler = ngx_http_upstream_handler;

    u->write_event_handler = ngx_http_upstream_send_request_handler;
    u->read_event_handler = ngx_http_upstream_process_header;

    c->sendfile &= r->connection->sendfile;
    u->output.sendfile = c->sendfile;

    if (r->connection->tcp_nopush == NGX_TCP_NOPUSH_DISABLED) {
        c->tcp_nopush = NGX_TCP_NOPUSH_DISABLED;
    }

    if (c->pool == 0) {



        c->pool = ngx_create_pool(128, r->connection->log);
        if (c->pool == 0) {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }
    }

    c->log = r->connection->log;
    c->pool->log = c->log;
    c->read->log = c->log;
    c->write->log = c->log;



    u->writer.out = 0;
    u->writer.last = &u->writer.out;
    u->writer.connection = c;
    u->writer.limit = 0;

    if (u->request_sent) {
        if (ngx_http_upstream_reinit(r, u) != 0) {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }
    }

    if (r->request_body
        && r->request_body->buf
        && r->request_body->temp_file
        && r == r->main)
    {





        u->output.free = ngx_alloc_chain_link(r->pool);
        if (u->output.free == 0) {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }

        u->output.free->buf = r->request_body->buf;
        u->output.free->next = 0;
        u->output.allocated = 1;

        r->request_body->buf->pos = r->request_body->buf->start;
        r->request_body->buf->last = r->request_body->buf->start;
        r->request_body->buf->tag = u->output.tag;
    }

    u->request_sent = 0;
    u->request_body_sent = 0;
    u->request_body_blocked = 0;

    if (rc == -2) {
        ngx_event_add_timer(c->write, u->conf->connect_timeout);
        return;
    }
# 1642 "src/http/ngx_http_upstream.c"
    ngx_http_upstream_send_request(r, u, 1);
}
# 1903 "src/http/ngx_http_upstream.c"
static ngx_int_t
ngx_http_upstream_reinit(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
    off_t file_pos;
    ngx_chain_t *cl;

    if (u->reinit_request(r) != 0) {
        return -1;
    }

    u->keepalive = 0;
    u->upgrade = 0;

    (void) memset(&u->headers_in, 0, sizeof(ngx_http_upstream_headers_in_t));
    u->headers_in.content_length_n = -1;
    u->headers_in.last_modified_time = -1;

    if (ngx_list_init(&u->headers_in.headers, r->pool, 8,
                      sizeof(ngx_table_elt_t))
        != 0)
    {
        return -1;
    }

    if (ngx_list_init(&u->headers_in.trailers, r->pool, 2,
                      sizeof(ngx_table_elt_t))
        != 0)
    {
        return -1;
    }



    file_pos = 0;

    for (cl = u->request_bufs; cl; cl = cl->next) {
        cl->buf->pos = cl->buf->start;



        if (cl->buf->in_file) {
            cl->buf->file_pos = file_pos;
            file_pos = cl->buf->file_last;
        }
    }



    if (r->request_body && r->request_body->temp_file
        && r != r->main && u->output.buf)
    {
        u->output.free = ngx_alloc_chain_link(r->pool);
        if (u->output.free == 0) {
            return -1;
        }

        u->output.free->buf = u->output.buf;
        u->output.free->next = 0;

        u->output.buf->pos = u->output.buf->start;
        u->output.buf->last = u->output.buf->start;
    }

    u->output.buf = 0;
    u->output.in = 0;
    u->output.busy = 0;



    u->buffer.pos = u->buffer.start;



    if (r->cache) {
        u->buffer.pos += r->cache->header_start;
    }



    u->buffer.last = u->buffer.pos;

    return 0;
}


static void
ngx_http_upstream_send_request(ngx_http_request_t *r, ngx_http_upstream_t *u,
    ngx_uint_t do_write)
{
    ngx_int_t rc;
    ngx_connection_t *c;

    c = u->peer.connection;

   
                                                ;

    if (u->state->connect_time == (ngx_msec_t) -1) {
        u->state->connect_time = ngx_current_msec - u->state->response_time;
    }

    if (!u->request_sent && ngx_http_upstream_test_connect(c) != 0) {
        ngx_http_upstream_next(r, u, 0x00000002);
        return;
    }

    c->log->action = "sending request to upstream";

    rc = ngx_http_upstream_send_request_body(r, u, do_write);

    if (rc == -1) {
        ngx_http_upstream_next(r, u, 0x00000002);
        return;
    }

    if (rc >= 300) {
        ngx_http_upstream_finalize_request(r, u, rc);
        return;
    }

    if (rc == -2) {
        if (!c->write->ready || u->request_body_blocked) {
            ngx_event_add_timer(c->write, u->conf->send_timeout);

        } else if (c->write->timer_set) {
            ngx_event_del_timer(c->write);
        }

        if (ngx_handle_write_event(c->write, u->conf->send_lowat) != 0) {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }

        if (c->write->ready && c->tcp_nopush == NGX_TCP_NOPUSH_SET) {
            if (ngx_tcp_push(c->fd) == -1) {
                if ((c->log)->log_level >= 3) ngx_log_error_core(3, c->log, errno, "setsockopt(!TCP_CORK)" " failed")
                                                       ;
                ngx_http_upstream_finalize_request(r, u,
                                               500);
                return;
            }

            c->tcp_nopush = NGX_TCP_NOPUSH_UNSET;
        }

        return;
    }



    if (c->write->timer_set) {
        ngx_event_del_timer(c->write);
    }

    if (c->tcp_nopush == NGX_TCP_NOPUSH_SET) {
        if (ngx_tcp_push(c->fd) == -1) {
            if ((c->log)->log_level >= 3) ngx_log_error_core(3, c->log, errno, "setsockopt(!TCP_CORK)" " failed")
                                                   ;
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }

        c->tcp_nopush = NGX_TCP_NOPUSH_UNSET;
    }

    if (!u->conf->preserve_output) {
        u->write_event_handler = ngx_http_upstream_dummy_handler;
    }

    if (ngx_handle_write_event(c->write, 0) != 0) {
        ngx_http_upstream_finalize_request(r, u,
                                           500);
        return;
    }

    if (!u->request_body_sent) {
        u->request_body_sent = 1;

        if (u->header_sent) {
            return;
        }

        ngx_event_add_timer(c->read, u->conf->read_timeout);

        if (c->read->ready) {
            ngx_http_upstream_process_header(r, u);
            return;
        }
    }
}


static ngx_int_t
ngx_http_upstream_send_request_body(ngx_http_request_t *r,
    ngx_http_upstream_t *u, ngx_uint_t do_write)
{
    ngx_int_t rc;
    ngx_chain_t *out, *cl, *ln;
    ngx_connection_t *c;
    ngx_http_core_loc_conf_t *clcf;

   
                                                     ;

    if (!r->request_body_no_buffering) {



        if (!u->request_sent) {
            u->request_sent = 1;
            out = u->request_bufs;

        } else {
            out = 0;
        }

        rc = ngx_output_chain(&u->output, out);

        if (rc == -2) {
            u->request_body_blocked = 1;

        } else {
            u->request_body_blocked = 0;
        }

        return rc;
    }

    if (!u->request_sent) {
        u->request_sent = 1;
        out = u->request_bufs;

        if (r->request_body->bufs) {
            for (cl = out; cl->next; cl = cl->next) { }
            cl->next = r->request_body->bufs;
            r->request_body->bufs = 0;
        }

        c = u->peer.connection;
        clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

        if (clcf->tcp_nodelay && ngx_tcp_nodelay(c) != 0) {
            return -1;
        }

        r->read_event_handler = ngx_http_upstream_read_request_handler;

    } else {
        out = 0;
    }

    for ( ;; ) {

        if (do_write) {
            rc = ngx_output_chain(&u->output, out);

            if (rc == -1) {
                return -1;
            }

            while (out) {
                ln = out;
                out = out->next;
                ln->next = r->pool->chain; r->pool->chain = ln;
            }

            if (rc == -2) {
                u->request_body_blocked = 1;

            } else {
                u->request_body_blocked = 0;
            }

            if (rc == 0 && !r->reading_body) {
                break;
            }
        }

        if (r->reading_body) {


            rc = ngx_http_read_unbuffered_request_body(r);

            if (rc >= 300) {
                return rc;
            }

            out = r->request_body->bufs;
            r->request_body->bufs = 0;
        }



        if (out == 0) {
            rc = -2;
            break;
        }

        do_write = 1;
    }

    if (!r->reading_body) {
        if (!u->store && !r->post_action && !u->conf->ignore_client_abort) {
            r->read_event_handler =
                                  ngx_http_upstream_rd_check_broken_connection;
        }
    }

    return rc;
}


static void
ngx_http_upstream_send_request_handler(ngx_http_request_t *r,
    ngx_http_upstream_t *u)
{
    ngx_connection_t *c;

    c = u->peer.connection;

   
                                                        ;

    if (c->write->timedout) {
        ngx_http_upstream_next(r, u, 0x00000004);
        return;
    }
# 2242 "src/http/ngx_http_upstream.c"
    if (u->header_sent && !u->conf->preserve_output) {
        u->write_event_handler = ngx_http_upstream_dummy_handler;

        (void) ngx_handle_write_event(c->write, 0);

        return;
    }

    ngx_http_upstream_send_request(r, u, 1);
}


static void
ngx_http_upstream_read_request_handler(ngx_http_request_t *r)
{
    ngx_connection_t *c;
    ngx_http_upstream_t *u;

    c = r->connection;
    u = r->upstream;

   
                                                        ;

    if (c->read->timedout) {
        c->timedout = 1;
        ngx_http_upstream_finalize_request(r, u, 408);
        return;
    }

    ngx_http_upstream_send_request(r, u, 0);
}


static void
ngx_http_upstream_process_header(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
    ssize_t n;
    ngx_int_t rc;
    ngx_connection_t *c;

    c = u->peer.connection;

   
                                                  ;

    c->log->action = "reading response header from upstream";

    if (c->read->timedout) {
        ngx_http_upstream_next(r, u, 0x00000004);
        return;
    }

    if (!u->request_sent && ngx_http_upstream_test_connect(c) != 0) {
        ngx_http_upstream_next(r, u, 0x00000002);
        return;
    }

    if (u->buffer.start == 0) {
        u->buffer.start = ngx_palloc(r->pool, u->conf->buffer_size);
        if (u->buffer.start == 0) {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }

        u->buffer.pos = u->buffer.start;
        u->buffer.last = u->buffer.start;
        u->buffer.end = u->buffer.start + u->conf->buffer_size;
        u->buffer.temporary = 1;

        u->buffer.tag = u->output.tag;

        if (ngx_list_init(&u->headers_in.headers, r->pool, 8,
                          sizeof(ngx_table_elt_t))
            != 0)
        {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }

        if (ngx_list_init(&u->headers_in.trailers, r->pool, 2,
                          sizeof(ngx_table_elt_t))
            != 0)
        {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return;
        }



        if (r->cache) {
            u->buffer.pos += r->cache->header_start;
            u->buffer.last = u->buffer.pos;
        }

    }

    for ( ;; ) {

        n = c->recv(c, u->buffer.last, u->buffer.end - u->buffer.last);

        if (n == -2) {




            if (ngx_handle_read_event(c->read, 0) != 0) {
                ngx_http_upstream_finalize_request(r, u,
                                               500);
                return;
            }

            return;
        }

        if (n == 0) {
            if ((c->log)->log_level >= 4) ngx_log_error_core(4, c->log, 0, "upstream prematurely closed connection")
                                                                   ;
        }

        if (n == -1 || n == 0) {
            ngx_http_upstream_next(r, u, 0x00000002);
            return;
        }

        u->state->bytes_received += n;

        u->buffer.last += n;







        rc = u->process_header(r);

        if (rc == -2) {

            if (u->buffer.last == u->buffer.end) {
                if ((c->log)->log_level >= 4) ngx_log_error_core(4, c->log, 0, "upstream sent too big header")
                                                             ;

                ngx_http_upstream_next(r, u,
                                       0x00000008);
                return;
            }

            continue;
        }

        break;
    }

    if (rc == 40) {
        ngx_http_upstream_next(r, u, 0x00000008);
        return;
    }

    if (rc == -1) {
        ngx_http_upstream_finalize_request(r, u,
                                           500);
        return;
    }



    u->state->header_time = ngx_current_msec - u->state->response_time;

    if (u->headers_in.status_n >= 300) {

        if (ngx_http_upstream_test_next(r, u) == 0) {
            return;
        }

        if (ngx_http_upstream_intercept_errors(r, u) == 0) {
            return;
        }
    }

    if (ngx_http_upstream_process_headers(r, u) != 0) {
        return;
    }

    ngx_http_upstream_send_response(r, u);
}


static ngx_int_t
ngx_http_upstream_test_next(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
    ngx_msec_t timeout;
    ngx_uint_t status, mask;
    ngx_http_upstream_next_t *un;

    status = u->headers_in.status_n;

    for (un = ngx_http_upstream_next_errors; un->status; un++) {

        if (status != un->status) {
            continue;
        }

        timeout = u->conf->next_upstream_timeout;

        if (u->request_sent
            && (r->method & (0x0008|0x1000|0x4000)))
        {
            mask = un->mask | 0x00004000;

        } else {
            mask = un->mask;
        }

        if (u->peer.tries > 1
            && ((u->conf->next_upstream & mask) == mask)
            && !(u->request_sent && r->request_body_no_buffering)
            && !(timeout && ngx_current_msec - u->peer.start_time >= timeout))
        {
            ngx_http_upstream_next(r, u, un->mask);
            return 0;
        }



        if (u->cache_status == 3
            && ((u->conf->cache_use_stale & un->mask) || r->cache->stale_error))
        {
            ngx_int_t rc;

            rc = u->reinit_request(r);

            if (rc != 0) {
                ngx_http_upstream_finalize_request(r, u, rc);
                return 0;
            }

            u->cache_status = 4;
            rc = ngx_http_upstream_cache_send(r, u);

            if (rc == -4) {
                return 0;
            }

            if (rc == 40) {
                rc = 500;
            }

            ngx_http_upstream_finalize_request(r, u, rc);
            return 0;
        }


    }



    if (status == 304
        && u->cache_status == 3
        && u->conf->cache_revalidate)
    {
        time_t now, valid, updating, error;
        ngx_int_t rc;

       
                                                    ;

        now = ngx_cached_time->sec;

        valid = r->cache->valid_sec;
        updating = r->cache->updating_sec;
        error = r->cache->error_sec;

        rc = u->reinit_request(r);

        if (rc != 0) {
            ngx_http_upstream_finalize_request(r, u, rc);
            return 0;
        }

        u->cache_status = 6;
        rc = ngx_http_upstream_cache_send(r, u);

        if (rc == -4) {
            return 0;
        }

        if (rc == 40) {
            rc = 500;
        }

        if (valid == 0) {
            valid = r->cache->valid_sec;
            updating = r->cache->updating_sec;
            error = r->cache->error_sec;
        }

        if (valid == 0) {
            valid = ngx_http_file_cache_valid(u->conf->cache_valid,
                                              u->headers_in.status_n);
            if (valid) {
                valid = now + valid;
            }
        }

        if (valid) {
            r->cache->valid_sec = valid;
            r->cache->updating_sec = updating;
            r->cache->error_sec = error;

            r->cache->date = now;

            ngx_http_file_cache_update_header(r);
        }

        ngx_http_upstream_finalize_request(r, u, rc);
        return 0;
    }



    return -5;
}


static ngx_int_t
ngx_http_upstream_intercept_errors(ngx_http_request_t *r,
    ngx_http_upstream_t *u)
{
    ngx_int_t status;
    ngx_uint_t i;
    ngx_table_elt_t *h;
    ngx_http_err_page_t *err_page;
    ngx_http_core_loc_conf_t *clcf;

    status = u->headers_in.status_n;

    if (status == 404 && u->conf->intercept_404) {
        ngx_http_upstream_finalize_request(r, u, 404);
        return 0;
    }

    if (!u->conf->intercept_errors) {
        return -5;
    }

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    if (clcf->error_pages == 0) {
        return -5;
    }

    err_page = clcf->error_pages->elts;
    for (i = 0; i < clcf->error_pages->nelts; i++) {

        if (err_page[i].status == status) {

            if (status == 401
                && u->headers_in.www_authenticate)
            {
                h = ngx_list_push(&r->headers_out.headers);

                if (h == 0) {
                    ngx_http_upstream_finalize_request(r, u,
                                               500);
                    return 0;
                }

                *h = *u->headers_in.www_authenticate;

                r->headers_out.www_authenticate = h;
            }



            if (r->cache) {

                if (u->cacheable) {
                    time_t valid;

                    valid = r->cache->valid_sec;

                    if (valid == 0) {
                        valid = ngx_http_file_cache_valid(u->conf->cache_valid,
                                                          status);
                        if (valid) {
                            r->cache->valid_sec = ngx_cached_time->sec + valid;
                        }
                    }

                    if (valid) {
                        r->cache->error = status;
                    }
                }

                ngx_http_file_cache_free(r->cache, u->pipe->temp_file);
            }

            ngx_http_upstream_finalize_request(r, u, status);

            return 0;
        }
    }

    return -5;
}


static ngx_int_t
ngx_http_upstream_test_connect(ngx_connection_t *c)
{
    int err;
    socklen_t len;
# 2678 "src/http/ngx_http_upstream.c"
    {
        err = 0;
        len = sizeof(int);






        if (getsockopt(c->fd, SOL_SOCKET, SO_ERROR, (void *) &err, &len)
            == -1)
        {
            err = errno;
        }

        if (err) {
            c->log->action = "connecting to upstream";
            (void) ngx_connection_error(c, err, "connect() failed");
            return -1;
        }
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_process_headers(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
    ngx_str_t uri, args;
    ngx_uint_t i, flags;
    ngx_list_part_t *part;
    ngx_table_elt_t *h;
    ngx_http_upstream_header_t *hh;
    ngx_http_upstream_main_conf_t *umcf;

    umcf = (r)->main_conf[ngx_http_upstream_module.ctx_index];

    if (u->headers_in.x_accel_redirect
        && !(u->conf->ignore_headers & 0x00000002))
    {
        ngx_http_upstream_finalize_request(r, u, -5);

        part = &u->headers_in.headers.part;
        h = part->elts;

        for (i = 0; ; i++) {

            if (i >= part->nelts) {
                if (part->next == 0) {
                    break;
                }

                part = part->next;
                h = part->elts;
                i = 0;
            }

            hh = ngx_hash_find(&umcf->headers_in_hash, h[i].hash,
                               h[i].lowcase_key, h[i].key.len);

            if (hh && hh->redirect) {
                if (hh->copy_handler(r, &h[i], hh->conf) != 0) {
                    ngx_http_finalize_request(r,
                                              500);
                    return -4;
                }
            }
        }

        uri = u->headers_in.x_accel_redirect->value;

        if (uri.data[0] == '@') {
            ngx_http_named_location(r, &uri);

        } else {
            (&args)->len = 0; (&args)->data = 0;
            flags = 1;

            if (ngx_http_parse_unsafe_uri(r, &uri, &args, &flags) != 0) {
                ngx_http_finalize_request(r, 404);
                return -4;
            }

            if (r->method != 0x0004) {
                r->method = 0x0002;
                r->method_name = ngx_http_core_get_method;
            }

            ngx_http_internal_redirect(r, &uri, &args);
        }

        ngx_http_finalize_request(r, -4);
        return -4;
    }

    part = &u->headers_in.headers.part;
    h = part->elts;

    for (i = 0; ; i++) {

        if (i >= part->nelts) {
            if (part->next == 0) {
                break;
            }

            part = part->next;
            h = part->elts;
            i = 0;
        }

        if (ngx_hash_find(&u->conf->hide_headers_hash, h[i].hash,
                          h[i].lowcase_key, h[i].key.len))
        {
            continue;
        }

        hh = ngx_hash_find(&umcf->headers_in_hash, h[i].hash,
                           h[i].lowcase_key, h[i].key.len);

        if (hh) {
            if (hh->copy_handler(r, &h[i], hh->conf) != 0) {
                ngx_http_upstream_finalize_request(r, u,
                                               500);
                return -4;
            }

            continue;
        }

        if (ngx_http_upstream_copy_header_line(r, &h[i], 0) != 0) {
            ngx_http_upstream_finalize_request(r, u,
                                               500);
            return -4;
        }
    }

    if (r->headers_out.server && r->headers_out.server->value.data == 0) {
        r->headers_out.server->hash = 0;
    }

    if (r->headers_out.date && r->headers_out.date->value.data == 0) {
        r->headers_out.date->hash = 0;
    }

    r->headers_out.status = u->headers_in.status_n;
    r->headers_out.status_line = u->headers_in.status_line;

    r->headers_out.content_length_n = u->headers_in.content_length_n;

    r->disable_not_modified = !u->cacheable;

    if (u->conf->force_ranges) {
        r->allow_ranges = 1;
        r->single_range = 1;


        if (r->cached) {
            r->single_range = 0;
        }

    }

    u->length = -1;

    return 0;
}


static ngx_int_t
ngx_http_upstream_process_trailers(ngx_http_request_t *r,
    ngx_http_upstream_t *u)
{
    ngx_uint_t i;
    ngx_list_part_t *part;
    ngx_table_elt_t *h, *ho;

    if (!u->conf->pass_trailers) {
        return 0;
    }

    part = &u->headers_in.trailers.part;
    h = part->elts;

    for (i = 0; ; i++) {

        if (i >= part->nelts) {
            if (part->next == 0) {
                break;
            }

            part = part->next;
            h = part->elts;
            i = 0;
        }

        if (ngx_hash_find(&u->conf->hide_headers_hash, h[i].hash,
                          h[i].lowcase_key, h[i].key.len))
        {
            continue;
        }

        ho = ngx_list_push(&r->headers_out.trailers);
        if (ho == 0) {
            return -1;
        }

        *ho = h[i];
    }

    return 0;
}


static void
ngx_http_upstream_send_response(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
    ssize_t n;
    ngx_int_t rc;
    ngx_event_pipe_t *p;
    ngx_connection_t *c;
    ngx_http_core_loc_conf_t *clcf;

    rc = ngx_http_send_header(r);

    if (rc == -1 || rc > 0 || r->post_action) {
        ngx_http_upstream_finalize_request(r, u, rc);
        return;
    }

    u->header_sent = 1;

    if (u->upgrade) {



        if (r->cache) {
            ngx_http_file_cache_free(r->cache, u->pipe->temp_file);
        }



        ngx_http_upstream_upgrade(r, u);
        return;
    }

    c = r->connection;

    if (r->header_only) {

        if (!u->buffering) {
            ngx_http_upstream_finalize_request(r, u, rc);
            return;
        }

        if (!u->cacheable && !u->store) {
            ngx_http_upstream_finalize_request(r, u, rc);
            return;
        }

        u->pipe->downstream_error = 1;
    }

    if (r->request_body && r->request_body->temp_file
        && r == r->main && !r->preserve_body
        && !u->conf->preserve_output)
    {
        ngx_pool_run_cleanup_file(r->pool, r->request_body->temp_file->file.fd);
        r->request_body->temp_file->file.fd = -1;
    }

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    if (!u->buffering) {



        if (r->cache) {
            ngx_http_file_cache_free(r->cache, u->pipe->temp_file);
        }



        if (u->input_filter == 0) {
            u->input_filter_init = ngx_http_upstream_non_buffered_filter_init;
            u->input_filter = ngx_http_upstream_non_buffered_filter;
            u->input_filter_ctx = r;
        }

        u->read_event_handler = ngx_http_upstream_process_non_buffered_upstream;
        r->write_event_handler =
                             ngx_http_upstream_process_non_buffered_downstream;

        r->limit_rate = 0;

        if (u->input_filter_init(u->input_filter_ctx) == -1) {
            ngx_http_upstream_finalize_request(r, u, -1);
            return;
        }

        if (clcf->tcp_nodelay && ngx_tcp_nodelay(c) != 0) {
            ngx_http_upstream_finalize_request(r, u, -1);
            return;
        }

        n = u->buffer.last - u->buffer.pos;

        if (n) {
            u->buffer.last = u->buffer.pos;

            u->state->response_length += n;

            if (u->input_filter(u->input_filter_ctx, n) == -1) {
                ngx_http_upstream_finalize_request(r, u, -1);
                return;
            }

            ngx_http_upstream_process_non_buffered_downstream(r);

        } else {
            u->buffer.pos = u->buffer.start;
            u->buffer.last = u->buffer.start;

            if (ngx_http_send_special(r, 2) == -1) {
                ngx_http_upstream_finalize_request(r, u, -1);
                return;
            }

            if (u->peer.connection->read->ready || u->length == 0) {
                ngx_http_upstream_process_non_buffered_upstream(r, u);
            }
        }

        return;
    }





    if (r->cache && r->cache->file.fd != -1) {
        ngx_pool_run_cleanup_file(r->pool, r->cache->file.fd);
        r->cache->file.fd = -1;
    }

    switch (ngx_http_test_predicates(r, u->conf->no_cache)) {

    case -1:
        ngx_http_upstream_finalize_request(r, u, -1);
        return;

    case -5:
        u->cacheable = 0;
        break;

    default:

        if (u->cache_status == 2) {



            if (ngx_http_file_cache_create(r) != 0) {
                ngx_http_upstream_finalize_request(r, u, -1);
                return;
            }
        }

        break;
    }

    if (u->cacheable) {
        time_t now, valid;

        now = ngx_cached_time->sec;

        valid = r->cache->valid_sec;

        if (valid == 0) {
            valid = ngx_http_file_cache_valid(u->conf->cache_valid,
                                              u->headers_in.status_n);
            if (valid) {
                r->cache->valid_sec = now + valid;
            }
        }

        if (valid) {
            r->cache->date = now;
            r->cache->body_start = (u_short) (u->buffer.pos - u->buffer.start);

            if (u->headers_in.status_n == 200
                || u->headers_in.status_n == 206)
            {
                r->cache->last_modified = u->headers_in.last_modified_time;

                if (u->headers_in.etag) {
                    r->cache->etag = u->headers_in.etag->value;

                } else {
                    (&r->cache->etag)->len = 0; (&r->cache->etag)->data = 0;
                }

            } else {
                r->cache->last_modified = -1;
                (&r->cache->etag)->len = 0; (&r->cache->etag)->data = 0;
            }

            if (ngx_http_file_cache_set_header(r, u->buffer.start) != 0) {
                ngx_http_upstream_finalize_request(r, u, -1);
                return;
            }

        } else {
            u->cacheable = 0;
        }
    }

   
                                                      ;

    if (u->cacheable == 0 && r->cache) {
        ngx_http_file_cache_free(r->cache, u->pipe->temp_file);
    }

    if (r->header_only && !u->cacheable && !u->store) {
        ngx_http_upstream_finalize_request(r, u, 0);
        return;
    }



    p = u->pipe;

    p->output_filter = ngx_http_upstream_output_filter;
    p->output_ctx = r;
    p->tag = u->output.tag;
    p->bufs = u->conf->bufs;
    p->busy_size = u->conf->busy_buffers_size;
    p->upstream = u->peer.connection;
    p->downstream = c;
    p->pool = r->pool;
    p->log = c->log;
    p->limit_rate = u->conf->limit_rate;
    p->start_sec = ngx_cached_time->sec;

    p->cacheable = u->cacheable || u->store;

    p->temp_file = ngx_pcalloc(r->pool, sizeof(ngx_temp_file_t));
    if (p->temp_file == 0) {
        ngx_http_upstream_finalize_request(r, u, -1);
        return;
    }

    p->temp_file->file.fd = -1;
    p->temp_file->file.log = c->log;
    p->temp_file->path = u->conf->temp_path;
    p->temp_file->pool = r->pool;

    if (p->cacheable) {
        p->temp_file->persistent = 1;


        if (r->cache && !r->cache->file_cache->use_temp_path) {
            p->temp_file->path = r->cache->file_cache->path;
            p->temp_file->file.name = r->cache->file.name;
        }


    } else {
        p->temp_file->log_level = 5;
        p->temp_file->warn = "an upstream response is buffered "
                             "to a temporary file";
    }

    p->max_temp_file_size = u->conf->max_temp_file_size;
    p->temp_file_write_size = u->conf->temp_file_write_size;
# 3161 "src/http/ngx_http_upstream.c"
    p->preread_bufs = ngx_alloc_chain_link(r->pool);
    if (p->preread_bufs == 0) {
        ngx_http_upstream_finalize_request(r, u, -1);
        return;
    }

    p->preread_bufs->buf = &u->buffer;
    p->preread_bufs->next = 0;
    u->buffer.recycled = 1;

    p->preread_size = u->buffer.last - u->buffer.pos;

    if (u->cacheable) {

        p->buf_to_file = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
        if (p->buf_to_file == 0) {
            ngx_http_upstream_finalize_request(r, u, -1);
            return;
        }

        p->buf_to_file->start = u->buffer.start;
        p->buf_to_file->pos = u->buffer.start;
        p->buf_to_file->last = u->buffer.pos;
        p->buf_to_file->temporary = 1;
    }

    if (ngx_event_flags & 0x00000200) {

        p->single_buf = 1;
    }


    p->free_bufs = 1;





    u->buffer.last = u->buffer.pos;

    if (u->conf->cyclic_temp_file) {







        p->cyclic_temp_file = 1;
        c->sendfile = 0;

    } else {
        p->cyclic_temp_file = 0;
    }

    p->read_timeout = u->conf->read_timeout;
    p->send_timeout = clcf->send_timeout;
    p->send_lowat = clcf->send_lowat;

    p->length = -1;

    if (u->input_filter_init
        && u->input_filter_init(p->input_ctx) != 0)
    {
        ngx_http_upstream_finalize_request(r, u, -1);
        return;
    }

    u->read_event_handler = ngx_http_upstream_process_upstream;
    r->write_event_handler = ngx_http_upstream_process_downstream;

    ngx_http_upstream_process_upstream(r, u);
}


static void
ngx_http_upstream_upgrade(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
    ngx_connection_t *c;
    ngx_http_core_loc_conf_t *clcf;

    c = r->connection;
    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];



    if (r != r->main) {
        if ((c->log)->log_level >= 4) ngx_log_error_core(4, c->log, 0, "connection upgrade in subrequest")
                                                         ;
        ngx_http_upstream_finalize_request(r, u, -1);
        return;
    }

    r->keepalive = 0;
    c->log->action = "proxying upgraded connection";

    u->read_event_handler = ngx_http_upstream_upgraded_read_upstream;
    u->write_event_handler = ngx_http_upstream_upgraded_write_upstream;
    r->read_event_handler = ngx_http_upstream_upgraded_read_downstream;
    r->write_event_handler = ngx_http_upstream_upgraded_write_downstream;

    if (clcf->tcp_nodelay) {

        if (ngx_tcp_nodelay(c) != 0) {
            ngx_http_upstream_finalize_request(r, u, -1);
            return;
        }

        if (ngx_tcp_nodelay(u->peer.connection) != 0) {
            ngx_http_upstream_finalize_request(r, u, -1);
            return;
        }
    }

    if (ngx_http_send_special(r, 2) == -1) {
        ngx_http_upstream_finalize_request(r, u, -1);
        return;
    }

    if (u->peer.connection->read->ready
        || u->buffer.pos != u->buffer.last)
    {
        if (!(c->read)->posted) { (c->read)->posted = 1; (&(c->read)->queue)->prev = (&ngx_posted_events)->prev; (&(c->read)->queue)->prev->next = &(c->read)->queue; (&(c->read)->queue)->next = &ngx_posted_events; (&ngx_posted_events)->prev = &(c->read)->queue; ; } else { ; };
        ngx_http_upstream_process_upgraded(r, 1, 1);
        return;
    }

    ngx_http_upstream_process_upgraded(r, 0, 1);
}


static void
ngx_http_upstream_upgraded_read_downstream(ngx_http_request_t *r)
{
    ngx_http_upstream_process_upgraded(r, 0, 0);
}


static void
ngx_http_upstream_upgraded_write_downstream(ngx_http_request_t *r)
{
    ngx_http_upstream_process_upgraded(r, 1, 1);
}


static void
ngx_http_upstream_upgraded_read_upstream(ngx_http_request_t *r,
    ngx_http_upstream_t *u)
{
    ngx_http_upstream_process_upgraded(r, 1, 0);
}


static void
ngx_http_upstream_upgraded_write_upstream(ngx_http_request_t *r,
    ngx_http_upstream_t *u)
{
    ngx_http_upstream_process_upgraded(r, 0, 1);
}


static void
ngx_http_upstream_process_upgraded(ngx_http_request_t *r,
    ngx_uint_t from_upstream, ngx_uint_t do_write)
{
    size_t size;
    ssize_t n;
    ngx_buf_t *b;
    ngx_connection_t *c, *downstream, *upstream, *dst, *src;
    ngx_http_upstream_t *u;
    ngx_http_core_loc_conf_t *clcf;

    c = r->connection;
    u = r->upstream;

   
                                                                           ;

    downstream = c;
    upstream = u->peer.connection;

    if (downstream->write->timedout) {
        c->timedout = 1;
        ngx_connection_error(c, ETIMEDOUT, "client timed out");
        ngx_http_upstream_finalize_request(r, u, 408);
        return;
    }

    if (upstream->read->timedout || upstream->write->timedout) {
        ngx_connection_error(c, ETIMEDOUT, "upstream timed out");
        ngx_http_upstream_finalize_request(r, u, 504);
        return;
    }

    if (from_upstream) {
        src = upstream;
        dst = downstream;
        b = &u->buffer;

    } else {
        src = downstream;
        dst = upstream;
        b = &u->from_client;

        if (r->header_in->last > r->header_in->pos) {
            b = r->header_in;
            b->end = b->last;
            do_write = 1;
        }

        if (b->start == 0) {
            b->start = ngx_palloc(r->pool, u->conf->buffer_size);
            if (b->start == 0) {
                ngx_http_upstream_finalize_request(r, u, -1);
                return;
            }

            b->pos = b->start;
            b->last = b->start;
            b->end = b->start + u->conf->buffer_size;
            b->temporary = 1;
            b->tag = u->output.tag;
        }
    }

    for ( ;; ) {

        if (do_write) {

            size = b->last - b->pos;

            if (size && dst->write->ready) {

                n = dst->send(dst, b->pos, size);

                if (n == -1) {
                    ngx_http_upstream_finalize_request(r, u, -1);
                    return;
                }

                if (n > 0) {
                    b->pos += n;

                    if (b->pos == b->last) {
                        b->pos = b->start;
                        b->last = b->start;
                    }
                }
            }
        }

        size = b->end - b->last;

        if (size && src->read->ready) {

            n = src->recv(src, b->last, size);

            if (n == -2 || n == 0) {
                break;
            }

            if (n > 0) {
                do_write = 1;
                b->last += n;

                if (from_upstream) {
                    u->state->bytes_received += n;
                }

                continue;
            }

            if (n == -1) {
                src->read->eof = 1;
            }
        }

        break;
    }

    if ((upstream->read->eof && u->buffer.pos == u->buffer.last)
        || (downstream->read->eof && u->from_client.pos == u->from_client.last)
        || (downstream->read->eof && upstream->read->eof))
    {
       
                                                     ;
        ngx_http_upstream_finalize_request(r, u, 0);
        return;
    }

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    if (ngx_handle_write_event(upstream->write, u->conf->send_lowat)
        != 0)
    {
        ngx_http_upstream_finalize_request(r, u, -1);
        return;
    }

    if (upstream->write->active && !upstream->write->ready) {
        ngx_event_add_timer(upstream->write, u->conf->send_timeout);

    } else if (upstream->write->timer_set) {
        ngx_event_del_timer(upstream->write);
    }

    if (ngx_handle_read_event(upstream->read, 0) != 0) {
        ngx_http_upstream_finalize_request(r, u, -1);
        return;
    }

    if (upstream->read->active && !upstream->read->ready) {
        ngx_event_add_timer(upstream->read, u->conf->read_timeout);

    } else if (upstream->read->timer_set) {
        ngx_event_del_timer(upstream->read);
    }

    if (ngx_handle_write_event(downstream->write, clcf->send_lowat)
        != 0)
    {
        ngx_http_upstream_finalize_request(r, u, -1);
        return;
    }

    if (ngx_handle_read_event(downstream->read, 0) != 0) {
        ngx_http_upstream_finalize_request(r, u, -1);
        return;
    }

    if (downstream->write->active && !downstream->write->ready) {
        ngx_event_add_timer(downstream->write, clcf->send_timeout);

    } else if (downstream->write->timer_set) {
        ngx_event_del_timer(downstream->write);
    }
}


static void
ngx_http_upstream_process_non_buffered_downstream(ngx_http_request_t *r)
{
    ngx_event_t *wev;
    ngx_connection_t *c;
    ngx_http_upstream_t *u;

    c = r->connection;
    u = r->upstream;
    wev = c->write;

   
                                                                   ;

    c->log->action = "sending to client";

    if (wev->timedout) {
        c->timedout = 1;
        ngx_connection_error(c, ETIMEDOUT, "client timed out");
        ngx_http_upstream_finalize_request(r, u, 408);
        return;
    }

    ngx_http_upstream_process_non_buffered_request(r, 1);
}


static void
ngx_http_upstream_process_non_buffered_upstream(ngx_http_request_t *r,
    ngx_http_upstream_t *u)
{
    ngx_connection_t *c;

    c = u->peer.connection;

   
                                                                 ;

    c->log->action = "reading upstream";

    if (c->read->timedout) {
        ngx_connection_error(c, ETIMEDOUT, "upstream timed out");
        ngx_http_upstream_finalize_request(r, u, 504);
        return;
    }

    ngx_http_upstream_process_non_buffered_request(r, 0);
}


static void
ngx_http_upstream_process_non_buffered_request(ngx_http_request_t *r,
    ngx_uint_t do_write)
{
    size_t size;
    ssize_t n;
    ngx_buf_t *b;
    ngx_int_t rc;
    ngx_connection_t *downstream, *upstream;
    ngx_http_upstream_t *u;
    ngx_http_core_loc_conf_t *clcf;

    u = r->upstream;
    downstream = r->connection;
    upstream = u->peer.connection;

    b = &u->buffer;

    do_write = do_write || u->length == 0;

    for ( ;; ) {

        if (do_write) {

            if (u->out_bufs || u->busy_bufs || downstream->buffered) {
                rc = ngx_http_output_filter(r, u->out_bufs);

                if (rc == -1) {
                    ngx_http_upstream_finalize_request(r, u, -1);
                    return;
                }

                ngx_chain_update_chains(r->pool, &u->free_bufs, &u->busy_bufs,
                                        &u->out_bufs, u->output.tag);
            }

            if (u->busy_bufs == 0) {

                if (u->length == 0
                    || (upstream->read->eof && u->length == -1))
                {
                    ngx_http_upstream_finalize_request(r, u, 0);
                    return;
                }

                if (upstream->read->eof) {
                    if ((upstream->log)->log_level >= 4) ngx_log_error_core(4, upstream->log, 0, "upstream prematurely closed connection")
                                                                           ;

                    ngx_http_upstream_finalize_request(r, u,
                                                       502);
                    return;
                }

                if (upstream->read->error) {
                    ngx_http_upstream_finalize_request(r, u,
                                                       502);
                    return;
                }

                b->pos = b->start;
                b->last = b->start;
            }
        }

        size = b->end - b->last;

        if (size && upstream->read->ready) {

            n = upstream->recv(upstream, b->last, size);

            if (n == -2) {
                break;
            }

            if (n > 0) {
                u->state->bytes_received += n;
                u->state->response_length += n;

                if (u->input_filter(u->input_filter_ctx, n) == -1) {
                    ngx_http_upstream_finalize_request(r, u, -1);
                    return;
                }
            }

            do_write = 1;

            continue;
        }

        break;
    }

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    if (downstream->data == r) {
        if (ngx_handle_write_event(downstream->write, clcf->send_lowat)
            != 0)
        {
            ngx_http_upstream_finalize_request(r, u, -1);
            return;
        }
    }

    if (downstream->write->active && !downstream->write->ready) {
        ngx_event_add_timer(downstream->write, clcf->send_timeout);

    } else if (downstream->write->timer_set) {
        ngx_event_del_timer(downstream->write);
    }

    if (ngx_handle_read_event(upstream->read, 0) != 0) {
        ngx_http_upstream_finalize_request(r, u, -1);
        return;
    }

    if (upstream->read->active && !upstream->read->ready) {
        ngx_event_add_timer(upstream->read, u->conf->read_timeout);

    } else if (upstream->read->timer_set) {
        ngx_event_del_timer(upstream->read);
    }
}


static ngx_int_t
ngx_http_upstream_non_buffered_filter_init(void *data)
{
    return 0;
}


static ngx_int_t
ngx_http_upstream_non_buffered_filter(void *data, ssize_t bytes)
{
    ngx_http_request_t *r = data;

    ngx_buf_t *b;
    ngx_chain_t *cl, **ll;
    ngx_http_upstream_t *u;

    u = r->upstream;

    for (cl = u->out_bufs, ll = &u->out_bufs; cl; cl = cl->next) {
        ll = &cl->next;
    }

    cl = ngx_chain_get_free_buf(r->pool, &u->free_bufs);
    if (cl == 0) {
        return -1;
    }

    *ll = cl;

    cl->buf->flush = 1;
    cl->buf->memory = 1;

    b = &u->buffer;

    cl->buf->pos = b->last;
    b->last += bytes;
    cl->buf->last = b->last;
    cl->buf->tag = u->output.tag;

    if (u->length == -1) {
        return 0;
    }

    u->length -= bytes;

    return 0;
}
# 3807 "src/http/ngx_http_upstream.c"
static ngx_int_t
ngx_http_upstream_output_filter(void *data, ngx_chain_t *chain)
{
    ngx_int_t rc;
    ngx_event_pipe_t *p;
    ngx_http_request_t *r;

    r = data;
    p = r->upstream->pipe;

    rc = ngx_http_output_filter(r, chain);

    p->aio = r->aio;

    return rc;
}


static void
ngx_http_upstream_process_downstream(ngx_http_request_t *r)
{
    ngx_event_t *wev;
    ngx_connection_t *c;
    ngx_event_pipe_t *p;
    ngx_http_upstream_t *u;

    c = r->connection;
    u = r->upstream;
    p = u->pipe;
    wev = c->write;

   
                                                      ;

    c->log->action = "sending to client";





    if (wev->timedout) {

        p->downstream_error = 1;
        c->timedout = 1;
        ngx_connection_error(c, ETIMEDOUT, "client timed out");

    } else {

        if (wev->delayed) {

           
                                                     ;

            if (ngx_handle_write_event(wev, p->send_lowat) != 0) {
                ngx_http_upstream_finalize_request(r, u, -1);
            }

            return;
        }

        if (ngx_event_pipe(p, 1) == -6) {
            ngx_http_upstream_finalize_request(r, u, -1);
            return;
        }
    }

    ngx_http_upstream_process_request(r, u);
}


static void
ngx_http_upstream_process_upstream(ngx_http_request_t *r,
    ngx_http_upstream_t *u)
{
    ngx_event_t *rev;
    ngx_event_pipe_t *p;
    ngx_connection_t *c;

    c = u->peer.connection;
    p = u->pipe;
    rev = c->read;

   
                                                    ;

    c->log->action = "reading upstream";

    if (rev->timedout) {

        p->upstream_error = 1;
        ngx_connection_error(c, ETIMEDOUT, "upstream timed out");

    } else {

        if (rev->delayed) {

           
                                                   ;

            if (ngx_handle_read_event(rev, 0) != 0) {
                ngx_http_upstream_finalize_request(r, u, -1);
            }

            return;
        }

        if (ngx_event_pipe(p, 0) == -6) {
            ngx_http_upstream_finalize_request(r, u, -1);
            return;
        }
    }

    ngx_http_upstream_process_request(r, u);
}


static void
ngx_http_upstream_process_request(ngx_http_request_t *r,
    ngx_http_upstream_t *u)
{
    ngx_temp_file_t *tf;
    ngx_event_pipe_t *p;

    p = u->pipe;
# 3953 "src/http/ngx_http_upstream.c"
    if (u->peer.connection) {

        if (u->store) {

            if (p->upstream_eof || p->upstream_done) {

                tf = p->temp_file;

                if (u->headers_in.status_n == 200
                    && (p->upstream_done || p->length == -1)
                    && (u->headers_in.content_length_n == -1
                        || u->headers_in.content_length_n == tf->offset))
                {
                    ngx_http_upstream_store(r, u);
                }
            }
        }



        if (u->cacheable) {

            if (p->upstream_done) {
                ngx_http_file_cache_update(r, p->temp_file);

            } else if (p->upstream_eof) {

                tf = p->temp_file;

                if (p->length == -1
                    && (u->headers_in.content_length_n == -1
                        || u->headers_in.content_length_n
                           == tf->offset - (off_t) r->cache->body_start))
                {
                    ngx_http_file_cache_update(r, tf);

                } else {
                    ngx_http_file_cache_free(r->cache, tf);
                }

            } else if (p->upstream_error) {
                ngx_http_file_cache_free(r->cache, p->temp_file);
            }
        }



        if (p->upstream_done || p->upstream_eof || p->upstream_error) {
           
                                                            ;

            if (p->upstream_done
                || (p->upstream_eof && p->length == -1))
            {
                ngx_http_upstream_finalize_request(r, u, 0);
                return;
            }

            if (p->upstream_eof) {
                if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "upstream prematurely closed connection")
                                                                       ;
            }

            ngx_http_upstream_finalize_request(r, u, 502);
            return;
        }
    }

    if (p->downstream_error) {
       
                                                        ;

        if (!u->cacheable && !u->store && u->peer.connection) {
            ngx_http_upstream_finalize_request(r, u, -1);
        }
    }
}


static void
ngx_http_upstream_store(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
    size_t root;
    time_t lm;
    ngx_str_t path;
    ngx_temp_file_t *tf;
    ngx_ext_rename_file_t ext;

    tf = u->pipe->temp_file;

    if (tf->file.fd == -1) {



        tf = ngx_pcalloc(r->pool, sizeof(ngx_temp_file_t));
        if (tf == 0) {
            return;
        }

        tf->file.fd = -1;
        tf->file.log = r->connection->log;
        tf->path = u->conf->temp_path;
        tf->pool = r->pool;
        tf->persistent = 1;

        if (ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                 tf->persistent, tf->clean, tf->access)
            != 0)
        {
            return;
        }

        u->pipe->temp_file = tf;
    }

    ext.access = u->conf->store_access;
    ext.path_access = u->conf->store_access;
    ext.time = -1;
    ext.create_path = 1;
    ext.delete_file = 1;
    ext.log = r->connection->log;

    if (u->headers_in.last_modified) {

        lm = ngx_parse_http_time(u->headers_in.last_modified->value.data,
                                 u->headers_in.last_modified->value.len);

        if (lm != -1) {
            ext.time = lm;
            ext.fd = tf->file.fd;
        }
    }

    if (u->conf->store_lengths == 0) {

        if (ngx_http_map_uri_to_path(r, &path, &root, 0) == 0) {
            return;
        }

    } else {
        if (ngx_http_script_run(r, &path, u->conf->store_lengths->elts, 0,
                                u->conf->store_values->elts)
            == 0)
        {
            return;
        }
    }

    path.len--;

   

                                                 ;

    (void) ngx_ext_rename_file(&tf->file.name, &path, &ext);

    u->store = 0;
}


static void
ngx_http_upstream_dummy_handler(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
   
                                                 ;
}


static void
ngx_http_upstream_next(ngx_http_request_t *r, ngx_http_upstream_t *u,
    ngx_uint_t ft_type)
{
    ngx_msec_t timeout;
    ngx_uint_t status, state;

   
                                                      ;

    if (u->peer.sockaddr) {

        if (ft_type == 0x00000100
            || ft_type == 0x00000200)
        {
            state = 2;

        } else {
            state = 4;
        }

        u->peer.free(&u->peer, u->peer.data, state);
        u->peer.sockaddr = 0;
    }

    if (ft_type == 0x00000004) {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, ETIMEDOUT, "upstream timed out")
                                           ;
    }

    if (u->peer.cached && ft_type == 0x00000002) {

        u->peer.tries++;
    }

    switch (ft_type) {

    case 0x00000004:
    case 0x00000080:
        status = 504;
        break;

    case 0x00000010:
        status = 500;
        break;

    case 0x00000040:
        status = 503;
        break;

    case 0x00000100:
        status = 403;
        break;

    case 0x00000200:
        status = 404;
        break;

    case 0x00000400:
        status = 429;
        break;






    default:
        status = 502;
    }

    if (r->connection->error) {
        ngx_http_upstream_finalize_request(r, u,
                                           499);
        return;
    }

    u->state->status = status;

    timeout = u->conf->next_upstream_timeout;

    if (u->request_sent
        && (r->method & (0x0008|0x1000|0x4000)))
    {
        ft_type |= 0x00004000;
    }

    if (u->peer.tries == 0
        || ((u->conf->next_upstream & ft_type) != ft_type)
        || (u->request_sent && r->request_body_no_buffering)
        || (timeout && ngx_current_msec - u->peer.start_time >= timeout))
    {


        if (u->cache_status == 3
            && ((u->conf->cache_use_stale & ft_type) || r->cache->stale_error))
        {
            ngx_int_t rc;

            rc = u->reinit_request(r);

            if (rc != 0) {
                ngx_http_upstream_finalize_request(r, u, rc);
                return;
            }

            u->cache_status = 4;
            rc = ngx_http_upstream_cache_send(r, u);

            if (rc == -4) {
                return;
            }

            if (rc == 40) {
                rc = 500;
            }

            ngx_http_upstream_finalize_request(r, u, rc);
            return;
        }


        ngx_http_upstream_finalize_request(r, u, status);
        return;
    }

    if (u->peer.connection) {
       

                                              ;
# 4261 "src/http/ngx_http_upstream.c"
        if (u->peer.connection->pool) {
            ngx_destroy_pool(u->peer.connection->pool);
        }

        ngx_close_connection(u->peer.connection);
        u->peer.connection = 0;
    }

    ngx_http_upstream_connect(r, u);
}


static void
ngx_http_upstream_cleanup(void *data)
{
    ngx_http_request_t *r = data;

   
                                                                    ;

    ngx_http_upstream_finalize_request(r, r->upstream, -4);
}


static void
ngx_http_upstream_finalize_request(ngx_http_request_t *r,
    ngx_http_upstream_t *u, ngx_int_t rc)
{
    ngx_uint_t flush;

   
                                                            ;

    if (u->cleanup == 0) {

        ngx_http_finalize_request(r, -4);
        return;
    }

    *u->cleanup = 0;
    u->cleanup = 0;

    if (u->resolved && u->resolved->ctx) {
        ngx_resolve_name_done(u->resolved->ctx);
        u->resolved->ctx = 0;
    }

    if (u->state && u->state->response_time) {
        u->state->response_time = ngx_current_msec - u->state->response_time;

        if (u->pipe && u->pipe->read_length) {
            u->state->bytes_received += u->pipe->read_length
                                        - u->pipe->preread_size;
            u->state->response_length = u->pipe->read_length;
        }
    }

    u->finalize_request(r, rc);

    if (u->peer.free && u->peer.sockaddr) {
        u->peer.free(&u->peer, u->peer.data, 0);
        u->peer.sockaddr = 0;
    }

    if (u->peer.connection) {
# 4345 "src/http/ngx_http_upstream.c"
       

                                              ;

        if (u->peer.connection->pool) {
            ngx_destroy_pool(u->peer.connection->pool);
        }

        ngx_close_connection(u->peer.connection);
    }

    u->peer.connection = 0;

    if (u->pipe && u->pipe->temp_file) {
       

                                                   ;
    }

    if (u->store && u->pipe && u->pipe->temp_file
        && u->pipe->temp_file->file.fd != -1)
    {
        if (unlink((const char *) u->pipe->temp_file->file.name.data)
            == -1)
        {
            if ((r->connection->log)->log_level >= 3) ngx_log_error_core(3, r->connection->log, errno, "unlink()" " \"%s\" failed", u->pipe->temp_file->file.name.data)

                                                             ;
        }
    }



    if (r->cache) {

        if (u->cacheable) {

            if (rc == 502 || rc == 504) {
                time_t valid;

                valid = ngx_http_file_cache_valid(u->conf->cache_valid, rc);

                if (valid) {
                    r->cache->valid_sec = ngx_cached_time->sec + valid;
                    r->cache->error = rc;
                }
            }
        }

        ngx_http_file_cache_free(r->cache, u->pipe->temp_file);
    }



    r->read_event_handler = ngx_http_block_reading;

    if (rc == -5) {
        return;
    }

    r->connection->log->action = "sending to client";

    if (!u->header_sent
        || rc == 408
        || rc == 499)
    {
        ngx_http_finalize_request(r, rc);
        return;
    }

    flush = 0;

    if (rc >= 300) {
        rc = -1;
        flush = 1;
    }

    if (r->header_only
        || (u->pipe && u->pipe->downstream_error))
    {
        ngx_http_finalize_request(r, rc);
        return;
    }

    if (rc == 0) {

        if (ngx_http_upstream_process_trailers(r, u) != 0) {
            ngx_http_finalize_request(r, -1);
            return;
        }

        rc = ngx_http_send_special(r, 1);

    } else if (flush) {
        r->keepalive = 0;
        rc = ngx_http_send_special(r, 2);
    }

    ngx_http_finalize_request(r, rc);
}


static ngx_int_t
ngx_http_upstream_process_header_line(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    ngx_table_elt_t **ph;

    ph = (ngx_table_elt_t **) ((char *) &r->upstream->headers_in + offset);

    if (*ph == 0) {
        *ph = h;
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_ignore_header_line(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    return 0;
}


static ngx_int_t
ngx_http_upstream_process_content_length(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset)
{
    ngx_http_upstream_t *u;

    u = r->upstream;

    u->headers_in.content_length = h;
    u->headers_in.content_length_n = ngx_atoof(h->value.data, h->value.len);

    return 0;
}


static ngx_int_t
ngx_http_upstream_process_last_modified(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset)
{
    ngx_http_upstream_t *u;

    u = r->upstream;

    u->headers_in.last_modified = h;
    u->headers_in.last_modified_time = ngx_parse_http_time(h->value.data,
                                                           h->value.len);

    return 0;
}


static ngx_int_t
ngx_http_upstream_process_set_cookie(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    ngx_array_t *pa;
    ngx_table_elt_t **ph;
    ngx_http_upstream_t *u;

    u = r->upstream;
    pa = &u->headers_in.cookies;

    if (pa->elts == 0) {
        if (ngx_array_init(pa, r->pool, 1, sizeof(ngx_table_elt_t *)) != 0)
        {
            return -1;
        }
    }

    ph = ngx_array_push(pa);
    if (ph == 0) {
        return -1;
    }

    *ph = h;


    if (!(u->conf->ignore_headers & 0x00000020)) {
        u->cacheable = 0;
    }


    return 0;
}


static ngx_int_t
ngx_http_upstream_process_cache_control(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset)
{
    ngx_array_t *pa;
    ngx_table_elt_t **ph;
    ngx_http_upstream_t *u;

    u = r->upstream;
    pa = &u->headers_in.cache_control;

    if (pa->elts == 0) {
        if (ngx_array_init(pa, r->pool, 2, sizeof(ngx_table_elt_t *)) != 0)
        {
            return -1;
        }
    }

    ph = ngx_array_push(pa);
    if (ph == 0) {
        return -1;
    }

    *ph = h;


    {
    u_char *p, *start, *last;
    ngx_int_t n;

    if (u->conf->ignore_headers & 0x00000010) {
        return 0;
    }

    if (r->cache == 0) {
        return 0;
    }

    if (r->cache->valid_sec != 0 && u->headers_in.x_accel_expires != 0) {
        return 0;
    }

    start = h->value.data;
    last = start + h->value.len;

    if (ngx_strlcasestrn(start, last, (u_char *) "no-cache", 8 - 1) != 0
        || ngx_strlcasestrn(start, last, (u_char *) "no-store", 8 - 1) != 0
        || ngx_strlcasestrn(start, last, (u_char *) "private", 7 - 1) != 0)
    {
        u->cacheable = 0;
        return 0;
    }

    p = ngx_strlcasestrn(start, last, (u_char *) "s-maxage=", 9 - 1);
    offset = 9;

    if (p == 0) {
        p = ngx_strlcasestrn(start, last, (u_char *) "max-age=", 8 - 1);
        offset = 8;
    }

    if (p) {
        n = 0;

        for (p += offset; p < last; p++) {
            if (*p == ',' || *p == ';' || *p == ' ') {
                break;
            }

            if (*p >= '0' && *p <= '9') {
                n = n * 10 + (*p - '0');
                continue;
            }

            u->cacheable = 0;
            return 0;
        }

        if (n == 0) {
            u->cacheable = 0;
            return 0;
        }

        r->cache->valid_sec = ngx_cached_time->sec + n;
    }

    p = ngx_strlcasestrn(start, last, (u_char *) "stale-while-revalidate=",
                         23 - 1);

    if (p) {
        n = 0;

        for (p += 23; p < last; p++) {
            if (*p == ',' || *p == ';' || *p == ' ') {
                break;
            }

            if (*p >= '0' && *p <= '9') {
                n = n * 10 + (*p - '0');
                continue;
            }

            u->cacheable = 0;
            return 0;
        }

        r->cache->updating_sec = n;
        r->cache->error_sec = n;
    }

    p = ngx_strlcasestrn(start, last, (u_char *) "stale-if-error=", 15 - 1);

    if (p) {
        n = 0;

        for (p += 15; p < last; p++) {
            if (*p == ',' || *p == ';' || *p == ' ') {
                break;
            }

            if (*p >= '0' && *p <= '9') {
                n = n * 10 + (*p - '0');
                continue;
            }

            u->cacheable = 0;
            return 0;
        }

        r->cache->error_sec = n;
    }
    }


    return 0;
}


static ngx_int_t
ngx_http_upstream_process_expires(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    ngx_http_upstream_t *u;

    u = r->upstream;
    u->headers_in.expires = h;


    {
    time_t expires;

    if (u->conf->ignore_headers & 0x00000008) {
        return 0;
    }

    if (r->cache == 0) {
        return 0;
    }

    if (r->cache->valid_sec != 0) {
        return 0;
    }

    expires = ngx_parse_http_time(h->value.data, h->value.len);

    if (expires == -1 || expires < ngx_cached_time->sec) {
        u->cacheable = 0;
        return 0;
    }

    r->cache->valid_sec = expires;
    }


    return 0;
}


static ngx_int_t
ngx_http_upstream_process_accel_expires(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset)
{
    ngx_http_upstream_t *u;

    u = r->upstream;
    u->headers_in.x_accel_expires = h;


    {
    u_char *p;
    size_t len;
    ngx_int_t n;

    if (u->conf->ignore_headers & 0x00000004) {
        return 0;
    }

    if (r->cache == 0) {
        return 0;
    }

    len = h->value.len;
    p = h->value.data;

    if (p[0] != '@') {
        n = ngx_atoi(p, len);

        switch (n) {
        case 0:
            u->cacheable = 0;


        case -1:
            return 0;

        default:
            r->cache->valid_sec = ngx_cached_time->sec + n;
            return 0;
        }
    }

    p++;
    len--;

    n = ngx_atoi(p, len);

    if (n != -1) {
        r->cache->valid_sec = n;
    }
    }


    return 0;
}


static ngx_int_t
ngx_http_upstream_process_limit_rate(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    ngx_int_t n;
    ngx_http_upstream_t *u;

    u = r->upstream;
    u->headers_in.x_accel_limit_rate = h;

    if (u->conf->ignore_headers & 0x00000040) {
        return 0;
    }

    n = ngx_atoi(h->value.data, h->value.len);

    if (n != -1) {
        r->limit_rate = (size_t) n;
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_process_buffering(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    u_char c0, c1, c2;
    ngx_http_upstream_t *u;

    u = r->upstream;

    if (u->conf->ignore_headers & 0x00000080) {
        return 0;
    }

    if (u->conf->change_buffering) {

        if (h->value.len == 2) {
            c0 = (u_char) ((h->value.data[0] >= 'A' && h->value.data[0] <= 'Z') ? (h->value.data[0] | 0x20) : h->value.data[0]);
            c1 = (u_char) ((h->value.data[1] >= 'A' && h->value.data[1] <= 'Z') ? (h->value.data[1] | 0x20) : h->value.data[1]);

            if (c0 == 'n' && c1 == 'o') {
                u->buffering = 0;
            }

        } else if (h->value.len == 3) {
            c0 = (u_char) ((h->value.data[0] >= 'A' && h->value.data[0] <= 'Z') ? (h->value.data[0] | 0x20) : h->value.data[0]);
            c1 = (u_char) ((h->value.data[1] >= 'A' && h->value.data[1] <= 'Z') ? (h->value.data[1] | 0x20) : h->value.data[1]);
            c2 = (u_char) ((h->value.data[2] >= 'A' && h->value.data[2] <= 'Z') ? (h->value.data[2] | 0x20) : h->value.data[2]);

            if (c0 == 'y' && c1 == 'e' && c2 == 's') {
                u->buffering = 1;
            }
        }
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_process_charset(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    if (r->upstream->conf->ignore_headers & 0x00000100) {
        return 0;
    }

    r->headers_out.override_charset = &h->value;

    return 0;
}


static ngx_int_t
ngx_http_upstream_process_connection(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    r->upstream->headers_in.connection = h;

    if (ngx_strlcasestrn(h->value.data, h->value.data + h->value.len,
                         (u_char *) "close", 5 - 1)
        != 0)
    {
        r->upstream->headers_in.connection_close = 1;
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_process_transfer_encoding(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset)
{
    r->upstream->headers_in.transfer_encoding = h;

    if (ngx_strlcasestrn(h->value.data, h->value.data + h->value.len,
                         (u_char *) "chunked", 7 - 1)
        != 0)
    {
        r->upstream->headers_in.chunked = 1;
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_process_vary(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset)
{
    ngx_http_upstream_t *u;

    u = r->upstream;
    u->headers_in.vary = h;



    if (u->conf->ignore_headers & 0x00000200) {
        return 0;
    }

    if (r->cache == 0) {
        return 0;
    }

    if (h->value.len > 128
        || (h->value.len == 1 && h->value.data[0] == '*'))
    {
        u->cacheable = 0;
    }

    r->cache->vary = h->value;



    return 0;
}


static ngx_int_t
ngx_http_upstream_copy_header_line(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    ngx_table_elt_t *ho, **ph;

    ho = ngx_list_push(&r->headers_out.headers);
    if (ho == 0) {
        return -1;
    }

    *ho = *h;

    if (offset) {
        ph = (ngx_table_elt_t **) ((char *) &r->headers_out + offset);
        *ph = ho;
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_copy_multi_header_lines(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset)
{
    ngx_array_t *pa;
    ngx_table_elt_t *ho, **ph;

    pa = (ngx_array_t *) ((char *) &r->headers_out + offset);

    if (pa->elts == 0) {
        if (ngx_array_init(pa, r->pool, 2, sizeof(ngx_table_elt_t *)) != 0)
        {
            return -1;
        }
    }

    ho = ngx_list_push(&r->headers_out.headers);
    if (ho == 0) {
        return -1;
    }

    *ho = *h;

    ph = ngx_array_push(pa);
    if (ph == 0) {
        return -1;
    }

    *ph = ho;

    return 0;
}


static ngx_int_t
ngx_http_upstream_copy_content_type(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    u_char *p, *last;

    r->headers_out.content_type_len = h->value.len;
    r->headers_out.content_type = h->value;
    r->headers_out.content_type_lowcase = 0;

    for (p = h->value.data; *p; p++) {

        if (*p != ';') {
            continue;
        }

        last = p;

        while (*++p == ' ') { }

        if (*p == '\0') {
            return 0;
        }

        if (ngx_strncasecmp(p, (u_char *) "charset=", 8) != 0) {
            continue;
        }

        p += 8;

        r->headers_out.content_type_len = last - h->value.data;

        if (*p == '"') {
            p++;
        }

        last = h->value.data + h->value.len;

        if (*(last - 1) == '"') {
            last--;
        }

        r->headers_out.charset.len = last - p;
        r->headers_out.charset.data = p;

        return 0;
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_copy_last_modified(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    ngx_table_elt_t *ho;

    ho = ngx_list_push(&r->headers_out.headers);
    if (ho == 0) {
        return -1;
    }

    *ho = *h;

    r->headers_out.last_modified = ho;
    r->headers_out.last_modified_time =
                                    r->upstream->headers_in.last_modified_time;

    return 0;
}


static ngx_int_t
ngx_http_upstream_rewrite_location(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    ngx_int_t rc;
    ngx_table_elt_t *ho;

    ho = ngx_list_push(&r->headers_out.headers);
    if (ho == 0) {
        return -1;
    }

    *ho = *h;

    if (r->upstream->rewrite_redirect) {
        rc = r->upstream->rewrite_redirect(r, ho, 0);

        if (rc == -5) {
            return 0;
        }

        if (rc == 0) {
            r->headers_out.location = ho;

           
                                                                    ;
        }

        return rc;
    }

    if (ho->value.data[0] != '/') {
        r->headers_out.location = ho;
    }






    return 0;
}


static ngx_int_t
ngx_http_upstream_rewrite_refresh(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    u_char *p;
    ngx_int_t rc;
    ngx_table_elt_t *ho;

    ho = ngx_list_push(&r->headers_out.headers);
    if (ho == 0) {
        return -1;
    }

    *ho = *h;

    if (r->upstream->rewrite_redirect) {

        p = ngx_strcasestrn(ho->value.data, "url=", 4 - 1);

        if (p) {
            rc = r->upstream->rewrite_redirect(r, ho, p + 4 - ho->value.data);

        } else {
            return 0;
        }

        if (rc == -5) {
            return 0;
        }

        if (rc == 0) {
            r->headers_out.refresh = ho;

           
                                                                   ;
        }

        return rc;
    }

    r->headers_out.refresh = ho;

    return 0;
}


static ngx_int_t
ngx_http_upstream_rewrite_set_cookie(ngx_http_request_t *r, ngx_table_elt_t *h,
    ngx_uint_t offset)
{
    ngx_int_t rc;
    ngx_table_elt_t *ho;

    ho = ngx_list_push(&r->headers_out.headers);
    if (ho == 0) {
        return -1;
    }

    *ho = *h;

    if (r->upstream->rewrite_cookie) {
        rc = r->upstream->rewrite_cookie(r, ho);

        if (rc == -5) {
            return 0;
        }
# 5163 "src/http/ngx_http_upstream.c"
        return rc;
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_copy_allow_ranges(ngx_http_request_t *r,
    ngx_table_elt_t *h, ngx_uint_t offset)
{
    ngx_table_elt_t *ho;

    if (r->upstream->conf->force_ranges) {
        return 0;
    }



    if (r->cached) {
        r->allow_ranges = 1;
        return 0;
    }

    if (r->upstream->cacheable) {
        r->allow_ranges = 1;
        r->single_range = 1;
        return 0;
    }



    ho = ngx_list_push(&r->headers_out.headers);
    if (ho == 0) {
        return -1;
    }

    *ho = *h;

    r->headers_out.accept_ranges = ho;

    return 0;
}
# 5231 "src/http/ngx_http_upstream.c"
static ngx_int_t
ngx_http_upstream_add_variables(ngx_conf_t *cf)
{
    ngx_http_variable_t *var, *v;

    for (v = ngx_http_upstream_vars; v->name.len; v++) {
        var = ngx_http_add_variable(cf, &v->name, v->flags);
        if (var == 0) {
            return -1;
        }

        var->get_handler = v->get_handler;
        var->data = v->data;
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_addr_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    u_char *p;
    size_t len;
    ngx_uint_t i;
    ngx_http_upstream_state_t *state;

    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;

    if (r->upstream_states == 0 || r->upstream_states->nelts == 0) {
        v->not_found = 1;
        return 0;
    }

    len = 0;
    state = r->upstream_states->elts;

    for (i = 0; i < r->upstream_states->nelts; i++) {
        if (state[i].peer) {
            len += state[i].peer->len + 2;

        } else {
            len += 3;
        }
    }

    p = ngx_pnalloc(r->pool, len);
    if (p == 0) {
        return -1;
    }

    v->data = p;

    i = 0;

    for ( ;; ) {
        if (state[i].peer) {
            p = (((u_char *) memcpy(p, state[i].peer->data, state[i].peer->len)) + (state[i].peer->len));
        }

        if (++i == r->upstream_states->nelts) {
            break;
        }

        if (state[i].peer) {
            *p++ = ',';
            *p++ = ' ';

        } else {
            *p++ = ' ';
            *p++ = ':';
            *p++ = ' ';

            if (++i == r->upstream_states->nelts) {
                break;
            }

            continue;
        }
    }

    v->len = p - v->data;

    return 0;
}


static ngx_int_t
ngx_http_upstream_status_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    u_char *p;
    size_t len;
    ngx_uint_t i;
    ngx_http_upstream_state_t *state;

    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;

    if (r->upstream_states == 0 || r->upstream_states->nelts == 0) {
        v->not_found = 1;
        return 0;
    }

    len = r->upstream_states->nelts * (3 + 2);

    p = ngx_pnalloc(r->pool, len);
    if (p == 0) {
        return -1;
    }

    v->data = p;

    i = 0;
    state = r->upstream_states->elts;

    for ( ;; ) {
        if (state[i].status) {
            p = ngx_sprintf(p, "%ui", state[i].status);

        } else {
            *p++ = '-';
        }

        if (++i == r->upstream_states->nelts) {
            break;
        }

        if (state[i].peer) {
            *p++ = ',';
            *p++ = ' ';

        } else {
            *p++ = ' ';
            *p++ = ':';
            *p++ = ' ';

            if (++i == r->upstream_states->nelts) {
                break;
            }

            continue;
        }
    }

    v->len = p - v->data;

    return 0;
}


static ngx_int_t
ngx_http_upstream_response_time_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    u_char *p;
    size_t len;
    ngx_uint_t i;
    ngx_msec_int_t ms;
    ngx_http_upstream_state_t *state;

    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;

    if (r->upstream_states == 0 || r->upstream_states->nelts == 0) {
        v->not_found = 1;
        return 0;
    }

    len = r->upstream_states->nelts * ((sizeof("-9223372036854775808") - 1) + 4 + 2);

    p = ngx_pnalloc(r->pool, len);
    if (p == 0) {
        return -1;
    }

    v->data = p;

    i = 0;
    state = r->upstream_states->elts;

    for ( ;; ) {
        if (state[i].status) {

            if (data == 1 && state[i].header_time != (ngx_msec_t) -1) {
                ms = state[i].header_time;

            } else if (data == 2 && state[i].connect_time != (ngx_msec_t) -1) {
                ms = state[i].connect_time;

            } else {
                ms = state[i].response_time;
            }

            ms = ((ms < 0) ? (0) : (ms));
            p = ngx_sprintf(p, "%T.%03M", (time_t) ms / 1000, ms % 1000);

        } else {
            *p++ = '-';
        }

        if (++i == r->upstream_states->nelts) {
            break;
        }

        if (state[i].peer) {
            *p++ = ',';
            *p++ = ' ';

        } else {
            *p++ = ' ';
            *p++ = ':';
            *p++ = ' ';

            if (++i == r->upstream_states->nelts) {
                break;
            }

            continue;
        }
    }

    v->len = p - v->data;

    return 0;
}


static ngx_int_t
ngx_http_upstream_response_length_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    u_char *p;
    size_t len;
    ngx_uint_t i;
    ngx_http_upstream_state_t *state;

    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;

    if (r->upstream_states == 0 || r->upstream_states->nelts == 0) {
        v->not_found = 1;
        return 0;
    }

    len = r->upstream_states->nelts * ((sizeof("-9223372036854775808") - 1) + 2);

    p = ngx_pnalloc(r->pool, len);
    if (p == 0) {
        return -1;
    }

    v->data = p;

    i = 0;
    state = r->upstream_states->elts;

    for ( ;; ) {

        if (data == 1) {
            p = ngx_sprintf(p, "%O", state[i].bytes_received);

        } else {
            p = ngx_sprintf(p, "%O", state[i].response_length);
        }

        if (++i == r->upstream_states->nelts) {
            break;
        }

        if (state[i].peer) {
            *p++ = ',';
            *p++ = ' ';

        } else {
            *p++ = ' ';
            *p++ = ':';
            *p++ = ' ';

            if (++i == r->upstream_states->nelts) {
                break;
            }

            continue;
        }
    }

    v->len = p - v->data;

    return 0;
}


static ngx_int_t
ngx_http_upstream_header_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    if (r->upstream == 0) {
        v->not_found = 1;
        return 0;
    }

    return ngx_http_variable_unknown_header(v, (ngx_str_t *) data,
                                         &r->upstream->headers_in.headers.part,
                                         sizeof("upstream_http_") - 1);
}


static ngx_int_t
ngx_http_upstream_trailer_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    if (r->upstream == 0) {
        v->not_found = 1;
        return 0;
    }

    return ngx_http_variable_unknown_header(v, (ngx_str_t *) data,
                                        &r->upstream->headers_in.trailers.part,
                                        sizeof("upstream_trailer_") - 1);
}


static ngx_int_t
ngx_http_upstream_cookie_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_str_t *name = (ngx_str_t *) data;

    ngx_str_t cookie, s;

    if (r->upstream == 0) {
        v->not_found = 1;
        return 0;
    }

    s.len = name->len - (sizeof("upstream_cookie_") - 1);
    s.data = name->data + sizeof("upstream_cookie_") - 1;

    if (ngx_http_parse_set_cookie_lines(&r->upstream->headers_in.cookies,
                                        &s, &cookie)
        == -5)
    {
        v->not_found = 1;
        return 0;
    }

    v->len = cookie.len;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = cookie.data;

    return 0;
}




static ngx_int_t
ngx_http_upstream_cache_status(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_uint_t n;

    if (r->upstream == 0 || r->upstream->cache_status == 0) {
        v->not_found = 1;
        return 0;
    }

    n = r->upstream->cache_status - 1;

    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->len = ngx_http_cache_status[n].len;
    v->data = ngx_http_cache_status[n].data;

    return 0;
}


static ngx_int_t
ngx_http_upstream_cache_last_modified(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    u_char *p;

    if (r->upstream == 0
        || !r->upstream->conf->cache_revalidate
        || r->upstream->cache_status != 3
        || r->cache->last_modified == -1)
    {
        v->not_found = 1;
        return 0;
    }

    p = ngx_pnalloc(r->pool, sizeof("Mon, 28 Sep 1970 06:00:00 GMT") - 1);
    if (p == 0) {
        return -1;
    }

    v->len = ngx_http_time(p, r->cache->last_modified) - p;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    return 0;
}


static ngx_int_t
ngx_http_upstream_cache_etag(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    if (r->upstream == 0
        || !r->upstream->conf->cache_revalidate
        || r->upstream->cache_status != 3
        || r->cache->etag.len == 0)
    {
        v->not_found = 1;
        return 0;
    }

    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->len = r->cache->etag.len;
    v->data = r->cache->etag.data;

    return 0;
}




static char *
ngx_http_upstream(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy)
{
    char *rv;
    void *mconf;
    ngx_str_t *value;
    ngx_url_t u;
    ngx_uint_t m;
    ngx_conf_t pcf;
    ngx_http_module_t *module;
    ngx_http_conf_ctx_t *ctx, *http_ctx;
    ngx_http_upstream_srv_conf_t *uscf;

    (void) memset(&u, 0, sizeof(ngx_url_t));

    value = cf->args->elts;
    u.host = value[1];
    u.no_resolve = 1;
    u.no_port = 1;

    uscf = ngx_http_upstream_add(cf, &u, 0x0001
                                         |0x0002
                                         |0x0100
                                         |0x0004
                                         |0x0008
                                         |0x0010
                                         |0x0020);
    if (uscf == 0) {
        return (void *) -1;
    }


    ctx = ngx_pcalloc(cf->pool, sizeof(ngx_http_conf_ctx_t));
    if (ctx == 0) {
        return (void *) -1;
    }

    http_ctx = cf->ctx;
    ctx->main_conf = http_ctx->main_conf;



    ctx->srv_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->srv_conf == 0) {
        return (void *) -1;
    }

    ctx->srv_conf[ngx_http_upstream_module.ctx_index] = uscf;

    uscf->srv_conf = ctx->srv_conf;




    ctx->loc_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->loc_conf == 0) {
        return (void *) -1;
    }

    for (m = 0; cf->cycle->modules[m]; m++) {
        if (cf->cycle->modules[m]->type != 0x50545448) {
            continue;
        }

        module = cf->cycle->modules[m]->ctx;

        if (module->create_srv_conf) {
            mconf = module->create_srv_conf(cf);
            if (mconf == 0) {
                return (void *) -1;
            }

            ctx->srv_conf[cf->cycle->modules[m]->ctx_index] = mconf;
        }

        if (module->create_loc_conf) {
            mconf = module->create_loc_conf(cf);
            if (mconf == 0) {
                return (void *) -1;
            }

            ctx->loc_conf[cf->cycle->modules[m]->ctx_index] = mconf;
        }
    }

    uscf->servers = ngx_array_create(cf->pool, 4,
                                     sizeof(ngx_http_upstream_server_t));
    if (uscf->servers == 0) {
        return (void *) -1;
    }




    pcf = *cf;
    cf->ctx = ctx;
    cf->cmd_type = 0x10000000;

    rv = ngx_conf_parse(cf, 0);

    *cf = pcf;

    if (rv != 0) {
        return rv;
    }

    if (uscf->servers->nelts == 0) {
        ngx_conf_log_error(1, cf, 0,
                           "no servers are inside upstream");
        return (void *) -1;
    }

    return rv;
}


static char *
ngx_http_upstream_server(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_upstream_srv_conf_t *uscf = conf;

    time_t fail_timeout;
    ngx_str_t *value, s;
    ngx_url_t u;
    ngx_int_t weight, max_conns, max_fails;
    ngx_uint_t i;
    ngx_http_upstream_server_t *us;

    us = ngx_array_push(uscf->servers);
    if (us == 0) {
        return (void *) -1;
    }

    (void) memset(us, 0, sizeof(ngx_http_upstream_server_t));

    value = cf->args->elts;

    weight = 1;
    max_conns = 0;
    max_fails = 1;
    fail_timeout = 10;

    for (i = 2; i < cf->args->nelts; i++) {

        if (strncmp((const char *) value[i].data, (const char *) "weight=", 7) == 0) {

            if (!(uscf->flags & 0x0002)) {
                goto not_supported;
            }

            weight = ngx_atoi(&value[i].data[7], value[i].len - 7);

            if (weight == -1 || weight == 0) {
                goto invalid;
            }

            continue;
        }

        if (strncmp((const char *) value[i].data, (const char *) "max_conns=", 10) == 0) {

            if (!(uscf->flags & 0x0100)) {
                goto not_supported;
            }

            max_conns = ngx_atoi(&value[i].data[10], value[i].len - 10);

            if (max_conns == -1) {
                goto invalid;
            }

            continue;
        }

        if (strncmp((const char *) value[i].data, (const char *) "max_fails=", 10) == 0) {

            if (!(uscf->flags & 0x0004)) {
                goto not_supported;
            }

            max_fails = ngx_atoi(&value[i].data[10], value[i].len - 10);

            if (max_fails == -1) {
                goto invalid;
            }

            continue;
        }

        if (strncmp((const char *) value[i].data, (const char *) "fail_timeout=", 13) == 0) {

            if (!(uscf->flags & 0x0008)) {
                goto not_supported;
            }

            s.len = value[i].len - 13;
            s.data = &value[i].data[13];

            fail_timeout = ngx_parse_time(&s, 1);

            if (fail_timeout == (time_t) -1) {
                goto invalid;
            }

            continue;
        }

        if (strcmp((const char *) value[i].data, (const char *) "backup") == 0) {

            if (!(uscf->flags & 0x0020)) {
                goto not_supported;
            }

            us->backup = 1;

            continue;
        }

        if (strcmp((const char *) value[i].data, (const char *) "down") == 0) {

            if (!(uscf->flags & 0x0010)) {
                goto not_supported;
            }

            us->down = 1;

            continue;
        }

        goto invalid;
    }

    (void) memset(&u, 0, sizeof(ngx_url_t));

    u.url = value[1];
    u.default_port = 80;

    if (ngx_parse_url(cf->pool, &u) != 0) {
        if (u.err) {
            ngx_conf_log_error(1, cf, 0,
                               "%s in upstream \"%V\"", u.err, &u.url);
        }

        return (void *) -1;
    }

    us->name = u.url;
    us->addrs = u.addrs;
    us->naddrs = u.naddrs;
    us->weight = weight;
    us->max_conns = max_conns;
    us->max_fails = max_fails;
    us->fail_timeout = fail_timeout;

    return 0;

invalid:

    ngx_conf_log_error(1, cf, 0,
                       "invalid parameter \"%V\"", &value[i]);

    return (void *) -1;

not_supported:

    ngx_conf_log_error(1, cf, 0,
                       "balancing method does not support parameter \"%V\"",
                       &value[i]);

    return (void *) -1;
}


ngx_http_upstream_srv_conf_t *
ngx_http_upstream_add(ngx_conf_t *cf, ngx_url_t *u, ngx_uint_t flags)
{
    ngx_uint_t i;
    ngx_http_upstream_server_t *us;
    ngx_http_upstream_srv_conf_t *uscf, **uscfp;
    ngx_http_upstream_main_conf_t *umcf;

    if (!(flags & 0x0001)) {

        if (ngx_parse_url(cf->pool, u) != 0) {
            if (u->err) {
                ngx_conf_log_error(1, cf, 0,
                                   "%s in upstream \"%V\"", u->err, &u->url);
            }

            return 0;
        }
    }

    umcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_upstream_module.ctx_index];

    uscfp = umcf->upstreams.elts;

    for (i = 0; i < umcf->upstreams.nelts; i++) {

        if (uscfp[i]->host.len != u->host.len
            || ngx_strncasecmp(uscfp[i]->host.data, u->host.data, u->host.len)
               != 0)
        {
            continue;
        }

        if ((flags & 0x0001)
             && (uscfp[i]->flags & 0x0001))
        {
            ngx_conf_log_error(1, cf, 0,
                               "duplicate upstream \"%V\"", &u->host);
            return 0;
        }

        if ((uscfp[i]->flags & 0x0001) && !u->no_port) {
            ngx_conf_log_error(1, cf, 0,
                               "upstream \"%V\" may not have port %d",
                               &u->host, u->port);
            return 0;
        }

        if ((flags & 0x0001) && !uscfp[i]->no_port) {
            if ((cf->log)->log_level >= 1) ngx_log_error_core(1, cf->log, 0, "upstream \"%V\" may not have port %d in %s:%ui", &u->host, uscfp[i]->port, uscfp[i]->file_name, uscfp[i]->line)


                                                              ;
            return 0;
        }

        if (uscfp[i]->port && u->port
            && uscfp[i]->port != u->port)
        {
            continue;
        }

        if (flags & 0x0001) {
            uscfp[i]->flags = flags;
            uscfp[i]->port = 0;
        }

        return uscfp[i];
    }

    uscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_upstream_srv_conf_t));
    if (uscf == 0) {
        return 0;
    }

    uscf->flags = flags;
    uscf->host = u->host;
    uscf->file_name = cf->conf_file->file.name.data;
    uscf->line = cf->conf_file->line;
    uscf->port = u->port;
    uscf->no_port = u->no_port;

    if (u->naddrs == 1 && (u->port || u->family == AF_UNIX)) {
        uscf->servers = ngx_array_create(cf->pool, 1,
                                         sizeof(ngx_http_upstream_server_t));
        if (uscf->servers == 0) {
            return 0;
        }

        us = ngx_array_push(uscf->servers);
        if (us == 0) {
            return 0;
        }

        (void) memset(us, 0, sizeof(ngx_http_upstream_server_t));

        us->addrs = u->addrs;
        us->naddrs = 1;
    }

    uscfp = ngx_array_push(&umcf->upstreams);
    if (uscfp == 0) {
        return 0;
    }

    *uscfp = uscf;

    return uscf;
}


char *
ngx_http_upstream_bind_set_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf)
{
    char *p = conf;

    ngx_int_t rc;
    ngx_str_t *value;
    ngx_http_complex_value_t cv;
    ngx_http_upstream_local_t **plocal, *local;
    ngx_http_compile_complex_value_t ccv;

    plocal = (ngx_http_upstream_local_t **) (p + cmd->offset);

    if (*plocal != (void *) -1) {
        return "is duplicate";
    }

    value = cf->args->elts;

    if (cf->args->nelts == 2 && strcmp((const char *) value[1].data, (const char *) "off") == 0) {
        *plocal = 0;
        return 0;
    }

    (void) memset(&ccv, 0, sizeof(ngx_http_compile_complex_value_t));

    ccv.cf = cf;
    ccv.value = &value[1];
    ccv.complex_value = &cv;

    if (ngx_http_compile_complex_value(&ccv) != 0) {
        return (void *) -1;
    }

    local = ngx_pcalloc(cf->pool, sizeof(ngx_http_upstream_local_t));
    if (local == 0) {
        return (void *) -1;
    }

    *plocal = local;

    if (cv.lengths) {
        local->value = ngx_palloc(cf->pool, sizeof(ngx_http_complex_value_t));
        if (local->value == 0) {
            return (void *) -1;
        }

        *local->value = cv;

    } else {
        local->addr = ngx_palloc(cf->pool, sizeof(ngx_addr_t));
        if (local->addr == 0) {
            return (void *) -1;
        }

        rc = ngx_parse_addr_port(cf->pool, local->addr, value[1].data,
                                 value[1].len);

        switch (rc) {
        case 0:
            local->addr->name = value[1];
            break;

        case -5:
            ngx_conf_log_error(1, cf, 0,
                               "invalid address \"%V\"", &value[1]);


        default:
            return (void *) -1;
        }
    }

    if (cf->args->nelts > 2) {
        if (strcmp((const char *) value[2].data, (const char *) "transparent") == 0) {

            ngx_core_conf_t *ccf;

            ccf = (ngx_core_conf_t *) cf->cycle->conf_ctx[ngx_core_module.index]
                                                                   ;

            ccf->transparent = 1;
            local->transparent = 1;





        } else {
            ngx_conf_log_error(1, cf, 0,
                               "invalid parameter \"%V\"", &value[2]);
            return (void *) -1;
        }
    }

    return 0;
}


static ngx_int_t
ngx_http_upstream_set_local(ngx_http_request_t *r, ngx_http_upstream_t *u,
    ngx_http_upstream_local_t *local)
{
    ngx_int_t rc;
    ngx_str_t val;
    ngx_addr_t *addr;

    if (local == 0) {
        u->peer.local = 0;
        return 0;
    }


    u->peer.transparent = local->transparent;


    if (local->value == 0) {
        u->peer.local = local->addr;
        return 0;
    }

    if (ngx_http_complex_value(r, local->value, &val) != 0) {
        return -1;
    }

    if (val.len == 0) {
        return 0;
    }

    addr = ngx_palloc(r->pool, sizeof(ngx_addr_t));
    if (addr == 0) {
        return -1;
    }

    rc = ngx_parse_addr_port(r->pool, addr, val.data, val.len);
    if (rc == -1) {
        return -1;
    }

    if (rc != 0) {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "invalid local address \"%V\"", &val)
                                                           ;
        return 0;
    }

    addr->name = val;
    u->peer.local = addr;

    return 0;
}


char *
ngx_http_upstream_param_set_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf)
{
    char *p = conf;

    ngx_str_t *value;
    ngx_array_t **a;
    ngx_http_upstream_param_t *param;

    a = (ngx_array_t **) (p + cmd->offset);

    if (*a == 0) {
        *a = ngx_array_create(cf->pool, 4, sizeof(ngx_http_upstream_param_t));
        if (*a == 0) {
            return (void *) -1;
        }
    }

    param = ngx_array_push(*a);
    if (param == 0) {
        return (void *) -1;
    }

    value = cf->args->elts;

    param->key = value[1];
    param->value = value[2];
    param->skip_empty = 0;

    if (cf->args->nelts == 4) {
        if (strcmp((const char *) value[3].data, (const char *) "if_not_empty") != 0) {
            ngx_conf_log_error(1, cf, 0,
                               "invalid parameter \"%V\"", &value[3]);
            return (void *) -1;
        }

        param->skip_empty = 1;
    }

    return 0;
}


ngx_int_t
ngx_http_upstream_hide_headers_hash(ngx_conf_t *cf,
    ngx_http_upstream_conf_t *conf, ngx_http_upstream_conf_t *prev,
    ngx_str_t *default_hide_headers, ngx_hash_init_t *hash)
{
    ngx_str_t *h;
    ngx_uint_t i, j;
    ngx_array_t hide_headers;
    ngx_hash_key_t *hk;

    if (conf->hide_headers == (void *) -1
        && conf->pass_headers == (void *) -1)
    {
        conf->hide_headers = prev->hide_headers;
        conf->pass_headers = prev->pass_headers;

        conf->hide_headers_hash = prev->hide_headers_hash;

        if (conf->hide_headers_hash.buckets) {
            return 0;
        }

    } else {
        if (conf->hide_headers == (void *) -1) {
            conf->hide_headers = prev->hide_headers;
        }

        if (conf->pass_headers == (void *) -1) {
            conf->pass_headers = prev->pass_headers;
        }
    }

    if (ngx_array_init(&hide_headers, cf->temp_pool, 4, sizeof(ngx_hash_key_t))
        != 0)
    {
        return -1;
    }

    for (h = default_hide_headers; h->len; h++) {
        hk = ngx_array_push(&hide_headers);
        if (hk == 0) {
            return -1;
        }

        hk->key = *h;
        hk->key_hash = ngx_hash_key_lc(h->data, h->len);
        hk->value = (void *) 1;
    }

    if (conf->hide_headers != (void *) -1) {

        h = conf->hide_headers->elts;

        for (i = 0; i < conf->hide_headers->nelts; i++) {

            hk = hide_headers.elts;

            for (j = 0; j < hide_headers.nelts; j++) {
                if (ngx_strcasecmp(h[i].data, hk[j].key.data) == 0) {
                    goto exist;
                }
            }

            hk = ngx_array_push(&hide_headers);
            if (hk == 0) {
                return -1;
            }

            hk->key = h[i];
            hk->key_hash = ngx_hash_key_lc(h[i].data, h[i].len);
            hk->value = (void *) 1;

        exist:

            continue;
        }
    }

    if (conf->pass_headers != (void *) -1) {

        h = conf->pass_headers->elts;
        hk = hide_headers.elts;

        for (i = 0; i < conf->pass_headers->nelts; i++) {
            for (j = 0; j < hide_headers.nelts; j++) {

                if (hk[j].key.data == 0) {
                    continue;
                }

                if (ngx_strcasecmp(h[i].data, hk[j].key.data) == 0) {
                    hk[j].key.data = 0;
                    break;
                }
            }
        }
    }

    hash->hash = &conf->hide_headers_hash;
    hash->key = ngx_hash_key_lc;
    hash->pool = cf->pool;
    hash->temp_pool = 0;

    if (ngx_hash_init(hash, hide_headers.elts, hide_headers.nelts) != 0) {
        return -1;
    }






    if (prev->hide_headers_hash.buckets == 0
        && conf->hide_headers == prev->hide_headers
        && conf->pass_headers == prev->pass_headers)
    {
        prev->hide_headers_hash = conf->hide_headers_hash;
    }

    return 0;
}


static void *
ngx_http_upstream_create_main_conf(ngx_conf_t *cf)
{
    ngx_http_upstream_main_conf_t *umcf;

    umcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_upstream_main_conf_t));
    if (umcf == 0) {
        return 0;
    }

    if (ngx_array_init(&umcf->upstreams, cf->pool, 4,
                       sizeof(ngx_http_upstream_srv_conf_t *))
        != 0)
    {
        return 0;
    }

    return umcf;
}


static char *
ngx_http_upstream_init_main_conf(ngx_conf_t *cf, void *conf)
{
    ngx_http_upstream_main_conf_t *umcf = conf;

    ngx_uint_t i;
    ngx_array_t headers_in;
    ngx_hash_key_t *hk;
    ngx_hash_init_t hash;
    ngx_http_upstream_init_pt init;
    ngx_http_upstream_header_t *header;
    ngx_http_upstream_srv_conf_t **uscfp;

    uscfp = umcf->upstreams.elts;

    for (i = 0; i < umcf->upstreams.nelts; i++) {

        init = uscfp[i]->peer.init_upstream ? uscfp[i]->peer.init_upstream:
                                            ngx_http_upstream_init_round_robin;

        if (init(cf, uscfp[i]) != 0) {
            return (void *) -1;
        }
    }




    if (ngx_array_init(&headers_in, cf->temp_pool, 32, sizeof(ngx_hash_key_t))
        != 0)
    {
        return (void *) -1;
    }

    for (header = ngx_http_upstream_headers_in; header->name.len; header++) {
        hk = ngx_array_push(&headers_in);
        if (hk == 0) {
            return (void *) -1;
        }

        hk->key = header->name;
        hk->key_hash = ngx_hash_key_lc(header->name.data, header->name.len);
        hk->value = header;
    }

    hash.hash = &umcf->headers_in_hash;
    hash.key = ngx_hash_key_lc;
    hash.max_size = 512;
    hash.bucket_size = (((64) + (ngx_cacheline_size - 1)) & ~(ngx_cacheline_size - 1));
    hash.name = "upstream_headers_in_hash";
    hash.pool = cf->pool;
    hash.temp_pool = 0;

    if (ngx_hash_init(&hash, headers_in.elts, headers_in.nelts) != 0) {
        return (void *) -1;
    }

    return 0;
}
