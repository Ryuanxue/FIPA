
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    ngx_uint_t                  hash_max_size;
    ngx_uint_t                  hash_bucket_size;
} ngx_http_map_conf_t;


typedef struct {
    ngx_hash_keys_arrays_t      keys;

    ngx_array_t                *values_hash;
#if (NGX_PCRE)
    ngx_array_t                 regexes;
#endif

    ngx_http_variable_value_t  *default_value;
    ngx_conf_t                 *cf;
    unsigned                    hostnames:1;
    unsigned                    no_cacheable:1;
} ngx_http_map_conf_ctx_t;


typedef struct {
    ngx_http_map_t              map;
    ngx_http_complex_value_t    value;
    ngx_http_variable_value_t  *default_value;
    ngx_uint_t                  hostnames;      /* unsigned  hostnames:1 */
} ngx_http_map_ctx_t;


    const void *two);
static void *ngx_http_map_create_conf(ngx_conf_t *cf);
static char *ngx_http_map_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_http_map(ngx_conf_t *cf, ngx_command_t *dummy, void *conf);


static ngx_command_t  ngx_http_map_commands[] = {

    { ngx_string("map"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_TAKE2,
      ngx_http_map_block,
      NGX_HTTP_MAIN_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("map_hash_max_size"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_MAIN_CONF_OFFSET,
      offsetof(ngx_http_map_conf_t, hash_max_size),
      NULL },

    { ngx_string("map_hash_bucket_size"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_MAIN_CONF_OFFSET,
      offsetof(ngx_http_map_conf_t, hash_bucket_size),
      NULL },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    ngx_http_map_create_conf,              /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


    NGX_MODULE_V1,
    &ngx_http_map_module_ctx,              /* module context */
    ngx_http_map_commands,                 /* module directives */
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








ngx_http_map_cmp_dns_wildcards(const void *one, const void *two)
{
    ngx_hash_key_t  *first, *second;

    first = (ngx_hash_key_t *) one;
    second = (ngx_hash_key_t *) two;

    return ngx_dns_strcmp(first->key.data, second->key.data);
}


static char *
ngx_http_map(ngx_conf_t *cf, ngx_command_t *dummy, void *conf)
{
    u_char                            *data;
    size_t                             len;
    ngx_int_t                          rv;
    ngx_str_t                         *value, v;
    ngx_uint_t                         i, key;
    ngx_http_map_conf_ctx_t           *ctx;
    ngx_http_complex_value_t           cv, *cvp;
    ngx_http_variable_value_t         *var, **vp;
    ngx_http_compile_complex_value_t   ccv;

    ctx = cf->ctx;

    value = cf->args->elts;

    if (cf->args->nelts == 1
        && ngx_strcmp(value[0].data, "hostnames") == 0)
    {
        ctx->hostnames = 1;
        return NGX_CONF_OK;
    }

    if (cf->args->nelts == 1
        && ngx_strcmp(value[0].data, "volatile") == 0)
    {
        ctx->no_cacheable = 1;
        return NGX_CONF_OK;
    }

    if (cf->args->nelts != 2) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "invalid number of the map parameters");
        return NGX_CONF_ERROR;
    }

    if (ngx_strcmp(value[0].data, "include") == 0) {
        return ngx_conf_include(cf, dummy, conf);
    }

    key = 0;

    for (i = 0; i < value[1].len; i++) {
        key = ngx_hash(key, value[1].data[i]);
    }

    key %= ctx->keys.hsize;

    vp = ctx->values_hash[key].elts;

    if (vp) {
        for (i = 0; i < ctx->values_hash[key].nelts; i++) {

            if (vp[i]->valid) {
                data = vp[i]->data;
                len = vp[i]->len;

            } else {
                cvp = (ngx_http_complex_value_t *) vp[i]->data;
                data = cvp->value.data;
                len = cvp->value.len;
            }

            if (value[1].len != len) {
                continue;
            }

            if (ngx_strncmp(value[1].data, data, len) == 0) {
                var = vp[i];
                goto found;
            }
        }

    } else {
        if (ngx_array_init(&ctx->values_hash[key], cf->pool, 4,
                           sizeof(ngx_http_variable_value_t *))
            != NGX_OK)
        {
            return NGX_CONF_ERROR;
        }
    }

    var = ngx_palloc(ctx->keys.pool, sizeof(ngx_http_variable_value_t));
    if (var == NULL) {
        return NGX_CONF_ERROR;
    }

    v.len = value[1].len;
    v.data = ngx_pstrdup(ctx->keys.pool, &value[1]);
    if (v.data == NULL) {
        return NGX_CONF_ERROR;
    }

    ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));

    ccv.cf = ctx->cf;
    ccv.value = &v;
    ccv.complex_value = &cv;

    if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    if (cv.lengths != NULL) {
        cvp = ngx_palloc(ctx->keys.pool, sizeof(ngx_http_complex_value_t));
        if (cvp == NULL) {
            return NGX_CONF_ERROR;
        }

        *cvp = cv;

        var->len = 0;
        var->data = (u_char *) cvp;
        var->valid = 0;

    } else {
        var->len = v.len;
        var->data = v.data;
        var->valid = 1;
    }

    var->no_cacheable = 0;
    var->not_found = 0;

    vp = ngx_array_push(&ctx->values_hash[key]);
    if (vp == NULL) {
        return NGX_CONF_ERROR;
    }

    *vp = var;

found:

    if (ngx_strcmp(value[0].data, "default") == 0) {

        if (ctx->default_value) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "duplicate default map parameter");
            return NGX_CONF_ERROR;
        }

        ctx->default_value = var;

        return NGX_CONF_OK;
    }

