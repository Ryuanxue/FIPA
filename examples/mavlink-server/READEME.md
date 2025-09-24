# mavlink-server Partitioning Project

## Project Overview

`mavlink-server` is a utility designed to receive heartbeat messages sent from MAVLink clients. 

## Annotation Strategy

- Sensitive sources: Data from internet.
- Annotation method: Annotated using FlowCheck's API, `FC_TAINT_WORLD`. For details, refer to the patch file `diff.patch` in the `source_code` directory.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the entire project (no need for autogen.sh or bear).
   - Ensure dependencies are installed as required by your project.
   - Navigate to the source directory:
     ```bash
     cd examples/mavlink-server/input/source_code/mavlink-server
     bear make mavlink_server
     mv mavlink_server ../../mavlink_server_64
     mv compile_commands.json ../../
     make clean
     ```
2. **Compilation Database**
    - The shell commands above have already moved the compilation database (`compile_commands.json`) to the `mavlink-server/input` directory.

3. **LLVM Bitcode File (.ll)**
   - Rebuild with bitcode flags:
     ```bash
     
     clang -g -S -emit-llvm -O0 -Igenerated/include  -g -O0 -fno-discard-value-names  mavlink_server.c
     mv mavlink_server.ll ../../
     ```

4. **32-bit Executable (for FlowCheck)**
   - Start Docker:
     ```bash
     cd examples/mavlink-server/input/source_code
     cp -r  mavlink-server mavlink-server_back
     patch -d mavlink-server -p1 < diff.patch
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/mavlink-server/input/source_code/mavlink-server
     make mavlink_server_32
     mv mavlink_server ../../mavlink_server_32
     make clean
     exit
     ```

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
