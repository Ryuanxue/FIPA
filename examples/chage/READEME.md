# chage Partitioning Project

## Project Overview

`chage` is a utility from the `shadow-utils` suite, used to change user password expiry information. This project demonstrates fine-grained partitioning of the `chage` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: [Describe here, e.g., file-based (e.g., `/etc/shadow`) or code annotation using `FC_TAINT_WORLD()`]
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
     ```
   - Copy the `chage` executable from `src/` to `examples/chage/input/` and rename to `chage_64`.

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/chage/input/source_code/shadow-utils`.
   - Find the entry corresponding to `chage.c`. Copy only the `chage.c` entry into a new file at `examples/chage/input/compile_commands.json`. This ensures that only the source code for `chage.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Clean previous builds:
     ```bash
     make clean
     ```
   - Rebuild with bitcode flags:
     ```bash
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o chage.bc chage.o
     ```
   - After completing the above steps, `chage.bc` will be generated in the `examples/chage/input/source_code/shadow-utils/src` directory. Move `chage.bc` to `examples/chage/input/`.
   - Run `make clean` again.

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
     ```
   - Copy the generated `chage` binary from `examples/chage/input/source_code/shadow-utils/src` to `examples/chage/input/` and rename it to `chage_32`.

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/step1_extract_statement_linerange.py --project_root examples/chage --compile_db examples/chage/input/compile_commands.json --output_dir examples/chage/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.
   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/chage
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/sub_global.py examples/chage
     python3 scripts/merge_pinout_and_generate_stmt_edges.py examples/chage
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