#if (NGX_PCRE)

    if (value[0].len && value[0].data[0] == '~') {
        ngx_regex_compile_t    rc;
        ngx_http_map_regex_t  *regex;
        u_char                 errstr[NGX_MAX_CONF_ERRSTR];

        regex = ngx_array_push(&ctx->regexes);
        if (regex == NULL) {
            return NGX_CONF_ERROR;
        }

        value[0].len--;
        value[0].data++;

        ngx_memzero(&rc, sizeof(ngx_regex_compile_t));

        if (value[0].data[0] == '*') {
            value[0].len--;
            value[0].data++;
            rc.options = NGX_REGEX_CASELESS;
        }

        rc.pattern = value[0];
        rc.err.len = NGX_MAX_CONF_ERRSTR;
        rc.err.data = errstr;

        regex->regex = ngx_http_regex_compile(ctx->cf, &rc);
        if (regex->regex == NULL) {
            return NGX_CONF_ERROR;
        }

        regex->value = var;

        return NGX_CONF_OK;
    }

#endif

    if (value[0].len && value[0].data[0] == '\\') {
        value[0].len--;
        value[0].data++;
    }

    rv = ngx_hash_add_key(&ctx->keys, &value[0], var,
                          (ctx->hostnames) ? NGX_HASH_WILDCARD_KEY : 0);

    if (rv == NGX_OK) {
        return NGX_CONF_OK;
    }

    if (rv == NGX_DECLINED) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "invalid hostname or wildcard \"%V\"", &value[0]);
    }

    if (rv == NGX_BUSY) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "conflicting parameter \"%V\"", &value[0]);
    }

    return NGX_CONF_ERROR;

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    ngx_uint_t                  hash_max_size;
    ngx_uint_t                  hash_bucket_size;
} ngx_http_map_conf_t;


typedef struct {
    ngx_hash_keys_arrays_t      keys;

    ngx_array_t                *values_hash;
#if (NGX_PCRE)
    ngx_array_t                 regexes;
#endif

    ngx_http_variable_value_t  *default_value;
    ngx_conf_t                 *cf;
    unsigned                    hostnames:1;
    unsigned                    no_cacheable:1;
} ngx_http_map_conf_ctx_t;


typedef struct {
    ngx_http_map_t              map;
    ngx_http_complex_value_t    value;
    ngx_http_variable_value_t  *default_value;
    ngx_uint_t                  hostnames;      /* unsigned  hostnames:1 */
} ngx_http_map_ctx_t;


    const void *two);
static void *ngx_http_map_create_conf(ngx_conf_t *cf);
static char *ngx_http_map_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_http_map(ngx_conf_t *cf, ngx_command_t *dummy, void *conf);


