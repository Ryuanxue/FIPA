#python
"""
compile_commands.json文件，文件中的element的格式如下
"arguments": [
            "gcc",
            "-c",
            "-I.",
            "-I.",
            "-DHAVE_CONFIG_H",
            "-DSYSTEM_WGETRC=\"/usr/local/etc/wgetrc\"",
            "-DLOCALEDIR=\"/usr/local/share/locale\"",
            "-O2",
            "-Wall",
            "-Wno-implicit",
            "cmpt.c"
        ],
        "directory": "/home/raoxue/Desktop/IF-driver-partition/tarbars/wget-1.8/src",
        "file": "cmpt.c"
    },
    {
        "arguments": [
            "gcc",
            "-c",
            "-I.",
            "-I.",
            "-DHAVE_CONFIG_H",
            "-DSYSTEM_WGETRC=\"/usr/local/etc/wgetrc\"",
            "-DLOCALEDIR=\"/usr/local/share/locale\"",
            "-O2",
            "-Wall",
            "-Wno-implicit",
            "cookies.c"
        ],
        "directory": "/home/raoxue/Desktop/IF-driver-partition/tarbars/wget-1.8/src",
        "file": "cookies.c"
    },
    获取每个element中的direcorty和arguments中的.c后缀的文件进行拼接，比如 filepath=/home/raoxue/Desktop/IF-driver-partition/tarbars/wget-1.8/src/cmpt.c
    对每个元素构造命令command="./get_function_range "+filepath
    运行命令
"""

import json
import subprocess
import os
import sys
import argparse
import glob
from time import sleep
import xml.etree.ElementTree as ET

def parse_args():
    parser = argparse.ArgumentParser(description='主干语句行号信息提取（语句级）')
    parser.add_argument('--project_root', type=str, default=None, help='项目根目录')
    parser.add_argument('--input_dir', type=str, default=None, help='输入目录，默认project_root/input/')
    parser.add_argument('--output_dir', type=str, default=None, help='输出目录，默认project_root/output/')
    parser.add_argument('--compile_db', type=str, default=None, help='编译数据库路径，默认input_dir/compile_commands.json')
    parser.add_argument('--filter_list', type=str, default=None, help='函数过滤列表（可选）')
    return parser.parse_args()

args = parse_args()

# 路径管理
project_root = args.project_root if args.project_root else '.'
input_dir = args.input_dir if args.input_dir else os.path.join(project_root, 'input')
output_dir = args.output_dir if args.output_dir else os.path.join(project_root, 'output')
compile_db = args.compile_db if args.compile_db else os.path.join(input_dir, 'compile_commands.json')
temp_dir = os.path.join(output_dir, 'temp')
os.makedirs(temp_dir, exist_ok=True)

# 读取编译数据库
with open(compile_db, 'r') as f:
    compile_commands = json.load(f)

# 遍历 input_dir 下所有 .c/.cpp 文件
source_files = glob.glob(os.path.join(input_dir, '**', '*.c'), recursive=True) + \
               glob.glob(os.path.join(input_dir, '**', '*.cpp'), recursive=True)

# 可选：函数过滤列表
filter_funcs = None
if args.filter_list:
    with open(args.filter_list, 'r') as f:
        filter_funcs = set(line.strip() for line in f if line.strip())

# 语句行号信息提取（假设 get_function_range 工具已存在）
for element in compile_commands:
    directory = element.get('directory', '')
    arguments = element.get('arguments', [])
    c_file = None
    for arg in arguments:
        if arg.endswith('.c') or arg.endswith('.cpp'):
            c_file = arg.split('/')[-1] if '/' in arg else arg
            break
    if c_file:
        filepath = os.path.join(directory, c_file)
        # 可选：过滤函数
        if filter_funcs:
            # 这里可扩展为只处理特定函数
            pass
        # 构造命令
        # command = f"./bin/get_function_range {filepath}"
        script_dir = os.path.dirname(os.path.abspath(__file__))
        get_func_range_bin = os.path.join(script_dir, '..', 'bin', 'get_statement_range')
        command = f"{get_func_range_bin} {filepath}"
        print(command)
        #其实c_file对应的xml文件就在当前c文件目录下，请将生成的同名xml文件移动到temp_dir目录下
        # chsh.c会生成chsh.xml，且和chsh.c在同一目录下，现在移动到temp_dir目录下
        xml_filename = os.path.join(directory, c_file.replace('.c', '.xml').replace('.cpp', '.xml'))
        temp_xml_filename = os.path.join(temp_dir, c_file.replace('.c', '.xml').replace('.cpp', '.xml'))
        # 移动生成的 XML 文件到 temp_dir
        if os.path.exists(xml_filename):
            os.rename(xml_filename, temp_xml_filename)
            print(f"Moved XML to temp directory: {temp_xml_filename}")
        try:
            result = subprocess.run(command, shell=True, check=True, capture_output=True, text=True)
            print(result.stdout)
        except subprocess.CalledProcessError as e:
            print(f"Error running command: {e}")

sleep(10)  # 等待所有命令执行完毕make CFLAGS+="-g -O0 -m32 -I/flowcheck/include"
# 合并 XML 文件
unique_functions = {}
for element in compile_commands:
    directory = element.get('directory', '')
    arguments = element.get('arguments', [])
    c_file = None
    for arg in arguments:
        if arg.endswith('.c') or arg.endswith('.cpp'):
            c_file = arg.split('/')[-1] if '/' in arg else arg
            break
    if c_file:
        xml_path = os.path.join(temp_dir, c_file.replace('.c', '.xml').replace('.cpp', '.xml'))
        print(f"Looking for XML: {xml_path}")
        if os.path.exists(xml_path):
            print(f"Processing XML: {xml_path}")
            tree = ET.parse(xml_path)
            root = tree.getroot()
            # 遍历 <function> 元素
            for function in root.findall('function'):
                function_name = function.get('name')
                function_dir = function.get('dir')
                
                # 唯一标识符
                function_key = (function_name, function_dir)
                
                # 检查是否已存在
                if function_key not in unique_functions:
                    unique_functions[function_key] = function

combined_root = ET.Element('root')
for func in unique_functions.values():
    combined_root.append(func)
combined_tree = ET.ElementTree(combined_root)
proname = os.path.basename(project_root.rstrip('/'))
final_xml_path = os.path.join(output_dir, f"{proname}_statements_ranges.xml")
combined_tree.write(final_xml_path, encoding='utf-8', xml_declaration=True)
print(f"合并后的 XML 文件已生成: {final_xml_path}")

