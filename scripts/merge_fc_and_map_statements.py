#python
"""
分析txt文件中的数据获得所有的敏感内存地址
txt中的一些内容如下所示
call_stack_funname:[ dl_main  main ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 ]
senseinfo:0x108842: main (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:55),0xFED375B0
call_stack_funname:[ dl_main  main ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 ]
senseinfo:0x108842: main (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:55),0xFED375B1
call_stack_funname:[ dl_main  main ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 ]
senseinfo:0x108842: main (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:55),0xFED375B2
call_stack_funname:[ dl_main  main ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 ]
senseinfo:0x108842: main (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:55),0xFED375B3
...
call_stack_funname:[ dl_main  main fun2 ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 0xFED37578 ]
senseinfo:0x1086DE: fun2 (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:17),0x10A010
call_stack_funname:[ dl_main  main fun2 ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 0xFED37578 ]
senseinfo:0x1086DE: fun2 (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:17),0x10A011
call_stack_funname:[ dl_main  main fun2 ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 0xFED37578 ]
senseinfo:0x1086DE: fun2 (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:17),0x10A012
call_stack_funname:[ dl_main  main fun2 ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 0xFED37578 ]
senseinfo:0x1086DE: fun2 (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:17),0x10A013
call_stack_funname:[ dl_main  main fun2 ]
...
call_stack_funname:[ dl_main  main ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 ]
senseinfo:0x10888E: main (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:61),0x4A1D028
call_stack_funname:[ dl_main  main ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 ]
senseinfo:0x10888E: main (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:61),0x4A1D029
call_stack_funname:[ dl_main  main ]
call_stack_ebp:[ 0xFED37578 0x0 0xFED375E8 ]
senseinfo:0x10888E: main (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:61),0x4A1D02A
...
每一条senseinfo之前都会有call_stack_funname记录当前函数的调用栈
计算从程序运行到当前为止所有的敏感地址，
敏感地址是enseinfo:0x10888E: main (/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp/test.c:61),0x4A1D02A字符串中末尾的十六进制值
计算方式是将所有的敏感地址取并集
但是当一个函数已经消亡之后计算敏感地址的时候不应该将此函数中的敏感地址计算在内，比如函数的调用栈为call_stack_funname:[ dl_main  main fun2 ]的时候，
fun2中当前位置的敏感地址集合即为程序程序运行到当前位置所有的senseinfo中提取的敏感地址，包括main,fun2.
而当fun2运行结束，需要推栈，又回到了main中的指令继续执行
此时计算main中当前指令的敏感地址的时候不应该把fun2中指令上的敏感地址计算在内

"""
import re
import xml.etree.ElementTree as ET
from collections import defaultdict
import pdb
from collections import defaultdict, deque

import os


def flush_info_dict(file_handle, data_dict):
    """将字典内容写入文件并清空字典"""
    if data_dict:
        # file_handle.write("--- Accumulated Sense Info ---\n")
        # 保持插入顺序进行输出
        for key, values in data_dict.items():
            location, sense_type = key  # 解包元组键
            # 将value列表转换为逗号分隔的字符串
            values_str = ", ".join(values)
            outputstr= sense_type + ":" + location + ":" + values_str + "\n"
            file_handle.write(outputstr)
            # file_handle.write(f"Location: {location}\n")
            # file_handle.write(f"  Type: {sense_type}\n")
            # file_handle.write(f"  Addresses: [{values_str}]\n")
        # file_handle.write("--- End Sense Info ---\n")
        data_dict.clear()

