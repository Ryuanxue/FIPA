# Socket FD 的创建和传递机制详解

## 核心问题回答

### Q1: TCP 三次握手后的 socket fd 是在哪里创建的？

**答案**：在 `ngx_event_accept()` 函数中通过 `accept()` 系统调用创建。

**位置**：`nginx_public_partition/src/event/ngx_event_accept.c:63`

### Q2: 若将 ngx_ssl_handshake 放在 sslkey 域，是谁创建 socket？

**答案**：**socket 是通过 `ngx_ssl_handshake` 的参数 `c` (ngx_connection_t*) 进行传递的**，不是由 sslkey 域创建。

- ✅ **public 域创建 socket**：通过 `accept()` 获得客户端 socket fd
- ✅ **通过参数传递**：socket fd 保存在 `c->fd` 中
- ❌ **不是 sslkey 创建**：sslkey 域接收已存在的 socket

## Socket 创建和传递的完整流程

```
┌──────────────────────────────────────────────────────────────────────┐
│ 第 1 步：监听 Socket 创建 (启动时)                                    │
├──────────────────────────────────────────────────────────────────────┤
│ 位置：nginx_public_partition/src/core/ngx_connection.c:490          │
│                                                                      │
│ s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type, 0);          │
│     ↓                                                                │
│ bind(s, addr, addrlen);    // 绑定到 0.0.0.0:443                   │
│     ↓                                                                │
│ listen(s, backlog);         // 开始监听 (行 654)                    │
│     ↓                                                                │
│ ls[i].fd = s;               // 保存监听 socket fd                   │
└──────────────────────────────────────────────────────────────────────┘
                                    │
                                    │ (客户端发起连接)
                                    ▼
┌──────────────────────────────────────────────────────────────────────┐
│ 第 2 步：Accept 客户端连接 (运行时)                                   │
├──────────────────────────────────────────────────────────────────────┤
│ 位置：nginx_public_partition/src/event/ngx_event_accept.c:60        │
│                                                                      │
│ void ngx_event_accept(ngx_event_t *ev)                              │
│ {                                                                    │
│     ngx_socket_t s;                                                  │
│     ngx_connection_t *c;                                            │
│     ngx_listening_t *ls;                                            │
│                                                                      │
│     lc = ev->data;                // 获取监听连接                   │
│     ls = lc->listening;           // 获取监听 socket 配置           │
│                                                                      │
│     // ====== 关键：创建客户端 socket fd ======                     │
│     s = accept(lc->fd, &sa.sockaddr, &socklen);  // 行 63          │
│     //        ^^^^^^                                                 │
│     //        监听 socket fd                                         │
│     //                                                               │
│     // 返回值 s 就是客户端 socket fd！                               │
│     // TCP 三次握手已经完成                                          │
│     // ==========================================                    │
│                                                                      │
│     if (s == (ngx_socket_t) -1) {                                   │
│         // 错误处理                                                  │
│         return;                                                      │
│     }                                                                │
│                                                                      │
│     // 获取一个连接对象                                              │
│     c = ngx_get_connection(s, ev->log);  // 行 144                 │
│     //                     ^                                         │
│     //                     传入客户端 socket fd                      │
│                                                                      │
│     c->fd = s;           // socket fd 保存在 c->fd 中               │
│     c->type = SOCK_STREAM;                                          │
│     c->pool = ngx_create_pool(...);                                 │
│     // ... 初始化其他字段                                            │
│                                                                      │
│     // 调用监听器的处理函数 (HTTP 或其他协议)                        │
│     ls->handler(c);      // 通常是 ngx_http_init_connection        │
│ }                                                                    │
└──────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌──────────────────────────────────────────────────────────────────────┐
│ 第 3 步：HTTP 连接初始化                                              │
├──────────────────────────────────────────────────────────────────────┤
│ 位置：nginx_public_partition/src/http/ngx_http_request.c:287        │
│                                                                      │
│ void ngx_http_init_connection(ngx_connection_t *c)                  │
│ {                                                                    │
│     // c->fd 已经包含了客户端 socket fd                              │
│                                                                      │
│     // 检测是否需要 SSL                                              │
│     if (hc->addr_conf->ssl) {                                       │
│         rev->handler = ngx_http_ssl_handshake;                      │
│     } else {                                                         │
│         rev->handler = ngx_http_wait_request_handler;               │
│     }                                                                │
│ }                                                                    │
└──────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼ (需要 SSL)
┌──────────────────────────────────────────────────────────────────────┐
│ 第 4 步：SSL 握手 (当前在 public 域)                                  │
├──────────────────────────────────────────────────────────────────────┤
│ 位置：nginx_public_partition/src/http/ngx_http_request.c:737        │
│                                                                      │
│ static void ngx_http_ssl_handshake(ngx_event_t *rev)                │
│ {                                                                    │
│     ngx_connection_t *c = rev->data;                                │
│     // c->fd 包含客户端 socket fd                                    │
│                                                                      │
│     rc = ngx_ssl_handshake(c);  // ◄── 边界函数！                   │
│     //                     ^                                         │
│     //                     通过 c 参数传递 socket fd                │
│ }                                                                    │
└──────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌──────────────────────────────────────────────────────────────────────┐
│ 第 5 步：SSL 握手实现 (边界函数)                                      │
├──────────────────────────────────────────────────────────────────────┤
│ 位置：nginx_public_partition/src/event/ngx_event_openssl.c:1502     │
│                                                                      │
│ ngx_int_t ngx_ssl_handshake(ngx_connection_t *c)                    │
│ {                                                                    │
│     // c->fd 包含客户端 socket fd                                    │
│     // c->ssl->connection 是 OpenSSL 的 SSL* 对象                   │
│                                                                      │
│     // OpenSSL 的 SSL* 对象已经通过 SSL_set_fd() 绑定到 socket     │
│     // (在 ngx_ssl_create_connection 中完成)                        │
│                                                                      │
│     n = SSL_do_handshake(c->ssl->connection);                       │
│     //                                                               │
│     // SSL_do_handshake 内部会使用 c->fd 进行读写操作              │
│     // 实际上是读写 socket fd                                        │
│                                                                      │
│     if (n == 1) {                                                    │
│         c->ssl->handshaked = 1;                                     │
│         return NGX_OK;                                               │
│     }                                                                │
│     // ... 错误处理                                                  │
│ }                                                                    │
└──────────────────────────────────────────────────────────────────────┘
```

