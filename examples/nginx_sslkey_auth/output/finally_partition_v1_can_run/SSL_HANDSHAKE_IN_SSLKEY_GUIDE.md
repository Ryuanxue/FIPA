# 将 SSL 握手过程迁移到 nginx_sslkey_partition

## 架构设计

### 方案 1：文件描述符传递（推荐）

使用 Unix Domain Socket 传递文件描述符，让 sslkey 域接管 SSL 连接。

```
┌─────────────────────────────────────────────────────────────────┐
│                    nginx_public_partition                       │
│                                                                 │
│  1. bind() + listen() on 0.0.0.0:443                          │
│  2. accept() 新连接 → 获得 client_fd                           │
│  3. 通过 Unix Socket 传递 client_fd 到 sslkey 域               │
│  4. 等待 SSL 握手完成通知                                       │
│  5. 接收解密后的 HTTP 请求数据                                  │
│  6. 处理 HTTP 请求（认证、路由等）                              │
│  7. 将 HTTP 响应发送给 sslkey 域加密                           │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
                           │
                           │ Unix Socket + sendmsg(fd)
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│                   nginx_sslkey_partition                        │
│                                                                 │
│  1. 接收 client_fd                                             │
│  2. SSL_set_fd(ssl, client_fd)                                │
│  3. SSL_accept(ssl) 执行握手 ✅                                │
│  4. SSL_read() 读取加密数据 → 解密                             │
│  5. 将明文数据发送给 public 域                                  │
│  6. 接收 public 域的 HTTP 响应                                 │
│  7. SSL_write() 加密并发送给客户端                             │
│  8. SSL_shutdown() 关闭连接                                    │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### 方案 2：SSL 代理模式（简化版）

sslkey 域作为 SSL 前端代理，解密后转发明文到 public 域。

```
Client ──HTTPS──> nginx_sslkey_partition ──HTTP──> nginx_public_partition
        (加密)         │ SSL_accept()    (明文)         │ 处理请求
                      │ SSL_read()                     │
                      │ 解密                           │
                      └────────────────────────────────┘
```

## 方案 1 详细实现：文件描述符传递

### 1.1 添加 RPC 接口定义

修改 `IDL/sslkey_auth_idl/sslkey_auth_idl.x`：

```c
/* SSL 连接处理请求 */
struct ssl_connection_request {
    int client_fd;              /* 客户端 socket 文件描述符 */
    opaque client_addr[16];     /* 客户端 IP 地址 */
    int client_port;            /* 客户端端口 */
};

/* SSL 连接处理响应 */
struct ssl_connection_response {
    int status;                 /* 0=成功, -1=失败 */
    char_ptr error_msg;        /* 错误消息 */
    int ssl_fd;                /* SSL 连接的文件描述符 */
};

/* SSL 数据读取请求 */
struct ssl_read_request {
    int ssl_fd;                /* SSL 连接的文件描述符 */
    int max_len;               /* 最大读取长度 */
};

/* SSL 数据读取响应 */
struct ssl_read_response {
    int status;                /* 读取的字节数，-1=错误 */
    char_ptr data;            /* 读取的数据 */
    char_ptr error_msg;       /* 错误消息 */
};

/* SSL 数据写入请求 */
struct ssl_write_request {
    int ssl_fd;                /* SSL 连接的文件描述符 */
    char_ptr data;            /* 要写入的数据 */
};

/* SSL 数据写入响应 */
struct ssl_write_response {
    int status;                /* 写入的字节数，-1=错误 */
    char_ptr error_msg;       /* 错误消息 */
};

