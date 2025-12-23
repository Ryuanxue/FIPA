
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
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
#include "ngx_http_ssl_module.h"

typedef ngx_int_t (*ngx_ssl_variable_handler_pt)(ngx_connection_t *c,
    ngx_pool_t *pool, ngx_str_t *s);


#define NGX_DEFAULT_CIPHERS     "HIGH:!aNULL:!MD5"
#define NGX_DEFAULT_ECDH_CURVE  "auto"

#define NGX_HTTP_NPN_ADVERTISE  "\x08http/1.1"


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation
static int ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned char *outlen,
    const unsigned char *in, unsigned int inlen, void *arg);
#endif

#ifdef TLSEXT_TYPE_next_proto_neg
static int ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg);
#endif

static ngx_int_t ngx_http_ssl_static_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ssl_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_ssl_add_variables(ngx_conf_t *cf);
static void *ngx_http_ssl_create_srv_conf(ngx_conf_t *cf);
static char *ngx_http_ssl_merge_srv_conf(ngx_conf_t *cf,
    void *parent, void *child);

static char *ngx_http_ssl_enable(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_password_file(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
static char *ngx_http_ssl_session_cache(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_int_t ngx_http_ssl_init(ngx_conf_t *cf);


static ngx_conf_bitmask_t  ngx_http_ssl_protocols[] = {
    { ngx_string("SSLv2"), NGX_SSL_SSLv2 },
    { ngx_string("SSLv3"), NGX_SSL_SSLv3 },
    { ngx_string("TLSv1"), NGX_SSL_TLSv1 },
    { ngx_string("TLSv1.1"), NGX_SSL_TLSv1_1 },
    { ngx_string("TLSv1.2"), NGX_SSL_TLSv1_2 },
    { ngx_string("TLSv1.3"), NGX_SSL_TLSv1_3 },
    { ngx_null_string, 0 }
};


static ngx_conf_enum_t  ngx_http_ssl_verify[] = {
    { ngx_string("off"), 0 },
    { ngx_string("on"), 1 },
    { ngx_string("optional"), 2 },
    { ngx_string("optional_no_ca"), 3 },
    { ngx_null_string, 0 }
};


    ngx_conf_deprecated, "ssl", "listen ... ssl"
};


static ngx_command_t  ngx_http_ssl_commands[] = {

    { ngx_string("ssl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_http_ssl_enable,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, enable),
      &ngx_http_ssl_deprecated },

    { ngx_string("ssl_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificates),
      NULL },

    { ngx_string("ssl_certificate_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, certificate_keys),
      NULL },

    { ngx_string("ssl_password_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_http_ssl_password_file,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_dhparam"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, dhparam),
      NULL },

    { ngx_string("ssl_ecdh_curve"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ecdh_curve),
      NULL },

    { ngx_string("ssl_protocols"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_conf_set_bitmask_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, protocols),
      &ngx_http_ssl_protocols },

    { ngx_string("ssl_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, ciphers),
      NULL },

    { ngx_string("ssl_buffer_size"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_size_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, buffer_size),
      NULL },

    { ngx_string("ssl_verify_client"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify),
      &ngx_http_ssl_verify },

    { ngx_string("ssl_verify_depth"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, verify_depth),
      NULL },

    { ngx_string("ssl_client_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, client_certificate),
      NULL },

    { ngx_string("ssl_trusted_certificate"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, trusted_certificate),
      NULL },

    { ngx_string("ssl_prefer_server_ciphers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, prefer_server_ciphers),
      NULL },

    { ngx_string("ssl_session_cache"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE12,
      ngx_http_ssl_session_cache,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("ssl_session_tickets"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_tickets),
      NULL },

    { ngx_string("ssl_session_ticket_key"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_array_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_ticket_keys),
      NULL },

    { ngx_string("ssl_session_timeout"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_sec_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, session_timeout),
      NULL },

    { ngx_string("ssl_crl"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, crl),
      NULL },

    { ngx_string("ssl_stapling"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling),
      NULL },

    { ngx_string("ssl_stapling_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_file),
      NULL },

    { ngx_string("ssl_stapling_responder"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_responder),
      NULL },

    { ngx_string("ssl_stapling_verify"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, stapling_verify),
      NULL },

    { ngx_string("ssl_early_data"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_SRV_CONF_OFFSET,
      offsetof(ngx_http_ssl_srv_conf_t, early_data),
      NULL },

      ngx_null_command
};


    ngx_http_ssl_add_variables,            /* preconfiguration */
    ngx_http_ssl_init,                     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_ssl_create_srv_conf,          /* create server configuration */
    ngx_http_ssl_merge_srv_conf,           /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_ssl_module = {
    NGX_MODULE_V1,
    &ngx_http_ssl_module_ctx,              /* module context */
    ngx_http_ssl_commands,                 /* module directives */
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


static ngx_http_variable_t  ngx_http_ssl_vars[] = {

    { ngx_string("ssl_protocol"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_protocol, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_cipher"), NULL, ngx_http_ssl_static_variable,
      (uintptr_t) ngx_ssl_get_cipher_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_ciphers"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_ciphers, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_curves"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_curves, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_id"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_id, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_session_reused"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_session_reused, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_early_data"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_early_data,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("ssl_server_name"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_server_name, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_certificate, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_raw_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_raw_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_escaped_cert"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_escaped_certificate,
      NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_s_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_subject_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_i_dn_legacy"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_issuer_dn_legacy, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_serial"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_serial_number, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_fingerprint"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_fingerprint, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_verify"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_verify, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_start"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_start, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_end"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_end, NGX_HTTP_VAR_CHANGEABLE, 0 },

    { ngx_string("ssl_client_v_remain"), NULL, ngx_http_ssl_variable,
      (uintptr_t) ngx_ssl_get_client_v_remain, NGX_HTTP_VAR_CHANGEABLE, 0 },

      ngx_http_null_variable
};


static ngx_str_t ngx_http_ssl_sess_id_ctx = ngx_string("HTTP");


#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

static int
ngx_http_ssl_alpn_select(ngx_ssl_conn_t *ssl_conn, const unsigned char **out,
    unsigned char *outlen, const unsigned char *in, unsigned int inlen,
    void *arg)
{
    unsigned int            srvlen;
    unsigned char          *srv;
#if (NGX_DEBUG)
    unsigned int            i;
#endif
#if (NGX_HTTP_V2)
    ngx_http_connection_t  *hc;
#endif
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t       *c;

    c = ngx_ssl_get_connection(ssl_conn);
#endif

#if (NGX_DEBUG)
    for (i = 0; i < inlen; i += in[i] + 1) {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                       "SSL ALPN supported by client: %*s",
                       (size_t) in[i], &in[i + 1]);
    }
#endif

#if (NGX_HTTP_V2)
    hc = c->data;

    if (hc->addr_conf->http2) {
        srv =
           (unsigned char *) NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_V2_ALPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

    } else
#endif
    {
        srv = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
        srvlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;
    }

    if (SSL_select_next_proto((unsigned char **) out, outlen, srv, srvlen,
                              in, inlen)
        != OPENSSL_NPN_NEGOTIATED)
    {
        return SSL_TLSEXT_ERR_NOACK;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0,
                   "SSL ALPN selected: %*s", (size_t) *outlen, *out);

    return SSL_TLSEXT_ERR_OK;
}

#endif


#ifdef TLSEXT_TYPE_next_proto_neg

static int
ngx_http_ssl_npn_advertised(ngx_ssl_conn_t *ssl_conn,
    const unsigned char **out, unsigned int *outlen, void *arg)
{
#if (NGX_HTTP_V2 || NGX_DEBUG)
    ngx_connection_t  *c;

    c = ngx_ssl_get_connection(ssl_conn);
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "SSL NPN advertised");
#endif

#if (NGX_HTTP_V2)
    {
    ngx_http_connection_t  *hc;

    hc = c->data;

    if (hc->addr_conf->http2) {
        *out =
            (unsigned char *) NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE;
        *outlen = sizeof(NGX_HTTP_V2_NPN_ADVERTISE NGX_HTTP_NPN_ADVERTISE) - 1;

        return SSL_TLSEXT_ERR_OK;
    }
    }
#endif

    *out = (unsigned char *) NGX_HTTP_NPN_ADVERTISE;
    *outlen = sizeof(NGX_HTTP_NPN_ADVERTISE) - 1;

    return SSL_TLSEXT_ERR_OK;
}

#endif








static void *
ngx_http_ssl_create_srv_conf(ngx_conf_t *cf)
{
    ngx_http_ssl_srv_conf_t  *sscf;

    sscf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ssl_srv_conf_t));
    if (sscf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     sscf->protocols = 0;
     *     sscf->dhparam = { 0, NULL };
     *     sscf->ecdh_curve = { 0, NULL };
     *     sscf->client_certificate = { 0, NULL };
     *     sscf->trusted_certificate = { 0, NULL };
     *     sscf->crl = { 0, NULL };
     *     sscf->ciphers = { 0, NULL };
     *     sscf->shm_zone = NULL;
     *     sscf->stapling_file = { 0, NULL };
     *     sscf->stapling_responder = { 0, NULL };
     */

    sscf->enable = NGX_CONF_UNSET;
    sscf->prefer_server_ciphers = NGX_CONF_UNSET;
    sscf->early_data = NGX_CONF_UNSET;
    sscf->buffer_size = NGX_CONF_UNSET_SIZE;
    sscf->verify = NGX_CONF_UNSET_UINT;
    sscf->verify_depth = NGX_CONF_UNSET_UINT;
    sscf->certificates = NGX_CONF_UNSET_PTR;
    sscf->certificate_keys = NGX_CONF_UNSET_PTR;
    sscf->passwords = NGX_CONF_UNSET_PTR;
    sscf->builtin_session_cache = NGX_CONF_UNSET;
    sscf->session_timeout = NGX_CONF_UNSET;
    sscf->session_tickets = NGX_CONF_UNSET;
    sscf->session_ticket_keys = NGX_CONF_UNSET_PTR;
    sscf->stapling = NGX_CONF_UNSET;
    sscf->stapling_verify = NGX_CONF_UNSET;

    return sscf;
}


char *ngx_http_ssl_merge_srv_conf_sense_1(int *ngx_http_ssl_merge_srv_conf_sense_1_ret, ngx_http_ssl_srv_conf_t *prev, ngx_http_ssl_srv_conf_t *conf, ngx_pool_cleanup_t *cln, ngx_conf_t *cf)
{
  if (conf->enable == (-1))
  {
    if (prev->enable == (-1))
    {
      conf->enable = 0;
    }
    else
    {
      conf->enable = prev->enable;
      conf->file = prev->file;
      conf->line = prev->line;
    }
  }
  if (conf->session_timeout == (-1))
  {
    conf->session_timeout = (prev->session_timeout == (-1)) ? (300) : (prev->session_timeout);
  }
  if (conf->prefer_server_ciphers == (-1))
  {
    conf->prefer_server_ciphers = (prev->prefer_server_ciphers == (-1)) ? (0) : (prev->prefer_server_ciphers);
  }
  if (conf->early_data == (-1))
  {
    conf->early_data = (prev->early_data == (-1)) ? (0) : (prev->early_data);
  }
  ;
  if (conf->protocols == 0)
  {
    conf->protocols = (prev->protocols == 0) ? (((1 | NGX_SSL_TLSv1) | NGX_SSL_TLSv1_1) | NGX_SSL_TLSv1_2) : (prev->protocols);
  }
  if (conf->buffer_size == ((size_t) (-1)))
  {
    conf->buffer_size = (prev->buffer_size == ((size_t) (-1))) ? (NGX_SSL_BUFSIZE) : (prev->buffer_size);
  }
  if (conf->verify == ((ngx_uint_t) (-1)))
  {
    conf->verify = (prev->verify == ((ngx_uint_t) (-1))) ? (0) : (prev->verify);
  }
  ;
  if (conf->verify_depth == ((ngx_uint_t) (-1)))
  {
    conf->verify_depth = (prev->verify_depth == ((ngx_uint_t) (-1))) ? (1) : (prev->verify_depth);
  }
  ;
  if (conf->certificates == ((void *) (-1)))
  {
    conf->certificates = (prev->certificates == ((void *) (-1))) ? (0) : (prev->certificates);
  }
  ;
  if (conf->certificate_keys == ((void *) (-1)))
  {
    conf->certificate_keys = (prev->certificate_keys == ((void *) (-1))) ? (0) : (prev->certificate_keys);
  }
  if (conf->passwords == ((void *) (-1)))
  {
    conf->passwords = (prev->passwords == ((void *) (-1))) ? (0) : (prev->passwords);
  }
  ;
  if (conf->dhparam.data == 0)
  {
    if (prev->dhparam.data)
    {
      conf->dhparam.len = prev->dhparam.len;
      conf->dhparam.data = prev->dhparam.data;
    }
    else
    {
      conf->dhparam.len = (sizeof("")) - 1;
      conf->dhparam.data = (u_char *) "";
    }
  }
  ;
  if (conf->client_certificate.data == 0)
  {
    if (prev->client_certificate.data)
    {
      conf->client_certificate.len = prev->client_certificate.len;
      conf->client_certificate.data = prev->client_certificate.data;
    }
    else
    {
      conf->client_certificate.len = (sizeof("")) - 1;
      conf->client_certificate.data = (u_char *) "";
    }
  }
  if (conf->trusted_certificate.data == 0)
  {
    if (prev->trusted_certificate.data)
    {
      conf->trusted_certificate.len = prev->trusted_certificate.len;
      conf->trusted_certificate.data = prev->trusted_certificate.data;
    }
    else
    {
      conf->trusted_certificate.len = (sizeof("")) - 1;
      conf->trusted_certificate.data = (u_char *) "";
    }
  }
  if (conf->crl.data == 0)
  {
    if (prev->crl.data)
    {
      conf->crl.len = prev->crl.len;
      conf->crl.data = prev->crl.data;
    }
    else
    {
      conf->crl.len = (sizeof("")) - 1;
      conf->crl.data = (u_char *) "";
    }
  }
  ;
  if (conf->ecdh_curve.data == 0)
  {
    if (prev->ecdh_curve.data)
    {
      conf->ecdh_curve.len = prev->ecdh_curve.len;
      conf->ecdh_curve.data = prev->ecdh_curve.data;
    }
    else
    {
      conf->ecdh_curve.len = (sizeof("auto")) - 1;
      conf->ecdh_curve.data = (u_char *) "auto";
    }
  }
  if (conf->ciphers.data == 0)
  {
    if (prev->ciphers.data)
    {
      conf->ciphers.len = prev->ciphers.len;
      conf->ciphers.data = prev->ciphers.data;
    }
    else
    {
      conf->ciphers.len = (sizeof("HIGH:!aNULL:!MD5")) - 1;
      conf->ciphers.data = (u_char *) "HIGH:!aNULL:!MD5";
    }
  }
  ;
  if (conf->stapling == (-1))
  {
    conf->stapling = (prev->stapling == (-1)) ? (0) : (prev->stapling);
  }
  ;
  if (conf->stapling_verify == (-1))
  {
    conf->stapling_verify = (prev->stapling_verify == (-1)) ? (0) : (prev->stapling_verify);
  }
  ;
  if (conf->stapling_file.data == 0)
  {
    if (prev->stapling_file.data)
    {
      conf->stapling_file.len = prev->stapling_file.len;
      conf->stapling_file.data = prev->stapling_file.data;
    }
    else
    {
      conf->stapling_file.len = (sizeof("")) - 1;
      conf->stapling_file.data = (u_char *) "";
    }
  }
  ;
  if (conf->stapling_responder.data == 0)
  {
    if (prev->stapling_responder.data)
    {
      conf->stapling_responder.len = prev->stapling_responder.len;
      conf->stapling_responder.data = prev->stapling_responder.data;
    }
    else
    {
      conf->stapling_responder.len = (sizeof("")) - 1;
      conf->stapling_responder.data = (u_char *) "";
    }
  }
  if (conf->enable)
  {
    if (conf->certificates == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys == 0)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for the \"ssl\" directive in %s:%ui", conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (conf->certificate_keys->nelts < conf->certificates->nelts)
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\" and the \"ssl\" directive in %s:%ui", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1, conf->file, conf->line);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  else
  {
    if (conf->certificates == 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return 0;
      }
    }
    if ((conf->certificate_keys == 0) || (conf->certificate_keys->nelts < conf->certificates->nelts))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate_key\" is defined for certificate \"%V\"", (((ngx_str_t *) conf->certificates->elts) + conf->certificates->nelts) - 1);
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_create(&conf->ssl, conf->protocols, conf) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  cln = ngx_pool_cleanup_add(cf->pool, 0);
  if (cln == 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_certificates(cf, &conf->ssl, conf->certificates, conf->certificate_keys, conf->passwords) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ciphers(cf, &conf->ssl, &conf->ciphers, conf->prefer_server_ciphers) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->verify)
  {
    if ((conf->client_certificate.len == 0) && (conf->verify != 3))
    {
      if (cf->log->log_level >= 1)
        ngx_log_error_core(1, cf->log, 0, "no ssl_client_certificate for ssl_client_verify");
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
    if (ngx_ssl_client_certificate(cf, &conf->ssl, &conf->client_certificate, conf->verify_depth) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_trusted_certificate(cf, &conf->ssl, &conf->trusted_certificate, conf->verify_depth) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_crl(cf, &conf->ssl, &conf->crl) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_dhparam(cf, &conf->ssl, &conf->dhparam) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (ngx_ssl_ecdh_curve(cf, &conf->ssl, &conf->ecdh_curve) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->builtin_session_cache == (-1))
  {
    conf->builtin_session_cache = (prev->builtin_session_cache == (-1)) ? (NGX_SSL_NONE_SCACHE) : (prev->builtin_session_cache);
  }
  if (conf->shm_zone == 0)
  {
    conf->shm_zone = prev->shm_zone;
  }
  if (ngx_ssl_session_cache(&conf->ssl, &ngx_http_ssl_sess_id_ctx, conf->builtin_session_cache, conf->shm_zone, conf->session_timeout) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->session_tickets == (-1))
  {
    conf->session_tickets = (prev->session_tickets == (-1)) ? (1) : (prev->session_tickets);
  }
  ;
  if (conf->session_ticket_keys == ((void *) (-1)))
  {
    conf->session_ticket_keys = (prev->session_ticket_keys == ((void *) (-1))) ? (0) : (prev->session_ticket_keys);
  }
  if (ngx_ssl_session_ticket_keys(cf, &conf->ssl, conf->session_ticket_keys) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  if (conf->stapling)
  {
    if (ngx_ssl_stapling(cf, &conf->ssl, &conf->stapling_file, &conf->stapling_responder, conf->stapling_verify) != 0)
    {
      {
        *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
        return (void *) (-1);
      }
    }
  }
  if (ngx_ssl_early_data(cf, &conf->ssl, conf->early_data) != 0)
  {
    {
      *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
      return (void *) (-1);
    }
  }
  {
    *ngx_http_ssl_merge_srv_conf_sense_1_ret = 1;
    return 0;
  }
}







char *ngx_http_ssl_session_cache_sense_1(int *ngx_http_ssl_session_cache_sense_1_ret, ngx_conf_t *cf, void *conf)
{
  ngx_http_ssl_srv_conf_t *sscf = conf;
  size_t len;
  ngx_str_t *value;
  ngx_str_t name;
  ngx_str_t size;
  ngx_int_t n;
  ngx_uint_t i;
  ngx_uint_t j;
  value = cf->args->elts;
  for (i = 1; i < cf->args->nelts; i++)
  {
    if (strcmp((const char *) value[i].data, (const char *) "off") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NO_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "none") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_NONE_SCACHE;
      continue;
    }
    if (strcmp((const char *) value[i].data, (const char *) "builtin") == 0)
    {
      sscf->builtin_session_cache = NGX_SSL_DFLT_BUILTIN_SCACHE;
      continue;
    }
    if ((value[i].len > ((sizeof("builtin:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "builtin:", (sizeof("builtin:")) - 1) == 0))
    {
      n = ngx_atoi((value[i].data + (sizeof("builtin:"))) - 1, value[i].len - ((sizeof("builtin:")) - 1));
      if (n == (-1))
      {
        goto invalid;
      }
      sscf->builtin_session_cache = n;
      continue;
    }
    if ((value[i].len > ((sizeof("shared:")) - 1)) && (strncmp((const char *) value[i].data, (const char *) "shared:", (sizeof("shared:")) - 1) == 0))
    {
      len = 0;
      for (j = (sizeof("shared:")) - 1; j < value[i].len; j++)
      {
        if (value[i].data[j] == ':')
        {
          break;
        }
        len++;
      }

      if (len == 0)
      {
        goto invalid;
      }
      name.len = len;
      name.data = (value[i].data + (sizeof("shared:"))) - 1;
      size.len = (value[i].len - j) - 1;
      size.data = (name.data + len) + 1;
      n = ngx_parse_size(&size);
      if (n == (-1))
      {
        goto invalid;
      }
      if (n < ((ngx_int_t) (8 * ngx_pagesize)))
      {
        ngx_conf_log_error(1, cf, 0, "session cache \"%V\" is too small", &value[i]);
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone = ngx_shared_memory_add(cf, &name, n, &ngx_http_ssl_module);
      if (sscf->shm_zone == 0)
      {
        {
          *ngx_http_ssl_session_cache_sense_1_ret = 1;
          return (void *) (-1);
        }
      }
      sscf->shm_zone->init = ngx_ssl_session_cache_init;
      continue;
    }
    goto invalid;
  }

  if (sscf->shm_zone && (sscf->builtin_session_cache == (-1)))
  {
    sscf->builtin_session_cache = NGX_SSL_NO_BUILTIN_SCACHE;
  }
  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return 0;
  }
  invalid:
  ngx_conf_log_error(1, cf, 0, "invalid session cache \"%V\"", &value[i]);

  {
    *ngx_http_ssl_session_cache_sense_1_ret = 1;
    return (void *) (-1);
  }
}



ngx_int_t ngx_http_ssl_init_sense_1(int *ngx_http_ssl_init_sense_1_ret, ngx_conf_t *cf)
{
  ngx_uint_t a;
  ngx_uint_t p;
  ngx_uint_t s;
  ngx_http_conf_addr_t *addr;
  ngx_http_conf_port_t *port;
  ngx_http_ssl_srv_conf_t *sscf;
  ngx_http_core_loc_conf_t *clcf;
  ngx_http_core_srv_conf_t **cscfp;
  ngx_http_core_srv_conf_t *cscf;
  ngx_http_core_main_conf_t *cmcf;
  cmcf = ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[ngx_http_core_module.ctx_index];
  cscfp = cmcf->servers.elts;
  for (s = 0; s < cmcf->servers.nelts; s++)
  {
    sscf = cscfp[s]->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
    if ((sscf->ssl.ctx == 0) || (!sscf->stapling))
    {
      continue;
    }
    clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];
    if (ngx_ssl_stapling_resolver(cf, &sscf->ssl, clcf->resolver, clcf->resolver_timeout) != 0)
    {
      {
        *ngx_http_ssl_init_sense_1_ret = 1;
        return -1;
      }
    }
  }

  if (cmcf->ports == 0)
  {
    {
      *ngx_http_ssl_init_sense_1_ret = 1;
      return 0;
    }
  }
  port = cmcf->ports->elts;
  for (p = 0; p < cmcf->ports->nelts; p++)
  {
    addr = port[p].addrs.elts;
    for (a = 0; a < port[p].addrs.nelts; a++)
    {
      if (!addr[a].opt.ssl)
      {
        continue;
      }
      cscf = addr[a].default_server;
      sscf = cscf->ctx->srv_conf[ngx_http_ssl_module.ctx_index];
      if (sscf->certificates == 0)
      {
        if (cf->log->log_level >= 1)
          ngx_log_error_core(1, cf->log, 0, "no \"ssl_certificate\" is defined for the \"listen ... ssl\" directive in %s:%ui", cscf->file_name, cscf->line);
        {
          *ngx_http_ssl_init_sense_1_ret = 1;
          return -1;
        }
      }
    }

  }

  {
    *ngx_http_ssl_init_sense_1_ret = 1;
    return 0;
  }
}

