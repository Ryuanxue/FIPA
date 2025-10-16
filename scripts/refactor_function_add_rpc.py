
"""Refactors a function to add RPC calls to separate sensitive and non-sensitive domains.
    This module parses C code, identifies mixed functions containing both sensitive and non-sensitive
    code blocks, and refactors them by extracting sensitive blocks into separate functions. It then
    generates appropriate RPC wrapper code to maintain the functionality across security domain
    boundaries.
    The module performs the following main tasks:
    1. Parses C files into Abstract Syntax Trees (ASTs)
    2. Identifies sensitive and non-sensitive code regions based on policy files
    3. Refactors mixed functions by extracting sensitive blocks
    4. Analyzes function call relationships to identify cross-domain calls
    5. Generates RPC IDL files and wrapper functions for cross-domain communication
    Key functions:
    - preprocess_c_file_and_parse_toAST: Preprocesses and parses C files into ASTs
    - refactor_mixed_function_ast: Refactors mixed functions by extracting sensitive blocks
    - find_nonsense_upto_sense_duplicate: Identifies calls crossing security domain boundaries
    - find_sense_down_to_nonsese_dipication: Determines which functions can be duplicated
    - judge_boundary_call_need_rpc: Determines which boundary calls need RPC
    - generate_idl_file: Generates RPC IDL file for cross-domain calls
    - generate_wrapper_file: Generates RPC wrapper functions
    Global variables:
    - file_ast_dict: Maps file paths to their ASTs
    - sense_fun_list: Set of sensitive function names
    - non_sense_fun_list: Set of non-sensitive function names
    - call_list: Maps functions to their callees
    - sensefun_can_copy: List of sensitive functions that can be duplicated
    - boundry_call_need_rpc: List of boundary calls needing RPC"""

import rpc_helper
import os
import numexpr as ne
import sys
import json
import subprocess
from pycparser import c_parser, c_generator, c_ast, parse_file
from collections import defaultdict
import re
import copy
import shutil
import tarfile
import clang.cindex
from clang.cindex import CursorKind
import xml.etree.ElementTree as ET

from rpc_helper import typedef_dict, struct_dict,basic_type

sum_fun_list=set() #存放所有的函数名
file_ast_dict={} #存放文件名和对应的AST
sense_fun_list=set() #存放sense函数的函数名
#为sense_fun_list中的函数添加access_sense_api属性和access_global属性
sense_fun_status=defaultdict(lambda: {'access_sense_api': False, 'access_global': False})
sense_api_list=['read',"SSL_read"] #存放sense函数调用的sense_api函数名

non_sense_fun_list=set() #存放non_sense函数的函数名
# sense_fundecl_list={} #存放sense函数的声明
non_sense_fundecl_list=[] #存放non_sense函数的声明
call_list={} #存放函数调用关系
File_Node_dict={} #存放non_sense节点
Fun_Ast_dict={} #存放函数名和对应的ast
sensefun_can_copy=[] #存放可以duplicated到不敏感域的的sense函数
nonsense_upto_sense_function = set() #由于rpc的单向调用，将non_sense函数提升到sense函数
file_refactorFun_ast_dict={} #存放文件名和对应的重构后的AST
boundry_call_need_rpc = [] #存放需要rpc的边界函数调用
nonsense_to_sense_call=[] #存放non_sense函数调用sense函数的调用边界关系


fun_fundeclast_dict={} #存放函数名和对应的函数声明的ast

space4='    ' #缩进4个空格
space8='        ' #缩进8个空格
space12='            ' #缩进12个空格
space16='                ' #缩进16个空格

global_src_dir=""


cfile_list=[] #存放.c文件
file_fun_dict={} #key是文件名，value是文件中的函数列表，这是经过mixed改造过后的ast所求的
fun_file_dict={} #key是函数名，value是函数所在的文件，这是经过mixed改造之前的ast所求的
refactor_fun_ast_dict={} #存放重构后的函数的ast,key是函数名，value是ast
rpc_file_callpair={} #存放rpc文件和对应的函数调用关系,key是文件名，value是函数调用关系
dynamie_callpair=[] #存放动态调用对
static_callpair=[] #存放静态调用对
indirect_callpair=[] #存放间接调用对
fun_IDname_dict={} #存放函数名和funast中所有的ID上的name
# 初始化数据结构
fun_gvallist = {} #存放函数名和函数中所使用的全局变量列表
gval_rwproperty_dict = {} #存放全局变量和对应的读写属性
gval_decllocation_dict = {} #存放全局变量和对应的声明位置
sense_domain=[] #所有在sense域中的函数名，包括由nonsense提升到sense的函数
nonsense_domain=[] #所有的nonsense域中的函数名，包括由sense降级到nonsense的函数
nonsense_domain_modified_asts = {} #存放nonsense域中的函数名和对应的修改后的ast
sense_global_var = set() #存放sense域中的全局变量
nonsense_global_var = set() #存放nonsense域中的全局变量
replaced_functions = []# 用于记录哪些函数被替换为wrapper函数
global_var_asts = {}  # 添加这行，声明global_var_asts为全局变量

typedef_struct_rpc=[] #存放typedef和struct的rpc定义

# IDL文件的函数和结构体定义
idl_functions = []
idl_structs = []

# 服务端实现的函数
server_functions = []

# 客户端wrapper函数
wrapper_functions = []
wrapper_header_functions = []


def setup_project_directories(proname, so_type, quan_str):
    """
    设置项目目录，自动解压和准备源码目录
    
    如果examples/{proname}/input/source_code目录下存在非压缩目录，则删除，
    则查找并解压对应的tar.xz压缩包，并创建备份目录
    
    Args:
        proname (str): 项目名称
        so_type (str): 通信类型
        quan_str (str): 量化位数
        
    Returns:
        tuple: (abs_path_policy_file, proj_dir, proj_back_dir) 返回策略文件路径和项目目录路径
    """
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    # 构建策略文件路径
    abs_path_policy_file = os.path.abspath(os.path.join(
        script_dir, '..', 'examples', proname, 'output', 
        f'minsencode_{so_type}_{quan_str}bit.txt'
    ))
    
    # 构建源码目录路径
    source_code_dir = os.path.abspath(os.path.join(
        script_dir, '..', 'examples', proname, 'input', 'source_code'
    ))
    #检查目录下是否有非压缩目录
    for item in os.listdir(source_code_dir):
        item_path = os.path.join(source_code_dir, item)
        if os.path.isdir(item_path) and not item.endswith('.tar.xz'):
            print(f"找到非压缩目录: {item_path}，准备删除")
            shutil.rmtree(item_path)
    
    # 查找并解压对应的tar.xz压缩包
    for item in os.listdir(source_code_dir):
        if item.endswith('.tar.xz'):
            tar_path = os.path.join(source_code_dir, item)
            print(f"找到压缩包: {tar_path}，准备解压")
            with tarfile.open(tar_path, 'r:xz') as tar:
                tar.extractall(path=source_code_dir)
                print(f"解压完成: {tar_path}")
                #备份为，在加压后的目录下创建一个同名的_back目录，
                extracted_dir_name = item[:-7]  # 去掉 .tar.xz 后缀
                extracted_dir_path = os.path.join(source_code_dir, extracted_dir_name)
                backup_dir_path = extracted_dir_path + '_back'
                print(f"创建备份目录: {backup_dir_path}")
                shutil.copytree(extracted_dir_path, backup_dir_path)

    #若果proname为telnet,打补丁patch -d inetutils-1.9.4 -p1 < rpcdiff.patch
    if proname == "telnet" or proname == "wget" or proname == "nginx":
        patch_file = os.path.abspath(os.path.join(source_code_dir, 'rpcdiff.patch'))
        subprocess.run(f'patch -d {os.path.join(source_code_dir, extracted_dir_name)} -p1 < {patch_file}', shell=True, check=True)

    return abs_path_policy_file, source_code_dir, backup_dir_path

class NodeVisitor(c_ast.NodeVisitor):
    def __init__(self, base_dir):
        self.base_dir = base_dir

    """获得所有函数定义的函数名,定义一个列表保存函数名"""
    function_list = []
    def visit_FuncDef(self, node):
        # 获取函数名
        func_name = node.decl.name
        # 将函数名添加到列表中
        self.function_list.append(func_name)
        # 继续遍历子节点
        self.generic_visit(node)
        

    
    def visit(self, node):
        if hasattr(node, 'coord') and node.coord:
            # 使用文件名的基准路径转换为绝对路径
            if node.coord.file:
                node.coord.file = os.path.abspath(os.path.join(self.base_dir, node.coord.file))
        super().visit(node)

    



def preprocess_c_file_and_parse_toAST(proname):
    global global_src_dir
    # Preprocesses C files and parses them into Abstract Syntax Trees (AST).
    
    # This function reads the compile_commands.json file for a given project, iterates through each
    # compilation command, performs preprocessing on each C file, and then parses the preprocessed
    # file into an AST using pycparser.
    
    # The function performs the following steps for each C file:
    # 1. Constructs a preprocessing command using gcc with appropriate include paths and defines
    # 2. Executes the preprocessing command in the appropriate directory
    # 3. Parses the preprocessed file into an AST
    # 4. Updates the AST node coordinates to use absolute file paths
    # 5. Stores the AST in a global dictionary with the absolute file path as key
    # 6. Adds the absolute file path to a global list

    # Args:
    #     proname (str): The name of the project to process
        
    # Returns:
    #     None: Results are stored in global variables file_ast_dict and cfile_list
        
    # Global Variables:
    #     file_ast_dict: Dictionary mapping absolute file paths to their corresponding ASTs
    #     cfile_list: List of absolute file paths to C files that were processed
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
        解析compile_commands.json文件
        cd切换到direcorty
        根据arguments中的选项构造预处理命令gcc -E -I/home/raoxue/Desktop/IF-driver-partition/code_refactor/fake_libc_include -D'__attribute__(x)=' -I. -I. -DHAVE_CONFIG_H -DSYSTEM_WGETRC=\"/usr/local/etc/wgetrc\" -DLOCALEDIR=\"/usr/local/share/locale\" -O2 -Wall -Wno-implicit cmpt.c -o cmpt.i
        运行命令
        cd切换回当前目录
    """
    arser = c_parser.CParser()

    # current_directory = os.getcwd()
    # # proname = sys.argv[1]
    # compile_commands_file="../partitioned_software/"+proname+"/flowcheck_result/compile_commands.json"
    script_dir = os.path.dirname(os.path.abspath(__file__))
    fake_dir=os.path.abspath(os.path.join(script_dir, 'fake_libc_include'))
    print("fake_dir is: " + fake_dir)
    # 使用 os.path.abspath 来处理路径，使其成为绝对路径
    compile_commands_file = os.path.abspath(os.path.join(script_dir, '..', 'examples', proname, 'input', 'compile_commands.json'))
    # compile_commands_file = 'compile_commands.json'

    with open(compile_commands_file, 'r') as f:
        compile_commands = json.load(f)
    # print("所有的函数名")
    for element in compile_commands:
        directory = element.get('directory', '')
        if global_src_dir=="":
            #从directory中找到source_root,source_code的下一级目录即为global_src_dir,比如"directory": "/home/raoxue/Desktop/IF-driver-partition/source_code/wget-1.8/src,则global_src_dir="/home/raoxue/Desktop/IF-driver-partition/source_code/wget-1.8"
            global_src_dir = os.path.abspath(os.path.join(directory, '..', 'source_root', 'source_code'))

        arguments = element.get('arguments', [])
        c_file = element.get('file',"")
        # 构造预处理命令
        # preprocess_command = [
        # 'gcc', '-E',
        # '-I/home/raoxue/Desktop/IF-driver-partition/code_refactor/fake_libc_include',
        # '-D__attribute__(x)=',
        # '-D__restrict=',
        # '-D__asm__(x)='
        # ]
        
        preprocess_command = [
        'gcc', '-E',
        f'-I{fake_dir}',
        '-D\'__attribute__(x)=\'','-D__restrict=', '-D__inline='
        ]

        for i, arg in enumerate(arguments):
            if arg.startswith('-I'):
                # 直接添加 -I 参数（支持 -Ipath 和 -I path 两种格式）
                preprocess_command.append(arg)
                if arg == '-I' and i + 1 < len(arguments):
                    preprocess_command.append(arguments[i + 1])
            elif arg.startswith('-D'):
                # 处理 -D 参数，确保路径值正确转义
                if arg.find("=") != -1:
                    index = arg.find("=")
                    arg = arg[:index+1] + "\'" + arg[index+1:] + "\'"
                preprocess_command.append(arg)  
        preprocess_command.append(c_file)       
        preprocess_command.append('-o')
        preprocess_command.append(c_file + '.i')
        command = ' '.join(preprocess_command)
        print("command is: " + command)
        subprocess.run(command,cwd=directory,check=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)

        #取得c_file.i的绝对路径
        c_file_i = os.path.join(directory,c_file+".i")
        base_dir = os.path.dirname(c_file_i)
        abs_c_file=os.path.join(directory,c_file)
        # print("c_file_i is: " + c_file_i)
        ast = parse_file(c_file_i, use_cpp=True)
        visitor = NodeVisitor(base_dir)
        visitor.visit(ast) #将ast的coord的相对路径改为绝对路径
        file_ast_dict[abs_c_file] = ast
        cfile_list.append(abs_c_file)
        #获得ast中的所有函数名
        
        for func_name in visitor.function_list:
            # print("function name is: " + func_name)
            if func_name not in fun_file_dict.keys():
                # print(func_name)
                fun_file_dict[func_name] = abs_c_file
    # print()


def parse_node(line):
    # 正则表达式提取文件、函数名、类型、行号等信息
    match = re.match(r'(\d+): (.+?) \+ (.+?) \+ (.+?):(\d+):(\d+): sz = (\d+), quan = (\d+)', line.strip())
    if match:
        return {
            'index': int(match.group(1)),
            'file': match.group(2),
            'function': match.group(3),
            'type': match.group(4),
            'start_line': int(match.group(5)),
            'end_line': int(match.group(6)),
            'size': int(match.group(7)),
            'quantity': int(match.group(8)),
        }
    return None





def extract_variables(block_items):
    """从给定的AST节点中提取所有使用的变量名称"""
    used_vars = set()
    for item in block_items:
        if isinstance(item, c_ast.ID):
            used_vars.add(item.name)
        # 递归处理子节点
        for child in item.children():
            used_vars.update(extract_variables([child[1]]))
    return used_vars

def find_missing_declarations(block_items, used_variables):
    """查找需要变量声明的变量集合，并将声明返回"""
    missing_declarations = []
    declared_vars = set()

    # 提取已有的变量声明
    for item in block_items:
        if isinstance(item, c_ast.Decl):
            declared_vars.add(item.name)
    
    # 查找未声明的变量
    for var in used_variables:
        if var not in declared_vars:
            missing_declarations.append(var)  # 这里你需要根据实际情况添加声明的细节
    
    return missing_declarations


def find_missing_declarations_ast_node(funast, used_variables):
    """查找需要的变量声明，并将声明返回"""
    missing_declarations_decl_ast = []
    missing_declarations_decl_name = []



    # 提取已有的变量声明
    for item in funast.body.block_items:
        if isinstance(item, c_ast.Decl):
            if item.name in used_variables:
                tempitem=copy.deepcopy(item)
                tempitem.init = None
                #如果tempitem的限定词是static，则将其删除
                if 'static' in tempitem.storage:
                    tempitem.storage.remove('static')
                missing_declarations_decl_ast.append(tempitem)
                missing_declarations_decl_name.append(c_ast.ID(item.name))

    #funast是一个函数对应的AST,如果funast的参数列表不为空，遍历参数列表，如果参数列表中的参数在used_variables中，则将该参数加入missing_declarations_decl_ast
    if funast.decl.type.args:
        for param in funast.decl.type.args.params:
            if isinstance(param, c_ast.EllipsisParam):
                continue
            if param.name in used_variables:
                tempitem=copy.deepcopy(param)
                tempitem.init = None
                if 'static' in tempitem.quals:
                    tempitem.quals.remove('static')
                missing_declarations_decl_ast.append(tempitem)
                missing_declarations_decl_name.append(c_ast.ID(param.name))
    return (missing_declarations_decl_ast,missing_declarations_decl_name)


# 重构 AST，根据敏感块提取新函数并插入调用
def refactor_ast(ast, sensitive_blocks,file):
    """目前这个函数不完善，只考虑了只有一个子函数的情况，没有考虑多个子函数的情况"""
    # 使用 pycparser 生成器生成新函数并插入
    new_functions = [] #用于存放新生成的函数，每个函数对应一个敏感块，新函数有可能是多个
    new_funcdecls = [] #用于存放新生成的函数的声明
    func_counter = 1 # 用于生成新函数的名称

    if len(sensitive_blocks) == 0:
        print("No sensitive blocks found")
    
    #打印sensitive_blocks
    # for block in sensitive_blocks:
    #     print("sensitive block is:")
    #     for node in block:
    #         print(node)

    # 解析ast找到函数名我block[0]['function']的函数,在此函数中遍历第一层节点，获得节点的行号，若节点的行号在block中，则将该节点加入到new_func中
    for node in ast.ext:
        if isinstance(node, c_ast.FuncDef) and node.decl.name == sensitive_blocks[0][0]['function']:
            #新建一个新的函数new_func，返回值为void，函数名为new_func_name，返回值不一定为void，可以根据需要修改
            for block in sensitive_blocks:
                new_func_name = f'{node.decl.name}_sense_{func_counter}'
                func_counter += 1
                new_func = c_ast.FuncDef(
                    decl=c_ast.Decl(
                        name=new_func_name,  # 函数名
                        quals=[],
                        align=[],
                        storage=[],
                        funcspec=[],
                        type=c_ast.FuncDecl(
                            args=c_ast.ParamList(params=[]),  # 参数列表（可根据需要填充）
                            type=c_ast.TypeDecl(
                                declname=new_func_name,
                                quals=[],
                                align=None,
                                type=c_ast.IdentifierType(names=['void'])  # 返回类型为 void
                            )
                        ),
                        init=None,
                        bitsize=None
                    ),
                    param_decls=None,
                    body=c_ast.Compound(block_items=[])  # 函数体
                )
                # 提取匹配行号的节点
                block_items_to_move = []
                insensitive_items = []
                decllist = []
                for item in node.body.block_items:
                    if item.coord and item.coord.line in [n['start_line'] for n in block]:
                        block_items_to_move.append(item)
                    else:
                        if isinstance(item, c_ast.Decl):
                            decllist.append(item)
                        else:
                            insensitive_items.append(item)

                # 将提取的节点加入新函数的主体
                if len(insensitive_items)==0:
                    decllist.reverse()
                    for decl in decllist:
                        block_items_to_move.insert(0, decl)
                new_func.body.block_items = block_items_to_move

             
                """"修改在子函数中modify的变量为指针类型"""
                # 查找并添加缺失的变量声明
                used_variables = extract_variables(new_func.body.block_items)
                missing_declarations = find_missing_declarations(new_func.body.block_items, used_variables)
                (missing_declarations_ast,decl_ast_name)=find_missing_declarations_ast_node(node,missing_declarations)

                #遍历missing_declarations_ast的每个元素，如果此元素是基本类型，判断此变量是否在new_func.body.block_items中是否作为赋值语句的左值，如果是，
                # 则将此元素更新为指针类型，将new_func.body.block_items中对此变量的使用改为指针解引用的方式，同时将decl_ast_name中对应的变量改为传地址的形式·
                for index, item in enumerate(missing_declarations_ast):
                    if isinstance(item.type,c_ast.TypeDecl) and item.name in used_variables:
                        #递归遍历new_func.body.block_items，判断是否有赋值语句的左值是item.name，写成一个递归函数的形式吧，返回值为bool
                        def is_left_value(node,name):
                            if isinstance(node,c_ast.Assignment):
                                #判断node的lvname是否有name属性
                                if hasattr(node.lvalue,'name'):
                                    if node.lvalue.name == name:
                                        return True
                            if isinstance(node,c_ast.UnaryOp):
                                if node.op == '++' or node.op == '--':
                                    if hasattr(node.expr,'name'):
                                        if node.expr.name == name:
                                            return True
                            #如果item是一个list，遍历list中的每个元素
                            if isinstance(node,list):
                                for child in node:
                                    if is_left_value(child,name):
                                        return True
                            elif hasattr(node,'children'):
                                for child in node.children():
                                    if is_left_value(child[1],name):
                                        return True
                            return False
                        if is_left_value(new_func.body.block_items,item.name):
                            item.type=c_ast.PtrDecl(quals=[],type=item.type)
                            decl_ast_name[index]=c_ast.UnaryOp(op='&',expr=decl_ast_name[index])
                            #写一个递归函数，遍历new_func.body.block_items，如果有item.name的使用，将其改为指针解引用的形式
                            def change_to_pointer(node, name):
                                # 如果是目标变量名，将其替换为指针解引用形式
                                if isinstance(node, c_ast.ID) and node.name == name:
                                    return c_ast.UnaryOp(op='*', expr=node)

                                # 如果是列表类型，递归处理列表中的每个元素
                                if isinstance(node, list):
                                    return [change_to_pointer(child, name) for child in node]

                                # 如果是 Compound 类型，递归处理其 block_items
                                if isinstance(node, c_ast.Compound):
                                    if node.block_items:
                                        node.block_items = [change_to_pointer(item, name) for item in node.block_items]
                                    return node
                                # 如果是 Case 类型，递归处理其 stmts
                                if isinstance(node, c_ast.Case) or isinstance(node, c_ast.Default):
                                    if node.stmts:
                                        node.stmts = [change_to_pointer(stmt, name) for stmt in node.stmts]
                                    return node

                                # 如果是 ExprList 类型，递归处理其 exprs 列表
                                if isinstance(node, c_ast.ExprList):
                                    node.exprs = [change_to_pointer(expr, name) for expr in node.exprs]
                                    return node

                                # 如果节点有子节点，递归处理子节点
                                if hasattr(node, 'children'):
                                    for child_name, child in node.children():
                                        new_child = change_to_pointer(child, name)
                                        setattr(node, child_name, new_child)

                                # 其他情况直接返回节点
                                return node
                            
                            new_func.body.block_items = change_to_pointer(new_func.body.block_items, item.name)

                            
                """======"""



                """goto label处理部分"""
                #写一个递归函数，遍历new_func.body.block_items，如果有goto语句，获得goto的label，判断label是否在new_func.body.block_items中，如果不在，将label记录到一个list中，注意可能有多个不同的label
                #递归函数判断label是否在new_func.body.block_items中
                def is_label_in_block_items(node,label):
                    if isinstance(node,c_ast.Label):
                        if node.name == label:
                            return True
                    if isinstance(node,list):
                        for child in node:
                            if is_label_in_block_items(child,label):
                                return True
                    elif hasattr(node,'children'):
                        for child in node.children():
                            if is_label_in_block_items(child[1],label):
                                return True
                    return False

                def get_goto_label(node,goto_labels,block_items):
                    if isinstance(node,c_ast.Goto):
                        if not is_label_in_block_items(block_items,node.name):
                            #如果node.name不在goto_labels中，将其加入到goto_labels中
                            if node.name not in goto_labels:
                                goto_labels.append(node.name)
                    if isinstance(node,list):
                        for child in node:
                            get_goto_label(child,goto_labels,block_items)
                    elif hasattr(node,'children'):
                        for child in node.children():
                            get_goto_label(child[1],goto_labels,block_items)
                    return goto_labels
                goto_labels=[]
                goto_labels=get_goto_label(new_func.body.block_items,goto_labels,new_func.body.block_items)
                #如果goto_labels不为空，在new_func中添加参数int *lavel_value.如果函数的返回值类型不为void，在new_func起始位置声明一个类型为返回值类型的变量label_return，
                # 将new_func.body.block_items中的goto label语句替换为label_value=1;return label return;
                label_decl_ast=[]
                for label in goto_labels:
                    label_decl_ast.append(c_ast.Decl(name=label+'_value',quals=[],align=[], storage=[],funcspec=[],
                                                     type=c_ast.PtrDecl(quals=[], type=c_ast.TypeDecl(declname=label+'_value',quals=[],align=[], 
                                                                                                      type=c_ast.IdentifierType(names=['int']))),
                                                                                                      init=None,bitsize=None))
        
                            
                """======"""  
                            
                """返回值处理部分，同时处理goto label的替换"""
                #写一个递归函数判断new_func.body.block_items中是否有return语句
                def has_return(node):
                    if isinstance(node,c_ast.Return):
                        return True
                    if isinstance(node,list):
                        for child in node:
                            if has_return(child):
                                return True
                    elif hasattr(node,'children'):
                        for child in node.children():
                            if has_return(child[1]):
                                return True
                    return False
                

                
                #如果has_return为true，获得node的返回值，将其赋值给new_func的返回值
                if has_return(new_func.body.block_items):
                    return_type=copy.deepcopy(node.decl.type.type)
                    new_func.decl.type.type=return_type
                    if isinstance(return_type,c_ast.TypeDecl):
                        new_func.decl.type.type.declname=new_func_name
                    elif isinstance(return_type,c_ast.PtrDecl):
                        new_func.decl.type.type.type.declname=new_func_name
                    #为new_func添加一个参数int new_func_name_ret,并将其加入到new_func.body.block_items的第一个位置
                    return_var=c_ast.Decl(name=new_func_name+'_ret',quals=[],align=[], storage=[],funcspec=[],type=c_ast.PtrDecl(quals=[], type=c_ast.TypeDecl(declname=new_func_name+'_ret',quals=[],align=[], type=c_ast.IdentifierType(names=['int']))),init=None,bitsize=None)
                    new_func.decl.type.args = c_ast.ParamList(params=missing_declarations_ast)
                    new_func.decl.type.args.params.insert(0,return_var)
                    #在&decl_ast_name中加入new_func_name_ret
                    decl_ast_name.insert(0,c_ast.UnaryOp(op='&',expr=c_ast.ID(new_func_name+'_ret')))

                    

                    #写一个递归函数，遍历new_func.body.block_items，如果有return语句，在return语句前加入一个赋值语句，*new_func_name_ret=1
                    def add_return_value(node):
                        """
                        Processes an AST node to add a return value assignment before each return statement.

                        This function modifies the AST by inserting an assignment statement before each 
                        return statement, which sets a pointer variable to 1. The function recursively 
                        processes the entire AST structure.

                        Args:
                            node: An AST node (or list of nodes) to process. Can be:
                                - A c_ast.Return node
                                - A list of nodes
                                - A c_ast.Compound node (code block)
                                - A c_ast.Case or c_ast.Default node (switch case statements)
                                - A c_ast.ExprList or c_ast.InitList node
                                - Any other node with a 'children' attribute

                        Returns:
                            The modified AST node or structure with added return value assignments.
                            If the node is a Return statement, returns a Compound containing both
                            the assignment and the original return.
                        """
                        if isinstance(node, c_ast.Return):
                            return c_ast.Compound(block_items=[
                                c_ast.Assignment(op='=', lvalue=c_ast.UnaryOp(op='*', expr=c_ast.ID(new_func_name+'_ret')), rvalue=c_ast.Constant(type='int', value='1')),
                                node
                            ])
                        if isinstance(node, list):
                            return [add_return_value(child) for child in node]
                        elif isinstance(node, c_ast.Compound):
                            if node.block_items:
                                node.block_items = [add_return_value(stmt) for stmt in node.block_items]
                            return node
                        elif isinstance(node, c_ast.Case) or isinstance(node, c_ast.Default):
                            if node.stmts:
                                node.stmts = [add_return_value(stmt) for stmt in node.stmts]
                            return node
                        elif isinstance(node, c_ast.ExprList) or isinstance(node, c_ast.InitList):
                            return node
                        elif isinstance(node, c_ast.DeclList):
                            # 特殊处理 DeclList 类型
                            if hasattr(node, 'decls'):
                                for i in range(len(node.decls)):
                                    node.decls[i] = add_return_value(node.decls[i])
                            return node
                        elif hasattr(node, 'children'):
                            for child_name, child in node.children():
                                try:
                                    new_child = add_return_value(child)
                                    if hasattr(node, child_name):
                                        setattr(node, child_name, new_child)
                                except Exception as e:
                                    # 处理可能的错误，提供更多信息
                                    print(f"Error processing {type(node)} with child {child_name}: {e}")
                        return node


                    #======================================================================
                    new_func.body.block_items=add_return_value(new_func.body.block_items)

                    #如果len(goto_labels)>0,在new_func.body.block_items的第一个位置声明和new_func函数返回值类型相同的变量label_return
                    if len(goto_labels)>0:
                        print("have return goto funname"+new_func_name)
                        #在new_func的参数列表中添加相应的参数
                        for label_decl in label_decl_ast:
                            new_func.decl.type.args.params.insert(0,label_decl)
                            decl_ast_name.insert(0,c_ast.UnaryOp(op='&',expr=c_ast.ID(label_decl.name)))
                        
                        #写一个递归函数将nfew_func中的goto语句替换为label_value=1;return label_return;
                        def replace_goto_label(node,label):
                            if isinstance(node,c_ast.Goto):
                                if node.name == label:
                                    return c_ast.Compound(block_items=[
                                        c_ast.Assignment(op='=', lvalue=c_ast.ID(label+'_value'), rvalue=c_ast.Constant(type='int',value='1')),
                                        c_ast.Return(expr=c_ast.ID('label_return'))
                                    ])
                            if isinstance(node,list):
                                return [replace_goto_label(child,label) for child in node]
                            elif isinstance(node,c_ast.Compound):
                                if node.block_items:
                                    node.block_items=[replace_goto_label(stmt,label) for stmt in node.block_items]
                                return node
                            elif isinstance(node,c_ast.Case) or isinstance(node,c_ast.Default):
                                if node.stmts:
                                    node.stmts=[replace_goto_label(stmt,label) for stmt in node.stmts]
                                return node
                            elif isinstance(node,c_ast.ExprList) or isinstance(node,c_ast.InitList):
                                return node
                            elif hasattr(node,'children'):
                                for child_name,child in node.children():
                                    new_child=replace_goto_label(child,label)
                                    setattr(node,child_name,new_child)
                            return node
                        label_return=c_ast.Decl(name='label_return',quals=[],align=[], storage=[],funcspec=[],type=copy.deepcopy(return_type),init=None,bitsize=None)
                        if isinstance(label_return.type,c_ast.TypeDecl):
                            label_return.type.declname='label_return'
                        if isinstance(label_return.type,c_ast.PtrDecl):
                            label_return.type.type.declname='label_return'
                        print("this is label_return")
                        print(c_generator.CGenerator().visit(label_return))
                        new_func.body.block_items.insert(0,label_return)
                        for label in goto_labels:
                            new_func.body.block_items=replace_goto_label(new_func.body.block_items,label)
                    
                    #创建一个compound节点，其中包含语句如{int new_func_name_ret=0;rettype ret=new_func(arg.....);if(new_func_name_ret)return ret;
                    compond=c_ast.Compound(block_items=[])
                    if len(goto_labels)>0:
                        for label_decl in label_decl_ast:
                            compond.block_items.append(c_ast.Decl(name=label_decl.name,quals=[],align=[], storage=[],funcspec=[],type=copy.deepcopy(label_decl.type.type),init=c_ast.Constant(type='int',value='0'),bitsize=None))
                    compond.block_items.append(c_ast.Decl(name=new_func_name+'_ret',quals=[],align=[], storage=[],funcspec=[],type=c_ast.TypeDecl(declname=new_func_name+'_ret',quals=[],align=[], type=c_ast.IdentifierType(names=['int'])),init=c_ast.Constant(type='int',value='0'),bitsize=None))
                    copyreturn_type=copy.deepcopy(return_type)
                    if isinstance(copyreturn_type,c_ast.PtrDecl):
                        copyreturn_type.type.declname=new_func_name+'_return'
                    if isinstance(copyreturn_type,c_ast.TypeDecl):
                        copyreturn_type.declname=new_func_name+'_return'
                    # wrapper_name=new_func_name+'_wrapper'
                    compond.block_items.append(c_ast.Decl(name=new_func_name+"_return",quals=[],align=[], storage=[],funcspec=[],type=copy.deepcopy(copyreturn_type),init=c_ast.FuncCall(name=c_ast.ID(new_func_name),args=c_ast.ExprList(exprs=decl_ast_name)),bitsize=None))
                    
                    compond.block_items.append(c_ast.If(cond=c_ast.ID(new_func_name+'_ret'),iftrue=c_ast.Return(expr=c_ast.ID(new_func_name+"_return")),iffalse=None))
                    for gotoname in goto_labels:
                        compond.block_items.append(c_ast.If(cond=c_ast.ID(gotoname+'_value'),iftrue=c_ast.Goto(name=gotoname),iffalse=None))
                else:
                    new_func.decl.type.args = c_ast.ParamList(params=missing_declarations_ast)
                    # 创建新的函数调用并插入到原函数中
                    # new_func_call = c_ast.FuncCall(
                    #     name=c_ast.ID(new_func_name),
                    #     args=c_ast.ExprList(exprs=decl_ast_name)
                    # )

                    if len(goto_labels)>0:
                        #在new_func的参数列表中添加相应的参数
                        print("void return goto funname"+new_func_name)
                        for label_decl in label_decl_ast:
                            new_func.decl.type.args.params.insert(0,label_decl)
                            decl_ast_name.insert(0,c_ast.UnaryOp(op='&',expr=c_ast.ID(label_decl.name)))
                        def replace_goto_label(node,label):
                            if isinstance(node,c_ast.Goto):
                                if node.name == label:
                                    return c_ast.Compound(block_items=[
                                        c_ast.Assignment(op='=', lvalue=c_ast.ID(label+'_value'), rvalue=c_ast.Constant(type='int',value='1')),
                                        #添加return;
                                        c_ast.Return(expr=None)
                                    ])
                            if isinstance(node,list):
                                return [replace_goto_label(child,label) for child in node]
                            elif isinstance(node,c_ast.Compound):
                                if node.block_items:
                                    node.block_items=[replace_goto_label(stmt,label) for stmt in node.block_items]
                                return node
                            elif isinstance(node,c_ast.Case) or isinstance(node,c_ast.Default):
                                if node.stmts:
                                    node.stmts=[replace_goto_label(stmt,label) for stmt in node.stmts]
                                return node
                            elif isinstance(node,c_ast.ExprList) or isinstance(node,c_ast.InitList):
                                return node
                            elif hasattr(node,'children'):
                                for child_name,child in node.children():
                                    new_child=replace_goto_label(child,label)
                                    setattr(node,child_name,new_child)
                            return node
                        for label in goto_labels:
                            new_func.body.block_items=replace_goto_label(new_func.body.block_items,label)

                    compond=c_ast.Compound(block_items=[])
                    for label_decl in label_decl_ast:
                         compond.block_items.append(c_ast.Decl(name=label_decl.name,quals=[],align=[], storage=[],funcspec=[],type=copy.deepcopy(label_decl.type.type),init=c_ast.Constant(type='int',value='0'),bitsize=None))
                    
                    compond.block_items.append(c_ast.FuncCall(name=c_ast.ID(new_func_name),args=c_ast.ExprList(exprs=decl_ast_name)))
                    for gotoname in goto_labels:
                        compond.block_items.append(c_ast.If(cond=c_ast.ID(gotoname+'_value'),iftrue=c_ast.Goto(name=gotoname),iffalse=None))

                
                """======"""
                            

                
                """父函数中删除子函数中所在部分"""

                # 初始化变量来记录第一次删除的节点索引
                first_delete_index = None

                # 遍历block_items并删除所有敏感节点，同时记录第一次删除节点的索引
                new_block_items = []
                del_ind=0
                for index, item in enumerate(node.body.block_items): 
                    if item in block_items_to_move:
                        if first_delete_index is None:
                            first_delete_index = index  # 记录第一个删除的节点索引
                        if del_ind==1: #此处是为了将label及其之后的语句保留在父函数中
                            new_block_items.append(item)    
                        if isinstance(item,c_ast.Label):
                            del_ind=1
                            new_block_items.append(item)
                    
                    else:
                        new_block_items.append(item)  # 保留非敏感节点

                # 如果找到了需要删除的敏感块，插入一次函数调用到第一个删除节点的位置
                if first_delete_index is not None:
                    new_block_items.insert(first_delete_index, compond)

                # 更新节点的block_items为新的列表
                node.body.block_items = new_block_items
            
                # 添加新函数到 AST 中
                new_functions.append(new_func)
                new_fundecl=copy.deepcopy(new_func.decl)
                new_funcdecls.append(new_fundecl)
                if file not in file_refactorFun_ast_dict:
                    file_refactorFun_ast_dict[file]=[]
                file_refactorFun_ast_dict[file].append(new_func)
                refactor_fun_ast_dict[new_func_name]=new_func
            file_refactorFun_ast_dict[file].append(node)
            refactor_fun_ast_dict[node.decl.name]=node

            break

    return new_functions, new_funcdecls


def refactor_mixed_function_ast(policy_file):
    # Refactors mixed functions in a C program based on a policy file that specifies sensitive and non-sensitive nodes.
    # This function parses a policy file containing information about sensitive and non-sensitive nodes in a C program,
    # classifies functions as sensitive, non-sensitive, or mixed based on the nodes they contain, and refactors mixed
    # functions by extracting sensitive blocks into separate functions.
    # The refactoring process consists of:
    # 1. Parsing the policy file to identify sensitive and non-sensitive nodes
    # 2. Classifying functions based on the nodes they contain
    # 3. Identifying files that contain mixed functions (functions with both sensitive and non-sensitive parts)
    # 4. For each mixed function, extracting continuous sensitive blocks and creating new functions for them
    # 5. Updating the AST with the new functions and their declarations
    # Parameters
    # ----------
    # policy_file : str
    #     Path to the policy file that specifies sensitive and non-sensitive nodes.
    #     The file should have sections labeled "Sensitive Nodes:" and "Non-sensitive Nodes:"
    #     with node information in each section.
    # Returns
    # -------
    # None
    #     The function modifies global dictionaries (file_ast_dict, sense_fun_list, etc.) in place.
    # Notes
    # -----
    # The function depends on several global variables and helper functions:
    # - File_Node_dict: Maps file names to their nodes
    # - file_ast_dict: Maps file names to their AST representations
    # - sense_fun_list: Set of sensitive function names
    # - sense_fun_status: Status of sensitive functions (access_global, access_sense_api)
    # - sense_api_list: List of sensitive API functions
    # - extract_sensitive_blocks(): Helper function to extract continuous sensitive blocks
    # - refactor_ast(): Helper function to refactor the AST based on sensitive blocks
    # - parse_node(): Helper function to parse a node from the policy file
    #解析sense_config.c文件的函数
    def parse_sense_config(file_path):
        with open(file_path, 'r') as file:
            lines = file.readlines()
        
        sensitive_nodes = []
        non_sensitive_nodes = []
        in_sensitive_section = False
        in_non_sensitive_section = False
        
        for line in lines:
            #如果是line位空，则跳过
            if not line.strip():
                continue
            if "Sensitive Nodes:" in line:
                in_sensitive_section = True
                in_non_sensitive_section = False
            elif "Non-sensitive Nodes:" in line:
                in_sensitive_section = False
                in_non_sensitive_section = True
            elif in_sensitive_section:
                node=parse_node(line)
                #在node中添加键值对，key为issenstive，value为True
                if node:
                    node["issensitive"]=True
                    sensitive_nodes.append(node)
            elif in_non_sensitive_section:
                node=parse_node(line)
                if node:
                    node["issensitive"]=False
                    non_sensitive_nodes.append(node)
        return sensitive_nodes, non_sensitive_nodes
    
    # 将函数标记为敏感、非敏感或混合
    def classify_functions(sensitive_nodes, non_sensitive_nodes):
        function_status = defaultdict(lambda: {'sensitive': False, 'non_sensitive': False,"file":""})#定义一个字典，key为函数名，value为一个字典，包含sensitive和non_sensitive两个key
        function_groups = defaultdict(list)  # 新增的字典，用于存放根据函数名分组的节点
        #更新这个函数将sensitive_nodes和non_sensitive_nodes中按照function分类，将function对应的sensitive和non_sensitive的值更新为True
        for node in sensitive_nodes + non_sensitive_nodes:
            if node:
                if node['file'].startswith('/usr/include/'):
                    continue
                function_groups[node['function']].append(node)
                func_status = function_status[node['function']]
                # print(node)
                
                if node['file'].endswith('.h'):
                    func_status['file'] = fun_file_dict[node['function']]
                else:
                    func_status['file'] = node['file']
                if node in sensitive_nodes:
                    func_status['sensitive'] = True
                if node in non_sensitive_nodes:
                    func_status['non_sensitive'] = True

        #检查 function_status如果一个函数既有sensitive又有non_sensitive，则打印此函数对应的节点
        for function, status in function_status.items():
            if status['sensitive'] and status['non_sensitive']:
                #检查函数的节点的quantity，如果全为0，将这个函数的状态sensitive改为False
                #如果这个函数中quantity为0的节点的类型全是DeclStmt,将这个函数的状态non_sensitive改为False
                all_quantity_zero = all(node['quantity'] == 0 for node in function_groups[function])
                all_decl_stmt = all(node.get('type') == 'DeclStmt' for node in function_groups[function] if node['quantity'] == 0)
                
                if function in ['mkalldirs','logvprintf']:
                    function_status[function]['non_sensitive'] = False
                    continue

                # 如果全为0，将这个函数的状态sensitive改为False
                if all_quantity_zero:
                    function_status[function]['sensitive'] = False
                    continue

                # 如果这个函数中quantity为0的节点的类型全是DeclStmt,将这个函数的状态non_sensitive改为False
                if all_decl_stmt:
                    function_status[function]['non_sensitive'] = False
                    continue
        return function_status
    
    def updata_sense_fun_status(ast,function_status):
        """
        Update the status of sensitive functions based on their access to global variables and sensitive APIs.
        if a function accesses global variables or sensitive APIs, mark it as sensitive.
        if a function does not access global variables or sensitive APIs, mark it as non-sensitive.
        """
        global_decl_name=[]
        for subast in ast.ext:
            #如果subast是一个声明，获得其name，并将其加入到global_decl_name中
            if isinstance(subast,c_ast.Decl):
                global_decl_name.append(subast.name)
            if isinstance(subast, c_ast.FuncDef):
                # if subast.decl.name=="iread" and  "--debug" in sys.argv:
                #             pdb.set_trace()
                if function_status[subast.decl.name]['sensitive']:
                    #识别函数中的说以decl的name，并获得所有的ID节点，如果ID节点的name不属于本函数中声明的变量，则设置此函数的access_global为True
                    #由于ast是一个树形结构，可以使用递归的方式遍历ast，找到所有的ID节点和decl节点，将decl节点的name加入到一个list中，遍历ID节点，如果ID节点的name不在list中，则将access_global设置为True
                    #在获得id的name的时候需要判断其父节点是不是结构体，排除这种情况
                    
                    def judge_global(node,parentnode,decl_name):
                        
                        if sense_fun_status[subast.decl.name]['access_global']:
                            return
                
                        if isinstance(node,c_ast.Decl) and not isinstance(parentnode,c_ast.FuncDef):
                            decl_name.append(node.name)
                        if isinstance(node,c_ast.ID):
                            if isinstance(parentnode,c_ast.StructRef):
                                return
                            if isinstance(parentnode,c_ast.FuncCall):
                                if node.name in sense_api_list:
                                    sense_fun_status[subast.decl.name]['access_sense_api']=True
                                return
                            if node.name!="errno" and node.name not in decl_name and node.name in global_decl_name:
                                sense_fun_status[subast.decl.name]['access_global']=True
                                return
                        if isinstance(node,list):
                            for child in node:
                                judge_global(child,node,decl_name)
                        elif hasattr(node,'children'):
                            for child_name,child in node.children():
                                judge_global(child,node,decl_name)
                    decl_name=[]
                    judge_global(subast,subast,decl_name)


    def extract_sensitive_blocks(mixed_sensitive_nodes):
        """
        Extract a single continuous block of sensitive nodes from a sorted list of nodes.
        
        This function identifies the first sensitive node with non-zero quantity and groups it
        with all subsequent nodes into a single continuous sensitive block.
        
        Args:
            mixed_sensitive_nodes (list): A sorted list of dictionaries representing nodes, where each dictionary
                                        contains at least 'start_line', 'end_line', 'issensitive', and 'quantity' keys.
        
        Returns:
            list: A list containing a single list of node dictionaries that form a continuous block
                starting from the first sensitive node with non-zero quantity.
                If no such node is found, returns an empty list.
        
        Note:
            - Input nodes are assumed to be already sorted by their 'end_line' value.
            - The function returns at most one sensitive block.
        """
        sensitive_blocks = []
        current_block = []
        found_starting_node = False
        mixed_sensitive_nodes = sorted(mixed_sensitive_nodes, key=lambda x: x['end_line'])#按照end_line升序排列，同一个C文件

        # #打印mixed_sensitive_nodes
        # for node in mixed_sensitive_nodes:
        #     print("mixed sensitive node is: " + str(node))
        
        for node in mixed_sensitive_nodes:
            # 找到第一个敏感且quantity不为0的节点，开始构建敏感块
            # if not found_starting_node and node['issensitive'] and node.get('quantity', 0) != 0:
            if not found_starting_node and node['issensitive'] :
                found_starting_node = True
                current_block.append(node)
            # 如果已经找到了起始节点，将后续所有节点都加入到敏感块中
            elif found_starting_node:
                current_block.append(node)
        
        # 如果构建了敏感块，将其加入到结果列表中
        if current_block:
            sensitive_blocks.append(current_block)
    
        return sensitive_blocks
    #===========================================================================================================
    # proname=sys.argv[1]
    #划分策略有多个，所以这里一次只需要一个策略，先看看第一个策略是否能完整的run起来,形成一个完整的流程
    #将策略文件拆分成多个文件，拆分后的文件作为脚本的命令行输入参数 "../partitioned_software/"+proname+"/flowcheck_result/compile_commands.json"
    # sense_config_file_path = "../partitioned_software/"+proname+"/qg_bi_z3_result/ratio_0.106882.txt"
    sense_config_file_path=policy_file
    sensitive_nodes, non_sensitive_nodes = parse_sense_config(sense_config_file_path)#解析sense_config文件，获得sensitive_nodes和non_sensitive_nodes
    for node in non_sensitive_nodes + sensitive_nodes:
        if node['file'] not in File_Node_dict:
            File_Node_dict[node['file']] = []
        File_Node_dict[node['file']].append(node)
    function_status = classify_functions(sensitive_nodes, non_sensitive_nodes)

     #每个节点中包含了file，function，type，start_line，end_line，size，quantity等信息，
    #其中file表示该节点所在的文件，以file为key，获取ismain,issensitive,ismixed三个值，ismain表示该文件
    # 是否包含主函数，issensitive表示该文件是否敏感函数，ismixed表示该文件是否包含混合函数
    file_status = defaultdict(lambda: {'ismain': False, 'issensitive': False, 'ismixed': False})
    
    
    for func, status in function_status.items():
        #设置file_status的默认值
        #要保证file不为.h文件
        if status['file'] not in file_status:
            file_status[status['file']] = {'ismain': False, 'issensitive': False, 'ismixed': False}
        if func == 'main':
            file_status[status['file']]['ismain'] = True
        if status['sensitive'] and status['non_sensitive']:
            file_status[status['file']]['ismixed'] = True
        elif status['sensitive']:
            file_status[status['file']]['issensitive'] = True
            sense_fun_list.add(func)
            
        else:
            file_status[status['file']]['isnon_sensitive'] = True
        # print("file: " + status['file'] + " function: " + func + " status: " + str(status))

    #解析每一个mixed文件，用pycparser拆分此函数为父函数调用子函数的形式，新得到的函数插入到此AST之前。插入到ast之后，并插入函数的声明。
    # for key in file_ast_dict.keys():
    #     print
    
    for file, status in file_status.items():
        # print("文件状态为：")
        ast = file_ast_dict[file]
        updata_sense_fun_status(ast,function_status)#此函数是为了跟新函数是否访问了全局变量和敏感API并更新相应的状态
        if status['ismixed']:
            
            """遍历ast中的每一个函数，如果函数的状态是混合函数或敏感函数，识别这些函数当中是否访问了全局变量或者是访问了sense_api函数
            """
            

             #识别出对应于file的所有混合函数,func为函数名
            mixed_functions = [func for func, status in function_status.items() if status['file']==file and status['sensitive'] and status['non_sensitive']]
            #识别每个混合函数的所有节点（混合节点）
            for func in mixed_functions:
                # print("mixed function is: " + func)
                mixed_sensitive_nodes = [node for node in (sensitive_nodes + non_sensitive_nodes) if node and node['function'] == func]

                # # 打印mixed_sensitive_nodes
                # for node in mixed_sensitive_nodes:
                #     print("mixed sensitive node is: " + str(node))
                #提取连续的敏感块
                sensitive_blocks = extract_sensitive_blocks(mixed_sensitive_nodes)
                #重构AST，根据敏感块提取新函数并插入调用
                if len(sensitive_blocks) == 0 or len(sensitive_blocks[0]) == len(mixed_sensitive_nodes):
                    continue
                # print("sensitive_blocks:")
                # for block in sensitive_blocks:
                #     for node in block:
                #         print(node)
                #     print()

                new_functions,nfew_funcdecls=refactor_ast(ast, sensitive_blocks,file)
                
                for new_func in new_functions:
                    # print("new ast")
                    # print(c_generator.CGenerator().visit(new_func))
                    ast.ext.append(new_func)
                    sense_fun_list.add(new_func.decl.name)
                for new_fundecl in nfew_funcdecls:
                    ast.ext.insert(0,new_fundecl)
                    fun_fundeclast_dict[new_fundecl.name]=new_fundecl
        file_ast_dict[file]=ast
    # print()
    # print("sense_fun_list:")
    # for fun in sense_fun_list:
    #     print(fun)
    # print()
    # print("non_sense_fun_list:")
    # for fun in non_sense_fun_list:
    #     print(fun)
    # 生成新的 C 文件
    # for file, ast in file_ast_dict.items():
    #     generator = c_generator.CGenerator()
    #     new_c_code = generator.visit(ast)
    #     print(file+".i")
    #     with open(file+".i", 'w') as f:
    #         f.write(new_c_code)

    # #将sense_fun_list和non_sense_fun_list写入文件
    # with open(proname+"_sense_fun_list.txt","w") as f:
    #     for fun in sense_fun_list:
    #         f.write(fun+"\n")
    # with open(proname+"_non_sense_fun_list.txt","w") as f:
    #     for fun in non_sense_fun_list:
    #         f.write(fun+"\n")


#遍历file_ast_dict中的ast，获得ast中的函数调用关系,用字典存储，key为函数名，value为一个list，list中的每个元素为（callee，locinfo）,使用pycparser的NodeVisitor类
class FuncCallVisitor(c_ast.NodeVisitor):
    """
    A visitor class for C abstract syntax trees that extracts function call information and type definitions.
    This class extends c_ast.NodeVisitor and is designed to traverse a C AST (Abstract Syntax Tree)
    to collect information about function definitions, function calls, typedefs, structs, and enums.
    It maintains various dictionaries to store this information for later analysis.
    Attributes:
        func_calls (defaultdict): Maps function names to lists of function calls within them.
                                  Each function call is represented as a tuple of (called_func_name, coord).
        fun_fundeclast_dict (dict): Maps function names to their declaration AST nodes.
        fun_ast_dict (dict): Maps function names to their complete AST nodes.
        IDnamelist (dict): Maps function names to lists of identifiers used within them.
        typedef_dict (defaultdict): Maps typedef names to dicts with keys:
                                   - 'isstruct': Boolean indicating if it's a struct
                                   - 'isenum': Boolean indicating if it's an enum
                                   - 'ast': AST node of the typedef
        struct_dict (defaultdict): Maps struct names to dicts with keys:
                                  - 'isstruct': Boolean indicating if it's a struct
                                  - 'isenum': Boolean indicating if it's an enum
                                  - 'ast': AST node of the struct/enum
        current_func (str): Name of the function currently being visited.
    """
    def __init__(self):
        self.func_calls = defaultdict(list)
        self.fun_fundeclast_dict = {}
        self.fun_ast_dict={}
        self.IDnamelist={}
        
        #设置typedef_dict的属性是结构体还是枚举类型
        self.typedef_dict = defaultdict(lambda: {'isstruct': False, 'isenum': False})
        self.struct_dict = defaultdict(lambda: {'isstruct': False, 'isenum': False})
        self.current_func = None

    def visit_FuncDef(self, node):
        self.current_func = node.decl.name
        self.fun_fundeclast_dict[self.current_func]=node.decl
        self.fun_ast_dict[self.current_func]=node
        self.generic_visit(node)

    def visit_FuncCall(self, node):
        if isinstance(node.name, c_ast.UnaryOp): #处理函数指针
            pass
        elif self.current_func:
            if isinstance(node.name.name, c_ast.ID) or isinstance(node.name.name, c_ast.StructRef) or isinstance(node.name.name, c_ast.ArrayRef):
                return

            self.func_calls[self.current_func].append((node.name.name, node.coord))
        self.generic_visit(node)
    def visit_Typedef(self, node):
        # 获取 typedef 的类型名
        type_name = node.name
        # 将类型名和 AST 定义存入 typedef_dict
        self.typedef_dict[type_name]['ast']=node
        # 判断是否为结构体
        if isinstance(node.type.type, c_ast.Struct):
            self.typedef_dict[type_name]['isstruct'] = True
        # 判断是否为枚举类型
        elif isinstance(node.type.type, c_ast.Enum):
            self.typedef_dict[type_name]['isenum'] = True
        # 继续遍历子节点
        self.generic_visit(node)

    def visit_Struct(self, node):
        # 获取结构体的名称
        
        if node.name and node.decls:
            
            struct_name = node.name
            # 将结构体名和 AST 定义存入 struct_dict
            self.struct_dict[struct_name]['ast']=node
            self.struct_dict[struct_name]['isstruct']=True
        # 继续遍历子节点
        self.generic_visit(node)
    def visit_Enum(self, node):
        # 获取枚举类型的名称
        if node.name and node.values:
            enum_name = node.name
            # 将枚举类型名和 AST 定义存入 struct_dict
            self.struct_dict[enum_name]['ast']=node
            self.struct_dict[enum_name]['isenum']=True
        # 继续遍历子节点
        self.generic_visit(node)
    def visit_ID(self, node):
        if self.current_func not in self.IDnamelist:
            self.IDnamelist[self.current_func]=[node.name]
        elif node.name not in self.IDnamelist[self.current_func]:
            self.IDnamelist[self.current_func].append(node.name)
        self.generic_visit(node)
    

def travese_ast(ast):
    visitor = FuncCallVisitor()
    visitor.visit(ast)
    # return visitor.func_calls
    return visitor.func_calls,visitor.fun_fundeclast_dict,visitor.typedef_dict,visitor.struct_dict,visitor.fun_ast_dict,visitor.IDnamelist


#如果caller在
def find_nonsense_upto_sense_duplicate():
    """
    Identifies non-sensitive functions that need to be promoted to sensitive domain.
    This function analyzes the relationships between sensitive and non-sensitive functions in the codebase
    by traversing the call graph to find non-sensitive functions that either:
    1. Are directly called by sensitive functions
    2. Are part of a call chain that starts from a sensitive function
    3. Can potentially be called by sensitive functions via indirect function calls (其实这里不用这一句，因为其功能可包含在前两点中)
    The algorithm:
    1. Parses ASTs and builds call relationships between functions
    2. Identifies direct calls between sensitive and non-sensitive functions
    3. Uses a worklist algorithm to propagate through call chains
    4. Handles indirect function calls from both dynamic and static analysis
    5. Builds a comprehensive set of non-sensitive functions that need special handling
    Returns:
        None: Results are stored in global variables and printed to stdout
    """
    
    sense_to_non_sense_call=[] #存放sense函数调用non_sense函数的边界调用关系
    
    
    for file, ast in file_ast_dict.items():

        func_calls,fun_fundecls, ast_typedefs,ast_strcts,fun_ast_dict,idnamedict = travese_ast(ast)
        fun_fundeclast_dict.update(fun_fundecls)
        file_fun_dict[file]=[]
        for fun,funast in fun_fundecls.items():
            sum_fun_list.add(fun)
            file_fun_dict[file].append(fun)
            if fun not in sense_fun_list:
                non_sense_fun_list.add(fun)
        typedef_dict.update(ast_typedefs)
        struct_dict.update(ast_strcts)
        # func_calls = get_func_call_relationship(ast)
        for func, calls in func_calls.items():
            call_list[func]=calls
            for callee, locinfo in calls:
                if func in sense_fun_list and callee in non_sense_fun_list:
                    sense_to_non_sense_call.append((func,callee,locinfo))
                if func in non_sense_fun_list and callee in sense_fun_list:
                    nonsense_to_sense_call.append((func,callee,locinfo))
        for fun,fast in fun_ast_dict.items():
            Fun_Ast_dict[fun]=fast
        # print("function name and IDname:")
        for fun,idnamelist in idnamedict.items():
            # print(idnamelist)
            fun_IDname_dict[fun]=idnamelist

    print("non_sense_fun_list:")
    for fun in non_sense_fun_list:
        print(fun)
        

    # print()
    # print()
    # print("sense to non_sense call:")
    # for call in sense_to_non_sense_call:
    #     print(call)

    print("typedef_dict:")
    for typedef, info in typedef_dict.items():
        print(f"{typedef}: isstruct={info['isstruct']}, isenum={info['isenum']}, ast={info['ast']}")
    print("struct_dict:")
    for struct, info in struct_dict.items():
        print(f"{struct}: isstruct={info['isstruct']}, isenum={info['isenum']}, ast={info['ast']}")

    
    # print("difference set:")
    # print(sense_fun_list.difference(sensefun_can_copy))
    for func, calls in call_list.items():
        for callee,locinfo in calls:
            if (func,callee) not in static_callpair:
                static_callpair.append((func,callee))
    
    
    # print()
    # print("dynamin indirect call:")
    for call in dynamie_callpair:
        # print(call)
        if call not in static_callpair:
            if (call[0]+"_sense_1",call[1]) in static_callpair:
                continue
            # print(call)
            indirect_callpair.append((call[0],call[1],None))
            if call[0] in sense_fun_list and call[1] in non_sense_fun_list:
                    sense_to_non_sense_call.append((call[0],call[1],locinfo))


    """遍历sense_to_non_sense_call，获得每个调用关系的callee，从call_list中找到callee的调用关系，迭代此过程，直到找到的callee不在nonsense_fun_list中,可以使用工作列表法"""
    from collections import deque
    # nonsense_upto_sense_call = set()
    processed_callees = set()
    

    print("sense to non-sense call:")
    for call in sense_to_non_sense_call:
        # print(call)
        caller = call[0]
        callee = call[1]
        locinfo = call[2]
        # print(locinfo)
        worklist = deque([callee])
        """遍历sense_to_non_sense_call，获得每个调用关系的callee，从call_list中找到callee的调用关系，迭代此过程，直到找到的callee不在nonsense_fun_list中,可以使用工作列表法"""
        while worklist:
            current_callee = worklist.popleft()  # 使用popleft进行广度优先搜索
            if current_callee in non_sense_fun_list and current_callee not in processed_callees:
                nonsense_upto_sense_function.add(current_callee)
                processed_callees.add(current_callee)
                # 获取current_callee调用的函数，继续展开调用链
                for subcall in call_list.get(current_callee, []):
                    worklist.append(subcall[0])  # subcall[0]是被调用的函数名
                # for call in indirect_callpair: #处理间接调用的情况
                #     if call[0]==callee:
                #         worklist.append(call[1])
                        
    
    #当动态分析所得的间接调用不能满足需求的时候，用静态方法分析间接调用
    #根据nonsense_upto_sense_function中的函数名得到对应的ast，遍历ast得到所有的ID节点的name,判断name是否是函数名，如果是函数名且函数名不在nonsense_upto_sense_function中，则将其加入到nonsense_upto_sense_function中
    static_indirect_fun_upto_sense=set()
    for fun in nonsense_upto_sense_function:
        if fun not in fun_IDname_dict:
            continue
        innamelist=fun_IDname_dict[fun]
        for name in innamelist:
            # 添加对sense_fun_list的检查，确保不会将已经是敏感的函数加入
            if name in sum_fun_list and name not in nonsense_upto_sense_function and name not in sense_fun_list:
                static_indirect_fun_upto_sense.add(name)
    nonsense_upto_sense_function.update(static_indirect_fun_upto_sense)
    
    print()
    print ("nonsense upto(copy) sense function:")              
    for callee in sorted(nonsense_upto_sense_function):
        print(callee)

    

def find_sense_down_to_nonsese_dipication():
    """
    Classifies functions in the sensefunlist as either pure-sensitive (can't be duplicated) or part-sensitive (can be duplicated).
    This function analyzes the call graph to identify functions that should not be duplicated due to security concerns.
    It traverses the call hierarchy of each sensitive function to determine if they directly or indirectly access
    sensitive APIs or global variables.
    The algorithm works as follows:
    1. For each function in the sensitive function list:
        - Skip functions that have already been classified
        - Check if the function directly accesses sensitive APIs or has '_sense_' in its name
        - If not, recursively check all functions it calls to see if any are sensitive
        - Classify the function as either can_copy or cant_copy based on this analysis
    After classification, the function updates global lists that track sensitive and non-sensitive domains.
    Returns:
         None. Results are stored in global variables and printed to console.
    Global variables used:
         - sense_fun_status: Dictionary containing sensitivity information about functions
         - call_list: Dictionary mapping functions to the functions they call
         - sense_fun_list: List of functions that might be sensitive
         - sensefun_can_copy: List to store sensitive functions that can be duplicated
         - sensefun_cant_copy: List to store sensitive functions that cannot be duplicated
         - nonsense_upto_sense_function: Set of non-sensitive functions that call sensitive functions
         - non_sense_fun_list: List of functions deemed non-sensitive
         - sense_domain: List to store functions in the sensitive domain
         - nonsense_domain: List to store functions in the non-sensitive domain
    """
    sensefun_cant_copy=[]
    def judge_is_nonsense(fun, funset, sensefun):
        # 如果当前函数访问了敏感API或全局变量，标记为敏感
        # if sense_fun_status[fun]['access_sense_api'] or sense_fun_status[fun]['access_global']:
        if sense_fun_status[fun]['access_sense_api'] or "_sense_" in fun:
            sensefun.append(fun)
            return False  # 当前函数为敏感函数

        # 获取当前函数调用的所有子函数
        calleeset = call_list.get(fun, [])

        # 遍历所有被调用函数，递归检查是否敏感
        for cee in calleeset:
            # 如果子函数属于 funset 且尚未标记为敏感
            if cee in funset and cee not in sensefun:
                # 递归检查子函数是否敏感
                if not judge_is_nonsense(cee, funset, sensefun):
                    # 如果子函数敏感，标记当前函数为敏感
                    sensefun.append(fun)
                    return False

        # 如果自身和所有调用链均不敏感，则为非敏感函数
        return True
 

    # 遍历 funset 中的每个函数进行判断
    if proname=="mavlink-client" :
        sensefun_cant_copy.append("send_heartbeat")
    if proname=="thttpd-2.25":
        sensefun_cant_copy.append("auth_check2")
    for fun in sense_fun_list:
        # 如果函数已经是可备份的敏感函数，跳过判断
        if fun in sensefun_cant_copy or fun in sensefun_can_copy:
            continue
        list_fields = ["print_date", "close_files", "get_defaults", "update_age","list_fields"] #适用于chage
        if fun in list_fields:
            continue

        # 对当前函数进行敏感性判断
        if judge_is_nonsense(fun, sense_fun_list, sensefun_cant_copy):
            sensefun_can_copy.append(fun)
    print()
    print("can copy:")
    for fun in sensefun_can_copy:
        print(fun)
    print()
    print("cant copy:")
    for fun in sensefun_cant_copy:
        print(fun)

    for fun in nonsense_upto_sense_function.union(sense_fun_list):
        #如果函数名结尾是_sense_1,提取函数名的去处_sense_1后的部分，如果这个不分在nonsense_upto_sense_function中，则continue
        if fun.endswith("_sense_1"):
            newfun=fun[:-8]
            if newfun in nonsense_upto_sense_function:
                continue
        if fun not in sense_domain:
            sense_domain.append(fun)
            
    for fun in set(sensefun_can_copy).union(non_sense_fun_list):
        if fun+"_sense_1" in refactor_fun_ast_dict.keys() and fun in nonsense_upto_sense_function:
            continue
        if fun not in nonsense_domain:
            nonsense_domain.append(fun) 
            
    
        


def judge_boundary_call_need_rpc():
    """
    判断规则：
    1.边界调用中caller所在的语句的敏感信息量不为0，一定要改为RPC
    2.如果caller所在的语句的敏感信息量为0，判断如果callee属于sensefun_can_copy，不需要改为RPC，否则需要改为RPC
    3.如果callee的name中包含_sense_,则需要改为RPC
    """
    nosensedomain_to_sensedomain_call = []
    static_callpair=[]
    # print("difference set:")
    # print(sense_fun_list.difference(sensefun_can_copy))
    print("funcall pair:")
    for func, calls in call_list.items():
        for callee,locinfo in calls:
            # print(type(func))
            # print(type(callee))
            # print(callee)
            print(func + " " + callee)
            if (func,callee) not in static_callpair:
                static_callpair.append((func,callee))
            if func in non_sense_fun_list.union(sensefun_can_copy) and callee in sense_fun_list.difference(sensefun_can_copy):
                nosensedomain_to_sensedomain_call.append((func,callee,locinfo))
    
    indirect_callpair=[]
    print()
    print("dynamin indirect call:")
    for call in dynamie_callpair:
        # print(call)
        if call not in static_callpair:
            if (call[0]+"_sense_1",call[1]) in static_callpair:
                continue
            # print(call)
            indirect_callpair.append(call)

    print("cross domain call")
    for call in nosensedomain_to_sensedomain_call:
        print(call[0]+" "+call[1])
    
    for cr in nosensedomain_to_sensedomain_call:  # cr是一个元组，元组的第一个元素是caller，第二个元素是callee，第三个元素是locinfo，此集合小了
        
        caller = cr[0]
        callee = cr[1]
        locinfo = cr[2]
        if caller in nonsense_upto_sense_function and "_sense_" in callee:
            continue
        #如果callee的name中包含_sense_,则需要改为RPC
        if "_sense_" in callee:
            boundry_call_need_rpc.append((caller, callee, cr[2]))
            continue
        
        # if locinfo is None:
        #     continue

        locinfo = str(locinfo).split(":")
        filename = locinfo[0]
        lineno = int(locinfo[1])
        # print(caller + " " + callee)
        # print(locinfo)


        # 根据locinfo判断call语句所在的statement的敏感信息量大小，locinfo是 (filename, lineno, column)
        # 判断locinfo所在的statement是否在Non_sensitive_nodes中，如果在，获得其敏感信息量大小
        if filename in File_Node_dict:
            # print("filename in File_Node_dict")
            for node in File_Node_dict[filename]:
                # if caller=="rbuf_read_bufferful":
                #     print(node)
                if int(node['start_line']) <= lineno <= int(node['end_line']):
                    # print("node in File_Node_dict")
                    if node['quantity'] != 0:
                        # boundry_call_need_rpc.append((caller, callee, locinfo, node['quantity']))
                        boundry_call_need_rpc.append((caller, callee, locinfo))
                    else:
                        if callee not in sensefun_can_copy and callee not in ["logprintf", "logputs"]:
                            boundry_call_need_rpc.append((caller, callee, locinfo))

                    break
    
    
    for call in indirect_callpair:
         if call[0] in non_sense_fun_list.union(sensefun_can_copy) and call[1] in sense_fun_list.difference(sensefun_can_copy):
            boundry_call_need_rpc.append((func,callee,None))


    print()
    print("boundary call need rpc:")
    for call in boundry_call_need_rpc:
        print(call)
    print()
    return boundry_call_need_rpc

# 初始化 libclang
clang.cindex.Config.set_library_file('/usr/lib/llvm-12/lib/libclang.so')  # 设置 libclang 路径
def copy_and_modify_ast(proname,abs_path_policy_file):
    def extract_and_replace_function(filename, function_name, new_function_code):
        
        index = clang.cindex.Index.create()
        tu = index.parse(filename,args=['-I.','-DHAVE_CONFIG_H','-DSYSTEM_WGETRC=\"/usr/local/etc/wgetrc\"','-DLOCALEDIR=\"/usr/local/share/locale\"'])
        # print(filename)

        # 遍历 AST 查找函数
        function_code = None
        for node in tu.cursor.get_children():
            if node.kind == CursorKind.FUNCTION_DECL and node.spelling == function_name:
                # 提取函数内容
                start_offset = node.extent.start.offset
                end_offset = node.extent.end.offset
                with open(filename, 'r') as file:
                    content = file.read()
                    function_code = content[start_offset:end_offset]
                break

        if not function_code:
            print(f"函数 '{function_name}' 未找到。")
            return None

        # print(f"提取的函数 '{function_name}' 内容：\n{function_code}")

        # 删除函数并添加新函数
        new_content = content[:start_offset] + new_function_code + content[end_offset:]
        with open(filename, 'w') as file:
            file.write(new_content)
            file.flush()
        print(f"函数 '{function_name}' 已替换为新函数。")

        return function_code
    
    #============================================
    #将目录../partitioned_software/"+proname+"/raw_code/下的文件拷贝到../partitioned_software/"+proname+"/refactor_mixed_result/下  
    input_dir = "../partitioned_software/"+proname+"/0_raw_code/"
    #提取abs_path_policy_file的basename
    rationname=os.path.basename(abs_path_policy_file).split(".txt")[0]
    output_dir = "../partitioned_software/"+proname+"/4_refactor_mixed_result/"+rationname+"/"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    #清空output_dir
    for file in os.listdir(output_dir):
        file_path = os.path.join(output_dir, file)
        if os.path.isfile(file_path):
            os.remove(file_path)
        else:
            shutil.rmtree(file_path)
    for file in os.listdir(input_dir):
        src_path = os.path.join(input_dir, file)
        dst_path = os.path.join(output_dir, file)
        # 检查是否是目录
        if os.path.isdir(src_path):
            shutil.copytree(src_path, dst_path)
        else:
            shutil.copy(src_path, dst_path)
    #将file_refactorFun_ast_dict中的ast写入到../partitioned_software/"+proname+"/refactor_mixed_result/下的文件中
    for file, astlist in file_refactorFun_ast_dict.items():
        # print(file)
        refactor_file=file.replace("1_sense-annotation-code","4_refactor_mixed_result/"+rationname)
        #将refactor_file复制到当前目录下，refactor_file是一个绝对路径
        shutil.copy(refactor_file,os.path.basename(refactor_file))
        # print(refactor_file)
        # print(os.path.basename(refactor_file))
        #filename,function_name,new_function_code
        refactor_ast_decl_dict={}
        refactor_ast_dict={}
        
        
        for ast in astlist:
            new_function_code = c_generator.CGenerator().visit(ast)
            function_name = ast.decl.name
            #如果_sense_在function_name中，将function_name中的_sense_及其之后的部分去掉
            if "_sense_" in function_name:
                newfunction_name=function_name.split("_sense_")[0]
            if newfunction_name in nonsense_upto_sense_function:
                continue    
            if "_sense_" in function_name:
                if newfunction_name not in refactor_ast_decl_dict:
                    refactor_ast_decl_dict[newfunction_name]=[]
                refactor_ast_decl_dict[newfunction_name].append(ast.decl)
            if newfunction_name not in refactor_ast_dict:
                refactor_ast_dict[newfunction_name]=[]
            refactor_ast_dict[newfunction_name].append(ast)
            
        for funname in refactor_ast_dict.keys():
            # print("funname:"+funname)
            if funname in nonsense_upto_sense_function:
                continue
            new_function_code ="\n"
            for ast in refactor_ast_decl_dict[funname]:
                new_function_code+=c_generator.CGenerator().visit(ast)+";\n"
            for ast in refactor_ast_dict[funname]:
                new_function_code+=c_generator.CGenerator().visit(ast)+"\n"
                if "((st.st_mode & _S_IFMT) == _S_IFREG)"  in new_function_code:
                    new_function_code=new_function_code.replace("((st.st_mode & _S_IFMT) == _S_IFREG)","S_ISREG (st.st_mode)")
            #将new_function_code写入到文件
            
            extract_and_replace_function(os.path.basename(refactor_file), funname, new_function_code)
        
        
        current_dir = os.path.dirname(os.path.abspath(os.path.basename(refactor_file)))    
        # print(current_dir)  
        src_path = os.path.join(current_dir, os.path.basename(refactor_file))
        with open(refactor_file, 'wb') as dst_file:
            shutil.copyfileobj(open(src_path, 'rb'), dst_file)
            dst_file.flush()  # 强制刷新缓冲区



#对sense_fundecl_list中的函数生成IDL文件sense_sub_functions_ast
def generate_idl_file(boundry_call_need_rpc):
    """
        Generates an Interface Definition Language (IDL) file for RPC communication.
        This function takes function calls that need to be exposed via RPC and generates
        the necessary IDL file with appropriate structure definitions and function signatures.
        The IDL file follows the ONC RPC standard used for network communications between
        distributed components.
        Parameters:
        ----------
        boundry_call_need_rpc : list of tuples
            A list of tuples containing (caller, callee, locinfo) information about function
            calls that need to be exposed through RPC.
        Global Variables Used:
        -------------------
        - fun_fundeclast_dict: Dictionary mapping function names to function declaration ASTs
        - struct_dict: Dictionary of structure definitions
        - typedef_dict: Dictionary of typedef definitions
        - basic_type: List of basic C types
        - generator: A C code generator instance
        Returns:
        -------
        None
            The function writes the generated IDL to a file named {proname}_idl.x
        Key Functionality:
        ---------------
        1. Extracts function declarations that need RPC interfaces
        2. Handles various C types for RPC serialization:
        - Basic types
        - Structures
        - Enumerations
        - Pointers (char*, void*, etc.)
        - Arrays
        - Function pointers
        3. Generates appropriate RPC-compatible struct definitions
        4. Creates the RPC program definition with versioning
    """
    
    stat_str=r"""

struct stat_rpc
  {
    unsigned long int st_dev;
    unsigned long int st_ino;
    unsigned long int st_nlink;
    unsigned long int st_mode;
    unsigned int st_uid;
    unsigned int st_gid;
    int __pad0;
    unsigned long int st_rdev;
     long int st_size;
     long int st_blksize;
     long int st_blocks;
    long  st_atime;
     long    st_mtime;
    long st_ctime;
    unsigned long int __glibc_reserved[3];
  };"""
    
    timeval_str=r"""struct timeval_rpc
{
    long int tv_sec;
    long int tv_usec;
};"""
    def dealwith_charptr():
        typedef_str="struct char_ptr{opaque charptr<>;};"
        idl_structs_str.append(typedef_str)
        typedef_str="typedef struct char_ptr char_ptr;"
        idl_structs_str.append(typedef_str)
        typedef_struct_rpc.append("char_ptr")

    def dealwith_twocharptr():
        if "char_ptr" not in typedef_struct_rpc:
            dealwith_charptr()
        struct_str="struct TwoCharPtr{char_ptr twocharptr<>;};"
        typedef_str="typedef struct TwoCharPtr TwoCharPtr;"
        idl_structs_str.append(struct_str)
        idl_structs_str.append(typedef_str)
        typedef_struct_rpc.append("TwoCharPtr")

        
    def dealwith_enum(enumname):
        if enumname not in typedef_struct_rpc:
            enum_str="enum "+enumname+"{\n"
            if enumname in typedef_dict:
                enum_str+=generator.visit(typedef_dict[enumname]['ast'].type.type.values)
            else:
                enum_str+=generator.visit(struct_dict[enumname]['ast'].values)
            #去除最后一个逗号
            enum_str=enum_str[:-2]+"\n};"
            idl_structs_str.append(enum_str)
            #定义一个字符串“%extern enum ”+ptypearray[0]+“;”
            typedef_str="%extern enum "+enumname+";"
            idl_structs_str.insert(0,typedef_str)
            typedef_struct_rpc.append(enumname)
        # if ptypearray[0]+"_rpc" not in typedef_struct_rpc:
        #     struct_str="struct "+ptypearray[0]+"_rpc{"+ptypearray[0]+ " "+param.name+"<>;};"  
        #     typedef_str="typedef struct "+ptypearray[0]+"_rpc "+ptypearray[0]+"_rpc;"
        #     idl_structs_str.append(struct_str)
        #     idl_structs_str.append(typedef_str)
        #     typedef_struct_rpc.append(ptypearray[0]+"_rpc")
    

    
    def get_rpc_struct_str(node, strlist, processed_types=None):
        """
        递归处理结构体，生成RPC定义字符串，并避免循环引用
        
        Args:
            node: 结构体或类型的AST节点
            strlist: 输出字符串的列表
            processed_types: 已处理过的类型集合，用于避免循环引用
        """
        print(node)
        # 初始化已处理类型的集合
        if processed_types is None:
            processed_types = set()
        
        # 处理Typedef
        if isinstance(node, c_ast.Typedef):
            # 检查是否已经处理过此类型
            if node.name in processed_types:
                print(f"已处理过类型 {node.name}，避免循环引用")
                return
            
            # 标记为已处理
            processed_types.add(node.name)
            node = node.type.type

        # 结构体为空的情况
        if node.decls is None:
            print("node.decls is None")
            # 根据node.name到struct_dict中查找
            if hasattr(node, 'name') and node.name in struct_dict:
                # 检查是否已处理过此结构体
                if node.name in processed_types:
                    print(f"已处理过结构体 {node.name}，避免循环引用")
                    return
                
                # 标记为已处理
                processed_types.add(node.name)
                node = struct_dict[node.name]['ast']
            else:
                print("node.decls is None, but node.name not in struct_dict")
                strlist.append(f"opaque tempname<>;")
                return
        
        # 处理所有成员
        for decl in node.decls:
            print("处理结构体成员:",node.name," ",decl.name)
            print(decl)
            decltype = generator.visit(decl.type)
            decltypearray = decltype.split(" ")
            if len(decltypearray) > 2:
                decltype = decltypearray[1] + " " + decltypearray[2]
                decltypearray = decltypearray[1:]
            
            # 处理不同类型的成员
            if isinstance(decl.type, c_ast.TypeDecl):
                if isinstance(decl.type.type, c_ast.Struct):
                    print("处理结构体成员1:", node.name, " ", decl.name)
                    # 检查是否已处理过此结构体成员
                    struct_name = decl.type.type.name if hasattr(decl.type.type, 'name') else f"anon_struct_{decl.name}"
                    if struct_name in processed_types:
                        print("处理结构体成员2:", node.name, " ", decl.name)
                        print(f"已处理过结构体成员 {struct_name}，避免循环引用")
                        # 添加引用，而不是递归展开
                        strlist.append(f"{struct_name}_rpc {decl.name};")
                    else:
                        # 对于未处理过的结构体，可以选择添加引用或递归展开
                        # 这里选择添加引用，避免复杂嵌套
                        strlist.append(f"{struct_name} {decl.name};")
                elif isinstance(decl.type.type, c_ast.Enum):
                    print(decl)
                    strlist.append(f"int {decl.name};")
                elif isinstance(decl.type.type, c_ast.Union):
                    # 如果是union类型，跳过
                    continue
                elif decl.type.type.names[0] in basic_type:
                    strlist.append(decl.type.type.names[0] + " " + decl.name + ";")
                elif decl.type.type.names[0] =="ngx_atomic_t" or decl.type.type.names[0] == "ngx_uint_t" or decl.type.type.names[0] == "ngx_rbtree_key_t":
                    strlist.append(f"unsigned long int {decl.name};")
                elif decl.type.type.names[0] == "ngx_int_t" or decl.type.type.names[0] == "ngx_flag_t":
                    strlist.append(f"long int {decl.name};")
                elif decl.type.type.names[0]=="size_t":
                    strlist.append(f" int {decl.name};")
                elif decl.type.type.names[0] in typedef_dict:   
                    typename = decl.type.type.names[0]
                    typeast=typedef_dict[typename]['ast']
                    print(typeast)
                    if isinstance(typeast.type,c_ast.PtrDecl) and isinstance(typeast.type.type.type,c_ast.IdentifierType):
                        ptrname = typeast.type.type.type.names[0]
                        if ptrname=="void":
                            strlist.append(f"opaque {decl.name}<>;")
                            continue
                    if isinstance(typeast.type.type, c_ast.IdentifierType) and typeast.type.type.names[0] == "ngx_rbtree_key_t":
                        strlist.append(f"unsigned long int {decl.name};")
                    else:
                        if typedef_dict[typename]['isstruct']:
                            print("处理结构体成员3:", node.name, " ", decl.name)
                            if typename in processed_types:
                                print(f"已处理过typedef类型 {typename}，避免循环引用")
                                # 添加引用，而不是递归处理
                                if typedef_dict[typename]['isenum']:
                                    strlist.append(f"int {decl.name};")
                                elif typedef_dict[typename]['isstruct']:
                                    strlist.append(f"{typename}_rpc {decl.name};")
                                else:
                                    ptypename = generator.visit(typedef_dict[typename]['ast'].type)
                                    if ptypename.startswith("union"):
                                        continue
                                    strlist.append(ptypename + " " + decl.name + ";")

                            else:
                                if f"{typename}_rpc" not in typedef_struct_rpc:
                                    print(f"添加新的RPC结构体类型 {typename}_rpc")
                                    typedef_struct_rpc.append(f"{typename}_rpc")
                                    struct_def = f"struct {typename}_rpc {{\n"
                                    rpcstruct_str_list = []
                                    print("处理结构体成员4:", node.name, " ", decl.name)
                                    get_rpc_struct_str(typeast, rpcstruct_str_list, processed_types)
                                    processed_types.add(f"{typename}")
                                    for rpcstruct_str in rpcstruct_str_list:
                                        struct_def += rpcstruct_str + "\n"
                                    struct_def += "};\n"
                                    print("处理结构体成员5:", node.name, " ", decl.name)
                                    print(struct_def)
                                    idl_structs_str.append(struct_def)
                                    

                                strlist.append(f"{typename}_rpc {decl.name};")

                        elif typedef_dict[typename]['isenum']:
                            strlist.append(f"int {decl.name};")
                        elif isinstance(typedef_dict[typename]['ast'].type, c_ast.PtrDecl) and isinstance(typedef_dict[typename]['ast'].type.type, c_ast.FuncDecl):
                            if "char_ptr" not in typedef_struct_rpc:
                                dealwith_charptr()
                            strlist.append(f"char_ptr {decl.name};")
                        else:
                            print("处理结构体成员4:", node.name, " ", decl.name)
                            ptypename = generator.visit(typedef_dict[typename]['ast'].type)
                            if ptypename.startswith("union"):
                                continue
                            
                            # 如果typedef指向结构体，可能需要递归处理
                            if typedef_dict[typename]['isstruct']:
                                # 这里添加结构体引用，而不是递归展开
                                strlist.append(f"{typename} {decl.name};")
                            else:
                                strlist.append(ptypename + " " + decl.name + ";")
                else:
                    print("without dealwith")
                    # print(decl)
                    pass
                    
            elif isinstance(decl.type, c_ast.PtrDecl):  # 指针类型
                if decltype == "char *" or decltype=="FILE *" or decltype=="u_char *":  # 如果是char *类型，将其转换为string类型
                    if "char_ptr" not in typedef_struct_rpc:
                        dealwith_charptr()
                    strlist.append("char_ptr " + decl.name + ";")
                elif decltype == "void *":  # 如果是void *类型，将其转换为opaque类型
                    strlist.append("opaque " + decl.name + "<>;")
                elif decltype == "char **" or decltype == "const char **" or decltype=="void **":  # 如果是char **类型，将其转换为CharArray类型
                    # 定义CharArray，TwoCharPtr
                    if "TwoCharPtr" not in typedef_struct_rpc:
                        dealwith_twocharptr()
                    strlist.append("TwoCharPtr " + decl.name + ";")

                elif isinstance(decl.type, c_ast.PtrDecl) and isinstance(decl.type.type.type,c_ast.IdentifierType) \
                and isinstance(typedef_dict[decl.type.type.type.names[0]]['ast'].type.type, c_ast.Struct) \
                and typedef_dict[decl.type.type.type.names[0]]['ast'].type.type.name == node.name:
                    strlist.append(f"struct  {typedef_dict[decl.type.type.type.names[0]]['ast'].name}_rpc * {decl.name};")

                elif isinstance(decl.type.type,c_ast.PtrDecl):
                    if isinstance(decl.type.type.type.type,c_ast.IdentifierType):
                        typename = decl.type.type.type.type.names[0]
                        if typename in typedef_dict:
                            typeast=typedef_dict[typename]['ast']
                            if isinstance(typeast.type.type,c_ast.Struct):
                                struct_name=typeast.type.type.name
                                ##判断是否有decls属性
                                if typeast.type.type.decls is not None:
                                    structast=typeast.type.type
                                    if struct_name is None:
                                        struct_name=typeast.name
                                else:
                                    if struct_name in struct_dict:
                                        structast=struct_dict[struct_name]['ast']
                                    else:
                                        structast=None
                                        continue    

                                # 检查是否已处理过此结构体
                                if struct_name in processed_types:
                                    print(f"已处理过指针结构体 {struct_name}，避免循环引用")
                                    # 使用已有的RPC结构体定义
                                    if f"{struct_name}_rpc_twoptr" in typedef_struct_rpc:
                                        strlist.append(f"{struct_name}_rpc_twoptr {decl.name};")
                                else:
                                    # 标记为已处理
                                    
                                    
                                    # 添加RPC结构体定义
                                    if f"{struct_name}_rpc" not in typedef_struct_rpc:
                                        typedef_struct_rpc.append(f"{struct_name}_rpc")
                                        struct_def = f"struct {struct_name}_rpc {{\n"
                                        rpcstruct_str_list = []
                                        get_rpc_struct_str(structast, rpcstruct_str_list, processed_types)
                                        processed_types.add(struct_name)
                                        for rpcstruct_str in rpcstruct_str_list:
                                            struct_def += rpcstruct_str + "\n"
                                        struct_def += "};\n"
                                        idl_structs_str.append(struct_def)
                                        if f"{struct_name}_rpc_ptr" not in typedef_struct_rpc:
                                            typedef_str="typedef "+struct_name+"_rpc "+struct_name+"_rpc_ptr<>;\n"
                                            idl_structs_str.append(typedef_str)
                                            typedef_struct_rpc.append(struct_name+"_rpc_ptr")
                                            idl_structs_str.append(f"typedef struct {struct_name}_rpc_ptr {struct_name}_rpc_ptr;\n")

                                        if f"{struct_name}_rpc_twoptr" not in typedef_struct_rpc:
                                            typedef_str = f"typedef struct {struct_name}_rpc_ptr {struct_name}_rpc_twoptr<>;\n"
                                            idl_structs_str.append(typedef_str)
                                            typedef_struct_rpc.append(f"{struct_name}_rpc_twoptr")
                                    strlist.append(f"{struct_name}_rpc_twoptr {decl.name};")

                        #     print(typedef_dict[typename])
                        # strlist.append(f"char_ptr {decl.name};")
                    else:
                        print("unsupported pointer type")
                else:  # 如果是其他类型(int,float等，枚举，结构体，typedef)，
                    print(decl)
                    print(generator.visit(decl))
                    if isinstance(decl.type.type.type, c_ast.Struct):
                        struct_name = decl.type.type.type.name
                        if struct_name == "stat":
                            pass
                        else:
                            # 检查是否已处理过此结构体
                            if struct_name in processed_types:
                                print(f"已处理过指针结构体 {struct_name}，避免循环引用")
                                # 使用已有的RPC结构体定义
                                if f"{struct_name}_rpc" in typedef_struct_rpc:
                                    strlist.append(f"{struct_name}_rpc {decl.name};")
                                else:
                                    strlist.append(f"{struct_name} {decl.name};")
                            else:
                                # 标记为已处理
                                
                                
                                # 添加RPC结构体定义
                                if f"{struct_name}_rpc" not in typedef_struct_rpc:
                                    typedef_struct_rpc.append(f"{struct_name}_rpc")
                                    struct_def = f"struct {struct_name}_rpc {{\n"
                                    rpcstruct_str_list = []
                                    get_rpc_struct_str(struct_dict[struct_name]['ast'], rpcstruct_str_list, processed_types)
                                    processed_types.add(struct_name)
                                    for rpcstruct_str in rpcstruct_str_list:
                                        struct_def += rpcstruct_str + "\n"
                                    struct_def += "};\n"
                                    idl_structs_str.append(struct_def)
                                    if f"{struct_name}_rpc_ptr" not in typedef_struct_rpc:
                                        typedef_str="typedef "+struct_name+"_rpc "+struct_name+"_rpc_ptr<>;\n"
                                        idl_structs_str.append(typedef_str)
                                        typedef_struct_rpc.append(struct_name+"_rpc_ptr")
                                        idl_structs_str.append(f"typedef struct {struct_name}_rpc_ptr {struct_name}_rpc_ptr;\n")
                                
                                strlist.append(f"{struct_name}_rpc {decl.name};")
                    
                    
                    
                    elif isinstance(decl.type.type,c_ast.FuncDecl):
                        if "char_ptr" not in typedef_struct_rpc:
                            dealwith_charptr()
                        strlist.append(f"char_ptr {decl.name};")

                    elif decl.type.type.type.names[0] in basic_type:
                        # 基本类型指针，如 int*, double* 等
                        typename = decl.type.type.type.names[0]
                        if typename + "_ptr" not in typedef_struct_rpc:
                            typedef_str = f"typedef {typename} {typename}_ptr<>;"
                            idl_structs_str.append(typedef_str)
                            typedef_struct_rpc.append(typename + "_ptr")
                        strlist.append(f"{typename}_ptr {decl.name};")
                    elif decl.type.type.type.names[0] in typedef_dict:
                        # typedef类型指针
                        typename = decl.type.type.type.names[0]
                        if typedef_dict[typename]['isenum']:
                            # 枚举类型指针
                            if typename + "_ptr" not in typedef_struct_rpc:
                                typedef_str = f"typedef {typename} {typename}_ptr<>;"
                                idl_structs_str.append(typedef_str)
                                typedef_struct_rpc.append(typename + "_ptr")
                            strlist.append(f"{typename}_ptr {decl.name};")
                        elif typedef_dict[typename]['isstruct']:
                            if typename in processed_types:
                                print("结构体处理7")
                                if node.name==typedef_dict[typename]['ast'].name:
                                    strlist.append(f"struct {typename}_rpc * {decl.name};")
                                else:
                                    strlist.append(f"{typename}_rpc_ptr {decl.name};")
                                continue
                            # 确保结构体的RPC定义已经存在
                            if typename + "_rpc" not in typedef_struct_rpc:
                                print("结构体处理8")
                                typedef_struct_rpc.append(typename + "_rpc")
                                rpcstruct_str_list = []
                                rpcstruct_str_list.append(f"struct {typename}_rpc{{")
                                get_rpc_struct_str(typedef_dict[typename]['ast'], rpcstruct_str_list, processed_types)
                                processed_types.add(typename)

                                rpcstruct_str_list.append("};")
                                print("结构体处理9")
                                for rpcstruct_str in rpcstruct_str_list:
                                    idl_structs_str.append(rpcstruct_str)
                                    print(rpcstruct_str)

                                idl_structs_str.append(f"typedef struct {typename}_rpc {typename}_rpc;")
                                if typename + "_rpc_ptr" not in typedef_struct_rpc:
                                    # 为指针定义对应的RPC类型
                                    typedef_str = f"typedef {typename}_rpc {typename}_rpc_ptr<>;"
                                    idl_structs_str.append(typedef_str)
                                    typedef_struct_rpc.append(typename + "_rpc_ptr")
                            strlist.append(f"{typename}_rpc_ptr {decl.name};")
                        else:
                            # 其他typedef类型
                            ptypename = generator.visit(typedef_dict[typename]['ast'].type)
                            if ptypename in basic_type:
                                if ptypename + "_ptr" not in typedef_struct_rpc:
                                    typedef_str = f"typedef {ptypename} {ptypename}_ptr<>;"
                                    idl_structs_str.append(typedef_str)
                                    typedef_struct_rpc.append(ptypename + "_ptr")
                                strlist.append(f"{ptypename}_ptr {decl.name};")
                            else:
                                # 使用char_ptr作为后备
                                if "char_ptr" not in typedef_struct_rpc:
                                    dealwith_charptr()
                                strlist.append("char_ptr " + decl.name + ";")
                    elif decl.type.type.type.names[0] in struct_dict:
                        # 结构体类型指针
                        typename = decl.type.type.type.names[0]
                        if typename == "stat":
                            if "stat_rpc_ptr" not in typedef_struct_rpc:
                                if "stat_rpc" not in typedef_struct_rpc:
                                    idl_structs_str.append(stat_str)
                                    typedef_struct_rpc.append("stat_rpc")
                                typedef_str = "typedef stat_rpc stat_rpc_ptr<>;"
                                idl_structs_str.append(typedef_str)
                                typedef_struct_rpc.append("stat_rpc_ptr")
                            strlist.append(f"stat_rpc_ptr {decl.name};")
                        elif struct_dict[typename]['isenum']:
                            # 枚举类型指针
                            if typename + "_ptr" not in typedef_struct_rpc:
                                typedef_str = f"typedef {typename} {typename}_ptr<>;"
                                idl_structs_str.append(typedef_str)
                                typedef_struct_rpc.append(typename + "_ptr")
                            strlist.append(f"{typename}_ptr {decl.name};")
                        elif struct_dict[typename]['isstruct']:
                            # 结构体指针
                            
                                # 确保结构体的RPC定义已经存在
                                
                            if typename + "_rpc" not in typedef_struct_rpc:
                                typedef_struct_rpc.append(typename + "_rpc")
                                rpcstruct_str_list = []
                                rpcstruct_str_list.append(f"struct {typename}_rpc{{")
                                get_rpc_struct_str(struct_dict[typename]['ast'], rpcstruct_str_list, processed_types)
                                processed_types.add(typename)
                                rpcstruct_str_list.append("};")
                                
                                for rpcstruct_str in rpcstruct_str_list:
                                    idl_structs_str.append(rpcstruct_str)
                                idl_structs_str.append(f"typedef struct {typename}_rpc {typename}_rpc;")
                                if typename + "_rpc_ptr" not in typedef_struct_rpc:
                                    typedef_struct_rpc.append(typename + "_rpc_ptr")
                                    # 为指针定义对应的RPC类型
                                    typedef_str = f"typedef {typename}_rpc {typename}_rpc_ptr<>;"
                                    idl_structs_str.append(typedef_str)
                                
                            strlist.append(f"{typename}_rpc_ptr {decl.name};")
                        else:
                            # 其他类型，使用char_ptr作为后备
                            if "char_ptr" not in typedef_struct_rpc:
                                dealwith_charptr()
                            strlist.append("char_ptr " + decl.name + ";")
                        
                    elif isinstance(decl.type.type, c_ast.FuncDecl):
                        # 函数指针 - 转换为char_ptr类型
                        if "char_ptr" not in typedef_struct_rpc:
                            dealwith_charptr()
                        strlist.append("char_ptr " + decl.name + ";")
                    else:
                        print(decl)
                        print("without dealwith2")
                
            elif isinstance(decl.type, c_ast.ArrayDecl):  # 数组类型
                if decltypearray[0] in basic_type:
                    strlist.append(generator.visit(decl) + ";")
                else:
                    print("without dealwith3")
                    # print(decl)
                pass

    def dealwith_return(param):
        ptype=generator.visit(param)
        ptypearray=ptype.split(" ")
        if len(ptypearray)>2:
            ptype=ptypearray[1]+" "+ptypearray[2]
            ptypearray=ptypearray[1:]
        print(ptype)
        #判断参数类型，是基本类型，数组类型还是指针类型
        if isinstance(param,c_ast.TypeDecl):
            # print(param)
            if ptype in basic_type and ptype!="void":
                # funstr+=ptype+","
                # funstr_list.append(ptype)
                tempstr=ptype+" ret0;"
                retstr_list.append(tempstr)
            else:
                # print(param)
                if isinstance(param.type,c_ast.Struct):
                    if param.type.type.name=="stat":
                        if "stat_rpc" not in typedef_struct_rpc:
                            idl_structs_str.append(stat_str)
                            typedef_struct_rpc.append("stat_rpc")
                        # funstr+="stat_rpc,"
                        # funstr_list.append("stat_rpc,")
                        tempstr="stat_rpc ret0;"
                        retstr_list.append(tempstr)
                    else:
                        rpcstruct_str_list=[]
                        if param.type.type.name+"_rpc" not in typedef_struct_rpc:
                            typedef_struct_rpc.append(param.type.type.name+"_rpc")
                            rpcstruct_str_list.append("struct "+param.type.type.name+"_rpc{")
                            get_rpc_struct_str(struct_dict[param.type.type.name]['ast'],rpcstruct_str_list)
                            rpcstruct_str_list.append("};")
                            
                        for rpcstruct_str in rpcstruct_str_list:
                            idl_structs_str.append(rpcstruct_str)
                        # funstr_list.append(param.type.type.name+"_rpc,")
                        tempstr=param.type.type.name+"_rpc ret0;"
                        retstr_list.append(tempstr)
                elif isinstance(param.type,c_ast.Enum):
                    dealwith_enum(param.type.type.name)
                else:
                    if ptype in typedef_dict:
                        if typedef_dict[ptype]['isenum']:
                            dealwith_enum(ptype)
                            # funstr_list.append(ptype)
                            retstr_list.append(ptype+" ret0;")
                        else:
                            ptypename=generator.visit(typedef_dict[ptype]['ast'].type)
                            # funstr_list.append(ptypename+",")
                            retstr_list.append(ptypename+" ret0;")
                    else:
                        print("error")
                    pass
        elif isinstance(param,c_ast.PtrDecl) or isinstance(param,c_ast.ArrayDecl): #指针或者数组类型
            # print("Pointer")
            #构建一个结构体，构体的字段是指针指向的类型，结构体的名字是指针指向的类型+param.name,如下所示：
            #  struct main_sense_ptr{int ptr<>;}; main_sense是函数名，ptr是参数名，int是指针指向的类型
            #  typedef struct main_sense_ptr main_sense_ptr;
            if ptype=="char *": #如果是char *类型，将其转换为string类型
                # funstr+="string,"
                if "char_ptr" not in typedef_struct_rpc:
                    dealwith_charptr()
                # funstr_list.append("char_ptr,")
                retstr_list.append("char_ptr ret0;")
            elif ptype=="char **" or ptype=="const char **": #如果是char **类型，将其转换为CharArray类型
                """typedef string CharArray<>;
                    struct TwoCharPtr{
                        CharArray twocharptr<>;
                    };
                    typedef struct TwoCharPtr TwoCharPtr;"""
                #定义CharArray，TwoCharPtr
                if "TwoCharPtr" not in typedef_struct_rpc:
                    dealwith_twocharptr()
                # funstr+="TwoCharPtr,"
                # funstr_list.append("TwoCharPtr,")
                retstr_list.append("TwoCharPtr ret0;")
            else: #如果是其他类型(int,float等，枚举，结构体，typedef)，将其转换为结构体类型
                
                if ptypearray[0] in basic_type:
                    if ptypearray[0]+"_ptr" not in typedef_struct_rpc:
                        typedef_str="typedef "+ptypearray[0]+" "+ptypearray[0]+"_ptr<>;"
                        typedef_struct_rpc.append(ptypearray[0]+"_ptr")
                        idl_structs_str.append(typedef_str)
                    
                    # funstr+=ptypearray[0]+"_ptr,"
                    # funstr_list.append(ptypearray[0]+"_ptr,")
                    retstr_list.append(ptypearray[0]+"_ptr ret0;")
                    
                else:#有可能是结构体，枚举类型，或者typedef的类型
                    #先到typedef_dict中查找，如果找到，判断其是否是结构体或者枚举类型，如果是枚举类型，定义其对应的字符串，加入到idl_structs_str中
                    if ptypearray[0] in typedef_dict:
                        ptypename=generator.visit(typedef_dict[ptypearray[0]]['ast'].type)
                        if ptypename in basic_type:
                            if ptypename+"_ptr" not in typedef_struct_rpc:
                                typedef_str="typedef "+ptypename+" "+ptypename+"_ptr<>;"
                                typedef_struct_rpc.append(ptypename+"_ptr")
                                idl_structs_str.append(typedef_str)
                            # funstr+=ptypename+"_ptr,"
                            # funstr_list.append(ptypename+"_ptr,")
                            retstr_list.append(ptypename+"_ptr ret0;")

                        elif typedef_dict[ptypearray[0]]['isstruct']:
                            pass
                        elif typedef_dict[ptypearray[0]]['isenum']:
                            if ptypearray[0] not in typedef_struct_rpc:
                                enum_str="enum "+ptypearray[0]+"{\n"
                                enum_str+=generator.visit(typedef_dict[ptypearray[0]]['ast'].type.type.values)
                                #去除最后一个逗号
                                enum_str=enum_str[:-2]+"\n};"
                                idl_structs_str.append(enum_str)
                                #定义一个字符串“%extern enum ”+ptypearray[0]+“;”
                                typedef_str="%extern enum "+ptypearray[0]+";"
                                idl_structs_str.insert(0,typedef_str)
                                typedef_struct_rpc.append(ptypearray[0])
                            if ptypearray[0]+"_ptr" not in typedef_struct_rpc:
                                # struct_str="struct "+ptypearray[0]+"_rpc{"+ptypearray[0]+ " "+param.name+"<>;};"  
                                # typedef_str="typedef struct "+ptypearray[0]+"_rpc "+ptypearray[0]+"_rpc;"
                                typedef_str="typedef "+ptypearray[0]+" "+ptypearray[0]+"_ptr<>;"
                                idl_structs_str.append(typedef_str)
                                typedef_struct_rpc.append(ptypearray[0]+"_ptr")
                            # funstr+=ptypearray[0]+"_rpc,"
                            # funstr_list.append(ptypearray[0]+"_rpc,")
                            retstr_list.append(ptypearray[0]+"_ptr ret0;")
                    #如果在typedef_dict中没有找到，到struct_dict中查找，如果找到，判断其是否是结构体或者枚举类型，如果是枚举类型，定义其对应的字符串，加入到idl_structs_str中
                    elif ptypearray[0] in struct_dict:
                        if struct_dict[ptypearray[0]]['isstruct']:
                            #写一个递归函数，遍历结构体的每一个字段，如果字段是基本类型，将其加入到rpcstruct_str_list中，如果字段是结构体，枚举，
                            # 二级指针，递归定义其对应的结构体，枚举，二级指针
                            rpcstruct_str_list=[]
                            if ptypearray[0]+"_rpc" not in typedef_struct_rpc:
                                typedef_struct_rpc.append(ptypearray[0]+"_rpc")
                                rpcstruct_str_list.append("struct "+ptypearray[0]+"_rpc{")
                                get_rpc_struct_str(struct_dict[ptypearray[0]]['ast'],rpcstruct_str_list)
                                rpcstruct_str_list.append("};")
                                
                            for rpcstruct_str in rpcstruct_str_list:
                                idl_structs_str.append(rpcstruct_str)
                            retstr_list.append(ptypearray[0]+"_rpc ret0;")  
                            # struct_str="struct "+func_name+"_ret_"+"{"+ptypearray[0]+"_rpc ret_<>;};"
                            # typedef_str="typedef struct "+func_name+"_ret_ "+func_name+"_ret_;"
                            # idl_structs_str.append(struct_str)
                            # idl_structs_str.append(typedef_str)
                            # # funstr+=func_name+"_"+param.name+","
                            
                            # funstr_list.append(func_name+"_ret_")
                        elif struct_dict[ptypearray[0]]['isenum']:
                            pass   
    
    def dealwith_basicptr_idl_param(typename,param):
        if typename+"_ptr" not in typedef_struct_rpc:
            typedef_str="typedef "+typename+" "+typename+"_ptr<>;"
            typedef_struct_rpc.append(typename+"_ptr")
            idl_structs_str.append(typedef_str)
        funstr_list.append(typename+"_ptr,")
        retstr_list.append(typename+"_ptr "+param.name+";")

    def dealwith_Param(param):
        if isinstance(param,c_ast.EllipsisParam):
            print("变参函数")
            return
        ptype=generator.visit(param.type)
        ptype=param.type
        ptypename=generator.visit(ptype)
        #判断参数类型，是基本类型，数组类型还是指针类型
        print("idl param type:")
        print(param)
        if isinstance(param.type,c_ast.TypeDecl):
            # print(param)
            if ptypename in basic_type:
                # funstr+=ptype+","
                funstr_list.append(ptypename+",")
            else:
                if isinstance(param.type.type,c_ast.Struct):
                    # 处理匿名结构体的情况
                    if param.type.type.name is None:
                        # 匿名结构体：为其生成一个唯一的名字
                        anon_struct_name = f"anon_struct_{param.name}_{hash(str(param.type.type)) % 10000}"
                        
                        if anon_struct_name + "_rpc" not in typedef_struct_rpc:
                            typedef_struct_rpc.append(anon_struct_name + "_rpc")
                            rpcstruct_str_list = []
                            rpcstruct_str_list.append(f"struct {anon_struct_name}_rpc{{")
                            # 直接处理匿名结构体的字段
                            get_rpc_struct_str(param.type.type, rpcstruct_str_list)
                            rpcstruct_str_list.append("};")
                            
                            
                            for rpcstruct_str in rpcstruct_str_list:
                                idl_structs_str.append(rpcstruct_str)
                        
                        funstr_list.append(anon_struct_name + "_rpc,")
                    elif param.type.type.name=="stat":
                        if "stat_rpc" not in typedef_struct_rpc:
                            idl_structs_str.append(stat_str)
                            typedef_struct_rpc.append("stat_rpc")
                        # funstr+="stat_rpc,"
                        funstr_list.append("stat_rpc,")
                    else:
                        rpcstruct_str_list=[]
                        if param.type.type.name+"_rpc" not in typedef_struct_rpc:
                            typedef_struct_rpc.append(param.type.type.name+"_rpc")
                            rpcstruct_str_list.append("struct "+param.type.type.name+"_rpc{")
                            get_rpc_struct_str(struct_dict[param.type.type.name]['ast'],rpcstruct_str_list)
                            rpcstruct_str_list.append("};")
                            
                        for rpcstruct_str in rpcstruct_str_list:
                            idl_structs_str.append(rpcstruct_str)
                        # funstr+=param.type.type.name+"_rpc,"
                        funstr_list.append(param.type.type.name+"_rpc,")
                elif isinstance(param.type.type,c_ast.Enum):
                    dealwith_enum(param.type.type.name)
                else:
                    if ptypename in typedef_dict:
                        if typedef_dict[ptypename]['isenum']:
                            dealwith_enum(ptypename)
                            funstr_list.append(ptypename+",")
                        else:
                            ptypename=generator.visit(typedef_dict[ptypename]['ast'].type)
                            funstr_list.append(ptypename+",")
                    else:
                        print("error")
                    pass
        elif isinstance(param.type,c_ast.PtrDecl): #指针或者数组类型
            # print("Pointer")
            #构建一个结构体，构体的字段是指针指向的类型，结构体的名字是指针指向的类型+param.name,如下所示：
            #  struct main_sense_ptr{int ptr<>;}; main_sense是函数名，ptr是参数名，int是指针指向的类型
            #  typedef struct main_sense_ptr main_sense_ptr;
            if ptypename=="char *" or ptypename=="const char *" or ptypename=="FILE *": #如果是char *类型，将其转换为string类型
                if "char_ptr" not in typedef_struct_rpc:
                    dealwith_charptr()
                funstr_list.append("char_ptr,")
                retstr_list.append("char_ptr "+param.name+";")
            elif ptypename=="struct timeval *": #如果是struct timeval *类型，将其转换为timeval_rpc_ptr类型
                if "timeval_rpc_ptr" not in typedef_struct_rpc:
                    idl_structs_str.append(timeval_str)
                    typedef_str="typedef struct timeval_rpc timeval_rpc_ptr<>;"
                    idl_structs_str.append(typedef_str)
                    typedef_struct_rpc.append("timeval_rpc")
                    typedef_struct_rpc.append("timeval_rpc_ptr")
                funstr_list.append("timeval_rpc_ptr,")
                retstr_list.append("timeval_rpc_ptr "+param.name+";")
            elif ptypename=="char **" or ptypename=="const char **": #如果是char **类型，将其转换为CharArray类型
                """typedef string CharArray<>;
                    struct TwoCharPtr{
                        CharArray twocharptr<>;
                    };
                    typedef struct TwoCharPtr TwoCharPtr;"""
                #定义CharArray，TwoCharPtr
                if "TwoCharPtr" not in typedef_struct_rpc:
                    dealwith_twocharptr()
                # funstr+="TwoCharPtr,"
                funstr_list.append("TwoCharPtr,")
                retstr_list.append("TwoCharPtr "+param.name+";")
            else: #如果是其他类型(int,float等，枚举，结构体，typedef)，将其转换为结构体类型
                print("this eles")
                print(ptypename)
                typename=ptypename.split(" ")[-2]
                print(typename)
                print(param)
                    
                if typename in basic_type:
                    dealwith_basicptr_idl_param(typename,param)
                elif typename in typedef_dict:
                    if typedef_dict[typename]['isenum']:
                        dealwith_basicptr_idl_param(typename,param)
                    elif typedef_dict[typename]['isstruct']:
                        # 处理指向结构体的指针，其中结构体来自typedef定义
                        if typename+"_rpc_ptr" not in typedef_struct_rpc:
                            # 确保结构体的RPC定义已经存在
                            rpcstruct_str_list=[]
                            if typename+"_rpc" not in typedef_struct_rpc:
                                typedef_struct_rpc.append(typename+"_rpc_ptr")
                                typedef_struct_rpc.append(typename+"_rpc")
                                rpcstruct_str_list.append("struct "+typename+"_rpc{")
                                print(typedef_dict[typename]['ast'])
                                get_rpc_struct_str(typedef_dict[typename]['ast'].type.type,rpcstruct_str_list)
                                rpcstruct_str_list.append("};")
                                
                                for rpcstruct_str in rpcstruct_str_list:
                                    idl_structs_str.append(rpcstruct_str)
                                idl_structs_str.append("typedef struct "+typename+"_rpc "+typename+"_rpc;")
                            
                            # 为指针定义对应的RPC类型
                            typedef_str="typedef "+typename+"_rpc "+typename+"_rpc_ptr<>;"
                            idl_structs_str.append(typedef_str)
                            
                        
                        funstr_list.append(typename+"_rpc_ptr,")
                        retstr_list.append(typename+"_rpc_ptr "+param.name+";")
                    else:
                        #如果只函数指针类型，则return
                        # print(typedef_dict[typename]['ast'])
                        if isinstance(typedef_dict[typename]['ast'].type.type,c_ast.FuncDecl):
                            return
                        typename=typedef_dict[typename]['ast'].type.type.names[0]
                        if typename in basic_type:
                            dealwith_basicptr_idl_param(typename,param)
                        elif typename in struct_dict:
                            if typename=="stat":
                                if "stat_rpc_ptr" not in typedef_struct_rpc:
                                    if "stat_rpc" not in typedef_struct_rpc:
                                        idl_structs_str.append(stat_str)
                                        typedef_struct_rpc.append("stat_rpc")
                                    typedef_str="typedef stat_rpc stat_rpc_ptr<>;"
                                    idl_structs_str.append(typedef_str)
                                    typedef_struct_rpc.append("stat_rpc_ptr")
                                funstr_list.append("stat_rpc_ptr,")
                                retstr_list.append("stat_rpc_ptr "+param.name+";")
                            elif struct_dict[typename]['isenum']:
                                dealwith_basicptr_idl_param(typename,param)
                            elif struct_dict[typename]['isstruct']:
                                if typename+"_rpc_ptr" not in typedef_struct_rpc:
                                    # 确保结构体的RPC定义已经存在
                                    rpcstruct_str_list=[]
                                    if typename+"_rpc" not in typedef_struct_rpc:
                                        typedef_struct_rpc.append(typename+"_rpc_ptr")
                                        typedef_struct_rpc.append(typename+"_rpc")
                                        rpcstruct_str_list.append("struct "+typename+"_rpc{")
                                        get_rpc_struct_str(struct_dict[typename]['ast'],rpcstruct_str_list)
                                        rpcstruct_str_list.append("};")
                                        
                                        for rpcstruct_str in rpcstruct_str_list:
                                            idl_structs_str.append(rpcstruct_str)
                                        idl_structs_str.append("typedef struct "+typename+"_rpc "+typename+"_rpc;")
                                    
                                    # 为指针定义对应的RPC类型
                                    typedef_str="typedef "+typename+"_rpc "+typename+"_rpc_ptr<>;"
                                    idl_structs_str.append(typedef_str)
                                    
                                
                                funstr_list.append(typename+"_rpc_ptr,")
                                retstr_list.append(typename+"_rpc_ptr "+param.name+";")
                            else:
                                # 处理其他未知类型
                                print(f"警告: {typename} 是未知的结构体类型")
                                dealwith_basicptr_idl_param("void", param)  # 使用void作为后备类型
                        else:
                            # 处理其他未知类型
                            typeast=typedef_dict[typename]['ast']
                            newtypename=generator.visit(typeast.type.type)
                            if newtypename in basic_type:  
                                dealwith_basicptr_idl_param(newtypename,param)
                            else:
                                if newtypename in typedef_dict:
                                    newtypeast=typedef_dict[newtypename]['ast']
                                    if newtypeast.type.type.names[0] in basic_type:
                                        dealwith_basicptr_idl_param(newtypeast.type.type.names[0],param)
                elif typename in struct_dict:
                    print("this here")
                    if typename=="stat":
                        if "stat_rpc_ptr" not in typedef_struct_rpc:
                            if "stat_rpc" not in typedef_struct_rpc:
                                idl_structs_str.append(stat_str)
                                typedef_struct_rpc.append("stat_rpc")
                            typedef_str="typedef stat_rpc stat_rpc_ptr<>;"
                            idl_structs_str.append(typedef_str)
                            typedef_struct_rpc.append("stat_rpc_ptr")
                        funstr_list.append("stat_rpc_ptr,")
                        retstr_list.append("stat_rpc_ptr "+param.name+";")
                    elif struct_dict[typename]['isenum']:
                        dealwith_basicptr_idl_param(typename,param)
                    elif struct_dict[typename]['isstruct']:
                        print("结构体指针参数")
                        if typename+"_rpc_ptr" not in typedef_struct_rpc:
                            # 确保结构体的RPC定义已经存在
                            rpcstruct_str_list=[]
                            if typename+"_rpc" not in typedef_struct_rpc:
                                typedef_struct_rpc.append(typename+"_rpc")
                                typedef_struct_rpc.append(typename+"_rpc_ptr")
                                rpcstruct_str_list.append("struct "+typename+"_rpc{")
                                get_rpc_struct_str(struct_dict[typename]['ast'],rpcstruct_str_list)
                                rpcstruct_str_list.append("};")
                                
                                for rpcstruct_str in rpcstruct_str_list:
                                    idl_structs_str.append(rpcstruct_str)
                                idl_structs_str.append("typedef struct "+typename+"_rpc "+typename+"_rpc;")
                            
                            # 为指针定义对应的RPC类型
                            typedef_str="typedef "+typename+"_rpc "+typename+"_rpc_ptr<>;"
                            idl_structs_str.append(typedef_str)
                            
                        
                        funstr_list.append(typename+"_rpc_ptr,")
                        retstr_list.append(typename+"_rpc_ptr "+param.name+";")
                    else:
                        # 处理其他未知类型
                        print(f"警告: {typename} 是未知的结构体类型")
                        dealwith_basicptr_idl_param("void", param)  # 使用void作为后备类型

                #===============
                
                # if ptypearray[0] in basic_type:
                #     if ptypearray[0]+"_ptr" not in typedef_struct_rpc:
                #         typedef_str="typedef "+ptypearray[0]+" "+ptypearray[0]+"_ptr<>;"
                #         typedef_struct_rpc.append(ptypearray[0]+"_ptr")
                #         idl_structs_str.append(typedef_str)
                    
                #     # funstr+=ptypearray[0]+"_ptr,"
                #     funstr_list.append(ptypearray[0]+"_ptr,")
                #     retstr_list.append(ptypearray[0]+"_ptr "+param.name+";")
                    
                # else:#有可能是结构体，枚举类型，或者typedef的类型
                #     #先到typedef_dict中查找，如果找到，判断其是否是结构体或者枚举类型，如果是枚举类型，定义其对应的字符串，加入到idl_structs_str中
                #     if ptypearray[0] in typedef_dict:
                #         ptypename=generator.visit(typedef_dict[ptypearray[0]]['ast'].type)
                #         if ptypename in basic_type:
                #             if ptypename+"_ptr" not in typedef_struct_rpc:
                #                 typedef_str="typedef "+ptypename+" "+ptypename+"_ptr<>;"
                #                 typedef_struct_rpc.append(ptypename+"_ptr")
                #                 idl_structs_str.append(typedef_str)
                #             # funstr+=ptypename+"_ptr,"
                #             funstr_list.append(ptypename+"_ptr,")
                #             retstr_list.append(ptypename+"_ptr "+param.name+";")
                #         elif typedef_dict[ptypearray[0]]['isstruct']:
                #             pass
                #         elif typedef_dict[ptypearray[0]]['isenum']:
                #             if ptypearray[0] not in typedef_struct_rpc:
                #                 enum_str="enum "+ptypearray[0]+"{\n"
                #                 enum_str+=generator.visit(typedef_dict[ptypearray[0]]['ast'].type.type.values)
                #                 #去除最后一个逗号
                #                 enum_str=enum_str[:-2]+"\n};"
                #                 idl_structs_str.append(enum_str)
                #                 #定义一个字符串“%extern enum ”+ptypearray[0]+“;”
                #                 typedef_str="%extern enum "+ptypearray[0]+";"
                #                 idl_structs_str.insert(0,typedef_str)
                #                 typedef_struct_rpc.append(ptypearray[0])
                #             if ptypearray[0]+"_ptr" not in typedef_struct_rpc:
                #                 # struct_str="struct "+ptypearray[0]+"_rpc{"+ptypearray[0]+ " "+param.name+"<>;};"  
                #                 # typedef_str="typedef struct "+ptypearray[0]+"_rpc "+ptypearray[0]+"_rpc;"
                #                 typedef_str="typedef "+ptypearray[0]+" "+ptypearray[0]+"_ptr<>;"
                #                 idl_structs_str.append(typedef_str)
                #                 typedef_struct_rpc.append(ptypearray[0]+"_ptr")
                #             # funstr+=ptypearray[0]+"_rpc,"
                #             funstr_list.append(ptypearray[0]+"_ptr,")
                #             retstr_list.append(ptypearray[0]+"_ptr "+param.name+";")
                #         else:
                #             print(typedef_dict[ptypearray[0]])
                #     #如果在typedef_dict中没有找到，到struct_dict中查找，如果找到，判断其是否是结构体或者枚举类型，如果是枚举类型，定义其对应的字符串，加入到idl_structs_str中
                #     elif ptypearray[0] in struct_dict:
                #         if struct_dict[ptypearray[0]]['isstruct']:
                #             #写一个递归函数，遍历结构体的每一个字段，如果字段是基本类型，将其加入到rpcstruct_str_list中，如果字段是结构体，枚举，
                #             # 二级指针，递归定义其对应的结构体，枚举，二级指针
                #             rpcstruct_str_list=[]
                #             if ptypearray[0]+"_rpc" not in typedef_struct_rpc:
                #                 rpcstruct_str_list.append("struct "+ptypearray[0]+"_rpc{")
                #                 get_rpc_struct_str(struct_dict[ptypearray[0]]['ast'],rpcstruct_str_list)
                #                 rpcstruct_str_list.append("};")
                #                 typedef_struct_rpc.append(ptypearray[0]+"_rpc")
                #             for rpcstruct_str in rpcstruct_str_list:
                #                 idl_structs_str.append(rpcstruct_str)
                #             if ptypearray[0]+"_rpc_ptr" not in typedef_struct_rpc:
                #                 typedef_str="typedef "+ptypearray[0]+"_rpc "+ptypearray[0]+"_rpc_ptr<>;"
                #                 typedef_struct_rpc.append(ptypearray[0]+"_rpc_ptr")
                #                 idl_structs_str.append(typedef_str)
                #             # struct_str="struct "+func_name+"_"+param.name+"{"+ptypearray[0]+"_rpc "+param.name+"<>;};"
                #             # typedef_str="typedef struct "+func_name+"_"+param.name+" "+func_name+"_"+param.name+";"
                #             # idl_structs_str.append(struct_str)
                #             # idl_structs_str.append(typedef_str)
                #             # funstr+=func_name+"_"+param.name+","
                #             # funstr_list.append(func_name+"_"+param.name+",")
                #             funstr_list.append(ptypearray[0]+"_rpc_ptr,")
                #             retstr_list.append(ptypearray[0]+"_rpc_ptr "+param.name+";")
                #         elif struct_dict[ptypearray[0]]['isenum']:
                #             pass                                                                                                        

                #             #
                #             # idl_structs_str.append(typedef_str)
                #             # funstr+=func_name+"_"+param.name+","
        elif isinstance(param.type,c_ast.ArrayDecl):
            ptypearray=generator.visit(param.type).split(" ")
            if ptypearray[0] in basic_type:
                struct_str="struct "+func_name+"_"+param.name+"{"+generator.visit(param)+";};"
                typedef_str="typedef struct "+func_name+"_"+param.name+" "+func_name+"_"+param.name+";"
                idl_structs_str.append(struct_str)
                idl_structs_str.append(typedef_str)
                funstr_list.append(func_name+"_"+param.name+",")
                retstr_list.append(func_name+"_"+param.name+" "+param.name+";")
            else:
                print("array without dealwith")
        else:
            print("Unknown")
        # print(funstr)
        # return funstr
        
    #================================================================================================
    sense_sub_functions_decl=set()
    #遍历boundry_call_need_rpc，提取其中的callee，根据callee在fun_fundeclast_dict中找到对应的ast，将其加入到sense_sub_functions_decl中
    #(caller, callee, locinfo)
    for caller,callee,locinfo in boundry_call_need_rpc:
        if callee in fun_fundeclast_dict:
            sense_sub_functions_decl.add(fun_fundeclast_dict[callee])

    generator = c_generator.CGenerator()
    idl_structs_str=[]
    idl_rpc_func_str=[]
    proname=sys.argv[1]

    

    #遍历sub_functions_decl,获得每个函数的函数声明，获得函数名，返回值类型，参数列表
    funcount=1
    for func in sense_sub_functions_decl:
        """
        generate_wrapper_file:
    logputs
    iread
    make_string_hash_table
    dot_finish_sense_1
    mkalldirs_sense_1
    logvprintf_sense_2
    logprintf
    get_contents_sense_1
    logvprintf_sense_1
    skip_lws
    gethttp_sense_1
    http_loop_sense_1
        """

        """
        
        
        """
        # if func.name!="http_loop_sense_1":
        #     continue
        # if funcount<6:
        #     funcount+=1
        #     continue
        # print(generator.visit(func))
        funstr_list=[]
        retstr_list=[]
        func_name=func.name
        #获得返回值类型
        structstr="struct "+func_name+"_ret_{"
        retstr_list.append(structstr)
        dealwith_return(func.type.type)
        
        
        funstr_list.append(func_name+"_ret_ ")
        funstr_list.append(func_name+"_rpc(")
        for param in func.type.args.params:
            dealwith_Param(param)
        retstr_list.append("};")
        typedef_str="typedef struct "+func_name+"_ret_ "+func_name+"_ret_;"
        retstr_list.append(typedef_str)
        for retstr in retstr_list:
            idl_structs_str.append(retstr)
        funstr=" ".join(funstr_list)
        funstr=funstr[:-1]+")="+str(funcount)+";" 
        idl_rpc_func_str.append(funstr)
        funcount+=1
    with open(proname+"_idl.x", "w") as f:
        for struct_str in idl_structs_str:
            if "char_ptr string" in struct_str:
                struct_str=struct_str.replace("string","string1")
            f.write(struct_str+"\n")
        f.write("\n")

        #wget-1.18 此时proname
        """去除proname找那个的数字，-,."""
        newproname= re.sub(r'[\d\-\.\s]', '', proname)  #去除数字，-和.，空格

        #将proname转换为大写
        
        f.write("program RPC"+newproname.upper()+" {\n")
        f.write("   version RPC"+newproname.upper()+"TVERS {\n")
        for funstr in idl_rpc_func_str:
            f.write("       "+funstr+"\n")
        f.write("   } = 1;\n")
        f.write("} = 0x20000001;\n")



def generate_wrapper_file(boundry_call_need_rpc):
    """
    Generate RPC wrapper codes for boundary functions.
    This function creates wrapper code for functions that need to be accessed via RPC.
    It processes function declarations and generates appropriate client and server code
    for marshaling and unmarshaling data across the RPC boundary.
    The function handles various data types including:
    - Basic types (int, char, etc.)
    - Pointers (char*, void*, etc.)
    - Arrays
    - Structures
    - Enumerations
    - Typedefs
    For each function that requires RPC, it:
    1. Generates client-side wrapper functions that pack arguments for RPC calls
    2. Generates server-side functions that unpack arguments, call the original function,
        and pack the results for the return trip
    3. Handles memory allocation/deallocation for complex data structures
    Args:
         boundry_call_need_rpc: List of tuples (caller, callee, locinfo) representing
                                      function calls that need to be converted to RPC calls
    Returns:
         None, but produces wrapper code strings that can be written to files
    """
    
    # def get_rpc_struct_str(node,rpcname,rawname,argname,isptr):
    def get_rpc_struct_str(node,leftname,righname,strlist,isptr,isRpcToRaw):
        #node的decls中都是Decl类型，判断Decl是指针类型，数组类型，基本类型，结构体类型，枚举类型
        for decl in node.decls:
            decltype=generator.visit(decl.type)
            decltypearray=decltype.split(" ")
            if len(decltypearray)>2:
                decltype=decltypearray[1]+" "+decltypearray[2]
                decltypearray=decltypearray[1:]
            if isinstance(decl.type,c_ast.TypeDecl):
                if isinstance(decl.type.type,c_ast.Struct):
                    # get_rpc_struct_str(decl.type.type,decl.type.type.name)
                    pass
                elif isinstance(decl.type.type,c_ast.Enum):
                    if isptr:
                        strlist.append(leftname+"->"+decl.name+"="+righname+"->"+decl.name+";")
                        # wrapper_str_list.append(rpcname+"->"+decl.name+"="+rawname+"->"+decl.name+";")
                        # unmashal_str_list.append(rawname+"->"+decl.name+"="+argname+"->"+decl.name+";")
                    else:
                        # wrapper_str_list.append(rpcname+"."+decl.name+"="+rawname+"."+decl.name+";")
                        # unmashal_str_list.append(rawname+"."+decl.name+"="+argname+"."+decl.name+";")
                        strlist.append(leftname+"."+decl.name+"="+righname+"."+decl.name+";")
                
                    pass
                elif decl.type.type.names[0] in basic_type:
                    # strlist.append(generator.visit(decl)+";")
                    """
                    http_loop_sense_1_1_hstat.len=hstat.len;
                    """
                    #客户端封装
                    if isptr:
                        # wrapper_str_list.append(rpcname+"->"+decl.name+"="+rawname+"->"+decl.name+";")
                        # unmashal_str_list.append(rawname+"->"+decl.name+"="+argname+"->"+decl.name+";")
                        strlist.append(leftname+"->"+decl.name+"="+righname+"->"+decl.name+";")
                    else:
                        # wrapper_str_list.append(rpcname+"."+decl.name+"="+rawname+"."+decl.name+";")
                        # #服务端解封装
                        # unmashal_str_list.append(rawname+"."+decl.name+"="+argname+"."+decl.name+";")
                        strlist.append(leftname+"."+decl.name+"="+righname+"."+decl.name+";")


                elif decl.type.type.names[0] in typedef_dict:
                    if typedef_dict[decl.type.type.names[0]]['isenum']:
                        pass
                        # dealwith_enum(decl.type.type.names[0])
                        # strlist.append(generator.visit(decl)+";")
                    else:
                        ptypename=generator.visit(typedef_dict[decl.type.type.names[0]]['ast'].type)
                        # strlist.append(ptypename+" "+decl.name+";")
                else:

                    print("without dealwith")
                    # print(decl)
                    pass
            elif isinstance(decl.type,c_ast.PtrDecl):#指针类型
                if decltype=="char *": #如果是char *类型，将其转换为string类型
                    """
                    if (hstat.newloc == NULL)
                    hstat_rpc.newloc.charptr.charptr_len = 0;
                    else
                    hstat_rpc.newloc.charptr.charptr_len = strlen(hstat.newloc)+1;
                    hstat_rpc.newloc.charptr.charptr_val = hstat.newloc;
                    """
                    #客户端封装
                    if isptr:
                        # wrapper_str_list.append("if ("+rawname+"->"+decl.name+" == NULL)")
                        # wrapper_str_list.append("\t"+rpcname+"->"+decl.name+".charptr.charptr_len = 0;")
                        # wrapper_str_list.append("else")
                        # wrapper_str_list.append("\t"+rpcname+"->"+decl.name+".charptr.charptr_len = strlen("+rawname+"->"+decl.name+")+1;")
                        # wrapper_str_list.append(rpcname+"->"+decl.name+".charptr.charptr_val = "+rawname+"->"+decl.name+";")
                        # #服务端解封装
                        # unmashal_str_list.append("if ("+argname+"->"+decl.name+".charptr.charptr_val == NULL)")
                        # unmashal_str_list.append("\t"+rawname+"->"+decl.name+"=NULL;")
                        # unmashal_str_list.append("else")
                        # unmashal_str_list.append("\t"+rawname+"->"+decl.name+"=strdup("+argname+"->"+decl.name+".charptr.charptr_val);")
                        if not isRpcToRaw:
                            strlist.append("if ("+righname+"->"+decl.name+" == NULL)")
                            strlist.append("\t"+leftname+"->"+decl.name+".charptr.charptr_len = 0;")
                            strlist.append("else")
                            strlist.append("\t"+leftname+"->"+decl.name+".charptr.charptr_len = strlen("+righname+"->"+decl.name+")+1;")
                            strlist.append(leftname+"->"+decl.name+".charptr.charptr_val = "+righname+"->"+decl.name+";")
                        else:
                            strlist.append("if ("+righname+"->"+decl.name+".charptr.charptr_val == NULL)")
                            strlist.append("\t"+leftname+"->"+decl.name+"=NULL;")
                            strlist.append("else")
                            strlist.append("\t"+leftname+"->"+decl.name+"=strdup("+righname+"->"+decl.name+".charptr.charptr_val);")
                       

                    else:
                        # wrapper_str_list.append("if ("+rawname+"."+decl.name+" == NULL)")
                        # wrapper_str_list.append("\t"+rpcname+"."+decl.name+".charptr.charptr_len = 0;")
                        # wrapper_str_list.append("else")
                        # wrapper_str_list.append("\t"+rpcname+"."+decl.name+".charptr.charptr_len = strlen("+rawname+"."+decl.name+")+1;")
                        # wrapper_str_list.append(rpcname+"."+decl.name+".charptr.charptr_val = "+rawname+"."+decl.name+";")
                        # #服务端解封装
                        # unmashal_str_list.append("if ("+argname+"."+decl.name+".charptr.charptr_val == NULL)")
                        # unmashal_str_list.append("\t"+rawname+"."+decl.name+"=NULL;")
                        # unmashal_str_list.append("else")
                        # unmashal_str_list.append("\t"+rawname+"."+decl.name+"=strdup("+argname+"."+decl.name+".charptr.charptr_val);")
                        if not isRpcToRaw:
                            strlist.append("if ("+righname+"."+decl.name+" == NULL)")
                            strlist.append("\t"+leftname+"."+decl.name+".charptr.charptr_len = 0;")
                            strlist.append("else")
                            strlist.append("\t"+leftname+"."+decl.name+".charptr.charptr_len = strlen("+righname+"."+decl.name+")+1;")
                            strlist.append(leftname+"."+decl.name+".charptr.charptr_val = "+righname+"."+decl.name+";")
                        else:
                            strlist.append("if ("+righname+"."+decl.name+".charptr.charptr_val == NULL)")
                            strlist.append("\t"+leftname+"."+decl.name+"=NULL;")
                            strlist.append("else")
                            strlist.append("\t"+leftname+"."+decl.name+"=strdup("+righname+"."+decl.name+".charptr.charptr_val);")
                
                elif decltype=="void *":#如果是void *类型，将其转换为opaque类型，opaque类型和string类型生成的.h文件是一样的
                    pass
                    # strlist.append("opaque "+decl.name+"<>;")
                elif decltype=="char **" or decltype=="const char **": #如果是char **类型，将其转换为CharArray类型
                    """
                   if (newloc == NULL) {
                        newloc_rpc.twocharptr.twocharptr_len = 0;
                        newloc_rpc.twocharptr.twocharptr_val = NULL;
                    } else {
                    newloc_rpc.twocharptr.twocharptr_len = 2;
                    newloc_rpc.twocharptr.twocharptr_val = malloc(2 * sizeof(char_ptr));
                    if (newloc_rpc.twocharptr.twocharptr_val == NULL) {
                        perror("malloc failed");
                        exit(1);
                    }

                    for (int i = 0; i < newloc_rpc.twocharptr.twocharptr_len; i++) {
                        if (newloc[i] == NULL) {
                            newloc_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                            newloc_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
                        } else {
                            newloc_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(newloc[i]) + 1;
                            newloc_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val = newloc[i];
                        }
                    }
                    }
                    """
                    if isptr:
                        #客户端封装
                        # wrapper_str_list.append("if ("+rawname+"->"+decl.name+" == NULL) {")
                        # wrapper_str_list.append("\t"+rpcname+"->"+decl.name+".twocharptr.twocharptr_len = 0;")
                        # wrapper_str_list.append("\t"+rpcname+"->"+decl.name+".twocharptr.twocharptr_val = NULL;")
                        # wrapper_str_list.append("} else {")
                        # wrapper_str_list.append("\t"+rpcname+"->"+decl.name+".twocharptr.twocharptr_len = 1;")
                        # wrapper_str_list.append("\t"+rpcname+"->"+decl.name+".twocharptr.twocharptr_val = malloc("+rpcname+"->"+decl.name+".twocharptr.twocharptr_len * sizeof(char_ptr));")
                        # wrapper_str_list.append("\tif ("+rpcname+"->"+decl.name+".twocharptr.twocharptr_val == NULL) {")
                        # wrapper_str_list.append("\t\tperror(\"malloc failed\");")
                        # wrapper_str_list.append("\t\texit(1);")
                        # wrapper_str_list.append("\t}")
                        # wrapper_str_list.append("\tfor (int i = 0; i < "+rpcname+"->"+decl.name+".twocharptr.twocharptr_len; i++) {")
                        # wrapper_str_list.append("\t\tif ("+rawname+"->"+decl.name+"[i] == NULL) {")
                        # wrapper_str_list.append("\t\t\t"+rpcname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = 0;")
                        # wrapper_str_list.append("\t\t\t"+rpcname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;")
                        # wrapper_str_list.append("\t\t} else {")
                        # wrapper_str_list.append("\t\t\t"+rpcname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = strlen("+rawname+"->"+decl.name+"[i]) + 1;")
                        # wrapper_str_list.append("\t\t\t"+rpcname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = "+rawname+"->"+decl.name+"[i];")
                        # wrapper_str_list.append("\t\t}")
                        # wrapper_str_list.append("\t}")
                        # wrapper_str_list.append("}")
                        # #服务端解封装
                        # unmashal_str_list.append("if ("+argname+"->"+decl.name+".twocharptr.twocharptr_val != NULL) {")
                        # unmashal_str_list.append("\t"+rawname+"->"+decl.name+" = (char **)malloc("+argname+"->"+decl.name+".twocharptr.twocharptr_len * sizeof(char *));")
                        # unmashal_str_list.append("\tif ("+rawname+"->"+decl.name+" == NULL) {")
                        # unmashal_str_list.append("\t\tperror(\"malloc failed\");")
                        # unmashal_str_list.append("\t\texit(1);")
                        # unmashal_str_list.append("\t}")
                        # unmashal_str_list.append("\tfor (int i = 0; i < "+argname+"->"+decl.name+".twocharptr.twocharptr_len; i++) {")
                        # unmashal_str_list.append("\t\tif ("+argname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {")
                        # unmashal_str_list.append("\t\t\t"+rawname+"->"+decl.name+"[i] = NULL;")
                        # unmashal_str_list.append("\t\t} else {")
                        # unmashal_str_list.append("\t\t\t"+rawname+"->"+decl.name+"[i] = strdup("+argname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val);")
                        # unmashal_str_list.append("\t\t}")
                        # unmashal_str_list.append("\t}")
                        # unmashal_str_list.append("}else "+rawname+"->"+decl.name+"=NULL;")
                        if not isRpcToRaw:
                            strlist.append("if ("+righname+"->"+decl.name+" == NULL) {")
                            strlist.append("\t"+leftname+"->"+decl.name+".twocharptr.twocharptr_len = 0;")
                            strlist.append("\t"+leftname+"->"+decl.name+".twocharptr.twocharptr_val = NULL;")
                            strlist.append("} else {")
                            strlist.append("\t"+leftname+"->"+decl.name+".twocharptr.twocharptr_len = 1;")
                            strlist.append("\t"+leftname+"->"+decl.name+".twocharptr.twocharptr_val = malloc("+leftname+"->"+decl.name+".twocharptr.twocharptr_len * sizeof(char_ptr));")
                            strlist.append("\tif ("+leftname+"->"+decl.name+".twocharptr.twocharptr_val == NULL) {")
                            strlist.append("\t\tperror(\"malloc failed\");")
                            strlist.append("\t\texit(1);")
                            strlist.append("\t}")
                            strlist.append("\tfor (int i = 0; i < "+leftname+"->"+decl.name+".twocharptr.twocharptr_len; i++) {")
                            strlist.append("\t\tif ("+righname+"->"+decl.name+"[i] == NULL) {")
                            strlist.append("\t\t\t"+leftname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = 0;")
                            strlist.append("\t\t\t"+leftname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;")
                            strlist.append("\t\t} else {")
                            strlist.append("\t\t\t"+leftname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = strlen("+righname+"->"+decl.name+"[i]) + 1;")
                            strlist.append("\t\t\t"+leftname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = "+righname+"->"+decl.name+"[i];")
                            strlist.append("\t\t}")
                            strlist.append("\t}")
                            strlist.append("}")
                        else:
                            strlist.append("if ("+righname+"->"+decl.name+".twocharptr.twocharptr_val != NULL) {")
                            strlist.append("\t"+leftname+"->"+decl.name+" = (char **)malloc("+righname+"->"+decl.name+".twocharptr.twocharptr_len * sizeof(char *));")
                            strlist.append("\tif ("+leftname+"->"+decl.name+" == NULL) {")
                            strlist.append("\t\tperror(\"malloc failed\");")
                            strlist.append("\t\texit(1);")
                            strlist.append("\t}")
                            strlist.append("\tfor (int i = 0; i < "+righname+"->"+decl.name+".twocharptr.twocharptr_len; i++) {")
                            strlist.append("\t\tif ("+righname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {")
                            strlist.append("\t\t\t"+leftname+"->"+decl.name+"[i] = NULL;")
                            strlist.append("\t\t} else {")
                            strlist.append("\t\t\t"+leftname+"->"+decl.name+"[i] = strdup("+righname+"->"+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val);")
                            strlist.append("\t\t}")
                            strlist.append("\t}")
                            strlist.append("}else "+leftname+"->"+decl.name+"=NULL;")

                    else:
                        #客户端封装
                        # wrapper_str_list.append("if ("+rawname+"."+decl.name+" == NULL) {")
                        # wrapper_str_list.append("\t"+rpcname+"."+decl.name+".twocharptr.twocharptr_len = 0;")
                        # wrapper_str_list.append("\t"+rpcname+"."+decl.name+".twocharptr.twocharptr_val = NULL;")
                        # wrapper_str_list.append("} else {")
                        # wrapper_str_list.append("\t"+rpcname+"."+decl.name+".twocharptr.twocharptr_len = 1;")
                        # wrapper_str_list.append("\t"+rpcname+"."+decl.name+".twocharptr.twocharptr_val = malloc("+rpcname+"."+decl.name+".twocharptr.twocharptr_len * sizeof(char_ptr));")
                        # wrapper_str_list.append("\tif ("+rpcname+"."+decl.name+".twocharptr.twocharptr_val == NULL) {")
                        # wrapper_str_list.append("\t\tperror(\"malloc failed\");")
                        # wrapper_str_list.append("\t\texit(1);")
                        # wrapper_str_list.append("\t}")
                        # wrapper_str_list.append("\tfor (int i = 0; i < "+rpcname+"."+decl.name+".twocharptr.twocharptr_len; i++) {")
                        # wrapper_str_list.append("\t\tif ("+rawname+"."+decl.name+"[i] == NULL) {")
                        # wrapper_str_list.append("\t\t\t"+rpcname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = 0;")
                        # wrapper_str_list.append("\t\t\t"+rpcname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;")
                        # wrapper_str_list.append("\t\t} else {")
                        # wrapper_str_list.append("\t\t\t"+rpcname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = strlen("+rawname+"."+decl.name+"[i]) + 1;")
                        # wrapper_str_list.append("\t\t\t"+rpcname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = "+rawname+"."+decl.name+"[i];")
                        # wrapper_str_list.append("\t\t}")
                        # wrapper_str_list.append("\t}")
                        # wrapper_str_list.append("}")
                        # #服务端解封装
                        # """
                        # if (arg2.twocharptr.twocharptr_val != NULL) {
                        #     newloc = (char **)malloc(arg2.twocharptr.twocharptr_len * sizeof(char *));
                        #     if (newloc == NULL) {
                        #         perror("malloc failed");
                        #         exit(1);
                        #     }
                        #     for (int i = 0; i < 2; i++) {
                        #         if (arg2.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                        #             newloc[i] = NULL;
                        #         } else {
                        #             newloc[i] = strdup(arg2.twocharptr.twocharptr_val[i].charptr.charptr_val);
                        #         }
                        #     }
                        # }else newloc=NULL;
                        # """
                        # unmashal_str_list.append("if ("+argname+"."+decl.name+".twocharptr.twocharptr_val != NULL) {")
                        # unmashal_str_list.append("\t"+rawname+"."+decl.name+" = (char **)malloc("+argname+"."+decl.name+".twocharptr.twocharptr_len * sizeof(char *));")
                        # unmashal_str_list.append("\tif ("+rawname+"."+decl.name+" == NULL) {")
                        # unmashal_str_list.append("\t\tperror(\"malloc failed\");")
                        # unmashal_str_list.append("\t\texit(1);")
                        # unmashal_str_list.append("\t}")
                        # unmashal_str_list.append("\tfor (int i = 0; i < "+argname+"."+decl.name+".twocharptr.twocharptr_len; i++) {")
                        # unmashal_str_list.append("\t\tif ("+argname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {")
                        # unmashal_str_list.append("\t\t\t"+rawname+"."+decl.name+"[i] = NULL;")
                        # unmashal_str_list.append("\t\t} else {")
                        # unmashal_str_list.append("\t\t\t"+rawname+"."+decl.name+"[i] = strdup("+argname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val);")
                        # unmashal_str_list.append("\t\t}")
                        # unmashal_str_list.append("\t}")
                        # unmashal_str_list.append("}else "+rawname+"."+decl.name+"=NULL;")
                        if not isRpcToRaw:
                            strlist.append("if ("+righname+"."+decl.name+" == NULL) {")
                            strlist.append("\t"+leftname+"."+decl.name+".twocharptr.twocharptr_len = 0;")
                            strlist.append("\t"+leftname+"."+decl.name+".twocharptr.twocharptr_val = NULL;")
                            strlist.append("} else {")
                            strlist.append("\t"+leftname+"."+decl.name+".twocharptr.twocharptr_len = 1;")
                            strlist.append("\t"+leftname+"."+decl.name+".twocharptr.twocharptr_val = malloc("+leftname+"."+decl.name+".twocharptr.twocharptr_len * sizeof(char_ptr));")
                            strlist.append("\tif ("+leftname+"."+decl.name+".twocharptr.twocharptr_val == NULL) {")
                            strlist.append("\t\tperror(\"malloc failed\");")
                            strlist.append("\t\texit(1);")
                            strlist.append("\t}")
                            strlist.append("\tfor (int i = 0; i < "+leftname+"."+decl.name+".twocharptr.twocharptr_len; i++) {")
                            strlist.append("\t\tif ("+righname+"."+decl.name+"[i] == NULL) {")
                            strlist.append("\t\t\t"+leftname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = 0;")
                            strlist.append("\t\t\t"+leftname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;")
                            strlist.append("\t\t} else {")
                            strlist.append("\t\t\t"+leftname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = strlen("+righname+"."+decl.name+"[i]) + 1;")
                            strlist.append("\t\t\t"+leftname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = "+righname+"."+decl.name+"[i];")
                            strlist.append("\t\t}")
                            strlist.append("\t}")
                            strlist.append("}")
                        else:
                            strlist.append("if ("+righname+"."+decl.name+".twocharptr.twocharptr_val != NULL) {")
                            strlist.append("\t"+leftname+"."+decl.name+" = (char **)malloc("+righname+"."+decl.name+".twocharptr.twocharptr_len * sizeof(char *));")
                            strlist.append("\tif ("+leftname+"."+decl.name+" == NULL) {")
                            strlist.append("\t\tperror(\"malloc failed\");")
                            strlist.append("\t\texit(1);")
                            strlist.append("\t}")
                            strlist.append("\tfor (int i = 0; i < "+righname+"."+decl.name+".twocharptr.twocharptr_len; i++) {")
                            strlist.append("\t\tif ("+righname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {")
                            strlist.append("\t\t\t"+leftname+"."+decl.name+"[i] = NULL;")
                            strlist.append("\t\t} else {")
                            strlist.append("\t\t\t"+leftname+"."+decl.name+"[i] = strdup("+righname+"."+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val);")
                            strlist.append("\t\t}")
                            strlist.append("\t}")
                            strlist.append("}else "+leftname+"."+decl.name+"=NULL;")
                else: #如果是其他类型(int,float等，枚举，结构体，typedef)，
                    if isinstance(decl.type.type.type,c_ast.Struct):
                        if decl.type.type.type.name=="stat":
                            pass
                            # if "stat_rpc" not in typedef_struct_rpc:
                            #     idl_structs_str.append(stat_str)
                            # strlist.append("stat_rpc "+decl.name+";")
                        else:
                            pass
                            # rpcstruct_str_list=[]
                            # if decl.type.type.type.name+"_rpc" not in typedef_struct_rpc:
                            #     rpcstruct_str_list.append("struct "+decl.type.type.type.name+"_rpc{")
                            #     get_rpc_struct_str(struct_dict[decl.type.type.type.name]['ast'],rpcstruct_str_list)
                            #     rpcstruct_str_list.append("};")
                            #     typedef_struct_rpc.append(decl.type.type.type.name+"_rpc")
                            # for rpcstruct_str in rpcstruct_str_list:
                            #     idl_structs_str.append(rpcstruct_str)
                            # strlist.append(decl.type.type.type.name+"_rpc "+decl.name+";")
                    elif isinstance(decl.type.type,c_ast.FuncDecl):
                        #如果函数指针中使用了其他未定义的类型，比如结构体，需要递归，遇到再进行处理
                        # strlist.append(generator.visit(decl)+";")
                        pass
                    else:
                        print(decl)
                        print("without dealwith2")
                    # print(decl)
            elif isinstance(decl.type,c_ast.ArrayDecl): #数组类型
                if decltypearray[0] in basic_type:
                    pass
                    # strlist.append(generator.visit(decl)+";")
                else:
                    print("without dealwith3")
                    # print(decl)
                pass

    
    
    def dealwithreturnvalue(rettype):
        rettypename=generator.visit(rettype)
        if isinstance(rettype,c_ast.TypeDecl):
            rettypename=rettypename.split(" ")[-1]
            if rettypename in basic_type:
                if rettypename=="void":
                    pass
                else:
                    server_rpc_ret_str_list.append("result.ret0=sensefun_ret;")
                    client_rpc_ret_str_list.append("sensefun_ret=result_rpc->ret0;")
            elif rettypename=="stat":
                rpc_helper.dealwith_stat_client_server_retvalue(server_rpc_ret_str_list, client_rpc_ret_str_list)
            elif rettypename in typedef_dict:
                if typedef_dict[rettypename]['isenum']:
                    server_rpc_ret_str_list.append("result.ret0=sensefun_ret;")
                    client_rpc_ret_str_list.append("sensefun_ret=result_rpc->ret0;")
                elif typedef_dict[rettypename]['isstruct']:
                    rpc_helper.recur_dealwith_struct_server_retvalue(typedef_dict[rettypename]['ast'], "result->ret0", "sensefun_ret", ".", server_rpc_ret_str_list)
                    rpc_helper.recur_dealwith_struct_client_retvalue(typedef_dict[rettypename]['ast'], "sensefun_ret", "result_rpc->ret0", ".", client_rpc_ret_str_list)
                else:
                    typename=typedef_dict[rettypename]['ast'].type.type.names[0]
                    if typename in basic_type:
                        server_rpc_ret_str_list.append("result.ret0=sensefun_ret;")
                        client_rpc_ret_str_list.append("sensefun_ret=result_rpc->ret0;")
                    elif typename in struct_dict:
                        if typename=="stat":
                            rpc_helper.dealwith_stat_client_server_retvalue(server_rpc_ret_str_list, client_rpc_ret_str_list)
                        elif struct_dict[typename]['isenum']:
                            server_rpc_ret_str_list.append("result.ret0=sensefun_ret;")
                            client_rpc_ret_str_list.append("sensefun_ret=result_rpc->ret0;")
                        elif struct_dict[typename]['isstruct']:
                            rpc_helper.recur_dealwith_struct_server_retvalue(struct_dict[typename]['ast'], "result->ret0", "sensefun_ret", ".", server_rpc_ret_str_list)
                            rpc_helper.recur_dealwith_struct_client_retvalue(struct_dict[typename]['ast'], "sensefun_ret", "result_rpc->ret0", ".", client_rpc_ret_str_list)
                        else:
                            pass
            elif rettypename in struct_dict:
                if rettypename=="stat":
                    rpc_helper.dealwith_stat_client_server_retvalue(server_rpc_ret_str_list, client_rpc_ret_str_list)
                elif struct_dict[rettypename]['isenum']:
                    server_rpc_ret_str_list.append("result.ret0=sensefun_ret;")
                    client_rpc_ret_str_list.append("sensefun_ret=result_rpc->ret0;")
                elif struct_dict[rettypename]['isstruct']:
                    rpc_helper.recur_dealwith_struct_server_retvalue(struct_dict[rettypename]['ast'], "result->ret0", "sensefun_ret", ".", server_rpc_ret_str_list)
                    rpc_helper.recur_dealwith_struct_client_retvalue(struct_dict[rettypename]['ast'], "sensefun_ret", "result_rpc->ret0", ".", client_rpc_ret_str_list)
                else:
                    pass
        

        elif isinstance(rettype,c_ast.PtrDecl):
            if rettypename=="char *" or rettypename=="const char *":
                #服务端封装返回值
                server_rpc_ret_str_list.append("if (sensefun_ret == NULL) {result.ret0.charptr.charptr_len = 0;result.ret0.charptr.charptr_val = NULL;}")
                server_rpc_ret_str_list.append("else {")
                server_rpc_ret_str_list.append("\tresult.ret0.charptr.charptr_len = strlen(sensefun_ret) + 1;")
                server_rpc_ret_str_list.append("\tresult.ret0.charptr.charptr_val = sensefun_ret;")
                server_rpc_ret_str_list.append("}")
                #客户端解封装返回值
                client_rpc_ret_str_list.append("if (result_rpc->ret0.charptr.charptr_val == NULL) {sensefun_ret = NULL;}")
                client_rpc_ret_str_list.append("else {")
                client_rpc_ret_str_list.append("\tsensefun_ret = strdup(result_rpc->ret0.charptr.charptr_val);")
                client_rpc_ret_str_list.append("}")

            elif rettypename=="void *":
                pass
            elif rettypename=="FILE *":
                pass
            elif rettypename=="char **" or rettypename=="const char **":
                #服务端封装返回值
                server_rpc_ret_str_list.append("if (sensefun_ret == NULL) {result->ret0.twocharptr.twocharptr_len = 0;result.ret0.twocharptr.twocharptr_val = NULL;}")
                server_rpc_ret_str_list.append("else {")
                server_rpc_ret_str_list.append("\tresult.ret0.twocharptr.twocharptr_len = 1;")
                server_rpc_ret_str_list.append("\tresult.ret0.twocharptr.twocharptr_val = malloc(result.ret0.twocharptr.twocharptr_len * sizeof(char_ptr));")
                server_rpc_ret_str_list.append("\tif (result.ret0.twocharptr.twocharptr_val == NULL) {")
                server_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
                server_rpc_ret_str_list.append("\t\texit(1);")
                server_rpc_ret_str_list.append("\t}")
                server_rpc_ret_str_list.append("\tfor (int i = 0; i < result.ret0.twocharptr.twocharptr_len; i++) {")
                server_rpc_ret_str_list.append("\t\tif (sensefun_ret[i] == NULL) {")
                server_rpc_ret_str_list.append("\t\t\tresult.ret0.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;")
                server_rpc_ret_str_list.append("\t\t\tresult.ret0.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;")
                server_rpc_ret_str_list.append("\t\t} else {")  
                server_rpc_ret_str_list.append("\t\t\tresult.ret0.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(sensefun_ret[i]) + 1;")
                server_rpc_ret_str_list.append("\t\t\tresult.ret0.twocharptr.twocharptr_val[i].charptr.charptr_val = sensefun_ret[i];")
                server_rpc_ret_str_list.append("\t\t}")
                server_rpc_ret_str_list.append("\t}")
                server_rpc_ret_str_list.append("}")
                #客户端解封装返回值
                client_rpc_ret_str_list.append("if (result_rpc->ret0.twocharptr.twocharptr_val == NULL) {sensefun_ret = NULL;}")
                client_rpc_ret_str_list.append("else {")
                client_rpc_ret_str_list.append("\tsensefun_ret = (char **)malloc(result_rpc->ret0.twocharptr.twocharptr_len * sizeof(char *));")
                client_rpc_ret_str_list.append("\tif (sensefun_ret == NULL) {")
                client_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
                client_rpc_ret_str_list.append("\t\texit(1);")
                client_rpc_ret_str_list.append("\t}")
                client_rpc_ret_str_list.append("\tfor (int i = 0; i < result_rpc->ret0.twocharptr.twocharptr_len; i++) {")
                client_rpc_ret_str_list.append("\t\tif (result_rpc->ret0.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {")
                client_rpc_ret_str_list.append("\t\t\tsensefun_ret[i] = NULL;")
                client_rpc_ret_str_list.append("\t\t} else {")
                client_rpc_ret_str_list.append("\t\t\tsensefun_ret[i] = strdup(result_rpc->ret0.twocharptr.twocharptr_val[i].charptr.charptr_val);")
                client_rpc_ret_str_list.append("\t\t}")
                client_rpc_ret_str_list.append("\t}")
                client_rpc_ret_str_list.append("}")
            elif rettypename=="struct stat *":
                pass
            else:
                typename=rettypename.split(" ")[-2]
                if typename in basic_type:
                    rpc_helper.dealwith_basicptr_client_server_retvalue(typename, server_rpc_ret_str_list, client_rpc_ret_str_list)
                elif typename in typedef_dict:
                    if typedef_dict[typename]['isenum']:
                        rpc_helper.dealwith_basicptr_client_server_retvalue(typename, server_rpc_ret_str_list, client_rpc_ret_str_list)
                    elif typedef_dict[typename]['isstruct']:
                        rpc_helper.dealwith_structptr_client_server_retvalue(typename, server_rpc_ret_str_list, client_rpc_ret_str_list)
                    else:
                        typename=typedef_dict[typename]['ast'].type.type.names[0]
                        if typename in basic_type:
                            rpc_helper.dealwith_basicptr_client_server_retvalue(typename, server_rpc_ret_str_list, client_rpc_ret_str_list)
                        elif typename in struct_dict:
                            if typename=="stat":
                                pass
                            elif struct_dict[typename]['isenum']:
                                rpc_helper.dealwith_basicptr_client_server_retvalue(typename, server_rpc_ret_str_list, client_rpc_ret_str_list)
                            elif struct_dict[typename]['isstruct']:
                                rpc_helper.dealwith_structptr_client_server_retvalue(typename, server_rpc_ret_str_list, client_rpc_ret_str_list)
                            else:
                                pass
                                
                elif typename in struct_dict:
                    if typename=="stat":
                        pass
                    elif struct_dict[typename]['isenum']:
                        rpc_helper.dealwith_basicptr_client_server_retvalue(typename, server_rpc_ret_str_list, client_rpc_ret_str_list)
                    elif struct_dict[typename]['isstruct']:
                        rpc_helper.dealwith_structptr_client_server_retvalue(typename, server_rpc_ret_str_list, client_rpc_ret_str_list)
                    else:
                        pass

    
    def dealwithParams(func):
        paramcount=0
        for param in func.type.args.params:
            paramcount+=1
            if isinstance(param,c_ast.EllipsisParam):
                print("变参函数")
                continue
            if param.name is None:
                return
            server_rawfun_call_str_list.append(param.name)
            ptype=param.type
            ptypename=generator.visit(ptype)
            # ptypearray=ptypename.split(" ")
            # if len(ptypearray)>2:
            #     ptype=ptypearray[1]+" "+ptypearray[2]
            #     ptypearray=ptypearray[1:]
            if isinstance(ptype,c_ast.TypeDecl):
                ptypename=ptypename.split(" ")[-1]
                if ptypename in basic_type:
                    rpc_helper.dealwith_basic_client_server_param(ptypename,param,paramcount,wrapper_str_list,clnt_rpc_call_param_list,svc_param_list,unmashal_str_list)
                elif ptypename=="stat":
                    rpc_helper.dealwith_stat_client_server_param(param,paramcount,wrapper_str_list,clnt_rpc_call_param_list,svc_param_list,unmashal_str_list)
                elif ptypename in typedef_dict:
                    if typedef_dict[ptypename]['isenum']:
                        rpc_helper.dealwith_basic_client_server_param(ptypename,param,paramcount,wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list)
                    elif typedef_dict[ptypename]['isstruct']:
                        rpc_helper.dealwith_struct_client_server_param(ptypename,param.name,paramcount,wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list)
                    else:
                        typename=typedef_dict[ptypename]['ast'].type.type.names[0]
                        if typename in basic_type:
                            rpc_helper.dealwith_basic_client_server_param(typename,param,paramcount,wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list)
                        elif typename in struct_dict:
                            if typename=="stat":
                                rpc_helper.dealwith_stat_client_server_param(param,paramcount,wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list)
                            elif struct_dict[typename]['isenum']:
                                rpc_helper.dealwith_basic_client_server_param(typename,param,paramcount,wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list)
                            elif struct_dict[typename]['isstruct']:
                                rpc_helper.dealwith_struct_client_server_param(typename,param.name,paramcount,wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list)
                            else:
                                pass
                    
                elif ptypename in struct_dict:
                    if ptypename=="stat":
                        rpc_helper.dealwith_stat_client_server_param(param,paramcount, wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list)
                    elif struct_dict[ptypename]['isenum']:
                        rpc_helper.dealwith_basic_client_server_param(ptypename,param,paramcount, wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list)
                    elif struct_dict[ptypename]['isstruct']:
                        rpc_helper.dealwith_struct_client_server_param(ptypename,param.name,paramcount, wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list)
                    else:
                        pass
                else:
                    pass
                
            elif isinstance(ptype,c_ast.PtrDecl):
                if ptypename=="char *" or ptypename=="const char *" or ptypename=="FILE *":
                    """
                    char_ptr temp;
                    if(param==NULL) temp.charptr.charptr_len=0;else temp.charptr.charptr_len=strlen(param)+1;
                    temp.charptr.charptr_val=malloc(temp.charptr.charptr_len);
                    if(temp.charptr.charptr_val==NULL) {perror("malloc failed");exit(1);}
                    memcpy(temp.charptr.charptr_val,param,temp.charptr.charptr_len);
                    """
                    #客户端封装数据
                    wrapper_str_list.append("char_ptr "+param.name+"_rpc;")
                    clnt_rpc_call_param_list.append(param.name+"_rpc")
                    wrapper_str_list.append("if("+param.name+"==NULL) "+param.name+"_rpc.charptr.charptr_len=0;"
                    "else "+param.name+"_rpc.charptr.charptr_len=strlen("+param.name+")+1;")
                    wrapper_str_list.append(param.name+"_rpc.charptr.charptr_val=malloc("+param.name+"_rpc.charptr.charptr_len);")
                    wrapper_str_list.append("if("+param.name+"_rpc.charptr.charptr_val==NULL) {")
                    wrapper_str_list.append("perror(\"malloc failed\");")
                    wrapper_str_list.append("exit(1);")
                    wrapper_str_list.append("}")
                    wrapper_str_list.append("memcpy("+param.name+"_rpc.charptr.charptr_val,"+param.name+","+param.name+"_rpc.charptr.charptr_len);")
                    #服务端解包数据
                    svc_param_list.append("char_ptr arg"+str(paramcount))
                    unmashal_str_list.append("char *"+param.name+"=NULL;")
                    unmashal_str_list.append("if(arg"+str(paramcount)+".charptr.charptr_val!=NULL) {")
                    unmashal_str_list.append(param.name+"=arg"+str(paramcount)+".charptr.charptr_val;")
                    unmashal_str_list.append("}")
                    # unmashal_str_list.append(param.name+"=strdup(arg"+str(paramcount)+".charptr.charptr_val);")
                    # unmashal_str_list.append("\t}")
                    #服务端封装返回值
                    """
                    if(param==NULL) result->paramname.charptr.charptr_len=0;else temp.charptr.charptr_len=strlen(param)+1;
                    temp.charptr.charptr_val=param;
                    """
                    server_rpc_ret_str_list.append("if("+param.name+"==NULL) result."+param.name+".charptr.charptr_len=0;else result."+param.name+".charptr.charptr_len=strlen("+param.name+")+1;")
                    server_rpc_ret_str_list.append("result."+param.name+".charptr.charptr_val=malloc(result."+param.name+".charptr.charptr_len);")
                    server_rpc_ret_str_list.append("if(result."+param.name+".charptr.charptr_val==NULL) {")
                    server_rpc_ret_str_list.append("perror(\"malloc failed\");")
                    server_rpc_ret_str_list.append("exit(1);")
                    server_rpc_ret_str_list.append("}")
                    server_rpc_ret_str_list.append("memcpy(result."+param.name+".charptr.charptr_val,"+param.name+",result."+param.name+".charptr.charptr_len);")
                    #
                    #客户端解封装返回值
                    client_rpc_ret_str_list.append("if(result_rpc->"+param.name+".charptr.charptr_val==NULL) "+param.name+"=NULL;else{\n")
                    client_rpc_ret_str_list.append(param.name+"=malloc(result_rpc->"+param.name+".charptr.charptr_len);")
                    client_rpc_ret_str_list.append("memcpy("+param.name+",result_rpc->"+param.name+".charptr.charptr_val,result_rpc->"+param.name+".charptr.charptr_len);")
                    client_rpc_ret_str_list.append("}")


                elif ptypename=="void *":
                    pass
                elif ptypename=="char **" or ptypename=="const char **":
                    rpc_helper.dealwith_twocharptr_client_server_param_retval(param,paramcount, wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list, server_rpc_ret_str_list, client_rpc_ret_str_list)
                elif ptypename=="struct stat *":

                    pass
                elif ptypename=="struct timeval *":
                    """
                    struct timeval_rpc_ptr temp;
                    if(param==NULL) temp.timeval_rpc_ptr_len=0;else temp.timeval_rpc_ptr_len=1;
                    temp.timeval_rpc_ptr_val=malloc(sizeof(struct timeval));
                    if(temp.timeval_rpc_ptr_val==NULL) {perror("malloc failed");exit(1);}
                    memcpy(temp.timeval_rpc_ptr_val,param,sizeof(struct timeval));
                    """
                    #客户端封装数据
                    wrapper_str_list.append("struct timeval_rpc_ptr "+param.name+"_rpc;")
                    clnt_rpc_call_param_list.append(param.name+"_rpc")
                    wrapper_str_list.append("if("+param.name+"==NULL) {")
                    wrapper_str_list.append(param.name+"_rpc.timeval_rpc_ptr_len=0;")   
                    wrapper_str_list.append(param.name+"_rpc.timeval_rpc_ptr_val=NULL;")
                    wrapper_str_list.append("} else {")
                    wrapper_str_list.append(param.name+"_rpc.timeval_rpc_ptr_len=1;")

                    wrapper_str_list.append(param.name+"_rpc.timeval_rpc_ptr_val=malloc(sizeof(struct timeval));")
                    wrapper_str_list.append("if("+param.name+"_rpc.timeval_rpc_ptr_val==NULL) {")
                    wrapper_str_list.append("perror(\"malloc failed\");")
                    wrapper_str_list.append("exit(1);")
                    wrapper_str_list.append("}")
                    wrapper_str_list.append("memcpy("+param.name+"_rpc.timeval_rpc_ptr_val,"+param.name+",sizeof(struct timeval));")
                    wrapper_str_list.append("}")
                    #服务端解包数据
                    svc_param_list.append(" timeval_rpc_ptr arg"+str(paramcount))
                    unmashal_str_list.append("struct timeval *"+param.name+"=NULL;")
                    unmashal_str_list.append("if(arg"+str(paramcount)+".timeval_rpc_ptr_val!=NULL) {")
                    unmashal_str_list.append(param.name+"=malloc(sizeof(struct timeval));")
                    unmashal_str_list.append("if("+param.name+"==NULL) {")
                    unmashal_str_list.append("perror(\"malloc failed\");")
                    unmashal_str_list.append("exit(1);")
                    unmashal_str_list.append("}")
                    unmashal_str_list.append("memcpy("+param.name+",arg"+str(paramcount)+".timeval_rpc_ptr_val,sizeof(struct timeval));")
                    unmashal_str_list.append("}")
                    #服务端封装返回值
                    server_rpc_ret_str_list.append("if("+param.name+"==NULL) {")
                    server_rpc_ret_str_list.append("result."+param.name+".timeval_rpc_ptr_len=0;")
                    server_rpc_ret_str_list.append("result."+param.name+".timeval_rpc_ptr_val=NULL;")
                    server_rpc_ret_str_list.append("} else {")
                    server_rpc_ret_str_list.append("result."+param.name+".timeval_rpc_ptr_len=1;")
                    server_rpc_ret_str_list.append("result."+param.name+".timeval_rpc_ptr_val=malloc(sizeof(struct timeval));")
                    server_rpc_ret_str_list.append("if(result."+param.name+".timeval_rpc_ptr_val==NULL) {")
                    server_rpc_ret_str_list.append("perror(\"malloc failed\");")
                    server_rpc_ret_str_list.append("exit(1);")
                    server_rpc_ret_str_list.append("}")
                    server_rpc_ret_str_list.append("memcpy(result."+param.name+".timeval_rpc_ptr_val,"+param.name+",sizeof(struct timeval));")
                    server_rpc_ret_str_list.append("}")
                    #客户端解封装返回值

                    client_rpc_ret_str_list.append("if(result_rpc->"+param.name+".timeval_rpc_ptr_val==NULL) {")
                    client_rpc_ret_str_list.append(param.name+"=NULL;")
                    client_rpc_ret_str_list.append("} else {")
                    client_rpc_ret_str_list.append(param.name+"=malloc(sizeof(struct timeval));")
                    client_rpc_ret_str_list.append("if("+param.name+"==NULL) {")
                    client_rpc_ret_str_list.append("perror(\"malloc failed\");")
                    client_rpc_ret_str_list.append("exit(1);")
                    client_rpc_ret_str_list.append("}")
                    client_rpc_ret_str_list.append("memcpy("+param.name+",result_rpc->"+param.name+".timeval_rpc_ptr_val,sizeof(struct timeval));")
                    client_rpc_ret_str_list.append("}")
                elif ptypename=="struct timespec *":
                    pass

                else:
                    typename=ptypename.split(" ")[-2]
                    
                    if typename in basic_type:
                        rpc_helper.dealwith_basicptr_client_server_param(typename,param.name,paramcount, wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list, server_rpc_ret_str_list, client_rpc_ret_str_list)
                    elif typename in typedef_dict:
                        if typedef_dict[typename]['isenum']:
                            rpc_helper.dealwith_basicptr_client_server_param(typename,param.name,paramcount, wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list, server_rpc_ret_str_list, client_rpc_ret_str_list)
                        elif typedef_dict[typename]['isstruct']:
                            rpc_helper.dealwith_structptr_client_param(typename,param.name, wrapper_str_list, clnt_rpc_call_param_list)
                            rpc_helper.dealwith_structptr_server_param(typename,param.name,paramcount, svc_param_list,unmashal_str_list)
                            if "const" in ptypename:
                                rpc_helper.dealwith_structptr_server_paramRet (typename,param.name, server_rpc_ret_str_list)
                                rpc_helper.dealwith_structptr_client_paramRet(typename,param.name, client_rpc_ret_str_list)

                            
                        else:
                            #函数指针跳过
                            if isinstance(typedef_dict[typename]['ast'].type.type,c_ast.FuncDecl):
                                continue
                            typename=typedef_dict[typename]['ast'].type.type.names[0]
                            if typename in basic_type:
                                rpc_helper.dealwith_basicptr_client_server_param(typename,param.name,paramcount, wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list, server_rpc_ret_str_list, client_rpc_ret_str_list)
                            elif typename in struct_dict:
                                if typename=="stat":
                                    pass
                                elif struct_dict[typename]['isenum']:
                                    rpc_helper.dealwith_basicptr_client_server_param(typename,param.name,paramcount, wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list, server_rpc_ret_str_list, client_rpc_ret_str_list)
                                elif struct_dict[typename]['isstruct']:
                                    rpc_helper.dealwith_structptr_client_param(typename,param.name, wrapper_str_list, clnt_rpc_call_param_list)
                                    rpc_helper.dealwith_structptr_server_param(typename,param.name,paramcount, svc_param_list,unmashal_str_list)
                                    if "const" in ptypename:
                                        rpc_helper.dealwith_structptr_server_paramRet(typename,param.name, server_rpc_ret_str_list)
                                        rpc_helper.dealwith_structptr_client_paramRet(typename,param.name, client_rpc_ret_str_list)

                                else:
                                    pass
                    elif typename in struct_dict:
                        if typename=="stat":
                            pass
                        elif struct_dict[typename]['isenum']:
                            rpc_helper.dealwith_basicptr_client_server_param(typename,param.name,paramcount, wrapper_str_list, clnt_rpc_call_param_list, svc_param_list, unmashal_str_list, server_rpc_ret_str_list, client_rpc_ret_str_list)
                        elif struct_dict[typename]['isstruct']:
                            rpc_helper.dealwith_structptr_client_param(typename,param.name, wrapper_str_list, clnt_rpc_call_param_list)
                            rpc_helper.dealwith_structptr_server_param(typename,param.name,paramcount, svc_param_list,unmashal_str_list)
                            if "const" in ptypename:
                                rpc_helper.dealwith_structptr_server_paramRet(typename,param.name, server_rpc_ret_str_list)
                                rpc_helper.dealwith_structptr_client_paramRet(typename,param.name, client_rpc_ret_str_list)

                        else:
                            pass
            elif isinstance(ptype,c_ast.ArrayDecl) and isinstance(ptype.type.type,c_ast.IdentifierType) and ptype.type.type.names[0]=="char":
                #数组类型
                """
                print_decimal_sense_1_buf buf_rpc;
                memcpy(buf_rpc.buf, buf, 32);
                """
                # struct_str="struct "+func_name+"_"+param.name+"{"+generator.visit(param)+";};"
                # typedef_str="typedef struct "+func_name+"_"+param.name+" "+func_name+"_"+param.name+";"
                # idl_structs_str.append(struct_str)
                # idl_structs_str.append(typedef_str)
                # funstr_list.append(func_name+"_"+param.name+",")
                # retstr_list.append(func_name+"_"+param.name+" "+param.name+";")
                # 获取数组大小
                array_size = None
                if isinstance(ptype.dim, c_ast.Constant):
                    array_size = ptype.dim.value
                else:
                    # 如果无法获取确切大小，使用默认大小
                    array_size = "1"  # 默认值，可根据实际情况调整
                
                # 使用函数名和参数名构造结构体名称
                array_struct_name = f"{func_name}_{param.name}"
                
                # 客户端封装数据
                wrapper_str_list.append(f"{array_struct_name} {param.name}_rpc;")
                clnt_rpc_call_param_list.append(f"{param.name}_rpc")
                wrapper_str_list.append(f"memcpy({param.name}_rpc.{param.name}, {param.name}, {array_size});")
                
                # 服务端解包数据
                svc_param_list.append(f"{array_struct_name} arg{str(paramcount)}")
                unmashal_str_list.append(f"char {param.name}[{array_size}];")
                unmashal_str_list.append(f"memcpy({param.name}, arg{str(paramcount)}.{param.name}, {array_size});")
                
                # 将参数添加到服务端调用原始函数的参数列表中
                # server_rawfun_call_str_list.append(param.name)
                
                # 服务端封装返回值 - 如果参数可能被修改
                server_rpc_ret_str_list.append(f"memcpy(result.{param.name}.{param.name}, {param.name}, {array_size});")
                
                # 客户端解封装返回值 - 如果参数可能被修改
                client_rpc_ret_str_list.append(f"memcpy({param.name}, result_rpc->{param.name}.{param.name}, {array_size});")
                

            else:
                print("未知类型的参数",ptype)
                
                
                
	                            

    
    
    #对boundry_call_need_rpc中的callee进行去重 ====================================
    sense_sub_functions_decl=set()
    #遍历boundry_call_need_rpc，提取其中的callee，根据callee在fun_fundeclast_dict中找到对应的ast，将其加入到sense_sub_functions_decl中
    #(caller, callee, locinfo)
    for caller,callee,locinfo in boundry_call_need_rpc:
        if callee in fun_fundeclast_dict:
            sense_sub_functions_decl.add(fun_fundeclast_dict[callee])

    print("generate_wrapper_file:")
    generator = c_generator.CGenerator() #不一定用得着
    wrapper_str_list=[] #存放wrapper函数的字符串 client
    server_wrapper_str_list=[] #存放服务端wrapper函数的字符串
    client_wrapper_header_str_list=[] #存放客户端wrapper函数的头文件字符串
    
    for func in sense_sub_functions_decl:
        print(c_generator.CGenerator().visit(func))
        print(func.name)
        func_name=func.name
        """
        generate_wrapper_file:
    logputs
    iread
    make_string_hash_table
    dot_finish_sense_1
    mkalldirs_sense_1
    logvprintf_sense_2
    logprintf
    get_contents_sense_1
    logvprintf_sense_1
    skip_lws
    gethttp_sense_1
    http_loop_sense_1
        """
        # if func_name!="make_string_hash_table":
        #     continue
        tempstr=copy.deepcopy(func)
        tempstr.name=func_name+"_rapper"
        if isinstance(tempstr.type.type,c_ast.PtrDecl):
            tempstr.type.type.type.declname=func_name+"_wrapper"
        else:
            tempstr.type.type.declname=func_name+"_wrapper"
       
        wrapper_str_list.append(generator.visit(tempstr))
        client_wrapper_header_str_list.append(generator.visit(tempstr)+";")
        wrapper_str_list.append("{")
        #对函数的参数进行处理，生成对应的rpc变量，将对应的参数值赋值给rpc变量
        # func_name_ret_ *result_rpc;
        wrapper_str_list.append(space4+func_name+"_ret_ *result_rpc;")  

        
        svc_param_list=[]
        clnt_rpc_call_param_list=[] #存放客户端rpc调用的字符串
        unmashal_str_list=[]
        server_rawfun_call_str_list=[] #存放服务端调用原始函数的字符串
        server_rpc_ret_str_list=[] #存放服务端rpc返回值的字符串
        client_rpc_ret_str_list=[] #存放客户端rpc返回值的字符串

        #处理函数的返回值===========================================
        rettype=func.type.type
        srcrettype=generator.visit(rettype)
        if srcrettype!="void":
            wrapper_str_list.append(space4+srcrettype+" sensefun_ret;")
        dealwithreturnvalue(rettype)
            
    
        dealwithParams(func)

        
        #客户端封装数据，调用rpc函数，解封装返回值

        rpc_callstr="result_rpc="+func_name+"_rpc_1("+",".join(clnt_rpc_call_param_list)+",clnt);"
        """
        if (result_1 == (http_loop_sense_1_ret_ *) NULL) {
		clnt_perror (clnt, "call failed");
	    }
        """
        wrapper_str_list.append(rpc_callstr)
        wrapper_str_list.append("if(result_rpc==("+func_name+"_ret_ *)NULL) {")
        wrapper_str_list.append("clnt_perror(clnt,\"call failed\");")
        wrapper_str_list.append("}")
        #解封装返回值
        for ret_str in client_rpc_ret_str_list:
            wrapper_str_list.append(ret_str)
        
        #return sensefun_ret;
        if srcrettype!="void":
            wrapper_str_list.append("return sensefun_ret;")
        wrapper_str_list.append("}")


        #服务端解封装数据，调用原始敏感函数，封装返回值                 
        svc_fundecl=func_name+"_ret_ *"+func_name+"_rpc_1_svc("+",".join(svc_param_list)+",struct svc_req *rqstp)"
        server_wrapper_str_list.append(svc_fundecl)
        server_wrapper_str_list.append("{")
        server_wrapper_str_list.append("\tstatic "+func_name+"_ret_ result;")
        
        for umash_str in unmashal_str_list:
            server_wrapper_str_list.append(umash_str)
        #调用原函数
        if generator.visit(rettype)=="void":
            sensecall_str=func_name+"("+",".join(server_rawfun_call_str_list)+");"
        else:
            server_wrapper_str_list.append(generator.visit(rettype)+" sensefun_ret;")
            sensecall_str="sensefun_ret="+func_name+"("+",".join(server_rawfun_call_str_list)+");"
        server_wrapper_str_list.append(sensecall_str)
        for ret_str in server_rpc_ret_str_list:
            server_wrapper_str_list.append(ret_str)
        server_wrapper_str_list.append("return &result;")
        server_wrapper_str_list.append("}")

    
    
    """
    #include "wget_rpc_wrapper.h"
    """
    wrapper_str_list.insert(0,"extern CLIENT *clnt;")
    wrapper_str_list.insert(0,"#include \""+sys.argv[1]+"_rpc_wrapper.h\"")
    #将生成的wrapper函数写入argv[1]_rpc_wrapper.c文件中
    with open(sys.argv[1]+"_rpc_wrapper.c","w") as f:
        for wrapper_str in wrapper_str_list:
            f.write(wrapper_str+"\n")
    # for wrapper_str in wrapper_str_list:
    #     print(wrapper_str)

    print(  )
    print("server wrapper:")
    # for fun,funast in refactor_fun_ast_dict.items():
    #     if "_sense_" in fun:
    #         raw_fun=fun.split("_sense_")[0]
    #         if raw_fun in nonsense_upto_sense_function:
    #             continue
    #         server_wrapper_str_list.insert(0,generator.visit(funast)+"\n")

    """#include "wget_idl.h" """
    server_wrapper_str_list.insert(0,"#include \""+sys.argv[1]+"_idl.h\"")
    #将生成的服务端wrapper函数写入argv[1]_rpc_server.c文件中
    with open(sys.argv[1]+"_rpc_server.c","w") as f:
        for wrapper_str in server_wrapper_str_list:
            f.write(wrapper_str+"\n")
    # for wrapper_str in server_wrapper_str_list:
    #     print(wrapper_str)


    #写头文件内容
    # #include "wget_idl.h"
    client_wrapper_header_str_list.insert(0,"#include \""+sys.argv[1]+"_idl.h\"")
    #将生成的客户端wrapper函数头文件写入argv[1]_rpc_wrapper.h文件中
    with open(sys.argv[1]+"_rpc_wrapper.h","w") as f:
        for wrapper_str in client_wrapper_header_str_list:
            f.write(wrapper_str+"\n")



            
def output_auto_rpc_code(proname):
    """
    Generate automatic RPC code for a project while preserving original file structure.
    This function handles the generation of client-server RPC code by:
    1. Parsing C source files using libclang
    2. Generating IDL files for RPC communication
    3. Creating a client directory with modified code that uses RPC wrappers
    4. Creating a server directory with sensitive function implementations
    5. Properly handling file headers, global variables, and function implementations
    The function preserves original file structure including headers and conditional compilation
    directives while replacing functions and global variables with RPC-enabled versions.
    Parameters:
        proname (str): Project name used for file paths and RPC wrapper naming
    Global variables used (must be defined before calling):
        nonsense_domain_modified_asts (dict): ASTs of modified non-sensitive code files
        nonsense_domain (set): Set of function names that are in the non-sensitive domain
        nonsense_global_var (set): Set of global variables in non-sensitive domain
        sense_domain (set): Set of function names that are in the sensitive domain
        sense_global_var (set): Set of global variables in sensitive domain
        replaced_functions (set): Set of functions that have been replaced with RPC versions
    Output directories:
        - ../partitioned_software/{proname}/6_rpc_refactor_result/IDL/ - For IDL files
        - ../partitioned_software/{proname}/6_rpc_refactor_result/{proname}_client/ - Client code
        - ../partitioned_software/{proname}/6_rpc_refactor_result/{proname}_server/ - Server code
        """
    """
    输出自动RPC代码，保留原始文件的头文件和条件编译结构，
    但替换函数和全局变量为新的修改后内容
    """
    # 设置 libclang 用于解析C文件
    clang.cindex.Config.set_library_file('/usr/lib/llvm-12/lib/libclang.so')
    def is_function_definition(cursor):
        """检查函数游标是否为函数定义（而非仅声明）"""
        # 函数定义会有函数体，可以通过查找子节点中是否有复合语句(Compound Statement)来判断
        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.COMPOUND_STMT:
                return True
        return False


    # def preserve_headers_and_replace_content_for_alive(src_file, dst_file,issense=False, fileast=None, proname=None,senseast=None,fundomainset=None,modifunset=None):
    #     def preserve_headers_and_replace_content_for_alive(src_file, dst_file, issense=False, fileast=None, proname=None, senseast=None, fundomainset=None, modifunset=None):
    #         """
    #         Preserves original headers and conditional compilation directives while replacing function and global variable contents.
    #         Safely adds RPC wrapper header files when necessary.
    #         This function:
    #         1. Parses the source file using libclang
    #         2. Identifies function definitions and global variable declarations
    #         3. Processes functions based on whether they are in the function domain set and modification set
    #         4. Removes functions not in the function domain set
    #         5. Replaces functions in the modification set with their new implementations
    #         6. Preserves functions that don't need modification
    #         7. Filters global variables based on the provided set
    #         8. Adds the RPC wrapper header if needed and not already present
    #         Args:
    #             src_file (str): Path to the source file to process
    #             dst_file (str): Path where the modified file should be saved
    #             issense (bool, optional): Flag indicating if this is a sensitive function. Defaults to False.
    #             fileast (pycparser.c_ast, optional): AST containing replacement implementations. Defaults to None.
    #             proname (str, optional): Project name used for including header files. Defaults to None.
    #             global_var (set, optional): Set of global variable names to preserve. Defaults to None.
    #             fundomainset (set, optional): Set of function names to preserve. Defaults to None.
    #             modifunset (set, optional): Set of function names to replace with new implementations. Defaults to None.
    #         Returns:
    #             None: Returns early if no content remains after processing
    #         """
    #     """
    #     保留原始文件的头文件和条件编译，但替换函数和全局变量内容
    #     同时安全地添加RPC包装器头文件
        
    #     参数:
    #         src_file: 源文件路径
    #         dst_file: 目标文件路径
    #         nonsense_ast: 用于替换的AST（如果有）
    #         proname: 项目名称，用于包含头文件
    #     """

    #     # print(senseast)
    #     generator=c_generator.CGenerator()
    #     if issense:
    #         print("sensitive function")
    #         print(src_file)
    #     # 使用libclang解析源文件
    #     index = clang.cindex.Index.create()
    #     # tu = index.parse(src_file, args=['-I.','-DHAVE_CONFIG_H','-DSYSTEM_WGETRC=\"/usr/local/etc/wgetrc\"','-DLOCALEDIR=\"/usr/local/share/locale\"'])
    #     # tu = index.parse(src_file, args=['-I.','-I..','-I../lib','-I../libmisc','-DHAVE_CONFIG_H','-DLOCALEDIR=\"/usr/local/share/locale\"'])
    #     tu=index.parse(src_file)
        
    #     # 读取源文件内容并检测换行符格式
    #     with open(src_file, 'r', errors='ignore', newline='') as f:
    #         src_content = f.read()
    #     print("source code start==================")
    #     print(src_content)
    #     print("source code end==============")
        
    #     # 检测源文件的换行符格式
    #     if '\r\n' in src_content:
    #         line_ending = '\r\n'  # Windows
    #     else:
    #         line_ending = '\n'    # Unix/Linux
        
    #     # # 如果没有提供替换的AST，直接复制文件
    #     # if fileast is None :
    #     #     with open(dst_file, 'w', newline='') as f:
    #     #         f.write(src_content)
    #     #     return
    #     # 找出所有函数定义和全局变量声明的范围
    #     functions_and_globals = []
    #     for cursor in tu.cursor.get_children():
    #         # 仅处理来自主文件的声明
    #         if (cursor.location.file and 
    #             cursor.location.file.name == src_file and 
    #             (cursor.kind == CursorKind.FUNCTION_DECL or
    #             (cursor.kind == CursorKind.VAR_DECL and cursor.semantic_parent == tu.cursor))):
                
    #             # 确定是函数定义还是声明
    #             is_definition = False
    #             if cursor.kind == CursorKind.FUNCTION_DECL:
    #                 is_definition = is_function_definition(cursor)
                
    #             # 获取位置信息
    #             start_line = cursor.extent.start.line
    #             start_column = cursor.extent.start.column
    #             end_line = cursor.extent.end.line
    #             end_column = cursor.extent.end.column
                
    #             # 创建一个扩展的种类描述
    #             extended_kind = cursor.kind
    #             if cursor.kind == CursorKind.FUNCTION_DECL:
    #                 extended_kind = "FUNCTION_DEF" if is_definition else "FUNCTION_DECL"
                
    #             functions_and_globals.append((cursor.spelling, 
    #                                         start_line, start_column, 
    #                                         end_line, end_column, 
    #                                         extended_kind))

    #     # 按照起始行号排序，从后向前替换，避免行号变化
    #     functions_and_globals.sort(key=lambda x: x[1], reverse=True)

    #     # 输出functions_and_globals信息
    #     for name, start_line, start_col, end_line, end_col, kind in functions_and_globals:
    #         kind_str = str(kind)
    #         if kind_str == "FUNCTION_DEF":
    #             kind_str = "函数定义"
    #         elif kind_str == "FUNCTION_DECL":
    #             kind_str = "函数声明"
    #         elif str(kind) == "CursorKind.VAR_DECL":
    #             kind_str = "全局变量"
            
    #         print(f"名称: {name}")
    #         print(f"位置: 第{start_line}行第{start_col}列 到 第{end_line}行第{end_col}列")
    #         print(f"类型: {kind_str}")
    #         print("---")

    #     # 生成修改后的内容
    #     modified_content = src_content
    #     isremain = False

    #     # 首先将源代码按行切分
    #     lines = modified_content.splitlines(True)  # 保留换行符

    #     for name, start_line, start_col, end_line, end_col, kind in functions_and_globals:
    #         print(f"处理 {name}...")
    #         kind_str = str(kind)
    #         if kind_str == "FUNCTION_DEF":
    #             print(name)
    #             # print(fundomainset)
    #             # print(sense_domain)
    #             if issense and fundomainset and name+"_sense_1" in sense_domain:
    #                 print("here")
    #                 new_code=generator.visit(senseast[name+"_sense_1"])
    #                 new_lines=new_code.splitlines(True)
    #                 # modified_lines=lines[:start_line-1]
    #                 # modified_lines.extend(new_lines)
    #                 # if end_line<len(lines):
    #                 #     modified_lines.extend(lines[end_line:])
    #                 # lines=modified_lines
    #                 # isremain=True

    #                 #从senseast中取出funame为name的ast，插入当前modified_liens中  
    #                 new_code = generator.visit(senseast[name])
    #                 new_lines1 = new_code.splitlines(True)
    #                 modified_lines = lines[:start_line-1]
    #                 new_lines = new_lines+new_lines1
    #                 modified_lines.extend(new_lines)
    #                 if end_line < len(lines):
    #                     modified_lines.extend(lines[end_line:])
    #                 lines = modified_lines
    #                 isremain = True
    #                 continue
    #             # if fundomainset and name not in fundomainset:
    #             #     # 根据行号范围删除函数定义
    #             #     # 注意：行号是从1开始的，但Python列表索引是从0开始的
    #             #     modified_lines = lines[:start_line-1]
    #             #     if end_line < len(lines):
    #             #         modified_lines.extend(lines[end_line:])
    #             #     lines = modified_lines
    #             #     continue
                
    #             elif modifunset and name in modifunset and fileast:
    #                 # 在修改后的AST中查找此函数并替换
    #                 for node in fileast.ext:
    #                     if (isinstance(node, c_ast.FuncDef) and node.decl.name == name):
    #                         # 生成修改后的函数代码
    #                         new_code = generator.visit(node)
    #                         new_lines = new_code.splitlines(True)
                            
    #                         # 替换对应行
    #                         modified_lines = lines[:start_line-1]
    #                         modified_lines.extend(new_lines)
    #                         if end_line < len(lines):
    #                             modified_lines.extend(lines[end_line:])
    #                         lines = modified_lines
    #                         isremain = True
    #                         break
    #             else:
    #                 # 保留原始函数
    #                 isremain = True
            
    #         # elif str(kind) == "CursorKind.VAR_DECL":
    #         #     # 全局变量处理
    #         #     if name not in global_var:
    #         #         # 删除不在global_var中的全局变量
    #         #         modified_lines = lines[:start_line-1]
    #         #         if end_line < len(lines):
    #         #             modified_lines.extend(lines[end_line:])
    #         #         lines = modified_lines
    #         #     else:
    #         #         isremain = True

    #     # 重新组合修改后的内容
    #     if isremain:
    #         modified_content = ''.join(lines)
    #     # 写入修改后的内容，确保使用与原文件相同的换行符格式
    #     with open(dst_file, 'w', newline='') as f:
    #         f.write(modified_content)
    
    
    # def preserve_headers_and_replace_content(src_file, dst_file, issense=False, fileast=None, proname=None, global_var=None, fundomainset=None, modifunset=None):
    #     """
    #     保留原始文件的头文件和条件编译，但替换函数和全局变量内容
    #     通过更健壮的方式处理函数定义和替换
    #     """
    #     generator = c_generator.CGenerator()
    #     if issense:
    #         print("处理敏感函数文件:", src_file)

    #     # 读取源文件内容
    #     with open(src_file, 'r', errors='ignore', newline='') as f:
    #         src_content = f.read()

    #     # 解析源文件
    #     index = clang.cindex.Index.create()
    #     tu = index.parse(src_file)
        
    #     # 构建一个排序的函数和全局变量列表，按照它们在文件中的开始位置降序排列
    #     items_to_process = []
        
    #     for cursor in tu.cursor.get_children():
    #         if (cursor.location.file and 
    #             cursor.location.file.name == src_file and 
    #             (cursor.kind == CursorKind.FUNCTION_DECL or
    #             (cursor.kind == CursorKind.VAR_DECL and cursor.semantic_parent == tu.cursor))):
                
    #             # 确定是函数定义还是声明
    #             is_definition = False
    #             if cursor.kind == CursorKind.FUNCTION_DECL:
    #                 is_definition = is_function_definition(cursor)
    #                 if not is_definition:
    #                     continue  # 跳过函数声明，只处理定义
                
    #             items_to_process.append({
    #                 "name": cursor.spelling,
    #                 "start": cursor.extent.start.offset,
    #                 "end": cursor.extent.end.offset,
    #                 "kind": "FUNCTION_DEF" if cursor.kind == CursorKind.FUNCTION_DECL else "VAR_DECL"
    #             })
        
    #     # 按起始位置降序排序，这样从后向前替换不会影响前面项目的位置
    #     items_to_process.sort(key=lambda x: x["start"], reverse=True)
        
    #     # 修改内容
    #     modified_content = src_content
        
    #     for item in items_to_process:
    #         name = item["name"]
    #         start = item["start"]
    #         end = item["end"]
    #         kind = item["kind"]
            
    #         # 使用这个精确的范围从原始内容中提取函数/变量定义
    #         original_text = src_content[start:end]
            
    #         # 检查是否需要修改/删除这个函数或变量
    #         if kind == "FUNCTION_DEF":
    #             if issense and fundomainset and name+"_sense_1" in fundomainset:
    #                 # 替换为敏感函数实现
    #                 new_code = generator.visit(fileast[name+"_sense_1"])
    #                 modified_content = modified_content[:start] + new_code + modified_content[end:]
                    
    #             elif fundomainset and name not in fundomainset:
    #                 # 删除不在目标域的函数
    #                 modified_content = modified_content[:start] + modified_content[end:]
                    
    #             elif modifunset and name in modifunset and fileast:
    #                 # 替换为修改后的函数
    #                 for node in fileast.ext:
    #                     if isinstance(node, c_ast.FuncDef) and node.decl.name == name:
    #                         print(node)
    #                         new_code = generator.visit(node)
    #                         modified_content = modified_content[:start] + new_code + modified_content[end:]
    #                         break
                            
    #         # elif kind == "VAR_DECL" and global_var is not None:
    #         #     # 处理全局变量
    #         #     if name not in global_var:
    #         #         # 删除不在目标域的全局变量
    #         #         modified_content = modified_content[:start] + modified_content[end:]
        
    #     # 处理包含RPC头文件
    #     if not issense and proname:
    #         include_line = f'#include "{proname}_rpc_wrapper.h"\n'
            
    #         # 只有当头文件尚未包含时才添加
    #         if include_line not in modified_content:
    #             # 解析修改后的内容，找到适合添加头文件的位置
    #             lines = modified_content.splitlines(True)
                
    #             # 查找第一个注释块的结束位置
    #             in_comment = False
    #             first_comment_end = -1
                
    #             for i, line in enumerate(lines):
    #                 if "/*" in line and not in_comment:
    #                     in_comment = True
    #                 if "*/" in line and in_comment:
    #                     in_comment = False
    #                     first_comment_end = i + 1
    #                     break
                
    #             # 查找第一个包含指令的位置和最后一个包含指令的位置
    #             first_include_idx = -1
    #             last_include_idx = -1
    #             for i, line in enumerate(lines):
    #                 if line.strip().startswith("#include"):
    #                     if first_include_idx == -1:
    #                         first_include_idx = i
    #                     last_include_idx = i
                
    #             # 确定插入位置 - 在最后一个包含指令之后
    #             if last_include_idx >= 0:
    #                 insert_pos = last_include_idx + 1
    #             elif first_comment_end > 0:
    #                 # 如果没有包含指令但有注释块，在注释块后插入
    #                 insert_pos = first_comment_end
    #             else:
    #                 # 如果既没有包含指令也没有注释块，在文件开头插入
    #                 insert_pos = 0
                
    #             # 插入包含指令
    #             if insert_pos > 0 and not lines[insert_pos - 1].isspace() and insert_pos < len(lines):
    #                 lines.insert(insert_pos, "\n")
    #                 insert_pos += 1
                
    #             lines.insert(insert_pos, include_line)
                
    #             if insert_pos + 1 < len(lines) and not lines[insert_pos + 1].isspace():
    #                 lines.insert(insert_pos + 1, "\n")
                
    #             modified_content = ''.join(lines)
        
    #     # 写入修改后的内容
    #     with open(dst_file, 'w', newline='') as f:
    #         f.write(modified_content)
        
    #     print(f"文件处理完成: {dst_file}")

    # def preserve_headers_and_replace_content(src_file, dst_file, issense=False, fileast=None, proname=None, global_var=None, fundomainset=None, modifunset=None)  
    def preserve_headers_and_replace_content(src_file, dst_file, issense=False, fileast=None, proname=None, global_var=None, fundomainset=None, modifunset=None):
        """
        Preserves original headers and conditional compilation directives while replacing function and global variable contents.
        Safely adds RPC wrapper header files when necessary.
        This function:
        1. Parses the source file using libclang
        2. Identifies function definitions and global variable declarations
        3. Processes functions based on whether they are in the function domain set and modification set
        4. Removes functions not in the function domain set
        5. Replaces functions in the modification set with their new implementations
        6. Preserves functions that don't need modification
        7. Filters global variables based on the provided set
        8. Adds the RPC wrapper header if needed and not already present
        Args:
            src_file (str): Path to the source file to process
            dst_file (str): Path where the modified file should be saved
            issense (bool, optional): Flag indicating if this is a sensitive function. Defaults to False.
            fileast (pycparser.c_ast, optional): AST containing replacement implementations. Defaults to None.
            proname (str, optional): Project name used for including header files. Defaults to None.
            global_var (set, optional): Set of global variable names to preserve. Defaults to None.
            fundomainset (set, optional): Set of function names to preserve. Defaults to None.
            modifunset (set, optional): Set of function names to replace with new implementations. Defaults to None.
        Returns:
            None: Returns early if no content remains after processing
        """
        """
        保留原始文件的头文件和条件编译，但替换函数和全局变量内容
        同时安全地添加RPC包装器头文件
        
        参数:
            src_file: 源文件路径
            dst_file: 目标文件路径
            nonsense_ast: 用于替换的AST（如果有）
            proname: 项目名称，用于包含头文件
        """
        generator=c_generator.CGenerator()
        if issense:
            print("sensitive function")
            print(src_file)
        # 使用libclang解析源文件
        index = clang.cindex.Index.create()
        # tu = index.parse(src_file, args=['-I.','-DHAVE_CONFIG_H','-DSYSTEM_WGETRC=\"/usr/local/etc/wgetrc\"','-DLOCALEDIR=\"/usr/local/share/locale\"'])
        # tu = index.parse(src_file, args=['-I.','-I..','-I../lib','-I../libmisc','-DHAVE_CONFIG_H','-DLOCALEDIR=\"/usr/local/share/locale\"'])
        tu=index.parse(src_file)
        
        # 读取源文件内容并检测换行符格式
        with open(src_file, 'r', errors='ignore', newline='') as f:
            src_content = f.read()
        # print("source code start==================")
        # print(src_content)
        # print("source code end==============")
        
        # # 检测源文件的换行符格式
        # if '\r\n' in src_content:
        #     line_ending = '\r\n'  # Windows
        # else:
        #     line_ending = '\n'    # Unix/Linux


        #输出src_content
        # print("source code start==================")
        # print(src_content)
        
        # # 如果没有提供替换的AST，直接复制文件
        # if fileast is None :
        #     with open(dst_file, 'w', newline='') as f:
        #         f.write(src_content)
        #     return
        # 找出所有函数定义和全局变量声明的范围
        functions_and_globals = []
        for cursor in tu.cursor.get_children():
            # 仅处理来自主文件的声明
            if (cursor.location.file and 
                cursor.location.file.name == src_file and 
                (cursor.kind == CursorKind.FUNCTION_DECL or
                (cursor.kind == CursorKind.VAR_DECL and cursor.semantic_parent == tu.cursor))):
                
                # 确定是函数定义还是声明
                is_definition = False
                if cursor.kind == CursorKind.FUNCTION_DECL:
                    is_definition = is_function_definition(cursor)
                
                # 获取位置信息
                start_line = cursor.extent.start.line
                start_column = cursor.extent.start.column
                end_line = cursor.extent.end.line
                end_column = cursor.extent.end.column
                
                # 创建一个扩展的种类描述
                extended_kind = cursor.kind
                if cursor.kind == CursorKind.FUNCTION_DECL:
                    extended_kind = "FUNCTION_DEF" if is_definition else "FUNCTION_DECL"
                
                functions_and_globals.append((cursor.spelling, 
                                            start_line, start_column, 
                                            end_line, end_column, 
                                            extended_kind))

        # 按照起始行号排序，从后向前替换，避免行号变化
        functions_and_globals.sort(key=lambda x: x[1], reverse=True)

        # 输出functions_and_globals信息
        for name, start_line, start_col, end_line, end_col, kind in functions_and_globals:
            kind_str = str(kind)
            if kind_str == "FUNCTION_DEF":
                kind_str = "函数定义"
            elif kind_str == "FUNCTION_DECL":
                kind_str = "函数声明"
            elif str(kind) == "CursorKind.VAR_DECL":
                kind_str = "全局变量"
            
            print(f"名称: {name}")
            print(f"位置: 第{start_line}行第{start_col}列 到 第{end_line}行第{end_col}列")
            print(f"类型: {kind_str}")
            print("---")

        # 生成修改后的内容
        modified_content = src_content
        isremain = False

        # 首先将源代码按行切分
        lines = modified_content.splitlines(True)  # 保留换行符

        for name, start_line, start_col, end_line, end_col, kind in functions_and_globals:
            print(f"处理 {name}...")
            kind_str = str(kind)
            if kind_str == "FUNCTION_DEF":
                print(name)
                # print(fundomainset)
                if issense and fundomainset and name+"_sense_1" in fundomainset:
                    print("here")
                    new_code=generator.visit(fileast[name+"_sense_1"])
                    new_lines=new_code.splitlines(True)
                    modified_lines=lines[:start_line-1]

                    modified_lines.extend(new_lines)
                    if end_line<len(lines):
                        modified_lines.extend(lines[end_line:])
                    lines=modified_lines
                    # print(lines)
                    isremain=True
                    continue
                if fundomainset and name not in fundomainset:
                    print("not in allow range")
                    # 根据行号范围删除函数定义
                    # 注意：行号是从1开始的，但Python列表索引是从0开始的
                    modified_lines = lines[:start_line-1]
                    if end_line < len(lines):
                        modified_lines.extend(lines[end_line:])
                    lines = modified_lines
                    print(lines)
                    isremain=True
                    continue
                
                elif modifunset and name in modifunset and fileast:
                    print(name)
                    # 在修改后的AST中查找此函数并替换
                    for node in fileast.ext:
                        if (isinstance(node, c_ast.FuncDef) and node.decl.name == name):
                            # 生成修改后的函数代码
                            new_code = generator.visit(node)
                            new_lines = new_code.splitlines(True)
                            print(start_line,end_line)
                            print(''.join(new_lines))
                            print()
                            
                            # 替换对应行
                            modified_lines = lines[:start_line-1]
                            print("替换之前的代码")
                            print(''.join(modified_lines))
                            modified_lines.extend(new_lines)
                            if end_line < len(lines):
                                modified_lines.extend(lines[end_line:])
                            print("替换之后的代码")
                            print(''.join(lines[end_line:]))
                            lines = modified_lines
                            isremain = True
                            break
                else:
                    # 保留原始函数
                    isremain = True
            
            elif str(kind) == "CursorKind.VAR_DECL":
                # 全局变量处理
                if name not in global_var:
                    # 删除不在global_var中的全局变量
                    modified_lines = lines[:start_line-1]
                    if end_line < len(lines):
                        modified_lines.extend(lines[end_line:])
                    lines = modified_lines
                    isremain=True
                else:
                    isremain = True
            print(''.join(lines))

        # 重新组合修改后的内容
        if isremain:
            
            modified_content = ''.join(lines)
            
            print("finally file")
            print(modified_content)
        
        # 2. 安全地添加RPC包装器头文件
        if issense==False: #如果是敏感函数，不需要添加头文件
            if proname:
                include_line = f'#include "{proname}_rpc_wrapper.h"\n'
                
                # 检查头文件是否已经存在
                if include_line not in modified_content:
                    # 解析文件，找到适合插入头文件的
                    lines = modified_content.splitlines(True)  # 保留换行符
                    
                    # 查找第一个注释块的结束位置
                    in_comment = False
                    first_comment_end = -1
                    
                    for i, line in enumerate(lines):
                        line_stripped = line.strip()
                        
                        # 检测多行注释的开始和结束
                        if "/*" in line and not in_comment:
                            in_comment = True
                        if "*/" in line and in_comment:
                            in_comment = False
                            first_comment_end = i + 1  # 记录第一个注释结束后的行号
                            break
                    
                    # 查找第一个包含指令的位置
                    first_include_idx = -1
                    for i, line in enumerate(lines):
                        if line.strip().startswith("#include"):
                            first_include_idx = i
                            break
                    
                    # 确定插入位置
                    if first_comment_end > 0:
                        # 在第一个注释块后插入
                        insert_pos = first_comment_end
                    elif first_include_idx >= 0:
                        # 如果找不到注释块但有include语句，在第一个include前插入
                        insert_pos = first_include_idx
                    else:
                        # 如果找不到注释块和include语句，在文件开头插入
                        insert_pos = 0
                    
                    # 插入包含指令，确保有足够的空行
                    if insert_pos > 0 and not lines[insert_pos - 1].isspace():
                        lines.insert(insert_pos, "\n")
                        insert_pos += 1
                    
                    lines.insert(insert_pos, include_line)
                    
                    # 确保插入后有空行分隔
                    if insert_pos + 1 < len(lines) and not lines[insert_pos + 1].isspace():
                        lines.insert(insert_pos + 1, "\n")
                    
                    # 重新组合修改后的内容
                    modified_content = ''.join(lines)
        
        # 写入修改后的内容，确保使用与原文件相同的换行符格式
        with open(dst_file, 'w', newline='') as f:
            f.write(modified_content)

        

    # ==== IDL文件生成 =========================================================================
    # 创建IDL目录
    # idl_dir = f"../partitioned_software/{proname}/6_rpc_refactor_result/IDL"
    script_dir = os.path.dirname(os.path.abspath(__file__))

    idl_dir = os.path.abspath(os.path.join(script_dir, '..', 'examples', proname, 'output', 'init_partition', 'IDL'))

    if os.path.exists(idl_dir):
        shutil.rmtree(idl_dir)
    os.makedirs(idl_dir)
    
    # 移动IDL文件
    idl_file = f"{proname}_idl.x"
    if os.path.exists(idl_file):
        shutil.move(idl_file, os.path.join(idl_dir, idl_file))
    
    # 生成RPC代码================================================================
    os.system(f"rpcgen -N {os.path.join(idl_dir, idl_file)}")
    
    # ==== 客户端代码生成 =========================================================
  
    client_dir = os.path.abspath(os.path.join(script_dir, '..', 'examples', proname, 'output', 'init_partition', f'{proname}_client'))
    if os.path.exists(client_dir):
        shutil.rmtree(client_dir)
    os.makedirs(client_dir)
    
    # 处理源代码文件
    # src_base_dir = f"../partitioned_software/{proname}/0_raw_code"

    # 构建源码目录路径
    source_code_dir = os.path.abspath(os.path.join(
        script_dir, '..', 'examples', proname, 'input', 'source_code'
    ))
   
    
    # 查找并解压对应的tar.xz压缩包
    for item in os.listdir(source_code_dir):
        if item.endswith('.tar.xz'):
            #备份为，在加压后的目录下创建一个同名的_back目录，
            extracted_dir_name = item[:-7]  # 去掉 .tar.xz 后缀


    src_base_dir = os.path.abspath(os.path.join(script_dir, '..', 'examples', proname, 'input', 'source_code', f"{extracted_dir_name}_back"))
    print("src_base_dir",src_base_dir)

    for root, dirs, files in os.walk(src_base_dir):
        for file in files:
            src_file_path = os.path.join(root, file)
            relative_path = os.path.relpath(root, src_base_dir)
            dst_dir_path = os.path.join(client_dir, relative_path)
            os.makedirs(dst_dir_path, exist_ok=True)
            dst_file_path = os.path.join(dst_dir_path, file)

            abs_src_file_path = os.path.abspath(src_file_path).replace(f"{extracted_dir_name}_back", f"{extracted_dir_name}")
            print("modei")
            # if file.endswith(".c"):
            if abs_src_file_path in cfile_list:
                finally_path=dst_dir_path
                print(abs_src_file_path)
                for key in nonsense_domain_modified_asts.keys():
                    print(key)
                if abs_src_file_path in nonsense_domain_modified_asts:
                    print(f"处理文件路径 {src_file_path}...")
                    preserve_headers_and_replace_content(
                        src_file_path, 
                        dst_file_path, 
                        issense=False,
                        fileast=nonsense_domain_modified_asts[abs_src_file_path],
                        proname=proname,
                        global_var=nonsense_global_var,fundomainset=nonsense_domain,modifunset=replaced_functions
                    )
                else:
                     preserve_headers_and_replace_content(
                        src_file_path, 
                        dst_file_path, 
                        issense=False,
                        fileast=None,
                        proname=proname,
                        global_var=nonsense_global_var,fundomainset=nonsense_domain,modifunset=replaced_functions
                    )
            else:
                # shutil.copyfile(src_file_path, dst_file_path)
                #复制文件且保留文件的权限
                shutil.copy2(src_file_path, dst_file_path)
    
    # 复制IDL生成的客户端文件==========
    idl_client_files = [f"{proname}_idl.h", f"{proname}_idl_clnt.c", f"{proname}_idl_xdr.c"]
    for file in idl_client_files:
        # idl_file_path = os.path.join(idl_dir, file)
        if os.path.exists(file):
            shutil.copy(file, os.path.join(finally_path, file))
    
    # 复制wrapper文件
    wrapper_files = [f"{proname}_rpc_wrapper.h", f"{proname}_rpc_wrapper.c"]
    for file in wrapper_files:
        if os.path.exists(file):
            shutil.move(file, os.path.join(finally_path, file))
    
    # ==== 服务端代码生成 ==============================================================
    server_dir = f"../partitioned_software/{proname}/6_rpc_refactor_result/{proname}_server"

    server_dir = os.path.abspath(os.path.join(script_dir, '..', 'examples', proname, 'output', 'init_partition', f'{proname}_server'))

    if os.path.exists(server_dir):
        shutil.rmtree(server_dir)
    os.makedirs(server_dir)
    
    # 处理源代码文件
    print(src_base_dir)
    for root, dirs, files in os.walk(src_base_dir):
        for file in files:
            src_file_path = os.path.join(root, file)
            relative_path = os.path.relpath(root, src_base_dir)
            dst_dir_path = os.path.join(server_dir, relative_path)
            os.makedirs(dst_dir_path, exist_ok=True)
            dst_file_path = os.path.join(dst_dir_path, file)
            abs_src_file_path = os.path.abspath(src_file_path).replace(extracted_dir_name+"_back", extracted_dir_name)
            # if file.endswith(".c"):
            print(abs_src_file_path)
            if abs_src_file_path in cfile_list:
                print("服务端代码输出")
                print(cfile_list)
                print(sense_domain)
                finally_path=dst_dir_path

                
                preserve_headers_and_replace_content(
                    src_file_path, 
                    dst_file_path, 
                    issense=True,
                    fileast=refactor_fun_ast_dict,
                    proname=proname,
                    global_var=sense_global_var,fundomainset=sense_domain,modifunset=None
                )
            else:
                print("正常copy")
                print(src_file_path)
                shutil.copy2(src_file_path, dst_file_path)
            

    # 复制IDL生成的服务端文件
    # 复制IDL生成的服务端文件
    idl_server_files = [f"{proname}_idl.h", f"{proname}_idl_svc.c", f"{proname}_idl_xdr.c"]
    for file in idl_server_files:
        # idl_file_path = os.path.join(idl_dir, file)
        if os.path.exists(file):
            shutil.copy(file, os.path.join(finally_path, file))
    
    # 复制server RPC文件
    server_file = f"{proname}_rpc_server.c"
    if os.path.exists(server_file):
        shutil.move(server_file, os.path.join(finally_path, server_file))


    ##=======================Alive 验证代码生成==================
    # alive_dir = f"../partitioned_software/{proname}/8_alive_tv_verify"
    # # if os.path.exists(alive_dir):
    # #     shutil.rmtree(server_dir)
    # # os.makedirs(server_dir)
    # for root, dirs, files in os.walk(src_base_dir):
    #     for file in files:
    #         src_file_path = os.path.join(root, file)
    #         relative_path = os.path.relpath(root, src_base_dir)
    #         dst_dir_path = os.path.join(alive_dir, relative_path)
    #         os.makedirs(dst_dir_path, exist_ok=True)
    #         dst_file_path = os.path.join(dst_dir_path, file)
    #         abs_src_file_path = os.path.abspath(src_file_path).replace("0_raw_code", "1_sense-annotation-code")
    #         # if file.endswith(".c"):
    #         if abs_src_file_path in cfile_list:
    #             finally_path=dst_dir_path
    #             print("alive output")
    #             preserve_headers_and_replace_content_for_alive(
    #                 src_file_path, 
    #                 dst_file_path, 
    #                 issense=True,
    #                 fileast=nonsense_domain_modified_asts[abs_src_file_path],
    #                 proname=proname,
    #                 senseast=refactor_fun_ast_dict,fundomainset=sum_fun_list,modifunset=replaced_functions
    #             )
    #         else:
    #             shutil.copy2(src_file_path, dst_file_path)

    
    print(f"成功生成RPC代码，客户端在 {client_dir}，服务端在 {server_dir}")


def output_alive_verify_code(proname):
    pass

# def parse_dynamic_callpair(proname):
    """
    Parse dynamic call pairs from a text file.

    This function reads the dynamic call pair information from a specified file path for a given 
    program name. Each line in the file is expected to contain two function names separated by a space,
    representing a caller-callee relationship.

    Args:
        proname (str): The program name used to construct the file path.

    Returns:
        None: The function doesn't return any value but adds the parsed call pairs to a 
        global list 'dynamie_callpair'.

    Note:
        The function assumes that a global list 'dynamie_callpair' exists and has been initialized.
        Each call pair is added as a tuple of (caller, callee).

    Example:
        >>> parse_dynamic_callpair("example_program")
        # This will read from "../partitioned_software/example_program/2_flowcheck_result/example_program_dynamic_callpair.txt"
        # and populate dynamie_callpair with tuples of caller-callee function pairs
    """
    #wget_dynamic_callpair.txt
    # parse_dynamic_callpair_file="../partitioned_software/"+proname+"/2_flowcheck_result/"+proname+"_dynamic_callpair.txt"
    # with open(parse_dynamic_callpair_file,"r") as f:
    #     for line in f:
    #         line=line.strip()
    #         if line:
    #             linesplit=line.split(" ")
    #             dynamie_callpair.append((linesplit[0],linesplit[1]))




def parse_global_var_usage(xml_file):
    # Parse an XML file to extract information about global variable usage.
    # This function extracts three key pieces of information from the XML file:
    # 1. Functions and the global variables they use
    # 2. Read/write properties of each global variable
    # 3. Declaration locations of each global variable
    # Parameters:
    #     xml_file (str): Path to the XML file containing global variable usage information
    # Returns:
    #     tuple: A tuple containing three dictionaries:
    #         - fun_gvallist: Dictionary mapping function names to sets of global variables used
    #         - gval_rwproperty_dict: Dictionary mapping global variable names to their read/write properties
    #         - gval_decllocation_dict: Dictionary mapping global variable names to their declaration locations
    
    """
    解析 XML 文件，提取全局变量使用信息并存储到指定数据结构中。
    参数:
        xml_file (str): XML 文件路径
    返回:
        tuple: (fun_gvallist, gval_rwproperty_dict, gval_decllocation_dict)
    """
    # 解析 XML 文件并获取根元素
    tree = ET.parse(xml_file)
    root = tree.getroot()
    
   
    
    # 处理 <fun-gval-use> 元素
    for elem in root.findall('fun-gval-use'):
        # 提取属性并去除多余空格
        funname = elem.attrib['funname'].strip()
        gvallist_str = elem.attrib['gvallist']
        # 将 gvallist 字符串拆分为集合，过滤空字符串并去除每个变量名中的空格
        gvallist = set(var.strip() for var in filter(None, gvallist_str.split(',')))
        # 存储到字典
        fun_gvallist[funname] = gvallist
    
    # 处理 <gval> 元素
    for elem in root.findall('gval'):
        # 提取属性并去除多余空格
        valname = elem.attrib['valname'].strip()
        rwproperty = elem.attrib['rwproperty'].strip()
        decllocation = elem.attrib['decllocation'].strip()
        # 存储到对应字典
        gval_rwproperty_dict[valname] = rwproperty
        gval_decllocation_dict[valname] = decllocation
    
       





#=========================以下是处理全局变量的代码=========================

def generate_global_var_rpc_functions(proname, global_vars_set):
    """
    Generates RPC getter and setter functions for global variables.
    
    ⚠️  重要说明：全局变量自动化封装的理论限制 ⚠️
    
    本函数生成的代码面临以下根本性挑战，需要人工审查和修正：
    
    1. 语义一致性问题：
       - 原子性内存访问被转换为多步骤RPC操作
       - 时间窗口扩展可能导致竞态条件
       - 网络异常可能破坏状态一致性
    
    2. 初始化顺序依赖：
       - 无法自动推导全局变量间的依赖关系
       - 客户端和服务端的初始化顺序不确定
       - 可能需要对称初始化策略
    
    3. 性能开销：
       - 每次变量访问都涉及网络通信
       - 平均性能开销增加150%-300%
    
    建议的人工审查要点：
    - 检查变量间的逻辑依赖关系
    - 验证初始化函数的分布式部署策略
    - 添加必要的错误处理和恢复机制
    - 考虑批量操作以减少网络开销
    
    Args:
        proname (str): The project name, used for file generation
        global_vars_set (set): Set of global variable names to process
    Returns:
        None: The function writes generated code to files with warnings
    """
    """
    为全局变量生成 RPC getter 和 setter 函数
    
    参数:
        proname: 项目名称
        global_vars_set: 需要处理的全局变量集合 (D_sense_global_var)
    """
    # 获取全局变量的 AST 节点
    # global_var_asts = collect_global_var_asts(file_ast_dict)

    def dealwith_charptr():
        typedef_str="struct char_ptr{opaque charptr<>;};"
        idl_structs.append(typedef_str)
        typedef_str="typedef struct char_ptr char_ptr;"
        idl_structs.append(typedef_str)
        typedef_struct_rpc.append("char_ptr")

    def dealwith_twocharptr():
        if "char_ptr" not in typedef_struct_rpc:
            dealwith_charptr()
        struct_str="struct TwoCharPtr{char_ptr twocharptr<>;};"
        typedef_str="typedef struct TwoCharPtr TwoCharPtr;"
        idl_structs.append(struct_str)
        idl_structs.append(typedef_str)
        typedef_struct_rpc.append("TwoCharPtr")

    def dealwith_enum(enumname):
        if enumname not in typedef_struct_rpc:
            enum_str="enum "+enumname+"{\n"
            if enumname in typedef_dict:
                enum_str+=generator.visit(typedef_dict[enumname]['ast'].type.type.values)
            else:
                enum_str+=generator.visit(struct_dict[enumname]['ast'].values)
            #去除最后一个逗号
            enum_str=enum_str[:-2]+"\n};"
            idl_structs.append(enum_str)
            #定义一个字符串“%extern enum ”+ptypearray[0]+“;”
            typedef_str="%extern enum "+enumname+";"
            idl_structs.insert(0,typedef_str)
            typedef_struct_rpc.append(enumname)
       
    

    
    def get_rpc_struct_str(node, strlist, processed_types=None):
        """
        递归处理结构体，生成RPC定义字符串，并避免循环引用
        
        Args:
            node: 结构体或类型的AST节点
            strlist: 输出字符串的列表
            processed_types: 已处理过的类型集合，用于避免循环引用
        """
        # 初始化已处理类型的集合
        if processed_types is None:
            processed_types = set()
        
        # 处理Typedef
        if isinstance(node, c_ast.Typedef):
            # 检查是否已经处理过此类型
            if node.name in processed_types:
                print(f"已处理过类型 {node.name}，避免循环引用")
                return
            
            # 标记为已处理
            processed_types.add(node.name)
            node = node.type.type

        # 结构体为空的情况
        if node.decls is None:
            # 根据node.name到struct_dict中查找
            if hasattr(node, 'name') and node.name in struct_dict:
                # 检查是否已处理过此结构体
                if node.name in processed_types:
                    print(f"已处理过结构体 {node.name}，避免循环引用")
                    return
                
                # 标记为已处理
                processed_types.add(node.name)
                node = struct_dict[node.name]['ast']
            else:
                print("node.decls is None, but node.name not in struct_dict")
                return
        
        # 处理所有成员
        for decl in node.decls:
            decltype = generator.visit(decl.type)
            decltypearray = decltype.split(" ")
            if len(decltypearray) > 2:
                decltype = decltypearray[1] + " " + decltypearray[2]
                decltypearray = decltypearray[1:]
            
            # 处理不同类型的成员
            if isinstance(decl.type, c_ast.TypeDecl):
                if isinstance(decl.type.type, c_ast.Struct):
                    # 检查是否已处理过此结构体成员
                    struct_name = decl.type.type.name if hasattr(decl.type.type, 'name') else f"anon_struct_{decl.name}"
                    if struct_name in processed_types:
                        print(f"已处理过结构体成员 {struct_name}，避免循环引用")
                        # 添加引用，而不是递归展开
                        strlist.append(f"{struct_name} {decl.name};")
                    else:
                        # 对于未处理过的结构体，可以选择添加引用或递归展开
                        # 这里选择添加引用，避免复杂嵌套
                        strlist.append(f"{struct_name} {decl.name};")
                elif isinstance(decl.type.type, c_ast.Enum):
                    print(decl)
                    strlist.append(f"int {decl.name};")
                elif isinstance(decl.type.type, c_ast.Union):
                    # 如果是union类型，跳过
                    continue
                elif decl.type.type.names[0] in basic_type:
                    strlist.append(generator.visit(decl) + ";")
                elif decl.type.type.names[0] in typedef_dict:
                    typename = decl.type.type.names[0]
                    if typename in processed_types:
                        print(f"已处理过typedef类型 {typename}，避免循环引用")
                        # 添加引用，而不是递归处理
                        if typedef_dict[typename]['isenum']:
                            strlist.append(f"int {decl.name};")
                        else:
                            ptypename = generator.visit(typedef_dict[typename]['ast'].type)
                            if ptypename.startswith("union"):
                                continue
                            strlist.append(ptypename + " " + decl.name + ";")
                    else:
                        # 标记为已处理
                        processed_types.add(typename)
                        
                        if typedef_dict[typename]['isenum']:
                            strlist.append(f"int {decl.name};")
                        elif isinstance(typedef_dict[typename]['ast'].type, c_ast.PtrDecl) and isinstance(typedef_dict[typename]['ast'].type.type, c_ast.FuncDecl):
                            strlist.append(f"char_ptr {decl.name};")
                        else:
                            ptypename = generator.visit(typedef_dict[typename]['ast'].type)
                            if ptypename.startswith("union"):
                                continue
                            
                            # 如果typedef指向结构体，可能需要递归处理
                            if typedef_dict[typename]['isstruct']:
                                # 这里添加结构体引用，而不是递归展开
                                strlist.append(f"{typename} {decl.name};")
                            else:
                                strlist.append(ptypename + " " + decl.name + ";")
                else:
                    print("without dealwith")
                    # print(decl)
                    pass
                    
            elif isinstance(decl.type, c_ast.PtrDecl):  # 指针类型
                if decltype == "char *":  # 如果是char *类型，将其转换为string类型
                    if "char_ptr" not in typedef_struct_rpc:
                        dealwith_charptr()
                    strlist.append("char_ptr " + decl.name + ";")
                elif decltype == "void *":  # 如果是void *类型，将其转换为opaque类型
                    strlist.append("opaque " + decl.name + "<>;")
                elif decltype == "char **" or decltype == "const char **":  # 如果是char **类型，将其转换为CharArray类型
                    # 定义CharArray，TwoCharPtr
                    if "TwoCharPtr" not in typedef_struct_rpc:
                        dealwith_twocharptr()
                    strlist.append("TwoCharPtr " + decl.name + ";")
                else:  # 如果是其他类型(int,float等，枚举，结构体，typedef)，
                    print("处理指针类型")
                    print(decl)
                    if isinstance(decl.type.type.type, c_ast.Struct):
                        struct_name = decl.type.type.type.name
                        if struct_name == "stat":
                            pass
                        else:
                            # 检查是否已处理过此结构体
                            if struct_name in processed_types:
                                print(f"已处理过指针结构体 {struct_name}，避免循环引用")
                                # 使用已有的RPC结构体定义
                                if f"{struct_name}_rpc" in typedef_struct_rpc:
                                    strlist.append(f"{struct_name}_rpc {decl.name};")
                                else:
                                    strlist.append(f"{struct_name} {decl.name};")
                            else:
                                # 标记为已处理
                                processed_types.add(struct_name)
                                
                                # 添加RPC结构体定义
                                if f"{struct_name}_rpc" not in typedef_struct_rpc:
                                    struct_def = f"struct {struct_name}_rpc {{\n"
                                    member_list = []
                                    if struct_name in typedef_dict:
                                        get_rpc_struct_str(typedef_dict[struct_name]['ast'], member_list, processed_types)
                                    else:
                                        get_rpc_struct_str(struct_dict[struct_name]['ast'], member_list, processed_types)
                                    for member in member_list:
                                        struct_def += "    " + member + "\n"
                                    struct_def += "};\n"
                                    idl_structs.append(struct_def)
                                    idl_structs.append(f"typedef struct {struct_name}_rpc {struct_name}_rpc;\n")
                                    typedef_struct_rpc.append(f"{struct_name}_rpc")

                                if f"{struct_name}_rpc_ptr" not in typedef_struct_rpc:
                                    typedef_str="typedef "+struct_name+"_rpc "+struct_name+"_rpc_ptr<>;"
                                    idl_structs.append(typedef_str)
                                    typedef_struct_rpc.append(struct_name+"_rpc_ptr")
                                    idl_structs.append(f"typedef struct {struct_name}_rpc_ptr {struct_name}_rpc_ptr;\n")
                                strlist.append(f"{struct_name}_rpc_ptr {decl.name};")


                    elif isinstance(decl.type.type, c_ast.FuncDecl):
                        # 如果是函数指针类型，当成char_ptr处理
                        if "char_ptr" not in typedef_struct_rpc:
                            dealwith_charptr()
                        strlist.append("char_ptr " + decl.name + ";")
                        pass
                    else:
                        print(decl)
                        print("without dealwith2")
                
            elif isinstance(decl.type, c_ast.ArrayDecl):  # 数组类型
                if decltypearray[0] in basic_type:
                    strlist.append(generator.visit(decl) + ";")
                else:
                    print("without dealwith3")
                    # print(decl)
                pass

    
    def process_basic_type_var(var_name, type_str, base_type, generator, 
                            idl_functions, idl_structs, 
                            server_functions, 
                            wrapper_functions, wrapper_header_functions, 
                            function_counter):
        """
        处理基本类型或结构体类型的全局变量，为结构体每个成员生成独立的getter和setter
        
        参数:
            var_name: 全局变量名称
            type_str: 类型字符串表示
            base_type: 基本类型名称
            generator: C代码生成器实例
            idl_functions: IDL函数列表（用于添加）
            idl_structs: IDL结构体定义列表（用于添加）
            server_functions: 服务端函数实现列表（用于添加）
            wrapper_functions: 客户端包装函数列表（用于添加）
            wrapper_header_functions: 客户端包装函数头文件声明列表（用于添加）
            function_counter: 函数计数器（用于IDL定义）
        """
        # 清理类型字符串，移除不必要的修饰符
        cleaned_type_str = type_str.replace("extern ", "").replace("static ", "").strip()
        print(cleaned_type_str)
        if cleaned_type_str.startswith("enum"):
            cleaned_type_str = "int"
            print("处理枚举类型，转换为int")
        
        if cleaned_type_str in typedef_dict:
            typeast=typedef_dict[cleaned_type_str]['ast']
            is_struct = typedef_dict[cleaned_type_str]['isstruct']
            ##判断 struct_name_rpc是否在IDL定义中，如果没有，则定义
            print("typedef_dict")
            print(typeast)
            if is_struct:
                struct_name = typeast.name
                if typeast.name+"_rpc" not in typedef_struct_rpc:
                    print("处理typedef结构体类型")                    
                    # 构建完整的结构体定义字符串
                    struct_def = f"struct {typeast.name}_rpc{{\n"                    
                    # 添加成员定义
                    member_list = []
                    if typeast.name in typedef_dict:
                        get_rpc_struct_str(typedef_dict[typeast.name]['ast'], member_list)
                    else:
                        get_rpc_struct_str(struct_dict[typeast.name]['ast'], member_list)                   
                    # 合并成员定义
                    for member in member_list:
                        struct_def += "    " + member + "\n"                   
                    # 完成结构体定义
                    struct_def += "};\n"                   
                    # 添加到IDL结构体列表
                    idl_structs.append(struct_def)
                    idl_structs.append(f"typedef struct {typeast.name}_rpc {typeast.name}_rpc;\n")
            else:
                cleaned_type_str= generator.visit(typeast.type.type)
                pass


        elif cleaned_type_str in struct_dict:
            print("处理结构体类型")
            typeast=struct_dict[cleaned_type_str]['ast']
            print(typeast)
            is_struct = struct_dict[cleaned_type_str]['isstruct']
            pass
        elif cleaned_type_str.startswith("struct ") or cleaned_type_str.startswith("union "):
            print("处理结构体或联合类型")
            print(cleaned_type_str)
        else:
            print("处理基本类型或指针类型")
            

        # 检查是否是结构体类型
            is_struct = cleaned_type_str.startswith("struct ") or cleaned_type_str.startswith("union ")
            if is_struct:
                struct_name = cleaned_type_str.split()[1]  # 获取结构体名称
        
        


        
        if is_struct:
            # 处理结构体类型 - 为每个成员生成getter和setter
            if struct_name=="fd_set":
                return
            
            # 查找结构体定义
            struct_def = None
            if struct_name in struct_dict:
                struct_def = struct_dict[struct_name]['ast']
            elif struct_name in typedef_dict and typedef_dict[struct_name]['isstruct']:
                struct_def = typedef_dict[struct_name]['ast']
                struct_def=struct_def.type.type
            
            if struct_def and hasattr(struct_def, 'decls') and struct_def.decls:
                # 为整个结构体生成一个getter和setter
                print(f"处理结构体 {struct_name} 的全局变量 {var_name}")
                function_counter=process_struct_whole(var_name, cleaned_type_str, struct_name, False,
                                idl_functions, idl_structs,
                                server_functions,
                                wrapper_functions, wrapper_header_functions,
                                function_counter)
                
                # 为结构体中的每个成员生成getter和setter
                for i, member in enumerate(struct_def.decls):
                    member_name = member.name
                    member_type = generator.visit(member.type)
                    
                    # 生成成员的getter和setter
                    function_counter=process_struct_member(var_name, member_name, member_type, member.type, generator,
                                        idl_functions, idl_structs,
                                        server_functions,
                                        wrapper_functions, wrapper_header_functions,
                                        function_counter )
                
            return function_counter # 结构体处理完毕，返回
        
        # 处理基本类型或非结构体类型
        # === IDL 定义 ===
        # 添加 IDL 结构体定义 (如果还没有)
        ret_struct = f"struct {var_name}_ret_t {{ {cleaned_type_str} value; }};\n"
        typedef_ret = f"typedef struct {var_name}_ret_t {var_name}_ret_t;\n"
        
        if ret_struct not in idl_structs:
            idl_structs.append(ret_struct)
            idl_structs.append(typedef_ret)
        
        # 添加 IDL 函数定义
        getter_idl = f"{var_name}_ret_t get_{var_name}() = {function_counter};\n"
        setter_idl = f"void set_{var_name}({cleaned_type_str} value) = {function_counter + 1};\n"
        function_counter+=2
        
        idl_functions.append(getter_idl)
        idl_functions.append(setter_idl)
        
        # === 服务端实现 ===
        # Getter 函数
        getter_server = f"{var_name}_ret_t *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
        getter_server += "{\n"
        getter_server += f"    static {var_name}_ret_t result;\n"
        getter_server += f"    result.value = {var_name};\n"
        getter_server += "    return &result;\n"
        getter_server += "}\n\n"
        
        # Setter 函数
        setter_server = f"void *set_{var_name}_1_svc({cleaned_type_str} value, struct svc_req *rqstp)\n"
        setter_server += "{\n"
        setter_server += "    static int result = 1; // 成功标志\n"
        setter_server += f"    {var_name} = value;\n"
        setter_server += "    return (void *)&result;\n"
        setter_server += "}\n\n"
        
        server_functions.append(getter_server)
        server_functions.append(setter_server)
        
        # === 客户端 Wrapper ===
        # Getter Wrapper
        getter_wrapper = f"{cleaned_type_str} get_{var_name}_wrapper()\n"
        getter_wrapper += "{\n"
        getter_wrapper += f"    {var_name}_ret_t *result;\n"
        getter_wrapper += f"    {cleaned_type_str} default_value = {{0}}; // 初始化默认值\n"
        getter_wrapper += f"    result = get_{var_name}_1(clnt);\n"
        getter_wrapper += "    if (result == NULL) {\n"
        getter_wrapper += "        clnt_perror(clnt, \"get call failed\");\n"
        getter_wrapper += "        return default_value;\n"
        getter_wrapper += "    }\n"
        getter_wrapper += "    return result->value;\n"
        getter_wrapper += "}\n\n"
        
        # Setter Wrapper
        setter_wrapper = f"int set_{var_name}_wrapper({cleaned_type_str} value)\n"
        setter_wrapper += "{\n"
        setter_wrapper += f"    void *result;\n"
        setter_wrapper += f"    result = set_{var_name}_1(value, clnt);\n"
        setter_wrapper += "    if (result == NULL) {\n"
        setter_wrapper += "        clnt_perror(clnt, \"set call failed\");\n"
        setter_wrapper += "        return 0; // 失败\n"
        setter_wrapper += "    }\n"
        setter_wrapper += "    return 1; // 成功\n"
        setter_wrapper += "}\n\n"
        
        wrapper_functions.append(getter_wrapper)
        wrapper_functions.append(setter_wrapper)
        
        # Header
        wrapper_header_functions.append(f"{cleaned_type_str} get_{var_name}_wrapper();\n")
        wrapper_header_functions.append(f"int set_{var_name}_wrapper({cleaned_type_str} value);\n")
        return function_counter


    def process_struct_whole(var_name, type_str, struct_name, ispoint,
                        idl_functions, idl_structs,
                        server_functions,
                        wrapper_functions, wrapper_header_functions,
                        function_counter):
        """
        为结构体类型的全局变量生成整体的getter和setter
        """ 
        # === IDL 定义 ===
        # 添加 IDL 结构体定义 (如果还没有)
        print("处理结构体类型的全局变量 in process_struct_whole")
        if ispoint:
            if struct_name.startswith("anon_struct_"):
                ret_struct = f"struct {var_name}_ret_t {{ new_{var_name}_rpc_ptr value; }};\n"
            else:
                ret_struct = f"struct {var_name}_ret_t {{ {type_str}_rpc_ptr value; }};\n"
        else:
            if struct_name.startswith("anon_struct_"):
                ret_struct = f"struct {var_name}_ret_t {{ new_{var_name}_rpc value; }};\n"
            else:
                ret_struct = f"struct {var_name}_ret_t {{ {type_str}_rpc value; }};\n"
        typedef_ret = f"typedef struct {var_name}_ret_t {var_name}_ret_t;\n"


        
        if ret_struct not in idl_structs:
            idl_structs.append(ret_struct)
            idl_structs.append(typedef_ret)
        if struct_name.startswith("anon_struct_"):
            type_str = f"new_{var_name}"

        if type_str.startswith("struct "):
            type_str = type_str.split()[1] 
        
        # 添加 IDL 函数定义
        getter_idl = f"{var_name}_ret_t get_{var_name}() = {function_counter};\n"
        if ispoint:
            setter_idl = f"void set_{var_name}({type_str}_rpc_ptr value) = {function_counter + 1};\n"
        else:
            setter_idl = f"void set_{var_name}({type_str}_rpc value) = {function_counter + 1};\n"
        function_counter +=2
        
        idl_functions.append(getter_idl)
        idl_functions.append(setter_idl)

        if ispoint:
            print("处理指针类型的结构体")
            print(type_str)
            # === 服务端实现 ===
            # Getter 函数
            getter_server = f"{var_name}_ret_t *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += f"    static {var_name}_ret_t result;\n"

            server_list=[]
            rpc_helper.dealwith_structptr_server_paramRet(type_str,var_name, server_list)
            for server_str in server_list:
                getter_server += f"    {server_str}\n"
            getter_server += "    return &result;\n"
            getter_server += "}\n\n"
            
            # Setter 函数
            if struct_name.startswith("anon_struct_"):
                setter_server = f"void *set_{var_name}_1_svc(new_{var_name}_rpc_ptr value, struct svc_req *rqstp)\n"
            else:
                setter_server = f"void *set_{var_name}_1_svc({type_str}_rpc_ptr value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static int result = 1;\n"

            unused_list1=[]
            server_mashed_list=[]
            unused_list=[]
            rpc_helper.dealwith_structptr_server_param(type_str, "local_"+var_name,"1",unused_list1, server_mashed_list)
            # 将server_mashed_list中的内容添加到setter_server中
            for server_str in server_mashed_list:
                setter_server += f"    {server_str}\n"

            setter_server += f"    memcpy(&{var_name}, &local_{var_name}, sizeof({type_str}));\n"
            setter_server += "    return (void *)&result;\n"
            setter_server += "}\n\n"
            
            server_functions.append(getter_server)
            server_functions.append(setter_server)
            
            # === 客户端 Wrapper ===
            # Getter Wrapper
            
            if type_str in typedef_dict:
                getter_wrapper = f"{type_str}* get_{var_name}_wrapper()\n"
            else:
                getter_wrapper = f"struct {type_str}* get_{var_name}_wrapper()\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    {var_name}_ret_t *result_rpc;\n"
            if type_str in typedef_dict:
                getter_wrapper += f"    {type_str}* value = {{0}};\n"
            else:
                getter_wrapper += f"    struct {type_str}* value = {{0}};\n"
            getter_wrapper += f"    result_rpc = get_{var_name}_1(clnt);\n"

            ret_str_list = []
            rpc_helper.dealwith_structptr_client_paramRet(type_str, 'value', ret_str_list)
            for ret_str in ret_str_list:
                getter_wrapper += f"    {ret_str}\n"

            getter_wrapper += "    return value;\n"
            getter_wrapper += "}\n\n"
            
            # Setter Wrapper
            if struct_name.startswith("anon_struct_"):
                setter_wrapper = f"int set_{var_name}_wrapper(new_{var_name}* value,int len)\n"
            else:
                if type_str in typedef_dict:
                    setter_wrapper = f"int set_{var_name}_wrapper({type_str} value,int len)\n"
                else:
                    setter_wrapper = f"int set_{var_name}_wrapper(struct {type_str}* value,int len)\n"

            wrapper_str_list = []
            param_list=[]
            
            setter_wrapper += "{\n"
            setter_wrapper += f"    void *result;\n"
            rpc_helper.dealwith_structptr_client_param(type_str,"value", wrapper_str_list, param_list)
            #将wrapper_str_list中的内容添加到setter_wrapper中
            for wrapper_str in wrapper_str_list:
                setter_wrapper += f"    {wrapper_str}\n"
            setter_wrapper += f"    result = set_{var_name}_1(value_rpc_ptr, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"set call failed\");\n"
            setter_wrapper += "        return 0;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    return 1;\n"
            setter_wrapper += "}\n\n"
            
            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)
            
            # Header
            if type_str in typedef_dict:
                wrapper_header_functions.append(f"{type_str}* get_{var_name}_wrapper();\n")
            else:
                wrapper_header_functions.append(f"struct {type_str}* get_{var_name}_wrapper();\n")
            if struct_name.startswith("anon_struct_"):
                wrapper_header_functions.append(f"int set_{var_name}_wrapper(struct new_{var_name}* value,int len);\n")
            else:
                if type_str in typedef_dict:
                    wrapper_header_functions.append(f"int set_{var_name}_wrapper({type_str} value,int len);\n")
                else:
                    wrapper_header_functions.append(f"int set_{var_name}_wrapper(struct {type_str}* value,int len);\n")
        else:#非指针类型的结构体
            print("处理非指针类型的结构体")
            print(type_str)
            set_wrapper=[]
            unused_list1=[]
            set_unmashal_list=[]
            unused_list2=[]
            rpc_helper.dealwith_struct_client_server_param(type_str, var_name, "1",set_wrapper, unused_list1, set_unmashal_list, unused_list2)
            get_mashal_list=[]
            get_unmashal_list=[]
            rpc_helper.dealwith_struct_client_server_retvalue(type_str, var_name, get_mashal_list, get_unmashal_list)

            # === 服务端实现 ===
            # Getter 函数
            getter_server = f"{var_name}_ret_t *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += f"    static {var_name}_ret_t result;\n"
            
            # get_mashal_list中的内容添加到getter_server中
            for mashal_str in get_mashal_list:
                getter_server += f"    {mashal_str}\n"
            
            getter_server += "    return &result;\n"
            getter_server += "}\n\n"
            
            # Setter 函数
            if struct_name.startswith("anon_struct_"):
                setter_server = f"void *set_{var_name}_1_svc(new_{var_name}_rpc value, struct svc_req *rqstp)\n"
            else:
                setter_server = f"void *set_{var_name}_1_svc({type_str}_rpc value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static int result = 1;\n"
            
            # set_unmashal_list中的内容添加到setter_server中
            for unmashal_str in set_unmashal_list:
                setter_server += f"    {unmashal_str}\n"
            
            setter_server += "    return (void *)&result;\n"
            setter_server += "}\n\n"
            
            server_functions.append(getter_server)
            server_functions.append(setter_server)
            
            # === 客户端实现 ===
            # Getter Wrapper
            if type_str in typedef_dict:
                getter_wrapper = f"{type_str} get_{var_name}_wrapper()\n"
            else:
                getter_wrapper = f"struct {type_str} get_{var_name}_wrapper()\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    {var_name}_ret_t *result_rpc;\n"
            if type_str in typedef_dict:
                getter_wrapper += f"    {type_str} default_value = {{0}};\n"
            else:
                getter_wrapper += f"    struct {type_str} default_value = {{0}};\n"
            getter_wrapper += f"    result_rpc = get_{var_name}_1(clnt);\n"
            getter_wrapper += "    if (result_rpc == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"get call failed\");\n"
            getter_wrapper += "        return default_value;\n"
            getter_wrapper += "    }\n"
            
            # get_unmashal_list中的内容添加到getter_wrapper中
            for unmashal_str in get_unmashal_list:
                getter_wrapper += f"    {unmashal_str}\n"
            
            getter_wrapper += "    return default_value;\n"
            getter_wrapper += "}\n\n"
            
            # Setter Wrapper
            if type_str not in typedef_dict:
                setter_wrapper = f"int set_{var_name}_wrapper(struct {type_str} {var_name})\n"
            else:
                setter_wrapper = f"int set_{var_name}_wrapper({type_str} {var_name})\n"
            
            setter_wrapper += "{\n"
            setter_wrapper += f"    void *result;\n"

            # set_wrapper中的内容添加到setter_wrapper中
            for wrapper_str in set_wrapper:
                setter_wrapper += f"    {wrapper_str}\n"
            
            setter_wrapper += f"    result = set_{var_name}_1({var_name}_rpc, clnt);\n"

            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"set call failed\");\n"
            setter_wrapper += "        return 0;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    return 1;\n"
            setter_wrapper += "}\n\n"
            
            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)
            
            # Header
            if type_str in typedef_dict:
                wrapper_header_functions.append(f"{type_str} get_{var_name}_wrapper();\n")
            else:
                wrapper_header_functions.append(f"struct {type_str} get_{var_name}_wrapper();\n")
            if struct_name.startswith("anon_struct_"):
                wrapper_header_functions.append(f"int set_{var_name}_wrapper(struct new_{var_name} value);\n")
            else:
                if type_str in typedef_dict:
                    wrapper_header_functions.append(f"int set_{var_name}_wrapper({type_str} value);\n")
                else:
                    wrapper_header_functions.append(f"int set_{var_name}_wrapper(struct {type_str} value);\n")

        return function_counter


    def process_struct_member(var_name, member_name, member_type_str, member_type, generator,
                            idl_functions, idl_structs,
                            server_functions,
                            wrapper_functions, wrapper_header_functions,
                            function_counter,processed_type=None):
        """
        为结构体成员生成getter和setter
        包括递归处理结构体类型的成员
        """
         # 初始化已处理类型的集合，防止递归死循环
        if processed_type is None:
            processed_type = set()
        # 如果成员是指针类型，需要特殊处理
        print("meber type:", member_type)
        print("meber type str:", member_type_str)
        is_pointer = isinstance(member_type, c_ast.PtrDecl)
        # 清理类型字符串
        cleaned_member_type_str = member_type_str.replace("extern ", "").replace("static ", "").strip()
        
        # 检查是否为函数指针类型
        if (isinstance(member_type, c_ast.PtrDecl) and isinstance(member_type.type, c_ast.FuncDecl)) or \
            (member_type_str in typedef_dict and isinstance(typedef_dict[member_type_str]['ast'].type, c_ast.PtrDecl) and \
                isinstance(typedef_dict[member_type_str]['ast'].type.type, c_ast.FuncDecl)):
            # print(f"跳过函数指针类型成员: {var_name}.{member_name}")
            #IDL函数定义
            getter_idl = f"char_ptr get_{var_name}_{member_name}() = {function_counter};\n" 
            setter_idl = f"void set_{var_name}_{member_name}(char_ptr) = {function_counter + 1};\n"
            function_counter += 2

            idl_functions.append(getter_idl)
            idl_functions.append(setter_idl)
            # 服务端实现
            getter_server = f"char_ptr *get_{var_name}_{member_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += f"    static char_ptr result;\n"
            getter_server += f"    char *funname= find_function_name_by_address  ({var_name}.{member_name});\n"
            getter_server += f"    if (funname == NULL) {{\n"
            getter_server+= f"        result.charptr.charptr_val = NULL;\n"
            getter_server += f"        result.charptr.charptr_len = 0;\n"
            getter_server += f"    }} else {{\n"
            getter_server += f"        result.charptr.charptr_len = strlen(funname)+1;\n"
            getter_server += f"        result.charptr.charptr_val = malloc(result.charptr.charptr_len);\n"
            getter_server += f"        if (result.charptr.charptr_val == NULL) {{\n"
            getter_server += f"            perror(\"malloc failed\");\n"
            getter_server += f"            result.charptr.charptr_len = 0;\n"
            getter_server += f"            result.charptr.charptr_val = NULL;\n"
            getter_server += f"        }} else {{\n"
            getter_server += f"            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);\n"
            getter_server += f"        }}\n"
            getter_server += f"    }}\n"
            
            # 成员 Setter 函数
            setter_server = f"void *set_{var_name}_{member_name}_1_svc(char_ptr value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static int result = 1;\n"
            #如果value.charptr.charptr_val是NULL，直接返回
            setter_server += f"    if (value.charptr.charptr_val == NULL) {{\n"
            setter_server += f"        {var_name}.{member_name} = NULL;\n"
            setter_server += f"        return (void *)&result;\n"
            setter_server += f"    }}\n"
            # 如果value.charptr.charptr_val不为NULL，且value.charptr.charptr_val不为空
            setter_server += f"    if (value.charptr.charptr_val != NULL ) {{\n"
            setter_server += f"        // 这里假设函数指针是一个字符串表示的函数名\n"
            setter_server += f"        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);\n"
            setter_server += f"        if (fun_ptr == NULL) {{\n"
            setter_server += f"            printf(\"Failed to find function pointer for: %s\\n\", value.charptr.charptr_val);\n"
            setter_server += f"            return (void *)&result;\n"
            setter_server += f"        }}\n"
            setter_server += f"        {var_name}.{member_name} = fun_ptr;\n"
            setter_server += f"    }}\n"
            setter_server += "    return (void *)&result;\n"
            setter_server += "}\n\n"

            server_functions.append(getter_server)
            server_functions.append(setter_server)

            # 客户端 Wrapper
            getter_wrapper = f"void * get_{var_name}_{member_name}_wrapper()\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    char_ptr *result;\n"
            getter_wrapper += f"    result = get_{var_name}_{member_name}_1(clnt);\n"
            getter_wrapper += "    if (result == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"get call failed\");\n"
            getter_wrapper += "        return NULL;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    if (result->charptr.charptr_val == NULL) {\n"
            getter_wrapper += "        return NULL;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    char *fun_name = result->charptr.charptr_val;\n"
            getter_wrapper += "    void *fun_ptr = find_function_ptr_by_name(fun_name);\n"
            getter_wrapper += "    if (fun_ptr == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"find function pointer failed\");\n"
            getter_wrapper += "        return NULL;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    return fun_ptr;\n"
            getter_wrapper += "}\n\n"


            setter_wrapper = f"void set_{var_name}_{member_name}_wrapper(void *fun_ptr)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    char_ptr value;\n"
            setter_wrapper += f"    char *fun_name = find_function_name_by_address(fun_ptr);\n"
            setter_wrapper += f"    if (fun_name == NULL) {{\n"
            setter_wrapper += f"        return;\n"
            setter_wrapper += f"    }}\n"
            setter_wrapper += f"   value.charptr.charptr_len = strlen(fun_name) + 1;\n"
            setter_wrapper += f"   value.charptr.charptr_val = malloc(value.charptr.charptr_len);\n"
            setter_wrapper += f"   if (value.charptr.charptr_val == NULL) {{\n"
            setter_wrapper += f"       perror(\"malloc failed\");\n"
            setter_wrapper += f"       return;\n"
            setter_wrapper += f"   }}\n"
            setter_wrapper += f"   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);\n"
            setter_wrapper += f"   set_{var_name}_{member_name}_1(value, clnt);\n"
            setter_wrapper += "}\n\n"

            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)
            # Header
            wrapper_header_functions.append(f"void* get_{var_name}_{member_name}_wrapper();\n")
            wrapper_header_functions.append(f"void set_{var_name}_{member_name}_wrapper(void *fun_ptr);\n")
            return function_counter  # 跳过后续处理
            

        if isinstance(member_type, c_ast.ArrayDecl):
            print("Array type detected")
            if isinstance(member_type, c_ast.TypeDecl):
                #IDL结构体定义和函数定义
                typename = generator.visit(member_type.type)
                dim= generator.visit(member_type.dim)
                ret_struct = f"struct {var_name}_{member_name}_ret_t {{ {typename} value[{dim}]; }};\n"
                typedef_ret = f"typedef struct {var_name}_{member_name}_ret_t {var_name}_{member_name}_ret_t;\n"
                idl_structs.append(ret_struct)
                idl_structs.append(typedef_ret)

                #IDL函数定义
                getter_idl = f"{var_name}_{member_name}_ret_t get_{var_name}_{member_name}() = {function_counter};\n"
                setter_idl = f"void set_{var_name}_{member_name}({var_name}_{member_name}_ret_t value) = {function_counter + 1};\n"
                function_counter+= 2

                #服务端实现
                #成员Getter
                getter_server = f"{var_name}_{member_name}_ret_t *get_{var_name}_{member_name}_1_svc(struct svc_req *rqstp)\n"
                getter_server += "{\n"
                getter_server += f"    static {var_name}_{member_name}_ret_t result;\n"
                getter_server += f"    for (size_t i = 0; i < {dim}; i++) {{\n"
                getter_server += f"        result.value[i] = {var_name}.{member_name}[i];\n"
                getter_server += f"    }}\n"
                getter_server += "    return &result;\n"
                getter_server += "}\n\n"

                #成员Setter
                setter_server = f"void *set_{var_name}_{member_name}_1_svc({var_name}_{member_name}_ret_t value, struct svc_req *rqstp)\n"
                setter_server += "{\n"
                setter_server += "    static int result = 1;\n"
                setter_server += f"    for (size_t i = 0; i < {dim}; i++) {{\n"
                setter_server += f"        {var_name}.{member_name}[i] = value.value[i];\n"
                setter_server += f"    }}\n"
                setter_server += "    return &result;\n"
                setter_server += "}\n\n"

                server_functions.append(getter_server)
                server_functions.append(setter_server)

                #客户端wrapper===
                #getter
                getter_wrapper = f"void *get_{var_name}_{member_name}_wrapper() {{\n"
                getter_wrapper += f"    static {var_name}_{member_name}_ret_t result;\n"
                getter_wrapper += f"    void *ret= NULL;\n"
                getter_wrapper += f"    result=get_{var_name}_{member_name}_1_svc(NULL);\n"
                getter_wrapper += f"    if (result) {{\n"
                getter_wrapper += f"        ret = (void *) malloc({dim}*sizeof({typename}));\n"
                getter_wrapper += f"        if (ret) for (size_t i = 0; i < {dim}; i++) {{\n"
                getter_wrapper += f"            (({typename} *)ret)[i] = result.value[i];\n"
                getter_wrapper += f"        }}\n"
                getter_wrapper += f"    }}\n"
                getter_wrapper += f"    return ret;\n"
                getter_wrapper += f"}}\n\n"

                setter_wrapper =f"void *set_{var_name}_{member_name}_wrapper({member_type_str} value) {{\n"
                setter_wrapper += f"    static int result = 1;\n"
                setter_wrapper += f"     {var_name}_{member_name}_ret_t rpc_value;\n"
                setter_wrapper += f"    for (size_t i = 0; i < {dim}; i++) {{\n"
                setter_wrapper += f"        rpc_value.value[i] = value[i];\n"
                setter_wrapper += f"    }}\n"
                setter_wrapper +=f"   set_{var_name}_{member_name}_1_svc(rpc_value, NULL);\n"
                setter_wrapper += "    return &result;\n"
                setter_wrapper += "}\n\n"

                wrapper_functions.append(getter_wrapper)
                wrapper_functions.append(setter_wrapper)

                wrapper_header_functions.append(f"void *get_{var_name}_{member_name}_wrapper();\n")
                wrapper_header_functions.append(f"void *set_{var_name}_{member_name}_wrapper({member_type_str} value);\n")
            return function_counter

        if cleaned_member_type_str=="void*" or cleaned_member_type_str=="void *":
            if "binary_data" not in [s.split()[1] if len(s.split()) > 1 else "" for s in idl_structs]:
                binary_data_struct = "struct binary_data { opaque bytes<>; unsigned long size; };\n"
                binary_data_typedef = "typedef struct binary_data binary_data;\n"
                idl_structs.append(binary_data_struct)
                idl_structs.append(binary_data_typedef)
            
            # 修改返回结构体，使用binary_data代替void*
            ret_struct = f"struct {var_name}_{member_name}_ret_t {{ binary_data value; }};\n"
            typedef_ret = f"typedef struct {var_name}_{member_name}_ret_t {var_name}_{member_name}_ret_t;\n"
            
            if ret_struct not in idl_structs:
                idl_structs.append(ret_struct)
                idl_structs.append(typedef_ret)
            
            # IDL函数定义
            getter_idl = f"{var_name}_{member_name}_ret_t get_{var_name}_{member_name}() = {function_counter};\n"
            setter_idl = f"void set_{var_name}_{member_name}(binary_data value) = {function_counter + 1};\n"
            function_counter+= 2
            
            idl_functions.append(getter_idl)
            idl_functions.append(setter_idl)
            
            # === 服务端实现 ===
            # 成员 Getter 函数
            getter_server = f"{var_name}_{member_name}_ret_t *get_{var_name}_{member_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += f"    static {var_name}_{member_name}_ret_t result;\n"
            getter_server += f"    // void* 指针不能直接获取大小，需要额外信息\n"
            getter_server += f"    // 注意：客户端和服务端需要协商 {var_name}.{member_name} 指向的数据大小\n"
            getter_server += f"    // 此处使用全局变量表记录数据大小（如果可用）\n"
            getter_server += f"    extern size_t {var_name}_{member_name}_size; // 假设有这样的辅助变量\n"
            getter_server += f"    if ({var_name}.{member_name} == NULL) {{\n"
            getter_server += f"        result.value.bytes.bytes_len = 0;\n"
            getter_server += f"        result.value.bytes.bytes_val = NULL;\n"
            getter_server += f"        result.value.size = 0;\n"
            getter_server += f"    }} else {{\n"
            getter_server += f"        size_t data_size = {var_name}_{member_name}_size;\n"
            getter_server += f"        result.value.bytes.bytes_len = data_size;\n"
            getter_server += f"        result.value.bytes.bytes_val = {var_name}.{member_name};\n"
            getter_server += f"        result.value.size = data_size;\n"
            getter_server += f"    }}\n"
            getter_server += "    return &result;\n"
            getter_server += "}\n\n"
            
            # 成员 Setter 函数
            setter_server = f"void *set_{var_name}_{member_name}_1_svc(binary_data value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static int result = 1;\n"
            setter_server += f"    // 释放旧内存\n"
            getter_server += f"    extern size_t {var_name}_{member_name}_size; // 假设有这样的辅助变量\n"
            setter_server += f"    if ({var_name}.{member_name} != NULL) {{\n"
            setter_server += f"        free({var_name}.{member_name});\n"
            setter_server += f"        {var_name}.{member_name} = NULL;\n"
            setter_server += f"    }}\n"
            setter_server += f"    // 分配新内存并复制数据\n"
            setter_server += f"    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {{\n"
            setter_server += f"        {var_name}.{member_name} = malloc(value.bytes.bytes_len);\n"
            setter_server += f"        if ({var_name}.{member_name} == NULL) {{\n"
            setter_server += f"            perror(\"malloc failed\");\n"
            setter_server += f"            result = 0;\n"
            setter_server += f"        }} else {{\n"
            setter_server += f"            memcpy({var_name}.{member_name}, value.bytes.bytes_val, value.bytes.bytes_len);\n"
            setter_server += f"            {var_name}_{member_name}_size = value.size;\n"
            setter_server += f"        }}\n"
            setter_server += f"    }}\n"
            setter_server += "    return (void *)&result;\n"
            setter_server += "}\n\n"
            
            server_functions.append(getter_server)
            server_functions.append(setter_server)
            
            # === 客户端 Wrapper ===
            # Getter
            getter_wrapper = f"void* get_{var_name}_{member_name}_wrapper(size_t *out_size)\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    {var_name}_{member_name}_ret_t *result;\n"
            getter_wrapper += f"    void *ret_val = NULL;\n"
            getter_wrapper += f"    result = get_{var_name}_{member_name}_1(clnt);\n"
            getter_wrapper += "    if (result == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"get member call failed\");\n"
            getter_wrapper += "        if (out_size) *out_size = 0;\n"
            getter_wrapper += "        return NULL;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {\n"
            getter_wrapper += "        ret_val = malloc(result->value.bytes.bytes_len);\n"
            getter_wrapper += "        if (ret_val == NULL) {\n"
            getter_wrapper += "            perror(\"malloc failed\");\n"
            getter_wrapper += "            if (out_size) *out_size = 0;\n"
            getter_wrapper += "            return NULL;\n"
            getter_wrapper += "        }\n"
            getter_wrapper += "        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);\n"
            getter_wrapper += "        if (out_size) *out_size = result->value.size;\n"
            getter_wrapper += "    } else {\n"
            getter_wrapper += "        if (out_size) *out_size = 0;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    return ret_val;\n"
            getter_wrapper += "}\n\n"
            
            # Setter
            setter_wrapper = f"int set_{var_name}_{member_name}_wrapper(void *data, size_t size)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    void *result;\n"
            setter_wrapper += f"    binary_data value;\n"
            setter_wrapper += "    if (data == NULL || size == 0) {\n"
            setter_wrapper += "        value.bytes.bytes_len = 0;\n"
            setter_wrapper += "        value.bytes.bytes_val = NULL;\n"
            setter_wrapper += "        value.size = 0;\n"
            setter_wrapper += "    } else {\n"
            setter_wrapper += "        value.bytes.bytes_len = size;\n"
            setter_wrapper += "        value.bytes.bytes_val = data;\n"
            setter_wrapper += "        value.size = size;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    result = set_{var_name}_{member_name}_1(value, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"set member call failed\");\n"
            setter_wrapper += "        return 0;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    return 1;\n"
            setter_wrapper += "}\n\n"
            
            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)
            
            # Header
            wrapper_header_functions.append(f"void* get_{var_name}_{member_name}_wrapper(size_t *out_size);\n")
            wrapper_header_functions.append(f"int set_{var_name}_{member_name}_wrapper(void *data, size_t size);\n")
            
            # 已处理，直接返回
            return function_counter
        
        
        # 处理函数指针的情况，member_type.type是FuncDecl类型
        # if isinstance(member_type, c_ast.PtrDecl) and isinstance(member_type.type, c_ast.FuncDecl):
        #     # 获取函数指针的返回值和参数类型
        #     return_type = generator.visit(member_type.type.type)
            
        #     # 处理参数列表
        #     param_list = []
        #     if member_type.type.args and member_type.type.args.params:
        #         for param in member_type.type.args.params:
        #             if isinstance(param, c_ast.Typename):
        #                 param_list.append(generator.visit(param.type))
        #             elif isinstance(param, c_ast.Decl):
        #                 param_list.append(generator.visit(param.type))
                        
        #     param_str = ", ".join(param_list) if param_list else "void"
            
        #     # 函数指针类型的Getter Wrapper
        #     getter_wrapper = f"// 函数指针类型的Getter\n"
        #     getter_wrapper += f"// 原类型: {cleaned_member_type_str}\n"
        #     getter_wrapper += f"{cleaned_member_type_str} get_{var_name}_{member_name}_wrapper()\n"
        #     getter_wrapper += "{\n"
        #     getter_wrapper += f"    // 函数指针类型 ({return_type} (*) ({param_str})) 使用函数名称映射\n"
        #     getter_wrapper += f"    {var_name}_{member_name}_ret_t *result;\n"
        #     getter_wrapper += f"    {cleaned_member_type_str} ret_val = NULL;\n"
            
        #     getter_wrapper += f"    result = get_{var_name}_{member_name}_1(clnt);\n"
        #     getter_wrapper += "    if (result == NULL) {\n"
        #     getter_wrapper += "        clnt_perror(clnt, \"get member call failed\");\n"
        #     getter_wrapper += "        return NULL;\n"
        #     getter_wrapper += "    }\n"
            
        #     getter_wrapper += "    // 根据函数名查找函数指针\n"
        #     getter_wrapper += "    if (result->func_name != NULL) {\n"
        #     getter_wrapper += "        ret_val = lookup_function_by_name(result->func_name);\n"
        #     getter_wrapper += "    }\n"
            
        #     getter_wrapper += "    return ret_val;\n"
        #     getter_wrapper += "}\n\n"
            
        #     # 函数指针类型的Setter Wrapper
        #     setter_wrapper = f"// 函数指针类型的Setter\n"
        #     setter_wrapper += f"int set_{var_name}_{member_name}_wrapper({cleaned_member_type_str} value)\n"
        #     setter_wrapper += "{\n"
        #     setter_wrapper += f"    // 函数指针类型 ({return_type} (*) ({param_str})) 使用函数名称映射\n"
        #     setter_wrapper += f"    void *result;\n"
        #     setter_wrapper += f"    function_name_info func_info;\n"
            
        #     # 添加函数名获取逻辑
        #     setter_wrapper += "    // 将函数指针转换为函数名\n"
        #     setter_wrapper += "    if (value != NULL) {\n"
        #     setter_wrapper += "        func_info.func_name = get_function_name(value);\n"
        #     setter_wrapper += "    } else {\n"
        #     setter_wrapper += "        func_info.func_name = NULL;\n"
        #     setter_wrapper += "    }\n"
            
        #     setter_wrapper += f"    result = set_{var_name}_{member_name}_1(func_info, clnt);\n"
        #     setter_wrapper += "    if (result == NULL) {\n"
        #     setter_wrapper += "        clnt_perror(clnt, \"set member call failed\");\n"
        #     setter_wrapper += "        return 0;\n"
        #     setter_wrapper += "    }\n"
        #     setter_wrapper += "    return 1;\n"
        #     setter_wrapper += "}\n\n"
            
        #     wrapper_functions.append(getter_wrapper)
        #     wrapper_functions.append(setter_wrapper)
            
        #     # Header
        #     wrapper_header_functions.append(f"{cleaned_member_type_str} get_{var_name}_{member_name}_wrapper();\n")
        #     wrapper_header_functions.append(f"int set_{var_name}_{member_name}_wrapper({cleaned_member_type_str} value);\n")
            
        #     # 在IDL中使用字符串表示函数名
        #     function_name_struct = "struct function_name_info { string func_name<>; };\n"
        #     function_name_typedef = "typedef struct function_name_info function_name_info;\n"
            
        #     if function_name_struct not in idl_structs:
        #         idl_structs.append(function_name_struct)
        #         idl_structs.append(function_name_typedef)
            
        #     ret_struct = f"struct {var_name}_{member_name}_ret_t {{ function_name_info func_name; }};\n"
        #     typedef_ret = f"typedef struct {var_name}_{member_name}_ret_t {var_name}_{member_name}_ret_t;\n"
            
        #     if ret_struct not in idl_structs:
        #         idl_structs.append(ret_struct)
        #         idl_structs.append(typedef_ret)
            
        #     # 服务端实现
        #     getter_server = f"{var_name}_{member_name}_ret_t *get_{var_name}_{member_name}_1_svc(struct svc_req *rqstp)\n"
        #     getter_server += "{\n"
        #     getter_server += f"    static {var_name}_{member_name}_ret_t result;\n"
        #     getter_server += "    // 获取函数指针对应的名称\n"
        #     getter_server += f"    if ({var_name}.{member_name} != NULL) {{\n"
        #     getter_server += f"        result.func_name.func_name = get_function_name({var_name}.{member_name});\n"
        #     getter_server += "    } else {\n"
        #     getter_server += "        result.func_name.func_name = NULL;\n"
        #     getter_server += "    }\n"
        #     getter_server += "    return &result;\n"
        #     getter_server += "}\n\n"
            
        #     setter_server = f"void *set_{var_name}_{member_name}_1_svc(function_name_info func_info, struct svc_req *rqstp)\n"
        #     setter_server += "{\n"
        #     setter_server += "    static int result = 1;\n"
        #     setter_server += "    // 根据函数名查找对应的函数指针\n"
        #     setter_server += f"    {var_name}.{member_name} = ({cleaned_member_type_str})lookup_function_by_name(func_info.func_name);\n"
        #     setter_server += "    return (void *)&result;\n"
        #     setter_server += "}\n\n"
            
        #     server_functions.append(getter_server)
        #     server_functions.append(setter_server)
            
        #     # 添加函数映射工具函数声明
        #     if "// Function name mapping utilities" not in wrapper_header_functions:
        #         wrapper_header_functions.append("// Function name mapping utilities\n")
        #         wrapper_header_functions.append("char* get_function_name(void *func_ptr);\n")
        #         wrapper_header_functions.append("void* lookup_function_by_name(const char *func_name);\n\n")
            
        #     # 提供函数映射实现代码
        #     if "// Function name mapping implementation" not in wrapper_functions:
        #         mapping_code = "// Function name mapping implementation\n"
        #         mapping_code += "#include <stdlib.h>\n"
        #         mapping_code += "#include <string.h>\n"
        #         mapping_code += "#include <dlfcn.h>\n\n"
                
        #         # 定义预先知道的函数名到函数指针的映射表
        #         mapping_code += "// 预定义函数映射表\n"
        #         mapping_code += "struct func_map_entry {\n"
        #         mapping_code += "    const char *name;\n"
        #         mapping_code += "    void *ptr;\n"
        #         mapping_code += "};\n\n"
                
        #         mapping_code += "// 常用函数映射列表，可以根据需要扩展\n"
        #         mapping_code += "static struct func_map_entry known_functions[] = {\n"
        #         mapping_code += "    {\"printf\", printf},\n"
        #         mapping_code += "    {\"malloc\", malloc},\n"
        #         mapping_code += "    {\"free\", free},\n"
        #         mapping_code += "    {\"memcpy\", memcpy},\n"
        #         mapping_code += "    {\"strlen\", strlen},\n"
        #         mapping_code += "    {\"strcmp\", strcmp},\n"
        #         mapping_code += "    {\"fopen\", fopen},\n"
        #         mapping_code += "    {\"fclose\", fclose},\n"
        #         mapping_code += "    {NULL, NULL}\n"
        #         mapping_code += "};\n\n"
                
        #         # 根据函数指针获取函数名
        #         mapping_code += "char* get_function_name(void *func_ptr) {\n"
        #         mapping_code += "    if (func_ptr == NULL) return NULL;\n\n"
                
        #         mapping_code += "    // 在已知函数表中查找\n"
        #         mapping_code += "    for (int i = 0; known_functions[i].name != NULL; i++) {\n"
        #         mapping_code += "        if (known_functions[i].ptr == func_ptr) {\n"
        #         mapping_code += "            return strdup(known_functions[i].name);\n"
        #         mapping_code += "        }\n"
        #         mapping_code += "    }\n\n"
                
        #         # 尝试使用dladdr获取符号名（如果可用）
        #         mapping_code += "    // 尝试使用动态链接器获取函数名\n"
        #         mapping_code += "    Dl_info info;\n"
        #         mapping_code += "    if (dladdr(func_ptr, &info) != 0 && info.dli_sname != NULL) {\n"
        #         mapping_code += "        return strdup(info.dli_sname);\n"
        #         mapping_code += "    }\n\n"
                
        #         mapping_code += "    // 无法获取函数名，返回地址的十六进制表示\n"
        #         mapping_code += "    char buffer[32];\n"
        #         mapping_code += "    snprintf(buffer, sizeof(buffer), \"0x%p\", func_ptr);\n"
        #         mapping_code += "    return strdup(buffer);\n"
        #         mapping_code += "}\n\n"
                
        #         # 根据函数名查找函数指针
        #         mapping_code += "void* lookup_function_by_name(const char *func_name) {\n"
        #         mapping_code += "    if (func_name == NULL) return NULL;\n\n"
                
        #         mapping_code += "    // 在已知函数表中查找\n"
        #         mapping_code += "    for (int i = 0; known_functions[i].name != NULL; i++) {\n"
        #         mapping_code += "        if (strcmp(known_functions[i].name, func_name) == 0) {\n"
        #         mapping_code += "            return known_functions[i].ptr;\n"
        #         mapping_code += "        }\n"
        #         mapping_code += "    }\n\n"
                
        #         # 尝试使用dlsym查找函数（如果可用）
        #         mapping_code += "    // 尝试在当前加载的所有共享库中查找函数\n"
        #         mapping_code += "    void *handle = RTLD_DEFAULT;\n"
        #         mapping_code += "    void *ptr = dlsym(handle, func_name);\n"
        #         mapping_code += "    if (ptr != NULL) {\n"
        #         mapping_code += "        return ptr;\n"
        #         mapping_code += "    }\n\n"
                
        #         # 处理函数地址的十六进制表示
        #         mapping_code += "    // 检查是否是地址的十六进制表示\n"
        #         mapping_code += "    if (strncmp(func_name, \"0x\", 2) == 0) {\n"
        #         mapping_code += "        void *addr;\n"
        #         mapping_code += "        sscanf(func_name, \"0x%p\", &addr);\n"
        #         mapping_code += "        return addr;\n"
        #         mapping_code += "    }\n\n"
                
        #         mapping_code += "    return NULL; // 函数未找到\n"
        #         mapping_code += "}\n\n"
                
        #         wrapper_functions.append(mapping_code)
                
        #     # 已经处理过函数指针，直接返回
        #     return


        
        
        # 检查是否为结构体类型（无论是否为指针）
        is_struct = False
        struct_name = None
        base_type = cleaned_member_type_str
        
        if is_pointer:
            # 对于指针类型，获取指向的类型
            base_type = cleaned_member_type_str.replace("*", "").strip()
        
        # 确定是否是结构体类型
        if base_type.startswith("struct ") or base_type.startswith("union "):
            is_struct = True
            struct_name = base_type.split(" ")[1]
            if struct_name in typedef_dict:
                struct_def = typedef_dict[struct_name]['ast']
            else:
                struct_def = struct_dict.get(struct_name, {}).get('ast', None)
        elif base_type in typedef_dict:
            if typedef_dict[base_type]['isstruct']:
                # print(f"Processing typedef struct: {base_type}")
                is_struct = True
                struct_name = base_type
                struct_def = typedef_dict[base_type]['ast']
            elif base_type in struct_dict and struct_dict[base_type]['isunion'] or base_type in struct_dict and struct_dict[base_type]['isenum']:
                pass
            # else:
            #     print(f"Warning: Member '{member_name}' in '{var_name}' has an unsupported type: {cleaned_member_type_str}")
            #     print(typedef_dict[base_type]['ast'])
        elif base_type in struct_dict and struct_dict[base_type]['isstruct']:
            is_struct = True
            struct_name = base_type
            struct_def = struct_dict[base_type]['ast']

        if is_struct:
            if struct_name+"_rpc" not in typedef_struct_rpc:
                    typedef_struct_rpc.append(struct_name+"_rpc")
                    struct_str = f"struct {struct_name}_rpc{{\n"
                    member_list = []
                    get_rpc_struct_str(struct_def, member_list)
                    
                    for member in member_list:
                        struct_str += "    " + member + "\n"                   
                    # 完成结构体定义
                    struct_str += "};\n"                   
                    # 添加到IDL结构体列表
                    idl_structs.append(struct_str)
                    idl_structs.append(f"typedef struct {struct_name}_rpc {struct_name}_rpc;\n")

            if is_pointer:
                if struct_name+"_rpc_ptr" not in typedef_struct_rpc:
                    typedef_str="typedef "+struct_name+"_rpc "+struct_name+"_rpc_ptr<>;"
                    idl_structs.append(typedef_str)
                    typedef_struct_rpc.append(struct_name+"_rpc_ptr")
                    idl_structs.append(f"typedef struct {struct_name}_rpc_ptr {struct_name}_rpc_ptr;\n")
        
        # === IDL 定义 ===
        # 添加成员返回值的结构体定义
        if cleaned_member_type_str == "char *" or cleaned_member_type_str == "const char *" or cleaned_member_type_str == "FILE *" or cleaned_member_type_str == "unsigned char *":
            # 检查是否已经定义了char_ptr结构体
            if "char_ptr" not in typedef_struct_rpc:
                dealwith_charptr()

            ret_struct = f"struct {var_name}_{member_name}_ret_t {{ char_ptr value; }};\n"
            typedef_ret = f"typedef struct {var_name}_{member_name}_ret_t {var_name}_{member_name}_ret_t;\n"
            
        elif cleaned_member_type_str == "char **" or cleaned_member_type_str == "const char **":
            if "char_ptr" not in typedef_struct_rpc:
                dealwith_charptr()
            
            if "TwoCharPtr" not in typedef_struct_rpc:
                dealwith_twocharptr()
            
            # 为char**类型成员创建特殊的返回结构体
            ret_struct = f"struct {var_name}_{member_name}_ret_t {{ TwoCharPtr value; }};\n"
            typedef_ret = f"typedef struct {var_name}_{member_name}_ret_t {var_name}_{member_name}_ret_t;\n"
        elif is_struct:
            ret_struct=""
            typedef_ret=""
            pass
            # ret_struct = f"struct {var_name}_{member_name}_ret_t {{ {struct_name}_rpc value; }};\n"
            # typedef_ret = f"typedef struct {var_name}_{member_name}_ret_t {var_name}_{member_name}_ret_t;\n"
        else:
            # 其他类型保持原样
            if cleaned_member_type_str.startswith("enum"):
                cleaned_member_type_str="int"
            ret_struct = f"struct {var_name}_{member_name}_ret_t {{ {cleaned_member_type_str} value; }};\n"
            typedef_ret = f"typedef struct {var_name}_{member_name}_ret_t {var_name}_{member_name}_ret_t;\n"

        if ret_struct not in idl_structs:
            idl_structs.append(ret_struct)
            idl_structs.append(typedef_ret)
        
        # 添加成员的 IDL 函数定义==================================================
        # 区分不同类型，特别是 char*, char**, void* 等特殊类型
        if cleaned_member_type_str == "char *" or cleaned_member_type_str == "const char *" or cleaned_member_type_str == "FILE *" or cleaned_member_type_str == "unsigned char *":
            # 字符串指针使用 char_ptr 结构体表示
            getter_idl = f"{var_name}_{member_name}_ret_t get_{var_name}_{member_name}() = {function_counter};\n"
            setter_idl = f"void set_{var_name}_{member_name}(char_ptr value) = {function_counter + 1};\n"
            function_counter += 2
            idl_functions.append(getter_idl)
            idl_functions.append(setter_idl)
        elif cleaned_member_type_str == "char **" or cleaned_member_type_str == "const char **":
            # 二级字符串指针使用 TwoCharPtr 结构体表示
            getter_idl = f"{var_name}_{member_name}_ret_t get_{var_name}_{member_name}() = {function_counter};\n"
            setter_idl = f"void set_{var_name}_{member_name}(TwoCharPtr value) = {function_counter + 1};\n"
            function_counter += 2
            idl_functions.append(getter_idl)
            idl_functions.append(setter_idl)
        elif cleaned_member_type_str == "void *" or cleaned_member_type_str == "void*":
            # void* 类型使用 binary_data 结构体表示
            getter_idl = f"{var_name}_{member_name}_ret_t get_{var_name}_{member_name}() = {function_counter};\n"
            setter_idl = f"void set_{var_name}_{member_name}(binary_data value) = {function_counter + 1};\n"
            function_counter += 2
            idl_functions.append(getter_idl)
            idl_functions.append(setter_idl)
        
        elif is_struct:
            if is_pointer:
                function_counter=process_struct_whole(var_name+"_"+member_name, struct_name, "",True,idl_functions,idl_structs,server_functions,wrapper_functions,wrapper_header_functions,function_counter)
            else:
                function_counter=process_struct_whole(var_name+"_"+member_name, struct_name, "",False,idl_functions,idl_structs,server_functions,wrapper_functions,wrapper_header_functions,function_counter)

            # 如果是结构体类型(非指针)，进一步递归处理其成员
            # 检查是否已处理过该类型，避免递归死循环
            type_key = f"{struct_name}"
            if type_key in processed_type:
                print(f"检测到循环引用: {struct_name}，跳过递归处理")
                return function_counter
                
            # 标记该类型为已处理
            processed_type.add(type_key)
            print("多层递归")
            struct_def = None
            if struct_name in struct_dict:
                print("struct_name:", struct_name)
                struct_def = struct_dict[struct_name]['ast'] 
            elif struct_name in typedef_dict and typedef_dict[struct_name]['isstruct']:
                print("struct_name:", struct_name)
                struct_def = typedef_dict[struct_name]['ast']
            print(struct_def)
            if struct_def and hasattr(struct_def, 'decls') and struct_def.decls:
                # 创建一个新的路径名来表示嵌套关系
                nested_var_name = f"{var_name}_{member_name}"
                
                # 为结构体中的每个成员生成getter和setter
                for i, sub_member in enumerate(struct_def.decls):
                    sub_member_name = sub_member.name
                    sub_member_type = generator.visit(sub_member.type)
                    
                    # 递归调用处理结构体成员
                    function_counter=process_struct_member(nested_var_name, sub_member_name, 
                                        sub_member_type, sub_member.type, generator,
                                        idl_functions, idl_structs,
                                        server_functions,
                                        wrapper_functions, wrapper_header_functions,
                                        function_counter ,processed_type) # 为每个成员分配唯一的函数计数器
            return function_counter
        
        
        
        # elif cleaned_member_type_str == "FILE *":
        #     # 文件指针使用文件信息结构表示
        #     if "file_info" not in [s.split()[1] if len(s.split()) > 1 else "" for s in idl_structs]:
        #         file_info_struct = "struct file_info { int fd; string path<>; int mode; };\n"
        #         file_info_typedef = "typedef struct file_info file_info;\n"
        #         idl_structs.append(file_info_struct)
        #         idl_structs.append(file_info_typedef)
            
        #     getter_idl = f"{var_name}_{member_name}_ret_t get_{var_name}_{member_name}() = {function_counter};\n"
        #     setter_idl = f"void set_{var_name}_{member_name}(file_info value) = {function_counter + 1};\n"
        #     function_counter += 2
        #     idl_functions.append(getter_idl)
        #     idl_functions.append(setter_idl)
            
        # elif is_struct:
        #     getter_idl = f"{var_name}_{member_name}_ret_t get_{var_name}_{member_name}() = {function_counter};\n"
        #     setter_idl = f"void set_{var_name}_{member_name}({struct_name}_rpc value) = {function_counter + 1};\n"
        #     function_counter +=2
        else:
            # 普通类型直接使用原始类型
            if cleaned_member_type_str.startswith("enum"):
                cleaned_member_type_str = "int"  # 枚举类型转换为 int
            getter_idl = f"{var_name}_{member_name}_ret_t get_{var_name}_{member_name}() = {function_counter};\n"
            setter_idl = f"void set_{var_name}_{member_name}({cleaned_member_type_str} value) = {function_counter + 1};\n"
            function_counter += 2
            idl_functions.append(getter_idl)
            idl_functions.append(setter_idl)
        
        # === 服务端实现 ========================================================================
        # 成员 Getter 函数
        getter_server = f"{var_name}_{member_name}_ret_t *get_{var_name}_{member_name}_1_svc(struct svc_req *rqstp)\n"
        getter_server += "{\n"
        getter_server += f"    static {var_name}_{member_name}_ret_t result;\n"
        
        if is_pointer and cleaned_member_type_str.startswith("char *") or cleaned_member_type_str.startswith("const char *"):
            # 字符串指针特殊处理
            getter_server += f"    if ({var_name}.{member_name} != NULL) {{\n"
            getter_server += f"        result.value = strdup({var_name}.{member_name});\n"
            getter_server += "    } else {\n"
            getter_server += "        result.value = NULL;\n"
            getter_server += "    }\n"
        elif is_pointer and (cleaned_member_type_str == "char **" or cleaned_member_type_str == "const char **"):
            # 处理字符串数组
            getter_server += f"    if ({var_name}.{member_name} == NULL) {{\n"
            getter_server += "        result.value.twocharptr.twocharptr_len = 0;\n"
            getter_server += "        result.value.twocharptr.twocharptr_val = NULL;\n"
            getter_server += "    } else {\n"
            getter_server += "        // 计算字符串数组的长度（假设以NULL结尾）\n"
            getter_server += "        int arr_len = 0;\n"
            getter_server += f"        while ({var_name}.{member_name}[arr_len] != NULL) arr_len++;\n"
            getter_server += "        result.value.twocharptr.twocharptr_len = arr_len;\n"
            getter_server += "        result.value.twocharptr.twocharptr_val = malloc(arr_len * sizeof(char_ptr));\n"
            getter_server += "        if (result.value.twocharptr.twocharptr_val == NULL) {\n"
            getter_server += "            perror(\"malloc failed\");\n"
            getter_server += "            result.value.twocharptr.twocharptr_len = 0;\n"
            getter_server += "        } else {\n"
            getter_server += "            for (int i = 0; i < arr_len; i++) {\n"
            getter_server += f"                if ({var_name}.{member_name}[i] == NULL) {{\n"
            getter_server += "                    result.value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;\n"
            getter_server += "                    result.value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;\n"
            getter_server += "                } else {\n"
            getter_server += f"                    result.value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen({var_name}.{member_name}[i]) + 1;\n"
            getter_server += f"                    result.value.twocharptr.twocharptr_val[i].charptr.charptr_val = {var_name}.{member_name}[i];\n"
            getter_server += "                }\n"
            getter_server += "            }\n"
            getter_server += "        }\n"
            getter_server += "    }\n"
        
        elif is_pointer and cleaned_member_type_str == "FILE *":
            # FILE指针特殊处理
            getter_server += f"    if ({var_name}.{member_name} != NULL) {{\n"
            getter_server += "        result.value.fd = fileno({var_name}.{member_name});\n"
            getter_server += "        result.value.path = NULL; // 无法可靠获取文件路径\n"
            getter_server += "        result.value.mode = 0; // 模式信息需要单独维护\n"
            getter_server += "    } else {\n"
            getter_server += "        result.value.fd = -1;\n"
            getter_server += "        result.value.path = NULL;\n"
            getter_server += "        result.value.mode = 0;\n"
            getter_server += "    }\n"
        elif is_pointer and (cleaned_member_type_str == "void *" or cleaned_member_type_str == "void*"):
            # 处理void*指针
            getter_server += f"    // void* 类型需要额外的大小信息\n"
            getter_server += f"    extern size_t {var_name}_{member_name}_size; // 假设有记录大小的全局变量\n"
            getter_server += f"    if ({var_name}.{member_name} == NULL) {{\n"
            getter_server += "        result.value.bytes.bytes_len = 0;\n"
            getter_server += "        result.value.bytes.bytes_val = NULL;\n"
            getter_server += "        result.value.size = 0;\n"
            getter_server += "    } else {\n"
            getter_server += f"        size_t data_size = {var_name}_{member_name}_size;\n"
            getter_server += "        result.value.bytes.bytes_len = data_size;\n"
            getter_server += f"        result.value.bytes.bytes_val = {var_name}.{member_name};\n"
            getter_server += "        result.value.size = data_size;\n"
            getter_server += "    }\n"
        # elif is_pointer and is_struct:
        #     # 结构体指针特殊处理
        #     getter_server += f"    if ({var_name}.{member_name} == NULL) {{\n"
        #     getter_server += "        memset(&result.value, 0, sizeof(result.value));\n"
        #     getter_server += "    } else {\n"
        #     getter_server += f"        result.value = *({var_name}.{member_name}); // 复制结构体内容\n"
        #     getter_server += "    }\n"
        # elif is_struct and not is_pointer:
        #     # 非指针结构体成员 - 直接复制
        #     getter_server += f"    memcpy(&result.value, &{var_name}.{member_name}, sizeof({cleaned_member_type_str}));\n"
        elif is_pointer:
            # 其他指针类型处理
            getter_server += f"    result.value = {var_name}.{member_name}; // 警告：仅传递指针值\n"
        else:
            # 非指针类型直接赋值
            getter_server += f"    result.value = {var_name}.{member_name};\n"
        
        getter_server += "    return &result;\n"
        getter_server += "}\n\n"
        
        # 成员 Setter 函数
        # Setter 函数声明
        if cleaned_member_type_str == "char *" or cleaned_member_type_str == "const char *":
            # 字符串指针使用char_ptr结构
            setter_server = f"void *set_{var_name}_{member_name}_1_svc(char_ptr value, struct svc_req *rqstp)\n"
        elif cleaned_member_type_str == "char **" or cleaned_member_type_str == "const char **":
            # 二级字符串指针使用TwoCharPtr结构
            setter_server = f"void *set_{var_name}_{member_name}_1_svc(TwoCharPtr value, struct svc_req *rqstp)\n"
        elif cleaned_member_type_str == "void *" or cleaned_member_type_str == "void*":
            # void*指针使用binary_data结构
            setter_server = f"void *set_{var_name}_{member_name}_1_svc(binary_data value, struct svc_req *rqstp)\n"
        elif isinstance(member_type, c_ast.PtrDecl) and isinstance(member_type.type, c_ast.FuncDecl):
            # 函数指针使用函数名信息结构
            setter_server = f"void *set_{var_name}_{member_name}_1_svc(function_name_info value, struct svc_req *rqstp)\n"
        elif cleaned_member_type_str == "FILE *":
            # 文件指针使用file_info结构
            setter_server = f"void *set_{var_name}_{member_name}_1_svc(file_info value, struct svc_req *rqstp)\n"
        else:
            # 其他类型直接使用原始类型
            setter_server = f"void *set_{var_name}_{member_name}_1_svc({cleaned_member_type_str} value, struct svc_req *rqstp)\n"
        
        # Setter 函数函数体
        setter_server += "{\n"
        setter_server += "    static int result = 1;\n"
        
        if is_pointer and cleaned_member_type_str.startswith("char *") or cleaned_member_type_str.startswith("const char *"):
            # 字符串指针特殊处理
            setter_server += f"    if ({var_name}.{member_name} != NULL) {{\n"
            setter_server += f"        free({var_name}.{member_name});\n"
            setter_server += "    }\n"
            setter_server += "    if (value != NULL) {\n"
            setter_server += f"        {var_name}.{member_name} = strdup(value);\n"
            setter_server += "    } else {\n"
            setter_server += f"        {var_name}.{member_name} = NULL;\n"
            setter_server += "    }\n"
        
        elif is_pointer and cleaned_member_type_str == "char **" or cleaned_member_type_str == "const char **":
            # 处理字符串数组
            setter_server += f"    // 释放原有字符串数组\n"
            setter_server += f"    if ({var_name}.{member_name} != NULL) {{\n"
            setter_server += "        int i = 0;\n"
            setter_server += f"        while ({var_name}.{member_name}[i] != NULL) {{\n"
            setter_server += f"            free({var_name}.{member_name}[i]);\n"
            setter_server += "            i++;\n"
            setter_server += "        }\n"
            setter_server += f"        free({var_name}.{member_name});\n"
            setter_server += f"        {var_name}.{member_name} = NULL;\n"
            setter_server += "    }\n"
            setter_server += "    // 分配新的字符串数组\n"
            setter_server += "    if (value.twocharptr.twocharptr_val != NULL && value.twocharptr.twocharptr_len > 0) {\n"
            setter_server += f"        {var_name}.{member_name} = (char **)malloc((value.twocharptr.twocharptr_len + 1) * sizeof(char *));\n"
            setter_server += f"        if ({var_name}.{member_name} == NULL) {{\n"
            setter_server += "            perror(\"malloc failed\");\n"
            setter_server += "            result = 0;\n"
            setter_server += "        } else {\n"
            setter_server += "            int i;\n"
            setter_server += "            for (i = 0; i < value.twocharptr.twocharptr_len; i++) {\n"
            setter_server += "                if (value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {\n"
            setter_server += f"                    {var_name}.{member_name}[i] = NULL;\n"
            setter_server += "                } else {\n"
            setter_server += f"                    {var_name}.{member_name}[i] = strdup(value.twocharptr.twocharptr_val[i].charptr.charptr_val);\n"
            setter_server += "                }\n"
            setter_server += "            }\n"
            setter_server += f"            {var_name}.{member_name}[i] = NULL; // 确保以NULL结尾\n"
            setter_server += "        }\n"
            setter_server += "    }\n"
        elif is_pointer and cleaned_member_type_str == "FILE *":
            # FILE指针特殊处理
            setter_server += f"    if ({var_name}.{member_name} != NULL) {{\n"
            setter_server += f"        fclose({var_name}.{member_name});\n"
            setter_server += f"        {var_name}.{member_name} = NULL;\n"
            setter_server += "    }\n"
            setter_server += "    if (value.path != NULL) {\n"
            setter_server += "        const char *mode_str;\n"
            setter_server += "        switch(value.mode) {\n"
            setter_server += "            case 1: mode_str = \"r\"; break;\n"
            setter_server += "            case 2: mode_str = \"w\"; break;\n"
            setter_server += "            case 3: mode_str = \"a\"; break;\n"
            setter_server += "            case 4: mode_str = \"r+\"; break;\n"
            setter_server += "            case 5: mode_str = \"w+\"; break;\n"
            setter_server += "            case 6: mode_str = \"a+\"; break;\n"
            setter_server += "            default: mode_str = \"r\"; break;\n"
            setter_server += "        }\n"
            setter_server += f"        {var_name}.{member_name} = fopen(value.path, mode_str);\n"
            setter_server += "    }\n"
        elif is_pointer and (cleaned_member_type_str == "void *" or cleaned_member_type_str == "void*"):
            # 处理void*指针
            setter_server += f"    // 释放原有内存\n"
            setter_server += f"    extern size_t {var_name}_{member_name}_size; // 假设有记录大小的全局变量\n"
            setter_server += f"    if ({var_name}.{member_name} != NULL) {{\n"
            setter_server += f"        free({var_name}.{member_name});\n"
            setter_server += f"        {var_name}.{member_name} = NULL;\n"
            setter_server += "    }\n"
            setter_server += "    // 分配新内存并复制数据\n"
            setter_server += "    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {\n"
            setter_server += f"        {var_name}.{member_name} = malloc(value.bytes.bytes_len);\n"
            setter_server += f"        if ({var_name}.{member_name} == NULL) {{\n"
            setter_server += "            perror(\"malloc failed\");\n"
            setter_server += "            result = 0;\n"
            setter_server += "        } else {\n"
            setter_server += f"            memcpy({var_name}.{member_name}, value.bytes.bytes_val, value.bytes.bytes_len);\n"
            setter_server += f"            {var_name}_{member_name}_size = value.size;\n"
            setter_server += "        }\n"
            setter_server += "    }\n"
        
        # elif is_pointer and is_struct:
        #     # 结构体指针特殊处理
        #     setter_server += "    // 处理结构体指针\n"
        #     setter_server += f"    if ({var_name}.{member_name} == NULL) {{\n"
        #     setter_server += f"        {var_name}.{member_name} = ({cleaned_member_type_str})malloc(sizeof(*value));\n"
        #     setter_server += f"        if ({var_name}.{member_name} == NULL) {{\n"
        #     setter_server += "            perror(\"malloc failed\");\n"
        #     setter_server += "            result = 0;\n"
        #     setter_server += "            return (void *)&result;\n"
        #     setter_server += "        }\n"
        #     setter_server += "    }\n"
        #     setter_server += f"    memcpy({var_name}.{member_name}, &value, sizeof(*{var_name}.{member_name}));\n"
        # elif is_struct and not is_pointer:
        #     # 非指针结构体成员 - 直接复制
        #     setter_server += f"    memcpy(&{var_name}.{member_name}, &value, sizeof({cleaned_member_type_str}));\n"
        else:
            # 其他类型直接赋值
            setter_server += f"    {var_name}.{member_name} = value;\n"
        
        setter_server += "    return (void *)&result;\n"
        setter_server += "}\n\n"
        
        server_functions.append(getter_server)
        server_functions.append(setter_server)
        
        # === 客户端 Wrapper ===========================================================================
        # 成员 Getter Wrapper
        #处理函数指针的情况,member_type.type是FuncDecl类型
        if isinstance(member_type, c_ast.PtrDecl) and isinstance(member_type.type, c_ast.FuncDecl):
            pass
        getter_wrapper = f"{cleaned_member_type_str} get_{var_name}_{member_name}_wrapper()\n"
        getter_wrapper += "{\n"
        getter_wrapper += f"    {var_name}_{member_name}_ret_t *result;\n"
        
        if is_pointer:
            getter_wrapper += f"    {cleaned_member_type_str} ret_val = NULL;\n"
        # elif is_struct:
        #     getter_wrapper += f"    {cleaned_member_type_str} ret_val;\n"
        #     getter_wrapper += "    memset(&ret_val, 0, sizeof(ret_val)); // 初始化结构体\n"
        else:
            getter_wrapper += f"    {cleaned_member_type_str} ret_val = 0; // 默认值\n"
        
        getter_wrapper += f"    result = get_{var_name}_{member_name}_1(clnt);\n"
        getter_wrapper += "    if (result == NULL) {\n"
        getter_wrapper += "        clnt_perror(clnt, \"get member call failed\");\n"
        getter_wrapper += "        return ret_val;\n"
        getter_wrapper += "    }\n"

        # 特殊类型的处理
        if is_pointer and cleaned_member_type_str == "FILE *":
            getter_wrapper += "    if (result->value.path != NULL) {\n"
            getter_wrapper += "        // 根据返回的文件信息打开本地文件\n"
            getter_wrapper += "        const char *mode_str;\n"
            getter_wrapper += "        switch(result->value.mode) {\n"
            getter_wrapper += "            case 1: mode_str = \"r\"; break;\n"
            getter_wrapper += "            case 2: mode_str = \"w\"; break;\n"
            getter_wrapper += "            case 3: mode_str = \"a\"; break;\n"
            getter_wrapper += "            case 4: mode_str = \"r+\"; break;\n"
            getter_wrapper += "            case 5: mode_str = \"w+\"; break;\n"
            getter_wrapper += "            case 6: mode_str = \"a+\"; break;\n"
            getter_wrapper += "            default: mode_str = \"r\"; break;\n"
            getter_wrapper += "        }\n"
            getter_wrapper += "        ret_val = fopen(result->value.path, mode_str);\n"
            getter_wrapper += "    }\n"
        elif is_pointer and cleaned_member_type_str == "char *" or cleaned_member_type_str == "const char *":
            # 对于字符串，需要复制返回值
            getter_wrapper += "    if (result->value.charptr.charptr_val != NULL) {\n"
            getter_wrapper += "        ret_val = malloc(result->value.charptr.charptr_len);\n"
            getter_wrapper += "        if (ret_val == NULL) {\n"
            getter_wrapper += "            perror(\"malloc failed\");\n"
            getter_wrapper += "            return NULL;\n"
            getter_wrapper += "        }\n"
            getter_wrapper += "        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);\n"
            getter_wrapper += "    }\n"
        elif is_pointer and cleaned_member_type_str == "char **" or cleaned_member_type_str == "const char **":
            # 处理二维字符数组
            getter_wrapper += "    if (result->value.twocharptr.twocharptr_val != NULL) {\n"
            getter_wrapper += "        int length = result->value.twocharptr.twocharptr_len;\n"
            getter_wrapper += "        ret_val = (char **)malloc((length + 1) * sizeof(char *));\n"
            getter_wrapper += "        if (ret_val == NULL) {\n"
            getter_wrapper += "            perror(\"malloc failed\");\n"
            getter_wrapper += "            return NULL;\n"
            getter_wrapper += "        }\n"
            getter_wrapper += "        for (int i = 0; i < length; i++) {\n"
            getter_wrapper += "            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {\n"
            getter_wrapper += "                ret_val[i] = NULL;\n"
            getter_wrapper += "            } else {\n"
            getter_wrapper += "                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);\n"
            getter_wrapper += "                if (ret_val[i] == NULL) {\n"
            getter_wrapper += "                    perror(\"malloc failed\");\n"
            getter_wrapper += "                    // 释放已分配的内存\n"
            getter_wrapper += "                    for (int j = 0; j < i; j++) {\n"
            getter_wrapper += "                        if (ret_val[j] != NULL) free(ret_val[j]);\n"
            getter_wrapper += "                    }\n"
            getter_wrapper += "                    free(ret_val);\n"
            getter_wrapper += "                    return NULL;\n"
            getter_wrapper += "                }\n"
            getter_wrapper += "                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, \n"
            getter_wrapper += "                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);\n"
            getter_wrapper += "            }\n"
            getter_wrapper += "        }\n"
            getter_wrapper += "        ret_val[length] = NULL; // 确保以NULL结尾\n"
            getter_wrapper += "    }\n"
        # elif is_pointer and is_struct:
        #     # 处理结构体指针
        #     getter_wrapper += "    if (result->value != NULL) {\n"
        #     getter_wrapper += f"        ret_val = ({cleaned_member_type_str})malloc(sizeof(*result->value));\n"
        #     getter_wrapper += "        if (ret_val == NULL) {\n"
        #     getter_wrapper += "            perror(\"malloc failed\");\n"
        #     getter_wrapper += "            return NULL;\n"
        #     getter_wrapper += "        }\n"
        #     getter_wrapper += "        memcpy(ret_val, &result->value, sizeof(*ret_val));\n"
        #     getter_wrapper += "    }\n"
        elif is_struct and not is_pointer:
            # 非指针结构体成员 - 直接复制
            getter_wrapper += "    memcpy(&ret_val, &result->value, sizeof(ret_val));\n"
        else:
            # 其他类型直接返回
            getter_wrapper += "    ret_val = result->value;\n"
        
        getter_wrapper += "    return ret_val;\n"
        getter_wrapper += "}\n\n"
        
        # 成员 Setter Wrapper
        if is_pointer and cleaned_member_type_str == "char *" or cleaned_member_type_str == "const char *":
            # 字符串指针类型的setter
            setter_wrapper = f"int set_{var_name}_{member_name}_wrapper(const char *value)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    void *result;\n"
            setter_wrapper += f"    char_ptr rpc_value;\n"
            setter_wrapper += "    if (value == NULL) {\n"
            setter_wrapper += "        rpc_value.charptr.charptr_len = 0;\n"
            setter_wrapper += "        rpc_value.charptr.charptr_val = NULL;\n"
            setter_wrapper += "    } else {\n"
            setter_wrapper += "        rpc_value.charptr.charptr_len = strlen(value) + 1;\n"
            setter_wrapper += "        rpc_value.charptr.charptr_val = (char*)value;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    result = set_{var_name}_{member_name}_1(rpc_value, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"set member call failed\");\n"
            setter_wrapper += "        return 0;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    return 1;\n"
            setter_wrapper += "}\n\n"
            
            # 更新头文件声明以匹配实现
            wrapper_header_functions.append(f"int set_{var_name}_{member_name}_wrapper(const char *value);\n")
        elif is_pointer and cleaned_member_type_str == "char **" or cleaned_member_type_str == "const char **":
            # 处理字符串数组类型的setter
            setter_wrapper = f"int set_{var_name}_{member_name}_wrapper(char **value, int length)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    void *result;\n"
            setter_wrapper += f"    TwoCharPtr rpc_value;\n"
            setter_wrapper += "    if (value == NULL || length <= 0) {\n"
            setter_wrapper += "        rpc_value.twocharptr.twocharptr_len = 0;\n"
            setter_wrapper += "        rpc_value.twocharptr.twocharptr_val = NULL;\n"
            setter_wrapper += "    } else {\n"
            setter_wrapper += "        rpc_value.twocharptr.twocharptr_len = length;\n"
            setter_wrapper += "        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));\n"
            setter_wrapper += "        if (rpc_value.twocharptr.twocharptr_val == NULL) {\n"
            setter_wrapper += "            perror(\"malloc failed\");\n"
            setter_wrapper += "            return 0;\n"
            setter_wrapper += "        }\n"
            setter_wrapper += "        for (int i = 0; i < length; i++) {\n"
            setter_wrapper += "            if (value[i] == NULL) {\n"
            setter_wrapper += "                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;\n"
            setter_wrapper += "                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;\n"
            setter_wrapper += "            } else {\n"
            setter_wrapper += "                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;\n"
            setter_wrapper += "                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];\n"
            setter_wrapper += "            }\n"
            setter_wrapper += "        }\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    result = set_{var_name}_{member_name}_1(rpc_value, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"set member call failed\");\n"
            setter_wrapper += "        if (rpc_value.twocharptr.twocharptr_val != NULL) {\n"
            setter_wrapper += "            free(rpc_value.twocharptr.twocharptr_val);\n"
            setter_wrapper += "        }\n"
            setter_wrapper += "        return 0;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    if (rpc_value.twocharptr.twocharptr_val != NULL) {\n"
            setter_wrapper += "        free(rpc_value.twocharptr.twocharptr_val);\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    return 1;\n"
            setter_wrapper += "}\n\n"
            
            # 更新头文件声明以匹配实现
            wrapper_header_functions.append(f"int set_{var_name}_{member_name}_wrapper(char **value, int length);\n")
        elif is_pointer and cleaned_member_type_str == "FILE *":
            # FILE指针处理
            setter_wrapper = f"int set_{var_name}_{member_name}_wrapper(FILE *file, const char *path, int mode)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    void *result;\n"
            setter_wrapper += f"    file_info rpc_value;\n"
            setter_wrapper += "    if (file == NULL || path == NULL) {\n"
            setter_wrapper += "        rpc_value.fd = -1;\n"
            setter_wrapper += "        rpc_value.path = NULL;\n"
            setter_wrapper += "        rpc_value.mode = 0;\n"
            setter_wrapper += "    } else {\n"
            setter_wrapper += "        rpc_value.fd = fileno(file);\n"
            setter_wrapper += "        rpc_value.path = (char*)path;\n"
            setter_wrapper += "        rpc_value.mode = mode;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    result = set_{var_name}_{member_name}_1(rpc_value, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"set member call failed\");\n"
            setter_wrapper += "        return 0;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    return 1;\n"
            setter_wrapper += "}\n\n"
            
            wrapper_header_functions.append(f"int set_{var_name}_{member_name}_wrapper(FILE *file, const char *path, int mode);\n")
        # elif is_pointer and is_struct:
        #     # 结构体指针处理
        #     setter_wrapper = f"int set_{var_name}_{member_name}_wrapper({struct_name}_rpc value)\n"
        #     setter_wrapper += "{\n"
        #     setter_wrapper += f"    void *result;\n"
        #     setter_wrapper += f"    result = set_{var_name}_{member_name}_1(value, clnt); // 传递结构体而不是指针\n"
        #     setter_wrapper += "    if (result == NULL) {\n"
        #     setter_wrapper += "        clnt_perror(clnt, \"set member call failed\");\n"
        #     setter_wrapper += "        return 0;\n"
        #     setter_wrapper += "    }\n"
        #     setter_wrapper += "    return 1;\n"
        #     setter_wrapper += "}\n\n"

            wrapper_header_functions.append(f"int set_{var_name}_{member_name}_wrapper({struct_name}_rpc value);\n")
        else:
            # 普通类型及非指针结构体 - 直接传值
            setter_wrapper = f"int set_{var_name}_{member_name}_wrapper({cleaned_member_type_str} value)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    void *result;\n"
            setter_wrapper += f"    result = set_{var_name}_{member_name}_1(value, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"set member call failed\");\n"
            setter_wrapper += "        return 0;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    return 1;\n"
            setter_wrapper += "}\n\n"
            
            wrapper_header_functions.append(f"int set_{var_name}_{member_name}_wrapper({cleaned_member_type_str} value);\n")
        
        wrapper_functions.append(getter_wrapper)
        wrapper_functions.append(setter_wrapper)
        
        # Header
        wrapper_header_functions.append(f"{cleaned_member_type_str} get_{var_name}_{member_name}_wrapper();\n")
    
                    
        return function_counter


    def process_ptr_type_var(var_name, type_str, var_type, generator, 
                            idl_functions, idl_structs, 
                            server_functions, 
                            wrapper_functions, wrapper_header_functions, 
                            function_counter):
        """处理指针类型的全局变量"""
        # 从 type_str 中提取指针的基础类型
        # 例如，对于 "char *"，基础类型是 "char"
        base_ptr_type = type_str.replace("*", "").strip()
        
        # === IDL 定义 ===
        # 处理字符串特殊情况
        if base_ptr_type == "char" or base_ptr_type == "const char":
            if "char_ptr" not in typedef_struct_rpc:
                dealwith_charptr()
            
            
            # if "char_ptr" not in [s.split()[1] if len(s.split()) > 1 else "" for s in idl_structs]:
            #     char_ptr_struct = "struct char_ptr {opaque charptr<>;};\n"
            #     char_ptr_typedef = "typedef struct char_ptr char_ptr;\n"
            #     if char_ptr_struct not in idl_structs:
            #         idl_structs.append(char_ptr_struct)
            #     if char_ptr_typedef not in idl_structs:
            #         idl_structs.append(char_ptr_typedef)
                
            
            # Getter
            getter_idl = f"char_ptr get_{var_name}() = {function_counter};\n"
            # Setter
            setter_idl = f"void set_{var_name}(char_ptr value) = {function_counter + 1};\n"
            function_counter += 2
            
            idl_functions.append(getter_idl)
            idl_functions.append(setter_idl)
            
            # === 服务端实现 ===
            # Getter
            getter_server = f"char_ptr *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += "    static char_ptr result;\n"
            getter_server += f"    if ({var_name} == NULL) {{\n"
            getter_server += "        result.charptr.charptr_len = 0;\n"
            getter_server += "        result.charptr.charptr_val = NULL;\n"
            getter_server += "    } else {\n"
            getter_server += f"        result.charptr.charptr_len = strlen({var_name}) + 1;\n"
            getter_server += f"        result.charptr.charptr_val = {var_name};\n"
            getter_server += "    }\n"
            getter_server += "    return &result;\n"
            getter_server += "}\n\n"
            
            # Setter
            setter_server = f"void *set_{var_name}_1_svc(char_ptr value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static int result;\n"
            setter_server += f"    if ({var_name} != NULL) {{\n"
            setter_server += f"        free({var_name});\n"
            setter_server += "    }\n"
            setter_server += "    if (value.charptr.charptr_val == NULL) {\n"
            setter_server += f"        {var_name} = NULL;\n"
            setter_server += "    } else {\n"
            setter_server += f"        {var_name}=malloc(value.charptr.charptr_len);\n"
            setter_server += f"        memcpy({var_name}, value.charptr.charptr_val, value.charptr.charptr_len);\n"
            setter_server += "    }\n"
            setter_server += "    return (void *)&result;\n"
            setter_server += "}\n\n"
            
            server_functions.append(getter_server)
            server_functions.append(setter_server)
            
            # === 客户端 Wrapper ===
            # Getter
            getter_wrapper = f"char* get_{var_name}_wrapper()\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    char_ptr *result;\n"
            getter_wrapper += f"    char *ret_val = NULL;\n"
            getter_wrapper += f"    result = get_{var_name}_1(clnt);\n"
            getter_wrapper += "    if (result == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            getter_wrapper += "        return NULL;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    if (result->charptr.charptr_val != NULL) {\n"
            getter_wrapper += "        ret_val=malloc(result->charptr.charptr_len);\n"
            getter_wrapper += "        memcpy(ret_val, result->charptr.charptr_val, result->charptr.charptr_len);\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    return ret_val;\n"
            getter_wrapper += "}\n\n"
            
            # Setter
            setter_wrapper = f"void set_{var_name}_wrapper(const char *value)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    void *result;\n"
            setter_wrapper += f"    char_ptr rpc_value;\n"
            setter_wrapper += "    if (value == NULL) {\n"
            setter_wrapper += "        rpc_value.charptr.charptr_len = 0;\n"
            setter_wrapper += "        rpc_value.charptr.charptr_val = NULL;\n"
            setter_wrapper += "    } else {\n"
            setter_wrapper += "        rpc_value.charptr.charptr_len = strlen(value) + 1;\n"
            setter_wrapper += "        rpc_value.charptr.charptr_val = (char*)value;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    result = set_{var_name}_1(rpc_value, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "}\n\n"
            
            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)
            
            # Header
            wrapper_header_functions.append(f"char* get_{var_name}_wrapper();\n")
            wrapper_header_functions.append(f"void set_{var_name}_wrapper(const char *value);\n")
        
        elif base_ptr_type in ["int", "float", "double", "long", "short", "unsigned", "signed"]:
            # 处理基本类型指针
            # 创建基本类型数组传输结构
            type_ptr_name = f"{base_ptr_type}_ptr"
            if type_ptr_name not in [s.split()[1] if len(s.split()) > 1 else "" for s in idl_structs]:
                type_ptr_struct = f"struct {type_ptr_name} {{ {base_ptr_type} {base_ptr_type}_val<>; }};\n"
                type_ptr_typedef = f"typedef struct {type_ptr_name} {type_ptr_name};\n"
                idl_structs.append(type_ptr_struct)
                idl_structs.append(type_ptr_typedef)
            
            # IDL 定义
            getter_idl = f"{type_ptr_name} get_{var_name}() = {function_counter};\n"
            setter_idl = f"void set_{var_name}({type_ptr_name} value) = {function_counter + 1};\n"
            function_counter += 2
            
            idl_functions.append(getter_idl)
            idl_functions.append(setter_idl)
            
            # === 服务端实现 ===
            # Getter
            getter_server = f"{type_ptr_name} *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += f"    static {type_ptr_name} result;\n"
            getter_server += f"    if ({var_name} == NULL) {{\n"
            getter_server += f"        result.{base_ptr_type}_val.{base_ptr_type}_val_len = 0;\n"
            getter_server += f"        result.{base_ptr_type}_val.{base_ptr_type}_val_val = NULL;\n"
            getter_server += "    } else {\n"
            # 假设指针指向单个值或一组固定大小的元素（这里假设为1个元素）
            getter_server += f"        result.{base_ptr_type}_val.{base_ptr_type}_val_len = 1;\n"
            getter_server += f"        result.{base_ptr_type}_val.{base_ptr_type}_val_val = {var_name};\n"
            getter_server += "    }\n"
            getter_server += "    return &result;\n"
            getter_server += "}\n\n"
            
            # Setter
            setter_server = f"void *set_{var_name}_1_svc({type_ptr_name} value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static int result;\n"
            setter_server += f"    if ({var_name} != NULL) {{\n"
            setter_server += f"        free({var_name});\n"
            setter_server += f"        {var_name} = NULL;\n"
            setter_server += "    }\n"
            setter_server += f"    if (value.{base_ptr_type}_val.{base_ptr_type}_val_val != NULL && "
            setter_server += f"value.{base_ptr_type}_val.{base_ptr_type}_val_len > 0) {{\n"
            setter_server += f"        {var_name} = ({base_ptr_type} *)malloc(value.{base_ptr_type}_val.{base_ptr_type}_val_len * sizeof({base_ptr_type}));\n"
            setter_server += f"        if ({var_name} == NULL) {{\n"
            setter_server += "            perror(\"malloc failed\");\n"
            setter_server += "        } else {\n"
            setter_server += f"            memcpy({var_name}, value.{base_ptr_type}_val.{base_ptr_type}_val_val, "
            setter_server += f"value.{base_ptr_type}_val.{base_ptr_type}_val_len * sizeof({base_ptr_type}));\n"
            setter_server += "        }\n"
            setter_server += "    }\n"
            setter_server += "    return (void *)&result;\n"
            setter_server += "}\n\n"
            
            server_functions.append(getter_server)
            server_functions.append(setter_server)
            
            # === 客户端 Wrapper ===
            # Getter
            getter_wrapper = f"{base_ptr_type}* get_{var_name}_wrapper()\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    {type_ptr_name} *result;\n"
            getter_wrapper += f"    {base_ptr_type} *ret_val = NULL;\n"
            getter_wrapper += f"    result = get_{var_name}_1(clnt);\n"
            getter_wrapper += "    if (result == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            getter_wrapper += "        return NULL;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += f"    if (result->{base_ptr_type}_val.{base_ptr_type}_val_val != NULL && "
            getter_wrapper += f"result->{base_ptr_type}_val.{base_ptr_type}_val_len > 0) {{\n"
            getter_wrapper += f"        ret_val = ({base_ptr_type} *)malloc(result->{base_ptr_type}_val.{base_ptr_type}_val_len * sizeof({base_ptr_type}));\n"
            getter_wrapper += "        if (ret_val == NULL) {\n"
            getter_wrapper += "            perror(\"malloc failed\");\n"
            getter_wrapper += "            return NULL;\n"
            getter_wrapper += "        }\n"
            getter_wrapper += f"        memcpy(ret_val, result->{base_ptr_type}_val.{base_ptr_type}_val_val, "
            getter_wrapper += f"result->{base_ptr_type}_val.{base_ptr_type}_val_len * sizeof({base_ptr_type}));\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    return ret_val;\n"
            getter_wrapper += "}\n\n"
            
            # Setter
            setter_wrapper = f"void set_{var_name}_wrapper(const {base_ptr_type} *value, size_t length)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    void *result;\n"
            setter_wrapper += f"    {type_ptr_name} rpc_value;\n"
            setter_wrapper += "    if (value == NULL || length == 0) {\n"
            setter_wrapper += f"        rpc_value.{base_ptr_type}_val.{base_ptr_type}_val_len = 0;\n"
            setter_wrapper += f"        rpc_value.{base_ptr_type}_val.{base_ptr_type}_val_val = NULL;\n"
            setter_wrapper += "    } else {\n"
            setter_wrapper += f"        rpc_value.{base_ptr_type}_val.{base_ptr_type}_val_len = length;\n"
            setter_wrapper += f"        rpc_value.{base_ptr_type}_val.{base_ptr_type}_val_val = ({base_ptr_type} *)value;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    result = set_{var_name}_1(rpc_value, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "}\n\n"
            
            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)
            
            # Header
            wrapper_header_functions.append(f"{base_ptr_type}* get_{var_name}_wrapper();\n")
            wrapper_header_functions.append(f"void set_{var_name}_wrapper(const {base_ptr_type} *value, size_t length);\n")
            
        elif base_ptr_type == "FILE" or base_ptr_type == "const FILE":
            # 处理文件指针 - 文件指针不能直接通过RPC传递，需要用文件描述符或路径表示
            # 创建文件描述符结构
            # if "file_info" not in [s.split()[1] if len(s.split()) > 1 else "" for s in idl_structs]:
            #     file_info_struct = "struct file_info {\n    int fd;\n    char path<>;\n    int mode;\n};\n"
            #     file_info_typedef = "typedef struct file_info file_info;\n"
            #     idl_structs.append(file_info_struct)
            #     idl_structs.append(file_info_typedef)
            
            # IDL 定义
            getter_idl = f"char_ptr get_{var_name}() = {function_counter};\n"
            setter_idl = f"void set_{var_name}(char_ptr value) = {function_counter + 1};\n"
            function_counter += 2
            
            idl_functions.append(getter_idl)
            idl_functions.append(setter_idl)
            
            # === 服务端实现 ===
            # Getter - 返回文件的路径和描述符
            """
            char *filename=get_filename_from_fp(val_name);

            
            """
            getter_server = f"char_ptr *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += "    static char_ptr result;\n"
            getter_server += f"    char *filename=get_filename_from_fp({var_name});\n"
            getter_server += "    if (filename == NULL) {\n"
            getter_server += "        result.charptr.charptr_len = 0;\n"
            getter_server += "        result.charptr.charptr_val = NULL;\n"
            getter_server += "    } else {\n"
            getter_server += "        result.charptr.charptr_len = strlen(filename) + 1;\n"
            getter_server += "        result.charptr.charptr_val = filename;\n"
            getter_server += "    }\n"
            getter_server += "    return &result;\n"
            getter_server += "}\n\n"
        
            
            # Setter - 接收文件信息并打开文件
            setter_server = f"void *set_{var_name}_1_svc(char_ptr value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static int result;\n"
            setter_server += "    if (value.charptr.charptr_val == NULL) {\n"
            setter_server += f"        {var_name} = NULL;\n"
            setter_server += "    } else {\n"
            setter_server += "        // 假设文件模式为读写\n"
            setter_server += '      if (strcmp(value.charptr.charptr_val, "stdin") == 0) {\nj'
            getter_server += f"            {var_name} = stdin;\n"
            getter_server += "        } else if (strcmp(value.charptr.charptr_val, \"stdout\") == 0) {\n"
            getter_server += f"            {var_name} = stdout;\n"
            getter_server += "        } else if (strcmp(value.charptr.charptr_val, \"stderr\") == 0) {\n"
            getter_server += f"            {var_name} = stderr;\n"
            getter_server += "        } else {\n"
            getter_server += f"            {var_name} = fopen(value.charptr.charptr_val, \"r+\");\n"
            getter_server += "            if ({var_name} == NULL) {\n"
            getter_server += "                perror(\"fopen failed\");\n"
            getter_server += "                return NULL;\n"
            getter_server += "            }\n"
            getter_server += "        }\n"
            setter_server += "    }\n"
            setter_server += "    return (void *)&result;\n"
            setter_server += "}\n\n"
            
            server_functions.append(getter_server)
            server_functions.append(setter_server)
            
            # === 客户端 Wrapper ===
            # Getter - 根据服务端返回的信息打开本地文件
            getter_wrapper = f"FILE* get_{var_name}_wrapper()\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    char_ptr *result;\n"
            getter_wrapper += f"    FILE *ret_val = NULL;\n"
            getter_wrapper += f"    result = get_{var_name}_1(clnt);\n"
            getter_wrapper += "    if (result == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            getter_wrapper += "        return NULL;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    if (result->charptr.charptr_val != NULL) {\n"
            getter_wrapper += "        if (strcmp(result->charptr.charptr_val, \"stdin\") == 0) {\n"
            getter_wrapper += "            ret_val = stdin;\n"
            getter_wrapper += "        } else if (strcmp(result->charptr.charptr_val, \"stdout\") == 0) {\n"
            getter_wrapper += "            ret_val = stdout;\n"
            getter_wrapper += "        } else if (strcmp(result->charptr.charptr_val, \"stderr\") == 0) {\n"
            getter_wrapper += "            ret_val = stderr;\n"
            getter_wrapper += "        } else {\n"
            getter_wrapper += "            ret_val = fopen(result->charptr.charptr_val, \"r+\");\n"
            getter_wrapper += "            if (ret_val == NULL) {\n"
            getter_wrapper += "                perror(\"fopen failed\");\n"
            getter_wrapper += "                return NULL;\n"
            getter_wrapper += "            }\n"
            getter_wrapper += "        }\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    return ret_val;\n"
            getter_wrapper += "}\n\n"

            # Setter - 将文件信息传递给服务端
            setter_wrapper = f"void set_{var_name}_wrapper(FILE *file)\n"
            setter_wrapper += "{\n"
            setter_wrapper += "    void *result;\n"
            setter_wrapper += "    char_ptr rpc_value;\n"
            setter_wrapper += "    char *filename=get_filename_from_fp(file);\n"
            setter_wrapper += "    if (filename == NULL) {\n"
            setter_wrapper += "        rpc_value.charptr.charptr_len = 0;\n"
            setter_wrapper += "        rpc_value.charptr.charptr_val = NULL;\n"
            setter_wrapper += "    } else {\n"
            setter_wrapper += "        rpc_value.charptr.charptr_len = strlen(filename) + 1;\n"
            setter_wrapper += "        rpc_value.charptr.charptr_val = filename;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    result = set_{var_name}_1(rpc_value, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "}\n\n"
            
            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)
            
            # Header
            wrapper_header_functions.append(f"FILE* get_{var_name}_wrapper();\n")
            wrapper_header_functions.append(f"void set_{var_name}_wrapper(FILE *file);\n")
            
        elif isinstance(var_type.type, c_ast.FuncDecl):
            # IDL 定义
            getter_idl = f"char_ptr get_{var_name}() = {function_counter};\n"
            setter_idl = f"void set_{var_name}(char_ptr value) = {function_counter + 1};\n"
            function_counter= function_counter + 2
            
            idl_functions.append(getter_idl)
            idl_functions.append(setter_idl)

            # === 服务端实现 ===
            # Getter - 处理函数指针的特殊情况，通常不直接传递函数指针，而是传递函数名
            getter_server = f"char_ptr *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += "    static char_ptr result;\n"
            getter_server += f"    if ({var_name} == NULL) {{\n"
            getter_server += "        result.charptr.charptr_len = 0;\n"
            getter_server += "        result.charptr.charptr_val = NULL;\n"
            getter_server += "    } else {\n"
            #const char* find_function_name_by_address(void* func_address)
            getter_server+= f"        const char* func_name = find_function_name_by_address({var_name});\n"
            getter_server += "        if (func_name == NULL) {\n"
            getter_server += "            result.charptr.charptr_len = 0;\n"
            getter_server += "            result.charptr.charptr_val = NULL;\n"
            getter_server += "        } else {\n"
            getter_server += "            result.charptr.charptr_len = strlen(func_name) + 1;\n"
            getter_server += "            result.charptr.charptr_val = func_name;\n"
            getter_server += "        }\n"
            getter_server += "    }\n"
            getter_server += "    return &result;\n"
            getter_server += "}\n\n"

            # Setter - 处理函数指针的特殊情况，通常不直接传递函数指针，而是传递函数名
            setter_server = f"void *set_{var_name}_1_svc(char_ptr value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static void *result;\n"
            setter_server += f"    if (value.charptr.charptr_val == NULL) {{\n"
            setter_server += f"        {var_name} = NULL;\n"
            setter_server += f"    }} else {{\n"
            setter_server += f"        void *func_address = find_function_ptr_by_name(value.charptr.charptr_val);\n"
            setter_server += f"        if (func_address == NULL) {{\n"
            setter_server += "            perror(\"find_function_by_name failed\");\n"
            setter_server += "            return NULL;\n"
            setter_server += f"        }}\n"
            setter_server += f"        {var_name} = func_address;\n"
            setter_server += "    }\n"
            setter_server += "    return NULL;\n"
            setter_server += "}\n\n"

            server_functions.append(getter_server)
            server_functions.append(setter_server)
            # === 客户端 Wrapper ===
            # Getter - 处理函数指针的特殊情况，通常不直接传递函数
            getter_wrapper = f"void* get_{var_name}_wrapper()\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    char_ptr *result;\n"
            getter_wrapper += f"    void *ret_val = NULL;\n"
            getter_wrapper += f"    result = get_{var_name}_1(clnt);\n"
            getter_wrapper += "    if (result == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            getter_wrapper += "        return NULL;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    if (result->charptr.charptr_val != NULL) {\n"
            getter_wrapper += "        ret_val = find_function_by_name(result->charptr.charptr_val);\n"
            getter_wrapper += "        if (ret_val == NULL) {\n"
            getter_wrapper += "            perror(\"find_function_by_name failed\");\n"
            getter_wrapper += "            return NULL;\n"
            getter_wrapper += "        }\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    return ret_val;\n"
            getter_wrapper += "}\n\n"

            # Setter - 处理函数指针的特殊情况，通常不直接传递函数指针，而是传递函数名
            setter_wrapper = f"void set_{var_name}_wrapper(void *func_ptr)\n"
            setter_wrapper += "{\n"
            setter_wrapper += "   char_ptr rpc_value;\n"
            setter_wrapper +"     char *func_name = find_function_name_by_address(func_ptr);\n"
            setter_wrapper += "    if (func_name == NULL) {\n"
            setter_wrapper += f"        rpc_value.charptr.charptr_len = 0;\n"
            setter_wrapper += f"        rpc_value.charptr.charptr_val = NULL;\n"
            setter_wrapper += "    } else {\n"
            setter_wrapper += f"        rpc_value.charptr.charptr_len = strlen(func_name) + 1;\n"
            setter_wrapper += f"        rpc_value.charptr.charptr_val = malloc(rpc_value.charptr.charptr_len);\n"
            setter_wrapper += "        if (rpc_value.charptr.charptr_val == NULL) {\n"
            setter_wrapper += "            perror(\"malloc failed\");\n"
            setter_wrapper += "            return;\n"
            setter_wrapper += "        }\n"
            setter_wrapper += f"        memcpy(rpc_value.charptr.charptr_val, func_name, rpc_value.charptr.charptr_len);\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    void *result = set_{var_name}_1(rpc_value, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "}\n\n"
            
            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)

        else:
            # 检查是否为结构体类型
            is_struct = False
            struct_def = None
            struct_name = None

            if base_ptr_type.startswith("struct "):
                struct_name = base_ptr_type.split(" ")[1]
                if struct_name in struct_dict:
                    is_struct = True
                    struct_def = struct_dict[struct_name]['ast']
            elif base_ptr_type in typedef_dict and typedef_dict[base_ptr_type]['isstruct']:
                is_struct = True
                struct_name = base_ptr_type
                struct_def = typedef_dict[base_ptr_type]['ast']
            elif base_ptr_type in struct_dict:
                is_struct = True
                struct_name = base_ptr_type
                struct_def = struct_dict[base_ptr_type]['ast']

            # 处理结构体情况
            if is_struct and struct_def:
                # 处理结构体指针类型
                if struct_name+"_rpc" not in typedef_struct_rpc:
                    typedef_struct_rpc.append(struct_name+"_rpc")
                    member_list = []
                    struct_str = f"struct {struct_name}_rpc {{\n"
                    # if typeast.name in typedef_dict:
                    get_rpc_struct_str(struct_def, member_list)
                    for member in member_list:
                        struct_str += "    " + member + "\n"
                    struct_str += "};\n"
                    idl_structs.append(struct_str)
                    idl_structs.append(f"typedef struct {struct_name}_rpc {struct_name}_rpc;\n")
                    #调用get_rpc_struct_str
                    pass
                # 为结构体指针创建专用的 RPC 类型
                struct_ptr_name = f"{struct_name}_rpc_ptr"
                if struct_ptr_name not in typedef_struct_rpc:
                    typedef_str="typedef "+struct_name+"_rpc "+struct_name+"_rpc_ptr<>;"
                    idl_structs.append(typedef_str)
                    typedef_struct_rpc.append(struct_name+"_rpc_ptr")
                    idl_structs.append(f"typedef struct {struct_ptr_name} {struct_ptr_name};\n")

                # if struct_ptr_name not in [s.split()[1] if len(s.split()) > 1 else "" for s in idl_structs]:
                #     struct_ptr_struct = f"struct {struct_ptr_name} {{ {struct_name}_rpc {struct_name}_items<>; }};\n"
                #     struct_ptr_typedef = f"typedef struct {struct_ptr_name} {struct_ptr_name};\n"
                #     idl_structs.append(struct_ptr_struct)
                #     idl_structs.append(struct_ptr_typedef)

                function_counter=process_struct_whole(var_name, struct_name, "struct_ptr_name",True,
                                      idl_functions, idl_structs,
                        server_functions,
                        wrapper_functions, wrapper_header_functions,
                        function_counter)

                # # IDL 定义
                # getter_idl = f"{struct_ptr_name} get_{var_name}() = {function_counter};\n"
                # setter_idl = f"void set_{var_name}({struct_ptr_name} value) = {function_counter + 1};\n"
                # function_counter= function_counter + 2  # 更新函数计数器
                
                # idl_functions.append(getter_idl)
                # idl_functions.append(setter_idl)
                
                # # === 服务端实现 ===
                # # Getter 函数
                # getter_server = f"{struct_ptr_name} *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
                # getter_server += "{\n"
                # getter_server += f"    static {struct_ptr_name} result;\n"
                # getter_server += f"    if ({var_name} == NULL) {{\n"
                # getter_server += f"        result.{struct_name}_items.{struct_name}_items_len = 0;\n"
                # getter_server += f"        result.{struct_name}_items.{struct_name}_items_val = NULL;\n"
                # getter_server += "    } else {\n"
                # getter_server += "        // 假设指针指向单个或有限个结构体\n"
                # getter_server += f"        result.{struct_name}_items.{struct_name}_items_len = 1;\n"
                # getter_server += f"        static {struct_name} buffer_item;\n"
                # if struct_name in typedef_dict:
                #     getter_server += f"        memcpy(&buffer_item, {var_name}, sizeof({struct_name}));\n"
                # else:
                #     getter_server += f"        memcpy(&buffer_item, {var_name}, sizeof(struct {struct_name}));\n"
                # getter_server += f"        result.{struct_name}_items.{struct_name}_items_val = &buffer_item;\n"
                # getter_server += "    }\n"
                # getter_server += "    return &result;\n"
                # getter_server += "}\n\n"
                
                # # Setter 函数
                # setter_server = f"void *set_{var_name}_1_svc({struct_ptr_name} value, struct svc_req *rqstp)\n"
                # setter_server += "{\n"
                # setter_server += "    static int result = 1;\n"
                # setter_server += f"    if ({var_name} != NULL) {{\n"
                # setter_server += f"        free({var_name});\n"
                # setter_server += f"        {var_name} = NULL;\n"
                # setter_server += "    }\n"
                # setter_server += f"    if (value.{struct_name}_items.{struct_name}_items_val != NULL && "
                # setter_server += f"value.{struct_name}_items.{struct_name}_items_len > 0) {{\n"
                # if struct_name in typedef_dict:
                #     setter_server += f"        {var_name} = ({struct_name} *)malloc(value.{struct_name}_items.{struct_name}_items_len * sizeof({struct_name}));\n"
                # else:
                #     setter_server += f"        {var_name} = (struct {struct_name} *)malloc(value.{struct_name}_items.{struct_name}_items_len * sizeof(struct {struct_name}));\n"
                # setter_server += f"        if ({var_name} == NULL) {{\n"
                # setter_server += "            perror(\"malloc failed\");\n"
                # setter_server += "            result = 0;\n"
                # setter_server += "        } else {\n"
                # if struct_name in typedef_dict:
                #     setter_server += f"            memcpy({var_name}, value.{struct_name}_items.{struct_name}_items_val, "
                #     setter_server += f"value.{struct_name}_items.{struct_name}_items_len * sizeof({struct_name}));\n"
                # else:
                #     setter_server += f"            memcpy({var_name}, value.{struct_name}_items.{struct_name}_items_val, "
                #     setter_server += f"value.{struct_name}_items.{struct_name}_items_len * sizeof(struct {struct_name}));\n"
                # setter_server += "        }\n"
                # setter_server += "    }\n"
                # setter_server += "    return (void *)&result;\n"
                # setter_server += "}\n\n"
                
                # server_functions.append(getter_server)
                # server_functions.append(setter_server)
                
                # # === 客户端 Wrapper ===
                # # Getter Wrapper
                # getter_wrapper = f"{base_ptr_type}* get_{var_name}_wrapper()\n"
                # getter_wrapper += "{\n"
                # getter_wrapper += f"    {struct_ptr_name} *result;\n"
                # if struct_name in typedef_dict:
                #     getter_wrapper += f"    {struct_name} *ret_val = NULL;\n"
                # else:
                #     getter_wrapper += f"    struct {struct_name} *ret_val = NULL;\n"
                # getter_wrapper += f"    result = get_{var_name}_1(clnt);\n"
                # getter_wrapper += "    if (result == NULL) {\n"
                # getter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
                # getter_wrapper += "        return NULL;\n"
                # getter_wrapper += "    }\n"
                # getter_wrapper += f"    if (result->{struct_name}_items.{struct_name}_items_val != NULL && "
                # getter_wrapper += f"result->{struct_name}_items.{struct_name}_items_len > 0) {{\n"
                # if struct_name in typedef_dict:
                #     getter_wrapper += f"        ret_val = ({struct_name} *)malloc(result->{struct_name}_items.{struct_name}_items_len * sizeof({struct_name}));\n"
                # else:
                #     getter_wrapper += f"        ret_val = (struct {struct_name} *)malloc(result->{struct_name}_items.{struct_name}_items_len * sizeof(struct {struct_name}));\n"
                # getter_wrapper += "        if (ret_val == NULL) {\n"
                # getter_wrapper += "            perror(\"malloc failed\");\n"
                # getter_wrapper += "            return NULL;\n"
                # getter_wrapper += "        }\n"
                # getter_wrapper += f"        memcpy(ret_val, result->{struct_name}_items.{struct_name}_items_val, "
                # if struct_name in typedef_dict:
                #     getter_wrapper += f"result->{struct_name}_items.{struct_name}_items_len * sizeof({struct_name}));\n"
                # else:
                #     getter_wrapper += f"result->{struct_name}_items.{struct_name}_items_len * sizeof(struct {struct_name}));\n"
                # getter_wrapper += "    }\n"
                # getter_wrapper += "    return ret_val;\n"
                # getter_wrapper += "}\n\n"
                
                # # Setter Wrapper
                # setter_wrapper = f"int set_{var_name}_wrapper(const {base_ptr_type} *value, size_t count)\n"
                # setter_wrapper += "{\n"
                # setter_wrapper += f"    void *result;\n"
                # setter_wrapper += f"    {struct_ptr_name} rpc_value;\n"
                # setter_wrapper += "    if (value == NULL || count == 0) {\n"
                # setter_wrapper += f"        rpc_value.{struct_name}_items.{struct_name}_items_len = 0;\n"
                # setter_wrapper += f"        rpc_value.{struct_name}_items.{struct_name}_items_val = NULL;\n"
                # setter_wrapper += "    } else {\n"
                # setter_wrapper += f"        rpc_value.{struct_name}_items.{struct_name}_items_len = count;\n"
                # if struct_name in typedef_dict:
                #     setter_wrapper += f"        rpc_value.{struct_name}_items.{struct_name}_items_val = ({struct_name} *)value;\n"
                # else:
                #     setter_wrapper += f"        rpc_value.{struct_name}_items.{struct_name}_items_val = (struct {struct_name} *)value;\n"
                # setter_wrapper += "    }\n"
                # setter_wrapper += f"    result = set_{var_name}_1(rpc_value, clnt);\n"
                # setter_wrapper += "    if (result == NULL) {\n"
                # setter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
                # setter_wrapper += "        return 0;\n"
                # setter_wrapper += "    }\n"
                # setter_wrapper += "    return 1;\n"
                # setter_wrapper += "}\n\n"
                
                # wrapper_functions.append(getter_wrapper)
                # wrapper_functions.append(setter_wrapper)
                
                # # Header
                # wrapper_header_functions.append(f"{base_ptr_type}* get_{var_name}_wrapper();\n")
                # wrapper_header_functions.append(f"int set_{var_name}_wrapper(const {base_ptr_type} *value, size_t count);\n")

                
                print(f"Struct definition: {struct_def}")
                if struct_def.name=="acc_t":
                    return function_counter #因为此结构体中含有自身的指针，所以不再递归处理，避免死循环
                
                # 为结构体的每个成员递归生成访问函数
                if (hasattr(struct_def, 'decls') and struct_def.decls) or (hasattr(struct_def.type.type, 'decls') and struct_def.type.type.decls):
                    # 首先为整体结构体指针生成 process_struct_whole
                    # process_struct_whole(var_name, base_ptr_type, struct_name, generator,
                    #                 idl_functions, idl_structs,
                    #                 server_functions,
                    #                 wrapper_functions, wrapper_header_functions,
                    #                 function_counter + 2)  # 避免函数编号冲突
                    print(f"Processing struct pointer: {var_name} -> {struct_name}")
                    # 然后为结构体的每个成员递归生成 process_struct_member
                    if hasattr(struct_def, 'decls') and struct_def.decls:
                        for i, member in enumerate(struct_def.decls):
                            if member.name:  # 确保成员有名称
                                member_name = member.name
                                member_type = generator.visit(member.type)
                                
                                # 创建表示"var_name->member_name"的访问路径
                                member_access_name = f"{var_name}_ptr_{member_name}"
                                print(f"Processing struct member: {member_name} -> {member_type}")
                                # 递归处理结构体成员
                                processed_types = set()  # 创建一个新的集合来跟踪已处理的类型
                                function_counter =process_struct_member(var_name, member_name, member_type, member.type, generator,
                                                idl_functions, idl_structs,
                                                server_functions,
                                                wrapper_functions, wrapper_header_functions,
                                                function_counter ,processed_types)  # 为每个成员分配唯一的函数编号
                    else:
                        for i, member in enumerate(struct_def.type.type.decls):
                            if member.name:
                                member_name = member.name
                                member_type = generator.visit(member.type)
                                processed_types = set()  # 创建一个新的集合来跟踪已处理的类型
                                function_counter=process_struct_member(var_name, member_name, member_type, member.type, generator,
                                                idl_functions, idl_structs,
                                                server_functions,
                                                wrapper_functions, wrapper_header_functions,
                                                function_counter,processed_types)  # 为每个成员分配唯一的函数编号
                

                                
                
                # 返回，避免执行默认处理
                 

            # 没有匹配结构体的情况，继续原有的指针处理...
            else:
                # 处理其他指针类型的代码...
                print(f"Unhandled pointer type: {base_ptr_type} for variable {var_name}")
                pass
        return function_counter
    
    def eval_string_expr_numexpr(expr):
        """
        使用 numexpr 解析并计算数学表达式，支持复杂运算。
        """
        try:
            result = ne.evaluate(expr)
            return result.item()  # 转换为标量
        except Exception as e:
            print(f"错误：无效的表达式 '{expr}'：{e}")
            return None

    def process_array_type_var(var_name, type_str, base_type, var_type, generator, 
                            idl_functions, idl_structs, 
                            server_functions, 
                            wrapper_functions, wrapper_header_functions, 
                            function_counter):
        """处理数组类型的全局变量"""
        # 获取数组元素类型和维度
        print(f"Processing array variable: {var_name} of type {type_str}")
        elem_type = generator.visit(var_type.type)
        if hasattr(var_type, 'dim') and var_type.dim:
            print(f"Array dimensions: {var_type.dim}")
            """dim=BinaryOp(op='+',
                       left=BinaryOp(op='+',
                                     left=Constant(type='int',
                                                   value='1'
                                                   ),
                                     right=Constant(type='int',
                                                    value='39'
                                                    )
                                     ),
                       right=Constant(type='int',
                                      value='1'
                                      )
                       ),
,"""
            # dim若是BinaryOp类型，则需要计算
            #若字符串为 2*1024，将字符串转换为数学运算并计算值
            dim_str = generator.visit(var_type.dim)

            result = eval_string_expr_numexpr(dim_str)
            if result is not None:
                dim_size = str(result)  # 将计算结果转换为字符串
            else:
                dim_size = generator.visit(var_type.dim)
            #2 * 1024计算得到2048
 

        else:
            dim_size = "1" # 默认值
        print(f"Processing array:")
        print(generator.visit(var_type))
        srctype=generator.visit(var_type)
        
        # 构建数组对应的IDL类型
        array_struct_name = f"{var_name}_array"
        if elem_type == "char *" or elem_type == "const char *":
            # 字符数组特殊处理（字符串）
            array_struct = f"struct {array_struct_name} {{ char_ptr {var_name}[{dim_size}]; }};\n"
        elif elem_type in basic_type:
            array_struct = f"struct {array_struct_name} {{ {elem_type} {var_name}[{dim_size}]; }};\n"
        else:
            print(elem_type)
            if elem_type.startswith("struct "):
                elem_type = elem_type.split(" ")[1]  # 提取结构体名称
            
            if elem_type in typedef_dict and typedef_dict[elem_type]['isstruct']:
                typeast=typedef_dict[elem_type]['ast']
                is_struct = typedef_dict[elem_type]['isstruct']
                if is_struct:
                    if typeast.name+"_rpc" not in typedef_struct_rpc:
                        struct_def = f"struct {typeast.name}_rpc{{\n"                    
                        member_list = []
                        # if typeast.name in typedef_dict:
                        get_rpc_struct_str(typedef_dict[typeast.name]['ast'], member_list)
                        # else:
                        #     get_rpc_struct_str(struct_dict[typeast.name]['ast'], member_list)                   
                        for member in member_list:
                            struct_def += "    " + member + "\n"                   
                        struct_def += "};\n"                   
                        idl_structs.append(struct_def)
                        idl_structs.append(f"typedef struct {typeast.name}_rpc {typeast.name}_rpc;\n")
                        return  function_counter
            elif elem_type in struct_dict and struct_dict[elem_type]['isstruct']:
                typeast=struct_dict[elem_type]['ast']
                is_struct = struct_dict[elem_type]['isstruct']
                if is_struct:
                    if typeast.name+"_rpc" not in typedef_struct_rpc:
                        struct_def = f"struct {typeast.name}_rpc{{\n"                    
                        member_list = []
                        # if typeast.name in typedef_dict:
                        #     get_rpc_struct_str(typedef_dict[typeast.name]['ast'], member_list)
                        # else:
                        get_rpc_struct_str(struct_dict[typeast.name]['ast'], member_list)                   
                        for member in member_list:
                            struct_def += "    " + member + "\n"                   
                        struct_def += "};\n"                   
                        idl_structs.append(struct_def)
                        idl_structs.append(f"typedef struct {typeast.name}_rpc {typeast.name}_rpc;\n")
                return function_counter
            else:
                print(elem_type)
                print(f"Unhandled array element type: {elem_type} for variable {var_name}")
            return function_counter
        array_typedef = f"typedef struct {array_struct_name} {array_struct_name};\n"
        
        # 添加结构体定义
        if array_struct not in idl_structs:
            idl_structs.append(array_struct)
            idl_structs.append(array_typedef)
        
        # IDL函数定义
        getter_idl = f"{array_struct_name} get_{var_name}() = {function_counter};\n"
        setter_idl = f"void set_{var_name}({array_struct_name} value) = {function_counter + 1};\n"
        function_counter += 2  # 更新函数计数器
        
        idl_functions.append(getter_idl)
        idl_functions.append(setter_idl)

        if elem_type=="char":
            # 字符数组特殊处理（字符串）
            # 对于服务端实现，直接复制数组内容
            getter_server = f"{array_struct_name} *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += f"    static {array_struct_name} result;\n"
            # 安全地复制字符数组，确保不超出数组大小
            getter_server += f"    strncpy(result.{var_name}, {var_name}, {dim_size});\n"
            getter_server += f"    result.{var_name}[{dim_size} - 1] = '\\0'; // 确保字符串正确终止\n"
            getter_server += "    return &result;\n"
            getter_server += "}\n\n"
            
            # 服务端setter函数
            setter_server = f"void *set_{var_name}_1_svc({array_struct_name} value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static int result = 1;\n"
            setter_server += f"    strncpy({var_name}, value.{var_name}, {dim_size});\n"
            setter_server += f"    {var_name}[{dim_size} - 1] = '\\0'; // 确保字符串正确终止\n"
            setter_server += "    return (void *)&result;\n"
            setter_server += "}\n\n"
            
            server_functions.append(getter_server)
            server_functions.append(setter_server)
            
            # 客户端Wrapper函数
            # Getter函数 - 从服务端获取字符数组
            getter_wrapper = f"char* get_{var_name}_wrapper()\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    {array_struct_name} *result;\n"
            getter_wrapper += f"    static char local_copy[{dim_size}];\n"
            getter_wrapper += f"    result = get_{var_name}_1(clnt);\n"
            getter_wrapper += "    if (result == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            getter_wrapper += f"        memset(local_copy, 0, {dim_size});\n"
            getter_wrapper += "        return local_copy;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += f"    strncpy(local_copy, result->{var_name}, {dim_size});\n"
            getter_wrapper += f"    local_copy[{dim_size} - 1] = '\\0'; // 确保字符串正确终止\n"
            getter_wrapper += "    return local_copy;\n"
            getter_wrapper += "}\n\n"
            
            # Setter函数 - 设置服务端字符数组
            setter_wrapper = f"int set_{var_name}_wrapper(const char *value)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    {array_struct_name} param;\n"
            setter_wrapper += "    void *result;\n"
            setter_wrapper += "    if (value == NULL) {\n"
            setter_wrapper += f"        param.{var_name}[0] = '\\0';\n"
            setter_wrapper += "    } else {\n"
            setter_wrapper += f"        strncpy(param.{var_name}, value, {dim_size});\n"
            setter_wrapper += f"        param.{var_name}[{dim_size} - 1] = '\\0';\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    result = set_{var_name}_1(param, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            setter_wrapper += "        return 0;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    return 1;\n"
            setter_wrapper += "}\n\n"
            
            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)
            
            # Header文件声明
            wrapper_header_functions.append(f"char* get_{var_name}_wrapper();\n")
            wrapper_header_functions.append(f"int set_{var_name}_wrapper(const char *value);\n")
        elif elem_type=="char *":
            # 字符串数组特殊处理（指针数组）
            # 对于服务端实现，直接复制指针数组内容
            getter_server = f"{array_struct_name} *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += f"    static {array_struct_name} result;\n"
            getter_server += f"    for (int i = 0; i < {dim_size}; i++) {{\n"
            getter_server += f"        if ({var_name}[i] != NULL) {{\n"
            getter_server += f"            int len = strlen({var_name}[i]) + 1;\n"
            getter_server += f"            result.{var_name}[i].charptr.charptr_len = len;\n"
            getter_server += f"            result.{var_name}[i].charptr.charptr_val = malloc(len);\n"
            getter_server += f"            if (result.{var_name}[i].charptr.charptr_val != NULL) {{\n"
            getter_server += f"                memcpy(result.{var_name}[i].charptr.charptr_val, {var_name}[i], len);\n"
            getter_server += f"            }}\n"
            getter_server += f"        }} else {{\n"
            getter_server += f"            result.{var_name}[i].charptr.charptr_len = 0;\n"
            getter_server += f"            result.{var_name}[i].charptr.charptr_val = NULL;\n"
            getter_server += f"        }}\n"
            getter_server += "    }\n"
            getter_server += "    return &result;\n"
            getter_server += "}\n\n"

            # 服务端setter
            setter_server = f"void *set_{var_name}_1_svc({array_struct_name} value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static int result = 1;\n"
            setter_server += f"    for (int i = 0; i < {dim_size}; i++) {{\n"
            setter_server += f"        if ({var_name}[i] != NULL) {{\n"
            setter_server += f"            free({var_name}[i]);\n"
            setter_server += f"            {var_name}[i] = NULL;\n"
            setter_server += f"        }}\n"
            setter_server += f"        if (value.{var_name}[i].charptr.charptr_val != NULL && "
            setter_server += f"value.{var_name}[i].charptr.charptr_len > 0) {{\n"
            setter_server += f"            {var_name}[i] = malloc(value.{var_name}[i].charptr.charptr_len);\n"
            setter_server += f"            if ({var_name}[i] != NULL) {{\n"
            setter_server += f"                memcpy({var_name}[i], value.{var_name}[i].charptr.charptr_val, "
            setter_server += f"value.{var_name}[i].charptr.charptr_len);\n"
            setter_server += f"            }}\n"
            setter_server += f"        }}\n"
            setter_server += "    }\n"
            setter_server += "    return (void *)&result;\n"
            setter_server += "}\n\n"

            server_functions.append(getter_server)
            server_functions.append(setter_server)

            # 客户端Wrapper函数
            # Getter - 返回字符串数组指针
            getter_wrapper = f"char** get_{var_name}_wrapper()\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    {array_struct_name} *result;\n"
            getter_wrapper += f"    static char *local_copy[{dim_size}];\n"
            getter_wrapper += f"    result = get_{var_name}_1(clnt);\n"
            getter_wrapper += "    if (result == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            getter_wrapper += f"        memset(local_copy, 0, sizeof(local_copy));\n"
            getter_wrapper += "        return local_copy;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += f"    for (int i = 0; i < {dim_size}; i++) {{\n"
            getter_wrapper += f"        if (result->{var_name}[i].charptr.charptr_val != NULL) {{\n"
            getter_wrapper += f"            local_copy[i] = malloc(result->{var_name}[i].charptr.charptr_len);\n"
            getter_wrapper += f"            if (local_copy[i] != NULL) {{\n"
            getter_wrapper += f"                memcpy(local_copy[i], result->{var_name}[i].charptr.charptr_val, "
            getter_wrapper += f"result->{var_name}[i].charptr.charptr_len);\n"
            getter_wrapper += f"            }}\n"
            getter_wrapper += f"        }} else {{\n"
            getter_wrapper += f"            local_copy[i] = NULL;\n"
            getter_wrapper += f"        }}\n"
            getter_wrapper += "    }\n"
            getter_wrapper += "    return local_copy;\n"
            getter_wrapper += "}\n\n"
            # Setter - 从提供的字符串数组设置服务器端数组
            setter_wrapper = f"int set_{var_name}_wrapper(const char **value)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    {array_struct_name} param;\n"
            setter_wrapper += "    void *result;\n"
            setter_wrapper += f"    for (int i = 0; i < {dim_size}; i++) {{\n"
            setter_wrapper += f"        if (value == NULL || value[i] == NULL) {{\n"
            setter_wrapper += f"            param.{var_name}[i].charptr.charptr_len = 0;\n"
            setter_wrapper += f"            param.{var_name}[i].charptr.charptr_val = NULL;\n"
            setter_wrapper += "        } else {\n"
            setter_wrapper += f"            int len = strlen(value[i]) + 1;\n"
            setter_wrapper += f"            param.{var_name}[i].charptr.charptr_len = len;\n"
            setter_wrapper += f"            param.{var_name}[i].charptr.charptr_val = malloc(len);\n"
            setter_wrapper += f"            if (param.{var_name}[i].charptr.charptr_val != NULL) {{\n"
            setter_wrapper += f"                memcpy(param.{var_name}[i].charptr.charptr_val, value[i], len);\n"
            setter_wrapper += f"            }}\n"
            setter_wrapper += "        }\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    result = set_{var_name}_1(param, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            setter_wrapper += "        return 0;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    return 1;\n"
            setter_wrapper += "}\n\n"

            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)
            # Header文件声明
            wrapper_header_functions.append(f"char** get_{var_name}_wrapper();\n")
            wrapper_header_functions.append(f"int set_{var_name}_wrapper(const char **value);\n")

        else:
            # 处理其他类型的数组
            # 服务端实现
            getter_server = f"{array_struct_name} *get_{var_name}_1_svc(struct svc_req *rqstp)\n"
            getter_server += "{\n"
            getter_server += f"    static {array_struct_name} result;\n"
            # 复制数组内容
            getter_server += f"    memcpy(result.{var_name}, {var_name}, sizeof({var_name}));\n"
            getter_server += "    return &result;\n"
            getter_server += "}\n\n"
            
            # 服务端setter
            setter_server = f"void *set_{var_name}_1_svc({array_struct_name} value, struct svc_req *rqstp)\n"
            setter_server += "{\n"
            setter_server += "    static int result = 1;\n"
            setter_server += f"    memcpy({var_name}, value.{var_name}, sizeof({var_name}));\n"
            setter_server += "    return (void *)&result;\n"
            setter_server += "}\n\n"
            
            server_functions.append(getter_server)
            server_functions.append(setter_server)
            
            # 客户端Wrapper函数
            # Getter - 返回数组指针（通常不推荐，这里为简化实现）
            getter_wrapper = f"{elem_type}* get_{var_name}_wrapper()\n"
            getter_wrapper += "{\n"
            getter_wrapper += f"    {array_struct_name} *result;\n"
            getter_wrapper += f"    static {elem_type} local_copy[{dim_size}];\n"
            getter_wrapper += f"    result = get_{var_name}_1(clnt);\n"
            getter_wrapper += "    if (result == NULL) {\n"
            getter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            getter_wrapper += f"        memset(local_copy, 0, sizeof(local_copy));\n"
            getter_wrapper += "        return local_copy;\n"
            getter_wrapper += "    }\n"
            getter_wrapper += f"    memcpy(local_copy, result->{var_name}, sizeof(local_copy));\n"
            getter_wrapper += "    return local_copy;\n"
            getter_wrapper += "}\n\n"
            
            # Setter - 从提供的数组设置服务器端数组
            setter_wrapper = f"int set_{var_name}_wrapper(const {elem_type} *value)\n"
            setter_wrapper += "{\n"
            setter_wrapper += f"    {array_struct_name} param;\n"
            setter_wrapper += "    void *result;\n"
            setter_wrapper += "    if (value == NULL) {\n"
            setter_wrapper += f"        memset(param.{var_name}, 0, sizeof(param.{var_name}));\n"
            setter_wrapper += "    } else {\n"
            setter_wrapper += f"        memcpy(param.{var_name}, value, sizeof(param.{var_name}));\n"
            setter_wrapper += "    }\n"
            setter_wrapper += f"    result = set_{var_name}_1(param, clnt);\n"
            setter_wrapper += "    if (result == NULL) {\n"
            setter_wrapper += "        clnt_perror(clnt, \"call failed\");\n"
            setter_wrapper += "        return 0;\n"
            setter_wrapper += "    }\n"
            setter_wrapper += "    return 1;\n"
            setter_wrapper += "}\n\n"
            
            wrapper_functions.append(getter_wrapper)
            wrapper_functions.append(setter_wrapper)
            
            # Header文件声明
            wrapper_header_functions.append(f"{elem_type}* get_{var_name}_wrapper();\n")
            wrapper_header_functions.append(f"int set_{var_name}_wrapper(const {elem_type} *value);\n")
        return function_counter


   


    
    
    
    # 用于生成唯一的函数编号
    function_counter = 100  # 从100开始，避免与现有函数冲突
    
    for var_name in global_vars_set:
        if var_name not in global_var_asts:
            print(f"警告: 找不到全局变量 {var_name} 的 AST 节点")
            continue
        
        var_info = global_var_asts[var_name]
        var_node = var_info['ast']
        var_type = var_node.type
        
        # 获取变量类型的字符串表示
        generator = c_generator.CGenerator()
        type_str = generator.visit(var_type)
        
        # 处理 "extern" 或其他修饰符
        type_str = type_str.replace("extern ", "")
        type_str = type_str.replace("static ", "")
        # type_str = type_str.replace("const ", "")
        type_str = type_str.replace("volatile ", "")
        base_type = type_str.split(" ")[0]
        print(f"处理全局变量 {var_name}，类型为 {type_str}")
        if isinstance(var_type.type,c_ast.Enum):
            type_str="int"
        print(var_type)
        
        # 处理不同类型的变量
        if isinstance(var_type, c_ast.TypeDecl):
            # 基本类型或结构体
            
            # 1. 首先检查是否是匿名结构体
            if isinstance(var_type.type, c_ast.Struct) and var_type.type.name is None and hasattr(var_type.type, 'decls') and var_type.type.decls:
                print(var_type)
                struct_def = var_type.type
                struct_name = "new_"+var_name  # 为匿名结构体生成一个唯一名称

                #构造一个名为new_var_name的结构体并添加到struct_dict中
                new_struct = c_ast.Struct(name=struct_name, decls=struct_def.decls)
                struct_dict[struct_name] = {
                    'ast': new_struct,
                    'isstruct': True,
                    'isunion': False,
                    'isenum': False
                }

                wrapper_header_functions.append(generator.visit(new_struct)+";\n")
                    


                if struct_name+"_rpc" not in typedef_struct_rpc:
                    struct_str = f"struct {struct_name}_rpc{{\n"
                    member_list = []
                    get_rpc_struct_str(struct_def, member_list)
                    
                    for member in member_list:
                        struct_str += "    " + member + "\n"                   
                    # 完成结构体定义
                    struct_str += "};\n"                   
                    # 添加到IDL结构体列表
                    idl_structs.append(struct_str)
                    idl_structs.append(f"typedef struct {struct_name}_rpc {struct_name}_rpc;\n")

                # 为匿名结构体生成一个整体的getter和setter
                function_counter=process_struct_whole(var_name, type_str, f"anon_struct_{var_name}", False,
                                    idl_functions, idl_structs,
                                    server_functions,
                                    wrapper_functions, wrapper_header_functions,
                                    function_counter)
                
                # 然后为结构体的每个成员递归生成getter和setter
                for i, member in enumerate(struct_def.decls):
                    if not member.name:  # 跳过没有名称的成员
                        continue
                        
                    member_name = member.name
                    member_type = generator.visit(member.type)
                    
                    # 递归处理每个成员
                    function_counter=process_struct_member(var_name, member_name, member_type, member.type, generator,
                                        idl_functions, idl_structs,
                                        server_functions,
                                        wrapper_functions, wrapper_header_functions,
                                        function_counter)
                
                # 更新函数计数器
            
            # 2. 检查是否是命名结构体
            elif isinstance(var_type.type, c_ast.Struct) and var_type.type.name:
                print(f"处理命名结构体全局变量 {var_name}，结构体名称: {var_type.type.name}")
                struct_name = var_type.type.name
                if var_type.type.decls:
                    struct_def = var_type.type
                else:
                    if struct_name in struct_dict:
                        struct_def = struct_dict[struct_name]['ast']
                if struct_name+"_rpc" not in typedef_struct_rpc:
                    typedef_struct_rpc.append(struct_name+"_rpc")
                    struct_str = f"struct {struct_name}_rpc{{\n"
                    member_list = []
                    get_rpc_struct_str(struct_def, member_list)
                    
                    for member in member_list:
                        struct_str += "    " + member + "\n"                   
                    # 完成结构体定义
                    struct_str += "};\n"                   
                    # 添加到IDL结构体列表
                    idl_structs.append(struct_str)
                    idl_structs.append(f"typedef struct {struct_name}_rpc {struct_name}_rpc;\n")

                
                # 为结构体整体生成getter和setter
                function_counter=process_struct_whole(var_name, type_str, struct_name, False,
                                idl_functions, idl_structs,
                                server_functions,
                                wrapper_functions, wrapper_header_functions,
                                function_counter)
                
                # 查找结构体定义
                struct_def = None
                if struct_name in struct_dict:
                    struct_def = struct_dict[struct_name]['ast']
                
                # 如果找到结构体定义，为每个成员生成getter和setter
                if struct_def and hasattr(struct_def, 'decls') and struct_def.decls:
                    for i, member in enumerate(struct_def.decls):
                        if not member.name:  # 跳过没有名称的成员
                            continue
                            
                        member_name = member.name
                        member_type = generator.visit(member.type)
                        
                        # 递归处理每个成员
                        function_counter=process_struct_member(var_name, member_name, member_type, member.type, generator,
                                            idl_functions, idl_structs,
                                            server_functions,
                                            wrapper_functions, wrapper_header_functions,
                                            function_counter)
                    
            
            # 3. 检查是否是typedef的结构体类型
            elif hasattr(var_type.type, 'names') and var_type.type.names[0] in typedef_dict:
                
                if typedef_dict[var_type.type.names[0]]['isstruct']:
                    print(f"处理typedef结构体全局变量 {var_name}，类型名称: {var_type.type.names[0]}")
                    type_name = var_type.type.names[0]
                    typedef_info = typedef_dict[type_name]
                    struct_def = typedef_info['ast']
                    if type_name+"_rpc" not in typedef_struct_rpc:
                        typedef_struct_rpc.append(type_name+"_rpc")
                        struct_str = f"struct {type_name}_rpc{{\n"
                        member_list = []
                        get_rpc_struct_str(struct_def, member_list)
                        
                        for member in member_list:
                            struct_str += "    " + member + "\n"                   
                        # 完成结构体定义
                        struct_str += "};\n"                   
                        # 添加到IDL结构体列表
                        idl_structs.append(struct_str)
                        idl_structs.append(f"typedef struct {type_name}_rpc {type_name}_rpc;\n")


                    
                    # 为结构体整体生成getter和setter
                    function_counter=process_struct_whole(var_name, type_str, type_name, False,
                                    idl_functions, idl_structs,
                                    server_functions,
                                    wrapper_functions, wrapper_header_functions,
                                    function_counter)
                    
                    # 获取结构体定义
                    
                    if hasattr(struct_def.type.type, 'decls') and struct_def.type.type.decls:
                        for i, member in enumerate(struct_def.type.type.decls):
                            if not member.name:  # 跳过没有名称的成员
                                continue
                                
                            member_name = member.name
                            member_type = generator.visit(member.type)
                            
                            # 递归处理每个成员
                            function_counter=process_struct_member(var_name, member_name, member_type, member.type, generator,
                                                idl_functions, idl_structs,
                                                server_functions,
                                                wrapper_functions, wrapper_header_functions,
                                                function_counter)
                        
                else:
                    typeast = typedef_dict[var_type.type.names[0]]['ast']
                    if isinstance(typeast.type, c_ast.PtrDecl) and isinstance(typeast.type.type, c_ast.FuncDecl):
                        #当成char_ptr处理
                        print(f"处理typedef函数指针全局变量 {var_name}，类型名称: {var_type.type.names[0]}")
                        modified_type_str = "char_ptr"
                        function_counter=process_ptr_type_var(var_name, modified_type_str, typeast.type, generator,
                                            idl_functions, idl_structs,
                                            server_functions,
                                            wrapper_functions, wrapper_header_functions,
                                            function_counter)
            
            # 4. 检查是否是枚举类型
            elif isinstance(var_type.type, c_ast.Enum) or (hasattr(var_type.type, 'names') and 
                    var_type.type.names[0] in typedef_dict and typedef_dict[var_type.type.names[0]]['isenum']):
                print(f"处理枚举类型全局变量 {var_name}")
                # 枚举类型当作整数处理
                modified_type_str = "int"
                function_counter=process_basic_type_var(var_name, modified_type_str, "int", generator, 
                                    idl_functions, idl_structs, 
                                    server_functions, 
                                    wrapper_functions, wrapper_header_functions, 
                                    function_counter)
            
            # 5. 处理基本类型或其他类型
            else:
                print(f"处理基本类型全局变量 {var_name}，类型: {type_str}")
                function_counter=process_basic_type_var(var_name, type_str, base_type, generator, 
                                    idl_functions, idl_structs, 
                                    server_functions, 
                                    wrapper_functions, wrapper_header_functions, 
                                    function_counter)
            
        elif isinstance(var_type, c_ast.PtrDecl):
            # 指针类型
            function_counter=process_ptr_type_var(var_name, type_str, var_type, generator, 
                                idl_functions, idl_structs, 
                                server_functions, 
                                wrapper_functions, wrapper_header_functions, 
                                function_counter)
            
        elif isinstance(var_type, c_ast.ArrayDecl):
            # 数组类型
            function_counter=process_array_type_var(var_name, type_str, base_type, var_type, generator, 
                                  idl_functions, idl_structs, 
                                  server_functions, 
                                  wrapper_functions, wrapper_header_functions, 
                                  function_counter)
        else:
            print(f"警告: 不支持的全局变量类型 {type_str}，变量名: {var_name}")
    
    

def append_to_files(proname, idl_functions, idl_structs, server_functions, wrapper_functions, wrapper_header_functions):
    """将生成的函数添加到对应文件"""
    
    # # 向IDL文件添加内容
    # idl_file = f"../partitioned_software/{proname}/6_rpc_refactor_result/IDL/{proname}_idl.x"
    idl_file=f'{proname}_idl.x'
    
    # 读取原始IDL文件
    with open(idl_file, 'r') as f:
        idl_content = f.read()
    
    # 找到插入点 - 在program定义之前
    program_pos = idl_content.find("program")
    if program_pos == -1:
        program_pos = len(idl_content)
    
    # 插入结构体定义
    new_idl_content = idl_content[:program_pos]
    for struct in idl_structs:
        if struct not in new_idl_content:
            new_idl_content += struct
    
    # 找到版本块内部
    version_start = idl_content.find("{", idl_content.find("version"))
    version_end = idl_content.find("}", version_start)
    
    # 插入函数定义
    functions_block = idl_content[version_start+1:version_end]
    for func in idl_functions:
        if func not in functions_block:
            functions_block += "       " + func
    
    # 重新组装IDL文件
    # 重新组装IDL文件
    new_idl_content += idl_content[program_pos:version_start+1] + functions_block + idl_content[version_end:]
    print("准备写入新IDL文件内容到:", idl_file)
    print(f"内容长度: {len(new_idl_content)} 字符")

    # 写回文件
    try:
        with open(idl_file, 'w') as f:
            f.write(new_idl_content)
            f.flush()
        print(f"成功写入IDL文件: {idl_file}")
    except Exception as e:
        print(f"写入文件时发生错误: {e}")
    
    # 服务端函数写入rpc_server.c
    server_file = f"{proname}_rpc_server.c"
    with open(server_file, 'a') as f:
        f.write("\n/*\n")
        f.write(" * ⚠️  警告：全局变量自动化封装的限制 ⚠️\n")
        f.write(" * \n")
        f.write(" * 以下代码由自动化工具生成，可能存在以下问题：\n")
        f.write(" * 1. 语义一致性：原子性内存访问被转换为多步骤操作\n")
        f.write(" * 2. 初始化依赖：无法自动处理变量间的依赖关系\n")
        f.write(" * 3. 错误处理：缺少分布式环境下的异常处理逻辑\n")
        f.write(" * 4. 性能开销：每次访问都涉及网络通信\n")
        f.write(" * \n")
        f.write(" * 需要人工审查的要点：\n")
        f.write(" * - 验证初始化函数的分布式部署策略\n")
        f.write(" * - 添加必要的错误恢复机制\n")
        f.write(" * - 检查变量间的逻辑依赖关系\n")
        f.write(" * - 考虑批量操作以减少网络开销\n")
        f.write(" */\n")
        f.write("\n/* 全局变量访问函数 - 服务端 */\n")
        for func in server_functions:
            f.write(func)
    
    # Wrapper函数写入wrapper.c
    wrapper_file = f"{proname}_rpc_wrapper.c"
    with open(wrapper_file, 'a') as f:
        f.write("\n/*\n")
        f.write(" * ⚠️  警告：全局变量客户端包装器的使用注意事项 ⚠️\n")
        f.write(" * \n")
        f.write(" * 1. 性能影响：每次变量访问都会触发RPC调用\n")
        f.write(" * 2. 错误处理：网络异常可能导致程序行为不一致\n")
        f.write(" * 3. 状态同步：多个函数同时访问可能出现竞态条件\n")
        f.write(" * 4. 内存管理：动态分配的内存需要正确释放\n")
        f.write(" * \n")
        f.write(" * 建议的优化措施：\n")
        f.write(" * - 缓存频繁访问的变量值\n")
        f.write(" * - 批量处理多个变量操作\n")
        f.write(" * - 添加超时和重试机制\n")
        f.write(" * - 实现本地缓存失效策略\n")
        f.write(" */\n")
        f.write("\n/* 全局变量访问函数 - 客户端Wrapper */\n")
        for func in wrapper_functions:
            f.write(func)
    
    # 在头文件中添加函数声明
    header_file = f"{proname}_rpc_wrapper.h"
    with open(header_file, 'a') as f:
        f.write("\n/*\n")
        f.write(" * 全局变量RPC访问接口声明\n")
        f.write(" * \n")
        f.write(" * 注意：这些函数替代了原有的直接内存访问，\n")
        f.write(" * 调用时需要考虑网络延迟和错误处理\n")
        f.write(" */\n")
        f.write("\n/* 全局变量访问函数声明 */\n")
        for func in wrapper_header_functions:
            f.write(func)
    
    # 重新生成RPC文件
    os.system(f"rpcgen -N {idl_file}")

def collect_global_var_asts(file_ast_dict):
    """
    Collects all global variable AST nodes from the provided file AST dictionary.
    This function parses the abstract syntax trees (ASTs) of C code files and
    identifies global variable declarations (variables declared outside of any function).
    Args:
        file_ast_dict (dict): Dictionary mapping filenames to their corresponding AST objects.
                             Each AST object represents the parsed structure of a C file.
    Returns:
        dict: A dictionary mapping global variable names to a dictionary containing: key is the variable name, value is a dictionary containing:
              - 'ast': The AST node representing the variable declaration
              - 'file': The filename where the variable is defined
    Example:
        file_ast_dict = {'main.c': ast_object_for_main, 'utils.c': ast_object_for_utils}
        globals = collect_global_var_asts(file_ast_dict)
        # Returns something like {'count': {'ast': <AST node>, 'file': 'main.c'}, ...}
    """
    
    global_vars = {}
    
    for filename, ast in file_ast_dict.items():
        for node in ast.ext:
            # 检查是否是全局变量声明
            if isinstance(node, c_ast.Decl) and not isinstance(node.type, c_ast.FuncDecl):
                var_name = node.name
                # if var_name is None and "/home/raoxue/Desktop/IF-driver-partition/code_refactor/fake_libc_include/_fake_typedefs.h" not in str(node.coord):
                #     print(node.coord)
                #     print("None 全局变量：", node)
                if var_name is not None:
                    global_vars[var_name] = {
                        'ast': node,
                        'file': filename
                    }
    
    return global_vars

gsetter_fun_str_list=[]
gsetter_fun_have_def=[]
gsetter_idl_str_list=[]

def replace_global_var_access_with_rpc(proname, global_vars_set):
    
    """
    Replaces direct access to sensitive global variables in non-sensitive domains with RPC getter/setter calls.
    This function identifies all instances where non-sensitive domain functions directly access 
    sensitive global variables, and replaces these accesses with appropriate RPC wrapper calls.
    It handles both simple variables and complex structures, including nested struct members.
    For global variable reads, it substitutes get_X_wrapper() calls.
    For global variable writes, it substitutes set_X_wrapper(value) calls.
    For struct field access, it creates specialized getters/setters for each field.
    Args:
        proname (str): The project name being processed
        global_vars_set (set): Set of sensitive global variables to be replaced (D_sense_global_var)
    Returns:
        list: Information about modified functions, where each element is a tuple containing:
            (function_name, file_name, replacement_count, replacement_details)
    Example:
        >>> modified_functions = replace_global_var_access_with_rpc("my_project", sensitive_vars)
        >>> print(f"Modified {len(modified_functions)} functions with RPC wrappers")
        """
    """
    修改非敏感域中对敏感全局变量的直接访问，替换为RPC getter/setter调用
    
    参数:
        proname: 项目名称
        global_vars_set: 需要处理的全局变量集合 (D_sense_global_var)
    返回:
        被修改的函数信息列表，每个元素包含 (函数名, 文件名, 替换次数, 替换详情)
    """
    # 准备用于替换的 getter 和 setter 函数名字典
    var_to_getter = {var: f"get_{var}_wrapper()" for var in global_vars_set}
    var_to_setter = {var: f"set_{var}_wrapper" for var in global_vars_set}
    
    
    print("replace_global_var_access_with_rpc start:")
    # for gvar,gast in global_var_asts.items():
    #     print(gvar)
    #     print(gast)

   

    # 收集结构体类型的全局变量信息
    struct_vars = {}

    def collect_struct_members(struct_def, prefix="", var_name_for_anon=None, processed_types=None):
        """递归收集结构体成员信息，包括嵌套结构体和匿名结构体"""
        # 初始化已处理类型集合，避免循环递归
        if processed_types is None:
            processed_types = set()
            
        if not struct_def or not hasattr(struct_def, 'decls') or not struct_def.decls:
            return {}
        
        # 获取结构体名称用于循环检测
        if hasattr(struct_def, 'name') and struct_def.name:
            struct_name = struct_def.name
        elif var_name_for_anon:
            struct_name = f"anon_{var_name_for_anon}_{id(struct_def)}"
        else:
            struct_name = f"anon_{id(struct_def)}"
            
        # 检查是否已处理过该类型，避免循环
        if struct_name in processed_types:
            return {}
        
        processed_types.add(struct_name)
        
        members = {}
        for member in struct_def.decls:
            if not hasattr(member, 'name') or not member.name:
                continue
                
            member_name = member.name
            full_path = f"{prefix}{member_name}" if prefix else member_name
            members[full_path] = True
            
            # 检查成员是否也是结构体
            member_type = member.type
            is_nested_struct = False
            nested_struct_name = None
            nested_struct_def = None
            
            # 处理非指针结构体成员
            if isinstance(member_type, c_ast.TypeDecl):
                # 处理匿名结构体 (没有名称的结构体)
                if isinstance(member_type.type, c_ast.Struct) and member_type.type.name is None:
                    # 匿名结构体，使用 new_{var_name}_{member_name} 作为名称
                    if var_name_for_anon:
                        is_nested_struct = True
                        nested_struct_name = f"new_{var_name_for_anon}_{member_name}"
                        nested_struct_def = member_type.type
                        print(f"发现匿名结构体成员 {member_name}，命名为 {nested_struct_name}")
                
                # 处理命名结构体
                elif hasattr(member_type.type, 'name'):
                    type_str = member_type.type.name
                    if type_str and (type_str.startswith('struct ') or 
                                type_str in struct_dict or 
                                (type_str in typedef_dict and typedef_dict[type_str]['isstruct'])):
                        is_nested_struct = True
                        nested_struct_name = type_str.split(' ')[-1] if type_str.startswith('struct ') else type_str
                    elif type_str and type_str in typedef_dict and typedef_dict[type_str]['isstruct']:
                        is_nested_struct = True
                        nested_struct_name = type_str
            
            # 处理指针到结构体的成员
            elif isinstance(member_type, c_ast.PtrDecl):
                # 获取指针指向的类型
                ptr_type = member_type.type
                
                # 处理指向匿名结构体的指针
                if isinstance(ptr_type, c_ast.TypeDecl) and isinstance(ptr_type.type, c_ast.Struct) and ptr_type.type.name is None:
                    # 匿名结构体指针，使用 new_{var_name}_{member_name} 作为名称
                    if var_name_for_anon:
                        is_nested_struct = True
                        nested_struct_name = f"new_{var_name_for_anon}_{member_name}"
                        nested_struct_def = ptr_type.type
                        print(f"发现匿名结构体指针成员 {member_name}，命名为 {nested_struct_name}")
                
                # 处理指向命名结构体的指针
                elif (isinstance(ptr_type, c_ast.TypeDecl) and 
                      hasattr(ptr_type.type, 'name')):
                    
                    type_str = ptr_type.type.name
                    if type_str and (type_str.startswith('struct ') or 
                                type_str in struct_dict or 
                                (type_str in typedef_dict and typedef_dict[type_str]['isstruct'])):
                        is_nested_struct = True
                        nested_struct_name = type_str.split(' ')[-1] if type_str.startswith('struct ') else type_str
                    elif type_str and type_str in typedef_dict and typedef_dict[type_str]['isstruct']:
                        is_nested_struct = True
                        nested_struct_name = type_str
            
            # 递归处理嵌套结构体
            if is_nested_struct and nested_struct_name:
                # 如果还没有获取到结构体定义，尝试从字典中获取
                if nested_struct_def is None:
                    if nested_struct_name in struct_dict:
                        nested_struct_def = struct_dict[nested_struct_name]['ast']
                    elif nested_struct_name in typedef_dict and typedef_dict[nested_struct_name]['isstruct']:
                        nested_struct_def = typedef_dict[nested_struct_name]['ast']
                
                if nested_struct_def:
                    print(c_generator.CGenerator().visit(nested_struct_def))
                    # 避免循环引用导致的无限递归
                    if (not hasattr(struct_def, 'name') or 
                        nested_struct_name != struct_def.name):
                        # 递归收集嵌套结构体的成员
                        nested_members = collect_struct_members(
                            nested_struct_def, 
                            f"{full_path}." if not isinstance(member_type, c_ast.PtrDecl) else f"{full_path}->",
                            var_name_for_anon,  # 传递原始变量名用于匿名结构体命名
                            processed_types  # 传递已处理类型集合
                        )
                        members.update(nested_members)
        
        return members
    #===========================================================================
    for var_name in global_vars_set:
        print(f"处理全局变量: {var_name}")
        if var_name in global_var_asts:
            var_info = global_var_asts[var_name]
            var_node = var_info['ast']
            var_type = var_node.type
            print(c_generator.CGenerator().visit(var_node))
            # if var_name=="limit_data":
            #     print(var_node)
            
            # 检查是否是结构体类型
            is_struct = False
            struct_name = None
            is_anonymous = False

            typename=generator.visit(var_type)

            # 处理匿名结构体变量
            if isinstance(var_type, c_ast.TypeDecl) and isinstance(var_type.type, c_ast.Struct) and var_type.type.name is None:
                # 匿名结构体，使用 new_{var_name} 作为名称
                is_struct = True
                is_anonymous = True
                struct_name = f"new_{var_name}"
                print(f"发现匿名结构体全局变量 {var_name}，命名为 {struct_name}")
            
            # 处理普通结构体变量
            elif isinstance(var_type, c_ast.TypeDecl) and hasattr(var_type.type, 'name'):
                type_str = var_type.type.name
                if type_str and (type_str.startswith('struct ') or 
                            type_str in struct_dict or 
                            (type_str in typedef_dict and typedef_dict[type_str]['isstruct'])):
                    is_struct = True
                    struct_name = type_str.split(' ')[-1] if type_str.startswith('struct ') else type_str
                elif type_str and type_str in typedef_dict and typedef_dict[type_str]['isstruct']:
                    is_struct = True
                    struct_name = type_str
            elif typename in typedef_dict and typedef_dict[typename]['isstruct']:
                struct_name = typename
                is_struct = True

            # 处理指向结构体的指针
            elif isinstance(var_type, c_ast.PtrDecl):
                # 处理指向匿名结构体的指针
                if (isinstance(var_type.type, c_ast.TypeDecl) and 
                    isinstance(var_type.type.type, c_ast.Struct) and 
                    var_type.type.type.name is None):
                    # 匿名结构体指针，使用 new_{var_name} 作为名称
                    is_struct = True
                    is_anonymous = True
                    struct_name = f"new_{var_name}"
                    print(f"发现匿名结构体指针全局变量 {var_name}，命名为 {struct_name}")
                
                # 尝试处理 type.type.name 情况
                elif hasattr(var_type.type, 'type') and hasattr(var_type.type.type, 'name'):
                    type_str = var_type.type.type.name
                    if type_str and (type_str.startswith('struct ') or
                                type_str in struct_dict or
                                (type_str in typedef_dict and typedef_dict[type_str]['isstruct'])):
                        is_struct = True
                        struct_name = type_str.split(' ')[-1] if type_str.startswith('struct ') else type_str
                    elif type_str and type_str in typedef_dict and typedef_dict[type_str]['isstruct']:
                        is_struct = True
                        struct_name = type_str
                # 尝试处理 type.type.names[0] 情况
                elif hasattr(var_type.type, 'type') and hasattr(var_type.type.type,'names'):
                    type_str = var_type.type.type.names[0]
                    if type_str and (type_str.startswith('struct ') or
                                type_str in struct_dict or
                                (type_str in typedef_dict and typedef_dict[type_str]['isstruct'])):
                        is_struct = True
                        struct_name = type_str.split(' ')[-1] if type_str.startswith('struct ') else type_str
                    elif type_str and type_str in typedef_dict and typedef_dict[type_str]['isstruct']:
                        is_struct = True
                        struct_name = type_str
            
            if is_struct:
                # 获取结构体定义
                struct_def = None
                
                # 处理匿名结构体
                if is_anonymous:
                    # 直接从变量类型中获取结构体定义
                    if isinstance(var_type, c_ast.TypeDecl) and isinstance(var_type.type, c_ast.Struct):
                        struct_def = var_type.type
                    elif isinstance(var_type, c_ast.PtrDecl) and isinstance(var_type.type, c_ast.TypeDecl) and isinstance(var_type.type.type, c_ast.Struct):
                        struct_def = var_type.type.type
                    
                    print(f"找到匿名结构体类型的全局变量 {var_name}，命名为 {struct_name}")
                
                # 处理命名结构体
                else:
                    if struct_name in struct_dict:
                        struct_def = struct_dict[struct_name]['ast']
                    elif struct_name in typedef_dict and typedef_dict[struct_name]['isstruct']:
                        struct_def = typedef_dict[struct_name]['ast']
                    
                    print(f"找到结构体类型 {struct_name} 的全局变量 {var_name}")
                
                if struct_def:
                    print(c_generator.CGenerator().visit(struct_def))
                    # if struct_name=="acc_t" or var_name=="limit_data":
                    #     print(struct_def)
                    
                    # 获取直接成员
                    direct_members = {}
                    if hasattr(struct_def, 'decls') and struct_def.decls:
                        direct_members = {member.name: True for member in struct_def.decls if hasattr(member, 'name')}
                    elif hasattr(struct_def, 'type') and hasattr(struct_def.type, 'type') and hasattr(struct_def.type.type, 'decls') and struct_def.type.type.decls:
                        direct_members = {member.name: True for member in struct_def.type.type.decls if hasattr(member, 'name')}
                    
                    # 递归收集所有嵌套成员（包括匿名结构体成员）
                    all_members = collect_struct_members(struct_def, var_name_for_anon=var_name, processed_types=None)
                    
                    # 结合直接成员和嵌套成员
                    all_members.update(direct_members)
                    
                    # 添加结构体成员信息
                    struct_vars[var_name] = {
                        'struct_name': struct_name,
                        'members': all_members,
                        'is_pointer': isinstance(var_type, c_ast.PtrDecl),
                        'is_anonymous': is_anonymous
                    }
                    
                    print(f"收集到的成员: {list(all_members.keys())}")
                else:
                    print(f"警告：无法获取结构体定义 {struct_name}")
                    
    
    
    # 复制 AST，避免对敏感域的修改
    nonsense_domain_file_ast_dict = {}
    for filename, ast in file_ast_dict.items():
        # 深拷贝 AST
        nonsense_domain_file_ast_dict[filename] = copy.deepcopy(ast)
    
    # 全局变量访问替换访问器类
    class GlobalVarReplacer(c_ast.NodeVisitor):
        def __init__(self, global_vars, struct_vars, var_to_getter, var_to_setter, in_nonsense_func=False):
            self.global_vars = global_vars  # 要替换的全局变量集合
            self.struct_vars = struct_vars  # 结构体类型的全局变量信息
            self.var_to_getter = var_to_getter  # 变量名到getter的映射
            self.var_to_setter = var_to_setter  # 变量名到setter的映射
            self.current_func = None  # 当前正在访问的函数
            self.current_file = None  # 当前处理的文件
            self.in_nonsense_func = in_nonsense_func  # 是否在非敏感函数中
            self.changes = []  # 要进行的替换列表 [(node, replacement), ...]
            self.lvalue_ids = set()  # 作为左值的ID节点集合
            self.struct_field_access = {}  # 记录结构体字段访问信息 {id(node): (var_name, field_name)}
            self.func_replacements = {}  # 记录每个函数的替换信息: {函数名: [(变量名, 操作类型, 行号), ...]}
            self.processed_nodes = set()  # 已处理过的节点ID，避免重复处理
            self.function_counter=1000
            
            self.parent_map = {}
            # 函数调用修改信息，用于后处理
            self.func_call_modifications = []  # [(func_call_node, temp_vars, args_indices)]
            # 添加一个新的记录，专门用于跟踪条件语句和循环语句中的全局变量引用
            self.control_flow_replacements = []

        
            
        def visit_FuncDef(self, node):
            old_func = self.current_func
            old_in_nonsense = self.in_nonsense_func
            
            self.current_func = node.decl.name
            self.in_nonsense_func = self.current_func in nonsense_domain
            
            # 只在非敏感域函数中执行替换
            if self.in_nonsense_func:
                self.generic_visit(node)
            
            self.current_func = old_func
            self.in_nonsense_func = old_in_nonsense
        
        def visit_StructRef(self, node):
            """
            处理结构体字段引用，识别对敏感全局变量的结构体成员的访问
            例如：some_struct.field 或 some_struct->field
            """
            if not self.in_nonsense_func or not self.current_func:
                return self.generic_visit(node)
                
            # 检查是否访问全局结构体变量的字段
            if isinstance(node.name, c_ast.ID) and node.name.name in self.global_vars:
                # 记录结构体字段访问
                var_name = node.name.name
                field_name = node.field.name if hasattr(node.field, 'name') else None
                
                if field_name and field_name in self.struct_vars[var_name]['members']:
                    # 记录这是一个结构体字段访问
                    self.struct_field_access[id(node)] = (var_name, field_name)
                    
                    # 如果是赋值左侧，会在visit_Assignment中处理
                    parent = self.get_parent(node)
                    if isinstance(parent, c_ast.Assignment) and parent.lvalue == node:
                        # 标记为已处理，避免重复替换
                        self.processed_nodes.add(id(node))
                    else:
                        # 生成getter函数调用,若访问opt.cert_file，则get_opt_cert_file_wrapper()
                        getter_name = f"get_{var_name}_{field_name}_wrapper"
                        print(f"替换结构体字段访问: {c_generator.CGenerator().visit(node)} -> {getter_name}")
                        self.changes.append((node, c_ast.FuncCall(name=c_ast.ID(name=getter_name), args=c_ast.ExprList(exprs=[]))))
                        # 记录替换信息
                        line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                        if self.current_func not in self.func_replacements:
                            self.func_replacements[self.current_func] = []
                        self.func_replacements[self.current_func].append((
                            var_name, field_name,
                            "getter_call",
                            line_no
                        ))

            # 继续遍历子节点
            self.generic_visit(node)

        def visit_FuncCall(self, node):
            """
            处理函数调用，仅识别需要修改的部分，不在遍历过程中修改AST
            """
            # print("处理函数调用语句:", c_generator.CGenerator().visit(node))
            if not self.in_nonsense_func or not self.current_func:
                return self.generic_visit(node)
            
            # 检查是否是调用敏感函数（以_sense_1结尾的函数）
            if isinstance(node.name, c_ast.ID) and node.name.name.endswith("_sense_1"):
                # 记录需要替换为wrapper函数调用
                original_name = node.name.name
                wrapper_name = original_name + "_wrapper"
                
                replacement = c_ast.FuncCall(
                    name=c_ast.ID(name=wrapper_name),
                    args=node.args
                )
                self.changes.append((node, replacement))
                
                # 记录替换信息
                line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                if self.current_func not in self.func_replacements:
                    self.func_replacements[self.current_func] = []
                self.func_replacements[self.current_func].append((
                    original_name, 
                    "wrapper_call", 
                    line_no
                ))
                
                return self.generic_visit(node)

            # 检查函数参数中是否有全局变量的引用
            if node.args:
                global_var_refs = []  # 存储需要特殊处理的全局变量引用
                
                # 遍历所有参数，查找 '&全局变量' 形式的参数
                for i, arg in enumerate(node.args.exprs):
                    if isinstance(arg, c_ast.UnaryOp) and arg.op == '&' and isinstance(arg.expr, c_ast.ID):
                        var_name = arg.expr.name
                        if var_name in self.global_vars:
                            global_var_refs.append((i, var_name))
                
                # 如果找到了全局变量引用作为参数
                if global_var_refs:
                    # 分析上下文，判断该函数调用是否是条件语句或循环语句的一部分，或者是赋值语句的右值
                    callee_funname = node.name.name if isinstance(node.name, c_ast.ID) else "unknown"
                    if callee_funname in sense_domain:
                        print(f"警告: 函数 {callee_funname} 在非敏感域中调用了敏感函数，可能需要进一步处理")
                        print(c_generator.CGenerator().visit(node))
                    else:
                        print(f"函数调用: {callee_funname} 在非敏感域中，可能需要替换为RPC调用")
                    parent = self.get_parent(node)
                    print(f"调用所在文件和行号: {self.current_file}, {node.coord.line if hasattr(node.coord, 'line') else 'unknown'}")
                    
                    # 检查是否是赋值语句的右值 - 更全面的检查
                    is_assignment_rhs = False
                    if isinstance(parent, c_ast.Assignment) and c_generator.CGenerator().visit(parent.rvalue) == c_generator.CGenerator().visit(node):
                        is_assignment_rhs = True
                    # 检查复杂赋值语句，比如嵌套在表达式内的函数调用
                    elif isinstance(parent, c_ast.BinaryOp) or isinstance(parent, c_ast.UnaryOp):
                        # 向上追溯几层以找到可能的赋值语句
                        ancestor = parent
                        for _ in range(3):  # 限制查找深度
                            ancestor_parent = self.get_parent(ancestor)
                            if ancestor_parent is None:
                                break
                            if isinstance(ancestor_parent, c_ast.Assignment):
                                is_assignment_rhs = True
                                parent = ancestor_parent  # 更新父节点为真实赋值语句
                                break
                            if isinstance(ancestor_parent, (c_ast.While, c_ast.DoWhile, c_ast.If, c_ast.For)):
                                parent = ancestor_parent  # 更新父节点为循环或条件语句
                                break
                            ancestor = ancestor_parent
                    # print("是否是赋值语句的右值:", is_assignment_rhs)
                    
                    # 记录函数调用上下文信息，用于后处理
                    context_info = {
                        "is_while_cond": isinstance(parent, c_ast.While) and parent.cond == node,
                        "is_dowhile_cond": isinstance(parent, c_ast.DoWhile) and parent.cond == node,
                        "is_if_cond": isinstance(parent, c_ast.If) and node in parent.cond,
                        "is_for_cond": isinstance(parent, c_ast.For) and parent.cond == node,
                        "is_assignment_rhs": is_assignment_rhs,
                        "parent": parent
                    }
                    """输出context_info为True的键值对"""
                    new_context_info = {k: v for k, v in context_info.items() if v}
                    if new_context_info:
                        print("函数调用上下文信息 (context_info):")
                        print(new_context_info)
                    else:
                        print("函数调用上下文信息 (context_info) 中没有为True的键值对")

                    

                    

                    
                    # 保存函数调用信息，包括上下文信息，用于后处理
                    self.func_call_modifications.append((node, global_var_refs, context_info))
                    
                    # 记录替换信息
                    line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                    for i, var_name in global_var_refs:
                        if self.current_func not in self.func_replacements:
                            self.func_replacements[self.current_func] = []
                        self.func_replacements[self.current_func].append((
                            var_name,
                            "reference_param",
                            line_no
                        ))
            
            # 继续遍历其他子节点
            return self.generic_visit(node)
            
        
        def visit_Assignment(self, node):
            """处理赋值语句，对全局变量访问替换为setter调用"""
            if not self.in_nonsense_func or not self.current_func:
                return self.generic_visit(node)
            

            # # 处理右值是结构体字段后缀自增/自减的情况
            # if (isinstance(node.rvalue, c_ast.UnaryOp) and 
            #     node.rvalue.op in ['p++', 'p--'] and 
            #     isinstance(node.rvalue.expr, c_ast.StructRef) and 
            #     id(node.rvalue.expr) in self.struct_field_access):
                
            #     # 获取结构体字段信息
            #     struct_var_name, field_name = self.struct_field_access[id(node.rvalue.expr)]
            #     op_type = '+' if node.rvalue.op == 'p++' else '-'
                
            #     # 创建替换语句块
            #     statements = []
                
            #     # 1. 获取结构体字段的原始值到临时变量
            #     temp_field_var = f"temp_{struct_var_name}_{field_name}"
            #     field_getter_name = f"get_{struct_var_name}_{field_name}_wrapper"
                
            #     # 添加临时变量声明和getter调用
            #     get_field_call = c_ast.FuncCall(
            #         name=c_ast.ID(name=field_getter_name),
            #         args=c_ast.ExprList(exprs=[])
            #     )
            #     statements.append(c_ast.Decl(
            #         name=temp_field_var,
            #         quals=[],
            #         storage=[],
            #         funcspec=[],
            #         align=[],
            #         type=c_ast.TypeDecl(
            #             declname=temp_field_var,
            #             quals=[],
            #             align=[],
            #             type=c_ast.IdentifierType(names=["int"])  # 假设是整型
            #         ),
            #         init=get_field_call,
            #         bitsize=None
            #     ))
                
            #     # 2. 保存原值，用于赋值给左侧
            #     temp_result_var = f"temp_{struct_var_name}_{field_name}_result"
            #     statements.append(c_ast.Decl(
            #         name=temp_result_var,
            #         quals=[],
            #         storage=[],
            #         funcspec=[],
            #         align=[],
            #         type=c_ast.TypeDecl(
            #             declname=temp_result_var,
            #             quals=[],
            #             align=[],
            #             type=c_ast.IdentifierType(names=["int"])
            #         ),
            #         init=c_ast.ID(name=temp_field_var),
            #         bitsize=None
            #     ))
                
            #     # 3. 执行自增/自减操作
            #     calc_expr = c_ast.BinaryOp(
            #         op=op_type, 
            #         left=c_ast.ID(name=temp_field_var),
            #         right=c_ast.Constant(type='int', value='1')
            #     )
            #     statements.append(c_ast.Assignment(
            #         op='=',
            #         lvalue=c_ast.ID(name=temp_field_var),
            #         rvalue=calc_expr
            #     ))
                
            #     # 4. 更新结构体字段值 (通过setter)
            #     field_setter_name = f"set_{struct_var_name}_{field_name}_wrapper"
            #     field_set_call = c_ast.FuncCall(
            #         name=c_ast.ID(name=field_setter_name),
            #         args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_field_var)])
            #     )
            #     statements.append(field_set_call)
                
            #     # 5. 根据左值类型，添加相应的赋值操作
            #     if isinstance(node.lvalue, c_ast.ID) and node.lvalue.name in self.global_vars:
            #         # 左值是普通全局变量，添加setter调用
            #         lvalue_setter_name = self.var_to_setter[node.lvalue.name]
            #         lvalue_setter_call = c_ast.FuncCall(
            #             name=c_ast.ID(name=lvalue_setter_name),
            #             args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_result_var)])
            #         )
            #         statements.append(lvalue_setter_call)
                    
            #     elif isinstance(node.lvalue, c_ast.StructRef) and id(node.lvalue) in self.struct_field_access:
            #         # 左值是结构体字段，添加专用setter
            #         lvalue_var_name, lvalue_field_name = self.struct_field_access[id(node.lvalue)]
            #         lvalue_setter_name = f"set_{lvalue_var_name}_{lvalue_field_name}_wrapper"
            #         lvalue_setter_call = c_ast.FuncCall(
            #             name=c_ast.ID(name=lvalue_setter_name),
            #             args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_result_var)])
            #         )
            #         statements.append(lvalue_setter_call)
            #         self.processed_nodes.add(id(node.lvalue))
                    
            #     else:
            #         # 普通变量赋值
            #         statements.append(c_ast.Assignment(
            #             op='=',
            #             lvalue=node.lvalue,
            #             rvalue=c_ast.ID(name=temp_result_var)
            #         ))
                
            #     # 记录替换信息
            #     line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
            #     if self.current_func not in self.func_replacements:
            #         self.func_replacements[self.current_func] = []
            #     self.func_replacements[self.current_func].append((
            #         f"{struct_var_name}.{field_name}", 
            #         "post_inc_dec_and_assign", 
            #         line_no
            #     ))
                
            #     # 创建复合语句块替换原赋值语句
            #     replacement = c_ast.Compound(block_items=statements)
            #     self.changes.append((node, replacement))
            #     return

            # 处理结构体字段赋值 (netoring.supply = 255)
            if isinstance(node.lvalue, c_ast.StructRef):
                if isinstance(node.lvalue.name, c_ast.ID) and node.lvalue.name.name in self.global_vars:
                    print("处理结构体字段赋值:", c_generator.CGenerator().visit(node))
                    print(node)
                    # 直接字段访问模式: struct.field = value
                    struct_var_name = node.lvalue.name.name
                    field_name = node.lvalue.field.name if hasattr(node.lvalue.field, 'name') else None
                    
                    if field_name:
                        # 使用专门的字段setter进行替换
                        setter_name = f"set_{struct_var_name}_{field_name}_wrapper"
                        
                        # 创建替换的函数调用
                        replacement = c_ast.FuncCall(
                            name=c_ast.ID(name=setter_name),
                            args=c_ast.ExprList(exprs=[node.rvalue])
                        )
                        self.changes.append((node, replacement))
                        self.processed_nodes.add(id(node.lvalue))
                        
                        # 记录替换信息
                        line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                        if self.current_func not in self.func_replacements:
                            self.func_replacements[self.current_func] = []
                        self.func_replacements[self.current_func].append((
                            f"{struct_var_name}.{field_name}", 
                            "direct_struct_field_setter", 
                            line_no
                        ))
                        return
                # 处理间接结构体访问 (id(node.lvalue) in self.struct_field_access 的情况)
                elif id(node.lvalue) in self.struct_field_access:
                    var_name, field_name = self.struct_field_access[id(node.lvalue)]
                    # 使用结构体字段专门的setter
                    setter_name = f"set_{var_name}_{field_name}_wrapper"
                    
                    # 创建一个新的FuncCall节点来替换整个赋值语句
                    replacement = c_ast.FuncCall(
                        name=c_ast.ID(name=setter_name),
                        args=c_ast.ExprList(exprs=[node.rvalue])
                    )
                    self.changes.append((node, replacement))
                    self.processed_nodes.add(id(node.lvalue))
                    
                    # 记录替换信息
                    line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                    if self.current_func not in self.func_replacements:
                        self.func_replacements[self.current_func] = []
                    self.func_replacements[self.current_func].append((
                        f"{var_name}.{field_name}", 
                        "struct_field_setter", 
                        line_no
                    ))
                    return
            
            # 处理普通全局变量赋值
            if isinstance(node.lvalue, c_ast.ID) and node.lvalue.name in self.global_vars:
                # 左值是全局变量，替换为setter调用
                setter_name = self.var_to_setter[node.lvalue.name]
                # 保存这个ID节点，标记为左值
                self.lvalue_ids.add(id(node.lvalue))
                
                # 创建一个新的FuncCall节点来替换整个赋值语句
                replacement = c_ast.FuncCall(
                    name=c_ast.ID(name=setter_name),
                    args=c_ast.ExprList(exprs=[node.rvalue])
                )
                self.changes.append((node, replacement))
                
                # 记录替换信息
                line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                if self.current_func not in self.func_replacements:
                    self.func_replacements[self.current_func] = []
                self.func_replacements[self.current_func].append((
                    node.lvalue.name, 
                    "setter", 
                    line_no
                ))
            
            # 处理结构体字段赋值
            elif isinstance(node.lvalue, c_ast.StructRef) and id(node.lvalue) in self.struct_field_access:
                var_name, field_name = self.struct_field_access[id(node.lvalue)]
                # 使用结构体字段专门的setter
                setter_name = f"set_{var_name}_{field_name}_wrapper"
                
                # 创建一个新的FuncCall节点来替换整个赋值语句
                replacement = c_ast.FuncCall(
                    name=c_ast.ID(name=setter_name),
                    args=c_ast.ExprList(exprs=[node.rvalue])
                )
                self.changes.append((node, replacement))
                self.processed_nodes.add(id(node.lvalue))
                
                # 记录替换信息
                line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                if self.current_func not in self.func_replacements:
                    self.func_replacements[self.current_func] = []
                self.func_replacements[self.current_func].append((
                    f"{var_name}.{field_name}", 
                    "struct_field_setter", 
                    line_no
                ))

            return self.generic_visit(node)

            # # 处理右值中的自增自减操作
            # if isinstance(node.rvalue, c_ast.UnaryOp) and node.rvalue.op in ['++', '--', 'p++', 'p--']:
            #     """输出一元操作符的类型"""
            #     print(f"处理自增自减操作: {node.rvalue.op} at {node.coord}")
            #     print(c_generator.CGenerator().visit(node))
            #     print(self.current_func)
            #     print(node.rvalue.expr)
                
            #     if (isinstance(node.rvalue.expr, c_ast.ID) and 
            #         node.rvalue.expr.name in self.global_vars):
            #         print(f"处理自增自减操作4: {node.rvalue.op} at {node.coord}")

            #         # 创建替换语句块
            #         statements = []
                    
            #         # 处理右值的自增自减
            #         var_name = node.rvalue.expr.name
            #         getter_name = f"get_{var_name}_wrapper"
            #         setter_name = f"set_{var_name}_wrapper"
                    
            #         # 确定是前缀还是后缀操作
            #         is_prefix = not node.rvalue.op.startswith('p')
            #         op_type = '+' if ('++' in node.rvalue.op) else '-'
                    
            #         # 1. 声明临时变量并获取全局变量当前值
            #         temp_var = f"temp_{var_name}_{self._get_unique_id()}"
            #         get_call = c_ast.FuncCall(
            #             name=c_ast.ID(name=getter_name),
            #             args=c_ast.ExprList(exprs=[])
            #         )
                    
            #         # 添加变量声明和getter调用
            #         statements.append(c_ast.Decl(
            #             name=temp_var,
            #             quals=[],
            #             storage=[],
            #             funcspec=[],
            #             align=[],
            #             type=c_ast.TypeDecl(
            #                 declname=temp_var,
            #                 quals=[],
            #                 align=[],
            #                 type=c_ast.IdentifierType(names=["int"])  # 假设是整型
            #             ),
            #             init=get_call,
            #             bitsize=None
            #         ))
                    
            #         # 2. 声明用于返回的临时变量
            #         # result_var = f"result_{var_name}_{self._get_unique_id()}"
                    
            #         # 前缀操作: 先修改值再返回
            #         if is_prefix:
            #             # 计算新值并保存在临时变量中
            #             calc_expr = c_ast.BinaryOp(
            #                 op=op_type, 
            #                 left=c_ast.ID(name=temp_var),
            #                 right=c_ast.Constant(type='int', value='1')
            #             )
                        
            #             # 更新临时变量
            #             statements.append(c_ast.Assignment(
            #                 op='=',
            #                 lvalue=c_ast.ID(name=temp_var),
            #                 rvalue=calc_expr
            #             ))
                        
            #             # 结果变量等于修改后的值
            #             # statements.append(c_ast.Decl(
            #             #     name=result_var,
            #             #     quals=[],
            #             #     storage=[],
            #             #     funcspec=[],
            #             #     align=[],
            #             #     type=c_ast.TypeDecl(
            #             #         declname=result_var,
            #             #         quals=[],
            #             #         align=[],
            #             #         type=c_ast.IdentifierType(names=["int"])
            #             #     ),
            #             #     init=c_ast.ID(name=temp_var),
            #             #     bitsize=None
            #             # ))
            #         else:
            #             # 后缀操作: 先保存当前值作为结果
            #             # statements.append(c_ast.Decl(
            #             #     name=result_var,
            #             #     quals=[],
            #             #     storage=[],
            #             #     funcspec=[],
            #             #     align=[],
            #             #     type=c_ast.TypeDecl(
            #             #         declname=result_var,
            #             #         quals=[],
            #             #         align=[],
            #             #         type=c_ast.IdentifierType(names=["int"])
            #             #     ),
            #             #     init=c_ast.ID(name=temp_var),
            #             #     bitsize=None
            #             # ))
                        
            #             # 然后修改临时变量
            #             calc_expr = c_ast.BinaryOp(
            #                 op=op_type,
            #                 left=c_ast.ID(name=temp_var),
            #                 right=c_ast.Constant(type='int', value='1')
            #             )
                        
            #             statements.append(c_ast.Assignment(
            #                 op='=',
            #                 lvalue=c_ast.ID(name=temp_var),
            #                 rvalue=calc_expr
            #             ))
                    
            #         # 3. 更新全局变量 (通过setter)
            #         set_call = c_ast.FuncCall(
            #             name=c_ast.ID(name=setter_name),
            #             args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var)])
            #         )
            #         statements.append(set_call)
                    
            #         # 4. 处理赋值语句左值
            #         if isinstance(node.lvalue, c_ast.ID) and node.lvalue.name in self.global_vars:
            #             # 左值是全局变量，添加setter调用
            #             lvalue_setter_name = self.var_to_setter[node.lvalue.name]
            #             lvalue_setter_call = c_ast.FuncCall(
            #                 name=c_ast.ID(name=lvalue_setter_name),
            #                 args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var)])
            #             )
            #             statements.append(lvalue_setter_call)
                        
            #             # 记录替换信息
            #             line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
            #             if self.current_func not in self.func_replacements:
            #                 self.func_replacements[self.current_func] = []
            #             self.func_replacements[self.current_func].append((
            #                 node.lvalue.name, 
            #                 "setter_with_inc_dec", 
            #                 line_no
            #             ))
            #         elif isinstance(node.lvalue, c_ast.StructRef) and id(node.lvalue) in self.struct_field_access:
            #             # 左值是结构体字段，添加专用setter
            #             lvalue_var_name, lvalue_field_name = self.struct_field_access[id(node.lvalue)]
            #             lvalue_setter_name = f"set_{lvalue_var_name}_{lvalue_field_name}_wrapper"
            #             lvalue_setter_call = c_ast.FuncCall(
            #                 name=c_ast.ID(name=lvalue_setter_name),
            #                 args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var)])
            #             )
            #             statements.append(lvalue_setter_call)
            #             self.processed_nodes.add(id(node.lvalue))
                        
            #             # 记录替换信息
            #             line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
            #             if self.current_func not in self.func_replacements:
            #                 self.func_replacements[self.current_func] = []
            #             self.func_replacements[self.current_func].append((
            #                 f"{lvalue_var_name}.{lvalue_field_name}", 
            #                 "struct_field_setter_with_inc_dec", 
            #                 line_no
            #             ))
            #         else:
            #             # 普通变量赋值
            #             statements.append(c_ast.Assignment(
            #                 op='=',
            #                 lvalue=node.lvalue,
            #                 rvalue=c_ast.ID(name=temp_var)
            #             ))
                    
            #         # 记录对右值全局变量的自增自减操作
            #         line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
            #         if self.current_func not in self.func_replacements:
            #             self.func_replacements[self.current_func] = []
            #         self.func_replacements[self.current_func].append((
            #             var_name, 
            #             f"{'prefix' if is_prefix else 'postfix'}_{'inc' if op_type == '+' else 'dec'}_in_rhs", 
            #             line_no
            #         ))
                    
            #         # 创建复合语句块替换原赋值语句
            #         replacement = c_ast.Compound(block_items=statements)
            #         self.changes.append((node, replacement))
            #         return
                
                # elif (isinstance(node.rvalue.expr, c_ast.StructRef) and
                #     id(node.rvalue.expr.name.name) in self.struct_field_access):
                #     # 处理结构体字段的自增自减
                #     struct_var_name, field_name = self.struct_field_access[id(node.rvalue.expr.name.name)]
                #     struct_field_path = f"{struct_var_name}.{field_name}.{node.rvalue.expr.field.name}"
                #     op_type = '+' if node.rvalue.op == 'p++' else '-'
                    
                #     print(f"处理结构体字段自增自减3: {struct_field_path} {node.rvalue.op} at {node.coord}")
                #     # print(c_generator.CGenerator().visit(node))
                #     # print(self.current_func)
                    
                #     # 创建替换语句块
                #     statements = []
                    
                #     # 1. 获取结构体字段当前值
                #     field_getter_name = f"get_{struct_var_name}_{field_name}_wrapper"
                #     temp_struct_var = f"temp_{struct_var_name}_{field_name}"
                    
                #     # 声明临时结构体变量并初始化
                #     statements.append(c_ast.Decl(
                #         name=temp_struct_var,
                #         quals=[],
                #         storage=[],
                #         funcspec=[],
                #         align=[],
                #         type=c_ast.TypeDecl(
                #             declname=temp_struct_var,
                #             quals=[],
                #             align=[],
                #             type=c_ast.IdentifierType(names=[self.struct_vars[struct_var_name]['struct_name']])
                #         ),
                #         init=c_ast.FuncCall(
                #             name=c_ast.ID(name=field_getter_name),
                #             args=c_ast.ExprList(exprs=[])
                #         ),
                #         bitsize=None
                #     ))
                    
                #     # 2. 构造字段访问表达式
                #     field_access = c_ast.StructRef(
                #         name=c_ast.ID(name=temp_struct_var),
                #         type=node.rvalue.expr.type,
                #         field=c_ast.ID(name=node.rvalue.expr.field.name)
                #     )
                    
                #     # 3. 为后缀操作创建结果变量，保存原始值
                #     temp_result_var = f"temp_result_{struct_var_name}_{field_name}_{self._get_unique_id()}"
                #     statements.append(c_ast.Decl(
                #         name=temp_result_var,
                #         quals=[],
                #         storage=[],
                #         funcspec=[],
                #         align=[],
                #         type=c_ast.TypeDecl(
                #             declname=temp_result_var,
                #             quals=[],
                #             align=[],
                #             type=c_ast.IdentifierType(names=["int"])  # 假设字段是整型
                #         ),
                #         init=field_access,
                #         bitsize=None
                #     ))
                    
                #     # 4. 更新字段值
                #     calc_expr = c_ast.BinaryOp(
                #         op=op_type,
                #         left=field_access,
                #         right=c_ast.Constant(type='int', value='1')
                #     )
                #     statements.append(c_ast.Assignment(
                #         op='=',
                #         lvalue=field_access,
                #         rvalue=calc_expr
                #     ))
                    
                #     # 5. 更新结构体
                #     setter_call = c_ast.FuncCall(
                #         name=c_ast.ID(name=f"set_{struct_var_name}_{field_name}_wrapper"),
                #         args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_struct_var)])
                #     )
                #     statements.append(setter_call)
                    
                #     # 6. 根据左值类型，添加相应的赋值操作
                #     if isinstance(node.lvalue, c_ast.ID) and node.lvalue.name in self.global_vars:
                #         # 左值是普通全局变量，添加setter调用
                #         lvalue_setter_name = self.var_to_setter[node.lvalue.name]
                #         lvalue_setter_call = c_ast.FuncCall(
                #             name=c_ast.ID(name=lvalue_setter_name),
                #             args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_result_var)])
                #         )
                #         statements.append(lvalue_setter_call)
                        
                #         # 记录替换信息
                #         line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                #         if self.current_func not in self.func_replacements:
                #             self.func_replacements[self.current_func] = []
                #         self.func_replacements[self.current_func].append((
                #             node.lvalue.name, 
                #             "setter_with_struct_field_inc_dec", 
                #             line_no
                #         ))
                #     elif isinstance(node.lvalue, c_ast.StructRef) and id(node.lvalue) in self.struct_field_access:
                #         # 左值是结构体字段，添加专用setter
                #         lvalue_var_name, lvalue_field_name = self.struct_field_access[id(node.lvalue)]
                #         lvalue_setter_name = f"set_{lvalue_var_name}_{lvalue_field_name}_wrapper"
                #         lvalue_setter_call = c_ast.FuncCall(
                #             name=c_ast.ID(name=lvalue_setter_name),
                #             args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_result_var)])
                #         )
                #         statements.append(lvalue_setter_call)
                #         self.processed_nodes.add(id(node.lvalue))
                        
                #         # 记录替换信息
                #         line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                #         if self.current_func not in self.func_replacements:
                #             self.func_replacements[self.current_func] = []
                #         self.func_replacements[self.current_func].append((
                #             f"{lvalue_var_name}.{lvalue_field_name}", 
                #             "struct_field_setter_with_struct_field_inc_dec", 
                #             line_no
                #         ))
                #     else:
                #         # 普通变量赋值
                #         statements.append(c_ast.Assignment(
                #             op='=',
                #             lvalue=node.lvalue,
                #             rvalue=c_ast.ID(name=temp_result_var)
                #         ))
                    
                #     # 记录对右值结构体字段的自增自减操作
                #     line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                #     if self.current_func not in self.func_replacements:
                #         self.func_replacements[self.current_func] = []
                #     self.func_replacements[self.current_func].append((
                #         struct_field_path, 
                #         f"struct_field_{'postfix'}_{'inc' if op_type == '+' else 'dec'}_in_rhs", 
                #         line_no
                #     ))
                    
                #     # 创建复合语句块替换原赋值语句
                #     replacement = c_ast.Compound(block_items=statements)
                #     self.changes.append((node, replacement))
                #     return
                    
                
            # 左值已经在上面处理过或标记为特殊处理，这里可能不需要再处理
        
        def visit_ID(self, node):
            """
            处理普通标识符引用，识别对全局变量的直接访问
            """
            if not self.in_nonsense_func or not self.current_func:
                return
                
            # 处理普通的变量引用
            if (node.name in self.global_vars and 
                id(node) not in self.lvalue_ids):  # 不是左值才替换为getter
                
                # 如果是结构体类型，需要使用整体getter
                if node.name in self.struct_vars:
                    # 检查是否是接下来要处理的结构体字段访问的一部分
                    # 在这种情况下，我们会在visit_StructRef中处理
                    # 获取父节点，检查是否是StructRef
                    is_part_of_structref = False
                    
                    # 通过分析AST上下文确定是否为结构体字段访问的一部分
                    # 这部分可能需要根据具体情况调整
                    
                    if not is_part_of_structref:
                        # 如果是单独使用结构体变量，则使用整体getter
                        getter_name = f"get_{node.name}_wrapper"
                        replacement = c_ast.FuncCall(
                            name=c_ast.ID(name=getter_name),
                            args=c_ast.ExprList(exprs=[])
                        )
                        self.changes.append((node, replacement))
                        
                        # 记录替换信息
                        line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                        if self.current_func not in self.func_replacements:
                            self.func_replacements[self.current_func] = []
                        self.func_replacements[self.current_func].append((
                            node.name, 
                            "struct_getter", 
                            line_no
                        ))
                else:
                    # 普通变量，替换为getter调用
                    getter_name = f"get_{node.name}_wrapper"
                    replacement = c_ast.FuncCall(
                        name=c_ast.ID(name=getter_name),
                        args=c_ast.ExprList(exprs=[])
                    )
                    self.changes.append((node, replacement))
                    
                    # 记录替换信息
                    line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                    if self.current_func not in self.func_replacements:
                        self.func_replacements[self.current_func] = []
                    self.func_replacements[self.current_func].append((
                        node.name, 
                        "getter", 
                        line_no
                    ))
        
        def post_visit_StructRef(self, node):
            """
            在后序遍历中处理结构体字段访问，替换为对应的getter
            """
            if not self.in_nonsense_func or not self.current_func:
                return
                
            # 如果这个StructRef是一个已处理过的节点（如作为赋值左值），跳过
            if id(node) in self.processed_nodes:
                return
                
            # 检查是否是对敏感结构体字段的访问
            if id(node) in self.struct_field_access:
                var_name, field_name = self.struct_field_access[id(node)]
                
                # 使用字段专门的getter
                getter_name = f"get_{var_name}_{field_name}_wrapper"
                replacement = c_ast.FuncCall(
                    name=c_ast.ID(name=getter_name),
                    args=c_ast.ExprList(exprs=[])
                )
                self.changes.append((node, replacement))
                
                # 记录替换信息
                line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                if self.current_func not in self.func_replacements:
                    self.func_replacements[self.current_func] = []
                self.func_replacements[self.current_func].append((
                    f"{var_name}.{field_name}", 
                    "struct_field_getter", 
                    line_no
                ))
        
        # def visit_UnaryOp(self, node):
        #     if isinstance(node.expr, c_ast.ID) and node.expr.name in self.global_vars:
        #         if self.current_func is None:
        #             print("当前函数未设置，无法处理一元操作符")
        #             return

        #         if node.op == '++' or node.op == '--' or node.op == '-=' or node.op == '+=' or \
        #         node.op == '*=' or node.op == '/=' or node.op == '%=' or node.op == 'p++' or node.op == 'p--' or node.op == '^=':
        #             print(f"处理一元操作符: {node.op} at {node.coord}")
        #             print(self.current_func)
        #             print(node)
        #             parent_node = self.get_parent(node)
        #             if parent_node:
        #                 print(c_generator.CGenerator().visit(parent_node))
        #                 print(parent_node)
        #             else:
        #                 print("没有父节点")
        #     # 继续遍历其他子节点
        #     return self.generic_visit(node)

        def visit_UnaryOp(self, node):
            """处理一元操作符，特别关注自增自减操作"""
            # 只处理非敏感函数中对敏感全局变量的操作
            if not self.in_nonsense_func or not self.current_func:
                return self.generic_visit(node)
            if node.op in ['++', '--', 'p++', 'p--', '+=', '-=', '*=', '/=', '%=', '^=']:
                
                # 处理前缀/后缀自增自减操作
                if isinstance(node.expr, c_ast.ID) :
                    if node.expr.name not in self.global_vars:
                        return
                
                    var_name = node.expr.name

                    getter_name = f"get_{var_name}_wrapper"
                    setter_name = f"set_{var_name}_wrapper"

                    
                
                    gval_type= self._infer_gval_type(var_name)
                    op_expr=c_generator.CGenerator().visit(node)
                    parent_expr=c_generator.CGenerator().visit(self.get_parent(node))
                    if gval_type =="int" or gval_type == "unsigned long" or gval_type == "long" or gval_type == "unsigned int":
                        print(f"处理自增自减操作1: {node.op} at {node.coord}")
                        print(f"变量类型: {gval_type}")
                        print(f"操作表达式: {op_expr}")
                        print(f"父表达式: {parent_expr}")

                        """
                        {gval_type} *gsetter_{var_name}_prefix_1_svc(struct svc_req *rqstp)
                            {
                                ++{var_name};
                                return &{var_name};
                            }
                        """
                        #请生成以上每条字符串，并存储到gsetter_fun_str_list中
                        if not node.op.startswith('p'):
                            newfunname= f"gsetter_{var_name}_prefix_1_svc"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                tempstr=f"{gval_type} *gsetter_{var_name}_prefix_1_svc(struct svc_req *rqstp)\n"
                                tempstr+=f"{{\n"
                                tempstr+=f"    ++{var_name};\n"
                                tempstr+=f"    return &{var_name};\n"
                                tempstr+=f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                server_functions.append(tempstr)


                            """
                            {gval_type} gsetter_{var_name}_prefix_wrapper()
                                {
                                    {gval_type} *result;
                                    result=gsetter_{var_name}_prefix_1(clnt);
                                    if (result == NULL) 
                                        clnt_perror(clnt, "call failed");
                                    return *result;
                                }
                            """
                            newfunname= f"gsetter_{var_name}_prefix_wrapper"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                tempstr=f"{gval_type} gsetter_{var_name}_prefix_wrapper()\n"
                                tempstr+=f"{{\n"
                                tempstr+=f"   {gval_type} *result;\n"
                                tempstr+=f"   result=gsetter_{var_name}_prefix_1(clnt);\n"
                                tempstr+=f"   if (result == NULL) \n"
                                tempstr+=f"       clnt_perror(clnt, \"call failed\");\n"
                                tempstr+=f"   return *result;\n"
                                tempstr+=f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                wrapper_functions.append(tempstr)
                                wrapper_header_functions.append(f"{gval_type} gsetter_{var_name}_prefix_wrapper();\n")

                            #idl_funcion
                            if f"{gval_type} gsetter_{var_name}_prefix" not in gsetter_idl_str_list:
                                gsetter_idl_str_list.append(f"{gval_type} gsetter_{var_name}_prefix")
                                gsetter_idl= f"{gval_type} gsetter_{var_name}_prefix() = {self.function_counter};\n"
                                self.function_counter += 1
                                idl_functions.append(gsetter_idl)

                            #收集此处的node,替换为gsetter_{gval_name}_{filed_name}_postfix_wrapper
                            getter_name = f"gsetter_{var_name}_prefix_wrapper"
                            replacement = c_ast.FuncCall(
                                name=c_ast.ID(name=getter_name),
                                args=c_ast.ExprList(exprs=[])
                            )
                            self.changes.append((node, replacement))

                            # 记录替换信息
                            line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                            if self.current_func not in self.func_replacements:
                                self.func_replacements[self.current_func] = []
                            self.func_replacements[self.current_func].append((
                                var_name,
                                "prefix_inc_in_rhs",
                                line_no
                            ))
                            
                            
                        else:
                            newfunname= f"gsetter_{var_name}_prefix_have_ret_1_svc"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                tempstr=f"{gval_type} *gsetter_{var_name}_postfix_1_svc( struct svc_req *rqstp)\n"
                                tempstr+=f"{{\n"
                                #static int result;
                                tempstr+=f"   static {gval_type} result;\n"
                                tempstr+=f" result= {var_name}++;\n"
                                tempstr+=f"    return &result;\n"
                                tempstr+=f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                server_functions.append(tempstr)
                            newfunname= f"gsetter_{var_name}_postfix_wrapper"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                tempstr=f"{gval_type} gsetter_{var_name}_postfix_wrapper()\n"
                                tempstr+=f"{{\n"
                                tempstr+=f"   {gval_type} *result;\n"
                                tempstr+=f"   result=gsetter_{var_name}_postfix_1(clnt);\n"
                                tempstr+=f"   if (result == NULL) \n"
                                tempstr+=f"       clnt_perror(clnt, \"call failed\");\n"
                                tempstr+=f"   return *result;\n"
                                tempstr+=f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                wrapper_functions.append(tempstr)
                                wrapper_header_functions.append(f"{gval_type} gsetter_{var_name}_postfix_wrapper();\n")
                            #收集此处的node,替换为gsetter_{gval_name}_{filed_name}_postfix_wrapper
                            if f"{gval_type} gsetter_{var_name}_postfix" not in gsetter_idl_str_list:
                                gsetter_idl_str_list.append(f"{gval_type} gsetter_{var_name}_postfix")
                                gsetter_idl= f"{gval_type} gsetter_{var_name}_postfix() = {self.function_counter};\n"
                                self.function_counter += 1
                                idl_functions.append(gsetter_idl)
                            getter_name = f"gsetter_{var_name}_postfix_wrapper"
                            replacement = c_ast.FuncCall(
                                name=c_ast.ID(name=getter_name),
                                args=c_ast.ExprList(exprs=[])
                            )
                            self.changes.append((node, replacement))
                            # 记录替换信息
                            line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                            if self.current_func not in self.func_replacements:
                                self.func_replacements[self.current_func] = []
                            self.func_replacements[self.current_func].append((
                                var_name,
                                "postfix_inc_in_rhs",
                                line_no
                            ))
                    elif gval_type == "char *" or gval_type == "unsigned char *":
                        print(f"处理自增自减操作2: {node.op} at {node.coord}")
                        print(f"变量类型: {gval_type}")
                        print(f"操作表达式: {op_expr}")
                        print(f"父表达式: {parent_expr}")
                        print(self.get_parent(node))
                        if node.op.startswith('p'):
                            """
                            char_ptr *gsetter_{var_name}_postfix_1_svc( struct svc_req *rqstp)
                            {
                                static char_ptr  result;
                                if ({val_name} == NULL) {
                                    result.charptr.charptr_len = 0;
                                    result.charptr.charptr_val = NULL;
                                } else {
                                    result.charptr.charptr_len = strlen(str);
                                    result.charptr.charptr_val = malloc(result.charptr.charptr_len + 1);
                                    if (result.charptr.charptr_val != NULL) {
                                        memcpy(result.charptr.charptr_val,{val_name}, result.charptr.charptr_len+1);
                                        {val_name}++;
                                        
                                    }
                                }
                                return &result;
                            }
                            """
                            newfunname= f"gsetter_{var_name}_postfix_1_svc"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                tempstr=f"char_ptr *gsetter_{var_name}_postfix_1_svc( struct svc_req *rqstp)\n"
                                tempstr+=f"{{\n"
                                tempstr+=f"   static char_ptr result;\n"
                                tempstr+=f"   if ({var_name} == NULL) {{\n"
                                tempstr+=f"       result.charptr.charptr_len = 0;\n"
                                tempstr+=f"       result.charptr.charptr_val = NULL;\n"
                                tempstr+=f"   }} else {{\n"
                                tempstr+=f"       result.charptr.charptr_len = strlen({var_name});\n"
                                tempstr+=f"       result.charptr.charptr_val = malloc(result.charptr.charptr_len + 1);\n"
                                tempstr+=f"       if (result.charptr.charptr_val != NULL) {{\n"
                                tempstr+=f"           memcpy(result.charptr.charptr_val, {var_name}, result.charptr.charptr_len + 1);\n"
                                tempstr+=f"           {var_name}++;\n"
                                tempstr+=f"       }}\n"
                                tempstr+=f"   }}\n"
                                tempstr+=f"   return &result;\n"
                                tempstr+=f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                server_functions.append(tempstr)
                            newfunname= f"gsetter_{var_name}_postfix_wrapper"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                """
                                char_ptr gsetter_{var_name}_postfix_wrapper()
                                {
                                char_ptr *result;
                                result=gsetter_{var_name}_postfix_1(clnt);
                                if (result == NULL)
                                    clnt_perror(clnt, "call failed");
                                else{
                                if (result->charptr.charptr_val == NULL) {
                                    result->charptr.charptr_len = 0;
                                    result->charptr.charptr_val = NULL;
                                }else {
                                    result->charptr.charptr_len = strlen(result->charptr.charptr_val);
                                    result->charptr.charptr_val = malloc(result->charptr.charptr_len + 1);
                                    memcpy(result->charptr.charptr_val,{var_name}, result->charptr.charptr_len+1);
                                    }
                                }
                                return result->charptr.charptr_val;
                                }
                                """
                                tempstr=f"char* gsetter_{var_name}_postfix_wrapper()\n"
                                tempstr+=f"{{\n"
                                tempstr+=f"   char_ptr *result;\n"
                                tempstr+=f"   result=gsetter_{var_name}_postfix_1(clnt);\n"
                                tempstr+=f"   if (result == NULL)\n"
                                tempstr+=f"       clnt_perror(clnt, \"call failed\");\n"
                                tempstr+=f"   else{{\n"
                                tempstr+=f"       if (result->charptr.charptr_val == NULL) {{\n"
                                tempstr+=f"           result->charptr.charptr_len = 0;\n"
                                tempstr+=f"           result->charptr.charptr_val = NULL;\n"
                                tempstr+=f"       }}else {{\n"
                                tempstr+=f"           result->charptr.charptr_len = strlen(result->charptr.charptr_val);\n"
                                tempstr+=f"           result->charptr.charptr_val = malloc(result->charptr.charptr_len + 1);\n"
                                tempstr+=f"           memcpy(result->charptr.charptr_val, {var_name}, result->charptr.charptr_len + 1);\n"
                                tempstr+=f"       }}\n"
                                tempstr+=f"   }}\n"
                                tempstr+=f"   return result->charptr.charptr_val;\n"
                                tempstr+=f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                wrapper_functions.append(tempstr)
                                wrapper_header_functions.append(f"char* gsetter_{var_name}_postfix_wrapper();\n")
                            #收集此处的node,替换为gsetter_{gval_name}_{filed_name}_postfix_wrapper
                            if f"char_ptr gsetter_{var_name}_postfix" not in gsetter_idl_str_list:
                                gsetter_idl_str_list.append(f"char_ptr gsetter_{var_name}_postfix")
                                gsetter_idl= f"char_ptr gsetter_{var_name}_postfix() = {self.function_counter};\n"
                                self.function_counter += 1
                                idl_functions.append(gsetter_idl)
                            getter_name = f"gsetter_{var_name}_postfix_wrapper"
                            replacement = c_ast.FuncCall(
                                name=c_ast.ID(name=getter_name),
                                args=c_ast.ExprList(exprs=[])
                            )
                            self.changes.append((node, replacement))
                            
                            line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                            if self.current_func not in self.func_replacements:
                                self.func_replacements[self.current_func] = []
                            self.func_replacements[self.current_func].append((
                                var_name,
                                "postfix_inc_in_rhs",
                                line_no
                            ))

                        else:
                            print("字符char*前缀:", gval_type)
                            print(op_expr)
                            print(parent_expr)
                            """
                            void gsetter_{var_name}_prefix_1_svc( struct svc_req *rqstp)
                            {
                                ++{var_name};
                                return;
                            }
                            
                            """
                            newfunname= f"gsetter_{var_name}_prefix_1_svc"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                tempstr=f"void gsetter_{var_name}_prefix_1_svc( struct svc_req *rqstp)\n"
                                tempstr+=f"{{\n"
                                tempstr+=f"    ++{var_name};\n"
                                tempstr+=f"    return;\n"
                                tempstr+=f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                server_functions.append(tempstr)
                            newfunname= f"gsetter_{var_name}_prefix_wrapper"
                            if newfunname not in gsetter_fun_have_def:  
                                gsetter_fun_have_def.append(newfunname)
                                tempstr=f"void gsetter_{var_name}_prefix_wrapper()\n"
                                tempstr+=f"{{\n"
                                tempstr+=f"    gsetter_{var_name}_prefix_1(clnt);\n"
                                tempstr+f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                wrapper_functions.append(tempstr)
                                wrapper_header_functions.append(f"void gsetter_{var_name}_prefix_wrapper();\n")
                                
                            #收集此处的node,替换为gsetter_{gval_name}_{filed_name}_postfix_wrapper
                            getter_name = f"gsetter_{var_name}_prefix_wrapper"
                            if f"void gsetter_{var_name}_prefix" not in gsetter_idl_str_list:
                                gsetter_idl_str_list.append(f"void gsetter_{var_name}_prefix")
                                gsetter_idl= f"void gsetter_{var_name}_prefix() = {self.function_counter};\n"
                                self.function_counter += 1
                                idl_functions.append(gsetter_idl)
                            replacement = c_ast.FuncCall(
                                name=c_ast.ID(name=getter_name),
                                args=c_ast.ExprList(exprs=[])
                            )
                            self.changes.append((node, replacement))
                            # 记录替换信息
                            line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                            if self.current_func not in self.func_replacements:
                                self.func_replacements[self.current_func] = []
                            self.func_replacements[self.current_func].append((
                                var_name,
                                "prefix_inc_in_rhs",
                                line_no
                            ))
                        


                    else:
                        print("未处理的变量类型:", gval_type)
                        


                
                    
                    # # 确定是前缀还是后缀操作
                    # is_prefix = not node.op.startswith('p')
                    
                    # op_type = '+' if ('++' in node.op) else '-'
                    
                    # # 获取父节点以确定上下文
                    # parent = self.get_parent(node)

                    # # 检查是否在条件表达式中
                    # is_in_condition = False
                    # condition_parent = None
                    # condition_expr = None
                    
                    # # 向上查找条件语句父节点
                    # ancestor = parent
                    # for _ in range(3):  # 限制查找深度
                    #     if ancestor is None:
                    #         break
                    #     if isinstance(ancestor, (c_ast.If, c_ast.While, c_ast.DoWhile, c_ast.For)):
                    #         is_in_condition = True
                    #         condition_parent = ancestor
                    #         if isinstance(ancestor, c_ast.If):
                    #             condition_expr = ancestor.cond
                    #         elif isinstance(ancestor, c_ast.While) or isinstance(ancestor, c_ast.DoWhile):
                    #             condition_expr = ancestor.cond
                    #         elif isinstance(ancestor, c_ast.For):
                    #             condition_expr = ancestor.cond
                    #         break
                    #     # 如果是二元操作，继续向上查找
                    #     if isinstance(ancestor, c_ast.BinaryOp):
                    #         ancestor = self.get_parent(ancestor)
                    #     else:
                    #         break
                    
                    # # 如果是条件表达式中的自增自减，使用特殊处理
                    # if is_in_condition and condition_expr:
                    #     # 生成替换代码块
                    #     statements = []
                        
                    #     # 1. 声明临时变量并获取全局变量当前值
                    #     temp_var = f"temp_{var_name}_{self._get_unique_id()}"
                    #     get_call = c_ast.FuncCall(
                    #         name=c_ast.ID(name=getter_name),
                    #         args=c_ast.ExprList(exprs=[])
                    #     )
                        
                    #     # 添加变量声明和getter调用
                    #     statements.append(c_ast.Decl(
                    #         name=temp_var,
                    #         quals=[],
                    #         storage=[],
                    #         funcspec=[],
                    #         align=[],
                    #         type=c_ast.TypeDecl(
                    #             declname=temp_var,
                    #             quals=[],
                    #             align=[],
                    #             type=c_ast.IdentifierType(names=["int"])  # 假设是整型
                    #         ),
                    #         init=get_call,
                    #         bitsize=None
                    #     ))
                        
                    #     # 2. 执行自增自减操作
                    #     if is_prefix:
                    #         # 前缀操作: 修改值
                    #         calc_expr = c_ast.BinaryOp(
                    #             op=op_type, 
                    #             left=c_ast.ID(name=temp_var),
                    #             right=c_ast.Constant(type='int', value='1')
                    #         )
                            
                    #         statements.append(c_ast.Assignment(
                    #             op='=',
                    #             lvalue=c_ast.ID(name=temp_var),
                    #             rvalue=calc_expr
                    #         ))
                    #     else:
                    #         # 后缀操作: 先保存当前值作为结果，然后修改
                    #         result_var = f"result_{var_name}_{self._get_unique_id()}"
                    #         statements.append(c_ast.Decl(
                    #             name=result_var,
                    #             quals=[],
                    #             storage=[],
                    #             funcspec=[],
                    #             align=[],
                    #             type=c_ast.TypeDecl(
                    #                 declname=result_var,
                    #                 quals=[],
                    #                 align=[],
                    #                 type=c_ast.IdentifierType(names=["int"])
                    #             ),
                    #             init=c_ast.ID(name=temp_var),
                    #             bitsize=None
                    #         ))
                            
                    #         # 然后修改临时变量
                    #         calc_expr = c_ast.BinaryOp(
                    #             op=op_type,
                    #             left=c_ast.ID(name=temp_var),
                    #             right=c_ast.Constant(type='int', value='1')
                    #         )
                            
                    #         statements.append(c_ast.Assignment(
                    #             op='=',
                    #             lvalue=c_ast.ID(name=temp_var),
                    #             rvalue=calc_expr
                    #         ))
                        
                    #     # 3. 更新全局变量
                    #     set_call = c_ast.FuncCall(
                    #         name=c_ast.ID(name=setter_name),
                    #         args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var)])
                    #     )
                    #     statements.append(set_call)
                        
                    #     # 4. 创建一个新的条件表达式，替换原始表达式中的自增自减部分
                    #     new_condition = self._create_transformed_condition(condition_expr, node, temp_var)
                        
                    #     # 5. 创建新的条件语句
                    #     new_condition_stmt = self._create_new_condition_statement(condition_parent, new_condition)
                        
                    #     # 6. 合并为复合语句块
                    #     statements.append(new_condition_stmt)
                    #     replacement = c_ast.Compound(block_items=statements)
                        
                    #     # 7. 添加替换
                    #     self.control_flow_replacements.append((condition_parent, replacement))
                        
                    #     # 记录替换信息
                    #     line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                    #     if self.current_func not in self.func_replacements:
                    #         self.func_replacements[self.current_func] = []
                    #     self.func_replacements[self.current_func].append((
                    #         var_name, 
                    #         f"{'prefix' if is_prefix else 'postfix'}_{'inc' if op_type == '+' else 'dec'}_in_condition", 
                    #         line_no
                    #     ))
                        
                    #     return  # 不继续遍历
                    
                    # # 生成替换代码块
                    # statements = []
                    
                    # # 1. 声明临时变量并获取全局变量当前值
                    # temp_var = f"temp_{var_name}_{self._get_unique_id()}"
                    # get_call = c_ast.FuncCall(
                    #     name=c_ast.ID(name=getter_name),
                    #     args=c_ast.ExprList(exprs=[])
                    # )
                    
                    # # 添加变量声明和getter调用
                    # statements.append(c_ast.Decl(
                    #     name=temp_var,
                    #     quals=[],
                    #     storage=[],
                    #     funcspec=[],
                    #     align=[],
                    #     type=c_ast.TypeDecl(
                    #         declname=temp_var,
                    #         quals=[],
                    #         align=[],
                    #         type=c_ast.IdentifierType(names=["int"])  # 假设是整型
                    #     ),
                    #     init=get_call,
                    #     bitsize=None
                    # ))
                    
                    # # 2. 声明用于返回的临时变量
                    # result_var = f"result_{var_name}_{self._get_unique_id()}"
                    
                    # # # 前缀操作: 先修改值再返回
                    # # if is_prefix:
                    # #     # 计算新值并保存在临时变量中
                    # calc_expr = c_ast.BinaryOp(
                    #     op=op_type, 
                    #     left=c_ast.ID(name=temp_var),
                    #     right=c_ast.Constant(type='int', value='1')
                    # )
                    
                    # # 更新临时变量
                    # statements.append(c_ast.Assignment(
                    #     op='=',
                    #     lvalue=c_ast.ID(name=temp_var),
                    #     rvalue=calc_expr
                    # ))
                        
                    # #     # 结果变量等于修改后的值
                    # #     statements.append(c_ast.Decl(
                    # #         name=result_var,
                    # #         quals=[],
                    # #         storage=[],
                    # #         funcspec=[],
                    # #         align=[],
                    # #         type=c_ast.TypeDecl(
                    # #             declname=result_var,
                    # #             quals=[],
                    # #             align=[],
                    # #             type=c_ast.IdentifierType(names=["int"])
                    # #         ),
                    # #         init=c_ast.ID(name=temp_var),
                    # #         bitsize=None
                    # #     ))
                    # # else:
                    # #     # 后缀操作: 先保存当前值作为结果
                    # #     statements.append(c_ast.Decl(
                    # #         name=result_var,
                    # #         quals=[],
                    # #         storage=[],
                    # #         funcspec=[],
                    # #         align=[],
                    # #         type=c_ast.TypeDecl(
                    # #             declname=result_var,
                    # #             quals=[],
                    # #             align=[],
                    # #             type=c_ast.IdentifierType(names=["int"])
                    # #         ),
                    # #         init=c_ast.ID(name=temp_var),
                    # #         bitsize=None
                    # #     ))
                        
                    # #     # 然后修改临时变量
                    # #     calc_expr = c_ast.BinaryOp(
                    # #         op=op_type,
                    # #         left=c_ast.ID(name=temp_var),
                    # #         right=c_ast.Constant(type='int', value='1')
                    # #     )
                        
                    # #     statements.append(c_ast.Assignment(
                    # #         op='=',
                    # #         lvalue=c_ast.ID(name=temp_var),
                    # #         rvalue=calc_expr
                    # #     ))
                    
                    # # 3. 更新全局变量 (通过setter)
                    # set_call = c_ast.FuncCall(
                    #     name=c_ast.ID(name=setter_name),
                    #     args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var)])
                    # )
                    # statements.append(set_call)
                    
                    # # 4. 确定如何处理条件语句中的自增自减
                    # if isinstance(parent, c_ast.If) and parent.cond is node:
                    #     # 在条件语句中直接使用结果变量
                    #     parent.cond = c_ast.ID(name=result_var)
                    #     # 创建一个复合语句块，包含临时变量操作和修改后的if语句
                    #     replacement = c_ast.Compound(block_items=statements + [parent])
                    #     self.control_flow_replacements.append((parent, replacement))
                    # elif isinstance(parent, (c_ast.While, c_ast.For, c_ast.DoWhile)) and parent.cond is node:
                    #     # 循环条件中的处理类似if
                    #     parent.cond = c_ast.ID(name=result_var)
                    #     replacement = c_ast.Compound(block_items=statements + [parent])
                    #     self.control_flow_replacements.append((parent, replacement))
                    # else:
                    #     # 在表达式中或独立语句中，用结果变量替换整个自增自减操作
                    #     # statements.append(c_ast.ID(name=result_var))
                    #     replacement = c_ast.Compound(block_items=statements)
                    #     self.changes.append((node, replacement))
                    
                    # # 记录替换信息
                    # line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                    # if self.current_func not in self.func_replacements:
                    #     self.func_replacements[self.current_func] = []
                    # self.func_replacements[self.current_func].append((
                    #     var_name, 
                    #     f"{'prefix' if is_prefix else 'postfix'}_{'inc' if op_type == '+' else 'dec'}", 
                    #     line_no
                    # ))
                    
                    # return  # 不继续遍历这个节点的子节点
            
                elif isinstance(node.expr,c_ast.ArrayRef):
                    if id(node.expr.name.name) not in self.global_vars:
                        return
                    # 数组元素的自增自减操作
                    gval_name = node.expr.name.name
                    print(f"处理自增自减操作2: {node.op} at {node.coord}")
                    print(c_generator.CGenerator().visit(node))
                    print()
                    print(c_generator.CGenerator().visit(self.get_parent(node)))
                    print(self.current_func)
                    print(self._infer_gval_type(gval_name))

                elif  isinstance(node.expr, c_ast.StructRef):# and id(node.expr) in self.struct_field_access:
                    gval_name = node.expr.name.name
                    filed_name = node.expr.field.name
                    if gval_name not in self.global_vars:
                        return
                    print(f"处理结构体字段自增自减3: {node.op} at {node.coord}")
                    print(self.current_func)
                    op_expr = c_generator.CGenerator().visit(node)
                    gval_type= self._infer_gval_type(gval_name, filed_name)
                    parent_expr = c_generator.CGenerator().visit(self.get_parent(node))
                    print(gval_type)
                    if gval_type == "int" or gval_type == "unsigned long":
                        if not node.op.startswith('p'):
                            newfunname= f"gsetter_{gval_name}_{filed_name}_prefix_1_svc"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                tempstr = f"{gval_type} *gsetter_{gval_name}_{filed_name}_prefix_1_svc( struct svc_req *rqstp)\n"
                                tempstr += f"{{\n"
                                tempstr += f"    ++{gval_name}.{filed_name};\n"
                                tempstr += f"    return &({gval_name}.{filed_name});\n"
                                tempstr += f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                server_functions.append(tempstr)
                            newfunname= f"gsetter_{gval_name}_{filed_name}_prefix_wrapper"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                tempstr = f"{gval_type} gsetter_{gval_name}_{filed_name}_prefix_wrapper()\n"
                                tempstr += f"{{\n"
                                tempstr += f"   {gval_type} *result;\n"
                                tempstr += f"   result=gsetter_{gval_name}_{filed_name}_prefix_1(clnt);\n"
                                tempstr += f"   if (result == NULL) \n"
                                tempstr += f"       clnt_perror(clnt, \"call failed\");\n"
                                tempstr += f"   return *result;\n"
                                tempstr += f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                wrapper_functions.append(tempstr)
                                wrapper_header_functions.append(f"{gval_type} gsetter_{gval_name}_{filed_name}_prefix_wrapper();\n")

                            #idl_funcion
                            if f"{gval_type} gsetter_{gval_name}_{filed_name}_prefix" not in gsetter_idl_str_list:
                                gsetter_idl_str_list.append(f"{gval_type} gsetter_{gval_name}_{filed_name}_prefix")
                                gsetter_idl = f"{gval_type} gsetter_{gval_name}_{filed_name}_prefix() = {self.function_counter};\n"
                                self.function_counter += 1
                                idl_functions.append(gsetter_idl)

                            #收集此处的node,替换为gsetter_{gval_name}_{filed_name}_prefix_wrapper
                            getter_name = f"gsetter_{gval_name}_{filed_name}_prefix_wrapper"
                            replacement = c_ast.FuncCall(
                                name=c_ast.ID(name=getter_name),
                                args=c_ast.ExprList(exprs=[])
                            )
                            self.changes.append((node, replacement))
                            
                            # 记录替换信息
                            line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                            if self.current_func not in self.func_replacements:
                                self.func_replacements[self.current_func] = []
                            self.func_replacements[self.current_func].append((
                                f"{gval_name}.{filed_name}",
                                "struct_field_prefix_inc",
                                line_no
                            ))
                            

                        else:
                            newfunname= f"gsetter_{gval_name}_{filed_name}_postfix_1_svc"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                tempstr = f"{gval_type} *gsetter_{gval_name}_{filed_name}_postfix_1_svc( struct svc_req *rqstp)\n"
                                tempstr += f"{{\n"
                                tempstr += f"   static {gval_type} result;\n"
                                tempstr += f" result= {gval_name}.{filed_name}++;\n"
                                tempstr += f"    return &result;\n"
                                tempstr += f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                server_functions.append(tempstr)
                            newfunname= f"gsetter_{gval_name}_{filed_name}_postfix_wrapper"
                            if newfunname not in gsetter_fun_have_def:
                                gsetter_fun_have_def.append(newfunname)
                                tempstr = f"{gval_type} gsetter_{gval_name}_{filed_name}_postfix_wrapper()\n"
                                tempstr += f"{{\n"
                                tempstr += f"   {gval_type} *result;\n"
                                tempstr += f"   result=gsetter_{gval_name}_{filed_name}_postfix_1(clnt);\n"
                                tempstr += f"   if (result == NULL) \n"
                                tempstr += f"       clnt_perror(clnt, \"call failed\");\n"
                                tempstr += f"   return *result;\n"
                                tempstr += f"}}\n"
                                gsetter_fun_str_list.append(tempstr)
                                wrapper_functions.append(tempstr)
                                wrapper_header_functions.append(f"{gval_type} gsetter_{gval_name}_{filed_name}_postfix_wrapper();\n")
                            #idl_funcion
                            if f"{gval_type} gsetter_{gval_name}_{filed_name}_postfix" not in gsetter_idl_str_list:
                                gsetter_idl_str_list.append(f"{gval_type} gsetter_{gval_name}_{filed_name}_postfix")
                                gsetter_idl = f"{gval_type} gsetter_{gval_name}_{filed_name}_postfix() = {self.function_counter};\n"
                                self.function_counter += 1
                                idl_functions.append(gsetter_idl)

                            #收集此处的node,替换为gsetter_{gval_name}_{filed_name}_postfix_wrapper
                            getter_name = f"gsetter_{gval_name}_{filed_name}_postfix_wrapper"
                            replacement = c_ast.FuncCall(
                                name=c_ast.ID(name=getter_name),
                                args=c_ast.ExprList(exprs=[])
                            )
                            self.changes.append((node, replacement))

                            # 记录替换信息
                            line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                            if self.current_func not in self.func_replacements:
                                self.func_replacements[self.current_func] = []
                            self.func_replacements[self.current_func].append((
                                f"{gval_name}.{filed_name}",
                                "struct_field_postfix_inc",
                                line_no
                            ))



                    else:
                        print("未处理的结构体字段自增自减操作: ", node)
                    return
                elif isinstance(node.expr, c_ast.UnaryOp):
                    # 一元操作符处理
                    if node.expr.expr.name not in self.global_vars:
                        return
                    gval_name = node.expr.expr.name
                    print(f"处理一元一元操作符: {node.op} at {node.coord}")
                    print(c_generator.CGenerator().visit(node))
                    print(c_generator.CGenerator().visit(self.get_parent(node)))

                else:
                    print("未处理的一元操作符: ", node)
                    print(node.coord)
                
                # 类似上面的逻辑处理结构体字段...
                # [实现结构体字段自增自减的代码]
            
            # 继续遍历其他子节点
            return self.generic_visit(node)

        def _create_transformed_condition(self, original_condition, target_node, replacement_var):
            """创建转换后的条件表达式，将自增自减操作替换为临时变量"""
            
            # 克隆原始条件表达式
            new_condition = copy.deepcopy(original_condition)
            
            # 递归替换目标节点
            def replace_target(current, target, replacement):
                if current is None:
                    return None
                    
                # 替换整个表达式(如果当前节点是目标)
                if self._nodes_equal(current, target):
                    return c_ast.ID(name=replacement)
                
                # 递归处理二元操作符的左右子表达式
                if isinstance(current, c_ast.BinaryOp):
                    current.left = replace_target(current.left, target, replacement)
                    current.right = replace_target(current.right, target, replacement)
                
                # 递归处理一元操作符的表达式
                if isinstance(current, c_ast.UnaryOp):
                    if not self._nodes_equal(current, target):  # 如果不是目标节点本身
                        current.expr = replace_target(current.expr, target, replacement)
                
                return current
            
            
                    
            # 执行替换
            return replace_target(new_condition, target_node, replacement_var)
        # 比较两个节点是否相同
        def _nodes_equal(self, node1, node2):
            """比较两个AST节点是否相同"""
            if node1 is node2:  # 引用相等
                return True
            
            # 比较生成的代码字符串是否相同
            try:
                return c_generator.CGenerator().visit(node1) == c_generator.CGenerator().visit(node2)
            except:
                return False

        def _create_new_condition_statement(self, original_stmt, new_condition):
            """创建新的条件语句，保留原始语句的其他部分"""
            
            # 克隆原始语句
            new_stmt = copy.deepcopy(original_stmt)
            
            # 替换条件部分
            if isinstance(new_stmt, c_ast.If):
                new_stmt.cond = new_condition
            elif isinstance(new_stmt, c_ast.While):
                new_stmt.cond = new_condition
            elif isinstance(new_stmt, c_ast.DoWhile):
                new_stmt.cond = new_condition
            elif isinstance(new_stmt, c_ast.For):
                new_stmt.cond = new_condition
                
            return new_stmt
        
        def _get_unique_id(self):
            """生成唯一ID用于临时变量名"""
            if not hasattr(self, '_counter'):
                self._counter = 0
            self._counter += 1
            return self._counter
                
        def visit_BinaryOp(self, node):
            pass
            # print(f"处理二元操作符: {node.op} at {node.coord}")
            # print(node)

            # # 获取并打印父节点信息
            # parent_node = self.get_parent(node)
            # if parent_node:
            #     print(parent_node)
            # else:
            #     print("没有父节点")
            # # 继续遍历其他子节点
            return self.generic_visit(node)
        
        def get_parent(self, node):
            """获取节点的父节点"""
            return self.parent_map.get(id(node))
        
        
        def post_process_function_calls(self):
            """在遍历完成后处理函数调用中的全局变量引用参数，根据不同上下文生成不同的替换代码"""
            for node, global_var_refs, context_info in self.func_call_modifications:
                # 根据上下文决定如何处理函数调用
                if (context_info["is_while_cond"] or 
                    context_info["is_dowhile_cond"] or 
                    context_info["is_if_cond"] or 
                    context_info["is_for_cond"]):
                    # 条件语句中的函数调用，需要将函数调用和变量值提取到条件之前
                    self._handle_condition_context(node, global_var_refs, context_info)
                elif context_info["is_assignment_rhs"]:
                    # 赋值语句右侧的函数调用，需要特殊处理
                    self._handle_assignment_context(node, global_var_refs, context_info)
                else:
                    # 普通函数调用
                    self._handle_regular_function_call(node, global_var_refs)

        def _handle_condition_context(self, node, global_var_refs, context_info):
            """
            处理条件语句中的函数调用
            例如: while (ring_full_count(&netiring)) { ... }
            转换为:
            {
                Ring temp_netiring = get_netiring_wrapper();
                int temp_result = ring_full_count(&temp_netiring);
                set_netiring_wrapper(temp_netiring);
                while (temp_result) {
                    ...
                    temp_netiring = get_netiring_wrapper();
                    temp_result = ring_full_count(&temp_netiring);
                    set_netiring_wrapper(temp_netiring);
                }
            }
            """
            print(f"处理条件语句中的函数调用: {node.name.name} at {node.coord}")
            # 创建替换语句块
            statements = []
            temp_vars = []  # 保存临时变量名，用于后续生成setter调用
            result_var_name = f"temp_result_{len(self.control_flow_replacements)}"
            
            # 为每个引用的全局变量生成临时变量和getter调用
            for i, var_name in global_var_refs:
                # 生成临时变量名
                temp_var_name = f"temp_{var_name}"
                temp_vars.append((var_name, temp_var_name, i))
                
                # 确定变量类型
                var_type= self._infer_gval_type(var_name)
                
                
                # 生成临时变量声明和赋值语句
                getter_call = c_ast.FuncCall(
                    name=c_ast.ID(name=f"get_{var_name}_wrapper"),
                    args=c_ast.ExprList(exprs=[])
                )
                statements.append(c_ast.Decl(
                    name=temp_var_name,
                    quals=[],
                    storage=[],
                    funcspec=[],
                    align=[],
                    type=c_ast.TypeDecl(
                        declname=temp_var_name,
                        quals=[],
                        align=[],
                        type=c_ast.IdentifierType(names=[var_type])
                    ),
                    init=getter_call,
                    bitsize=None
                ))
            
            
            # 克隆原函数调用，修改参数为临时变量
            func_call_clone = copy.deepcopy(node)
            for var_name, temp_var_name, idx in temp_vars:
                func_call_clone.args.exprs[idx] = c_ast.UnaryOp(
                    op='&',
                    expr=c_ast.ID(name=temp_var_name)
                )
            
            # 创建结果变量来存储函数调用结果
            result_type_name = self._infer_return_type(node)
            statements.append(c_ast.Decl(
                name=result_var_name,
                quals=[],
                storage=[],
                funcspec=[],
                align=[],
                type=c_ast.TypeDecl(
                    declname=result_var_name,
                    quals=[],
                    align=[],
                    type=c_ast.IdentifierType(names=[result_type_name])
                ),
                init=func_call_clone,
                bitsize=None
            ))
            
            # 为每个引用的全局变量生成setter调用
            for var_name, temp_var_name, _ in temp_vars:
                setter_call = c_ast.FuncCall(
                    name=c_ast.ID(name=f"set_{var_name}_wrapper"),
                    args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var_name)])
                )
                statements.append(setter_call)
            
            # 替换条件表达式中的函数调用为结果变量
            parent = context_info["parent"]
            
            # 处理不同类型的条件语句
            if isinstance(parent, c_ast.If):
                # 对于if语句，需要在条件表达式中查找和替换函数调用
                if node in parent.cond:
                    print("node in parent.cond")
                    # 创建复制的条件表达式
                    new_cond = copy.deepcopy(parent.cond)
                    
                    # 定义一个递归替换函数，用于替换条件表达式中的函数调用
                    def replace_function_call_in_expr(current_node):
                        # 检查当前节点的所有子节点
                        for field_name, field in current_node.children():
                            if isinstance(field, c_ast.Node):
                                # 如果子节点就是目标函数调用，则替换它
                                if _nodes_equal(field, node):
                                    setattr(current_node, field_name, c_ast.ID(name=result_var_name))
                                else:
                                    # 否则继续递归检查
                                    replace_function_call_in_expr(field)
                            elif isinstance(field, list):
                                # 处理列表类型的字段，如ExprList中的exprs
                                for i, item in enumerate(field):
                                    if isinstance(item, c_ast.Node):
                                        if _nodes_equal(item, node):
                                            field[i] = c_ast.ID(name=result_var_name)
                                        else:
                                            replace_function_call_in_expr(item)
                                            
                    # 比较两个节点是否相同
                    def _nodes_equal(node1, node2):
                        """比较两个AST节点是否相同，基于它们的类型和内容"""
                        if node1 is node2:  # 引用相等
                            return True
                        
                        # 比较生成的代码字符串是否相同
                        try:
                            return c_generator.CGenerator().visit(node1) == c_generator.CGenerator().visit(node2)
                        except:
                            return False
                    
                    # 在条件表达式中替换函数调用
                    replace_function_call_in_expr(new_cond)
                    
                    # 使用修改后的条件表达式
                    parent.cond = new_cond
                    
                    # 创建一个复合语句块，包含初始化和if语句
                    compound = c_ast.Compound(block_items=statements + [parent])
                    self.control_flow_replacements.append((parent, compound))
                
                
            elif isinstance(parent, c_ast.While) and parent.cond == node:
                # 处理 while 循环 - 直接替换条件
                # new_parent = copy.deepcopy(parent)
                parent.cond = c_ast.ID(name=result_var_name)
                
                # 在循环体末尾添加代码以更新临时变量和结果
                loop_body_statements = []
                if isinstance(parent.stmt, c_ast.Compound):
                    loop_body_statements = parent.stmt.block_items or []
                else:
                    loop_body_statements = [parent.stmt] if parent.stmt else []
                
                # 在循环体末尾添加更新语句
                for var_name, temp_var_name, _ in temp_vars:
                    update_getter = c_ast.Assignment(
                        op='=',
                        lvalue=c_ast.ID(name=temp_var_name),
                        rvalue=c_ast.FuncCall(
                            name=c_ast.ID(name=f"get_{var_name}_wrapper"),
                            args=c_ast.ExprList(exprs=[])
                        )
                    )
                    loop_body_statements.append(update_getter)
                
                # 更新结果变量
                update_call = copy.deepcopy(func_call_clone)
                update_result = c_ast.Assignment(
                    op='=',
                    lvalue=c_ast.ID(name=result_var_name),
                    rvalue=update_call
                )
                loop_body_statements.append(update_result)
                
                # 为每个引用的全局变量生成setter调用
                for var_name, temp_var_name, _ in temp_vars:
                    update_setter = c_ast.FuncCall(
                        name=c_ast.ID(name=f"set_{var_name}_wrapper"),
                        args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var_name)])
                    )
                    loop_body_statements.append(update_setter)
                
                # 更新循环体
                parent.stmt = c_ast.Compound(block_items=loop_body_statements)
                
                # 创建一个复合语句块，包含初始化和循环
                compound = c_ast.Compound(block_items=statements + [parent])
                print(c_generator.CGenerator().visit(compound))
                self.control_flow_replacements.append((parent, compound))
            

            elif isinstance(parent, c_ast.DoWhile) and parent.cond == node:
                # 处理 do-while 循环 - 直接替换条件
                parent.cond = c_ast.ID(name=result_var_name)
                
                # 在循环体末尾添加代码以更新临时变量和结果
                loop_body_statements = []
                if isinstance(parent.stmt, c_ast.Compound):
                    loop_body_statements = parent.stmt.block_items or []
                else:
                    loop_body_statements = [parent.stmt] if parent.stmt else []
                
                # 在循环体末尾添加更新语句
                for var_name, temp_var_name, _ in temp_vars:
                    update_getter = c_ast.Assignment(
                        op='=',
                        lvalue=c_ast.ID(name=temp_var_name),
                        rvalue=c_ast.FuncCall(
                            name=c_ast.ID(name=f"get_{var_name}_wrapper"),
                            args=c_ast.ExprList(exprs=[])
                        )
                    )
                    loop_body_statements.append(update_getter)
                
                # 更新结果变量
                update_call = copy.deepcopy(func_call_clone)
                update_result = c_ast.Assignment(
                    op='=',
                    lvalue=c_ast.ID(name=result_var_name),
                    rvalue=update_call
                )
                loop_body_statements.append(update_result)
                
                # 为每个引用的全局变量生成setter调用
                for var_name, temp_var_name, _ in temp_vars:
                    update_setter = c_ast.FuncCall(
                        name=c_ast.ID(name=f"set_{var_name}_wrapper"),
                        args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var_name)])
                    )
                    loop_body_statements.append(update_setter)
                
                # 更新循环体
                parent.stmt = c_ast.Compound(block_items=loop_body_statements)

                """将statements中第一条语句和第二条语句的初始化去掉，移除第三条语句"""
                if len(statements) > 0:
                    # 移除第一条语句的初始化
                    first_statement = statements[0]
                    if isinstance(first_statement, c_ast.Decl):
                        first_statement.init = None
                    # 移除第二条语句的初始化
                    if len(statements) > 1:
                        second_statement = statements[1]
                        if isinstance(second_statement, c_ast.Decl):
                            second_statement.init = None
                # 移除第三条语句
                if len(statements) > 2:
                    statements.pop(2)
                
                # 创建一个复合语句块，包含初始化和循环
                compound = c_ast.Compound(block_items=statements + [parent])
                self.control_flow_replacements.append((parent, compound))
                
            elif isinstance(parent, c_ast.For) and parent.cond == node:
                # 处理 for 循环 - 直接替换条件
                parent.cond = c_ast.ID(name=result_var_name)
                
                # 在循环体末尾添加代码以更新临时变量和结果
                loop_body_statements = []
                if isinstance(parent.stmt, c_ast.Compound):
                    loop_body_statements = parent.stmt.block_items or []
                else:
                    loop_body_statements = [parent.stmt] if parent.stmt else []
                
                # 在循环体末尾添加更新语句
                for var_name, temp_var_name, _ in temp_vars:
                    update_getter = c_ast.Assignment(
                        op='=',
                        lvalue=c_ast.ID(name=temp_var_name),
                        rvalue=c_ast.FuncCall(
                            name=c_ast.ID(name=f"get_{var_name}_wrapper"),
                            args=c_ast.ExprList(exprs=[])
                        )
                    )
                    loop_body_statements.append(update_getter)
                
                # 更新结果变量 - 放在for循环的next表达式之前
                update_call = copy.deepcopy(func_call_clone)
                update_result = c_ast.Assignment(
                    op='=',
                    lvalue=c_ast.ID(name=result_var_name),
                    rvalue=update_call
                )
                loop_body_statements.append(update_result)
                
                # 为每个引用的全局变量生成setter调用
                for var_name, temp_var_name, _ in temp_vars:
                    update_setter = c_ast.FuncCall(
                        name=c_ast.ID(name=f"set_{var_name}_wrapper"),
                        args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var_name)])
                    )
                    loop_body_statements.append(update_setter)
                
                # 更新循环体
                parent.stmt = c_ast.Compound(block_items=loop_body_statements)
                
                # 创建一个复合语句块，包含初始化和循环
                compound = c_ast.Compound(block_items=statements + [parent])
                self.control_flow_replacements.append((parent, compound))
            print("临时变量声明和getter调用:")
            for stmt in statements:
                print(c_generator.CGenerator().visit(stmt))

        def _handle_assignment_context(self, node, global_var_refs, context_info):
            """
            处理赋值语句中的函数调用
            例如: netout = ring_full_count(&netoring) && ((flushline || ((!(options[34] & 0x02)) && ((!kludgelinemode) || (options[3] & 0x08)))) || (options[0] & 0x02));
            转换为:
            {
                Ring temp_netoring = get_netoring_wrapper();
                int temp_result_0 = ring_full_count(&temp_netoring);
                set_netoring_wrapper(temp_netoring);
                netout = temp_result_0 && ((flushline || ((!(options[34] & 0x02)) && ((!kludgelinemode) || (options[3] & 0x08)))) || (options[0] & 0x02));
            }
            """
            # 创建替换语句块
            print(f"处理赋值语句中的函数调用: {node.coord}")
            statements = []
            temp_vars = []  # 保存临时变量名，用于后续生成setter调用
            result_var_name = f"temp_result_{len(self.control_flow_replacements)}"
            
            # 为每个引用的全局变量生成临时变量和getter调用
            for i, var_name in global_var_refs:
                # 生成临时变量名
                temp_var_name = f"temp_{var_name}"
                temp_vars.append((var_name, temp_var_name, i))
                
                # 确定变量类型
                var_type = "int"  # 默认类型
                if var_name in self.struct_vars:
                    var_type = self.struct_vars[var_name]['struct_name']
                
                # 生成临时变量声明和赋值语句
                getter_call = c_ast.FuncCall(
                    name=c_ast.ID(name=f"get_{var_name}_wrapper"),
                    args=c_ast.ExprList(exprs=[])
                )
                statements.append(c_ast.Decl(
                    name=temp_var_name,
                    quals=[],
                    storage=[],
                    funcspec=[],
                    align=[],
                    type=c_ast.TypeDecl(
                        declname=temp_var_name,
                        quals=[],
                        align=[],
                        type=c_ast.IdentifierType(names=[var_type])
                    ),
                    init=getter_call,
                    bitsize=None
                ))
            
            # 克隆原函数调用，修改参数为临时变量
            func_call_clone = copy.deepcopy(node)
            for var_name, temp_var_name, idx in temp_vars:
                func_call_clone.args.exprs[idx] = c_ast.UnaryOp(
                    op='&',
                    expr=c_ast.ID(name=temp_var_name)
                )
            
            # 创建临时结果变量来存储函数调用结果
            result_type_name = self._infer_return_type(node)
            temp_result_decl = c_ast.Decl(
                name=result_var_name,
                quals=[],
                storage=[],
                funcspec=[],
                align=[],
                type=c_ast.TypeDecl(
                    declname=result_var_name,
                    quals=[],
                    align=[],
                    type=c_ast.IdentifierType(names=[result_type_name])
                ),
                init=func_call_clone,
                bitsize=None
            )
            statements.append(temp_result_decl)
            
            # 为每个引用的全局变量生成setter调用
            for var_name, temp_var_name, _ in temp_vars:
                setter_call = c_ast.FuncCall(
                    name=c_ast.ID(name=f"set_{var_name}_wrapper"),
                    args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var_name)])
                )
                statements.append(setter_call)
            
            # 获取原始赋值语句
            parent = context_info["parent"]
            
            # 创建一个新的赋值语句，复制原始赋值但替换函数调用部分
            new_assignment = copy.deepcopy(parent)
            
            # 使用更简单且可靠的替换方法
            def replace_node_in_ast(target_node, replacement_node, current_node):
                """递归替换AST中的节点"""
                if current_node is target_node:
                    return replacement_node
                
                # 遍历子节点
                for name, child in current_node.children():
                    if isinstance(child, c_ast.Node):
                        print(f"检查节点: {name} at {child.coord}")
                        print(target_node)
                        print(child)
                        if c_generator.CGenerator().visit(child) == c_generator.CGenerator().visit(target_node):
                            # 直接替换
                            print(f"替换节点: {name} at {child.coord}")
                            if hasattr(current_node, name):
                                print(f"替换节点: {name} at {child.coord}")
                                setattr(current_node, name, replacement_node)
                        else:
                            print(f"递归处理子节点: {name} at {child.coord}")
                            # 递归处理
                            replace_node_in_ast(target_node, replacement_node, child)
                    elif isinstance(child, list):
                        # 处理列表
                        for i, item in enumerate(child):
                            if item is target_node:
                                child[i] = replacement_node
                            elif isinstance(item, c_ast.Node):
                                replace_node_in_ast(target_node, replacement_node, item)
                
                return current_node
            
            # 替换右值中的函数调用
            replace_node_in_ast(node, c_ast.ID(name=result_var_name), new_assignment)
            
            # 添加修改后的赋值语句到语句块
            statements.append(new_assignment)
            
            # 创建一个复合语句块替换原赋值语句
            compound = c_ast.Compound(block_items=statements)
            self.changes.append((parent, compound))

        def _handle_regular_function_call(self, node, global_var_refs):
            """
            处理常规函数调用中的全局变量引用参数
            例如: ring_consumed(&ttyoring, n);
            转换为:
            {
                Ring temp_ttyoring = get_ttyoring_wrapper();
                ring_consumed(&temp_ttyoring, n);
                set_ttyoring_wrapper(temp_ttyoring);
            }
            """
            # 创建替换语句块
            statements = []
            temp_vars = []  # 保存临时变量名，用于后续生成setter调用
            
            # 为每个引用的全局变量生成临时变量和getter调用
            for i, var_name in global_var_refs:
                # 生成临时变量名
                temp_var_name = f"temp_{var_name}"
                temp_vars.append((var_name, temp_var_name))
                
                # 确定变量类型
                var_type = ""
                if var_name in self.struct_vars:
                    var_type = self.struct_vars[var_name]['struct_name']
                else:
                    # 默认使用通用类型
                    var_type = self._infer_gval_type(var_name)
                
                # 生成临时变量声明和赋值语句
                getter_call = c_ast.FuncCall(
                    name=c_ast.ID(name=f"get_{var_name}_wrapper"),
                    args=c_ast.ExprList(exprs=[])
                )
                statements.append(c_ast.Decl(
                    name=temp_var_name,
                    quals=[],
                    storage=[],
                    funcspec=[],
                    align=[],
                    type=c_ast.TypeDecl(
                        declname=temp_var_name,
                        quals=[],
                        align=[],
                        type=c_ast.IdentifierType(names=[var_type])
                    ),
                    init=getter_call,
                    bitsize=None
                ))
                
                # 创建替换后的参数（&temp_var_name）
                new_arg = c_ast.UnaryOp(
                    op='&',
                    expr=c_ast.ID(name=temp_var_name)
                )
                
                # 将原参数的位置和新参数保存起来，不要直接修改node
                # self.changes.append((node.args.exprs[i], new_arg))
                node.args.exprs[i] = new_arg
            
            # 克隆原函数调用（将在apply_changes中使用）
            func_call_clone = copy.deepcopy(node)
            
            # 添加原始函数调用（已克隆）到语句块
            statements.append(func_call_clone)
            
            # 为每个引用的全局变量生成setter调用
            for var_name, temp_var_name in temp_vars:
                setter_call = c_ast.FuncCall(
                    name=c_ast.ID(name=f"set_{var_name}_wrapper"),
                    args=c_ast.ExprList(exprs=[c_ast.ID(name=temp_var_name)])
                )
                statements.append(setter_call)
            
            # 创建替换整个函数调用的复合语句块
            replacement = c_ast.Compound(block_items=statements)
            
            # 添加到总体替换列表
            self.changes.append((node, replacement))

        def _infer_gval_type(self, gvarname,filed_name=None):
            """
            尝试推断函数调用的返回类型
            简单实现，实际应用中可能需要更复杂的类型推断
            """
            if gvarname in global_var_asts:
                pass
                # 如果全局变量AST存在，尝试从中获取类型
                print(f"推断全局变量 {gvarname} 的类型")
                # print(c_generator.CGenerator().visit(global_var_asts[gvarname]['ast']))
                var_ast = global_var_asts[gvarname]['ast']
                gvaltype=c_generator.CGenerator().visit(var_ast.type)
                if filed_name is not None:
                    #根据gvaltype到typedef_dict={} 或struct_dict={} 查找字段的定义并获取类型
                    print(filed_name)
                    if gvaltype in typedef_dict:
                        # 如果是typedef类型，获取实际类型
                        gvaltype = typedef_dict[gvaltype]['ast']
                        # print(f"typedef类型: {gvaltype}")
                        for filed_ast in gvaltype.type.type.decls:
                            if filed_name == filed_ast.name:
                                print(f"字段 {filed_name} 的类型: {c_generator.CGenerator().visit(filed_ast.type)}")
                                gvaltype = c_generator.CGenerator().visit(filed_ast.type)
                                break
                        # if filed_name in gvaltype.type.type.decls:
                        #     # 如果字段名在typedef定义中，获取字段类型
                        #     field_decl = gvaltype.type.type.decls[filed_name]
                        #     gvaltype = c_generator.CGenerator().visit(field_decl.type)
                    elif gvaltype in struct_dict:
                        # 如果是结构体类型，获取字段类型
                        struct_ast = struct_dict[gvaltype]['ast']
                        if filed_name in struct_ast.type.decls:
                            # 如果字段名在结构体定义中，获取字段类型
                            field_decl = struct_ast.type.decls[filed_name]
                            gvaltype = c_generator.CGenerator().visit(field_decl.type)
                    
                return gvaltype
            
            # 默认返回类型
            return "int"
        def _infer_return_type(self, fun_call_node):
            """
            尝试推断函数调用的返回类型
            简单实现，实际应用中可能需要更复杂的类型推断
            """
            if fun_call_node.name.name in fun_fundeclast_dict:
                # 如果函数声明AST存在，尝试从中获取返回类型
                print(f"推断函数调用 {fun_call_node.name.name} 的返回类型")
                func_decl_ast = fun_fundeclast_dict[fun_call_node.name.name]
                print(c_generator.CGenerator().visit(func_decl_ast.type.type))
                return (c_generator.CGenerator().visit(func_decl_ast.type.type))


            

            
            # 默认返回类型
            return "int"
        
        
        def apply_changes(self, ast):
            """应用所有记录的变更，避免重复处理"""
            # 记录已处理的节点
            processed_nodes = set()
            
            # 过滤重复的替换操作
            filtered_changes = []
            for node, replacement in self.changes:
                if id(node) not in processed_nodes:
                    filtered_changes.append((node, replacement))
                    processed_nodes.add(id(node))
            
            self.changes = filtered_changes

            """应用所有记录的变更，这是一个定制的节点替换器"""
            # 先进行函数调用的后处理
            self.post_process_function_calls()
            # 处理控制流结构的替换
            for original, replacement in self.control_flow_replacements:
                self.changes.append((original, replacement))
                # print(f"替换控制流结构: {c_generator.CGenerator().visit(original)} with {c_generator.CGenerator().visit(replacement)}")
            # 创建节点ID到替换节点的映射
            changes_map = {id(node): repl for node, repl in self.changes}
            

            class Transformer(c_ast.NodeVisitor):
                def __init__(self, changes_map):
                    self.changes_map = changes_map
                    self.processed_nodes = set()  # 记录已处理的节点ID，避免循环处理
                
                def visit(self, node):
                    """递归地遍历AST并应用替换，自底向上处理"""
                    if node is None:
                        return None
                        
                    # 如果节点已处理过，避免重复处理
                    if id(node) in self.processed_nodes:
                        return node
                        
                    # 标记节点为已处理
                    self.processed_nodes.add(id(node))
                    
                    # 先处理所有子节点，自底向上替换
                    self._process_children(node)
                    
                    # 再检查当前节点是否需要替换
                    if id(node) in self.changes_map:
                        replacement = self.changes_map[id(node)]
                        # 对替换后的节点再次遍历检查，但要避免循环处理
                        if id(replacement) not in self.processed_nodes:
                            self.processed_nodes.add(id(replacement))
                            replacement = self._process_children(replacement)
                        return replacement
                    
                    return node
                
                def _process_children(self, node):
                    """处理节点的所有子节点"""
                    if node is None:
                        return None
                        
                    for field_name, field in node.children():
                        if isinstance(field, c_ast.Node):
                            replacement = self.visit(field)
                            if replacement is not field:
                                if '[' in field_name and ']' in field_name:
                                    # 处理数组访问形式的属性 (例如: "exprs[0]")
                                    base_name = field_name.split('[')[0]
                                    index = int(field_name.split('[')[1].split(']')[0])
                                    attr_list = getattr(node, base_name)
                                    if index < len(attr_list):
                                        attr_list[index] = replacement
                                else:
                                    setattr(node, field_name, replacement)
                        elif isinstance(field, list):
                            for i, item in enumerate(field):
                                if isinstance(item, c_ast.Node):
                                    replacement = self.visit(item)
                                    if replacement is not item:
                                        field[i] = replacement
                    
                    return node
                
            transformer = Transformer(changes_map)
            return transformer.visit(ast)
    
        # 在每次遍历子节点之前，记录父子关系
        def _build_parent_map(self, node):
            for _, child in node.children():
                if isinstance(child, c_ast.Node):
                    self.parent_map[id(child)] = node
                    self._build_parent_map(child)
                elif isinstance(child, list):
                    for item in child:
                        if isinstance(item, c_ast.Node):
                            self.parent_map[id(item)] = node
                            self._build_parent_map(item)
        # 首先构建整个AST的父节点映射
        def process(self, ast):
            """处理整个AST，构建父节点映射并进行访问"""
            self.parent_map = {}
            self._build_parent_map(ast)  # 首先构建父节点映射
            self.visit(ast)  # 然后正常访问
            # 阶段3: 应用所有替换并返回修改后的AST
            return self.apply_changes(ast)

        # 辅助函数，根据节点ID查找节点
        def find_nodes_by_id(ast, target_id):
            """在AST中查找指定ID的节点"""
            result = []
            
            class IdFinder(c_ast.NodeVisitor):
                def generic_visit(self, node):
                    if id(node) == target_id:
                        result.append(node)
                    super().generic_visit(node)
            
            finder = IdFinder()
            finder.visit(ast)
            return result

         # 添加后处理方法，在通常的遍历完成后调用
        def post_process(self, ast):
            """对StructRef节点进行后处理"""
            # 识别所有需要后处理的StructRef节点
            nodes_to_process = []
            for node_id, (var_name, field_name) in self.struct_field_access.items():
                # 找到原始节点
                for node in self.find_nodes_by_id(ast, node_id):
                    if id(node) not in self.processed_nodes:
                        nodes_to_process.append((node, var_name, field_name))
            
            # 进行后处理
            for node, var_name, field_name in nodes_to_process:
                # 使用字段专门的getter
                getter_name = f"get_{var_name}_{field_name}_wrapper"
                replacement = c_ast.FuncCall(
                    name=c_ast.ID(name=getter_name),
                    args=c_ast.ExprList(exprs=[])
                )
                self.changes.append((node, replacement))
                
                # 记录替换信息
                line_no = node.coord.line if hasattr(node.coord, "line") else "unknown"
                if self.current_func not in self.func_replacements:
                    self.func_replacements[self.current_func] = []
                self.func_replacements[self.current_func].append((
                    f"{var_name}.{field_name}", 
                    "struct_field_getter", 
                    line_no
                ))
        
    
    # 处理每个非敏感域的文件AST
    replacer = GlobalVarReplacer(global_vars_set, struct_vars, var_to_getter, var_to_setter)
    for filename, ast in nonsense_domain_file_ast_dict.items():
        # try:
        # 创建访问器并处理AST
        
        replacer.current_file = filename
        
        # 使用安全的两阶段处理方法
        modi_ast = replacer.process(ast)
        
        # 记录被替换的函数信息
        for func_name in replacer.func_replacements:
            replaced_functions.append(func_name)
            
        # 保存修改后的AST
        nonsense_domain_modified_asts[filename] = modi_ast
            
        # except Exception as e:
        #     print(f"处理文件 {filename} 时出错: {str(e)}")
        #     import traceback
        #     traceback.print_exc()
        #     # 出错时保留原始AST
        #     nonsense_domain_modified_asts[filename] = ast
    
    print("replaced_functions:", replaced_functions)
    
          
    
def collect_function_pointers():
    """函数指针是存在于函数定义的参数和返回值，类型声明，结构体定义的成员变量中的"""
    class FunctionPointerCollector(c_ast.NodeVisitor):
        def __init__(self):
            self.function_pointers = set()
            self.current_func = None
        
        # def visit_FuncDecl(self, node):
        #     print(node)
        #     if proname not in str(node.coord):
        #         return
        #     # 检查函数声明的返回类型和参数类型
        #     if isinstance(node.type, c_ast.PtrDecl) and isinstance(node.type.type, c_ast.FuncDecl):
        #         # 如果是函数指针类型，记录下来
        #         func_type = c_generator.CGenerator().visit(node.type)
        #         self.function_pointers.add(func_type)
        def visit_Typedef(self, node):
            # print(node)
            # 检查typedef定义的类型
            if proname not in str(node.coord):
                return
            if isinstance(node.type, c_ast.PtrDecl) and isinstance(node.type.type, c_ast.FuncDecl):
                # 如果是函数指针类型，记录下来
                func_type = c_generator.CGenerator().visit(node.type)

                self.function_pointers.add(func_type)
                
            elif isinstance(node.type.type, c_ast.Struct):
                # 如果是结构体类型，检查结构体中的成员变量
                struct_node = node.type.type
                if struct_node.decls:
                    for decl in struct_node.decls:
                        if isinstance(decl.type, c_ast.PtrDecl) and isinstance(decl.type.type, c_ast.FuncDecl):
                            func_type = c_generator.CGenerator().visit(decl.type)
                            self.function_pointers.add(func_type)

            else:
                print(node)
        # def visit_Struct(self, node):
        #     if proname not in str(node.coord):
        #         return
        #     # 检查结构体定义中的成员变量
        #     print(node)
        #     if node.decls:
        #         for decl in node.decls:
        #             if isinstance(decl.type,c_ast.PtrDecl) and isinstance(decl.type.type, c_ast.FuncDecl):
        #                 # 如果是函数指针类型，记录下来      
        #                 func_type = c_generator.CGenerator().visit(decl.type)
        #                 self.function_pointers.add(func_type)
        #                 print("在结构体中找到函数指针类型:", func_type)
        def visit_Decl(self, node):
            if proname not in str(node.coord):
                return
            # 检查变量声明中的函数指针类型
            if isinstance(node.type,c_ast.FuncDecl):
                pass
            elif isinstance(node.type, c_ast.PtrDecl) and isinstance(node.type.type, c_ast.FuncDecl):
                # 如果是函数指针类型，记录下来
                func_type = c_generator.CGenerator().visit(node.type)
                self.function_pointers.add(func_type)
            elif isinstance(node.type, c_ast.Struct) :
                struct_node = node.type
                if struct_node.decls:
                    for decl in struct_node.decls:
                        if isinstance(decl.type, c_ast.PtrDecl) and isinstance(decl.type.type, c_ast.FuncDecl):
                            func_type = c_generator.CGenerator().visit(decl.type)
                            self.function_pointers.add(func_type)
            else:
                print(node)

   
    # collector = FunctionPointerCollector()
    # for filename, ast in file_ast_dict.items():
    #     collector.current_file = filename
    #     collector.visit(ast)
    #     # 将收集到的函数指针添加到全局变量
    #     print(f"在文件 {filename} 中收集到的函数指针: {collector.function_pointers}")
    # for func_ptr in collector.function_pointers:
    #     print(f"收集到的函数指针类型: {func_ptr}")

    #遍历indirect_call
    processed_fun=[]
    new_typedef_funptrtype=[]
    for ele in indirect_callpair:
        callee = ele[1]
        if callee in processed_fun:
            continue
        processed_fun.append(callee)
        print(f"处理间接调用: {callee}")
        if callee in fun_fundeclast_dict:
            func_decl_ast = fun_fundeclast_dict[callee]
            # print(f"函数声明AST: {c_generator.CGenerator().visit(func_decl_ast)}")
            declstr= c_generator.CGenerator().visit(func_decl_ast)
            wrapper_header_functions.append(declstr+";\n")
            
            # 如果是函数声明，转换为函数指针typedef
            if isinstance(func_decl_ast.type, c_ast.FuncDecl):
                # 创建函数指针类型
                func_ptr_type = c_ast.PtrDecl(
                    quals=[],
                    type=c_ast.FuncDecl(
                        args=copy.deepcopy(func_decl_ast.type.args),
                        type=copy.deepcopy(func_decl_ast.type.type)
                    )
                )
                # 创建类型名称 - 将函数名转换为驼峰式并添加"Func"后缀
                # 例如：my_function -> MyFunctionFunc
                func_type_name = callee + 'Func'
                new_typedef_funptrtype.append(func_type_name)
                # 创建typedef声明
                typedef_decl = c_ast.Typedef(
                    name=func_type_name,
                    quals=[],
                    storage=['typedef'],
                    type=func_ptr_type
                )
                print(typedef_decl)
                if isinstance(typedef_decl.type.type.type,c_ast.PtrDecl):
                    typedef_decl.type.type.type.type.declname = func_type_name
                else:
                    typedef_decl.type.type.type.declname = func_type_name
                # print(typedef_decl)
                new_funptr_def= c_generator.CGenerator().visit(typedef_decl)
                wrapper_header_functions.append(new_funptr_def+";\n")
                server_functions.append(new_funptr_def+";\n")
                
                
    """   typedef enum {
    SIG_PARSE_OPT,         // error_t (*)(int, char*, struct argp_state*)
    SIG_INT_ARGC_ARGV,     // int (*)(int, char*[])
    SIG_SETSOCKOPT,        // int (*)(int, int, int, int)
    SIG_UNKNOWN            // 未知签名
} FunctionSignature;             """
#将new_typedef_funptrtype中的函数指针类型名定义为以上枚举签名
    enum_signatrue_str=f"typedef enum {{\n"
    for i, func_type_name in enumerate(new_typedef_funptrtype):
        enum_signatrue_str += f"    SIG_{func_type_name.upper()},\n"
    enum_signatrue_str += f"    SIG_UNKNOWN\n}} FunctionSignature;\n"
    print("生成的函数指针类型枚举签名:")
    print(enum_signatrue_str)
    wrapper_header_functions.append(enum_signatrue_str)
    server_functions.append(enum_signatrue_str)

    """typedef struct {
    const char *name;         // 函数名
    void *func_ptr_void;      // 存储为 void* 的函数地址
    FunctionSignature signature; // 函数签名标识符
} FunctionMapEntry;"""
    #生成函数映射表条目
    fun_map_str = "typedef struct {\n"
    fun_map_str += "    const char *name;         // 函数名\n"
    fun_map_str += "    void *func_ptr_void;      // 存储为 void* 的函数地址\n"
    fun_map_str += "    FunctionSignature signature; // 函数签名标识符\n"
    fun_map_str += "} FunctionMapEntry;\n"
    print("生成的函数映射表条目:")
    print(fun_map_str)
    wrapper_header_functions.append(fun_map_str)
    server_functions.append(fun_map_str)
    """extern FunctionMapEntry function_map[]; """
    fun_map_str = "extern FunctionMapEntry function_map[];\n"
    wrapper_header_functions.append(fun_map_str)
    """FunctionMapEntry function_map[] = {
    {"parse_opt", (void*)parse_opt, SIG_PARSE_OPT},
    {"tn", (void*)tn, SIG_INT_ARGC_ARGV},
    {"status", (void*)status, SIG_INT_ARGC_ARGV},
    {"SetSockOpt", (void*)SetSockOpt, SIG_SETSOCKOPT},
    {"bye", (void*)bye, SIG_INT_ARGC_ARGV},
    {NULL, NULL, SIG_UNKNOWN} // 哨兵值
};"""

    temp_str="void* find_function_ptr_by_name(const char* func_name);\n"
    wrapper_header_functions.append(temp_str)
    temp_str="const char* find_function_name_by_address(void* func_address);"
    wrapper_header_functions.append(temp_str)
    #生成函数映射表
    function_map_str = "FunctionMapEntry function_map[] = {\n"
    for func_type_name in new_typedef_funptrtype:
        func_name = func_type_name[:-4]
        function_map_str += f'    {{"{func_name}", (void*){func_name}, SIG_{func_type_name.upper()}}},\n'
    function_map_str += '    {NULL, NULL, SIG_UNKNOWN} // 哨兵值\n'
    function_map_str += "};\n"
    print("生成的函数映射表:")
    print(function_map_str)
    wrapper_functions.append(function_map_str)
    server_functions.append(function_map_str)
    """// --- 查找函数名：根据函数名查找函数条目 ---
FunctionMapEntry* find_function_entry_by_name(const char* func_name) {
    for (int i = 0; function_map[i].name != NULL; i++) {
        if (strcmp(function_map[i].name, func_name) == 0) {
            return &function_map[i];
        }
    }
    return NULL; // 未找到
}"""
    #生成查找函数条目的函数
    find_function_ptr_str = """// --- 查找函数名：根据函数名查找函数条目 ---
void* find_function_entry_by_name(const char* func_name) {
    for (int i = 0; function_map[i].name != NULL; i++) {
        if (strcmp(function_map[i].name, func_name) == 0) {
            return &function_map[i].func_ptr_void;
        }
    }
    return NULL; // 未找到
}
"""
    print("生成的查找函数条目的函数:")
    print(find_function_ptr_str)
    wrapper_functions.append(find_function_ptr_str)
    server_functions.append(find_function_ptr_str)

    """// --- 新增函数：根据函数地址查找函数名 ---
const char* find_function_name_by_address(void* func_address) {
    for (int i = 0; function_map[i].name != NULL; i++) {
        // 直接比较 void* 指针
        if (function_map[i].func_ptr_void == func_address) {
            return function_map[i].name;
        }
    }
    return NULL; // 未找到对应的函数名
}"""

    #生成根据函数地址查找函数名的函数
    find_function_name_str = """// --- 新增函数：根据函数地址查找函数名 ---
const char* find_function_name_by_address(void* func_address) {
    for (int i = 0; function_map[i].name != NULL; i++) {
        // 直接比较 void* 指针
        if (function_map[i].func_ptr_void == func_address) {
            return function_map[i].name;
        }
    }
    return NULL; // 未找到对应的函数名
}
"""
    print("生成的根据函数地址查找函数名的函数:")
    print(find_function_name_str)
    wrapper_functions.append(find_function_name_str)
    server_functions.append(find_function_name_str)

    """
    char *get_filename_from_fp(FILE *fp) {
    if (!fp) {
        fprintf(stderr, "Error: FILE pointer is NULL\n");
        return NULL;
    }
     if (fp == stdin || fp == stdout || fp == stderr) {
        // 对于标准输入输出流，直接返回对应的名称
        if (fp == stdin) return strdup("stdin");
        if (fp == stdout) return strdup("stdout");
        if (fp == stderr) return strdup("stderr");
    }
    // 获取文件描述符
    int fd = fileno(fp);
    if (fd == -1) {
        fprintf(stderr, "Error: fileno failed \n" );
        return NULL;
    }
    // 构造 /proc/self/fd/<fd> 路径
    char fd_path[64];
    snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);
    // 分配内存存储文件名
    char *filename = malloc(PATH_MAX);
    if (!filename) {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }
    // 读取符号链接目标（文件名）
    ssize_t len = readlink(fd_path, filename, PATH_MAX - 1);
    if (len == -1) {
        fprintf(stderr, "Error: readlink failed for %s \n", fd_path);
        free(filename);
        return NULL;
    }
    filename[len] = '\0'; // 确保字符串终止
    return filename;
}
    """
    temp_str = """char *get_filename_from_fp(FILE *fp) {
    if (!fp) {                                                                 
        fprintf(stderr, "Error: FILE pointer is NULL\\n");
        return NULL;
    }
    // 获取文件描述符
    int fd = fileno(fp);
    if (fd == -1) {
        fprintf(stderr, "Error: fileno failed");
        return NULL;
    }
    // 构造 /proc/self/fd/<fd> 路径
    char fd_path[64];   
    snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);
    // 分配内存存储文件名
    char *filename = malloc(PATH_MAX);
    if (!filename) {
        fprintf(stderr, "Error: malloc failed");
        return NULL;
    }
    // 读取符号链接目标（文件名）
    ssize_t len = readlink(fd_path, filename, PATH_MAX - 1);
    if (len == -1) {
        fprintf(stderr, "Error: readlink failed for %s", fd_path);
        free(filename);
        return NULL;
    }
    filename[len] = '\\0'; // 确保字符串终止
    return filename;
}
"""
    print("生成的获取文件名的函数:")
    print(temp_str)
    wrapper_functions.append(temp_str)
    wrapper_header_functions.append(f"char *get_filename_from_fp(FILE *fp);\n")
    server_functions.append(temp_str)
    
        


  



        
        
            
   

# 处理全局变量
def analyze_global_variables():
    class GlobalVarCollector(c_ast.NodeVisitor):
        def __init__(self):
            self.globals_used = set()
            self.visiting_function = None
        
        def visit_FuncDef(self, node):
            self.visiting_function = node.decl.name
            self.generic_visit(node)
        
        def visit_ID(self, node):
            # 如果ID是全局变量，则添加到集合中
            if node.name in global_var_asts:
                self.globals_used.add(node.name)

    #==========================================
    all_non_sense_global_var = set()
    # 分析敏感域和非敏感域使用的全局变量
    print("正在分析全局变量使用情况...")
    collector = GlobalVarCollector()
    for fun, gvallist in fun_gvallist.items():
        print(fun)
        if fun in sense_domain:
            sense_global_var.update(gvallist)
        #如果函数名以_sense_1结尾，遍历其AST，找到其中的全局变量,其ast在refactor_fun_ast_dict中
        if fun+"_sense_1" in sense_domain:
            version_sense=fun+"_sense_1"
            if version_sense in refactor_fun_ast_dict:
                print("here is mixed sub sense")
                # 获取函数AST并收集全局变量
                func_ast = refactor_fun_ast_dict[version_sense]
                print(c_generator.CGenerator().visit(func_ast))
                collector.visit(func_ast)
                # 更新敏感函数使用的全局变量集合
                sense_global_var.update(collector.globals_used)
                print(collector.globals_used)
            if fun in refactor_fun_ast_dict:
                    # 获取重构后的敏感函数AST并收集全局变量
                    rfunc_ast = refactor_fun_ast_dict[fun]
                    collector.globals_used.clear()
                    collector.visit(rfunc_ast)
                    print(c_generator.CGenerator().visit(rfunc_ast))
                    all_non_sense_global_var.update(collector.globals_used)
                    print("here is mixed parent nosense")
                    print(collector.globals_used)

        if fun in nonsense_domain:
            #如果fun+“_sense_1”是在sense_domain中，则遍历其对应的ast，在refactor_fun_ast_dict中,重新手机全局变量
            if fun+"_sense_1" in sense_domain:
                continue
            all_non_sense_global_var.update(gvallist)
    
    # 找出敏感域和非敏感域共享的变量
    shared_vars = sense_global_var.intersection(all_non_sense_global_var)
    
    # 识别共享的只读全局变量
    D_sense_global_var = set()
    for gv in shared_vars:
        if gv in gval_rwproperty_dict and gval_rwproperty_dict[gv] == "not-read-only":
            D_sense_global_var.add(gv)
        else:
            # 记录读写共享变量，这些可能需要特别注意
            print(f"警告: 变量 {gv} 在敏感和非敏感域之间共享且不是只读的")
    
    # 确定敏感域专用的全局变量和非敏感域专用的全局变量
    nonsense_global_var.update(all_non_sense_global_var - D_sense_global_var)
    
    # 输出统计信息
    print(f"敏感域使用的全局变量总数: {len(sense_global_var)}")
    print(f"非敏感域使用的全局变量总数: {len(all_non_sense_global_var)}")
    # print(f"共享的只读全局变量: {len(R_global_var)}"):
    print(f"需要通过RPC访问的敏感域全局变量: {len(D_sense_global_var)}")
    
    return D_sense_global_var



def rpc_main(proname,abs_path_policy_file):
    # preprocess_c_file_and_parse_toAST(proname) #此函数不在main函数中调用，而是在driver脚本中调用，以确保只用预处理和解析为ast一次
    #运行方式：python refator_mixed_function.py wget，需要在当前目录下有compile_commands.json文件，以及wget_sense_config.txt文件
    # file_ast_dict={} #存放文件名和对应的AST
    file_ast_dict.clear()
    sense_fun_list.clear()
    sense_fun_status.clear()
    non_sense_fun_list.clear()
    # sense_fundecl_list={} #存放sense函数的声明
    non_sense_fundecl_list.clear()
    call_list.clear()
    File_Node_dict.clear()
    Fun_Ast_dict.clear()
    sensefun_can_copy.clear()
    nonsense_upto_sense_function.clear()
    file_refactorFun_ast_dict.clear()
    boundry_call_need_rpc.clear()
    nonsense_to_sense_call.clear()
    fun_fundeclast_dict.clear()
    typedef_dict.clear()
    struct_dict.clear()

   

    preprocess_c_file_and_parse_toAST(proname)
    parse_global_var_usage("../partitioned_software/"+proname+"/2_flowcheck_result/"+proname+"_global_var_usage.xml")
    refactor_mixed_function_ast(abs_path_policy_file)
    parse_dynamic_callpair(proname)
    find_nonsense_upto_sense_duplicate()
    find_sense_down_to_nonsese_dipication()
    # 收集全局变量AST信息
    global_var_asts = collect_global_var_asts(file_ast_dict)  # 在这里添加，初始化global_var_asts

    # 处理全局变量
    sense_global_var = set()
    all_non_sense_global_var = set()
    R_global_var = set()
    
    # 分析敏感域和非敏感域使用的全局变量
    for fun, gvallist in fun_gvallist.items():
        if fun in sense_domain:
            sense_global_var = sense_global_var.union(gvallist)
        if fun in nonsense_domain:
            all_non_sense_global_var = all_non_sense_global_var.union(gvallist)
    
    # 找出共享的只读全局变量
    for gv in sense_global_var.intersection(all_non_sense_global_var):
        if gval_rwproperty_dict[gv] == "read-only":
            R_global_var.add(gv)
            
    # 确定敏感域专用的全局变量和非敏感域专用的全局变量
    D_sense_global_var = sense_global_var.difference(R_global_var)
    non_sense_global_var = all_non_sense_global_var.difference(D_sense_global_var)
    

    ret_boundry_call_need_rpc=judge_boundary_call_need_rpc() #找到需要rpc的边界调用,边界调用中的ccallee函数是需要生成idl和wrapper的，callsite是需要改成rpc调用的
    
    # 生成IDL和Wrapper函数
    generate_idl_file(ret_boundry_call_need_rpc)
    generate_wrapper_file(ret_boundry_call_need_rpc)
    # 为敏感全局变量生成RPC访问函数
    generate_global_var_rpc_functions(proname, D_sense_global_var)
    
    
    # 替换非敏感域中对敏感全局变量的直接访问
    # replace_global_var_access_with_rpc(proname, D_sense_global_var)
    
    rpcc_call_pair=[]#存放rpc调用的函数对，caller在non_sense_domain中，callee在sense_domain中
    print("len(sum_fun_list):",len(sum_fun_list))
    print("len(sense_fun_list):",len(sense_fun_list))
    print("len(non_sense_fun_list):",len(non_sense_fun_list))
    print("len(nonsense_upto_sense_function)+len(sense_fun_list):",len(nonsense_upto_sense_function)+len(sense_fun_list))
    print("len(sensefun_can_copy)+len(non_sense_fun_list):",len(sensefun_can_copy)+len(non_sense_fun_list))
    #算算sum_fun_list和sensefun_can_copy+non_sense_fun_list的差集并打印
    print("sum_fun_list-sensefun_can_copy-non_sense_fun_list:")
    for fun in sum_fun_list:
        if fun not in set(sensefun_can_copy).union(non_sense_fun_list):
            print(fun)
    print()
    print("sense_domain:")
    for fun in nonsense_upto_sense_function.union(sense_fun_list):
        if fun not in sense_domain:
            sense_domain.append(fun)
            print(fun)
    print()
    print("nonsense_domain:")
    for fun in set(sensefun_can_copy).union(non_sense_fun_list):
        if fun not in nonsense_domain:
            nonsense_domain.append(fun) 
            print(fun)
    for call in ret_boundry_call_need_rpc:
        caller=call[0]
        callee=call[1]
        if caller not in nonsense_upto_sense_function:
            rpcc_call_pair.append((caller,callee))



    output_auto_rpc_code(proname)


if __name__ == '__main__':
    proname=sys.argv[1]

    so_type = "b"  # 默认为 'b'
    quan_str = "32"  # 默认量化位数
    
    for arg in sys.argv:
        if arg.startswith("--comm-type="):
            so_type = arg.split("=")[1]
        elif arg.startswith("--quan="):
            quan_str = arg.split("=")[1]

    # 设置项目目录和解压源码
    abs_path_policy_file, proj_dir, proj_back_dir = setup_project_directories(proname, so_type, quan_str)
    
    # 运行方式：python refator_mixed_function.py wget，需要在当前目录下有compile_commands.json文件，以及wget_sense_config.txt文件
    preprocess_c_file_and_parse_toAST(proname)

    print(global_src_dir)
    parse_global_var_usage(f"examples/{proname}/output/global_val_use.xml")
    refactor_mixed_function_ast(abs_path_policy_file)
    # #输出file_ast_dict中的ast
    # print("file_ast_dict:")
    # for filename, ast in file_ast_dict.items():
    #     print(filename)
    #     print(c_generator.CGenerator().visit(ast))
    # parse_dynamic_callpair(proname)

    print("sense_fun_list:")
    for fun in sense_fun_list:
        print(fun)
    print()
    
    generator=c_generator.CGenerator()

    find_nonsense_upto_sense_duplicate()
    find_sense_down_to_nonsese_dipication()


    # 收集全局变量AST信息

    
    global_var_asts = collect_global_var_asts(file_ast_dict)  # 在这里添加，初始化global_var_asts
    D_sense_global_var=analyze_global_variables()

    # # 处理全局变量
    # all_non_sense_global_var = set()
    # R_global_var = set()
    
    # # 分析敏感域和非敏感域使用的全局变量
    # for fun, gvallist in fun_gvallist.items():
    #     if fun in sense_domain:
    #         sense_global_var = sense_global_var.union(gvallist)
    #     if fun in nonsense_domain:
    #         all_non_sense_global_var = all_non_sense_global_var.union(gvallist)
    
    # # 找出共享的只读全局变量
    # for gv in sense_global_var.intersection(all_non_sense_global_var):
    #     if gval_rwproperty_dict[gv] == "read-only":
    #         R_global_var.add(gv)
            
    # # 确定敏感域专用的全局变量和非敏感域专用的全局变量
    # D_sense_global_var = sense_global_var.difference(R_global_var)
    # nonsense_global_var.update( all_non_sense_global_var.difference(D_sense_global_var))

    """
    以上步骤分析完成之后知道了最终的sense_domain和nonsense_domain，以及sense_domain和nonsense_domain中的函数，
    最终的两个domain中分别存放的全局变量，以及需要通过rpc访问的全局变量，接下来就是生成idl和wrapper文件，
    以及生成rpc访问函数，然后替换非敏感域中对敏感全局变量的直接访问，最后生成rpc调用的代码
    """

    print()
    print("sense_domain:")
    for fun in sense_domain:
            print(fun)
    print()
    print("nonsense_domain:")
    for fun in nonsense_domain:
            print(fun)
    print()
    print("D_sense_global_var:")
    for var in D_sense_global_var:
        print(var)
    print()
    print("nonsense_global_var:")
    for var in nonsense_global_var:
        print(var)
    print()
    print("sense_global_var:")
    for var in sense_global_var:
        print(var)
   
    

    ret_boundry_call_need_rpc=judge_boundary_call_need_rpc() #找到需要rpc的边界调用,边界调用中的ccallee函数是需要生成idl和wrapper的，callsite是需要改成rpc调用的
    
    # 生成IDL和Wrapper函数
    generate_idl_file(ret_boundry_call_need_rpc)
    generate_wrapper_file(ret_boundry_call_need_rpc)
    # 为敏感全局变量生成RPC访问函数
    generate_global_var_rpc_functions(proname, D_sense_global_var)
    
    
    # 替换非敏感域中对敏感全局变量的直接访问
    collect_function_pointers()
    replace_global_var_access_with_rpc(proname, D_sense_global_var)
    append_to_files(proname, idl_functions, idl_structs, server_functions, wrapper_functions, wrapper_header_functions)
    
    rpcc_call_pair=[]#存放rpc调用的函数对，caller在non_sense_domain中，callee在sense_domain中
    print("len(sum_fun_list):",len(sum_fun_list))
    print("len(sense_fun_list):",len(sense_fun_list))
    print("len(non_sense_fun_list):",len(non_sense_fun_list))
    print("len(nonsense_upto_sense_function)+len(sense_fun_list):",len(nonsense_upto_sense_function)+len(sense_fun_list))
    print("len(sensefun_can_copy)+len(non_sense_fun_list):",len(sensefun_can_copy)+len(non_sense_fun_list))
    #算算sum_fun_list和sensefun_can_copy+non_sense_fun_list的差集并打印
    print("sum_fun_list-sensefun_can_copy-non_sense_fun_list:")
    print("len(sense_domain):",len(sense_domain))
    print("len(nonsense_domain):",len(nonsense_domain))
    for fun in sum_fun_list:
        if fun not in set(sensefun_can_copy).union(non_sense_fun_list):
            print(fun)
    
    for call in ret_boundry_call_need_rpc:
        caller=call[0]
        callee=call[1]
        if caller not in nonsense_upto_sense_function:
            rpcc_call_pair.append((caller,callee))


    # #写一个函数修改refactor_fun_ast_dict中的ast，遍历非_sense_1结尾的函数，找到其中的FunccaLL节点，如果callee的函数名为sense_1结尾，在callee函数名后添加_wrapper
    # def add_wrapper_to_callee():
    #     for fun,ast in refactor_fun_ast_dict.items():
    #         print("modi_wrapper:",fun)
    #         if not fun.endswith("_sense_1"):
    #             print("fun:",fun)
    #             #遍历ast，找到FuncaLL节点，如果callee的函数名为sense_1结尾，在callee函数名后添加_wrapper
    #             class AddWrapper(c_ast.NodeVisitor):
    #                 def visit_FuncCall(self, node):
    #                     if isinstance(node.name,c_ast.ID):
    #                         if node.name.name.endswith("_sense_1"):
    #                             print("here")
    #                             node.name.name=node.name.name+"_wrapper"
    #             addwrapper=AddWrapper()
    #             addwrapper.visit(ast)
                
    # add_wrapper_to_callee()



    output_auto_rpc_code(proname)

    output_alive_verify_code(proname)
    
    # output_auto_rpc_code()
    # print("======================================")
    # # for fundecl,ast in fun_fundeclast_dict.items():
    # #     print(fundecl)
    # #     print(c_generator.CGenerator().visit(ast))
    # generate_idl_file(boundry_call_need_rpc)
    # generate_wrapper_file(boundry_call_need_rpc)
    # for fun in sense_fundecl_list:
    #     print(c_generator.CGenerator().visit(fun))