def process_log_file(input_file, output_file):
    """
    处理flowcheck的输出日志文件。

    Args:
        input_file (str): 输入日志文件的路径。
        output_file (str): 输出文件的路径。
    """
    last_call_stack = ""
    info_dict = {}

    # 正则表达式用于解析 nosenseinfo 和 hassenseinfo 行
    # 匹配括号内的文件路径和行号作为key，以及最后的地址作为value
    info_pattern = re.compile(r'.*?\((/.*?:\d+)\),(0x[0-9a-fA-F]+)')

    try:
        with open(input_file, 'r') as rf, open(output_file, 'w') as wf:
            for line in rf:
                stripped_line = line.strip()

                if stripped_line.startswith("call_stack_funname:"):
                    # 提取当前的调用栈信息
                    current_call_stack = stripped_line
                    if current_call_stack != last_call_stack:
                        wf.write(line)
                        last_call_stack = current_call_stack

                elif stripped_line.startswith(("nosenseinfo:", "hassenseinfo:")):
                    match = info_pattern.match(stripped_line)
                    if match:
                        location = match.group(1)
                        value = match.group(2)
                        # 根据行前缀判断敏感类型
                        sense_type = "sensitive" if stripped_line.startswith("hassenseinfo:") else "non-sensitive"
                        
                        # 创建一个包含位置和敏感类型的元组作为键
                        key = (location, sense_type)
                        
                        if key not in info_dict:
                            info_dict[key] = []
                        
                        # 只有当value不存在时才添加
                        if value not in info_dict[key]:
                            info_dict[key].append(value)

                elif stripped_line.startswith(("Function entry:", "Function exit:")):
                    # 遇到函数进入或退出时，先将累积的字典内容写入文件
                    flush_info_dict(wf, info_dict)
                    # 然后将当前行（函数进入/退出信息）写入文件
                    wf.write(line)

            # 处理文件末尾可能剩余的字典内容
            flush_info_dict(wf, info_dict)

        print(f"文件处理完成，结果已保存到 {output_file}")

    except FileNotFoundError:
        print(f"错误: 输入文件 {input_file} 未找到。")
    except Exception as e:
        print(f"处理文件时发生错误: {e}")

class CallGraphIO:
    def __init__(self, filename, project_dir=None):
        self.filename = filename
        self.file = open(filename, 'r')
        self.project_dir = project_dir  # 新增：项目绝对路径
    
    def get_next_line(self):
        # 读取文件中的下一行并去除尾部换行符
        return self.file.readline().strip()

    def end_of_stream(self):
        # 检查是否已到文件末尾
        return self.file.tell() == os.fstat(self.file.fileno()).st_size

    def get_caller_callee_pair(self):
        line = self.get_next_line()
        if line.startswith('hassenseinfo:'):
            print(line)

        # 如果读取到文件末尾，返回空对
        
        return line
    
    def process_file(self):
        while not self.end_of_stream():
            line = self.get_caller_callee_pair()
            # print(line)

        self.file.close()

    



    def extract_and_print_sensitive_addresses(self):
        sense_data = defaultdict(set)
        current_call_stack = deque()
        function_sensitive = defaultdict(lambda: {'current': set(), 'inherited': set()})
        line_numbers = defaultdict(list)

        call_stack_re = re.compile(r"call_stack_funname:\[(.*?)\]")
        sense_info_re = re.compile(r"^(sensitive|non-sensitive):(.+):(\d+):(.+)$")
        function_entry_re = re.compile(r"Function entry: (\w+)")

        # 获取项目绝对路径和标准化前缀
        if self.project_dir:
            abs_project_dir = os.path.abspath(self.project_dir)
            # 只保留项目名部分
            project_name = os.path.basename(abs_project_dir)
            desktop_prefix = f"/Desktop/{project_name}"

        while not self.end_of_stream():
            line = self.get_next_line().strip()
            # 解析调用栈
            call_stack_match = call_stack_re.match(line)
            if call_stack_match:
                new_stack = call_stack_match.group(1).split()
                # 检查栈是否弹出（函数返回）
                while current_call_stack and current_call_stack[-1] not in new_stack:
                    exited_func = current_call_stack.pop()
                    # 函数返回时，清空其当前敏感地址，但保留继承的地址
                    function_sensitive[exited_func]['current'].clear()
                current_call_stack = deque(new_stack)
                continue

            # 解析函数进入
            entry_match = function_entry_re.match(line)
            if entry_match:
                callee = entry_match.group(1)
                if current_call_stack:
                    caller = current_call_stack[-1]
                    # 被调用函数继承调用者的当前敏感地址
                    function_sensitive[callee]['inherited'] = function_sensitive[caller]['current'].copy()
                    function_sensitive[callee]['current'] = function_sensitive[callee]['inherited'].copy()  # 初始化当前状态
                current_call_stack.append(callee)
                continue

            # 解析敏感地址或非敏感地址
            sense_match = sense_info_re.match(line)
            if sense_match:
                sense_type_str = sense_match.group(1)
                filepath = sense_match.group(2)
                line_number = sense_match.group(3)
                addresses_str = sense_match.group(4)
                is_sensitive = (sense_type_str == "sensitive")
                addresses = {addr.strip() for addr in addresses_str.split(',')}
                if not current_call_stack:
                    continue
                func_name = current_call_stack[-1]
                # 路径标准化处理
                
                if self.project_dir and filepath.startswith(f"/Desktop/{project_dir}"):
                    filepath = filepath.replace(f"/Desktop/{project_dir}", abs_project_dir, 1)
                else:
                    filepath = os.path.abspath(filepath)
                # 记录行号顺序
                # print(filepath)
                line_numbers[func_name].append(int(line_number))
                # 更新敏感地址
                if is_sensitive:
                    function_sensitive[func_name]['current'].update(addresses)
                else:
                    function_sensitive[func_name]['current'].difference_update(addresses)
                # 为当前行和所有后续行更新 sense_data
                sorted_lines = sorted(line_numbers[func_name])
                current_idx = sorted_lines.index(int(line_number))
                for ln in sorted_lines[current_idx:]:
                    key = (filepath, func_name, str(ln))
                    sense_data[key].update(function_sensitive[func_name]['current'])
        return sense_data



