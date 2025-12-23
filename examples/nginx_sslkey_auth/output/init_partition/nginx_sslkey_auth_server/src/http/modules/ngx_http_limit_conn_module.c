
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    u_char                     color;
    u_char                     len;
    u_short                    conn;
    u_char                     data[1];
} ngx_http_limit_conn_node_t;


typedef struct {
    ngx_shm_zone_t            *shm_zone;
    ngx_rbtree_node_t         *node;
} ngx_http_limit_conn_cleanup_t;


typedef struct {
    ngx_rbtree_t              *rbtree;
    ngx_http_complex_value_t   key;
} ngx_http_limit_conn_ctx_t;


typedef struct {
    ngx_shm_zone_t            *shm_zone;
    ngx_uint_t                 conn;
} ngx_http_limit_conn_limit_t;


typedef struct {
    ngx_array_t                limits;
    ngx_uint_t                 log_level;
    ngx_uint_t                 status_code;
} ngx_http_limit_conn_conf_t;


static ngx_rbtree_node_t *ngx_http_limit_conn_lookup(ngx_rbtree_t *rbtree,
    ngx_str_t *key, uint32_t hash);
static void ngx_http_limit_conn_cleanup(void *data);
static ngx_inline void ngx_http_limit_conn_cleanup_all(ngx_pool_t *pool);

static void *ngx_http_limit_conn_create_conf(ngx_conf_t *cf);
static char *ngx_http_limit_conn_merge_conf(ngx_conf_t *cf, void *parent,
    void *child);
