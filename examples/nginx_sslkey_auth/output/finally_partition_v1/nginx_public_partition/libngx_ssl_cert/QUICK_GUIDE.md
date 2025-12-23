# 使用共享库替代静态函数 - 快速参考

## 方案总结

有三种方案可以将 nginx 中的 `ngx_ssl_certificate` 相关函数改为使用共享库：

### 方案一：使用存根文件（推荐 - 最简单）

**步骤：**

1. **创建存根文件** - 已完成
   - 文件位置：`src/event/ngx_event_openssl_stub.c`
   - 作用：提供函数实现，内部调用共享库

2. **修改 ngx_event_openssl.c**
   ```bash
   # 在文件开头添加
   #include "ngx_event_openssl_stub.c"
   
   # 注释掉原有的两个函数实现：
   # - ngx_ssl_certificate() (约第 550-780 行)
   # - ngx_ssl_certificates() (约第 524-548 行)
   
   # 使用 #if 0 ... #endif 包围
   ```

3. **编译**
   ```bash
   cd libngx_ssl_cert && make
   cd .. && make
   ```

**一键执行：**
```bash
cd libngx_ssl_cert
./quick_patch.sh
```

---

### 方案二：替换函数实现（中等）

**步骤：**

1. 在 `ngx_event_openssl.c` 开头添加：
   ```c
   #include <dlfcn.h>
   
   static void *lib_handle = NULL;
   static ngx_ssl_certificate_func_t cert_func = NULL;
   static ngx_ssl_certificates_func_t certs_func = NULL;
   ```

2. 删除原 `ngx_ssl_certificate()` 和 `ngx_ssl_certificates()` 的实现

3. 替换为动态加载版本：
   ```c
   ngx_int_t
   ngx_ssl_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *cert,
       ngx_str_t *key, ngx_array_t *passwords)
   {
       if (cert_func == NULL) {
           lib_handle = dlopen("./libngx_ssl_cert/libngx_ssl_cert.so", RTLD_LAZY);
           cert_func = dlsym(lib_handle, "ngx_ssl_certificate");
       }
       return cert_func(cf, ssl, cert, key, passwords);
   }
   ```

4. 编译时添加 `-ldl` 链接选项

---

### 方案三：完全模块化（高级 - 最灵活）

**步骤：**

1. 创建独立的动态模块
2. 使用 nginx 的模块系统加载
3. 通过函数指针表注册功能

**优点：**
- 最灵活
- 可以在运行时加载/卸载
- 符合 nginx 架构

**缺点：**
- 实现较复杂
- 需要深入了解 nginx 模块系统

---

## 详细修改步骤（方案一）

### 1. 备份文件

```bash
cp src/event/ngx_event_openssl.c src/event/ngx_event_openssl.c.backup
```

### 2. 编辑 ngx_event_openssl.c

在文件开头的 `#include` 部分末尾添加：

```c
#include <sslkey_auth_idl.h>

/* 使用共享库实现 */
#include "ngx_event_openssl_stub.c"
```

### 3. 注释原函数实现

找到并注释以下函数（使用 `#if 0 ... #endif`）：

**位置 1：** `ngx_ssl_certificates` 函数（约 524-548 行）
```c
#if 0  /* MIGRATED TO SHARED LIB */
ngx_int_t
ngx_ssl_certificates(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_array_t *certs,
    ngx_array_t *keys, ngx_array_t *passwords)
{
    // ... 整个函数体 ...
}
#endif
```

**位置 2：** `ngx_ssl_certificate` 函数（约 550-780 行）
```c
#if 0  /* MIGRATED TO SHARED LIB */
ngx_int_t
ngx_ssl_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *cert,
    ngx_str_t *key, ngx_array_t *passwords)
{
    // ... 整个函数体 ...
}
#endif
```

**位置 3：** `ngx_ssl_password_callback` 函数（如果只被这两个函数使用）
```c
#if 0  /* MIGRATED TO SHARED LIB */
static int
ngx_ssl_password_callback(char *buf, int size, int rwflag, void *userdata)
{
    // ... 整个函数体 ...
}
#endif
```

### 4. 添加清理调用

在 `ngx_openssl_exit()` 函数中添加：

```c
static void
ngx_openssl_exit(ngx_cycle_t *cycle)
{
    // ...existing code...
    
    /* 卸载 SSL 证书共享库 */
    ngx_ssl_cert_unload_library();
}
```

### 5. 编译

```bash
# 编译共享库
cd libngx_ssl_cert
make clean && make

# 编译 nginx（确保链接 -ldl）
cd ..
./configure --with-http_ssl_module
make
```

### 6. 测试

```bash
export LD_LIBRARY_PATH=./libngx_ssl_cert:$LD_LIBRARY_PATH
./objs/nginx -t
./objs/nginx
```

---

## 验证检查

编译后检查：

```bash
# 1. 检查 nginx 可执行文件依赖
ldd ./objs/nginx | grep ssl_cert

# 2. 检查共享库符号
nm -D libngx_ssl_cert/libngx_ssl_cert.so | grep ngx_ssl

# 3. 运行测试
./objs/nginx -t

# 4. 检查日志
tail -f logs/error.log
```

---

## 故障排除

### 问题：找不到共享库

**解决：**
```bash
export LD_LIBRARY_PATH=/path/to/libngx_ssl_cert:$LD_LIBRARY_PATH
# 或在链接时使用 -Wl,-rpath
```

### 问题：符号未定义

**解决：**
```bash
# 检查符号
nm -D libngx_ssl_cert.so | grep ngx_ssl_certificate

# 确保函数没有被 static 修饰
# 确保使用了正确的导出宏
```

### 问题：段错误

**解决：**
```bash
# 使用 gdb 调试
gdb ./objs/nginx
(gdb) run -t
(gdb) bt  # 查看堆栈

# 检查编译选项是否一致
# 检查 ABI 兼容性
```

---

## 完整示例

```bash
# 完整流程
cd /path/to/nginx_public_partition

# 1. 编译共享库
cd libngx_ssl_cert
make clean
make
cd ..

# 2. 备份并修改源文件
cp src/event/ngx_event_openssl.c src/event/ngx_event_openssl.c.backup

# 3. 运行快速修补脚本
cd libngx_ssl_cert
./quick_patch.sh
cd ..

# 4. 重新编译 nginx
make clean
./configure --with-http_ssl_module
make

# 5. 测试
export LD_LIBRARY_PATH=./libngx_ssl_cert:$LD_LIBRARY_PATH
./objs/nginx -t
./objs/nginx

# 6. 查看日志确认共享库加载
tail -f logs/error.log | grep "SSL certificate"
```

---

## 恢复原状

如果需要恢复：

```bash
cp src/event/ngx_event_openssl.c.backup src/event/ngx_event_openssl.c
make clean
make
```