def parse_statement_xml(xml_file):
    tree = ET.parse(xml_file)
    root = tree.getroot()

    statements = []
    for function in root.findall('function'):
        func_name = function.get('name')
        file_path = function.get('dir')
        # if file_path and "/Desktop" in file_path:  # 确保只处理包含 /Desktop 的路径
        #     start_idx = file_path.find("/Desktop")
        #     if start_idx != -1:
        #         file_path = file_path[start_idx:]  # 只保留从 /Desktop 开始的部分
        for stmt in function.findall('stmt'):
            stmt_desc = stmt.get('dec')
            statements.append((file_path, func_name, stmt_desc))
    
    return statements



def match_statements_with_sense_info(statements, sense_data):
    results = []
    # 已处理的语句集合，用于去重
    processed = set()
    
    for file_path, func_name, stmt_desc in statements:
        # 创建唯一标识符
        stmt_identifier = f"{file_path} + {func_name} + {stmt_desc}"
        
        # 如果已处理过此语句，则跳过
        if stmt_identifier in processed:
            continue
            
        stmt_type, start_line, end_line = stmt_desc.split(":")
        start_line, end_line = int(start_line), int(end_line)

        if "/Desktop" not in file_path:
            continue

        total_addresses = set()
        # 只累积从第一个敏感地址行开始的后续行
        first_sensitive_line = float('inf')
        for key in sense_data.keys():
            if key[1] == func_name and key[0].startswith(file_path) and int(key[2]) >= start_line and int(key[2]) <= end_line:
                first_sensitive_line = min(first_sensitive_line, int(key[2]))

        for line in range(max(start_line, first_sensitive_line) if first_sensitive_line != float('inf') else start_line, end_line + 1):
            key = (file_path, func_name, str(line))
            if key in sense_data:
                total_addresses.update(sense_data[key])

        total_sensitivity = len(total_addresses)
        result_str = f"{file_path} + {func_name} + {stmt_desc} + {total_sensitivity}"
        results.append(result_str)
        processed.add(stmt_identifier)

    return results


