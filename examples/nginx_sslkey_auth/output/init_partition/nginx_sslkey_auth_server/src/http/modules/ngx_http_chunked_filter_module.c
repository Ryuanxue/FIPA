
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    ngx_chain_t         *free;
    ngx_chain_t         *busy;
} ngx_http_chunked_filter_ctx_t;


static ngx_int_t ngx_http_chunked_filter_init(ngx_conf_t *cf);
static ngx_chain_t *ngx_http_chunked_create_trailers(ngx_http_request_t *r,
    ngx_http_chunked_filter_ctx_t *ctx);


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


ngx_int_t ngx_http_chunked_header_filter_sense_1(int *ngx_http_chunked_header_filter_sense_1_ret, ngx_http_request_t *r)
{
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_chunked_filter_ctx_t *ctx;
  if (((((r->headers_out.status == 304) || (r->headers_out.status == 204)) || (r->headers_out.status < 200)) || (r != r->main)) || (r->method == 0x0004))
  {
    {
      *ngx_http_chunked_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
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
        {
          *ngx_http_chunked_header_filter_sense_1_ret = 1;
          return -1;
        }
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
  {
    *ngx_http_chunked_header_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}







static ngx_int_t
ngx_http_chunked_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_chunked_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_chunked_body_filter;

    return NGX_OK;
}
