# GDB调试Nginx SSL握手指南

## 问题分析

当你在GDB中设置 `ngx_ssl_handshake` 断点时，会看到断点没有触发，原因是：

1. **Nginx的进程模型**：
   - Nginx启动时会fork出master进程和worker子进程
   - SSL握手发生在**worker子进程**中，而非master进程
   - GDB默认只跟踪父进程（master），所以断点不会触发

2. **GDB输出证据**：
   ```
   [Detaching after fork from child process 1790181]
   ```
   这说明GDB检测到了fork，但选择继续跟踪父进程，分离了子进程

## 解决方案

### 方案1：让GDB跟踪子进程（推荐用于调试worker逻辑）

在GDB启动时设置跟踪子进程：

```bash
cd /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition_v1

gdb /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/input/nginx_64
```

在GDB中输入以下命令：

```gdb
# 设置GDB跟踪子进程而非父进程
(gdb) set follow-fork-mode child

# 设置断点
(gdb) b ngx_ssl_handshake
Breakpoint 1 at 0x4c195: file src/event/ngx_event_openssl.c, line 1354.

# 运行nginx
(gdb) run -p /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth -c conf/nginx-ssl-auth.conf -g "daemon off;"
```

然后在另一个终端发起HTTPS请求：

```bash
curl -k -u testuser:testpass123 https://localhost:8443/api/
```

**此时断点应该会触发！**

### 方案2：使用单进程模式（更简单的调试）

修改nginx配置，让它以单进程模式运行：

在 `conf/nginx-ssl-auth.conf` 文件最顶部添加：

```nginx
master_process off;    # 禁用master-worker模式
daemon off;            # 前台运行
worker_processes 1;    # 单个worker
```

然后直接运行：

```bash
gdb /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/input/nginx_64

(gdb) b ngx_ssl_handshake
(gdb) run -p /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth -c conf/nginx-ssl-auth.conf
```

### 方案3：直接attach到worker进程

1. 先正常启动nginx（不用GDB）
2. 找到worker进程的PID：
   ```bash
   ps aux | grep nginx
   ```
   输出示例：
   ```
   root      12345  ... nginx: master process
   nobody    12346  ... nginx: worker process    <- 这个是worker
   ```

3. 用GDB attach到worker进程：
   ```bash
   sudo gdb -p 12346
   ```

4. 设置断点：
   ```gdb
   (gdb) b ngx_ssl_handshake
   (gdb) continue
   ```

5. 发起HTTPS请求触发断点

## 完整的GDB调试会话示例

```bash
# 终端1：启动GDB调试
cd /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition_v1

gdb /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/input/nginx_64

# 在GDB中
(gdb) set follow-fork-mode child
(gdb) b ngx_ssl_handshake
Breakpoint 1 at 0x4c195: file src/event/ngx_event_openssl.c, line 1354.

# 可以设置更多相关断点
(gdb) b ngx_http_ssl_handshake
(gdb) b SSL_do_handshake

(gdb) run -p /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth -c conf/nginx-ssl-auth.conf -g "daemon off;"
```

```bash
# 终端2：触发SSL握手
curl -k -u testuser:testpass123 https://localhost:8443/api/
```

## SSL握手调用链（按执行顺序）

当HTTPS请求到达时，调用链如下：

```
1. ngx_event_accept()                    [src/event/ngx_event_accept.c:60]
   └─ accept() 创建客户端连接的socket fd
   └─ ngx_http_init_connection()        [src/http/ngx_http_request.c:389]

2. ngx_http_init_connection()
   └─ ngx_http_ssl_handshake()          [src/http/ngx_http_request.c:735]

3. ngx_http_ssl_handshake()
   └─ ngx_ssl_handshake(c)              [src/http/ngx_http_request.c:735]

4. ngx_ssl_handshake()                   [src/event/ngx_event_openssl.c:1502]
   └─ SSL_do_handshake()                [OpenSSL库函数]
```

## 调试技巧

### 1. 查看当前进程信息
```gdb
(gdb) info inferior
(gdb) info proc
```

### 2. 查看SSL连接状态
```gdb
(gdb) print *c->ssl
(gdb) print c->ssl->connection
```

### 3. 查看握手错误
```gdb
(gdb) print ERR_get_error()
(gdb) call ERR_error_string(ERR_get_error(), 0)
```

### 4. 设置条件断点（只在第一次握手时停止）
```gdb
(gdb) b ngx_ssl_handshake if c->ssl->handshaked == 0
```

### 5. 查看调用栈
```gdb
(gdb) backtrace
(gdb) bt full
```

## 验证SSL配置是否生效

在不使用GDB的情况下，可以通过以下方式验证：

```bash
# 1. 检查nginx是否监听8443端口
netstat -tlnp | grep 8443

# 2. 使用openssl s_client测试SSL握手
openssl s_client -connect localhost:8443 -showcerts

# 3. 查看nginx错误日志
tail -f logs/error.log

# 4. 使用curl带详细输出
curl -k -v -u testuser:testpass123 https://localhost:8443/api/
```

## 常见问题

### Q1: 为什么断点不触发？
**A**: 因为SSL握手发生在worker子进程，需要设置 `set follow-fork-mode child`

### Q2: 可以同时调试多个worker吗？
**A**: GDB一次只能跟踪一个进程。建议设置 `worker_processes 1;`

### Q3: 如何知道当前在哪个进程？
**A**: 使用 `info inferior` 和 `info proc` 命令

### Q4: 为什么有时候断点触发，有时候不触发？
**A**: 因为nginx可能有多个worker，请求可能被分配到不同的worker。使用单worker模式可避免此问题。

## 推荐的调试流程

1. ✅ 使用 `master_process off;` 配置单进程模式
2. ✅ 或者使用 `set follow-fork-mode child` 跟踪子进程  
3. ✅ 设置 `worker_processes 1;` 只用一个worker
4. ✅ 在 `ngx_ssl_handshake` 和 `ngx_http_ssl_handshake` 都设置断点
5. ✅ 启动nginx后，用curl发起HTTPS请求
6. ✅ 观察断点触发和SSL握手过程

## 参考文档

- [SOCKET_FD_CREATION_AND_PASSING.md](SOCKET_FD_CREATION_AND_PASSING.md) - Socket创建机制
- [SSL_HANDSHAKE_CALL_CHAIN.md](SSL_HANDSHAKE_CALL_CHAIN.md) - 完整调用链
- [SSL_BOUNDARY_FUNCTIONS.md](SSL_BOUNDARY_FUNCTIONS.md) - 边界函数说明