program RPCSSLKEY {
   version RPCSSLKEYTVERS {
        int verify_sslcert(char_ptr value) = 1;
        ngx_ssl_certificate_response ngx_ssl_certificate_rpc_1(ngx_ssl_certificate_request) = 2;
        ssl_connection_response ssl_accept_connection(ssl_connection_request) = 3;
        ssl_read_response ssl_read_data(ssl_read_request) = 4;
        ssl_write_response ssl_write_data(ssl_write_request) = 5;
        int ssl_close_connection(int ssl_fd) = 6;
   } = 1;
} = 0x20000002;
```

### 1.2 实现文件描述符传递（Unix Socket）

**注意**：RPC 本身不支持传递文件描述符，需要使用 Unix Domain Socket 的 `sendmsg()` 和 `SCM_RIGHTS`。

创建 `nginx_sslkey_partion/src/core/fd_passing.c`：

```c
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define SOCKET_PATH "/tmp/nginx_sslkey_fd.sock"

/* 发送文件描述符 */
int send_fd(int unix_sock, int fd_to_send) {
    struct msghdr msg;
    struct iovec iov[1];
    struct cmsghdr *cmsg;
    char buf[CMSG_SPACE(sizeof(int))];
    char dummy = 'X';
    
    memset(&msg, 0, sizeof(msg));
    
    iov[0].iov_base = &dummy;
    iov[0].iov_len = 1;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    
    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    
    memcpy(CMSG_DATA(cmsg), &fd_to_send, sizeof(int));
    
    if (sendmsg(unix_sock, &msg, 0) < 0) {
        perror("sendmsg");
        return -1;
    }
    
    return 0;
}

/* 接收文件描述符 */
int recv_fd(int unix_sock) {
    struct msghdr msg;
    struct iovec iov[1];
    struct cmsghdr *cmsg;
    char buf[CMSG_SPACE(sizeof(int))];
    char dummy;
    int received_fd = -1;
    
    memset(&msg, 0, sizeof(msg));
    
    iov[0].iov_base = &dummy;
    iov[0].iov_len = 1;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    
    if (recvmsg(unix_sock, &msg, 0) < 0) {
        perror("recvmsg");
        return -1;
    }
    
    cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg && cmsg->cmsg_level == SOL_SOCKET && 
        cmsg->cmsg_type == SCM_RIGHTS) {
        memcpy(&received_fd, CMSG_DATA(cmsg), sizeof(int));
    }
    
    return received_fd;
}

/* 创建 Unix Domain Socket 服务器 */
int create_unix_server() {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }
    
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    unlink(SOCKET_PATH);
    
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return -1;
    }
    
    if (listen(sock, 5) < 0) {
        perror("listen");
        close(sock);
        return -1;
    }
    
    return sock;
}

/* 连接到 Unix Domain Socket 服务器 */
int connect_unix_server() {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }
    
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return -1;
    }
    
    return sock;
}
```

### 1.3 nginx_sslkey_partition：SSL 握手处理

创建 `nginx_sslkey_partion/src/core/ssl_handler.c`：

```c
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event_openssl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "fd_passing.h"

/* 全局 SSL_CTX */
static SSL_CTX *g_ssl_ctx = NULL;

