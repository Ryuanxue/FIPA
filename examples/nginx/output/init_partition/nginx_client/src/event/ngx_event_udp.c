
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>

#include "nginx_rpc_wrapper.h"


#if !(NGX_WIN32)

struct ngx_udp_connection_s {
    ngx_rbtree_node_t   node;
    ngx_connection_t   *connection;
    ngx_buf_t          *buffer;
};


static void ngx_close_accepted_udp_connection(ngx_connection_t *c);
static ssize_t ngx_udp_shared_recv(ngx_connection_t *c, u_char *buf,
    size_t size);
static ngx_int_t ngx_insert_udp_connection(ngx_connection_t *c);
static void ngx_delete_udp_connection(void *data);
static ngx_connection_t *ngx_lookup_udp_connection(ngx_listening_t *ls,
    struct sockaddr *sockaddr, socklen_t socklen,
    struct sockaddr *local_sockaddr, socklen_t local_socklen);


void ngx_event_recvmsg(ngx_event_t *ev)
{
  ssize_t n;
  ngx_buf_t buf;
  ngx_log_t *log;
  ngx_err_t err;
  socklen_t socklen;
  socklen_t local_socklen;
  ngx_event_t *rev;
  ngx_event_t *wev;
  struct iovec iov[1];
  struct msghdr msg;
  ngx_sockaddr_t sa;
  ngx_sockaddr_t lsa;
  struct sockaddr *sockaddr;
  struct sockaddr *local_sockaddr;
  ngx_listening_t *ls;
  ngx_event_conf_t *ecf;
  ngx_connection_t *c;
  ngx_connection_t *lc;
  static u_char buffer[65535];
  u_char msg_control[CMSG_SPACE(sizeof(struct in_pktinfo))];
  u_char msg_control6[CMSG_SPACE(sizeof(struct in6_pktinfo))];
  if (ev->timedout)
  {
    if (ngx_enable_accept_events((ngx_cycle_t *) get_ngx_cycle_wrapper()) != 0)
    {
      return;
    }
    ev->timedout = 0;
  }
  ecf = (*get_ngx_cycle_wrapper()->conf_ctx[ngx_events_module.index])[ngx_event_core_module.ctx_index];
  ;
  if (!(get_ngx_event_flags_wrapper() & 0x00000008))
  {
    ev->available = ecf->multi_accept;
  }
  lc = ev->data;
  ls = lc->listening;
  ev->ready = 0;
  ;
  do
  {
    (void) memset(&msg, 0, sizeof(struct msghdr));
    iov[0].iov_base = (void *) buffer;
    iov[0].iov_len = sizeof(buffer);
    msg.msg_name = &sa;
    msg.msg_namelen = sizeof(ngx_sockaddr_t);
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    if (ls->wildcard)
    {
      if (ls->sockaddr->sa_family == AF_INET)
      {
        msg.msg_control = &msg_control;
        msg.msg_controllen = sizeof(msg_control);
      }
      if (ls->sockaddr->sa_family == AF_INET6)
      {
        msg.msg_control = &msg_control6;
        msg.msg_controllen = sizeof(msg_control6);
      }
    }
    n = recvmsg(lc->fd, &msg, 0);
    if (n == (-1))
    {
      err = errno;
      if (err == EAGAIN)
      {
        ;
        return;
      }
      if (ev->log->log_level >= 2)
        ngx_log_error_core(2, ev->log, err, "recvmsg() failed");
      return;
    }
    if (msg.msg_flags & (MSG_TRUNC | MSG_CTRUNC))
    {
      if (ev->log->log_level >= 2)
        ngx_log_error_core(2, ev->log, 0, "recvmsg() truncated data");
      continue;
    }
    sockaddr = msg.msg_name;
    socklen = msg.msg_namelen;
    if (socklen > ((socklen_t) (sizeof(ngx_sockaddr_t))))
    {
      socklen = sizeof(ngx_sockaddr_t);
    }
    if (socklen == 0)
    {
      socklen = sizeof(struct sockaddr);
      (void) memset(&sa, 0, sizeof(struct sockaddr));
      sa.sockaddr.sa_family = ls->sockaddr->sa_family;
    }
    local_sockaddr = ls->sockaddr;
    local_socklen = ls->socklen;
    if (ls->wildcard)
    {
      struct cmsghdr *cmsg;
      (void) memcpy(&lsa, local_sockaddr, local_socklen);
      local_sockaddr = &lsa.sockaddr;
      for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != 0; cmsg = CMSG_NXTHDR(&msg, cmsg))
      {
        if (((cmsg->cmsg_level == IPPROTO_IP) && (cmsg->cmsg_type == IP_PKTINFO)) && (local_sockaddr->sa_family == AF_INET))
        {
          struct in_pktinfo *pkt;
          struct sockaddr_in *sin;
          pkt = (struct in_pktinfo *) CMSG_DATA(cmsg);
          sin = (struct sockaddr_in *) local_sockaddr;
          sin->sin_addr = pkt->ipi_addr;
          break;
        }
        if (((cmsg->cmsg_level == IPPROTO_IPV6) && (cmsg->cmsg_type == IPV6_PKTINFO)) && (local_sockaddr->sa_family == AF_INET6))
        {
          struct in6_pktinfo *pkt6;
          struct sockaddr_in6 *sin6;
          pkt6 = (struct in6_pktinfo *) CMSG_DATA(cmsg);
          sin6 = (struct sockaddr_in6 *) local_sockaddr;
          sin6->sin6_addr = pkt6->ipi6_addr;
          break;
        }
      }

    }
    c = ngx_lookup_udp_connection(ls, sockaddr, socklen, local_sockaddr, local_socklen);
    if (c)
    {
      (void) memset(&buf, 0, sizeof(ngx_buf_t));
      buf.pos = buffer;
      buf.last = buffer + n;
      rev = c->read;
      c->udp->buffer = &buf;
      rev->ready = 1;
      rev->handler(rev);
      c->udp->buffer = 0;
      rev->ready = 0;
      goto next;
    }
    ngx_accept_disabled = (get_ngx_cycle_wrapper()->connection_n / 8) - get_ngx_cycle_wrapper()->free_connection_n;
    c = ngx_get_connection(lc->fd, ev->log);
    if (c == 0)
    {
      return;
    }
    c->shared = 1;
    c->type = SOCK_DGRAM;
    c->socklen = socklen;
    c->pool = ngx_create_pool(ls->pool_size, ev->log);
    if (c->pool == 0)
    {
      ngx_close_accepted_udp_connection(c);
      return;
    }
    c->sockaddr = ngx_palloc(c->pool, socklen);
    if (c->sockaddr == 0)
    {
      ngx_close_accepted_udp_connection(c);
      return;
    }
    (void) memcpy(c->sockaddr, sockaddr, socklen);
    log = ngx_palloc(c->pool, sizeof(ngx_log_t));
    if (log == 0)
    {
      ngx_close_accepted_udp_connection(c);
      return;
    }
    *log = ls->log;
    c->recv = ngx_udp_shared_recv;
    c->send = get_ngx_io_udp_send_wrapper();
    c->send_chain = get_ngx_io_udp_send_chain_wrapper();
    c->log = log;
    c->pool->log = log;
    c->listening = ls;
    if (local_sockaddr == (&lsa.sockaddr))
    {
      local_sockaddr = ngx_palloc(c->pool, local_socklen);
      if (local_sockaddr == 0)
      {
        ngx_close_accepted_udp_connection(c);
        return;
      }
      (void) memcpy(local_sockaddr, &lsa, local_socklen);
    }
    c->local_sockaddr = local_sockaddr;
    c->local_socklen = local_socklen;
    c->buffer = ngx_create_temp_buf(c->pool, n);
    if (c->buffer == 0)
    {
      ngx_close_accepted_udp_connection(c);
      return;
    }
    c->buffer->last = ((u_char *) memcpy(c->buffer->last, buffer, n)) + n;
    rev = c->read;
    wev = c->write;
    wev->ready = 1;
    rev->log = log;
    wev->log = log;
    c->number = __sync_fetch_and_add(ngx_connection_counter, 1);
    if (ls->addr_ntop)
    {
      c->addr_text.data = ngx_pnalloc(c->pool, ls->addr_text_max_len);
      if (c->addr_text.data == 0)
      {
        ngx_close_accepted_udp_connection(c);
        return;
      }
      c->addr_text.len = ngx_sock_ntop(c->sockaddr, c->socklen, c->addr_text.data, ls->addr_text_max_len, 0);
      if (c->addr_text.len == 0)
      {
        ngx_close_accepted_udp_connection(c);
        return;
      }
    }
    if (ngx_insert_udp_connection(c) != 0)
    {
      ngx_close_accepted_udp_connection(c);
      return;
    }
    log->data = 0;
    log->handler = 0;
    ls->handler(c);
    next:
    if (get_ngx_event_flags_wrapper() & 0x00000008)
    {
      ev->available -= n;
    }

  }
  while (ev->available);
}




