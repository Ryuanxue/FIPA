
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    u_char                       color;
    u_char                       dummy;
    u_short                      len;
    ngx_queue_t                  queue;
    ngx_msec_t                   last;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   excess;
    ngx_uint_t                   count;
    u_char                       data[1];
} ngx_http_limit_req_node_t;


typedef struct {
    ngx_rbtree_t                  rbtree;
    ngx_rbtree_node_t             sentinel;
    ngx_queue_t                   queue;
} ngx_http_limit_req_shctx_t;


typedef struct {
    ngx_http_limit_req_shctx_t  *sh;
    ngx_slab_pool_t             *shpool;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   rate;
    ngx_http_complex_value_t     key;
    ngx_http_limit_req_node_t   *node;
} ngx_http_limit_req_ctx_t;


typedef struct {
    ngx_shm_zone_t              *shm_zone;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   burst;
    ngx_uint_t                   nodelay; /* unsigned  nodelay:1 */
} ngx_http_limit_req_limit_t;


typedef struct {
    ngx_array_t                  limits;
    ngx_uint_t                   limit_log_level;
    ngx_uint_t                   delay_log_level;
    ngx_uint_t                   status_code;
} ngx_http_limit_req_conf_t;


static void ngx_http_limit_req_delay(ngx_http_request_t *r);
static ngx_int_t ngx_http_limit_req_lookup(ngx_http_limit_req_limit_t *limit,
    ngx_uint_t hash, ngx_str_t *key, ngx_uint_t *ep, ngx_uint_t account);
static ngx_msec_t ngx_http_limit_req_account(ngx_http_limit_req_limit_t *limits,
    ngx_uint_t n, ngx_uint_t *ep, ngx_http_limit_req_limit_t **limit);
static void ngx_http_limit_req_expire(ngx_http_limit_req_ctx_t *ctx,
    ngx_uint_t n);

static void *ngx_http_limit_req_create_conf(ngx_conf_t *cf);
static char *ngx_http_limit_req_merge_conf(ngx_conf_t *cf, void *parent,
    void *child);
