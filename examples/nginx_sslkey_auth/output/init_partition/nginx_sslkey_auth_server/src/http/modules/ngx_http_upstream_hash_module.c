
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    uint32_t                            hash;
    ngx_str_t                          *server;
} ngx_http_upstream_chash_point_t;


typedef struct {
    ngx_uint_t                          number;
    ngx_http_upstream_chash_point_t     point[1];
} ngx_http_upstream_chash_points_t;


typedef struct {
    ngx_http_complex_value_t            key;
    ngx_http_upstream_chash_points_t   *points;
} ngx_http_upstream_hash_srv_conf_t;


typedef struct {
    /* the round robin data must be first */
    ngx_http_upstream_rr_peer_data_t    rrp;
    ngx_http_upstream_hash_srv_conf_t  *conf;
    ngx_str_t                           key;
    ngx_uint_t                          tries;
    ngx_uint_t                          rehash;
    uint32_t                            hash;
    ngx_event_get_peer_pt               get_rr_peer;
} ngx_http_upstream_hash_peer_data_t;


static ngx_int_t ngx_http_upstream_init_hash(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_init_hash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_get_hash_peer(ngx_peer_connection_t *pc,
    void *data);

static ngx_int_t ngx_http_upstream_init_chash(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
    ngx_http_upstream_chash_cmp_points(const void *one, const void *two);
static ngx_uint_t ngx_http_upstream_find_chash_point(
    ngx_http_upstream_chash_points_t *points, uint32_t hash);
static ngx_int_t ngx_http_upstream_init_chash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_get_chash_peer(ngx_peer_connection_t *pc,
    void *data);

static void *ngx_http_upstream_hash_create_conf(ngx_conf_t *cf);
static char *ngx_http_upstream_hash(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


static ngx_command_t  ngx_http_upstream_hash_commands[] = {

    { ngx_string("hash"),
      NGX_HTTP_UPS_CONF|NGX_CONF_TAKE12,
      ngx_http_upstream_hash,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_upstream_hash_create_conf,    /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


    NGX_MODULE_V1,
    &ngx_http_upstream_hash_module_ctx,    /* module context */
    ngx_http_upstream_hash_commands,       /* module directives */
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










ngx_http_upstream_chash_cmp_points(const void *one, const void *two)
{
    ngx_http_upstream_chash_point_t *first =
                                       (ngx_http_upstream_chash_point_t *) one;
    ngx_http_upstream_chash_point_t *second =
                                       (ngx_http_upstream_chash_point_t *) two;

    if (first->hash < second->hash) {
        return -1;

    } else if (first->hash > second->hash) {
        return 1;

    } else {
        return 0;
    }
}


static ngx_uint_t
ngx_http_upstream_find_chash_point(ngx_http_upstream_chash_points_t *points,
    uint32_t hash)
{
    ngx_uint_t                        i, j, k;
    ngx_http_upstream_chash_point_t  *point;

    /* find first point >= hash */

    point = &points->point[0];

    i = 0;
    j = points->number;

    while (i < j) {
        k = (i + j) / 2;

        if (hash > point[k].hash) {
            i = k + 1;

        } else if (hash < point[k].hash) {
            j = k;

        } else {
            return k;
        }
    }

    return i;
}


static ngx_int_t
ngx_http_upstream_init_chash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us)
{
    uint32_t                             hash;
    ngx_http_upstream_hash_srv_conf_t   *hcf;
    ngx_http_upstream_hash_peer_data_t  *hp;

    if (ngx_http_upstream_init_hash_peer(r, us) != NGX_OK) {
        return NGX_ERROR;
    }

    r->upstream->peer.get = ngx_http_upstream_get_chash_peer;

    hp = r->upstream->peer.data;
    hcf = ngx_http_conf_upstream_srv_conf(us, ngx_http_upstream_hash_module);

    hash = ngx_crc32_long(hp->key.data, hp->key.len);

    ngx_http_upstream_rr_peers_rlock(hp->rrp.peers);

    hp->hash = ngx_http_upstream_find_chash_point(hcf->points, hash);

    ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);

    return NGX_OK;
}


static ngx_int_t
ngx_http_upstream_get_chash_peer(ngx_peer_connection_t *pc, void *data)
{
    ngx_http_upstream_hash_peer_data_t  *hp = data;

    time_t                              now;
    intptr_t                            m;
    ngx_str_t                          *server;
    ngx_int_t                           total;
    ngx_uint_t                          i, n, best_i;
    ngx_http_upstream_rr_peer_t        *peer, *best;
    ngx_http_upstream_chash_point_t    *point;
    ngx_http_upstream_chash_points_t   *points;
    ngx_http_upstream_hash_srv_conf_t  *hcf;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                   "get consistent hash peer, try: %ui", pc->tries);

    ngx_http_upstream_rr_peers_wlock(hp->rrp.peers);

    if (hp->tries > 20 || hp->rrp.peers->single) {
        ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);
        return hp->get_rr_peer(pc, &hp->rrp);
    }

    pc->cached = 0;
    pc->connection = NULL;

    now = ngx_time();
    hcf = hp->conf;

    points = hcf->points;
    point = &points->point[0];

    for ( ;; ) {
        server = point[hp->hash % points->number].server;

        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                       "consistent hash peer:%uD, server:\"%V\"",
                       hp->hash, server);

        best = NULL;
        best_i = 0;
        total = 0;

        for (peer = hp->rrp.peers->peer, i = 0;
             peer;
             peer = peer->next, i++)
        {
            n = i / (8 * sizeof(uintptr_t));
            m = (uintptr_t) 1 << i % (8 * sizeof(uintptr_t));

            if (hp->rrp.tried[n] & m) {
                continue;
            }

            if (peer->down) {
                continue;
            }

            if (peer->max_fails
                && peer->fails >= peer->max_fails
                && now - peer->checked <= peer->fail_timeout)
            {
                continue;
            }

            if (peer->max_conns && peer->conns >= peer->max_conns) {
                continue;
            }

            if (peer->server.len != server->len
                || ngx_strncmp(peer->server.data, server->data, server->len)
                   != 0)
            {
                continue;
            }

            peer->current_weight += peer->effective_weight;
            total += peer->effective_weight;

            if (peer->effective_weight < peer->weight) {
                peer->effective_weight++;
            }

            if (best == NULL || peer->current_weight > best->current_weight) {
                best = peer;
                best_i = i;
            }
        }

        if (best) {
            best->current_weight -= total;
            goto found;
        }

        hp->hash++;
        hp->tries++;

        if (hp->tries > 20) {
            ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);
            return hp->get_rr_peer(pc, &hp->rrp);
        }
    }

