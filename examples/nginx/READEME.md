# nginx Partitioning Project

## Project Overview

`nginx` is a high-performance web server and reverse proxy. This project demonstrates fine-grained partitioning of the `nginx` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: [Describe here, e.g., network packets, configuration files, or code annotation using `FC_TAINT_WORLD()`]
- Annotation method: [File permission modification or source code annotation, as appropriate]

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **32-bit Executable (for FlowCheck and analysis)**
   - Compile the `nginx` project with minimal modules and 32-bit flags.
   - Ensure dependencies are installed as required by your project.
   - Navigate to the source directory:
     ```bash
     cd examples/nginx/input/source_code/nginx-1.15.5
     ./configure \
       --prefix=/Desktop/IF-driver-partition/partitioned_software/nginx-1.15.5/1_sense-annotation-code/nginx-1.15.5/nginx_install \
       --without-http_rewrite_module \
       --without-http_gzip_module \
       --without-http_charset_module \
       --without-http_ssi_module \
       --without-http_userid_module \
       --without-http_access_module \
       --without-http_autoindex_module \
       --without-http_geo_module \
       --without-http_map_module \
       --without-http_split_clients_module \
       --without-http_referer_module \
       --without-http_proxy_module \
       --without-http_fastcgi_module \
       --without-http_uwsgi_module \
       --without-http_scgi_module \
       --without-http_memcached_module \
       --without-http_limit_conn_module \
       --without-http_limit_req_module \
       --without-http_empty_gif_module \
       --without-http_browser_module \
       --without-http_upstream_ip_hash_module \
       --with-cc-opt="-m32 -O0 -g -Wno-implicit-fallthrough -I/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/include" \
       --with-ld-opt="-m32"
     make -j8
     make install
     ```
   - Copy the generated `nginx` binary from the install directory to `examples/nginx/input/` and rename to `nginx_32`.

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/nginx/input/source_code/nginx-1.15.5` (if available).
   - Find the entry corresponding to `nginx.c`. Copy only the `nginx.c` entry into a new file at `examples/nginx/input/compile_commands.json`. This ensures that only the source code for `nginx.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Clean previous builds:
     ```bash
     make clean
     ```
   - Rebuild with bitcode flags:
     ```bash
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o nginx.bc nginx.o
     ```
   - After completing the above steps, `nginx.bc` will be generated in the `examples/nginx/input/source_code/nginx-1.15.5/src` directory. Move `nginx.bc` to `examples/nginx/input/`.
   - Run `make clean` again.

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/step1_extract_statement_linerange.py --project_root examples/nginx --compile_db examples/nginx/input/compile_commands.json --output_dir examples/nginx/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.
   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/nginx
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/sub_global.py examples/nginx
     python3 scripts/merge_pinout_and_generate_stmt_edges.py examples/nginx
     ```
4. **Build Graph and Solve**
   ```bash
   python3 scripts/build_sqg.py --statements examples/nginx/output/nginx_statements_ranges.xml --quant examples/nginx/output/statement_quantities.xml --edges examples/nginx/output/stmt_edge_counts.txt --bc examples/nginx/input/nginx.bc --threshold_A 10 --budget_scode 0.2 --output examples/nginx/output/partition_policies.txt
   ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/nginx/output/partition_policies.txt --source examples/nginx/input/nginx.c --bc examples/nginx/input/nginx.bc --output examples/nginx/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.
