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
     #unpack the thttp-2.2.7.tar.xz archive in this directory.
     ./configure
     # Add `-g -O0 ` to `CCOPT`.
     bear make -j8
     mv thttpd ../../thttpd_64
     mv compile_commands.json ../../
     make clean
     ```

2. **Compilation Database**
   - The shell commands above have already moved the compilation database (`compile_commands.json`) to the `thttpd/input` directory. (移除不必要的c文件) //by xue

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
     # To ensure the thttpd process can access the .htpasswd file, create a user in the container
     # with the same UID and GID as the file's owner.
     # First, inspect the file to find its user and group IDs.
     # For example, running 'ls -lh examples/thttpd/html/protected/.htpasswd' might produce:
     # -rw-rw---- 1 1000 1000  23 Oct  9 03:01 examples/thttpd/html/protected/.htpasswd
     # In this output, the UID is 1000 and the GID is 1000. Use these values in the commands below.
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

   - Merge traces and map quantitative info to statements:(running on the host)
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/thttpd
     ```
3. **Collect Edge Information**
   - On the host machine, run the 64-bit executable with Pin to generate `.pinout` trace files. This requires running the server under Pin in one terminal and using `curl` to interact with it from another.

     **Example: Accessing a protected resource**
     - In your first terminal, start the `thttpd` server under Pin. We'll use port 8081 to avoid conflicts.
       ```bash
       src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/thttpd/output/temp/thttpd_protected.pinout -- ./examples/thttpd/input/thttpd_64 -D -p 8081 -d examples/thttpd/html/
       ```
     - In a second terminal, use `curl` to request a file from the protected directory:
       ```bash
       curl -u testuser:123456 http://localhost:8081/protected/
       ```
     - After the `curl` command completes, return to the first terminal and press `Ctrl+C` to stop the server. This will generate `thttpd_protected.pinout`.


   - Once you have generated the trace files, merge them and replace addresses with symbol names:
     ```bash
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/thttpd
     ```
4. **Build Graph and Solve for Partitioning**

   This step uses an automated script to construct the graph and find mutiple partitioning solution. You can run the solver with different communication models and leakage budgets by specifying the `--comm-type` and `min-quan` parameters. The script will generate result files (e.g., `thttpd_z3_result_u_0bit.txt`) in the `examples/thttpd/output/` directory.

   -   **To solve using the unidirectional model (`u`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py thttpd min-quan=0 max-code-sz=0.01 --comm-type=u
       ```

   -   **To solve using the unidirectional model (`u`) with a 64-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py thttpd min-quan=64 max-code-sz=0.01 --comm-type=u
       ```

   -   **To solve using the bidirectional model (`b`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py thttpd min-quan=0 max-code-sz=0.01 --comm-type=b
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
    python3 scripts/prepare_refactor_data.py thttpd --comm-type=u --quan=0

    # For a unidirectional model with a 64-bit leakage budget
    python3 scripts/prepare_refactor_data.py thttpd --comm-type=u --quan=64

    # For a bidirectional model with a 0-bit leakage budget
    python3 scripts/prepare_refactor_data.py thttpd --comm-type=b --quan=0
    ```

  -  **Optional: Analyze Partitioning Statistics**

        After preparing the refactoring data, you can run an additional script to view detailed statistics about the chosen partition. This is not a required step for the main workflow but is useful for analysis. It provides information such as the number of functions in each partition and the percentage of sensitive code.

      Command Format:

      ```bash
          # Analyze the result for a unidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py thttpd --comm-type=u --quan=0

          # Analyze the result for a unidirectional model with a 64-bit leakage budget
          python3 scripts/analyze_partition_results.py thttpd --comm-type=u --quan=64

          # Analyze the result for a bidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py thttpd --comm-type=b --quan=0
      ```

6. **Code Refactoring**
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

