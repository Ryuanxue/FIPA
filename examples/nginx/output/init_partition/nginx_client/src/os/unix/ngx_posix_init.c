
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <nginx.h>

#include "nginx_rpc_wrapper.h"


ngx_int_t   ngx_ncpu;
ngx_int_t   ngx_max_sockets;
ngx_uint_t  ngx_inherited_nonblocking;
ngx_uint_t  ngx_tcp_nodelay_and_tcp_nopush;


struct rlimit  rlmt;


ngx_os_io_t ngx_os_io = {
    ngx_unix_recv,
    ngx_readv_chain,
    ngx_udp_unix_recv,
    ngx_unix_send,
    ngx_udp_unix_send,
    ngx_udp_unix_sendmsg_chain,
    ngx_writev_chain,
    0
};


ngx_int_t ngx_os_init(ngx_log_t *log)
{
  ngx_time_t *tp;
  ngx_uint_t n;
  long size;
  if (ngx_os_specific_init(log) != 0)
  {
    return -1;
  }
  if (ngx_init_setproctitle(log) != 0)
  {
    return -1;
  }
  set_ngx_pagesize_wrapper(getpagesize());
  ngx_cacheline_size = 64;
  for (n = get_ngx_pagesize_wrapper(); n >>= 1; ngx_pagesize_shift++)
  {
  }

  if (ngx_ncpu == 0)
  {
    ngx_ncpu = sysconf(_SC_NPROCESSORS_ONLN);
  }
  if (ngx_ncpu < 1)
  {
    ngx_ncpu = 1;
  }
  size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
  if (size > 0)
  {
    ngx_cacheline_size = size;
  }
  ngx_cpuinfo();
  if (getrlimit(RLIMIT_NOFILE, &rlmt) == (-1))
  {
    if (log->log_level >= 2)
      ngx_log_error_core(2, log, errno, "getrlimit(RLIMIT_NOFILE) failed");
    return -1;
  }
  ngx_max_sockets = (ngx_int_t) rlmt.rlim_cur;
  ngx_inherited_nonblocking = 1;
  tp = (ngx_time_t *) get_ngx_cached_time_wrapper();
  srandom(((((unsigned) get_ngx_pid_wrapper()) << 16) ^ tp->sec) ^ tp->msec);
  return 0;
}




void
ngx_os_status(ngx_log_t *log)
{
    ngx_log_error(NGX_LOG_NOTICE, log, 0, NGINX_VER_BUILD);

#ifdef NGX_COMPILER
    ngx_log_error(NGX_LOG_NOTICE, log, 0, "built by " NGX_COMPILER);
#endif

#if (NGX_HAVE_OS_SPECIFIC_INIT)
    ngx_os_specific_status(log);
#endif

    ngx_log_error(NGX_LOG_NOTICE, log, 0,
                  "getrlimit(RLIMIT_NOFILE): %r:%r",
                  rlmt.rlim_cur, rlmt.rlim_max);
}


#if 0

ngx_int_t
ngx_posix_post_conf_init(ngx_log_t *log)
{
    ngx_fd_t  pp[2];

    if (pipe(pp) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "pipe() failed");
        return NGX_ERROR;
    }

    if (dup2(pp[1], STDERR_FILENO) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, errno, "dup2(STDERR) failed");
        return NGX_ERROR;
    }

    if (pp[1] > STDERR_FILENO) {
        if (close(pp[1]) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, errno, "close() failed");
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}

#endif
