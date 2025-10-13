# nginx Partitioning Project

## Project Overview

`nginx` is a high-performance web server and reverse proxy. This project demonstrates fine-grained partitioning of the `nginx` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: Authentication files. nginx specifies the authentication method and the location of the authentication file in its configuration. In this experiment, the authentication file is assumed to be located at `nginx/auth` and named `.htpasswd`. The file contains a user `testuser` with password `123456`.
- Annotation method: Although the sensitive data originates from the authentication file, nginx reads this data using `pread`, which is not modeled as a sensitive API in FlowCheck. Therefore, the annotation method uses FlowCheck's API, `FC_TAINT_WORD`. For details, refer to the patch file `diff.patch` in the `source_code` directory.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the `nginx` project with minimal modules and 32-bit flags.
   - Ensure dependencies are installed as required by your project.
   - Navigate to the source directory:
     ```bash
     cd examples/nginx/input/source_code/nginx-1.15.5
     ./configure --prefix=/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/temp/nginx_install_64 --without-http_rewrite_module --without-http_gzip_module --without-http_charset_module --without-http_ssi_module --without-http_userid_module --without-http_access_module --without-http_autoindex_module    --without-http_geo_module --without-http_map_module --without-http_split_clients_module --without-http_referer_module --without-http_proxy_module --without-http_fastcgi_module --without-http_uwsgi_module --without-http_scgi_module  --without-http_memcached_module  --without-http_limit_conn_module  --without-http_limit_req_module  --without-http_empty_gif_module   --without-http_browser_module  --without-http_upstream_ip_hash_module  --with-cc-opt="-O0 -g -Wno-implicit-fallthrough -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=uninitialized -Wno-error=sign-compare -Wno-error=cast-function-type "
     # Please replace the --prefix value above with your own installation path
     
     bear make -j8
     make install
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
     ./configure --prefix=/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/temp/nginx_install_64 --without-http_rewrite_module --without-http_gzip_module  --without-http_charset_module   --without-http_ssi_module  --without-http_userid_module  --without-http_access_module --without-http_autoindex_module --without-http_geo_module --without-http_map_module  --without-http_split_clients_module  --without-http_referer_module --without-http_proxy_module  --without-http_fastcgi_module  --without-http_uwsgi_module   --without-http_scgi_module  --without-http_memcached_module  --without-http_limit_conn_module  --without-http_limit_req_module   --without-http_empty_gif_module   --without-http_browser_module   --without-http_upstream_ip_hash_module  --with-cc-opt="-O0 -g -Wno-implicit-fallthrough -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=uninitialized -Wno-error=sign-compare -Wno-error=cast-function-type "
      # Please replace the --prefix value above with your own installation path
     ```
   - Build the project (compilation errors may occur, but the bitcode file is the only required artifact):
     ```bash
     make -j8
     ```
   - Manually link all object files to generate the complete bitcode file:
     ```bash
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -flto -fuse-ld=lld -o objs/nginx.bc objs/src/core/nginx.o objs/src/core/ngx_log.o objs/src/core/ngx_palloc.o objs/src/core/ngx_array.o objs/src/core/ngx_list.o objs/src/core/ngx_hash.o objs/src/core/ngx_buf.o objs/src/core/ngx_queue.o objs/src/core/ngx_output_chain.o objs/src/core/ngx_string.o objs/src/core/ngx_parse.o objs/src/core/ngx_parse_time.o objs/src/core/ngx_inet.o objs/src/core/ngx_file.o objs/src/core/ngx_crc32.o objs/src/core/ngx_murmurhash.o objs/src/core/ngx_md5.o objs/src/core/ngx_sha1.o objs/src/core/ngx_rbtree.o objs/src/core/ngx_radix_tree.o objs/src/core/ngx_slab.o objs/src/core/ngx_times.o objs/src/core/ngx_shmtx.o objs/src/core/ngx_connection.o objs/src/core/ngx_cycle.o objs/src/core/ngx_spinlock.o objs/src/core/ngx_rwlock.o objs/src/core/ngx_cpuinfo.o objs/src/core/ngx_conf_file.o objs/src/core/ngx_module.o objs/src/core/ngx_resolver.o objs/src/core/ngx_open_file_cache.o objs/src/core/ngx_crypt.o objs/src/core/ngx_proxy_protocol.o objs/src/core/ngx_syslog.o objs/src/event/ngx_event.o objs/src/event/ngx_event_timer.o objs/src/event/ngx_event_posted.o objs/src/event/ngx_event_accept.o objs/src/event/ngx_event_udp.o objs/src/event/ngx_event_connect.o objs/src/event/ngx_event_pipe.o objs/src/os/unix/ngx_time.o objs/src/os/unix/ngx_errno.o objs/src/os/unix/ngx_alloc.o objs/src/os/unix/ngx_files.o objs/src/os/unix/ngx_socket.o objs/src/os/unix/ngx_recv.o objs/src/os/unix/ngx_readv_chain.o objs/src/os/unix/ngx_udp_recv.o objs/src/os/unix/ngx_send.o objs/src/os/unix/ngx_writev_chain.o objs/src/os/unix/ngx_udp_send.o objs/src/os/unix/ngx_udp_sendmsg_chain.o objs/src/os/unix/ngx_channel.o objs/src/os/unix/ngx_shmem.o objs/src/os/unix/ngx_process.o objs/src/os/unix/ngx_daemon.o objs/src/os/unix/ngx_setaffinity.o objs/src/os/unix/ngx_setproctitle.o objs/src/os/unix/ngx_posix_init.o objs/src/os/unix/ngx_user.o objs/src/os/unix/ngx_dlopen.o objs/src/os/unix/ngx_process_cycle.o objs/src/os/unix/ngx_linux_init.o objs/src/event/modules/ngx_epoll_module.o objs/src/os/unix/ngx_linux_sendfile_chain.o objs/src/http/ngx_http.o objs/src/http/ngx_http_core_module.o objs/src/http/ngx_http_special_response.o objs/src/http/ngx_http_request.o objs/src/http/ngx_http_parse.o objs/src/http/modules/ngx_http_log_module.o objs/src/http/ngx_http_request_body.o objs/src/http/ngx_http_variables.o objs/src/http/ngx_http_script.o objs/src/http/ngx_http_upstream.o objs/src/http/ngx_http_upstream_round_robin.o objs/src/http/ngx_http_file_cache.o objs/src/http/ngx_http_write_filter_module.o objs/src/http/ngx_http_header_filter_module.o objs/src/http/modules/ngx_http_chunked_filter_module.o objs/src/http/modules/ngx_http_range_filter_module.o objs/src/http/modules/ngx_http_headers_filter_module.o objs/src/http/ngx_http_copy_filter_module.o objs/src/http/modules/ngx_http_not_modified_filter_module.o objs/src/http/modules/ngx_http_static_module.o objs/src/http/modules/ngx_http_index_module.o objs/src/http/modules/ngx_http_mirror_module.o objs/src/http/modules/ngx_http_try_files_module.o objs/src/http/modules/ngx_http_auth_basic_module.o objs/src/http/modules/ngx_http_upstream_hash_module.o objs/src/http/modules/ngx_http_upstream_least_conn_module.o objs/src/http/modules/ngx_http_upstream_random_module.o objs/src/http/modules/ngx_http_upstream_keepalive_module.o objs/src/http/modules/ngx_http_upstream_zone_module.o objs/ngx_modules.o

        mv objs/nginx.bc ../../nginx.bc
        make clean
     ```

