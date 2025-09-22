# mavlink-client Partitioning Project

## Project Overview

`mavlink-client` is a client utility for interacting with MAVLink-based systems. This project demonstrates fine-grained partitioning of the `mavlink-client` program using the FIPA toolchain.

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
     cd examples/mavlink-client/input/source_code/mavlink-client
     make -j8
     ```
   - Copy the `mavlink-client` executable from `src/` to `examples/mavlink-client/input/` and rename to `mavlink-client_64`.

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/mavlink-client/input/source_code/mavlink-client` (if available).
   - Find the entry corresponding to `mavlink-client.c`. Copy only the `mavlink-client.c` entry into a new file at `examples/mavlink-client/input/compile_commands.json`. This ensures that only the source code for `mavlink-client.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Clean previous builds:
     ```bash
     make clean
     ```
   - Rebuild with bitcode flags:
     ```bash
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o mavlink-client.bc mavlink-client.o
     ```
   - After completing the above steps, `mavlink-client.bc` will be generated in the `examples/mavlink-client/input/source_code/mavlink-client/src` directory. Move `mavlink-client.bc` to `examples/mavlink-client/input/`.
   - Run `make clean` again.

4. **32-bit Executable (for FlowCheck)**
   - In your build configuration, add `-m32` to CFLAGS.
   - Start Docker:
     ```bash
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/mavlink-client/input/source_code/mavlink-client
     make -j8
     cd src
     ```
   - Copy the generated `mavlink-client` binary from `examples/mavlink-client/input/source_code/mavlink-client/src` to `examples/mavlink-client/input/` and rename it to `mavlink-client_32`.

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/step1_extract_statement_linerange.py --project_root examples/mavlink-client --compile_db examples/mavlink-client/input/compile_commands.json --output_dir examples/mavlink-client/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.
   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/mavlink-client
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/sub_global.py examples/mavlink-client
     python3 scripts/merge_pinout_and_generate_stmt_edges.py examples/mavlink-client
     ```
4. **Build Graph and Solve**
   ```bash
   python3 scripts/build_sqg.py --statements examples/mavlink-client/output/mavlink-client_statements_ranges.xml --quant examples/mavlink-client/output/statement_quantities.xml --edges examples/mavlink-client/output/stmt_edge_counts.txt --bc examples/mavlink-client/input/mavlink-client.bc --threshold_A 10 --budget_scode 0.2 --output examples/mavlink-client/output/partition_policies.txt
   ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/mavlink-client/output/partition_policies.txt --source examples/mavlink-client/input/mavlink-client.c --bc examples/mavlink-client/input/mavlink-client.bc --output examples/mavlink-client/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.
