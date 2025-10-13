
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_event_connect.h>

#include "nginx_rpc_wrapper.h"


#if (NGX_HAVE_TRANSPARENT_PROXY)
static ngx_int_t ngx_event_connect_set_transparent(ngx_peer_connection_t *pc,
    ngx_socket_t s);
#endif


ngx_int_t ngx_event_connect_peer(ngx_peer_connection_t *pc)
{
  int rc;
  int type;
  in_port_t port;
  ngx_int_t event;
  ngx_err_t err;
  ngx_uint_t level;
  ngx_socket_t s;
  ngx_event_t *rev;
  ngx_event_t *wev;
  ngx_connection_t *c;
  rc = pc->get(pc, pc->data);
  if (rc != 0)
  {
    return rc;
  }
  type = (pc->type) ? (pc->type) : (SOCK_STREAM);
  s = socket(pc->sockaddr->sa_family, type, 0);
  ;
  if (s == ((ngx_socket_t) (-1)))
  {
    if (pc->log->log_level >= 2)
      ngx_log_error_core(2, pc->log, errno, "socket() failed");
    return -1;
  }
  c = ngx_get_connection(s, pc->log);
  if (c == 0)
  {
    if (close(s) == (-1))
    {
      if (pc->log->log_level >= 2)
        ngx_log_error_core(2, pc->log, errno, "close() socket failed");
    }
    return -1;
  }
  c->type = type;
  if (pc->rcvbuf)
  {
    if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, (const void *) (&pc->rcvbuf), sizeof(int)) == (-1))
    {
      if (pc->log->log_level >= 2)
        ngx_log_error_core(2, pc->log, errno, "setsockopt(SO_RCVBUF) failed");
      goto failed;
    }
  }
  if (ngx_nonblocking(s) == (-1))
  {
    if (pc->log->log_level >= 2)
      ngx_log_error_core(2, pc->log, errno, "ioctl(FIONBIO) failed");
    goto failed;
  }
  if (pc->local)
  {
    if (pc->transparent)
    {
      if (ngx_event_connect_set_transparent(pc, s) != 0)
      {
        goto failed;
      }
    }
    port = ngx_inet_get_port(pc->local->sockaddr);
    if ((pc->sockaddr->sa_family != AF_UNIX) && (port == 0))
    {
      static int bind_address_no_port = 1;
      if (bind_address_no_port)
      {
        if (setsockopt(s, IPPROTO_IP, IP_BIND_ADDRESS_NO_PORT, (const void *) (&bind_address_no_port), sizeof(int)) == (-1))
        {
          err = errno;
          if ((err != EOPNOTSUPP) && (err != ENOPROTOOPT))
          {
            if (pc->log->log_level >= 2)
              ngx_log_error_core(2, pc->log, err, "setsockopt(IP_BIND_ADDRESS_NO_PORT) failed, ignored");
          }
          else
          {
            bind_address_no_port = 0;
          }
        }
      }
    }
    if ((pc->type == SOCK_DGRAM) && (port != 0))
    {
      int reuse_addr = 1;
      if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const void *) (&reuse_addr), sizeof(int)) == (-1))
      {
        if (pc->log->log_level >= 2)
          ngx_log_error_core(2, pc->log, errno, "setsockopt(SO_REUSEADDR) failed");
        goto failed;
      }
    }
    if (bind(s, pc->local->sockaddr, pc->local->socklen) == (-1))
    {
      if (pc->log->log_level >= 3)
        ngx_log_error_core(3, pc->log, errno, "bind(%V) failed", &pc->local->name);
      goto failed;
    }
  }
  if (type == SOCK_STREAM)
  {
    c->recv = get_ngx_io_recv_wrapper();
    c->send = get_ngx_io_send_wrapper();
    c->recv_chain = get_ngx_io_recv_chain_wrapper();
    c->send_chain = get_ngx_io_send_chain_wrapper();
    c->sendfile = 1;
    if (pc->sockaddr->sa_family == AF_UNIX)
    {
      c->tcp_nopush = NGX_TCP_NOPUSH_DISABLED;
      c->tcp_nodelay = NGX_TCP_NODELAY_DISABLED;
    }
  }
  else
  {
    c->recv = get_ngx_io_udp_recv_wrapper();
    c->send = get_ngx_io_send_wrapper();
    c->send_chain = get_ngx_io_udp_send_chain_wrapper();
  }
  c->log_error = pc->log_error;
  rev = c->read;
  wev = c->write;
  rev->log = pc->log;
  wev->log = pc->log;
  pc->connection = c;
  c->number = __sync_fetch_and_add(ngx_connection_counter, 1);
  if (ngx_event_actions.add_conn)
  {
    if (ngx_event_actions.add_conn(c) == (-1))
    {
      goto failed;
    }
  }
  ;
  rc = connect(s, pc->sockaddr, pc->socklen);
  if (rc == (-1))
  {
    err = errno;
    if (err != EINPROGRESS)
    {
      if (((((((err == ECONNREFUSED) || (err == EAGAIN)) || (err == ECONNRESET)) || (err == ENETDOWN)) || (err == ENETUNREACH)) || (err == EHOSTDOWN)) || (err == EHOSTUNREACH))
      {
        level = 4;
      }
      else
      {
        level = 3;
      }
      if (c->log->log_level >= level)
        ngx_log_error_core(level, c->log, err, "connect() to %V failed", pc->name);
      ngx_close_connection(c);
      pc->connection = 0;
      return -5;
    }
  }
  if (ngx_event_actions.add_conn)
  {
    if (rc == (-1))
    {
      return -2;
    }
    ;
    wev->ready = 1;
    return 0;
  }
  if (get_ngx_event_flags_wrapper() & 0x00000200)
  {
    ;
    if (ngx_blocking(s) == (-1))
    {
      if (pc->log->log_level >= 2)
        ngx_log_error_core(2, pc->log, errno, "ioctl(!FIONBIO) failed");
      goto failed;
    }
    rev->ready = 1;
    wev->ready = 1;
    return 0;
  }
  if (get_ngx_event_flags_wrapper() & 0x00000004)
  {
    event = EPOLLET;
  }
  else
  {
    event = 0;
  }
  if (ngx_event_actions.add(rev, EPOLLIN | EPOLLRDHUP, event) != 0)
  {
    goto failed;
  }
  if (rc == (-1))
  {
    if (ngx_event_actions.add(wev, EPOLLOUT, event) != 0)
    {
      goto failed;
    }
    return -2;
  }
  ;
  wev->ready = 1;
  return 0;
  failed:
  ngx_close_connection(c);

  pc->connection = 0;
  return -1;
}




