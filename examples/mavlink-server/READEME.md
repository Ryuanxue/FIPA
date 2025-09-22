# mavlink-server Partitioning Project

## Project Overview

`mavlink-server` is a server utility for interacting with MAVLink-based systems. This project demonstrates fine-grained partitioning of the `mavlink-server` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: [Describe here, e.g., network packets, configuration files, or code annotation using `FC_TAINT_WORLD()`]
- Annotation method: [File permission modification or source code annotation, as appropriate]

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the entire project (no need for autogen.sh or bear).
   - Ensure dependencies are installed as required by your project.
   - Navigate to the source directory:
     ```bash
     cd examples/mavlink-server/input/source_code/mavlink-server
     make -j8
     ```
   - Copy the `mavlink-server` executable from `src/` to `examples/mavlink-server/input/` and rename to `mavlink-server_64`.

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/mavlink-server/input/source_code/mavlink-server` (if available).
   - Find the entry corresponding to `mavlink-server.c`. Copy only the `mavlink-server.c` entry into a new file at `examples/mavlink-server/input/compile_commands.json`. This ensures that only the source code for `mavlink-server.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Clean previous builds:
     ```bash
     make clean
     ```
   - Rebuild with bitcode flags:
     ```bash
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o mavlink-server.bc mavlink-server.o
     ```
   - After completing the above steps, `mavlink-server.bc` will be generated in the `examples/mavlink-server/input/source_code/mavlink-server/src` directory. Move `mavlink-server.bc` to `examples/mavlink-server/input/`.
   - Run `make clean` again.

4. **32-bit Executable (for FlowCheck)**
   - In your build configuration, add `-m32` to CFLAGS.
   - Start Docker:
     ```bash
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/mavlink-server/input/source_code/mavlink-server
     make -j8
     cd src
     ```
   - Copy the generated `mavlink-server` binary from `examples/mavlink-server/input/source_code/mavlink-server/src` to `examples/mavlink-server/input/` and rename it to `mavlink-server_32`.

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/step1_extract_statement_linerange.py --project_root examples/mavlink-server --compile_db examples/mavlink-server/input/compile_commands.json --output_dir examples/mavlink-server/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.
   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/mavlink-server
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/sub_global.py examples/mavlink-server
     python3 scripts/merge_pinout_and_generate_stmt_edges.py examples/mavlink-server
     ```
4. **Build Graph and Solve**
   ```bash
   python3 scripts/build_sqg.py --statements examples/mavlink-server/output/mavlink-server_statements_ranges.xml --quant examples/mavlink-server/output/statement_quantities.xml --edges examples/mavlink-server/output/stmt_edge_counts.txt --bc examples/mavlink-server/input/mavlink-server.bc --threshold_A 10 --budget_scode 0.2 --output examples/mavlink-server/output/partition_policies.txt
   ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/mavlink-server/output/partition_policies.txt --source examples/mavlink-server/input/mavlink-server.c --bc examples/mavlink-server/input/mavlink-server.bc --output examples/mavlink-server/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.
