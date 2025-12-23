
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_md5.h>


static ngx_int_t ngx_http_file_cache_lock(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static void ngx_http_file_cache_lock_wait_handler(ngx_event_t *ev);
static void ngx_http_file_cache_lock_wait(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_read(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ssize_t ngx_http_file_cache_aio_read(ngx_http_request_t *r,
    ngx_http_cache_t *c);
#if (NGX_HAVE_FILE_AIO)
static void ngx_http_cache_aio_event_handler(ngx_event_t *ev);
#endif
#if (NGX_THREADS)
static ngx_int_t ngx_http_cache_thread_handler(ngx_thread_task_t *task,
    ngx_file_t *file);
static void ngx_http_cache_thread_event_handler(ngx_event_t *ev);
#endif
static ngx_int_t ngx_http_file_cache_exists(ngx_http_file_cache_t *cache,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_name(ngx_http_request_t *r,
    ngx_path_t *path);
static ngx_http_file_cache_node_t *
    ngx_http_file_cache_lookup(ngx_http_file_cache_t *cache, u_char *key);
static void ngx_http_file_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
static void ngx_http_file_cache_vary(ngx_http_request_t *r, u_char *vary,
    size_t len, u_char *hash);
static void ngx_http_file_cache_vary_header(ngx_http_request_t *r,
    ngx_md5_t *md5, ngx_str_t *name);
static ngx_int_t ngx_http_file_cache_reopen(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_update_variant(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static void ngx_http_file_cache_cleanup(void *data);
static time_t ngx_http_file_cache_forced_expire(ngx_http_file_cache_t *cache);
static time_t ngx_http_file_cache_expire(ngx_http_file_cache_t *cache);
static void ngx_http_file_cache_delete(ngx_http_file_cache_t *cache,
    ngx_queue_t *q, u_char *name);
static void ngx_http_file_cache_loader_sleep(ngx_http_file_cache_t *cache);
static ngx_int_t ngx_http_file_cache_noop(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_manage_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_manage_directory(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_add_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_add(ngx_http_file_cache_t *cache,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_delete_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static void ngx_http_file_cache_set_watermark(ngx_http_file_cache_t *cache);


ngx_str_t  ngx_http_cache_status[] = {
    ngx_string("MISS"),
    ngx_string("BYPASS"),
    ngx_string("EXPIRED"),
    ngx_string("STALE"),
    ngx_string("UPDATING"),
    ngx_string("REVALIDATED"),
    ngx_string("HIT")
};


static u_char  ngx_http_file_cache_key[] = { LF, 'K', 'E', 'Y', ':', ' ' };


static ngx_int_t
ngx_http_file_cache_init(ngx_shm_zone_t *shm_zone, void *data)
{
    ngx_http_file_cache_t  *ocache = data;

    size_t                  len;
    ngx_uint_t              n;
    ngx_http_file_cache_t  *cache;

    cache = shm_zone->data;

    if (ocache) {
        if (ngx_strcmp(cache->path->name.data, ocache->path->name.data) != 0) {
            ngx_log_error(NGX_LOG_EMERG, shm_zone->shm.log, 0,
                          "cache \"%V\" uses the \"%V\" cache path "
                          "while previously it used the \"%V\" cache path",
                          &shm_zone->shm.name, &cache->path->name,
                          &ocache->path->name);

            return NGX_ERROR;
        }

        for (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
            if (cache->path->level[n] != ocache->path->level[n]) {
                ngx_log_error(NGX_LOG_EMERG, shm_zone->shm.log, 0,
                              "cache \"%V\" had previously different levels",
                              &shm_zone->shm.name);
                return NGX_ERROR;
            }
        }

        cache->sh = ocache->sh;

        cache->shpool = ocache->shpool;
        cache->bsize = ocache->bsize;

        cache->max_size /= cache->bsize;

        if (!cache->sh->cold || cache->sh->loading) {
            cache->path->loader = NULL;
        }

        return NGX_OK;
    }

    cache->shpool = (ngx_slab_pool_t *) shm_zone->shm.addr;

    if (shm_zone->shm.exists) {
        cache->sh = cache->shpool->data;
        cache->bsize = ngx_fs_bsize(cache->path->name.data);
        cache->max_size /= cache->bsize;

        return NGX_OK;
    }

    cache->sh = ngx_slab_alloc(cache->shpool, sizeof(ngx_http_file_cache_sh_t));
    if (cache->sh == NULL) {
        return NGX_ERROR;
    }

    cache->shpool->data = cache->sh;

    ngx_rbtree_init(&cache->sh->rbtree, &cache->sh->sentinel,
                    ngx_http_file_cache_rbtree_insert_value);

    ngx_queue_init(&cache->sh->queue);

    cache->sh->cold = 1;
    cache->sh->loading = 0;
    cache->sh->size = 0;
    cache->sh->count = 0;
    cache->sh->watermark = (ngx_uint_t) -1;

    cache->bsize = ngx_fs_bsize(cache->path->name.data);

    cache->max_size /= cache->bsize;

    len = sizeof(" in cache keys zone \"\"") + shm_zone->shm.name.len;

    cache->shpool->log_ctx = ngx_slab_alloc(cache->shpool, len);
    if (cache->shpool->log_ctx == NULL) {
        return NGX_ERROR;
    }

    ngx_sprintf(cache->shpool->log_ctx, " in cache keys zone \"%V\"%Z",
                &shm_zone->shm.name);

    cache->shpool->log_nomem = 0;

    return NGX_OK;
}




















#if (NGX_HAVE_FILE_AIO)

static void
ngx_http_cache_aio_event_handler(ngx_event_t *ev)
{
    ngx_event_aio_t     *aio;
    ngx_connection_t    *c;
    ngx_http_request_t  *r;

    aio = ev->data;
    r = aio->data;
    c = r->connection;

    ngx_http_set_log_request(c->log, r);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "http file cache aio: \"%V?%V\"", &r->uri, &r->args);

    r->main->blocked--;
    r->aio = 0;

    r->write_event_handler(r);

    ngx_http_run_posted_requests(c);
}

#endif


#if (NGX_THREADS)

static ngx_int_t
ngx_http_cache_thread_handler(ngx_thread_task_t *task, ngx_file_t *file)
{
    ngx_str_t                  name;
    ngx_thread_pool_t         *tp;
    ngx_http_request_t        *r;
    ngx_http_core_loc_conf_t  *clcf;

    r = file->thread_ctx;

    clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);
    tp = clcf->thread_pool;

    if (tp == NULL) {
        if (ngx_http_complex_value(r, clcf->thread_pool_value, &name)
            != NGX_OK)
        {
            return NGX_ERROR;
        }

        tp = ngx_thread_pool_get((ngx_cycle_t *) ngx_cycle, &name);

        if (tp == NULL) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "thread pool \"%V\" not found", &name);
            return NGX_ERROR;
        }
    }

    task->event.data = r;
    task->event.handler = ngx_http_cache_thread_event_handler;

    if (ngx_thread_task_post(tp, task) != NGX_OK) {
        return NGX_ERROR;
    }

    r->main->blocked++;
    r->aio = 1;

    return NGX_OK;
}


static void
ngx_http_cache_thread_event_handler(ngx_event_t *ev)
{
    ngx_connection_t    *c;
    ngx_http_request_t  *r;

    r = ev->data;
    c = r->connection;

    ngx_http_set_log_request(c->log, r);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "http file cache thread: \"%V?%V\"", &r->uri, &r->args);

    r->main->blocked--;
    r->aio = 0;

    r->write_event_handler(r);

    ngx_http_run_posted_requests(c);
}

#endif






static ngx_http_file_cache_node_t *
ngx_http_file_cache_lookup(ngx_http_file_cache_t *cache, u_char *key)
{
    ngx_int_t                    rc;
    ngx_rbtree_key_t             node_key;
    ngx_rbtree_node_t           *node, *sentinel;
    ngx_http_file_cache_node_t  *fcn;

    ngx_memcpy((u_char *) &node_key, key, sizeof(ngx_rbtree_key_t));

    node = cache->sh->rbtree.root;
    sentinel = cache->sh->rbtree.sentinel;

    while (node != sentinel) {

        if (node_key < node->key) {
            node = node->left;
            continue;
        }

        if (node_key > node->key) {
            node = node->right;
            continue;
        }

        /* node_key == node->key */

        fcn = (ngx_http_file_cache_node_t *) node;

        rc = ngx_memcmp(&key[sizeof(ngx_rbtree_key_t)], fcn->key,
                        NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t));

        if (rc == 0) {
            return fcn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    return NULL;
}


static void
ngx_http_file_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t           **p;
    ngx_http_file_cache_node_t   *cn, *cnt;

    for ( ;; ) {

        if (node->key < temp->key) {

            p = &temp->left;

        } else if (node->key > temp->key) {

            p = &temp->right;

        } else { /* node->key == temp->key */

            cn = (ngx_http_file_cache_node_t *) node;
            cnt = (ngx_http_file_cache_node_t *) temp;

            p = (ngx_memcmp(cn->key, cnt->key,
                            NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t))
                 < 0)
                    ? &temp->left : &temp->right;
        }

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}






























static void
ngx_http_file_cache_loader(void *data)
{
    ngx_http_file_cache_t  *cache = data;

    ngx_tree_ctx_t  tree;

    if (!cache->sh->cold || cache->sh->loading) {
        return;
    }

    if (!ngx_atomic_cmp_set(&cache->sh->loading, 0, ngx_pid)) {
        return;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache loader");

    tree.init_handler = NULL;
    tree.file_handler = ngx_http_file_cache_manage_file;
    tree.pre_tree_handler = ngx_http_file_cache_manage_directory;
    tree.post_tree_handler = ngx_http_file_cache_noop;
    tree.spec_handler = ngx_http_file_cache_delete_file;
    tree.data = cache;
    tree.alloc = 0;
    tree.log = ngx_cycle->log;

    cache->last = ngx_current_msec;
    cache->files = 0;

    if (ngx_walk_tree(&tree, &cache->path->name) == NGX_ABORT) {
        cache->sh->loading = 0;
        return;
    }

    cache->sh->cold = 0;
    cache->sh->loading = 0;

    ngx_log_error(NGX_LOG_NOTICE, ngx_cycle->log, 0,
                  "http file cache: %V %.3fM, bsize: %uz",
                  &cache->path->name,
                  ((double) cache->sh->size * cache->bsize) / (1024 * 1024),
                  cache->bsize);
}










static ngx_int_t
ngx_http_file_cache_add_file(ngx_tree_ctx_t *ctx, ngx_str_t *name)
{
    u_char                 *p;
    ngx_int_t               n;
    ngx_uint_t              i;
    ngx_http_cache_t        c;
    ngx_http_file_cache_t  *cache;

    if (name->len < 2 * NGX_HTTP_CACHE_KEY_LEN) {
        return NGX_ERROR;
    }

    /*
     * Temporary files in cache have a suffix consisting of a dot
     * followed by 10 digits.
     */

    if (name->len >= 2 * NGX_HTTP_CACHE_KEY_LEN + 1 + 10
        && name->data[name->len - 10 - 1] == '.')
    {
        return NGX_OK;
    }

    if (ctx->size < (off_t) sizeof(ngx_http_file_cache_header_t)) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, 0,
                      "cache file \"%s\" is too small", name->data);
        return NGX_ERROR;
    }

    ngx_memzero(&c, sizeof(ngx_http_cache_t));
    cache = ctx->data;

    c.length = ctx->size;
    c.fs_size = (ctx->fs_size + cache->bsize - 1) / cache->bsize;

    p = &name->data[name->len - 2 * NGX_HTTP_CACHE_KEY_LEN];

    for (i = 0; i < NGX_HTTP_CACHE_KEY_LEN; i++) {
        n = ngx_hextoi(p, 2);

        if (n == NGX_ERROR) {
            return NGX_ERROR;
        }

        p += 2;

        c.key[i] = (u_char) n;
    }

    return ngx_http_file_cache_add(cache, &c);
}


static ngx_int_t
ngx_http_file_cache_add(ngx_http_file_cache_t *cache, ngx_http_cache_t *c)
{
    ngx_http_file_cache_node_t  *fcn;

    ngx_shmtx_lock(&cache->shpool->mutex);

    fcn = ngx_http_file_cache_lookup(cache, c->key);

    if (fcn == NULL) {

        fcn = ngx_slab_calloc_locked(cache->shpool,
                                     sizeof(ngx_http_file_cache_node_t));
        if (fcn == NULL) {
            ngx_http_file_cache_set_watermark(cache);

            if (cache->fail_time != ngx_time()) {
                cache->fail_time = ngx_time();
                ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                           "could not allocate node%s", cache->shpool->log_ctx);
            }

            ngx_shmtx_unlock(&cache->shpool->mutex);
            return NGX_ERROR;
        }

        cache->sh->count++;

        ngx_memcpy((u_char *) &fcn->node.key, c->key, sizeof(ngx_rbtree_key_t));

        ngx_memcpy(fcn->key, &c->key[sizeof(ngx_rbtree_key_t)],
                   NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t));

        ngx_rbtree_insert(&cache->sh->rbtree, &fcn->node);

        fcn->uses = 1;
        fcn->exists = 1;
        fcn->fs_size = c->fs_size;

        cache->sh->size += c->fs_size;

    } else {
        ngx_queue_remove(&fcn->queue);
    }

    fcn->expire = ngx_time() + cache->inactive;

    ngx_queue_insert_head(&cache->sh->queue, &fcn->queue);

    ngx_shmtx_unlock(&cache->shpool->mutex);

    return NGX_OK;
}




static void
ngx_http_file_cache_set_watermark(ngx_http_file_cache_t *cache)
{
    cache->sh->watermark = cache->sh->count - cache->sh->count / 8;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache watermark: %ui", cache->sh->watermark);
}






/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_md5.h>


static ngx_int_t ngx_http_file_cache_lock(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static void ngx_http_file_cache_lock_wait_handler(ngx_event_t *ev);
static void ngx_http_file_cache_lock_wait(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_read(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ssize_t ngx_http_file_cache_aio_read(ngx_http_request_t *r,
    ngx_http_cache_t *c);
#if (NGX_HAVE_FILE_AIO)
static void ngx_http_cache_aio_event_handler(ngx_event_t *ev);
#endif
#if (NGX_THREADS)
static ngx_int_t ngx_http_cache_thread_handler(ngx_thread_task_t *task,
    ngx_file_t *file);
static void ngx_http_cache_thread_event_handler(ngx_event_t *ev);
#endif
static ngx_int_t ngx_http_file_cache_exists(ngx_http_file_cache_t *cache,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_name(ngx_http_request_t *r,
    ngx_path_t *path);
static ngx_http_file_cache_node_t *
    ngx_http_file_cache_lookup(ngx_http_file_cache_t *cache, u_char *key);
static void ngx_http_file_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
static void ngx_http_file_cache_vary(ngx_http_request_t *r, u_char *vary,
    size_t len, u_char *hash);
static void ngx_http_file_cache_vary_header(ngx_http_request_t *r,
    ngx_md5_t *md5, ngx_str_t *name);
static ngx_int_t ngx_http_file_cache_reopen(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_update_variant(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static void ngx_http_file_cache_cleanup(void *data);
static time_t ngx_http_file_cache_forced_expire(ngx_http_file_cache_t *cache);
static time_t ngx_http_file_cache_expire(ngx_http_file_cache_t *cache);
static void ngx_http_file_cache_delete(ngx_http_file_cache_t *cache,
    ngx_queue_t *q, u_char *name);
static void ngx_http_file_cache_loader_sleep(ngx_http_file_cache_t *cache);
static ngx_int_t ngx_http_file_cache_noop(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_manage_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_manage_directory(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_add_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_add(ngx_http_file_cache_t *cache,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_delete_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static void ngx_http_file_cache_set_watermark(ngx_http_file_cache_t *cache);


ngx_str_t  ngx_http_cache_status[] = {
    ngx_string("MISS"),
    ngx_string("BYPASS"),
    ngx_string("EXPIRED"),
    ngx_string("STALE"),
    ngx_string("UPDATING"),
    ngx_string("REVALIDATED"),
    ngx_string("HIT")
};


static u_char  ngx_http_file_cache_key[] = { LF, 'K', 'E', 'Y', ':', ' ' };


static ngx_int_t
ngx_http_file_cache_init(ngx_shm_zone_t *shm_zone, void *data)
{
    ngx_http_file_cache_t  *ocache = data;

    size_t                  len;
    ngx_uint_t              n;
    ngx_http_file_cache_t  *cache;

    cache = shm_zone->data;

    if (ocache) {
        if (ngx_strcmp(cache->path->name.data, ocache->path->name.data) != 0) {
            ngx_log_error(NGX_LOG_EMERG, shm_zone->shm.log, 0,
                          "cache \"%V\" uses the \"%V\" cache path "
                          "while previously it used the \"%V\" cache path",
                          &shm_zone->shm.name, &cache->path->name,
                          &ocache->path->name);

            return NGX_ERROR;
        }

        for (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
            if (cache->path->level[n] != ocache->path->level[n]) {
                ngx_log_error(NGX_LOG_EMERG, shm_zone->shm.log, 0,
                              "cache \"%V\" had previously different levels",
                              &shm_zone->shm.name);
                return NGX_ERROR;
            }
        }

        cache->sh = ocache->sh;

        cache->shpool = ocache->shpool;
        cache->bsize = ocache->bsize;

        cache->max_size /= cache->bsize;

        if (!cache->sh->cold || cache->sh->loading) {
            cache->path->loader = NULL;
        }

        return NGX_OK;
    }

    cache->shpool = (ngx_slab_pool_t *) shm_zone->shm.addr;

    if (shm_zone->shm.exists) {
        cache->sh = cache->shpool->data;
        cache->bsize = ngx_fs_bsize(cache->path->name.data);
        cache->max_size /= cache->bsize;

        return NGX_OK;
    }

    cache->sh = ngx_slab_alloc(cache->shpool, sizeof(ngx_http_file_cache_sh_t));
    if (cache->sh == NULL) {
        return NGX_ERROR;
    }

    cache->shpool->data = cache->sh;

    ngx_rbtree_init(&cache->sh->rbtree, &cache->sh->sentinel,
                    ngx_http_file_cache_rbtree_insert_value);

    ngx_queue_init(&cache->sh->queue);

    cache->sh->cold = 1;
    cache->sh->loading = 0;
    cache->sh->size = 0;
    cache->sh->count = 0;
    cache->sh->watermark = (ngx_uint_t) -1;

    cache->bsize = ngx_fs_bsize(cache->path->name.data);

    cache->max_size /= cache->bsize;

    len = sizeof(" in cache keys zone \"\"") + shm_zone->shm.name.len;

    cache->shpool->log_ctx = ngx_slab_alloc(cache->shpool, len);
    if (cache->shpool->log_ctx == NULL) {
        return NGX_ERROR;
    }

    ngx_sprintf(cache->shpool->log_ctx, " in cache keys zone \"%V\"%Z",
                &shm_zone->shm.name);

    cache->shpool->log_nomem = 0;

    return NGX_OK;
}




















#if (NGX_HAVE_FILE_AIO)

static void
ngx_http_cache_aio_event_handler(ngx_event_t *ev)
{
    ngx_event_aio_t     *aio;
    ngx_connection_t    *c;
    ngx_http_request_t  *r;

    aio = ev->data;
    r = aio->data;
    c = r->connection;

    ngx_http_set_log_request(c->log, r);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "http file cache aio: \"%V?%V\"", &r->uri, &r->args);

    r->main->blocked--;
    r->aio = 0;

    r->write_event_handler(r);

    ngx_http_run_posted_requests(c);
}

#endif


#if (NGX_THREADS)

static ngx_int_t
ngx_http_cache_thread_handler(ngx_thread_task_t *task, ngx_file_t *file)
{
    ngx_str_t                  name;
    ngx_thread_pool_t         *tp;
    ngx_http_request_t        *r;
    ngx_http_core_loc_conf_t  *clcf;

    r = file->thread_ctx;

    clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);
    tp = clcf->thread_pool;

    if (tp == NULL) {
        if (ngx_http_complex_value(r, clcf->thread_pool_value, &name)
            != NGX_OK)
        {
            return NGX_ERROR;
        }

        tp = ngx_thread_pool_get((ngx_cycle_t *) ngx_cycle, &name);

        if (tp == NULL) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "thread pool \"%V\" not found", &name);
            return NGX_ERROR;
        }
    }

    task->event.data = r;
    task->event.handler = ngx_http_cache_thread_event_handler;

    if (ngx_thread_task_post(tp, task) != NGX_OK) {
        return NGX_ERROR;
    }

    r->main->blocked++;
    r->aio = 1;

    return NGX_OK;
}


static void
ngx_http_cache_thread_event_handler(ngx_event_t *ev)
{
    ngx_connection_t    *c;
    ngx_http_request_t  *r;

    r = ev->data;
    c = r->connection;

    ngx_http_set_log_request(c->log, r);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "http file cache thread: \"%V?%V\"", &r->uri, &r->args);

    r->main->blocked--;
    r->aio = 0;

    r->write_event_handler(r);

    ngx_http_run_posted_requests(c);
}

#endif






static ngx_http_file_cache_node_t *
ngx_http_file_cache_lookup(ngx_http_file_cache_t *cache, u_char *key)
{
    ngx_int_t                    rc;
    ngx_rbtree_key_t             node_key;
    ngx_rbtree_node_t           *node, *sentinel;
    ngx_http_file_cache_node_t  *fcn;

    ngx_memcpy((u_char *) &node_key, key, sizeof(ngx_rbtree_key_t));

    node = cache->sh->rbtree.root;
    sentinel = cache->sh->rbtree.sentinel;

    while (node != sentinel) {

        if (node_key < node->key) {
            node = node->left;
            continue;
        }

        if (node_key > node->key) {
            node = node->right;
            continue;
        }

        /* node_key == node->key */

        fcn = (ngx_http_file_cache_node_t *) node;

        rc = ngx_memcmp(&key[sizeof(ngx_rbtree_key_t)], fcn->key,
                        NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t));

        if (rc == 0) {
            return fcn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    return NULL;
}


static void
ngx_http_file_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t           **p;
    ngx_http_file_cache_node_t   *cn, *cnt;

    for ( ;; ) {

        if (node->key < temp->key) {

            p = &temp->left;

        } else if (node->key > temp->key) {

            p = &temp->right;

        } else { /* node->key == temp->key */

            cn = (ngx_http_file_cache_node_t *) node;
            cnt = (ngx_http_file_cache_node_t *) temp;

            p = (ngx_memcmp(cn->key, cnt->key,
                            NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t))
                 < 0)
                    ? &temp->left : &temp->right;
        }

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}






























static void
ngx_http_file_cache_loader(void *data)
{
    ngx_http_file_cache_t  *cache = data;

    ngx_tree_ctx_t  tree;

    if (!cache->sh->cold || cache->sh->loading) {
        return;
    }

    if (!ngx_atomic_cmp_set(&cache->sh->loading, 0, ngx_pid)) {
        return;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache loader");

    tree.init_handler = NULL;
    tree.file_handler = ngx_http_file_cache_manage_file;
    tree.pre_tree_handler = ngx_http_file_cache_manage_directory;
    tree.post_tree_handler = ngx_http_file_cache_noop;
    tree.spec_handler = ngx_http_file_cache_delete_file;
    tree.data = cache;
    tree.alloc = 0;
    tree.log = ngx_cycle->log;

    cache->last = ngx_current_msec;
    cache->files = 0;

    if (ngx_walk_tree(&tree, &cache->path->name) == NGX_ABORT) {
        cache->sh->loading = 0;
        return;
    }

    cache->sh->cold = 0;
    cache->sh->loading = 0;

    ngx_log_error(NGX_LOG_NOTICE, ngx_cycle->log, 0,
                  "http file cache: %V %.3fM, bsize: %uz",
                  &cache->path->name,
                  ((double) cache->sh->size * cache->bsize) / (1024 * 1024),
                  cache->bsize);
}










static ngx_int_t
ngx_http_file_cache_add_file(ngx_tree_ctx_t *ctx, ngx_str_t *name)
{
    u_char                 *p;
    ngx_int_t               n;
    ngx_uint_t              i;
    ngx_http_cache_t        c;
    ngx_http_file_cache_t  *cache;

    if (name->len < 2 * NGX_HTTP_CACHE_KEY_LEN) {
        return NGX_ERROR;
    }

    /*
     * Temporary files in cache have a suffix consisting of a dot
     * followed by 10 digits.
     */

    if (name->len >= 2 * NGX_HTTP_CACHE_KEY_LEN + 1 + 10
        && name->data[name->len - 10 - 1] == '.')
    {
        return NGX_OK;
    }

    if (ctx->size < (off_t) sizeof(ngx_http_file_cache_header_t)) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, 0,
                      "cache file \"%s\" is too small", name->data);
        return NGX_ERROR;
    }

    ngx_memzero(&c, sizeof(ngx_http_cache_t));
    cache = ctx->data;

    c.length = ctx->size;
    c.fs_size = (ctx->fs_size + cache->bsize - 1) / cache->bsize;

    p = &name->data[name->len - 2 * NGX_HTTP_CACHE_KEY_LEN];

    for (i = 0; i < NGX_HTTP_CACHE_KEY_LEN; i++) {
        n = ngx_hextoi(p, 2);

        if (n == NGX_ERROR) {
            return NGX_ERROR;
        }

        p += 2;

        c.key[i] = (u_char) n;
    }

    return ngx_http_file_cache_add(cache, &c);
}


static ngx_int_t
ngx_http_file_cache_add(ngx_http_file_cache_t *cache, ngx_http_cache_t *c)
{
    ngx_http_file_cache_node_t  *fcn;

    ngx_shmtx_lock(&cache->shpool->mutex);

    fcn = ngx_http_file_cache_lookup(cache, c->key);

    if (fcn == NULL) {

        fcn = ngx_slab_calloc_locked(cache->shpool,
                                     sizeof(ngx_http_file_cache_node_t));
        if (fcn == NULL) {
            ngx_http_file_cache_set_watermark(cache);

            if (cache->fail_time != ngx_time()) {
                cache->fail_time = ngx_time();
                ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                           "could not allocate node%s", cache->shpool->log_ctx);
            }

            ngx_shmtx_unlock(&cache->shpool->mutex);
            return NGX_ERROR;
        }

        cache->sh->count++;

        ngx_memcpy((u_char *) &fcn->node.key, c->key, sizeof(ngx_rbtree_key_t));

        ngx_memcpy(fcn->key, &c->key[sizeof(ngx_rbtree_key_t)],
                   NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t));

        ngx_rbtree_insert(&cache->sh->rbtree, &fcn->node);

        fcn->uses = 1;
        fcn->exists = 1;
        fcn->fs_size = c->fs_size;

        cache->sh->size += c->fs_size;

    } else {
        ngx_queue_remove(&fcn->queue);
    }

    fcn->expire = ngx_time() + cache->inactive;

    ngx_queue_insert_head(&cache->sh->queue, &fcn->queue);

    ngx_shmtx_unlock(&cache->shpool->mutex);

    return NGX_OK;
}




static void
ngx_http_file_cache_set_watermark(ngx_http_file_cache_t *cache)
{
    cache->sh->watermark = cache->sh->count - cache->sh->count / 8;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache watermark: %ui", cache->sh->watermark);
}






/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_md5.h>


static ngx_int_t ngx_http_file_cache_lock(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static void ngx_http_file_cache_lock_wait_handler(ngx_event_t *ev);
static void ngx_http_file_cache_lock_wait(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_read(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ssize_t ngx_http_file_cache_aio_read(ngx_http_request_t *r,
    ngx_http_cache_t *c);
#if (NGX_HAVE_FILE_AIO)
static void ngx_http_cache_aio_event_handler(ngx_event_t *ev);
#endif
#if (NGX_THREADS)
static ngx_int_t ngx_http_cache_thread_handler(ngx_thread_task_t *task,
    ngx_file_t *file);
static void ngx_http_cache_thread_event_handler(ngx_event_t *ev);
#endif
static ngx_int_t ngx_http_file_cache_exists(ngx_http_file_cache_t *cache,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_name(ngx_http_request_t *r,
    ngx_path_t *path);
static ngx_http_file_cache_node_t *
    ngx_http_file_cache_lookup(ngx_http_file_cache_t *cache, u_char *key);
static void ngx_http_file_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
static void ngx_http_file_cache_vary(ngx_http_request_t *r, u_char *vary,
    size_t len, u_char *hash);
static void ngx_http_file_cache_vary_header(ngx_http_request_t *r,
    ngx_md5_t *md5, ngx_str_t *name);
static ngx_int_t ngx_http_file_cache_reopen(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_update_variant(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static void ngx_http_file_cache_cleanup(void *data);
static time_t ngx_http_file_cache_forced_expire(ngx_http_file_cache_t *cache);
static time_t ngx_http_file_cache_expire(ngx_http_file_cache_t *cache);
static void ngx_http_file_cache_delete(ngx_http_file_cache_t *cache,
    ngx_queue_t *q, u_char *name);
static void ngx_http_file_cache_loader_sleep(ngx_http_file_cache_t *cache);
static ngx_int_t ngx_http_file_cache_noop(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_manage_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_manage_directory(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_add_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_add(ngx_http_file_cache_t *cache,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_delete_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static void ngx_http_file_cache_set_watermark(ngx_http_file_cache_t *cache);


ngx_str_t  ngx_http_cache_status[] = {
    ngx_string("MISS"),
    ngx_string("BYPASS"),
    ngx_string("EXPIRED"),
    ngx_string("STALE"),
    ngx_string("UPDATING"),
    ngx_string("REVALIDATED"),
    ngx_string("HIT")
};


static u_char  ngx_http_file_cache_key[] = { LF, 'K', 'E', 'Y', ':', ' ' };


static ngx_int_t
ngx_http_file_cache_init(ngx_shm_zone_t *shm_zone, void *data)
{
    ngx_http_file_cache_t  *ocache = data;

    size_t                  len;
    ngx_uint_t              n;
    ngx_http_file_cache_t  *cache;

    cache = shm_zone->data;

    if (ocache) {
        if (ngx_strcmp(cache->path->name.data, ocache->path->name.data) != 0) {
            ngx_log_error(NGX_LOG_EMERG, shm_zone->shm.log, 0,
                          "cache \"%V\" uses the \"%V\" cache path "
                          "while previously it used the \"%V\" cache path",
                          &shm_zone->shm.name, &cache->path->name,
                          &ocache->path->name);

            return NGX_ERROR;
        }

        for (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
            if (cache->path->level[n] != ocache->path->level[n]) {
                ngx_log_error(NGX_LOG_EMERG, shm_zone->shm.log, 0,
                              "cache \"%V\" had previously different levels",
                              &shm_zone->shm.name);
                return NGX_ERROR;
            }
        }

        cache->sh = ocache->sh;

        cache->shpool = ocache->shpool;
        cache->bsize = ocache->bsize;

        cache->max_size /= cache->bsize;

        if (!cache->sh->cold || cache->sh->loading) {
            cache->path->loader = NULL;
        }

        return NGX_OK;
    }

    cache->shpool = (ngx_slab_pool_t *) shm_zone->shm.addr;

    if (shm_zone->shm.exists) {
        cache->sh = cache->shpool->data;
        cache->bsize = ngx_fs_bsize(cache->path->name.data);
        cache->max_size /= cache->bsize;

        return NGX_OK;
    }

    cache->sh = ngx_slab_alloc(cache->shpool, sizeof(ngx_http_file_cache_sh_t));
    if (cache->sh == NULL) {
        return NGX_ERROR;
    }

    cache->shpool->data = cache->sh;

    ngx_rbtree_init(&cache->sh->rbtree, &cache->sh->sentinel,
                    ngx_http_file_cache_rbtree_insert_value);

    ngx_queue_init(&cache->sh->queue);

    cache->sh->cold = 1;
    cache->sh->loading = 0;
    cache->sh->size = 0;
    cache->sh->count = 0;
    cache->sh->watermark = (ngx_uint_t) -1;

    cache->bsize = ngx_fs_bsize(cache->path->name.data);

    cache->max_size /= cache->bsize;

    len = sizeof(" in cache keys zone \"\"") + shm_zone->shm.name.len;

    cache->shpool->log_ctx = ngx_slab_alloc(cache->shpool, len);
    if (cache->shpool->log_ctx == NULL) {
        return NGX_ERROR;
    }

    ngx_sprintf(cache->shpool->log_ctx, " in cache keys zone \"%V\"%Z",
                &shm_zone->shm.name);

    cache->shpool->log_nomem = 0;

    return NGX_OK;
}




















#if (NGX_HAVE_FILE_AIO)

static void
ngx_http_cache_aio_event_handler(ngx_event_t *ev)
{
    ngx_event_aio_t     *aio;
    ngx_connection_t    *c;
    ngx_http_request_t  *r;

    aio = ev->data;
    r = aio->data;
    c = r->connection;

    ngx_http_set_log_request(c->log, r);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "http file cache aio: \"%V?%V\"", &r->uri, &r->args);

    r->main->blocked--;
    r->aio = 0;

    r->write_event_handler(r);

    ngx_http_run_posted_requests(c);
}

#endif


#if (NGX_THREADS)

static ngx_int_t
ngx_http_cache_thread_handler(ngx_thread_task_t *task, ngx_file_t *file)
{
    ngx_str_t                  name;
    ngx_thread_pool_t         *tp;
    ngx_http_request_t        *r;
    ngx_http_core_loc_conf_t  *clcf;

    r = file->thread_ctx;

    clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);
    tp = clcf->thread_pool;

    if (tp == NULL) {
        if (ngx_http_complex_value(r, clcf->thread_pool_value, &name)
            != NGX_OK)
        {
            return NGX_ERROR;
        }

        tp = ngx_thread_pool_get((ngx_cycle_t *) ngx_cycle, &name);

        if (tp == NULL) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "thread pool \"%V\" not found", &name);
            return NGX_ERROR;
        }
    }

    task->event.data = r;
    task->event.handler = ngx_http_cache_thread_event_handler;

    if (ngx_thread_task_post(tp, task) != NGX_OK) {
        return NGX_ERROR;
    }

    r->main->blocked++;
    r->aio = 1;

    return NGX_OK;
}


static void
ngx_http_cache_thread_event_handler(ngx_event_t *ev)
{
    ngx_connection_t    *c;
    ngx_http_request_t  *r;

    r = ev->data;
    c = r->connection;

    ngx_http_set_log_request(c->log, r);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "http file cache thread: \"%V?%V\"", &r->uri, &r->args);

    r->main->blocked--;
    r->aio = 0;

    r->write_event_handler(r);

    ngx_http_run_posted_requests(c);
}

#endif






static ngx_http_file_cache_node_t *
ngx_http_file_cache_lookup(ngx_http_file_cache_t *cache, u_char *key)
{
    ngx_int_t                    rc;
    ngx_rbtree_key_t             node_key;
    ngx_rbtree_node_t           *node, *sentinel;
    ngx_http_file_cache_node_t  *fcn;

    ngx_memcpy((u_char *) &node_key, key, sizeof(ngx_rbtree_key_t));

    node = cache->sh->rbtree.root;
    sentinel = cache->sh->rbtree.sentinel;

    while (node != sentinel) {

        if (node_key < node->key) {
            node = node->left;
            continue;
        }

        if (node_key > node->key) {
            node = node->right;
            continue;
        }

        /* node_key == node->key */

        fcn = (ngx_http_file_cache_node_t *) node;

        rc = ngx_memcmp(&key[sizeof(ngx_rbtree_key_t)], fcn->key,
                        NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t));

        if (rc == 0) {
            return fcn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    return NULL;
}


static void
ngx_http_file_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t           **p;
    ngx_http_file_cache_node_t   *cn, *cnt;

    for ( ;; ) {

        if (node->key < temp->key) {

            p = &temp->left;

        } else if (node->key > temp->key) {

            p = &temp->right;

        } else { /* node->key == temp->key */

            cn = (ngx_http_file_cache_node_t *) node;
            cnt = (ngx_http_file_cache_node_t *) temp;

            p = (ngx_memcmp(cn->key, cnt->key,
                            NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t))
                 < 0)
                    ? &temp->left : &temp->right;
        }

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}






























static void
ngx_http_file_cache_loader(void *data)
{
    ngx_http_file_cache_t  *cache = data;

    ngx_tree_ctx_t  tree;

    if (!cache->sh->cold || cache->sh->loading) {
        return;
    }

    if (!ngx_atomic_cmp_set(&cache->sh->loading, 0, ngx_pid)) {
        return;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache loader");

    tree.init_handler = NULL;
    tree.file_handler = ngx_http_file_cache_manage_file;
    tree.pre_tree_handler = ngx_http_file_cache_manage_directory;
    tree.post_tree_handler = ngx_http_file_cache_noop;
    tree.spec_handler = ngx_http_file_cache_delete_file;
    tree.data = cache;
    tree.alloc = 0;
    tree.log = ngx_cycle->log;

    cache->last = ngx_current_msec;
    cache->files = 0;

    if (ngx_walk_tree(&tree, &cache->path->name) == NGX_ABORT) {
        cache->sh->loading = 0;
        return;
    }

    cache->sh->cold = 0;
    cache->sh->loading = 0;

    ngx_log_error(NGX_LOG_NOTICE, ngx_cycle->log, 0,
                  "http file cache: %V %.3fM, bsize: %uz",
                  &cache->path->name,
                  ((double) cache->sh->size * cache->bsize) / (1024 * 1024),
                  cache->bsize);
}










static ngx_int_t
ngx_http_file_cache_add_file(ngx_tree_ctx_t *ctx, ngx_str_t *name)
{
    u_char                 *p;
    ngx_int_t               n;
    ngx_uint_t              i;
    ngx_http_cache_t        c;
    ngx_http_file_cache_t  *cache;

    if (name->len < 2 * NGX_HTTP_CACHE_KEY_LEN) {
        return NGX_ERROR;
    }

    /*
     * Temporary files in cache have a suffix consisting of a dot
     * followed by 10 digits.
     */

    if (name->len >= 2 * NGX_HTTP_CACHE_KEY_LEN + 1 + 10
        && name->data[name->len - 10 - 1] == '.')
    {
        return NGX_OK;
    }

    if (ctx->size < (off_t) sizeof(ngx_http_file_cache_header_t)) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, 0,
                      "cache file \"%s\" is too small", name->data);
        return NGX_ERROR;
    }

    ngx_memzero(&c, sizeof(ngx_http_cache_t));
    cache = ctx->data;

    c.length = ctx->size;
    c.fs_size = (ctx->fs_size + cache->bsize - 1) / cache->bsize;

    p = &name->data[name->len - 2 * NGX_HTTP_CACHE_KEY_LEN];

    for (i = 0; i < NGX_HTTP_CACHE_KEY_LEN; i++) {
        n = ngx_hextoi(p, 2);

        if (n == NGX_ERROR) {
            return NGX_ERROR;
        }

        p += 2;

        c.key[i] = (u_char) n;
    }

    return ngx_http_file_cache_add(cache, &c);
}


static ngx_int_t
ngx_http_file_cache_add(ngx_http_file_cache_t *cache, ngx_http_cache_t *c)
{
    ngx_http_file_cache_node_t  *fcn;

    ngx_shmtx_lock(&cache->shpool->mutex);

    fcn = ngx_http_file_cache_lookup(cache, c->key);

    if (fcn == NULL) {

        fcn = ngx_slab_calloc_locked(cache->shpool,
                                     sizeof(ngx_http_file_cache_node_t));
        if (fcn == NULL) {
            ngx_http_file_cache_set_watermark(cache);

            if (cache->fail_time != ngx_time()) {
                cache->fail_time = ngx_time();
                ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                           "could not allocate node%s", cache->shpool->log_ctx);
            }

            ngx_shmtx_unlock(&cache->shpool->mutex);
            return NGX_ERROR;
        }

        cache->sh->count++;

        ngx_memcpy((u_char *) &fcn->node.key, c->key, sizeof(ngx_rbtree_key_t));

        ngx_memcpy(fcn->key, &c->key[sizeof(ngx_rbtree_key_t)],
                   NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t));

        ngx_rbtree_insert(&cache->sh->rbtree, &fcn->node);

        fcn->uses = 1;
        fcn->exists = 1;
        fcn->fs_size = c->fs_size;

        cache->sh->size += c->fs_size;

    } else {
        ngx_queue_remove(&fcn->queue);
    }

    fcn->expire = ngx_time() + cache->inactive;

    ngx_queue_insert_head(&cache->sh->queue, &fcn->queue);

    ngx_shmtx_unlock(&cache->shpool->mutex);

    return NGX_OK;
}




static void
ngx_http_file_cache_set_watermark(ngx_http_file_cache_t *cache)
{
    cache->sh->watermark = cache->sh->count - cache->sh->count / 8;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache watermark: %ui", cache->sh->watermark);
}






/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_md5.h>


static ngx_int_t ngx_http_file_cache_lock(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static void ngx_http_file_cache_lock_wait_handler(ngx_event_t *ev);
static void ngx_http_file_cache_lock_wait(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_read(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ssize_t ngx_http_file_cache_aio_read(ngx_http_request_t *r,
    ngx_http_cache_t *c);
#if (NGX_HAVE_FILE_AIO)
static void ngx_http_cache_aio_event_handler(ngx_event_t *ev);
#endif
#if (NGX_THREADS)
static ngx_int_t ngx_http_cache_thread_handler(ngx_thread_task_t *task,
    ngx_file_t *file);
static void ngx_http_cache_thread_event_handler(ngx_event_t *ev);
#endif
static ngx_int_t ngx_http_file_cache_exists(ngx_http_file_cache_t *cache,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_name(ngx_http_request_t *r,
    ngx_path_t *path);
static ngx_http_file_cache_node_t *
    ngx_http_file_cache_lookup(ngx_http_file_cache_t *cache, u_char *key);
static void ngx_http_file_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
static void ngx_http_file_cache_vary(ngx_http_request_t *r, u_char *vary,
    size_t len, u_char *hash);
static void ngx_http_file_cache_vary_header(ngx_http_request_t *r,
    ngx_md5_t *md5, ngx_str_t *name);
static ngx_int_t ngx_http_file_cache_reopen(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_update_variant(ngx_http_request_t *r,
    ngx_http_cache_t *c);
static void ngx_http_file_cache_cleanup(void *data);
static time_t ngx_http_file_cache_forced_expire(ngx_http_file_cache_t *cache);
static time_t ngx_http_file_cache_expire(ngx_http_file_cache_t *cache);
static void ngx_http_file_cache_delete(ngx_http_file_cache_t *cache,
    ngx_queue_t *q, u_char *name);
static void ngx_http_file_cache_loader_sleep(ngx_http_file_cache_t *cache);
static ngx_int_t ngx_http_file_cache_noop(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_manage_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_manage_directory(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_add_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static ngx_int_t ngx_http_file_cache_add(ngx_http_file_cache_t *cache,
    ngx_http_cache_t *c);
static ngx_int_t ngx_http_file_cache_delete_file(ngx_tree_ctx_t *ctx,
    ngx_str_t *path);
static void ngx_http_file_cache_set_watermark(ngx_http_file_cache_t *cache);


ngx_str_t  ngx_http_cache_status[] = {
    ngx_string("MISS"),
    ngx_string("BYPASS"),
    ngx_string("EXPIRED"),
    ngx_string("STALE"),
    ngx_string("UPDATING"),
    ngx_string("REVALIDATED"),
    ngx_string("HIT")
};


static u_char  ngx_http_file_cache_key[] = { LF, 'K', 'E', 'Y', ':', ' ' };


static ngx_int_t
ngx_http_file_cache_init(ngx_shm_zone_t *shm_zone, void *data)
{
    ngx_http_file_cache_t  *ocache = data;

    size_t                  len;
    ngx_uint_t              n;
    ngx_http_file_cache_t  *cache;

    cache = shm_zone->data;

    if (ocache) {
        if (ngx_strcmp(cache->path->name.data, ocache->path->name.data) != 0) {
            ngx_log_error(NGX_LOG_EMERG, shm_zone->shm.log, 0,
                          "cache \"%V\" uses the \"%V\" cache path "
                          "while previously it used the \"%V\" cache path",
                          &shm_zone->shm.name, &cache->path->name,
                          &ocache->path->name);

            return NGX_ERROR;
        }

        for (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
            if (cache->path->level[n] != ocache->path->level[n]) {
                ngx_log_error(NGX_LOG_EMERG, shm_zone->shm.log, 0,
                              "cache \"%V\" had previously different levels",
                              &shm_zone->shm.name);
                return NGX_ERROR;
            }
        }

        cache->sh = ocache->sh;

        cache->shpool = ocache->shpool;
        cache->bsize = ocache->bsize;

        cache->max_size /= cache->bsize;

        if (!cache->sh->cold || cache->sh->loading) {
            cache->path->loader = NULL;
        }

        return NGX_OK;
    }

    cache->shpool = (ngx_slab_pool_t *) shm_zone->shm.addr;

    if (shm_zone->shm.exists) {
        cache->sh = cache->shpool->data;
        cache->bsize = ngx_fs_bsize(cache->path->name.data);
        cache->max_size /= cache->bsize;

        return NGX_OK;
    }

    cache->sh = ngx_slab_alloc(cache->shpool, sizeof(ngx_http_file_cache_sh_t));
    if (cache->sh == NULL) {
        return NGX_ERROR;
    }

    cache->shpool->data = cache->sh;

    ngx_rbtree_init(&cache->sh->rbtree, &cache->sh->sentinel,
                    ngx_http_file_cache_rbtree_insert_value);

    ngx_queue_init(&cache->sh->queue);

    cache->sh->cold = 1;
    cache->sh->loading = 0;
    cache->sh->size = 0;
    cache->sh->count = 0;
    cache->sh->watermark = (ngx_uint_t) -1;

    cache->bsize = ngx_fs_bsize(cache->path->name.data);

    cache->max_size /= cache->bsize;

    len = sizeof(" in cache keys zone \"\"") + shm_zone->shm.name.len;

    cache->shpool->log_ctx = ngx_slab_alloc(cache->shpool, len);
    if (cache->shpool->log_ctx == NULL) {
        return NGX_ERROR;
    }

    ngx_sprintf(cache->shpool->log_ctx, " in cache keys zone \"%V\"%Z",
                &shm_zone->shm.name);

    cache->shpool->log_nomem = 0;

    return NGX_OK;
}




















#if (NGX_HAVE_FILE_AIO)

static void
ngx_http_cache_aio_event_handler(ngx_event_t *ev)
{
    ngx_event_aio_t     *aio;
    ngx_connection_t    *c;
    ngx_http_request_t  *r;

    aio = ev->data;
    r = aio->data;
    c = r->connection;

    ngx_http_set_log_request(c->log, r);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "http file cache aio: \"%V?%V\"", &r->uri, &r->args);

    r->main->blocked--;
    r->aio = 0;

    r->write_event_handler(r);

    ngx_http_run_posted_requests(c);
}

#endif


#if (NGX_THREADS)

static ngx_int_t
ngx_http_cache_thread_handler(ngx_thread_task_t *task, ngx_file_t *file)
{
    ngx_str_t                  name;
    ngx_thread_pool_t         *tp;
    ngx_http_request_t        *r;
    ngx_http_core_loc_conf_t  *clcf;

    r = file->thread_ctx;

    clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);
    tp = clcf->thread_pool;

    if (tp == NULL) {
        if (ngx_http_complex_value(r, clcf->thread_pool_value, &name)
            != NGX_OK)
        {
            return NGX_ERROR;
        }

        tp = ngx_thread_pool_get((ngx_cycle_t *) ngx_cycle, &name);

        if (tp == NULL) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "thread pool \"%V\" not found", &name);
            return NGX_ERROR;
        }
    }

    task->event.data = r;
    task->event.handler = ngx_http_cache_thread_event_handler;

    if (ngx_thread_task_post(tp, task) != NGX_OK) {
        return NGX_ERROR;
    }

    r->main->blocked++;
    r->aio = 1;

    return NGX_OK;
}


static void
ngx_http_cache_thread_event_handler(ngx_event_t *ev)
{
    ngx_connection_t    *c;
    ngx_http_request_t  *r;

    r = ev->data;
    c = r->connection;

    ngx_http_set_log_request(c->log, r);

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "http file cache thread: \"%V?%V\"", &r->uri, &r->args);

    r->main->blocked--;
    r->aio = 0;

    r->write_event_handler(r);

    ngx_http_run_posted_requests(c);
}

#endif






static ngx_http_file_cache_node_t *
ngx_http_file_cache_lookup(ngx_http_file_cache_t *cache, u_char *key)
{
    ngx_int_t                    rc;
    ngx_rbtree_key_t             node_key;
    ngx_rbtree_node_t           *node, *sentinel;
    ngx_http_file_cache_node_t  *fcn;

    ngx_memcpy((u_char *) &node_key, key, sizeof(ngx_rbtree_key_t));

    node = cache->sh->rbtree.root;
    sentinel = cache->sh->rbtree.sentinel;

    while (node != sentinel) {

        if (node_key < node->key) {
            node = node->left;
            continue;
        }

        if (node_key > node->key) {
            node = node->right;
            continue;
        }

        /* node_key == node->key */

        fcn = (ngx_http_file_cache_node_t *) node;

        rc = ngx_memcmp(&key[sizeof(ngx_rbtree_key_t)], fcn->key,
                        NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t));

        if (rc == 0) {
            return fcn;
        }

        node = (rc < 0) ? node->left : node->right;
    }

    /* not found */

    return NULL;
}


static void
ngx_http_file_cache_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t           **p;
    ngx_http_file_cache_node_t   *cn, *cnt;

    for ( ;; ) {

        if (node->key < temp->key) {

            p = &temp->left;

        } else if (node->key > temp->key) {

            p = &temp->right;

        } else { /* node->key == temp->key */

            cn = (ngx_http_file_cache_node_t *) node;
            cnt = (ngx_http_file_cache_node_t *) temp;

            p = (ngx_memcmp(cn->key, cnt->key,
                            NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t))
                 < 0)
                    ? &temp->left : &temp->right;
        }

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}






























static void
ngx_http_file_cache_loader(void *data)
{
    ngx_http_file_cache_t  *cache = data;

    ngx_tree_ctx_t  tree;

    if (!cache->sh->cold || cache->sh->loading) {
        return;
    }

    if (!ngx_atomic_cmp_set(&cache->sh->loading, 0, ngx_pid)) {
        return;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache loader");

    tree.init_handler = NULL;
    tree.file_handler = ngx_http_file_cache_manage_file;
    tree.pre_tree_handler = ngx_http_file_cache_manage_directory;
    tree.post_tree_handler = ngx_http_file_cache_noop;
    tree.spec_handler = ngx_http_file_cache_delete_file;
    tree.data = cache;
    tree.alloc = 0;
    tree.log = ngx_cycle->log;

    cache->last = ngx_current_msec;
    cache->files = 0;

    if (ngx_walk_tree(&tree, &cache->path->name) == NGX_ABORT) {
        cache->sh->loading = 0;
        return;
    }

    cache->sh->cold = 0;
    cache->sh->loading = 0;

    ngx_log_error(NGX_LOG_NOTICE, ngx_cycle->log, 0,
                  "http file cache: %V %.3fM, bsize: %uz",
                  &cache->path->name,
                  ((double) cache->sh->size * cache->bsize) / (1024 * 1024),
                  cache->bsize);
}










static ngx_int_t
ngx_http_file_cache_add_file(ngx_tree_ctx_t *ctx, ngx_str_t *name)
{
    u_char                 *p;
    ngx_int_t               n;
    ngx_uint_t              i;
    ngx_http_cache_t        c;
    ngx_http_file_cache_t  *cache;

    if (name->len < 2 * NGX_HTTP_CACHE_KEY_LEN) {
        return NGX_ERROR;
    }

    /*
     * Temporary files in cache have a suffix consisting of a dot
     * followed by 10 digits.
     */

    if (name->len >= 2 * NGX_HTTP_CACHE_KEY_LEN + 1 + 10
        && name->data[name->len - 10 - 1] == '.')
    {
        return NGX_OK;
    }

    if (ctx->size < (off_t) sizeof(ngx_http_file_cache_header_t)) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, 0,
                      "cache file \"%s\" is too small", name->data);
        return NGX_ERROR;
    }

    ngx_memzero(&c, sizeof(ngx_http_cache_t));
    cache = ctx->data;

    c.length = ctx->size;
    c.fs_size = (ctx->fs_size + cache->bsize - 1) / cache->bsize;

    p = &name->data[name->len - 2 * NGX_HTTP_CACHE_KEY_LEN];

    for (i = 0; i < NGX_HTTP_CACHE_KEY_LEN; i++) {
        n = ngx_hextoi(p, 2);

        if (n == NGX_ERROR) {
            return NGX_ERROR;
        }

        p += 2;

        c.key[i] = (u_char) n;
    }

    return ngx_http_file_cache_add(cache, &c);
}


static ngx_int_t
ngx_http_file_cache_add(ngx_http_file_cache_t *cache, ngx_http_cache_t *c)
{
    ngx_http_file_cache_node_t  *fcn;

    ngx_shmtx_lock(&cache->shpool->mutex);

    fcn = ngx_http_file_cache_lookup(cache, c->key);

    if (fcn == NULL) {

        fcn = ngx_slab_calloc_locked(cache->shpool,
                                     sizeof(ngx_http_file_cache_node_t));
        if (fcn == NULL) {
            ngx_http_file_cache_set_watermark(cache);

            if (cache->fail_time != ngx_time()) {
                cache->fail_time = ngx_time();
                ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                           "could not allocate node%s", cache->shpool->log_ctx);
            }

            ngx_shmtx_unlock(&cache->shpool->mutex);
            return NGX_ERROR;
        }

        cache->sh->count++;

        ngx_memcpy((u_char *) &fcn->node.key, c->key, sizeof(ngx_rbtree_key_t));

        ngx_memcpy(fcn->key, &c->key[sizeof(ngx_rbtree_key_t)],
                   NGX_HTTP_CACHE_KEY_LEN - sizeof(ngx_rbtree_key_t));

        ngx_rbtree_insert(&cache->sh->rbtree, &fcn->node);

        fcn->uses = 1;
        fcn->exists = 1;
        fcn->fs_size = c->fs_size;

        cache->sh->size += c->fs_size;

    } else {
        ngx_queue_remove(&fcn->queue);
    }

    fcn->expire = ngx_time() + cache->inactive;

    ngx_queue_insert_head(&cache->sh->queue, &fcn->queue);

    ngx_shmtx_unlock(&cache->shpool->mutex);

    return NGX_OK;
}




static void
ngx_http_file_cache_set_watermark(ngx_http_file_cache_t *cache)
{
    cache->sh->watermark = cache->sh->count - cache->sh->count / 8;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, ngx_cycle->log, 0,
                   "http file cache watermark: %ui", cache->sh->watermark);
}






