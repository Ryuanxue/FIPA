#!/bin/bash

# 更新nginx客户端的Makefile以包含RPC文件

NGINX_CLIENT_DIR="../examples/nginx_sslkey_auth/output/finally_partition/nginx_sslkey_auth_client"
MAKEFILE="$NGINX_CLIENT_DIR/objs/Makefile"

echo "正在更新nginx客户端Makefile..."

# 备份原始Makefile
cp "$MAKEFILE" "$MAKEFILE.backup"

# 在对象文件列表中添加RPC相关文件
sed -i '/objs\/src\/core\/ngx_syslog\.o \\/a\\tobjs/src/core/nginx_sslkey_auth_rpc_wrapper.o \\\
\tobjs/src/core/nginx_sslkey_auth_simple_clnt.o \\\
\tobjs/src/core/nginx_sslkey_auth_simple_xdr.o \\' "$MAKEFILE"

# 在链接命令中添加RPC对象文件
sed -i '/objs\/src\/core\/ngx_syslog\.o \\/a\\tobjs/src/core/nginx_sslkey_auth_rpc_wrapper.o \\\
\tobjs/src/core/nginx_sslkey_auth_simple_clnt.o \\\
\tobjs/src/core/nginx_sslkey_auth_simple_xdr.o \\' "$MAKEFILE"

# 在依赖文件中添加RPC wrapper头文件
sed -i '/src\/core\/ngx_syslog\.h \\/a\\tsrc/core/nginx_sslkey_auth_rpc_wrapper.h \\\
\tsrc/core/nginx_sslkey_auth_simple.h \\' "$MAKEFILE"

# 添加RPC文件的编译规则
cat >> "$MAKEFILE" << 'EOF'

# RPC wrapper文件编译规则
objs/src/core/nginx_sslkey_auth_rpc_wrapper.o:	$(CORE_DEPS) \
	src/core/nginx_sslkey_auth_rpc_wrapper.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o objs/src/core/nginx_sslkey_auth_rpc_wrapper.o \
		src/core/nginx_sslkey_auth_rpc_wrapper.c

objs/src/core/nginx_sslkey_auth_simple_clnt.o:	$(CORE_DEPS) \
	src/core/nginx_sslkey_auth_simple_clnt.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o objs/src/core/nginx_sslkey_auth_simple_clnt.o \
		src/core/nginx_sslkey_auth_simple_clnt.c

objs/src/core/nginx_sslkey_auth_simple_xdr.o:	$(CORE_DEPS) \
	src/core/nginx_sslkey_auth_simple_xdr.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) \
		-o objs/src/core/nginx_sslkey_auth_simple_xdr.o \
		src/core/nginx_sslkey_auth_simple_xdr.c
EOF

echo "Makefile 更新完成！"
echo "备份文件保存为: $MAKEFILE.backup"
