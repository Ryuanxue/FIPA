
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    ngx_array_t  *mirror;
    ngx_flag_t    request_body;
} ngx_http_mirror_loc_conf_t;


typedef struct {
    ngx_int_t     status;
} ngx_http_mirror_ctx_t;


static ngx_int_t ngx_http_mirror_handler(ngx_http_request_t *r);
static void ngx_http_mirror_body_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_mirror_handler_internal(ngx_http_request_t *r);
static void *ngx_http_mirror_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_mirror_merge_loc_conf(ngx_conf_t *cf, void *parent,
    void *child);
static char *ngx_http_mirror(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_mirror_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_mirror_commands[] = {

    { ngx_string("mirror"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_http_mirror,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("mirror_request_body"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_mirror_loc_conf_t, request_body),
      NULL },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    ngx_http_mirror_init,                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_mirror_create_loc_conf,       /* create location configuration */
    ngx_http_mirror_merge_loc_conf         /* merge location configuration */
};


ngx_module_t  ngx_http_mirror_module = {
    NGX_MODULE_V1,
    &ngx_http_mirror_module_ctx,           /* module context */
    ngx_http_mirror_commands,              /* module directives */
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


ngx_int_t ngx_http_mirror_handler_sense_1(int *ngx_http_mirror_handler_sense_1_ret, ngx_int_t *rc, ngx_http_mirror_ctx_t *ctx, ngx_http_mirror_loc_conf_t *mlcf, ngx_http_request_t *r)
{
  if (r != r->main)
  {
    {
      *ngx_http_mirror_handler_sense_1_ret = 1;
      return -5;
    }
  }
  mlcf = r->loc_conf[ngx_http_mirror_module.ctx_index];
  if (mlcf->mirror == 0)
  {
    {
      *ngx_http_mirror_handler_sense_1_ret = 1;
      return -5;
    }
  }
  if (mlcf->request_body)
  {
    ctx = r->ctx[ngx_http_mirror_module.ctx_index];
    if (ctx)
    {
      {
        *ngx_http_mirror_handler_sense_1_ret = 1;
        return ctx->status;
      }
    }
    ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_mirror_ctx_t));
    if (ctx == 0)
    {
      {
        *ngx_http_mirror_handler_sense_1_ret = 1;
        return -1;
      }
    }
    ctx->status = -4;
    r->ctx[ngx_http_mirror_module.ctx_index] = ctx;
    ;
    *rc = ngx_http_read_client_request_body(r, ngx_http_mirror_body_handler);
    if ((*rc) >= 300)
    {
      {
        *ngx_http_mirror_handler_sense_1_ret = 1;
        return *rc;
      }
    }
    ngx_http_finalize_request(r, -4);
    {
      *ngx_http_mirror_handler_sense_1_ret = 1;
      return -4;
    }
  }
  {
    *ngx_http_mirror_handler_sense_1_ret = 1;
    return ngx_http_mirror_handler_internal(r);
  }
}





static ngx_int_t
ngx_http_mirror_handler_internal(ngx_http_request_t *r)
{
    ngx_str_t                   *name;
    ngx_uint_t                   i;
    ngx_http_request_t          *sr;
    ngx_http_mirror_loc_conf_t  *mlcf;

    mlcf = ngx_http_get_module_loc_conf(r, ngx_http_mirror_module);

    name = mlcf->mirror->elts;

    for (i = 0; i < mlcf->mirror->nelts; i++) {
        if (ngx_http_subrequest(r, &name[i], &r->args, &sr, NULL,
                                NGX_HTTP_SUBREQUEST_BACKGROUND)
            != NGX_OK)
        {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        sr->header_only = 1;
        sr->method = r->method;
        sr->method_name = r->method_name;
    }

    return NGX_DECLINED;
}


static void *
ngx_http_mirror_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_mirror_loc_conf_t  *mlcf;

    mlcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_mirror_loc_conf_t));
    if (mlcf == NULL) {
        return NULL;
    }

    mlcf->mirror = NGX_CONF_UNSET_PTR;
    mlcf->request_body = NGX_CONF_UNSET;

    return mlcf;
}


static char *
ngx_http_mirror_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_mirror_loc_conf_t *prev = parent;
    ngx_http_mirror_loc_conf_t *conf = child;

    ngx_conf_merge_ptr_value(conf->mirror, prev->mirror, NULL);
    ngx_conf_merge_value(conf->request_body, prev->request_body, 1);

    return NGX_CONF_OK;
}




ngx_int_t ngx_http_mirror_init_sense_1(int *ngx_http_mirror_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_mirror_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_mirror_handler;
  {
    *ngx_http_mirror_init_sense_1_ret = 1;
    return 0;
  }
}