static void
ngx_close_accepted_udp_connection(ngx_connection_t *c)
{
    ngx_free_connection(c);

    c->fd = (ngx_socket_t) -1;

    if (c->pool) {
        ngx_destroy_pool(c->pool);
    }

#if (NGX_STAT_STUB)
    (void) ngx_atomic_fetch_add(ngx_stat_active, -1);
#endif
}


static ssize_t
ngx_udp_shared_recv(ngx_connection_t *c, u_char *buf, size_t size)
{
    ssize_t     n;
    ngx_buf_t  *b;

    if (c->udp == NULL || c->udp->buffer == NULL) {
        return NGX_AGAIN;
    }

    b = c->udp->buffer;

    n = ngx_min(b->last - b->pos, (ssize_t) size);

    ngx_memcpy(buf, b->pos, n);

    c->udp->buffer = NULL;
    c->read->ready = 0;

    return n;
}


void
ngx_udp_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_int_t               rc;
    ngx_connection_t       *c, *ct;
    ngx_rbtree_node_t     **p;
    ngx_udp_connection_t   *udp, *udpt;

    for ( ;; ) {

        if (node->key < temp->key) {

            p = &temp->left;

        } else if (node->key > temp->key) {

            p = &temp->right;

        } else { /* node->key == temp->key */

            udp = (ngx_udp_connection_t *) node;
            c = udp->connection;

            udpt = (ngx_udp_connection_t *) temp;
            ct = udpt->connection;

            rc = ngx_cmp_sockaddr(c->sockaddr, c->socklen,
                                  ct->sockaddr, ct->socklen, 1);

            if (rc == 0 && c->listening->wildcard) {
                rc = ngx_cmp_sockaddr(c->local_sockaddr, c->local_socklen,
                                      ct->local_sockaddr, ct->local_socklen, 1);
            }

            p = (rc < 0) ? &temp->left : &temp->right;
        }

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}