## Socket FD 的绑定时机

### 关键位置：`ngx_ssl_create_connection()`

**位置**：`nginx_public_partition/src/event/ngx_event_openssl.c:1410`

```c
ngx_int_t
ngx_ssl_create_connection(ngx_ssl_t *ssl, ngx_connection_t *c, ngx_uint_t flags)
{
    ngx_ssl_connection_t  *sc;

    sc = ngx_pcalloc(c->pool, sizeof(ngx_ssl_connection_t));
    if (sc == NULL) {
        return NGX_ERROR;
    }

    // 创建 OpenSSL 的 SSL* 对象
    sc->connection = SSL_new(ssl->ctx);
    if (sc->connection == NULL) {
        return NGX_ERROR;
    }

    // ====== 关键：将 socket fd 绑定到 SSL* 对象 ======
    if (SSL_set_fd(sc->connection, c->fd) == 0) {
    //                             ^^^^
    //                             这里使用了 c->fd (客户端 socket fd)
        return NGX_ERROR;
    }
    // ================================================

    c->ssl = sc;
    
    return NGX_OK;
}
```

**调用时机**：在 `ngx_http_ssl_handshake()` 调用 `ngx_ssl_handshake()` 之前。

## ngx_connection_t 结构体中的 fd

```c
struct ngx_connection_s {
    void               *data;
    ngx_event_t        *read;
    ngx_event_t        *write;
    
    // ===== 关键字段：socket 文件描述符 =====
    ngx_socket_t        fd;        // 客户端 socket fd
    // ======================================
    
    ngx_recv_pt         recv;      // 接收函数指针
    ngx_send_pt         send;      // 发送函数指针
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;
    
    ngx_listening_t    *listening;  // 所属的监听socket
    
    // ...
    
    ngx_ssl_connection_t *ssl;     // SSL 连接信息
    
    // ...
};
```

