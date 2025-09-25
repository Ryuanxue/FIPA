# wget Partitioning Project

## Project Overview

`wget` is a widely used utility for non-interactive network downloads. This project demonstrates fine-grained partitioning of the `wget` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources:  Data from internet.
- Annotation method: Annotate sensitive sources in the code using `FC_TAINT_WORD`. For details, refer to `source_input/diff.patch`.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Ensure dependencies are installed as required by your project.
        ```
        apt-get install pkg-config
        apt-get install libgnutls28-dev
        ```
   - Navigate to the source directory:
     ```bash
     cd examples/wget/input/source_code/wget-1.8
     ./configure   # (sometimes not needed)
     bear make CFLAGS+="-g -O0" -j8 
     cd src
     mv wget ../../../wget_64
     cd ..
     make clean
     ```

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/wget/input/source_code/wget-1.8` (if available).
   - In `examples/wget/input`, create a new `compile_commands.json` file.
   - From the original compilation database, copy all entries where the `"directory"` field contains the substring `examples/wget/input/source_code/wget-1.8/src` (i.e., any entry whose source file is under the `src` subdirectory). Paste these entries into the new file. This ensures only the `wget` source files are included for partitioning.

3. **LLVM Bitcode File (.bc)**
   - Set environment variables for bitcode compilation:
     ```bash
     export CC=clang
     export LDFLAGS="-flto -fuse-ld=lld"
     export CFLAGS="-flto -g -O0 -fno-discard-value-names -fembed-bitcode"
     ```
   - Clean previous builds and config cache to ensure environment variables take effect:
     ```bash
     make clean
     rm -f config.cache
     ./configure
     make -j8 V=1   # V=1 shows full compilation commands
     ```
   - All generated `.o` files will be LLVM bitcode objects.
   - Locate the final link command for the `wget` executable (e.g., starting with `clang ... -o wget ...`).
   - Modify the link command by adding `-Wl,--plugin-opt=emit-llvm` and changing the output to `wget.bc`, for example:
     ```bash
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -flto -fuse-ld=lld -o wget.bc connect.o convert.o cookies.o ftp.o css_.o css-url.o ftp-basic.o ftp-ls.o hash.o host.o hsts.o html-parse.o html-url.o http.o init.o log.o main.o netrc.o progress.o ptimer.o recur.o res.o retr.o spider.o url.o warc.o utils.o exits.o build_info.o   version.o ftp-opie.o gnutls.o http-ntlm.o
     mv wget.bc ../../../wget.bc
     cd ..
     make clean
     ```

4. **32-bit Executable (for FlowCheck)**
   - Navigate to the source directory:
     ```bash
     cd examples/wget/input/source_code/
     cp -r wget-1.18 wget-1.18_back
     patch -d wget-1.18 -p1 < diff.patch
     cp FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/wget/input/source_code/wget-1.18
     rm -f config.cache
     ./configure   
     make CFLAGS+="-g -O0 -m32 -I/flowcheck/include"
     cd src
     mv wget ../../../wget_32
     exit

     ```
   - Copy the generated `wget` binary from `examples/wget/input/source_code/wget-1.8/src` to `examples/wget/input/` and rename it to `wget_32`.

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/wget --compile_db examples/wget/input/compile_commands.json --output_dir examples/wget/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.

     Start Docker:
     ```bash
     docker run -it -v .:/Desktop flowcheck-image
     ```
     Example 1: Test HTTP redirect
     ```bash
     valgrind --tool=exp-flowcheck --fullpath-after= --folding-level=0 --project-name=wget-1.18/src --trace-secret-graph=yes --graph-file=temp.g ./examples/wget/input/wget_32 http://httpbin.org/redirect/1 2>examples/wget/output/temp/wgetoutput1.fc
     # Note: This command may take some time to complete. Please wait patiently.
     ```
     Example 2: Download a local file via HTTP
     - On the host, in the directory containing `test.txt` (e.g., `wget/input`), start an HTTP server:
       ```bash
       python3 -m http.server 8000
       ```
     - In Docker, run FlowCheck to download the file:
       ```bash
       valgrind --tool=exp-flowcheck --fullpath-after= --folding-level=0 --project-name=wget-1.18/src --trace-secret-graph=yes --graph-file=temp.g ./examples/wget/input//wget_32 http://localhost:8000/test.txt 2>examples/wget/output/temp/wgetoutput2.fc
        # Note: This command may take some time to complete. Please wait patiently.
       ```
   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/wget
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files. For example:
        
        Example 1: Test HTTP redirect
        ```bash
        src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin \
        -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so \
        -o examples/wget/output/temp/wgetoutput1.pinout -- ./examples/wget/input/wget_64 http://httpbin.org/redirect/1

        ```

        Example 2: Download a local file via HTTP
        - On the host, in the directory containing `test.txt` (e.g., `wget/input`), start an HTTP server:
        ```bash
        python3 -m http.server 8000
        ```
        - In your main terminal, run Pin to download the file:
        ```bash
            cd FIPA
            src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin \
            -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so \
            -o examples/wget/output/temp/wgetoutput2.pinout -- ./examples/wget/input/wget_64 http://localhost:8000/test.txt
        ```
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/wget
     ```
