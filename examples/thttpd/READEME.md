# thttpd Partitioning Project

## Project Overview

`thttpd` is a lightweight web server. This project demonstrates fine-grained partitioning of the `thttpd` program using the FIPA toolchain.

## Annotation Strategy

thttpd is a lightweight server for serving static files. When starting thttpd, you specify a root directory that clients can access. Resources are placed under this root, and subdirectories can have different protection levels. Any directory containing a `.htpasswd` file requires authentication for access. To protect multiple directories, place `.htpasswd` files in each directory you wish to secure.

In our example, the client-accessible root directory is `thttpd/html`, and the subdirectory `thttpd/html/protected` is protected. The `.htpasswd` file in `thttpd/html/protected` contains a user `testuser` with password `123456`.

- Sensitive sources: The `.htpasswd` file in `thttpd/html/protected`.
- Annotation method: Use `chmod` to modify the permissions of `.htpasswd` files.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the `thttpd` project.
   - Ensure dependencies are installed as required by your project.
   - Navigate to the source directory:
     ```bash
     cd examples/thttpd/input/source_code/thttpd-2.27
     #將此目錄下的壓縮包thttp-2.2.7.tar.xz解壓到此處
     ./configure
     bear make -j8
     mv thttpd ../../thttpd_64
     mv compile_commands.json ../../
     make clean
     ```

2. **Compilation Database**
   - The shell commands above have already moved the compilation database (`compile_commands.json`) to the `thttpd/input` directory.

3. **LLVM Bitcode File (.bc)**
   - Edit the `Makefile`:
     - Change `CC` from `gcc` to `clang`.
     - Add `-flto -g -O0 -fno-discard-value-names -fembed-bitcode` to `CCOPT`.
   - Compile the project:
     ```bash
     make
     ```
   - Locate the final link command for `thttpd` and modify it as follows to generate the bitcode file:
     ```bash
     clang -Wl,--plugin-opt=emit-llvm -O0 -g -flto -g -O0 -fno-discard-value-names -fembed-bitcode -DHAVE__PROGNAME=1 -DHAVE_FCNTL_H=1 -DHAVE_GRP_H=1 -DHAVE_MEMORY_H=1 -DHAVE_PATHS_H=1 -DHAVE_POLL_H=1 -DHAVE_SYS_POLL_H=1 -DTIME_WITH_SYS_TIME=1 -DHAVE_DIRENT_H=1 -DHAVE_LIBCRYPT=1 -DHAVE_STRERROR=1 -DHAVE_WAITPID=1 -DHAVE_VSNPRINTF=1 -DHAVE_DAEMON=1 -DHAVE_SETSID=1 -DHAVE_GETADDRINFO=1 -DHAVE_GETNAMEINFO=1 -DHAVE_GAI_STRERROR=1 -DHAVE_SIGSET=1 -DHAVE_ATOLL=1 -DHAVE_UNISTD_H=1 -DHAVE_GETPAGESIZE=1 -DHAVE_MMAP=1 -DHAVE_SELECT=1 -DHAVE_POLL=1 -DHAVE_TM_GMTOFF=1 -DHAVE_INT64T=1 -DHAVE_SOCKLENT=1  -I.  -o thttpd.bc thttpd.o libhttpd.o fdwatch.o mmc.o timers.o match.o tdate_parse.o  -lcrypt

     mv thttpd.bc ../../thttpd.bc
     make clean
     ```

4. **32-bit Executable (for FlowCheck)**
   - Start Docker:
     ```bash
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/thttpd/input/source_code/thttpd-2.27
     ./configure
     # Edit the Makefile: set CC to gcc and CCOPT to "-O0 -g -m32" for 32-bit compilation.
     make -j8
     mv thttpd ../../thttpd_32
     make clean
     exit
     ```

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/thttpd --compile_db examples/thttpd/input/compile_commands.json --output_dir examples/thttpd/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.

     To allow the host to access the thttpd service running in Docker, map the container port to the host port:
     ```bash
     docker run -it -p 8080:8080 -v .:/Desktop flowcheck-image
     ```
     In Docker, modify the permissions of the `.htpasswd` file:
     ```bash
     chmod o-r examples/thttpd/html/protected/.htpasswd
     ```
     Add a user in the container:
     ```bash
     groupadd -g 1000 testthttpd
     useradd -u 1000 -g 1000 testthttpd
     ```
     Start thttpd with the `-D` option to keep it in the foreground for Valgrind tracing:
     ```bash
     valgrind --trace-children=yes --tool=exp-flowcheck --sigill-diagnostics=yes --fullpath-after= --folding-level=0 --project-name=thttpd --private-files-are-secret=yes --trace-secret-graph=yes --graph-file=temp.g ./examples/thttpd/input/thttpd_32 -D -u testthttpd -p 8080 -d examples/thttpd/html/ 2>examples/thttpd/output/temp/thttpdoutput1.fc
     ```
     On the host, use curl to download from the protected directory:
     ```bash
     curl -u testuser:123456 http://localhost:8080/protected/
     ```
     After confirming the download is successful, stop thttpd in Docker by pressing Ctrl+C.

   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/thttpd
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/sub_global.py examples/thttpd
     python3 scripts/merge_pinout_and_generate_stmt_edges.py examples/thttpd
     ```
4. **Build Graph and Solve**
   ```bash
   python3 scripts/build_sqg.py --statements examples/thttpd/output/thttpd_statements_ranges.xml --quant examples/thttpd/output/statement_quantities.xml --edges examples/thttpd/output/stmt_edge_counts.txt --bc examples/thttpd/input/thttpd.bc --threshold_A 10 --budget_scode 0.2 --output examples/thttpd/output/partition_policies.txt
   ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/thttpd/output/partition_policies.txt --source examples/thttpd/input/thttpd.c --bc examples/thttpd/input/thttpd.bc --output examples/thttpd/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.

## Running the  Partitioned Program

The result of automatic partitioning may require manual adjustments. We provide a runnable version in `output/finally_partition`.

### 1. Compilation

First, decompress the two archives in `examples/thttpd/output/finally_partition/`.

Then, compile the client and server components similarly to the 64-bit executable in the preprocessing stage.

**Compile thttpd_client:**
```bash
cd examples/thttpd/output/finally_partition/thttpd_client/thttpd-2.27
./configure
make -j8
# The thttpd_client executable will be generated in the current directory.
```

**Compile thttpd_server:**
```bash
cd examples/thttpd/output/finally_partition/thttpd_server/thttpd-2.27
./configure
make -j8
# The thttpd_server executable will be generated in the current directory.
```

### 2. Execution

Run the client and server in two separate terminals.

- **Terminal 1 (Server):**
  ```bash
  ./examples/thttpd/output/finally_partition/thttpd_server/thttpd-2.27/thttpd_server
  ```
- **Terminal 2 (Client):**
  ```bash
  ./examples/thttpd/output/finally_partition/thttpd_client/thttpd-2.27/thttpd_client -D  -p 8080 -d examples/thttpd/html/
  ```
- **Terminal 3 (On host):**
  ```bash
  curl -u testuser:123456 http://localhost:8080/protected/
  ```
After confirming the download is successful, you can stop the client and server by pressing Ctrl+C in their respective terminals.

