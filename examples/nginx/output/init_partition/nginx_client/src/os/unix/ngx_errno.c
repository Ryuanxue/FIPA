
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

#include "nginx_rpc_wrapper.h"


/*
 * The strerror() messages are copied because:
 *
 * 1) strerror() and strerror_r() functions are not Async-Signal-Safe,
 *    therefore, they cannot be used in signal handlers;
 *
 * 2) a direct sys_errlist[] array may be used instead of these functions,
 *    but Linux linker warns about its usage:
 *
 * warning: `sys_errlist' is deprecated; use `strerror' or `strerror_r' instead
 * warning: `sys_nerr' is deprecated; use `strerror' or `strerror_r' instead
 *
 *    causing false bug reports.
 */


static ngx_str_t  *ngx_sys_errlist;
static ngx_str_t   ngx_unknown_error = ngx_string("Unknown error");


u_char *ngx_strerror(ngx_err_t err, u_char *errstr, size_t size)
{
  ngx_str_t *msg;
  msg = (((ngx_uint_t) err) < 135) ? (&get_ngx_sys_errlist_wrapper()[err]) : (&ngx_unknown_error);
  size = (size > msg->len) ? (msg->len) : (size);
  return ((u_char *) memcpy(errstr, msg->data, size)) + size;
}




ngx_int_t ngx_strerror_init(void)
{
  char *msg;
  u_char *p;
  size_t len;
  ngx_err_t err;
  len = 135 * (sizeof(ngx_str_t));
  set_ngx_sys_errlist_wrapper(malloc(len));
  if (get_ngx_sys_errlist_wrapper() == 0)
  {
    goto failed;
  }
  for (err = 0; err < 135; err++)
  {
    msg = strerror(err);
    len = strlen((const char *) msg);
    p = malloc(len);
    if (p == 0)
    {
      goto failed;
    }
    (void) memcpy(p, msg, len);
    get_ngx_sys_errlist_wrapper()[err].len = len;
    get_ngx_sys_errlist_wrapper()[err].data = p;
  }

  return 0;
  failed:
  err = errno;

  ngx_log_stderr(0, "malloc(%uz) failed (%d: %s)", len, err, strerror(err));
  return -1;
}


