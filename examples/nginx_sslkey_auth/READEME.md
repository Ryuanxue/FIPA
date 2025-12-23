## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the `nginx` project with SSL key support.
   - Ensure dependencies are installed as required by your project: `apt install libssl-dev`
   - Navigate to the source directory:
     ```bash
     cd examples/nginx_sslkey_auth/input/source_code/nginx-1.15.5
     ./configure  --with-http_ssl_module
     
     bear make -j8
     mv objs/nginx ../../nginx_64
     mv compile_commands.json ../../
     make clean
     ```

2. **Compilation Database**
   - The shell commands above have already moved the compilation database (`compile_commands.json`) to the `nginx/input` directory.

3. **LLVM Bitcode File (.bc)**
   - Set environment variables for bitcode generation:
     ```bash
     export CC=clang
     export LDFLAGS="-flto -fuse-ld=lld"
     export CFLAGS="-flto -g -O0 -fno-discard-value-names -fembed-bitcode"
     ```
   - Configure the project:
     ```bash
     ./configure --with-http_ssl_module
      # Please replace the --prefix value above with your own installation path
     ```
   - Build the project (compilation errors may occur, but the bitcode file is the only required artifact):
     ```bash
     make -j8
     ```
   - Manually link all object files to generate the complete bitcode file:
     ```bash
    clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -flto -fuse-ld=lld  -o objs/nginx.bc \
objs/src/core/nginx.o \
objs/src/core/ngx_log.o \
objs/src/core/ngx_palloc.o \
objs/src/core/ngx_array.o \
objs/src/core/ngx_list.o \
objs/src/core/ngx_hash.o \
objs/src/core/ngx_buf.o \
objs/src/core/ngx_queue.o \
objs/src/core/ngx_output_chain.o \
objs/src/core/ngx_string.o \
objs/src/core/ngx_parse.o \
objs/src/core/ngx_parse_time.o \
objs/src/core/ngx_inet.o \
objs/src/core/ngx_file.o \
objs/src/core/ngx_crc32.o \
objs/src/core/ngx_murmurhash.o \
objs/src/core/ngx_md5.o \
objs/src/core/ngx_sha1.o \
objs/src/core/ngx_rbtree.o \
objs/src/core/ngx_radix_tree.o \
objs/src/core/ngx_slab.o \
objs/src/core/ngx_times.o \
objs/src/core/ngx_shmtx.o \
objs/src/core/ngx_connection.o \
objs/src/core/ngx_cycle.o \
objs/src/core/ngx_spinlock.o \
objs/src/core/ngx_rwlock.o \
objs/src/core/ngx_cpuinfo.o \
objs/src/core/ngx_conf_file.o \
objs/src/core/ngx_module.o \
objs/src/core/ngx_resolver.o \
objs/src/core/ngx_open_file_cache.o \
objs/src/core/ngx_crypt.o \
objs/src/core/ngx_proxy_protocol.o \
objs/src/core/ngx_syslog.o \
objs/src/event/ngx_event.o \
objs/src/event/ngx_event_timer.o \
objs/src/event/ngx_event_posted.o \
objs/src/event/ngx_event_accept.o \
objs/src/event/ngx_event_udp.o \
objs/src/event/ngx_event_connect.o \
objs/src/event/ngx_event_pipe.o \
objs/src/os/unix/ngx_time.o \
objs/src/os/unix/ngx_errno.o \
objs/src/os/unix/ngx_alloc.o \
objs/src/os/unix/ngx_files.o \
objs/src/os/unix/ngx_socket.o \
objs/src/os/unix/ngx_recv.o \
objs/src/os/unix/ngx_readv_chain.o \
objs/src/os/unix/ngx_udp_recv.o \
objs/src/os/unix/ngx_send.o \
objs/src/os/unix/ngx_writev_chain.o \
objs/src/os/unix/ngx_udp_send.o \
objs/src/os/unix/ngx_udp_sendmsg_chain.o \
objs/src/os/unix/ngx_channel.o \
objs/src/os/unix/ngx_shmem.o \
objs/src/os/unix/ngx_process.o \
objs/src/os/unix/ngx_daemon.o \
objs/src/os/unix/ngx_setaffinity.o \
objs/src/os/unix/ngx_setproctitle.o \
objs/src/os/unix/ngx_posix_init.o \
objs/src/os/unix/ngx_user.o \
objs/src/os/unix/ngx_dlopen.o \
objs/src/os/unix/ngx_process_cycle.o \
objs/src/os/unix/ngx_linux_init.o \
objs/src/event/modules/ngx_epoll_module.o \
objs/src/os/unix/ngx_linux_sendfile_chain.o \
objs/src/event/ngx_event_openssl.o \
objs/src/event/ngx_event_openssl_stapling.o \
objs/src/core/ngx_regex.o \
objs/src/http/ngx_http.o \
objs/src/http/ngx_http_core_module.o \
objs/src/http/ngx_http_special_response.o \
objs/src/http/ngx_http_request.o \
objs/src/http/ngx_http_parse.o \
objs/src/http/modules/ngx_http_log_module.o \
objs/src/http/ngx_http_request_body.o \
objs/src/http/ngx_http_variables.o \
objs/src/http/ngx_http_script.o \
objs/src/http/ngx_http_upstream.o \
objs/src/http/ngx_http_upstream_round_robin.o \
objs/src/http/ngx_http_file_cache.o \
objs/src/http/ngx_http_write_filter_module.o \
objs/src/http/ngx_http_header_filter_module.o \
objs/src/http/modules/ngx_http_chunked_filter_module.o \
objs/src/http/modules/ngx_http_range_filter_module.o \
objs/src/http/modules/ngx_http_gzip_filter_module.o \
objs/src/http/ngx_http_postpone_filter_module.o \
objs/src/http/modules/ngx_http_ssi_filter_module.o \
objs/src/http/modules/ngx_http_charset_filter_module.o \
objs/src/http/modules/ngx_http_userid_filter_module.o \
objs/src/http/modules/ngx_http_headers_filter_module.o \
objs/src/http/ngx_http_copy_filter_module.o \
objs/src/http/modules/ngx_http_not_modified_filter_module.o \
objs/src/http/modules/ngx_http_static_module.o \
objs/src/http/modules/ngx_http_autoindex_module.o \
objs/src/http/modules/ngx_http_index_module.o \
objs/src/http/modules/ngx_http_mirror_module.o \
objs/src/http/modules/ngx_http_try_files_module.o \
objs/src/http/modules/ngx_http_auth_basic_module.o \
objs/src/http/modules/ngx_http_access_module.o \
objs/src/http/modules/ngx_http_limit_conn_module.o \
objs/src/http/modules/ngx_http_limit_req_module.o \
objs/src/http/modules/ngx_http_geo_module.o \
objs/src/http/modules/ngx_http_map_module.o \
objs/src/http/modules/ngx_http_split_clients_module.o \
objs/src/http/modules/ngx_http_referer_module.o \
objs/src/http/modules/ngx_http_rewrite_module.o \
objs/src/http/modules/ngx_http_ssl_module.o \
objs/src/http/modules/ngx_http_proxy_module.o \
objs/src/http/modules/ngx_http_fastcgi_module.o \
objs/src/http/modules/ngx_http_uwsgi_module.o \
objs/src/http/modules/ngx_http_scgi_module.o \
objs/src/http/modules/ngx_http_memcached_module.o \
objs/src/http/modules/ngx_http_empty_gif_module.o \
objs/src/http/modules/ngx_http_browser_module.o \
objs/src/http/modules/ngx_http_upstream_hash_module.o \
objs/src/http/modules/ngx_http_upstream_ip_hash_module.o \
objs/src/http/modules/ngx_http_upstream_least_conn_module.o \
objs/src/http/modules/ngx_http_upstream_random_module.o \
objs/src/http/modules/ngx_http_upstream_keepalive_module.o \
objs/src/http/modules/ngx_http_upstream_zone_module.o \
objs/ngx_modules.o

        mv objs/nginx.bc ../../nginx.bc
        make clean
     ```