4. **32-bit Executable (for FlowCheck)**

     ```bash
     cd examples/nginx/input/source_code
     cp -r  nginx-1.15.5 nginx-1.15.5_back
     patch -d nginx-1.15.5 -p1 < diff.patch
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/nginx/input/source_code/nginx-1.15.5
     ```
   - Configure for 32-bit build with minimal modules:
     ```bash
     ./configure --prefix=/Desktop/examples/nginx/output/temp/nginx_install_32 --without-http_rewrite_module --without-http_gzip_module --without-http_charset_module --without-http_ssi_module --without-http_userid_module --without-http_access_module --without-http_autoindex_module --without-http_geo_module --without-http_map_module --without-http_split_clients_module --without-http_referer_module --without-http_proxy_module --without-http_fastcgi_module --without-http_uwsgi_module --without-http_scgi_module --without-http_memcached_module --without-http_limit_conn_module --without-http_limit_req_module --without-http_empty_gif_module --without-http_browser_module --without-http_upstream_ip_hash_module --with-cc-opt="-m32 -O0 -g -Wno-implicit-fallthrough -I/flowcheck/include" --with-ld-opt="-m32"
     ```
   - Build and install:
     ```bash
     make -j8
     make install
     mv objs/nginx ../../nginx_32
     make clean
     ```

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/nginx --compile_db examples/nginx/input/compile_commands.json --output_dir examples/nginx/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.

        To allow the host to access the nginx service running in Docker, map the container port to the host port:
        ```bash
        docker run -it -p 8080:8080 -v .:/Desktop flowcheck-image
        ```

        In the nginx_32 installation directory (the path specified by --prefix during configure), locate the conf/nginx.conf file. Configure authentication, port, and the location of the authentication file in the server block, for example:
        ```nginx
        listen       8080;
        server_name  localhost;
        auth_basic "Experiment";
        auth_basic_user_file "/Desktop/examples/nginx/auth/.htpasswd";
        ```
        The .htpasswd file should be prepared in advance and placed in the /Desktop/examples/nginx/auth/ directory.

        Start the nginx service in the foreground (for Valgrind tracing):
        ```bash
        valgrind --tool=exp-flowcheck --fullpath-after= --folding-level=0 --project-name=nginx-1.15.5 --trace-secret-graph=yes --graph-file=temp.g ./examples/nginx/input/nginx_32 -g "daemon off;" 2>examples/nginx/output/temp/nginxoutput1.fc
        ```

        On the host, use curl to test authenticated download:
        ```bash
        curl --user testuser:123456 http://localhost:8080
        ```
        After confirming the download is successful, stop the nginx service in Docker by pressing Ctrl+C. This process may take several minutes, please be patient.

   - Merge traces and map quantitative info to statements:(running on the host)
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/nginx
     ```
3. **Collect Edge Information**
   - On the host machine, run the 64-bit executable with Pin to generate `.pinout` trace files. This requires running the server under Pin in one terminal and using `curl` to interact with it from another.

   - In the nginx_32 installation directory (the path specified by --prefix during configure), locate the conf/nginx.conf file. Configure authentication, port, and the location of the authentication file in the server block, for example:
        ```nginx
        listen       8080;
        server_name  localhost;
        auth_basic "Experiment";
        auth_basic_user_file "/abspath/to/FIPA/examples/nginx/auth/.htpasswd";
        ```
        The .htpasswd file should be prepared in advance and placed in the /abspath/to/FIPA/examples/nginx/auth/ directory.

     - In your first terminal, start the `nginx` server under Pin. We'll use port 8081 to avoid conflicts.
       ```bash
       src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/nginx/output/temp/nginx_protected.pinout -- ./examples/nginx/input/nginx_64 -g "daemon off;"
       ```
     - In a second terminal, use `curl` to request a file from the protected directory:
       ```bash
       curl --user testuser:123456 http://localhost:8080
       ```
     - After the `curl` command completes, return to the first terminal and press `Ctrl+C` to stop the server. This will generate `nginx_protected.pinout`.

   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/nginx
     ```
