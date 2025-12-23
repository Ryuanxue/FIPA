# 将 ngx_ssl_certificate 编译为共享库 - 完整方案

## 概述

本方案提供了将 nginx 中的 `ngx_ssl_certificate` 及其相关函数编译为独立共享库（libngx_ssl_cert.so）的完整解决方案，包括：

1. ✅ 共享库的创建和编译
2. ✅ nginx 源码的修改方法
3. ✅ 自动化脚本和工具
4. ✅ 测试和验证步骤

## 文件结构

```
nginx_public_partition/
├── libngx_ssl_cert/                    # 共享库目录
│   ├── Makefile                        # 简单编译配置
│   ├── Makefile.advanced               # 高级编译配置（链接nginx核心）
│   ├── ngx_ssl_cert_wrapper.c          # 共享库实现
│   ├── ngx_ssl_cert_wrapper.h          # 共享库头文件
│   ├── test_main.c                     # 测试程序
│   ├── build.sh                        # 自动化构建脚本
│   ├── migrate_to_shared_lib.sh        # 迁移准备脚本
│   ├── quick_patch.sh                  # 快速修改脚本（推荐）
│   ├── QUICK_GUIDE.md                  # 快速入门指南（重要！）
│   ├── MIGRATION_GUIDE.md              # 详细迁移指南
│   └── README.md                       # 共享库说明文档
├── src/event/
│   ├── ngx_event_openssl.c             # 原nginx SSL实现（需修改）
│   ├── ngx_event_openssl.h             # 头文件
│   └── ngx_event_openssl_stub.c        # 存根文件（新增）
└── ...
```

## 快速开始（3步完成）

### 方式 A：自动化（推荐新手）

```bash
cd libngx_ssl_cert

# 步骤 1: 编译共享库
./build.sh

# 步骤 2: 自动修改 nginx 源码
./quick_patch.sh

# 步骤 3: 编译 nginx
cd ..
make
```

### 方式 B：手动（推荐专业用户）

```bash
# 步骤 1: 编译共享库
cd libngx_ssl_cert
make clean && make

# 步骤 2: 手动修改源码（参考 QUICK_GUIDE.md）
# 主要是在 ngx_event_openssl.c 中：
#   - 添加: #include "ngx_event_openssl_stub.c"
#   - 注释掉原 ngx_ssl_certificate 和 ngx_ssl_certificates 实现

# 步骤 3: 重新编译 nginx
cd ..
make clean
./configure --with-http_ssl_module
make
```

## 关键文件说明

### 1. 共享库核心文件

**ngx_ssl_cert_wrapper.c** 和 **ngx_ssl_cert_wrapper.h**
- 包含从 `ngx_event_openssl.c` 提取的函数实现
- 导出的主要函数：
  - `ngx_ssl_cert_init()` - 初始化
  - `ngx_ssl_certificate()` - 加载单个证书
  - `ngx_ssl_certificates()` - 批量加载证书
  - `ngx_ssl_error()` - 错误处理

### 2. 存根文件（关键）

**ngx_event_openssl_stub.c**
- 提供动态加载功能
- 自动查找共享库（多个路径）
- 透明地将调用重定向到共享库
- 包含错误处理和日志

### 3. 辅助脚本

| 脚本 | 用途 | 推荐度 |
|------|------|--------|
| `build.sh` | 自动编译共享库 | ⭐⭐⭐⭐⭐ |
| `quick_patch.sh` | 快速修改nginx源码 | ⭐⭐⭐⭐⭐ |
| `migrate_to_shared_lib.sh` | 生成迁移指南 | ⭐⭐⭐ |

### 4. 文档

| 文档 | 内容 | 适合人群 |
|------|------|----------|
| `QUICK_GUIDE.md` | 快速入门 | 所有人 |
| `MIGRATION_GUIDE.md` | 详细步骤 | 深入了解者 |
| `README.md` | API文档 | 开发者 |

## 修改原理

### 原来的调用链

```
nginx main
  └─> ngx_event_openssl.c
        └─> ngx_ssl_certificate()  [静态链接]
              └─> OpenSSL APIs
```

### 修改后的调用链

```
nginx main
  └─> ngx_event_openssl.c
        └─> #include "ngx_event_openssl_stub.c"
              └─> ngx_ssl_certificate()  [存根]
                    └─> dlopen("libngx_ssl_cert.so")
                          └─> ngx_ssl_certificate()  [共享库实现]
                                └─> OpenSSL APIs
```

## 需要修改的代码位置

### 修改 1: 添加存根文件引用

在 `src/event/ngx_event_openssl.c` 的头文件引用部分：

```c
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <sslkey_auth_idl.h>

/* 新增：使用共享库实现 */
#include "ngx_event_openssl_stub.c"
```

### 修改 2: 注释原函数实现

使用 `#if 0 ... #endif` 包围以下函数：

1. **ngx_ssl_certificates()** (约 524-548 行)
2. **ngx_ssl_certificate()** (约 550-780 行)  
3. **ngx_ssl_password_callback()** (约 782-803 行)