/* 初始化 SSL_CTX */
int init_ssl_context(const char *cert_path, const char *key_path) {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
    
    g_ssl_ctx = SSL_CTX_new(TLS_server_method());
    if (!g_ssl_ctx) {
        fprintf(stderr, "Failed to create SSL_CTX\n");
        return -1;
    }
    
    if (SSL_CTX_use_certificate_file(g_ssl_ctx, cert_path, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    
    if (SSL_CTX_use_PrivateKey_file(g_ssl_ctx, key_path, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    
    if (!SSL_CTX_check_private_key(g_ssl_ctx)) {
        fprintf(stderr, "Private key does not match certificate\n");
        return -1;
    }
    
    printf("SSL context initialized successfully\n");
    return 0;
}

/* SSL 连接管理结构 */
typedef struct {
    int client_fd;
    SSL *ssl;
    int is_active;
} ssl_connection_t;

#define MAX_SSL_CONNECTIONS 1024
static ssl_connection_t g_ssl_connections[MAX_SSL_CONNECTIONS];

/* 分配 SSL 连接槽位 */
static int alloc_ssl_connection_slot() {
    for (int i = 0; i < MAX_SSL_CONNECTIONS; i++) {
        if (!g_ssl_connections[i].is_active) {
            return i;
        }
    }
    return -1;
}

/* 处理新的 SSL 连接 */
int handle_ssl_accept(int client_fd) {
    if (!g_ssl_ctx) {
        fprintf(stderr, "SSL context not initialized\n");
        return -1;
    }
    
    int slot = alloc_ssl_connection_slot();
    if (slot < 0) {
        fprintf(stderr, "No available connection slot\n");
        return -1;
    }
    
    SSL *ssl = SSL_new(g_ssl_ctx);
    if (!ssl) {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    
    SSL_set_fd(ssl, client_fd);
    
    printf("Performing SSL handshake on fd=%d...\n", client_fd);
    
    int ret = SSL_accept(ssl);
    if (ret <= 0) {
        int err = SSL_get_error(ssl, ret);
        fprintf(stderr, "SSL_accept failed: %d\n", err);
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        return -1;
    }
    
    printf("SSL handshake successful on fd=%d\n", client_fd);
    
    g_ssl_connections[slot].client_fd = client_fd;
    g_ssl_connections[slot].ssl = ssl;
    g_ssl_connections[slot].is_active = 1;
    
    return slot;  /* 返回槽位索引作为 ssl_fd */
}

/* 读取 SSL 数据 */
int handle_ssl_read(int ssl_fd, char *buffer, int max_len) {
    if (ssl_fd < 0 || ssl_fd >= MAX_SSL_CONNECTIONS) {
        return -1;
    }
    
    ssl_connection_t *conn = &g_ssl_connections[ssl_fd];
    if (!conn->is_active || !conn->ssl) {
        return -1;
    }
    
    int n = SSL_read(conn->ssl, buffer, max_len);
    if (n <= 0) {
        int err = SSL_get_error(conn->ssl, n);
        if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
            return 0;  /* 需要重试 */
        }
        fprintf(stderr, "SSL_read failed: %d\n", err);
        return -1;
    }
    
    return n;
}

/* 写入 SSL 数据 */
int handle_ssl_write(int ssl_fd, const char *data, int len) {
    if (ssl_fd < 0 || ssl_fd >= MAX_SSL_CONNECTIONS) {
        return -1;
    }
    
    ssl_connection_t *conn = &g_ssl_connections[ssl_fd];
    if (!conn->is_active || !conn->ssl) {
        return -1;
    }
    
    int n = SSL_write(conn->ssl, data, len);
    if (n <= 0) {
        int err = SSL_get_error(conn->ssl, n);
        fprintf(stderr, "SSL_write failed: %d\n", err);
        return -1;
    }
    
    return n;
}

/* 关闭 SSL 连接 */
int handle_ssl_close(int ssl_fd) {
    if (ssl_fd < 0 || ssl_fd >= MAX_SSL_CONNECTIONS) {
        return -1;
    }
    
    ssl_connection_t *conn = &g_ssl_connections[ssl_fd];
    if (!conn->is_active) {
        return -1;
    }
    
    if (conn->ssl) {
        SSL_shutdown(conn->ssl);
        SSL_free(conn->ssl);
    }
    
    if (conn->client_fd >= 0) {
        close(conn->client_fd);
    }
    
    conn->is_active = 0;
    conn->ssl = NULL;
    conn->client_fd = -1;
    
    return 0;
}

/* 主循环：接收文件描述符并处理 SSL */
void ssl_handler_main_loop() {
    int unix_server = create_unix_server();
    if (unix_server < 0) {
        fprintf(stderr, "Failed to create Unix socket server\n");
        return;
    }
    
    printf("SSL handler waiting for connections...\n");
    
    while (1) {
        int client_sock = accept(unix_server, NULL, NULL);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }
        
        int client_fd = recv_fd(client_sock);
        if (client_fd < 0) {
            fprintf(stderr, "Failed to receive file descriptor\n");
            close(client_sock);
            continue;
        }
        
        printf("Received client_fd=%d\n", client_fd);
        
        int ssl_fd = handle_ssl_accept(client_fd);
        if (ssl_fd < 0) {
            fprintf(stderr, "SSL handshake failed\n");
            close(client_fd);
            close(client_sock);
            continue;
        }
        
        /* 将 ssl_fd 发送回 public 域 */
        write(client_sock, &ssl_fd, sizeof(ssl_fd));
        
        close(client_sock);
    }
}
```

### 1.4 nginx_public_partition：传递连接到 sslkey 域

修改 `nginx_public_partition` 的连接处理代码：

```c
#include "fd_passing.h"

/* 在 accept 新连接后调用 */
void handle_new_connection(int client_fd) {
    /* 连接到 sslkey 域的 Unix socket */
    int unix_sock = connect_unix_server();
    if (unix_sock < 0) {
        fprintf(stderr, "Failed to connect to sslkey domain\n");
        close(client_fd);
        return;
    }
    
    /* 传递文件描述符 */
    if (send_fd(unix_sock, client_fd) < 0) {
        fprintf(stderr, "Failed to send file descriptor\n");
        close(unix_sock);
        close(client_fd);
        return;
    }
    
    /* 接收 ssl_fd */
    int ssl_fd = -1;
    read(unix_sock, &ssl_fd, sizeof(ssl_fd));
    
    close(unix_sock);
    close(client_fd);  /* 原始 fd 已传递，可以关闭 */
    
    if (ssl_fd < 0) {
        fprintf(stderr, "SSL handshake failed in sslkey domain\n");
        return;
    }
    
    printf("SSL connection established, ssl_fd=%d\n", ssl_fd);
    
    /* 现在可以通过 RPC 进行 SSL 读写操作 */
    /* ... */
}
```

## 方案 2 简化实现：SSL 代理模式

这个方案更简单，但需要在两个域之间传输明文数据。

### 2.1 架构

```
Client ─HTTPS(443)─> nginx_sslkey ─HTTP(8080)─> nginx_public
                    [SSL Termination]            [应用处理]
```

### 2.2 nginx_sslkey_partition 配置

创建一个简单的 nginx 配置作为 SSL 前端：

```nginx
# nginx_sslkey_partition/conf/nginx.conf
events {
    worker_connections 1024;
}

http {
    # SSL 监听
    server {
        listen 443 ssl;
        server_name _;
        
        ssl_certificate /path/to/cert.pem;
        ssl_certificate_key /path/to/cert.key;
        
        # 解密后转发到 public 域
        location / {
            proxy_pass http://127.0.0.1:8080;
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_set_header X-Forwarded-Proto $scheme;
        }
    }
}
```

### 2.3 nginx_public_partition 配置

```nginx
# nginx_public_partition/conf/nginx.conf
http {
    # 监听来自 sslkey 域的明文连接
    server {
        listen 127.0.0.1:8080;
        
        # 认证保护的路径
        location /admin {
            # 通过 RPC 调用认证模块
            auth_basic "Restricted";
            auth_basic_user_file auth/.htpasswd;
            root html;
        }
        
        # 其他路径
        location / {
            root html/public;
        }
    }
}
```

## 对比总结

| 特性 | 方案1：FD传递 | 方案2：代理模式 |
|------|--------------|----------------|
| 复杂度 | ⭐⭐⭐⭐⭐ 高 | ⭐⭐ 低 |
| 性能 | ⭐⭐⭐⭐ 好 | ⭐⭐⭐ 一般 |
| 安全性 | ⭐⭐⭐⭐⭐ 最高 | ⭐⭐⭐⭐ 高 |
| 实现难度 | 需要自定义代码 | 使用标准 nginx |
| 维护成本 | 高 | 低 |

## 推荐方案

**推荐使用方案 2（SSL 代理模式）**，原因：

1. ✅ 实现简单，使用标准 nginx 功能
2. ✅ 配置清晰，易于维护
3. ✅ SSL 握手在 sslkey 域完成
4. ✅ 证书文件完全隔离
5. ✅ 可以使用现有的 nginx 生态工具

方案 1 虽然性能更好，但实现复杂度极高，且需要大量自定义代码，容易引入 bug。
