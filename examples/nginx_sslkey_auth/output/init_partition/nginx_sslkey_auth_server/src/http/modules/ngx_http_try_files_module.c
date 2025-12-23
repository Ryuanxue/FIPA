
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


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


ngx_int_t ngx_http_try_files_handler_sense_1(int *ngx_http_try_files_handler_sense_1_ret, size_t *len, size_t *root, size_t *alias, size_t *reserve, size_t *allocated, u_char *p, u_char *name, ngx_str_t path, ngx_str_t args, ngx_uint_t *test_dir, ngx_http_try_file_t *tf, ngx_open_file_info_t of, ngx_http_script_code_pt *code, ngx_http_script_engine_t e, ngx_http_core_loc_conf_t *clcf, ngx_http_script_len_code_pt *lcode, ngx_http_try_files_loc_conf_t *tlcf, ngx_http_request_t *r)
{
  tlcf = r->loc_conf[ngx_http_try_files_module.ctx_index];
  if (tlcf->try_files == 0)
  {
    {
      *ngx_http_try_files_handler_sense_1_ret = 1;
      return -5;
    }
  }
  *allocated = 0;
  *root = 0;
  name = 0;
  path.data = 0;
  tf = tlcf->try_files;
  clcf = r->loc_conf[ngx_http_core_module.ctx_index];
  *alias = clcf->*alias;
  for (;;)
  {
    if (tf->lengths)
    {
      (void) memset(&e, 0, sizeof(ngx_http_script_engine_t));
      e.ip = tf->lengths->elts;
      e.request = r;
      *len = 1;
      while (*((uintptr_t *) e.ip))
      {
        *lcode = *((ngx_http_script_len_code_pt *) e.ip);
        *len += (*lcode)(&e);
      }

    }
    else
    {
      *len = tf->name.*len;
    }
    if (!(*alias))
    {
      *reserve = ((*len) > r->uri.*len) ? ((*len) - r->uri.*len) : (0);
    }
    else
      if ((*alias) == 9223372036854775807LL)
    {
      *reserve = *len;
    }
    else
    {
      *reserve = ((*len) > (r->uri.*len - (*alias))) ? ((*len) - (r->uri.*len - (*alias))) : (0);
    }
    if (((*reserve) > (*allocated)) || (!(*allocated)))
    {
      *allocated = (*reserve) + 16;
      if (ngx_http_map_uri_to_path(r, &path, &(*root), *allocated) == 0)
      {
        {
          *ngx_http_try_files_handler_sense_1_ret = 1;
          return 500;
        }
      }
      name = path.data + (*root);
    }
    if (tf->values == 0)
    {
      (void) memcpy(name, tf->name.data, tf->name.*len);
      path.*len = ((name + tf->name.*len) - 1) - path.data;
    }
    else
    {
      e.ip = tf->values->elts;
      e.pos = name;
      e.flushed = 1;
      while (*((uintptr_t *) e.ip))
      {
        *code = *((ngx_http_script_code_pt *) e.ip);
        (*code)((ngx_http_script_engine_t *) (&e));
      }

      path.*len = e.pos - path.data;
      *e.pos = '\0';
      if (((*alias) && ((*alias) != 9223372036854775807LL)) && (strncmp((const char *) name, (const char *) r->uri.data, *alias) == 0))
      {
        (void) memmove(name, name + (*alias), (*len) - (*alias));
        path.*len -= *alias;
      }
    }
    *test_dir = tf->*test_dir;
    tf++;
    ;
    if ((tf->lengths == 0) && (tf->name.*len == 0))
    {
      if (tf->*code)
      {
        {
          *ngx_http_try_files_handler_sense_1_ret = 1;
          return tf->*code;
        }
      }
      path.*len -= *root;
      path.data += *root;
      if (path.data[0] == '@')
      {
        (void) ngx_http_named_location(r, &path);
      }
      else
      {
        ngx_http_split_args(r, &path, &args);
        (void) ngx_http_internal_redirect(r, &path, &args);
      }
      ngx_http_finalize_request(r, -4);
      {
        *ngx_http_try_files_handler_sense_1_ret = 1;
        return -4;
      }
    }
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
        *ngx_http_try_files_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (ngx_open_cached_file(clcf->open_file_cache, &path, &of, r->pool) != 0)
    {
      if (of.err == 0)
      {
        {
          *ngx_http_try_files_handler_sense_1_ret = 1;
          return 500;
        }
      }
      if (((of.err != ENOENT) && (of.err != ENOTDIR)) && (of.err != ENAMETOOLONG))
      {
        if (r->connection->log->log_level >= 3)
          ngx_log_error_core(3, r->connection->log, of.err, "%s \"%s\" failed", of.failed, path.data);
      }
      continue;
    }
    if (of.is_dir != (*test_dir))
    {
      continue;
    }
    path.*len -= *root;
    path.data += *root;
    if (!(*alias))
    {
      r->uri = path;
    }
    else
      if ((*alias) == 9223372036854775807LL)
    {
      if (!(*test_dir))
      {
        r->uri = path;
        r->add_uri_to_alias = 1;
      }
    }
    else
    {
      name = r->uri.data;
      r->uri.*len = (*alias) + path.*len;
      r->uri.data = ngx_pnalloc(r->pool, r->uri.*len);
      if (r->uri.data == 0)
      {
        r->uri.*len = 0;
        {
          *ngx_http_try_files_handler_sense_1_ret = 1;
          return 500;
        }
      }
      p = ((u_char *) memcpy(r->uri.data, name, *alias)) + (*alias);
      (void) memcpy(p, path.data, path.*len);
    }
    ngx_http_set_exten(r);
    ;
    {
      *ngx_http_try_files_handler_sense_1_ret = 1;
      return -5;
    }
  }

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


ngx_int_t ngx_http_try_files_init_sense_1(int *ngx_http_try_files_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_try_files_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_try_files_handler;
  {
    *ngx_http_try_files_init_sense_1_ret = 1;
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


ngx_int_t ngx_http_try_files_handler_sense_1(int *ngx_http_try_files_handler_sense_1_ret, size_t *len, size_t *root, size_t *alias, size_t *reserve, size_t *allocated, u_char *p, u_char *name, ngx_str_t path, ngx_str_t args, ngx_uint_t *test_dir, ngx_http_try_file_t *tf, ngx_open_file_info_t of, ngx_http_script_code_pt *code, ngx_http_script_engine_t e, ngx_http_core_loc_conf_t *clcf, ngx_http_script_len_code_pt *lcode, ngx_http_try_files_loc_conf_t *tlcf, ngx_http_request_t *r)
{
  tlcf = r->loc_conf[ngx_http_try_files_module.ctx_index];
  if (tlcf->try_files == 0)
  {
    {
      *ngx_http_try_files_handler_sense_1_ret = 1;
      return -5;
    }
  }
  *allocated = 0;
  *root = 0;
  name = 0;
  path.data = 0;
  tf = tlcf->try_files;
  clcf = r->loc_conf[ngx_http_core_module.ctx_index];
  *alias = clcf->*alias;
  for (;;)
  {
    if (tf->lengths)
    {
      (void) memset(&e, 0, sizeof(ngx_http_script_engine_t));
      e.ip = tf->lengths->elts;
      e.request = r;
      *len = 1;
      while (*((uintptr_t *) e.ip))
      {
        *lcode = *((ngx_http_script_len_code_pt *) e.ip);
        *len += (*lcode)(&e);
      }

    }
    else
    {
      *len = tf->name.*len;
    }
    if (!(*alias))
    {
      *reserve = ((*len) > r->uri.*len) ? ((*len) - r->uri.*len) : (0);
    }
    else
      if ((*alias) == 9223372036854775807LL)
    {
      *reserve = *len;
    }
    else
    {
      *reserve = ((*len) > (r->uri.*len - (*alias))) ? ((*len) - (r->uri.*len - (*alias))) : (0);
    }
    if (((*reserve) > (*allocated)) || (!(*allocated)))
    {
      *allocated = (*reserve) + 16;
      if (ngx_http_map_uri_to_path(r, &path, &(*root), *allocated) == 0)
      {
        {
          *ngx_http_try_files_handler_sense_1_ret = 1;
          return 500;
        }
      }
      name = path.data + (*root);
    }
    if (tf->values == 0)
    {
      (void) memcpy(name, tf->name.data, tf->name.*len);
      path.*len = ((name + tf->name.*len) - 1) - path.data;
    }
    else
    {
      e.ip = tf->values->elts;
      e.pos = name;
      e.flushed = 1;
      while (*((uintptr_t *) e.ip))
      {
        *code = *((ngx_http_script_code_pt *) e.ip);
        (*code)((ngx_http_script_engine_t *) (&e));
      }

      path.*len = e.pos - path.data;
      *e.pos = '\0';
      if (((*alias) && ((*alias) != 9223372036854775807LL)) && (strncmp((const char *) name, (const char *) r->uri.data, *alias) == 0))
      {
        (void) memmove(name, name + (*alias), (*len) - (*alias));
        path.*len -= *alias;
      }
    }
    *test_dir = tf->*test_dir;
    tf++;
    ;
    if ((tf->lengths == 0) && (tf->name.*len == 0))
    {
      if (tf->*code)
      {
        {
          *ngx_http_try_files_handler_sense_1_ret = 1;
          return tf->*code;
        }
      }
      path.*len -= *root;
      path.data += *root;
      if (path.data[0] == '@')
      {
        (void) ngx_http_named_location(r, &path);
      }
      else
      {
        ngx_http_split_args(r, &path, &args);
        (void) ngx_http_internal_redirect(r, &path, &args);
      }
      ngx_http_finalize_request(r, -4);
      {
        *ngx_http_try_files_handler_sense_1_ret = 1;
        return -4;
      }
    }
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
        *ngx_http_try_files_handler_sense_1_ret = 1;
        return 500;
      }
    }
    if (ngx_open_cached_file(clcf->open_file_cache, &path, &of, r->pool) != 0)
    {
      if (of.err == 0)
      {
        {
          *ngx_http_try_files_handler_sense_1_ret = 1;
          return 500;
        }
      }
      if (((of.err != ENOENT) && (of.err != ENOTDIR)) && (of.err != ENAMETOOLONG))
      {
        if (r->connection->log->log_level >= 3)
          ngx_log_error_core(3, r->connection->log, of.err, "%s \"%s\" failed", of.failed, path.data);
      }
      continue;
    }
    if (of.is_dir != (*test_dir))
    {
      continue;
    }
    path.*len -= *root;
    path.data += *root;
    if (!(*alias))
    {
      r->uri = path;
    }
    else
      if ((*alias) == 9223372036854775807LL)
    {
      if (!(*test_dir))
      {
        r->uri = path;
        r->add_uri_to_alias = 1;
      }
    }
    else
    {
      name = r->uri.data;
      r->uri.*len = (*alias) + path.*len;
      r->uri.data = ngx_pnalloc(r->pool, r->uri.*len);
      if (r->uri.data == 0)
      {
        r->uri.*len = 0;
        {
          *ngx_http_try_files_handler_sense_1_ret = 1;
          return 500;
        }
      }
      p = ((u_char *) memcpy(r->uri.data, name, *alias)) + (*alias);
      (void) memcpy(p, path.data, path.*len);
    }
    ngx_http_set_exten(r);
    ;
    {
      *ngx_http_try_files_handler_sense_1_ret = 1;
      return -5;
    }
  }

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


ngx_int_t ngx_http_try_files_init_sense_1(int *ngx_http_try_files_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_try_files_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_try_files_handler;
  {
    *ngx_http_try_files_init_sense_1_ret = 1;
    return 0;
  }
}

