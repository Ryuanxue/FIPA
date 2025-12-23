#!/bin/bash
#
# quick_patch.sh - 快速修改 nginx 使用共享库
#
# 此脚本会自动修改 ngx_event_openssl.c，将原有的
# ngx_ssl_certificate 和 ngx_ssl_certificates 函数实现
# 注释掉，并使用存根文件重定向到共享库
#

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

print_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NGINX_ROOT="$SCRIPT_DIR/.."
OPENSSL_C="$NGINX_ROOT/src/event/ngx_event_openssl.c"

print_info "========================================="
print_info "快速修补：使用共享库替代静态函数"
print_info "========================================="
echo ""

# 1. 备份
if [ ! -f "${OPENSSL_C}.original" ]; then
    print_info "备份原文件..."
    cp "$OPENSSL_C" "${OPENSSL_C}.original"
    print_info "✓ 备份到: ${OPENSSL_C}.original"
fi

# 2. 查找需要注释的函数位置
print_info "分析源代码..."

# 查找 ngx_ssl_certificate 函数的起始行
CERT_FUNC_START=$(grep -n "^ngx_ssl_certificate(ngx_conf_t \*cf, ngx_ssl_t \*ssl, ngx_str_t \*cert," "$OPENSSL_C" | cut -d: -f1 | head -1)
# 查找 ngx_ssl_certificates 函数的起始行  
CERTS_FUNC_START=$(grep -n "^ngx_ssl_certificates(ngx_conf_t \*cf, ngx_ssl_t \*ssl, ngx_array_t \*certs," "$OPENSSL_C" | cut -d: -f1 | head -1)

if [ -z "$CERT_FUNC_START" ] || [ -z "$CERTS_FUNC_START" ]; then
    print_error "无法找到函数位置"
    print_info "可能已经修改过，或文件格式不匹配"
    exit 1
fi

print_info "找到函数位置："
print_info "  ngx_ssl_certificate: 行 $CERT_FUNC_START"
print_info "  ngx_ssl_certificates: 行 $CERTS_FUNC_START"

# 3. 创建修改后的文件
print_info "应用修改..."

# 方案：在原函数前添加 #if 0 ... #endif 来禁用

# 使用 sed 在指定行添加注释
cp "$OPENSSL_C" "${OPENSSL_C}.tmp"

# 在 ngx_ssl_certificates 函数前添加 #if 0
sed -i "${CERTS_FUNC_START}i\\
#if 0  /* DISABLED: Using shared library implementation */
" "${OPENSSL_C}.tmp"

# 在 ngx_ssl_certificate 函数后找到结束的 }，添加 #endif
# 这里简化处理：在 ngx_ssl_password_callback 函数前添加 #endif
PASSWORD_CB_LINE=$(grep -n "^static int$" "${OPENSSL_C}.tmp" | grep -A1 "ngx_ssl_password_callback" | head -1 | cut -d: -f1)
if [ -n "$PASSWORD_CB_LINE" ]; then
    sed -i "${PASSWORD_CB_LINE}i\\
#endif  /* DISABLED: Using shared library implementation */
" "${OPENSSL_C}.tmp"
fi

mv "${OPENSSL_C}.tmp" "$OPENSSL_C"
print_info "✓ 已注释原函数实现"

# 4. 添加头文件引用
print_info "添加共享库头文件引用..."

# 在 #include <sslkey_auth_idl.h> 后添加
sed -i '/#include <sslkey_auth_idl.h>/a\
\
/* 使用共享库实现的 SSL 证书加载函数 */\
#include "ngx_event_openssl_stub.c"
' "$OPENSSL_C"

print_info "✓ 已添加存根文件引用"

# 5. 完成
echo ""
print_info "========================================="
print_info "修改完成！"
print_info "========================================="
echo ""
print_info "下一步："
print_info "  1. 编译共享库:"
print_info "     cd $SCRIPT_DIR && make"
print_info ""
print_info "  2. 重新编译 nginx:"
print_info "     cd $NGINX_ROOT && make"
print_info ""
print_info "  3. 测试 nginx:"
print_info "     export LD_LIBRARY_PATH=$SCRIPT_DIR:\$LD_LIBRARY_PATH"
print_info "     ./objs/nginx -t"
echo ""
print_warning "如需恢复原文件："
print_info "     cp ${OPENSSL_C}.original $OPENSSL_C"
echo ""

exit 0
