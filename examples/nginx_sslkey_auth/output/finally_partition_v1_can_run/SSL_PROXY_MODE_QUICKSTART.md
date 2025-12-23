# SSL 代理模式快速实现指南

## 架构图

```
           HTTPS (443)               HTTP (8080)
Client ════════════════> nginx_sslkey ────────────> nginx_public
            加密           │ SSL_accept()   明文      │ 业务逻辑
                          │ SSL_read()               │ 认证授权
                          │ 解密                     │ 路由
                          └──────────────────────────┘
```

## 步骤 1：配置 nginx_sslkey_partition（SSL 终止）

创建 `nginx_sslkey_partion/conf/nginx-ssl-proxy.conf`：

```nginx
worker_processes 1;

events {
    worker_connections 1024;
}

http {
    include       mime.types;
    default_type  application/octet-stream;
    
    # 日志
    access_log  logs/sslkey_access.log;
    error_log   logs/sslkey_error.log info;
    
    # SSL 前端服务器
    server {
        listen 443 ssl;
        server_name localhost;
        
        # SSL 证书配置
        ssl_certificate      /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition_v1/ssl/cert.pem;
        ssl_certificate_key  /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition_v1/ssl/cert.key;
        
        # SSL 参数
        ssl_protocols TLSv1.2 TLSv1.3;
        ssl_ciphers HIGH:!aNULL:!MD5;
        ssl_prefer_server_ciphers on;
        
        # 转发到 public 域（明文 HTTP）
        location / {
            proxy_pass http://127.0.0.1:8080;
            proxy_http_version 1.1;
            
            # 传递客户端信息
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_set_header X-Forwarded-Proto $scheme;
            
            # 传递 SSL 信息（如果需要）
            proxy_set_header X-SSL-Protocol $ssl_protocol;
            proxy_set_header X-SSL-Cipher $ssl_cipher;
            
            # 超时设置
            proxy_connect_timeout 10s;
            proxy_send_timeout 60s;
            proxy_read_timeout 60s;
        }
        
        # 健康检查
        location /health {
            access_log off;
            return 200 "OK\n";
            add_header Content-Type text/plain;
        }
    }
}
```

## 步骤 2：配置 nginx_public_partition（应用处理）

创建 `nginx_public_partition/conf/nginx-app.conf`：

```nginx
worker_processes 1;

events {
    worker_connections 1024;
}

http {
    include       mime.types;
    default_type  application/octet-stream;
    
    # 日志
    access_log  logs/public_access.log;
    error_log   logs/public_error.log info;
    
    # 应用服务器（接收来自 sslkey 域的明文流量）
    server {
        listen 127.0.0.1:8080;  # 只监听本地回环，不对外暴露
        server_name localhost;
        
        # 公共内容（无需认证）
        location /public {
            root html;
            index index.html;
        }
        
        # API 接口（无需认证）
        location /api {
            root html;
            index index.html;
        }
        
        # 管理页面（需要认证）- 这里会触发 RPC 调用
        location /admin {
            auth_basic "Restricted Area";
            auth_basic_user_file /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition_v1/auth/.htpasswd;
            
            root html;
            index index.html;
        }
        
        # 健康检查
        location /health {
            access_log off;
            return 200 "OK\n";
            add_header Content-Type text/plain;
        }
    }
}
```

## 步骤 3：启动脚本

创建 `start_ssl_proxy_mode.sh`：

```bash
#!/bin/bash

BASE_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition_v1"

echo "========================================="
echo "启动 SSL 代理模式"
echo "========================================="

# 1. 启动 nginx_auth_partition 的 RPC 服务器（认证服务）
echo ""
echo "1. 启动认证 RPC 服务器..."
cd "$BASE_DIR/nginx_auth_partition/objs"
if [ -f "./nginx_auth_rpc_server" ]; then
    ./nginx_auth_rpc_server &
    AUTH_RPC_PID=$!
    echo "   认证 RPC 服务器已启动 (PID: $AUTH_RPC_PID)"
else
    echo "   错误: 未找到 nginx_auth_rpc_server"
    exit 1
fi

sleep 1

# 2. 启动 nginx_public_partition（应用处理，监听 8080）
echo ""
echo "2. 启动应用服务器 (nginx_public_partition)..."
cd "$BASE_DIR/nginx_public_partition"
if [ -f "./objs/nginx" ]; then
    ./objs/nginx -c "$BASE_DIR/nginx_public_partition/conf/nginx-app.conf" -p "$BASE_DIR/nginx_public_partition"
    echo "   应用服务器已启动，监听 127.0.0.1:8080"
else
    echo "   错误: 未找到 nginx 可执行文件"
    kill $AUTH_RPC_PID
    exit 1
fi

sleep 1

# 3. 启动 nginx_sslkey_partition（SSL 终止，监听 443）
echo ""
echo "3. 启动 SSL 前端服务器 (nginx_sslkey_partition)..."
cd "$BASE_DIR/nginx_sslkey_partion"
if [ -f "./objs/nginx" ]; then
    ./objs/nginx -c "$BASE_DIR/nginx_sslkey_partion/conf/nginx-ssl-proxy.conf" -p "$BASE_DIR/nginx_sslkey_partion"
    echo "   SSL 前端服务器已启动，监听 0.0.0.0:443"
else
    echo "   错误: 未找到 nginx 可执行文件"
    kill $AUTH_RPC_PID
    "$BASE_DIR/nginx_public_partition/objs/nginx" -s stop
    exit 1
fi

echo ""
echo "========================================="
echo "所有服务已启动！"
echo "========================================="
echo ""
echo "请求流程："
echo "  Client -> nginx_sslkey (443, HTTPS)"
echo "       └─> nginx_public (8080, HTTP)"
echo "            └─> nginx_auth (RPC)"
echo ""
echo "测试命令："
echo "  curl -k https://localhost/public/             # 公开内容"
echo "  curl -k https://localhost/api/                # API"
echo "  curl -k -u admin:admin https://localhost/admin/  # 需要认证"
echo ""
echo "停止服务："
echo "  ./stop_all.sh"
```

