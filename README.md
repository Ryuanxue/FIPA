### **项目名称**

**`Statement-level-Partitioner`**

#### **项目简介**

`Statement-level-Partitioner` 是一个创新的开源工具，旨在实现对大型单体程序的\*\*语句级（Statement-level）\*\*自动分区。与传统的函数级分区工具不同，本项目通过更细粒度的分析，能够精准地识别和提取代码片段，从而为遗留系统的微服务化改造、代码复用和性能优化提供全新的解决方案。

SecurePartitioner 是一个开源工具，用于基于量化信息流分析将单体程序分区为敏感和不敏感部分，减少潜在信息泄露。项目涉及Clang AST、FlowCheck扩展、Intel Pin、LLVM和Python自动化，支持C++程序。

-----

### **核心优势**

本项目通过独特的语句级分析，提供了以下核心优势：

  * **更细粒度的代码拆分**：打破函数边界，支持将一个函数内部的不同逻辑拆分到不同的服务中，实现更灵活的架构重构。
  * **减少代码冗余**：精准提取和复用核心代码片段，避免在不同微服务间重复复制整个函数，显著降低维护成本。
  * **高效的代码重用**：将通用的语句集合打包成可独立部署的组件，促进代码的高效复用。
  
### 关键功能：
- 提取主干语句行号。
- 量化信息流跟踪。
- 函数调用频次分析。
- 构建SQG图并求解整数规划。
- AST-based代码重构和RPC接口生成。

适用于安全研究、代码优化和隐私保护。

-----

### **安装指南**

本项目由多个独立的子模块组成，需要按顺序进行编译和安装。请确保你的系统已安装 **`g++`**、**`make`**、**`cmake`** 和 **`docker`**。llvm-12
#### Prerequisites
- OS: Linux (推荐Ubuntu 18.04+)
- Dependencies:
  - C++: Clang/LLVM 10+, CMake 3.10+
  - Python: 3.8+ (pip install -r requirements.txt)
  - Tools: Intel Pin (下载自Intel官网), FlowCheck (克隆自其仓库并扩展)
  - Docker: 用于FlowCheck 32位环境
  
安装依赖：
```bash
sudo apt update
sudo apt install clang llvm cmake docker.io
pip install -r requirements.txt  # 如clang-python绑定等
```

#### **第一步：克隆项目仓库**

```bash
git clone https://github.com/your-username/Statement-level-Partitioner.git
cd Statement-level-Partitioner
```

#### **第二步：安装“源代码语句行号范围”模块 (C++)**

```bash
cd src/statement_range
mkdir build && cd build
cmake ..
make
sudo make install  # 可选：安装到系统路径path/QuanStatementFlowPartitioner/bin
```

#### **第三步：安装“Flowchek 扩展”模块 (C & Docker)**
  3. 构建Docker环境（for FlowCheck）：
   ```bash
   cd ../docker
   docker build -t flowcheck-image .
   ```


```bash
cd ../../part2_flowchek_ext
docker build -t flowchek-image .
```

#### **第四步：安装 “Pin” 模块 (C++)**

```bash
cd src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/
make obj-intel64/funcgvrelation.so
make
```

#### **第五步：安装“SQG 与 BIP”模块 (C++)**

```bash
cd ../../part4_sqg_bip
mkdir build && cd build
cmake ..
make
```

-----




## Usage
项目分为5个步骤。运行主脚本`scripts/main_pipeline.py`（可选添加）以自动化全流程，或逐步执行。

### Step 1: Extract Statement Ranges
```bash
python3 scripts/step1_extract_statements.py --project_root /path/to/my_project --compile_db /path/to/my_project/input/compile_commands.json --output_dir /path/to/my_project/output/

python3 scripts/get_statement_linerange.py --project_root examples/chage --compile_db examples/chage/input/compile_commands.json --output_dir examples/chage/output/
```

### Step 2: Quantitative Information Flow Tracking
在Docker中运行：
1.注解，若敏感数据来源于文件，可通过设置文件的权限来指定敏感源，否则，用FC_TAINT_WORLD注解敏感源 (对shadow-utils进行跟踪时，在docker中奖/etc/passwd和/etc/shadow文件的权限修改一下，chmod o-r /etc/passwd)
1.编译（注解后）project为32为可执行文件，编译的时候一定要添加-m32 和 -g 编译选项

cd path/to/QuanStatementFlowPartitioner
docker run -it -v .:/Desktop flowcheck-image

valgrind --tool=exp-flowcheck --private-files-are-secret=yes  --project-name=shadow-utils/src --fullpath-after= --trace-secret-graph=yes --graph-file=temp.g ./examples/chage/input/chage -M 3 nobody 2>examples/chage/output/chageoutput2.txt
```bash
docker run -v $(pwd):/workspace securepartitioner-env bash -c "cd /workspace && scripts/run_flowcheck.py --input input/sample.cpp --datasets data/inputs/ --output output/quantified_statements.txt"
```

### Step 3: Collect Edge Information

pin -t /home/raoxue/Desktop/IF-driver-partition/QuanStatementFlowPartitioner/src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o wget.pinout -- ./src/pin-3.18-98332-gaebd7b1e6-gcc-linux/wget www.baidu.com

 - 动态运行，根据不同的输入得到不同的输出边信息，每次运行的时候输出的文件名不同，但都要以.pinout为后缀
su root
/home/raoxue/Desktop/IF-driver-partition/QuanStatementFlowPartitioner/src/pin-3.18-98332-gaebd7b1e6-gcc-linux/pin -t /home/raoxue/Desktop/IF-driver-partition/QuanStatementFlowPartitioner/src/pin-3.18-98332-gaebd7b1e6-gcc-linux/source/tools/ManualExamples/obj-intel64/funcgvrelation.so -o chage/output/chage.pinout2 -- ./chage/input/chage_64 -M 4 nobody


 - 后续的生成相应的的符号表以及解析多个.pinout为后缀的文件替换地址名为函数名全局变量名，以及后续的生成具体的边信息，可以在同一个脚本中进行完整
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

完整示例：见`examples/`目录。运行`python scripts/main_pipeline.py --help`获取参数。

## Examples
1. 下载`examples/input/sample.cpp`。
2. 生成`compile_commands.json`（使用bear工具）。
3. 运行全流程，检查`output/`中的分区代码。

## Contributing
欢迎PR！请fork仓库，创建branch，添加测试。Issue用于bug报告或功能请求。

## License
MIT License - 见LICENSE文件。
```