static ngx_int_t
ngx_insert_udp_connection(ngx_connection_t *c)
{
    uint32_t               hash;
    ngx_pool_cleanup_t    *cln;
    ngx_udp_connection_t  *udp;

    if (c->udp) {
        return NGX_OK;
    }

    udp = ngx_pcalloc(c->pool, sizeof(ngx_udp_connection_t));
    if (udp == NULL) {
        return NGX_ERROR;
    }

    udp->connection = c;

    ngx_crc32_init(hash);
    ngx_crc32_update(&hash, (u_char *) c->sockaddr, c->socklen);

    if (c->listening->wildcard) {
        ngx_crc32_update(&hash, (u_char *) c->local_sockaddr, c->local_socklen);
    }

    ngx_crc32_final(hash);

    udp->node.key = hash;

    cln = ngx_pool_cleanup_add(c->pool, 0);
    if (cln == NULL) {
        return NGX_ERROR;
    }

    cln->data = c;
    cln->handler = ngx_delete_udp_connection;

    ngx_rbtree_insert(&c->listening->rbtree, &udp->node);

    c->udp = udp;

    return NGX_OK;
}


static void
ngx_delete_udp_connection(void *data)
{
    ngx_connection_t  *c = data;

    ngx_rbtree_delete(&c->listening->rbtree, &c->udp->node);
}


static ngx_connection_t *
ngx_lookup_udp_connection(ngx_listening_t *ls, struct sockaddr *sockaddr,
    socklen_t socklen, struct sockaddr *local_sockaddr, socklen_t local_socklen)
{
    uint32_t               hash;
    ngx_int_t              rc;
    ngx_connection_t      *c;
    ngx_rbtree_node_t     *node, *sentinel;
    ngx_udp_connection_t  *udp;

#if (NGX_HAVE_UNIX_DOMAIN)

    if (sockaddr->sa_family == AF_UNIX) {
        struct sockaddr_un *saun = (struct sockaddr_un *) sockaddr;

        if (socklen <= (socklen_t) offsetof(struct sockaddr_un, sun_path)
            || saun->sun_path[0] == '\0')
        {
            ngx_log_debug0(NGX_LOG_DEBUG_EVENT, ngx_cycle->log, 0,
                           "unbound unix socket");
            return NULL;
        }
    }

#endif

    node = ls->rbtree.root;
    sentinel = ls->rbtree.sentinel;

    ngx_crc32_init(hash);
    ngx_crc32_update(&hash, (u_char *) sockaddr, socklen);

    if (ls->wildcard) {
        ngx_crc32_update(&hash, (u_char *) local_sockaddr, local_socklen);
    }

    ngx_crc32_final(hash);

    while (node != sentinel) {

        if (hash < node->key) {
            node = node->left;
            continue;
        }

        if (hash > node->key) {
            node = node->right;
            continue;
        }

        /* hash == node->key */

        udp = (ngx_udp_connection_t *) node;

        c = udp->connection;

        rc = ngx_cmp_sockaddr(sockaddr, socklen,
                              c->sockaddr, c->socklen, 1);

        if (rc == 0 && ls->wildcard) {
            rc = ngx_cmp_sockaddr(local_sockaddr, local_socklen,
                                  c->local_sockaddr, c->local_socklen, 1);
        }

        if (rc == 0) {
            return c;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    return NULL;
}

#endif