found:

    hp->rrp.current = best;

    pc->sockaddr = best->sockaddr;
    pc->socklen = best->socklen;
    pc->name = &best->name;

    best->conns++;

    if (now - best->checked > best->fail_timeout) {
        best->checked = now;
    }

    ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);

    n = best_i / (8 * sizeof(uintptr_t));
    m = (uintptr_t) 1 << best_i % (8 * sizeof(uintptr_t));

    hp->rrp.tried[n] |= m;

    return NGX_OK;
}


static void *
ngx_http_upstream_hash_create_conf(ngx_conf_t *cf)
{
    ngx_http_upstream_hash_srv_conf_t  *conf;

    conf = ngx_palloc(cf->pool, sizeof(ngx_http_upstream_hash_srv_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    conf->points = NULL;

    return conf;
}


static char *
ngx_http_upstream_hash(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_upstream_hash_srv_conf_t  *hcf = conf;

    ngx_str_t                         *value;
    ngx_http_upstream_srv_conf_t      *uscf;
    ngx_http_compile_complex_value_t   ccv;

    value = cf->args->elts;

    ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));

    ccv.cf = cf;
    ccv.value = &value[1];
    ccv.complex_value = &hcf->key;

    if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    uscf = ngx_http_conf_get_module_srv_conf(cf, ngx_http_upstream_module);

    if (uscf->peer.init_upstream) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "load balancing method redefined");
    }

    uscf->flags = NGX_HTTP_UPSTREAM_CREATE
                  |NGX_HTTP_UPSTREAM_WEIGHT
                  |NGX_HTTP_UPSTREAM_MAX_CONNS
                  |NGX_HTTP_UPSTREAM_MAX_FAILS
                  |NGX_HTTP_UPSTREAM_FAIL_TIMEOUT
                  |NGX_HTTP_UPSTREAM_DOWN;

    if (cf->args->nelts == 2) {
        uscf->peer.init_upstream = ngx_http_upstream_init_hash;

    } else if (ngx_strcmp(value[2].data, "consistent") == 0) {
        uscf->peer.init_upstream = ngx_http_upstream_init_chash;

    } else {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "invalid parameter \"%V\"", &value[2]);
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;

