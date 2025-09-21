##解析project_statements_ranges.xml,提取所有的文件名，文件格式如下
"""
<function name="__uint64_identity" dir="/usr/include/x86_64-linux-gnu/bits/uintn-identity.h">
        <stmt dec="ReturnStmt:47:47" />
    </function>
    <function name="usage" dir="/home/raoxue/Desktop/IF-driver-partition/QuanStatementFlowPartitioner/examples/shadow-utils/src/chage.c">
        <stmt dec="DeclStmt:143:143" />
        <stmt dec="CStyleCastExpr:144:148" />
        <stmt dec="CStyleCastExpr:149:149" />
        <stmt dec="CStyleCastExpr:150:150" />
        <stmt dec="CStyleCastExpr:151:151" />
        <stmt dec="CStyleCastExpr:152:153" />
        <stmt dec="CStyleCastExpr:154:154" />
        <stmt dec="CStyleCastExpr:155:156" />
        <stmt dec="CStyleCastExpr:157:158" />
        <stmt dec="CStyleCastExpr:159:159" />
        <stmt dec="CStyleCastExpr:160:160" />
        <stmt dec="CStyleCastExpr:161:161" />
        <stmt dec="CallExpr:162:162" />
    </function>
    <function name="date_to_str" dir="/home/raoxue/Desktop/IF-driver-partition/QuanStatementFlowPartitioner/examples/shadow-utils/src/chage.c">
        <stmt dec="DeclStmt:167:167" />
        <stmt dec="BinaryOperator:169:169" />
        <stmt dec="CStyleCastExpr:171:171" />
    </function>
"""
#解析标签<function>，提取函数名和dir,如果dir的开头为/usr/include/，则忽略该函数，否则将函数名存到一个集合中


""" 解析project_statements_ranges，提取函数名 ，dir,并解析函数之下的每个stmt标签，提取dec属性，形成 dir + func_name + stmt_dec 的列表 """

import xml.etree.ElementTree as ET
func_names = set()
fun_statement_dict = dict()
def extract_function_names(xml_path):
    tree = ET.parse(xml_path)
    root = tree.getroot()
    
    for func in root.findall('function'):
        name = func.get('name')
        dir_path = func.get('dir')
        if dir_path and not dir_path.startswith('/usr/include/'):
            func_names.add(name)
    return func_names


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

        # 如果读取到文件末尾，返回空对
        if not line:
            return "", "", ""

        arr = line.split(' ')
        if len(arr)!=3:
            return "", "", ""
        # print (arr)
        # assert len(arr) == 3, "find ill formatted line!"

        if arr[0].startswith('.') or arr[1].startswith('.'):
            return "-", "", ""

        if '@' in arr[0] or '@' in arr[1]:
            return "-", "", ""

        # 去掉全局变量标志符
        if arr[0].startswith('$'):
            arr[0] = arr[0][1:]

        if arr[1].startswith('$'):
            arr[1] = arr[1][1:]

        return arr[0], arr[1], arr[2]
    
    def process_file(self):
        edge_dict = dict()
        edge_line_dict = dict()
        while not self.end_of_stream():
            caller, callee, line_num = self.get_caller_callee_pair()
            if caller != "" and callee != "" and caller in func_names and callee in func_names and caller!=callee:
                key = (caller, callee)
                key_line = (caller, callee, line_num)
                if key in edge_dict:
                    edge_dict[key] += 1
                else:
                    edge_dict[key] = 1
                if key_line in edge_line_dict:
                    edge_line_dict[key_line] += 1
                else:
                    edge_line_dict[key_line] = 1
                # print(f"Caller: {caller}, Callee: {callee}, Count: {edge_dict[key]}")
        self.file.close()
        # 可选：返回字典
        return edge_dict, edge_line_dict

def count_function_executions(edge_dict):
    # 统计每个函数的入度（被调用次数），main为入口，执行次数为1
    exec_count = dict()
    callees = set()
    callers = set()
    for (caller, callee), count in edge_dict.items():
        exec_count[callee] = exec_count.get(callee, 0) + count
        callees.add(callee)
        callers.add(caller)
    # main为入度为0的caller
    for caller in callers:
        if caller not in callees:
            exec_count[caller] = 1
    return exec_count


