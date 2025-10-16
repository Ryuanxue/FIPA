# telnet Partitioning Project

## Project Overview

`telnet` is a client utility for remote terminal access, typically built from the `inetutils` suite. This project demonstrates fine-grained partitioning of the `telnet` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: Data from internet.
- Annotation method: Annotate sensitive sources in the code using `FC_TAINT_WORD`. For details, refer to the patch file `diff.patch` under `source_code`.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the `inetutils` project with only the `telnet` client enabled.
   - Ensure all required dependencies are installed.
   - Navigate to the source directory:
     ```bash
     cd examples/telnet/input/source_code/inetutils-1.9.4
     ./configure --disable-servers --disable-clients --enable-telnet
     # Edit the Makefile: locate the CFLAGS option and change `-O2` to `-O0` for debugging.
     bear make -j8
     cd telnet
     mv telnet ../../../telnet_64
     ```

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/telnet/input/source_code/inetutils-1.9.4` (if available).
   - In `examples/telnet/input`, create a new `compile_commands.json` file.
   - From the original compilation database, copy all entries where the `"directory"` field contains the substring `examples/telnet/input/source_code/inetutils-1.9.4/telnet` (i.e., any entry whose source file is under the `telnet` subdirectory). Paste these entries into the new file. This ensures only the `telnet` source files are included for partitioning.

3. **LLVM Bitcode File (.bc)**
   - Rebuild with bitcode flags:
     ```bash
     make clean
     ./configure --disable-servers --disable-clients --enable-telnet CC=clang CFLAGS="-flto -g -O0 -fno-discard-value-names -fembed-bitcode " LDFLAGS="-flto -fuse-ld=lld"
     make -j8 V=1
     ```
   - Locate the  link command (e.g., starting with `clang ... -o telnet ...`).
   - Modify the command by adding `-Wl,--plugin-opt=emit-llvm`, changing `-o telnet` to `-o telnet.bc`, so it looks like:
     ```bash
     cd telnet
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -flto -fuse-ld=lld -o telnet.bc authenc.o commands.o main.o network.o ring.o sys_bsd.o telnet.o terminal.o tn3270.o utilities.o ../libtelnet/libtelnet.a ../libinetutils/libinetutils.a ../lib/libgnu.a -ltermcap -lcrypt
     mv telnet.bc ../../../telnet.bc
     cd ..
     make clean
     ```

4. **32-bit Executable (for FlowCheck)**
   - Start Docker:
     ```bash
     cd FIPA/examples/telnet/input/source_code
     cp -r inetutils-1.9.4 inetutils-1.9.4_back
     patch -d inetutils-1.9.4 -p1 < diff.patch
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/telnet/input/source_code/inetutils-1.9.4
     ./configure --disable-servers --disable-clients --enable-telnet CFLAGS="-g -O0 -m32  -I/flowcheck/include/"
     make -j8
     cd telnet
     mv telnet ../../../telnet_32
     cd ..
     make clean
     exit
     ```

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/telnet --compile_db examples/telnet/input/compile_commands.json --output_dir examples/telnet/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.

     By default, the host and Docker container are network-isolated. To allow the container to use the host's network stack, add `--network=host` when starting Docker:
     ```bash
     cd FIPA
     docker run -it --network=host -v .:/Desktop flowcheck-image
     ```
     Example FlowCheck usage:
     - For local connection (requires running `nc -l 8081` in any terminal on the host, not inside Docker):
       ```bash
       # On the host (not in Docker), start a listener:
       nc -l 8081
       # In Docker, run FlowCheck:
       valgrind --tool=exp-flowcheck --fullpath-after= --folding-level=0 --project-name=inetutils-1.9.4/telnet --trace-secret-graph=yes --graph-file=temp.g ./examples/telnet/input/telnet_32 localhost 8081 2>examples/telnet/output/temp/telnetoutput1.fc
       # When you see "Escape character is '^]'", type "hello" and press Enter, then press Ctrl+], type q and press Enter to exit.
       ```
     - Connect to local telnetd service:
       
       **Prerequisites:** Before running the following commands, install and configure xinetd and telnetd services on the host:
       ```bash
       # Install required packages
       sudo apt install xinetd telnetd
       
       # Configure telnet service in xinetd
       sudo vim /etc/xinetd.d/telnet
       
       Add the following content to the file:
      
       service telnet
       {
           disable = no
           flags = REUSE
           socket_type = stream
           wait = no
           user = root
           server = /usr/sbin/in.telnetd
           log_on_failure += USERID
       }
       
       
       # Start the xinetd service
       sudo systemctl start xinetd
       
       # Verify service status (should show "active (running)")
       sudo systemctl status xinetd
       
       # Check if telnet port 23 is listening
       sudo ss -tuln | grep 23
       ```
       
       **Run FlowCheck:**
       ```bash
       valgrind --tool=exp-flowcheck --fullpath-after= --folding-level=0 --project-name=inetutils-1.9.4/telnet --trace-secret-graph=yes --graph-file=temp.g ./examples/telnet/input/telnet_32 127.0.0.1 23 2>examples/telnet/output/temp/telnetoutput2.fc
       # When you see "ubuntu login:", press Ctrl+], type q and press Enter to exit.
       exit
       ```
   - Merge traces and map quantitative info to statements (running on the host):
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/telnet
     ```
