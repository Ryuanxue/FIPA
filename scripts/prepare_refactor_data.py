import os
import sys
import re
import glob
import subprocess

config_file_list = []

def parse_log_file(file_path):
    """
    解析日志文件，统计函数调用次数和每次调用的敏感性。
    
    参数:
        file_path (str): 日志文件的路径
        
    返回:
        dict: 每个函数的调用敏感性列表，格式为 {func_name: [True/False, ...]}
    """
    func_calls = {}  # 记录每个函数的调用敏感性列表
    call_stack = []  # 调用栈，存储 (func_name, call_index, has_sense)

    with open(file_path, 'r', encoding='utf-8') as file:
        for line in file:
            line = line.strip()

            # 匹配 "Function entry: func_name"
            entry_match = re.match(r'Function entry: (\w+)', line)
            if entry_match:
                func_name = entry_match.group(1)
                # 初始化函数的调用列表（如果不存在）
                if func_name not in func_calls:
                    func_calls[func_name] = []
                call_index = len(func_calls[func_name])
                # 压入调用栈，初始敏感性为 False
                call_stack.append((func_name, call_index, False))
                # 添加一个新的调用记录，初始为不敏感
                func_calls[func_name].append(False)
                continue

            # 匹配 "Function exit: func_name"
            exit_match = re.match(r'Function exit: (\w+)', line)
            if exit_match:
                func_name = exit_match.group(1)
                if call_stack and call_stack[-1][0] == func_name:
                    # 弹出调用栈
                    call_stack.pop()
                else:
                    print(f"警告: 发现不匹配的函数退出 - {func_name}")
                continue

            # 匹配 "hassenseinfo"
            if line.startswith("hassenseinfo"):
                if call_stack:
                    # 获取栈顶函数的信息
                    top_func, top_index, _ = call_stack[-1]
                    # 标记该次调用为敏感
                    func_calls[top_func][top_index] = True
                    # 更新调用栈中的敏感性标记
                    call_stack[-1] = (top_func, top_index, True)
                continue

            # 其他行（如 "nosenseinfo" 或无关行）直接忽略

    return func_calls

def print_results(func_calls):
    """
    打印每个函数的调用统计信息。
    
    参数:
        func_calls (dict): 每个函数的调用敏感性列表
    """
    for func_name, calls in func_calls.items():
        call_count = len(calls)  # 总调用次数
        sensitive_calls = sum(1 for call in calls if call)  # 敏感调用次数
        print(f"函数: {func_name}")
        print(f"  调用次数: {call_count}")
        print(f"  敏感调用次数: {sensitive_calls}")
        print(f"  每次调用的敏感性: {calls}")
        #检查敏感性是否一致
        if all(calls):
            print(f"  敏感性一致: 是")

        #检查calls同时有True和False
        if any(calls) and any(not call for call in calls):
            print(f"  敏感性一致: 否")
        
        #检查敏感性是否全为True
        if all(calls):
            print(f"  敏感性: 是")
        print()