/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    uint32_t                            hash;
    ngx_str_t                          *server;
} ngx_http_upstream_chash_point_t;


typedef struct {
    ngx_uint_t                          number;
    ngx_http_upstream_chash_point_t     point[1];
} ngx_http_upstream_chash_points_t;


typedef struct {
    ngx_http_complex_value_t            key;
    ngx_http_upstream_chash_points_t   *points;
} ngx_http_upstream_hash_srv_conf_t;


typedef struct {
    /* the round robin data must be first */
    ngx_http_upstream_rr_peer_data_t    rrp;
    ngx_http_upstream_hash_srv_conf_t  *conf;
    ngx_str_t                           key;
    ngx_uint_t                          tries;
    ngx_uint_t                          rehash;
    uint32_t                            hash;
    ngx_event_get_peer_pt               get_rr_peer;
} ngx_http_upstream_hash_peer_data_t;


static ngx_int_t ngx_http_upstream_init_hash(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_init_hash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_get_hash_peer(ngx_peer_connection_t *pc,
    void *data);

static ngx_int_t ngx_http_upstream_init_chash(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
    ngx_http_upstream_chash_cmp_points(const void *one, const void *two);
static ngx_uint_t ngx_http_upstream_find_chash_point(
    ngx_http_upstream_chash_points_t *points, uint32_t hash);
static ngx_int_t ngx_http_upstream_init_chash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_get_chash_peer(ngx_peer_connection_t *pc,
    void *data);

static void *ngx_http_upstream_hash_create_conf(ngx_conf_t *cf);
static char *ngx_http_upstream_hash(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


static ngx_command_t  ngx_http_upstream_hash_commands[] = {

    { ngx_string("hash"),
      NGX_HTTP_UPS_CONF|NGX_CONF_TAKE12,
      ngx_http_upstream_hash,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_upstream_hash_create_conf,    /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


    NGX_MODULE_V1,
    &ngx_http_upstream_hash_module_ctx,    /* module context */
    ngx_http_upstream_hash_commands,       /* module directives */
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










ngx_http_upstream_chash_cmp_points(const void *one, const void *two)
{
    ngx_http_upstream_chash_point_t *first =
                                       (ngx_http_upstream_chash_point_t *) one;
    ngx_http_upstream_chash_point_t *second =
                                       (ngx_http_upstream_chash_point_t *) two;

    if (first->hash < second->hash) {
        return -1;

    } else if (first->hash > second->hash) {
        return 1;

    } else {
        return 0;
    }
}


static ngx_uint_t
ngx_http_upstream_find_chash_point(ngx_http_upstream_chash_points_t *points,
    uint32_t hash)
{
    ngx_uint_t                        i, j, k;
    ngx_http_upstream_chash_point_t  *point;

    /* find first point >= hash */

    point = &points->point[0];

    i = 0;
    j = points->number;

    while (i < j) {
        k = (i + j) / 2;

        if (hash > point[k].hash) {
            i = k + 1;

        } else if (hash < point[k].hash) {
            j = k;

        } else {
            return k;
        }
    }

    return i;
}


static ngx_int_t
ngx_http_upstream_init_chash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us)
{
    uint32_t                             hash;
    ngx_http_upstream_hash_srv_conf_t   *hcf;
    ngx_http_upstream_hash_peer_data_t  *hp;

    if (ngx_http_upstream_init_hash_peer(r, us) != NGX_OK) {
        return NGX_ERROR;
    }

    r->upstream->peer.get = ngx_http_upstream_get_chash_peer;

    hp = r->upstream->peer.data;
    hcf = ngx_http_conf_upstream_srv_conf(us, ngx_http_upstream_hash_module);

    hash = ngx_crc32_long(hp->key.data, hp->key.len);

    ngx_http_upstream_rr_peers_rlock(hp->rrp.peers);

    hp->hash = ngx_http_upstream_find_chash_point(hcf->points, hash);

    ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);

    return NGX_OK;
}


static ngx_int_t
ngx_http_upstream_get_chash_peer(ngx_peer_connection_t *pc, void *data)
{
    ngx_http_upstream_hash_peer_data_t  *hp = data;

    time_t                              now;
    intptr_t                            m;
    ngx_str_t                          *server;
    ngx_int_t                           total;
    ngx_uint_t                          i, n, best_i;
    ngx_http_upstream_rr_peer_t        *peer, *best;
    ngx_http_upstream_chash_point_t    *point;
    ngx_http_upstream_chash_points_t   *points;
    ngx_http_upstream_hash_srv_conf_t  *hcf;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                   "get consistent hash peer, try: %ui", pc->tries);

    ngx_http_upstream_rr_peers_wlock(hp->rrp.peers);

    if (hp->tries > 20 || hp->rrp.peers->single) {
        ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);
        return hp->get_rr_peer(pc, &hp->rrp);
    }

    pc->cached = 0;
    pc->connection = NULL;

    now = ngx_time();
    hcf = hp->conf;

    points = hcf->points;
    point = &points->point[0];

    for ( ;; ) {
        server = point[hp->hash % points->number].server;

        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                       "consistent hash peer:%uD, server:\"%V\"",
                       hp->hash, server);

        best = NULL;
        best_i = 0;
        total = 0;

        for (peer = hp->rrp.peers->peer, i = 0;
             peer;
             peer = peer->next, i++)
        {
            n = i / (8 * sizeof(uintptr_t));
            m = (uintptr_t) 1 << i % (8 * sizeof(uintptr_t));

            if (hp->rrp.tried[n] & m) {
                continue;
            }

            if (peer->down) {
                continue;
            }

            if (peer->max_fails
                && peer->fails >= peer->max_fails
                && now - peer->checked <= peer->fail_timeout)
            {
                continue;
            }

            if (peer->max_conns && peer->conns >= peer->max_conns) {
                continue;
            }

            if (peer->server.len != server->len
                || ngx_strncmp(peer->server.data, server->data, server->len)
                   != 0)
            {
                continue;
            }

            peer->current_weight += peer->effective_weight;
            total += peer->effective_weight;

            if (peer->effective_weight < peer->weight) {
                peer->effective_weight++;
            }

            if (best == NULL || peer->current_weight > best->current_weight) {
                best = peer;
                best_i = i;
            }
        }

        if (best) {
            best->current_weight -= total;
            goto found;
        }

        hp->hash++;
        hp->tries++;

        if (hp->tries > 20) {
            ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);
            return hp->get_rr_peer(pc, &hp->rrp);
        }
    }

