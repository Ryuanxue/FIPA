#!/bin/bash

# 修复包含顺序的脚本
find /home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition/nginx_sslkey_auth_client/src -name "*.c" | while read file; do
    if grep -q '#include "nginx_sslkey_auth_rpc_wrapper.h"' "$file"; then
        echo "修复文件: $file"
        
        # 创建临时文件
        tmpfile=$(mktemp)
        
        # 处理文件：删除wrapper包含行，然后在nginx头文件后添加简化版本
        sed '/^#include "nginx_sslkey_auth_rpc_wrapper.h"$/d' "$file" > "$tmpfile"
        
        # 在nginx相关头文件后添加简化wrapper
        if grep -q '#include <nginx.h>' "$tmpfile"; then
            sed '/^#include <nginx.h>$/a\
#include "nginx_sslkey_auth_rpc_wrapper_simple.h"' "$tmpfile" > "$file"
        elif grep -q '#include <ngx_core.h>' "$tmpfile"; then
            sed '/^#include <ngx_core.h>$/a\
#include "nginx_sslkey_auth_rpc_wrapper_simple.h"' "$tmpfile" > "$file"
        elif grep -q '#include <ngx_config.h>' "$tmpfile"; then
            sed '/^#include <ngx_config.h>$/a\
#include "nginx_sslkey_auth_rpc_wrapper_simple.h"' "$tmpfile" > "$file"
        else
            # 如果没有找到合适的位置，在第一个include后添加
            sed '1,/^#include/ { /^#include/ a\
#include "nginx_sslkey_auth_rpc_wrapper_simple.h"
}' "$tmpfile" > "$file"
        fi
        
        rm "$tmpfile"
    fi
done

echo "包含顺序修复完成！"
