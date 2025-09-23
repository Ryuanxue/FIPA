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
        valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=examples/passwd --fullpath-after= --folding-level=0 --trace-secret-graph=yes ./examples/passwd/input/passwd_32 -l nobody 2>examples/passwd/output/temp/passwdoutput1.fc
        
        valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=examples/passwd --fullpath-after= --folding-level=0 --trace-secret-graph=yes ./examples/passwd/input/passwd_32 -n 8 nobody 2>examples/passwd/output/temp/passwdoutput2.fc
        ```

   - Merge traces and map quantitative info to statements:
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
4. **Build Graph and Solve**
   ```bash
   python3 scripts/build_sqg.py --statements examples/passwd/output/passwd_statements_ranges.xml --quant examples/passwd/output/statement_quantities.xml --edges examples/passwd/output/stmt_edge_counts.txt --bc examples/passwd/input/passwd.bc --threshold_A 10 --budget_scode 0.2 --output examples/passwd/output/partition_policies.txt
   ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/passwd/output/partition_policies.txt --source examples/passwd/input/passwd.c --bc examples/passwd/input/passwd.bc --output examples/passwd/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.
