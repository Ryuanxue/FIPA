# mavlink-server Partitioning Project

## Project Overview

`mavlink-server` is a utility designed to receive heartbeat messages sent from MAVLink clients. 

## Annotation Strategy

- Sensitive sources: Data from internet.
- Annotation method: Annotated using FlowCheck's API, `FC_TAINT_WORD`. For details, refer to the patch file `diff.patch` in the `source_code` directory.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the entire project (no need for autogen.sh or bear).
   - Ensure dependencies are installed as required by your project.
   - Navigate to the source directory:
     ```bash
     cd examples/mavlink-server/input/source_code/mavlink-server
     bear make mavlink_server
     mv mavlink_server ../../mavlink_server_64
     mv compile_commands.json ../../
     make clean
     ```
2. **Compilation Database**
    - The shell commands above have already moved the compilation database (`compile_commands.json`) to the `mavlink-server/input` directory.

3. **LLVM Bitcode File (.ll)**
   - Rebuild with bitcode flags:
     ```bash
     
     clang -g -S -emit-llvm -O0 -Igenerated/include  -g -O0 -fno-discard-value-names  mavlink_server.c
     mv mavlink_server.ll ../../
     ```

4. **32-bit Executable (for FlowCheck)**
   - Start Docker:
     ```bash
     cd examples/mavlink-server/input/source_code
     cp -r  mavlink-server mavlink-server_back
     patch -d mavlink-server -p1 < diff.patch
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/mavlink-server/input/source_code/mavlink-server
     make mavlink_server_32
     mv mavlink_server ../../mavlink_server_32
     make clean
     exit
     ```

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/mavlink-server --compile_db examples/mavlink-server/input/compile_commands.json --output_dir examples/mavlink-server/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.

     ```bash
     cd FIPA
     docker run --network host -it -v .:/Desktop flowcheck-image
     ```

     In Docker, use Valgrind to start the 32-bit server executable:
     ```bash
     valgrind --tool=exp-flowcheck --private-files-are-secret=yes --fullpath-after= --folding-level=0 --project-name=mavlink-server --trace-secret-graph=yes --graph-file=temp.g ./examples/mavlink-server/input/mavlink_server_32 2>examples/mavlink-server/output/temp/mavlink-serveroutput1.fc
     ```

     On the host, start the mavlink_client_64 program:
     ```bash
     ./examples/mavlink-client/input/mavlink_client_64
     ```

     After observing the reception of heartbeat packets in Docker, stop both the Docker mavlink_server_32 and the host mavlink_client_64 by pressing Ctrl+C in each terminal.

   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/mavlink-server
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.

     In terminal, use Pin to trace the 64-bit server executable with different inputs:
     ```bash
     cd FIPA
     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/mavlink-server/output/temp/mavlink-server1.pinout -- ./examples/mavlink-server/input/mavlink_server_64
     # You can run additional tests with different arguments or input files as needed
     ```

     On anoter terminal, start the mavlink_client_64 program (from the mavlink-client project):
     ```bash
     cd FIPA
     ./examples/mavlink-client/input/mavlink_client_64
     ```

   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/mavlink-server
     ```
4. **Build Graph and Solve for Partitioning**

   This step uses an automated script to construct the graph and find an optimal partitioning solution. You can run the solver with different communication models by specifying the `--so-type` parameter. The script will generate result files (e.g., `mavlink-server_z3_result_u.txt`) in the `examples/mavlink-server/output/` directory.

   -   **To solve using the unidirectional model (`u`):**
       ```bash
       python3 scripts/based_qg_bi_praming.py mavlink-server min-quan=0 max-code-sz=0.1 --so-type=u
       ```

   -   **To solve using the bidirectional model (`b`):**
       ```bash
       python3 scripts/based_qg_bi_praming.py mavlink-server min-quan=0 max-code-sz=0.1 --so-type=b
       ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/mavlink-server/output/partition_policies.txt --source examples/mavlink-server/input/mavlink-server.c --bc examples/mavlink-server/input/mavlink-server.ll --output examples/mavlink-server/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.

## Running the Partitioned Program

The result of automatic partitioning may require manual adjustments. We provide a runnable version in `output/finally_partition`.

### 1. Compilation

First, decompress the two archives in `examples/mavlink-server/output/finally_partition/`.

Then, compile the client and server components similarly to the 64-bit executable in the preprocessing stage.

**Compile mavlink_server_client:**
```bash
cd examples/mavlink-server/output/finally_partition/mavlink_server_client/mavlink-server
make mavlink_server
# The mavlink_server executable will be generated in the current directory.
```

**Compile mavlink_server_server:**
```bash
cd examples/mavlink-server/output/finally_partition/mavlink_server_server/mavlink-server
make mavlink_server
# The mavlink_server executable will be generated in the current directory.
```

### 2. Execution

Run the client and server in two separate terminals.

- **Terminal 1 (Partitioned Server):**
  ```bash
  ./examples/mavlink-server/output/finally_partition/mavlink_server_server/mavlink-server/mavlink_server
  ```
- **Terminal 2 (Partitioned Client):**
  ```bash
  ./examples/mavlink-server/output/finally_partition/mavlink_server_client/mavlink-server/mavlink_server
  ```
- **Terminal 3 (Original Client):**
  ```bash
  # Start the original 64-bit client from the mavlink-client project
  ./examples/mavlink-client/input/mavlink_client_64
  ```
After observing the reception of heartbeat packets, you can stop all three processes by pressing Ctrl+C in each terminal.