def split_file_by_ratio(input_file, output_file):
    # 确保输出目录存在
    output_dir = os.path.dirname(output_file)
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    with open(input_file, 'r') as file:
        content = file.readlines()

    min_ratio = float('inf')
    min_ratio_content = None
    current_content = []
    found_ratio = None

    for line in content:
        current_content.append(line)

        # 检测是否包含 "Sensitive Nodes Total sz Ratio:"
        if "Sensitive Nodes Total sz Ratio:" in line:
            # 提取比例值
            try:
                ratio_str = line.split("Sensitive Nodes Total sz Ratio:")[1].strip()
                ratio = float(ratio_str)

                if ratio < min_ratio:
                    min_ratio = ratio
                    min_ratio_content = list(current_content) # Store a copy
                    found_ratio = ratio_str
                
                # 重置当前内容以开始下一个块
                current_content = []
            except (ValueError, IndexError):
                # 如果行格式不正确，则跳过
                continue
    
    # 如果找到了最小ratio的块，则将其写入文件
    if min_ratio_content is not None:
        config_file_list.append(os.path.abspath(output_file))
        with open(output_file, 'w') as output:
            output.writelines(min_ratio_content)
        # 返回找到的最小ratio值，以便调用者使用
        return f"{min_ratio:.6f}"
    
    return None


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python3 parse_log.py <project_name>")
        sys.exit(1)
    
    proname = sys.argv[1]
    
    # 新的输入目录和文件模式
    input_dir = f"examples/{proname}/output/temp"
    input_pattern = os.path.join(input_dir, "*.fc")
    
    # 新的输出文件路径
    output_file_path = f"examples/{proname}/output/temp/prepare_shard.txt"
    output_dir = os.path.dirname(output_file_path)
    os.makedirs(output_dir, exist_ok=True)

    # 获取匹配的文件列表
    input_files = glob.glob(input_pattern)
    
    if not input_files:
        print(f"警告: 在目录 '{input_dir}' 中没有找到与 '*.fc' 匹配的文件")
        sys.exit(1)
    
    print(f"找到 {len(input_files)} 个匹配的文件，将结果输出到 {output_file_path}")

    # 重定向 stdout 到输出文件

    #===============生成 shard.txt ==============
    original_stdout = sys.stdout
    with open(output_file_path, 'w') as f:
        sys.stdout = f
        
        # 处理所有匹配的文件
        for filename in input_files:
            print(f"--- Processing file: {os.path.basename(filename)} ---")
            func_calls = parse_log_file(filename)
            print_results(func_calls)
            print(f"--- End of file: {os.path.basename(filename)} ---\n")

    # 恢复原始的 stdout
    sys.stdout = original_stdout
    
    print("处理完成。")





    #===============生成 minsencode_xxx.txt ==============
    so_type = "b"  # 默认为 'b'
    for arg in sys.argv:
        if arg.startswith("--comm-type="):
            so_type = arg.split("=")[1]
        elif arg.startswith("--quan="):
            quan_str = arg.split("=")[1]

    # 根据 so_type 构建输入文件路径
    input_file = f"examples/{proname}/output/{proname}_z3_result_{so_type}_{quan_str}bit.txt"
    # 定义最终输出的配置文件路径
    output_file = f"examples/{proname}/output/minsencode_{so_type}_{quan_str}bit.txt"
    # 检查输入文件是否存在
    if not os.path.exists(input_file):
        print(f"错误：输入文件不存在: {input_file}")
        sys.exit(1)

    best_partition_ratio = split_file_by_ratio(input_file, output_file)

    if best_partition_ratio is None:
        print("错误：未能从输入文件中找到任何 ratio。")
        sys.exit(1)

    print(f"找到最小的 Ratio: {best_partition_ratio}")
    print(f"最小Ratio的配置已写入: {output_file}")




    #===============生成 global_val_use.xml ==============
    print("\n--- 开始生成 global_val_use.xml ---")
    gvu_output_file = f"examples/{proname}/output/global_val_use.xml"
    input_bc_file = f"examples/{proname}/input/{proname}.bc"

    # 确保输入 bitcode 文件存在
    if not os.path.exists(input_bc_file):
        print(f"错误：默认的 bitcode 文件不存在: {input_bc_file}")
        # 尝试查找目录中任何 .bc 文件
        bc_files = glob.glob(f"examples/{proname}/input/*.bc")
        if bc_files:
            input_bc_file = bc_files[0]
            print(f"警告：将使用找到的第一个 .bc 文件: {input_bc_file}")
        else:
            print(f"错误：在 'examples/{proname}/input/' 目录中未找到任何 .bc 文件。")
            sys.exit(1)

    

    command = [
        "opt-12",
        "-load", "lib/libpdg_b.so",
        "--global-var-usage",
        f"--gvu-output={gvu_output_file}",
        input_bc_file
    ]

    try:
        # 设置30分钟超时，并忽略解码错误
        result = subprocess.run(
            command,
            check=True,
            capture_output=True,
            text=True,
            timeout=1800,
            errors='ignore'  # 忽略解码时遇到的无效字节
        )
        print("命令执行成功。")
        # if result.stdout:
        #     print("stdout:", result.stdout)
        # if result.stderr:
        #     print("stderr:", result.stderr)
        print(f"文件已生成: {gvu_output_file}")
    except FileNotFoundError:
        # print("错误: 'opt-12' 命令未找到。请确保 LLVM 工具链已正确安装并位于您的 PATH 中。")
        sys.exit(1)
    except subprocess.TimeoutExpired:
        # print("错误: 命令执行超时 (超过30分钟)，已终止。")
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        # print("错误: 命令执行失败。")
        # # print(f"返回码: {e.returncode}")
        # if e.stdout:
        #     # print("stdout:", e.stdout)
        # if e.stderr:
        #     # print("stderr:", e.stderr)
        sys.exit(1) 

    

