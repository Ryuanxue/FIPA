# FIPA: Fine-Grained Information-Flow-Driven Program Partitioning

## **Installation Guide**

This project consists of several independent submodules that must be compiled and installed in order. 
- Sliver has been tested on Ubuntu 20.04.
- Install dependencies including LLVM-12, various development libraries, and `rpcbind`:
```bash
sudo apt install llvm-12 libclang-12-dev llvm-12-dev libtinyxml2-dev zlib1g zlib1g-dev libpugixml-dev rpcbind
```

- To install Docker, please refer to: [https://docs.docker.com/engine/install/ubuntu/](https://docs.docker.com/engine/install/ubuntu/)

#### **Step 1: Clone the Project Repository**

```bash
git clone git@github.com:Ryuanxue/FIPA.git
cd FIPA
```

#### **Step 2: Install "Statement Ranges " Module (C++)**

```bash
cd src/statement_range
mkdir build && cd build
cmake ..
make
sudo make install  
```

#### **Step 3: Install "flowcheck Extension" Module (C & Docker)**

```bash
cd src/Flowcheckdocker
docker build -t flowcheck-image .
```

#### **Step 4: Install "Pin" Module (C++)**

```bash
cd src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/
make obj-intel64/funcgvrelation.so
make
```

#### **Step 5: Install "SQG and BIP" Module (C++)**

```bash
cd src/program-dependence-graph-main
mkdir build && cd build
cmake ..
make -j8
make install
```
## Project Preprocessing (Before Usage)

Before running the partitioning workflow, you must preprocess your target project as follows:

1. **Compile a 64-bit executable** for your project.
2. **Generate a compilation database** (`compile_commands.json`) using tools like `bear`.
3. **Build a complete LLVM bitcode file** (`.bc`) for the project.
4. **Annotate the source code** for sensitive data sources as needed (see annotation methods below).
5. **Use Docker to compile the annotated source into a 32-bit binary** (add `-m32 -g` flags during compilation).


**Sensitive Source Annotation Methods:**
- If the sensitive source is a file (e.g., `/etc/passwd` in `chage`), you do not need to annotate the source code. Instead, modify the file's permissions in Docker (e.g., `chmod o-r /etc/passwd`).
- If the sensitive source is not a file, annotate it directly in the source code using the FlowCheck API, such as `FC_TAINT_WORD()`. For example, in `telnet`, sensitive sources are annotated in code using this API.

 
**Example: Preprocessing for `chage` (from `shadow-utils`)**

Before compiling `shadow-utils`, make sure the following dependencies are installed:
```bash
sudo apt-get install autoconf autopoint gettext
sudo apt install -y libsemanage-dev libselinux1-dev
```

`chage` is an application within the `shadow-utils` project. To preprocess it for partitioning, follow these steps:

1. **Compile the entire `shadow-utils` project (not just chage):**
   - Navigate to the source directory:
     ```bash
     cd examples/chage/input/source_code/shadow-utils
     ```
   - Edit `autogen.sh` to add debug flags (`-g -O0`) to CFLAGS.
   - Run the build scripts:
     ```bash
     ./autogen.sh
     bear make -j8
     ```
   - After compilation, you will have a 64-bit binary (in `src/`) and a compilation database (`compile_commands.json`).
   - Copy the `chage` executable to `examples/chage/input/` and rename it to `chage_64`.

2. **Prepare a minimal compilation database for chage:**
   - Manually create `examples/chage/input/compile_commands.json`.
   - Open the generated compilation database, find the entry for `chage.c`, and copy it into your new minimal database.

3. **Build the LLVM bitcode file (`.bc`):**
   - Clean previous builds:
     ```bash
     make clean
     ```
   - Rebuild with bitcode flags:
     ```bash
     make CC=clang CFLAGS+="-flto -g -O0 -fno-discard-value-names -fembed-bitcode" -j8 # If you encounter linker errors, you can safely ignore them
     cd src
     clang -Wl,--plugin-opt=emit-llvm -flto -g -O0 -fno-discard-value-names -fembed-bitcode -o chage.bc chage.o
     ```
   - Move `chage.bc` to `examples/chage/input/`.
   - Run `make clean` again to clean up.

4. **Compile the 32-bit binary in Docker:**
   - In `autogen.sh`, add `-m32` to CFLAGS.
   - Start Docker:
     ```bash
     cd FIPA
     docker run -it -v .:/Desktop flowcheck-image
     ```
   - Inside Docker, navigate to the source directory and rebuild:
     ```bash
     cd examples/chage/input/source_code/shadow-utils
     ./autogen.sh
     make -j8
     cd src
     ```
   - Copy the generated `chage` binary to `examples/chage/input/chage_32`.

This process ensures you have all required artifacts (64-bit binary, 32-bit binary, compilation database, bitcode file) for the partitioning workflow. Adjust paths and filenames as needed for your own project.

## Subproject Documentation

The `examples` directory contains partitioning workflows for ten representative applications. Each subproject has its own README.md with detailed instructions for annotation, compilation, usage, and partitioning. Click the links below for step-by-step guides:

- [chage](./examples/chage/READEME.md): Partitioning the password expiry utility from shadow-utils.
- [chsh](./examples/chsh/READEME.md): Partitioning the shell change utility from shadow-utils.
- [passwd](./examples/passwd/READEME.md): Partitioning the password change utility from shadow-utils.
- [useradd](./examples/useradd/READEME.md): Partitioning the user account creation utility from shadow-utils.
- [mavlink-client](./examples/mavlink-client/READEME.md): Partitioning a MAVLink protocol client.
- [mavlink-server](./examples/mavlink-server/READEME.md): Partitioning a MAVLink protocol server.
- [telnet](./examples/telnet/READEME.md): Partitioning the telnet client from inetutils.
- [thttpd](./examples/thttpd/READEME.md): Partitioning the lightweight web server thttpd.
- [wget](./examples/wget/READEME.md): Partitioning the network download utility wget.
- [nginx](./examples/nginx/READEME.md): Partitioning the high-performance web server nginx.

Each README covers:
- Sensitive source annotation strategy
- Preprocessing and compilation steps
- Partitioning workflow and usage pipeline
- Project-specific notes and troubleshooting

Refer to these documents for reproducible partitioning of each application.

## Usage

Below we use `chage` as an example. For other partitioned projects, see the subproject documentation above for details on annotation, compilation, usage, and partitioning.

The usage pipeline consists of 5 steps:

### Step 1: Extract Statement Ranges
```bash
python3 scripts/extract_statement_linerange.py --project_root /path/to/my_project --compile_db /path/to/my_project/input/compile_commands.json --output_dir /path/to/my_project/output/

python3 scripts/extract_statement_linerange.py --project_root examples/chage --compile_db examples/chage/input/compile_commands.json --output_dir examples/chage/output/
```

### Step 2: Quantitative Information Flow Tracking

- Start Docker:
  ```bash
  cd FIPA
  docker run -it -v .:/Desktop flowcheck-image
  ```
- Modify permissions for sensitive files (if applicable):
  ```bash
  chmod o-r /etc/passwd /etc/shadow
  ```
- Run FlowCheck with different inputs to generate trace files:
  ```bash
  valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=examples/chage --fullpath-after= --folding-level=0 --trace-secret-graph=yes ./examples/chage/input/chage_32 -M 3 nobody 2>examples/chage/output/temp/chageoutput1.fc
  
  valgrind --tool=exp-flowcheck --private-files-are-secret=yes --project-name=examples/chage --fullpath-after= --folding-level=0 --trace-secret-graph=yes ./examples/chage/input/chage_32 -l nobody 2>examples/chage/output/temp/chageoutput2.fc
  ```
- Merge FlowCheck Trace  and Map Quantitative Information to Statements. 
  ```bash
  python3 scripts/merge_fc_and_map_statements.py <project_dir>
  ```
  For example:
  ```bash
  python3 scripts/merge_fc_and_map_statements.py examples/chage
  ```

### Step 3: Collect Edge Information

-  Pin usage:
  ```bash
  cd FIPA
  su root

  src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/chage/output/temp/chage1.pinout -- ./examples/chage/input/chage_64 -M 3 nobody

  src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o examples/chage/output/temp/chage2.pinout -- ./examples/chage/input/chage_64 -l nobody
  ```
- Merge all .pinout files and generate statement-level edge execution counts using the following script:
  ```bash
  python3 scripts/merge_pinout_and_generate_stmt_edge.py <project_dir>
  ```
  For example:
  ```bash
  python3 scripts/merge_pinout_and_generate_stmt_edge.py examples/chage
  ```

### Step 4: Build Graph and Solve for Partitioning

This step constructs a Security Quantified Graph (SQG) and uses a Z3-based solver to find an optimal binary integer programming (BIP) solution for partitioning. The provided `based_qg_bi_praming.py` script automates this process by iteratively invoking the solver with varying code size budgets (`max-code-sz`) until a feasible solution is found.

The script supports different partitioning strategies via the `--comm-type` parameter, which loads different solver logic:
-   `--comm-type=u`: Utilizes a unidirectional communication model, where information is only allowed to flow from the non-sensitive to the sensitive domain.
-   `--comm-type=b`: Utilizes a bidirectional communication model.

**Command Format:**
```bash
python3 scripts/based_qg_bi_praming.py <project_name> min-quan=<value> max-code-sz=<value> --comm-type=<type>
```

**Example for `chage`:**

You can run the solver with different strategies. The script will generate result files (e.g., `chage_z3_result_u.txt`) in the project's `output` directory.

-   To solve using the **unidirectional** model (`u`):
    ```bash
    python3 scripts/based_qg_bi_praming.py chage min-quan=0 max-code-sz=0.1 --comm-type=u
    ```

-   To solve using the **bidirectional** model (`b`):
    ```bash
    python3 scripts/based_qg_bi_praming.py chage min-quan=0 max-code-sz=0.1 --comm-type=b
    ```

### Step 5: Prepare Data for Refactoring

This step processes the raw output from the solver to generate a final, consolidated policy file that will guide the code refactoring process. The `prepare_refactor_data.py` script performs several key actions:

- Selects the optimal solution from multiple candidates generated by the solver.
- Analyzes read/write dependencies for global variables across partitions.
- Identifies functions that may be shared or need to be duplicated.

**Command:**
```bash
python3 scripts/prepare_refactor_data.py --project_root examples/chage --output_dir examples/chage/output/ --min_quan 0 --max_code_sz 0.1
```

**Optional: Analyze Partitioning Statistics**

  After preparing the refactoring data, you can run an additional script to view detailed statistics about the chosen partition. This is not a required step for the main workflow but is useful for analysis. It provides information such as the number of functions in each partition and the percentage of sensitive code.

  Command Format:
  ```bash
  python3 scripts/analyze_partition_results.py <project_name> --comm-type <u|b> --quan <value>
  ```

### Step 6: Code Refactoring
   
   This step generates the final partitioned code with RPC communication interfaces. The example below uses unidirectional communication with 0-bit leakage. For other configurations, modify the `--comm-type` and `--quan` parameters accordingly.
   
   ```bash
   python3 scripts/refactor_function_add_rpc.py chage --comm-type=u --quan=0
   ```
   
   **Parameter Options:**
   - `--comm-type`: Communication model (`u` for unidirectional, `b` for bidirectional)
   - `--quan`: Leakage budget in bits (e.g., `0`, `64`)

See complete examples and project-specific instructions in the `examples/` directory. Each subproject's README.md provides details for annotation, compilation, and usage.

## Examples

See the subproject documentation above for complete, real-world partitioning workflows. Each subproject's README.md provides details for annotation, compilation, and usage.

## Contributing
PRs are welcome! Please fork the repo, create a branch, and add tests. Use Issues for bug reports or feature requests.

## License
MIT License - see LICENSE file.


