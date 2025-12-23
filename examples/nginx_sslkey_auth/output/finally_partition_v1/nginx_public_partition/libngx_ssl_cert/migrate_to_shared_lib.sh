#!/bin/bash
#
# migrate_to_shared_lib.sh
# 
# 将 nginx 中的 ngx_ssl_certificate 相关函数迁移到共享库调用
#

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }
print_step() { echo -e "${BLUE}[STEP]${NC} $1"; }

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NGINX_ROOT="$SCRIPT_DIR/.."
OPENSSL_C="$NGINX_ROOT/src/event/ngx_event_openssl.c"
OPENSSL_H="$NGINX_ROOT/src/event/ngx_event_openssl.h"

print_info "=================================="
print_info "迁移 ngx_ssl_certificate 到共享库"
print_info "=================================="
echo ""

# 步骤 1: 备份原文件
print_step "1. 备份原始文件"
if [ ! -f "${OPENSSL_C}.backup" ]; then
    cp "$OPENSSL_C" "${OPENSSL_C}.backup"
    print_info "✓ 备份: ${OPENSSL_C}.backup"
fi
if [ ! -f "${OPENSSL_H}.backup" ]; then
    cp "$OPENSSL_H" "${OPENSSL_H}.backup"
    print_info "✓ 备份: ${OPENSSL_H}.backup"
fi
echo ""

# 步骤 2: 修改头文件
print_step "2. 修改头文件 - 添加共享库声明"
cat > "${SCRIPT_DIR}/shared_lib_patch.h" << 'EOF'
/* 
 * 共享库函数声明
 * 这些函数将从 libngx_ssl_cert.so 动态加载
 */

#ifndef _NGX_SSL_CERT_SHARED_LIB_H_
#define _NGX_SSL_CERT_SHARED_LIB_H_

#include <dlfcn.h>

/* 函数指针类型定义 */
typedef ngx_int_t (*ngx_ssl_cert_init_func_t)(ngx_log_t *log);
typedef ngx_int_t (*ngx_ssl_certificate_func_t)(ngx_conf_t *cf, ngx_ssl_t *ssl,
                                                 ngx_str_t *cert, ngx_str_t *key,
                                                 ngx_array_t *passwords);
typedef ngx_int_t (*ngx_ssl_certificates_func_t)(ngx_conf_t *cf, ngx_ssl_t *ssl,
                                                  ngx_array_t *certs, ngx_array_t *keys,
                                                  ngx_array_t *passwords);

/* 全局函数指针 */
extern void *ngx_ssl_cert_lib_handle;
extern ngx_ssl_cert_init_func_t ngx_ssl_cert_init_dl;
extern ngx_ssl_certificate_func_t ngx_ssl_certificate_dl;
extern ngx_ssl_certificates_func_t ngx_ssl_certificates_dl;

/* 动态库加载/卸载函数 */
ngx_int_t ngx_ssl_cert_load_shared_lib(ngx_log_t *log);
void ngx_ssl_cert_unload_shared_lib(void);

#endif /* _NGX_SSL_CERT_SHARED_LIB_H_ */
EOF
print_info "✓ 创建: ${SCRIPT_DIR}/shared_lib_patch.h"
echo ""

# 步骤 3: 创建修改说明文档
print_step "3. 创建迁移指南"
cat > "${SCRIPT_DIR}/MIGRATION_GUIDE.md" << 'EOF'
# 迁移 ngx_ssl_certificate 到共享库

## 概述

本指南说明如何将 nginx 中的 `ngx_ssl_certificate` 和 `ngx_ssl_certificates` 函数
从静态编译改为使用共享库 `libngx_ssl_cert.so` 动态加载。

## 修改步骤

### 1. 修改 ngx_event_openssl.c

#### 1.1 添加头文件和全局变量

在文件开头添加：

```c
#include <dlfcn.h>
#include "../libngx_ssl_cert/ngx_ssl_cert_wrapper.h"

/* 共享库句柄和函数指针 */
static void *ngx_ssl_cert_lib_handle = NULL;
static ngx_ssl_cert_init_func_t ngx_ssl_cert_init_dl = NULL;
static ngx_ssl_certificate_func_t ngx_ssl_certificate_dl = NULL;
static ngx_ssl_certificates_func_t ngx_ssl_certificates_dl = NULL;
```

