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
class CallGraphIO:
    def __init__(self, filename):
        self.filename = filename
        self.file = open(filename, 'r')
    
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
        sense_data = defaultdict(set)  # 键是 (filepath, func_name, line_number)，值是敏感地址集合
        current_call_stack = deque()  # 当前函数调用栈
        function_sensitive = defaultdict(lambda: {'current': set(), 'inherited': set()})  # 每个函数的当前和继承敏感地址
        line_numbers = defaultdict(list)  # 跟踪每个函数的行号顺序

        # 编译正则表达式
        call_stack_re = re.compile(r"call_stack_funname:\[(.*?)\]")
        # 新的正则表达式来匹配 "sensitive:" 和 "non-sensitive:" 行
        sense_info_re = re.compile(r"^(sensitive|non-sensitive):(.+):(\d+):(.+)$")
        function_entry_re = re.compile(r"Function entry: (\w+)")

        
        # lines = self.file.readlines()

        # for line in lines:
        while not self.end_of_stream():
            line = self.get_next_line()
            line=line.strip()
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

                # 从调用栈中获取当前函数名
                if not current_call_stack:
                    continue
                func_name = current_call_stack[-1]

                # 路径处理
                filepath = os.path.abspath(filepath)

                # 记录行号顺序
                line_numbers[func_name].append(int(line_number))

                # 更新敏感地址
                if is_sensitive:
                    # 添加新敏感地址
                    function_sensitive[func_name]['current'].update(addresses)
                else:
                    # 移除非敏感地址
                    function_sensitive[func_name]['current'].difference_update(addresses)

                # 为当前行和所有后续行更新 sense_data
                filepath = filepath.replace("/Desktop", "/Desktop/IF-driver-partition/FIPA")
                # print(filepath)
            
                sorted_lines = sorted(line_numbers[func_name])
                current_idx = sorted_lines.index(int(line_number))

                for ln in sorted_lines[current_idx:]:  # 从当前行到最后一行
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
        if file_path and "/Desktop" in file_path:  # 确保只处理包含 /Desktop 的路径
            start_idx = file_path.find("/Desktop")
            if start_idx != -1:
                file_path = file_path[start_idx:]  # 只保留从 /Desktop 开始的部分
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



#================================================================

"""# 使用示例"""
# statement_xml_file = 'statement_test.xml'
# sense_file = 'sense_test.txt'

global_fun_senaddress_map = {}  # 全局敏感地址存储

"""#将statement.xml文件和sense文件列表的获取改为从命令行参数中获取"""
import sys
proname=sys.argv[1]
output_file = "../examples/"+proname+"/output/temp/"+proname+"_quanfile_refactor.txt"
# statement_xml_file=sys.argv[1] #statement.xml文件，可以是其他的文件名
statement_xml_file="../examples/"+proname+"/output/"+proname+"_statements_ranges.xml"
# sense_file=sys.argv[2:]  #sense文件列表，可能有多个sense文件，用空格隔开,我不确定是否需要用空格隔开，现在我先测试文件列表中只有一个文件的情况
#从sys.argv[2]开始的所有参数都是sense文件
sensefile_list=sys.argv[2:]
sense_file=[]
# for sensefile in sensefile_list:
#     sense_file=["../partitioned_software/"+proname+"/2_flowcheck_result/"+sensefile]
# sense_file=["../partitioned_software/"+proname+"/2_flowcheck_result/"+proname+"output1.txt","../partitioned_software/"+proname+"/2_flowcheck_result/"+proname+"output2.txt"]

sense_file=["../examples/"+proname+"/output/temp/"+"senseAddress.txt"]

""" # 解析 XML 和敏感信息文件"""
statements = parse_statement_xml(statement_xml_file)
# sense_data=extract_and_print_sensitive_addresses(sense_file)

"""# 聚合多次运行的结果
# 若有多个sense_file,也就是说sense_file是一个列表
# 从第二个开始，将其敏感地址集合与前面的敏感地址集合合并，pre_sense_data和sense_data的key相同，则将其value合并，若key不同，
# 则将其value作为一个新的key-value对加入pre_sense_data中"""

import time
start_time = time.time()
print("开始打开文件")
call_graph_io = CallGraphIO(sense_file[0])
pre_sense_data=call_graph_io.extract_and_print_sensitive_addresses()
end_time = time.time()
print(f"打开文件的时间: {end_time - start_time:.8f} 秒")

# 打印pre_sense_data
# for key,value in pre_sense_data.items():
#     print(key)
#     print(value)

for i in range(1,len(sense_file)):
    call_graph_io = CallGraphIO(sense_file[i])
    sense_data=call_graph_io.extract_and_print_sensitive_addresses()
    for key,value in sense_data.items():
        if key in pre_sense_data.keys():
            # print("innnnnnn")
            pre_sense_data[key].update(value)
        else:
            pre_sense_data[key]=value
# sense_data=pre_sense_data

# print()
print("合并后")
for key,value in pre_sense_data.items():
    print(key)
    print(value)


"""合并程序多次执行的敏感地址集合"""

"""# 匹配 statements 和敏感信息"""
results = match_statements_with_sense_info(statements, pre_sense_data)


# statements_dict=flush_decl_return_quan(statements, results)

# # 将 statements_dict 转换回原始的 results 格式
# results = []
# for key, statements in statements_dict.items():
#     for stmt_desc, total_sensitivity, start_line, end_line in statements:
#         results.append(f"{key} + {stmt_desc} + {total_sensitivity}")

# 写入结果
write_results_to_file(results, output_file)

print("# 以下是信息量不为0的语句：")
for result in results:
    parts = result.split(" + ")
    if parts[-1] != "0":
        print(result)

#打印混合函数的语句及其信息量
# for key, statements in statements_dict.items():
#     quan_set = set()
#     for stmt_desc, total_sensitivity, start_line, end_line in statements:
#         if total_sensitivity == 0:
#             quan_set.add("0")
#         else:
#             quan_set.add("1")

#         if len(quan_set) == 2:
#             break

#     if len(quan_set) == 2:
#         for stmt_desc, total_sensitivity, start_line, end_line in statements:
#             print(f"{key} + {stmt_desc} + {total_sensitivity}")


        


# for result in results:
#     result = result.split(" + ")
#     if result[-1] != "0":
#         print(result)

