#!/bin/bash

BASE_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition_v1"

echo "========================================="
echo "启动 SSL 代理模式"
echo "========================================="
echo ""
echo "架构："
echo "  Client (HTTPS) -> nginx_sslkey:443 (SSL终止)"
echo "                    -> nginx_public:8080 (应用处理)"
echo "                       -> nginx_auth (RPC认证)"
echo ""

# 检查证书文件
if [ ! -f "$BASE_DIR/ssl/cert.pem" ] || [ ! -f "$BASE_DIR/ssl/cert.key" ]; then
    echo "错误: SSL 证书文件不存在"
    echo "请运行: cd $BASE_DIR/ssl && ./generate_cert.sh"
    exit 1
fi

# 检查 .htpasswd 文件
if [ ! -f "$BASE_DIR/auth/.htpasswd" ]; then
    echo "警告: 认证文件不存在，创建默认用户 admin:admin"
    mkdir -p "$BASE_DIR/auth"
    # 密码 'admin' 的 htpasswd 格式
    echo 'admin:$apr1$3pGTv7lE$I2sXjLqNsqUdX0u/L2RKU0' > "$BASE_DIR/auth/.htpasswd"
fi

# 1. 启动 nginx_auth_partition 的 RPC 服务器
echo ""
echo "[1/3] 启动认证 RPC 服务器..."
cd "$BASE_DIR/nginx_auth_partition/objs"
if [ -f "./nginx_auth_rpc_server" ]; then
    ./nginx_auth_rpc_server > /dev/null 2>&1 &
    AUTH_RPC_PID=$!
    sleep 0.5
    if ps -p $AUTH_RPC_PID > /dev/null; then
        echo "      ✓ 认证 RPC 服务器已启动 (PID: $AUTH_RPC_PID)"
    else
        echo "      ✗ 认证 RPC 服务器启动失败"
        exit 1
    fi
else
    echo "      ✗ 未找到 nginx_auth_rpc_server"
    exit 1
fi

# 2. 启动 nginx_public_partition (应用服务器，监听 8080)
echo ""
echo "[2/3] 启动应用服务器 (nginx_public_partition)..."
cd "$BASE_DIR/nginx_public_partition"
if [ -f "./objs/nginx" ]; then
    ./objs/nginx -c "$BASE_DIR/nginx_public_partition/conf/nginx-app.conf" \
                 -p "$BASE_DIR/nginx_public_partition" > /dev/null 2>&1
    sleep 0.5
    if netstat -tln 2>/dev/null | grep -q ':8080'; then
        echo "      ✓ 应用服务器已启动 (监听 127.0.0.1:8080)"
    else
        echo "      ✗ 应用服务器启动失败"
        kill $AUTH_RPC_PID 2>/dev/null
        exit 1
    fi
else
    echo "      ✗ 未找到 nginx 可执行文件"
    kill $AUTH_RPC_PID 2>/dev/null
    exit 1
fi

# 3. 启动 nginx_sslkey_partition (SSL 前端，监听 443)
echo ""
echo "[3/3] 启动 SSL 前端服务器 (nginx_sslkey_partition)..."
cd "$BASE_DIR/nginx_sslkey_partion"
if [ -f "./objs/nginx" ]; then
    ./objs/nginx -c "$BASE_DIR/nginx_sslkey_partion/conf/nginx-ssl-proxy.conf" \
                 -p "$BASE_DIR/nginx_sslkey_partion" > /dev/null 2>&1
    sleep 0.5
    if netstat -tln 2>/dev/null | grep -q ':443'; then
        echo "      ✓ SSL 前端服务器已启动 (监听 0.0.0.0:443)"
    else
        echo "      ✗ SSL 前端服务器启动失败"
        kill $AUTH_RPC_PID 2>/dev/null
        "$BASE_DIR/nginx_public_partition/objs/nginx" -s stop 2>/dev/null
        exit 1
    fi
else
    echo "      ✗ 未找到 nginx 可执行文件"
    kill $AUTH_RPC_PID 2>/dev/null
    "$BASE_DIR/nginx_public_partition/objs/nginx" -s stop 2>/dev/null
    exit 1
fi

echo ""
echo "========================================="
echo "✓ 所有服务已成功启动！"
echo "========================================="
echo ""
echo "测试命令："
echo "  # 公开内容（无需认证）"
echo "  curl -k https://localhost/public/"
echo ""
echo "  # API 接口（无需认证）"
echo "  curl -k https://localhost/api/"
echo ""
echo "  # 管理页面（需要认证）"
echo "  curl -k https://localhost/admin/"
echo "  curl -k -u admin:admin https://localhost/admin/"
echo ""
echo "  # 健康检查"
echo "  curl -k https://localhost/sslkey-health"
echo "  curl http://127.0.0.1:8080/public-health"
echo ""
echo "查看日志："
echo "  tail -f nginx_sslkey_partion/logs/sslkey_access.log"
echo "  tail -f nginx_public_partition/logs/public_access.log"
echo ""
echo "停止服务："
echo "  ./stop_ssl_proxy.sh"
echo ""