```c
#if 0  /* MIGRATED TO SHARED LIBRARY */

ngx_int_t
ngx_ssl_certificate(ngx_conf_t *cf, ngx_ssl_t *ssl, ngx_str_t *cert,
    ngx_str_t *key, ngx_array_t *passwords)
{
    // ... 原实现 ...
}

#endif  /* MIGRATED TO SHARED LIBRARY */
```

### 修改 3: 添加清理调用

在 `ngx_openssl_exit()` 函数末尾添加：

```c
static void
ngx_openssl_exit(ngx_cycle_t *cycle)
{
    // ...existing code...
    
    /* 卸载共享库 */
    ngx_ssl_cert_unload_library();
}
```

## 编译和运行

### 编译共享库

```bash
cd libngx_ssl_cert

# 方式 1: 使用构建脚本（推荐）
./build.sh

# 方式 2: 直接使用 Make
make clean
make

# 方式 3: 使用高级 Makefile（链接 nginx 核心对象）
make -f Makefile.advanced
```

### 编译 nginx

```bash
cd ..
make clean
./configure --with-http_ssl_module
make
```

### 运行 nginx

```bash
# 设置库搜索路径
export LD_LIBRARY_PATH=./libngx_ssl_cert:$LD_LIBRARY_PATH

# 测试配置
./objs/nginx -t

# 启动
./objs/nginx
```

## 验证步骤

### 1. 检查共享库

```bash
# 查看共享库信息
ls -lh libngx_ssl_cert/libngx_ssl_cert.so

# 查看导出的符号
nm -D libngx_ssl_cert/libngx_ssl_cert.so | grep ngx_ssl

# 查看库依赖
ldd libngx_ssl_cert/libngx_ssl_cert.so
```

### 2. 检查 nginx 二进制

```bash
# 查看依赖（应该包含 libdl）
ldd ./objs/nginx | grep dl

# 测试配置
./objs/nginx -t
```

### 3. 运行时验证

```bash
# 查看日志，应该看到共享库加载成功的消息
tail -f logs/error.log

# 应该看到类似：
# "SSL certificate shared library loaded from: ./libngx_ssl_cert/libngx_ssl_cert.so"
# "SSL certificate shared library initialized successfully"
```

## 常见问题和解决方案

### Q1: 找不到共享库

**错误：** `error while loading shared libraries: libngx_ssl_cert.so`

**解决：**
```bash
export LD_LIBRARY_PATH=./libngx_ssl_cert:$LD_LIBRARY_PATH
# 或
./configure --with-http_ssl_module --with-ld-opt="-Wl,-rpath,./libngx_ssl_cert"
```

### Q2: 符号未定义

**错误：** `undefined reference to ngx_ssl_certificate`

**解决：**
```bash
# 检查共享库符号
nm -D libngx_ssl_cert.so | grep ngx_ssl_certificate

# 确保没有使用 static 修饰函数
# 确保正确导出符号
```

### Q3: 编译错误

**错误：** 头文件找不到

**解决：**
```bash
# 检查 Makefile 中的包含路径
# 确保 nginx 已经配置和编译过
cd .. && ./configure && make
```

### Q4: 段错误

**解决：**
```bash
# 使用 gdb 调试
gdb ./objs/nginx
(gdb) run -t
(gdb) bt

# 检查编译选项是否一致
# 检查是否有符号冲突
```

## 性能考虑

- ✅ 动态加载只在启动时发生一次
- ✅ 运行时性能与静态链接相同
- ✅ 内存开销可忽略不计（约几KB）
- ✅ 支持热更新（重载共享库）

## 优势

1. **模块化**: 证书加载逻辑独立维护
2. **灵活性**: 可以独立更新共享库
3. **可测试**: 共享库可以独立测试
4. **可复用**: 其他程序也可以使用此共享库
5. **隔离性**: 错误不会直接影响主程序

## 下一步

1. **阅读快速指南**: `cat libngx_ssl_cert/QUICK_GUIDE.md`
2. **运行构建脚本**: `cd libngx_ssl_cert && ./build.sh`
3. **应用修改**: `./quick_patch.sh`
4. **测试**: `make && ./objs/nginx -t`

## 回滚

如果需要恢复原状：

```bash
# 恢复备份
cp src/event/ngx_event_openssl.c.backup src/event/ngx_event_openssl.c

# 重新编译
make clean
./configure --with-http_ssl_module
make
```

## 支持

- 📖 详细文档：`libngx_ssl_cert/MIGRATION_GUIDE.md`
- 🚀 快速开始：`libngx_ssl_cert/QUICK_GUIDE.md`
- 📚 API 文档：`libngx_ssl_cert/README.md`

## 总结

本方案提供了将 `ngx_ssl_certificate` 相关函数编译为共享库的完整解决方案，包括：

- ✅ 完整的共享库实现
- ✅ 自动化构建脚本
- ✅ 存根文件支持动态加载
- ✅ 详细的文档和指南
- ✅ 测试和验证工具

使用推荐的快速方法，只需3个命令即可完成迁移！
