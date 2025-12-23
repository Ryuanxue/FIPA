
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_ssi_filter_module.h"
#define NGX_HTTP_SSI_ERROR          1

#define NGX_HTTP_SSI_DATE_LEN       2048

#define NGX_HTTP_SSI_ADD_PREFIX     1
#define NGX_HTTP_SSI_ADD_ZERO       2


typedef struct {
    ngx_flag_t    enable;
    ngx_flag_t    silent_errors;
    ngx_flag_t    ignore_recycled_buffers;
    ngx_flag_t    last_modified;

    ngx_hash_t    types;

    size_t        min_file_chunk;
    size_t        value_len;

    ngx_array_t  *types_keys;
} ngx_http_ssi_loc_conf_t;


typedef struct {
    ngx_str_t     name;
    ngx_uint_t    key;
    ngx_str_t     value;
} ngx_http_ssi_var_t;


typedef struct {
    ngx_str_t     name;
    ngx_chain_t  *bufs;
    ngx_uint_t    count;
} ngx_http_ssi_block_t;


typedef enum {
    ssi_start_state = 0,
    ssi_tag_state,
    ssi_comment0_state,
    ssi_comment1_state,
    ssi_sharp_state,
    ssi_precommand_state,
    ssi_command_state,
    ssi_preparam_state,
    ssi_param_state,
    ssi_preequal_state,
    ssi_prevalue_state,
    ssi_double_quoted_value_state,
    ssi_quoted_value_state,
    ssi_quoted_symbol_state,
    ssi_postparam_state,
    ssi_comment_end0_state,
    ssi_comment_end1_state,
    ssi_error_state,
    ssi_error_end0_state,
    ssi_error_end1_state
} ngx_http_ssi_state_e;


