# 将 SSL_CTX 从 nginx_sslkey_partition 传递到 nginx_public_partition

## 问题描述

SSL_CTX 是一个复杂的 OpenSSL 内存结构，包含大量指针和内部状态，**无法直接通过 RPC 序列化传输**。

## 解决方案：传递证书内容而非 SSL_CTX

### 架构设计

```
nginx_public_partition (RPC Client)                nginx_sslkey_partition (RPC Server)
┌────────────────────────────┐                    ┌──────────────────────────────┐
│                            │                    │                              │
│  1. 需要加载证书           │ RPC Request        │  3. 读取证书文件             │
│     cert_path, key_path    │ ──────────────────>│     cert.pem, key.pem        │
│                            │                    │                              │
│                            │                    │  4. 读取文件内容到内存       │
│                            │                    │     char* cert_pem           │
│                            │                    │     char* key_pem            │
│                            │                    │                              │
│  6. 从内存加载证书到本地   │ RPC Response       │  5. 返回 PEM 内容            │
│     SSL_CTX                │ <──────────────────│     {cert_data, key_data}    │
│                            │                    │                              │
│  7. 使用 BIO_new_mem_buf   │                    └──────────────────────────────┘
│     + PEM_read_bio_XXX     │
│                            │
│  8. SSL_CTX_use_certificate│
│     SSL_CTX_use_PrivateKey │
│                            │
│  9. 进行 SSL 握手          │
│                            │
└────────────────────────────┘
```

### 关键要点

1. **不传递 SSL_CTX 指针**：SSL_CTX 在另一个进程的地址空间无效
2. **传递 PEM 格式内容**：以字符串形式传递证书和私钥的 PEM 内容
3. **在客户端加载**：nginx_public_partition 在本地创建 SSL_CTX 并加载证书
4. **SSL 握手在客户端**：所有 SSL/TLS 操作都在 nginx_public_partition 中进行

## 实现步骤

### 步骤 1：修改 RPC 服务器（nginx_sslkey_partition）

文件：`nginx_sslkey_partion/src/core/sslkey_auth_idl_server.c`

**功能**：读取证书和私钥文件，返回 PEM 格式内容

```c
ngx_ssl_certificate_response *
ngx_ssl_certificate_rpc_1_1_svc(ngx_ssl_certificate_request arg1,  struct svc_req *rqstp)
{
    // 1. 解析请求参数（cert_path, key_path）
    // 2. 使用 fopen + fread 读取证书文件内容
    // 3. 使用 fopen + fread 读取私钥文件内容
    // 4. 返回响应：
    //    response.status = 0 (成功)
    //    response.cert_data = 证书 PEM 内容
    //    response.key_data = 私钥 PEM 内容
}
```

参考实现：`sslkey_auth_idl_server_new.c`（已创建）

### 步骤 2：在 RPC 客户端加载证书（nginx_public_partition）

文件：`nginx_public_partition/ssl_load_from_memory.c`（已创建）

**关键函数**：

#### 2.1 从内存加载证书

```c
ngx_int_t 
ngx_ssl_certificate_from_memory(ngx_ssl_t *ssl, 
                                const char *cert_pem, size_t cert_len,
                                const char *key_pem, size_t key_len,
                                ngx_log_t *log)
{
    // 1. 创建内存 BIO：BIO_new_mem_buf(cert_pem, cert_len)
    // 2. 解析证书：PEM_read_bio_X509_AUX()
    // 3. 加载到 SSL_CTX：SSL_CTX_use_certificate()
    // 4. 加载证书链：循环 PEM_read_bio_X509() + SSL_CTX_add0_chain_cert()
    // 5. 解析私钥：PEM_read_bio_PrivateKey()
    // 6. 加载私钥：SSL_CTX_use_PrivateKey()
    // 7. 验证匹配：SSL_CTX_check_private_key()
}
```

#### 2.2 通过 RPC 加载

```c
ngx_int_t
load_ssl_certificate_via_rpc(ngx_ssl_t *ssl, 
                             const char *cert_path,
                             const char *key_path,
                             ngx_log_t *log)
{
    // 1. 创建 RPC 客户端
    // 2. 准备请求参数
    // 3. 调用 RPC：response = ngx_ssl_certificate_rpc_1(&request, clnt)
    // 4. 检查响应状态
    // 5. 调用 ngx_ssl_certificate_from_memory() 加载到本地 SSL_CTX
    // 6. 清理资源
}
```

### 步骤 3：集成到 nginx 配置加载流程

在 `nginx_public_partition` 的 SSL 配置处理代码中，替换原来的 `ngx_ssl_certificate()` 调用：