static ngx_command_t  ngx_http_map_commands[] = {

    { ngx_string("map"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_TAKE2,
      ngx_http_map_block,
      NGX_HTTP_MAIN_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("map_hash_max_size"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_MAIN_CONF_OFFSET,
      offsetof(ngx_http_map_conf_t, hash_max_size),
      NULL },

    { ngx_string("map_hash_bucket_size"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_MAIN_CONF_OFFSET,
      offsetof(ngx_http_map_conf_t, hash_bucket_size),
      NULL },

      ngx_null_command
};


    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    ngx_http_map_create_conf,              /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


    NGX_MODULE_V1,
    &ngx_http_map_module_ctx,              /* module context */
    ngx_http_map_commands,                 /* module directives */
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








ngx_http_map_cmp_dns_wildcards(const void *one, const void *two)
{
    ngx_hash_key_t  *first, *second;

    first = (ngx_hash_key_t *) one;
    second = (ngx_hash_key_t *) two;

    return ngx_dns_strcmp(first->key.data, second->key.data);
}


static char *
ngx_http_map(ngx_conf_t *cf, ngx_command_t *dummy, void *conf)
{
    u_char                            *data;
    size_t                             len;
    ngx_int_t                          rv;
    ngx_str_t                         *value, v;
    ngx_uint_t                         i, key;
    ngx_http_map_conf_ctx_t           *ctx;
    ngx_http_complex_value_t           cv, *cvp;
    ngx_http_variable_value_t         *var, **vp;
    ngx_http_compile_complex_value_t   ccv;

    ctx = cf->ctx;

    value = cf->args->elts;

    if (cf->args->nelts == 1
        && ngx_strcmp(value[0].data, "hostnames") == 0)
    {
        ctx->hostnames = 1;
        return NGX_CONF_OK;
    }

    if (cf->args->nelts == 1
        && ngx_strcmp(value[0].data, "volatile") == 0)
    {
        ctx->no_cacheable = 1;
        return NGX_CONF_OK;
    }

    if (cf->args->nelts != 2) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "invalid number of the map parameters");
        return NGX_CONF_ERROR;
    }

    if (ngx_strcmp(value[0].data, "include") == 0) {
        return ngx_conf_include(cf, dummy, conf);
    }

    key = 0;

    for (i = 0; i < value[1].len; i++) {
        key = ngx_hash(key, value[1].data[i]);
    }

    key %= ctx->keys.hsize;

    vp = ctx->values_hash[key].elts;

    if (vp) {
        for (i = 0; i < ctx->values_hash[key].nelts; i++) {

            if (vp[i]->valid) {
                data = vp[i]->data;
                len = vp[i]->len;

            } else {
                cvp = (ngx_http_complex_value_t *) vp[i]->data;
                data = cvp->value.data;
                len = cvp->value.len;
            }

            if (value[1].len != len) {
                continue;
            }

            if (ngx_strncmp(value[1].data, data, len) == 0) {
                var = vp[i];
                goto found;
            }
        }

    } else {
        if (ngx_array_init(&ctx->values_hash[key], cf->pool, 4,
                           sizeof(ngx_http_variable_value_t *))
            != NGX_OK)
        {
            return NGX_CONF_ERROR;
        }
    }

    var = ngx_palloc(ctx->keys.pool, sizeof(ngx_http_variable_value_t));
    if (var == NULL) {
        return NGX_CONF_ERROR;
    }

    v.len = value[1].len;
    v.data = ngx_pstrdup(ctx->keys.pool, &value[1]);
    if (v.data == NULL) {
        return NGX_CONF_ERROR;
    }

    ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));

    ccv.cf = ctx->cf;
    ccv.value = &v;
    ccv.complex_value = &cv;

    if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    if (cv.lengths != NULL) {
        cvp = ngx_palloc(ctx->keys.pool, sizeof(ngx_http_complex_value_t));
        if (cvp == NULL) {
            return NGX_CONF_ERROR;
        }

        *cvp = cv;

        var->len = 0;
        var->data = (u_char *) cvp;
        var->valid = 0;

    } else {
        var->len = v.len;
        var->data = v.data;
        var->valid = 1;
    }

    var->no_cacheable = 0;
    var->not_found = 0;

    vp = ngx_array_push(&ctx->values_hash[key]);
    if (vp == NULL) {
        return NGX_CONF_ERROR;
    }

    *vp = var;

found:

    if (ngx_strcmp(value[0].data, "default") == 0) {

        if (ctx->default_value) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "duplicate default map parameter");
            return NGX_CONF_ERROR;
        }

        ctx->default_value = var;

        return NGX_CONF_OK;
    }

#if (NGX_PCRE)

    if (value[0].len && value[0].data[0] == '~') {
        ngx_regex_compile_t    rc;
        ngx_http_map_regex_t  *regex;
        u_char                 errstr[NGX_MAX_CONF_ERRSTR];

        regex = ngx_array_push(&ctx->regexes);
        if (regex == NULL) {
            return NGX_CONF_ERROR;
        }

        value[0].len--;
        value[0].data++;

        ngx_memzero(&rc, sizeof(ngx_regex_compile_t));

        if (value[0].data[0] == '*') {
            value[0].len--;
            value[0].data++;
            rc.options = NGX_REGEX_CASELESS;
        }

        rc.pattern = value[0];
        rc.err.len = NGX_MAX_CONF_ERRSTR;
        rc.err.data = errstr;

        regex->regex = ngx_http_regex_compile(ctx->cf, &rc);
        if (regex->regex == NULL) {
            return NGX_CONF_ERROR;
        }

        regex->value = var;

        return NGX_CONF_OK;
    }

#endif

    if (value[0].len && value[0].data[0] == '\\') {
        value[0].len--;
        value[0].data++;
    }

    rv = ngx_hash_add_key(&ctx->keys, &value[0], var,
                          (ctx->hostnames) ? NGX_HASH_WILDCARD_KEY : 0);

    if (rv == NGX_OK) {
        return NGX_CONF_OK;
    }

    if (rv == NGX_DECLINED) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "invalid hostname or wildcard \"%V\"", &value[0]);
    }

    if (rv == NGX_BUSY) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "conflicting parameter \"%V\"", &value[0]);
    }

    return NGX_CONF_ERROR;
}
