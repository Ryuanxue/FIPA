
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


/*
 * the single part format:
 *
 * "HTTP/1.0 206 Partial Content" CRLF
 * ... header ...
 * "Content-Type: image/jpeg" CRLF
 * "Content-Length: SIZE" CRLF
 * "Content-Range: bytes START-END/SIZE" CRLF
 * CRLF
 * ... data ...
 *
 *
 * the multipart format:
 *
 * "HTTP/1.0 206 Partial Content" CRLF
 * ... header ...
 * "Content-Type: multipart/byteranges; boundary=0123456789" CRLF
 * CRLF
 * CRLF
 * "--0123456789" CRLF
 * "Content-Type: image/jpeg" CRLF
 * "Content-Range: bytes START0-END0/SIZE" CRLF
 * CRLF
 * ... data ...
 * CRLF
 * "--0123456789" CRLF
 * "Content-Type: image/jpeg" CRLF
 * "Content-Range: bytes START1-END1/SIZE" CRLF
 * CRLF
 * ... data ...
 * CRLF
 * "--0123456789--" CRLF
 */


typedef struct {
    off_t        start;
    off_t        end;
    ngx_str_t    content_range;
} ngx_http_range_t;


typedef struct {
    off_t        offset;
    ngx_str_t    boundary_header;
    ngx_array_t  ranges;
} ngx_http_range_filter_ctx_t;


static ngx_int_t ngx_http_range_parse(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_uint_t ranges);
static ngx_int_t ngx_http_range_singlepart_header(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx);
static ngx_int_t ngx_http_range_multipart_header(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx);
static ngx_int_t ngx_http_range_not_satisfiable(ngx_http_request_t *r);
static ngx_int_t ngx_http_range_test_overlapped(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_chain_t *in);
static ngx_int_t ngx_http_range_singlepart_body(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_chain_t *in);
static ngx_int_t ngx_http_range_multipart_body(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx, ngx_chain_t *in);

static ngx_int_t ngx_http_range_header_filter_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_range_body_filter_init(ngx_conf_t *cf);


    NULL,                                  /* preconfiguration */
    ngx_http_range_header_filter_init,     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL,                                  /* merge location configuration */
};


    NGX_MODULE_V1,
    &ngx_http_range_header_filter_module_ctx, /* module context */
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


    NULL,                                  /* preconfiguration */
    ngx_http_range_body_filter_init,       /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL,                                  /* merge location configuration */
};


ngx_module_t  ngx_http_range_body_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_range_body_filter_module_ctx, /* module context */
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


