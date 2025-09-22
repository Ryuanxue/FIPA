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


SymTbl = dict()
GlobRanges = list()
GlobObjs = dict()

def process_symtbl(symtbl_file):
    with open(symtbl_file, 'r') as f:
        for line in f:
            if 'OBJECT' in line:
                ObjName = line.strip().split()[7]
                ObjLopc = int(line.strip().split()[1], 16)
                ObjSize = 0
                if "0x" in line.strip().split()[2]:
                    ObjSize = int(line.strip().split()[2], 16)
                else:
                    ObjSize = int(line.strip().split()[2], 10)
                GlobObjs[ObjLopc] = (ObjName, ObjLopc+ObjSize)
                GlobRanges.append((ObjLopc,ObjSize))

def sub_addr_with_name(pinout_file):
    with open(pinout_file, 'r') as fin:
        with open(pinout_file.replace('.pinout', '.pinout_new'), 'w') as fout:
            for line in fin:
                From = line.strip().split()[0]
                To = line.strip().split()[1]
                if "0x" in From:
                    FromValue = int(From,16)
                    if FromValue in GlobObjs:
                        From = GlobObjs[FromValue][0]
                    else:
                        for (start, size) in GlobRanges:
                            if FromValue > start and FromValue < start+size:
                                From = GlobObjs[start][0]
                                break
                    if not "0x" in From:
                        fout.write('$'+From+' '+To+'\n')
                elif "0x" in To:
                    ToValue = int(To,16)
                    if ToValue in GlobObjs:
                        To = GlobObjs[ToValue][0]
                    else:
                        for (start, size) in GlobRanges:
                            if ToValue > start and ToValue < start+size:
                                To = GlobObjs[start][0]
                                break
                    if not "0x" in To:
                        fout.write(From+' $'+To+'\n')
                else:
                    fout.write(line)

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
import subprocess
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
        

import sys
# 使用示例
if __name__ == "__main__":
    import sys
    import glob
    import os
    if len(sys.argv) < 2:
        print("Usage: python get_edge_info.py <project_dir>")
        sys.exit(1)
    project_dir = sys.argv[1]
    print(f"Project Dir: {project_dir}")
    output_dir = os.path.join(project_dir, "output")
    temp_dir = os.path.join(output_dir, "temp")


    # 自动生成.symtbl文件
    input_dir = os.path.join(project_dir, "input")
    # 查找64位可执行文件
    exe_files = glob.glob(os.path.join(input_dir, "*_64"))
    if not exe_files:
        print("No 64-bit executable found in input directory.")
        sys.exit(1)
    exe_file = exe_files[0]
    symtbl_file = os.path.join(temp_dir, os.path.basename(exe_file).replace('_64', '') + ".symtbl")
    print(f"Generating symtbl: readelf --dyn-syms {exe_file} > {symtbl_file}")
    subprocess.run(["readelf", "--dyn-syms", exe_file], stdout=open(symtbl_file, "w"))
    process_symtbl(symtbl_file)
    # 处理所有.pinout文件
    pinout_files = glob.glob(os.path.join(temp_dir, "*.pinout"))
    for pinout_file in pinout_files:
        sub_addr_with_name(pinout_file)


    # 读取 statements_ranges.xml
    xml_file = os.path.join(output_dir, os.path.basename(project_dir) + "_statements_ranges.xml")
    result = extract_function_names(xml_file)
    # 自动识别所有 pinout_new 文件
    pinout_files = glob.glob(os.path.join(temp_dir, "*.pinout_new"))
    print(pinout_files)
    merged_edge_dict = dict()
    merged_edge_line_dict = dict()
    for fname in pinout_files:
        call_graph_io = CallGraphIO(fname)
        edge_dict, edge_line_dict = call_graph_io.process_file()
        for key, count in edge_dict.items():
            if key in merged_edge_dict:
                merged_edge_dict[key] = max(merged_edge_dict[key], count)
            else:
                merged_edge_dict[key] = count
        for key, count in edge_line_dict.items():
            if key in merged_edge_line_dict:
                merged_edge_line_dict[key] = max(merged_edge_line_dict[key], count)
            else:
                merged_edge_line_dict[key] = count
    exec_count = count_function_executions(merged_edge_dict)
    extract_func_stmt_info(xml_file)
    write_stmt_edges(exec_count, merged_edge_line_dict, os.path.join(output_dir, "edge_info_output.txt"))








