
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


static ngx_int_t ngx_http_script_init_arrays(ngx_http_script_compile_t *sc);
static ngx_int_t ngx_http_script_done(ngx_http_script_compile_t *sc);
static ngx_int_t ngx_http_script_add_copy_code(ngx_http_script_compile_t *sc,
    ngx_str_t *value, ngx_uint_t last);
static ngx_int_t ngx_http_script_add_var_code(ngx_http_script_compile_t *sc,
    ngx_str_t *name);
static ngx_int_t ngx_http_script_add_args_code(ngx_http_script_compile_t *sc);
#if (NGX_PCRE)
static ngx_int_t ngx_http_script_add_capture_code(ngx_http_script_compile_t *sc,
    ngx_uint_t n);
#endif
static ngx_int_t
    ngx_http_script_add_full_name_code(ngx_http_script_compile_t *sc);
static size_t ngx_http_script_full_name_len_code(ngx_http_script_engine_t *e);
static void ngx_http_script_full_name_code(ngx_http_script_engine_t *e);


#define ngx_http_script_exit  (u_char *) &ngx_http_script_exit_code



void
ngx_http_script_flush_complex_value(ngx_http_request_t *r,
    ngx_http_complex_value_t *val)
{
    ngx_uint_t *index;

    index = val->flushes;

    if (index) {
        while (*index != (ngx_uint_t) -1) {

            if (r->variables[*index].no_cacheable) {
                r->variables[*index].valid = 0;
                r->variables[*index].not_found = 0;
            }

            index++;
        }
    }
}


ngx_int_t
ngx_http_complex_value(ngx_http_request_t *r, ngx_http_complex_value_t *val,
    ngx_str_t *value)
{
    size_t                        len;
    ngx_http_script_code_pt       code;
    ngx_http_script_len_code_pt   lcode;
    ngx_http_script_engine_t      e;

    if (val->lengths == NULL) {
        *value = val->value;
        return NGX_OK;
    }

    ngx_http_script_flush_complex_value(r, val);

    ngx_memzero(&e, sizeof(ngx_http_script_engine_t));

    e.ip = val->lengths;
    e.request = r;
    e.flushed = 1;

    len = 0;

    while (*(uintptr_t *) e.ip) {
        lcode = *(ngx_http_script_len_code_pt *) e.ip;
        len += lcode(&e);
    }

    value->len = len;
    value->data = ngx_pnalloc(r->pool, len);
    if (value->data == NULL) {
        return NGX_ERROR;
    }

    e.ip = val->values;
    e.pos = value->data;
    e.buf = *value;

    while (*(uintptr_t *) e.ip) {
        code = *(ngx_http_script_code_pt *) e.ip;
        code((ngx_http_script_engine_t *) &e);
    }

    *value = e.buf;

    return NGX_OK;
}