4. **32-bit Executable (for FlowCheck)**
    
    First, compile nginx with auth file annotations as the taint source into a 32-bit executable:
     ```bash
     cd examples/nginx_sslkey_auth/input/source_code
     patch -d nginx-1.15.5 -p1 < diff_auth.patch
     cd FIPA
     docker run -it -p 8080:8080 -p 8081:8081 -p 8443:8443 -v .:/Desktop flowcheck-image
     apt update
     apt install libssl-dev:i386
     cd examples/nginx_sslkey_auth/input/source_code/nginx-1.15.5
     ```
   - Configure for 32-bit build with minimal modules:
     ```bash
     ./configure --with-http_ssl_module  --with-cc-opt="-m32 -O0 -g -Wno-implicit-fallthrough -I/Desktop/src/Flowcheckdocker/flowcheck-1.20/include" --with-ld-opt="-m32"
     ```
   - Build and install:
     ```bash
     make -j8
     mv objs/nginx ../../nginx_auth_32
     make clean
     ```

    Compile nginx with SSL key annotations as the taint source into a 32-bit executable:
    ```bash
        cd examples/nginx/input/source_code
        # Remove nginx-1.15.5
        # Re-extract nginx-1.15.5.tar.xz to the current directory
        patch -d nginx-1.15.5 -p1 < diff_sslkey.patch
        cd nginx-1.15.5
        ./configure --with-http_ssl_module  --with-cc-opt="-m32 -O0 -g -Wno-implicit-fallthrough -I/Desktop/src/Flowcheckdocker/flowcheck-1.20/include" --with-ld-opt="-m32"
        make -j8
        mv objs/nginx ../../nginx_sslkey_32
        make clean
    ```

