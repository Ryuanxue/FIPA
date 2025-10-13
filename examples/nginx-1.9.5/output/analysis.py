
import re
from collections import defaultdict

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



def classify_functions(sensitive_nodes, non_sensitive_nodes):
        function_status = defaultdict(lambda: {'sensitive': False, 'non_sensitive': False})#定义一个字典，key为函数名，value为一个字典，包含sensitive和non_sensitive两个key
        function_groups = defaultdict(list)  # 新增的字典，用于存放根据函数名分组的节点
        #更新这个函数将sensitive_nodes和non_sensitive_nodes中按照function分类，将function对应的sensitive和non_sensitive的值更新为True
        for node in sensitive_nodes + non_sensitive_nodes:
            if node:
                function_groups[node['function']].append(node)
                func_status = function_status[node['function']]
                
                if node in sensitive_nodes:
                    func_status['sensitive'] = True
                if node in non_sensitive_nodes:
                    func_status['non_sensitive'] = True

        # # 检查 function_status如果一个函数既有sensitive又有non_sensitive，则打印此函数对应的节点
        # for function, status in function_status.items():
        #     if status['sensitive'] and status['non_sensitive']:
        #         #检查函数的节点的quantity，如果全为0，将这个函数的状态sensitive改为False
        #         #如果这个函数中quantity为0的节点的类型全是DeclStmt,将这个函数的状态non_sensitive改为False
        #         all_quantity_zero = all(node['quantity'] == 0 for node in function_groups[function])
        #         all_decl_stmt = all(node.get('type') == 'DeclStmt' for node in function_groups[function] if node['quantity'] == 0)
                
        #         if function in ['mkalldirs','logvprintf']:
        #             function_status[function]['non_sensitive'] = False
        #             continue

        #         # 如果全为0，将这个函数的状态sensitive改为False
        #         if all_quantity_zero:
        #             function_status[function]['sensitive'] = False
        #             continue

        #         # 如果这个函数中quantity为0的节点的类型全是DeclStmt,将这个函数的状态non_sensitive改为False
        #         if all_decl_stmt:
        #             function_status[function]['non_sensitive'] = False
        #             continue
        return function_status

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

        #打印mixed_sensitive_nodes
        for node in mixed_sensitive_nodes:
            print("mixed sensitive node is: " + str(node))
        
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






# file="no_rpc_ns_result.txt"
# #read file
# with open(file, 'r') as f:
#     lines = f.readlines()

# sense_fun_list = []
# non_sense_fun_list = []
# mixed_fun_list = []
# issensitive = False
# isnosensitive = False
# for line in lines:
#     #跳过空行
#     if line.strip() == "":
#         continue
#     #split line
#     line = line.strip()
#     if line.startswith("sense_fun_list"):
#         issensitive = True
#         isnosensitive = False
#         continue
#     if line.startswith("non_sense_fun_list:"):
#         isnosensitive = True
#         issensitive = False
#         continue
#     if issensitive:
#         if line.endswith("_sense_1"):
#             #去除后缀"_sense_1"
#             line = line[:-8]
#             # print("line:", line)
#             mixed_fun_list.append(line)
#         else:
#             sense_fun_list.append(line)
#     if isnosensitive:
#         non_sense_fun_list.append(line)

# print(len(sense_fun_list)+len(non_sense_fun_list))

#判断sense_fun_list和non_sense_fun_list是否有交集
# intersection = set(sense_fun_list) & set(non_sense_fun_list)
# if intersection:
#     print("sense_fun_list和non_sense_fun_list有交集:")
#     print(intersection)

# print("len(sense_fun_list):", len(sense_fun_list))
# print("len(non_sense_fun_list):", len(non_sense_fun_list))
# print("len(mixed_fun_list):", len(mixed_fun_list))



"""
shard.txt的文件内容如下:
函数: init_terminal
  调用次数: 2
  敏感调用次数: 1
  每次调用的敏感性: [False, True]
  敏感性一致: 否

函数: ring_init
  调用次数: 8
  敏感调用次数: 3
  每次调用的敏感性: [False, False, False, False, True, False, True, True]
  敏感性一致: 否

函数: TerminalAutoFlush
  调用次数: 2
  敏感调用次数: 0
  每次调用的敏感性: [False, False]

函数: init_network
  调用次数: 2
  敏感调用次数: 1
  每次调用的敏感性: [False, True]
  敏感性一致: 否

  当识别到“敏感性一致”开始的行的时候，判断状态为“是”还是“否”，如果是“是”，向前取5行，取出函数名，存入quanallTrue_list列表中，如果是“否”，向前取5行，取出函数名，存入shard_fun_list列表中

  若敏感性一致为是，则此函数是不可以copy的，其余的函数都是可以copy到不敏感域的

"""
may_shard_funlist=[]
shard_fun_list = []
quanallTrue_list = []
shared_file="temp/prepare_shard.txt"
with open(shared_file, 'r') as f:
    lines = f.readlines()