ngx_int_t
ngx_http_compile_complex_value(ngx_http_compile_complex_value_t *ccv)
{
    ngx_str_t                  *v;
    ngx_uint_t                  i, n, nv, nc;
    ngx_array_t                 flushes, lengths, values, *pf, *pl, *pv;
    ngx_http_script_compile_t   sc;

    v = ccv->value;

    nv = 0;
    nc = 0;

    for (i = 0; i < v->len; i++) {
        if (v->data[i] == '$') {
            if (v->data[i + 1] >= '1' && v->data[i + 1] <= '9') {
                nc++;

            } else {
                nv++;
            }
        }
    }

    if ((v->len == 0 || v->data[0] != '$')
        && (ccv->conf_prefix || ccv->root_prefix))
    {
        if (ngx_conf_full_name(ccv->cf->cycle, v, ccv->conf_prefix) != NGX_OK) {
            return NGX_ERROR;
        }

        ccv->conf_prefix = 0;
        ccv->root_prefix = 0;
    }

    ccv->complex_value->value = *v;
    ccv->complex_value->flushes = NULL;
    ccv->complex_value->lengths = NULL;
    ccv->complex_value->values = NULL;

    if (nv == 0 && nc == 0) {
        return NGX_OK;
    }

    n = nv + 1;

    if (ngx_array_init(&flushes, ccv->cf->pool, n, sizeof(ngx_uint_t))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    n = nv * (2 * sizeof(ngx_http_script_copy_code_t)
                  + sizeof(ngx_http_script_var_code_t))
        + sizeof(uintptr_t);

    if (ngx_array_init(&lengths, ccv->cf->pool, n, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    n = (nv * (2 * sizeof(ngx_http_script_copy_code_t)
                   + sizeof(ngx_http_script_var_code_t))
                + sizeof(uintptr_t)
                + v->len
                + sizeof(uintptr_t) - 1)
            & ~(sizeof(uintptr_t) - 1);

    if (ngx_array_init(&values, ccv->cf->pool, n, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    pf = &flushes;
    pl = &lengths;
    pv = &values;

    ngx_memzero(&sc, sizeof(ngx_http_script_compile_t));

    sc.cf = ccv->cf;
    sc.source = v;
    sc.flushes = &pf;
    sc.lengths = &pl;
    sc.values = &pv;
    sc.complete_lengths = 1;
    sc.complete_values = 1;
    sc.zero = ccv->zero;
    sc.conf_prefix = ccv->conf_prefix;
    sc.root_prefix = ccv->root_prefix;

    if (ngx_http_script_compile(&sc) != NGX_OK) {
        return NGX_ERROR;
    }

    if (flushes.nelts) {
        ccv->complex_value->flushes = flushes.elts;
        ccv->complex_value->flushes[flushes.nelts] = (ngx_uint_t) -1;
    }

    ccv->complex_value->lengths = lengths.elts;
    ccv->complex_value->values = values.elts;

    return NGX_OK;
}


char *
ngx_http_set_complex_value_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_str_t                          *value;
    ngx_http_complex_value_t          **cv;
    ngx_http_compile_complex_value_t    ccv;

    cv = (ngx_http_complex_value_t **) (p + cmd->offset);

    if (*cv != NULL) {
        return "is duplicate";
    }

    *cv = ngx_palloc(cf->pool, sizeof(ngx_http_complex_value_t));
    if (*cv == NULL) {
        return NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));

    ccv.cf = cf;
    ccv.value = &value[1];
    ccv.complex_value = *cv;

    if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}






ngx_uint_t
ngx_http_script_variables_count(ngx_str_t *value)
{
    ngx_uint_t  i, n;

    for (n = 0, i = 0; i < value->len; i++) {
        if (value->data[i] == '$') {
            n++;
        }
    }

    return n;
}


ngx_int_t
ngx_http_script_compile(ngx_http_script_compile_t *sc)
{
    u_char       ch;
    ngx_str_t    name;
    ngx_uint_t   i, bracket;

    if (ngx_http_script_init_arrays(sc) != NGX_OK) {
        return NGX_ERROR;
    }

    for (i = 0; i < sc->source->len; /* void */ ) {

        name.len = 0;

        if (sc->source->data[i] == '$') {

            if (++i == sc->source->len) {
                goto invalid_variable;
            }

            if (sc->source->data[i] >= '1' && sc->source->data[i] <= '9') {
#if (NGX_PCRE)
                ngx_uint_t  n;

                n = sc->source->data[i] - '0';

                if (sc->captures_mask & ((ngx_uint_t) 1 << n)) {
                    sc->dup_capture = 1;
                }

                sc->captures_mask |= (ngx_uint_t) 1 << n;

                if (ngx_http_script_add_capture_code(sc, n) != NGX_OK) {
                    return NGX_ERROR;
                }

                i++;

                continue;
#else
                ngx_conf_log_error(NGX_LOG_EMERG, sc->cf, 0,
                                   "using variable \"$%c\" requires "
                                   "PCRE library", sc->source->data[i]);
                return NGX_ERROR;
#endif
            }

            if (sc->source->data[i] == '{') {
                bracket = 1;

                if (++i == sc->source->len) {
                    goto invalid_variable;
                }

                name.data = &sc->source->data[i];

            } else {
                bracket = 0;
                name.data = &sc->source->data[i];
            }

            for ( /* void */ ; i < sc->source->len; i++, name.len++) {
                ch = sc->source->data[i];

                if (ch == '}' && bracket) {
                    i++;
                    bracket = 0;
                    break;
                }

                if ((ch >= 'A' && ch <= 'Z')
                    || (ch >= 'a' && ch <= 'z')
                    || (ch >= '0' && ch <= '9')
                    || ch == '_')
                {
                    continue;
                }

                break;
            }

            if (bracket) {
                ngx_conf_log_error(NGX_LOG_EMERG, sc->cf, 0,
                                   "the closing bracket in \"%V\" "
                                   "variable is missing", &name);
                return NGX_ERROR;
            }

            if (name.len == 0) {
                goto invalid_variable;
            }

            sc->variables++;

            if (ngx_http_script_add_var_code(sc, &name) != NGX_OK) {
                return NGX_ERROR;
            }

            continue;
        }

        if (sc->source->data[i] == '?' && sc->compile_args) {
            sc->args = 1;
            sc->compile_args = 0;

            if (ngx_http_script_add_args_code(sc) != NGX_OK) {
                return NGX_ERROR;
            }

            i++;

            continue;
        }

        name.data = &sc->source->data[i];

        while (i < sc->source->len) {

            if (sc->source->data[i] == '$') {
                break;
            }

            if (sc->source->data[i] == '?') {

                sc->args = 1;

                if (sc->compile_args) {
                    break;
                }
            }

            i++;
            name.len++;
        }

        sc->size += name.len;

        if (ngx_http_script_add_copy_code(sc, &name, (i == sc->source->len))
            != NGX_OK)
        {
            return NGX_ERROR;
        }
    }

    return ngx_http_script_done(sc);

invalid_variable:

    ngx_conf_log_error(NGX_LOG_EMERG, sc->cf, 0, "invalid variable name");

    return NGX_ERROR;
}


u_char *
ngx_http_script_run(ngx_http_request_t *r, ngx_str_t *value,
    void *code_lengths, size_t len, void *code_values)
{
    ngx_uint_t                    i;
    ngx_http_script_code_pt       code;
    ngx_http_script_len_code_pt   lcode;
    ngx_http_script_engine_t      e;
    ngx_http_core_main_conf_t    *cmcf;

    cmcf = ngx_http_get_module_main_conf(r, ngx_http_core_module);

    for (i = 0; i < cmcf->variables.nelts; i++) {
        if (r->variables[i].no_cacheable) {
            r->variables[i].valid = 0;
            r->variables[i].not_found = 0;
        }
    }

    ngx_memzero(&e, sizeof(ngx_http_script_engine_t));

    e.ip = code_lengths;
    e.request = r;
    e.flushed = 1;

    while (*(uintptr_t *) e.ip) {
        lcode = *(ngx_http_script_len_code_pt *) e.ip;
        len += lcode(&e);
    }


    value->len = len;
    value->data = ngx_pnalloc(r->pool, len);
    if (value->data == NULL) {
        return NULL;
    }

    e.ip = code_values;
    e.pos = value->data;

    while (*(uintptr_t *) e.ip) {
        code = *(ngx_http_script_code_pt *) e.ip;
        code((ngx_http_script_engine_t *) &e);
    }

    return e.pos;
}


void
ngx_http_script_flush_no_cacheable_variables(ngx_http_request_t *r,
    ngx_array_t *indices)
{
    ngx_uint_t  n, *index;

    if (indices) {
        index = indices->elts;
        for (n = 0; n < indices->nelts; n++) {
            if (r->variables[index[n]].no_cacheable) {
                r->variables[index[n]].valid = 0;
                r->variables[index[n]].not_found = 0;
            }
        }
    }
}


static ngx_int_t
ngx_http_script_init_arrays(ngx_http_script_compile_t *sc)
{
    ngx_uint_t   n;

    if (sc->flushes && *sc->flushes == NULL) {
        n = sc->variables ? sc->variables : 1;
        *sc->flushes = ngx_array_create(sc->cf->pool, n, sizeof(ngx_uint_t));
        if (*sc->flushes == NULL) {
            return NGX_ERROR;
        }
    }

    if (*sc->lengths == NULL) {
        n = sc->variables * (2 * sizeof(ngx_http_script_copy_code_t)
                             + sizeof(ngx_http_script_var_code_t))
            + sizeof(uintptr_t);

        *sc->lengths = ngx_array_create(sc->cf->pool, n, 1);
        if (*sc->lengths == NULL) {
            return NGX_ERROR;
        }
    }

    if (*sc->values == NULL) {
        n = (sc->variables * (2 * sizeof(ngx_http_script_copy_code_t)
                              + sizeof(ngx_http_script_var_code_t))
                + sizeof(uintptr_t)
                + sc->source->len
                + sizeof(uintptr_t) - 1)
            & ~(sizeof(uintptr_t) - 1);

        *sc->values = ngx_array_create(sc->cf->pool, n, 1);
        if (*sc->values == NULL) {
            return NGX_ERROR;
        }
    }

    sc->variables = 0;

    return NGX_OK;
}


static ngx_int_t
ngx_http_script_done(ngx_http_script_compile_t *sc)
{
    ngx_str_t    zero;
    uintptr_t   *code;

    if (sc->zero) {

        zero.len = 1;
        zero.data = (u_char *) "\0";

        if (ngx_http_script_add_copy_code(sc, &zero, 0) != NGX_OK) {
            return NGX_ERROR;
        }
    }

    if (sc->conf_prefix || sc->root_prefix) {
        if (ngx_http_script_add_full_name_code(sc) != NGX_OK) {
            return NGX_ERROR;
        }
    }

    if (sc->complete_lengths) {
        code = ngx_http_script_add_code(*sc->lengths, sizeof(uintptr_t), NULL);
        if (code == NULL) {
            return NGX_ERROR;
        }

        *code = (uintptr_t) NULL;
    }

    if (sc->complete_values) {
        code = ngx_http_script_add_code(*sc->values, sizeof(uintptr_t),
                                        &sc->main);
        if (code == NULL) {
            return NGX_ERROR;
        }

        *code = (uintptr_t) NULL;
    }

    return NGX_OK;
}




void *
ngx_http_script_add_code(ngx_array_t *codes, size_t size, void *code)
{
    u_char  *elts, **p;
    void    *new;

    elts = codes->elts;

    new = ngx_array_push_n(codes, size);
    if (new == NULL) {
        return NULL;
    }

    if (code) {
        if (elts != codes->elts) {
            p = code;
            *p += (u_char *) codes->elts - elts;
        }
    }

    return new;
}


static ngx_int_t
ngx_http_script_add_copy_code(ngx_http_script_compile_t *sc, ngx_str_t *value,
    ngx_uint_t last)
{
    u_char                       *p;
    size_t                        size, len, zero;
    ngx_http_script_copy_code_t  *code;

    zero = (sc->zero && last);
    len = value->len + zero;

    code = ngx_http_script_add_code(*sc->lengths,
                                    sizeof(ngx_http_script_copy_code_t), NULL);
    if (code == NULL) {
        return NGX_ERROR;
    }

    code->code = (ngx_http_script_code_pt) (void *)
                                                 ngx_http_script_copy_len_code;
    code->len = len;

    size = (sizeof(ngx_http_script_copy_code_t) + len + sizeof(uintptr_t) - 1)
            & ~(sizeof(uintptr_t) - 1);

    code = ngx_http_script_add_code(*sc->values, size, &sc->main);
    if (code == NULL) {
        return NGX_ERROR;
    }

    code->code = ngx_http_script_copy_code;
    code->len = len;

    p = ngx_cpymem((u_char *) code + sizeof(ngx_http_script_copy_code_t),
                   value->data, value->len);

    if (zero) {
        *p = '\0';
        sc->zero = 0;
    }

    return NGX_OK;
}






static ngx_int_t
ngx_http_script_add_var_code(ngx_http_script_compile_t *sc, ngx_str_t *name)
{
    ngx_int_t                    index, *p;
    ngx_http_script_var_code_t  *code;

    index = ngx_http_get_variable_index(sc->cf, name);

    if (index == NGX_ERROR) {
        return NGX_ERROR;
    }

    if (sc->flushes) {
        p = ngx_array_push(*sc->flushes);
        if (p == NULL) {
            return NGX_ERROR;
        }

        *p = index;
    }

    code = ngx_http_script_add_code(*sc->lengths,
                                    sizeof(ngx_http_script_var_code_t), NULL);
    if (code == NULL) {
        return NGX_ERROR;
    }

    code->code = (ngx_http_script_code_pt) (void *)
                                             ngx_http_script_copy_var_len_code;
    code->index = (uintptr_t) index;

    code = ngx_http_script_add_code(*sc->values,
                                    sizeof(ngx_http_script_var_code_t),
                                    &sc->main);
    if (code == NULL) {
        return NGX_ERROR;
    }

    code->code = ngx_http_script_copy_var_code;
    code->index = (uintptr_t) index;

    return NGX_OK;
}






static ngx_int_t
ngx_http_script_add_args_code(ngx_http_script_compile_t *sc)
{
    uintptr_t   *code;

    code = ngx_http_script_add_code(*sc->lengths, sizeof(uintptr_t), NULL);
    if (code == NULL) {
        return NGX_ERROR;
    }

    *code = (uintptr_t) ngx_http_script_mark_args_code;

    code = ngx_http_script_add_code(*sc->values, sizeof(uintptr_t), &sc->main);
    if (code == NULL) {
        return NGX_ERROR;
    }

    *code = (uintptr_t) ngx_http_script_start_args_code;

    return NGX_OK;
}






#if (NGX_PCRE)

void
ngx_http_script_regex_start_code(ngx_http_script_engine_t *e)
{
    size_t                         len;
    ngx_int_t                      rc;
    ngx_uint_t                     n;
    ngx_http_request_t            *r;
    ngx_http_script_engine_t       le;
    ngx_http_script_len_code_pt    lcode;
    ngx_http_script_regex_code_t  *code;

    code = (ngx_http_script_regex_code_t *) e->ip;

    r = e->request;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http script regex: \"%V\"", &code->name);

    if (code->uri) {
        e->line = r->uri;
    } else {
        e->sp--;
        e->line.len = e->sp->len;
        e->line.data = e->sp->data;
    }

    rc = ngx_http_regex_exec(r, code->regex, &e->line);

    if (rc == NGX_DECLINED) {
        if (e->log || (r->connection->log->log_level & NGX_LOG_DEBUG_HTTP)) {
            ngx_log_error(NGX_LOG_NOTICE, r->connection->log, 0,
                          "\"%V\" does not match \"%V\"",
                          &code->name, &e->line);
        }

        r->ncaptures = 0;

        if (code->test) {
            if (code->negative_test) {
                e->sp->len = 1;
                e->sp->data = (u_char *) "1";

            } else {
                e->sp->len = 0;
                e->sp->data = (u_char *) "";
            }

            e->sp++;

            e->ip += sizeof(ngx_http_script_regex_code_t);
            return;
        }

        e->ip += code->next;
        return;
    }

    if (rc == NGX_ERROR) {
        e->ip = ngx_http_script_exit;
        e->status = NGX_HTTP_INTERNAL_SERVER_ERROR;
        return;
    }

    if (e->log || (r->connection->log->log_level & NGX_LOG_DEBUG_HTTP)) {
        ngx_log_error(NGX_LOG_NOTICE, r->connection->log, 0,
                      "\"%V\" matches \"%V\"", &code->name, &e->line);
    }

    if (code->test) {
        if (code->negative_test) {
            e->sp->len = 0;
            e->sp->data = (u_char *) "";

        } else {
            e->sp->len = 1;
            e->sp->data = (u_char *) "1";
        }

        e->sp++;

        e->ip += sizeof(ngx_http_script_regex_code_t);
        return;
    }

    if (code->status) {
        e->status = code->status;

        if (!code->redirect) {
            e->ip = ngx_http_script_exit;
            return;
        }
    }

    if (code->uri) {
        r->internal = 1;
        r->valid_unparsed_uri = 0;

        if (code->break_cycle) {
            r->valid_location = 0;
            r->uri_changed = 0;

        } else {
            r->uri_changed = 1;
        }
    }

    if (code->lengths == NULL) {
        e->buf.len = code->size;

        if (code->uri) {
            if (r->ncaptures && (r->quoted_uri || r->plus_in_uri)) {
                e->buf.len += 2 * ngx_escape_uri(NULL, r->uri.data, r->uri.len,
                                                 NGX_ESCAPE_ARGS);
            }
        }

        for (n = 2; n < r->ncaptures; n += 2) {
            e->buf.len += r->captures[n + 1] - r->captures[n];
        }

    } else {
        ngx_memzero(&le, sizeof(ngx_http_script_engine_t));

        le.ip = code->lengths->elts;
        le.line = e->line;
        le.request = r;
        le.quote = code->redirect;

        len = 0;

        while (*(uintptr_t *) le.ip) {
            lcode = *(ngx_http_script_len_code_pt *) le.ip;
            len += lcode(&le);
        }

        e->buf.len = len;
    }

    if (code->add_args && r->args.len) {
        e->buf.len += r->args.len + 1;
    }

    e->buf.data = ngx_pnalloc(r->pool, e->buf.len);
    if (e->buf.data == NULL) {
        e->ip = ngx_http_script_exit;
        e->status = NGX_HTTP_INTERNAL_SERVER_ERROR;
        return;
    }

    e->quote = code->redirect;

    e->pos = e->buf.data;

    e->ip += sizeof(ngx_http_script_regex_code_t);
}


void
ngx_http_script_regex_end_code(ngx_http_script_engine_t *e)
{
    u_char                            *dst, *src;
    ngx_http_request_t                *r;
    ngx_http_script_regex_end_code_t  *code;

    code = (ngx_http_script_regex_end_code_t *) e->ip;

    r = e->request;

    e->quote = 0;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http script regex end");

    if (code->redirect) {

        dst = e->buf.data;
        src = e->buf.data;

        ngx_unescape_uri(&dst, &src, e->pos - e->buf.data,
                         NGX_UNESCAPE_REDIRECT);

        if (src < e->pos) {
            dst = ngx_movemem(dst, src, e->pos - src);
        }

        e->pos = dst;

        if (code->add_args && r->args.len) {
            *e->pos++ = (u_char) (code->args ? '&' : '?');
            e->pos = ngx_copy(e->pos, r->args.data, r->args.len);
        }

        e->buf.len = e->pos - e->buf.data;

        if (e->log || (r->connection->log->log_level & NGX_LOG_DEBUG_HTTP)) {
            ngx_log_error(NGX_LOG_NOTICE, r->connection->log, 0,
                          "rewritten redirect: \"%V\"", &e->buf);
        }

        ngx_http_clear_location(r);

        r->headers_out.location = ngx_list_push(&r->headers_out.headers);
        if (r->headers_out.location == NULL) {
            e->ip = ngx_http_script_exit;
            e->status = NGX_HTTP_INTERNAL_SERVER_ERROR;
            return;
        }

        r->headers_out.location->hash = 1;
        ngx_str_set(&r->headers_out.location->key, "Location");
        r->headers_out.location->value = e->buf;

        e->ip += sizeof(ngx_http_script_regex_end_code_t);
        return;
    }

    if (e->args) {
        e->buf.len = e->args - e->buf.data;

        if (code->add_args && r->args.len) {
            *e->pos++ = '&';
            e->pos = ngx_copy(e->pos, r->args.data, r->args.len);
        }

        r->args.len = e->pos - e->args;
        r->args.data = e->args;

        e->args = NULL;

    } else {
        e->buf.len = e->pos - e->buf.data;

        if (!code->add_args) {
            r->args.len = 0;
        }
    }

    if (e->log || (r->connection->log->log_level & NGX_LOG_DEBUG_HTTP)) {
        ngx_log_error(NGX_LOG_NOTICE, r->connection->log, 0,
                      "rewritten data: \"%V\", args: \"%V\"",
                      &e->buf, &r->args);
    }

    if (code->uri) {
        r->uri = e->buf;

        if (r->uri.len == 0) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "the rewritten URI has a zero length");
            e->ip = ngx_http_script_exit;
            e->status = NGX_HTTP_INTERNAL_SERVER_ERROR;
            return;
        }

        ngx_http_set_exten(r);
    }

    e->ip += sizeof(ngx_http_script_regex_end_code_t);
}


