
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>

#include "nginx_rpc_wrapper.h"


ssize_t ngx_unix_recv(ngx_connection_t *c, u_char *buf, size_t size)
{
  ssize_t n;
  ngx_err_t err;
  ngx_event_t *rev;
  rev = c->read;
  if (get_ngx_event_flags_wrapper() & 0x00000040)
  {
    ;
    if ((!rev->available) && (!rev->pending_eof))
    {
      rev->ready = 0;
      return -2;
    }
  }
  do
  {
    n = recv(c->fd, buf, size, 0);
    ;
    if (n == 0)
    {
      rev->ready = 0;
      rev->eof = 1;
      return 0;
    }
    if (n > 0)
    {
      if ((get_ngx_event_flags_wrapper() & 0x00000040) && ngx_use_epoll_rdhup)
      {
        if (((size_t) n) < size)
        {
          if (!rev->pending_eof)
          {
            rev->ready = 0;
          }
          rev->available = 0;
        }
        return n;
      }
      if ((((size_t) n) < size) && (!(get_ngx_event_flags_wrapper() & 0x00000020)))
      {
        rev->ready = 0;
      }
      return n;
    }
    err = errno;
    if ((err == EAGAIN) || (err == EINTR))
    {
      ;
      n = -2;
    }
    else
    {
      n = ngx_connection_error(c, err, "recv() failed");
      break;
    }
  }
  while (err == EINTR);
  rev->ready = 0;
  if (n == (-1))
  {
    rev->error = 1;
  }
  return n;
}


