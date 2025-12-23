# libngx_ssl_cert - Nginx SSL 证书加载共享库

## 概述

本共享库从 nginx 的 `ngx_event_openssl.c` 中提取 `ngx_ssl_certificate` 函数及其所有依赖，
编译为独立的共享库 `libngx_ssl_cert.so`，可供其他程序调用。

## 包含的函数

### 导出函数

1. **ngx_ssl_cert_init()** - 初始化函数（必须首先调用）
2. **ngx_ssl_certificate()** - 加载单个 SSL 证书和私钥
3. **ngx_ssl_certificates()** - 批量加载多个证书
4. **ngx_ssl_error()** - SSL 错误日志函数

### 内部函数

- `ngx_ssl_password_callback()` - 私钥密码回调

### 全局变量

- `ngx_ssl_certificate_index`
- `ngx_ssl_certificate_name_index`
- `ngx_ssl_next_certificate_index`

## 编译步骤

### 1. 准备环境

确保已安装以下依赖：

```bash
# Ubuntu/Debian
sudo apt-get install build-essential libssl-dev libpcre3-dev zlib1g-dev

# CentOS/RHEL
sudo yum install gcc make openssl-devel pcre-devel zlib-devel
```

### 2. 编译共享库

```bash
cd libngx_ssl_cert
make
```

这将生成 `libngx_ssl_cert.so` 文件。

### 3. 编译测试程序

```bash
make test
```

### 4. 运行测试

```bash
LD_LIBRARY_PATH=. ./test_cert /path/to/cert.pem /path/to/key.pem
```

### 5. 安装到系统（可选）

```bash
sudo make install
```

这会将库复制到 `/usr/local/lib/` 并运行 `ldconfig`。

## 使用示例

```c
#include "ngx_ssl_cert_wrapper.h"

int main() {
    ngx_log_t log;
    ngx_ssl_t ssl;
    ngx_conf_t cf;
    ngx_str_t cert, key;
    
    // 1. 初始化
    ngx_ssl_cert_init(&log);
    
    // 2. 创建 SSL 上下文
    ssl.ctx = SSL_CTX_new(SSLv23_server_method());
    
    // 3. 加载证书
    cert.data = (u_char *) "cert.pem";
    cert.len = strlen("cert.pem");
    key.data = (u_char *) "key.pem";
    key.len = strlen("key.pem");
    
    ngx_ssl_certificate(&cf, &ssl, &cert, &key, NULL);
    
    // 4. 使用 SSL 上下文...
    
    // 5. 清理
    SSL_CTX_free(ssl.ctx);
    
    return 0;
}
```

## 编译链接

编译使用此库的程序时：

```bash
gcc -o myapp myapp.c -L/path/to/libngx_ssl_cert -lngx_ssl_cert -lssl -lcrypto \
    -I/path/to/nginx/src/core -I/path/to/nginx/src/event -I/path/to/nginx/objs
```

运行时设置库路径：

```bash
LD_LIBRARY_PATH=/path/to/libngx_ssl_cert ./myapp
```

## 依赖项

### Nginx 核心库依赖

本库需要 nginx 的以下核心函数和结构：

- `ngx_conf_full_name()` - 路径处理
- `ngx_memcpy()`, `ngx_memzero()` - 内存操作
- `ngx_str_t`, `ngx_array_t` - 数据结构
- `ngx_log_error()` - 日志功能
- `ngx_ssl_error()` - SSL 错误处理

### OpenSSL 依赖

- libssl
- libcrypto

### 编译时包含路径

需要以下头文件：

- `nginx/src/core/ngx_core.h`
- `nginx/src/event/ngx_event_openssl.h`
- `nginx/objs/ngx_auto_config.h`
- OpenSSL headers

## 函数详细说明

### ngx_ssl_cert_init()

```c
ngx_int_t ngx_ssl_cert_init(ngx_log_t *log);
```

**功能**: 初始化 SSL 证书模块，必须在使用其他函数前调用

**参数**:
- `log`: nginx 日志对象指针

**返回值**:
- `NGX_OK`: 成功
- `NGX_ERROR`: 失败

### ngx_ssl_certificate()

```c
ngx_int_t ngx_ssl_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl,
                               ngx_str_t *cert, ngx_str_t *key,
                               ngx_array_t *passwords);
```

**功能**: 加载 SSL 证书和私钥到 SSL 上下文

**参数**:
- `cf`: nginx 配置对象
- `ssl`: SSL 上下文
- `cert`: 证书文件路径
- `key`: 私钥文件路径
- `passwords`: 密码数组（可选，用于加密私钥）

**返回值**:
- `NGX_OK`: 成功
- `NGX_ERROR`: 失败

**支持特性**:
- 证书链加载
- 加密私钥（带密码）
- ENGINE 私钥（如 `engine:id:key_id`）

## 注意事项

1. **初始化顺序**: 必须先调用 `ngx_ssl_cert_init()` 再使用其他函数
2. **线程安全**: 本库依赖 OpenSSL，需要确保 OpenSSL 已正确初始化线程支持
3. **内存管理**: SSL_CTX 由调用者管理，使用完毕后需调用 `SSL_CTX_free()`
4. **路径处理**: 证书和私钥路径会被 `ngx_conf_full_name()` 处理为绝对路径

## 故障排查

### 编译错误

如果遇到头文件找不到的错误：

```bash
# 检查 nginx 源码路径
ls -la ../src/core/ngx_core.h
ls -la ../objs/ngx_auto_config.h

# 修改 Makefile 中的 NGINX_ROOT 路径
```

### 运行时错误

```bash
# 检查库依赖
ldd libngx_ssl_cert.so

# 添加库路径到环境变量
export LD_LIBRARY_PATH=/path/to/libngx_ssl_cert:$LD_LIBRARY_PATH
```

### 符号未定义错误

如果链接时提示符号未定义，可能需要链接 nginx 的其他核心对象文件：

```bash
gcc -shared -o libngx_ssl_cert.so ngx_ssl_cert_wrapper.o \
    ../objs/src/core/ngx_*.o \
    -lssl -lcrypto
```

## 清理

```bash
make clean
```

## 版本信息

- 基于 nginx 1.15.5
- OpenSSL 1.0.2+

## 许可证

遵循 nginx 许可证（BSD-like）