static ngx_int_t
ngx_http_script_add_capture_code(ngx_http_script_compile_t *sc, ngx_uint_t n)
{
    ngx_http_script_copy_capture_code_t  *code;

    code = ngx_http_script_add_code(*sc->lengths,
                                    sizeof(ngx_http_script_copy_capture_code_t),
                                    NULL);
    if (code == NULL) {
        return NGX_ERROR;
    }

    code->code = (ngx_http_script_code_pt) (void *)
                                         ngx_http_script_copy_capture_len_code;
    code->n = 2 * n;


    code = ngx_http_script_add_code(*sc->values,
                                    sizeof(ngx_http_script_copy_capture_code_t),
                                    &sc->main);
    if (code == NULL) {
        return NGX_ERROR;
    }

    code->code = ngx_http_script_copy_capture_code;
    code->n = 2 * n;

    if (sc->ncaptures < n) {
        sc->ncaptures = n;
    }

    return NGX_OK;
}


size_t
ngx_http_script_copy_capture_len_code(ngx_http_script_engine_t *e)
{
    int                                  *cap;
    u_char                               *p;
    ngx_uint_t                            n;
    ngx_http_request_t                   *r;
    ngx_http_script_copy_capture_code_t  *code;

    r = e->request;

    code = (ngx_http_script_copy_capture_code_t *) e->ip;

    e->ip += sizeof(ngx_http_script_copy_capture_code_t);

    n = code->n;

    if (n < r->ncaptures) {

        cap = r->captures;

        if ((e->is_args || e->quote)
            && (e->request->quoted_uri || e->request->plus_in_uri))
        {
            p = r->captures_data;

            return cap[n + 1] - cap[n]
                   + 2 * ngx_escape_uri(NULL, &p[cap[n]], cap[n + 1] - cap[n],
                                        NGX_ESCAPE_ARGS);
        } else {
            return cap[n + 1] - cap[n];
        }
    }

    return 0;
}