#### 1.2 添加动态库加载函数

```c
/* 加载共享库 */
static ngx_int_t
ngx_ssl_cert_load_shared_lib(ngx_log_t *log)
{
    const char *lib_path = "./libngx_ssl_cert/libngx_ssl_cert.so";
    char *error;

    if (ngx_ssl_cert_lib_handle != NULL) {
        /* 已加载 */
        return NGX_OK;
    }

    /* 加载共享库 */
    ngx_ssl_cert_lib_handle = dlopen(lib_path, RTLD_LAZY);
    if (!ngx_ssl_cert_lib_handle) {
        ngx_log_error(NGX_LOG_EMERG, log, 0,
                      "dlopen(\"%s\") failed: %s", lib_path, dlerror());
        return NGX_ERROR;
    }

    /* 清除错误 */
    dlerror();

    /* 加载 ngx_ssl_cert_init */
    ngx_ssl_cert_init_dl = (ngx_ssl_cert_init_func_t)
        dlsym(ngx_ssl_cert_lib_handle, "ngx_ssl_cert_init");
    if ((error = dlerror()) != NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, 0,
                      "dlsym(ngx_ssl_cert_init) failed: %s", error);
        dlclose(ngx_ssl_cert_lib_handle);
        ngx_ssl_cert_lib_handle = NULL;
        return NGX_ERROR;
    }

    /* 加载 ngx_ssl_certificate */
    ngx_ssl_certificate_dl = (ngx_ssl_certificate_func_t)
        dlsym(ngx_ssl_cert_lib_handle, "ngx_ssl_certificate");
    if ((error = dlerror()) != NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, 0,
                      "dlsym(ngx_ssl_certificate) failed: %s", error);
        dlclose(ngx_ssl_cert_lib_handle);
        ngx_ssl_cert_lib_handle = NULL;
        return NGX_ERROR;
    }

    /* 加载 ngx_ssl_certificates */
    ngx_ssl_certificates_dl = (ngx_ssl_certificates_func_t)
        dlsym(ngx_ssl_cert_lib_handle, "ngx_ssl_certificates");
    if ((error = dlerror()) != NULL) {
        ngx_log_error(NGX_LOG_EMERG, log, 0,
                      "dlsym(ngx_ssl_certificates) failed: %s", error);
        dlclose(ngx_ssl_cert_lib_handle);
        ngx_ssl_cert_lib_handle = NULL;
        return NGX_ERROR;
    }

    /* 初始化共享库 */
    if (ngx_ssl_cert_init_dl(log) != NGX_OK) {
        ngx_log_error(NGX_LOG_EMERG, log, 0,
                      "ngx_ssl_cert_init() failed");
        dlclose(ngx_ssl_cert_lib_handle);
        ngx_ssl_cert_lib_handle = NULL;
        return NGX_ERROR;
    }

    ngx_log_error(NGX_LOG_NOTICE, log, 0,
                  "SSL certificate shared library loaded successfully");

    return NGX_OK;
}

/* 卸载共享库 */
static void
ngx_ssl_cert_unload_shared_lib(void)
{
    if (ngx_ssl_cert_lib_handle != NULL) {
        dlclose(ngx_ssl_cert_lib_handle);
        ngx_ssl_cert_lib_handle = NULL;
        ngx_ssl_cert_init_dl = NULL;
        ngx_ssl_certificate_dl = NULL;
        ngx_ssl_certificates_dl = NULL;
    }
}
```

#### 1.3 修改 ngx_ssl_init() 函数

在 `ngx_ssl_init()` 函数中添加共享库加载：

```c
ngx_int_t
ngx_ssl_init(ngx_log_t *log)
{
    // ...existing code...

    /* 加载 SSL 证书共享库 */
    if (ngx_ssl_cert_load_shared_lib(log) != NGX_OK) {
        return NGX_ERROR;
    }

    // ...rest of existing code...
}
```