## 迁移到 sslkey 域的方案

### 方案 A：通过 Unix Socket 传递 fd（需要自定义代码）

```c
// 在 public 域 (ngx_ssl_handshake 中)
ngx_int_t ngx_ssl_handshake(ngx_connection_t *c)
{
    // c->fd 已经包含了客户端 socket fd
    int client_fd = c->fd;
    
    // 1. 连接到 sslkey 域的 Unix socket
    int unix_sock = connect_unix_server();
    
    // 2. 通过 sendmsg() + SCM_RIGHTS 传递 client_fd
    send_fd(unix_sock, client_fd);
    
    // 3. 等待 sslkey 域完成握手
    int result;
    read(unix_sock, &result, sizeof(result));
    
    close(unix_sock);
    
    if (result == 0) {
        c->ssl->handshaked = 1;
        return NGX_OK;
    }
    
    return NGX_ERROR;
}
```

```c
// 在 sslkey 域 (接收 fd 并握手)
void ssl_handler_main_loop()
{
    int unix_server = create_unix_server();
    
    while (1) {
        int client_sock = accept(unix_server, NULL, NULL);
        
        // 接收从 public 域传来的 client_fd
        int client_fd = recv_fd(client_sock);
        
        // 创建 SSL* 对象
        SSL *ssl = SSL_new(g_ssl_ctx);
        SSL_set_fd(ssl, client_fd);  // 绑定 fd
        
        // 执行握手
        int ret = SSL_accept(ssl);
        
        // 返回结果
        int result = (ret == 1) ? 0 : -1;
        write(client_sock, &result, sizeof(result));
        
        close(client_sock);
    }
}
```

### 方案 B：SSL 代理模式（推荐，无需传递 fd）

```
Client ──TCP 连接──> nginx_sslkey:443
                    │ accept() 创建 client_fd
                    │ SSL_set_fd(ssl, client_fd)
                    │ SSL_accept(ssl)  ← 握手在 sslkey 域
                    │ SSL_read()  ← 解密
                    ↓
                    proxy_pass http://127.0.0.1:8080
                    │ 转发明文数据
                    ↓
                 nginx_public:8080
                    │ 处理 HTTP 请求
```

在这种模式下：
- ✅ **sslkey 域自己创建 socket**：通过自己的 `accept()` 
- ✅ **sslkey 域完成 SSL 握手**：在自己的进程中
- ✅ **无需传递 fd**：每个域管理自己的连接

## 总结

### Socket FD 创建位置

| 步骤 | 函数 | 位置 | Socket 类型 | 创建方式 |
|------|------|------|------------|----------|
| 1 | `ngx_open_listening_sockets()` | `ngx_connection.c:490` | 监听 socket | `socket()` + `bind()` + `listen()` |
| 2 | `ngx_event_accept()` | `ngx_event_accept.c:63` | 客户端 socket | `accept(listening_fd)` |
| 3 | `ngx_get_connection()` | `ngx_connection.c:1088` | - | 将 socket fd 封装到 ngx_connection_t |
| 4 | `ngx_ssl_create_connection()` | `ngx_event_openssl.c:1451` | - | `SSL_set_fd(ssl, c->fd)` 绑定 |

### 参数传递方式

```
客户端 socket fd 的传递链：

accept() 返回 s
    ↓
c = ngx_get_connection(s, ...)
    ↓
c->fd = s
    ↓
ngx_http_init_connection(c)
    ↓
ngx_http_ssl_handshake(rev)
    c = rev->data
    ↓
ngx_ssl_handshake(c)  ← 边界函数
    ↓
SSL_do_handshake() 使用 c->fd
```

**关键点**：
1. ✅ Socket fd 在 **public 域的 `accept()` 中创建**
2. ✅ 通过 **`ngx_connection_t *c` 参数传递**，保存在 `c->fd` 字段
3. ✅ 如果迁移到 sslkey 域，需要**通过 Unix Socket + SCM_RIGHTS 传递 fd**
4. ✅ **推荐方案**：使用 SSL 代理模式，让 sslkey 域独立处理连接，无需传递 fd
