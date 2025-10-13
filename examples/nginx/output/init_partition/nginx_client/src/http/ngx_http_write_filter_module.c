
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "nginx_rpc_wrapper.h"


static ngx_int_t ngx_http_write_filter_init(ngx_conf_t *cf);


static ngx_http_module_t  ngx_http_write_filter_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_write_filter_init,            /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL,                                  /* merge location configuration */
};


ngx_module_t  ngx_http_write_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_write_filter_module_ctx,     /* module context */
    NULL,                                  /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ngx_int_t ngx_http_write_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
  off_t size;
  off_t sent;
  off_t nsent;
  off_t limit;
  ngx_uint_t last;
  ngx_uint_t flush;
  ngx_uint_t sync;
  ngx_msec_t delay;
  ngx_chain_t *cl;
  ngx_chain_t *ln;
  ngx_chain_t **ll;
  ngx_chain_t *chain;
  ngx_connection_t *c;
  ngx_http_core_loc_conf_t *clcf;
  c = r->connection;
  if (c->error)
  {
    return -1;
  }
  size = 0;
  flush = 0;
  sync = 0;
  last = 0;
  ll = &r->out;
  for (cl = r->out; cl; cl = cl->next)
  {
    ll = &cl->next;
    ;
    if (((((cl->buf->temporary || cl->buf->memory) || cl->buf->mmap) ? ((off_t) (cl->buf->last - cl->buf->pos)) : (cl->buf->file_last - cl->buf->file_pos)) == 0) && (!((((cl->buf->flush || cl->buf->last_buf) || cl->buf->sync) && (!((cl->buf->temporary || cl->buf->memory) || cl->buf->mmap))) && (!cl->buf->in_file))))
    {
      if (c->log->log_level >= 2)
        ngx_log_error_core(2, c->log, 0, "zero size buf in writer t:%d r:%d f:%d %p %p-%p %p %O-%O", cl->buf->temporary, cl->buf->recycled, cl->buf->in_file, cl->buf->start, cl->buf->pos, cl->buf->last, cl->buf->file, cl->buf->file_pos, cl->buf->file_last);
      ngx_debug_point();
      return -1;
    }
    size += ((cl->buf->temporary || cl->buf->memory) || cl->buf->mmap) ? ((off_t) (cl->buf->last - cl->buf->pos)) : (cl->buf->file_last - cl->buf->file_pos);
    if (cl->buf->flush || cl->buf->recycled)
    {
      flush = 1;
    }
    if (cl->buf->sync)
    {
      sync = 1;
    }
    if (cl->buf->last_buf)
    {
      last = 1;
    }
  }

  for (ln = in; ln; ln = ln->next)
  {
    cl = ngx_alloc_chain_link(r->pool);
    if (cl == 0)
    {
      return -1;
    }
    cl->buf = ln->buf;
    *ll = cl;
    ll = &cl->next;
    ;
    if (((((cl->buf->temporary || cl->buf->memory) || cl->buf->mmap) ? ((off_t) (cl->buf->last - cl->buf->pos)) : (cl->buf->file_last - cl->buf->file_pos)) == 0) && (!((((cl->buf->flush || cl->buf->last_buf) || cl->buf->sync) && (!((cl->buf->temporary || cl->buf->memory) || cl->buf->mmap))) && (!cl->buf->in_file))))
    {
      if (c->log->log_level >= 2)
        ngx_log_error_core(2, c->log, 0, "zero size buf in writer t:%d r:%d f:%d %p %p-%p %p %O-%O", cl->buf->temporary, cl->buf->recycled, cl->buf->in_file, cl->buf->start, cl->buf->pos, cl->buf->last, cl->buf->file, cl->buf->file_pos, cl->buf->file_last);
      ngx_debug_point();
      return -1;
    }
    size += ((cl->buf->temporary || cl->buf->memory) || cl->buf->mmap) ? ((off_t) (cl->buf->last - cl->buf->pos)) : (cl->buf->file_last - cl->buf->file_pos);
    if (cl->buf->flush || cl->buf->recycled)
    {
      flush = 1;
    }
    if (cl->buf->sync)
    {
      sync = 1;
    }
    if (cl->buf->last_buf)
    {
      last = 1;
    }
  }

  *ll = 0;
  ;
  clcf = r->loc_conf[ngx_http_core_module.ctx_index];
  if ((((!last) && (!flush)) && in) && (size < ((off_t) clcf->postpone_output)))
  {
    return 0;
  }
  if (c->write->delayed)
  {
    c->buffered |= 0x10;
    return -2;
  }
  if (((size == 0) && (!(c->buffered & 0x0f))) && (!(last && c->need_last_buf)))
  {
    if ((last || flush) || sync)
    {
      for (cl = r->out; cl;)
      {
        ln = cl;
        cl = cl->next;
        ln->next = r->pool->chain;
        r->pool->chain = ln;
      }

      r->out = 0;
      c->buffered &= ~0x10;
      return 0;
    }
    if (c->log->log_level >= 2)
      ngx_log_error_core(2, c->log, 0, "the http output chain is empty");
    ngx_debug_point();
    return -1;
  }
  if (r->limit_rate)
  {
    if (r->limit_rate_after == 0)
    {
      r->limit_rate_after = clcf->limit_rate_after;
    }
    limit = (((off_t) r->limit_rate) * ((get_ngx_cached_time_sec_wrapper() - r->start_sec) + 1)) - (c->sent - r->limit_rate_after);
    if (limit <= 0)
    {
      c->write->delayed = 1;
      delay = (ngx_msec_t) ((((-limit) * 1000) / r->limit_rate) + 1);
      ngx_event_add_timer(c->write, delay);
      c->buffered |= 0x10;
      return -2;
    }
    if (clcf->sendfile_max_chunk && (((off_t) clcf->sendfile_max_chunk) < limit))
    {
      limit = clcf->sendfile_max_chunk;
    }
  }
  else
  {
    limit = clcf->sendfile_max_chunk;
  }
  sent = c->sent;
  ;
  chain = c->send_chain(c, r->out, limit);
  ;
  if (chain == ((ngx_chain_t *) (-1)))
  {
    c->error = 1;
    return -1;
  }
  if (r->limit_rate)
  {
    nsent = c->sent;
    if (r->limit_rate_after)
    {
      sent -= r->limit_rate_after;
      if (sent < 0)
      {
        sent = 0;
      }
      nsent -= r->limit_rate_after;
      if (nsent < 0)
      {
        nsent = 0;
      }
    }
    delay = (ngx_msec_t) (((nsent - sent) * 1000) / r->limit_rate);
    if (delay > 0)
    {
      limit = 0;
      c->write->delayed = 1;
      ngx_event_add_timer(c->write, delay);
    }
  }
  if ((limit && c->write->ready) && ((c->sent - sent) >= (limit - ((off_t) (2 * get_ngx_pagesize_wrapper())))))
  {
    c->write->delayed = 1;
    ngx_event_add_timer(c->write, 1);
  }
  for (cl = r->out; cl && (cl != chain);)
  {
    ln = cl;
    cl = cl->next;
    ln->next = r->pool->chain;
    r->pool->chain = ln;
  }

  r->out = chain;
  if (chain)
  {
    c->buffered |= 0x10;
    return -2;
  }
  c->buffered &= ~0x10;
  if ((c->buffered & 0x0f) && (r->postponed == 0))
  {
    return -2;
  }
  return 0;
}




static ngx_int_t ngx_http_write_filter_init(ngx_conf_t *cf)
{
  set_ngx_http_top_body_filter_wrapper(ngx_http_write_filter);
  return 0;
}


