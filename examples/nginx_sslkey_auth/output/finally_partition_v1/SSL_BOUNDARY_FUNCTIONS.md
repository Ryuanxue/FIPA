# SSL 握手功能边界函数说明

## 问题：public 域应该调用哪个边界函数？

如果要将 SSL 握手功能划分到 sslkey 域，**public 域应该调用的关键边界函数是 `ngx_ssl_handshake()`**。

## 边界函数分析

### 1. 核心边界函数：`ngx_ssl_handshake()`

**位置**：`nginx_public_partition/src/event/ngx_event_openssl.c:1502`

**函数签名**：
```c
ngx_int_t ngx_ssl_handshake(ngx_connection_t *c)
```

**功能**：
- 执行 SSL/TLS 握手过程
- 内部调用 `SSL_do_handshake(c->ssl->connection)` (第 1515 行)
- 这是握手的核心操作点

**调用链**：
```
nginx HTTP 请求处理
    ↓
ngx_http_ssl_handshake()           // HTTP 模块层
    ↓
ngx_ssl_handshake()                // ← 这是边界函数！
    ↓
SSL_do_handshake()                 // OpenSSL 库
```

### 2. 需要迁移到 sslkey 域的内容

当前 `ngx_ssl_handshake()` 函数中的关键操作：

```c
// 第 1515 行 - 这是需要迁移的核心
n = SSL_do_handshake(c->ssl->connection);

// 握手成功后 (n == 1)
if (n == 1) {
    c->ssl->handshaked = 1;
    
    // 设置 SSL 读写函数指针
    c->recv = ngx_ssl_recv;           // SSL_read
    c->send = ngx_ssl_write;          // SSL_write
    c->recv_chain = ngx_ssl_recv_chain;
    c->send_chain = ngx_ssl_send_chain;
    
    return NGX_OK;
}
```

## 实现方案

### 方案 A：直接替换 `ngx_ssl_handshake()` 为 RPC 调用

在 public 域中修改 `ngx_ssl_handshake()` 函数：

```c
ngx_int_t
ngx_ssl_handshake(ngx_connection_t *c)
{
    // ===== 原来的实现 =====
    // n = SSL_do_handshake(c->ssl->connection);
    
    // ===== 新的实现：通过 RPC 调用 sslkey 域 =====
    
    // 1. 获取客户端 socket fd
    int client_fd = c->fd;
    
    // 2. 通过 Unix Socket 传递 fd 到 sslkey 域
    int unix_sock = connect_unix_server();
    if (unix_sock < 0) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0, 
                      "Failed to connect to sslkey domain");
        return NGX_ERROR;
    }
    
    // 3. 发送 fd
    if (send_fd(unix_sock, client_fd) < 0) {
        close(unix_sock);
        return NGX_ERROR;
    }
    
    // 4. 等待 sslkey 域完成握手
    int result;
    if (read(unix_sock, &result, sizeof(result)) != sizeof(result)) {
        close(unix_sock);
        return NGX_ERROR;
    }
    
    close(unix_sock);
    
    // 5. 检查握手结果
    if (result == 0) {  // 握手成功
        c->ssl->handshaked = 1;
        
        // 设置读写函数为 RPC 版本
        c->recv = ngx_ssl_recv_rpc;           // 通过 RPC 读取
        c->send = ngx_ssl_write_rpc;          // 通过 RPC 写入
        c->recv_chain = ngx_ssl_recv_chain_rpc;
        c->send_chain = ngx_ssl_send_chain_rpc;
        
        return NGX_OK;
    } else if (result == SSL_ERROR_WANT_READ || 
               result == SSL_ERROR_WANT_WRITE) {
        // 需要继续握手
        c->read->handler = ngx_ssl_handshake_handler;
        c->write->handler = ngx_ssl_handshake_handler;
        return NGX_AGAIN;
    } else {
        // 握手失败
        ngx_log_error(NGX_LOG_ERR, c->log, 0, 
                      "SSL handshake failed in sslkey domain");
        return NGX_ERROR;
    }
}
```

### 方案 B：保持架构清晰 - 使用 SSL 代理模式（推荐）

