# chsh Partitioning Project

## Project Overview

`chsh` is a utility from the `shadow-utils` suite, used to change a user's login shell. This project demonstrates fine-grained partitioning of the `chsh` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: [Describe here, e.g., file-based (e.g., `/etc/passwd`) or code annotation using `FC_TAINT_WORLD()`]
- Annotation method: [File permission modification or source code annotation, as appropriate]

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
     ```
   - Copy the `chsh` executable from `src/` to `examples/chsh/input/` and rename to `chsh_64`.

2. **Compilation Database**
   - Manually create `examples/chsh/input/compile_commands.json`.
   - Copy the relevant entry for `chsh.c` from the generated database.

3. **LLVM Bitcode File (.bc)**
   - Clean previous builds:
     ```bash
     make clean
     ```
   - Rebuild with bitcode flags:
     ```bash
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o chsh.bc chsh.o
     ```
   - Move `chsh.bc` to `examples/chsh/input/`.
   - Run `make clean` again.

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
     ```
   - Copy the generated `chsh` binary to `examples/chsh/input/chsh_32`.

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/step1_extract_statement_linerange.py --project_root examples/chsh --compile_db examples/chsh/input/compile_commands.json --output_dir examples/chsh/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.
   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/chsh
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/sub_global.py examples/chsh
     python3 scripts/merge_pinout_and_generate_stmt_edges.py examples/chsh
     ```
4. **Build Graph and Solve**
   ```bash
   python3 scripts/build_sqg.py --statements examples/chsh/output/chsh_statements_ranges.xml --quant examples/chsh/output/statement_quantities.xml --edges examples/chsh/output/stmt_edge_counts.txt --bc examples/chsh/input/chsh.bc --threshold_A 10 --budget_scode 0.2 --output examples/chsh/output/partition_policies.txt
   ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/chsh/output/partition_policies.txt --source examples/chsh/input/chsh.c --bc examples/chsh/input/chsh.bc --output examples/chsh/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.
