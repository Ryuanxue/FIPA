# SSL 握手边界函数调用链

## 完整调用链（从客户端连接到握手完成）

```
┌─────────────────────────────────────────────────────────────────────────┐
│                         Client 发起 HTTPS 请求                           │
│                              TCP 三次握手                                │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  nginx_public_partition 接收连接                                         │
│                                                                         │
│  [1] ngx_http_init_connection(ngx_connection_t *c)                     │
│      位置: src/http/ngx_http_request.c:287                             │
│      功能: 初始化 HTTP 连接                                             │
│                                                                         │
│      检测到 SSL 配置                                                     │
│      ↓                                                                  │
│  [2] rev->handler = ngx_http_ssl_handshake;                           │
│      设置读事件处理器为 SSL 握手函数                                     │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼ (事件触发)
┌─────────────────────────────────────────────────────────────────────────┐
│  [3] ngx_http_ssl_handshake(ngx_event_t *rev)                          │
│      位置: src/http/ngx_http_request.c:737                             │
│      功能: HTTP 层的 SSL 握手处理                                        │
│      层次: HTTP 协议层                                                   │
│                                                                         │
│      ▼ 调用 SSL 层边界函数                                              │
│  [4] rc = ngx_ssl_handshake(c);  ◄─── 这是关键边界函数！               │
│      ├─ 返回 NGX_AGAIN → 握手未完成，继续等待                          │
│      ├─ 返回 NGX_OK    → 握手成功，调用 [5]                            │
│      └─ 返回 NGX_ERROR → 握手失败，关闭连接                            │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  ╔═══════════════════════════════════════════════════════════════╗     │
│  ║  [4] ngx_ssl_handshake(ngx_connection_t *c)                  ║     │
│  ║      位置: src/event/ngx_event_openssl.c:1502                ║     │
│  ║      功能: 执行 SSL/TLS 握手                                   ║     │
│  ║      层次: SSL/TLS 层（边界函数）                              ║     │
│  ║                                                               ║     │
│  ║  这是 public 域和 sslkey 域的边界点！                          ║     │
│  ╚═══════════════════════════════════════════════════════════════╝     │
│                                                                         │
│  当前实现（在 public 域）：                                              │
│  ┌───────────────────────────────────────────────────────────┐         │
│  │ n = SSL_do_handshake(c->ssl->connection);  // 第 1515 行  │         │
│  │                                                           │         │
│  │ if (n == 1) {  // 握手成功                               │         │
│  │     c->ssl->handshaked = 1;                             │         │
│  │     c->recv = ngx_ssl_recv;         // SSL_read 封装     │         │
│  │     c->send = ngx_ssl_write;        // SSL_write 封装    │         │
│  │     c->recv_chain = ngx_ssl_recv_chain;                 │         │
│  │     c->send_chain = ngx_ssl_send_chain;                 │         │
│  │     return NGX_OK;                                       │         │
│  │ }                                                        │         │
│  │                                                           │         │
│  │ // 处理 SSL_ERROR_WANT_READ / SSL_ERROR_WANT_WRITE      │         │
│  │ // 设置事件处理器继续握手                                 │         │
│  │ c->read->handler = ngx_ssl_handshake_handler;           │         │
│  │ c->write->handler = ngx_ssl_handshake_handler;          │         │
│  │ return NGX_AGAIN;                                        │         │
│  └───────────────────────────────────────────────────────────┘         │
│                                                                         │
│  ▼ 握手成功后返回                                                       │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼ (rc == NGX_OK)
┌─────────────────────────────────────────────────────────────────────────┐
│  [5] ngx_http_ssl_handshake_handler(c)                                 │
│      位置: src/http/ngx_http_request.c:796                             │
│      功能: 握手完成后，开始处理 HTTP 请求                                │
│                                                                         │
│      ▼                                                                  │
│  [6] ngx_http_process_request_line(rev)                                │
│      开始解析 HTTP 请求行（GET /admin HTTP/1.1）                         │
└─────────────────────────────────────────────────────────────────────────┘
```

## 边界函数的上下文

### 边界函数：`ngx_ssl_handshake()`

```c
/* 位置：nginx_public_partition/src/event/ngx_event_openssl.c:1502 */

ngx_int_t
ngx_ssl_handshake(ngx_connection_t *c)
{
    int        n, sslerr;
    ngx_err_t  err;

    // ============ 边界点：这里是关键 ============
    // 当前：直接调用 OpenSSL
    n = SSL_do_handshake(c->ssl->connection);
    
    // 如果要迁移到 sslkey 域，需要：
    // 1. 通过 Unix Socket 传递 c->fd 到 sslkey 域
    // 2. sslkey 域执行 SSL_do_handshake()
    // 3. 返回结果给 public 域
    // ============================================

    if (n == 1) {
        // 握手成功
        c->ssl->handshaked = 1;
        
        // 设置后续的读写函数
        c->recv = ngx_ssl_recv;           // 后续也需要迁移
        c->send = ngx_ssl_write;          // 后续也需要迁移
        c->recv_chain = ngx_ssl_recv_chain;
        c->send_chain = ngx_ssl_send_chain;
        
        return NGX_OK;
    }

    // 处理握手未完成或错误
    sslerr = SSL_get_error(c->ssl->connection, n);
    
    if (sslerr == SSL_ERROR_WANT_READ || 
        sslerr == SSL_ERROR_WANT_WRITE) {
        // 非阻塞握手，需要继续
        c->read->handler = ngx_ssl_handshake_handler;
        c->write->handler = ngx_ssl_handshake_handler;
        return NGX_AGAIN;
    }

    // 握手失败
    return NGX_ERROR;
}
```