4. **Build Graph and Solve for Partitioning**

   This step uses an automated script to construct the graph and find an optimal partitioning solution. You can run the solver with different communication models by specifying the `--so-type` parameter. The script will generate result files (e.g., `wget_z3_result_u.txt`) in the `examples/wget/output/` directory.

   -   **To solve using the unidirectional model (`u`):**
       ```bash
       python3 scripts/based_qg_bi_praming.py wget min-quan=0 max-code-sz=0.1 --so-type=u
       ```

   -   **To solve using the bidirectional model (`b`):**
       ```bash
       python3 scripts/based_qg_bi_praming.py wget min-quan=0 max-code-sz=0.1 --so-type=b
       ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/wget/output/partition_policies.txt --source examples/wget/input/wget.c --bc examples/wget/input/wget.bc --output examples/wget/output/refactored/
   ```

## Running the  Partitioned Program

The result of automatic partitioning may require manual adjustments. We provide a runnable version in `output/finally_partition`.

### 1. Compilation

First, decompress the two archives in `examples/wget/output/finally_partition/`.

Then, compile the client and server components similarly to the 64-bit executable in the preprocessing stage.

**Compile wget_client:**
```bash
cd examples/wget/output/finally_partition/wget_client/wget-1.18
./configure
make CFLAGS+="-g -O0" -j8
# The wget executable will be generated in the src/ directory.
```

**Compile wget_server:**
```bash
cd examples/wget/output/finally_partition/wget_server/wget-1.18
./configure
make CFLAGS+="-g -O0" -j8
# The wget executable will be generated in the src/ directory.
```

### 2. Execution

Run the client and server in two separate terminals.

**Example 1: Test HTTP redirect**
- **Terminal 1 (Server):**
  ```bash
  ./examples/wget/output/finally_partition/wget_server/wget-1.18/src/wget
  ```
- **Terminal 2 (Client):**
  ```bash
  ./examples/wget/output/finally_partition/wget_client/wget-1.18/src/wget www.baidu.com 80
  ```

**Example 2: Download a local file via HTTP**
- **Terminal 1 (Server):**
  ```bash
  ./examples/wget/output/finally_partition/wget_server/wget-1.18/src/wget
  ```
- **Terminal 2 (HTTP Server on host):**
  ```bash
  # In the directory containing test.txt
  python3 -m http.server 8000
  ```
- **Terminal 3 (Client):**
  ```bash
  ./examples/wget/output/finally_partition/wget_client/wget-1.18/src/wget http://localhost:8000/test.txt
  ```
