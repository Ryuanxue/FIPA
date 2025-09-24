# telnet Partitioning Project

## Project Overview

`telnet` is a client utility for remote terminal access, typically built from the `inetutils` suite. This project demonstrates fine-grained partitioning of the `telnet` program using the FIPA toolchain.

## Annotation Strategy

- Sensitive sources: Data from internet.
- Annotation method: Annotate sensitive sources in the code using `FC_TAINT_WORLD`. For details, refer to the patch file `diff.patch` under `source_code`.

## Preprocessing Steps

Before running the partitioning workflow, generate the following artifacts:

1. **64-bit Executable**
   - Compile the `inetutils` project with only the `telnet` client enabled.
   - Ensure all required dependencies are installed.
   - Navigate to the source directory:
     ```bash
     cd examples/telnet/input/source_code/inetutils-1.9.4
     ./configure --disable-servers --disable-clients --enable-telnet
     # Edit the Makefile: locate the CFLAGS option and change `-O2` to `-O0` for debugging.
     bear make -j8
     cd telnet
     mv telnet ../../../telnet_64
     ```

2. **Compilation Database**
   - Locate the full `compile_commands.json` generated in `examples/telnet/input/source_code/inetutils-1.9.4` (if available).
   - In `examples/telnet/input`, create a new `compile_commands.json` file.
   - From the original compilation database, copy all entries where the `"directory"` field contains the substring `examples/telnet/input/source_code/inetutils-1.9.4/telnet` (i.e., any entry whose source file is under the `telnet` subdirectory). Paste these entries into the new file. This ensures only the `telnet` source files are included for partitioning.

3. **LLVM Bitcode File (.bc)**
   - Rebuild with bitcode flags:
     ```bash
     make clean
     ./configure --disable-servers --disable-clients --enable-telnet CC=clang CFLAGS="-flto -g -O0 -fno-discard-value-names -fembed-bitcode " LDFLAGS="-flto -fuse-ld=lld"
     make -j8 V=1
     ```
   - Locate the  link command (e.g., starting with `clang ... -o telnet ...`).
   - Modify the command by adding `-Wl,--plugin-opt=emit-llvm`, changing `-o telnet` to `-o telnet.bc`, so it looks like:
     ```bash
     cd telnet
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -flto -fuse-ld=lld -o telnet.bc authenc.o commands.o main.o network.o ring.o sys_bsd.o telnet.o terminal.o tn3270.o utilities.o ../libtelnet/libtelnet.a ../libinetutils/libinetutils.a ../lib/libgnu.a -ltermcap -lcrypt
     mv telnet.bc ../../../telnet.bc
     cd ..
     make clean
     ```

4. **32-bit Executable (for FlowCheck)**
   - Start Docker:
     ```bash
     cd FIPA/examples/telnet/input/source_code
     cp -r inetutils-1.9.4 inetutils-1.9.4_back
     patch -d inetutils-1.9.4 -p1 < diff.patch
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     cd examples/telnet/input/source_code/inetutils-1.9.4
     ./configure --disable-servers --disable-clients --enable-telnet CFLAGS="-g -O0 -m32  -I/flowcheck/include/"
     make -j8
     cd telnet
     mv telnet ../../../telnet_32
     exit
     ```

## Partitioning Workflow Steps

1. **Extract Statement Ranges**
   ```bash
   python3 scripts/extract_statement_linerange.py --project_root examples/telnet --compile_db examples/telnet/input/compile_commands.json --output_dir examples/telnet/output/
   ```
2. **Quantitative Information Flow Tracking**
   - Run FlowCheck in Docker with different inputs to generate `.fc` trace files.
    docker run  -it --network=host -v .:/Desktop/ flowcheck-image
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
