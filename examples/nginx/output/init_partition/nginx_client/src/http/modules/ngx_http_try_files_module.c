
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "nginx_rpc_wrapper.h"


typedef struct {
    ngx_array_t           *lengths;
    ngx_array_t           *values;
    ngx_str_t              name;

    unsigned               code:10;
    unsigned               test_dir:1;
} ngx_http_try_file_t;


typedef struct {
    ngx_http_try_file_t   *try_files;
} ngx_http_try_files_loc_conf_t;


static ngx_int_t ngx_http_try_files_handler(ngx_http_request_t *r);
static char *ngx_http_try_files(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static void *ngx_http_try_files_create_loc_conf(ngx_conf_t *cf);
static ngx_int_t ngx_http_try_files_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_try_files_commands[] = {

    { ngx_string("try_files"),
      NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_2MORE,
      ngx_http_try_files,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};


static ngx_http_module_t  ngx_http_try_files_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_try_files_init,               /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_try_files_create_loc_conf,    /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_try_files_module = {
    NGX_MODULE_V1,
    &ngx_http_try_files_module_ctx,        /* module context */
    ngx_http_try_files_commands,           /* module directives */
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


static ngx_int_t ngx_http_try_files_handler(ngx_http_request_t *r)
{
  size_t len;
  size_t root;
  size_t alias;
  size_t reserve;
  size_t allocated;
  u_char *p;
  u_char *name;
  ngx_str_t path;
  ngx_str_t args;
  ngx_uint_t test_dir;
  ngx_http_try_file_t *tf;
  ngx_open_file_info_t of;
  ngx_http_script_code_pt code;
  ngx_http_script_engine_t e;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_script_len_code_pt lcode;
  ngx_http_try_files_loc_conf_t *tlcf;
  {
    int ngx_http_try_files_handler_sense_1_ret = 0;
    ngx_int_t ngx_http_try_files_handler_sense_1_return = ngx_http_try_files_handler_sense_1_wrapper(&ngx_http_try_files_handler_sense_1_ret, &len, &root, &alias, &reserve, &allocated, p, name, path, args, &test_dir, tf, of, &code, e, clcf, &lcode, tlcf, r);
    if (ngx_http_try_files_handler_sense_1_ret)
      return ngx_http_try_files_handler_sense_1_return;
  }
  ;
}




static char *
ngx_http_try_files(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_try_files_loc_conf_t *tlcf = conf;

    ngx_str_t                  *value;
    ngx_int_t                   code;
    ngx_uint_t                  i, n;
    ngx_http_try_file_t        *tf;
    ngx_http_script_compile_t   sc;

    if (tlcf->try_files) {
        return "is duplicate";
    }

    tf = ngx_pcalloc(cf->pool, cf->args->nelts * sizeof(ngx_http_try_file_t));
    if (tf == NULL) {
        return NGX_CONF_ERROR;
    }

    tlcf->try_files = tf;

    value = cf->args->elts;

    for (i = 0; i < cf->args->nelts - 1; i++) {

        tf[i].name = value[i + 1];

        if (tf[i].name.len > 0
            && tf[i].name.data[tf[i].name.len - 1] == '/'
            && i + 2 < cf->args->nelts)
        {
            tf[i].test_dir = 1;
            tf[i].name.len--;
            tf[i].name.data[tf[i].name.len] = '\0';
        }

        n = ngx_http_script_variables_count(&tf[i].name);

        if (n) {
            ngx_memzero(&sc, sizeof(ngx_http_script_compile_t));

            sc.cf = cf;
            sc.source = &tf[i].name;
            sc.lengths = &tf[i].lengths;
            sc.values = &tf[i].values;
            sc.variables = n;
            sc.complete_lengths = 1;
            sc.complete_values = 1;

            if (ngx_http_script_compile(&sc) != NGX_OK) {
                return NGX_CONF_ERROR;
            }

        } else {
            /* add trailing '\0' to length */
            tf[i].name.len++;
        }
    }

    if (tf[i - 1].name.data[0] == '=') {

        code = ngx_atoi(tf[i - 1].name.data + 1, tf[i - 1].name.len - 2);

        if (code == NGX_ERROR || code > 999) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid code \"%*s\"",
                               tf[i - 1].name.len - 1, tf[i - 1].name.data);
            return NGX_CONF_ERROR;
        }

        tf[i].code = code;
    }

    return NGX_CONF_OK;
}


static void *
ngx_http_try_files_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_try_files_loc_conf_t  *tlcf;

    tlcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_try_files_loc_conf_t));
    if (tlcf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     tlcf->try_files = NULL;
     */

    return tlcf;
}


static ngx_int_t
ngx_http_try_files_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt        *h;
    ngx_http_core_main_conf_t  *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_try_files_handler;

    return NGX_OK;
}
