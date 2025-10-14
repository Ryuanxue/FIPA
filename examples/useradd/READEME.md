# useradd Partitioning Project

## Project Overview

`useradd` is a utility from the `shadow-utils` suite, used to create new user accounts. This project demonstrates fine-grained partitioning of the `useradd` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: `/etc/passwd` and `/etc/shadow`
- Annotation method: Modify the permissions of `/etc/passwd` and `/etc/shadow` to restrict access.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the entire `shadow-utils` project (not just `useradd`).
   - Ensure dependencies are installed:
     ```bash
     sudo apt-get install autoconf autopoint gettext
     sudo apt install -y libsemanage-dev libselinux1-dev
     ```
   - Navigate to the source directory:
     ```bash
     cd examples/useradd/input/source_code/shadow-utils
     ./autogen.sh
     bear make -j8
     cd src
     mv useradd ../../../useradd_64
     ```

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/useradd/input/source_code/shadow-utils`.
   - Find the entry corresponding to `useradd.c`. Copy only the `useradd.c` entry into a new file at `examples/useradd/input/compile_commands.json`. This ensures that only the source code for `useradd.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Rebuild with bitcode flags:
     ```bash
     make clean
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     # Note: If you encounter linking errors, please ignore them as we only need the intermediate IR representation
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o useradd.bc useradd.o
     mv useradd.bc ../../../useradd.bc
     cd ../
     make clean
     ```

4. **32-bit Executable (for FlowCheck)**
   - In `autogen.sh`, add `-m32` to CFLAGS.
   - Start Docker:
     ```bash
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/useradd/input/source_code/shadow-utils
     ./autogen.sh
     make -j8
     cd src
     mv useradd ../../../useradd_32
     cd ..
     make clean
     exit
     ```

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/useradd --compile_db examples/useradd/input/compile_commands.json --output_dir examples/useradd/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files:
        Start Docker and set file permissions:
        ```bash
        docker run -it -v .:/Desktop flowcheck-image
        chmod o-r /etc/passwd /etc/shadow
        ```
        Run FlowCheck for different test cases:
        ```bash
        valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=shadow-utils/src --fullpath-after= --folding-level=0 --trace-secret-graph=yes --graph-file=temp.g ./examples/useradd/input/useradd_32 -m -s /bin/zsh rawuser1 2>examples/useradd/output/temp/useraddoutput1.fc

        valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=shadow-utils/src --fullpath-after= --folding-level=0 --trace-secret-graph=yes --graph-file=temp.g ./examples/useradd/input/useradd_32 -m -p '$6$hashedpassword' -e 2025-12-31 rawuser2 2>examples/useradd/output/temp/useraddoutput2.fc
        ```
   - Merge traces and map quantitative info to statements (running on the host):
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/useradd
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files:
     ```bash
     cd FIPA
     su root
     ```
     Run Pin for different test cases:
     ```bash
     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/useradd/output/temp/useradd1.pinout -- ./examples/useradd/input/useradd_64 -m -s /bin/zsh rawuser1
     
     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/useradd/output/temp/useradd2.pinout -- ./examples/useradd/input/useradd_64 -m -p '$6$hashedpassword' -e 2025-12-31 rawuser2
     ```
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/useradd
     ```
4. **Build Graph and Solve for Partitioning**

   This step uses an automated script to construct the graph and find mutiple partitioning solution. You can run the solver with different communication models and leakage budgets by specifying the `--comm-type` and `min-quan` parameters. The script will generate result files (e.g., `useradd_z3_result_u_0bit.txt`) in the `examples/useradd/output/` directory.

   -   **To solve using the unidirectional model (`u`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py useradd min-quan=0 max-code-sz=0.1 --comm-type=u
       ```

   -   **To solve using the unidirectional model (`u`) with a 64-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py useradd min-quan=64 max-code-sz=0.1 --comm-type=u
       ```

   -   **To solve using the bidirectional model (`b`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py useradd min-quan=0 max-code-sz=0.01 --comm-type=b
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
    python3 scripts/prepare_refactor_data.py useradd --comm-type=u --quan=0

    # For a unidirectional model with a 64-bit leakage budget
    python3 scripts/prepare_refactor_data.py useradd --comm-type=u --quan=64

    # For a bidirectional model with a 0-bit leakage budget
    python3 scripts/prepare_refactor_data.py useradd --comm-type=b --quan=0
    ```

  -  **Optional: Analyze Partitioning Statistics**

        After preparing the refactoring data, you can run an additional script to view detailed statistics about the chosen partition. This is not a required step for the main workflow but is useful for analysis. It provides information such as the number of functions in each partition and the percentage of sensitive code.

      Command Format:

      ```bash
          # Analyze the result for a unidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py useradd --comm-type=u --quan=0

          # Analyze the result for a unidirectional model with a 64-bit leakage budget
          python3 scripts/analyze_partition_results.py useradd --comm-type=u --quan=64

          # Analyze the result for a bidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py useradd --comm-type=b --quan=0
      ```

6. **Code Refactoring**
   
   This step generates the final partitioned code with RPC communication interfaces. The example below uses unidirectional communication with 0-bit leakage. For other configurations, modify the `--comm-type` and `--quan` parameters accordingly.
   
   ```bash
   python3 scripts/refactor_function_add_rpc.py useradd --comm-type=u --quan=0
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

First, decompress the two archives in `examples/useradd/output/finally_partition/`.

Then, compile the client and server components similarly to the 64-bit executable in the preprocessing stage.

**Compile useradd_client:**
```bash
cd examples/useradd/output/finally_partition/useradd_client/shadow-utils
make -j8
# The useradd_client executable will be generated in the src/ directory.
```

**Compile useradd_server:**
```bash
cd examples/useradd/output/finally_partition/useradd_server/shadow-utils
make -j8
# The useradd_server executable will be generated in the src/ directory.
```

### 2. Execution

Run the client and server in two separate terminals.

**Example 1: Create a new user with a specific shell**
- **Terminal 1 (Server):**
  ```bash
  sudo ./examples/useradd/output/finally_partition/useradd_server/shadow-utils/src/useradd_server
  ```
- **Terminal 2 (Client):**
  ```bash
  sudo ./examples/useradd/output/finally_partition/useradd_client/shadow-utils/src/useradd_client -m -s /bin/zsh paruser1
  ```

**Example 2: Create a new user with password and expiry date**
- **Terminal 1 (Server):**
  ```bash
  sudo ./examples/useradd/output/finally_partition/useradd_server/shadow-utils/src/useradd_server
  ```
- **Terminal 2 (Client):**
  ```bash
  sudo ./examples/useradd/output/finally_partition/useradd_client/shadow-utils/src/useradd_client -m -p '$6$hashedpassword' -e 2025-12-31 paruser2
  ```
