# mavlink-client Partitioning Project

## Project Overview

`mavlink-client` is a client utility for interacting with MAVLink-based systems. This project demonstrates fine-grained partitioning of the `mavlink-client` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: Heartbeat packet data.
- Annotation method: Annotated using FlowCheck's API, `FC_TAINT_WORD`. For details, refer to the patch file `diff.patch` in the `source_code` directory.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the entire project (no need for autogen.sh or bear).
   - Ensure dependencies are installed as required by your project.
   - Navigate to the source directory:
     ```bash
     cd examples/mavlink-client/input/source_code/mavlink-client
     bear make mavlink_client
     mv mavlink_client ../../mavlink_client_64
     mv compile_commands.json ../../
     make clean
     ```
2. **Compilation Database**
    - The shell commands above have already moved the compilation database (`compile_commands.json`) to the `mavlink-client/input` directory.

3. **LLVM Bitcode File (.ll)**
   - Rebuild with bitcode flags:
     ```bash
     
     clang -c -g -emit-llvm -O0 -Igenerated/include  -g -O0 -fno-discard-value-names mavlink_client.c -o mavlink-client.bc
     mv mavlink-client.bc ../../
     ```

4. **32-bit Executable (for FlowCheck)**
   - Start Docker:
     ```bash
     cd examples/mavlink-client/input/source_code
     cp -r  mavlink-client mavlink-client_back
     patch -d mavlink-client -p1 < diff.patch
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/mavlink-client/input/source_code/mavlink-client
     make mavlink_client_32
     mv mavlink_client ../../mavlink_client_32
     make clean
     exit
     ```

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/mavlink-client --compile_db examples/mavlink-client/input/compile_commands.json --output_dir examples/mavlink-client/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.

     In the project root (FIPA), start Docker with host networking:
     ```bash
     docker run --network host -it -v .:/Desktop flowcheck-image
     ```

     On the host, first start the mavlink_server_64 program (from the mavlink-server project):
     ```bash
     ./examples/mavlink-server/input/mavlink_server_64
     ```

     In Docker, run FlowCheck on the 32-bit client executable:
     ```bash
     valgrind --tool=exp-flowcheck --private-files-are-secret=yes --fullpath-after= --folding-level=0 --project-name=mavlink-client --trace-secret-graph=yes --graph-file=temp.g ./examples/mavlink-client/input/mavlink_client_32 2>examples/mavlink-client/output/temp/mavlink-clientoutput1.fc
     ```

     After observing the transmission of several heartbeat packets (e.g., 3-4), stop both the Docker mavlink_client_32 and the host mavlink_server_64 by pressing Ctrl+C in each terminal.

   - Merge traces and map quantitative info to statements ((running on the host)):
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/mavlink-client
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.

     On the host, start the mavlink_server_64 program (from the mavlink-server project):
     ```bash
     cd FIPA
     ./examples/mavlink-server/input/mavlink_server_64
     ```

     In a separate terminal, use Pin to trace the 64-bit client executable with different inputs:
     ```bash
     cd FIPA
     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/mavlink-client/output/temp/mavlink-client1.pinout -- ./examples/mavlink-client/input/mavlink_client_64
     ```

     After observing the transmission of several heartbeat packets (e.g., 3-4), stop both the mavlink_client_64 and the host mavlink_server_64 by pressing Ctrl+C in each terminal.

   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/mavlink-client
     ```
4. **Build Graph and Solve for Partitioning**

   This step uses an automated script to construct the graph and find mutiple partitioning solution. You can run the solver with different communication models and leakage budgets by specifying the `--comm-type` and `min-quan` parameters. The script will generate result files (e.g., `mavlink-client_z3_result_u_0bit.txt`) in the `examples/mavlink-client/output/` directory.

   -   **To solve using the unidirectional model (`u`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py mavlink-client min-quan=0 max-code-sz=0.1 --comm-type=u
       ```

   -   **To solve using the unidirectional model (`u`) with a 64-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py mavlink-client min-quan=64 max-code-sz=0.1 --comm-type=u
       ```

   -   **To solve using the bidirectional model (`b`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py mavlink-client min-quan=0 max-code-sz=0.1 --comm-type=b
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
    python3 scripts/prepare_refactor_data.py mavlink-client --comm-type=u --quan=0

    # For a unidirectional model with a 64-bit leakage budget
    python3 scripts/prepare_refactor_data.py mavlink-client --comm-type=u --quan=64

    # For a bidirectional model with a 0-bit leakage budget
    python3 scripts/prepare_refactor_data.py mavlink-client --comm-type=b --quan=0
    ```

  -  **Optional: Analyze Partitioning Statistics**

        After preparing the refactoring data, you can run an additional script to view detailed statistics about the chosen partition. This is not a required step for the main workflow but is useful for analysis. It provides information such as the number of functions in each partition and the percentage of sensitive code.

      Command Format:

      ```bash
          # Analyze the result for a unidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py mavlink-client --comm-type=u --quan=0

          # Analyze the result for a unidirectional model with a 64-bit leakage budget
          python3 scripts/analyze_partition_results.py mavlink-client --comm-type=u --quan=64

          # Analyze the result for a bidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py mavlink-client --comm-type=b --quan=0
      ```

6. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/mavlink-client/output/partition_policies.txt --source examples/mavlink-client/input/mavlink-client.c --bc examples/mavlink-client/input/mavlink-client.ll --output examples/mavlink-client/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.

## Running the Manually Partitioned Program

The result of automatic partitioning may require manual adjustments. We provide a runnable version in `output/finally_partition`.

### 1. Compilation

First, decompress the two archives in `examples/mavlink-client/output/finally_partition/`.

Then, compile the client and server components similarly to the 64-bit executable in the preprocessing stage.

**Compile mavlink_client_client:**
```bash
cd examples/mavlink-client/output/finally_partition/mavlink-client-client/mavlink-client
make mavlink_client
# The mavlink_client executable will be generated in the current directory.
```

**Compile mavlink_client_server:**
```bash
cd examples/mavlink-client/output/finally_partition/mavlink_client_server/mavlink-client
make mavlink_client_svc
# The mavlink_client_svc executable will be generated in the current directory.
```

### 2. Execution

Run the client and server in two separate terminals. This requires the original `mavlink_server_64` to be running first to act as the endpoint.

- **Terminal 1 (Original Server):**
  ```bash
  # Start the original 64-bit server from the mavlink-server project
  ./examples/mavlink-server/input/mavlink_server_64
  ```
- **Terminal 2 (Partitioned rpc Server):**
  ```bash
  ./examples/mavlink-client/output/finally_partition/mavlink_client_server/mavlink-client/mavlink_client_svc
  ```
- **Terminal 3 (Partitioned Client):**
  ```bash
  ./examples/mavlink-client/output/finally_partition/mavlink-client_client/mavlink-client/mavlink_client
  ```
After observing the transmission of several heartbeat packets, you can stop all three processes by pressing Ctrl+C in each terminal.