4. **Build Graph and Solve for Partitioning**

   This step uses an automated script to construct the graph and find mutiple partitioning solution. You can run the solver with different communication models and leakage budgets by specifying the `--comm-type` and `min-quan` parameters. The script will generate result files (e.g., `nginx_z3_result_u_0bit.txt`) in the `examples/nginx/output/` directory.

   -   **To solve using the unidirectional model (`u`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py nginx min-quan=0 max-code-sz=0.1 --comm-type=u
       ```

   -   **To solve using the unidirectional model (`u`) with a 64-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py nginx min-quan=64 max-code-sz=0.1 --comm-type=u
       ```

   -   **To solve using the bidirectional model (`b`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py nginx min-quan=0 max-code-sz=0.1 --comm-type=b
       ```

5. **Prepare Data for Refactoring**

   The  step include several key actions:

    - Selects the optimal solution from multiple candidates generated by the solver.
    - Analyzes read/write dependencies for global variables across partitions.
    - Identifies functions that may be shared or need to be duplicated.

  - **Command:**

    Run the script with the parameters corresponding to the desired solution from Step 4.
    ```bash
    # For a unidirectional model with a 0-bit leakage budget
    python3 scripts/prepare_refactor_data.py nginx --comm-type=u --quan=0

    # For a unidirectional model with a 64-bit leakage budget
    python3 scripts/prepare_refactor_data.py nginx --comm-type=u --quan=64

    # For a bidirectional model with a 0-bit leakage budget
    python3 scripts/prepare_refactor_data.py nginx --comm-type=b --quan=0
    ```

  -  **Optional: Analyze Partitioning Statistics**

        After preparing the refactoring data, you can run an additional script to view detailed statistics about the chosen partition. This is not a required step for the main workflow but is useful for analysis. It provides information such as the number of functions in each partition and the percentage of sensitive code.

      Command Format:

      ```bash
          # Analyze the result for a unidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py nginx --comm-type=u --quan=0

          # Analyze the result for a unidirectional model with a 64-bit leakage budget
          python3 scripts/analyze_partition_results.py nginx --comm-type=u --quan=64

          # Analyze the result for a bidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py nginx --comm-type=b --quan=0
      ```

6. **Code Refactoring**
   
   This step generates the final partitioned code with RPC communication interfaces. The example below uses unidirectional communication with 0-bit leakage. For other configurations, modify the `--comm-type` and `--quan` parameters accordingly.
   
   ```bash
   python3 scripts/refactor_function_add_rpc.py nginx --comm-type=u --quan=0
   ```
   
   **Parameter Options:**
   - `--comm-type`: Communication model (`u` for unidirectional, `b` for bidirectional)
   - `--quan`: Leakage budget in bits (e.g., `0`, `64`)

## Running the Partitioned Program

The result of automatic partitioning may require manual adjustments. We provide a runnable version in `output/finally_partition`.

### 1. Compilation

First, decompress the two archives in `examples/nginx/output/finally_partition/`.

Then, compile the client and server components.

**Compile `nginx_server`:**
```bash
cd examples/nginx/output/finally_partition/nginx_server/nginx-1.15.5
./configure --prefix=`pwd`/nginx_install --without-http_rewrite_module --without-http_gzip_module --without-http_charset_module --without-http_ssi_module --without-http_userid_module --without-http_access_module --without-http_autoindex_module --without-http_geo_module --without-http_map_module --without-http_split_clients_module --without-http_referer_module --without-http_proxy_module --without-http_fastcgi_module --without-http_uwsgi_module --without-http_scgi_module --without-http_memcached_module --without-http_limit_conn_module --without-http_limit_req_module --without-http_empty_gif_module --without-http_browser_module --without-http_upstream_ip_hash_module --with-cc-opt="-O0 -g"
make -j8
make install
# The nginx_server executable will be in the objs/ directory and nginx_install/sbin/ directory.
```

**Compile `nginx_client`:**
```bash
cd examples/nginx/output/finally_partition/nginx_client/nginx-1.15.5
./configure --prefix=`pwd`/nginx_install --without-http_rewrite_module --without-http_gzip_module --without-http_charset_module --without-http_ssi_module --without-http_userid_module --without-http_access_module --without-http_autoindex_module --without-http_geo_module --without-http_map_module --without-http_split_clients_module --without-http_referer_module --without-http_proxy_module --without-http_fastcgi_module --without-http_uwsgi_module --without-http_scgi_module --without-http_memcached_module --without-http_limit_conn_module --without-http_limit_req_module --without-http_empty_gif_module --without-http_browser_module --without-http_upstream_ip_hash_module --with-cc-opt="-O0 -g"
make -j8
make install
# The nginx_client executable will be in the objs/ directory and nginx_install/sbin/ directory.
```

### 2. Execution

- **Terminal 1 (Server):** Start the server program. It will wait for a connection from the client.
  ```bash
  cd examples/nginx/output/finally_partition/nginx_server/nginx-1.15.5
  ./objs/nginx_server
  ```
- **Terminal 2 (Client):** Start the client program. It will handle incoming HTTP requests and communicate with the server for sensitive operations.
  ```bash
  cd examples/nginx/output/finally_partition/nginx_client/nginx-1.15.5
  ./objs/nginx_client -g "daemon off;"
  ```

### 3. Testing

- On the host, use `curl` to test the authenticated access. The client part of nginx will handle the request and interact with the server part for authentication.
  ```bash
  curl --user testuser:123456 http://localhost:8080
  ```
- The server terminal will print "Authentication successful" upon successful verification.

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.