**不需要修改边界函数**，而是让 nginx_sslkey_partition 作为独立的 SSL 前端：

```
Client ──HTTPS(443)──> nginx_sslkey (SSL 握手在这里)
                           │
                           │ 解密后转发
                           ↓
                       HTTP(8080) ──> nginx_public (处理业务逻辑)
```

在这种模式下：
- **nginx_sslkey** 独立运行，完整处理 SSL 握手和加解密
- **nginx_public** 只处理明文 HTTP 请求
- **无需修改任何边界函数**

## 相关边界函数清单

如果采用方案 A（RPC 方式），需要处理的所有边界函数：

### 1. 握手相关
```c
ngx_int_t ngx_ssl_handshake(ngx_connection_t *c);              // 主要边界
static void ngx_ssl_handshake_handler(ngx_event_t *ev);        // 辅助
```

### 2. 读写相关（握手后的数据传输）
```c
ssize_t ngx_ssl_recv(ngx_connection_t *c, u_char *buf, size_t size);
ngx_chain_t *ngx_ssl_recv_chain(ngx_connection_t *c, ngx_chain_t *in, 
                                 off_t limit);
ssize_t ngx_ssl_write(ngx_connection_t *c, u_char *data, size_t size);
ngx_chain_t *ngx_ssl_send_chain(ngx_connection_t *c, ngx_chain_t *in, 
                                 off_t limit);
```

### 3. 关闭相关
```c
ngx_int_t ngx_ssl_shutdown(ngx_connection_t *c);
static void ngx_ssl_shutdown_handler(ngx_event_t *ev);
```

## 调用位置追踪

### HTTP 模块中的调用点

**文件**：`nginx_public_partition/src/http/ngx_http_request.c`

```c
static void
ngx_http_ssl_handshake(ngx_event_t *rev)
{
    // ...
    
    // 关键调用点
    rc = ngx_ssl_handshake(c);  // ← 这里调用边界函数
    
    if (rc == NGX_AGAIN) {
        // 握手未完成，等待
        return;
    }
    
    if (rc == NGX_ERROR) {
        // 握手失败
        ngx_http_close_connection(c);
        return;
    }
    
    // 握手成功，继续处理 HTTP 请求
    ngx_http_ssl_handshake_handler(c);
}
```

## 实施建议

### 推荐：方案 B（SSL 代理模式）

**优点**：
1. ✅ **边界清晰**：完全独立的两个 nginx 进程
2. ✅ **无需修改代码**：使用标准 nginx 配置
3. ✅ **易于调试**：各自独立运行
4. ✅ **易于维护**：配置简单明了

**实施步骤**：
1. 使用已创建的配置文件：
   - `nginx_sslkey_partion/conf/nginx-ssl-proxy.conf`
   - `nginx_public_partition/conf/nginx-app.conf`

2. 启动脚本：
   ```bash
   ./start_ssl_proxy.sh
   ```

3. 测试：
   ```bash
   curl -k https://localhost/admin/
   ```

### 备选：方案 A（RPC + 文件描述符传递）

**仅在有特殊需求时使用**（如需要更细粒度的控制）

**实施步骤**：
1. 实现 Unix Domain Socket 的 fd 传递（参考 `SSL_HANDSHAKE_IN_SSLKEY_GUIDE.md`）
2. 在 sslkey 域实现 SSL 握手处理器
3. 修改 `ngx_ssl_handshake()` 调用 RPC
4. 实现 RPC 版本的读写函数：
   - `ngx_ssl_recv_rpc()`
   - `ngx_ssl_write_rpc()`

## 总结

**边界函数**：`ngx_ssl_handshake(ngx_connection_t *c)`

**位置**：`nginx_public_partition/src/event/ngx_event_openssl.c:1502`

**调用它的上层函数**：
- `ngx_http_ssl_handshake()` (HTTP 层)
- `ngx_http_ssl_handshake_handler()` (HTTP 层)

**它调用的下层函数**：
- `SSL_do_handshake()` ← 这是真正执行握手的 OpenSSL 函数

**建议**：使用 SSL 代理模式（方案 B），无需修改任何代码，配置即可实现域隔离。
