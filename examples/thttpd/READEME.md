# thttpd Partitioning Project

## Project Overview

`thttpd` is a lightweight web server. This project demonstrates fine-grained partitioning of the `thttpd` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: [Describe here, e.g., network packets, configuration files, or code annotation using `FC_TAINT_WORLD()`]
- Annotation method: [File permission modification or source code annotation, as appropriate]

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the `thttpd` project.
   - Ensure dependencies are installed as required by your project.
   - Navigate to the source directory:
     ```bash
     cd examples/thttpd/input/source_code/thttpd-2.27
     ./configure
     make -j8
     ```
   - Copy the `thttpd` executable from the build directory to `examples/thttpd/input/` and rename to `thttpd_64`.

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/thttpd/input/source_code/thttpd-2.27` (if available).
   - Find the entry corresponding to `thttpd.c`. Copy only the `thttpd.c` entry into a new file at `examples/thttpd/input/compile_commands.json`. This ensures that only the source code for `thttpd.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Clean previous builds:
     ```bash
     make clean
     ```
   - Rebuild with bitcode flags:
     ```bash
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o thttpd.bc thttpd.o
     ```
   - After completing the above steps, `thttpd.bc` will be generated in the `examples/thttpd/input/source_code/thttpd-2.27/src` directory. Move `thttpd.bc` to `examples/thttpd/input/`.
   - Run `make clean` again.

4. **32-bit Executable (for FlowCheck)**
   - In your build configuration, add `-m32` to CFLAGS.
   - Start Docker:
     ```bash
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/thttpd/input/source_code/thttpd-2.27
     ./configure
     make -j8
     cd src
     ```
   - Copy the generated `thttpd` binary from `examples/thttpd/input/source_code/thttpd-2.27/src` to `examples/thttpd/input/` and rename it to `thttpd_32`.

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/step1_extract_statement_linerange.py --project_root examples/thttpd --compile_db examples/thttpd/input/compile_commands.json --output_dir examples/thttpd/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.
   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/thttpd
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/sub_global.py examples/thttpd
     python3 scripts/merge_pinout_and_generate_stmt_edges.py examples/thttpd
     ```
4. **Build Graph and Solve**
   ```bash
   python3 scripts/build_sqg.py --statements examples/thttpd/output/thttpd_statements_ranges.xml --quant examples/thttpd/output/statement_quantities.xml --edges examples/thttpd/output/stmt_edge_counts.txt --bc examples/thttpd/input/thttpd.bc --threshold_A 10 --budget_scode 0.2 --output examples/thttpd/output/partition_policies.txt
   ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/thttpd/output/partition_policies.txt --source examples/thttpd/input/thttpd.c --bc examples/thttpd/input/thttpd.bc --output examples/thttpd/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.
