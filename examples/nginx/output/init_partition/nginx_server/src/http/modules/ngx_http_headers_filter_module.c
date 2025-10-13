
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct ngx_http_header_val_s  ngx_http_header_val_t;

typedef ngx_int_t (*ngx_http_set_header_pt)(ngx_http_request_t *r,
    ngx_http_header_val_t *hv, ngx_str_t *value);


typedef struct {
    ngx_str_t                  name;
    ngx_uint_t                 offset;
    ngx_http_set_header_pt     handler;
} ngx_http_set_header_t;


struct ngx_http_header_val_s {
    ngx_http_complex_value_t   value;
    ngx_str_t                  key;
    ngx_http_set_header_pt     handler;
    ngx_uint_t                 offset;
    ngx_uint_t                 always;  /* unsigned  always:1 */
};


typedef enum {
    NGX_HTTP_EXPIRES_OFF,
    NGX_HTTP_EXPIRES_EPOCH,
    NGX_HTTP_EXPIRES_MAX,
    NGX_HTTP_EXPIRES_ACCESS,
    NGX_HTTP_EXPIRES_MODIFIED,
    NGX_HTTP_EXPIRES_DAILY,
    NGX_HTTP_EXPIRES_UNSET
} ngx_http_expires_t;


typedef struct {
    ngx_http_expires_t         expires;
    time_t                     expires_time;
    ngx_http_complex_value_t  *expires_value;
    ngx_array_t               *headers;
    ngx_array_t               *trailers;
} ngx_http_headers_conf_t;


static ngx_int_t ngx_http_set_expires(ngx_http_request_t *r,
    ngx_http_headers_conf_t *conf);
static ngx_int_t ngx_http_parse_expires(ngx_str_t *value,
    ngx_http_expires_t *expires, time_t *expires_time, char **err);
static ngx_int_t ngx_http_add_multi_header_lines(ngx_http_request_t *r,
    ngx_http_header_val_t *hv, ngx_str_t *value);
static ngx_int_t ngx_http_add_header(ngx_http_request_t *r,
    ngx_http_header_val_t *hv, ngx_str_t *value);
static ngx_int_t ngx_http_set_last_modified(ngx_http_request_t *r,
    ngx_http_header_val_t *hv, ngx_str_t *value);
static ngx_int_t ngx_http_set_response_header(ngx_http_request_t *r,
    ngx_http_header_val_t *hv, ngx_str_t *value);

static void *ngx_http_headers_create_conf(ngx_conf_t *cf);
static char *ngx_http_headers_merge_conf(ngx_conf_t *cf,
    void *parent, void *child);
