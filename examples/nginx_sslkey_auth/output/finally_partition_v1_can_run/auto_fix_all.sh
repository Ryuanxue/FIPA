#!/bin/bash

# Script to automatically fix all nginx source files by replacing them with originals + RPC wrapper
CLIENT_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition/nginx_sslkey_auth_client"
ORIG_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition/nginx-1.15.5"

echo "Auto-fixing ALL nginx source files..."

# Find all .c files in original nginx and replace client versions
find "$ORIG_DIR" -name "*.c" | while read -r orig_file; do
    # Get relative path
    rel_path="${orig_file#$ORIG_DIR/}"
    client_file="$CLIENT_DIR/$rel_path"
    
    # Skip if directory doesn't exist
    if [ ! -d "$(dirname "$client_file")" ]; then
        continue
    fi
    
    # Only replace if client file exists and isn't our special wrapper file
    if [ -f "$client_file" ] && [[ ! "$rel_path" == *"nginx_sslkey_auth_rpc_wrapper"* ]]; then
        echo "Fixing: $rel_path"
        rm "$client_file"
        cp "$orig_file" "$client_file"
        
        # Add RPC wrapper header if file contains ngx_core.h
        if grep -q "#include <ngx_core.h>" "$client_file"; then
            sed -i '/^#include <ngx_core.h>/a #include "nginx_sslkey_auth_rpc_wrapper_simple.h"' "$client_file"
        fi
    fi
done

echo "Done auto-fixing all files."