```c
// 原来的调用（直接读取文件）
// ngx_ssl_certificate(cf, ssl, &cert_path, &key_path, passwords);

// 新的调用（通过 RPC）
load_ssl_certificate_via_rpc(ssl, cert_path_str, key_path_str, cf->log);
```

## OpenSSL API 对比

### 原始方式（从文件加载）

```c
// ngx_ssl_certificate() 中使用
BIO *bio = BIO_new_file(cert_file, "r");
X509 *x509 = PEM_read_bio_X509_AUX(bio, NULL, NULL, NULL);
SSL_CTX_use_certificate(ctx, x509);
```

### 新方式（从内存加载）

```c
// 从 RPC 返回的内存加载
BIO *bio = BIO_new_mem_buf(cert_pem, cert_len);  // 区别：内存 BIO
X509 *x509 = PEM_read_bio_X509_AUX(bio, NULL, NULL, NULL);
SSL_CTX_use_certificate(ctx, x509);
```

**唯一的区别**：BIO 的来源（文件 vs 内存），其他流程完全相同。

## 数据流示例

### RPC 请求

```json
{
  "cert_path": "ssl/cert.pem",
  "key_path": "ssl/cert.key",
  "passwords": "",
  "passwords_count": 0
}
```

### RPC 响应

```json
{
  "status": 0,
  "error_msg": "Success",
  "cert_data": "-----BEGIN CERTIFICATE-----\nMIIDXTCCAkWgAwIBAgI...\n-----END CERTIFICATE-----\n",
  "key_data": "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhki...\n-----END PRIVATE KEY-----\n"
}
```

## 编译和链接

### nginx_sslkey_partition (RPC Server)

```bash
cd nginx_sslkey_partion/src/core
gcc -c sslkey_auth_idl_server_new.c -I../../objs -I../.. -I../../src/core \
    -I../../src/event -I../../src/event/modules -I../../src/os/unix
```

### nginx_public_partition (RPC Client)

```bash
cd nginx_public_partition
gcc -c ssl_load_from_memory.c -I./objs -I. -I./src/core \
    -I./src/event -I./src/event/modules -I./src/os/unix \
    -lssl -lcrypto
```

## 优势

1. ✅ **隔离证书文件**：只有 nginx_sslkey_partition 能访问证书文件
2. ✅ **SSL_CTX 在正确进程**：证书加载到 nginx_public_partition 的 SSL_CTX
3. ✅ **SSL 握手正常**：握手在拥有证书的进程中进行
4. ✅ **安全性提升**：证书文件权限可以严格限制

## 劣势

1. ❌ **网络开销**：证书内容通过 RPC 传输（通常几 KB）
2. ❌ **内存占用**：证书内容在传输时需要额外内存
3. ❌ **复杂度增加**：需要维护额外的内存加载代码

## 测试步骤

1. 编译新的 RPC 服务器和客户端代码
2. 启动 nginx_sslkey_partition 的 RPC 服务器
3. 启动 nginx_public_partition，触发 SSL 配置加载
4. 检查日志确认证书加载成功
5. 使用 `curl https://localhost:443` 测试 SSL 握手

## 常见问题

### Q: 为什么不直接让 nginx_public_partition 读取文件？
A: 为了安全隔离。nginx_sslkey_partition 专门负责证书管理，可以设置更严格的权限。

### Q: 证书内容在网络上传输安全吗？
A: 当前使用 localhost UDP，相对安全。生产环境应考虑使用 Unix domain socket 或加密 RPC。

### Q: 性能影响如何？
A: 证书只在启动时加载一次，运行时无影响。传输几 KB 数据的开销可以忽略。

### Q: 可以缓存证书内容吗？
A: 可以在 nginx_public_partition 中缓存，但证书更新时需要重载。

## 相关文件

- ✅ `sslkey_auth_idl_server_new.c` - RPC 服务器实现（读取文件）
- ✅ `ssl_load_from_memory.c` - RPC 客户端实现（从内存加载）
- 📄 `IDL/sslkey_auth_idl/sslkey_auth_idl.x` - RPC 接口定义
- 📄 `nginx_public_partition/src/http/ngx_http.c` - 集成点（需要修改）

## 下一步

1. 替换 `sslkey_auth_idl_server.c` 为 `sslkey_auth_idl_server_new.c`
2. 在 nginx_public_partition 中集成 `ssl_load_from_memory.c`
3. 修改 SSL 配置加载代码调用 `load_ssl_certificate_via_rpc()`
4. 重新编译并测试