#if (NGX_HAVE_TRANSPARENT_PROXY)

static ngx_int_t
ngx_event_connect_set_transparent(ngx_peer_connection_t *pc, ngx_socket_t s)
{
    int  value;

    value = 1;

#if defined(SO_BINDANY)

    if (setsockopt(s, SOL_SOCKET, SO_BINDANY,
                   (const void *) &value, sizeof(int)) == -1)
    {
        ngx_log_error(NGX_LOG_ALERT, pc->log, ngx_socket_errno,
                      "setsockopt(SO_BINDANY) failed");
        return NGX_ERROR;
    }

#else

    switch (pc->local->sockaddr->sa_family) {

    case AF_INET:

#if defined(IP_TRANSPARENT)

        if (setsockopt(s, IPPROTO_IP, IP_TRANSPARENT,
                       (const void *) &value, sizeof(int)) == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, pc->log, ngx_socket_errno,
                          "setsockopt(IP_TRANSPARENT) failed");
            return NGX_ERROR;
        }

#elif defined(IP_BINDANY)

        if (setsockopt(s, IPPROTO_IP, IP_BINDANY,
                       (const void *) &value, sizeof(int)) == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, pc->log, ngx_socket_errno,
                          "setsockopt(IP_BINDANY) failed");
            return NGX_ERROR;
        }

#endif

        break;

#if (NGX_HAVE_INET6)

    case AF_INET6:

#if defined(IPV6_TRANSPARENT)

        if (setsockopt(s, IPPROTO_IPV6, IPV6_TRANSPARENT,
                       (const void *) &value, sizeof(int)) == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, pc->log, ngx_socket_errno,
                          "setsockopt(IPV6_TRANSPARENT) failed");
            return NGX_ERROR;
        }

#elif defined(IPV6_BINDANY)

        if (setsockopt(s, IPPROTO_IPV6, IPV6_BINDANY,
                       (const void *) &value, sizeof(int)) == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, pc->log, ngx_socket_errno,
                          "setsockopt(IPV6_BINDANY) failed");
            return NGX_ERROR;
        }

#else

        ngx_log_error(NGX_LOG_ALERT, pc->log, 0,
                      "could not enable transparent proxying for IPv6 "
                      "on this platform");

        return NGX_ERROR;

#endif

        break;

#endif /* NGX_HAVE_INET6 */

    }

#endif /* SO_BINDANY */

    return NGX_OK;
}

#endif


ngx_int_t
ngx_event_get_peer(ngx_peer_connection_t *pc, void *data)
{
    return NGX_OK;
}