found:

    hp->rrp.current = best;

    pc->sockaddr = best->sockaddr;
    pc->socklen = best->socklen;
    pc->name = &best->name;

    best->conns++;

    if (now - best->checked > best->fail_timeout) {
        best->checked = now;
    }

    ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);

    n = best_i / (8 * sizeof(uintptr_t));
    m = (uintptr_t) 1 << best_i % (8 * sizeof(uintptr_t));

    hp->rrp.tried[n] |= m;

    return NGX_OK;
}


static void *
ngx_http_upstream_hash_create_conf(ngx_conf_t *cf)
{
    ngx_http_upstream_hash_srv_conf_t  *conf;

    conf = ngx_palloc(cf->pool, sizeof(ngx_http_upstream_hash_srv_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    conf->points = NULL;

    return conf;
}


static char *
ngx_http_upstream_hash(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_upstream_hash_srv_conf_t  *hcf = conf;

    ngx_str_t                         *value;
    ngx_http_upstream_srv_conf_t      *uscf;
    ngx_http_compile_complex_value_t   ccv;

    value = cf->args->elts;

    ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));

    ccv.cf = cf;
    ccv.value = &value[1];
    ccv.complex_value = &hcf->key;

    if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    uscf = ngx_http_conf_get_module_srv_conf(cf, ngx_http_upstream_module);

    if (uscf->peer.init_upstream) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "load balancing method redefined");
    }

    uscf->flags = NGX_HTTP_UPSTREAM_CREATE
                  |NGX_HTTP_UPSTREAM_WEIGHT
                  |NGX_HTTP_UPSTREAM_MAX_CONNS
                  |NGX_HTTP_UPSTREAM_MAX_FAILS
                  |NGX_HTTP_UPSTREAM_FAIL_TIMEOUT
                  |NGX_HTTP_UPSTREAM_DOWN;

    if (cf->args->nelts == 2) {
        uscf->peer.init_upstream = ngx_http_upstream_init_hash;

    } else if (ngx_strcmp(value[2].data, "consistent") == 0) {
        uscf->peer.init_upstream = ngx_http_upstream_init_chash;

    } else {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "invalid parameter \"%V\"", &value[2]);
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}