static char *ngx_http_limit_req_zone(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_limit_req(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static ngx_int_t ngx_http_limit_req_init(ngx_conf_t *cf);


static ngx_conf_enum_t  ngx_http_limit_req_log_levels[] = {
    { ngx_string("info"), NGX_LOG_INFO },
    { ngx_string("notice"), NGX_LOG_NOTICE },
    { ngx_string("warn"), NGX_LOG_WARN },
    { ngx_string("error"), NGX_LOG_ERR },
    { ngx_null_string, 0 }
};


    ngx_conf_check_num_bounds, 400, 599
};


static ngx_command_t  ngx_http_limit_req_commands[] = {

    { ngx_string("limit_req_zone"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE3,
      ngx_http_limit_req_zone,
      0,
      0,
      NULL },

    { ngx_string("limit_req"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE123,
      ngx_http_limit_req,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("limit_req_log_level"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_req_conf_t, limit_log_level),
      &ngx_http_limit_req_log_levels },

    { ngx_string("limit_req_status"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_req_conf_t, status_code),
      &ngx_http_limit_req_status_bounds },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    ngx_http_limit_req_init,               /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_limit_req_create_conf,        /* create location configuration */
    ngx_http_limit_req_merge_conf          /* merge location configuration */
};


ngx_module_t  ngx_http_limit_req_module = {
    NGX_MODULE_V1,
    &ngx_http_limit_req_module_ctx,        /* module context */
    ngx_http_limit_req_commands,           /* module directives */
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


ngx_int_t ngx_http_limit_req_handler_sense_1(int *ngx_http_limit_req_handler_sense_1_ret, uint32_t *hash, ngx_str_t key, ngx_int_t *rc, ngx_uint_t *n, ngx_uint_t *excess, ngx_msec_t *delay, ngx_http_limit_req_ctx_t *ctx, ngx_http_limit_req_conf_t *lrcf, ngx_http_limit_req_limit_t *limit, ngx_http_limit_req_limit_t *limits, ngx_http_request_t *r)
{
  if (r->main->limit_req_set)
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  lrcf = r->loc_conf[ngx_http_limit_req_module.ctx_index];
  limits = lrcf->limits.elts;
  *excess = 0;
  *rc = -5;
  for (*n = 0; (*n) < lrcf->limits.nelts; (*n)++)
  {
    limit = &limits[*n];
    ctx = limit->shm_zone->data;
    if (ngx_http_complex_value(r, &ctx->key, &key) != 0)
    {
      {
        *ngx_http_limit_req_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (key.len == 0)
    {
      continue;
    }
    if (key.len > 65535)
    {
      if (r->connection->log->log_level >= 4)
        ngx_log_error_core(4, r->connection->log, 0, "the value of the \"%V\" key is more than 65535 bytes: \"%V\"", &ctx->key.value, &key);
      continue;
    }
    *hash = ngx_crc32_short(key.data, key.len);
    ngx_shmtx_lock(&ctx->shpool->mutex);
    *rc = ngx_http_limit_req_lookup(limit, *hash, &key, &(*excess), (*n) == (lrcf->limits.nelts - 1));
    ngx_shmtx_unlock(&ctx->shpool->mutex);
    ;
    if ((*rc) != (-2))
    {
      break;
    }
  }

  if ((*rc) == (-5))
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  r->main->limit_req_set = 1;
  if (((*rc) == (-3)) || ((*rc) == (-1)))
  {
    if ((*rc) == (-3))
    {
      if (r->connection->log->log_level >= lrcf->limit_log_level)
        ngx_log_error_core(lrcf->limit_log_level, r->connection->log, 0, "limiting requests, excess: %ui.%03ui by zone \"%V\"", (*excess) / 1000, (*excess) % 1000, &limit->shm_zone->shm.name);
    }
    while ((*n)--)
    {
      ctx = limits[*n].shm_zone->data;
      if (ctx->node == 0)
      {
        continue;
      }
      ngx_shmtx_lock(&ctx->shpool->mutex);
      ctx->node->count--;
      ngx_shmtx_unlock(&ctx->shpool->mutex);
      ctx->node = 0;
    }

    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return lrcf->status_code;
    }
  }
  if ((*rc) == (-2))
  {
    *excess = 0;
  }
  *delay = ngx_http_limit_req_account(limits, *n, &(*excess), &limit);
  if (!(*delay))
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  if (r->connection->log->log_level >= lrcf->delay_log_level)
    ngx_log_error_core(lrcf->delay_log_level, r->connection->log, 0, "delaying request, excess: %ui.%03ui, by zone \"%V\"", (*excess) / 1000, (*excess) % 1000, &limit->shm_zone->shm.name);
  if (ngx_handle_read_event(r->connection->read, 0) != 0)
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return 500;
    }
  }
  r->read_event_handler = ngx_http_test_reading;
  r->write_event_handler = ngx_http_limit_req_delay;
  r->connection->write->delayed = 1;
  ngx_event_add_timer(r->connection->write, *delay);
  {
    *ngx_http_limit_req_handler_sense_1_ret = 1;
    return -2;
  }
}







static ngx_int_t
ngx_http_limit_req_lookup(ngx_http_limit_req_limit_t *limit, ngx_uint_t hash,
    ngx_str_t *key, ngx_uint_t *ep, ngx_uint_t account)
{
    size_t                      size;
    ngx_int_t                   rc, excess;
    ngx_msec_t                  now;
    ngx_msec_int_t              ms;
    ngx_rbtree_node_t          *node, *sentinel;
    ngx_http_limit_req_ctx_t   *ctx;
    ngx_http_limit_req_node_t  *lr;

    now = ngx_current_msec;

    ctx = limit->shm_zone->data;

    node = ctx->sh->rbtree.root;
    sentinel = ctx->sh->rbtree.sentinel;

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

        lr = (ngx_http_limit_req_node_t *) &node->color;

        rc = ngx_memn2cmp(key->data, lr->data, key->len, (size_t) lr->len);

        if (rc == 0) {
            ngx_queue_remove(&lr->queue);
            ngx_queue_insert_head(&ctx->sh->queue, &lr->queue);

            ms = (ngx_msec_int_t) (now - lr->last);

            if (ms < -60000) {
                ms = 1;

            } else if (ms < 0) {
                ms = 0;
            }

            excess = lr->excess - ctx->rate * ms / 1000 + 1000;

            if (excess < 0) {
                excess = 0;
            }

            *ep = excess;

            if ((ngx_uint_t) excess > limit->burst) {
                return NGX_BUSY;
            }

            if (account) {
                lr->excess = excess;

                if (ms) {
                    lr->last = now;
                }

                return NGX_OK;
            }

            lr->count++;

            ctx->node = lr;

            return NGX_AGAIN;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    *ep = 0;

    size = offsetof(ngx_rbtree_node_t, color)
           + offsetof(ngx_http_limit_req_node_t, data)
           + key->len;

    ngx_http_limit_req_expire(ctx, 1);

    node = ngx_slab_alloc_locked(ctx->shpool, size);

    if (node == NULL) {
        ngx_http_limit_req_expire(ctx, 0);

        node = ngx_slab_alloc_locked(ctx->shpool, size);
        if (node == NULL) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                          "could not allocate node%s", ctx->shpool->log_ctx);
            return NGX_ERROR;
        }
    }

    node->key = hash;

    lr = (ngx_http_limit_req_node_t *) &node->color;

    lr->len = (u_short) key->len;
    lr->excess = 0;

    ngx_memcpy(lr->data, key->data, key->len);

    ngx_rbtree_insert(&ctx->sh->rbtree, node);

    ngx_queue_insert_head(&ctx->sh->queue, &lr->queue);

    if (account) {
        lr->last = now;
        lr->count = 0;
        return NGX_OK;
    }

    lr->last = 0;
    lr->count = 1;

    ctx->node = lr;

    return NGX_AGAIN;
}


static ngx_msec_t
ngx_http_limit_req_account(ngx_http_limit_req_limit_t *limits, ngx_uint_t n,
    ngx_uint_t *ep, ngx_http_limit_req_limit_t **limit)
{
    ngx_int_t                   excess;
    ngx_msec_t                  now, delay, max_delay;
    ngx_msec_int_t              ms;
    ngx_http_limit_req_ctx_t   *ctx;
    ngx_http_limit_req_node_t  *lr;

    excess = *ep;

    if (excess == 0 || (*limit)->nodelay) {
        max_delay = 0;

    } else {
        ctx = (*limit)->shm_zone->data;
        max_delay = excess * 1000 / ctx->rate;
    }

    while (n--) {
        ctx = limits[n].shm_zone->data;
        lr = ctx->node;

        if (lr == NULL) {
            continue;
        }

        ngx_shmtx_lock(&ctx->shpool->mutex);

        now = ngx_current_msec;
        ms = (ngx_msec_int_t) (now - lr->last);

        if (ms < -60000) {
            ms = 1;

        } else if (ms < 0) {
            ms = 0;
        }

        excess = lr->excess - ctx->rate * ms / 1000 + 1000;

        if (excess < 0) {
            excess = 0;
        }

        if (ms) {
            lr->last = now;
        }

        lr->excess = excess;
        lr->count--;

        ngx_shmtx_unlock(&ctx->shpool->mutex);

        ctx->node = NULL;

        if (limits[n].nodelay) {
            continue;
        }

        delay = excess * 1000 / ctx->rate;

        if (delay > max_delay) {
            max_delay = delay;
            *ep = excess;
            *limit = &limits[n];
        }
    }

    return max_delay;
}


static void
ngx_http_limit_req_expire(ngx_http_limit_req_ctx_t *ctx, ngx_uint_t n)
{
    ngx_int_t                   excess;
    ngx_msec_t                  now;
    ngx_queue_t                *q;
    ngx_msec_int_t              ms;
    ngx_rbtree_node_t          *node;
    ngx_http_limit_req_node_t  *lr;

    now = ngx_current_msec;

    /*
     * n == 1 deletes one or two zero rate entries
     * n == 0 deletes oldest entry by force
     *        and one or two zero rate entries
     */

    while (n < 3) {

        if (ngx_queue_empty(&ctx->sh->queue)) {
            return;
        }

        q = ngx_queue_last(&ctx->sh->queue);

        lr = ngx_queue_data(q, ngx_http_limit_req_node_t, queue);

        if (lr->count) {

            /*
             * There is not much sense in looking further,
             * because we bump nodes on the lookup stage.
             */

            return;
        }

        if (n++ != 0) {

            ms = (ngx_msec_int_t) (now - lr->last);
            ms = ngx_abs(ms);

            if (ms < 60000) {
                return;
            }

            excess = lr->excess - ctx->rate * ms / 1000;

            if (excess > 0) {
                return;
            }
        }

        ngx_queue_remove(q);

        node = (ngx_rbtree_node_t *)
                   ((u_char *) lr - offsetof(ngx_rbtree_node_t, color));

        ngx_rbtree_delete(&ctx->sh->rbtree, node);

        ngx_slab_free_locked(ctx->shpool, node);
    }
}




static void *
ngx_http_limit_req_create_conf(ngx_conf_t *cf)
{
    ngx_http_limit_req_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_limit_req_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->limits.elts = NULL;
     */

    conf->limit_log_level = NGX_CONF_UNSET_UINT;
    conf->status_code = NGX_CONF_UNSET_UINT;

    return conf;
}


static char *
ngx_http_limit_req_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_limit_req_conf_t *prev = parent;
    ngx_http_limit_req_conf_t *conf = child;

    if (conf->limits.elts == NULL) {
        conf->limits = prev->limits;
    }

    ngx_conf_merge_uint_value(conf->limit_log_level, prev->limit_log_level,
                              NGX_LOG_ERR);

    conf->delay_log_level = (conf->limit_log_level == NGX_LOG_INFO) ?
                                NGX_LOG_INFO : conf->limit_log_level + 1;

    ngx_conf_merge_uint_value(conf->status_code, prev->status_code,
                              NGX_HTTP_SERVICE_UNAVAILABLE);

    return NGX_CONF_OK;
}






ngx_int_t ngx_http_limit_req_init_sense_1(int *ngx_http_limit_req_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_limit_req_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_limit_req_handler;
  {
    *ngx_http_limit_req_init_sense_1_ret = 1;
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
    u_char                       color;
    u_char                       dummy;
    u_short                      len;
    ngx_queue_t                  queue;
    ngx_msec_t                   last;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   excess;
    ngx_uint_t                   count;
    u_char                       data[1];
} ngx_http_limit_req_node_t;


typedef struct {
    ngx_rbtree_t                  rbtree;
    ngx_rbtree_node_t             sentinel;
    ngx_queue_t                   queue;
} ngx_http_limit_req_shctx_t;


typedef struct {
    ngx_http_limit_req_shctx_t  *sh;
    ngx_slab_pool_t             *shpool;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   rate;
    ngx_http_complex_value_t     key;
    ngx_http_limit_req_node_t   *node;
} ngx_http_limit_req_ctx_t;


typedef struct {
    ngx_shm_zone_t              *shm_zone;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   burst;
    ngx_uint_t                   nodelay; /* unsigned  nodelay:1 */
} ngx_http_limit_req_limit_t;


typedef struct {
    ngx_array_t                  limits;
    ngx_uint_t                   limit_log_level;
    ngx_uint_t                   delay_log_level;
    ngx_uint_t                   status_code;
} ngx_http_limit_req_conf_t;


static void ngx_http_limit_req_delay(ngx_http_request_t *r);
static ngx_int_t ngx_http_limit_req_lookup(ngx_http_limit_req_limit_t *limit,
    ngx_uint_t hash, ngx_str_t *key, ngx_uint_t *ep, ngx_uint_t account);
static ngx_msec_t ngx_http_limit_req_account(ngx_http_limit_req_limit_t *limits,
    ngx_uint_t n, ngx_uint_t *ep, ngx_http_limit_req_limit_t **limit);
static void ngx_http_limit_req_expire(ngx_http_limit_req_ctx_t *ctx,
    ngx_uint_t n);

static void *ngx_http_limit_req_create_conf(ngx_conf_t *cf);
static char *ngx_http_limit_req_merge_conf(ngx_conf_t *cf, void *parent,
    void *child);
static char *ngx_http_limit_req_zone(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_limit_req(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static ngx_int_t ngx_http_limit_req_init(ngx_conf_t *cf);


static ngx_conf_enum_t  ngx_http_limit_req_log_levels[] = {
    { ngx_string("info"), NGX_LOG_INFO },
    { ngx_string("notice"), NGX_LOG_NOTICE },
    { ngx_string("warn"), NGX_LOG_WARN },
    { ngx_string("error"), NGX_LOG_ERR },
    { ngx_null_string, 0 }
};


    ngx_conf_check_num_bounds, 400, 599
};


static ngx_command_t  ngx_http_limit_req_commands[] = {

    { ngx_string("limit_req_zone"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE3,
      ngx_http_limit_req_zone,
      0,
      0,
      NULL },

    { ngx_string("limit_req"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE123,
      ngx_http_limit_req,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("limit_req_log_level"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_req_conf_t, limit_log_level),
      &ngx_http_limit_req_log_levels },

    { ngx_string("limit_req_status"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_req_conf_t, status_code),
      &ngx_http_limit_req_status_bounds },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    ngx_http_limit_req_init,               /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_limit_req_create_conf,        /* create location configuration */
    ngx_http_limit_req_merge_conf          /* merge location configuration */
};


ngx_module_t  ngx_http_limit_req_module = {
    NGX_MODULE_V1,
    &ngx_http_limit_req_module_ctx,        /* module context */
    ngx_http_limit_req_commands,           /* module directives */
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


ngx_int_t ngx_http_limit_req_handler_sense_1(int *ngx_http_limit_req_handler_sense_1_ret, uint32_t *hash, ngx_str_t key, ngx_int_t *rc, ngx_uint_t *n, ngx_uint_t *excess, ngx_msec_t *delay, ngx_http_limit_req_ctx_t *ctx, ngx_http_limit_req_conf_t *lrcf, ngx_http_limit_req_limit_t *limit, ngx_http_limit_req_limit_t *limits, ngx_http_request_t *r)
{
  if (r->main->limit_req_set)
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  lrcf = r->loc_conf[ngx_http_limit_req_module.ctx_index];
  limits = lrcf->limits.elts;
  *excess = 0;
  *rc = -5;
  for (*n = 0; (*n) < lrcf->limits.nelts; (*n)++)
  {
    limit = &limits[*n];
    ctx = limit->shm_zone->data;
    if (ngx_http_complex_value(r, &ctx->key, &key) != 0)
    {
      {
        *ngx_http_limit_req_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (key.len == 0)
    {
      continue;
    }
    if (key.len > 65535)
    {
      if (r->connection->log->log_level >= 4)
        ngx_log_error_core(4, r->connection->log, 0, "the value of the \"%V\" key is more than 65535 bytes: \"%V\"", &ctx->key.value, &key);
      continue;
    }
    *hash = ngx_crc32_short(key.data, key.len);
    ngx_shmtx_lock(&ctx->shpool->mutex);
    *rc = ngx_http_limit_req_lookup(limit, *hash, &key, &(*excess), (*n) == (lrcf->limits.nelts - 1));
    ngx_shmtx_unlock(&ctx->shpool->mutex);
    ;
    if ((*rc) != (-2))
    {
      break;
    }
  }

  if ((*rc) == (-5))
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  r->main->limit_req_set = 1;
  if (((*rc) == (-3)) || ((*rc) == (-1)))
  {
    if ((*rc) == (-3))
    {
      if (r->connection->log->log_level >= lrcf->limit_log_level)
        ngx_log_error_core(lrcf->limit_log_level, r->connection->log, 0, "limiting requests, excess: %ui.%03ui by zone \"%V\"", (*excess) / 1000, (*excess) % 1000, &limit->shm_zone->shm.name);
    }
    while ((*n)--)
    {
      ctx = limits[*n].shm_zone->data;
      if (ctx->node == 0)
      {
        continue;
      }
      ngx_shmtx_lock(&ctx->shpool->mutex);
      ctx->node->count--;
      ngx_shmtx_unlock(&ctx->shpool->mutex);
      ctx->node = 0;
    }

    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return lrcf->status_code;
    }
  }
  if ((*rc) == (-2))
  {
    *excess = 0;
  }
  *delay = ngx_http_limit_req_account(limits, *n, &(*excess), &limit);
  if (!(*delay))
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  if (r->connection->log->log_level >= lrcf->delay_log_level)
    ngx_log_error_core(lrcf->delay_log_level, r->connection->log, 0, "delaying request, excess: %ui.%03ui, by zone \"%V\"", (*excess) / 1000, (*excess) % 1000, &limit->shm_zone->shm.name);
  if (ngx_handle_read_event(r->connection->read, 0) != 0)
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return 500;
    }
  }
  r->read_event_handler = ngx_http_test_reading;
  r->write_event_handler = ngx_http_limit_req_delay;
  r->connection->write->delayed = 1;
  ngx_event_add_timer(r->connection->write, *delay);
  {
    *ngx_http_limit_req_handler_sense_1_ret = 1;
    return -2;
  }
}







static ngx_int_t
ngx_http_limit_req_lookup(ngx_http_limit_req_limit_t *limit, ngx_uint_t hash,
    ngx_str_t *key, ngx_uint_t *ep, ngx_uint_t account)
{
    size_t                      size;
    ngx_int_t                   rc, excess;
    ngx_msec_t                  now;
    ngx_msec_int_t              ms;
    ngx_rbtree_node_t          *node, *sentinel;
    ngx_http_limit_req_ctx_t   *ctx;
    ngx_http_limit_req_node_t  *lr;

    now = ngx_current_msec;

    ctx = limit->shm_zone->data;

    node = ctx->sh->rbtree.root;
    sentinel = ctx->sh->rbtree.sentinel;

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

        lr = (ngx_http_limit_req_node_t *) &node->color;

        rc = ngx_memn2cmp(key->data, lr->data, key->len, (size_t) lr->len);

        if (rc == 0) {
            ngx_queue_remove(&lr->queue);
            ngx_queue_insert_head(&ctx->sh->queue, &lr->queue);

            ms = (ngx_msec_int_t) (now - lr->last);

            if (ms < -60000) {
                ms = 1;

            } else if (ms < 0) {
                ms = 0;
            }

            excess = lr->excess - ctx->rate * ms / 1000 + 1000;

            if (excess < 0) {
                excess = 0;
            }

            *ep = excess;

            if ((ngx_uint_t) excess > limit->burst) {
                return NGX_BUSY;
            }

            if (account) {
                lr->excess = excess;

                if (ms) {
                    lr->last = now;
                }

                return NGX_OK;
            }

            lr->count++;

            ctx->node = lr;

            return NGX_AGAIN;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    *ep = 0;

    size = offsetof(ngx_rbtree_node_t, color)
           + offsetof(ngx_http_limit_req_node_t, data)
           + key->len;

    ngx_http_limit_req_expire(ctx, 1);

    node = ngx_slab_alloc_locked(ctx->shpool, size);

    if (node == NULL) {
        ngx_http_limit_req_expire(ctx, 0);

        node = ngx_slab_alloc_locked(ctx->shpool, size);
        if (node == NULL) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                          "could not allocate node%s", ctx->shpool->log_ctx);
            return NGX_ERROR;
        }
    }

    node->key = hash;

    lr = (ngx_http_limit_req_node_t *) &node->color;

    lr->len = (u_short) key->len;
    lr->excess = 0;

    ngx_memcpy(lr->data, key->data, key->len);

    ngx_rbtree_insert(&ctx->sh->rbtree, node);

    ngx_queue_insert_head(&ctx->sh->queue, &lr->queue);

    if (account) {
        lr->last = now;
        lr->count = 0;
        return NGX_OK;
    }

    lr->last = 0;
    lr->count = 1;

    ctx->node = lr;

    return NGX_AGAIN;
}


static ngx_msec_t
ngx_http_limit_req_account(ngx_http_limit_req_limit_t *limits, ngx_uint_t n,
    ngx_uint_t *ep, ngx_http_limit_req_limit_t **limit)
{
    ngx_int_t                   excess;
    ngx_msec_t                  now, delay, max_delay;
    ngx_msec_int_t              ms;
    ngx_http_limit_req_ctx_t   *ctx;
    ngx_http_limit_req_node_t  *lr;

    excess = *ep;

    if (excess == 0 || (*limit)->nodelay) {
        max_delay = 0;

    } else {
        ctx = (*limit)->shm_zone->data;
        max_delay = excess * 1000 / ctx->rate;
    }

    while (n--) {
        ctx = limits[n].shm_zone->data;
        lr = ctx->node;

        if (lr == NULL) {
            continue;
        }

        ngx_shmtx_lock(&ctx->shpool->mutex);

        now = ngx_current_msec;
        ms = (ngx_msec_int_t) (now - lr->last);

        if (ms < -60000) {
            ms = 1;

        } else if (ms < 0) {
            ms = 0;
        }

        excess = lr->excess - ctx->rate * ms / 1000 + 1000;

        if (excess < 0) {
            excess = 0;
        }

        if (ms) {
            lr->last = now;
        }

        lr->excess = excess;
        lr->count--;

        ngx_shmtx_unlock(&ctx->shpool->mutex);

        ctx->node = NULL;

        if (limits[n].nodelay) {
            continue;
        }

        delay = excess * 1000 / ctx->rate;

        if (delay > max_delay) {
            max_delay = delay;
            *ep = excess;
            *limit = &limits[n];
        }
    }

    return max_delay;
}


static void
ngx_http_limit_req_expire(ngx_http_limit_req_ctx_t *ctx, ngx_uint_t n)
{
    ngx_int_t                   excess;
    ngx_msec_t                  now;
    ngx_queue_t                *q;
    ngx_msec_int_t              ms;
    ngx_rbtree_node_t          *node;
    ngx_http_limit_req_node_t  *lr;

    now = ngx_current_msec;

    /*
     * n == 1 deletes one or two zero rate entries
     * n == 0 deletes oldest entry by force
     *        and one or two zero rate entries
     */

    while (n < 3) {

        if (ngx_queue_empty(&ctx->sh->queue)) {
            return;
        }

        q = ngx_queue_last(&ctx->sh->queue);

        lr = ngx_queue_data(q, ngx_http_limit_req_node_t, queue);

        if (lr->count) {

            /*
             * There is not much sense in looking further,
             * because we bump nodes on the lookup stage.
             */

            return;
        }

        if (n++ != 0) {

            ms = (ngx_msec_int_t) (now - lr->last);
            ms = ngx_abs(ms);

            if (ms < 60000) {
                return;
            }

            excess = lr->excess - ctx->rate * ms / 1000;

            if (excess > 0) {
                return;
            }
        }

        ngx_queue_remove(q);

        node = (ngx_rbtree_node_t *)
                   ((u_char *) lr - offsetof(ngx_rbtree_node_t, color));

        ngx_rbtree_delete(&ctx->sh->rbtree, node);

        ngx_slab_free_locked(ctx->shpool, node);
    }
}




static void *
ngx_http_limit_req_create_conf(ngx_conf_t *cf)
{
    ngx_http_limit_req_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_limit_req_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->limits.elts = NULL;
     */

    conf->limit_log_level = NGX_CONF_UNSET_UINT;
    conf->status_code = NGX_CONF_UNSET_UINT;

    return conf;
}


static char *
ngx_http_limit_req_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_limit_req_conf_t *prev = parent;
    ngx_http_limit_req_conf_t *conf = child;

    if (conf->limits.elts == NULL) {
        conf->limits = prev->limits;
    }

    ngx_conf_merge_uint_value(conf->limit_log_level, prev->limit_log_level,
                              NGX_LOG_ERR);

    conf->delay_log_level = (conf->limit_log_level == NGX_LOG_INFO) ?
                                NGX_LOG_INFO : conf->limit_log_level + 1;

    ngx_conf_merge_uint_value(conf->status_code, prev->status_code,
                              NGX_HTTP_SERVICE_UNAVAILABLE);

    return NGX_CONF_OK;
}






ngx_int_t ngx_http_limit_req_init_sense_1(int *ngx_http_limit_req_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_limit_req_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_limit_req_handler;
  {
    *ngx_http_limit_req_init_sense_1_ret = 1;
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
    u_char                       color;
    u_char                       dummy;
    u_short                      len;
    ngx_queue_t                  queue;
    ngx_msec_t                   last;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   excess;
    ngx_uint_t                   count;
    u_char                       data[1];
} ngx_http_limit_req_node_t;


typedef struct {
    ngx_rbtree_t                  rbtree;
    ngx_rbtree_node_t             sentinel;
    ngx_queue_t                   queue;
} ngx_http_limit_req_shctx_t;


typedef struct {
    ngx_http_limit_req_shctx_t  *sh;
    ngx_slab_pool_t             *shpool;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   rate;
    ngx_http_complex_value_t     key;
    ngx_http_limit_req_node_t   *node;
} ngx_http_limit_req_ctx_t;


typedef struct {
    ngx_shm_zone_t              *shm_zone;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   burst;
    ngx_uint_t                   nodelay; /* unsigned  nodelay:1 */
} ngx_http_limit_req_limit_t;


typedef struct {
    ngx_array_t                  limits;
    ngx_uint_t                   limit_log_level;
    ngx_uint_t                   delay_log_level;
    ngx_uint_t                   status_code;
} ngx_http_limit_req_conf_t;


static void ngx_http_limit_req_delay(ngx_http_request_t *r);
static ngx_int_t ngx_http_limit_req_lookup(ngx_http_limit_req_limit_t *limit,
    ngx_uint_t hash, ngx_str_t *key, ngx_uint_t *ep, ngx_uint_t account);
static ngx_msec_t ngx_http_limit_req_account(ngx_http_limit_req_limit_t *limits,
    ngx_uint_t n, ngx_uint_t *ep, ngx_http_limit_req_limit_t **limit);
static void ngx_http_limit_req_expire(ngx_http_limit_req_ctx_t *ctx,
    ngx_uint_t n);

static void *ngx_http_limit_req_create_conf(ngx_conf_t *cf);
static char *ngx_http_limit_req_merge_conf(ngx_conf_t *cf, void *parent,
    void *child);
static char *ngx_http_limit_req_zone(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_limit_req(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static ngx_int_t ngx_http_limit_req_init(ngx_conf_t *cf);


static ngx_conf_enum_t  ngx_http_limit_req_log_levels[] = {
    { ngx_string("info"), NGX_LOG_INFO },
    { ngx_string("notice"), NGX_LOG_NOTICE },
    { ngx_string("warn"), NGX_LOG_WARN },
    { ngx_string("error"), NGX_LOG_ERR },
    { ngx_null_string, 0 }
};


    ngx_conf_check_num_bounds, 400, 599
};


static ngx_command_t  ngx_http_limit_req_commands[] = {

    { ngx_string("limit_req_zone"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE3,
      ngx_http_limit_req_zone,
      0,
      0,
      NULL },

    { ngx_string("limit_req"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE123,
      ngx_http_limit_req,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("limit_req_log_level"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_req_conf_t, limit_log_level),
      &ngx_http_limit_req_log_levels },

    { ngx_string("limit_req_status"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_req_conf_t, status_code),
      &ngx_http_limit_req_status_bounds },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    ngx_http_limit_req_init,               /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_limit_req_create_conf,        /* create location configuration */
    ngx_http_limit_req_merge_conf          /* merge location configuration */
};


ngx_module_t  ngx_http_limit_req_module = {
    NGX_MODULE_V1,
    &ngx_http_limit_req_module_ctx,        /* module context */
    ngx_http_limit_req_commands,           /* module directives */
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


ngx_int_t ngx_http_limit_req_handler_sense_1(int *ngx_http_limit_req_handler_sense_1_ret, uint32_t *hash, ngx_str_t key, ngx_int_t *rc, ngx_uint_t *n, ngx_uint_t *excess, ngx_msec_t *delay, ngx_http_limit_req_ctx_t *ctx, ngx_http_limit_req_conf_t *lrcf, ngx_http_limit_req_limit_t *limit, ngx_http_limit_req_limit_t *limits, ngx_http_request_t *r)
{
  if (r->main->limit_req_set)
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  lrcf = r->loc_conf[ngx_http_limit_req_module.ctx_index];
  limits = lrcf->limits.elts;
  *excess = 0;
  *rc = -5;
  for (*n = 0; (*n) < lrcf->limits.nelts; (*n)++)
  {
    limit = &limits[*n];
    ctx = limit->shm_zone->data;
    if (ngx_http_complex_value(r, &ctx->key, &key) != 0)
    {
      {
        *ngx_http_limit_req_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (key.len == 0)
    {
      continue;
    }
    if (key.len > 65535)
    {
      if (r->connection->log->log_level >= 4)
        ngx_log_error_core(4, r->connection->log, 0, "the value of the \"%V\" key is more than 65535 bytes: \"%V\"", &ctx->key.value, &key);
      continue;
    }
    *hash = ngx_crc32_short(key.data, key.len);
    ngx_shmtx_lock(&ctx->shpool->mutex);
    *rc = ngx_http_limit_req_lookup(limit, *hash, &key, &(*excess), (*n) == (lrcf->limits.nelts - 1));
    ngx_shmtx_unlock(&ctx->shpool->mutex);
    ;
    if ((*rc) != (-2))
    {
      break;
    }
  }

  if ((*rc) == (-5))
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  r->main->limit_req_set = 1;
  if (((*rc) == (-3)) || ((*rc) == (-1)))
  {
    if ((*rc) == (-3))
    {
      if (r->connection->log->log_level >= lrcf->limit_log_level)
        ngx_log_error_core(lrcf->limit_log_level, r->connection->log, 0, "limiting requests, excess: %ui.%03ui by zone \"%V\"", (*excess) / 1000, (*excess) % 1000, &limit->shm_zone->shm.name);
    }
    while ((*n)--)
    {
      ctx = limits[*n].shm_zone->data;
      if (ctx->node == 0)
      {
        continue;
      }
      ngx_shmtx_lock(&ctx->shpool->mutex);
      ctx->node->count--;
      ngx_shmtx_unlock(&ctx->shpool->mutex);
      ctx->node = 0;
    }

    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return lrcf->status_code;
    }
  }
  if ((*rc) == (-2))
  {
    *excess = 0;
  }
  *delay = ngx_http_limit_req_account(limits, *n, &(*excess), &limit);
  if (!(*delay))
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  if (r->connection->log->log_level >= lrcf->delay_log_level)
    ngx_log_error_core(lrcf->delay_log_level, r->connection->log, 0, "delaying request, excess: %ui.%03ui, by zone \"%V\"", (*excess) / 1000, (*excess) % 1000, &limit->shm_zone->shm.name);
  if (ngx_handle_read_event(r->connection->read, 0) != 0)
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return 500;
    }
  }
  r->read_event_handler = ngx_http_test_reading;
  r->write_event_handler = ngx_http_limit_req_delay;
  r->connection->write->delayed = 1;
  ngx_event_add_timer(r->connection->write, *delay);
  {
    *ngx_http_limit_req_handler_sense_1_ret = 1;
    return -2;
  }
}







static ngx_int_t
ngx_http_limit_req_lookup(ngx_http_limit_req_limit_t *limit, ngx_uint_t hash,
    ngx_str_t *key, ngx_uint_t *ep, ngx_uint_t account)
{
    size_t                      size;
    ngx_int_t                   rc, excess;
    ngx_msec_t                  now;
    ngx_msec_int_t              ms;
    ngx_rbtree_node_t          *node, *sentinel;
    ngx_http_limit_req_ctx_t   *ctx;
    ngx_http_limit_req_node_t  *lr;

    now = ngx_current_msec;

    ctx = limit->shm_zone->data;

    node = ctx->sh->rbtree.root;
    sentinel = ctx->sh->rbtree.sentinel;

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

        lr = (ngx_http_limit_req_node_t *) &node->color;

        rc = ngx_memn2cmp(key->data, lr->data, key->len, (size_t) lr->len);

        if (rc == 0) {
            ngx_queue_remove(&lr->queue);
            ngx_queue_insert_head(&ctx->sh->queue, &lr->queue);

            ms = (ngx_msec_int_t) (now - lr->last);

            if (ms < -60000) {
                ms = 1;

            } else if (ms < 0) {
                ms = 0;
            }

            excess = lr->excess - ctx->rate * ms / 1000 + 1000;

            if (excess < 0) {
                excess = 0;
            }

            *ep = excess;

            if ((ngx_uint_t) excess > limit->burst) {
                return NGX_BUSY;
            }

            if (account) {
                lr->excess = excess;

                if (ms) {
                    lr->last = now;
                }

                return NGX_OK;
            }

            lr->count++;

            ctx->node = lr;

            return NGX_AGAIN;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    *ep = 0;

    size = offsetof(ngx_rbtree_node_t, color)
           + offsetof(ngx_http_limit_req_node_t, data)
           + key->len;

    ngx_http_limit_req_expire(ctx, 1);

    node = ngx_slab_alloc_locked(ctx->shpool, size);

    if (node == NULL) {
        ngx_http_limit_req_expire(ctx, 0);

        node = ngx_slab_alloc_locked(ctx->shpool, size);
        if (node == NULL) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                          "could not allocate node%s", ctx->shpool->log_ctx);
            return NGX_ERROR;
        }
    }

    node->key = hash;

    lr = (ngx_http_limit_req_node_t *) &node->color;

    lr->len = (u_short) key->len;
    lr->excess = 0;

    ngx_memcpy(lr->data, key->data, key->len);

    ngx_rbtree_insert(&ctx->sh->rbtree, node);

    ngx_queue_insert_head(&ctx->sh->queue, &lr->queue);

    if (account) {
        lr->last = now;
        lr->count = 0;
        return NGX_OK;
    }

    lr->last = 0;
    lr->count = 1;

    ctx->node = lr;

    return NGX_AGAIN;
}


static ngx_msec_t
ngx_http_limit_req_account(ngx_http_limit_req_limit_t *limits, ngx_uint_t n,
    ngx_uint_t *ep, ngx_http_limit_req_limit_t **limit)
{
    ngx_int_t                   excess;
    ngx_msec_t                  now, delay, max_delay;
    ngx_msec_int_t              ms;
    ngx_http_limit_req_ctx_t   *ctx;
    ngx_http_limit_req_node_t  *lr;

    excess = *ep;

    if (excess == 0 || (*limit)->nodelay) {
        max_delay = 0;

    } else {
        ctx = (*limit)->shm_zone->data;
        max_delay = excess * 1000 / ctx->rate;
    }

    while (n--) {
        ctx = limits[n].shm_zone->data;
        lr = ctx->node;

        if (lr == NULL) {
            continue;
        }

        ngx_shmtx_lock(&ctx->shpool->mutex);

        now = ngx_current_msec;
        ms = (ngx_msec_int_t) (now - lr->last);

        if (ms < -60000) {
            ms = 1;

        } else if (ms < 0) {
            ms = 0;
        }

        excess = lr->excess - ctx->rate * ms / 1000 + 1000;

        if (excess < 0) {
            excess = 0;
        }

        if (ms) {
            lr->last = now;
        }

        lr->excess = excess;
        lr->count--;

        ngx_shmtx_unlock(&ctx->shpool->mutex);

        ctx->node = NULL;

        if (limits[n].nodelay) {
            continue;
        }

        delay = excess * 1000 / ctx->rate;

        if (delay > max_delay) {
            max_delay = delay;
            *ep = excess;
            *limit = &limits[n];
        }
    }

    return max_delay;
}


static void
ngx_http_limit_req_expire(ngx_http_limit_req_ctx_t *ctx, ngx_uint_t n)
{
    ngx_int_t                   excess;
    ngx_msec_t                  now;
    ngx_queue_t                *q;
    ngx_msec_int_t              ms;
    ngx_rbtree_node_t          *node;
    ngx_http_limit_req_node_t  *lr;

    now = ngx_current_msec;

    /*
     * n == 1 deletes one or two zero rate entries
     * n == 0 deletes oldest entry by force
     *        and one or two zero rate entries
     */

    while (n < 3) {

        if (ngx_queue_empty(&ctx->sh->queue)) {
            return;
        }

        q = ngx_queue_last(&ctx->sh->queue);

        lr = ngx_queue_data(q, ngx_http_limit_req_node_t, queue);

        if (lr->count) {

            /*
             * There is not much sense in looking further,
             * because we bump nodes on the lookup stage.
             */

            return;
        }

        if (n++ != 0) {

            ms = (ngx_msec_int_t) (now - lr->last);
            ms = ngx_abs(ms);

            if (ms < 60000) {
                return;
            }

            excess = lr->excess - ctx->rate * ms / 1000;

            if (excess > 0) {
                return;
            }
        }

        ngx_queue_remove(q);

        node = (ngx_rbtree_node_t *)
                   ((u_char *) lr - offsetof(ngx_rbtree_node_t, color));

        ngx_rbtree_delete(&ctx->sh->rbtree, node);

        ngx_slab_free_locked(ctx->shpool, node);
    }
}




static void *
ngx_http_limit_req_create_conf(ngx_conf_t *cf)
{
    ngx_http_limit_req_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_limit_req_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->limits.elts = NULL;
     */

    conf->limit_log_level = NGX_CONF_UNSET_UINT;
    conf->status_code = NGX_CONF_UNSET_UINT;

    return conf;
}


static char *
ngx_http_limit_req_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_limit_req_conf_t *prev = parent;
    ngx_http_limit_req_conf_t *conf = child;

    if (conf->limits.elts == NULL) {
        conf->limits = prev->limits;
    }

    ngx_conf_merge_uint_value(conf->limit_log_level, prev->limit_log_level,
                              NGX_LOG_ERR);

    conf->delay_log_level = (conf->limit_log_level == NGX_LOG_INFO) ?
                                NGX_LOG_INFO : conf->limit_log_level + 1;

    ngx_conf_merge_uint_value(conf->status_code, prev->status_code,
                              NGX_HTTP_SERVICE_UNAVAILABLE);

    return NGX_CONF_OK;
}






ngx_int_t ngx_http_limit_req_init_sense_1(int *ngx_http_limit_req_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_limit_req_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_limit_req_handler;
  {
    *ngx_http_limit_req_init_sense_1_ret = 1;
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
    u_char                       color;
    u_char                       dummy;
    u_short                      len;
    ngx_queue_t                  queue;
    ngx_msec_t                   last;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   excess;
    ngx_uint_t                   count;
    u_char                       data[1];
} ngx_http_limit_req_node_t;


typedef struct {
    ngx_rbtree_t                  rbtree;
    ngx_rbtree_node_t             sentinel;
    ngx_queue_t                   queue;
} ngx_http_limit_req_shctx_t;


typedef struct {
    ngx_http_limit_req_shctx_t  *sh;
    ngx_slab_pool_t             *shpool;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   rate;
    ngx_http_complex_value_t     key;
    ngx_http_limit_req_node_t   *node;
} ngx_http_limit_req_ctx_t;


typedef struct {
    ngx_shm_zone_t              *shm_zone;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   burst;
    ngx_uint_t                   nodelay; /* unsigned  nodelay:1 */
} ngx_http_limit_req_limit_t;


typedef struct {
    ngx_array_t                  limits;
    ngx_uint_t                   limit_log_level;
    ngx_uint_t                   delay_log_level;
    ngx_uint_t                   status_code;
} ngx_http_limit_req_conf_t;


static void ngx_http_limit_req_delay(ngx_http_request_t *r);
static ngx_int_t ngx_http_limit_req_lookup(ngx_http_limit_req_limit_t *limit,
    ngx_uint_t hash, ngx_str_t *key, ngx_uint_t *ep, ngx_uint_t account);
static ngx_msec_t ngx_http_limit_req_account(ngx_http_limit_req_limit_t *limits,
    ngx_uint_t n, ngx_uint_t *ep, ngx_http_limit_req_limit_t **limit);
static void ngx_http_limit_req_expire(ngx_http_limit_req_ctx_t *ctx,
    ngx_uint_t n);

static void *ngx_http_limit_req_create_conf(ngx_conf_t *cf);
static char *ngx_http_limit_req_merge_conf(ngx_conf_t *cf, void *parent,
    void *child);
static char *ngx_http_limit_req_zone(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_limit_req(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static ngx_int_t ngx_http_limit_req_init(ngx_conf_t *cf);


static ngx_conf_enum_t  ngx_http_limit_req_log_levels[] = {
    { ngx_string("info"), NGX_LOG_INFO },
    { ngx_string("notice"), NGX_LOG_NOTICE },
    { ngx_string("warn"), NGX_LOG_WARN },
    { ngx_string("error"), NGX_LOG_ERR },
    { ngx_null_string, 0 }
};


    ngx_conf_check_num_bounds, 400, 599
};


static ngx_command_t  ngx_http_limit_req_commands[] = {

    { ngx_string("limit_req_zone"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE3,
      ngx_http_limit_req_zone,
      0,
      0,
      NULL },

    { ngx_string("limit_req"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE123,
      ngx_http_limit_req,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("limit_req_log_level"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_req_conf_t, limit_log_level),
      &ngx_http_limit_req_log_levels },

    { ngx_string("limit_req_status"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_limit_req_conf_t, status_code),
      &ngx_http_limit_req_status_bounds },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    ngx_http_limit_req_init,               /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_limit_req_create_conf,        /* create location configuration */
    ngx_http_limit_req_merge_conf          /* merge location configuration */
};


ngx_module_t  ngx_http_limit_req_module = {
    NGX_MODULE_V1,
    &ngx_http_limit_req_module_ctx,        /* module context */
    ngx_http_limit_req_commands,           /* module directives */
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


ngx_int_t ngx_http_limit_req_handler_sense_1(int *ngx_http_limit_req_handler_sense_1_ret, uint32_t *hash, ngx_str_t key, ngx_int_t *rc, ngx_uint_t *n, ngx_uint_t *excess, ngx_msec_t *delay, ngx_http_limit_req_ctx_t *ctx, ngx_http_limit_req_conf_t *lrcf, ngx_http_limit_req_limit_t *limit, ngx_http_limit_req_limit_t *limits, ngx_http_request_t *r)
{
  if (r->main->limit_req_set)
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  lrcf = r->loc_conf[ngx_http_limit_req_module.ctx_index];
  limits = lrcf->limits.elts;
  *excess = 0;
  *rc = -5;
  for (*n = 0; (*n) < lrcf->limits.nelts; (*n)++)
  {
    limit = &limits[*n];
    ctx = limit->shm_zone->data;
    if (ngx_http_complex_value(r, &ctx->key, &key) != 0)
    {
      {
        *ngx_http_limit_req_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (key.len == 0)
    {
      continue;
    }
    if (key.len > 65535)
    {
      if (r->connection->log->log_level >= 4)
        ngx_log_error_core(4, r->connection->log, 0, "the value of the \"%V\" key is more than 65535 bytes: \"%V\"", &ctx->key.value, &key);
      continue;
    }
    *hash = ngx_crc32_short(key.data, key.len);
    ngx_shmtx_lock(&ctx->shpool->mutex);
    *rc = ngx_http_limit_req_lookup(limit, *hash, &key, &(*excess), (*n) == (lrcf->limits.nelts - 1));
    ngx_shmtx_unlock(&ctx->shpool->mutex);
    ;
    if ((*rc) != (-2))
    {
      break;
    }
  }

  if ((*rc) == (-5))
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  r->main->limit_req_set = 1;
  if (((*rc) == (-3)) || ((*rc) == (-1)))
  {
    if ((*rc) == (-3))
    {
      if (r->connection->log->log_level >= lrcf->limit_log_level)
        ngx_log_error_core(lrcf->limit_log_level, r->connection->log, 0, "limiting requests, excess: %ui.%03ui by zone \"%V\"", (*excess) / 1000, (*excess) % 1000, &limit->shm_zone->shm.name);
    }
    while ((*n)--)
    {
      ctx = limits[*n].shm_zone->data;
      if (ctx->node == 0)
      {
        continue;
      }
      ngx_shmtx_lock(&ctx->shpool->mutex);
      ctx->node->count--;
      ngx_shmtx_unlock(&ctx->shpool->mutex);
      ctx->node = 0;
    }

    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return lrcf->status_code;
    }
  }
  if ((*rc) == (-2))
  {
    *excess = 0;
  }
  *delay = ngx_http_limit_req_account(limits, *n, &(*excess), &limit);
  if (!(*delay))
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return -5;
    }
  }
  if (r->connection->log->log_level >= lrcf->delay_log_level)
    ngx_log_error_core(lrcf->delay_log_level, r->connection->log, 0, "delaying request, excess: %ui.%03ui, by zone \"%V\"", (*excess) / 1000, (*excess) % 1000, &limit->shm_zone->shm.name);
  if (ngx_handle_read_event(r->connection->read, 0) != 0)
  {
    {
      *ngx_http_limit_req_handler_sense_1_ret = 1;
      return 500;
    }
  }
  r->read_event_handler = ngx_http_test_reading;
  r->write_event_handler = ngx_http_limit_req_delay;
  r->connection->write->delayed = 1;
  ngx_event_add_timer(r->connection->write, *delay);
  {
    *ngx_http_limit_req_handler_sense_1_ret = 1;
    return -2;
  }
}







static ngx_int_t
ngx_http_limit_req_lookup(ngx_http_limit_req_limit_t *limit, ngx_uint_t hash,
    ngx_str_t *key, ngx_uint_t *ep, ngx_uint_t account)
{
    size_t                      size;
    ngx_int_t                   rc, excess;
    ngx_msec_t                  now;
    ngx_msec_int_t              ms;
    ngx_rbtree_node_t          *node, *sentinel;
    ngx_http_limit_req_ctx_t   *ctx;
    ngx_http_limit_req_node_t  *lr;

    now = ngx_current_msec;

    ctx = limit->shm_zone->data;

    node = ctx->sh->rbtree.root;
    sentinel = ctx->sh->rbtree.sentinel;

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

        lr = (ngx_http_limit_req_node_t *) &node->color;

        rc = ngx_memn2cmp(key->data, lr->data, key->len, (size_t) lr->len);

        if (rc == 0) {
            ngx_queue_remove(&lr->queue);
            ngx_queue_insert_head(&ctx->sh->queue, &lr->queue);

            ms = (ngx_msec_int_t) (now - lr->last);

            if (ms < -60000) {
                ms = 1;

            } else if (ms < 0) {
                ms = 0;
            }

            excess = lr->excess - ctx->rate * ms / 1000 + 1000;

            if (excess < 0) {
                excess = 0;
            }

            *ep = excess;

            if ((ngx_uint_t) excess > limit->burst) {
                return NGX_BUSY;
            }

            if (account) {
                lr->excess = excess;

                if (ms) {
                    lr->last = now;
                }

                return NGX_OK;
            }

            lr->count++;

            ctx->node = lr;

            return NGX_AGAIN;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    *ep = 0;

    size = offsetof(ngx_rbtree_node_t, color)
           + offsetof(ngx_http_limit_req_node_t, data)
           + key->len;

    ngx_http_limit_req_expire(ctx, 1);

    node = ngx_slab_alloc_locked(ctx->shpool, size);

    if (node == NULL) {
        ngx_http_limit_req_expire(ctx, 0);

        node = ngx_slab_alloc_locked(ctx->shpool, size);
        if (node == NULL) {
            ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                          "could not allocate node%s", ctx->shpool->log_ctx);
            return NGX_ERROR;
        }
    }

    node->key = hash;

    lr = (ngx_http_limit_req_node_t *) &node->color;

    lr->len = (u_short) key->len;
    lr->excess = 0;

    ngx_memcpy(lr->data, key->data, key->len);

    ngx_rbtree_insert(&ctx->sh->rbtree, node);

    ngx_queue_insert_head(&ctx->sh->queue, &lr->queue);

    if (account) {
        lr->last = now;
        lr->count = 0;
        return NGX_OK;
    }

    lr->last = 0;
    lr->count = 1;

    ctx->node = lr;

    return NGX_AGAIN;
}


static ngx_msec_t
ngx_http_limit_req_account(ngx_http_limit_req_limit_t *limits, ngx_uint_t n,
    ngx_uint_t *ep, ngx_http_limit_req_limit_t **limit)
{
    ngx_int_t                   excess;
    ngx_msec_t                  now, delay, max_delay;
    ngx_msec_int_t              ms;
    ngx_http_limit_req_ctx_t   *ctx;
    ngx_http_limit_req_node_t  *lr;

    excess = *ep;

    if (excess == 0 || (*limit)->nodelay) {
        max_delay = 0;

    } else {
        ctx = (*limit)->shm_zone->data;
        max_delay = excess * 1000 / ctx->rate;
    }

    while (n--) {
        ctx = limits[n].shm_zone->data;
        lr = ctx->node;

        if (lr == NULL) {
            continue;
        }

        ngx_shmtx_lock(&ctx->shpool->mutex);

        now = ngx_current_msec;
        ms = (ngx_msec_int_t) (now - lr->last);

        if (ms < -60000) {
            ms = 1;

        } else if (ms < 0) {
            ms = 0;
        }

        excess = lr->excess - ctx->rate * ms / 1000 + 1000;

        if (excess < 0) {
            excess = 0;
        }

        if (ms) {
            lr->last = now;
        }

        lr->excess = excess;
        lr->count--;

        ngx_shmtx_unlock(&ctx->shpool->mutex);

        ctx->node = NULL;

        if (limits[n].nodelay) {
            continue;
        }

        delay = excess * 1000 / ctx->rate;

        if (delay > max_delay) {
            max_delay = delay;
            *ep = excess;
            *limit = &limits[n];
        }
    }

    return max_delay;
}


static void
ngx_http_limit_req_expire(ngx_http_limit_req_ctx_t *ctx, ngx_uint_t n)
{
    ngx_int_t                   excess;
    ngx_msec_t                  now;
    ngx_queue_t                *q;
    ngx_msec_int_t              ms;
    ngx_rbtree_node_t          *node;
    ngx_http_limit_req_node_t  *lr;

    now = ngx_current_msec;

    /*
     * n == 1 deletes one or two zero rate entries
     * n == 0 deletes oldest entry by force
     *        and one or two zero rate entries
     */

    while (n < 3) {

        if (ngx_queue_empty(&ctx->sh->queue)) {
            return;
        }

        q = ngx_queue_last(&ctx->sh->queue);

        lr = ngx_queue_data(q, ngx_http_limit_req_node_t, queue);

        if (lr->count) {

            /*
             * There is not much sense in looking further,
             * because we bump nodes on the lookup stage.
             */

            return;
        }

        if (n++ != 0) {

            ms = (ngx_msec_int_t) (now - lr->last);
            ms = ngx_abs(ms);

            if (ms < 60000) {
                return;
            }

            excess = lr->excess - ctx->rate * ms / 1000;

            if (excess > 0) {
                return;
            }
        }

        ngx_queue_remove(q);

        node = (ngx_rbtree_node_t *)
                   ((u_char *) lr - offsetof(ngx_rbtree_node_t, color));

        ngx_rbtree_delete(&ctx->sh->rbtree, node);

        ngx_slab_free_locked(ctx->shpool, node);
    }
}




static void *
ngx_http_limit_req_create_conf(ngx_conf_t *cf)
{
    ngx_http_limit_req_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_limit_req_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->limits.elts = NULL;
     */

    conf->limit_log_level = NGX_CONF_UNSET_UINT;
    conf->status_code = NGX_CONF_UNSET_UINT;

    return conf;
}


static char *
ngx_http_limit_req_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_limit_req_conf_t *prev = parent;
    ngx_http_limit_req_conf_t *conf = child;

    if (conf->limits.elts == NULL) {
        conf->limits = prev->limits;
    }

    ngx_conf_merge_uint_value(conf->limit_log_level, prev->limit_log_level,
                              NGX_LOG_ERR);

    conf->delay_log_level = (conf->limit_log_level == NGX_LOG_INFO) ?
                                NGX_LOG_INFO : conf->limit_log_level + 1;

    ngx_conf_merge_uint_value(conf->status_code, prev->status_code,
                              NGX_HTTP_SERVICE_UNAVAILABLE);

    return NGX_CONF_OK;
}






ngx_int_t ngx_http_limit_req_init_sense_1(int *ngx_http_limit_req_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_limit_req_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_limit_req_handler;
  {
    *ngx_http_limit_req_init_sense_1_ret = 1;
    return 0;
  }
}

