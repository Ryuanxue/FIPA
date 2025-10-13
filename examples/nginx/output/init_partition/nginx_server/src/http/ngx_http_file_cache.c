
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


























































