static char *ngx_http_limit_conn_zone(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_limit_conn(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static ngx_int_t ngx_http_limit_conn_init(ngx_conf_t *cf);


static ngx_conf_enum_t  ngx_http_limit_conn_log_levels[] = {
    { ngx_string("info"), NGX_LOG_INFO },
    { ngx_string("notice"), NGX_LOG_NOTICE },
    { ngx_string("warn"), NGX_LOG_WARN },
    { ngx_string("error"), NGX_LOG_ERR },
    { ngx_null_string, 0 }
};


    ngx_conf_check_num_bounds, 400, 599
};


static ngx_command_t  ngx_http_limit_conn_commands[] = {

    { ngx_string("limit_conn_zone"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE2,
      ngx_http_limit_conn_zone,
      0,
      0,
      NULL },

    { ngx_string("limit_conn"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE2,
      ngx_http_limit_conn,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("limit_conn_log_level"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_conn_conf_t, log_level),
      &ngx_http_limit_conn_log_levels },

    { ngx_string("limit_conn_status"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_conn_conf_t, status_code),
      &ngx_http_limit_conn_status_bounds },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    ngx_http_limit_conn_init,              /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_limit_conn_create_conf,       /* create location configuration */
    ngx_http_limit_conn_merge_conf         /* merge location configuration */
};


ngx_module_t  ngx_http_limit_conn_module = {
    NGX_MODULE_V1,
    &ngx_http_limit_conn_module_ctx,       /* module context */
    ngx_http_limit_conn_commands,          /* module directives */
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


ngx_int_t ngx_http_limit_conn_handler_sense_1(int *ngx_http_limit_conn_handler_sense_1_ret, ngx_http_request_t *r)
{
  size_t n;
  uint32_t hash;
  ngx_str_t key;
  ngx_uint_t i;
  ngx_slab_pool_t *shpool;
  ngx_rbtree_node_t *node;
  ngx_pool_cleanup_t *cln;
  ngx_http_limit_conn_ctx_t *ctx;
  ngx_http_limit_conn_node_t *lc;
  ngx_http_limit_conn_conf_t *lccf;
  ngx_http_limit_conn_limit_t *limits;
  ngx_http_limit_conn_cleanup_t *lccln;
  if (r->main->limit_conn_set)
  {
    {
      *ngx_http_limit_conn_handler_sense_1_ret = 1;
      return -5;
    }
  }
  lccf = r->loc_conf[ngx_http_limit_conn_module.ctx_index];
  limits = lccf->limits.elts;
  for (i = 0; i < lccf->limits.nelts; i++)
  {
    ctx = limits[i].shm_zone->data;
    if (ngx_http_complex_value(r, &ctx->key, &key) != 0)
    {
      {
        *ngx_http_limit_conn_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (key.len == 0)
    {
      continue;
    }
    if (key.len > 255)
    {
      if (r->connection->log->log_level >= 4)
        ngx_log_error_core(4, r->connection->log, 0, "the value of the \"%V\" key is more than 255 bytes: \"%V\"", &ctx->key.value, &key);
      continue;
    }
    r->main->limit_conn_set = 1;
    hash = ngx_crc32_short(key.data, key.len);
    shpool = (ngx_slab_pool_t *) limits[i].shm_zone->shm.addr;
    ngx_shmtx_lock(&shpool->mutex);
    node = ngx_http_limit_conn_lookup(ctx->rbtree, &key, hash);
    if (node == 0)
    {
      n = (0 + 0) + key.len;
      node = ngx_slab_alloc_locked(shpool, n);
      if (node == 0)
      {
        ngx_shmtx_unlock(&shpool->mutex);
        ngx_http_limit_conn_cleanup_all(r->pool);
        {
          *ngx_http_limit_conn_handler_sense_1_ret = 1;
          return lccf->status_code;
        }
      }
      lc = (ngx_http_limit_conn_node_t *) (&node->color);
      node->key = hash;
      lc->len = (u_char) key.len;
      lc->conn = 1;
      (void) memcpy(lc->data, key.data, key.len);
      ngx_rbtree_insert(ctx->rbtree, node);
    }
    else
    {
      lc = (ngx_http_limit_conn_node_t *) (&node->color);
      if (((ngx_uint_t) lc->conn) >= limits[i].conn)
      {
        ngx_shmtx_unlock(&shpool->mutex);
        if (r->connection->log->log_level >= lccf->log_level)
          ngx_log_error_core(lccf->log_level, r->connection->log, 0, "limiting connections by zone \"%V\"", &limits[i].shm_zone->shm.name);
        ngx_http_limit_conn_cleanup_all(r->pool);
        {
          *ngx_http_limit_conn_handler_sense_1_ret = 1;
          return lccf->status_code;
        }
      }
      lc->conn++;
    }
    ;
    ngx_shmtx_unlock(&shpool->mutex);
    cln = ngx_pool_cleanup_add(r->pool, sizeof(ngx_http_limit_conn_cleanup_t));
    if (cln == 0)
    {
      {
        *ngx_http_limit_conn_handler_sense_1_ret = 1;
        return 500;
      }
    }
    cln->handler = ngx_http_limit_conn_cleanup;
    lccln = cln->data;
    lccln->shm_zone = limits[i].shm_zone;
    lccln->node = node;
  }

  {
    *ngx_http_limit_conn_handler_sense_1_ret = 1;
    return -5;
  }
}





static ngx_rbtree_node_t *
ngx_http_limit_conn_lookup(ngx_rbtree_t *rbtree, ngx_str_t *key, uint32_t hash)
{
    ngx_int_t                    rc;
    ngx_rbtree_node_t           *node, *sentinel;
    ngx_http_limit_conn_node_t  *lcn;

    node = rbtree->root;
    sentinel = rbtree->sentinel;

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

        lcn = (ngx_http_limit_conn_node_t *) &node->color;

        rc = ngx_memn2cmp(key->data, lcn->data, key->len, (size_t) lcn->len);

        if (rc == 0) {
            return node;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    return NULL;
}


static void
ngx_http_limit_conn_cleanup(void *data)
{
    ngx_http_limit_conn_cleanup_t  *lccln = data;

    ngx_slab_pool_t             *shpool;
    ngx_rbtree_node_t           *node;
    ngx_http_limit_conn_ctx_t   *ctx;
    ngx_http_limit_conn_node_t  *lc;

    ctx = lccln->shm_zone->data;
    shpool = (ngx_slab_pool_t *) lccln->shm_zone->shm.addr;
    node = lccln->node;
    lc = (ngx_http_limit_conn_node_t *) &node->color;

    ngx_shmtx_lock(&shpool->mutex);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, lccln->shm_zone->shm.log, 0,
                   "limit conn cleanup: %08Xi %d", node->key, lc->conn);

    lc->conn--;

    if (lc->conn == 0) {
        ngx_rbtree_delete(ctx->rbtree, node);
        ngx_slab_free_locked(shpool, node);
    }

    ngx_shmtx_unlock(&shpool->mutex);
}


static ngx_inline void
ngx_http_limit_conn_cleanup_all(ngx_pool_t *pool)
{
    ngx_pool_cleanup_t  *cln;

    cln = pool->cleanup;

    while (cln && cln->handler == ngx_http_limit_conn_cleanup) {
        ngx_http_limit_conn_cleanup(cln->data);
        cln = cln->next;
    }

    pool->cleanup = cln;
}




static void *
ngx_http_limit_conn_create_conf(ngx_conf_t *cf)
{
    ngx_http_limit_conn_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_limit_conn_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->limits.elts = NULL;
     */

    conf->log_level = NGX_CONF_UNSET_UINT;
    conf->status_code = NGX_CONF_UNSET_UINT;

    return conf;
}


static char *
ngx_http_limit_conn_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_limit_conn_conf_t *prev = parent;
    ngx_http_limit_conn_conf_t *conf = child;

    if (conf->limits.elts == NULL) {
        conf->limits = prev->limits;
    }

    ngx_conf_merge_uint_value(conf->log_level, prev->log_level, NGX_LOG_ERR);
    ngx_conf_merge_uint_value(conf->status_code, prev->status_code,
                              NGX_HTTP_SERVICE_UNAVAILABLE);

    return NGX_CONF_OK;
}






ngx_int_t ngx_http_limit_conn_init_sense_1(int *ngx_http_limit_conn_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_limit_conn_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_limit_conn_handler;
  {
    *ngx_http_limit_conn_init_sense_1_ret = 1;
    return 0;
  }
}

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    u_char                     color;
    u_char                     len;
    u_short                    conn;
    u_char                     data[1];
} ngx_http_limit_conn_node_t;


typedef struct {
    ngx_shm_zone_t            *shm_zone;
    ngx_rbtree_node_t         *node;
} ngx_http_limit_conn_cleanup_t;


typedef struct {
    ngx_rbtree_t              *rbtree;
    ngx_http_complex_value_t   key;
} ngx_http_limit_conn_ctx_t;


typedef struct {
    ngx_shm_zone_t            *shm_zone;
    ngx_uint_t                 conn;
} ngx_http_limit_conn_limit_t;


typedef struct {
    ngx_array_t                limits;
    ngx_uint_t                 log_level;
    ngx_uint_t                 status_code;
} ngx_http_limit_conn_conf_t;


static ngx_rbtree_node_t *ngx_http_limit_conn_lookup(ngx_rbtree_t *rbtree,
    ngx_str_t *key, uint32_t hash);
static void ngx_http_limit_conn_cleanup(void *data);
static ngx_inline void ngx_http_limit_conn_cleanup_all(ngx_pool_t *pool);

static void *ngx_http_limit_conn_create_conf(ngx_conf_t *cf);
static char *ngx_http_limit_conn_merge_conf(ngx_conf_t *cf, void *parent,
    void *child);
static char *ngx_http_limit_conn_zone(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_limit_conn(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static ngx_int_t ngx_http_limit_conn_init(ngx_conf_t *cf);


static ngx_conf_enum_t  ngx_http_limit_conn_log_levels[] = {
    { ngx_string("info"), NGX_LOG_INFO },
    { ngx_string("notice"), NGX_LOG_NOTICE },
    { ngx_string("warn"), NGX_LOG_WARN },
    { ngx_string("error"), NGX_LOG_ERR },
    { ngx_null_string, 0 }
};


    ngx_conf_check_num_bounds, 400, 599
};


static ngx_command_t  ngx_http_limit_conn_commands[] = {

    { ngx_string("limit_conn_zone"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE2,
      ngx_http_limit_conn_zone,
      0,
      0,
      NULL },

    { ngx_string("limit_conn"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE2,
      ngx_http_limit_conn,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("limit_conn_log_level"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_conn_conf_t, log_level),
      &ngx_http_limit_conn_log_levels },

    { ngx_string("limit_conn_status"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_conn_conf_t, status_code),
      &ngx_http_limit_conn_status_bounds },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    ngx_http_limit_conn_init,              /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_limit_conn_create_conf,       /* create location configuration */
    ngx_http_limit_conn_merge_conf         /* merge location configuration */
};


ngx_module_t  ngx_http_limit_conn_module = {
    NGX_MODULE_V1,
    &ngx_http_limit_conn_module_ctx,       /* module context */
    ngx_http_limit_conn_commands,          /* module directives */
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


ngx_int_t ngx_http_limit_conn_handler_sense_1(int *ngx_http_limit_conn_handler_sense_1_ret, ngx_http_request_t *r)
{
  size_t n;
  uint32_t hash;
  ngx_str_t key;
  ngx_uint_t i;
  ngx_slab_pool_t *shpool;
  ngx_rbtree_node_t *node;
  ngx_pool_cleanup_t *cln;
  ngx_http_limit_conn_ctx_t *ctx;
  ngx_http_limit_conn_node_t *lc;
  ngx_http_limit_conn_conf_t *lccf;
  ngx_http_limit_conn_limit_t *limits;
  ngx_http_limit_conn_cleanup_t *lccln;
  if (r->main->limit_conn_set)
  {
    {
      *ngx_http_limit_conn_handler_sense_1_ret = 1;
      return -5;
    }
  }
  lccf = r->loc_conf[ngx_http_limit_conn_module.ctx_index];
  limits = lccf->limits.elts;
  for (i = 0; i < lccf->limits.nelts; i++)
  {
    ctx = limits[i].shm_zone->data;
    if (ngx_http_complex_value(r, &ctx->key, &key) != 0)
    {
      {
        *ngx_http_limit_conn_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (key.len == 0)
    {
      continue;
    }
    if (key.len > 255)
    {
      if (r->connection->log->log_level >= 4)
        ngx_log_error_core(4, r->connection->log, 0, "the value of the \"%V\" key is more than 255 bytes: \"%V\"", &ctx->key.value, &key);
      continue;
    }
    r->main->limit_conn_set = 1;
    hash = ngx_crc32_short(key.data, key.len);
    shpool = (ngx_slab_pool_t *) limits[i].shm_zone->shm.addr;
    ngx_shmtx_lock(&shpool->mutex);
    node = ngx_http_limit_conn_lookup(ctx->rbtree, &key, hash);
    if (node == 0)
    {
      n = (0 + 0) + key.len;
      node = ngx_slab_alloc_locked(shpool, n);
      if (node == 0)
      {
        ngx_shmtx_unlock(&shpool->mutex);
        ngx_http_limit_conn_cleanup_all(r->pool);
        {
          *ngx_http_limit_conn_handler_sense_1_ret = 1;
          return lccf->status_code;
        }
      }
      lc = (ngx_http_limit_conn_node_t *) (&node->color);
      node->key = hash;
      lc->len = (u_char) key.len;
      lc->conn = 1;
      (void) memcpy(lc->data, key.data, key.len);
      ngx_rbtree_insert(ctx->rbtree, node);
    }
    else
    {
      lc = (ngx_http_limit_conn_node_t *) (&node->color);
      if (((ngx_uint_t) lc->conn) >= limits[i].conn)
      {
        ngx_shmtx_unlock(&shpool->mutex);
        if (r->connection->log->log_level >= lccf->log_level)
          ngx_log_error_core(lccf->log_level, r->connection->log, 0, "limiting connections by zone \"%V\"", &limits[i].shm_zone->shm.name);
        ngx_http_limit_conn_cleanup_all(r->pool);
        {
          *ngx_http_limit_conn_handler_sense_1_ret = 1;
          return lccf->status_code;
        }
      }
      lc->conn++;
    }
    ;
    ngx_shmtx_unlock(&shpool->mutex);
    cln = ngx_pool_cleanup_add(r->pool, sizeof(ngx_http_limit_conn_cleanup_t));
    if (cln == 0)
    {
      {
        *ngx_http_limit_conn_handler_sense_1_ret = 1;
        return 500;
      }
    }
    cln->handler = ngx_http_limit_conn_cleanup;
    lccln = cln->data;
    lccln->shm_zone = limits[i].shm_zone;
    lccln->node = node;
  }

  {
    *ngx_http_limit_conn_handler_sense_1_ret = 1;
    return -5;
  }
}





static ngx_rbtree_node_t *
ngx_http_limit_conn_lookup(ngx_rbtree_t *rbtree, ngx_str_t *key, uint32_t hash)
{
    ngx_int_t                    rc;
    ngx_rbtree_node_t           *node, *sentinel;
    ngx_http_limit_conn_node_t  *lcn;

    node = rbtree->root;
    sentinel = rbtree->sentinel;

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

        lcn = (ngx_http_limit_conn_node_t *) &node->color;

        rc = ngx_memn2cmp(key->data, lcn->data, key->len, (size_t) lcn->len);

        if (rc == 0) {
            return node;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    return NULL;
}


static void
ngx_http_limit_conn_cleanup(void *data)
{
    ngx_http_limit_conn_cleanup_t  *lccln = data;

    ngx_slab_pool_t             *shpool;
    ngx_rbtree_node_t           *node;
    ngx_http_limit_conn_ctx_t   *ctx;
    ngx_http_limit_conn_node_t  *lc;

    ctx = lccln->shm_zone->data;
    shpool = (ngx_slab_pool_t *) lccln->shm_zone->shm.addr;
    node = lccln->node;
    lc = (ngx_http_limit_conn_node_t *) &node->color;

    ngx_shmtx_lock(&shpool->mutex);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, lccln->shm_zone->shm.log, 0,
                   "limit conn cleanup: %08Xi %d", node->key, lc->conn);

    lc->conn--;

    if (lc->conn == 0) {
        ngx_rbtree_delete(ctx->rbtree, node);
        ngx_slab_free_locked(shpool, node);
    }

    ngx_shmtx_unlock(&shpool->mutex);
}


static ngx_inline void
ngx_http_limit_conn_cleanup_all(ngx_pool_t *pool)
{
    ngx_pool_cleanup_t  *cln;

    cln = pool->cleanup;

    while (cln && cln->handler == ngx_http_limit_conn_cleanup) {
        ngx_http_limit_conn_cleanup(cln->data);
        cln = cln->next;
    }

    pool->cleanup = cln;
}




static void *
ngx_http_limit_conn_create_conf(ngx_conf_t *cf)
{
    ngx_http_limit_conn_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_limit_conn_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->limits.elts = NULL;
     */

    conf->log_level = NGX_CONF_UNSET_UINT;
    conf->status_code = NGX_CONF_UNSET_UINT;

    return conf;
}


static char *
ngx_http_limit_conn_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_limit_conn_conf_t *prev = parent;
    ngx_http_limit_conn_conf_t *conf = child;

    if (conf->limits.elts == NULL) {
        conf->limits = prev->limits;
    }

    ngx_conf_merge_uint_value(conf->log_level, prev->log_level, NGX_LOG_ERR);
    ngx_conf_merge_uint_value(conf->status_code, prev->status_code,
                              NGX_HTTP_SERVICE_UNAVAILABLE);

    return NGX_CONF_OK;
}






ngx_int_t ngx_http_limit_conn_init_sense_1(int *ngx_http_limit_conn_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_limit_conn_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_limit_conn_handler;
  {
    *ngx_http_limit_conn_init_sense_1_ret = 1;
    return 0;
  }
}

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    u_char                     color;
    u_char                     len;
    u_short                    conn;
    u_char                     data[1];
} ngx_http_limit_conn_node_t;


typedef struct {
    ngx_shm_zone_t            *shm_zone;
    ngx_rbtree_node_t         *node;
} ngx_http_limit_conn_cleanup_t;


typedef struct {
    ngx_rbtree_t              *rbtree;
    ngx_http_complex_value_t   key;
} ngx_http_limit_conn_ctx_t;


typedef struct {
    ngx_shm_zone_t            *shm_zone;
    ngx_uint_t                 conn;
} ngx_http_limit_conn_limit_t;


typedef struct {
    ngx_array_t                limits;
    ngx_uint_t                 log_level;
    ngx_uint_t                 status_code;
} ngx_http_limit_conn_conf_t;


static ngx_rbtree_node_t *ngx_http_limit_conn_lookup(ngx_rbtree_t *rbtree,
    ngx_str_t *key, uint32_t hash);
static void ngx_http_limit_conn_cleanup(void *data);
static ngx_inline void ngx_http_limit_conn_cleanup_all(ngx_pool_t *pool);

static void *ngx_http_limit_conn_create_conf(ngx_conf_t *cf);
static char *ngx_http_limit_conn_merge_conf(ngx_conf_t *cf, void *parent,
    void *child);
static char *ngx_http_limit_conn_zone(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_limit_conn(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static ngx_int_t ngx_http_limit_conn_init(ngx_conf_t *cf);


static ngx_conf_enum_t  ngx_http_limit_conn_log_levels[] = {
    { ngx_string("info"), NGX_LOG_INFO },
    { ngx_string("notice"), NGX_LOG_NOTICE },
    { ngx_string("warn"), NGX_LOG_WARN },
    { ngx_string("error"), NGX_LOG_ERR },
    { ngx_null_string, 0 }
};


    ngx_conf_check_num_bounds, 400, 599
};


static ngx_command_t  ngx_http_limit_conn_commands[] = {

    { ngx_string("limit_conn_zone"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE2,
      ngx_http_limit_conn_zone,
      0,
      0,
      NULL },

    { ngx_string("limit_conn"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE2,
      ngx_http_limit_conn,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("limit_conn_log_level"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_conn_conf_t, log_level),
      &ngx_http_limit_conn_log_levels },

    { ngx_string("limit_conn_status"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_conn_conf_t, status_code),
      &ngx_http_limit_conn_status_bounds },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    ngx_http_limit_conn_init,              /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_limit_conn_create_conf,       /* create location configuration */
    ngx_http_limit_conn_merge_conf         /* merge location configuration */
};


ngx_module_t  ngx_http_limit_conn_module = {
    NGX_MODULE_V1,
    &ngx_http_limit_conn_module_ctx,       /* module context */
    ngx_http_limit_conn_commands,          /* module directives */
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


ngx_int_t ngx_http_limit_conn_handler_sense_1(int *ngx_http_limit_conn_handler_sense_1_ret, ngx_http_request_t *r)
{
  size_t n;
  uint32_t hash;
  ngx_str_t key;
  ngx_uint_t i;
  ngx_slab_pool_t *shpool;
  ngx_rbtree_node_t *node;
  ngx_pool_cleanup_t *cln;
  ngx_http_limit_conn_ctx_t *ctx;
  ngx_http_limit_conn_node_t *lc;
  ngx_http_limit_conn_conf_t *lccf;
  ngx_http_limit_conn_limit_t *limits;
  ngx_http_limit_conn_cleanup_t *lccln;
  if (r->main->limit_conn_set)
  {
    {
      *ngx_http_limit_conn_handler_sense_1_ret = 1;
      return -5;
    }
  }
  lccf = r->loc_conf[ngx_http_limit_conn_module.ctx_index];
  limits = lccf->limits.elts;
  for (i = 0; i < lccf->limits.nelts; i++)
  {
    ctx = limits[i].shm_zone->data;
    if (ngx_http_complex_value(r, &ctx->key, &key) != 0)
    {
      {
        *ngx_http_limit_conn_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (key.len == 0)
    {
      continue;
    }
    if (key.len > 255)
    {
      if (r->connection->log->log_level >= 4)
        ngx_log_error_core(4, r->connection->log, 0, "the value of the \"%V\" key is more than 255 bytes: \"%V\"", &ctx->key.value, &key);
      continue;
    }
    r->main->limit_conn_set = 1;
    hash = ngx_crc32_short(key.data, key.len);
    shpool = (ngx_slab_pool_t *) limits[i].shm_zone->shm.addr;
    ngx_shmtx_lock(&shpool->mutex);
    node = ngx_http_limit_conn_lookup(ctx->rbtree, &key, hash);
    if (node == 0)
    {
      n = (0 + 0) + key.len;
      node = ngx_slab_alloc_locked(shpool, n);
      if (node == 0)
      {
        ngx_shmtx_unlock(&shpool->mutex);
        ngx_http_limit_conn_cleanup_all(r->pool);
        {
          *ngx_http_limit_conn_handler_sense_1_ret = 1;
          return lccf->status_code;
        }
      }
      lc = (ngx_http_limit_conn_node_t *) (&node->color);
      node->key = hash;
      lc->len = (u_char) key.len;
      lc->conn = 1;
      (void) memcpy(lc->data, key.data, key.len);
      ngx_rbtree_insert(ctx->rbtree, node);
    }
    else
    {
      lc = (ngx_http_limit_conn_node_t *) (&node->color);
      if (((ngx_uint_t) lc->conn) >= limits[i].conn)
      {
        ngx_shmtx_unlock(&shpool->mutex);
        if (r->connection->log->log_level >= lccf->log_level)
          ngx_log_error_core(lccf->log_level, r->connection->log, 0, "limiting connections by zone \"%V\"", &limits[i].shm_zone->shm.name);
        ngx_http_limit_conn_cleanup_all(r->pool);
        {
          *ngx_http_limit_conn_handler_sense_1_ret = 1;
          return lccf->status_code;
        }
      }
      lc->conn++;
    }
    ;
    ngx_shmtx_unlock(&shpool->mutex);
    cln = ngx_pool_cleanup_add(r->pool, sizeof(ngx_http_limit_conn_cleanup_t));
    if (cln == 0)
    {
      {
        *ngx_http_limit_conn_handler_sense_1_ret = 1;
        return 500;
      }
    }
    cln->handler = ngx_http_limit_conn_cleanup;
    lccln = cln->data;
    lccln->shm_zone = limits[i].shm_zone;
    lccln->node = node;
  }

  {
    *ngx_http_limit_conn_handler_sense_1_ret = 1;
    return -5;
  }
}





static ngx_rbtree_node_t *
ngx_http_limit_conn_lookup(ngx_rbtree_t *rbtree, ngx_str_t *key, uint32_t hash)
{
    ngx_int_t                    rc;
    ngx_rbtree_node_t           *node, *sentinel;
    ngx_http_limit_conn_node_t  *lcn;

    node = rbtree->root;
    sentinel = rbtree->sentinel;

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

        lcn = (ngx_http_limit_conn_node_t *) &node->color;

        rc = ngx_memn2cmp(key->data, lcn->data, key->len, (size_t) lcn->len);

        if (rc == 0) {
            return node;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    return NULL;
}


static void
ngx_http_limit_conn_cleanup(void *data)
{
    ngx_http_limit_conn_cleanup_t  *lccln = data;

    ngx_slab_pool_t             *shpool;
    ngx_rbtree_node_t           *node;
    ngx_http_limit_conn_ctx_t   *ctx;
    ngx_http_limit_conn_node_t  *lc;

    ctx = lccln->shm_zone->data;
    shpool = (ngx_slab_pool_t *) lccln->shm_zone->shm.addr;
    node = lccln->node;
    lc = (ngx_http_limit_conn_node_t *) &node->color;

    ngx_shmtx_lock(&shpool->mutex);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, lccln->shm_zone->shm.log, 0,
                   "limit conn cleanup: %08Xi %d", node->key, lc->conn);

    lc->conn--;

    if (lc->conn == 0) {
        ngx_rbtree_delete(ctx->rbtree, node);
        ngx_slab_free_locked(shpool, node);
    }

    ngx_shmtx_unlock(&shpool->mutex);
}


static ngx_inline void
ngx_http_limit_conn_cleanup_all(ngx_pool_t *pool)
{
    ngx_pool_cleanup_t  *cln;

    cln = pool->cleanup;

    while (cln && cln->handler == ngx_http_limit_conn_cleanup) {
        ngx_http_limit_conn_cleanup(cln->data);
        cln = cln->next;
    }

    pool->cleanup = cln;
}




static void *
ngx_http_limit_conn_create_conf(ngx_conf_t *cf)
{
    ngx_http_limit_conn_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_limit_conn_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->limits.elts = NULL;
     */

    conf->log_level = NGX_CONF_UNSET_UINT;
    conf->status_code = NGX_CONF_UNSET_UINT;

    return conf;
}


static char *
ngx_http_limit_conn_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_limit_conn_conf_t *prev = parent;
    ngx_http_limit_conn_conf_t *conf = child;

    if (conf->limits.elts == NULL) {
        conf->limits = prev->limits;
    }

    ngx_conf_merge_uint_value(conf->log_level, prev->log_level, NGX_LOG_ERR);
    ngx_conf_merge_uint_value(conf->status_code, prev->status_code,
                              NGX_HTTP_SERVICE_UNAVAILABLE);

    return NGX_CONF_OK;
}






ngx_int_t ngx_http_limit_conn_init_sense_1(int *ngx_http_limit_conn_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_limit_conn_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_limit_conn_handler;
  {
    *ngx_http_limit_conn_init_sense_1_ret = 1;
    return 0;
  }
}

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    u_char                     color;
    u_char                     len;
    u_short                    conn;
    u_char                     data[1];
} ngx_http_limit_conn_node_t;


typedef struct {
    ngx_shm_zone_t            *shm_zone;
    ngx_rbtree_node_t         *node;
} ngx_http_limit_conn_cleanup_t;


typedef struct {
    ngx_rbtree_t              *rbtree;
    ngx_http_complex_value_t   key;
} ngx_http_limit_conn_ctx_t;


typedef struct {
    ngx_shm_zone_t            *shm_zone;
    ngx_uint_t                 conn;
} ngx_http_limit_conn_limit_t;


typedef struct {
    ngx_array_t                limits;
    ngx_uint_t                 log_level;
    ngx_uint_t                 status_code;
} ngx_http_limit_conn_conf_t;


static ngx_rbtree_node_t *ngx_http_limit_conn_lookup(ngx_rbtree_t *rbtree,
    ngx_str_t *key, uint32_t hash);
static void ngx_http_limit_conn_cleanup(void *data);
static ngx_inline void ngx_http_limit_conn_cleanup_all(ngx_pool_t *pool);

static void *ngx_http_limit_conn_create_conf(ngx_conf_t *cf);
static char *ngx_http_limit_conn_merge_conf(ngx_conf_t *cf, void *parent,
    void *child);
static char *ngx_http_limit_conn_zone(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_limit_conn(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static ngx_int_t ngx_http_limit_conn_init(ngx_conf_t *cf);


static ngx_conf_enum_t  ngx_http_limit_conn_log_levels[] = {
    { ngx_string("info"), NGX_LOG_INFO },
    { ngx_string("notice"), NGX_LOG_NOTICE },
    { ngx_string("warn"), NGX_LOG_WARN },
    { ngx_string("error"), NGX_LOG_ERR },
    { ngx_null_string, 0 }
};


    ngx_conf_check_num_bounds, 400, 599
};


static ngx_command_t  ngx_http_limit_conn_commands[] = {

    { ngx_string("limit_conn_zone"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE2,
      ngx_http_limit_conn_zone,
      0,
      0,
      NULL },

    { ngx_string("limit_conn"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE2,
      ngx_http_limit_conn,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("limit_conn_log_level"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_conn_conf_t, log_level),
      &ngx_http_limit_conn_log_levels },

    { ngx_string("limit_conn_status"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_conn_conf_t, status_code),
      &ngx_http_limit_conn_status_bounds },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    ngx_http_limit_conn_init,              /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_limit_conn_create_conf,       /* create location configuration */
    ngx_http_limit_conn_merge_conf         /* merge location configuration */
};


ngx_module_t  ngx_http_limit_conn_module = {
    NGX_MODULE_V1,
    &ngx_http_limit_conn_module_ctx,       /* module context */
    ngx_http_limit_conn_commands,          /* module directives */
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


ngx_int_t ngx_http_limit_conn_handler_sense_1(int *ngx_http_limit_conn_handler_sense_1_ret, ngx_http_request_t *r)
{
  size_t n;
  uint32_t hash;
  ngx_str_t key;
  ngx_uint_t i;
  ngx_slab_pool_t *shpool;
  ngx_rbtree_node_t *node;
  ngx_pool_cleanup_t *cln;
  ngx_http_limit_conn_ctx_t *ctx;
  ngx_http_limit_conn_node_t *lc;
  ngx_http_limit_conn_conf_t *lccf;
  ngx_http_limit_conn_limit_t *limits;
  ngx_http_limit_conn_cleanup_t *lccln;
  if (r->main->limit_conn_set)
  {
    {
      *ngx_http_limit_conn_handler_sense_1_ret = 1;
      return -5;
    }
  }
  lccf = r->loc_conf[ngx_http_limit_conn_module.ctx_index];
  limits = lccf->limits.elts;
  for (i = 0; i < lccf->limits.nelts; i++)
  {
    ctx = limits[i].shm_zone->data;
    if (ngx_http_complex_value(r, &ctx->key, &key) != 0)
    {
      {
        *ngx_http_limit_conn_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (key.len == 0)
    {
      continue;
    }
    if (key.len > 255)
    {
      if (r->connection->log->log_level >= 4)
        ngx_log_error_core(4, r->connection->log, 0, "the value of the \"%V\" key is more than 255 bytes: \"%V\"", &ctx->key.value, &key);
      continue;
    }
    r->main->limit_conn_set = 1;
    hash = ngx_crc32_short(key.data, key.len);
    shpool = (ngx_slab_pool_t *) limits[i].shm_zone->shm.addr;
    ngx_shmtx_lock(&shpool->mutex);
    node = ngx_http_limit_conn_lookup(ctx->rbtree, &key, hash);
    if (node == 0)
    {
      n = (0 + 0) + key.len;
      node = ngx_slab_alloc_locked(shpool, n);
      if (node == 0)
      {
        ngx_shmtx_unlock(&shpool->mutex);
        ngx_http_limit_conn_cleanup_all(r->pool);
        {
          *ngx_http_limit_conn_handler_sense_1_ret = 1;
          return lccf->status_code;
        }
      }
      lc = (ngx_http_limit_conn_node_t *) (&node->color);
      node->key = hash;
      lc->len = (u_char) key.len;
      lc->conn = 1;
      (void) memcpy(lc->data, key.data, key.len);
      ngx_rbtree_insert(ctx->rbtree, node);
    }
    else
    {
      lc = (ngx_http_limit_conn_node_t *) (&node->color);
      if (((ngx_uint_t) lc->conn) >= limits[i].conn)
      {
        ngx_shmtx_unlock(&shpool->mutex);
        if (r->connection->log->log_level >= lccf->log_level)
          ngx_log_error_core(lccf->log_level, r->connection->log, 0, "limiting connections by zone \"%V\"", &limits[i].shm_zone->shm.name);
        ngx_http_limit_conn_cleanup_all(r->pool);
        {
          *ngx_http_limit_conn_handler_sense_1_ret = 1;
          return lccf->status_code;
        }
      }
      lc->conn++;
    }
    ;
    ngx_shmtx_unlock(&shpool->mutex);
    cln = ngx_pool_cleanup_add(r->pool, sizeof(ngx_http_limit_conn_cleanup_t));
    if (cln == 0)
    {
      {
        *ngx_http_limit_conn_handler_sense_1_ret = 1;
        return 500;
      }
    }
    cln->handler = ngx_http_limit_conn_cleanup;
    lccln = cln->data;
    lccln->shm_zone = limits[i].shm_zone;
    lccln->node = node;
  }

  {
    *ngx_http_limit_conn_handler_sense_1_ret = 1;
    return -5;
  }
}





static ngx_rbtree_node_t *
ngx_http_limit_conn_lookup(ngx_rbtree_t *rbtree, ngx_str_t *key, uint32_t hash)
{
    ngx_int_t                    rc;
    ngx_rbtree_node_t           *node, *sentinel;
    ngx_http_limit_conn_node_t  *lcn;

    node = rbtree->root;
    sentinel = rbtree->sentinel;

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

        lcn = (ngx_http_limit_conn_node_t *) &node->color;

        rc = ngx_memn2cmp(key->data, lcn->data, key->len, (size_t) lcn->len);

        if (rc == 0) {
            return node;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    return NULL;
}


static void
ngx_http_limit_conn_cleanup(void *data)
{
    ngx_http_limit_conn_cleanup_t  *lccln = data;

    ngx_slab_pool_t             *shpool;
    ngx_rbtree_node_t           *node;
    ngx_http_limit_conn_ctx_t   *ctx;
    ngx_http_limit_conn_node_t  *lc;

    ctx = lccln->shm_zone->data;
    shpool = (ngx_slab_pool_t *) lccln->shm_zone->shm.addr;
    node = lccln->node;
    lc = (ngx_http_limit_conn_node_t *) &node->color;

    ngx_shmtx_lock(&shpool->mutex);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, lccln->shm_zone->shm.log, 0,
                   "limit conn cleanup: %08Xi %d", node->key, lc->conn);

    lc->conn--;

    if (lc->conn == 0) {
        ngx_rbtree_delete(ctx->rbtree, node);
        ngx_slab_free_locked(shpool, node);
    }

    ngx_shmtx_unlock(&shpool->mutex);
}


static ngx_inline void
ngx_http_limit_conn_cleanup_all(ngx_pool_t *pool)
{
    ngx_pool_cleanup_t  *cln;

    cln = pool->cleanup;

    while (cln && cln->handler == ngx_http_limit_conn_cleanup) {
        ngx_http_limit_conn_cleanup(cln->data);
        cln = cln->next;
    }

    pool->cleanup = cln;
}




static void *
ngx_http_limit_conn_create_conf(ngx_conf_t *cf)
{
    ngx_http_limit_conn_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_limit_conn_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->limits.elts = NULL;
     */

    conf->log_level = NGX_CONF_UNSET_UINT;
    conf->status_code = NGX_CONF_UNSET_UINT;

    return conf;
}


static char *
ngx_http_limit_conn_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_limit_conn_conf_t *prev = parent;
    ngx_http_limit_conn_conf_t *conf = child;

    if (conf->limits.elts == NULL) {
        conf->limits = prev->limits;
    }

    ngx_conf_merge_uint_value(conf->log_level, prev->log_level, NGX_LOG_ERR);
    ngx_conf_merge_uint_value(conf->status_code, prev->status_code,
                              NGX_HTTP_SERVICE_UNAVAILABLE);

    return NGX_CONF_OK;
}






ngx_int_t ngx_http_limit_conn_init_sense_1(int *ngx_http_limit_conn_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_limit_conn_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_limit_conn_handler;
  {
    *ngx_http_limit_conn_init_sense_1_ret = 1;
    return 0;
  }
}

