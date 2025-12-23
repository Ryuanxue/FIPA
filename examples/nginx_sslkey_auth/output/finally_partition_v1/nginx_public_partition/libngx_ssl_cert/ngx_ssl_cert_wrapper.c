/*
 * ngx_ssl_cert_wrapper.c
 * 
 * 从 ngx_event_openssl.c 提取 ngx_ssl_certificate 及其依赖函数
 * 编译为独立的共享库
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event_openssl.h>
#include "ngx_ssl_cert_wrapper.h"

#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif
#include <openssl/evp.h>

/* 全局变量定义 */
int ngx_ssl_certificate_index = -1;
int ngx_ssl_certificate_name_index = -1;
int ngx_ssl_next_certificate_index = -1;

/* 静态函数声明 */
static int ngx_ssl_password_callback(char *buf, int size, int rwflag, 
                                      void *userdata);

/**
 * 初始化函数 - 必须先调用
 */
ngx_int_t ngx_ssl_cert_init(ngx_log_t *log)
{
    if (ngx_ssl_certificate_index != -1) {
        /* 已经初始化 */
        return NGX_OK;
    }

    ngx_ssl_certificate_index = SSL_CTX_get_ex_new_index(0, NULL, NULL, 
                                                          NULL, NULL);
    if (ngx_ssl_certificate_index == -1) {
        ngx_ssl_error(NGX_LOG_EMERG, log, 0,
                      "SSL_CTX_get_ex_new_index() for certificate failed");
        return NGX_ERROR;
    }

    ngx_ssl_next_certificate_index = X509_get_ex_new_index(0, NULL, NULL,
                                                             NULL, NULL);
    if (ngx_ssl_next_certificate_index == -1) {
        ngx_ssl_error(NGX_LOG_EMERG, log, 0,
                      "X509_get_ex_new_index() for next certificate failed");
        return NGX_ERROR;
    }

    ngx_ssl_certificate_name_index = X509_get_ex_new_index(0, NULL, NULL,
                                                             NULL, NULL);
    if (ngx_ssl_certificate_name_index == -1) {
        ngx_ssl_error(NGX_LOG_EMERG, log, 0,
                      "X509_get_ex_new_index() for certificate name failed");
        return NGX_ERROR;
    }

    return NGX_OK;
}

/**
 * 密码回调函数
 */
static int
ngx_ssl_password_callback(char *buf, int size, int rwflag, void *userdata)
{
    ngx_str_t *pwd = userdata;

    if (rwflag) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      "ngx_ssl_password_callback() is called for encryption");
        return 0;
    }

    if (pwd->len > (size_t) size) {
        ngx_log_error(NGX_LOG_ERR, ngx_cycle->log, 0,
                      "password is truncated to %d bytes", size);
    } else {
        size = pwd->len;
    }

    ngx_memcpy(buf, pwd->data, size);

    return size;
}

/**
 * 加载 SSL 证书 - 核心函数
 */
ngx_int_t
ngx_ssl_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *cert,
    ngx_str_t *key, ngx_array_t *passwords)
{
    BIO         *bio;
    X509        *x509;
    u_long       n;
    ngx_str_t   *pwd;
    ngx_uint_t   tries;

    if (ngx_conf_full_name(cf->cycle, cert, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    /*
     * 不使用 SSL_CTX_use_certificate_chain_file()，因为它不允许
     * 之后从 SSL_CTX 访问证书，所以这里重新实现
     */

    bio = BIO_new_file((char *) cert->data, "r");
    if (bio == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "BIO_new_file(\"%s\") failed", cert->data);
        return NGX_ERROR;
    }

    x509 = PEM_read_bio_X509_AUX(bio, NULL, NULL, NULL);
    if (x509 == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "PEM_read_bio_X509_AUX(\"%s\") failed", cert->data);
        BIO_free(bio);
        return NGX_ERROR;
    }

    if (SSL_CTX_use_certificate(ssl->ctx, x509) == 0) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_use_certificate(\"%s\") failed", cert->data);
        X509_free(x509);
        BIO_free(bio);
        return NGX_ERROR;
    }

    if (X509_set_ex_data(x509, ngx_ssl_certificate_name_index, cert->data)
        == 0)
    {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0, "X509_set_ex_data() failed");
        X509_free(x509);
        BIO_free(bio);
        return NGX_ERROR;
    }

    if (X509_set_ex_data(x509, ngx_ssl_next_certificate_index,
                      SSL_CTX_get_ex_data(ssl->ctx, ngx_ssl_certificate_index))
        == 0)
    {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0, "X509_set_ex_data() failed");
        X509_free(x509);
        BIO_free(bio);
        return NGX_ERROR;
    }

    if (SSL_CTX_set_ex_data(ssl->ctx, ngx_ssl_certificate_index, x509)
        == 0)
    {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_set_ex_data() failed");
        X509_free(x509);
        BIO_free(bio);
        return NGX_ERROR;
    }

    /* 读取证书链的其余部分 */

    for ( ;; ) {

        x509 = PEM_read_bio_X509(bio, NULL, NULL, NULL);
        if (x509 == NULL) {
            n = ERR_peek_last_error();

            if (ERR_GET_LIB(n) == ERR_LIB_PEM
                && ERR_GET_REASON(n) == PEM_R_NO_START_LINE)
            {
                /* 文件结束 */
                ERR_clear_error();
                break;
            }

            /* 发生实际错误 */

            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "PEM_read_bio_X509(\"%s\") failed", cert->data);
            BIO_free(bio);
            return NGX_ERROR;
        }

