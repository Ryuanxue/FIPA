#!/bin/bash
# GDB 诊断脚本 - 用于调试 types_hash NULL 问题

cat > /tmp/gdb_commands.txt << 'EOF'
# 设置断点在初始化代码处
break multi_idl_server.c:322
commands
  silent
  printf "=== Breakpoint: Setting ngx_http_core_module.ctx_index ===\n"
  printf "Before: ngx_http_core_module.ctx_index = %d\n", ngx_http_core_module.ctx_index
  continue
end

# 设置断点在 types_hash 初始化后
break multi_idl_server.c:328
commands
  silent
  printf "=== Breakpoint: After r->loc_conf[0] = clcf ===\n"
  printf "clcf = %p\n", clcf
  printf "r->loc_conf[0] = %p\n", r->loc_conf[0]
  printf "clcf->types_hash.size = %u\n", clcf->types_hash.size
  printf "clcf->types_hash.buckets = %p\n", clcf->types_hash.buckets
  printf "&clcf->types_hash = %p\n", &clcf->types_hash
  printf "ngx_http_core_module.ctx_index = %d\n", ngx_http_core_module.ctx_index
  continue
end

# 设置断点在 ngx_http_handler 调用处
break multi_idl_server.c:1480
commands
  silent
  printf "=== Breakpoint: Before ngx_http_handler ===\n"
  printf "r = %p\n", r
  printf "r->loc_conf = %p\n", r->loc_conf
  printf "r->loc_conf[0] = %p\n", r->loc_conf[0]
  printf "ngx_http_core_module.ctx_index = %d\n", ngx_http_core_module.ctx_index
  continue
end

# 设置断点在 ngx_http_set_content_type
break ngx_http_set_content_type
commands
  silent
  printf "=== Breakpoint: ngx_http_set_content_type ===\n"
  printf "r = %p\n", r
  printf "r->loc_conf = %p\n", r->loc_conf
  printf "ngx_http_core_module.ctx_index = %d\n", ngx_http_core_module.ctx_index
  printf "About to call ngx_http_get_module_loc_conf...\n"
  next
  printf "clcf = %p\n", clcf
  if clcf != 0
    printf "clcf->types_hash.size = %u\n", clcf->types_hash.size
    printf "clcf->types_hash.buckets = %p\n", clcf->types_hash.buckets
    printf "&clcf->types_hash = %p\n", &clcf->types_hash
  else
    printf "ERROR: clcf is NULL!\n"
  end
  continue
end

# 运行程序
run

EOF

echo "Running GDB with diagnostic commands..."
gdb -batch -x /tmp/gdb_commands.txt ./objs/nginx

echo ""
echo "Diagnostic complete. Check the output above."
