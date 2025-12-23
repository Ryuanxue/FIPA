
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    ngx_http_upstream_rr_peer_t          *peer;
    ngx_uint_t                            range;
} ngx_http_upstream_random_range_t;


typedef struct {
    ngx_uint_t                            two;
    ngx_http_upstream_random_range_t     *ranges;
} ngx_http_upstream_random_srv_conf_t;


typedef struct {
    /* the round robin data must be first */
    ngx_http_upstream_rr_peer_data_t      rrp;

    ngx_http_upstream_random_srv_conf_t  *conf;
    u_char                                tries;
} ngx_http_upstream_random_peer_data_t;


static ngx_int_t ngx_http_upstream_init_random(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_update_random(ngx_pool_t *pool,
    ngx_http_upstream_srv_conf_t *us);

static ngx_int_t ngx_http_upstream_init_random_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_get_random_peer(ngx_peer_connection_t *pc,
    void *data);
static ngx_int_t ngx_http_upstream_get_random2_peer(ngx_peer_connection_t *pc,
    void *data);
static ngx_uint_t ngx_http_upstream_peek_random_peer(
    ngx_http_upstream_rr_peers_t *peers,
    ngx_http_upstream_random_peer_data_t *rp);
static void *ngx_http_upstream_random_create_conf(ngx_conf_t *cf);
static char *ngx_http_upstream_random(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


static ngx_command_t  ngx_http_upstream_random_commands[] = {

    { ngx_string("random"),
      NGX_HTTP_UPS_CONF|NGX_CONF_NOARGS|NGX_CONF_TAKE12,
      ngx_http_upstream_random,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_upstream_random_create_conf,  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


    NGX_MODULE_V1,
    &ngx_http_upstream_random_module_ctx,  /* module context */
    ngx_http_upstream_random_commands,     /* module directives */
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
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    ngx_http_upstream_rr_peer_t          *peer;
    ngx_uint_t                            range;
} ngx_http_upstream_random_range_t;


typedef struct {
    ngx_uint_t                            two;
    ngx_http_upstream_random_range_t     *ranges;
} ngx_http_upstream_random_srv_conf_t;


typedef struct {
    /* the round robin data must be first */
    ngx_http_upstream_rr_peer_data_t      rrp;

    ngx_http_upstream_random_srv_conf_t  *conf;
    u_char                                tries;
} ngx_http_upstream_random_peer_data_t;


static ngx_int_t ngx_http_upstream_init_random(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_update_random(ngx_pool_t *pool,
    ngx_http_upstream_srv_conf_t *us);

static ngx_int_t ngx_http_upstream_init_random_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_get_random_peer(ngx_peer_connection_t *pc,
    void *data);
static ngx_int_t ngx_http_upstream_get_random2_peer(ngx_peer_connection_t *pc,
    void *data);
static ngx_uint_t ngx_http_upstream_peek_random_peer(
    ngx_http_upstream_rr_peers_t *peers,
    ngx_http_upstream_random_peer_data_t *rp);
static void *ngx_http_upstream_random_create_conf(ngx_conf_t *cf);
static char *ngx_http_upstream_random(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


static ngx_command_t  ngx_http_upstream_random_commands[] = {

    { ngx_string("random"),
      NGX_HTTP_UPS_CONF|NGX_CONF_NOARGS|NGX_CONF_TAKE12,
      ngx_http_upstream_random,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_upstream_random_create_conf,  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


    NGX_MODULE_V1,
    &ngx_http_upstream_random_module_ctx,  /* module context */
    ngx_http_upstream_random_commands,     /* module directives */
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
















