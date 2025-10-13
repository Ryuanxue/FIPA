
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

#include "nginx_rpc_wrapper.h"


ngx_int_t ngx_daemon(ngx_log_t *log)
{
  int fd;
  switch (fork())
  {
    case -1:
      if (log->log_level >= 1)
      ngx_log_error_core(1, log, errno, "fork() failed");
      return -1;

    case 0:
      break;

    default:
      exit(0);

  }

  ngx_parent = get_ngx_pid_wrapper();
  set_ngx_pid_wrapper(getpid());
  if (setsid() == (-1))
  {
    if (log->log_level >= 1)
      ngx_log_error_core(1, log, errno, "setsid() failed");
    return -1;
  }
  umask(0);
  fd = open("/dev/null", O_RDWR);
  if (fd == (-1))
  {
    if (log->log_level >= 1)
      ngx_log_error_core(1, log, errno, "open(\"/dev/null\") failed");
    return -1;
  }
  if (dup2(fd, STDIN_FILENO) == (-1))
  {
    if (log->log_level >= 1)
      ngx_log_error_core(1, log, errno, "dup2(STDIN) failed");
    return -1;
  }
  if (dup2(fd, STDOUT_FILENO) == (-1))
  {
    if (log->log_level >= 1)
      ngx_log_error_core(1, log, errno, "dup2(STDOUT) failed");
    return -1;
  }
  if (fd > STDERR_FILENO)
  {
    if (close(fd) == (-1))
    {
      if (log->log_level >= 1)
        ngx_log_error_core(1, log, errno, "close() failed");
      return -1;
    }
  }
  return 0;
}


