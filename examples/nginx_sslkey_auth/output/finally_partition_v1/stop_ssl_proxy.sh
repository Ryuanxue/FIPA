#!/bin/bash

BASE_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition_v1"

echo "========================================="
echo "停止 SSL 代理模式的所有服务"
echo "========================================="
echo ""

# 停止 nginx_sslkey_partition
echo "[1/3] 停止 SSL 前端服务器..."
cd "$BASE_DIR/nginx_sslkey_partion"
if [ -f "./objs/nginx" ]; then
    ./objs/nginx -s stop 2>/dev/null
    sleep 0.5
    echo "      ✓ nginx_sslkey_partition 已停止"
else
    echo "      - nginx_sslkey_partition 未运行"
fi

# 停止 nginx_public_partition
echo ""
echo "[2/3] 停止应用服务器..."
cd "$BASE_DIR/nginx_public_partition"
if [ -f "./objs/nginx" ]; then
    ./objs/nginx -s stop 2>/dev/null
    sleep 0.5
    echo "      ✓ nginx_public_partition 已停止"
else
    echo "      - nginx_public_partition 未运行"
fi

# 停止 RPC 服务器
echo ""
echo "[3/3] 停止认证 RPC 服务器..."
if pkill -f nginx_auth_rpc_server 2>/dev/null; then
    sleep 0.5
    echo "      ✓ 认证 RPC 服务器已停止"
else
    echo "      - 认证 RPC 服务器未运行"
fi

echo ""
echo "========================================="
echo "✓ 所有服务已停止"
echo "========================================="
