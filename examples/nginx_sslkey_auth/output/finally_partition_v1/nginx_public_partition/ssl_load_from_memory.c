/*
 * 在 nginx_public_partition 中加载RPC返回的证书内容
 * 
 * 使用步骤：
 * 1. 通过RPC获取证书和私钥的PEM内容
 * 2. 使用BIO_new_mem_buf创建内存BIO
 * 3. 使用PEM_read_bio_X509和PEM_read_bio_PrivateKey解析
 * 4. 加载到SSL_CTX中
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event_openssl.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/bio.h>

/* 从内存加载证书到SSL_CTX */
ngx_int_t 
ngx_ssl_certificate_from_memory(ngx_ssl_t *ssl, 
                                const char *cert_pem, size_t cert_len,
                                const char *key_pem, size_t key_len,
                                ngx_log_t *log)
{
    BIO *cert_bio = NULL;
    BIO *key_bio = NULL;
    X509 *x509 = NULL;
    EVP_PKEY *pkey = NULL;
    int ret = NGX_ERROR;
    
    if (!ssl || !ssl->ctx || !cert_pem || !key_pem) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "Invalid parameters");
        return NGX_ERROR;
    }
    
    ngx_log_error(NGX_LOG_INFO, log, 0, 
                  "Loading certificate from memory (cert_len=%uz, key_len=%uz)",
                  cert_len, key_len);
    
    /* 1. 从内存创建证书BIO */
    cert_bio = BIO_new_mem_buf(cert_pem, cert_len);
    if (!cert_bio) {
        ngx_ssl_error(NGX_LOG_EMERG, log, 0, "BIO_new_mem_buf() failed for certificate");
        goto cleanup;
    }
    
    /* 2. 解析PEM格式的证书 */
    x509 = PEM_read_bio_X509_AUX(cert_bio, NULL, NULL, NULL);
    if (!x509) {
        ngx_ssl_error(NGX_LOG_EMERG, log, 0, "PEM_read_bio_X509_AUX() failed");
        goto cleanup;
    }
    
    /* 3. 将证书加载到SSL_CTX */
    if (SSL_CTX_use_certificate(ssl->ctx, x509) == 0) {
        ngx_ssl_error(NGX_LOG_EMERG, log, 0, "SSL_CTX_use_certificate() failed");
        goto cleanup;
    }
    
    ngx_log_error(NGX_LOG_INFO, log, 0, "Certificate loaded successfully");
    
    /* 4. 加载证书链（如果有） */
    X509 *ca;
    while ((ca = PEM_read_bio_X509(cert_bio, NULL, NULL, NULL)) != NULL) {
        if (SSL_CTX_add0_chain_cert(ssl->ctx, ca) == 0) {
            X509_free(ca);
            ngx_ssl_error(NGX_LOG_EMERG, log, 0, "SSL_CTX_add0_chain_cert() failed");
            goto cleanup;
        }
        ngx_log_error(NGX_LOG_INFO, log, 0, "Chain certificate added");
    }
    
    /* 5. 从内存创建私钥BIO */
    key_bio = BIO_new_mem_buf(key_pem, key_len);
    if (!key_bio) {
        ngx_ssl_error(NGX_LOG_EMERG, log, 0, "BIO_new_mem_buf() failed for key");
        goto cleanup;
    }
    
    /* 6. 解析PEM格式的私钥 */
    pkey = PEM_read_bio_PrivateKey(key_bio, NULL, NULL, NULL);
    if (!pkey) {
        ngx_ssl_error(NGX_LOG_EMERG, log, 0, "PEM_read_bio_PrivateKey() failed");
        goto cleanup;
    }
    
    /* 7. 将私钥加载到SSL_CTX */
    if (SSL_CTX_use_PrivateKey(ssl->ctx, pkey) == 0) {
        ngx_ssl_error(NGX_LOG_EMERG, log, 0, "SSL_CTX_use_PrivateKey() failed");
        goto cleanup;
    }
    
    /* 8. 验证私钥和证书是否匹配 */
    if (SSL_CTX_check_private_key(ssl->ctx) == 0) {
        ngx_ssl_error(NGX_LOG_EMERG, log, 0, "SSL_CTX_check_private_key() failed");
        goto cleanup;
    }
    
    ngx_log_error(NGX_LOG_INFO, log, 0, "Private key loaded and verified successfully");
    ret = NGX_OK;
    
cleanup:
    if (cert_bio) BIO_free(cert_bio);
    if (key_bio) BIO_free(key_bio);
    if (x509) X509_free(x509);
    if (pkey) EVP_PKEY_free(pkey);
    
    return ret;
}

/* 使用示例：在nginx_public_partition的SSL配置初始化时调用 */
ngx_int_t
load_ssl_certificate_via_rpc(ngx_ssl_t *ssl, 
                             const char *cert_path,
                             const char *key_path,
                             ngx_log_t *log)
{
    CLIENT *clnt;
    ngx_ssl_certificate_request request;
    ngx_ssl_certificate_response *response;
    ngx_int_t rc = NGX_ERROR;
    
    /* 1. 创建RPC客户端连接 */
    clnt = clnt_create("localhost", RPCSSLKEY, RPCSSLKEYTVERS, "udp");
    if (!clnt) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, "Failed to create RPC client");
        return NGX_ERROR;
    }
    
    /* 2. 准备RPC请求参数 */
    memset(&request, 0, sizeof(request));
    
    request.cert_path.len = strlen(cert_path);
    request.cert_path.data.charptr.charptr_len = strlen(cert_path);
    request.cert_path.data.charptr.charptr_val = (char*)cert_path;
    
    request.key_path.len = strlen(key_path);
    request.key_path.data.charptr.charptr_len = strlen(key_path);
    request.key_path.data.charptr.charptr_val = (char*)key_path;
    
    request.passwords.charptr.charptr_len = 0;
    request.passwords.charptr.charptr_val = NULL;
    request.passwords_count = 0;
    
    /* 3. 调用RPC获取证书内容 */
    response = ngx_ssl_certificate_rpc_1(&request, clnt);
    if (!response) {
        clnt_perror(clnt, "RPC call failed");
        clnt_destroy(clnt);
        return NGX_ERROR;
    }
    
    /* 4. 检查响应状态 */
    if (response->status != 0) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, 
                      "RPC returned error: %s",
                      response->error_msg.charptr.charptr_val);
        clnt_destroy(clnt);
        return NGX_ERROR;
    }
    
    /* 5. 从内存加载证书和私钥到SSL_CTX */
    rc = ngx_ssl_certificate_from_memory(
        ssl,
        response->cert_data.charptr.charptr_val,
        response->cert_data.charptr.charptr_len,
        response->key_data.charptr.charptr_val,
        response->key_data.charptr.charptr_len,
        log
    );
    
    if (rc != NGX_OK) {
        ngx_log_error(NGX_LOG_EMERG, log, 0, 
                      "Failed to load certificate from memory");
    } else {
        ngx_log_error(NGX_LOG_INFO, log, 0, 
                      "Certificate loaded via RPC successfully");
    }
    
    /* 6. 清理 */
    clnt_destroy(clnt);
    
    return rc;
}