static ngx_int_t ngx_http_ssi_output(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static void ngx_http_ssi_buffered(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_int_t ngx_http_ssi_parse(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_str_t *ngx_http_ssi_get_variable(ngx_http_request_t *r,
    ngx_str_t *name, ngx_uint_t key);
static ngx_int_t ngx_http_ssi_evaluate_string(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t *text, ngx_uint_t flags);
static ngx_int_t ngx_http_ssi_regex_match(ngx_http_request_t *r,
    ngx_str_t *pattern, ngx_str_t *str);

static ngx_int_t ngx_http_ssi_include(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_stub_output(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_echo(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_config(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_set(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_if(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_else(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endif(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_block(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endblock(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);

static ngx_int_t ngx_http_ssi_date_gmt_local_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t gmt);

static ngx_int_t ngx_http_ssi_preconfiguration(ngx_conf_t *cf);
static void *ngx_http_ssi_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_ssi_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);
static ngx_int_t ngx_http_ssi_filter_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_ssi_filter_commands[] = {

    { ngx_string("ssi"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, enable),
      NULL },

    { ngx_string("ssi_silent_errors"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, silent_errors),
      NULL },

    { ngx_string("ssi_ignore_recycled_buffers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, ignore_recycled_buffers),
      NULL },

    { ngx_string("ssi_min_file_chunk"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, min_file_chunk),
      NULL },

    { ngx_string("ssi_value_length"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, value_len),
      NULL },

    { ngx_string("ssi_types"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
      ngx_http_types_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, types_keys),
      &ngx_http_html_default_types[0] },

    { ngx_string("ssi_last_modified"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, last_modified),
      NULL },

      ngx_null_command
};



    ngx_http_ssi_preconfiguration,         /* preconfiguration */
    ngx_http_ssi_filter_init,              /* postconfiguration */

    ngx_http_ssi_create_main_conf,         /* create main configuration */
    ngx_http_ssi_init_main_conf,           /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_ssi_create_loc_conf,          /* create location configuration */
    ngx_http_ssi_merge_loc_conf            /* merge location configuration */
};


ngx_module_t  ngx_http_ssi_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_ssi_filter_module_ctx,       /* module context */
    ngx_http_ssi_filter_commands,          /* module directives */
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


static u_char ngx_http_ssi_string[] = "<!--";

static ngx_str_t ngx_http_ssi_timefmt = ngx_string("%A, %d-%b-%Y %H:%M:%S %Z");


#define  NGX_HTTP_SSI_INCLUDE_VIRTUAL  0
#define  NGX_HTTP_SSI_INCLUDE_FILE     1
#define  NGX_HTTP_SSI_INCLUDE_WAIT     2
#define  NGX_HTTP_SSI_INCLUDE_SET      3
#define  NGX_HTTP_SSI_INCLUDE_STUB     4

#define  NGX_HTTP_SSI_ECHO_VAR         0
#define  NGX_HTTP_SSI_ECHO_DEFAULT     1
#define  NGX_HTTP_SSI_ECHO_ENCODING    2

#define  NGX_HTTP_SSI_CONFIG_ERRMSG    0
#define  NGX_HTTP_SSI_CONFIG_TIMEFMT   1

#define  NGX_HTTP_SSI_SET_VAR          0
#define  NGX_HTTP_SSI_SET_VALUE        1

#define  NGX_HTTP_SSI_IF_EXPR          0

#define  NGX_HTTP_SSI_BLOCK_NAME       0


    { ngx_string("virtual"), NGX_HTTP_SSI_INCLUDE_VIRTUAL, 0, 0 },
    { ngx_string("file"), NGX_HTTP_SSI_INCLUDE_FILE, 0, 0 },
    { ngx_string("wait"), NGX_HTTP_SSI_INCLUDE_WAIT, 0, 0 },
    { ngx_string("set"), NGX_HTTP_SSI_INCLUDE_SET, 0, 0 },
    { ngx_string("stub"), NGX_HTTP_SSI_INCLUDE_STUB, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_ECHO_VAR, 1, 0 },
    { ngx_string("default"), NGX_HTTP_SSI_ECHO_DEFAULT, 0, 0 },
    { ngx_string("encoding"), NGX_HTTP_SSI_ECHO_ENCODING, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("errmsg"), NGX_HTTP_SSI_CONFIG_ERRMSG, 0, 0 },
    { ngx_string("timefmt"), NGX_HTTP_SSI_CONFIG_TIMEFMT, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_SET_VAR, 1, 0 },
    { ngx_string("value"), NGX_HTTP_SSI_SET_VALUE, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("expr"), NGX_HTTP_SSI_IF_EXPR, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("name"), NGX_HTTP_SSI_BLOCK_NAME, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("include"), ngx_http_ssi_include,
                       ngx_http_ssi_include_params, 0, 0, 1 },
    { ngx_string("echo"), ngx_http_ssi_echo,
                       ngx_http_ssi_echo_params, 0, 0, 0 },
    { ngx_string("config"), ngx_http_ssi_config,
                       ngx_http_ssi_config_params, 0, 0, 0 },
    { ngx_string("set"), ngx_http_ssi_set, ngx_http_ssi_set_params, 0, 0, 0 },

    { ngx_string("if"), ngx_http_ssi_if, ngx_http_ssi_if_params, 0, 0, 0 },
    { ngx_string("elif"), ngx_http_ssi_if, ngx_http_ssi_if_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("else"), ngx_http_ssi_else, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("endif"), ngx_http_ssi_endif, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_ELSE, 0, 0 },

    { ngx_string("block"), ngx_http_ssi_block,
                       ngx_http_ssi_block_params, 0, 0, 0 },
    { ngx_string("endblock"), ngx_http_ssi_endblock,
                       ngx_http_ssi_no_params, 0, 1, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_http_variable_t  ngx_http_ssi_vars[] = {

    { ngx_string("date_local"), NULL, ngx_http_ssi_date_gmt_local_variable, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("date_gmt"), NULL, ngx_http_ssi_date_gmt_local_variable, 1,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

      ngx_http_null_variable
};



ngx_int_t ngx_http_ssi_header_filter_sense_1(int *ngx_http_ssi_header_filter_sense_1_ret, ngx_http_ssi_ctx_t *ctx, ngx_http_ssi_loc_conf_t *slcf, ngx_http_request_t *r)
{
  slcf = r->loc_conf[ngx_http_ssi_filter_module.ctx_index];
  if (((!slcf->enable) || (r->headers_out.content_length_n == 0)) || (ngx_http_test_content_type(r, &slcf->types) == 0))
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  if (ctx == 0)
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return -1;
    }
  }
  r->ctx[ngx_http_ssi_filter_module.ctx_index] = ctx;
  ;
  r->filter_need_in_memory = 1;
  if (r == r->main)
  {
    r->headers_out.content_length_n = -1;
    if (r->headers_out.content_length)
    {
      r->headers_out.content_length->hash = 0;
      r->headers_out.content_length = 0;
    }
    ;
    r->allow_ranges = 0;
    if (r->headers_out.accept_ranges)
    {
      r->headers_out.accept_ranges->hash = 0;
      r->headers_out.accept_ranges = 0;
    }
    ;
    r->preserve_body = 1;
    if (!slcf->last_modified)
    {
      r->headers_out.last_modified_time = -1;
      if (r->headers_out.last_modified)
      {
        r->headers_out.last_modified->hash = 0;
        r->headers_out.last_modified = 0;
      }
      ;
      if (r->headers_out.etag)
      {
        r->headers_out.etag->hash = 0;
        r->headers_out.etag = 0;
      }
      ;
    }
    else
    {
      ngx_http_weak_etag(r);
    }
  }
  {
    *ngx_http_ssi_header_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}





















static ngx_int_t
ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    ngx_str_t  *value = data;

    if (r->headers_out.status < NGX_HTTP_SPECIAL_RESPONSE
        && r->out && r->out->buf)
    {
        value->len = r->out->buf->last - r->out->buf->pos;
        value->data = r->out->buf->pos;
    }

    return rc;
}


























static void *
ngx_http_ssi_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_ssi_loc_conf_t  *slcf;

    slcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssi_loc_conf_t));
    if (slcf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->types = { NULL };
     *     conf->types_keys = NULL;
     */

    slcf->enable = NGX_CONF_UNSET;
    slcf->silent_errors = NGX_CONF_UNSET;
    slcf->ignore_recycled_buffers = NGX_CONF_UNSET;
    slcf->last_modified = NGX_CONF_UNSET;

    slcf->min_file_chunk = NGX_CONF_UNSET_SIZE;
    slcf->value_len = NGX_CONF_UNSET_SIZE;

    return slcf;
}


static char *
ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ssi_loc_conf_t *prev = parent;
    ngx_http_ssi_loc_conf_t *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);
    ngx_conf_merge_value(conf->silent_errors, prev->silent_errors, 0);
    ngx_conf_merge_value(conf->ignore_recycled_buffers,
                         prev->ignore_recycled_buffers, 0);
    ngx_conf_merge_value(conf->last_modified, prev->last_modified, 0);

    ngx_conf_merge_size_value(conf->min_file_chunk, prev->min_file_chunk, 1024);
    ngx_conf_merge_size_value(conf->value_len, prev->value_len, 255);

    if (ngx_http_merge_types(cf, &conf->types_keys, &conf->types,
                             &prev->types_keys, &prev->types,
                             ngx_http_html_default_types)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_ssi_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_ssi_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_ssi_body_filter;

    return NGX_OK;

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_ssi_filter_module.h"
#define NGX_HTTP_SSI_ERROR          1

#define NGX_HTTP_SSI_DATE_LEN       2048

#define NGX_HTTP_SSI_ADD_PREFIX     1
#define NGX_HTTP_SSI_ADD_ZERO       2


typedef struct {
    ngx_flag_t    enable;
    ngx_flag_t    silent_errors;
    ngx_flag_t    ignore_recycled_buffers;
    ngx_flag_t    last_modified;

    ngx_hash_t    types;

    size_t        min_file_chunk;
    size_t        value_len;

    ngx_array_t  *types_keys;
} ngx_http_ssi_loc_conf_t;


typedef struct {
    ngx_str_t     name;
    ngx_uint_t    key;
    ngx_str_t     value;
} ngx_http_ssi_var_t;


typedef struct {
    ngx_str_t     name;
    ngx_chain_t  *bufs;
    ngx_uint_t    count;
} ngx_http_ssi_block_t;


typedef enum {
    ssi_start_state = 0,
    ssi_tag_state,
    ssi_comment0_state,
    ssi_comment1_state,
    ssi_sharp_state,
    ssi_precommand_state,
    ssi_command_state,
    ssi_preparam_state,
    ssi_param_state,
    ssi_preequal_state,
    ssi_prevalue_state,
    ssi_double_quoted_value_state,
    ssi_quoted_value_state,
    ssi_quoted_symbol_state,
    ssi_postparam_state,
    ssi_comment_end0_state,
    ssi_comment_end1_state,
    ssi_error_state,
    ssi_error_end0_state,
    ssi_error_end1_state
} ngx_http_ssi_state_e;


static ngx_int_t ngx_http_ssi_output(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static void ngx_http_ssi_buffered(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_int_t ngx_http_ssi_parse(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_str_t *ngx_http_ssi_get_variable(ngx_http_request_t *r,
    ngx_str_t *name, ngx_uint_t key);
static ngx_int_t ngx_http_ssi_evaluate_string(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t *text, ngx_uint_t flags);
static ngx_int_t ngx_http_ssi_regex_match(ngx_http_request_t *r,
    ngx_str_t *pattern, ngx_str_t *str);

static ngx_int_t ngx_http_ssi_include(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_stub_output(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_echo(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_config(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_set(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_if(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_else(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endif(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_block(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endblock(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);

static ngx_int_t ngx_http_ssi_date_gmt_local_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t gmt);

static ngx_int_t ngx_http_ssi_preconfiguration(ngx_conf_t *cf);
static void *ngx_http_ssi_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_ssi_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);
static ngx_int_t ngx_http_ssi_filter_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_ssi_filter_commands[] = {

    { ngx_string("ssi"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, enable),
      NULL },

    { ngx_string("ssi_silent_errors"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, silent_errors),
      NULL },

    { ngx_string("ssi_ignore_recycled_buffers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, ignore_recycled_buffers),
      NULL },

    { ngx_string("ssi_min_file_chunk"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, min_file_chunk),
      NULL },

    { ngx_string("ssi_value_length"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, value_len),
      NULL },

    { ngx_string("ssi_types"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
      ngx_http_types_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, types_keys),
      &ngx_http_html_default_types[0] },

    { ngx_string("ssi_last_modified"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, last_modified),
      NULL },

      ngx_null_command
};



    ngx_http_ssi_preconfiguration,         /* preconfiguration */
    ngx_http_ssi_filter_init,              /* postconfiguration */

    ngx_http_ssi_create_main_conf,         /* create main configuration */
    ngx_http_ssi_init_main_conf,           /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_ssi_create_loc_conf,          /* create location configuration */
    ngx_http_ssi_merge_loc_conf            /* merge location configuration */
};


ngx_module_t  ngx_http_ssi_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_ssi_filter_module_ctx,       /* module context */
    ngx_http_ssi_filter_commands,          /* module directives */
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


static u_char ngx_http_ssi_string[] = "<!--";

static ngx_str_t ngx_http_ssi_timefmt = ngx_string("%A, %d-%b-%Y %H:%M:%S %Z");


#define  NGX_HTTP_SSI_INCLUDE_VIRTUAL  0
#define  NGX_HTTP_SSI_INCLUDE_FILE     1
#define  NGX_HTTP_SSI_INCLUDE_WAIT     2
#define  NGX_HTTP_SSI_INCLUDE_SET      3
#define  NGX_HTTP_SSI_INCLUDE_STUB     4

#define  NGX_HTTP_SSI_ECHO_VAR         0
#define  NGX_HTTP_SSI_ECHO_DEFAULT     1
#define  NGX_HTTP_SSI_ECHO_ENCODING    2

#define  NGX_HTTP_SSI_CONFIG_ERRMSG    0
#define  NGX_HTTP_SSI_CONFIG_TIMEFMT   1

#define  NGX_HTTP_SSI_SET_VAR          0
#define  NGX_HTTP_SSI_SET_VALUE        1

#define  NGX_HTTP_SSI_IF_EXPR          0

#define  NGX_HTTP_SSI_BLOCK_NAME       0


    { ngx_string("virtual"), NGX_HTTP_SSI_INCLUDE_VIRTUAL, 0, 0 },
    { ngx_string("file"), NGX_HTTP_SSI_INCLUDE_FILE, 0, 0 },
    { ngx_string("wait"), NGX_HTTP_SSI_INCLUDE_WAIT, 0, 0 },
    { ngx_string("set"), NGX_HTTP_SSI_INCLUDE_SET, 0, 0 },
    { ngx_string("stub"), NGX_HTTP_SSI_INCLUDE_STUB, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_ECHO_VAR, 1, 0 },
    { ngx_string("default"), NGX_HTTP_SSI_ECHO_DEFAULT, 0, 0 },
    { ngx_string("encoding"), NGX_HTTP_SSI_ECHO_ENCODING, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("errmsg"), NGX_HTTP_SSI_CONFIG_ERRMSG, 0, 0 },
    { ngx_string("timefmt"), NGX_HTTP_SSI_CONFIG_TIMEFMT, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_SET_VAR, 1, 0 },
    { ngx_string("value"), NGX_HTTP_SSI_SET_VALUE, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("expr"), NGX_HTTP_SSI_IF_EXPR, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("name"), NGX_HTTP_SSI_BLOCK_NAME, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("include"), ngx_http_ssi_include,
                       ngx_http_ssi_include_params, 0, 0, 1 },
    { ngx_string("echo"), ngx_http_ssi_echo,
                       ngx_http_ssi_echo_params, 0, 0, 0 },
    { ngx_string("config"), ngx_http_ssi_config,
                       ngx_http_ssi_config_params, 0, 0, 0 },
    { ngx_string("set"), ngx_http_ssi_set, ngx_http_ssi_set_params, 0, 0, 0 },

    { ngx_string("if"), ngx_http_ssi_if, ngx_http_ssi_if_params, 0, 0, 0 },
    { ngx_string("elif"), ngx_http_ssi_if, ngx_http_ssi_if_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("else"), ngx_http_ssi_else, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("endif"), ngx_http_ssi_endif, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_ELSE, 0, 0 },

    { ngx_string("block"), ngx_http_ssi_block,
                       ngx_http_ssi_block_params, 0, 0, 0 },
    { ngx_string("endblock"), ngx_http_ssi_endblock,
                       ngx_http_ssi_no_params, 0, 1, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_http_variable_t  ngx_http_ssi_vars[] = {

    { ngx_string("date_local"), NULL, ngx_http_ssi_date_gmt_local_variable, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("date_gmt"), NULL, ngx_http_ssi_date_gmt_local_variable, 1,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

      ngx_http_null_variable
};



ngx_int_t ngx_http_ssi_header_filter_sense_1(int *ngx_http_ssi_header_filter_sense_1_ret, ngx_http_ssi_ctx_t *ctx, ngx_http_ssi_loc_conf_t *slcf, ngx_http_request_t *r)
{
  slcf = r->loc_conf[ngx_http_ssi_filter_module.ctx_index];
  if (((!slcf->enable) || (r->headers_out.content_length_n == 0)) || (ngx_http_test_content_type(r, &slcf->types) == 0))
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  if (ctx == 0)
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return -1;
    }
  }
  r->ctx[ngx_http_ssi_filter_module.ctx_index] = ctx;
  ;
  r->filter_need_in_memory = 1;
  if (r == r->main)
  {
    r->headers_out.content_length_n = -1;
    if (r->headers_out.content_length)
    {
      r->headers_out.content_length->hash = 0;
      r->headers_out.content_length = 0;
    }
    ;
    r->allow_ranges = 0;
    if (r->headers_out.accept_ranges)
    {
      r->headers_out.accept_ranges->hash = 0;
      r->headers_out.accept_ranges = 0;
    }
    ;
    r->preserve_body = 1;
    if (!slcf->last_modified)
    {
      r->headers_out.last_modified_time = -1;
      if (r->headers_out.last_modified)
      {
        r->headers_out.last_modified->hash = 0;
        r->headers_out.last_modified = 0;
      }
      ;
      if (r->headers_out.etag)
      {
        r->headers_out.etag->hash = 0;
        r->headers_out.etag = 0;
      }
      ;
    }
    else
    {
      ngx_http_weak_etag(r);
    }
  }
  {
    *ngx_http_ssi_header_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}





















static ngx_int_t
ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    ngx_str_t  *value = data;

    if (r->headers_out.status < NGX_HTTP_SPECIAL_RESPONSE
        && r->out && r->out->buf)
    {
        value->len = r->out->buf->last - r->out->buf->pos;
        value->data = r->out->buf->pos;
    }

    return rc;
}


























static void *
ngx_http_ssi_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_ssi_loc_conf_t  *slcf;

    slcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssi_loc_conf_t));
    if (slcf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->types = { NULL };
     *     conf->types_keys = NULL;
     */

    slcf->enable = NGX_CONF_UNSET;
    slcf->silent_errors = NGX_CONF_UNSET;
    slcf->ignore_recycled_buffers = NGX_CONF_UNSET;
    slcf->last_modified = NGX_CONF_UNSET;

    slcf->min_file_chunk = NGX_CONF_UNSET_SIZE;
    slcf->value_len = NGX_CONF_UNSET_SIZE;

    return slcf;
}


static char *
ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ssi_loc_conf_t *prev = parent;
    ngx_http_ssi_loc_conf_t *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);
    ngx_conf_merge_value(conf->silent_errors, prev->silent_errors, 0);
    ngx_conf_merge_value(conf->ignore_recycled_buffers,
                         prev->ignore_recycled_buffers, 0);
    ngx_conf_merge_value(conf->last_modified, prev->last_modified, 0);

    ngx_conf_merge_size_value(conf->min_file_chunk, prev->min_file_chunk, 1024);
    ngx_conf_merge_size_value(conf->value_len, prev->value_len, 255);

    if (ngx_http_merge_types(cf, &conf->types_keys, &conf->types,
                             &prev->types_keys, &prev->types,
                             ngx_http_html_default_types)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_ssi_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_ssi_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_ssi_body_filter;

    return NGX_OK;

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_ssi_filter_module.h"
#define NGX_HTTP_SSI_ERROR          1

#define NGX_HTTP_SSI_DATE_LEN       2048

#define NGX_HTTP_SSI_ADD_PREFIX     1
#define NGX_HTTP_SSI_ADD_ZERO       2


typedef struct {
    ngx_flag_t    enable;
    ngx_flag_t    silent_errors;
    ngx_flag_t    ignore_recycled_buffers;
    ngx_flag_t    last_modified;

    ngx_hash_t    types;

    size_t        min_file_chunk;
    size_t        value_len;

    ngx_array_t  *types_keys;
} ngx_http_ssi_loc_conf_t;


typedef struct {
    ngx_str_t     name;
    ngx_uint_t    key;
    ngx_str_t     value;
} ngx_http_ssi_var_t;


typedef struct {
    ngx_str_t     name;
    ngx_chain_t  *bufs;
    ngx_uint_t    count;
} ngx_http_ssi_block_t;


typedef enum {
    ssi_start_state = 0,
    ssi_tag_state,
    ssi_comment0_state,
    ssi_comment1_state,
    ssi_sharp_state,
    ssi_precommand_state,
    ssi_command_state,
    ssi_preparam_state,
    ssi_param_state,
    ssi_preequal_state,
    ssi_prevalue_state,
    ssi_double_quoted_value_state,
    ssi_quoted_value_state,
    ssi_quoted_symbol_state,
    ssi_postparam_state,
    ssi_comment_end0_state,
    ssi_comment_end1_state,
    ssi_error_state,
    ssi_error_end0_state,
    ssi_error_end1_state
} ngx_http_ssi_state_e;


static ngx_int_t ngx_http_ssi_output(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static void ngx_http_ssi_buffered(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_int_t ngx_http_ssi_parse(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_str_t *ngx_http_ssi_get_variable(ngx_http_request_t *r,
    ngx_str_t *name, ngx_uint_t key);
static ngx_int_t ngx_http_ssi_evaluate_string(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t *text, ngx_uint_t flags);
static ngx_int_t ngx_http_ssi_regex_match(ngx_http_request_t *r,
    ngx_str_t *pattern, ngx_str_t *str);

static ngx_int_t ngx_http_ssi_include(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_stub_output(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_echo(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_config(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_set(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_if(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_else(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endif(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_block(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endblock(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);

static ngx_int_t ngx_http_ssi_date_gmt_local_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t gmt);

static ngx_int_t ngx_http_ssi_preconfiguration(ngx_conf_t *cf);
static void *ngx_http_ssi_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_ssi_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);
static ngx_int_t ngx_http_ssi_filter_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_ssi_filter_commands[] = {

    { ngx_string("ssi"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, enable),
      NULL },

    { ngx_string("ssi_silent_errors"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, silent_errors),
      NULL },

    { ngx_string("ssi_ignore_recycled_buffers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, ignore_recycled_buffers),
      NULL },

    { ngx_string("ssi_min_file_chunk"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, min_file_chunk),
      NULL },

    { ngx_string("ssi_value_length"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, value_len),
      NULL },

    { ngx_string("ssi_types"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
      ngx_http_types_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, types_keys),
      &ngx_http_html_default_types[0] },

    { ngx_string("ssi_last_modified"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, last_modified),
      NULL },

      ngx_null_command
};



    ngx_http_ssi_preconfiguration,         /* preconfiguration */
    ngx_http_ssi_filter_init,              /* postconfiguration */

    ngx_http_ssi_create_main_conf,         /* create main configuration */
    ngx_http_ssi_init_main_conf,           /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_ssi_create_loc_conf,          /* create location configuration */
    ngx_http_ssi_merge_loc_conf            /* merge location configuration */
};


ngx_module_t  ngx_http_ssi_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_ssi_filter_module_ctx,       /* module context */
    ngx_http_ssi_filter_commands,          /* module directives */
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


static u_char ngx_http_ssi_string[] = "<!--";

static ngx_str_t ngx_http_ssi_timefmt = ngx_string("%A, %d-%b-%Y %H:%M:%S %Z");


#define  NGX_HTTP_SSI_INCLUDE_VIRTUAL  0
#define  NGX_HTTP_SSI_INCLUDE_FILE     1
#define  NGX_HTTP_SSI_INCLUDE_WAIT     2
#define  NGX_HTTP_SSI_INCLUDE_SET      3
#define  NGX_HTTP_SSI_INCLUDE_STUB     4

#define  NGX_HTTP_SSI_ECHO_VAR         0
#define  NGX_HTTP_SSI_ECHO_DEFAULT     1
#define  NGX_HTTP_SSI_ECHO_ENCODING    2

#define  NGX_HTTP_SSI_CONFIG_ERRMSG    0
#define  NGX_HTTP_SSI_CONFIG_TIMEFMT   1

#define  NGX_HTTP_SSI_SET_VAR          0
#define  NGX_HTTP_SSI_SET_VALUE        1

#define  NGX_HTTP_SSI_IF_EXPR          0

#define  NGX_HTTP_SSI_BLOCK_NAME       0


    { ngx_string("virtual"), NGX_HTTP_SSI_INCLUDE_VIRTUAL, 0, 0 },
    { ngx_string("file"), NGX_HTTP_SSI_INCLUDE_FILE, 0, 0 },
    { ngx_string("wait"), NGX_HTTP_SSI_INCLUDE_WAIT, 0, 0 },
    { ngx_string("set"), NGX_HTTP_SSI_INCLUDE_SET, 0, 0 },
    { ngx_string("stub"), NGX_HTTP_SSI_INCLUDE_STUB, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_ECHO_VAR, 1, 0 },
    { ngx_string("default"), NGX_HTTP_SSI_ECHO_DEFAULT, 0, 0 },
    { ngx_string("encoding"), NGX_HTTP_SSI_ECHO_ENCODING, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("errmsg"), NGX_HTTP_SSI_CONFIG_ERRMSG, 0, 0 },
    { ngx_string("timefmt"), NGX_HTTP_SSI_CONFIG_TIMEFMT, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_SET_VAR, 1, 0 },
    { ngx_string("value"), NGX_HTTP_SSI_SET_VALUE, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("expr"), NGX_HTTP_SSI_IF_EXPR, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("name"), NGX_HTTP_SSI_BLOCK_NAME, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("include"), ngx_http_ssi_include,
                       ngx_http_ssi_include_params, 0, 0, 1 },
    { ngx_string("echo"), ngx_http_ssi_echo,
                       ngx_http_ssi_echo_params, 0, 0, 0 },
    { ngx_string("config"), ngx_http_ssi_config,
                       ngx_http_ssi_config_params, 0, 0, 0 },
    { ngx_string("set"), ngx_http_ssi_set, ngx_http_ssi_set_params, 0, 0, 0 },

    { ngx_string("if"), ngx_http_ssi_if, ngx_http_ssi_if_params, 0, 0, 0 },
    { ngx_string("elif"), ngx_http_ssi_if, ngx_http_ssi_if_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("else"), ngx_http_ssi_else, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("endif"), ngx_http_ssi_endif, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_ELSE, 0, 0 },

    { ngx_string("block"), ngx_http_ssi_block,
                       ngx_http_ssi_block_params, 0, 0, 0 },
    { ngx_string("endblock"), ngx_http_ssi_endblock,
                       ngx_http_ssi_no_params, 0, 1, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_http_variable_t  ngx_http_ssi_vars[] = {

    { ngx_string("date_local"), NULL, ngx_http_ssi_date_gmt_local_variable, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("date_gmt"), NULL, ngx_http_ssi_date_gmt_local_variable, 1,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

      ngx_http_null_variable
};



ngx_int_t ngx_http_ssi_header_filter_sense_1(int *ngx_http_ssi_header_filter_sense_1_ret, ngx_http_ssi_ctx_t *ctx, ngx_http_ssi_loc_conf_t *slcf, ngx_http_request_t *r)
{
  slcf = r->loc_conf[ngx_http_ssi_filter_module.ctx_index];
  if (((!slcf->enable) || (r->headers_out.content_length_n == 0)) || (ngx_http_test_content_type(r, &slcf->types) == 0))
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  if (ctx == 0)
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return -1;
    }
  }
  r->ctx[ngx_http_ssi_filter_module.ctx_index] = ctx;
  ;
  r->filter_need_in_memory = 1;
  if (r == r->main)
  {
    r->headers_out.content_length_n = -1;
    if (r->headers_out.content_length)
    {
      r->headers_out.content_length->hash = 0;
      r->headers_out.content_length = 0;
    }
    ;
    r->allow_ranges = 0;
    if (r->headers_out.accept_ranges)
    {
      r->headers_out.accept_ranges->hash = 0;
      r->headers_out.accept_ranges = 0;
    }
    ;
    r->preserve_body = 1;
    if (!slcf->last_modified)
    {
      r->headers_out.last_modified_time = -1;
      if (r->headers_out.last_modified)
      {
        r->headers_out.last_modified->hash = 0;
        r->headers_out.last_modified = 0;
      }
      ;
      if (r->headers_out.etag)
      {
        r->headers_out.etag->hash = 0;
        r->headers_out.etag = 0;
      }
      ;
    }
    else
    {
      ngx_http_weak_etag(r);
    }
  }
  {
    *ngx_http_ssi_header_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}





















static ngx_int_t
ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    ngx_str_t  *value = data;

    if (r->headers_out.status < NGX_HTTP_SPECIAL_RESPONSE
        && r->out && r->out->buf)
    {
        value->len = r->out->buf->last - r->out->buf->pos;
        value->data = r->out->buf->pos;
    }

    return rc;
}


























static void *
ngx_http_ssi_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_ssi_loc_conf_t  *slcf;

    slcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssi_loc_conf_t));
    if (slcf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->types = { NULL };
     *     conf->types_keys = NULL;
     */

    slcf->enable = NGX_CONF_UNSET;
    slcf->silent_errors = NGX_CONF_UNSET;
    slcf->ignore_recycled_buffers = NGX_CONF_UNSET;
    slcf->last_modified = NGX_CONF_UNSET;

    slcf->min_file_chunk = NGX_CONF_UNSET_SIZE;
    slcf->value_len = NGX_CONF_UNSET_SIZE;

    return slcf;
}


static char *
ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ssi_loc_conf_t *prev = parent;
    ngx_http_ssi_loc_conf_t *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);
    ngx_conf_merge_value(conf->silent_errors, prev->silent_errors, 0);
    ngx_conf_merge_value(conf->ignore_recycled_buffers,
                         prev->ignore_recycled_buffers, 0);
    ngx_conf_merge_value(conf->last_modified, prev->last_modified, 0);

    ngx_conf_merge_size_value(conf->min_file_chunk, prev->min_file_chunk, 1024);
    ngx_conf_merge_size_value(conf->value_len, prev->value_len, 255);

    if (ngx_http_merge_types(cf, &conf->types_keys, &conf->types,
                             &prev->types_keys, &prev->types,
                             ngx_http_html_default_types)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_ssi_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_ssi_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_ssi_body_filter;

    return NGX_OK;

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_ssi_filter_module.h"
#define NGX_HTTP_SSI_ERROR          1

#define NGX_HTTP_SSI_DATE_LEN       2048

#define NGX_HTTP_SSI_ADD_PREFIX     1
#define NGX_HTTP_SSI_ADD_ZERO       2


typedef struct {
    ngx_flag_t    enable;
    ngx_flag_t    silent_errors;
    ngx_flag_t    ignore_recycled_buffers;
    ngx_flag_t    last_modified;

    ngx_hash_t    types;

    size_t        min_file_chunk;
    size_t        value_len;

    ngx_array_t  *types_keys;
} ngx_http_ssi_loc_conf_t;


typedef struct {
    ngx_str_t     name;
    ngx_uint_t    key;
    ngx_str_t     value;
} ngx_http_ssi_var_t;


typedef struct {
    ngx_str_t     name;
    ngx_chain_t  *bufs;
    ngx_uint_t    count;
} ngx_http_ssi_block_t;


typedef enum {
    ssi_start_state = 0,
    ssi_tag_state,
    ssi_comment0_state,
    ssi_comment1_state,
    ssi_sharp_state,
    ssi_precommand_state,
    ssi_command_state,
    ssi_preparam_state,
    ssi_param_state,
    ssi_preequal_state,
    ssi_prevalue_state,
    ssi_double_quoted_value_state,
    ssi_quoted_value_state,
    ssi_quoted_symbol_state,
    ssi_postparam_state,
    ssi_comment_end0_state,
    ssi_comment_end1_state,
    ssi_error_state,
    ssi_error_end0_state,
    ssi_error_end1_state
} ngx_http_ssi_state_e;


static ngx_int_t ngx_http_ssi_output(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static void ngx_http_ssi_buffered(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_int_t ngx_http_ssi_parse(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_str_t *ngx_http_ssi_get_variable(ngx_http_request_t *r,
    ngx_str_t *name, ngx_uint_t key);
static ngx_int_t ngx_http_ssi_evaluate_string(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t *text, ngx_uint_t flags);
static ngx_int_t ngx_http_ssi_regex_match(ngx_http_request_t *r,
    ngx_str_t *pattern, ngx_str_t *str);

static ngx_int_t ngx_http_ssi_include(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_stub_output(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_echo(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_config(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_set(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_if(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_else(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endif(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_block(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endblock(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);

static ngx_int_t ngx_http_ssi_date_gmt_local_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t gmt);

static ngx_int_t ngx_http_ssi_preconfiguration(ngx_conf_t *cf);
static void *ngx_http_ssi_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_ssi_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);
static ngx_int_t ngx_http_ssi_filter_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_ssi_filter_commands[] = {

    { ngx_string("ssi"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, enable),
      NULL },

    { ngx_string("ssi_silent_errors"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, silent_errors),
      NULL },

    { ngx_string("ssi_ignore_recycled_buffers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, ignore_recycled_buffers),
      NULL },

    { ngx_string("ssi_min_file_chunk"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, min_file_chunk),
      NULL },

    { ngx_string("ssi_value_length"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, value_len),
      NULL },

    { ngx_string("ssi_types"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
      ngx_http_types_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, types_keys),
      &ngx_http_html_default_types[0] },

    { ngx_string("ssi_last_modified"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, last_modified),
      NULL },

      ngx_null_command
};



    ngx_http_ssi_preconfiguration,         /* preconfiguration */
    ngx_http_ssi_filter_init,              /* postconfiguration */

    ngx_http_ssi_create_main_conf,         /* create main configuration */
    ngx_http_ssi_init_main_conf,           /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_ssi_create_loc_conf,          /* create location configuration */
    ngx_http_ssi_merge_loc_conf            /* merge location configuration */
};


ngx_module_t  ngx_http_ssi_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_ssi_filter_module_ctx,       /* module context */
    ngx_http_ssi_filter_commands,          /* module directives */
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


static u_char ngx_http_ssi_string[] = "<!--";

static ngx_str_t ngx_http_ssi_timefmt = ngx_string("%A, %d-%b-%Y %H:%M:%S %Z");


#define  NGX_HTTP_SSI_INCLUDE_VIRTUAL  0
#define  NGX_HTTP_SSI_INCLUDE_FILE     1
#define  NGX_HTTP_SSI_INCLUDE_WAIT     2
#define  NGX_HTTP_SSI_INCLUDE_SET      3
#define  NGX_HTTP_SSI_INCLUDE_STUB     4

#define  NGX_HTTP_SSI_ECHO_VAR         0
#define  NGX_HTTP_SSI_ECHO_DEFAULT     1
#define  NGX_HTTP_SSI_ECHO_ENCODING    2

#define  NGX_HTTP_SSI_CONFIG_ERRMSG    0
#define  NGX_HTTP_SSI_CONFIG_TIMEFMT   1

#define  NGX_HTTP_SSI_SET_VAR          0
#define  NGX_HTTP_SSI_SET_VALUE        1

#define  NGX_HTTP_SSI_IF_EXPR          0

#define  NGX_HTTP_SSI_BLOCK_NAME       0


    { ngx_string("virtual"), NGX_HTTP_SSI_INCLUDE_VIRTUAL, 0, 0 },
    { ngx_string("file"), NGX_HTTP_SSI_INCLUDE_FILE, 0, 0 },
    { ngx_string("wait"), NGX_HTTP_SSI_INCLUDE_WAIT, 0, 0 },
    { ngx_string("set"), NGX_HTTP_SSI_INCLUDE_SET, 0, 0 },
    { ngx_string("stub"), NGX_HTTP_SSI_INCLUDE_STUB, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_ECHO_VAR, 1, 0 },
    { ngx_string("default"), NGX_HTTP_SSI_ECHO_DEFAULT, 0, 0 },
    { ngx_string("encoding"), NGX_HTTP_SSI_ECHO_ENCODING, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("errmsg"), NGX_HTTP_SSI_CONFIG_ERRMSG, 0, 0 },
    { ngx_string("timefmt"), NGX_HTTP_SSI_CONFIG_TIMEFMT, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_SET_VAR, 1, 0 },
    { ngx_string("value"), NGX_HTTP_SSI_SET_VALUE, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("expr"), NGX_HTTP_SSI_IF_EXPR, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("name"), NGX_HTTP_SSI_BLOCK_NAME, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("include"), ngx_http_ssi_include,
                       ngx_http_ssi_include_params, 0, 0, 1 },
    { ngx_string("echo"), ngx_http_ssi_echo,
                       ngx_http_ssi_echo_params, 0, 0, 0 },
    { ngx_string("config"), ngx_http_ssi_config,
                       ngx_http_ssi_config_params, 0, 0, 0 },
    { ngx_string("set"), ngx_http_ssi_set, ngx_http_ssi_set_params, 0, 0, 0 },

    { ngx_string("if"), ngx_http_ssi_if, ngx_http_ssi_if_params, 0, 0, 0 },
    { ngx_string("elif"), ngx_http_ssi_if, ngx_http_ssi_if_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("else"), ngx_http_ssi_else, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("endif"), ngx_http_ssi_endif, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_ELSE, 0, 0 },

    { ngx_string("block"), ngx_http_ssi_block,
                       ngx_http_ssi_block_params, 0, 0, 0 },
    { ngx_string("endblock"), ngx_http_ssi_endblock,
                       ngx_http_ssi_no_params, 0, 1, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_http_variable_t  ngx_http_ssi_vars[] = {

    { ngx_string("date_local"), NULL, ngx_http_ssi_date_gmt_local_variable, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("date_gmt"), NULL, ngx_http_ssi_date_gmt_local_variable, 1,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

      ngx_http_null_variable
};



ngx_int_t ngx_http_ssi_header_filter_sense_1(int *ngx_http_ssi_header_filter_sense_1_ret, ngx_http_ssi_ctx_t *ctx, ngx_http_ssi_loc_conf_t *slcf, ngx_http_request_t *r)
{
  slcf = r->loc_conf[ngx_http_ssi_filter_module.ctx_index];
  if (((!slcf->enable) || (r->headers_out.content_length_n == 0)) || (ngx_http_test_content_type(r, &slcf->types) == 0))
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  if (ctx == 0)
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return -1;
    }
  }
  r->ctx[ngx_http_ssi_filter_module.ctx_index] = ctx;
  ;
  r->filter_need_in_memory = 1;
  if (r == r->main)
  {
    r->headers_out.content_length_n = -1;
    if (r->headers_out.content_length)
    {
      r->headers_out.content_length->hash = 0;
      r->headers_out.content_length = 0;
    }
    ;
    r->allow_ranges = 0;
    if (r->headers_out.accept_ranges)
    {
      r->headers_out.accept_ranges->hash = 0;
      r->headers_out.accept_ranges = 0;
    }
    ;
    r->preserve_body = 1;
    if (!slcf->last_modified)
    {
      r->headers_out.last_modified_time = -1;
      if (r->headers_out.last_modified)
      {
        r->headers_out.last_modified->hash = 0;
        r->headers_out.last_modified = 0;
      }
      ;
      if (r->headers_out.etag)
      {
        r->headers_out.etag->hash = 0;
        r->headers_out.etag = 0;
      }
      ;
    }
    else
    {
      ngx_http_weak_etag(r);
    }
  }
  {
    *ngx_http_ssi_header_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}





















static ngx_int_t
ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    ngx_str_t  *value = data;

    if (r->headers_out.status < NGX_HTTP_SPECIAL_RESPONSE
        && r->out && r->out->buf)
    {
        value->len = r->out->buf->last - r->out->buf->pos;
        value->data = r->out->buf->pos;
    }

    return rc;
}


























static void *
ngx_http_ssi_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_ssi_loc_conf_t  *slcf;

    slcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssi_loc_conf_t));
    if (slcf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->types = { NULL };
     *     conf->types_keys = NULL;
     */

    slcf->enable = NGX_CONF_UNSET;
    slcf->silent_errors = NGX_CONF_UNSET;
    slcf->ignore_recycled_buffers = NGX_CONF_UNSET;
    slcf->last_modified = NGX_CONF_UNSET;

    slcf->min_file_chunk = NGX_CONF_UNSET_SIZE;
    slcf->value_len = NGX_CONF_UNSET_SIZE;

    return slcf;
}


static char *
ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ssi_loc_conf_t *prev = parent;
    ngx_http_ssi_loc_conf_t *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);
    ngx_conf_merge_value(conf->silent_errors, prev->silent_errors, 0);
    ngx_conf_merge_value(conf->ignore_recycled_buffers,
                         prev->ignore_recycled_buffers, 0);
    ngx_conf_merge_value(conf->last_modified, prev->last_modified, 0);

    ngx_conf_merge_size_value(conf->min_file_chunk, prev->min_file_chunk, 1024);
    ngx_conf_merge_size_value(conf->value_len, prev->value_len, 255);

    if (ngx_http_merge_types(cf, &conf->types_keys, &conf->types,
                             &prev->types_keys, &prev->types,
                             ngx_http_html_default_types)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_ssi_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_ssi_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_ssi_body_filter;

    return NGX_OK;

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_ssi_filter_module.h"
#define NGX_HTTP_SSI_ERROR          1

#define NGX_HTTP_SSI_DATE_LEN       2048

#define NGX_HTTP_SSI_ADD_PREFIX     1
#define NGX_HTTP_SSI_ADD_ZERO       2


typedef struct {
    ngx_flag_t    enable;
    ngx_flag_t    silent_errors;
    ngx_flag_t    ignore_recycled_buffers;
    ngx_flag_t    last_modified;

    ngx_hash_t    types;

    size_t        min_file_chunk;
    size_t        value_len;

    ngx_array_t  *types_keys;
} ngx_http_ssi_loc_conf_t;


typedef struct {
    ngx_str_t     name;
    ngx_uint_t    key;
    ngx_str_t     value;
} ngx_http_ssi_var_t;


typedef struct {
    ngx_str_t     name;
    ngx_chain_t  *bufs;
    ngx_uint_t    count;
} ngx_http_ssi_block_t;


typedef enum {
    ssi_start_state = 0,
    ssi_tag_state,
    ssi_comment0_state,
    ssi_comment1_state,
    ssi_sharp_state,
    ssi_precommand_state,
    ssi_command_state,
    ssi_preparam_state,
    ssi_param_state,
    ssi_preequal_state,
    ssi_prevalue_state,
    ssi_double_quoted_value_state,
    ssi_quoted_value_state,
    ssi_quoted_symbol_state,
    ssi_postparam_state,
    ssi_comment_end0_state,
    ssi_comment_end1_state,
    ssi_error_state,
    ssi_error_end0_state,
    ssi_error_end1_state
} ngx_http_ssi_state_e;


static ngx_int_t ngx_http_ssi_output(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static void ngx_http_ssi_buffered(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_int_t ngx_http_ssi_parse(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_str_t *ngx_http_ssi_get_variable(ngx_http_request_t *r,
    ngx_str_t *name, ngx_uint_t key);
static ngx_int_t ngx_http_ssi_evaluate_string(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t *text, ngx_uint_t flags);
static ngx_int_t ngx_http_ssi_regex_match(ngx_http_request_t *r,
    ngx_str_t *pattern, ngx_str_t *str);

static ngx_int_t ngx_http_ssi_include(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_stub_output(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_echo(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_config(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_set(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_if(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_else(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endif(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_block(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endblock(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);

static ngx_int_t ngx_http_ssi_date_gmt_local_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t gmt);

static ngx_int_t ngx_http_ssi_preconfiguration(ngx_conf_t *cf);
static void *ngx_http_ssi_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_ssi_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);
static ngx_int_t ngx_http_ssi_filter_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_ssi_filter_commands[] = {

    { ngx_string("ssi"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, enable),
      NULL },

    { ngx_string("ssi_silent_errors"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, silent_errors),
      NULL },

    { ngx_string("ssi_ignore_recycled_buffers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, ignore_recycled_buffers),
      NULL },

    { ngx_string("ssi_min_file_chunk"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, min_file_chunk),
      NULL },

    { ngx_string("ssi_value_length"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, value_len),
      NULL },

    { ngx_string("ssi_types"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
      ngx_http_types_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, types_keys),
      &ngx_http_html_default_types[0] },

    { ngx_string("ssi_last_modified"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, last_modified),
      NULL },

      ngx_null_command
};



    ngx_http_ssi_preconfiguration,         /* preconfiguration */
    ngx_http_ssi_filter_init,              /* postconfiguration */

    ngx_http_ssi_create_main_conf,         /* create main configuration */
    ngx_http_ssi_init_main_conf,           /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_ssi_create_loc_conf,          /* create location configuration */
    ngx_http_ssi_merge_loc_conf            /* merge location configuration */
};


ngx_module_t  ngx_http_ssi_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_ssi_filter_module_ctx,       /* module context */
    ngx_http_ssi_filter_commands,          /* module directives */
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


static u_char ngx_http_ssi_string[] = "<!--";

static ngx_str_t ngx_http_ssi_timefmt = ngx_string("%A, %d-%b-%Y %H:%M:%S %Z");


#define  NGX_HTTP_SSI_INCLUDE_VIRTUAL  0
#define  NGX_HTTP_SSI_INCLUDE_FILE     1
#define  NGX_HTTP_SSI_INCLUDE_WAIT     2
#define  NGX_HTTP_SSI_INCLUDE_SET      3
#define  NGX_HTTP_SSI_INCLUDE_STUB     4

#define  NGX_HTTP_SSI_ECHO_VAR         0
#define  NGX_HTTP_SSI_ECHO_DEFAULT     1
#define  NGX_HTTP_SSI_ECHO_ENCODING    2

#define  NGX_HTTP_SSI_CONFIG_ERRMSG    0
#define  NGX_HTTP_SSI_CONFIG_TIMEFMT   1

#define  NGX_HTTP_SSI_SET_VAR          0
#define  NGX_HTTP_SSI_SET_VALUE        1

#define  NGX_HTTP_SSI_IF_EXPR          0

#define  NGX_HTTP_SSI_BLOCK_NAME       0


    { ngx_string("virtual"), NGX_HTTP_SSI_INCLUDE_VIRTUAL, 0, 0 },
    { ngx_string("file"), NGX_HTTP_SSI_INCLUDE_FILE, 0, 0 },
    { ngx_string("wait"), NGX_HTTP_SSI_INCLUDE_WAIT, 0, 0 },
    { ngx_string("set"), NGX_HTTP_SSI_INCLUDE_SET, 0, 0 },
    { ngx_string("stub"), NGX_HTTP_SSI_INCLUDE_STUB, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_ECHO_VAR, 1, 0 },
    { ngx_string("default"), NGX_HTTP_SSI_ECHO_DEFAULT, 0, 0 },
    { ngx_string("encoding"), NGX_HTTP_SSI_ECHO_ENCODING, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("errmsg"), NGX_HTTP_SSI_CONFIG_ERRMSG, 0, 0 },
    { ngx_string("timefmt"), NGX_HTTP_SSI_CONFIG_TIMEFMT, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_SET_VAR, 1, 0 },
    { ngx_string("value"), NGX_HTTP_SSI_SET_VALUE, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("expr"), NGX_HTTP_SSI_IF_EXPR, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("name"), NGX_HTTP_SSI_BLOCK_NAME, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("include"), ngx_http_ssi_include,
                       ngx_http_ssi_include_params, 0, 0, 1 },
    { ngx_string("echo"), ngx_http_ssi_echo,
                       ngx_http_ssi_echo_params, 0, 0, 0 },
    { ngx_string("config"), ngx_http_ssi_config,
                       ngx_http_ssi_config_params, 0, 0, 0 },
    { ngx_string("set"), ngx_http_ssi_set, ngx_http_ssi_set_params, 0, 0, 0 },

    { ngx_string("if"), ngx_http_ssi_if, ngx_http_ssi_if_params, 0, 0, 0 },
    { ngx_string("elif"), ngx_http_ssi_if, ngx_http_ssi_if_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("else"), ngx_http_ssi_else, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("endif"), ngx_http_ssi_endif, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_ELSE, 0, 0 },

    { ngx_string("block"), ngx_http_ssi_block,
                       ngx_http_ssi_block_params, 0, 0, 0 },
    { ngx_string("endblock"), ngx_http_ssi_endblock,
                       ngx_http_ssi_no_params, 0, 1, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_http_variable_t  ngx_http_ssi_vars[] = {

    { ngx_string("date_local"), NULL, ngx_http_ssi_date_gmt_local_variable, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("date_gmt"), NULL, ngx_http_ssi_date_gmt_local_variable, 1,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

      ngx_http_null_variable
};



ngx_int_t ngx_http_ssi_header_filter_sense_1(int *ngx_http_ssi_header_filter_sense_1_ret, ngx_http_ssi_ctx_t *ctx, ngx_http_ssi_loc_conf_t *slcf, ngx_http_request_t *r)
{
  slcf = r->loc_conf[ngx_http_ssi_filter_module.ctx_index];
  if (((!slcf->enable) || (r->headers_out.content_length_n == 0)) || (ngx_http_test_content_type(r, &slcf->types) == 0))
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  if (ctx == 0)
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return -1;
    }
  }
  r->ctx[ngx_http_ssi_filter_module.ctx_index] = ctx;
  ;
  r->filter_need_in_memory = 1;
  if (r == r->main)
  {
    r->headers_out.content_length_n = -1;
    if (r->headers_out.content_length)
    {
      r->headers_out.content_length->hash = 0;
      r->headers_out.content_length = 0;
    }
    ;
    r->allow_ranges = 0;
    if (r->headers_out.accept_ranges)
    {
      r->headers_out.accept_ranges->hash = 0;
      r->headers_out.accept_ranges = 0;
    }
    ;
    r->preserve_body = 1;
    if (!slcf->last_modified)
    {
      r->headers_out.last_modified_time = -1;
      if (r->headers_out.last_modified)
      {
        r->headers_out.last_modified->hash = 0;
        r->headers_out.last_modified = 0;
      }
      ;
      if (r->headers_out.etag)
      {
        r->headers_out.etag->hash = 0;
        r->headers_out.etag = 0;
      }
      ;
    }
    else
    {
      ngx_http_weak_etag(r);
    }
  }
  {
    *ngx_http_ssi_header_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}





















static ngx_int_t
ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    ngx_str_t  *value = data;

    if (r->headers_out.status < NGX_HTTP_SPECIAL_RESPONSE
        && r->out && r->out->buf)
    {
        value->len = r->out->buf->last - r->out->buf->pos;
        value->data = r->out->buf->pos;
    }

    return rc;
}


























static void *
ngx_http_ssi_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_ssi_loc_conf_t  *slcf;

    slcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssi_loc_conf_t));
    if (slcf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->types = { NULL };
     *     conf->types_keys = NULL;
     */

    slcf->enable = NGX_CONF_UNSET;
    slcf->silent_errors = NGX_CONF_UNSET;
    slcf->ignore_recycled_buffers = NGX_CONF_UNSET;
    slcf->last_modified = NGX_CONF_UNSET;

    slcf->min_file_chunk = NGX_CONF_UNSET_SIZE;
    slcf->value_len = NGX_CONF_UNSET_SIZE;

    return slcf;
}


static char *
ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ssi_loc_conf_t *prev = parent;
    ngx_http_ssi_loc_conf_t *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);
    ngx_conf_merge_value(conf->silent_errors, prev->silent_errors, 0);
    ngx_conf_merge_value(conf->ignore_recycled_buffers,
                         prev->ignore_recycled_buffers, 0);
    ngx_conf_merge_value(conf->last_modified, prev->last_modified, 0);

    ngx_conf_merge_size_value(conf->min_file_chunk, prev->min_file_chunk, 1024);
    ngx_conf_merge_size_value(conf->value_len, prev->value_len, 255);

    if (ngx_http_merge_types(cf, &conf->types_keys, &conf->types,
                             &prev->types_keys, &prev->types,
                             ngx_http_html_default_types)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_ssi_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_ssi_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_ssi_body_filter;

    return NGX_OK;

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_ssi_filter_module.h"
#define NGX_HTTP_SSI_ERROR          1

#define NGX_HTTP_SSI_DATE_LEN       2048

#define NGX_HTTP_SSI_ADD_PREFIX     1
#define NGX_HTTP_SSI_ADD_ZERO       2


typedef struct {
    ngx_flag_t    enable;
    ngx_flag_t    silent_errors;
    ngx_flag_t    ignore_recycled_buffers;
    ngx_flag_t    last_modified;

    ngx_hash_t    types;

    size_t        min_file_chunk;
    size_t        value_len;

    ngx_array_t  *types_keys;
} ngx_http_ssi_loc_conf_t;


typedef struct {
    ngx_str_t     name;
    ngx_uint_t    key;
    ngx_str_t     value;
} ngx_http_ssi_var_t;


typedef struct {
    ngx_str_t     name;
    ngx_chain_t  *bufs;
    ngx_uint_t    count;
} ngx_http_ssi_block_t;


typedef enum {
    ssi_start_state = 0,
    ssi_tag_state,
    ssi_comment0_state,
    ssi_comment1_state,
    ssi_sharp_state,
    ssi_precommand_state,
    ssi_command_state,
    ssi_preparam_state,
    ssi_param_state,
    ssi_preequal_state,
    ssi_prevalue_state,
    ssi_double_quoted_value_state,
    ssi_quoted_value_state,
    ssi_quoted_symbol_state,
    ssi_postparam_state,
    ssi_comment_end0_state,
    ssi_comment_end1_state,
    ssi_error_state,
    ssi_error_end0_state,
    ssi_error_end1_state
} ngx_http_ssi_state_e;


static ngx_int_t ngx_http_ssi_output(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static void ngx_http_ssi_buffered(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_int_t ngx_http_ssi_parse(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_str_t *ngx_http_ssi_get_variable(ngx_http_request_t *r,
    ngx_str_t *name, ngx_uint_t key);
static ngx_int_t ngx_http_ssi_evaluate_string(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t *text, ngx_uint_t flags);
static ngx_int_t ngx_http_ssi_regex_match(ngx_http_request_t *r,
    ngx_str_t *pattern, ngx_str_t *str);

static ngx_int_t ngx_http_ssi_include(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_stub_output(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_echo(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_config(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_set(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_if(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_else(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endif(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_block(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endblock(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);

static ngx_int_t ngx_http_ssi_date_gmt_local_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t gmt);

static ngx_int_t ngx_http_ssi_preconfiguration(ngx_conf_t *cf);
static void *ngx_http_ssi_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_ssi_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);
static ngx_int_t ngx_http_ssi_filter_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_ssi_filter_commands[] = {

    { ngx_string("ssi"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, enable),
      NULL },

    { ngx_string("ssi_silent_errors"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, silent_errors),
      NULL },

    { ngx_string("ssi_ignore_recycled_buffers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, ignore_recycled_buffers),
      NULL },

    { ngx_string("ssi_min_file_chunk"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, min_file_chunk),
      NULL },

    { ngx_string("ssi_value_length"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, value_len),
      NULL },

    { ngx_string("ssi_types"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
      ngx_http_types_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, types_keys),
      &ngx_http_html_default_types[0] },

    { ngx_string("ssi_last_modified"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, last_modified),
      NULL },

      ngx_null_command
};



    ngx_http_ssi_preconfiguration,         /* preconfiguration */
    ngx_http_ssi_filter_init,              /* postconfiguration */

    ngx_http_ssi_create_main_conf,         /* create main configuration */
    ngx_http_ssi_init_main_conf,           /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_ssi_create_loc_conf,          /* create location configuration */
    ngx_http_ssi_merge_loc_conf            /* merge location configuration */
};


ngx_module_t  ngx_http_ssi_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_ssi_filter_module_ctx,       /* module context */
    ngx_http_ssi_filter_commands,          /* module directives */
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


static u_char ngx_http_ssi_string[] = "<!--";

static ngx_str_t ngx_http_ssi_timefmt = ngx_string("%A, %d-%b-%Y %H:%M:%S %Z");


#define  NGX_HTTP_SSI_INCLUDE_VIRTUAL  0
#define  NGX_HTTP_SSI_INCLUDE_FILE     1
#define  NGX_HTTP_SSI_INCLUDE_WAIT     2
#define  NGX_HTTP_SSI_INCLUDE_SET      3
#define  NGX_HTTP_SSI_INCLUDE_STUB     4

#define  NGX_HTTP_SSI_ECHO_VAR         0
#define  NGX_HTTP_SSI_ECHO_DEFAULT     1
#define  NGX_HTTP_SSI_ECHO_ENCODING    2

#define  NGX_HTTP_SSI_CONFIG_ERRMSG    0
#define  NGX_HTTP_SSI_CONFIG_TIMEFMT   1

#define  NGX_HTTP_SSI_SET_VAR          0
#define  NGX_HTTP_SSI_SET_VALUE        1

#define  NGX_HTTP_SSI_IF_EXPR          0

#define  NGX_HTTP_SSI_BLOCK_NAME       0


    { ngx_string("virtual"), NGX_HTTP_SSI_INCLUDE_VIRTUAL, 0, 0 },
    { ngx_string("file"), NGX_HTTP_SSI_INCLUDE_FILE, 0, 0 },
    { ngx_string("wait"), NGX_HTTP_SSI_INCLUDE_WAIT, 0, 0 },
    { ngx_string("set"), NGX_HTTP_SSI_INCLUDE_SET, 0, 0 },
    { ngx_string("stub"), NGX_HTTP_SSI_INCLUDE_STUB, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_ECHO_VAR, 1, 0 },
    { ngx_string("default"), NGX_HTTP_SSI_ECHO_DEFAULT, 0, 0 },
    { ngx_string("encoding"), NGX_HTTP_SSI_ECHO_ENCODING, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("errmsg"), NGX_HTTP_SSI_CONFIG_ERRMSG, 0, 0 },
    { ngx_string("timefmt"), NGX_HTTP_SSI_CONFIG_TIMEFMT, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_SET_VAR, 1, 0 },
    { ngx_string("value"), NGX_HTTP_SSI_SET_VALUE, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("expr"), NGX_HTTP_SSI_IF_EXPR, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("name"), NGX_HTTP_SSI_BLOCK_NAME, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("include"), ngx_http_ssi_include,
                       ngx_http_ssi_include_params, 0, 0, 1 },
    { ngx_string("echo"), ngx_http_ssi_echo,
                       ngx_http_ssi_echo_params, 0, 0, 0 },
    { ngx_string("config"), ngx_http_ssi_config,
                       ngx_http_ssi_config_params, 0, 0, 0 },
    { ngx_string("set"), ngx_http_ssi_set, ngx_http_ssi_set_params, 0, 0, 0 },

    { ngx_string("if"), ngx_http_ssi_if, ngx_http_ssi_if_params, 0, 0, 0 },
    { ngx_string("elif"), ngx_http_ssi_if, ngx_http_ssi_if_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("else"), ngx_http_ssi_else, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("endif"), ngx_http_ssi_endif, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_ELSE, 0, 0 },

    { ngx_string("block"), ngx_http_ssi_block,
                       ngx_http_ssi_block_params, 0, 0, 0 },
    { ngx_string("endblock"), ngx_http_ssi_endblock,
                       ngx_http_ssi_no_params, 0, 1, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_http_variable_t  ngx_http_ssi_vars[] = {

    { ngx_string("date_local"), NULL, ngx_http_ssi_date_gmt_local_variable, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("date_gmt"), NULL, ngx_http_ssi_date_gmt_local_variable, 1,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

      ngx_http_null_variable
};



ngx_int_t ngx_http_ssi_header_filter_sense_1(int *ngx_http_ssi_header_filter_sense_1_ret, ngx_http_ssi_ctx_t *ctx, ngx_http_ssi_loc_conf_t *slcf, ngx_http_request_t *r)
{
  slcf = r->loc_conf[ngx_http_ssi_filter_module.ctx_index];
  if (((!slcf->enable) || (r->headers_out.content_length_n == 0)) || (ngx_http_test_content_type(r, &slcf->types) == 0))
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  if (ctx == 0)
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return -1;
    }
  }
  r->ctx[ngx_http_ssi_filter_module.ctx_index] = ctx;
  ;
  r->filter_need_in_memory = 1;
  if (r == r->main)
  {
    r->headers_out.content_length_n = -1;
    if (r->headers_out.content_length)
    {
      r->headers_out.content_length->hash = 0;
      r->headers_out.content_length = 0;
    }
    ;
    r->allow_ranges = 0;
    if (r->headers_out.accept_ranges)
    {
      r->headers_out.accept_ranges->hash = 0;
      r->headers_out.accept_ranges = 0;
    }
    ;
    r->preserve_body = 1;
    if (!slcf->last_modified)
    {
      r->headers_out.last_modified_time = -1;
      if (r->headers_out.last_modified)
      {
        r->headers_out.last_modified->hash = 0;
        r->headers_out.last_modified = 0;
      }
      ;
      if (r->headers_out.etag)
      {
        r->headers_out.etag->hash = 0;
        r->headers_out.etag = 0;
      }
      ;
    }
    else
    {
      ngx_http_weak_etag(r);
    }
  }
  {
    *ngx_http_ssi_header_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}





















static ngx_int_t
ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    ngx_str_t  *value = data;

    if (r->headers_out.status < NGX_HTTP_SPECIAL_RESPONSE
        && r->out && r->out->buf)
    {
        value->len = r->out->buf->last - r->out->buf->pos;
        value->data = r->out->buf->pos;
    }

    return rc;
}


























static void *
ngx_http_ssi_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_ssi_loc_conf_t  *slcf;

    slcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssi_loc_conf_t));
    if (slcf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->types = { NULL };
     *     conf->types_keys = NULL;
     */

    slcf->enable = NGX_CONF_UNSET;
    slcf->silent_errors = NGX_CONF_UNSET;
    slcf->ignore_recycled_buffers = NGX_CONF_UNSET;
    slcf->last_modified = NGX_CONF_UNSET;

    slcf->min_file_chunk = NGX_CONF_UNSET_SIZE;
    slcf->value_len = NGX_CONF_UNSET_SIZE;

    return slcf;
}


static char *
ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ssi_loc_conf_t *prev = parent;
    ngx_http_ssi_loc_conf_t *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);
    ngx_conf_merge_value(conf->silent_errors, prev->silent_errors, 0);
    ngx_conf_merge_value(conf->ignore_recycled_buffers,
                         prev->ignore_recycled_buffers, 0);
    ngx_conf_merge_value(conf->last_modified, prev->last_modified, 0);

    ngx_conf_merge_size_value(conf->min_file_chunk, prev->min_file_chunk, 1024);
    ngx_conf_merge_size_value(conf->value_len, prev->value_len, 255);

    if (ngx_http_merge_types(cf, &conf->types_keys, &conf->types,
                             &prev->types_keys, &prev->types,
                             ngx_http_html_default_types)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_ssi_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_ssi_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_ssi_body_filter;

    return NGX_OK;

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_ssi_filter_module.h"
#define NGX_HTTP_SSI_ERROR          1

#define NGX_HTTP_SSI_DATE_LEN       2048

#define NGX_HTTP_SSI_ADD_PREFIX     1
#define NGX_HTTP_SSI_ADD_ZERO       2


typedef struct {
    ngx_flag_t    enable;
    ngx_flag_t    silent_errors;
    ngx_flag_t    ignore_recycled_buffers;
    ngx_flag_t    last_modified;

    ngx_hash_t    types;

    size_t        min_file_chunk;
    size_t        value_len;

    ngx_array_t  *types_keys;
} ngx_http_ssi_loc_conf_t;


typedef struct {
    ngx_str_t     name;
    ngx_uint_t    key;
    ngx_str_t     value;
} ngx_http_ssi_var_t;


typedef struct {
    ngx_str_t     name;
    ngx_chain_t  *bufs;
    ngx_uint_t    count;
} ngx_http_ssi_block_t;


typedef enum {
    ssi_start_state = 0,
    ssi_tag_state,
    ssi_comment0_state,
    ssi_comment1_state,
    ssi_sharp_state,
    ssi_precommand_state,
    ssi_command_state,
    ssi_preparam_state,
    ssi_param_state,
    ssi_preequal_state,
    ssi_prevalue_state,
    ssi_double_quoted_value_state,
    ssi_quoted_value_state,
    ssi_quoted_symbol_state,
    ssi_postparam_state,
    ssi_comment_end0_state,
    ssi_comment_end1_state,
    ssi_error_state,
    ssi_error_end0_state,
    ssi_error_end1_state
} ngx_http_ssi_state_e;


static ngx_int_t ngx_http_ssi_output(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static void ngx_http_ssi_buffered(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_int_t ngx_http_ssi_parse(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_str_t *ngx_http_ssi_get_variable(ngx_http_request_t *r,
    ngx_str_t *name, ngx_uint_t key);
static ngx_int_t ngx_http_ssi_evaluate_string(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t *text, ngx_uint_t flags);
static ngx_int_t ngx_http_ssi_regex_match(ngx_http_request_t *r,
    ngx_str_t *pattern, ngx_str_t *str);

static ngx_int_t ngx_http_ssi_include(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_stub_output(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_echo(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_config(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_set(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_if(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_else(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endif(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_block(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endblock(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);

static ngx_int_t ngx_http_ssi_date_gmt_local_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t gmt);

static ngx_int_t ngx_http_ssi_preconfiguration(ngx_conf_t *cf);
static void *ngx_http_ssi_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_ssi_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);
static ngx_int_t ngx_http_ssi_filter_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_ssi_filter_commands[] = {

    { ngx_string("ssi"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, enable),
      NULL },

    { ngx_string("ssi_silent_errors"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, silent_errors),
      NULL },

    { ngx_string("ssi_ignore_recycled_buffers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, ignore_recycled_buffers),
      NULL },

    { ngx_string("ssi_min_file_chunk"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, min_file_chunk),
      NULL },

    { ngx_string("ssi_value_length"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, value_len),
      NULL },

    { ngx_string("ssi_types"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
      ngx_http_types_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, types_keys),
      &ngx_http_html_default_types[0] },

    { ngx_string("ssi_last_modified"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, last_modified),
      NULL },

      ngx_null_command
};



    ngx_http_ssi_preconfiguration,         /* preconfiguration */
    ngx_http_ssi_filter_init,              /* postconfiguration */

    ngx_http_ssi_create_main_conf,         /* create main configuration */
    ngx_http_ssi_init_main_conf,           /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_ssi_create_loc_conf,          /* create location configuration */
    ngx_http_ssi_merge_loc_conf            /* merge location configuration */
};


ngx_module_t  ngx_http_ssi_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_ssi_filter_module_ctx,       /* module context */
    ngx_http_ssi_filter_commands,          /* module directives */
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


static u_char ngx_http_ssi_string[] = "<!--";

static ngx_str_t ngx_http_ssi_timefmt = ngx_string("%A, %d-%b-%Y %H:%M:%S %Z");


#define  NGX_HTTP_SSI_INCLUDE_VIRTUAL  0
#define  NGX_HTTP_SSI_INCLUDE_FILE     1
#define  NGX_HTTP_SSI_INCLUDE_WAIT     2
#define  NGX_HTTP_SSI_INCLUDE_SET      3
#define  NGX_HTTP_SSI_INCLUDE_STUB     4

#define  NGX_HTTP_SSI_ECHO_VAR         0
#define  NGX_HTTP_SSI_ECHO_DEFAULT     1
#define  NGX_HTTP_SSI_ECHO_ENCODING    2

#define  NGX_HTTP_SSI_CONFIG_ERRMSG    0
#define  NGX_HTTP_SSI_CONFIG_TIMEFMT   1

#define  NGX_HTTP_SSI_SET_VAR          0
#define  NGX_HTTP_SSI_SET_VALUE        1

#define  NGX_HTTP_SSI_IF_EXPR          0

#define  NGX_HTTP_SSI_BLOCK_NAME       0


    { ngx_string("virtual"), NGX_HTTP_SSI_INCLUDE_VIRTUAL, 0, 0 },
    { ngx_string("file"), NGX_HTTP_SSI_INCLUDE_FILE, 0, 0 },
    { ngx_string("wait"), NGX_HTTP_SSI_INCLUDE_WAIT, 0, 0 },
    { ngx_string("set"), NGX_HTTP_SSI_INCLUDE_SET, 0, 0 },
    { ngx_string("stub"), NGX_HTTP_SSI_INCLUDE_STUB, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_ECHO_VAR, 1, 0 },
    { ngx_string("default"), NGX_HTTP_SSI_ECHO_DEFAULT, 0, 0 },
    { ngx_string("encoding"), NGX_HTTP_SSI_ECHO_ENCODING, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("errmsg"), NGX_HTTP_SSI_CONFIG_ERRMSG, 0, 0 },
    { ngx_string("timefmt"), NGX_HTTP_SSI_CONFIG_TIMEFMT, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_SET_VAR, 1, 0 },
    { ngx_string("value"), NGX_HTTP_SSI_SET_VALUE, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("expr"), NGX_HTTP_SSI_IF_EXPR, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("name"), NGX_HTTP_SSI_BLOCK_NAME, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("include"), ngx_http_ssi_include,
                       ngx_http_ssi_include_params, 0, 0, 1 },
    { ngx_string("echo"), ngx_http_ssi_echo,
                       ngx_http_ssi_echo_params, 0, 0, 0 },
    { ngx_string("config"), ngx_http_ssi_config,
                       ngx_http_ssi_config_params, 0, 0, 0 },
    { ngx_string("set"), ngx_http_ssi_set, ngx_http_ssi_set_params, 0, 0, 0 },

    { ngx_string("if"), ngx_http_ssi_if, ngx_http_ssi_if_params, 0, 0, 0 },
    { ngx_string("elif"), ngx_http_ssi_if, ngx_http_ssi_if_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("else"), ngx_http_ssi_else, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("endif"), ngx_http_ssi_endif, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_ELSE, 0, 0 },

    { ngx_string("block"), ngx_http_ssi_block,
                       ngx_http_ssi_block_params, 0, 0, 0 },
    { ngx_string("endblock"), ngx_http_ssi_endblock,
                       ngx_http_ssi_no_params, 0, 1, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_http_variable_t  ngx_http_ssi_vars[] = {

    { ngx_string("date_local"), NULL, ngx_http_ssi_date_gmt_local_variable, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("date_gmt"), NULL, ngx_http_ssi_date_gmt_local_variable, 1,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

      ngx_http_null_variable
};



ngx_int_t ngx_http_ssi_header_filter_sense_1(int *ngx_http_ssi_header_filter_sense_1_ret, ngx_http_ssi_ctx_t *ctx, ngx_http_ssi_loc_conf_t *slcf, ngx_http_request_t *r)
{
  slcf = r->loc_conf[ngx_http_ssi_filter_module.ctx_index];
  if (((!slcf->enable) || (r->headers_out.content_length_n == 0)) || (ngx_http_test_content_type(r, &slcf->types) == 0))
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  if (ctx == 0)
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return -1;
    }
  }
  r->ctx[ngx_http_ssi_filter_module.ctx_index] = ctx;
  ;
  r->filter_need_in_memory = 1;
  if (r == r->main)
  {
    r->headers_out.content_length_n = -1;
    if (r->headers_out.content_length)
    {
      r->headers_out.content_length->hash = 0;
      r->headers_out.content_length = 0;
    }
    ;
    r->allow_ranges = 0;
    if (r->headers_out.accept_ranges)
    {
      r->headers_out.accept_ranges->hash = 0;
      r->headers_out.accept_ranges = 0;
    }
    ;
    r->preserve_body = 1;
    if (!slcf->last_modified)
    {
      r->headers_out.last_modified_time = -1;
      if (r->headers_out.last_modified)
      {
        r->headers_out.last_modified->hash = 0;
        r->headers_out.last_modified = 0;
      }
      ;
      if (r->headers_out.etag)
      {
        r->headers_out.etag->hash = 0;
        r->headers_out.etag = 0;
      }
      ;
    }
    else
    {
      ngx_http_weak_etag(r);
    }
  }
  {
    *ngx_http_ssi_header_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}





















static ngx_int_t
ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    ngx_str_t  *value = data;

    if (r->headers_out.status < NGX_HTTP_SPECIAL_RESPONSE
        && r->out && r->out->buf)
    {
        value->len = r->out->buf->last - r->out->buf->pos;
        value->data = r->out->buf->pos;
    }

    return rc;
}


























static void *
ngx_http_ssi_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_ssi_loc_conf_t  *slcf;

    slcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssi_loc_conf_t));
    if (slcf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->types = { NULL };
     *     conf->types_keys = NULL;
     */

    slcf->enable = NGX_CONF_UNSET;
    slcf->silent_errors = NGX_CONF_UNSET;
    slcf->ignore_recycled_buffers = NGX_CONF_UNSET;
    slcf->last_modified = NGX_CONF_UNSET;

    slcf->min_file_chunk = NGX_CONF_UNSET_SIZE;
    slcf->value_len = NGX_CONF_UNSET_SIZE;

    return slcf;
}


static char *
ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ssi_loc_conf_t *prev = parent;
    ngx_http_ssi_loc_conf_t *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);
    ngx_conf_merge_value(conf->silent_errors, prev->silent_errors, 0);
    ngx_conf_merge_value(conf->ignore_recycled_buffers,
                         prev->ignore_recycled_buffers, 0);
    ngx_conf_merge_value(conf->last_modified, prev->last_modified, 0);

    ngx_conf_merge_size_value(conf->min_file_chunk, prev->min_file_chunk, 1024);
    ngx_conf_merge_size_value(conf->value_len, prev->value_len, 255);

    if (ngx_http_merge_types(cf, &conf->types_keys, &conf->types,
                             &prev->types_keys, &prev->types,
                             ngx_http_html_default_types)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_ssi_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_ssi_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_ssi_body_filter;

    return NGX_OK;

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_ssi_filter_module.h"
#define NGX_HTTP_SSI_ERROR          1

#define NGX_HTTP_SSI_DATE_LEN       2048

#define NGX_HTTP_SSI_ADD_PREFIX     1
#define NGX_HTTP_SSI_ADD_ZERO       2


typedef struct {
    ngx_flag_t    enable;
    ngx_flag_t    silent_errors;
    ngx_flag_t    ignore_recycled_buffers;
    ngx_flag_t    last_modified;

    ngx_hash_t    types;

    size_t        min_file_chunk;
    size_t        value_len;

    ngx_array_t  *types_keys;
} ngx_http_ssi_loc_conf_t;


typedef struct {
    ngx_str_t     name;
    ngx_uint_t    key;
    ngx_str_t     value;
} ngx_http_ssi_var_t;


typedef struct {
    ngx_str_t     name;
    ngx_chain_t  *bufs;
    ngx_uint_t    count;
} ngx_http_ssi_block_t;


typedef enum {
    ssi_start_state = 0,
    ssi_tag_state,
    ssi_comment0_state,
    ssi_comment1_state,
    ssi_sharp_state,
    ssi_precommand_state,
    ssi_command_state,
    ssi_preparam_state,
    ssi_param_state,
    ssi_preequal_state,
    ssi_prevalue_state,
    ssi_double_quoted_value_state,
    ssi_quoted_value_state,
    ssi_quoted_symbol_state,
    ssi_postparam_state,
    ssi_comment_end0_state,
    ssi_comment_end1_state,
    ssi_error_state,
    ssi_error_end0_state,
    ssi_error_end1_state
} ngx_http_ssi_state_e;


static ngx_int_t ngx_http_ssi_output(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static void ngx_http_ssi_buffered(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_int_t ngx_http_ssi_parse(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx);
static ngx_str_t *ngx_http_ssi_get_variable(ngx_http_request_t *r,
    ngx_str_t *name, ngx_uint_t key);
static ngx_int_t ngx_http_ssi_evaluate_string(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t *text, ngx_uint_t flags);
static ngx_int_t ngx_http_ssi_regex_match(ngx_http_request_t *r,
    ngx_str_t *pattern, ngx_str_t *str);

static ngx_int_t ngx_http_ssi_include(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_stub_output(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data,
    ngx_int_t rc);
static ngx_int_t ngx_http_ssi_echo(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_config(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_set(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_if(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_else(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endif(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_block(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);
static ngx_int_t ngx_http_ssi_endblock(ngx_http_request_t *r,
    ngx_http_ssi_ctx_t *ctx, ngx_str_t **params);

static ngx_int_t ngx_http_ssi_date_gmt_local_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t gmt);

static ngx_int_t ngx_http_ssi_preconfiguration(ngx_conf_t *cf);
static void *ngx_http_ssi_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_init_main_conf(ngx_conf_t *cf, void *conf);
static void *ngx_http_ssi_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);
static ngx_int_t ngx_http_ssi_filter_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_ssi_filter_commands[] = {

    { ngx_string("ssi"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, enable),
      NULL },

    { ngx_string("ssi_silent_errors"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, silent_errors),
      NULL },

    { ngx_string("ssi_ignore_recycled_buffers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, ignore_recycled_buffers),
      NULL },

    { ngx_string("ssi_min_file_chunk"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, min_file_chunk),
      NULL },

    { ngx_string("ssi_value_length"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, value_len),
      NULL },

    { ngx_string("ssi_types"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
      ngx_http_types_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, types_keys),
      &ngx_http_html_default_types[0] },

    { ngx_string("ssi_last_modified"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_ssi_loc_conf_t, last_modified),
      NULL },

      ngx_null_command
};



    ngx_http_ssi_preconfiguration,         /* preconfiguration */
    ngx_http_ssi_filter_init,              /* postconfiguration */

    ngx_http_ssi_create_main_conf,         /* create main configuration */
    ngx_http_ssi_init_main_conf,           /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_ssi_create_loc_conf,          /* create location configuration */
    ngx_http_ssi_merge_loc_conf            /* merge location configuration */
};


ngx_module_t  ngx_http_ssi_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_ssi_filter_module_ctx,       /* module context */
    ngx_http_ssi_filter_commands,          /* module directives */
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


static u_char ngx_http_ssi_string[] = "<!--";

static ngx_str_t ngx_http_ssi_timefmt = ngx_string("%A, %d-%b-%Y %H:%M:%S %Z");


#define  NGX_HTTP_SSI_INCLUDE_VIRTUAL  0
#define  NGX_HTTP_SSI_INCLUDE_FILE     1
#define  NGX_HTTP_SSI_INCLUDE_WAIT     2
#define  NGX_HTTP_SSI_INCLUDE_SET      3
#define  NGX_HTTP_SSI_INCLUDE_STUB     4

#define  NGX_HTTP_SSI_ECHO_VAR         0
#define  NGX_HTTP_SSI_ECHO_DEFAULT     1
#define  NGX_HTTP_SSI_ECHO_ENCODING    2

#define  NGX_HTTP_SSI_CONFIG_ERRMSG    0
#define  NGX_HTTP_SSI_CONFIG_TIMEFMT   1

#define  NGX_HTTP_SSI_SET_VAR          0
#define  NGX_HTTP_SSI_SET_VALUE        1

#define  NGX_HTTP_SSI_IF_EXPR          0

#define  NGX_HTTP_SSI_BLOCK_NAME       0


    { ngx_string("virtual"), NGX_HTTP_SSI_INCLUDE_VIRTUAL, 0, 0 },
    { ngx_string("file"), NGX_HTTP_SSI_INCLUDE_FILE, 0, 0 },
    { ngx_string("wait"), NGX_HTTP_SSI_INCLUDE_WAIT, 0, 0 },
    { ngx_string("set"), NGX_HTTP_SSI_INCLUDE_SET, 0, 0 },
    { ngx_string("stub"), NGX_HTTP_SSI_INCLUDE_STUB, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_ECHO_VAR, 1, 0 },
    { ngx_string("default"), NGX_HTTP_SSI_ECHO_DEFAULT, 0, 0 },
    { ngx_string("encoding"), NGX_HTTP_SSI_ECHO_ENCODING, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("errmsg"), NGX_HTTP_SSI_CONFIG_ERRMSG, 0, 0 },
    { ngx_string("timefmt"), NGX_HTTP_SSI_CONFIG_TIMEFMT, 0, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("var"), NGX_HTTP_SSI_SET_VAR, 1, 0 },
    { ngx_string("value"), NGX_HTTP_SSI_SET_VALUE, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("expr"), NGX_HTTP_SSI_IF_EXPR, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("name"), NGX_HTTP_SSI_BLOCK_NAME, 1, 0 },
    { ngx_null_string, 0, 0, 0 }
};


    { ngx_null_string, 0, 0, 0 }
};


    { ngx_string("include"), ngx_http_ssi_include,
                       ngx_http_ssi_include_params, 0, 0, 1 },
    { ngx_string("echo"), ngx_http_ssi_echo,
                       ngx_http_ssi_echo_params, 0, 0, 0 },
    { ngx_string("config"), ngx_http_ssi_config,
                       ngx_http_ssi_config_params, 0, 0, 0 },
    { ngx_string("set"), ngx_http_ssi_set, ngx_http_ssi_set_params, 0, 0, 0 },

    { ngx_string("if"), ngx_http_ssi_if, ngx_http_ssi_if_params, 0, 0, 0 },
    { ngx_string("elif"), ngx_http_ssi_if, ngx_http_ssi_if_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("else"), ngx_http_ssi_else, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_IF, 0, 0 },
    { ngx_string("endif"), ngx_http_ssi_endif, ngx_http_ssi_no_params,
                       NGX_HTTP_SSI_COND_ELSE, 0, 0 },

    { ngx_string("block"), ngx_http_ssi_block,
                       ngx_http_ssi_block_params, 0, 0, 0 },
    { ngx_string("endblock"), ngx_http_ssi_endblock,
                       ngx_http_ssi_no_params, 0, 1, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_http_variable_t  ngx_http_ssi_vars[] = {

    { ngx_string("date_local"), NULL, ngx_http_ssi_date_gmt_local_variable, 0,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("date_gmt"), NULL, ngx_http_ssi_date_gmt_local_variable, 1,
      NGX_HTTP_VAR_NOCACHEABLE, 0 },

      ngx_http_null_variable
};



ngx_int_t ngx_http_ssi_header_filter_sense_1(int *ngx_http_ssi_header_filter_sense_1_ret, ngx_http_ssi_ctx_t *ctx, ngx_http_ssi_loc_conf_t *slcf, ngx_http_request_t *r)
{
  slcf = r->loc_conf[ngx_http_ssi_filter_module.ctx_index];
  if (((!slcf->enable) || (r->headers_out.content_length_n == 0)) || (ngx_http_test_content_type(r, &slcf->types) == 0))
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return ngx_http_next_header_filter(r);
    }
  }
  if (ctx == 0)
  {
    {
      *ngx_http_ssi_header_filter_sense_1_ret = 1;
      return -1;
    }
  }
  r->ctx[ngx_http_ssi_filter_module.ctx_index] = ctx;
  ;
  r->filter_need_in_memory = 1;
  if (r == r->main)
  {
    r->headers_out.content_length_n = -1;
    if (r->headers_out.content_length)
    {
      r->headers_out.content_length->hash = 0;
      r->headers_out.content_length = 0;
    }
    ;
    r->allow_ranges = 0;
    if (r->headers_out.accept_ranges)
    {
      r->headers_out.accept_ranges->hash = 0;
      r->headers_out.accept_ranges = 0;
    }
    ;
    r->preserve_body = 1;
    if (!slcf->last_modified)
    {
      r->headers_out.last_modified_time = -1;
      if (r->headers_out.last_modified)
      {
        r->headers_out.last_modified->hash = 0;
        r->headers_out.last_modified = 0;
      }
      ;
      if (r->headers_out.etag)
      {
        r->headers_out.etag->hash = 0;
        r->headers_out.etag = 0;
      }
      ;
    }
    else
    {
      ngx_http_weak_etag(r);
    }
  }
  {
    *ngx_http_ssi_header_filter_sense_1_ret = 1;
    return ngx_http_next_header_filter(r);
  }
}





















static ngx_int_t
ngx_http_ssi_set_variable(ngx_http_request_t *r, void *data, ngx_int_t rc)
{
    ngx_str_t  *value = data;

    if (r->headers_out.status < NGX_HTTP_SPECIAL_RESPONSE
        && r->out && r->out->buf)
    {
        value->len = r->out->buf->last - r->out->buf->pos;
        value->data = r->out->buf->pos;
    }

    return rc;
}


























static void *
ngx_http_ssi_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_ssi_loc_conf_t  *slcf;

    slcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssi_loc_conf_t));
    if (slcf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->types = { NULL };
     *     conf->types_keys = NULL;
     */

    slcf->enable = NGX_CONF_UNSET;
    slcf->silent_errors = NGX_CONF_UNSET;
    slcf->ignore_recycled_buffers = NGX_CONF_UNSET;
    slcf->last_modified = NGX_CONF_UNSET;

    slcf->min_file_chunk = NGX_CONF_UNSET_SIZE;
    slcf->value_len = NGX_CONF_UNSET_SIZE;

    return slcf;
}


static char *
ngx_http_ssi_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ssi_loc_conf_t *prev = parent;
    ngx_http_ssi_loc_conf_t *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);
    ngx_conf_merge_value(conf->silent_errors, prev->silent_errors, 0);
    ngx_conf_merge_value(conf->ignore_recycled_buffers,
                         prev->ignore_recycled_buffers, 0);
    ngx_conf_merge_value(conf->last_modified, prev->last_modified, 0);

    ngx_conf_merge_size_value(conf->min_file_chunk, prev->min_file_chunk, 1024);
    ngx_conf_merge_size_value(conf->value_len, prev->value_len, 255);

    if (ngx_http_merge_types(cf, &conf->types_keys, &conf->types,
                             &prev->types_keys, &prev->types,
                             ngx_http_html_default_types)
        != NGX_OK)
    {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_ssi_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_ssi_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_ssi_body_filter;

    return NGX_OK;
}
