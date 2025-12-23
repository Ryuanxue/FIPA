/*
 * ngx_event_openssl_stub.c
 * 
 * 存根文件：将 ngx_ssl_certificate 相关函数调用重定向到共享库
 * 
 * 用法：
 *   1. 将此文件添加到 nginx 源码
 *   2. 在 ngx_event_openssl.c 中删除或注释掉原 ngx_ssl_certificate 实现
 *   3. 编译时链接 -ldl
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event_openssl.h>
#include <dlfcn.h>

/* 共享库句柄和函数指针 */
static void *ngx_ssl_cert_lib_handle = NULL;

typedef ngx_int_t (*ngx_ssl_cert_init_func_t)(ngx_log_t *log);
typedef ngx_int_t (*ngx_ssl_certificate_func_t)(ngx_conf_t *cf, ngx_ssl_t *ssl,
                                                 ngx_str_t *cert, ngx_str_t *key,
                                                 ngx_array_t *passwords);
typedef ngx_int_t (*ngx_ssl_certificates_func_t)(ngx_conf_t *cf, ngx_ssl_t *ssl,
                                                  ngx_array_t *certs, ngx_array_t *keys,
                                                  ngx_array_t *passwords);

static ngx_ssl_cert_init_func_t ngx_ssl_cert_init_dl = NULL;
static ngx_ssl_certificate_func_t ngx_ssl_certificate_impl = NULL;
static ngx_ssl_certificates_func_t ngx_ssl_certificates_impl = NULL;

/* 共享库路径候选列表 */
static const char *lib_search_paths[] = {
    /* 相对于 nginx 可执行文件的路径 */
    "./libngx_ssl_cert/libngx_ssl_cert.so",
    "../libngx_ssl_cert/libngx_ssl_cert.so",
    "./nginx_public_partition/libngx_ssl_cert/libngx_ssl_cert.so",
    /* 系统库路径 */
    "/usr/local/lib/libngx_ssl_cert.so",
    "/usr/lib/libngx_ssl_cert.so",
    /* 仅文件名（依赖 LD_LIBRARY_PATH）*/
    "libngx_ssl_cert.so",
    NULL
};

/**
 * 加载共享库
 */
static ngx_int_t
ngx_ssl_cert_load_library(ngx_log_t *log)
{
    int i;
    char *error;

    if (ngx_ssl_cert_lib_handle != NULL) {
        /* 已加载 */
        return NGX_OK;
    }

    /* 尝试多个路径 */
    for (i = 0; lib_search_paths[i] != NULL; i++) {
        ngx_ssl_cert_lib_handle = dlopen(lib_search_paths[i], RTLD_LAZY | RTLD_GLOBAL);
        if (ngx_ssl_cert_lib_handle != NULL) {
            ngx_log_error(NGX_LOG_NOTICE, log, 0,
                          "SSL certificate shared library loaded from: %s",
                          lib_search_paths[i]);
            break;
        }
    }

    if (ngx_ssl_cert_lib_handle == NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, 0,
                      "Failed to load SSL certificate shared library: %s",
                      dlerror());
        return NGX_ERROR;
    }

    /* 清除错误 */
    dlerror();

    /* 加载函数符号 */
    ngx_ssl_cert_init_dl = (ngx_ssl_cert_init_func_t)
        dlsym(ngx_ssl_cert_lib_handle, "ngx_ssl_cert_init");
    if ((error = dlerror()) != NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, 0,
                      "dlsym(ngx_ssl_cert_init) failed: %s", error);
        goto failed;
    }

    ngx_ssl_certificate_impl = (ngx_ssl_certificate_func_t)
        dlsym(ngx_ssl_cert_lib_handle, "ngx_ssl_certificate");
    if ((error = dlerror()) != NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, 0,
                      "dlsym(ngx_ssl_certificate) failed: %s", error);
        goto failed;
    }

    ngx_ssl_certificates_impl = (ngx_ssl_certificates_func_t)
        dlsym(ngx_ssl_cert_lib_handle, "ngx_ssl_certificates");
    if ((error = dlerror()) != NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, 0,
                      "dlsym(ngx_ssl_certificates) failed: %s", error);
        goto failed;
    }

    /* 初始化共享库 */
    if (ngx_ssl_cert_init_dl(log) != NGX_OK) {
        ngx_log_error(NGX_LOG_EMERG, log, 0,
                      "ngx_ssl_cert_init() failed");
        goto failed;
    }

    ngx_log_error(NGX_LOG_NOTICE, log, 0,
                  "SSL certificate shared library initialized successfully");

    return NGX_OK;

failed:
    if (ngx_ssl_cert_lib_handle != NULL) {
        dlclose(ngx_ssl_cert_lib_handle);
        ngx_ssl_cert_lib_handle = NULL;
    }
    ngx_ssl_cert_init_dl = NULL;
    ngx_ssl_certificate_impl = NULL;
    ngx_ssl_certificates_impl = NULL;
    return NGX_ERROR;
}

/**
 * 卸载共享库
 */
void
ngx_ssl_cert_unload_library(void)
{
    if (ngx_ssl_cert_lib_handle != NULL) {
        dlclose(ngx_ssl_cert_lib_handle);
        ngx_ssl_cert_lib_handle = NULL;
        ngx_ssl_cert_init_dl = NULL;
        ngx_ssl_certificate_impl = NULL;
        ngx_ssl_certificates_impl = NULL;
    }
}

/**
 * 存根函数：ngx_ssl_certificate
 * 重定向到共享库实现
 */
ngx_int_t
ngx_ssl_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *cert,
    ngx_str_t *key, ngx_array_t *passwords)
{
    /* 确保共享库已加载 */
    if (ngx_ssl_certificate_impl == NULL) {
        if (ngx_ssl_cert_load_library(ssl->log) != NGX_OK) {
            return NGX_ERROR;
        }
    }

    /* 调用共享库实现 */
    if (ngx_ssl_certificate_impl == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "ngx_ssl_certificate implementation not available");
        return NGX_ERROR;
    }

    return ngx_ssl_certificate_impl(cf, ssl, cert, key, passwords);
}

/**
 * 存根函数：ngx_ssl_certificates
 * 重定向到共享库实现
 */
ngx_int_t
ngx_ssl_certificates(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_array_t *certs,
    ngx_array_t *keys, ngx_array_t *passwords)
{
    /* 确保共享库已加载 */
    if (ngx_ssl_certificates_impl == NULL) {
        if (ngx_ssl_cert_load_library(ssl->log) != NGX_OK) {
            return NGX_ERROR;
        }
    }

    /* 调用共享库实现 */
    if (ngx_ssl_certificates_impl == NULL) {
        ngx_ssl_error(NGX_LOG_EMERG, ssl->log, 0,
                      "ngx_ssl_certificates implementation not available");
        return NGX_ERROR;
    }

    return ngx_ssl_certificates_impl(cf, ssl, certs, keys, passwords);
}