for line in lines:
    #split line
    line = line.strip()
    if line.startswith("函数:"):
        func_name = line.split(":")[1].strip()
        may_shard_funlist.append(func_name)
        continue
    if line.startswith("敏感性一致:"):
        if "是" in line:
            may_shard_funlist.remove(func_name)
            if func_name not in quanallTrue_list:
                quanallTrue_list.append(func_name)
                
        else:
            if func_name not in shard_fun_list:
                shard_fun_list.append(func_name)

print("len(shard_fun_list):", len(shard_fun_list))
print("len(quanallTrue_list):", len(quanallTrue_list))

# #找出shard_fun_list和mixed_fun_list的交集
# print("shard_fun_list与mixed_fun_list的交集函数名:")
# intersection_list = []
# for fun in shard_fun_list:
#     if fun in mixed_fun_list:
#         print("交集函数名:", fun)


print()
# print("quanallTrue_list与mixed_fun_list的交集函数名:")
# #找出quanallTrue_list和mixed_fun_list的交集
# intersection_list = []
# for fun in quanallTrue_list:
#     if fun in mixed_fun_list:
#         print("交集函数名:", fun)

#打印quanallTrue_list与sense_fun_list+mixed_fun_list的差集
# print()
# print("quanallTrue_list与sense_fun_list+mixed_fun_list的差集函数名:")
# #找出quanallTrue_list和sense_fun_list的差集
# difference_list = []
# for fun in quanallTrue_list:
#     if fun not in sense_fun_list and fun not in mixed_fun_list:
#         print("差集函数名:", fun)

# print()
# print("shard_fun_list与sense_fun_list的交集函数名:")
# #找出shard_fun_list和sense_fun_list的交集
# intersection_list = []
# for fun in shard_fun_list:
#     if fun in sense_fun_list:
#         print("交集函数名:", fun)

#找出quanallTrue_list和sense_fun_list的交集
# print()
# print("quanallTrue_list与sense_fun_list的交集函数名:")
# intersection_list = []
# for fun in quanallTrue_list:
#     if fun in sense_fun_list:
#         print("交集函数名:", fun)

#将sense_fun_list和mixed_fun_list做并集
#将shard_fun_list和quanallTrue_list做并集
#然后将两个列表做差集
# print()
# print("sense_fun_list和mixed_fun_list的并集函数名:")
# aunion_list = list(set(sense_fun_list) | set(mixed_fun_list))
# print("len(aunion_list):", len(aunion_list))
# print("len(shard_fun_list):", len(shard_fun_list))
# print("len(quanallTrue_list):", len(quanallTrue_list))
# #打印shard_fun_list的函数名
# print("shard_fun_list的函数名:")
# for fun in shard_fun_list:
#     print("函数名:", fun)
# print()
# #打印quanallTrue_list的函数名
# print("quanallTrue_list的函数名:")
# for fun in quanallTrue_list:
#     print("函数名:", fun)
# print()

# bunion_list = list(set(shard_fun_list) | set(quanallTrue_list))
# print("len(bunion_list):", len(bunion_list))

# #打印shard_fun_list和quanallTrue_list的交 集
# print("shard_fun_list和quanallTrue_list的交集函数名:")
# intersection_list = []
# for fun in shard_fun_list:
#     if fun in quanallTrue_list:
#         print("交集函数名:", fun)



#分析敏感代码的比例
sense_config_file_path = "minsencode_u_64bit.txt"  # 假设配置文件名为 sense_config.txt
sensitive_nodes, non_sensitive_nodes = parse_sense_config(sense_config_file_path)#解析sense_config文件，获得sensitive_nodes和non_sensitive_nodes
function_status = classify_functions(sensitive_nodes, non_sensitive_nodes)
mixed_functions = [func for func, status in function_status.items() if status['sensitive'] and status['non_sensitive']]
#获得所有敏感函数
sensitive_functions = [func for func, status in function_status.items() if status['sensitive'] and not status['non_sensitive']]
#获得所有非敏感函数
non_sensitive_functions = [func for func, status in function_status.items() if not status['sensitive'] and status['non_sensitive']]
print("所有敏感函数:")
print(len(sensitive_functions))
for func in sensitive_functions:
    print("敏感函数名:", func)
print("所有非敏感函数:", non_sensitive_functions)
print("所有mixed函数:")
print(len(mixed_functions))
for func in mixed_functions:
    print("mixed函数名:", func)