ngx_int_t ngx_http_range_header_filter_sense_1(int *ngx_http_range_header_filter_sense_1_ret, ngx_http_request_t *r)
{
  time_t if_range_time;
  ngx_str_t *if_range;
  ngx_str_t *etag;
  ngx_uint_t ranges;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_range_filter_ctx_t *ctx;
  if (((((r->http_version < 1000) || (r->headers_out.status != 200)) || ((r != r->main) && (!r->subrequest_ranges))) || (r->headers_out.content_length_n == (-1))) || (!r->allow_ranges))
  {
    {
      *ngx_http_range_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  clcf = r->loc_conf[ngx_http_core_module.ctx_index];
  if (clcf->max_ranges == 0)
  {
    {
      *ngx_http_range_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  if (((r->headers_in.range == 0) || (r->headers_in.range->value.len < 7)) || (ngx_strncasecmp(r->headers_in.range->value.data, (u_char *) "bytes=", 6) != 0))
  {
    goto next_filter;
  }
  if (r->headers_in.if_range)
  {
    if_range = &r->headers_in.if_range->value;
    if ((if_range->len >= 2) && (if_range->data[if_range->len - 1] == '"'))
    {
      if (r->headers_out.etag == 0)
      {
        goto next_filter;
      }
      etag = &r->headers_out.etag->value;
      ;
      if ((if_range->len != etag->len) || (strncmp((const char *) if_range->data, (const char *) etag->data, etag->len) != 0))
      {
        goto next_filter;
      }
      goto parse;
    }
    if (r->headers_out.last_modified_time == ((time_t) (-1)))
    {
      goto next_filter;
    }
    if_range_time = ngx_parse_http_time(if_range->data, if_range->len);
    ;
    if (if_range_time != r->headers_out.last_modified_time)
    {
      goto next_filter;
    }
  }
  parse:
  ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_range_filter_ctx_t));

  if (ctx == 0)
  {
    {
      *ngx_http_range_header_filter_sense_1_ret = 1;
      return -1;
    }
  }
  ctx->offset = r->headers_out.content_offset;
  ranges = (r->single_range) ? (1) : (clcf->max_ranges);
  switch (ngx_http_range_parse(r, ctx, ranges))
  {
    case 0:
      r->ctx[ngx_http_range_body_filter_module.ctx_index] = ctx;
      ;
      r->headers_out.status = 206;
      r->headers_out.status_line.len = 0;
      if (ctx->ranges.nelts == 1)
    {
      {
        *ngx_http_range_header_filter_sense_1_ret = 1;
        return ngx_http_range_singlepart_header(r, ctx);
      }
    }
    {
      *ngx_http_range_header_filter_sense_1_ret = 1;
      return ngx_http_range_multipart_header(r, ctx);
    }

    case 416:
    {
      *ngx_http_range_header_filter_sense_1_ret = 1;
      return ngx_http_range_not_satisfiable(r);
    }

    case -1:
    {
      *ngx_http_range_header_filter_sense_1_ret = 1;
      return -1;
    }

    default:
      break;

  }

  next_filter:
  r->headers_out.accept_ranges = ngx_list_push(&r->headers_out.headers);

  if (r->headers_out.accept_ranges == 0)
  {
    {
      *ngx_http_range_header_filter_sense_1_ret = 1;
      return -1;
    }
  }
  r->headers_out.accept_ranges->hash = 1;
  (&r->headers_out.accept_ranges->key)->len = (sizeof("Accept-Ranges")) - 1;
  (&r->headers_out.accept_ranges->key)->data = (u_char *) "Accept-Ranges";
  (&r->headers_out.accept_ranges->value)->len = (sizeof("bytes")) - 1;
  (&r->headers_out.accept_ranges->value)->data = (u_char *) "bytes";
  {
    *ngx_http_range_header_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}



static ngx_int_t
ngx_http_range_parse(ngx_http_request_t *r, ngx_http_range_filter_ctx_t *ctx,
    ngx_uint_t ranges)
{
    u_char                       *p;
    off_t                         start, end, size, content_length, cutoff,
                                  cutlim;
    ngx_uint_t                    suffix;
    ngx_http_range_t             *range;
    ngx_http_range_filter_ctx_t  *mctx;

    if (r != r->main) {
        mctx = ngx_http_get_module_ctx(r->main,
                                       ngx_http_range_body_filter_module);
        if (mctx) {
            ctx->ranges = mctx->ranges;
            return NGX_OK;
        }
    }

    if (ngx_array_init(&ctx->ranges, r->pool, 1, sizeof(ngx_http_range_t))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    p = r->headers_in.range->value.data + 6;
    size = 0;
    content_length = r->headers_out.content_length_n;

    cutoff = NGX_MAX_OFF_T_VALUE / 10;
    cutlim = NGX_MAX_OFF_T_VALUE % 10;

    for ( ;; ) {
        start = 0;
        end = 0;
        suffix = 0;

        while (*p == ' ') { p++; }

        if (*p != '-') {
            if (*p < '0' || *p > '9') {
                return NGX_HTTP_RANGE_NOT_SATISFIABLE;
            }

            while (*p >= '0' && *p <= '9') {
                if (start >= cutoff && (start > cutoff || *p - '0' > cutlim)) {
                    return NGX_HTTP_RANGE_NOT_SATISFIABLE;
                }

                start = start * 10 + (*p++ - '0');
            }

            while (*p == ' ') { p++; }

            if (*p++ != '-') {
                return NGX_HTTP_RANGE_NOT_SATISFIABLE;
            }

            while (*p == ' ') { p++; }

            if (*p == ',' || *p == '\0') {
                end = content_length;
                goto found;
            }

        } else {
            suffix = 1;
            p++;
        }

        if (*p < '0' || *p > '9') {
            return NGX_HTTP_RANGE_NOT_SATISFIABLE;
        }

        while (*p >= '0' && *p <= '9') {
            if (end >= cutoff && (end > cutoff || *p - '0' > cutlim)) {
                return NGX_HTTP_RANGE_NOT_SATISFIABLE;
            }

            end = end * 10 + (*p++ - '0');
        }

        while (*p == ' ') { p++; }

        if (*p != ',' && *p != '\0') {
            return NGX_HTTP_RANGE_NOT_SATISFIABLE;
        }

        if (suffix) {
            start = (end < content_length) ? content_length - end : 0;
            end = content_length - 1;
        }

        if (end >= content_length) {
            end = content_length;

        } else {
            end++;
        }

    found:

        if (start < end) {
            range = ngx_array_push(&ctx->ranges);
            if (range == NULL) {
                return NGX_ERROR;
            }

            range->start = start;
            range->end = end;

            if (size > NGX_MAX_OFF_T_VALUE - (end - start)) {
                return NGX_HTTP_RANGE_NOT_SATISFIABLE;
            }

            size += end - start;

            if (ranges-- == 0) {
                return NGX_DECLINED;
            }

        } else if (start == 0) {
            return NGX_DECLINED;
        }

        if (*p++ != ',') {
            break;
        }
    }

    if (ctx->ranges.nelts == 0) {
        return NGX_HTTP_RANGE_NOT_SATISFIABLE;
    }

    if (size > content_length) {
        return NGX_DECLINED;
    }

    return NGX_OK;
}


static ngx_int_t
ngx_http_range_singlepart_header(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx)
{
    ngx_table_elt_t   *content_range;
    ngx_http_range_t  *range;

    if (r != r->main) {
        return ngx_http_next_header_filter(r);
    }

    content_range = ngx_list_push(&r->headers_out.headers);
    if (content_range == NULL) {
        return NGX_ERROR;
    }

    r->headers_out.content_range = content_range;

    content_range->hash = 1;
    ngx_str_set(&content_range->key, "Content-Range");

    content_range->value.data = ngx_pnalloc(r->pool,
                                    sizeof("bytes -/") - 1 + 3 * NGX_OFF_T_LEN);
    if (content_range->value.data == NULL) {
        content_range->hash = 0;
        r->headers_out.content_range = NULL;
        return NGX_ERROR;
    }

    /* "Content-Range: bytes SSSS-EEEE/TTTT" header */

    range = ctx->ranges.elts;

    content_range->value.len = ngx_sprintf(content_range->value.data,
                                           "bytes %O-%O/%O",
                                           range->start, range->end - 1,
                                           r->headers_out.content_length_n)
                               - content_range->value.data;

    r->headers_out.content_length_n = range->end - range->start;
    r->headers_out.content_offset = range->start;

    if (r->headers_out.content_length) {
        r->headers_out.content_length->hash = 0;
        r->headers_out.content_length = NULL;
    }

    return ngx_http_next_header_filter(r);
}


static ngx_int_t
ngx_http_range_multipart_header(ngx_http_request_t *r,
    ngx_http_range_filter_ctx_t *ctx)
{
    off_t               len;
    size_t              size;
    ngx_uint_t          i;
    ngx_http_range_t   *range;
    ngx_atomic_uint_t   boundary;

    size = sizeof(CRLF "--") - 1 + NGX_ATOMIC_T_LEN
           + sizeof(CRLF "Content-Type: ") - 1
           + r->headers_out.content_type.len
           + sizeof(CRLF "Content-Range: bytes ") - 1;

    if (r->headers_out.content_type_len == r->headers_out.content_type.len
        && r->headers_out.charset.len)
    {
        size += sizeof("; charset=") - 1 + r->headers_out.charset.len;
    }

    ctx->boundary_header.data = ngx_pnalloc(r->pool, size);
    if (ctx->boundary_header.data == NULL) {
        return NGX_ERROR;
    }

    boundary = ngx_next_temp_number(0);

    /*
     * The boundary header of the range:
     * CRLF
     * "--0123456789" CRLF
     * "Content-Type: image/jpeg" CRLF
     * "Content-Range: bytes "
     */

    if (r->headers_out.content_type_len == r->headers_out.content_type.len
        && r->headers_out.charset.len)
    {
        ctx->boundary_header.len = ngx_sprintf(ctx->boundary_header.data,
                                           CRLF "--%0muA" CRLF
                                           "Content-Type: %V; charset=%V" CRLF
                                           "Content-Range: bytes ",
                                           boundary,
                                           &r->headers_out.content_type,
                                           &r->headers_out.charset)
                                   - ctx->boundary_header.data;

    } else if (r->headers_out.content_type.len) {
        ctx->boundary_header.len = ngx_sprintf(ctx->boundary_header.data,
                                           CRLF "--%0muA" CRLF
                                           "Content-Type: %V" CRLF
                                           "Content-Range: bytes ",
                                           boundary,
                                           &r->headers_out.content_type)
                                   - ctx->boundary_header.data;

    } else {
        ctx->boundary_header.len = ngx_sprintf(ctx->boundary_header.data,
                                           CRLF "--%0muA" CRLF
                                           "Content-Range: bytes ",
                                           boundary)
                                   - ctx->boundary_header.data;
    }

    r->headers_out.content_type.data =
        ngx_pnalloc(r->pool,
                    sizeof("Content-Type: multipart/byteranges; boundary=") - 1
                    + NGX_ATOMIC_T_LEN);

    if (r->headers_out.content_type.data == NULL) {
        return NGX_ERROR;
    }

    r->headers_out.content_type_lowcase = NULL;

    /* "Content-Type: multipart/byteranges; boundary=0123456789" */

    r->headers_out.content_type.len =
                           ngx_sprintf(r->headers_out.content_type.data,
                                       "multipart/byteranges; boundary=%0muA",
                                       boundary)
                           - r->headers_out.content_type.data;

    r->headers_out.content_type_len = r->headers_out.content_type.len;

    r->headers_out.charset.len = 0;

    /* the size of the last boundary CRLF "--0123456789--" CRLF */

    len = sizeof(CRLF "--") - 1 + NGX_ATOMIC_T_LEN + sizeof("--" CRLF) - 1;

    range = ctx->ranges.elts;
    for (i = 0; i < ctx->ranges.nelts; i++) {

        /* the size of the range: "SSSS-EEEE/TTTT" CRLF CRLF */

        range[i].content_range.data =
                               ngx_pnalloc(r->pool, 3 * NGX_OFF_T_LEN + 2 + 4);

        if (range[i].content_range.data == NULL) {
            return NGX_ERROR;
        }

        range[i].content_range.len = ngx_sprintf(range[i].content_range.data,
                                               "%O-%O/%O" CRLF CRLF,
                                               range[i].start, range[i].end - 1,
                                               r->headers_out.content_length_n)
                                     - range[i].content_range.data;

        len += ctx->boundary_header.len + range[i].content_range.len
                                             + (range[i].end - range[i].start);
    }

    r->headers_out.content_length_n = len;

    if (r->headers_out.content_length) {
        r->headers_out.content_length->hash = 0;
        r->headers_out.content_length = NULL;
    }

    return ngx_http_next_header_filter(r);
}


static ngx_int_t
ngx_http_range_not_satisfiable(ngx_http_request_t *r)
{
    ngx_table_elt_t  *content_range;

    r->headers_out.status = NGX_HTTP_RANGE_NOT_SATISFIABLE;

    content_range = ngx_list_push(&r->headers_out.headers);
    if (content_range == NULL) {
        return NGX_ERROR;
    }

    r->headers_out.content_range = content_range;

    content_range->hash = 1;
    ngx_str_set(&content_range->key, "Content-Range");

    content_range->value.data = ngx_pnalloc(r->pool,
                                       sizeof("bytes */") - 1 + NGX_OFF_T_LEN);
    if (content_range->value.data == NULL) {
        content_range->hash = 0;
        r->headers_out.content_range = NULL;
        return NGX_ERROR;
    }

    content_range->value.len = ngx_sprintf(content_range->value.data,
                                           "bytes */%O",
                                           r->headers_out.content_length_n)
                               - content_range->value.data;

    ngx_http_clear_content_length(r);

    return NGX_HTTP_RANGE_NOT_SATISFIABLE;
}










static ngx_int_t
ngx_http_range_header_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_range_header_filter;

    return NGX_OK;
}


static ngx_int_t
ngx_http_range_body_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_range_body_filter;

    return NGX_OK;
}