## Partitioning Workflow Steps

### Workflow for Auth File Taint Source

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/nginx_sslkey_auth --compile_db examples/nginx_sslkey_auth/input/compile_commands.json --output_dir examples/nginx_sslkey_auth/output/
   ```

2. **Quantitative Information Flow Tracking for Auth File**
   
   Run FlowCheck in Docker to perform quantitative information flow tracking on `nginx_auth_32` executable.

   - To allow the host to access the nginx service running in Docker, map the container port to the host port:
     ```bash
     docker run -it -p 8080:8080 -p 8081:8081 -p 8443:8443 -v .:/Desktop flowcheck-image
     ```

   - Start the nginx service in the foreground (for Valgrind tracing):
     ```bash
     valgrind --tool=exp-flowcheck --fullpath-after= --folding-level=0 --project-name=nginx-1.15.5 --trace-secret-graph=yes --graph-file=temp.g ./examples/nginx_sslkey_auth/input/nginx_auth_32 -p $(pwd)/examples/nginx_sslkey_auth -c conf/nginx-ssl-auth.conf -g "daemon off;" 2>examples/nginx_sslkey_auth/output/temp/nginxoutput_auth.fc
     ```

   - On the host, use curl to test authenticated download:
     ```bash
     curl -k -u testuser:testpass123 -I https://localhost:8443/api/
     ```
     After confirming the download is successful, stop the nginx service in Docker by pressing Ctrl+C. This process may take several minutes, please be patient.

   - Merge traces and map quantitative info to statements (running on the host):
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/nginx_sslkey_auth
     # This generates nginx_sslkey_auth_quanfile.txt in examples/nginx_sslkey_auth/output/
     # Rename it for the auth file taint source
     ```

