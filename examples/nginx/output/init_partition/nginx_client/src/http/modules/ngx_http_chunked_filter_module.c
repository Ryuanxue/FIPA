
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "nginx_rpc_wrapper.h"


typedef struct {
    ngx_chain_t         *free;
    ngx_chain_t         *busy;
} ngx_http_chunked_filter_ctx_t;


static ngx_int_t ngx_http_chunked_filter_init(ngx_conf_t *cf);
static ngx_chain_t *ngx_http_chunked_create_trailers(ngx_http_request_t *r,
    ngx_http_chunked_filter_ctx_t *ctx);


static ngx_http_module_t  ngx_http_chunked_filter_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_chunked_filter_init,          /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_chunked_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_chunked_filter_module_ctx,   /* module context */
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


static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt    ngx_http_next_body_filter;


static ngx_int_t ngx_http_chunked_header_filter(ngx_http_request_t *r)
{
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_chunked_filter_ctx_t *ctx;
  if (((((r->headers_out.status == 304) || (r->headers_out.status == 204)) || (r->headers_out.status < 200)) || (r != r->main)) || (r->method == 0x0004))
  {
    return get_ngx_http_next_header_filter_wrapper()(r);
  }
  if ((r->headers_out.content_length_n == (-1)) || r->expect_trailers)
  {
    clcf = r->loc_conf[ngx_http_core_module.ctx_index];
    if ((r->http_version >= 1001) && clcf->chunked_transfer_encoding)
    {
      if (r->expect_trailers)
      {
        r->headers_out.content_length_n = -1;
        if (r->headers_out.content_length)
        {
          r->headers_out.content_length->hash = 0;
          r->headers_out.content_length = 0;
        }
        ;
      }
      r->chunked = 1;
      ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_chunked_filter_ctx_t));
      if (ctx == 0)
      {
        return -1;
      }
      r->ctx[ngx_http_chunked_filter_module.ctx_index] = ctx;
      ;
    }
    else
      if (r->headers_out.content_length_n == (-1))
    {
      r->keepalive = 0;
    }
  }
  return get_ngx_http_next_header_filter_wrapper()(r);
}




static ngx_int_t ngx_http_chunked_body_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
  {
    int ngx_http_chunked_body_filter_sense_1_ret = 0;
    ngx_int_t ngx_http_chunked_body_filter_sense_1_return = ngx_http_chunked_body_filter_sense_1_wrapper(&ngx_http_chunked_body_filter_sense_1_ret, r, in);
    if (ngx_http_chunked_body_filter_sense_1_ret)
      return ngx_http_chunked_body_filter_sense_1_return;
  }
}




static ngx_chain_t *
ngx_http_chunked_create_trailers(ngx_http_request_t *r,
    ngx_http_chunked_filter_ctx_t *ctx)
{
    size_t            len;
    ngx_buf_t        *b;
    ngx_uint_t        i;
    ngx_chain_t      *cl;
    ngx_list_part_t  *part;
    ngx_table_elt_t  *header;

    len = 0;

    part = &r->headers_out.trailers.part;
    header = part->elts;

    for (i = 0; /* void */; i++) {

        if (i >= part->nelts) {
            if (part->next == NULL) {
                break;
            }

            part = part->next;
            header = part->elts;
            i = 0;
        }

        if (header[i].hash == 0) {
            continue;
        }

        len += header[i].key.len + sizeof(": ") - 1
               + header[i].value.len + sizeof(CRLF) - 1;
    }

    cl = ngx_chain_get_free_buf(r->pool, &ctx->free);
    if (cl == NULL) {
        return NULL;
    }

    b = cl->buf;

    b->tag = (ngx_buf_tag_t) &ngx_http_chunked_filter_module;
    b->temporary = 0;
    b->memory = 1;
    b->last_buf = 1;

    if (len == 0) {
        b->pos = (u_char *) CRLF "0" CRLF CRLF;
        b->last = b->pos + sizeof(CRLF "0" CRLF CRLF) - 1;
        return cl;
    }

    len += sizeof(CRLF "0" CRLF CRLF) - 1;

    b->pos = ngx_palloc(r->pool, len);
    if (b->pos == NULL) {
        return NULL;
    }

    b->last = b->pos;

    *b->last++ = CR; *b->last++ = LF;
    *b->last++ = '0';
    *b->last++ = CR; *b->last++ = LF;

    part = &r->headers_out.trailers.part;
    header = part->elts;

    for (i = 0; /* void */; i++) {

        if (i >= part->nelts) {
            if (part->next == NULL) {
                break;
            }

            part = part->next;
            header = part->elts;
            i = 0;
        }

        if (header[i].hash == 0) {
            continue;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "http trailer: \"%V: %V\"",
                       &header[i].key, &header[i].value);

        b->last = ngx_copy(b->last, header[i].key.data, header[i].key.len);
        *b->last++ = ':'; *b->last++ = ' ';

        b->last = ngx_copy(b->last, header[i].value.data, header[i].value.len);
        *b->last++ = CR; *b->last++ = LF;
    }

    *b->last++ = CR; *b->last++ = LF;

    return cl;
}


static ngx_int_t ngx_http_chunked_filter_init(ngx_conf_t *cf)
{
  set_ngx_http_next_header_filter_wrapper(ngx_http_top_header_filter);
  set_ngx_http_top_header_filter_wrapper(ngx_http_chunked_header_filter);
  set_ngx_http_next_body_filter_wrapper(ngx_http_top_body_filter);
  set_ngx_http_top_body_filter_wrapper(ngx_http_chunked_body_filter);
  return 0;
}


