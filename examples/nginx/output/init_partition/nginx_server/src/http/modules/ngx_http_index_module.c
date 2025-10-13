
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    ngx_str_t                name;
    ngx_array_t             *lengths;
    ngx_array_t             *values;
} ngx_http_index_t;


typedef struct {
    ngx_array_t             *indices;    /* array of ngx_http_index_t */
    size_t                   max_index_len;
} ngx_http_index_loc_conf_t;


#define NGX_HTTP_DEFAULT_INDEX   "index.html"


static ngx_int_t ngx_http_index_test_dir(ngx_http_request_t *r,
    ngx_http_core_loc_conf_t *clcf, u_char *path, u_char *last);
static ngx_int_t ngx_http_index_error(ngx_http_request_t *r,
    ngx_http_core_loc_conf_t *clcf, u_char *file, ngx_err_t err);

static ngx_int_t ngx_http_index_init(ngx_conf_t *cf);
static void *ngx_http_index_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_index_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);
static char *ngx_http_index_set_index(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


static ngx_command_t  ngx_http_index_commands[] = {

    { ngx_string("index"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
      ngx_http_index_set_index,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};


static ngx_http_module_t  ngx_http_index_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_index_init,                   /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_index_create_loc_conf,        /* create location configuration */
    ngx_http_index_merge_loc_conf          /* merge location configuration */
};


ngx_module_t  ngx_http_index_module = {
    NGX_MODULE_V1,
    &ngx_http_index_module_ctx,            /* module context */
    ngx_http_index_commands,               /* module directives */
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
 * Try to open/test the first index file before the test of directory
 * existence because valid requests should prevail over invalid ones.
 * If open()/stat() of a file will fail then stat() of a directory
 * should be faster because kernel may have already cached some data.
 * Besides, Win32 may return ERROR_PATH_NOT_FOUND (NGX_ENOTDIR) at once.
 * Unix has ENOTDIR error; however, it's less helpful than Win32's one:
 * it only indicates that path points to a regular file, not a directory.
 */

ngx_int_t ngx_http_index_handler_sense_1(int *ngx_http_index_handler_sense_1_ret, ngx_http_request_t *r)
{
  u_char *p;
  u_char *name;
  size_t len;
  size_t root;
  size_t reserve;
  size_t allocated;
  ngx_int_t rc;
  ngx_str_t path;
  ngx_str_t uri;
  ngx_uint_t i;
  ngx_uint_t dir_tested;
  ngx_http_index_t *index;
  ngx_open_file_info_t of;
  ngx_http_script_code_pt code;
  ngx_http_script_engine_t e;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_index_loc_conf_t *ilcf;
  ngx_http_script_len_code_pt lcode;
  if (r->uri.data[r->uri.len - 1] != '/')
  {
    {
      *ngx_http_index_handler_sense_1_ret = 1;
      return -5;
    }
  }
  if (!(r->method & ((0x0002 | 0x0004) | 0x0008)))
  {
    {
      *ngx_http_index_handler_sense_1_ret = 1;
      return -5;
    }
  }
  ilcf = r->loc_conf[ngx_http_index_module.ctx_index];
  clcf = r->loc_conf[ngx_http_core_module.ctx_index];
  allocated = 0;
  root = 0;
  dir_tested = 0;
  name = 0;
  path.data = 0;
  index = ilcf->indices->elts;
  for (i = 0; i < ilcf->indices->nelts; i++)
  {
    if (index[i].lengths == 0)
    {
      if (index[i].name.data[0] == '/')
      {
        {
          *ngx_http_index_handler_sense_1_ret = 1;
          return ngx_http_internal_redirect(r, &index[i].name, &r->args);
        }
      }
      reserve = ilcf->max_index_len;
      len = index[i].name.len;
    }
    else
    {
      (void) memset(&e, 0, sizeof(ngx_http_script_engine_t));
      e.ip = index[i].lengths->elts;
      e.request = r;
      e.flushed = 1;
      len = 1;
      while (*((uintptr_t *) e.ip))
      {
        lcode = *((ngx_http_script_len_code_pt *) e.ip);
        len += lcode(&e);
      }

      reserve = len + 16;
    }
    if (reserve > allocated)
    {
      name = ngx_http_map_uri_to_path(r, &path, &root, reserve);
      if (name == 0)
      {
        {
          *ngx_http_index_handler_sense_1_ret = 1;
          return -1;
        }
      }
      allocated = (path.data + path.len) - name;
    }
    if (index[i].values == 0)
    {
      (void) memcpy(name, index[i].name.data, index[i].name.len);
      path.len = ((name + index[i].name.len) - 1) - path.data;
    }
    else
    {
      e.ip = index[i].values->elts;
      e.pos = name;
      while (*((uintptr_t *) e.ip))
      {
        code = *((ngx_http_script_code_pt *) e.ip);
        code((ngx_http_script_engine_t *) (&e));
      }

      if ((*name) == '/')
      {
        uri.len = len - 1;
        uri.data = name;
        {
          *ngx_http_index_handler_sense_1_ret = 1;
          return ngx_http_internal_redirect(r, &uri, &r->args);
        }
      }
      path.len = e.pos - path.data;
      *e.pos = '\0';
    }
    ;
    (void) memset(&of, 0, sizeof(ngx_open_file_info_t));
    of.read_ahead = clcf->read_ahead;
    of.directio = clcf->directio;
    of.valid = clcf->open_file_cache_valid;
    of.min_uses = clcf->open_file_cache_min_uses;
    of.test_only = 1;
    of.errors = clcf->open_file_cache_errors;
    of.events = clcf->open_file_cache_events;
    if (ngx_http_set_disable_symlinks(r, clcf, &path, &of) != 0)
    {
      {
        *ngx_http_index_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (ngx_open_cached_file(clcf->open_file_cache, &path, &of, r->pool) != 0)
    {
      if (of.err == 0)
      {
        {
          *ngx_http_index_handler_sense_1_ret = 1;
          return 500;
        }
      }
      ;
      if ((of.err == EMLINK) || (of.err == ELOOP))
      {
        {
          *ngx_http_index_handler_sense_1_ret = 1;
          return 403;
        }
      }
      if (((of.err == ENOTDIR) || (of.err == ENAMETOOLONG)) || (of.err == EACCES))
      {
        {
          *ngx_http_index_handler_sense_1_ret = 1;
          return ngx_http_index_error(r, clcf, path.data, of.err);
        }
      }
      if (!dir_tested)
      {
        rc = ngx_http_index_test_dir(r, clcf, path.data, name - 1);
        if (rc != 0)
        {
          {
            *ngx_http_index_handler_sense_1_ret = 1;
            return rc;
          }
        }
        dir_tested = 1;
      }
      if (of.err == ENOENT)
      {
        continue;
      }
      if (r->connection->log->log_level >= 3)
        ngx_log_error_core(3, r->connection->log, of.err, "%s \"%s\" failed", of.failed, path.data);
      {
        *ngx_http_index_handler_sense_1_ret = 1;
        return 500;
      }
    }
    uri.len = (r->uri.len + len) - 1;
    if (!clcf->alias)
    {
      uri.data = path.data + root;
    }
    else
    {
      uri.data = ngx_pnalloc(r->pool, uri.len);
      if (uri.data == 0)
      {
        {
          *ngx_http_index_handler_sense_1_ret = 1;
          return 500;
        }
      }
      p = ((u_char *) memcpy(uri.data, r->uri.data, r->uri.len)) + r->uri.len;
      (void) memcpy(p, name, len - 1);
    }
    {
      *ngx_http_index_handler_sense_1_ret = 1;
      return ngx_http_internal_redirect(r, &uri, &r->args);
    }
  }

  {
    *ngx_http_index_handler_sense_1_ret = 1;
    return -5;
  }
}




static ngx_int_t
ngx_http_index_test_dir(ngx_http_request_t *r, ngx_http_core_loc_conf_t *clcf,
    u_char *path, u_char *last)
{
    u_char                c;
    ngx_str_t             dir;
    ngx_open_file_info_t  of;

    c = *last;
    if (c != '/' || path == last) {
        /* "alias" without trailing slash */
        c = *(++last);
    }
    *last = '\0';

    dir.len = last - path;
    dir.data = path;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http index check dir: \"%V\"", &dir);

    ngx_memzero(&of, sizeof(ngx_open_file_info_t));

    of.test_dir = 1;
    of.test_only = 1;
    of.valid = clcf->open_file_cache_valid;
    of.errors = clcf->open_file_cache_errors;

    if (ngx_http_set_disable_symlinks(r, clcf, &dir, &of) != NGX_OK) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    if (ngx_open_cached_file(clcf->open_file_cache, &dir, &of, r->pool)
        != NGX_OK)
    {
        if (of.err) {

#if (NGX_HAVE_OPENAT)
            if (of.err == NGX_EMLINK
                || of.err == NGX_ELOOP)
            {
                return NGX_HTTP_FORBIDDEN;
            }
#endif

            if (of.err == NGX_ENOENT) {
                *last = c;
                return ngx_http_index_error(r, clcf, dir.data, NGX_ENOENT);
            }

            if (of.err == NGX_EACCES) {

                *last = c;

                /*
                 * ngx_http_index_test_dir() is called after the first index
                 * file testing has returned an error distinct from NGX_EACCES.
                 * This means that directory searching is allowed.
                 */

                return NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, r->connection->log, of.err,
                          "%s \"%s\" failed", of.failed, dir.data);
        }

        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    *last = c;

    if (of.is_dir) {
        return NGX_OK;
    }

    ngx_log_error(NGX_LOG_ALERT, r->connection->log, 0,
                  "\"%s\" is not a directory", dir.data);

    return NGX_HTTP_INTERNAL_SERVER_ERROR;
}


static ngx_int_t
ngx_http_index_error(ngx_http_request_t *r, ngx_http_core_loc_conf_t  *clcf,
    u_char *file, ngx_err_t err)
{
    if (err == NGX_EACCES) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, err,
                      "\"%s\" is forbidden", file);

        return NGX_HTTP_FORBIDDEN;
    }

    if (clcf->log_not_found) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, err,
                      "\"%s\" is not found", file);
    }

    return NGX_HTTP_NOT_FOUND;
}











/* TODO: warn about duplicate indices */


