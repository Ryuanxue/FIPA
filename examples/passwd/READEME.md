# passwd Partitioning Project

## Project Overview

`passwd` is a utility from the `shadow-utils` suite, used to change a user's password. This project demonstrates fine-grained partitioning of the `passwd` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: `/etc/passwd` and `/etc/shadow`
- Annotation method: Modify the permissions of `/etc/passwd` and `/etc/shadow` to restrict access.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the entire `shadow-utils` project (not just `passwd`).
   - Ensure dependencies are installed:
     ```bash
     sudo apt-get install autoconf autopoint gettext
     sudo apt install -y libsemanage-dev libselinux1-dev
     ```
   - Navigate to the source directory:
     ```bash
     cd examples/passwd/input/source_code/shadow-utils
     ./autogen.sh
     bear make -j8
     cd src
     mv passwd ../../../passwd_64

     ```

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/passwd/input/source_code/shadow-utils`.
   - Find the entry corresponding to `passwd.c`. Copy only the `passwd.c` entry into a new file at `examples/passwd/input/compile_commands.json`. This ensures that only the source code for `passwd.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Rebuild with bitcode flags:
     ```bash
     make clean
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     # Note: If you encounter linking errors, please ignore them as we only need the intermediate IR representation
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o passwd.bc passwd.o
     mv passwd.bc ../../../passwd.bc
     cd ../
     make clean
     ```

4. **32-bit Executable (for FlowCheck)**
   - In `autogen.sh`, add `-m32` to CFLAGS.
   - Start Docker:
     ```bash
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/passwd/input/source_code/shadow-utils
     ./autogen.sh
     make -j8
     cd src
     mv passwd ../../../passwd_32
     exit
     ```

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/passwd --compile_db examples/passwd/input/compile_commands.json --output_dir examples/passwd/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.
        Start Docker and set file permissions:
        ```bash
        docker run -it -v .:/Desktop flowcheck-image
        chmod o-r /etc/passwd /etc/shadow
        ```

        Run FlowCheck with different inputs to generate trace files:
        ```bash
        valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=shadow-utils/src --fullpath-after= --folding-level=0 --trace-secret-graph=yes --graph-file=temp.g ./examples/passwd/input/passwd_32 -l nobody 2>examples/passwd/output/temp/passwdoutput1.fc
        
        valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=shadow-utils/src --fullpath-after= --folding-level=0 --trace-secret-graph=yes --graph-file=temp.g ./examples/passwd/input/passwd_32 -n 8 nobody 2>examples/passwd/output/temp/passwdoutput2.fc
        ```

   - Merge traces and map quantitative info to statements (running on the host):
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/passwd
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.
     ```bash
     cd FIPA
     su root
     ```
     Run Pin for different test cases:
     ```bash
     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/passwd/output/temp/passwd1.pinout -- ./examples/passwd/input/passwd_64 -l nobody

     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/passwd/output/temp/passwd2.pinout -- ./examples/passwd/input/passwd_64 -n 9 nobody
     ```
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/passwd
     ```
4. **Build Graph and Solve for Partitioning**

   This step uses an automated script to construct the graph and find mutiple partitioning solution. You can run the solver with different communication models and leakage budgets by specifying the `--comm-type` and `min-quan` parameters. The script will generate result files (e.g., `passwd_z3_result_u_0bit.txt`) in the `examples/passwd/output/` directory.

   -   **To solve using the unidirectional model (`u`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py passwd min-quan=0 max-code-sz=0.1 --comm-type=u
       ```

   -   **To solve using the unidirectional model (`u`) with a 64-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py passwd min-quan=64 max-code-sz=0.1 --comm-type=u
       ```

   -   **To solve using the bidirectional model (`b`) with a 0-bit leakage budget:**
       ```bash
       python3 scripts/based_qg_bi_praming.py passwd min-quan=0 max-code-sz=0.1 --comm-type=b
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
    python3 scripts/prepare_refactor_data.py passwd --comm-type=u --quan=0

    # For a unidirectional model with a 64-bit leakage budget
    python3 scripts/prepare_refactor_data.py passwd --comm-type=u --quan=64

    # For a bidirectional model with a 0-bit leakage budget
    python3 scripts/prepare_refactor_data.py passwd --comm-type=b --quan=0
    ```

  -  **Optional: Analyze Partitioning Statistics**

        After preparing the refactoring data, you can run an additional script to view detailed statistics about the chosen partition. This is not a required step for the main workflow but is useful for analysis. It provides information such as the number of functions in each partition and the percentage of sensitive code.

      Command Format:

      ```bash
          # Analyze the result for a unidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py passwd --comm-type=u --quan=0

          # Analyze the result for a unidirectional model with a 64-bit leakage budget
          python3 scripts/analyze_partition_results.py passwd --comm-type=u --quan=64

          # Analyze the result for a bidirectional model with a 0-bit leakage budget
          python3 scripts/analyze_partition_results.py passwd --comm-type=b --quan=0
      ```

6. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/passwd/output/partition_policies.txt --source examples/passwd/input/passwd.c --bc examples/passwd/input/passwd.bc --output examples/passwd/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.

## Running the Partitioned Program

The result of automatic partitioning may require manual adjustments. We provide a runnable version in `output/finally_partition`.

### 1. Compilation

First, decompress the two archives in `examples/passwd/output/finally_partition/`.

Then, compile the client and server components similarly to the 64-bit executable in the preprocessing stage.

**Compile passwd_client:**
```bash
cd examples/passwd/output/finally_partition/passwd_client/shadow-utils
./autogen.sh
make -j8
# The passwd_client executable will be generated in the src/ directory.
```

**Compile passwd_server:**
```bash
cd examples/passwd/output/finally_partition/passwd_server/shadow-utils
./autogen.sh
make -j8
# The passwd_server executable will be generated in the src/ directory.
```

### 2. Execution

Run the client and server in two separate terminals.

**Example 1: Lock password for user 'nobody'**
- **Terminal 1 (Server):**
  ```bash
  ./examples/passwd/output/finally_partition/passwd_server/shadow-utils/src/passwd_server
  ```
- **Terminal 2 (Client):**
  ```bash
  sudo ./examples/passwd/output/finally_partition/passwd_client/shadow-utils/src/passwd_client -l nobody
  ```

**Example 2: Set minimum password age for 'nobody'**
- **Terminal 1 (Server):**
  ```bash
  ./examples/passwd/output/finally_partition/passwd_server/shadow-utils/src/passwd_server
  ```
- **Terminal 2 (Client):**
  ```bash
  sudo ./examples/passwd/output/finally_partition/passwd_client/shadow-utils/src/passwd_client -n 8 nobody
  ```