3. **Collect Edge Information**
   - Run Pin in the host environment with different inputs to generate `.pinout` files. For example:
     ```bash
     # Example 1: Connect to a local listener
     # On the host (not in Docker), open another terminal and run:
     nc -l 8081
     # Then, in your main terminal, run Pin:
     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/telnet/output/temp/telnetoutput1.pinout -- ./examples/telnet/input/telnet_64 localhost 8081
     # When you see "Escape character is '^]'", type "hello" and press Enter, then press Ctrl+], type q and press Enter to exit.

     # Example 2: Connect to an external server
     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/telnet/output/temp/telnetoutput2.pinout -- ./examples/telnet/input/telnet_64 127.0.0.1 23
     # When you see "ubuntu login:", press Ctrl+], type q and press Enter to exit.
     ```
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/telnet
     ```
4. **Build Graph and Solve for Partitioning**

   This step uses an automated script to construct the graph and find mutiple partitioning solution. You can run the solver with different communication models and leakage budgets by specifying the `--comm-type` and `min-quan` parameters. The script will generate result files (e.g., `telnet_z3_result_u_0bit.txt`) in the `examples/telnet/output/` directory.

   -   **To solve using the unidirectional model (`u`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py telnet min-quan=0 max-code-sz=0.1 --comm-type=u
       ```

   -   **To solve using the unidirectional model (`u`) with a 64-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py telnet min-quan=64 max-code-sz=0.1 --comm-type=u
       ```

   -   **To solve using the bidirectional model (`b`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py telnet min-quan=0 max-code-sz=0.1 --comm-type=b
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
    python3 scripts/prepare_refactor_data.py telnet --comm-type=u --quan=0

    # For a unidirectional model with a 64-bit leakage budget
    python3 scripts/prepare_refactor_data.py telnet --comm-type=u --quan=64

    # For a bidirectional model with a 0-bit leakage budget
    python3 scripts/prepare_refactor_data.py telnet --comm-type=b --quan=0
    ```

  -  **Optional: Analyze Partitioning Statistics**

        After preparing the refactoring data, you can run an additional script to view detailed statistics about the chosen partition. This is not a required step for the main workflow but is useful for analysis. It provides information such as the number of functions in each partition and the percentage of sensitive code.

      Command Format:

      ```bash
          # Analyze the result for a unidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py telnet --comm-type=u --quan=0

          # Analyze the result for a unidirectional model with a 64-bit leakage budget
          python3 scripts/analyze_partition_results.py telnet --comm-type=u --quan=64

          # Analyze the result for a bidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py telnet --comm-type=b --quan=0
      ```

6. **Code Refactoring**
   
   This step generates the final partitioned code with RPC communication interfaces. The example below uses unidirectional communication with 0-bit leakage. For other configurations, modify the `--comm-type` and `--quan` parameters accordingly.
   
   ```bash
   python3 scripts/refactor_function_add_rpc.py telnet --comm-type=u --quan=0
   ```
   
   **Parameter Options:**
   - `--comm-type`: Communication model (`u` for unidirectional, `b` for bidirectional)
   - `--quan`: Leakage budget in bits (e.g., `0`, `64`)

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.

## Running the Partitioned Program

The result of automatic partitioning may require manual adjustments. We provide a runnable version in `output/finally_partition`.

### 1. Compilation

First, decompress the two archives in `examples/telnet/output/finally_partition/`.

Then, compile the client and server components similarly to the 64-bit executable in the preprocessing stage.

**Compile telnet_client:**
```bash
cd examples/telnet/output/finally_partition/telnet_client/inetutils-1.9.4
make -j8
# The telnet executable will be generated in the telnet/ directory.
```

**Compile telnet_server:**
```bash
cd examples/telnet/output/finally_partition/telnet_server/inetutils-1.9.4
make -j8
# The telnet executable will be generated in the telnet/ directory.
```

### 2. Execution

Run the client and server in two separate terminals.

**Example 1: Connect to a local listener**
- **Terminal 1 (Server):**
  ```bash
  ./examples/telnet/output/finally_partition/telnet_server/inetutils-1.9.4/telnet/telnet
  ```

- **Terminal 2 (Client):**
  ```bash
  ./examples/telnet/output/finally_partition/telnet_client/inetutils-1.9.4/telnet/telnet localhost
  ```

**Example 2: Connect to an external server**
- **Terminal 1 (Server):**
  ```bash
  ./examples/telnet/output/finally_partition/telnet_server/inetutils-1.9.4/telnet/telnet
  ```
- **Terminal 2 (Client):**
  ```bash
  ./examples/telnet/output/finally_partition/telnet_client/inetutils-1.9.4/telnet/telnet www.baidu.com 80
  ```