print("所有函数的数量:", len(function_status))
print("所有敏感函数的数量:", len(sensitive_functions))
print("所有非敏感函数的数量:", len(non_sensitive_functions))
print("所有mixed函数的数量:", len(mixed_functions))

#所有sensitive_functions和quanallTrue_list的交集
s_onlyfunction_list = (set(quanallTrue_list) & set(sensitive_functions)) 
print("quanallTrue_list和sensitive_functions的交集函数名:")
print("S-only函数的数量:", len(s_onlyfunction_list))
# for func in quan_list:
#     print("交集函数名:", func)
print("sensitive_functions与quan_list的差集函数名:")
#sensitive_functions与quan_list的差集
common_list = set(sensitive_functions) - set(s_onlyfunction_list)
print("may_shard_funlist函数名:", may_shard_funlist   )
new_common_list = set(sensitive_functions) & set(may_shard_funlist)
print("old共享函数的数量:", len(common_list))
print("old共享函数名:", common_list)
print("new共享函数的数量:", len(new_common_list))
print("new共享函数名:", new_common_list)
# for func in difference_list:
#     print("共享函数名:", func)



#求敏感代码所占的比例
"""
1.获得sdomain的代码， commondomain的大小， ndomain的大小
sdomain=s-onlyfunction_list+mixed_functions中的敏感部分
ndomain=non_sensitive_functions+ mixed_functions中的非敏感部分
conmondomain=common_list中的部分
2.计算比例
   1）获得s-onlyfunction_list的所有size
   2)获得mixed_functions中的敏感部分的所有size
   3)获得non_sensitive_functions的所有size
   4)获得mixed_functions中的非敏感部分的所有size
   5)获得common_list的所有size

"""
sdomain_size = 0
for func in s_onlyfunction_list:
    mixed_sensitive_nodes = [node for node in (sensitive_nodes + non_sensitive_nodes) if node and node['function'] == func]
    if mixed_sensitive_nodes:
        fun_size = sum(node['size'] for node in mixed_sensitive_nodes)
        sdomain_size += fun_size
print("sdomain的代码总大小:", sdomain_size)
ndomain_size = 0
for func in non_sensitive_functions:
    mixed_non_sensitive_nodes = [node for node in (sensitive_nodes + non_sensitive_nodes) if node and node['function'] == func]
    if mixed_non_sensitive_nodes:
        fun_size = sum(node['size'] for node in mixed_non_sensitive_nodes)
        ndomain_size += fun_size
print("ndomain的代码总大小:", ndomain_size)
common_size = 0
for func in new_common_list:
    mixed_sensitive_nodes = [node for node in (sensitive_nodes + non_sensitive_nodes) if node and node['function'] == func]
    if mixed_sensitive_nodes:
        fun_size = sum(node['size'] for node in mixed_sensitive_nodes)
        common_size += fun_size
print("conmondomain的代码总大小:", common_size)
#统计mixed_functions中的敏感代码比例
mixed_sensitive_size = 0
for func in mixed_functions:
    mixed_sensitive_nodes = [node for node in (sensitive_nodes + non_sensitive_nodes) if node and node['function'] == func]
    if mixed_sensitive_nodes:
        for node in mixed_sensitive_nodes:
            if node['issensitive']:
                mixed_sensitive_size += node['size']
print("mixed_functions中的敏感代码总大小:", mixed_sensitive_size)
#统计mixed_functions中的非敏感代码比例
mixed_non_sensitive_size = 0
for func in mixed_functions:
    mixed_non_sensitive_nodes = [node for node in (sensitive_nodes + non_sensitive_nodes) if node and node['function'] == func]
    if mixed_non_sensitive_nodes:
        for node in mixed_non_sensitive_nodes:
            if not node['issensitive']:
                mixed_non_sensitive_size += node['size']
print("mixed_functions中的非敏感代码总大小:", mixed_non_sensitive_size)
#打印比例
sensitive_codesize= sdomain_size + mixed_sensitive_size+ common_size
all_codesize = sdomain_size + ndomain_size + mixed_sensitive_size + mixed_non_sensitive_size + common_size+ common_size
print("敏感代码所占比例:", sensitive_codesize / all_codesize)




total_mixed_size = 0
for func in mixed_functions:
    print("mixed function is: " + func)
    mixed_sensitive_nodes = [node for node in (sensitive_nodes + non_sensitive_nodes) if node and node['function'] == func]
    sensitive_blocks = extract_sensitive_blocks(mixed_sensitive_nodes)
    if len(sensitive_blocks) == 0 or len(sensitive_blocks[0]) == len(mixed_sensitive_nodes):
        continue
    print("sensitive_blocks:")
    for block in sensitive_blocks:
        #统计mixed中的敏感代码比例
        block_size = sum(node['size'] for node in block)
        total_mixed_size += block_size
        for node in block:
            print(node)
        print()