def extract_func_stmt_info(xml_path):
    """
    解析project_statements_ranges.xml，提取函数名、dir，并解析每个<stmt>标签的dec属性，
    形成 dir + func_name + stmt_dec 的列表
    """
    tree = ET.parse(xml_path)
    root = tree.getroot()
    # result_list = []
    for func in root.findall('function'):
        fun_list=[]
        name = func.get('name')
        dir_path = func.get('dir')
        if dir_path and not dir_path.startswith('/usr/include/'):
            for stmt in func.findall('stmt'):
                dec = stmt.get('dec')
                if dec:
                    fun_list.append(f"{dir_path} + {name} + {dec}")
                fun_statement_dict[name] = fun_list






def find_stmt_by_line(fun, line_num):
    stmts = fun_statement_dict.get(fun, [])
    # line_num格式为 path/to/file.c:10
    if ':' in line_num:
        file_path, line = line_num.rsplit(':', 1)
        try:
            line = int(line)
        except ValueError:
            return None
        for stmt in stmts:
            # stmt格式为 dir + func_name + stmt_dec
            if file_path in stmt:
                # stmt_dec可能为 IfStmt:306:313
                parts = stmt.split('+')[-1].strip().split(':')
                if len(parts) >= 2:
                    try:
                        start = int(parts[1])
                        end = int(parts[2]) if len(parts) > 2 else start
                        if start <= line <= end:
                            return stmt
                    except ValueError:
                        continue
    return None

def write_stmt_edges(exec_count, merged_edge_line_dict, output_file):
    with open(output_file, 'w') as f:
        for fun, count in exec_count.items():
            stmts = fun_statement_dict.get(fun, [])
            for i in range(len(stmts) - 1):
                f.write(f"{stmts[i]},{stmts[i+1]},{count}\n")
        for (caller, callee, line_num), count in merged_edge_line_dict.items():
            stmt_call = find_stmt_by_line(caller, line_num)
            stmt1 = fun_statement_dict.get(callee, [])[0] if fun_statement_dict.get(callee, []) else None
            stmtN = fun_statement_dict.get(callee, [])[-1] if fun_statement_dict.get(callee, []) else None
            if stmt_call and stmt1:
                f.write(f"{stmt_call},{stmt1},{count}\n")
            if stmt_call and stmtN:
                f.write(f"{stmtN},{stmt_call},{count}\n")
        

# 使用示例
if __name__ == "__main__":
    # sys.argv[1]输出项目名
    project_name = sys.argv[1]
    print(f"Project Name: {project_name}")


    # xml_file = "project_statements_ranges.xml"
    # result = extract_function_names(xml_file)
    # # 自动识别当前目录下所有包含pinout_new的文件
    # import glob
    # pinout_files = glob.glob("chage.pinout_new*")
    # print(pinout_files)
    # merged_edge_dict = dict()  #记录每个函数被调用的最大次数
    # merged_edge_line_dict = dict()  # 记录每个函数callee在同一个调用点被调用的最大次数
    # for fname in pinout_files:
    #     call_graph_io = CallGraphIO(fname)
    #     edge_dict, edge_line_dict = call_graph_io.process_file()
    #     for key, count in edge_dict.items():
    #         if key in merged_edge_dict:
    #             merged_edge_dict[key] = max(merged_edge_dict[key], count)
    #         else:
    #             merged_edge_dict[key] = count
    #     for key, count in edge_line_dict.items():
    #         if key in merged_edge_line_dict:
    #             merged_edge_line_dict[key] = max(merged_edge_line_dict[key], count)
    #         else:
    #             merged_edge_line_dict[key] = count
    #  # 统计每个函数的总执行次数
    # exec_count = count_function_executions(merged_edge_dict) # 统计每个函数的总执行次数

    # extract_func_stmt_info(xml_file)

    # write_stmt_edges(exec_count, merged_edge_line_dict, "edge_info_output.txt")
    








    # for (caller, callee), count in merged_edge_dict.items():
    #     print(f"Caller: {caller}, Callee: {callee}, Count: {count}")

    # print()
    # for (caller, callee, line_num), count in merged_edge_line_dict.items():
    #     print(f"Caller: {caller}, Callee: {callee}, Line: {line_num}, Count: {count}")

    # print("\n每个函数的总执行次数：")
    # for func, cnt in exec_count.items():
    #     print(f"Function: {func}, Total Executions: {cnt}")


    