static ngx_int_t ngx_http_headers_filter_init(ngx_conf_t *cf);
static char *ngx_http_headers_expires(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_headers_add(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


static ngx_http_set_header_t  ngx_http_set_headers[] = {

    { ngx_string("Cache-Control"),
                 offsetof(ngx_http_headers_out_t, cache_control),
                 ngx_http_add_multi_header_lines },

    { ngx_string("Link"),
                 offsetof(ngx_http_headers_out_t, link),
                 ngx_http_add_multi_header_lines },

    { ngx_string("Last-Modified"),
                 offsetof(ngx_http_headers_out_t, last_modified),
                 ngx_http_set_last_modified },

    { ngx_string("ETag"),
                 offsetof(ngx_http_headers_out_t, etag),
                 ngx_http_set_response_header },

    { ngx_null_string, 0, NULL }
};


static ngx_command_t  ngx_http_headers_filter_commands[] = {

    { ngx_string("expires"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_TAKE12,
      ngx_http_headers_expires,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("add_header"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_TAKE23,
      ngx_http_headers_add,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_headers_conf_t, headers),
      NULL },

    { ngx_string("add_trailer"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_TAKE23,
      ngx_http_headers_add,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_headers_conf_t, trailers),
      NULL },

      ngx_null_command
};


static ngx_http_module_t  ngx_http_headers_filter_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_headers_filter_init,          /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_headers_create_conf,          /* create location configuration */
    ngx_http_headers_merge_conf            /* merge location configuration */
};


ngx_module_t  ngx_http_headers_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_headers_filter_module_ctx,   /* module context */
    ngx_http_headers_filter_commands,      /* module directives */
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


ngx_int_t ngx_http_headers_filter_sense_1(int *ngx_http_headers_filter_sense_1_ret, ngx_http_request_t *r)
{
  ngx_str_t value;
  ngx_uint_t i;
  ngx_uint_t safe_status;
  ngx_http_header_val_t *h;
  ngx_http_headers_conf_t *conf;
  if (r != r->main)
  {
    {
      *ngx_http_headers_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  conf = r->loc_conf[ngx_http_headers_filter_module.ctx_index];
  if (((conf->expires == NGX_HTTP_EXPIRES_OFF) && (conf->headers == 0)) && (conf->trailers == 0))
  {
    {
      *ngx_http_headers_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  switch (r->headers_out.status)
  {
    case 200:

    case 201:

    case 204:

    case 206:

    case 301:

    case 302:

    case 303:

    case 304:

    case 307:

    case 308:
      safe_status = 1;
      break;

    default:
      safe_status = 0;
      break;

  }

  if ((conf->expires != NGX_HTTP_EXPIRES_OFF) && safe_status)
  {
    if (ngx_http_set_expires(r, conf) != 0)
    {
      {
        *ngx_http_headers_filter_sense_1_ret = 1;
        return -1;
      }
    }
  }
  if (conf->headers)
  {
    h = conf->headers->elts;
    for (i = 0; i < conf->headers->nelts; i++)
    {
      if ((!safe_status) && (!h[i].always))
      {
        continue;
      }
      if (ngx_http_complex_value(r, &h[i].value, &value) != 0)
      {
        {
          *ngx_http_headers_filter_sense_1_ret = 1;
          return -1;
        }
      }
      if (h[i].handler(r, &h[i], &value) != 0)
      {
        {
          *ngx_http_headers_filter_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }
  if (conf->trailers)
  {
    h = conf->trailers->elts;
    for (i = 0; i < conf->trailers->nelts; i++)
    {
      if ((!safe_status) && (!h[i].always))
      {
        continue;
      }
      r->expect_trailers = 1;
      break;
    }

  }
  {
    *ngx_http_headers_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}




ngx_int_t ngx_http_trailers_filter_sense_1(int *ngx_http_trailers_filter_sense_1_ret, ngx_http_request_t *r, ngx_chain_t *in)
{
  ngx_str_t value;
  ngx_uint_t i;
  ngx_uint_t safe_status;
  ngx_chain_t *cl;
  ngx_table_elt_t *t;
  ngx_http_header_val_t *h;
  ngx_http_headers_conf_t *conf;
  conf = r->loc_conf[ngx_http_headers_filter_module.ctx_index];
  if ((((in == 0) || (conf->trailers == 0)) || (!r->expect_trailers)) || r->header_only)
  {
    {
      *ngx_http_trailers_filter_sense_1_ret = 1;
      return ngx_http_next_body_filter(r, in);
    }
  }
  for (cl = in; cl; cl = cl->next)
  {
    if (cl->buf->last_buf)
    {
      break;
    }
  }

  if (cl == 0)
  {
    {
      *ngx_http_trailers_filter_sense_1_ret = 1;
      return ngx_http_next_body_filter(r, in);
    }
  }
  switch (r->headers_out.status)
  {
    case 200:

    case 201:

    case 204:

    case 206:

    case 301:

    case 302:

    case 303:

    case 304:

    case 307:

    case 308:
      safe_status = 1;
      break;

    default:
      safe_status = 0;
      break;

  }

  h = conf->trailers->elts;
  for (i = 0; i < conf->trailers->nelts; i++)
  {
    if ((!safe_status) && (!h[i].always))
    {
      continue;
    }
    if (ngx_http_complex_value(r, &h[i].value, &value) != 0)
    {
      {
        *ngx_http_trailers_filter_sense_1_ret = 1;
        return -1;
      }
    }
    if (value.len)
    {
      t = ngx_list_push(&r->headers_out.trailers);
      if (t == 0)
      {
        {
          *ngx_http_trailers_filter_sense_1_ret = 1;
          return -1;
        }
      }
      t->key = h[i].key;
      t->value = value;
      t->hash = 1;
    }
  }

  {
    *ngx_http_trailers_filter_sense_1_ret = 1;
    return ngx_http_next_body_filter(r, in);
  }
}




static ngx_int_t
ngx_http_set_expires(ngx_http_request_t *r, ngx_http_headers_conf_t *conf)
{
    char                *err;
    size_t               len;
    time_t               now, expires_time, max_age;
    ngx_str_t            value;
    ngx_int_t            rc;
    ngx_uint_t           i;
    ngx_table_elt_t     *e, *cc, **ccp;
    ngx_http_expires_t   expires;

    expires = conf->expires;
    expires_time = conf->expires_time;

    if (conf->expires_value != NULL) {

        if (ngx_http_complex_value(r, conf->expires_value, &value) != NGX_OK) {
            return NGX_ERROR;
        }

        rc = ngx_http_parse_expires(&value, &expires, &expires_time, &err);

        if (rc != NGX_OK) {
            return NGX_OK;
        }

        if (expires == NGX_HTTP_EXPIRES_OFF) {
            return NGX_OK;
        }
    }

    e = r->headers_out.expires;

    if (e == NULL) {

        e = ngx_list_push(&r->headers_out.headers);
        if (e == NULL) {
            return NGX_ERROR;
        }

        r->headers_out.expires = e;

        e->hash = 1;
        ngx_str_set(&e->key, "Expires");
    }

    len = sizeof("Mon, 28 Sep 1970 06:00:00 GMT");
    e->value.len = len - 1;

    ccp = r->headers_out.cache_control.elts;

    if (ccp == NULL) {

        if (ngx_array_init(&r->headers_out.cache_control, r->pool,
                           1, sizeof(ngx_table_elt_t *))
            != NGX_OK)
        {
            return NGX_ERROR;
        }

        cc = ngx_list_push(&r->headers_out.headers);
        if (cc == NULL) {
            return NGX_ERROR;
        }

        cc->hash = 1;
        ngx_str_set(&cc->key, "Cache-Control");

        ccp = ngx_array_push(&r->headers_out.cache_control);
        if (ccp == NULL) {
            return NGX_ERROR;
        }

        *ccp = cc;

    } else {
        for (i = 1; i < r->headers_out.cache_control.nelts; i++) {
            ccp[i]->hash = 0;
        }

        cc = ccp[0];
    }

    if (expires == NGX_HTTP_EXPIRES_EPOCH) {
        e->value.data = (u_char *) "Thu, 01 Jan 1970 00:00:01 GMT";
        ngx_str_set(&cc->value, "no-cache");
        return NGX_OK;
    }

    if (expires == NGX_HTTP_EXPIRES_MAX) {
        e->value.data = (u_char *) "Thu, 31 Dec 2037 23:55:55 GMT";
        /* 10 years */
        ngx_str_set(&cc->value, "max-age=315360000");
        return NGX_OK;
    }

    e->value.data = ngx_pnalloc(r->pool, len);
    if (e->value.data == NULL) {
        return NGX_ERROR;
    }

    if (expires_time == 0 && expires != NGX_HTTP_EXPIRES_DAILY) {
        ngx_memcpy(e->value.data, ngx_cached_http_time.data,
                   ngx_cached_http_time.len + 1);
        ngx_str_set(&cc->value, "max-age=0");
        return NGX_OK;
    }

    now = ngx_time();

    if (expires == NGX_HTTP_EXPIRES_DAILY) {
        expires_time = ngx_next_time(expires_time);
        max_age = expires_time - now;

    } else if (expires == NGX_HTTP_EXPIRES_ACCESS
               || r->headers_out.last_modified_time == -1)
    {
        max_age = expires_time;
        expires_time += now;

    } else {
        expires_time += r->headers_out.last_modified_time;
        max_age = expires_time - now;
    }

    ngx_http_time(e->value.data, expires_time);

    if (conf->expires_time < 0 || max_age < 0) {
        ngx_str_set(&cc->value, "no-cache");
        return NGX_OK;
    }

    cc->value.data = ngx_pnalloc(r->pool,
                                 sizeof("max-age=") + NGX_TIME_T_LEN + 1);
    if (cc->value.data == NULL) {
        return NGX_ERROR;
    }

    cc->value.len = ngx_sprintf(cc->value.data, "max-age=%T", max_age)
                    - cc->value.data;

    return NGX_OK;
}


static ngx_int_t
ngx_http_parse_expires(ngx_str_t *value, ngx_http_expires_t *expires,
    time_t *expires_time, char **err)
{
    ngx_uint_t  minus;

    if (*expires != NGX_HTTP_EXPIRES_MODIFIED) {

        if (value->len == 5 && ngx_strncmp(value->data, "epoch", 5) == 0) {
            *expires = NGX_HTTP_EXPIRES_EPOCH;
            return NGX_OK;
        }

        if (value->len == 3 && ngx_strncmp(value->data, "max", 3) == 0) {
            *expires = NGX_HTTP_EXPIRES_MAX;
            return NGX_OK;
        }

        if (value->len == 3 && ngx_strncmp(value->data, "off", 3) == 0) {
            *expires = NGX_HTTP_EXPIRES_OFF;
            return NGX_OK;
        }
    }

    if (value->len && value->data[0] == '@') {
        value->data++;
        value->len--;
        minus = 0;

        if (*expires == NGX_HTTP_EXPIRES_MODIFIED) {
            *err = "daily time cannot be used with \"modified\" parameter";
            return NGX_ERROR;
        }

        *expires = NGX_HTTP_EXPIRES_DAILY;

    } else if (value->len && value->data[0] == '+') {
        value->data++;
        value->len--;
        minus = 0;

    } else if (value->len && value->data[0] == '-') {
        value->data++;
        value->len--;
        minus = 1;

    } else {
        minus = 0;
    }

    *expires_time = ngx_parse_time(value, 1);

    if (*expires_time == (time_t) NGX_ERROR) {
        *err = "invalid value";
        return NGX_ERROR;
    }

    if (*expires == NGX_HTTP_EXPIRES_DAILY
        && *expires_time > 24 * 60 * 60)
    {
        *err = "daily time value must be less than 24 hours";
        return NGX_ERROR;
    }

    if (minus) {
        *expires_time = - *expires_time;
    }

    return NGX_OK;
}











static ngx_int_t
ngx_http_set_response_header(ngx_http_request_t *r, ngx_http_header_val_t *hv,
    ngx_str_t *value)
{
    ngx_table_elt_t  *h, **old;

    old = (ngx_table_elt_t **) ((char *) &r->headers_out + hv->offset);

    if (value->len == 0) {
        if (*old) {
            (*old)->hash = 0;
            *old = NULL;
        }

        return NGX_OK;
    }

    if (*old) {
        h = *old;

    } else {
        h = ngx_list_push(&r->headers_out.headers);
        if (h == NULL) {
            return NGX_ERROR;
        }

        *old = h;
    }

    h->hash = 1;
    h->key = hv->key;
    h->value = *value;

    return NGX_OK;
}















