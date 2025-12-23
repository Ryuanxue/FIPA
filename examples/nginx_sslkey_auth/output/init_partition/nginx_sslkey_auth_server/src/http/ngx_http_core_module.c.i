# 1 "src/http/ngx_http_core_module.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "src/http/ngx_http_core_module.c"







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
# 9 "src/http/ngx_http_core_module.c" 2
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
# 10 "src/http/ngx_http_core_module.c" 2
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
# 11 "src/http/ngx_http_core_module.c" 2


typedef struct {
    u_char *name;
    uint32_t method;
} ngx_http_method_name_t;







static ngx_int_t ngx_http_core_find_location(ngx_http_request_t *r);
static ngx_int_t ngx_http_core_find_static_location(ngx_http_request_t *r,
    ngx_http_location_tree_node_t *node);

static ngx_int_t ngx_http_core_preconfiguration(ngx_conf_t *cf);
static ngx_int_t ngx_http_core_postconfiguration(ngx_conf_t *cf);
static void *ngx_http_core_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_core_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_core_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_core_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);
static void *ngx_http_core_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_core_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_core_server(ngx_conf_t *cf, ngx_command_t *cmd,
    void *dummy);
static char *ngx_http_core_location(ngx_conf_t *cf, ngx_command_t *cmd,
    void *dummy);
static ngx_int_t ngx_http_core_regex_location(ngx_conf_t *cf,
    ngx_http_core_loc_conf_t *clcf, ngx_str_t *regex, ngx_uint_t caseless);