def flush_decl_return_quan(statements, results):
    #将statements中的元素组织为字典，key是文件路径+函数名，value是行号范围的语句（列表），语句行为IfStmt:151:156，其中151是开始行号，156是结束行号，按照结束行号对语句进行排序
    # 初始化 statements_dict
    statements_dict = defaultdict(list)

    # 处理 statements
    for file_path, func_name, stmt_desc in statements:
        if file_path and "/Desktop" in file_path:  # 只处理文件路径中包含/Desktop的文件
            start_line, end_line = map(int, stmt_desc.split(":")[1:])
            key = f"{file_path}+{func_name}"
            statements_dict[key].append([stmt_desc, start_line, end_line])  # 使用列表而不是元组

    # 按照结束行号对语句进行排序
    for key in statements_dict:
        statements_dict[key].sort(key=lambda x: x[2])

    # 将 results 转换为字典以便快速查找
    results_dict = {}
    for result in results:
        parts = result.split(" + ")
        key = f"{parts[0]}+{parts[1]}"
        stmt_desc = parts[2]
        total_sensitivity = parts[3]
        results_dict[(key, stmt_desc)] = int(total_sensitivity)

    # 将 results 中的敏感信息量添加到 statements_dict 中对应的语句中
    for key, statements in statements_dict.items():
        for i, (stmt_desc, start_line, end_line) in enumerate(statements):
            if (key, stmt_desc) in results_dict:
                total_sensitivity = results_dict[(key, stmt_desc)]
                statements_dict[key][i] = [stmt_desc, total_sensitivity, start_line, end_line]  # 使用列表而不是元组

    # 遍历 statements_dict，更新敏感信息量
    for key, statements in statements_dict.items():
        count = 0
        if len(statements) > 1 and statements[-1][1] == 0:
            statements[-1][1] = statements[-2][1]
        
        for statement in statements:
            senquantity = int(statement[1])
            
            if count == 0 and senquantity != 0:
                break
            
            if senquantity != 0:
                for i in range(count):
                    statements[i][1] = senquantity
            
            sttype = statement[0].split(":")[0]
            if sttype != "DeclStmt":
                break
            count += 1
    return statements_dict

def write_results_to_file(results, output_file):
    """将匹配结果写入到文件中。"""
    with open(output_file, 'w') as f:
        for result in results:
            f.write(result + "\n")




if __name__ == "__main__":
    import sys
    import glob
    import time
    import os
    
    if len(sys.argv) < 2:
        print("Usage: python get_all_senseAddr_use_progate_for_refactor_output.py <project_dir>")
        sys.exit(1)


    
    
    
    
    
    project_dir = sys.argv[1]
    temp_dir = os.path.join(project_dir, "output", "temp")

    fc_files = glob.glob(os.path.join(temp_dir, "*.fc"))
    if not fc_files:
        print(f"No .fc files found in {temp_dir}")
        sys.exit(1)
    for fc_file in fc_files:
        output_file = fc_file + "_new"
        print(f"Processing {fc_file} -> {output_file}")
        process_log_file(fc_file, output_file)
    print("All .fc files processed.")



    sense_file = sorted(glob.glob(os.path.join(temp_dir, "*.fc_new")))
    if not sense_file:
        print(f"No .fc_new files found in {temp_dir}")
        sys.exit(1)
    
    proname = os.path.basename(project_dir.rstrip('/'))
    output_file = os.path.join(project_dir, "output", f"{proname}_quanfile.txt")
    statement_xml_file = os.path.join(project_dir, "output", f"{proname}_statements_ranges.xml")
    
    statements = parse_statement_xml(statement_xml_file)
    print("开始打开文件")
    start_time = time.time()
    call_graph_io = CallGraphIO(sense_file[0], project_dir)
    pre_sense_data = call_graph_io.extract_and_print_sensitive_addresses()
    end_time = time.time()
    print(f"打开文件的时间: {end_time - start_time:.8f} 秒")
    for i in range(1, len(sense_file)):
        call_graph_io = CallGraphIO(sense_file[i], project_dir)
        sense_data = call_graph_io.extract_and_print_sensitive_addresses()
        for key, value in sense_data.items():
            if key in pre_sense_data:
                pre_sense_data[key].update(value)
            else:
                pre_sense_data[key] = value


    # print("敏感信息结果")
    # for key, value in pre_sense_data.items():
    #     print(f"{key} : {value}")
    # print(statements)

    print("合并后")
    results = match_statements_with_sense_info(statements, pre_sense_data)
    write_results_to_file(results, output_file)
    print("# 以下是信息量不为0的语句：")
    for result in results:
        parts = result.split(" + ")
        if parts[-1] != "0":
            print(result)

