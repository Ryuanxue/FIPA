#!/bin/bash

# 生成自签名SSL证书脚本
# 此脚本用于生成 nginx SSL 配置所需的证书文件

echo "正在生成自签名SSL证书..."

# 创建证书目录
CERT_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/input/source_code/nginx-1.15.5/ssl"
mkdir -p "$CERT_DIR"

# 生成私钥
openssl genrsa -out "$CERT_DIR/cert.key" 2048

# 生成证书签名请求 (CSR)
openssl req -new -key "$CERT_DIR/cert.key" -out "$CERT_DIR/cert.csr" -subj "/C=CN/ST=Beijing/L=Beijing/O=Test/OU=IT Department/CN=localhost"

# 生成自签名证书
openssl x509 -req -days 365 -in "$CERT_DIR/cert.csr" -signkey "$CERT_DIR/cert.key" -out "$CERT_DIR/cert.pem"

# 设置适当的权限
chmod 600 "$CERT_DIR/cert.key"
chmod 644 "$CERT_DIR/cert.pem"

echo "SSL证书已生成完成:"
echo "  私钥: $CERT_DIR/cert.key"
echo "  证书: $CERT_DIR/cert.pem"
echo ""
echo "注意: 这是自签名证书，浏览器会显示安全警告。"
echo "生产环境请使用由受信任CA签发的证书。"

# 清理临时文件
rm -f "$CERT_DIR/cert.csr"
