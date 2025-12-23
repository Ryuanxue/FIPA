#!/bin/bash

# Script to batch fix all corrupted HTTP module files
CLIENT_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition/nginx_sslkey_auth_client"
ORIG_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition/nginx-1.15.5"

echo "Batch fixing HTTP module files..."

# List of commonly corrupted files based on compilation errors
COMMON_CORRUPTED=(
    "src/http/modules/ngx_http_gzip_filter_module.c"
    "src/http/modules/ngx_http_ssi_filter_module.c"
    "src/http/modules/ngx_http_postpone_filter_module.c"
    "src/http/modules/ngx_http_gzip_static_module.c"
    "src/http/modules/ngx_http_index_module.c"
    "src/http/modules/ngx_http_auth_basic_module.c"
    "src/http/modules/ngx_http_access_module.c"
    "src/http/modules/ngx_http_referer_module.c"
    "src/http/modules/ngx_http_rewrite_module.c"
    "src/http/modules/ngx_http_geo_module.c"
    "src/http/modules/ngx_http_map_module.c"
    "src/http/modules/ngx_http_split_clients_module.c"
    "src/http/modules/ngx_http_browser_module.c"
    "src/http/modules/ngx_http_upstream_ip_hash_module.c"
    "src/http/modules/ngx_http_upstream_least_conn_module.c"
    "src/http/modules/ngx_http_upstream_random_module.c"
    "src/http/modules/ngx_http_upstream_keepalive_module.c"
    "src/http/modules/ngx_http_upstream_zone_module.c"
    "src/http/ngx_http_copy_filter_module.c"
)

for rel_path in "${COMMON_CORRUPTED[@]}"; do
    client_file="$CLIENT_DIR/$rel_path"
    orig_file="$ORIG_DIR/$rel_path"
    
    if [ -f "$orig_file" ] && [ -f "$client_file" ]; then
        echo "Fixing: $rel_path"
        rm "$client_file"
        cp "$orig_file" "$client_file"
        
        # Add RPC wrapper
        if grep -q "#include <ngx_core.h>" "$client_file"; then
            sed -i '/^#include <ngx_core.h>/a #include "nginx_sslkey_auth_rpc_wrapper_simple.h"' "$client_file"
        fi
    fi
done

echo "Done batch fixing files."
