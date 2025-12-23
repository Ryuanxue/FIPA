
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    uint32_t                    percent;
    ngx_http_variable_value_t   value;
} ngx_http_split_clients_part_t;


typedef struct {
    ngx_http_complex_value_t    value;
    ngx_array_t                 parts;
} ngx_http_split_clients_ctx_t;


static char *ngx_conf_split_clients_block(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_split_clients(ngx_conf_t *cf, ngx_command_t *dummy,
    void *conf);

static ngx_command_t  ngx_http_split_clients_commands[] = {

    { ngx_string("split_clients"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_TAKE2,
      ngx_conf_split_clients_block,
      NGX_HTTP_MAIN_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


    NGX_MODULE_V1,
    &ngx_http_split_clients_module_ctx,    /* module context */
    ngx_http_split_clients_commands,       /* module directives */
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






/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    uint32_t                    percent;
    ngx_http_variable_value_t   value;
} ngx_http_split_clients_part_t;


typedef struct {
    ngx_http_complex_value_t    value;
    ngx_array_t                 parts;
} ngx_http_split_clients_ctx_t;


static char *ngx_conf_split_clients_block(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_split_clients(ngx_conf_t *cf, ngx_command_t *dummy,
    void *conf);

static ngx_command_t  ngx_http_split_clients_commands[] = {

    { ngx_string("split_clients"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_TAKE2,
      ngx_conf_split_clients_block,
      NGX_HTTP_MAIN_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


    NGX_MODULE_V1,
    &ngx_http_split_clients_module_ctx,    /* module context */
    ngx_http_split_clients_commands,       /* module directives */
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






