
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


static ngx_int_t ngx_http_static_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_static_init(ngx_conf_t *cf);


    NULL,                                  /* preconfiguration */
    ngx_http_static_init,                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


    NGX_MODULE_V1,
    &ngx_http_static_module_ctx,           /* module context */
    NULL,                                  /* module directives */
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


ngx_int_t ngx_http_static_handler_sense_1(int *ngx_http_static_handler_sense_1_ret, u_char *last, u_char *location, size_t root, size_t *len, ngx_str_t path, ngx_int_t *rc, ngx_uint_t *level, ngx_log_t *log, ngx_buf_t *b, ngx_chain_t out, ngx_open_file_info_t of, ngx_http_core_loc_conf_t *clcf, ngx_http_request_t *r)
{
  if (!(r->method & ((0x0002 | 0x0004) | 0x0008)))
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return 405;
    }
  }
  if (r->uri.data[r->uri.*len - 1] == '/')
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return -5;
    }
  }
  log = r->connection->log;
  last = ngx_http_map_uri_to_path(r, &path, &root, 0);
  if (last == 0)
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return 500;
    }
  }
  path.*len = last - path.data;
  clcf = r->loc_conf[ngx_http_core_module.ctx_index];
  (void) memset(&of, 0, sizeof(ngx_open_file_info_t));
  of.read_ahead = clcf->read_ahead;
  of.directio = clcf->directio;
  of.valid = clcf->open_file_cache_valid;
  of.min_uses = clcf->open_file_cache_min_uses;
  of.errors = clcf->open_file_cache_errors;
  of.events = clcf->open_file_cache_events;
  if (ngx_http_set_disable_symlinks(r, clcf, &path, &of) != 0)
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return 500;
    }
  }
  if (ngx_open_cached_file(clcf->open_file_cache, &path, &of, r->pool) != 0)
  {
    switch (of.err)
    {
      case 0:
      {
        *ngx_http_static_handler_sense_1_ret = 1;
        return 500;
      }

      case ENOENT:

      case ENOTDIR:

      case ENAMETOOLONG:
        *level = 4;
        *rc = 404;
        break;

      case EACCES:

      case EMLINK:

      case ELOOP:
        *level = 4;
        *rc = 403;
        break;

      default:
        *level = 3;
        *rc = 500;
        break;

    }

    if (((*rc) != 404) || clcf->log_not_found)
    {
      if (log->log_level >= (*level))
        ngx_log_error_core(*level, log, of.err, "%s \"%s\" failed", of.failed, path.data);
    }
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return *rc;
    }
  }
  r->root_tested = !r->error_page;
  if (of.is_dir)
  {
    ;
    if (r->headers_out.location)
    {
      r->headers_out.location->hash = 0;
      r->headers_out.location = 0;
    }
    ;
    r->headers_out.location = ngx_list_push(&r->headers_out.headers);
    if (r->headers_out.location == 0)
    {
      {
        *ngx_http_static_handler_sense_1_ret = 1;
        return 500;
      }
    }
    *len = r->uri.*len + 1;
    if (((!clcf->alias) && (clcf->root_lengths == 0)) && (r->args.*len == 0))
    {
      location = path.data + clcf->root.*len;
      *last = '/';
    }
    else
    {
      if (r->args.*len)
      {
        *len += r->args.*len + 1;
      }
      location = ngx_pnalloc(r->pool, *len);
      if (location == 0)
      {
        if (r->headers_out.location)
        {
          r->headers_out.location->hash = 0;
          r->headers_out.location = 0;
        }
        ;
        {
          *ngx_http_static_handler_sense_1_ret = 1;
          return 500;
        }
      }
      last = ((u_char *) memcpy(location, r->uri.data, r->uri.*len)) + r->uri.*len;
      *last = '/';
      if (r->args.*len)
      {
        *(++last) = '?';
        (void) memcpy(++last, r->args.data, r->args.*len);
      }
    }
    r->headers_out.location->hash = 1;
    (&r->headers_out.location->key)->*len = (sizeof("Location")) - 1;
    (&r->headers_out.location->key)->data = (u_char *) "Location";
    r->headers_out.location->value.*len = *len;
    r->headers_out.location->value.data = location;
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return 301;
    }
  }
  if (!of.is_file)
  {
    if (log->log_level >= 3)
      ngx_log_error_core(3, log, 0, "\"%s\" is not a regular file", path.data);
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return 404;
    }
  }
  if (r->method == 0x0008)
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return 405;
    }
  }
  *rc = ngx_http_discard_request_body(r);
  if ((*rc) != 0)
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return *rc;
    }
  }
  log->action = "sending response to client";
  r->headers_out.status = 200;
  r->headers_out.content_length_n = of.size;
  r->headers_out.last_modified_time = of.mtime;
  if (ngx_http_set_etag(r) != 0)
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return 500;
    }
  }
  if (ngx_http_set_content_type(r) != 0)
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return 500;
    }
  }
  if ((r != r->main) && (of.size == 0))
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return ngx_http_send_header(r);
    }
  }
  r->allow_ranges = 1;
  b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
  if (b == 0)
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return 500;
    }
  }
  b->file = ngx_pcalloc(r->pool, sizeof(ngx_file_t));
  if (b->file == 0)
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return 500;
    }
  }
  *rc = ngx_http_send_header(r);
  if ((((*rc) == (-1)) || ((*rc) > 0)) || r->header_only)
  {
    {
      *ngx_http_static_handler_sense_1_ret = 1;
      return *rc;
    }
  }
  b->file_pos = 0;
  b->file_last = of.size;
  b->in_file = (b->file_last) ? (1) : (0);
  b->last_buf = (r == r->main) ? (1) : (0);
  b->last_in_chain = 1;
  b->file->fd = of.fd;
  b->file->name = path;
  b->file->log = log;
  b->file->directio = of.is_directio;
  out.buf = b;
  out.next = 0;
  {
    *ngx_http_static_handler_sense_1_ret = 1;
    return ngx_http_output_filter(r, &out);
  }
}



ngx_int_t ngx_http_static_init_sense_1(int *ngx_http_static_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
  if (h == 0)
  {
    {
      *ngx_http_static_init_sense_1_ret = 1;
      return -1;
    }
  }
  *h = ngx_http_static_handler;
  {
    *ngx_http_static_init_sense_1_ret = 1;
    return 0;
  }
}

