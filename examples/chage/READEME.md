# chage Partitioning Project

## Project Overview

`chage` is a utility from the `shadow-utils` suite, used to change user password expiry information. This project demonstrates fine-grained partitioning of the `chage` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: [Describe here, e.g., file-based (e.g., `/etc/shadow`) or code annotation using `FC_TAINT_WORD()`]
- Annotation method: [File permission modification or source code annotation, as appropriate]

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the entire `shadow-utils` project (not just `chage`).
   - Ensure dependencies are installed:
     ```bash
     sudo apt-get install autoconf autopoint gettext
     sudo apt install -y libsemanage-dev libselinux1-dev
     ```
   - Navigate to the source directory:
        ```bash
        cd examples/chage/input/source_code/shadow-utils
        ./autogen.sh
        bear make -j8
        cd src
        mv chage ../../../chage_64
        ```

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/chage/input/source_code/shadow-utils`.
   - Find the entry corresponding to `chage.c`. Copy only the `chage.c` entry into a new file at `examples/chage/input/compile_commands.json`. This ensures that only the source code for `chage.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Rebuild with bitcode flags:
     ```bash
     make clean
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o chage.bc chage.o
     mv chage.bc ../../../chage.bc
     cd ..
     make clean
     ```

4. **32-bit Executable (for FlowCheck)**
   - In `autogen.sh`, add `-m32` to CFLAGS.
   - Start Docker:
     ```bash
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/chage/input/source_code/shadow-utils
     ./autogen.sh
     make -j8
     cd src
     mv chage ../../../chage_32
     exit
     ```

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/chage --compile_db examples/chage/input/compile_commands.json --output_dir examples/chage/output/
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
     valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=examples/chage --fullpath-after= --folding-level=0 --trace-secret-graph=yes ./examples/chage/input/chage_32 -M 3 nobody 2>examples/chage/output/temp/chageoutput1.fc

     valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=examples/chage --fullpath-after= --folding-level=0 --trace-secret-graph=yes ./examples/chage/input/chage_32 -l nobody 2>examples/chage/output/temp/chageoutput2.fc
     ```
   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/chage
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files:
     ```bash
     cd FIPA
     su root
     ```
     Run Pin for different test cases:
     ```bash
     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/chage/output/temp/chage1.pinout -- ./examples/chage/input/chage_64 -M 3 nobody

     src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/chage/output/temp/chage2.pinout -- ./examples/chage/input/chage_64 -l nobody
     ```
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/sub_global.py examples/chage
     python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/chage
     ```
4. **Build Graph and Solve**
   ```bash
   python3 scripts/build_sqg.py --statements examples/chage/output/chage_statements_ranges.xml --quant examples/chage/output/statement_quantities.xml --edges examples/chage/output/stmt_edge_counts.txt --bc examples/chage/input/chage.bc --threshold_A 10 --budget_scode 0.2 --output examples/chage/output/partition_policies.txt
   ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/chage/output/partition_policies.txt --source examples/chage/input/chage.c --bc examples/chage/input/chage.bc --output examples/chage/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.