#### 1.4 注释或删除原有函数实现

将以下函数的实现注释掉或删除：

- `ngx_ssl_certificate()` (大约第 550-780 行)
- `ngx_ssl_certificates()` (大约第 524-548 行)
- `ngx_ssl_password_callback()` (如果只在这两个函数中使用)

**重要**: 保留全局变量声明：
```c
int  ngx_ssl_certificate_index;
int  ngx_ssl_certificate_name_index;
int  ngx_ssl_next_certificate_index;
```

这些变量现在由共享库管理，但仍需在主程序中声明为 extern。

#### 1.5 修改函数调用

将所有对 `ngx_ssl_certificate()` 的调用改为：

```c
// 原来：
ngx_ssl_certificate(cf, ssl, cert, key, passwords)

// 修改为：
ngx_ssl_certificate_dl(cf, ssl, cert, key, passwords)
```

将所有对 `ngx_ssl_certificates()` 的调用改为：

```c
// 原来：
ngx_ssl_certificates(cf, ssl, certs, keys, passwords)

// 修改为：
ngx_ssl_certificates_dl(cf, ssl, certs, keys, passwords)
```

#### 1.6 添加清理函数

在 `ngx_openssl_exit()` 中添加共享库卸载：

```c
static void
ngx_openssl_exit(ngx_cycle_t *cycle)
{
    // ...existing code...

    /* 卸载 SSL 证书共享库 */
    ngx_ssl_cert_unload_shared_lib();
}
```

### 2. 修改 ngx_event_openssl.h

#### 2.1 移除函数声明

注释或删除以下声明：

```c
// ngx_int_t ngx_ssl_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl,
//     ngx_str_t *cert, ngx_str_t *key, ngx_array_t *passwords);
// ngx_int_t ngx_ssl_certificates(ngx_conf_t *cf, ngx_ssl_t *ssl,
//     ngx_array_t *certs, ngx_array_t *keys, ngx_array_t *passwords);
```

#### 2.2 添加函数指针类型定义

```c
/* 共享库函数指针类型 */
typedef ngx_int_t (*ngx_ssl_certificate_func_t)(ngx_conf_t *cf, ngx_ssl_t *ssl,
                                                 ngx_str_t *cert, ngx_str_t *key,
                                                 ngx_array_t *passwords);
typedef ngx_int_t (*ngx_ssl_certificates_func_t)(ngx_conf_t *cf, ngx_ssl_t *ssl,
                                                  ngx_array_t *certs, ngx_array_t *keys,
                                                  ngx_array_t *passwords);
```

### 3. 修改 Makefile 或构建配置

#### 3.1 添加链接选项

在编译 nginx 时添加 `-ldl` 选项以支持动态加载：

```makefile
LDFLAGS += -ldl
```

#### 3.2 设置运行时库路径

方式一：使用 LD_LIBRARY_PATH

```bash
export LD_LIBRARY_PATH=/path/to/libngx_ssl_cert:$LD_LIBRARY_PATH
./objs/nginx
```

方式二：使用 RPATH（推荐）

在链接时添加：
```makefile
LDFLAGS += -Wl,-rpath,/path/to/libngx_ssl_cert
```

### 4. 处理全局变量

共享库中定义的全局变量需要在主程序中声明为 extern：

在 `ngx_event_openssl.c` 中修改：

```c
// 原来：
int  ngx_ssl_certificate_index;
int  ngx_ssl_certificate_name_index;

// 修改为（如果这些变量在共享库中）：
extern int  ngx_ssl_certificate_index;
extern int  ngx_ssl_certificate_name_index;
```

**或者**保持在主程序中定义，共享库中声明为 extern。

### 5. 编译和测试

#### 5.1 编译共享库

```bash
cd libngx_ssl_cert
make clean
make
```

#### 5.2 编译 nginx

```bash
cd ..
./configure --with-http_ssl_module
make
```

#### 5.3 测试

```bash
# 设置库路径
export LD_LIBRARY_PATH=./libngx_ssl_cert:$LD_LIBRARY_PATH

# 运行 nginx
./objs/nginx -t
./objs/nginx
```