3. **Collect Edge Information**
   
   On the host machine, run the 64-bit executable with Pin to generate `.pinout` trace files. This requires running the server under Pin in one terminal and using `curl` to interact with it from another.

   - In your first terminal, start the `nginx` server under Pin:
     ```bash
     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/nginx_sslkey_auth/output/temp/nginx_sslkey.pinout -- ./examples/nginx_sslkey_auth/input/nginx_64 -p $(pwd)/examples/nginx_sslkey_auth -c conf/nginx-ssl-auth.conf -g "daemon off;" 
     ```
   
   - In a second terminal, use `curl` to request a file from the protected directory:
     ```bash
     curl -k -u testuser:testpass123 -I https://localhost:8443/api/
     ```
   
   - After the `curl` command completes, return to the first terminal and press `Ctrl+C` to stop the server. This will generate `nginx_sslkey.pinout`.

   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/nginx_sslkey_auth
     ```

4. **Build Graph and Solve for Partitioning (Auth File Taint Source)**

   Solve using the unidirectional model (`u`) with a 0-bit leakage budget:
   ```bash
   python3 scripts/based_qg_bi_praming.py nginx_sslkey_auth min-quan=0 max-code-sz=0.1 --comm-type=u
   # This generates nginx_sslkey_auth_z3_result_u_0bit.txt, rename it for auth file taint source
   ```

5. **Prepare Data for Refactoring (Auth File Taint Source)**

   This step includes several key actions:
   - Selects the optimal solution from multiple candidates generated by the solver.
   - Analyzes read/write dependencies for global variables across partitions.
   - Identifies functions that may be shared or need to be duplicated.

   Run the script with the parameters corresponding to the desired solution from Step 4:
   ```bash
   # For a unidirectional model with a 0-bit leakage budget
   python3 scripts/prepare_refactor_data.py nginx_sslkey_auth --comm-type=u --quan=0
   ```

   **Optional: Analyze Partitioning Statistics**

   After preparing the refactoring data, you can run an additional script to view detailed statistics about the chosen partition. This is not a required step for the main workflow but is useful for analysis. It provides information such as the number of functions in each partition and the percentage of sensitive code.

   ```bash
   # Analyze the result for a unidirectional model with a 0-bit leakage budget
   python3 scripts/analyze_partition_results.py nginx_sslkey_auth --comm-type=u --quan=0
   ```

6. **Code Refactoring**
   
   This step generates the initial partitioned code with RPC communication interfaces. The example below uses unidirectional communication with 0-bit leakage. For other configurations, modify the `--comm-type` and `--quan` parameters accordingly.
   
   ```bash
   python3 scripts/refactor_function_add_rpc.py nginx_sslkey_auth --comm-type=u --quan=0
   # This generates examples/nginx_sslkey_auth/output/init_partition, rename it to init_partition_auth
   mv examples/nginx_sslkey_auth/output/init_partition examples/nginx_sslkey_auth/output/init_partition_auth
   ```
   
   **Parameter Options:**
   - `--comm-type`: Communication model (`u` for unidirectional, `b` for bidirectional)
   - `--quan`: Leakage budget in bits (e.g., `0`, `64`)

---

### Workflow for SSL Key Path Taint Source

1. **Quantitative Information Flow Tracking for SSL Key**
   
   Run FlowCheck in Docker to perform quantitative information flow tracking on `nginx_sslkey_32` executable.

   - To allow the host to access the nginx service running in Docker, map the container port to the host port:
     ```bash
     docker run -it -p 8080:8080 -p 8081:8081 -p 8443:8443 -v .:/Desktop flowcheck-image
     ```

   - Start the nginx service in the foreground (for Valgrind tracing):
     ```bash
    valgrind --tool=exp-flowcheck --fullpath-after= --folding-level=0 --project-name=nginx-1.15.5 --trace-secret-graph=yes --graph-file=temp.g ./examples/nginx_sslkey_auth/input/nginx_sslkey_32 -p $(pwd)/examples/nginx_sslkey_auth -c conf/nginx-ssl-auth.conf -g "daemon off;" 2>examples/nginx_sslkey_auth/output/temp/nginxoutput_sslkey.fc
     ```

   - On the host, use curl to test authenticated download:
     ```bash
     curl -k -u testuser:testpass123 -I https://localhost:8443/api/
     ```
     After confirming the download is successful, stop the nginx service in Docker by pressing Ctrl+C. This process may take several minutes, please be patient.

   - Merge traces and map quantitative info to statements (running on the host):
     ```bash
     # Remove the auth file .fc trace to avoid interference
     mv examples/nginx_sslkey_auth/output/temp/nginxoutput_auth.fc examples/nginx_sslkey_auth/output/temp/nginxoutput_auth.fc.backup
     python3 scripts/merge_fc_and_map_statements.py examples/nginx_sslkey_auth
     # This generates nginx_sslkey_auth_quanfile.txt in examples/nginx_sslkey_auth/output/
     # Rename it for the SSL key taint source
     ```

2. **Build Graph and Solve for Partitioning (SSL Key Path Taint Source)**

   Solve using the unidirectional model (`u`) with a 0-bit leakage budget:
   ```bash
   python3 scripts/based_qg_bi_praming.py nginx_sslkey_auth min-quan=0 max-code-sz=0.1 --comm-type=u
   # This generates nginx_sslkey_auth_z3_result_u_0bit.txt, rename it for SSL key taint source
   ```

3. **Prepare Data for Refactoring (SSL Key Path Taint Source)**

   This step includes several key actions:
   - Selects the optimal solution from multiple candidates generated by the solver.
   - Analyzes read/write dependencies for global variables across partitions.
   - Identifies functions that may be shared or need to be duplicated.

   Run the script with the parameters corresponding to the desired solution from Step 2:
   ```bash
   # For a unidirectional model with a 0-bit leakage budget
   python3 scripts/prepare_refactor_data.py nginx_sslkey_auth --comm-type=u --quan=0
   ```

   **Optional: Analyze Partitioning Statistics**

   After preparing the refactoring data, you can run an additional script to view detailed statistics about the chosen partition.

   ```bash
   # Analyze the result for a unidirectional model with a 0-bit leakage budget
   python3 scripts/analyze_partition_results.py nginx_sslkey_auth --comm-type=u --quan=0
   ```

4. **Code Refactoring**
   
   This step generates the initial partitioned code with RPC communication interfaces. The example below uses unidirectional communication with 0-bit leakage. For other configurations, modify the `--comm-type` and `--quan` parameters accordingly.
   
   ```bash
   python3 scripts/refactor_function_add_rpc.py nginx_sslkey_auth --comm-type=u --quan=0
   # This generates examples/nginx_sslkey_auth/output/init_partition, rename it to init_partition_sslkey
   mv examples/nginx_sslkey_auth/output/init_partition examples/nginx_sslkey_auth/output/init_partition_sslkey
   ```
   
   **Parameter Options:**
   - `--comm-type`: Communication model (`u` for unidirectional, `b` for bidirectional)
   - `--quan`: Leakage budget in bits (e.g., `0`, `64`)




  ## Running the Partitioned Program

  ### Multiple-Partition (Three Partitions)

    First, decompress the two archives in `examples/nginx_sslkey_auth/output/multi-partition/`.

    This configuration creates three partitions: one public partition and two sensitive partitions. The partitioning is guided by different taint sources: the SSL key path taint source generates the SSL partition, and the auth file taint source generates the auth partition. The SSL partition is compiled as a shared library, while the auth partition is compiled as an RPC server. The partitioning results are located in `examples/nginx_sslkey_auth/output/muti-partition/`.

    #### 1. Compilation

    **Compile the SSL Partition as a Shared Library:**
     ```
     # Assuming you are in the FIPA directory
     cd examples/nginx_sslkey_auth/output/multi-partition/nginx_public_partition
         ./configure --with-http_ssl_module --with-cc-opt="-O0 -g -Wno-implicit-fallthrough -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-function -Wno-error=int-conversion -Wno-error=format -Wno-error=cast-function-type -Wno-error=incompatible-pointer-types -Wno-error=missing-field-initializers"

     cd libngx_ssl_cert
     make
     ```

    **Compile the Public Partition:**
    ```bash
    # Assuming you are in the FIPA directory
    cd examples/nginx_sslkey_auth/output/muti-partition/nginx_public_partition
    ./configure --with-http_ssl_module --with-cc-opt="-O0 -g -Wno-implicit-fallthrough -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-function -Wno-error=int-conversion -Wno-error=format -Wno-error=cast-function-type -Wno-error=incompatible-pointer-types -Wno-error=missing-field-initializers"
    make -j8
    ```

    **Compile the Auth Partition:**
    ```bash
    # Assuming you are in the FIPA directory
    cd examples/nginx_sslkey_auth/output/muti-partition/nginx_auth_partition
    ./configure --with-http_ssl_module --with-cc-opt="-O0 -g -Wno-implicit-fallthrough -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-function -Wno-error=int-conversion -Wno-error=format -Wno-error=cast-function-type -Wno-error=incompatible-pointer-types -Wno-error=missing-field-initializers"
    make -j8
    ```


    #### 2. Execution

    **Terminal 1 (Auth Partition):** Start the core service program. It will wait for connections from the public partition
    ```bash
    # Assuming you are in the FIPA directory
    cd examples/nginx_sslkey_auth/output/muti-partition/nginx_auth_partition
    ./objs/nginx
    ```

    **Terminal 2 (Public Partition and SSL Partition):** Start the public partition (this includes both the public partition and SSL partition components)
    ```bash
    # Assuming you are in the FIPA directory
    cd examples/nginx_sslkey_auth/output/muti-partition
    ./nginx_public_partition/objs/nginx -p $(pwd) -c conf/nginx-ssl-auth.conf -g "daemon off;"
    ```

    

    #### 3. Testing

    Use `curl` to test the multi-partition nginx with authentication:
    ```bash
    curl -k -u testuser:testpass123 -I https://localhost:8443/api/
    ```



