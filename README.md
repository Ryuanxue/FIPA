# FIPA: Fine-Grained Information-Flow-Driven Program Partitioning

## Project Overview

 **FIPA** is an innovative open-source tool designed for automatic statement-level partitioning of large monolithic programs. Unlike traditional function-level partitioning tools, this project provides finer-grained analysis to accurately identify and extract code fragments, offering new solutions for microservice transformation, code reuse, and performance optimization of legacy systems.

 **FIPA** is an open-source tool that partitions monolithic programs into sensitive and non-sensitive parts based on quantitative information flow analysis, reducing potential information leakage. The project involves Clang AST, FlowCheck extensions, Intel Pin, LLVM, and Python automation, supporting C++ programs.


## **Key Advantages**

This project offers unique statement-level analysis with the following core benefits:

  * **Finer-grained code splitting**: Breaks function boundaries, allowing different logic within a single function to be split into different services for more flexible architecture refactoring.
  * **Reduced code redundancy**: Precisely extracts and reuses core code fragments, avoiding duplication of entire functions across microservices and significantly lowering maintenance costs.
  * **Efficient code reuse**: Packages common statement sets into independently deployable components, promoting efficient code reuse.
  
## Key Features:
- Extracts backbone statement line numbers.
- Quantitative information flow tracking.
- Function call frequency analysis.
- Builds SQG graphs and solves integer programming.
- AST-based code refactoring and RPC interface generation.

Applicable for security research, code optimization, and privacy protection.


## **Installation Guide**

This project consists of several independent submodules that must be compiled and installed in order. 
- Sliver has been tested on Ubuntu 20.04.
- Install llvm-12,libclang-12-dev, llvm-12-dev, libtinyxml2-dev,zlib1g, zlib1g-dev, libpugixml-dev:
```bash
sudo apt install llvm-12 libclang-12-dev llvm-12-dev libtinyxml2-dev zlib1g zlib1g-dev libpugixml-dev
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

#### **Step 3: Install "Flowchek Extension" Module (C & Docker)**

```bash
cd src/Flowcheckdocker
docker build -t flowchek-image .
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
make
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
- If the sensitive source is not a file, annotate it directly in the source code using the FlowCheck API, such as `FC_TAINT_WORLD()`. For example, in `telnet`, sensitive sources are annotated in code using this API.

 
**Example: Preprocessing for `chage` (from `shadow-utils`)**

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

## Usage

Below we use `chage` as an example. For other partitioned projects (a total of ten), see the README.md files under each project in the `examples` directory for details on annotation, compilation, usage, and partitioning. [See examples/README.md](./examples/) for links to each subproject's documentation.

The usage pipeline consists of 5 steps:

### Step 1: Extract Statement Ranges
```bash
python3 scripts/step1_extract_statements.py --project_root /path/to/my_project --compile_db /path/to/my_project/input/compile_commands.json --output_dir /path/to/my_project/output/

python3 scripts/get_statement_linerange.py --project_root examples/chage --compile_db examples/chage/input/compile_commands.json --output_dir examples/chage/output/
```

### Step 2: Quantitative Information Flow Tracking

This step involves two main parts:
1. **FlowCheck information flow tracking in Docker**
2. **Pin dynamic instrumentation and edge collection**

#### FlowCheck Tracking (chage example)
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
- Merge multiple `.fc` trace files and map the quantitative information to statements using a script. Inputs: all `.fc` files and `statement_range.xml`. Output: `statement_quantities.xml` (recommended name).

### Step 3: Collect Edge Information

This step also involves two main parts:
1. **Run Pin with different inputs to generate `.pinout` files in the temp directory**
2. **Merge all `.pinout` files to produce the final edge information file**

- Example Pin usage:
  ```bash
  pin -t /path/to/funcgvrelation.so -o examples/chage/output/temp/chage.pinout1 -- ./examples/chage/input/chage_64 -M 3 nobody
  pin -t /path/to/funcgvrelation.so -o examples/chage/output/temp/chage.pinout2 -- ./examples/chage/input/chage_64 -l nobody
  ```
- Merge all `.pinout` files using a script to produce `edge_info_output.txt` (recommended name) in the output directory.

### Step 4: Build Graph and Solve
```bash
python scripts/build_sqg.py --statements output/statements_ranges.xml --quant output/quantified_statements.txt --edges output/edge_weights.txt --bc input/sample.bc --threshold_A 10 --budget_scode 0.2 --output output/partition_policies.txt
```

### Step 5: Code Refactoring
```bash
python scripts/refactor_code.py --policy output/partition_policies.txt --source input/sample.cpp --bc input/sample.bc --output output/refactored/
```

See complete examples and project-specific instructions in the `examples/` directory. Each subproject's README.md provides details for annotation, compilation, and usage.

## Examples
1. Download `examples/input/sample.cpp`.
2. Generate `compile_commands.json` (using bear tool).
3. Run the full pipeline and check partitioned code in `output/`.

## Contributing
PRs are welcome! Please fork the repo, create a branch, and add tests. Use Issues for bug reports or feature requests.

## License
MIT License - see LICENSE file.