## 注意事项

### 1. 路径问题

- 确保共享库路径正确
- 可以使用绝对路径或相对路径
- 建议在代码中提供多个备选路径

```c
const char *lib_paths[] = {
    "./libngx_ssl_cert/libngx_ssl_cert.so",
    "/usr/local/lib/libngx_ssl_cert.so",
    "libngx_ssl_cert.so",
    NULL
};

for (int i = 0; lib_paths[i] != NULL; i++) {
    ngx_ssl_cert_lib_handle = dlopen(lib_paths[i], RTLD_LAZY);
    if (ngx_ssl_cert_lib_handle != NULL) {
        break;
    }
}
```

### 2. 符号冲突

- 确保共享库中的符号不与主程序冲突
- 使用 `nm` 命令检查符号
- 必要时使用 `static` 限制符号可见性

### 3. 内存管理

- 主程序和共享库可能使用不同的堆
- 确保内存在同一侧分配和释放
- 考虑使用传递内存池的方式

### 4. 错误处理

- 共享库加载失败时要有友好的错误提示
- 提供降级方案（可选）
- 记录详细的日志

## 回滚方案

如果迁移遇到问题，可以快速回滚：

```bash
# 恢复备份文件
cp src/event/ngx_event_openssl.c.backup src/event/ngx_event_openssl.c
cp src/event/ngx_event_openssl.h.backup src/event/ngx_event_openssl.h

# 重新编译
make clean
./configure --with-http_ssl_module
make
```

## 验证清单

- [ ] 共享库编译成功
- [ ] nginx 编译成功
- [ ] nginx -t 测试通过
- [ ] SSL 证书加载正常
- [ ] HTTPS 连接工作正常
- [ ] 日志中没有错误
- [ ] 性能没有明显下降

## 常见问题

### Q: 找不到共享库

A: 设置 `LD_LIBRARY_PATH` 或使用 `-Wl,-rpath`

### Q: 符号未定义

A: 检查共享库是否正确导出符号：`nm -D libngx_ssl_cert.so`

### Q: 段错误

A: 可能是 ABI 不兼容，检查编译选项是否一致

### Q: 性能问题

A: 动态加载只在启动时发生一次，不影响运行时性能
EOF

print_info "✓ 创建: ${SCRIPT_DIR}/MIGRATION_GUIDE.md"
echo ""

# 步骤 4: 创建自动化修改脚本
print_step "4. 创建自动化修改脚本"
cat > "${SCRIPT_DIR}/apply_patches.sh" << 'PATCH_SCRIPT'
#!/bin/bash
# apply_patches.sh - 自动应用修改

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NGINX_ROOT="$SCRIPT_DIR/.."

echo "正在修改 ngx_event_openssl.c ..."

# 在 ngx_ssl_certificate 函数前添加注释标记
sed -i '/^ngx_int_t$/,/^ngx_ssl_certificate(ngx_conf_t \*cf/ {
    s/^ngx_int_t$/\/* MIGRATED_TO_SHARED_LIB\nngx_int_t/
}' "$NGINX_ROOT/src/event/ngx_event_openssl.c"

echo "修改完成！"
echo "请查看 MIGRATION_GUIDE.md 了解详细步骤"
PATCH_SCRIPT

chmod +x "${SCRIPT_DIR}/apply_patches.sh"
print_info "✓ 创建: ${SCRIPT_DIR}/apply_patches.sh"
echo ""

# 完成
print_info "=================================="
print_info "迁移准备完成！"
print_info "=================================="
echo ""
print_info "下一步："
print_info "  1. 阅读迁移指南: cat libngx_ssl_cert/MIGRATION_GUIDE.md"
print_info "  2. 手动修改源文件（推荐）或运行自动脚本"
print_info "  3. 编译共享库: cd libngx_ssl_cert && make"
print_info "  4. 重新编译 nginx: make"
print_info ""
print_warning "重要: 请先备份重要数据！"
print_info "备份文件已创建："
print_info "  - ${OPENSSL_C}.backup"
print_info "  - ${OPENSSL_H}.backup"
echo ""

exit 0