创建 `stop_all.sh`：

```bash
#!/bin/bash

BASE_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition_v1"

echo "停止所有服务..."

# 停止 nginx_sslkey_partition
cd "$BASE_DIR/nginx_sslkey_partion"
if [ -f "./objs/nginx" ]; then
    ./objs/nginx -s stop 2>/dev/null
    echo "  nginx_sslkey_partition 已停止"
fi

# 停止 nginx_public_partition
cd "$BASE_DIR/nginx_public_partition"
if [ -f "./objs/nginx" ]; then
    ./objs/nginx -s stop 2>/dev/null
    echo "  nginx_public_partition 已停止"
fi

# 停止 RPC 服务器
pkill -f nginx_auth_rpc_server 2>/dev/null
echo "  RPC 服务器已停止"

echo "所有服务已停止"
```

## 步骤 4：测试

```bash
# 1. 启动服务
chmod +x start_ssl_proxy_mode.sh stop_all.sh
./start_ssl_proxy_mode.sh

# 2. 测试公开内容
curl -k https://localhost/public/
# 预期：返回 public/index.html

# 3. 测试 API
curl -k https://localhost/api/
# 预期：返回 api/index.html

# 4. 测试需要认证的管理页面（无凭据）
curl -k https://localhost/admin/
# 预期：401 Unauthorized

# 5. 测试需要认证的管理页面（有凭据）
curl -k -u admin:admin https://localhost/admin/
# 预期：返回 admin/index.html（通过 RPC 认证）

# 6. 查看日志
tail -f nginx_sslkey_partion/logs/sslkey_access.log
tail -f nginx_public_partition/logs/public_access.log
tail -f nginx_auth_partition/logs/error.log
```

## 优势

1. ✅ **SSL 握手在 sslkey 域**：证书文件完全隔离
2. ✅ **配置简单**：使用标准 nginx 配置，无需自定义代码
3. ✅ **易于调试**：可以分别查看每个域的日志
4. ✅ **性能良好**：本地回环连接，延迟极低
5. ✅ **灵活扩展**：可以轻松添加负载均衡、缓存等

## 数据流示例

```
1. Client 发起 HTTPS 请求
   └─> GET https://localhost/admin/

2. nginx_sslkey (443) 接收并解密
   ├─ SSL_accept() 完成握手
   ├─ SSL_read() 读取：GET /admin/ HTTP/1.1
   └─ 解密后转发：GET http://127.0.0.1:8080/admin/

3. nginx_public (8080) 处理请求
   ├─ 匹配 location /admin
   ├─ 触发 auth_basic_user_file
   ├─ 通过 RPC 调用 nginx_auth_partition 验证
   ├─ 认证成功
   └─ 返回：HTTP/1.1 200 OK + admin/index.html

4. nginx_sslkey 加密并返回
   ├─ 接收来自 public 的明文响应
   ├─ SSL_write() 加密
   └─ 发送给 Client

5. Client 接收 HTTPS 响应
   └─ 显示 admin/index.html
```

## 与原方案对比

| 项目 | 原方案 | SSL 代理模式 |
|------|--------|-------------|
| SSL 握手位置 | public 域 ❌ | sslkey 域 ✅ |
| 证书隔离 | 需要 public 访问证书 ❌ | 完全隔离 ✅ |
| 实现复杂度 | 高（RPC 传证书内容） | 低（标准配置）|
| 性能 | 好 | 很好（本地连接）|
| 维护成本 | 高 | 低 |

## 注意事项

1. **端口冲突**：确保 443 和 8080 端口未被占用
2. **证书路径**：修改配置中的证书路径为实际路径
3. **防火墙**：8080 端口应只允许本地访问
4. **日志轮转**：生产环境需要配置日志轮转

## 故障排查

```bash
# 检查端口占用
netstat -tlnp | grep -E '443|8080'

# 检查 nginx 进程
ps aux | grep nginx

# 测试 sslkey 到 public 的连接
curl http://127.0.0.1:8080/health

# 查看错误日志
tail -100 nginx_sslkey_partion/logs/sslkey_error.log
tail -100 nginx_public_partition/logs/public_error.log
```
