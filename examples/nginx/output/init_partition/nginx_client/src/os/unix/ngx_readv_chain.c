
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>

#include "nginx_rpc_wrapper.h"


ssize_t ngx_readv_chain(ngx_connection_t *c, ngx_chain_t *chain, off_t limit)
{
  u_char *prev;
  ssize_t n;
  ssize_t size;
  ngx_err_t err;
  ngx_array_t vec;
  ngx_event_t *rev;
  struct iovec *iov;
  struct iovec iovs[IOV_MAX];
  rev = c->read;
  if (get_ngx_event_flags_wrapper() & 0x00000040)
  {
    ;
    if ((!rev->available) && (!rev->pending_eof))
    {
      return -2;
    }
  }
  prev = 0;
  iov = 0;
  size = 0;
  vec.elts = iovs;
  vec.nelts = 0;
  vec.size = sizeof(struct iovec);
  vec.nalloc = IOV_MAX;
  vec.pool = c->pool;
  while (chain)
  {
    n = chain->buf->end - chain->buf->last;
    if (limit)
    {
      if (size >= limit)
      {
        break;
      }
      if ((size + n) > limit)
      {
        n = (ssize_t) (limit - size);
      }
    }
    if (prev == chain->buf->last)
    {
      iov->iov_len += n;
    }
    else
    {
      if (vec.nelts >= IOV_MAX)
      {
        break;
      }
      iov = ngx_array_push(&vec);
      if (iov == 0)
      {
        return -1;
      }
      iov->iov_base = (void *) chain->buf->last;
      iov->iov_len = n;
    }
    size += n;
    prev = chain->buf->end;
    chain = chain->next;
  }

  ;
  do
  {
    n = readv(c->fd, (struct iovec *) vec.elts, vec.nelts);
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
        if (n < size)
        {
          if (!rev->pending_eof)
          {
            rev->ready = 0;
          }
          rev->available = 0;
        }
        return n;
      }
      if ((n < size) && (!(get_ngx_event_flags_wrapper() & 0x00000020)))
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
      n = ngx_connection_error(c, err, "readv() failed");
      break;
    }
  }
  while (err == EINTR);
  rev->ready = 0;
  if (n == (-1))
  {
    c->read->error = 1;
  }
  return n;
}