/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    ngx_array_t  *mirror;
    ngx_flag_t    request_body;
} ngx_http_mirror_loc_conf_t;


typedef struct {
    ngx_int_t     status;
} ngx_http_mirror_ctx_t;


static ngx_int_t ngx_http_mirror_handler(ngx_http_request_t *r);
static void ngx_http_mirror_body_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_mirror_handler_internal(ngx_http_request_t *r);
static void *ngx_http_mirror_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_mirror_merge_loc_conf(ngx_conf_t *cf, void *parent,
    void *child);
static char *ngx_http_mirror(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_mirror_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_mirror_commands[] = {

    { ngx_string("mirror"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_http_mirror,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("mirror_request_body"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_mirror_loc_conf_t, request_body),
      NULL },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    ngx_http_mirror_init,                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_mirror_create_loc_conf,       /* create location configuration */
    ngx_http_mirror_merge_loc_conf         /* merge location configuration */
};


ngx_module_t  ngx_http_mirror_module = {
    NGX_MODULE_V1,
    &ngx_http_mirror_module_ctx,           /* module context */
    ngx_http_mirror_commands,              /* module directives */
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


ngx_int_t ngx_http_mirror_handler_sense_1(int *ngx_http_mirror_handler_sense_1_ret, ngx_int_t *rc, ngx_http_mirror_ctx_t *ctx, ngx_http_mirror_loc_conf_t *mlcf, ngx_http_request_t *r)
{
  if (r != r->main)
  {
    {
      *ngx_http_mirror_handler_sense_1_ret = 1;
      return -5;
    }
  }
  mlcf = r->loc_conf[ngx_http_mirror_module.ctx_index];
  if (mlcf->mirror == 0)
  {
    {
      *ngx_http_mirror_handler_sense_1_ret = 1;
      return -5;
    }
  }
  if (mlcf->request_body)
  {
    ctx = r->ctx[ngx_http_mirror_module.ctx_index];
    if (ctx)
    {
      {
        *ngx_http_mirror_handler_sense_1_ret = 1;
        return ctx->status;
      }
    }
    ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_mirror_ctx_t));
    if (ctx == 0)
    {
      {
        *ngx_http_mirror_handler_sense_1_ret = 1;
        return -1;
      }
    }
    ctx->status = -4;
    r->ctx[ngx_http_mirror_module.ctx_index] = ctx;
    ;
    *rc = ngx_http_read_client_request_body(r, ngx_http_mirror_body_handler);
    if ((*rc) >= 300)
    {
      {
        *ngx_http_mirror_handler_sense_1_ret = 1;
        return *rc;
      }
    }
    ngx_http_finalize_request(r, -4);
    {
      *ngx_http_mirror_handler_sense_1_ret = 1;
      return -4;
    }
  }
  {
    *ngx_http_mirror_handler_sense_1_ret = 1;
    return ngx_http_mirror_handler_internal(r);
  }
}





static ngx_int_t
ngx_http_mirror_handler_internal(ngx_http_request_t *r)
{
    ngx_str_t                   *name;
    ngx_uint_t                   i;
    ngx_http_request_t          *sr;
    ngx_http_mirror_loc_conf_t  *mlcf;

    mlcf = ngx_http_get_module_loc_conf(r, ngx_http_mirror_module);

    name = mlcf->mirror->elts;

    for (i = 0; i < mlcf->mirror->nelts; i++) {
        if (ngx_http_subrequest(r, &name[i], &r->args, &sr, NULL,
                                NGX_HTTP_SUBREQUEST_BACKGROUND)
            != NGX_OK)
        {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        sr->header_only = 1;
        sr->method = r->method;
        sr->method_name = r->method_name;
    }

    return NGX_DECLINED;
}


static void *
ngx_http_mirror_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_mirror_loc_conf_t  *mlcf;

    mlcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_mirror_loc_conf_t));
    if (mlcf == NULL) {
        return NULL;
    }

    mlcf->mirror = NGX_CONF_UNSET_PTR;
    mlcf->request_body = NGX_CONF_UNSET;

    return mlcf;
}


static char *
ngx_http_mirror_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_mirror_loc_conf_t *prev = parent;
    ngx_http_mirror_loc_conf_t *conf = child;

    ngx_conf_merge_ptr_value(conf->mirror, prev->mirror, NULL);
    ngx_conf_merge_value(conf->request_body, prev->request_body, 1);

    return NGX_CONF_OK;
}




ngx_int_t ngx_http_mirror_init_sense_1(int *ngx_http_mirror_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_mirror_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_mirror_handler;
  {
    *ngx_http_mirror_init_sense_1_ret = 1;
    return 0;
  }
}

