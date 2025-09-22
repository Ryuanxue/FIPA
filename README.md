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
- Install llvm-12,clang-12-dev, llvm-12-dev:
```bash
sudo apt install llvm-12 clang-12-dev llvm-12-dev
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
sudo make install  # Optional: install to system path path/QuanStatementFlowPartitioner/bin
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
## Usage
The project consists of 5 steps. Run the main script `scripts/main_pipeline.py` (optional) to automate the entire process, or execute each step manually.

### Step 1: Extract Statement Ranges
```bash
python3 scripts/step1_extract_statements.py --project_root /path/to/my_project --compile_db /path/to/my_project/input/compile_commands.json --output_dir /path/to/my_project/output/

python3 scripts/get_statement_linerange.py --project_root examples/chage --compile_db examples/chage/input/compile_commands.json --output_dir examples/chage/output/
```

### Step 2: Quantitative Information Flow Tracking
Run in Docker:
1. Note: If sensitive data comes from files, set file permissions to specify sensitive sources; otherwise, use FC_TAINT_WORLD annotation (for shadow-utils, modify permissions of /etc/passwd and /etc/shadow in Docker: chmod o-r /etc/passwd)
1. Compile (after annotation) the project as a 32-bit executable, be sure to add -m32 and -g options

cd path/to/QuanStatementFlowPartitioner
docker run -it -v .:/Desktop flowcheck-image

valgrind --tool=exp-flowcheck --private-files-are-secret=yes  --project-name=shadow-utils/src --fullpath-after= --trace-secret-graph=yes --graph-file=temp.g ./examples/chage/input/chage -M 3 nobody 2>examples/chage/output/chageoutput2.txt
```
docker run -v $(pwd):/workspace securepartitioner-env bash -c "cd /workspace && scripts/run_flowcheck.py --input input/sample.cpp --datasets data/inputs/ --output output/quantified_statements.txt"
```

### Step 3: Collect Edge Information

pin -t /home/raoxue/Desktop/IF-driver-partition/QuanStatementFlowPartitioner/src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o wget.pinout -- ./src/pin-3.18-98332-gaebd7b1e6-gcc-linux/wget www.baidu.com

 - Run dynamically, different inputs produce different output edge info; output filenames should end with .pinout
su root
/home/raoxue/Desktop/IF-driver-partition/QuanStatementFlowPartitioner/src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t /home/raoxue/Desktop/IF-driver-partition/QuanStatementFlowPartitioner/src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o chage/output/chage.pinout2 -- ./chage/input/chage_64 -M 4 nobody

 - Subsequent steps: generate symbol tables, parse multiple .pinout files, replace address names with function/global variable names, and generate edge info; can be done in a single script
readelf --dyn-syms  src/pin-3.18-98332-gaebd7b1e6-gcc-linux/wget >wget.symtbl

python3 sub_global.py wget

```bash
python scripts/collect_pin_data.py --pin_path /path/to/pin --input input/sample.exe --datasets data/inputs/ --output output/edge_weights.txt
```

### Step 4: Build Graph and Solve
```bash
python scripts/build_sqg.py --statements output/statements_ranges.xml --quant output/quantified_statements.txt --edges output/edge_weights.txt --bc input/sample.bc --threshold_A 10 --budget_scode 0.2 --output output/partition_policies.txt
```

### Step 5: Code Refactoring
```bash
python scripts/refactor_code.py --policy output/partition_policies.txt --source input/sample.cpp --bc input/sample.bc --output output/refactored/
```

See complete examples in the `examples/` directory. Run `python scripts/main_pipeline.py --help` for parameters.

## Examples
1. Download `examples/input/sample.cpp`.
2. Generate `compile_commands.json` (using bear tool).
3. Run the full pipeline and check partitioned code in `output/`.

## Contributing
PRs are welcome! Please fork the repo, create a branch, and add tests. Use Issues for bug reports or feature requests.

## License
MIT License - see LICENSE file.


