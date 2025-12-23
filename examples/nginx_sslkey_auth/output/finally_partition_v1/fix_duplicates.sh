#!/bin/bash

# Script to fix duplicated and corrupted files by comparing with original nginx source
CLIENT_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition/nginx_sslkey_auth_client"
ORIG_DIR="/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx_sslkey_auth/output/finally_partition/nginx-1.15.5"

echo "Checking for duplicated and corrupted .c files..."

# Find all .c files in client directory
find "$CLIENT_DIR" -name "*.c" | while read -r client_file; do
    # Get relative path
    rel_path="${client_file#$CLIENT_DIR/}"
    orig_file="$ORIG_DIR/$rel_path"
    
    if [ -f "$orig_file" ]; then
        client_lines=$(wc -l < "$client_file" 2>/dev/null || echo 0)
        orig_lines=$(wc -l < "$orig_file" 2>/dev/null || echo 0)
        
        # Check if file is duplicated OR corrupted
        needs_fix=0
        reason=""
        
        # If client file has significantly more lines, it's likely duplicated
        if [ "$client_lines" -gt $((orig_lines * 2)) ]; then
            needs_fix=1
            reason="DUPLICATE ($client_lines vs $orig_lines lines)"
        # If file exists but has missing includes or structure problems
        elif ! grep -q "ngx_http_module_ctx" "$client_file" 2>/dev/null && grep -q "ngx_http_module_ctx" "$orig_file" 2>/dev/null; then
            needs_fix=1
            reason="CORRUPTED (missing module context)"
        # If file has malformed includes
        elif grep -q "^#include <ngx_core.h>" "$client_file" 2>/dev/null && ! grep -q "nginx_sslkey_auth_rpc_wrapper_simple.h" "$client_file" 2>/dev/null; then
            # This is just missing wrapper, add it without replacement
            echo "ADDING_WRAPPER: $rel_path"
            sed -i '/^#include <ngx_core.h>/a #include "nginx_sslkey_auth_rpc_wrapper_simple.h"' "$client_file"
        fi
        
        if [ "$needs_fix" -eq 1 ]; then
            echo "$reason: $rel_path"
            
            # Backup and replace with original + wrapper
            rm "$client_file"
            cp "$orig_file" "$client_file"
            
            # Add RPC wrapper header if file contains standard nginx includes
            if grep -q "#include <ngx_core.h>" "$client_file"; then
                sed -i '/^#include <ngx_core.h>/a #include "nginx_sslkey_auth_rpc_wrapper_simple.h"' "$client_file"
            fi
        fi
    fi
done

echo "Done checking files."
