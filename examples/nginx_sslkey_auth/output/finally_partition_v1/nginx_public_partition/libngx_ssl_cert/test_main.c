/*
 * test_main.c
 * 
 * 测试 libngx_ssl_cert.so 共享库
 */

#include <stdio.h>
#include <stdlib.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include "ngx_ssl_cert_wrapper.h"

int main(int argc, char *argv[])
{
    ngx_log_t       log;
    ngx_ssl_t       ssl;
    ngx_conf_t      cf;
    ngx_cycle_t     cycle;
    ngx_str_t       cert, key;
    ngx_int_t       rc;

    printf("=== 测试 libngx_ssl_cert.so ===\n\n");

    /* 初始化日志 */
    ngx_memzero(&log, sizeof(ngx_log_t));
    log.log_level = NGX_LOG_DEBUG;
    log.file = ngx_stderr;

    /* 初始化 cycle */
    ngx_memzero(&cycle, sizeof(ngx_cycle_t));
    cycle.log = &log;
    cycle.new_log = &log;
    cycle.prefix.len = sizeof("/tmp") - 1;
    cycle.prefix.data = (u_char *) "/tmp";

    /* 初始化配置 */
    ngx_memzero(&cf, sizeof(ngx_conf_t));
    cf.cycle = &cycle;
    cf.log = &log;

    /* 初始化 SSL */
    ngx_memzero(&ssl, sizeof(ngx_ssl_t));
    ssl.log = &log;

    /* 初始化证书库 */
    printf("1. 初始化证书库...\n");
    rc = ngx_ssl_cert_init(&log);
    if (rc != NGX_OK) {
        fprintf(stderr, "错误: 初始化失败\n");
        return 1;
    }
    printf("   成功: 证书库初始化完成\n\n");

    /* 创建 SSL 上下文 */
    printf("2. 创建 SSL 上下文...\n");
    ssl.ctx = SSL_CTX_new(SSLv23_server_method());
    if (ssl.ctx == NULL) {
        fprintf(stderr, "错误: SSL_CTX_new() 失败\n");
        return 1;
    }
    printf("   成功: SSL 上下文创建完成\n\n");

    /* 测试加载证书 */
    if (argc >= 3) {
        printf("3. 加载证书和私钥...\n");
        cert.data = (u_char *) argv[1];
        cert.len = strlen(argv[1]);
        key.data = (u_char *) argv[2];
        key.len = strlen(argv[2]);

        printf("   证书文件: %s\n", argv[1]);
        printf("   私钥文件: %s\n", argv[2]);

        rc = ngx_ssl_certificate(&cf, &ssl, &cert, &key, NULL);
        if (rc != NGX_OK) {
            fprintf(stderr, "错误: 加载证书失败\n");
            SSL_CTX_free(ssl.ctx);
            return 1;
        }
        printf("   成功: 证书和私钥加载完成\n\n");
    } else {
        printf("3. 跳过证书加载测试（需要提供证书和私钥路径）\n");
        printf("   用法: %s <cert.pem> <key.pem>\n\n", argv[0]);
    }

    /* 清理 */
    printf("4. 清理资源...\n");
    SSL_CTX_free(ssl.ctx);
    printf("   成功: 资源清理完成\n\n");

    printf("=== 测试完成 ===\n");

    return 0;
}
