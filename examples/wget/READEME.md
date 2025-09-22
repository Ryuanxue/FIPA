# wget Partitioning Project

## Project Overview

`wget` is a widely used utility for non-interactive network downloads. This project demonstrates fine-grained partitioning of the `wget` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: [Describe here, e.g., network packets, configuration files, or code annotation using `FC_TAINT_WORLD()`]
- Annotation method: [File permission modification or source code annotation, as appropriate]

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the `wget` project (configure may be optional).
   - Ensure dependencies are installed as required by your project.
   - Navigate to the source directory:
     ```bash
     cd examples/wget/input/source_code/wget-1.8
     ./configure   # (sometimes not needed)
     make clean
     make -j8
     ```
   - Copy the `wget` executable from the build directory to `examples/wget/input/` and rename to `wget_64`.

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/wget/input/source_code/wget-1.8` (if available).
   - Find the entry corresponding to `wget.c`. Copy only the `wget.c` entry into a new file at `examples/wget/input/compile_commands.json`. This ensures that only the source code for `wget.c` is partitioned in subsequent steps.

3. **LLVM Bitcode File (.bc)**
   - Clean previous builds:
     ```bash
     make clean
     ```
   - Rebuild with bitcode flags:
     ```bash
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o wget.bc wget.o
     ```
   - After completing the above steps, `wget.bc` will be generated in the `examples/wget/input/source_code/wget-1.8/src` directory. Move `wget.bc` to `examples/wget/input/`.
   - Run `make clean` again.

4. **32-bit Executable (for FlowCheck)**
   - In your build configuration, add `-m32` to CFLAGS and include FlowCheck headers:
     ```bash
     cd examples/wget/input/source_code/wget-1.8
     ./configure   # (sometimes not needed)
     make clean
     make CPPFLAGS+="-I/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/include" CFLAGS+="-g -O0 -m32"
     ```
   - Copy the generated `wget` binary from `examples/wget/input/source_code/wget-1.8/src` to `examples/wget/input/` and rename it to `wget_32`.

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/step1_extract_statement_linerange.py --project_root examples/wget --compile_db examples/wget/input/compile_commands.json --output_dir examples/wget/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.
   - Merge traces and map quantitative info to statements:
     ```bash
     python3 scripts/merge_fc_and_map_statements.py examples/wget
     ```
3. **Collect Edge Information**
   - Run Pin with different inputs to generate `.pinout` files.
   - Replace addresses with symbol names and merge edges:
     ```bash
     python3 scripts/sub_global.py examples/wget
     python3 scripts/merge_pinout_and_generate_stmt_edges.py examples/wget
     ```
4. **Build Graph and Solve**
   ```bash
   python3 scripts/build_sqg.py --statements examples/wget/output/wget_statements_ranges.xml --quant examples/wget/output/statement_quantities.xml --edges examples/wget/output/stmt_edge_counts.txt --bc examples/wget/input/wget.bc --threshold_A 10 --budget_scode 0.2 --output examples/wget/output/partition_policies.txt
   ```
5. **Code Refactoring**
   ```bash
   python3 scripts/refactor_code.py --policy examples/wget/output/partition_policies.txt --source examples/wget/input/wget.c --bc examples/wget/input/wget.bc --output examples/wget/output/refactored/
   ```

## Notes
- For details on each step, refer to the main FIPA README.md in the project root.
- Adjust paths and filenames as needed for your own environment.