print("所有mixed_functions中的敏感代码总大小:", total_mixed_size)
#统计所有quanallTrue_list中的函数的敏感代码比例
total_size=0
for func in quanallTrue_list:
    mixed_sensitive_nodes = [node for node in (sensitive_nodes + non_sensitive_nodes) if node and node['function'] == func]
    if mixed_sensitive_nodes:
        fun_size = sum(node['size'] for node in mixed_sensitive_nodes)
        total_size += fun_size
print("所有quanallTrue_list中的函数的敏感代码总大小:", total_size)

#统计所有函数的代码size
total_all_size = 0
for node in sensitive_nodes + non_sensitive_nodes:
    if node:
        total_all_size += node['size']
print("所有函数的代码总大小:", total_all_size)
        
# 打印敏感代码比例

#读取nginx-1.9.5output1.txt文件，   获得个以callpair开头的行，提取caller和callee函数名，存入call_pairs列表中,每行数据格式如下：
# callpair: caller:ngx_http_core_access_phase,callee:ngx_http_auth_basic_handler, location:0x187885: ngx_http_auth_basic_handler (/Desktop/IF-driver-partition/partitioned_software/nginx-1.15.5/1_sensitive-annotation-code/nginx-1.15.5/src/http/modules/ngx_http_auth_basic_module.c:92)

call_pairs = []
with open("temp/nginx-1.9.5output1.fc", 'r') as f:
    lines = f.readlines()

for line in lines:
    line = line.strip()
    if line.startswith("callpair"):
        match = re.match(r'callpair: caller:(.+?),callee:(.+?), location:(.+)', line)
        if match:
            caller = match.group(1).strip()
            callee = match.group(2).strip()
            call_pairs.append((caller, callee))

#打印call_pairs
# print("len(call_pairs):", len(call_pairs))
# for pair in call_pairs:
#     print("caller:", pair[0], "callee:", pair[1])


#将mixed_functions与sensitive_functions的并集称为A, non_sensitive_functions称为B，检查call_pairs中是否存在caller在A中且callee在B中的情况，或者caller在B中且callee在A中的情况，统计这样的调用对数量
A = set(mixed_functions) | set(sensitive_functions)
B = set(non_sensitive_functions)
cross_domain_calls = []
for caller, callee in call_pairs:
    if (caller in A and callee in B) or (caller in B and callee in A and callee not in may_shard_funlist): #
        cross_domain_calls.append((caller, callee))

print("跨域调用对数量:", len(cross_domain_calls))
for pair in cross_domain_calls:
    print("跨域调用对 - caller:", pair[0], "callee:", pair[1])


#将mixed_functions与sensitive_functions的并集称为A, non_sensitive_functions称为B，new_common_list称为C，检查call_pairs中是否存在caller在B中且callee在A中的情况，统计这样的调用对数量
# def has_non_zero_quan(function_name, all_nodes):
#     """检查函数是否有quan不为0的节点"""
#     function_nodes = [node for node in all_nodes if node and node['function'] == function_name]
#     has_non_zero = any(node.get('quantity', 0) != 0 for node in function_nodes)
    
#     # 调试信息：打印函数的quan值情况
#     quan_values = [node.get('quantity', 0) for node in function_nodes]
#     print(f"函数 {function_name}: quan值={quan_values}, 有非零quan={has_non_zero}")
    
#     return has_non_zero

# C = set(new_common_list)
# cross_domain_calls_2 = []
# all_nodes = sensitive_nodes + non_sensitive_nodes
# filtered_count = 0  # 记录被过滤掉的调用对数量

# for caller, callee in call_pairs:
#     if callee=="ngx_palloc" or callee=="ngx_pnalloc" or callee=="ngx_pcalloc" or callee=="ngx_strncasecmp" \
#     or callee=="ngx_sprintf" or callee=="ngx_vslprintf" or callee=="ngx_gmtime":
#         continue
#     if (caller in B and callee in A and callee not in C): # :
#         # 检查callee函数是否有quan不为0的节点，如果全为0则跳过
#         if has_non_zero_quan(callee, all_nodes):
#             cross_domain_calls_2.append((caller, callee))
#         else:
#             filtered_count += 1
#             print(f"过滤掉的调用对: {caller} -> {callee} (所有quan都为0)")

# print(f"过滤前符合条件的调用对数量: {len(cross_domain_calls_2) + filtered_count}")
# print(f"被过滤掉的调用对数量: {filtered_count}")
# print("跨域调用对数量2:", len(cross_domain_calls_2))
# for pair in cross_domain_calls_2:
#     print("跨域调用对2 - caller:", pair[0], "callee:", pair[1])
