
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_regex.h>

typedef struct {
    ngx_flag_t  pcre_jit;
} ngx_regex_conf_t;


static void * ngx_libc_cdecl ngx_regex_malloc(size_t size);
static void ngx_libc_cdecl ngx_regex_free(void *p);
#if (NGX_HAVE_PCRE_JIT)
static void ngx_pcre_free_studies(void *data);
#endif

static ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

static void *ngx_regex_create_conf(ngx_cycle_t *cycle);
static char *ngx_regex_init_conf(ngx_cycle_t *cycle, void *conf);

static char *ngx_regex_pcre_jit(ngx_conf_t *cf, void *post, void *data);



    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};






static ngx_pool_t  *ngx_pcre_pool;
static ngx_list_t  *ngx_pcre_studies;




static ngx_inline void
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_pcre_pool = pool;
}


static ngx_inline void
ngx_regex_malloc_done(void)
{
    ngx_pcre_pool = NULL;
}










#if (NGX_HAVE_PCRE_JIT)

static void
ngx_pcre_free_studies(void *data)
{
    ngx_list_t *studies = data;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &studies->part;
    elts = part->elts;

    for (i = 0; /* void */ ; i++) {

        if (i >= part->nelts) {
            if (part->next == NULL) {
                break;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

        if (elts[i].regex->extra != NULL) {
            pcre_free_study(elts[i].regex->extra);
        }
    }
}

#endif


ngx_int_t ngx_regex_module_init_sense_1(int *ngx_regex_module_init_sense_1_ret, int *opt, const char *errstr, ngx_uint_t *i, ngx_list_part_t *part, ngx_regex_elt_t *elts, ngx_cycle_t *cycle)
{
  *opt = 0;
  ngx_regex_malloc_init(cycle->pool);
  part = &ngx_pcre_studies->part;
  for (*i = 0;; (*i)++)
  {
    if ((*i) >= part->nelts)
    {
      if (part->next == 0)
      {
        break;
      }
      part = part->next;
      elts = part->elts;
      *i = 0;
    }
    elts[*i].regex->extra = pcre_study(elts[*i].regex->code, *opt, &errstr);
    if (errstr != 0)
    {
      if (cycle->log->log_level >= 2)
        ngx_log_error_core(2, cycle->log, 0, "pcre_study() failed: %s in \"%s\"", errstr, elts[*i].name);
    }
  }

  ngx_regex_malloc_done();
  ngx_pcre_studies = 0;
  {
    *ngx_regex_module_init_sense_1_ret = 1;
    return 0;
  }
}







