/*
 * ngx_ssl_cert_wrapper.h
 * 
 * 共享库头文件，导出 ngx_ssl_certificate 及相关函数
 */

#ifndef _NGX_SSL_CERT_WRAPPER_H_
#define _NGX_SSL_CERT_WRAPPER_H_

#include <ngx_config.h>
#include <ngx_core.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/bio.h>
#include <openssl/pem.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 导出的全局变量 */
extern int ngx_ssl_certificate_index;
extern int ngx_ssl_certificate_name_index;
extern int ngx_ssl_next_certificate_index;

/* 核心函数声明 */

/**
 * 初始化 SSL 证书相关的全局索引
 * 必须在使用其他函数前调用
 * 
 * @param log nginx 日志对象
 * @return NGX_OK 成功，NGX_ERROR 失败
 */
ngx_int_t ngx_ssl_cert_init(ngx_log_t *log);

/**
 * 加载 SSL 证书和私钥
 * 
 * @param cf nginx 配置对象
 * @param ssl SSL 上下文
 * @param cert 证书文件路径
 * @param key 私钥文件路径
 * @param passwords 密码数组（可选）
 * @return NGX_OK 成功，NGX_ERROR 失败
 */
ngx_int_t ngx_ssl_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl, 
                               ngx_str_t *cert, ngx_str_t *key, 
                               ngx_array_t *passwords);

/**
 * 批量加载多个证书
 * 
 * @param cf nginx 配置对象
 * @param ssl SSL 上下文
 * @param certs 证书文件路径数组
 * @param keys 私钥文件路径数组
 * @param passwords 密码数组（可选）
 * @return NGX_OK 成功，NGX_ERROR 失败
 */
ngx_int_t ngx_ssl_certificates(ngx_conf_t *cf, ngx_ssl_t *ssl,
                                ngx_array_t *certs, ngx_array_t *keys,
                                ngx_array_t *passwords);

/* 辅助函数 */

/**
 * SSL 错误日志
 * 
 * @param level 日志级别
 * @param log 日志对象
 * @param err 错误码
 * @param fmt 格式化字符串
 */
void ngx_ssl_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err, 
                   char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* _NGX_SSL_CERT_WRAPPER_H_ */