#ifdef SSL_CTRL_CHAIN_CERT

        /*
         * SSL_CTX_add0_chain_cert() 在使用多个证书时需要添加链
         * 到特定证书；仅在 OpenSSL 1.0.2+ 可用
         */

        if (SSL_CTX_add0_chain_cert(ssl->ctx, x509) == 0) {
            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "SSL_CTX_add0_chain_cert(\"%s\") failed",
                          cert->data);
            X509_free(x509);
            BIO_free(bio);
            return NGX_ERROR;
        }

#else
        if (SSL_CTX_add_extra_chain_cert(ssl->ctx, x509) == 0) {
            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "SSL_CTX_add_extra_chain_cert(\"%s\") failed",
                          cert->data);
            X509_free(x509);
            BIO_free(bio);
            return NGX_ERROR;
        }
#endif
    }

    BIO_free(bio);

    /* 处理 engine: 前缀的私钥 */
    if (ngx_strncmp(key->data, "engine:", sizeof("engine:") - 1) == 0) {

#ifndef OPENSSL_NO_ENGINE

        u_char      *p, *last;
        ENGINE      *engine;
        EVP_PKEY    *pkey;

        p = key->data + sizeof("engine:") - 1;
        last = (u_char *) ngx_strchr(p, ':');

        if (last == NULL) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid syntax in \"%V\"", key);
            return NGX_ERROR;
        }

        *last = '\0';

        engine = ENGINE_by_id((char *) p);

        if (engine == NULL) {
            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "ENGINE_by_id(\"%s\") failed", p);
            return NGX_ERROR;
        }

        *last++ = ':';

        pkey = ENGINE_load_private_key(engine, (char *) last, 0, 0);

        if (pkey == NULL) {
            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "ENGINE_load_private_key(\"%s\") failed", last);
            ENGINE_free(engine);
            return NGX_ERROR;
        }

        ENGINE_free(engine);

        if (SSL_CTX_use_PrivateKey(ssl->ctx, pkey) == 0) {
            ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                          "SSL_CTX_use_PrivateKey(\"%s\") failed", last);
            EVP_PKEY_free(pkey);
            return NGX_ERROR;
        }

        EVP_PKEY_free(pkey);

        return NGX_OK;

#else

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "loading \"engine:...\" certificate keys "
                           "is not supported");
        return NGX_ERROR;

#endif
    }

    /* 处理普通文件私钥 */
    if (ngx_conf_full_name(cf->cycle, key, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    if (passwords) {
        tries = passwords->nelts;
        pwd = passwords->elts;

        SSL_CTX_set_default_passwd_cb(ssl->ctx, ngx_ssl_password_callback);
        SSL_CTX_set_default_passwd_cb_userdata(ssl->ctx, pwd);

    } else {
        tries = 1;
#if (NGX_SUPPRESS_WARN)
        pwd = NULL;
#endif
    }

    for ( ;; ) {

        if (SSL_CTX_use_PrivateKey_file(ssl->ctx, (char *) key->data,
                                        SSL_FILETYPE_PEM)
            != 0)
        {
            break;
        }

        if (--tries) {
            ERR_clear_error();
            SSL_CTX_set_default_passwd_cb_userdata(ssl->ctx, ++pwd);
            continue;
        }

        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "SSL_CTX_use_PrivateKey_file(\"%s\") failed", key->data);
        return NGX_ERROR;
    }

    SSL_CTX_set_default_passwd_cb(ssl->ctx, NULL);

    return NGX_OK;
}

/**
 * 批量加载证书
 */
ngx_int_t
ngx_ssl_certificates(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_array_t *certs,
    ngx_array_t *keys, ngx_array_t *passwords)
{
    ngx_str_t   *cert, *key;
    ngx_uint_t   i;

    cert = certs->elts;
    key = keys->elts;

    for (i = 0; i < certs->nelts; i++) {

        if (ngx_ssl_certificate(cf, ssl, &cert[i], &key[i], passwords)
            != NGX_OK)
        {
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}
