# telnet Partitioning Project

## Project Overview

`telnet` is a client utility for remote terminal access, typically built from the `inetutils` suite. This project demonstrates fine-grained partitioning of the `telnet` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: [Describe here, e.g., network packets, configuration files, or code annotation using `FC_TAINT_WORLD()`]
- Annotation method: [File permission modification or source code annotation, as appropriate]

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the `inetutils` project with only the `telnet` client enabled.
   - Ensure dependencies are installed as required by your project.
   - Navigate to the source directory:
     ```bash
     cd examples/telnet/input/source_code/inetutils-1.9.4
     ./configure --disable-servers --disable-clients --enable-telnet
     make -j8
     ```
   - Copy the `telnet` executable from `telnet/` to `examples/telnet/input/` and rename to `telnet_64`.

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/telnet/input/source_code/inetutils-1.9.4` (if available).
   - Find the entry corresponding to `telnet.c`. Copy only the `telnet.c` entry into a new file at `examples/telnet/input/compile_commands.json`. This ensures that only the source code for `telnet.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Clean previous builds:
     ```bash
     make clean
     ```
   - Rebuild with bitcode flags:
     ```bash
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     cd telnet
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o telnet.bc telnet.o
     ```
   - After completing the above steps, `telnet.bc` will be generated in the `examples/telnet/input/source_code/inetutils-1.9.4/telnet` directory. Move `telnet.bc` to `examples/telnet/input/`.
   - Run `make clean` again.

4. **32-bit Executable (for FlowCheck)**
   - In your build configuration, add `-m32` to CFLAGS.
   - Start Docker:
     ```bash
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/telnet/input/source_code/inetutils-1.9.4
     ./configure --disable-servers --disable-clients --enable-telnet
     make -j8
     cd telnet
     ```
   - Copy the generated `telnet` binary from `examples/telnet/input/source_code/inetutils-1.9.4/telnet` to `examples/telnet/input/` and rename it to `telnet_32`.

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/step1_extract_statement_linerange.py --project_root examples/telnet --compile_db examples/telnet/input/compile_commands.json --output_dir examples/telnet/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.
   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/telnet
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/sub_global.py examples/telnet
     python3 scripts/merge_pinout_and_generate_stmt_edges.py examples/telnet
     ```
4. **Build Graph and Solve**
   ```bash
   python3 scripts/build_sqg.py --statements examples/telnet/output/telnet_statements_ranges.xml --quant examples/telnet/output/statement_quantities.xml --edges examples/telnet/output/stmt_edge_counts.txt --bc examples/telnet/input/telnet.bc --threshold_A 10 --budget_scode 0.2 --output examples/telnet/output/partition_policies.txt
   ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/telnet/output/partition_policies.txt --source examples/telnet/input/telnet.c --bc examples/telnet/input/telnet.bc --output examples/telnet/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.
