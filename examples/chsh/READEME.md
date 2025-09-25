# chsh Partitioning Project

## Project Overview

`chsh` is a utility from the `shadow-utils` suite, used to change a user's login shell. This project demonstrates fine-grained partitioning of the `chsh` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: `/etc/passwd` and `/etc/shadow`
- Annotation method: Modify the permissions of `/etc/passwd` and `/etc/shadow` to restrict access.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the entire `shadow-utils` project (not just `chsh`).
   - Ensure dependencies are installed:
     ```bash
     sudo apt-get install autoconf autopoint gettext
     sudo apt install -y libsemanage-dev libselinux1-dev
     ```
   - Navigate to the source directory:
     ```bash
     cd examples/chsh/input/source_code/shadow-utils
     ./autogen.sh
     bear make -j8
     cd src
     mv chsh ../../../chsh_64
     ```

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/chsh/input/source_code/shadow-utils`.
   - Find the entry corresponding to `chsh.c`.Copy only the `chsh.c` entry into a new file at `examples/chsh/input/compile_commands.json`.This ensures that only the source code for `chsh.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Rebuild with bitcode flags:
     ```bash
     make clean
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o chsh.bc chsh.o
     mv chsh.bc ../../../chsh.bc
     cd ../
     make clean
     ```
    
4. **32-bit Executable (for FlowCheck)**
   - In `autogen.sh`, add `-m32` to CFLAGS.
   - Start Docker:
     ```bash
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/chsh/input/source_code/shadow-utils
     ./autogen.sh
     make -j8
     cd src
     mv chsh ../../../chsh_32
     exit
     ```

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/chsh --compile_db examples/chsh/input/compile_commands.json --output_dir examples/chsh/output/
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
        valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=examples/chsh --fullpath-after= --folding-level=0 --trace-secret-graph=yes ./examples/chsh/input/chsh_32 -s /bin/sh nobody 2>examples/chsh/output/temp/chshoutput1.fc
        ```
        For interactive execution (e.g., changing root's shell), run:
        ```bash
        valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=examples/chsh --fullpath-after= --folding-level=0 --trace-secret-graph=yes ./examples/chsh/input/chsh_32 2>examples/chsh/output/temp/chshoutput2.fc
        ```
        During interactive mode, you will be prompted to enter the desired shell for root, such as `/bin/bash`, `/bin/sh`, or `/bin/dash`.

   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/chsh
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files:
        ```bash
        cd FIPA
        su root
        ```
        Run Pin for different test cases:
        ```bash
        src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/chsh/output/temp/chsh1.pinout -- ./examples/chsh/input/chsh_64 -s /bin/bash nobody

        src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/chsh/output/temp/chsh2.pinout -- ./examples/chsh/input/chsh_64
        ```
        For interactive execution, you will be prompted to enter the desired shell for root, such as `/bin/bash`, `/bin/sh`, or `/bin/dash`.

   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/chsh
     ```
4. **Build Graph and Solve for Partitioning**

   This step uses an automated script to construct the graph and find an optimal partitioning solution. You can run the solver with different communication models by specifying the `--so-type` parameter. The script will generate result files (e.g., `chsh_z3_result_u.txt`) in the `examples/chsh/output/` directory.

   -   **To solve using the unidirectional model (`u`):**
       ```bash
       python3 scripts/based_qg_bi_praming.py chsh min-quan=0 max-code-sz=0.1 --so-type=u
       ```

   -   **To solve using the bidirectional model (`b`):**
       ```bash
       python3 scripts/based_qg_bi_praming.py chsh min-quan=0 max-code-sz=0.1 --so-type=b
       ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/chsh/output/partition_policies.txt --source examples/chsh/input/chsh.c --bc examples/chsh/input/chsh.bc --output examples/chsh/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.

## Running the Partitioned Program

The result of automatic partitioning may require manual adjustments. We provide a runnable version in `output/finally_partition`.

### 1. Compilation

First, decompress the two archives in `examples/chsh/output/finally_partition/`.

Then, compile the client and server components similarly to the 64-bit executable in the preprocessing stage.

**Compile chsh_client:**
```bash
cd examples/chsh/output/finally_partition/chsh_client/shadow-utils
./autogen.sh
make -j8
# The chsh_client executable will be generated in the src/ directory.
```

**Compile chsh_server:**
```bash
cd examples/chsh/output/finally_partition/chsh_server/shadow-utils
./autogen.sh
make -j8
# The chsh_server executable will be generated in the src/ directory.
```

### 2. Execution

Run the client and server in two separate terminals.

**Example 1: Change shell for user 'nobody'**
- **Terminal 1 (Server):**
  ```bash
  ./examples/chsh/output/finally_partition/chsh_server/shadow-utils/src/chsh_server
  ```
- **Terminal 2 (Client):**
  ```bash
  su root
  ./examples/chsh/output/finally_partition/chsh_client/shadow-utils/src/chsh_client -s /bin/sh nobody
  ```

**Example 2: Change shell for 'root' (interactive)**
- **Terminal 1 (Server):**
  ```bash
  ./examples/chsh/output/finally_partition/chsh_server/shadow-utils/src/chsh_server
  ```
- **Terminal 2 (Client):**
  ```bash
  su root
  ./examples/chsh/output/finally_partition/chsh_client/shadow-utils/src/chsh_client
  # You will be prompted to enter the desired shell for root.
  ```