void
ngx_http_script_copy_capture_code(ngx_http_script_engine_t *e)
{
    int                                  *cap;
    u_char                               *p, *pos;
    ngx_uint_t                            n;
    ngx_http_request_t                   *r;
    ngx_http_script_copy_capture_code_t  *code;

    r = e->request;

    code = (ngx_http_script_copy_capture_code_t *) e->ip;

    e->ip += sizeof(ngx_http_script_copy_capture_code_t);

    n = code->n;

    pos = e->pos;

    if (n < r->ncaptures) {

        cap = r->captures;
        p = r->captures_data;

        if ((e->is_args || e->quote)
            && (e->request->quoted_uri || e->request->plus_in_uri))
        {
            e->pos = (u_char *) ngx_escape_uri(pos, &p[cap[n]],
                                               cap[n + 1] - cap[n],
                                               NGX_ESCAPE_ARGS);
        } else {
            e->pos = ngx_copy(pos, &p[cap[n]], cap[n + 1] - cap[n]);
        }
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, e->request->connection->log, 0,
                   "http script capture: \"%*s\"", e->pos - pos, pos);
}

#endif


static ngx_int_t
ngx_http_script_add_full_name_code(ngx_http_script_compile_t *sc)
{
    ngx_http_script_full_name_code_t  *code;

    code = ngx_http_script_add_code(*sc->lengths,
                                    sizeof(ngx_http_script_full_name_code_t),
                                    NULL);
    if (code == NULL) {
        return NGX_ERROR;
    }

    code->code = (ngx_http_script_code_pt) (void *)
                                            ngx_http_script_full_name_len_code;
    code->conf_prefix = sc->conf_prefix;

    code = ngx_http_script_add_code(*sc->values,
                                    sizeof(ngx_http_script_full_name_code_t),
                                    &sc->main);
    if (code == NULL) {
        return NGX_ERROR;
    }

    code->code = ngx_http_script_full_name_code;
    code->conf_prefix = sc->conf_prefix;

    return NGX_OK;
}


























void
ngx_http_script_var_code(ngx_http_script_engine_t *e)
{
    ngx_http_variable_value_t   *value;
    ngx_http_script_var_code_t  *code;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, e->request->connection->log, 0,
                   "http script var");

    code = (ngx_http_script_var_code_t *) e->ip;

    e->ip += sizeof(ngx_http_script_var_code_t);

    value = ngx_http_get_flushed_variable(e->request, code->index);

    if (value && !value->not_found) {
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, e->request->connection->log, 0,
                       "http script var: \"%v\"", value);

        *e->sp = *value;
        e->sp++;

        return;
    }

    *e->sp = ngx_http_variable_null_value;
    e->sp++;
}


