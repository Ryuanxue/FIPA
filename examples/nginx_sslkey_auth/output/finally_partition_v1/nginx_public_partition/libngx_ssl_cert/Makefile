# Makefile for libngx_ssl_cert.so
# 编译 ngx_ssl_certificate 及其依赖函数为共享库

# 编译器和标志
CC = gcc
CFLAGS = -fPIC -Wall -O2 -g
LDFLAGS = -shared

# 包含路径
NGINX_ROOT = ..
INCLUDES = -I$(NGINX_ROOT)/src/core \
           -I$(NGINX_ROOT)/src/event \
           -I$(NGINX_ROOT)/src/os/unix \
           -I$(NGINX_ROOT)/objs \
           -I/usr/include/openssl

# 库依赖
LIBS = -lssl -lcrypto

# 目标库
TARGET = libngx_ssl_cert.so

# 源文件 - 从 ngx_event_openssl.c 提取需要的函数
SOURCES = ngx_ssl_cert_wrapper.c

# 目标文件
OBJECTS = $(SOURCES:.c=.o)

# 默认目标
all: $(TARGET)

# 编译共享库
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo "共享库 $(TARGET) 编译完成"

# 编译目标文件
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 清理
clean:
	rm -f $(OBJECTS) $(TARGET)

# 安装
install: $(TARGET)
	cp $(TARGET) /usr/local/lib/
	ldconfig

# 测试
test: test_main.c $(TARGET)
	$(CC) -o test_cert test_main.c -L. -lngx_ssl_cert $(LIBS) $(INCLUDES)
	@echo "测试程序编译完成，运行: LD_LIBRARY_PATH=. ./test_cert"

.PHONY: all clean install test
