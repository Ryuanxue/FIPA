#!/bin/bash
#
# build.sh - 自动化构建脚本
# 用于编译 libngx_ssl_cert.so 共享库
#

set -e  # 遇到错误立即退出

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 打印函数
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# 脚本目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# 配置
NGINX_ROOT="../"
BUILD_TYPE="${1:-simple}"  # simple 或 advanced

print_info "================================"
print_info "构建 libngx_ssl_cert.so"
print_info "================================"

# 检查 nginx 源码
if [ ! -f "$NGINX_ROOT/src/core/nginx.h" ]; then
    print_error "未找到 nginx 源码，请检查路径: $NGINX_ROOT"
    exit 1
fi
print_info "✓ Nginx 源码路径: $NGINX_ROOT"

# 检查 nginx 是否已编译
if [ ! -f "$NGINX_ROOT/objs/ngx_auto_config.h" ]; then
    print_warning "Nginx 尚未配置，正在运行 ./configure..."
    cd "$NGINX_ROOT"
    ./configure --with-http_ssl_module
    cd "$SCRIPT_DIR"
fi

if [ ! -f "$NGINX_ROOT/objs/src/core/ngx_log.o" ]; then
    print_warning "Nginx 对象文件不存在，正在编译..."
    cd "$NGINX_ROOT"
    make
    cd "$SCRIPT_DIR"
fi
print_info "✓ Nginx 已编译"

# 检查依赖
print_info "检查依赖..."

if ! pkg-config --exists openssl; then
    print_error "OpenSSL 未安装，请安装: sudo apt-get install libssl-dev"
    exit 1
fi
print_info "✓ OpenSSL: $(pkg-config --modversion openssl)"

# 选择 Makefile
if [ "$BUILD_TYPE" = "advanced" ]; then
    MAKEFILE="Makefile.advanced"
    print_info "使用高级 Makefile（链接 nginx 核心对象）"
else
    MAKEFILE="Makefile"
    print_info "使用简单 Makefile"
fi

# 清理旧文件
print_info "清理旧文件..."
make -f "$MAKEFILE" clean 2>/dev/null || true

# 编译
print_info "开始编译..."
if make -f "$MAKEFILE"; then
    print_info "✓ 编译成功"
else
    print_error "编译失败"
    exit 1
fi

# 检查生成的库
if [ -f "libngx_ssl_cert.so" ]; then
    print_info "✓ 共享库已生成: libngx_ssl_cert.so"
    
    # 显示库信息
    print_info ""
    print_info "库信息:"
    ls -lh libngx_ssl_cert.so
    
    print_info ""
    print_info "导出的符号:"
    nm -D libngx_ssl_cert.so | grep "T ngx_ssl" || print_warning "未找到导出符号"
    
    print_info ""
    print_info "库依赖:"
    ldd libngx_ssl_cert.so
else
    print_error "共享库未生成"
    exit 1
fi

# 编译测试程序
print_info ""
print_info "编译测试程序..."
if make -f "$MAKEFILE" test; then
    print_info "✓ 测试程序编译成功"
else
    print_warning "测试程序编译失败（可选）"
fi

print_info ""
print_info "================================"
print_info "构建完成！"
print_info "================================"
print_info ""
print_info "使用方法："
print_info "  1. 运行测试: LD_LIBRARY_PATH=. ./test_cert <cert.pem> <key.pem>"
print_info "  2. 安装到系统: sudo make -f $MAKEFILE install"
print_info "  3. 在项目中使用:"
print_info "     gcc -o myapp myapp.c -L$SCRIPT_DIR -lngx_ssl_cert -lssl -lcrypto"
print_info ""

exit 0