## 迁移到 sslkey 域后的调用链

```
┌──────────────────────────────┐        ┌──────────────────────────────┐
│   nginx_public_partition     │        │   nginx_sslkey_partition     │
│                              │        │                              │
│  [1] ngx_http_ssl_handshake  │        │                              │
│       ↓                      │        │                              │
│  [2] ngx_ssl_handshake       │        │                              │
│       ↓                      │        │                              │
│  [3] 获取 client_fd          │        │                              │
│       ↓                      │        │                              │
│  [4] connect_unix_server()   │───────>│  accept() 接收连接           │
│       ↓                      │        │       ↓                      │
│  [5] send_fd(client_fd) ────>│───────>│  recv_fd() 接收 fd           │
│       ↓                      │        │       ↓                      │
│  [6] 等待结果...             │        │  SSL_do_handshake()  ◄───┐  │
│       │                      │        │       ↓                  │  │
│       │                      │        │  if (n == 1) {           │  │
│       │                      │        │      return success;     │  │
│       │                      │        │  }                       │  │
│       │                      │        │  if (need retry) {       │  │
│       │                      │        │      return AGAIN; ──────┘  │
│       │                      │        │  }                          │
│       │                      │        │       ↓                      │
│  [7] read(result) <──────────│<───────│  发送结果                    │
│       ↓                      │        │                              │
│  [8] 检查结果                │        │  [9] SSL_read()              │
│       ↓                      │        │  [10] SSL_write()            │
│  设置 RPC 读写函数           │        │                              │
│  c->recv = ngx_ssl_recv_rpc  │───RPC->│  handle_ssl_read()           │
│  c->send = ngx_ssl_write_rpc │<──RPC─│  handle_ssl_write()          │
│                              │        │                              │
└──────────────────────────────┘        └──────────────────────────────┘
```

## 需要迁移的函数列表

### 核心握手函数（优先级最高）

```c
ngx_ssl_handshake()              // 第 1502 行 - 主边界函数
  ↓ 调用
SSL_do_handshake()               // OpenSSL - 需要在 sslkey 域执行
```

### 握手后的 I/O 函数（后续需要迁移）

```c
ngx_ssl_recv()                   // 第 1713 行 - SSL_read 封装
ngx_ssl_write()                  // 第 2034 行 - SSL_write 封装
ngx_ssl_recv_chain()             // 第 1839 行
ngx_ssl_send_chain()             // 第 2244 行
```

### 关闭函数

```c
ngx_ssl_shutdown()               // 第 2664 行 - SSL_shutdown 封装
```

## 关键数据结构

### ngx_connection_t (连接对象)

```c
struct ngx_connection_s {
    void               *data;
    ngx_event_t        *read;      // 读事件
    ngx_event_t        *write;     // 写事件
    
    ngx_socket_t        fd;        // socket 文件描述符 ← 需要传递到 sslkey
    
    ngx_recv_pt         recv;      // 接收函数指针 ← 需要改为 RPC 版本
    ngx_send_pt         send;      // 发送函数指针 ← 需要改为 RPC 版本
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;
    
    ngx_ssl_connection_t *ssl;     // SSL 连接信息
    
    // ... 其他字段
};
```

### ngx_ssl_connection_t (SSL 连接对象)

```c
struct ngx_ssl_connection_s {
    ngx_ssl_conn_t     *connection;  // OpenSSL SSL* 对象 ← 在 sslkey 域
    
    ngx_uint_t          handshaked:1;  // 握手是否完成
    ngx_uint_t          buffer:1;
    
    // ... 其他字段
};
```

## 总结

**public 域应该调用的边界函数**：

1. **主边界函数**：`ngx_ssl_handshake(ngx_connection_t *c)`
   - 位置：`src/event/ngx_event_openssl.c:1502`
   - 功能：执行 SSL 握手
   - 内部调用：`SSL_do_handshake()` ← 这个需要在 sslkey 域执行

2. **后续 I/O 边界函数**（握手成功后）：
   - `ngx_ssl_recv()` - 读取加密数据
   - `ngx_ssl_write()` - 写入加密数据

**建议**：使用 SSL 代理模式，让 nginx_sslkey_partition 独立处理所有 SSL 操作，无需修改这些边界函数。