static char *ngx_http_core_types(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_type(ngx_conf_t *cf, ngx_command_t *dummy,
    void *conf);

static char *ngx_http_core_listen(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_server_name(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_root(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_http_core_limit_except(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_set_aio(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_directio(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_error_page(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_open_file_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_error_log(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_keepalive(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_internal(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_core_resolver(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);






static ngx_int_t ngx_http_get_forwarded_addr_internal(ngx_http_request_t *r,
    ngx_addr_t *addr, u_char *xff, size_t xfflen, ngx_array_t *proxies,
    int recursive);

static char *ngx_http_disable_symlinks(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


static char *ngx_http_core_lowat_check(ngx_conf_t *cf, void *post, void *data);
static char *ngx_http_core_pool_size(ngx_conf_t *cf, void *post, void *data);

static ngx_conf_post_t ngx_http_core_lowat_post =
    { ngx_http_core_lowat_check };

static ngx_conf_post_handler_pt ngx_http_core_pool_size_p =
    ngx_http_core_pool_size;


static ngx_conf_enum_t ngx_http_core_request_body_in_file[] = {
    { { sizeof("off") - 1, (u_char *) "off" }, 0 },
    { { sizeof("on") - 1, (u_char *) "on" }, 1 },
    { { sizeof("clean") - 1, (u_char *) "clean" }, 2 },
    { { 0, 0 }, 0 }
};


static ngx_conf_enum_t ngx_http_core_satisfy[] = {
    { { sizeof("all") - 1, (u_char *) "all" }, 0 },
    { { sizeof("any") - 1, (u_char *) "any" }, 1 },
    { { 0, 0 }, 0 }
};


static ngx_conf_enum_t ngx_http_core_lingering_close[] = {
    { { sizeof("off") - 1, (u_char *) "off" }, 0 },
    { { sizeof("on") - 1, (u_char *) "on" }, 1 },
    { { sizeof("always") - 1, (u_char *) "always" }, 2 },
    { { 0, 0 }, 0 }
};


static ngx_conf_enum_t ngx_http_core_server_tokens[] = {
    { { sizeof("off") - 1, (u_char *) "off" }, 0 },
    { { sizeof("on") - 1, (u_char *) "on" }, 1 },
    { { sizeof("build") - 1, (u_char *) "build" }, 2 },
    { { 0, 0 }, 0 }
};


static ngx_conf_enum_t ngx_http_core_if_modified_since[] = {
    { { sizeof("off") - 1, (u_char *) "off" }, 0 },
    { { sizeof("exact") - 1, (u_char *) "exact" }, 1 },
    { { sizeof("before") - 1, (u_char *) "before" }, 2 },
    { { 0, 0 }, 0 }
};


static ngx_conf_bitmask_t ngx_http_core_keepalive_disable[] = {
    { { sizeof("none") - 1, (u_char *) "none" }, 0x0002 },
    { { sizeof("msie6") - 1, (u_char *) "msie6" }, 0x0004 },
    { { sizeof("safari") - 1, (u_char *) "safari" }, 0x0008 },
    { { 0, 0 }, 0 }
};


static ngx_path_init_t ngx_http_client_temp_path = {
    { sizeof("client_body_temp") - 1, (u_char *) "client_body_temp" }, { 0, 0, 0 }
};
# 180 "src/http/ngx_http_core_module.c"
static ngx_command_t ngx_http_core_commands[] = {

    { { sizeof("variables_hash_max_size") - 1, (u_char *) "variables_hash_max_size" },
      0x02000000|0x00000002,
      ngx_conf_set_num_slot,
      
# 185 "src/http/ngx_http_core_module.c" 3 4
     0
# 185 "src/http/ngx_http_core_module.c"
                              ,
      
# 186 "src/http/ngx_http_core_module.c" 3 4
     0
# 186 "src/http/ngx_http_core_module.c"
                                                                 ,
      0 },

    { { sizeof("variables_hash_bucket_size") - 1, (u_char *) "variables_hash_bucket_size" },
      0x02000000|0x00000002,
      ngx_conf_set_num_slot,
      
# 192 "src/http/ngx_http_core_module.c" 3 4
     0
# 192 "src/http/ngx_http_core_module.c"
                              ,
      
# 193 "src/http/ngx_http_core_module.c" 3 4
     0
# 193 "src/http/ngx_http_core_module.c"
                                                                    ,
      0 },

    { { sizeof("server_names_hash_max_size") - 1, (u_char *) "server_names_hash_max_size" },
      0x02000000|0x00000002,
      ngx_conf_set_num_slot,
      
# 199 "src/http/ngx_http_core_module.c" 3 4
     0
# 199 "src/http/ngx_http_core_module.c"
                              ,
      
# 200 "src/http/ngx_http_core_module.c" 3 4
     0
# 200 "src/http/ngx_http_core_module.c"
                                                                    ,
      0 },

    { { sizeof("server_names_hash_bucket_size") - 1, (u_char *) "server_names_hash_bucket_size" },
      0x02000000|0x00000002,
      ngx_conf_set_num_slot,
      
# 206 "src/http/ngx_http_core_module.c" 3 4
     0
# 206 "src/http/ngx_http_core_module.c"
                              ,
      
# 207 "src/http/ngx_http_core_module.c" 3 4
     0
# 207 "src/http/ngx_http_core_module.c"
                                                                       ,
      0 },

    { { sizeof("server") - 1, (u_char *) "server" },
      0x02000000|0x00000100|0x00000001,
      ngx_http_core_server,
      0,
      0,
      0 },

    { { sizeof("connection_pool_size") - 1, (u_char *) "connection_pool_size" },
      0x02000000|0x04000000|0x00000002,
      ngx_conf_set_size_slot,
      
# 220 "src/http/ngx_http_core_module.c" 3 4
     0
# 220 "src/http/ngx_http_core_module.c"
                             ,
      
# 221 "src/http/ngx_http_core_module.c" 3 4
     0
# 221 "src/http/ngx_http_core_module.c"
                                                             ,
      &ngx_http_core_pool_size_p },

    { { sizeof("request_pool_size") - 1, (u_char *) "request_pool_size" },
      0x02000000|0x04000000|0x00000002,
      ngx_conf_set_size_slot,
      
# 227 "src/http/ngx_http_core_module.c" 3 4
     0
# 227 "src/http/ngx_http_core_module.c"
                             ,
      
# 228 "src/http/ngx_http_core_module.c" 3 4
     0
# 228 "src/http/ngx_http_core_module.c"
                                                          ,
      &ngx_http_core_pool_size_p },

    { { sizeof("client_header_timeout") - 1, (u_char *) "client_header_timeout" },
      0x02000000|0x04000000|0x00000002,
      ngx_conf_set_msec_slot,
      
# 234 "src/http/ngx_http_core_module.c" 3 4
     0
# 234 "src/http/ngx_http_core_module.c"
                             ,
      
# 235 "src/http/ngx_http_core_module.c" 3 4
     0
# 235 "src/http/ngx_http_core_module.c"
                                                              ,
      0 },

    { { sizeof("client_header_buffer_size") - 1, (u_char *) "client_header_buffer_size" },
      0x02000000|0x04000000|0x00000002,
      ngx_conf_set_size_slot,
      
# 241 "src/http/ngx_http_core_module.c" 3 4
     0
# 241 "src/http/ngx_http_core_module.c"
                             ,
      
# 242 "src/http/ngx_http_core_module.c" 3 4
     0
# 242 "src/http/ngx_http_core_module.c"
                                                                  ,
      0 },

    { { sizeof("large_client_header_buffers") - 1, (u_char *) "large_client_header_buffers" },
      0x02000000|0x04000000|0x00000004,
      ngx_conf_set_bufs_slot,
      
# 248 "src/http/ngx_http_core_module.c" 3 4
     0
# 248 "src/http/ngx_http_core_module.c"
                             ,
      
# 249 "src/http/ngx_http_core_module.c" 3 4
     0
# 249 "src/http/ngx_http_core_module.c"
                                                                    ,
      0 },

    { { sizeof("ignore_invalid_headers") - 1, (u_char *) "ignore_invalid_headers" },
      0x02000000|0x04000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 255 "src/http/ngx_http_core_module.c" 3 4
     0
# 255 "src/http/ngx_http_core_module.c"
                             ,
      
# 256 "src/http/ngx_http_core_module.c" 3 4
     0
# 256 "src/http/ngx_http_core_module.c"
                                                               ,
      0 },

    { { sizeof("merge_slashes") - 1, (u_char *) "merge_slashes" },
      0x02000000|0x04000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 262 "src/http/ngx_http_core_module.c" 3 4
     0
# 262 "src/http/ngx_http_core_module.c"
                             ,
      
# 263 "src/http/ngx_http_core_module.c" 3 4
     0
# 263 "src/http/ngx_http_core_module.c"
                                                      ,
      0 },

    { { sizeof("underscores_in_headers") - 1, (u_char *) "underscores_in_headers" },
      0x02000000|0x04000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 269 "src/http/ngx_http_core_module.c" 3 4
     0
# 269 "src/http/ngx_http_core_module.c"
                             ,
      
# 270 "src/http/ngx_http_core_module.c" 3 4
     0
# 270 "src/http/ngx_http_core_module.c"
                                                               ,
      0 },

    { { sizeof("location") - 1, (u_char *) "location" },
      0x04000000|0x08000000|0x00000100|(0x00000002|0x00000004),
      ngx_http_core_location,
      
# 276 "src/http/ngx_http_core_module.c" 3 4
     0
# 276 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("listen") - 1, (u_char *) "listen" },
      0x04000000|0x00000800,
      ngx_http_core_listen,
      
# 283 "src/http/ngx_http_core_module.c" 3 4
     0
# 283 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("server_name") - 1, (u_char *) "server_name" },
      0x04000000|0x00000800,
      ngx_http_core_server_name,
      
# 290 "src/http/ngx_http_core_module.c" 3 4
     0
# 290 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("types_hash_max_size") - 1, (u_char *) "types_hash_max_size" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_num_slot,
      
# 297 "src/http/ngx_http_core_module.c" 3 4
     0
# 297 "src/http/ngx_http_core_module.c"
                             ,
      
# 298 "src/http/ngx_http_core_module.c" 3 4
     0
# 298 "src/http/ngx_http_core_module.c"
                                                            ,
      0 },

    { { sizeof("types_hash_bucket_size") - 1, (u_char *) "types_hash_bucket_size" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_num_slot,
      
# 304 "src/http/ngx_http_core_module.c" 3 4
     0
# 304 "src/http/ngx_http_core_module.c"
                             ,
      
# 305 "src/http/ngx_http_core_module.c" 3 4
     0
# 305 "src/http/ngx_http_core_module.c"
                                                               ,
      0 },

    { { sizeof("types") - 1, (u_char *) "types" },
      0x02000000|0x04000000|0x08000000
                                          |0x00000100|0x00000001,
      ngx_http_core_types,
      
# 312 "src/http/ngx_http_core_module.c" 3 4
     0
# 312 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("default_type") - 1, (u_char *) "default_type" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_str_slot,
      
# 319 "src/http/ngx_http_core_module.c" 3 4
     0
# 319 "src/http/ngx_http_core_module.c"
                             ,
      
# 320 "src/http/ngx_http_core_module.c" 3 4
     0
# 320 "src/http/ngx_http_core_module.c"
                                                     ,
      0 },

    { { sizeof("root") - 1, (u_char *) "root" },
      0x02000000|0x04000000|0x08000000|0x40000000
                        |0x00000002,
      ngx_http_core_root,
      
# 327 "src/http/ngx_http_core_module.c" 3 4
     0
# 327 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("alias") - 1, (u_char *) "alias" },
      0x08000000|0x00000002,
      ngx_http_core_root,
      
# 334 "src/http/ngx_http_core_module.c" 3 4
     0
# 334 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("limit_except") - 1, (u_char *) "limit_except" },
      0x08000000|0x00000100|0x00000800,
      ngx_http_core_limit_except,
      
# 341 "src/http/ngx_http_core_module.c" 3 4
     0
# 341 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("client_max_body_size") - 1, (u_char *) "client_max_body_size" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_off_slot,
      
# 348 "src/http/ngx_http_core_module.c" 3 4
     0
# 348 "src/http/ngx_http_core_module.c"
                             ,
      
# 349 "src/http/ngx_http_core_module.c" 3 4
     0
# 349 "src/http/ngx_http_core_module.c"
                                                             ,
      0 },

    { { sizeof("client_body_buffer_size") - 1, (u_char *) "client_body_buffer_size" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_size_slot,
      
# 355 "src/http/ngx_http_core_module.c" 3 4
     0
# 355 "src/http/ngx_http_core_module.c"
                             ,
      
# 356 "src/http/ngx_http_core_module.c" 3 4
     0
# 356 "src/http/ngx_http_core_module.c"
                                                                ,
      0 },

    { { sizeof("client_body_timeout") - 1, (u_char *) "client_body_timeout" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_msec_slot,
      
# 362 "src/http/ngx_http_core_module.c" 3 4
     0
# 362 "src/http/ngx_http_core_module.c"
                             ,
      
# 363 "src/http/ngx_http_core_module.c" 3 4
     0
# 363 "src/http/ngx_http_core_module.c"
                                                            ,
      0 },

    { { sizeof("client_body_temp_path") - 1, (u_char *) "client_body_temp_path" },
      0x02000000|0x04000000|0x08000000|(0x00000002|0x00000004|0x00000008 |0x00000010),
      ngx_conf_set_path_slot,
      
# 369 "src/http/ngx_http_core_module.c" 3 4
     0
# 369 "src/http/ngx_http_core_module.c"
                             ,
      
# 370 "src/http/ngx_http_core_module.c" 3 4
     0
# 370 "src/http/ngx_http_core_module.c"
                                                              ,
      0 },

    { { sizeof("client_body_in_file_only") - 1, (u_char *) "client_body_in_file_only" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_enum_slot,
      
# 376 "src/http/ngx_http_core_module.c" 3 4
     0
# 376 "src/http/ngx_http_core_module.c"
                             ,
      
# 377 "src/http/ngx_http_core_module.c" 3 4
     0
# 377 "src/http/ngx_http_core_module.c"
                                                                 ,
      &ngx_http_core_request_body_in_file },

    { { sizeof("client_body_in_single_buffer") - 1, (u_char *) "client_body_in_single_buffer" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 383 "src/http/ngx_http_core_module.c" 3 4
     0
# 383 "src/http/ngx_http_core_module.c"
                             ,
      
# 384 "src/http/ngx_http_core_module.c" 3 4
     0
# 384 "src/http/ngx_http_core_module.c"
                                                                     ,
      0 },

    { { sizeof("sendfile") - 1, (u_char *) "sendfile" },
      0x02000000|0x04000000|0x08000000|0x40000000
                        |0x00000200,
      ngx_conf_set_flag_slot,
      
# 391 "src/http/ngx_http_core_module.c" 3 4
     0
# 391 "src/http/ngx_http_core_module.c"
                             ,
      
# 392 "src/http/ngx_http_core_module.c" 3 4
     0
# 392 "src/http/ngx_http_core_module.c"
                                                 ,
      0 },

    { { sizeof("sendfile_max_chunk") - 1, (u_char *) "sendfile_max_chunk" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_size_slot,
      
# 398 "src/http/ngx_http_core_module.c" 3 4
     0
# 398 "src/http/ngx_http_core_module.c"
                             ,
      
# 399 "src/http/ngx_http_core_module.c" 3 4
     0
# 399 "src/http/ngx_http_core_module.c"
                                                           ,
      0 },

    { { sizeof("subrequest_output_buffer_size") - 1, (u_char *) "subrequest_output_buffer_size" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_size_slot,
      
# 405 "src/http/ngx_http_core_module.c" 3 4
     0
# 405 "src/http/ngx_http_core_module.c"
                             ,
      
# 406 "src/http/ngx_http_core_module.c" 3 4
     0
# 406 "src/http/ngx_http_core_module.c"
                                                                      ,
      0 },

    { { sizeof("aio") - 1, (u_char *) "aio" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_http_core_set_aio,
      
# 412 "src/http/ngx_http_core_module.c" 3 4
     0
# 412 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("aio_write") - 1, (u_char *) "aio_write" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 419 "src/http/ngx_http_core_module.c" 3 4
     0
# 419 "src/http/ngx_http_core_module.c"
                             ,
      
# 420 "src/http/ngx_http_core_module.c" 3 4
     0
# 420 "src/http/ngx_http_core_module.c"
                                                  ,
      0 },

    { { sizeof("read_ahead") - 1, (u_char *) "read_ahead" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_size_slot,
      
# 426 "src/http/ngx_http_core_module.c" 3 4
     0
# 426 "src/http/ngx_http_core_module.c"
                             ,
      
# 427 "src/http/ngx_http_core_module.c" 3 4
     0
# 427 "src/http/ngx_http_core_module.c"
                                                   ,
      0 },

    { { sizeof("directio") - 1, (u_char *) "directio" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_http_core_directio,
      
# 433 "src/http/ngx_http_core_module.c" 3 4
     0
# 433 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("directio_alignment") - 1, (u_char *) "directio_alignment" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_off_slot,
      
# 440 "src/http/ngx_http_core_module.c" 3 4
     0
# 440 "src/http/ngx_http_core_module.c"
                             ,
      
# 441 "src/http/ngx_http_core_module.c" 3 4
     0
# 441 "src/http/ngx_http_core_module.c"
                                                           ,
      0 },

    { { sizeof("tcp_nopush") - 1, (u_char *) "tcp_nopush" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 447 "src/http/ngx_http_core_module.c" 3 4
     0
# 447 "src/http/ngx_http_core_module.c"
                             ,
      
# 448 "src/http/ngx_http_core_module.c" 3 4
     0
# 448 "src/http/ngx_http_core_module.c"
                                                   ,
      0 },

    { { sizeof("tcp_nodelay") - 1, (u_char *) "tcp_nodelay" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 454 "src/http/ngx_http_core_module.c" 3 4
     0
# 454 "src/http/ngx_http_core_module.c"
                             ,
      
# 455 "src/http/ngx_http_core_module.c" 3 4
     0
# 455 "src/http/ngx_http_core_module.c"
                                                    ,
      0 },

    { { sizeof("send_timeout") - 1, (u_char *) "send_timeout" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_msec_slot,
      
# 461 "src/http/ngx_http_core_module.c" 3 4
     0
# 461 "src/http/ngx_http_core_module.c"
                             ,
      
# 462 "src/http/ngx_http_core_module.c" 3 4
     0
# 462 "src/http/ngx_http_core_module.c"
                                                     ,
      0 },

    { { sizeof("send_lowat") - 1, (u_char *) "send_lowat" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_size_slot,
      
# 468 "src/http/ngx_http_core_module.c" 3 4
     0
# 468 "src/http/ngx_http_core_module.c"
                             ,
      
# 469 "src/http/ngx_http_core_module.c" 3 4
     0
# 469 "src/http/ngx_http_core_module.c"
                                                   ,
      &ngx_http_core_lowat_post },

    { { sizeof("postpone_output") - 1, (u_char *) "postpone_output" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_size_slot,
      
# 475 "src/http/ngx_http_core_module.c" 3 4
     0
# 475 "src/http/ngx_http_core_module.c"
                             ,
      
# 476 "src/http/ngx_http_core_module.c" 3 4
     0
# 476 "src/http/ngx_http_core_module.c"
                                                        ,
      0 },

    { { sizeof("limit_rate") - 1, (u_char *) "limit_rate" },
      0x02000000|0x04000000|0x08000000|0x40000000
                        |0x00000002,
      ngx_conf_set_size_slot,
      
# 483 "src/http/ngx_http_core_module.c" 3 4
     0
# 483 "src/http/ngx_http_core_module.c"
                             ,
      
# 484 "src/http/ngx_http_core_module.c" 3 4
     0
# 484 "src/http/ngx_http_core_module.c"
                                                   ,
      0 },

    { { sizeof("limit_rate_after") - 1, (u_char *) "limit_rate_after" },
      0x02000000|0x04000000|0x08000000|0x40000000
                        |0x00000002,
      ngx_conf_set_size_slot,
      
# 491 "src/http/ngx_http_core_module.c" 3 4
     0
# 491 "src/http/ngx_http_core_module.c"
                             ,
      
# 492 "src/http/ngx_http_core_module.c" 3 4
     0
# 492 "src/http/ngx_http_core_module.c"
                                                         ,
      0 },

    { { sizeof("keepalive_timeout") - 1, (u_char *) "keepalive_timeout" },
      0x02000000|0x04000000|0x08000000|(0x00000002|0x00000004),
      ngx_http_core_keepalive,
      
# 498 "src/http/ngx_http_core_module.c" 3 4
     0
# 498 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("keepalive_requests") - 1, (u_char *) "keepalive_requests" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_num_slot,
      
# 505 "src/http/ngx_http_core_module.c" 3 4
     0
# 505 "src/http/ngx_http_core_module.c"
                             ,
      
# 506 "src/http/ngx_http_core_module.c" 3 4
     0
# 506 "src/http/ngx_http_core_module.c"
                                                           ,
      0 },

    { { sizeof("keepalive_disable") - 1, (u_char *) "keepalive_disable" },
      0x02000000|0x04000000|0x08000000|(0x00000002|0x00000004),
      ngx_conf_set_bitmask_slot,
      
# 512 "src/http/ngx_http_core_module.c" 3 4
     0
# 512 "src/http/ngx_http_core_module.c"
                             ,
      
# 513 "src/http/ngx_http_core_module.c" 3 4
     0
# 513 "src/http/ngx_http_core_module.c"
                                                          ,
      &ngx_http_core_keepalive_disable },

    { { sizeof("satisfy") - 1, (u_char *) "satisfy" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_enum_slot,
      
# 519 "src/http/ngx_http_core_module.c" 3 4
     0
# 519 "src/http/ngx_http_core_module.c"
                             ,
      
# 520 "src/http/ngx_http_core_module.c" 3 4
     0
# 520 "src/http/ngx_http_core_module.c"
                                                ,
      &ngx_http_core_satisfy },

    { { sizeof("internal") - 1, (u_char *) "internal" },
      0x08000000|0x00000001,
      ngx_http_core_internal,
      
# 526 "src/http/ngx_http_core_module.c" 3 4
     0
# 526 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("lingering_close") - 1, (u_char *) "lingering_close" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_enum_slot,
      
# 533 "src/http/ngx_http_core_module.c" 3 4
     0
# 533 "src/http/ngx_http_core_module.c"
                             ,
      
# 534 "src/http/ngx_http_core_module.c" 3 4
     0
# 534 "src/http/ngx_http_core_module.c"
                                                        ,
      &ngx_http_core_lingering_close },

    { { sizeof("lingering_time") - 1, (u_char *) "lingering_time" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_msec_slot,
      
# 540 "src/http/ngx_http_core_module.c" 3 4
     0
# 540 "src/http/ngx_http_core_module.c"
                             ,
      
# 541 "src/http/ngx_http_core_module.c" 3 4
     0
# 541 "src/http/ngx_http_core_module.c"
                                                       ,
      0 },

    { { sizeof("lingering_timeout") - 1, (u_char *) "lingering_timeout" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_msec_slot,
      
# 547 "src/http/ngx_http_core_module.c" 3 4
     0
# 547 "src/http/ngx_http_core_module.c"
                             ,
      
# 548 "src/http/ngx_http_core_module.c" 3 4
     0
# 548 "src/http/ngx_http_core_module.c"
                                                          ,
      0 },

    { { sizeof("reset_timedout_connection") - 1, (u_char *) "reset_timedout_connection" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 554 "src/http/ngx_http_core_module.c" 3 4
     0
# 554 "src/http/ngx_http_core_module.c"
                             ,
      
# 555 "src/http/ngx_http_core_module.c" 3 4
     0
# 555 "src/http/ngx_http_core_module.c"
                                                                  ,
      0 },

    { { sizeof("absolute_redirect") - 1, (u_char *) "absolute_redirect" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 561 "src/http/ngx_http_core_module.c" 3 4
     0
# 561 "src/http/ngx_http_core_module.c"
                             ,
      
# 562 "src/http/ngx_http_core_module.c" 3 4
     0
# 562 "src/http/ngx_http_core_module.c"
                                                          ,
      0 },

    { { sizeof("server_name_in_redirect") - 1, (u_char *) "server_name_in_redirect" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 568 "src/http/ngx_http_core_module.c" 3 4
     0
# 568 "src/http/ngx_http_core_module.c"
                             ,
      
# 569 "src/http/ngx_http_core_module.c" 3 4
     0
# 569 "src/http/ngx_http_core_module.c"
                                                                ,
      0 },

    { { sizeof("port_in_redirect") - 1, (u_char *) "port_in_redirect" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 575 "src/http/ngx_http_core_module.c" 3 4
     0
# 575 "src/http/ngx_http_core_module.c"
                             ,
      
# 576 "src/http/ngx_http_core_module.c" 3 4
     0
# 576 "src/http/ngx_http_core_module.c"
                                                         ,
      0 },

    { { sizeof("msie_padding") - 1, (u_char *) "msie_padding" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 582 "src/http/ngx_http_core_module.c" 3 4
     0
# 582 "src/http/ngx_http_core_module.c"
                             ,
      
# 583 "src/http/ngx_http_core_module.c" 3 4
     0
# 583 "src/http/ngx_http_core_module.c"
                                                     ,
      0 },

    { { sizeof("msie_refresh") - 1, (u_char *) "msie_refresh" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 589 "src/http/ngx_http_core_module.c" 3 4
     0
# 589 "src/http/ngx_http_core_module.c"
                             ,
      
# 590 "src/http/ngx_http_core_module.c" 3 4
     0
# 590 "src/http/ngx_http_core_module.c"
                                                     ,
      0 },

    { { sizeof("log_not_found") - 1, (u_char *) "log_not_found" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 596 "src/http/ngx_http_core_module.c" 3 4
     0
# 596 "src/http/ngx_http_core_module.c"
                             ,
      
# 597 "src/http/ngx_http_core_module.c" 3 4
     0
# 597 "src/http/ngx_http_core_module.c"
                                                      ,
      0 },

    { { sizeof("log_subrequest") - 1, (u_char *) "log_subrequest" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 603 "src/http/ngx_http_core_module.c" 3 4
     0
# 603 "src/http/ngx_http_core_module.c"
                             ,
      
# 604 "src/http/ngx_http_core_module.c" 3 4
     0
# 604 "src/http/ngx_http_core_module.c"
                                                       ,
      0 },

    { { sizeof("recursive_error_pages") - 1, (u_char *) "recursive_error_pages" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 610 "src/http/ngx_http_core_module.c" 3 4
     0
# 610 "src/http/ngx_http_core_module.c"
                             ,
      
# 611 "src/http/ngx_http_core_module.c" 3 4
     0
# 611 "src/http/ngx_http_core_module.c"
                                                              ,
      0 },

    { { sizeof("server_tokens") - 1, (u_char *) "server_tokens" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_enum_slot,
      
# 617 "src/http/ngx_http_core_module.c" 3 4
     0
# 617 "src/http/ngx_http_core_module.c"
                             ,
      
# 618 "src/http/ngx_http_core_module.c" 3 4
     0
# 618 "src/http/ngx_http_core_module.c"
                                                      ,
      &ngx_http_core_server_tokens },

    { { sizeof("if_modified_since") - 1, (u_char *) "if_modified_since" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_enum_slot,
      
# 624 "src/http/ngx_http_core_module.c" 3 4
     0
# 624 "src/http/ngx_http_core_module.c"
                             ,
      
# 625 "src/http/ngx_http_core_module.c" 3 4
     0
# 625 "src/http/ngx_http_core_module.c"
                                                          ,
      &ngx_http_core_if_modified_since },

    { { sizeof("max_ranges") - 1, (u_char *) "max_ranges" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_num_slot,
      
# 631 "src/http/ngx_http_core_module.c" 3 4
     0
# 631 "src/http/ngx_http_core_module.c"
                             ,
      
# 632 "src/http/ngx_http_core_module.c" 3 4
     0
# 632 "src/http/ngx_http_core_module.c"
                                                   ,
      0 },

    { { sizeof("chunked_transfer_encoding") - 1, (u_char *) "chunked_transfer_encoding" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 638 "src/http/ngx_http_core_module.c" 3 4
     0
# 638 "src/http/ngx_http_core_module.c"
                             ,
      
# 639 "src/http/ngx_http_core_module.c" 3 4
     0
# 639 "src/http/ngx_http_core_module.c"
                                                                  ,
      0 },

    { { sizeof("etag") - 1, (u_char *) "etag" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 645 "src/http/ngx_http_core_module.c" 3 4
     0
# 645 "src/http/ngx_http_core_module.c"
                             ,
      
# 646 "src/http/ngx_http_core_module.c" 3 4
     0
# 646 "src/http/ngx_http_core_module.c"
                                             ,
      0 },

    { { sizeof("error_page") - 1, (u_char *) "error_page" },
      0x02000000|0x04000000|0x08000000|0x40000000
                        |0x00001000,
      ngx_http_core_error_page,
      
# 653 "src/http/ngx_http_core_module.c" 3 4
     0
# 653 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("post_action") - 1, (u_char *) "post_action" },
      0x02000000|0x04000000|0x08000000|0x40000000
                        |0x00000002,
      ngx_conf_set_str_slot,
      
# 661 "src/http/ngx_http_core_module.c" 3 4
     0
# 661 "src/http/ngx_http_core_module.c"
                             ,
      
# 662 "src/http/ngx_http_core_module.c" 3 4
     0
# 662 "src/http/ngx_http_core_module.c"
                                                    ,
      0 },

    { { sizeof("error_log") - 1, (u_char *) "error_log" },
      0x02000000|0x04000000|0x08000000|0x00000800,
      ngx_http_core_error_log,
      
# 668 "src/http/ngx_http_core_module.c" 3 4
     0
# 668 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("open_file_cache") - 1, (u_char *) "open_file_cache" },
      0x02000000|0x04000000|0x08000000|(0x00000002|0x00000004),
      ngx_http_core_open_file_cache,
      
# 675 "src/http/ngx_http_core_module.c" 3 4
     0
# 675 "src/http/ngx_http_core_module.c"
                             ,
      
# 676 "src/http/ngx_http_core_module.c" 3 4
     0
# 676 "src/http/ngx_http_core_module.c"
                                                        ,
      0 },

    { { sizeof("open_file_cache_valid") - 1, (u_char *) "open_file_cache_valid" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_sec_slot,
      
# 682 "src/http/ngx_http_core_module.c" 3 4
     0
# 682 "src/http/ngx_http_core_module.c"
                             ,
      
# 683 "src/http/ngx_http_core_module.c" 3 4
     0
# 683 "src/http/ngx_http_core_module.c"
                                                              ,
      0 },

    { { sizeof("open_file_cache_min_uses") - 1, (u_char *) "open_file_cache_min_uses" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_num_slot,
      
# 689 "src/http/ngx_http_core_module.c" 3 4
     0
# 689 "src/http/ngx_http_core_module.c"
                             ,
      
# 690 "src/http/ngx_http_core_module.c" 3 4
     0
# 690 "src/http/ngx_http_core_module.c"
                                                                 ,
      0 },

    { { sizeof("open_file_cache_errors") - 1, (u_char *) "open_file_cache_errors" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 696 "src/http/ngx_http_core_module.c" 3 4
     0
# 696 "src/http/ngx_http_core_module.c"
                             ,
      
# 697 "src/http/ngx_http_core_module.c" 3 4
     0
# 697 "src/http/ngx_http_core_module.c"
                                                               ,
      0 },

    { { sizeof("open_file_cache_events") - 1, (u_char *) "open_file_cache_events" },
      0x02000000|0x04000000|0x08000000|0x00000200,
      ngx_conf_set_flag_slot,
      
# 703 "src/http/ngx_http_core_module.c" 3 4
     0
# 703 "src/http/ngx_http_core_module.c"
                             ,
      
# 704 "src/http/ngx_http_core_module.c" 3 4
     0
# 704 "src/http/ngx_http_core_module.c"
                                                               ,
      0 },

    { { sizeof("resolver") - 1, (u_char *) "resolver" },
      0x02000000|0x04000000|0x08000000|0x00000800,
      ngx_http_core_resolver,
      
# 710 "src/http/ngx_http_core_module.c" 3 4
     0
# 710 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },

    { { sizeof("resolver_timeout") - 1, (u_char *) "resolver_timeout" },
      0x02000000|0x04000000|0x08000000|0x00000002,
      ngx_conf_set_msec_slot,
      
# 717 "src/http/ngx_http_core_module.c" 3 4
     0
# 717 "src/http/ngx_http_core_module.c"
                             ,
      
# 718 "src/http/ngx_http_core_module.c" 3 4
     0
# 718 "src/http/ngx_http_core_module.c"
                                                         ,
      0 },
# 755 "src/http/ngx_http_core_module.c"
    { { sizeof("disable_symlinks") - 1, (u_char *) "disable_symlinks" },
      0x02000000|0x04000000|0x08000000|(0x00000002|0x00000004),
      ngx_http_disable_symlinks,
      
# 758 "src/http/ngx_http_core_module.c" 3 4
     0
# 758 "src/http/ngx_http_core_module.c"
                             ,
      0,
      0 },



      { { 0, 0 }, 0, 0, 0, 0, 0 }
};


static ngx_http_module_t ngx_http_core_module_ctx = {
    ngx_http_core_preconfiguration,
    ngx_http_core_postconfiguration,

    ngx_http_core_create_main_conf,
    ngx_http_core_init_main_conf,

    ngx_http_core_create_srv_conf,
    ngx_http_core_merge_srv_conf,

    ngx_http_core_create_loc_conf,
    ngx_http_core_merge_loc_conf
};


ngx_module_t ngx_http_core_module = {
    (ngx_uint_t) -1, (ngx_uint_t) -1, 0, 0, 0, 1015005, "8" "," "4" "," "8" "," "0" "0" "0" "0" "1" "1" "1" "1" "1" "1" "0" "1" "0" "1" "1" "1" "0" "0" "1" "1" "1" "0" "0" "0" "1" "0" "1" "0" "0" "0" "0" "1" "1" "0",
    &ngx_http_core_module_ctx,
    ngx_http_core_commands,
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


ngx_str_t ngx_http_core_get_method = { 3, (u_char *) "GET" };


void
ngx_http_handler(ngx_http_request_t *r)
{
    ngx_http_core_main_conf_t *cmcf;

    r->connection->log->action = 0;

    if (!r->internal) {
        switch (r->headers_in.connection_type) {
        case 0:
            r->keepalive = (r->http_version > 1000);
            break;

        case 1:
            r->keepalive = 0;
            break;

        case 2:
            r->keepalive = 1;
            break;
        }

        r->lingering_close = (r->headers_in.content_length_n > 0
                              || r->headers_in.chunked);
        r->phase_handler = 0;

    } else {
        cmcf = (r)->main_conf[ngx_http_core_module.ctx_index];
        r->phase_handler = cmcf->phase_engine.server_rewrite_index;
    }

    r->valid_location = 1;






    r->write_event_handler = ngx_http_core_run_phases;
    ngx_http_core_run_phases(r);
}


void
ngx_http_core_run_phases(ngx_http_request_t *r)
{
    ngx_int_t rc;
    ngx_http_phase_handler_t *ph;
    ngx_http_core_main_conf_t *cmcf;

    cmcf = (r)->main_conf[ngx_http_core_module.ctx_index];

    ph = cmcf->phase_engine.handlers;

    while (ph[r->phase_handler].checker) {

        rc = ph[r->phase_handler].checker(r, &ph[r->phase_handler]);

        if (rc == 0) {
            return;
        }
    }
}


ngx_int_t
ngx_http_core_generic_phase(ngx_http_request_t *r, ngx_http_phase_handler_t *ph)
{
    ngx_int_t rc;






   
                                                          ;

    rc = ph->handler(r);

    if (rc == 0) {
        r->phase_handler = ph->next;
        return -2;
    }

    if (rc == -5) {
        r->phase_handler++;
        return -2;
    }

    if (rc == -2 || rc == -4) {
        return 0;
    }



    ngx_http_finalize_request(r, rc);

    return 0;
}


ngx_int_t
ngx_http_core_rewrite_phase(ngx_http_request_t *r, ngx_http_phase_handler_t *ph)
{
    ngx_int_t rc;

   
                                                          ;

    rc = ph->handler(r);

    if (rc == -5) {
        r->phase_handler++;
        return -2;
    }

    if (rc == -4) {
        return 0;
    }



    ngx_http_finalize_request(r, rc);

    return 0;
}


ngx_int_t
ngx_http_core_find_config_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph)
{
    u_char *p;
    size_t len;
    ngx_int_t rc;
    ngx_http_core_loc_conf_t *clcf;

    r->content_handler = 0;
    r->uri_changed = 0;

    rc = ngx_http_core_find_location(r);

    if (rc == -1) {
        ngx_http_finalize_request(r, 500);
        return 0;
    }

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    if (!r->internal && clcf->internal) {
        ngx_http_finalize_request(r, 404);
        return 0;
    }

   


                               ;

    ngx_http_update_location_config(r);

   

                                                                              ;

    if (r->headers_in.content_length_n != -1
        && !r->discard_body
        && clcf->client_max_body_size
        && clcf->client_max_body_size < r->headers_in.content_length_n)
    {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "client intended to send too large body: %O bytes", r->headers_in.content_length_n)

                                                     ;

        r->expect_tested = 1;
        (void) ngx_http_discard_request_body(r);
        ngx_http_finalize_request(r, 413);
        return 0;
    }

    if (rc == -4) {
        if (r->headers_out.location) { r->headers_out.location->hash = 0; r->headers_out.location = 0; };

        r->headers_out.location = ngx_list_push(&r->headers_out.headers);
        if (r->headers_out.location == 0) {
            ngx_http_finalize_request(r, 500);
            return 0;
        }

        r->headers_out.location->hash = 1;
        (&r->headers_out.location->key)->len = sizeof("Location") - 1; (&r->headers_out.location->key)->data = (u_char *) "Location";

        if (r->args.len == 0) {
            r->headers_out.location->value = clcf->name;

        } else {
            len = clcf->name.len + 1 + r->args.len;
            p = ngx_pnalloc(r->pool, len);

            if (p == 0) {
                if (r->headers_out.location) { r->headers_out.location->hash = 0; r->headers_out.location = 0; };
                ngx_http_finalize_request(r, 500);
                return 0;
            }

            r->headers_out.location->value.len = len;
            r->headers_out.location->value.data = p;

            p = (((u_char *) memcpy(p, clcf->name.data, clcf->name.len)) + (clcf->name.len));
            *p++ = '?';
            (void) memcpy(p, r->args.data, r->args.len);
        }

        ngx_http_finalize_request(r, 301);
        return 0;
    }

    r->phase_handler++;
    return -2;
}


ngx_int_t
ngx_http_core_post_rewrite_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph)
{
    ngx_http_core_srv_conf_t *cscf;

   
                                                               ;

    if (!r->uri_changed) {
        r->phase_handler++;
        return -2;
    }

   
                                                     ;
# 1049 "src/http/ngx_http_core_module.c"
    r->uri_changes--;

    if (r->uri_changes == 0) {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "rewrite or internal redirection cycle " "while processing \"%V\"", &r->uri)

                                                         ;

        ngx_http_finalize_request(r, 500);
        return 0;
    }

    r->phase_handler = ph->next;

    cscf = (r)->srv_conf[ngx_http_core_module.ctx_index];
    r->loc_conf = cscf->ctx->loc_conf;

    return -2;
}


ngx_int_t
ngx_http_core_access_phase(ngx_http_request_t *r, ngx_http_phase_handler_t *ph)
{
    ngx_int_t rc;
    ngx_http_core_loc_conf_t *clcf;

    if (r != r->main) {
        r->phase_handler = ph->next;
        return -2;
    }

   
                                                         ;

    rc = ph->handler(r);

    if (rc == -5) {
        r->phase_handler++;
        return -2;
    }

    if (rc == -2 || rc == -4) {
        return 0;
    }

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    if (clcf->satisfy == 0) {

        if (rc == 0) {
            r->phase_handler++;
            return -2;
        }

    } else {
        if (rc == 0) {
            r->access_code = 0;

            if (r->headers_out.www_authenticate) {
                r->headers_out.www_authenticate->hash = 0;
            }

            r->phase_handler = ph->next;
            return -2;
        }

        if (rc == 403 || rc == 401) {
            if (r->access_code != 401) {
                r->access_code = rc;
            }

            r->phase_handler++;
            return -2;
        }
    }



    ngx_http_finalize_request(r, rc);
    return 0;
}


ngx_int_t
ngx_http_core_post_access_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph)
{
    ngx_int_t access_code;

   
                                                              ;

    access_code = r->access_code;

    if (access_code) {
        if (access_code == 403) {
            if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "access forbidden by rule")
                                                     ;
        }

        r->access_code = 0;
        ngx_http_finalize_request(r, access_code);
        return 0;
    }

    r->phase_handler++;
    return -2;
}


ngx_int_t
ngx_http_core_content_phase(ngx_http_request_t *r,
    ngx_http_phase_handler_t *ph)
{
    size_t root;
    ngx_int_t rc;
    ngx_str_t path;

    if (r->content_handler) {
        r->write_event_handler = ngx_http_request_empty_handler;
        ngx_http_finalize_request(r, r->content_handler(r));
        return 0;
    }

   
                                                          ;

    rc = ph->handler(r);

    if (rc != -5) {
        ngx_http_finalize_request(r, rc);
        return 0;
    }



    ph++;

    if (ph->checker) {
        r->phase_handler++;
        return -2;
    }



    if (r->uri.data[r->uri.len - 1] == '/') {

        if (ngx_http_map_uri_to_path(r, &path, &root, 0) != 0) {
            if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "directory index of \"%s\" is forbidden", path.data)
                                                                              ;
        }

        ngx_http_finalize_request(r, 403);
        return 0;
    }

    if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "no handler found");

    ngx_http_finalize_request(r, 404);
    return 0;
}


void
ngx_http_update_location_config(ngx_http_request_t *r)
{
    ngx_http_core_loc_conf_t *clcf;

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    if (r->method & clcf->limit_except) {
        r->loc_conf = clcf->limit_except_loc_conf;
        clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];
    }

    if (r == r->main) {
        r->connection->log->file = clcf->error_log->file; r->connection->log->next = clcf->error_log->next; r->connection->log->writer = clcf->error_log->writer; r->connection->log->wdata = clcf->error_log->wdata; if (!(r->connection->log->log_level & 0x80000000)) { r->connection->log->log_level = clcf->error_log->log_level; };
    }

    if ((ngx_io.flags & 1) && clcf->sendfile) {
        r->connection->sendfile = 1;

    } else {
        r->connection->sendfile = 0;
    }

    if (clcf->client_body_in_file_only) {
        r->request_body_in_file_only = 1;
        r->request_body_in_persistent_file = 1;
        r->request_body_in_clean_file =
            clcf->client_body_in_file_only == 2;
        r->request_body_file_log_level = 6;

    } else {
        r->request_body_file_log_level = 5;
    }

    r->request_body_in_single_buf = clcf->client_body_in_single_buffer;

    if (r->keepalive) {
        if (clcf->keepalive_timeout == 0) {
            r->keepalive = 0;

        } else if (r->connection->requests >= clcf->keepalive_requests) {
            r->keepalive = 0;

        } else if (r->headers_in.msie6
                   && r->method == 0x0008
                   && (clcf->keepalive_disable
                       & 0x0004))
        {




            r->keepalive = 0;

        } else if (r->headers_in.safari
                   && (clcf->keepalive_disable
                       & 0x0008))
        {





            r->keepalive = 0;
        }
    }

    if (!clcf->tcp_nopush) {

        r->connection->tcp_nopush = NGX_TCP_NOPUSH_DISABLED;
    }

    if (r->limit_rate == 0) {
        r->limit_rate = clcf->limit_rate;
    }

    if (clcf->handler) {
        r->content_handler = clcf->handler;
    }
}
# 1302 "src/http/ngx_http_core_module.c"
static ngx_int_t
ngx_http_core_find_location(ngx_http_request_t *r)
{
    ngx_int_t rc;
    ngx_http_core_loc_conf_t *pclcf;
# 1315 "src/http/ngx_http_core_module.c"
    pclcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    rc = ngx_http_core_find_static_location(r, pclcf->static_locations);

    if (rc == -2) {
# 1329 "src/http/ngx_http_core_module.c"
        rc = ngx_http_core_find_location(r);
    }

    if (rc == 0 || rc == -4) {
        return rc;
    }
# 1368 "src/http/ngx_http_core_module.c"
    return rc;
}
# 1379 "src/http/ngx_http_core_module.c"
static ngx_int_t
ngx_http_core_find_static_location(ngx_http_request_t *r,
    ngx_http_location_tree_node_t *node)
{
    u_char *uri;
    size_t len, n;
    ngx_int_t rc, rv;

    len = r->uri.len;
    uri = r->uri.data;

    rv = -5;

    for ( ;; ) {

        if (node == 0) {
            return rv;
        }

       

                                                      ;

        n = (len <= (size_t) node->len) ? len : node->len;

        rc = ngx_filename_cmp(uri, node->name, n);

        if (rc != 0) {
            node = (rc < 0) ? node->left : node->right;

            continue;
        }

        if (len > (size_t) node->len) {

            if (node->inclusive) {

                r->loc_conf = node->inclusive->loc_conf;
                rv = -2;

                node = node->tree;
                uri += n;
                len -= n;

                continue;
            }



            node = node->right;

            continue;
        }

        if (len == (size_t) node->len) {

            if (node->exact) {
                r->loc_conf = node->exact->loc_conf;
                return 0;

            } else {
                r->loc_conf = node->inclusive->loc_conf;
                return -2;
            }
        }



        if (len + 1 == (size_t) node->len && node->auto_redirect) {

            r->loc_conf = (node->exact) ? node->exact->loc_conf:
                                          node->inclusive->loc_conf;
            rv = -4;
        }

        node = node->left;
    }
}


void *
ngx_http_test_content_type(ngx_http_request_t *r, ngx_hash_t *types_hash)
{
    u_char c, *lowcase;
    size_t len;
    ngx_uint_t i, hash;

    if (types_hash->size == 0) {
        return (void *) 4;
    }

    if (r->headers_out.content_type.len == 0) {
        return 0;
    }

    len = r->headers_out.content_type_len;

    if (r->headers_out.content_type_lowcase == 0) {

        lowcase = ngx_pnalloc(r->pool, len);
        if (lowcase == 0) {
            return 0;
        }

        r->headers_out.content_type_lowcase = lowcase;

        hash = 0;

        for (i = 0; i < len; i++) {
            c = (u_char) ((r->headers_out.content_type.data[i] >= 'A' && r->headers_out.content_type.data[i] <= 'Z') ? (r->headers_out.content_type.data[i] | 0x20) : r->headers_out.content_type.data[i]);
            hash = ((ngx_uint_t) hash * 31 + c);
            lowcase[i] = c;
        }

        r->headers_out.content_type_hash = hash;
    }

    return ngx_hash_find(types_hash, r->headers_out.content_type_hash,
                         r->headers_out.content_type_lowcase, len);
}


ngx_int_t
ngx_http_set_content_type(ngx_http_request_t *r)
{
    u_char c, *exten;
    ngx_str_t *type;
    ngx_uint_t i, hash;
    ngx_http_core_loc_conf_t *clcf;

    if (r->headers_out.content_type.len) {
        return 0;
    }

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    if (r->exten.len) {

        hash = 0;

        for (i = 0; i < r->exten.len; i++) {
            c = r->exten.data[i];

            if (c >= 'A' && c <= 'Z') {

                exten = ngx_pnalloc(r->pool, r->exten.len);
                if (exten == 0) {
                    return -1;
                }

                hash = ngx_hash_strlow(exten, r->exten.data, r->exten.len);

                r->exten.data = exten;

                break;
            }

            hash = ((ngx_uint_t) hash * 31 + c);
        }

        type = ngx_hash_find(&clcf->types_hash, hash,
                             r->exten.data, r->exten.len);

        if (type) {
            r->headers_out.content_type_len = type->len;
            r->headers_out.content_type = *type;

            return 0;
        }
    }

    r->headers_out.content_type_len = clcf->default_type.len;
    r->headers_out.content_type = clcf->default_type;

    return 0;
}


void
ngx_http_set_exten(ngx_http_request_t *r)
{
    ngx_int_t i;

    (&r->exten)->len = 0; (&r->exten)->data = 0;

    for (i = r->uri.len - 1; i > 1; i--) {
        if (r->uri.data[i] == '.' && r->uri.data[i - 1] != '/') {

            r->exten.len = r->uri.len - i - 1;
            r->exten.data = &r->uri.data[i + 1];

            return;

        } else if (r->uri.data[i] == '/') {
            return;
        }
    }

    return;
}


ngx_int_t
ngx_http_set_etag(ngx_http_request_t *r)
{
    ngx_table_elt_t *etag;
    ngx_http_core_loc_conf_t *clcf;

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    if (!clcf->etag) {
        return 0;
    }

    etag = ngx_list_push(&r->headers_out.headers);
    if (etag == 0) {
        return -1;
    }

    etag->hash = 1;
    (&etag->key)->len = sizeof("ETag") - 1; (&etag->key)->data = (u_char *) "ETag";

    etag->value.data = ngx_pnalloc(r->pool, (sizeof("-9223372036854775808") - 1) + (sizeof("-9223372036854775808") - 1) + 3);
    if (etag->value.data == 0) {
        etag->hash = 0;
        return -1;
    }

    etag->value.len = ngx_sprintf(etag->value.data, "\"%xT-%xO\"",
                                  r->headers_out.last_modified_time,
                                  r->headers_out.content_length_n)
                      - etag->value.data;

    r->headers_out.etag = etag;

    return 0;
}


void
ngx_http_weak_etag(ngx_http_request_t *r)
{
    size_t len;
    u_char *p;
    ngx_table_elt_t *etag;

    etag = r->headers_out.etag;

    if (etag == 0) {
        return;
    }

    if (etag->value.len > 2
        && etag->value.data[0] == 'W'
        && etag->value.data[1] == '/')
    {
        return;
    }

    if (etag->value.len < 1 || etag->value.data[0] != '"') {
        r->headers_out.etag->hash = 0;
        r->headers_out.etag = 0;
        return;
    }

    p = ngx_pnalloc(r->pool, etag->value.len + 2);
    if (p == 0) {
        r->headers_out.etag->hash = 0;
        r->headers_out.etag = 0;
        return;
    }

    len = ngx_sprintf(p, "W/%V", &etag->value) - p;

    etag->value.data = p;
    etag->value.len = len;
}


ngx_int_t
ngx_http_send_response(ngx_http_request_t *r, ngx_uint_t status,
    ngx_str_t *ct, ngx_http_complex_value_t *cv)
{
    ngx_int_t rc;
    ngx_str_t val;
    ngx_buf_t *b;
    ngx_chain_t out;

    if (ngx_http_discard_request_body(r) != 0) {
        return 500;
    }

    r->headers_out.status = status;

    if (ngx_http_complex_value(r, cv, &val) != 0) {
        return 500;
    }

    if (status == 301
        || status == 302
        || status == 303
        || status == 307
        || status == 308)
    {
        if (r->headers_out.location) { r->headers_out.location->hash = 0; r->headers_out.location = 0; };

        r->headers_out.location = ngx_list_push(&r->headers_out.headers);
        if (r->headers_out.location == 0) {
            return 500;
        }

        r->headers_out.location->hash = 1;
        (&r->headers_out.location->key)->len = sizeof("Location") - 1; (&r->headers_out.location->key)->data = (u_char *) "Location";
        r->headers_out.location->value = val;

        return status;
    }

    r->headers_out.content_length_n = val.len;

    if (ct) {
        r->headers_out.content_type_len = ct->len;
        r->headers_out.content_type = *ct;

    } else {
        if (ngx_http_set_content_type(r) != 0) {
            return 500;
        }
    }

    if (r->method == 0x0004 || (r != r->main && val.len == 0)) {
        return ngx_http_send_header(r);
    }

    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == 0) {
        return 500;
    }

    b->pos = val.data;
    b->last = val.data + val.len;
    b->memory = val.len ? 1 : 0;
    b->last_buf = (r == r->main) ? 1 : 0;
    b->last_in_chain = 1;

    out.buf = b;
    out.next = 0;

    rc = ngx_http_send_header(r);

    if (rc == -1 || rc > 0 || r->header_only) {
        return rc;
    }

    return ngx_http_output_filter(r, &out);
}


ngx_int_t
ngx_http_send_header(ngx_http_request_t *r)
{
    if (r->post_action) {
        return 0;
    }

    if (r->header_sent) {
        if ((r->connection->log)->log_level >= 2) ngx_log_error_core(2, r->connection->log, 0, "header already sent")
                                            ;
        return -1;
    }

    if (r->err_status) {
        r->headers_out.status = r->err_status;
        r->headers_out.status_line.len = 0;
    }

    return ngx_http_top_header_filter(r);
}


ngx_int_t
ngx_http_output_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
    ngx_int_t rc;
    ngx_connection_t *c;

    c = r->connection;

   
                                                                     ;

    rc = ngx_http_top_body_filter(r, in);

    if (rc == -1) {

        c->error = 1;
    }

    return rc;
}


u_char *
ngx_http_map_uri_to_path(ngx_http_request_t *r, ngx_str_t *path,
    size_t *root_length, size_t reserved)
{
    u_char *last;
    size_t alias;
    ngx_http_core_loc_conf_t *clcf;

    clcf = (r)->loc_conf[ngx_http_core_module.ctx_index];

    alias = clcf->alias;

    if (alias && !r->valid_location) {
        if ((r->connection->log)->log_level >= 2) ngx_log_error_core(2, r->connection->log, 0, "\"alias\" cannot be used in location \"%V\" " "where URI was rewritten", &clcf->name)

                                                             ;
        return 0;
    }

    if (clcf->root_lengths == 0) {

        *root_length = clcf->root.len;

        path->len = clcf->root.len + reserved + r->uri.len - alias + 1;

        path->data = ngx_pnalloc(r->pool, path->len);
        if (path->data == 0) {
            return 0;
        }

        last = (((u_char *) memcpy(path->data, clcf->root.data, clcf->root.len)) + (clcf->root.len));

    } else {

        if (alias == 9223372036854775807LL) {
            reserved += r->add_uri_to_alias ? r->uri.len + 1 : 1;

        } else {
            reserved += r->uri.len - alias + 1;
        }

        if (ngx_http_script_run(r, path, clcf->root_lengths->elts, reserved,
                                clcf->root_values->elts)
            == 0)
        {
            return 0;
        }

        if (ngx_get_full_name(r->pool, (ngx_str_t *) &ngx_cycle->prefix, path)
            != 0)
        {
            return 0;
        }

        *root_length = path->len - reserved;
        last = path->data + *root_length;

        if (alias == 9223372036854775807LL) {
            if (!r->add_uri_to_alias) {
                *last = '\0';
                return last;
            }

            alias = 0;
        }
    }

    last = ngx_cpystrn(last, r->uri.data + alias, r->uri.len - alias + 1);

    return last;
}


ngx_int_t
ngx_http_auth_basic_user(ngx_http_request_t *r)
{
    ngx_str_t auth, encoded;
    ngx_uint_t len;

    if (r->headers_in.user.len == 0 && r->headers_in.user.data != 0) {
        return -5;
    }

    if (r->headers_in.authorization == 0) {
        r->headers_in.user.data = (u_char *) "";
        return -5;
    }

    encoded = r->headers_in.authorization->value;

    if (encoded.len < sizeof("Basic ") - 1
        || ngx_strncasecmp(encoded.data, (u_char *) "Basic ",
                           sizeof("Basic ") - 1)
           != 0)
    {
        r->headers_in.user.data = (u_char *) "";
        return -5;
    }

    encoded.len -= sizeof("Basic ") - 1;
    encoded.data += sizeof("Basic ") - 1;

    while (encoded.len && encoded.data[0] == ' ') {
        encoded.len--;
        encoded.data++;
    }

    if (encoded.len == 0) {
        r->headers_in.user.data = (u_char *) "";
        return -5;
    }

    auth.len = (((encoded.len + 3) / 4) * 3);
    auth.data = ngx_pnalloc(r->pool, auth.len + 1);
    if (auth.data == 0) {
        return -1;
    }

    if (ngx_decode_base64(&auth, &encoded) != 0) {
        r->headers_in.user.data = (u_char *) "";
        return -5;
    }

    auth.data[auth.len] = '\0';

    for (len = 0; len < auth.len; len++) {
        if (auth.data[len] == ':') {
            break;
        }
    }

    if (len == 0 || len == auth.len) {
        r->headers_in.user.data = (u_char *) "";
        return -5;
    }

    r->headers_in.user.len = len;
    r->headers_in.user.data = auth.data;
    r->headers_in.passwd.len = auth.len - len - 1;
    r->headers_in.passwd.data = &auth.data[len + 1];

    return 0;
}
# 2220 "src/http/ngx_http_core_module.c"
ngx_int_t
ngx_http_subrequest(ngx_http_request_t *r,
    ngx_str_t *uri, ngx_str_t *args, ngx_http_request_t **psr,
    ngx_http_post_subrequest_t *ps, ngx_uint_t flags)
{
    ngx_time_t *tp;
    ngx_connection_t *c;
    ngx_http_request_t *sr;
    ngx_http_core_srv_conf_t *cscf;
    ngx_http_postponed_request_t *pr, *p;

    if (r->subrequests == 0) {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "subrequests cycle while processing \"%V\"", uri)
                                                                       ;
        return -1;
    }




    if (r->main->count >= 65535 - 1000) {
        if ((r->connection->log)->log_level >= 3) ngx_log_error_core(3, r->connection->log, 0, "request reference counter overflow " "while processing \"%V\"", uri)

                                                     ;
        return -1;
    }

    if (r->subrequest_in_memory) {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "nested in-memory subrequest \"%V\"", uri)
                                                                ;
        return -1;
    }

    sr = ngx_pcalloc(r->pool, sizeof(ngx_http_request_t));
    if (sr == 0) {
        return -1;
    }

    sr->signature = 0x50545448;

    c = r->connection;
    sr->connection = c;

    sr->ctx = ngx_pcalloc(r->pool, sizeof(void *) * ngx_http_max_module);
    if (sr->ctx == 0) {
        return -1;
    }

    if (ngx_list_init(&sr->headers_out.headers, r->pool, 20,
                      sizeof(ngx_table_elt_t))
        != 0)
    {
        return -1;
    }

    if (ngx_list_init(&sr->headers_out.trailers, r->pool, 4,
                      sizeof(ngx_table_elt_t))
        != 0)
    {
        return -1;
    }

    cscf = (r)->srv_conf[ngx_http_core_module.ctx_index];
    sr->main_conf = cscf->ctx->main_conf;
    sr->srv_conf = cscf->ctx->srv_conf;
    sr->loc_conf = cscf->ctx->loc_conf;

    sr->pool = r->pool;

    sr->headers_in = r->headers_in;

    sr->headers_out.content_length_n = -1; if (sr->headers_out.content_length) { sr->headers_out.content_length->hash = 0; sr->headers_out.content_length = 0; };
    sr->allow_ranges = 0; if (sr->headers_out.accept_ranges) { sr->headers_out.accept_ranges->hash = 0; sr->headers_out.accept_ranges = 0; };
    sr->headers_out.last_modified_time = -1; if (sr->headers_out.last_modified) { sr->headers_out.last_modified->hash = 0; sr->headers_out.last_modified = 0; };

    sr->request_body = r->request_body;





    sr->method = 0x0002;
    sr->http_version = r->http_version;

    sr->request_line = r->request_line;
    sr->uri = *uri;

    if (args) {
        sr->args = *args;
    }

   
                                                               ;

    sr->subrequest_in_memory = (flags & 2) != 0;
    sr->waited = (flags & 4) != 0;
    sr->background = (flags & 16) != 0;

    sr->unparsed_uri = r->unparsed_uri;
    sr->method_name = ngx_http_core_get_method;
    sr->http_protocol = r->http_protocol;
    sr->schema = r->schema;

    ngx_http_set_exten(sr);

    sr->main = r->main;
    sr->parent = r;
    sr->post_subrequest = ps;
    sr->read_event_handler = ngx_http_request_empty_handler;
    sr->write_event_handler = ngx_http_handler;

    sr->variables = r->variables;

    sr->log_handler = r->log_handler;

    if (sr->subrequest_in_memory) {
        sr->filter_need_in_memory = 1;
    }

    if (!sr->background) {
        if (c->data == r && r->postponed == 0) {
            c->data = sr;
        }

        pr = ngx_palloc(r->pool, sizeof(ngx_http_postponed_request_t));
        if (pr == 0) {
            return -1;
        }

        pr->request = sr;
        pr->out = 0;
        pr->next = 0;

        if (r->postponed) {
            for (p = r->postponed; p->next; p = p->next) { }
            p->next = pr;

        } else {
            r->postponed = pr;
        }
    }

    sr->internal = 1;

    sr->discard_body = r->discard_body;
    sr->expect_tested = 1;
    sr->main_filter_need_in_memory = r->main_filter_need_in_memory;

    sr->uri_changes = 10 + 1;
    sr->subrequests = r->subrequests - 1;

    tp = (ngx_time_t *) ngx_cached_time;
    sr->start_sec = tp->sec;
    sr->start_msec = tp->msec;

    r->main->count++;

    *psr = sr;

    if (flags & 8) {
        sr->method = r->method;
        sr->method_name = r->method_name;
        sr->loc_conf = r->loc_conf;
        sr->valid_location = r->valid_location;
        sr->valid_unparsed_uri = r->valid_unparsed_uri;
        sr->content_handler = r->content_handler;
        sr->phase_handler = r->phase_handler;
        sr->write_event_handler = ngx_http_core_run_phases;

        ngx_http_update_location_config(sr);
    }

    return ngx_http_post_request(sr, 0);
}


ngx_int_t
ngx_http_internal_redirect(ngx_http_request_t *r,
    ngx_str_t *uri, ngx_str_t *args)
{
    ngx_http_core_srv_conf_t *cscf;

    r->uri_changes--;

    if (r->uri_changes == 0) {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "rewrite or internal redirection cycle " "while internally redirecting to \"%V\"", uri)

                                                                    ;

        r->main->count++;
        ngx_http_finalize_request(r, 500);
        return -4;
    }

    r->uri = *uri;

    if (args) {
        r->args = *args;

    } else {
        (&r->args)->len = 0; (&r->args)->data = 0;
    }

   
                                                                 ;

    ngx_http_set_exten(r);


    (void) memset(r->ctx, 0, sizeof(void *) * ngx_http_max_module);

    cscf = (r)->srv_conf[ngx_http_core_module.ctx_index];
    r->loc_conf = cscf->ctx->loc_conf;

    ngx_http_update_location_config(r);


    r->cache = 0;


    r->internal = 1;
    r->valid_unparsed_uri = 0;
    r->add_uri_to_alias = 0;
    r->main->count++;

    ngx_http_handler(r);

    return -4;
}


ngx_int_t
ngx_http_named_location(ngx_http_request_t *r, ngx_str_t *name)
{
    ngx_http_core_srv_conf_t *cscf;
    ngx_http_core_loc_conf_t **clcfp;
    ngx_http_core_main_conf_t *cmcf;

    r->main->count++;
    r->uri_changes--;

    if (r->uri_changes == 0) {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "rewrite or internal redirection cycle " "while redirect to named location \"%V\"", name)

                                                                      ;

        ngx_http_finalize_request(r, 500);
        return -4;
    }

    if (r->uri.len == 0) {
        if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "empty URI in redirect to named location \"%V\"", name)
                                                                             ;

        ngx_http_finalize_request(r, 500);
        return -4;
    }

    cscf = (r)->srv_conf[ngx_http_core_module.ctx_index];

    if (cscf->named_locations) {

        for (clcfp = cscf->named_locations; *clcfp; clcfp++) {

           
                                                                    ;

            if (name->len != (*clcfp)->name.len
                || strncmp((const char *) name->data, (const char *) (*clcfp)->name.data, name->len) != 0)
            {
                continue;
            }

           

                                                   ;

            r->internal = 1;
            r->content_handler = 0;
            r->uri_changed = 0;
            r->loc_conf = (*clcfp)->loc_conf;


            (void) memset(r->ctx, 0, sizeof(void *) * ngx_http_max_module);

            ngx_http_update_location_config(r);

            cmcf = (r)->main_conf[ngx_http_core_module.ctx_index];

            r->phase_handler = cmcf->phase_engine.location_rewrite_index;

            r->write_event_handler = ngx_http_core_run_phases;
            ngx_http_core_run_phases(r);

            return -4;
        }
    }

    if ((r->connection->log)->log_level >= 4) ngx_log_error_core(4, r->connection->log, 0, "could not find named location \"%V\"", name)
                                                               ;

    ngx_http_finalize_request(r, 500);

    return -4;
}


ngx_http_cleanup_t *
ngx_http_cleanup_add(ngx_http_request_t *r, size_t size)
{
    ngx_http_cleanup_t *cln;

    r = r->main;

    cln = ngx_palloc(r->pool, sizeof(ngx_http_cleanup_t));
    if (cln == 0) {
        return 0;
    }

    if (size) {
        cln->data = ngx_palloc(r->pool, size);
        if (cln->data == 0) {
            return 0;
        }

    } else {
        cln->data = 0;
    }

    cln->handler = 0;
    cln->next = r->cleanup;

    r->cleanup = cln;

   
                                               ;

    return cln;
}


ngx_int_t
ngx_http_set_disable_symlinks(ngx_http_request_t *r,
    ngx_http_core_loc_conf_t *clcf, ngx_str_t *path, ngx_open_file_info_t *of)
{

    u_char *p;
    ngx_str_t from;

    of->disable_symlinks = clcf->disable_symlinks;

    if (clcf->disable_symlinks_from == 0) {
        return 0;
    }

    if (ngx_http_complex_value(r, clcf->disable_symlinks_from, &from)
        != 0)
    {
        return -1;
    }

    if (from.len == 0
        || from.len > path->len
        || memcmp((const char *) path->data, (const char *) from.data, from.len) != 0)
    {
        return 0;
    }

    if (from.len == path->len) {
        of->disable_symlinks = 0;
        return 0;
    }

    p = path->data + from.len;

    if (*p == '/') {
        of->disable_symlinks_from = from.len;
        return 0;
    }

    p--;

    if (*p == '/') {
        of->disable_symlinks_from = from.len - 1;
    }


    return 0;
}


ngx_int_t
ngx_http_get_forwarded_addr(ngx_http_request_t *r, ngx_addr_t *addr,
    ngx_array_t *headers, ngx_str_t *value, ngx_array_t *proxies,
    int recursive)
{
    ngx_int_t rc;
    ngx_uint_t i, found;
    ngx_table_elt_t **h;

    if (headers == 0) {
        return ngx_http_get_forwarded_addr_internal(r, addr, value->data,
                                                    value->len, proxies,
                                                    recursive);
    }

    i = headers->nelts;
    h = headers->elts;

    rc = -5;

    found = 0;

    while (i-- > 0) {
        rc = ngx_http_get_forwarded_addr_internal(r, addr, h[i]->value.data,
                                                  h[i]->value.len, proxies,
                                                  recursive);

        if (!recursive) {
            break;
        }

        if (rc == -5 && found) {
            rc = -4;
            break;
        }

        if (rc != 0) {
            break;
        }

        found = 1;
    }

    return rc;
}


static ngx_int_t
ngx_http_get_forwarded_addr_internal(ngx_http_request_t *r, ngx_addr_t *addr,
    u_char *xff, size_t xfflen, ngx_array_t *proxies, int recursive)
{
    u_char *p;
    ngx_int_t rc;
    ngx_addr_t paddr;

    if (ngx_cidr_match(addr->sockaddr, proxies) != 0) {
        return -5;
    }

    for (p = xff + xfflen - 1; p > xff; p--, xfflen--) {
        if (*p != ' ' && *p != ',') {
            break;
        }
    }

    for ( ; p > xff; p--) {
        if (*p == ' ' || *p == ',') {
            p++;
            break;
        }
    }

    if (ngx_parse_addr_port(r->pool, &paddr, p, xfflen - (p - xff)) != 0) {
        return -5;
    }

    *addr = paddr;

    if (recursive && p > xff) {
        rc = ngx_http_get_forwarded_addr_internal(r, addr, xff, p - 1 - xff,
                                                  proxies, 1);

        if (rc == -5) {
            return -4;
        }


        return rc;
    }

    return 0;
}


static char *
ngx_http_core_server(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy)
{
    char *rv;
    void *mconf;
    ngx_uint_t i;
    ngx_conf_t pcf;
    ngx_http_module_t *module;
    struct sockaddr_in *sin;
    ngx_http_conf_ctx_t *ctx, *http_ctx;
    ngx_http_listen_opt_t lsopt;
    ngx_http_core_srv_conf_t *cscf, **cscfp;
    ngx_http_core_main_conf_t *cmcf;

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



    ctx->loc_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->loc_conf == 0) {
        return (void *) -1;
    }

    for (i = 0; cf->cycle->modules[i]; i++) {
        if (cf->cycle->modules[i]->type != 0x50545448) {
            continue;
        }

        module = cf->cycle->modules[i]->ctx;

        if (module->create_srv_conf) {
            mconf = module->create_srv_conf(cf);
            if (mconf == 0) {
                return (void *) -1;
            }

            ctx->srv_conf[cf->cycle->modules[i]->ctx_index] = mconf;
        }

        if (module->create_loc_conf) {
            mconf = module->create_loc_conf(cf);
            if (mconf == 0) {
                return (void *) -1;
            }

            ctx->loc_conf[cf->cycle->modules[i]->ctx_index] = mconf;
        }
    }




    cscf = ctx->srv_conf[ngx_http_core_module.ctx_index];
    cscf->ctx = ctx;


    cmcf = ctx->main_conf[ngx_http_core_module.ctx_index];

    cscfp = ngx_array_push(&cmcf->servers);
    if (cscfp == 0) {
        return (void *) -1;
    }

    *cscfp = cscf;




    pcf = *cf;
    cf->ctx = ctx;
    cf->cmd_type = 0x04000000;

    rv = ngx_conf_parse(cf, 0);

    *cf = pcf;

    if (rv == 0 && !cscf->listen) {
        (void) memset(&lsopt, 0, sizeof(ngx_http_listen_opt_t));

        sin = &lsopt.sockaddr.sockaddr_in;

        sin->sin_family = AF_INET;



        sin->sin_port = htons((getuid() == 0) ? 80 : 8000);

        sin->sin_addr.s_addr = INADDR_ANY;

        lsopt.socklen = sizeof(struct sockaddr_in);

        lsopt.backlog = 511;
        lsopt.rcvbuf = -1;
        lsopt.sndbuf = -1;




        lsopt.fastopen = -1;

        lsopt.wildcard = 1;

        (void) ngx_sock_ntop(&lsopt.sockaddr.sockaddr, lsopt.socklen,
                             lsopt.addr, (sizeof("unix:") - 1 + sizeof(struct sockaddr_un) - 
# 2821 "src/http/ngx_http_core_module.c" 3 4
                                        0
# 2821 "src/http/ngx_http_core_module.c"
                                        ), 1);

        if (ngx_http_add_listen(cf, cscf, &lsopt) != 0) {
            return (void *) -1;
        }
    }

    return rv;
}


static char *
ngx_http_core_location(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy)
{
    char *rv;
    u_char *mod;
    size_t len;
    ngx_str_t *value, *name;
    ngx_uint_t i;
    ngx_conf_t save;
    ngx_http_module_t *module;
    ngx_http_conf_ctx_t *ctx, *pctx;
    ngx_http_core_loc_conf_t *clcf, *pclcf;

    ctx = ngx_pcalloc(cf->pool, sizeof(ngx_http_conf_ctx_t));
    if (ctx == 0) {
        return (void *) -1;
    }

    pctx = cf->ctx;
    ctx->main_conf = pctx->main_conf;
    ctx->srv_conf = pctx->srv_conf;

    ctx->loc_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->loc_conf == 0) {
        return (void *) -1;
    }

    for (i = 0; cf->cycle->modules[i]; i++) {
        if (cf->cycle->modules[i]->type != 0x50545448) {
            continue;
        }

        module = cf->cycle->modules[i]->ctx;

        if (module->create_loc_conf) {
            ctx->loc_conf[cf->cycle->modules[i]->ctx_index] =
                                                   module->create_loc_conf(cf);
            if (ctx->loc_conf[cf->cycle->modules[i]->ctx_index] == 0) {
                return (void *) -1;
            }
        }
    }

    clcf = ctx->loc_conf[ngx_http_core_module.ctx_index];
    clcf->loc_conf = ctx->loc_conf;

    value = cf->args->elts;

    if (cf->args->nelts == 3) {

        len = value[1].len;
        mod = value[1].data;
        name = &value[2];

        if (len == 1 && mod[0] == '=') {

            clcf->name = *name;
            clcf->exact_match = 1;

        } else if (len == 2 && mod[0] == '^' && mod[1] == '~') {

            clcf->name = *name;
            clcf->noregex = 1;

        } else if (len == 1 && mod[0] == '~') {

            if (ngx_http_core_regex_location(cf, clcf, name, 0) != 0) {
                return (void *) -1;
            }

        } else if (len == 2 && mod[0] == '~' && mod[1] == '*') {

            if (ngx_http_core_regex_location(cf, clcf, name, 1) != 0) {
                return (void *) -1;
            }

        } else {
            ngx_conf_log_error(1, cf, 0,
                               "invalid location modifier \"%V\"", &value[1]);
            return (void *) -1;
        }

    } else {

        name = &value[1];

        if (name->data[0] == '=') {

            clcf->name.len = name->len - 1;
            clcf->name.data = name->data + 1;
            clcf->exact_match = 1;

        } else if (name->data[0] == '^' && name->data[1] == '~') {

            clcf->name.len = name->len - 2;
            clcf->name.data = name->data + 2;
            clcf->noregex = 1;

        } else if (name->data[0] == '~') {

            name->len--;
            name->data++;

            if (name->data[0] == '*') {

                name->len--;
                name->data++;

                if (ngx_http_core_regex_location(cf, clcf, name, 1) != 0) {
                    return (void *) -1;
                }

            } else {
                if (ngx_http_core_regex_location(cf, clcf, name, 0) != 0) {
                    return (void *) -1;
                }
            }

        } else {

            clcf->name = *name;

            if (name->data[0] == '@') {
                clcf->named = 1;
            }
        }
    }

    pclcf = pctx->loc_conf[ngx_http_core_module.ctx_index];

    if (cf->cmd_type == 0x08000000) {







        if (pclcf->exact_match) {
            ngx_conf_log_error(1, cf, 0,
                               "location \"%V\" cannot be inside "
                               "the exact location \"%V\"",
                               &clcf->name, &pclcf->name);
            return (void *) -1;
        }

        if (pclcf->named) {
            ngx_conf_log_error(1, cf, 0,
                               "location \"%V\" cannot be inside "
                               "the named location \"%V\"",
                               &clcf->name, &pclcf->name);
            return (void *) -1;
        }

        if (clcf->named) {
            ngx_conf_log_error(1, cf, 0,
                               "named location \"%V\" can be "
                               "on the server level only",
                               &clcf->name);
            return (void *) -1;
        }

        len = pclcf->name.len;





        if (ngx_filename_cmp(clcf->name.data, pclcf->name.data, len) != 0)

        {
            ngx_conf_log_error(1, cf, 0,
                               "location \"%V\" is outside location \"%V\"",
                               &clcf->name, &pclcf->name);
            return (void *) -1;
        }
    }

    if (ngx_http_add_location(cf, &pclcf->locations, clcf) != 0) {
        return (void *) -1;
    }

    save = *cf;
    cf->ctx = ctx;
    cf->cmd_type = 0x08000000;

    rv = ngx_conf_parse(cf, 0);

    *cf = save;

    return rv;
}


static ngx_int_t
ngx_http_core_regex_location(ngx_conf_t *cf, ngx_http_core_loc_conf_t *clcf,
    ngx_str_t *regex, ngx_uint_t caseless)
{
# 3057 "src/http/ngx_http_core_module.c"
    ngx_conf_log_error(1, cf, 0,
                       "using regex \"%V\" requires PCRE library",
                       regex);
    return -1;


}


static char *
ngx_http_core_types(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    char *rv;
    ngx_conf_t save;

    if (clcf->types == 0) {
        clcf->types = ngx_array_create(cf->pool, 64, sizeof(ngx_hash_key_t));
        if (clcf->types == 0) {
            return (void *) -1;
        }
    }

    save = *cf;
    cf->handler = ngx_http_core_type;
    cf->handler_conf = conf;

    rv = ngx_conf_parse(cf, 0);

    *cf = save;

    return rv;
}


static char *
ngx_http_core_type(ngx_conf_t *cf, ngx_command_t *dummy, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    ngx_str_t *value, *content_type, *old;
    ngx_uint_t i, n, hash;
    ngx_hash_key_t *type;

    value = cf->args->elts;

    if (strcmp((const char *) value[0].data, (const char *) "include") == 0) {
        if (cf->args->nelts != 2) {
            ngx_conf_log_error(1, cf, 0,
                               "invalid number of arguments"
                               " in \"include\" directive");
            return (void *) -1;
        }

        return ngx_conf_include(cf, dummy, conf);
    }

    content_type = ngx_palloc(cf->pool, sizeof(ngx_str_t));
    if (content_type == 0) {
        return (void *) -1;
    }

    *content_type = value[0];

    for (i = 1; i < cf->args->nelts; i++) {

        hash = ngx_hash_strlow(value[i].data, value[i].data, value[i].len);

        type = clcf->types->elts;
        for (n = 0; n < clcf->types->nelts; n++) {
            if (strcmp((const char *) value[i].data, (const char *) type[n].key.data) == 0) {
                old = type[n].value;
                type[n].value = content_type;

                ngx_conf_log_error(5, cf, 0,
                                   "duplicate extension \"%V\", "
                                   "content type: \"%V\", "
                                   "previous content type: \"%V\"",
                                   &value[i], content_type, old);
                goto next;
            }
        }


        type = ngx_array_push(clcf->types);
        if (type == 0) {
            return (void *) -1;
        }

        type->key = value[i];
        type->key_hash = hash;
        type->value = content_type;

    next:
        continue;
    }

    return 0;
}


static ngx_int_t
ngx_http_core_preconfiguration(ngx_conf_t *cf)
{
    return ngx_http_variables_add_core_vars(cf);
}


static ngx_int_t
ngx_http_core_postconfiguration(ngx_conf_t *cf)
{
    ngx_http_top_request_body_filter = ngx_http_request_body_save_filter;

    return 0;
}


static void *
ngx_http_core_create_main_conf(ngx_conf_t *cf)
{
    ngx_http_core_main_conf_t *cmcf;

    cmcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_core_main_conf_t));
    if (cmcf == 0) {
        return 0;
    }

    if (ngx_array_init(&cmcf->servers, cf->pool, 4,
                       sizeof(ngx_http_core_srv_conf_t *))
        != 0)
    {
        return 0;
    }

    cmcf->server_names_hash_max_size = (ngx_uint_t) -1;
    cmcf->server_names_hash_bucket_size = (ngx_uint_t) -1;

    cmcf->variables_hash_max_size = (ngx_uint_t) -1;
    cmcf->variables_hash_bucket_size = (ngx_uint_t) -1;

    return cmcf;
}


static char *
ngx_http_core_init_main_conf(ngx_conf_t *cf, void *conf)
{
    ngx_http_core_main_conf_t *cmcf = conf;

    if (cmcf->server_names_hash_max_size == (ngx_uint_t) -1) { cmcf->server_names_hash_max_size = 512; };
    if (cmcf->server_names_hash_bucket_size == (ngx_uint_t) -1) { cmcf->server_names_hash_bucket_size = ngx_cacheline_size; }
                                                ;

    cmcf->server_names_hash_bucket_size =
            (((cmcf->server_names_hash_bucket_size) + (ngx_cacheline_size - 1)) & ~(ngx_cacheline_size - 1));


    if (cmcf->variables_hash_max_size == (ngx_uint_t) -1) { cmcf->variables_hash_max_size = 1024; };
    if (cmcf->variables_hash_bucket_size == (ngx_uint_t) -1) { cmcf->variables_hash_bucket_size = 64; };

    cmcf->variables_hash_bucket_size =
               (((cmcf->variables_hash_bucket_size) + (ngx_cacheline_size - 1)) & ~(ngx_cacheline_size - 1));

    if (cmcf->ncaptures) {
        cmcf->ncaptures = (cmcf->ncaptures + 1) * 3;
    }

    return 0;
}


static void *
ngx_http_core_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_core_srv_conf_t *cscf;

    cscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_core_srv_conf_t));
    if (cscf == 0) {
        return 0;
    }







    if (ngx_array_init(&cscf->server_names, cf->temp_pool, 4,
                       sizeof(ngx_http_server_name_t))
        != 0)
    {
        return 0;
    }

    cscf->connection_pool_size = (size_t) -1;
    cscf->request_pool_size = (size_t) -1;
    cscf->client_header_timeout = (ngx_msec_t) -1;
    cscf->client_header_buffer_size = (size_t) -1;
    cscf->ignore_invalid_headers = -1;
    cscf->merge_slashes = -1;
    cscf->underscores_in_headers = -1;

    cscf->file_name = cf->conf_file->file.name.data;
    cscf->line = cf->conf_file->line;

    return cscf;
}


static char *
ngx_http_core_merge_srv_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_core_srv_conf_t *prev = parent;
    ngx_http_core_srv_conf_t *conf = child;

    ngx_str_t name;
    ngx_http_server_name_t *sn;



    if (conf->connection_pool_size == (size_t) -1) { conf->connection_pool_size = (prev->connection_pool_size == (size_t) -1) ? 64 * sizeof(void *) : prev->connection_pool_size; }
                                                                              ;
    if (conf->request_pool_size == (size_t) -1) { conf->request_pool_size = (prev->request_pool_size == (size_t) -1) ? 4096 : prev->request_pool_size; }
                                                            ;
    if (conf->client_header_timeout == (ngx_msec_t) -1) { conf->client_header_timeout = (prev->client_header_timeout == (ngx_msec_t) -1) ? 60000 : prev->client_header_timeout; }
                                                                 ;
    if (conf->client_header_buffer_size == (size_t) -1) { conf->client_header_buffer_size = (prev->client_header_buffer_size == (size_t) -1) ? 1024 : prev->client_header_buffer_size; }
                                                                    ;
    if (conf->large_client_header_buffers.num == 0) { if (prev->large_client_header_buffers.num) { conf->large_client_header_buffers.num = prev->large_client_header_buffers.num; conf->large_client_header_buffers.size = prev->large_client_header_buffers.size; } else { conf->large_client_header_buffers.num = 4; conf->large_client_header_buffers.size = 8192; } }

                                      ;

    if (conf->large_client_header_buffers.size < conf->connection_pool_size) {
        ngx_conf_log_error(1, cf, 0,
                           "the \"large_client_header_buffers\" size must be "
                           "equal to or greater than \"connection_pool_size\"");
        return (void *) -1;
    }

    if (conf->ignore_invalid_headers == -1) { conf->ignore_invalid_headers = (prev->ignore_invalid_headers == -1) ? 1 : prev->ignore_invalid_headers; }
                                                              ;

    if (conf->merge_slashes == -1) { conf->merge_slashes = (prev->merge_slashes == -1) ? 1 : prev->merge_slashes; };

    if (conf->underscores_in_headers == -1) { conf->underscores_in_headers = (prev->underscores_in_headers == -1) ? 0 : prev->underscores_in_headers; }
                                                              ;

    if (conf->server_names.nelts == 0) {

        sn = ngx_array_push(&conf->server_names);



        sn->server = conf;
        (&sn->name)->len = sizeof("") - 1; (&sn->name)->data = (u_char *) "";
    }

    sn = conf->server_names.elts;
    name = sn[0].name;
# 3325 "src/http/ngx_http_core_module.c"
    if (name.data[0] == '.') {
        name.len--;
        name.data++;
    }

    conf->server_name.len = name.len;
    conf->server_name.data = ngx_pstrdup(cf->pool, &name);
    if (conf->server_name.data == 0) {
        return (void *) -1;
    }

    return 0;
}


static void *
ngx_http_core_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_core_loc_conf_t *clcf;

    clcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_core_loc_conf_t));
    if (clcf == 0) {
        return 0;
    }
# 3369 "src/http/ngx_http_core_module.c"
    clcf->client_max_body_size = -1;
    clcf->client_body_buffer_size = (size_t) -1;
    clcf->client_body_timeout = (ngx_msec_t) -1;
    clcf->satisfy = (ngx_uint_t) -1;
    clcf->if_modified_since = (ngx_uint_t) -1;
    clcf->max_ranges = (ngx_uint_t) -1;
    clcf->client_body_in_file_only = (ngx_uint_t) -1;
    clcf->client_body_in_single_buffer = -1;
    clcf->internal = -1;
    clcf->sendfile = -1;
    clcf->sendfile_max_chunk = (size_t) -1;
    clcf->subrequest_output_buffer_size = (size_t) -1;
    clcf->aio = -1;
    clcf->aio_write = -1;




    clcf->read_ahead = (size_t) -1;
    clcf->directio = -1;
    clcf->directio_alignment = -1;
    clcf->tcp_nopush = -1;
    clcf->tcp_nodelay = -1;
    clcf->send_timeout = (ngx_msec_t) -1;
    clcf->send_lowat = (size_t) -1;
    clcf->postpone_output = (size_t) -1;
    clcf->limit_rate = (size_t) -1;
    clcf->limit_rate_after = (size_t) -1;
    clcf->keepalive_timeout = (ngx_msec_t) -1;
    clcf->keepalive_header = -1;
    clcf->keepalive_requests = (ngx_uint_t) -1;
    clcf->lingering_close = (ngx_uint_t) -1;
    clcf->lingering_time = (ngx_msec_t) -1;
    clcf->lingering_timeout = (ngx_msec_t) -1;
    clcf->resolver_timeout = (ngx_msec_t) -1;
    clcf->reset_timedout_connection = -1;
    clcf->absolute_redirect = -1;
    clcf->server_name_in_redirect = -1;
    clcf->port_in_redirect = -1;
    clcf->msie_padding = -1;
    clcf->msie_refresh = -1;
    clcf->log_not_found = -1;
    clcf->log_subrequest = -1;
    clcf->recursive_error_pages = -1;
    clcf->chunked_transfer_encoding = -1;
    clcf->etag = -1;
    clcf->server_tokens = (ngx_uint_t) -1;
    clcf->types_hash_max_size = (ngx_uint_t) -1;
    clcf->types_hash_bucket_size = (ngx_uint_t) -1;

    clcf->open_file_cache = (void *) -1;
    clcf->open_file_cache_valid = -1;
    clcf->open_file_cache_min_uses = (ngx_uint_t) -1;
    clcf->open_file_cache_errors = -1;
    clcf->open_file_cache_events = -1;
# 3438 "src/http/ngx_http_core_module.c"
    clcf->disable_symlinks = (ngx_uint_t) -1;
    clcf->disable_symlinks_from = (void *) -1;


    return clcf;
}


static ngx_str_t ngx_http_core_text_html_type = { sizeof("text/html") - 1, (u_char *) "text/html" };
static ngx_str_t ngx_http_core_image_gif_type = { sizeof("image/gif") - 1, (u_char *) "image/gif" };
static ngx_str_t ngx_http_core_image_jpeg_type = { sizeof("image/jpeg") - 1, (u_char *) "image/jpeg" };

static ngx_hash_key_t ngx_http_core_default_types[] = {
    { { sizeof("html") - 1, (u_char *) "html" }, 0, &ngx_http_core_text_html_type },
    { { sizeof("gif") - 1, (u_char *) "gif" }, 0, &ngx_http_core_image_gif_type },
    { { sizeof("jpg") - 1, (u_char *) "jpg" }, 0, &ngx_http_core_image_jpeg_type },
    { { 0, 0 }, 0, 0 }
};


static char *
ngx_http_core_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_core_loc_conf_t *prev = parent;
    ngx_http_core_loc_conf_t *conf = child;

    ngx_uint_t i;
    ngx_hash_key_t *type;
    ngx_hash_init_t types_hash;

    if (conf->root.data == 0) {

        conf->alias = prev->alias;
        conf->root = prev->root;
        conf->root_lengths = prev->root_lengths;
        conf->root_values = prev->root_values;

        if (prev->root.data == 0) {
            (&conf->root)->len = sizeof("html") - 1; (&conf->root)->data = (u_char *) "html";

            if (ngx_conf_full_name(cf->cycle, &conf->root, 0) != 0) {
                return (void *) -1;
            }
        }
    }

    if (conf->post_action.data == 0) {
        conf->post_action = prev->post_action;
    }

    if (conf->types_hash_max_size == (ngx_uint_t) -1) { conf->types_hash_max_size = (prev->types_hash_max_size == (ngx_uint_t) -1) ? 1024 : prev->types_hash_max_size; }
                                                              ;

    if (conf->types_hash_bucket_size == (ngx_uint_t) -1) { conf->types_hash_bucket_size = (prev->types_hash_bucket_size == (ngx_uint_t) -1) ? 64 : prev->types_hash_bucket_size; }
                                                               ;

    conf->types_hash_bucket_size = (((conf->types_hash_bucket_size) + (ngx_cacheline_size - 1)) & ~(ngx_cacheline_size - 1))
                                                                ;






    if (prev->types && prev->types_hash.buckets == 0) {

        types_hash.hash = &prev->types_hash;
        types_hash.key = ngx_hash_key_lc;
        types_hash.max_size = conf->types_hash_max_size;
        types_hash.bucket_size = conf->types_hash_bucket_size;
        types_hash.name = "types_hash";
        types_hash.pool = cf->pool;
        types_hash.temp_pool = 0;

        if (ngx_hash_init(&types_hash, prev->types->elts, prev->types->nelts)
            != 0)
        {
            return (void *) -1;
        }
    }

    if (conf->types == 0) {
        conf->types = prev->types;
        conf->types_hash = prev->types_hash;
    }

    if (conf->types == 0) {
        conf->types = ngx_array_create(cf->pool, 3, sizeof(ngx_hash_key_t));
        if (conf->types == 0) {
            return (void *) -1;
        }

        for (i = 0; ngx_http_core_default_types[i].key.len; i++) {
            type = ngx_array_push(conf->types);
            if (type == 0) {
                return (void *) -1;
            }

            type->key = ngx_http_core_default_types[i].key;
            type->key_hash =
                       ngx_hash_key_lc(ngx_http_core_default_types[i].key.data,
                                       ngx_http_core_default_types[i].key.len);
            type->value = ngx_http_core_default_types[i].value;
        }
    }

    if (conf->types_hash.buckets == 0) {

        types_hash.hash = &conf->types_hash;
        types_hash.key = ngx_hash_key_lc;
        types_hash.max_size = conf->types_hash_max_size;
        types_hash.bucket_size = conf->types_hash_bucket_size;
        types_hash.name = "types_hash";
        types_hash.pool = cf->pool;
        types_hash.temp_pool = 0;

        if (ngx_hash_init(&types_hash, conf->types->elts, conf->types->nelts)
            != 0)
        {
            return (void *) -1;
        }
    }

    if (conf->error_log == 0) {
        if (prev->error_log) {
            conf->error_log = prev->error_log;
        } else {
            conf->error_log = &cf->cycle->new_log;
        }
    }

    if (conf->error_pages == 0 && prev->error_pages) {
        conf->error_pages = prev->error_pages;
    }

    if (conf->default_type.data == 0) { if (prev->default_type.data) { conf->default_type.len = prev->default_type.len; conf->default_type.data = prev->default_type.data; } else { conf->default_type.len = sizeof("text/plain") - 1; conf->default_type.data = (u_char *) "text/plain"; } }
                                                               ;

    if (conf->client_max_body_size == -1) { conf->client_max_body_size = (prev->client_max_body_size == -1) ? 1 * 1024 * 1024 : prev->client_max_body_size; }
                                                                          ;
    if (conf->client_body_buffer_size == (size_t) -1) { conf->client_body_buffer_size = (prev->client_body_buffer_size == (size_t) -1) ? (size_t) 2 * ngx_pagesize : prev->client_body_buffer_size; }

                                                        ;
    if (conf->client_body_timeout == (ngx_msec_t) -1) { conf->client_body_timeout = (prev->client_body_timeout == (ngx_msec_t) -1) ? 60000 : prev->client_body_timeout; }
                                                               ;

    if (conf->keepalive_disable == 0) { conf->keepalive_disable = (prev->keepalive_disable == 0) ? (1 |0x0004) : prev->keepalive_disable; }


                                                                  ;
    if (conf->satisfy == (ngx_uint_t) -1) { conf->satisfy = (prev->satisfy == (ngx_uint_t) -1) ? 0 : prev->satisfy; }
                                                   ;
    if (conf->if_modified_since == (ngx_uint_t) -1) { conf->if_modified_since = (prev->if_modified_since == (ngx_uint_t) -1) ? 1 : prev->if_modified_since; }
                                                 ;
    if (conf->max_ranges == (ngx_uint_t) -1) { conf->max_ranges = (prev->max_ranges == (ngx_uint_t) -1) ? (uint32_t) 0x7fffffff : prev->max_ranges; }
                                                  ;
    if (conf->client_body_in_file_only == (ngx_uint_t) -1) { conf->client_body_in_file_only = (prev->client_body_in_file_only == (ngx_uint_t) -1) ? 0 : prev->client_body_in_file_only; }

                                                             ;
    if (conf->client_body_in_single_buffer == -1) { conf->client_body_in_single_buffer = (prev->client_body_in_single_buffer == -1) ? 0 : prev->client_body_in_single_buffer; }
                                                                    ;
    if (conf->internal == -1) { conf->internal = (prev->internal == -1) ? 0 : prev->internal; };
    if (conf->sendfile == -1) { conf->sendfile = (prev->sendfile == -1) ? 0 : prev->sendfile; };
    if (conf->sendfile_max_chunk == (size_t) -1) { conf->sendfile_max_chunk = (prev->sendfile_max_chunk == (size_t) -1) ? 0 : prev->sendfile_max_chunk; }
                                                          ;
    if (conf->subrequest_output_buffer_size == (size_t) -1) { conf->subrequest_output_buffer_size = (prev->subrequest_output_buffer_size == (size_t) -1) ? (size_t) ngx_pagesize : prev->subrequest_output_buffer_size; }

                                                    ;
    if (conf->aio == -1) { conf->aio = (prev->aio == -1) ? 0 : prev->aio; };
    if (conf->aio_write == -1) { conf->aio_write = (prev->aio_write == -1) ? 0 : prev->aio_write; };





    if (conf->read_ahead == (size_t) -1) { conf->read_ahead = (prev->read_ahead == (size_t) -1) ? 0 : prev->read_ahead; };
    if (conf->directio == -1) { conf->directio = (prev->directio == -1) ? 9223372036854775807LL : prev->directio; }
                                                         ;
    if (conf->directio_alignment == -1) { conf->directio_alignment = (prev->directio_alignment == -1) ? 512 : prev->directio_alignment; }
                                  ;
    if (conf->tcp_nopush == -1) { conf->tcp_nopush = (prev->tcp_nopush == -1) ? 0 : prev->tcp_nopush; };
    if (conf->tcp_nodelay == -1) { conf->tcp_nodelay = (prev->tcp_nodelay == -1) ? 1 : prev->tcp_nodelay; };

    if (conf->send_timeout == (ngx_msec_t) -1) { conf->send_timeout = (prev->send_timeout == (ngx_msec_t) -1) ? 60000 : prev->send_timeout; };
    if (conf->send_lowat == (size_t) -1) { conf->send_lowat = (prev->send_lowat == (size_t) -1) ? 0 : prev->send_lowat; };
    if (conf->postpone_output == (size_t) -1) { conf->postpone_output = (prev->postpone_output == (size_t) -1) ? 1460 : prev->postpone_output; }
                                   ;
    if (conf->limit_rate == (size_t) -1) { conf->limit_rate = (prev->limit_rate == (size_t) -1) ? 0 : prev->limit_rate; };
    if (conf->limit_rate_after == (size_t) -1) { conf->limit_rate_after = (prev->limit_rate_after == (size_t) -1) ? 0 : prev->limit_rate_after; }
                                ;
    if (conf->keepalive_timeout == (ngx_msec_t) -1) { conf->keepalive_timeout = (prev->keepalive_timeout == (ngx_msec_t) -1) ? 75000 : prev->keepalive_timeout; }
                                                             ;
    if (conf->keepalive_header == -1) { conf->keepalive_header = (prev->keepalive_header == -1) ? 0 : prev->keepalive_header; }
                                                        ;
    if (conf->keepalive_requests == (ngx_uint_t) -1) { conf->keepalive_requests = (prev->keepalive_requests == (ngx_uint_t) -1) ? 100 : prev->keepalive_requests; }
                                                            ;
    if (conf->lingering_close == (ngx_uint_t) -1) { conf->lingering_close = (prev->lingering_close == (ngx_uint_t) -1) ? 1 : prev->lingering_close; }
                                                                           ;
    if (conf->lingering_time == (ngx_msec_t) -1) { conf->lingering_time = (prev->lingering_time == (ngx_msec_t) -1) ? 30000 : prev->lingering_time; }
                                                          ;
    if (conf->lingering_timeout == (ngx_msec_t) -1) { conf->lingering_timeout = (prev->lingering_timeout == (ngx_msec_t) -1) ? 5000 : prev->lingering_timeout; }
                                                            ;
    if (conf->resolver_timeout == (ngx_msec_t) -1) { conf->resolver_timeout = (prev->resolver_timeout == (ngx_msec_t) -1) ? 30000 : prev->resolver_timeout; }
                                                            ;

    if (conf->resolver == 0) {

        if (prev->resolver == 0) {






            prev->resolver = ngx_resolver_create(cf, 0, 0);
            if (prev->resolver == 0) {
                return (void *) -1;
            }
        }

        conf->resolver = prev->resolver;
    }

    if (ngx_conf_merge_path_value(cf, &conf->client_body_temp_path,
                              prev->client_body_temp_path,
                              &ngx_http_client_temp_path)
        != 0)
    {
        return (void *) -1;
    }

    if (conf->reset_timedout_connection == -1) { conf->reset_timedout_connection = (prev->reset_timedout_connection == -1) ? 0 : prev->reset_timedout_connection; }
                                                                 ;
    if (conf->absolute_redirect == -1) { conf->absolute_redirect = (prev->absolute_redirect == -1) ? 1 : prev->absolute_redirect; }
                                                         ;
    if (conf->server_name_in_redirect == -1) { conf->server_name_in_redirect = (prev->server_name_in_redirect == -1) ? 0 : prev->server_name_in_redirect; }
                                                               ;
    if (conf->port_in_redirect == -1) { conf->port_in_redirect = (prev->port_in_redirect == -1) ? 1 : prev->port_in_redirect; };
    if (conf->msie_padding == -1) { conf->msie_padding = (prev->msie_padding == -1) ? 1 : prev->msie_padding; };
    if (conf->msie_refresh == -1) { conf->msie_refresh = (prev->msie_refresh == -1) ? 0 : prev->msie_refresh; };
    if (conf->log_not_found == -1) { conf->log_not_found = (prev->log_not_found == -1) ? 1 : prev->log_not_found; };
    if (conf->log_subrequest == -1) { conf->log_subrequest = (prev->log_subrequest == -1) ? 0 : prev->log_subrequest; };
    if (conf->recursive_error_pages == -1) { conf->recursive_error_pages = (prev->recursive_error_pages == -1) ? 0 : prev->recursive_error_pages; }
                                                             ;
    if (conf->chunked_transfer_encoding == -1) { conf->chunked_transfer_encoding = (prev->chunked_transfer_encoding == -1) ? 1 : prev->chunked_transfer_encoding; }
                                                                 ;
    if (conf->etag == -1) { conf->etag = (prev->etag == -1) ? 1 : prev->etag; };

    if (conf->server_tokens == (ngx_uint_t) -1) { conf->server_tokens = (prev->server_tokens == (ngx_uint_t) -1) ? 1 : prev->server_tokens; }
                                                        ;

    if (conf->open_file_cache == (void *) -1) { conf->open_file_cache = (prev->open_file_cache == (void *) -1) ? 0 : prev->open_file_cache; }
                                                          ;

    if (conf->open_file_cache_valid == -1) { conf->open_file_cache_valid = (prev->open_file_cache_valid == -1) ? 60 : prev->open_file_cache_valid; }
                                                              ;

    if (conf->open_file_cache_min_uses == (ngx_uint_t) -1) { conf->open_file_cache_min_uses = (prev->open_file_cache_min_uses == (ngx_uint_t) -1) ? 1 : prev->open_file_cache_min_uses; }
                                                                ;

    if (conf->open_file_cache_errors == -1) { conf->open_file_cache_errors = (prev->open_file_cache_errors == -1) ? 0 : prev->open_file_cache_errors; }
                                                              ;

    if (conf->open_file_cache_events == -1) { conf->open_file_cache_events = (prev->open_file_cache_events == -1) ? 0 : prev->open_file_cache_events; }
                                                              ;
# 3732 "src/http/ngx_http_core_module.c"
    if (conf->disable_symlinks == (ngx_uint_t) -1) { conf->disable_symlinks = (prev->disable_symlinks == (ngx_uint_t) -1) ? 0 : prev->disable_symlinks; }
                                                       ;
    if (conf->disable_symlinks_from == (void *) -1) { conf->disable_symlinks_from = (prev->disable_symlinks_from == (void *) -1) ? 0 : prev->disable_symlinks_from; }
                                                               ;


    return 0;
}


static char *
ngx_http_core_listen(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_srv_conf_t *cscf = conf;

    ngx_str_t *value, size;
    ngx_url_t u;
    ngx_uint_t n;
    ngx_http_listen_opt_t lsopt;

    cscf->listen = 1;

    value = cf->args->elts;

    (void) memset(&u, 0, sizeof(ngx_url_t));

    u.url = value[1];
    u.listen = 1;
    u.default_port = 80;

    if (ngx_parse_url(cf->pool, &u) != 0) {
        if (u.err) {
            ngx_conf_log_error(1, cf, 0,
                               "%s in \"%V\" of the \"listen\" directive",
                               u.err, &u.url);
        }

        return (void *) -1;
    }

    (void) memset(&lsopt, 0, sizeof(ngx_http_listen_opt_t));

    (void) memcpy(&lsopt.sockaddr.sockaddr, &u.sockaddr, u.socklen);

    lsopt.socklen = u.socklen;
    lsopt.backlog = 511;
    lsopt.rcvbuf = -1;
    lsopt.sndbuf = -1;




    lsopt.fastopen = -1;

    lsopt.wildcard = u.wildcard;

    lsopt.ipv6only = 1;


    (void) ngx_sock_ntop(&lsopt.sockaddr.sockaddr, lsopt.socklen, lsopt.addr,
                         (sizeof("unix:") - 1 + sizeof(struct sockaddr_un) - 
# 3792 "src/http/ngx_http_core_module.c" 3 4
                        0
# 3792 "src/http/ngx_http_core_module.c"
                        ), 1);

    for (n = 2; n < cf->args->nelts; n++) {

        if (strcmp((const char *) value[n].data, (const char *) "default_server") == 0
            || strcmp((const char *) value[n].data, (const char *) "default") == 0)
        {
            lsopt.default_server = 1;
            continue;
        }

        if (strcmp((const char *) value[n].data, (const char *) "bind") == 0) {
            lsopt.set = 1;
            lsopt.bind = 1;
            continue;
        }
# 3826 "src/http/ngx_http_core_module.c"
        if (strncmp((const char *) value[n].data, (const char *) "fastopen=", 9) == 0) {
            lsopt.fastopen = ngx_atoi(value[n].data + 9, value[n].len - 9);
            lsopt.set = 1;
            lsopt.bind = 1;

            if (lsopt.fastopen == -1) {
                ngx_conf_log_error(1, cf, 0,
                                   "invalid fastopen \"%V\"", &value[n]);
                return (void *) -1;
            }

            continue;
        }


        if (strncmp((const char *) value[n].data, (const char *) "backlog=", 8) == 0) {
            lsopt.backlog = ngx_atoi(value[n].data + 8, value[n].len - 8);
            lsopt.set = 1;
            lsopt.bind = 1;

            if (lsopt.backlog == -1 || lsopt.backlog == 0) {
                ngx_conf_log_error(1, cf, 0,
                                   "invalid backlog \"%V\"", &value[n]);
                return (void *) -1;
            }

            continue;
        }

        if (strncmp((const char *) value[n].data, (const char *) "rcvbuf=", 7) == 0) {
            size.len = value[n].len - 7;
            size.data = value[n].data + 7;

            lsopt.rcvbuf = ngx_parse_size(&size);
            lsopt.set = 1;
            lsopt.bind = 1;

            if (lsopt.rcvbuf == -1) {
                ngx_conf_log_error(1, cf, 0,
                                   "invalid rcvbuf \"%V\"", &value[n]);
                return (void *) -1;
            }

            continue;
        }

        if (strncmp((const char *) value[n].data, (const char *) "sndbuf=", 7) == 0) {
            size.len = value[n].len - 7;
            size.data = value[n].data + 7;

            lsopt.sndbuf = ngx_parse_size(&size);
            lsopt.set = 1;
            lsopt.bind = 1;

            if (lsopt.sndbuf == -1) {
                ngx_conf_log_error(1, cf, 0,
                                   "invalid sndbuf \"%V\"", &value[n]);
                return (void *) -1;
            }

            continue;
        }

        if (strncmp((const char *) value[n].data, (const char *) "accept_filter=", 14) == 0) {





            ngx_conf_log_error(1, cf, 0,
                               "accept filters \"%V\" are not supported "
                               "on this platform, ignored",
                               &value[n]);

            continue;
        }

        if (strcmp((const char *) value[n].data, (const char *) "deferred") == 0) {





            ngx_conf_log_error(1, cf, 0,
                               "the deferred accept is not supported "
                               "on this platform, ignored");

            continue;
        }

        if (strncmp((const char *) value[n].data, (const char *) "ipv6only=o", 10) == 0) {
# 3948 "src/http/ngx_http_core_module.c"
            ngx_conf_log_error(1, cf, 0,
                               "ipv6only is not supported "
                               "on this platform");
            return (void *) -1;

        }

        if (strcmp((const char *) value[n].data, (const char *) "reuseport") == 0) {

            lsopt.reuseport = 1;
            lsopt.set = 1;
            lsopt.bind = 1;





            continue;
        }

        if (strcmp((const char *) value[n].data, (const char *) "ssl") == 0) {




            ngx_conf_log_error(1, cf, 0,
                               "the \"ssl\" parameter requires "
                               "ngx_http_ssl_module");
            return (void *) -1;

        }

        if (strcmp((const char *) value[n].data, (const char *) "http2") == 0) {




            ngx_conf_log_error(1, cf, 0,
                               "the \"http2\" parameter requires "
                               "ngx_http_v2_module");
            return (void *) -1;

        }

        if (strcmp((const char *) value[n].data, (const char *) "spdy") == 0) {
            ngx_conf_log_error(5, cf, 0,
                               "invalid parameter \"spdy\": "
                               "ngx_http_spdy_module was superseded "
                               "by ngx_http_v2_module");
            continue;
        }

        if (strncmp((const char *) value[n].data, (const char *) "so_keepalive=", 13) == 0) {

            if (strcmp((const char *) &value[n].data[13], (const char *) "on") == 0) {
                lsopt.so_keepalive = 1;

            } else if (strcmp((const char *) &value[n].data[13], (const char *) "off") == 0) {
                lsopt.so_keepalive = 2;

            } else {


                u_char *p, *end;
                ngx_str_t s;

                end = value[n].data + value[n].len;
                s.data = value[n].data + 13;

                p = ngx_strlchr(s.data, end, ':');
                if (p == 0) {
                    p = end;
                }

                if (p > s.data) {
                    s.len = p - s.data;

                    lsopt.tcp_keepidle = ngx_parse_time(&s, 1);
                    if (lsopt.tcp_keepidle == (time_t) -1) {
                        goto invalid_so_keepalive;
                    }
                }

                s.data = (p < end) ? (p + 1) : end;

                p = ngx_strlchr(s.data, end, ':');
                if (p == 0) {
                    p = end;
                }

                if (p > s.data) {
                    s.len = p - s.data;

                    lsopt.tcp_keepintvl = ngx_parse_time(&s, 1);
                    if (lsopt.tcp_keepintvl == (time_t) -1) {
                        goto invalid_so_keepalive;
                    }
                }

                s.data = (p < end) ? (p + 1) : end;

                if (s.data < end) {
                    s.len = end - s.data;

                    lsopt.tcp_keepcnt = ngx_atoi(s.data, s.len);
                    if (lsopt.tcp_keepcnt == -1) {
                        goto invalid_so_keepalive;
                    }
                }

                if (lsopt.tcp_keepidle == 0 && lsopt.tcp_keepintvl == 0
                    && lsopt.tcp_keepcnt == 0)
                {
                    goto invalid_so_keepalive;
                }

                lsopt.so_keepalive = 1;
# 4074 "src/http/ngx_http_core_module.c"
            }

            lsopt.set = 1;
            lsopt.bind = 1;

            continue;


        invalid_so_keepalive:

            ngx_conf_log_error(1, cf, 0,
                               "invalid so_keepalive value: \"%s\"",
                               &value[n].data[13]);
            return (void *) -1;

        }

        if (strcmp((const char *) value[n].data, (const char *) "proxy_protocol") == 0) {
            lsopt.proxy_protocol = 1;
            continue;
        }

        ngx_conf_log_error(1, cf, 0,
                           "invalid parameter \"%V\"", &value[n]);
        return (void *) -1;
    }

    if (ngx_http_add_listen(cf, cscf, &lsopt) == 0) {
        return 0;
    }

    return (void *) -1;
}


static char *
ngx_http_core_server_name(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_srv_conf_t *cscf = conf;

    u_char ch;
    ngx_str_t *value;
    ngx_uint_t i;
    ngx_http_server_name_t *sn;

    value = cf->args->elts;

    for (i = 1; i < cf->args->nelts; i++) {

        ch = value[i].data[0];

        if ((ch == '*' && (value[i].len < 3 || value[i].data[1] != '.'))
            || (ch == '.' && value[i].len < 2))
        {
            ngx_conf_log_error(1, cf, 0,
                               "server name \"%V\" is invalid", &value[i]);
            return (void *) -1;
        }

        if (strchr((const char *) value[i].data, (int) '/')) {
            ngx_conf_log_error(5, cf, 0,
                               "server name \"%V\" has suspicious symbols",
                               &value[i]);
        }

        sn = ngx_array_push(&cscf->server_names);
        if (sn == 0) {
            return (void *) -1;
        }




        sn->server = cscf;

        if (ngx_strcasecmp(value[i].data, (u_char *) "$hostname") == 0) {
            sn->name = cf->cycle->hostname;

        } else {
            sn->name = value[i];
        }

        if (value[i].data[0] != '~') {
            ngx_strlow(sn->name.data, sn->name.data, sn->name.len);
            continue;
        }
# 4198 "src/http/ngx_http_core_module.c"
        ngx_conf_log_error(1, cf, 0,
                           "using regex \"%V\" "
                           "requires PCRE library", &value[i]);

        return (void *) -1;

    }

    return 0;
}


static char *
ngx_http_core_root(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    ngx_str_t *value;
    ngx_int_t alias;
    ngx_uint_t n;
    ngx_http_script_compile_t sc;

    alias = (cmd->name.len == sizeof("alias") - 1) ? 1 : 0;

    if (clcf->root.data) {

        if ((clcf->alias != 0) == alias) {
            return "is duplicate";
        }

        ngx_conf_log_error(1, cf, 0,
                           "\"%V\" directive is duplicate, "
                           "\"%s\" directive was specified earlier",
                           &cmd->name, clcf->alias ? "alias" : "root");

        return (void *) -1;
    }

    if (clcf->named && alias) {
        ngx_conf_log_error(1, cf, 0,
                           "the \"alias\" directive cannot be used "
                           "inside the named location");

        return (void *) -1;
    }

    value = cf->args->elts;

    if (strstr((const char *) value[1].data, (const char *) "$document_root")
        || strstr((const char *) value[1].data, (const char *) "${document_root}"))
    {
        ngx_conf_log_error(1, cf, 0,
                           "the $document_root variable cannot be used "
                           "in the \"%V\" directive",
                           &cmd->name);

        return (void *) -1;
    }

    if (strstr((const char *) value[1].data, (const char *) "$realpath_root")
        || strstr((const char *) value[1].data, (const char *) "${realpath_root}"))
    {
        ngx_conf_log_error(1, cf, 0,
                           "the $realpath_root variable cannot be used "
                           "in the \"%V\" directive",
                           &cmd->name);

        return (void *) -1;
    }

    clcf->alias = alias ? clcf->name.len : 0;
    clcf->root = value[1];

    if (!alias && clcf->root.len > 0
        && clcf->root.data[clcf->root.len - 1] == '/')
    {
        clcf->root.len--;
    }

    if (clcf->root.data[0] != '$') {
        if (ngx_conf_full_name(cf->cycle, &clcf->root, 0) != 0) {
            return (void *) -1;
        }
    }

    n = ngx_http_script_variables_count(&clcf->root);

    (void) memset(&sc, 0, sizeof(ngx_http_script_compile_t));
    sc.variables = n;
# 4295 "src/http/ngx_http_core_module.c"
    if (n) {
        sc.cf = cf;
        sc.source = &clcf->root;
        sc.lengths = &clcf->root_lengths;
        sc.values = &clcf->root_values;
        sc.complete_lengths = 1;
        sc.complete_values = 1;

        if (ngx_http_script_compile(&sc) != 0) {
            return (void *) -1;
        }
    }

    return 0;
}


static ngx_http_method_name_t ngx_methods_names[] = {
    { (u_char *) "GET", (uint32_t) ~0x0002 },
    { (u_char *) "HEAD", (uint32_t) ~0x0004 },
    { (u_char *) "POST", (uint32_t) ~0x0008 },
    { (u_char *) "PUT", (uint32_t) ~0x0010 },
    { (u_char *) "DELETE", (uint32_t) ~0x0020 },
    { (u_char *) "MKCOL", (uint32_t) ~0x0040 },
    { (u_char *) "COPY", (uint32_t) ~0x0080 },
    { (u_char *) "MOVE", (uint32_t) ~0x0100 },
    { (u_char *) "OPTIONS", (uint32_t) ~0x0200 },
    { (u_char *) "PROPFIND", (uint32_t) ~0x0400 },
    { (u_char *) "PROPPATCH", (uint32_t) ~0x0800 },
    { (u_char *) "LOCK", (uint32_t) ~0x1000 },
    { (u_char *) "UNLOCK", (uint32_t) ~0x2000 },
    { (u_char *) "PATCH", (uint32_t) ~0x4000 },
    { 0, 0 }
};


static char *
ngx_http_core_limit_except(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *pclcf = conf;

    char *rv;
    void *mconf;
    ngx_str_t *value;
    ngx_uint_t i;
    ngx_conf_t save;
    ngx_http_module_t *module;
    ngx_http_conf_ctx_t *ctx, *pctx;
    ngx_http_method_name_t *name;
    ngx_http_core_loc_conf_t *clcf;

    if (pclcf->limit_except) {
        return "is duplicate";
    }

    pclcf->limit_except = 0xffffffff;

    value = cf->args->elts;

    for (i = 1; i < cf->args->nelts; i++) {
        for (name = ngx_methods_names; name->name; name++) {

            if (ngx_strcasecmp(value[i].data, name->name) == 0) {
                pclcf->limit_except &= name->method;
                goto next;
            }
        }

        ngx_conf_log_error(1, cf, 0,
                           "invalid method \"%V\"", &value[i]);
        return (void *) -1;

    next:
        continue;
    }

    if (!(pclcf->limit_except & 0x0002)) {
        pclcf->limit_except &= (uint32_t) ~0x0004;
    }

    ctx = ngx_pcalloc(cf->pool, sizeof(ngx_http_conf_ctx_t));
    if (ctx == 0) {
        return (void *) -1;
    }

    pctx = cf->ctx;
    ctx->main_conf = pctx->main_conf;
    ctx->srv_conf = pctx->srv_conf;

    ctx->loc_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->loc_conf == 0) {
        return (void *) -1;
    }

    for (i = 0; cf->cycle->modules[i]; i++) {
        if (cf->cycle->modules[i]->type != 0x50545448) {
            continue;
        }

        module = cf->cycle->modules[i]->ctx;

        if (module->create_loc_conf) {

            mconf = module->create_loc_conf(cf);
            if (mconf == 0) {
                return (void *) -1;
            }

            ctx->loc_conf[cf->cycle->modules[i]->ctx_index] = mconf;
        }
    }


    clcf = ctx->loc_conf[ngx_http_core_module.ctx_index];
    pclcf->limit_except_loc_conf = ctx->loc_conf;
    clcf->loc_conf = ctx->loc_conf;
    clcf->name = pclcf->name;
    clcf->noname = 1;
    clcf->lmt_excpt = 1;

    if (ngx_http_add_location(cf, &pclcf->locations, clcf) != 0) {
        return (void *) -1;
    }

    save = *cf;
    cf->ctx = ctx;
    cf->cmd_type = 0x80000000;

    rv = ngx_conf_parse(cf, 0);

    *cf = save;

    return rv;
}


static char *
ngx_http_core_set_aio(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    ngx_str_t *value;

    if (clcf->aio != -1) {
        return "is duplicate";
    }






    value = cf->args->elts;

    if (strcmp((const char *) value[1].data, (const char *) "off") == 0) {
        clcf->aio = 0;
        return 0;
    }

    if (strcmp((const char *) value[1].data, (const char *) "on") == 0) {




        ngx_conf_log_error(1, cf, 0,
                           "\"aio on\" "
                           "is unsupported on this platform");
        return (void *) -1;

    }
# 4479 "src/http/ngx_http_core_module.c"
    if (strncmp((const char *) value[1].data, (const char *) "threads", 7) == 0
        && (value[1].len == 7 || value[1].data[7] == '='))
    {
# 4530 "src/http/ngx_http_core_module.c"
        ngx_conf_log_error(1, cf, 0,
                           "\"aio threads\" "
                           "is unsupported on this platform");
        return (void *) -1;

    }

    return "invalid value";
}


static char *
ngx_http_core_directio(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    ngx_str_t *value;

    if (clcf->directio != -1) {
        return "is duplicate";
    }

    value = cf->args->elts;

    if (strcmp((const char *) value[1].data, (const char *) "off") == 0) {
        clcf->directio = 9223372036854775807LL;
        return 0;
    }

    clcf->directio = ngx_parse_offset(&value[1]);
    if (clcf->directio == (off_t) -1) {
        return "invalid value";
    }

    return 0;
}


static char *
ngx_http_core_error_page(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    u_char *p;
    ngx_int_t overwrite;
    ngx_str_t *value, uri, args;
    ngx_uint_t i, n;
    ngx_http_err_page_t *err;
    ngx_http_complex_value_t cv;
    ngx_http_compile_complex_value_t ccv;

    if (clcf->error_pages == 0) {
        clcf->error_pages = ngx_array_create(cf->pool, 4,
                                             sizeof(ngx_http_err_page_t));
        if (clcf->error_pages == 0) {
            return (void *) -1;
        }
    }

    value = cf->args->elts;

    i = cf->args->nelts - 2;

    if (value[i].data[0] == '=') {
        if (i == 1) {
            ngx_conf_log_error(1, cf, 0,
                               "invalid value \"%V\"", &value[i]);
            return (void *) -1;
        }

        if (value[i].len > 1) {
            overwrite = ngx_atoi(&value[i].data[1], value[i].len - 1);

            if (overwrite == -1) {
                ngx_conf_log_error(1, cf, 0,
                                   "invalid value \"%V\"", &value[i]);
                return (void *) -1;
            }

        } else {
            overwrite = 0;
        }

        n = 2;

    } else {
        overwrite = -1;
        n = 1;
    }

    uri = value[cf->args->nelts - 1];

    (void) memset(&ccv, 0, sizeof(ngx_http_compile_complex_value_t));

    ccv.cf = cf;
    ccv.value = &uri;
    ccv.complex_value = &cv;

    if (ngx_http_compile_complex_value(&ccv) != 0) {
        return (void *) -1;
    }

    (&args)->len = 0; (&args)->data = 0;

    if (cv.lengths == 0 && uri.len && uri.data[0] == '/') {
        p = (u_char *) strchr((const char *) uri.data, (int) '?');

        if (p) {
            cv.value.len = p - uri.data;
            cv.value.data = uri.data;
            p++;
            args.len = (uri.data + uri.len) - p;
            args.data = p;
        }
    }

    for (i = 1; i < cf->args->nelts - n; i++) {
        err = ngx_array_push(clcf->error_pages);
        if (err == 0) {
            return (void *) -1;
        }

        err->status = ngx_atoi(value[i].data, value[i].len);

        if (err->status == -1 || err->status == 499) {
            ngx_conf_log_error(1, cf, 0,
                               "invalid value \"%V\"", &value[i]);
            return (void *) -1;
        }

        if (err->status < 300 || err->status > 599) {
            ngx_conf_log_error(1, cf, 0,
                               "value \"%V\" must be between 300 and 599",
                               &value[i]);
            return (void *) -1;
        }

        err->overwrite = overwrite;

        if (overwrite == -1) {
            switch (err->status) {
                case 497:
                case 495:
                case 496:
                    err->overwrite = 400;
            }
        }

        err->value = cv;
        err->args = args;
    }

    return 0;
}


static char *
ngx_http_core_open_file_cache(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    time_t inactive;
    ngx_str_t *value, s;
    ngx_int_t max;
    ngx_uint_t i;

    if (clcf->open_file_cache != (void *) -1) {
        return "is duplicate";
    }

    value = cf->args->elts;

    max = 0;
    inactive = 60;

    for (i = 1; i < cf->args->nelts; i++) {

        if (strncmp((const char *) value[i].data, (const char *) "max=", 4) == 0) {

            max = ngx_atoi(value[i].data + 4, value[i].len - 4);
            if (max <= 0) {
                goto failed;
            }

            continue;
        }

        if (strncmp((const char *) value[i].data, (const char *) "inactive=", 9) == 0) {

            s.len = value[i].len - 9;
            s.data = value[i].data + 9;

            inactive = ngx_parse_time(&s, 1);
            if (inactive == (time_t) -1) {
                goto failed;
            }

            continue;
        }

        if (strcmp((const char *) value[i].data, (const char *) "off") == 0) {

            clcf->open_file_cache = 0;

            continue;
        }

    failed:

        ngx_conf_log_error(1, cf, 0,
                           "invalid \"open_file_cache\" parameter \"%V\"",
                           &value[i]);
        return (void *) -1;
    }

    if (clcf->open_file_cache == 0) {
        return 0;
    }

    if (max == 0) {
        ngx_conf_log_error(1, cf, 0,
                        "\"open_file_cache\" must have the \"max\" parameter");
        return (void *) -1;
    }

    clcf->open_file_cache = ngx_open_file_cache_init(cf->pool, max, inactive);
    if (clcf->open_file_cache) {
        return 0;
    }

    return (void *) -1;
}


static char *
ngx_http_core_error_log(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    return ngx_log_set_log(cf, &clcf->error_log);
}


static char *
ngx_http_core_keepalive(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    ngx_str_t *value;

    if (clcf->keepalive_timeout != (ngx_msec_t) -1) {
        return "is duplicate";
    }

    value = cf->args->elts;

    clcf->keepalive_timeout = ngx_parse_time(&value[1], 0);

    if (clcf->keepalive_timeout == (ngx_msec_t) -1) {
        return "invalid value";
    }

    if (cf->args->nelts == 2) {
        return 0;
    }

    clcf->keepalive_header = ngx_parse_time(&value[2], 1);

    if (clcf->keepalive_header == (time_t) -1) {
        return "invalid value";
    }

    return 0;
}


static char *
ngx_http_core_internal(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    if (clcf->internal != -1) {
        return "is duplicate";
    }

    clcf->internal = 1;

    return 0;
}


static char *
ngx_http_core_resolver(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    ngx_str_t *value;

    if (clcf->resolver) {
        return "is duplicate";
    }

    value = cf->args->elts;

    clcf->resolver = ngx_resolver_create(cf, &value[1], cf->args->nelts - 1);
    if (clcf->resolver == 0) {
        return (void *) -1;
    }

    return 0;
}
# 4947 "src/http/ngx_http_core_module.c"
static char *
ngx_http_disable_symlinks(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf = conf;

    ngx_str_t *value;
    ngx_uint_t i;
    ngx_http_compile_complex_value_t ccv;

    if (clcf->disable_symlinks != (ngx_uint_t) -1) {
        return "is duplicate";
    }

    value = cf->args->elts;

    for (i = 1; i < cf->args->nelts; i++) {

        if (strcmp((const char *) value[i].data, (const char *) "off") == 0) {
            clcf->disable_symlinks = 0;
            continue;
        }

        if (strcmp((const char *) value[i].data, (const char *) "if_not_owner") == 0) {
            clcf->disable_symlinks = 2;
            continue;
        }

        if (strcmp((const char *) value[i].data, (const char *) "on") == 0) {
            clcf->disable_symlinks = 1;
            continue;
        }

        if (strncmp((const char *) value[i].data, (const char *) "from=", 5) == 0) {
            value[i].len -= 5;
            value[i].data += 5;

            (void) memset(&ccv, 0, sizeof(ngx_http_compile_complex_value_t));

            ccv.cf = cf;
            ccv.value = &value[i];
            ccv.complex_value = ngx_palloc(cf->pool,
                                           sizeof(ngx_http_complex_value_t));
            if (ccv.complex_value == 0) {
                return (void *) -1;
            }

            if (ngx_http_compile_complex_value(&ccv) != 0) {
                return (void *) -1;
            }

            clcf->disable_symlinks_from = ccv.complex_value;

            continue;
        }

        ngx_conf_log_error(1, cf, 0,
                           "invalid parameter \"%V\"", &value[i]);
        return (void *) -1;
    }

    if (clcf->disable_symlinks == (ngx_uint_t) -1) {
        ngx_conf_log_error(1, cf, 0,
                           "\"%V\" must have \"off\", \"on\" "
                           "or \"if_not_owner\" parameter",
                           &cmd->name);
        return (void *) -1;
    }

    if (cf->args->nelts == 2) {
        clcf->disable_symlinks_from = 0;
        return 0;
    }

    if (clcf->disable_symlinks_from == (void *) -1) {
        ngx_conf_log_error(1, cf, 0,
                           "duplicate parameters \"%V %V\"",
                           &value[1], &value[2]);
        return (void *) -1;
    }

    if (clcf->disable_symlinks == 0) {
        ngx_conf_log_error(1, cf, 0,
                           "\"from=\" cannot be used with \"off\" parameter");
        return (void *) -1;
    }

    return 0;
}




static char *
ngx_http_core_lowat_check(ngx_conf_t *cf, void *post, void *data)
{
# 5055 "src/http/ngx_http_core_module.c"
    ssize_t *np = data;

    ngx_conf_log_error(5, cf, 0,
                       "\"send_lowat\" is not supported, ignored");

    *np = 0;



    return 0;
}


static char *
ngx_http_core_pool_size(ngx_conf_t *cf, void *post, void *data)
{
    size_t *sp = data;

    if (*sp < ((((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t))) + (16 - 1)) & ~(16 - 1))) {
        ngx_conf_log_error(1, cf, 0,
                           "the pool size must be no less than %uz",
                           ((((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t))) + (16 - 1)) & ~(16 - 1)));
        return (void *) -1;
    }

    if (*sp % 16) {
        ngx_conf_log_error(1, cf, 0,
                           "the pool size must be a multiple of %uz",
                           16);
        return (void *) -1;
    }

    return 0;
}
