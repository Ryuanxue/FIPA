import re
from collections import defaultdict
import argparse
import os
import sys

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

"""
prepare_shard.txt的文件内容如下:
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
def parse_shard_file(file_path):
    """从 shard.txt 解析函数分类"""
    may_shard_funlist=[]
    shard_fun_list = []
    quanallTrue_list = []
    
    if not os.path.exists(file_path):
        print(f"警告: shard 文件不存在: {file_path}")
        return may_shard_funlist, shard_fun_list, quanallTrue_list

    with open(file_path, 'r') as f:
        lines = f.readlines()

    func_name = None
    for line in lines:
        line = line.strip()
        if line.startswith("函数:"):
            func_name = line.split(":", 1)[1].strip()
            if func_name and func_name not in may_shard_funlist:
                may_shard_funlist.append(func_name)
            continue
        if func_name and line.startswith("敏感性一致:"):
            if "是" in line:
                if func_name in may_shard_funlist:
                    pass
                    may_shard_funlist.remove(func_name)
                if func_name not in quanallTrue_list:
                    quanallTrue_list.append(func_name)
            else: # "否"
                if func_name not in shard_fun_list:
                    shard_fun_list.append(func_name)
    print(len(may_shard_funlist))
    return may_shard_funlist, shard_fun_list, quanallTrue_list

def refine_function_lists(mixed_functions, sensitive_functions, non_sensitive_functions, all_nodes):
    """根据特定规则优化函数分类"""
    for func in mixed_functions[:]:
        mixed_nodes = [node for node in all_nodes if node and node['function'] == func]
        
    
        non_sensitive_in_func = [node for node in mixed_nodes if not node['issensitive']]
        
        if non_sensitive_in_func and all(node['size'] == 0 for node in non_sensitive_in_func):
            mixed_functions.remove(func)
            sensitive_functions.append(func)
        elif non_sensitive_in_func and all(node.get('type') == 'DeclStmt' for node in non_sensitive_in_func):
            mixed_functions.remove(func)
            sensitive_functions.append(func)
        

def calculate_and_print_stats(proname, so_type, quan):
    """执行所有分析并将关键结果输出到文件"""
    # 动态构建文件路径
    # 注意：此脚本应从 FIPA/scripts/ 目录运行
    base_path = f"examples/{proname}/output"
    output_dir=f"examples/{proname}/output/analyze_results"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    shard_file = os.path.join(base_path, "temp/prepare_shard.txt") # 修正：移除了 temp 前的斜杠
    
    # 根据 quan 参数构建文件名
    if quan:
        quan_str = f"_{quan}bit"
    else:
        quan_str = ""

    sense_config_file = os.path.join(base_path, f"minsencode_{so_type}{quan_str}.txt")
    output_summary_file = os.path.join(output_dir, f"analysis_summary_{so_type}{quan_str}.txt")
    sensitive_partition_file = os.path.join(output_dir, f"sensitive_partition_{so_type}{quan_str}.txt")
    nonsensitive_partition_file = os.path.join(output_dir, f"nonsensitive_partition_{so_type}{quan_str}.txt")

    if not os.path.exists(sense_config_file):
        print(f"错误: 配置文件不存在: {sense_config_file}")
        sys.exit(1)
    print(shard_file)

    # 1. 解析 shard.txt
    may_shard_funlist, shard_fun_list, quanallTrue_list = parse_shard_file(shard_file)

    # 2. 解析分区配置文件
    sensitive_nodes, non_sensitive_nodes = parse_sense_config(sense_config_file)
    all_nodes = sensitive_nodes + non_sensitive_nodes
    function_status = classify_functions(sensitive_nodes, non_sensitive_nodes)
    
    mixed_functions = [func for func, status in function_status.items() if status['sensitive'] and status['non_sensitive']]
    sensitive_functions = [func for func, status in function_status.items() if status['sensitive'] and not status['non_sensitive']]
    non_sensitive_functions = [func for func, status in function_status.items() if not status['sensitive'] and status['non_sensitive']]

    # 3. 根据规则优化函数分类
    print("优化前混合函数的数量: " + str(len(mixed_functions)))
    refine_function_lists(mixed_functions, sensitive_functions, non_sensitive_functions, all_nodes)

    print("优化后混合函数的数量: " + str(len(mixed_functions)))


    #检查所有sensitive_functions的每个节点的quantity是否都大于0，若所有节点的quantity都=0，则将此函数添加到may_shard_funlist列表中

    print("优化前may_shard_funlist的数量: " + str(len(may_shard_funlist)))
    for func in sensitive_functions:
        if all(node['quantity'] == 0 for node in all_nodes if node and node['function'] == func):
            if func not in may_shard_funlist:
                may_shard_funlist.append(func)
    print("优化后may_shard_funlist的数量: " + str(len(may_shard_funlist)))

    # 4. 计算最终的函数集合
    s_onlyfunction_list = set(quanallTrue_list) & set(sensitive_functions)
    new_common_list = set(sensitive_functions) & set(may_shard_funlist)

    

    # 5. 计算各域代码大小
    
    sdomain_size = sum(node['size'] for node in all_nodes if node and node['function'] in sensitive_functions)
    ndomain_size = sum(node['size'] for node in all_nodes if node and node['function'] in non_sensitive_functions)
    common_size = sum(node['size'] for node in all_nodes if node and node['function'] in new_common_list)
    
    mixed_sensitive_size = sum(node['size'] for node in all_nodes if node and node['function'] in mixed_functions and node['issensitive'])
    mixed_non_sensitive_size = sum(node['size'] for node in all_nodes if node and node['function'] in mixed_functions and not node['issensitive'])

    # 6. 计算敏感代码比例
    sensitive_codesize = sdomain_size + mixed_sensitive_size 
    non_sensitive_codesize = ndomain_size + mixed_non_sensitive_size + common_size
    all_codesize = sensitive_codesize + non_sensitive_codesize

    #计算函数数量,从节点中统计不重复的函数名
    all_functions = set(node['function'] for node in all_nodes if node)
    print("所有函数的数量: " + str(len(all_functions)))
    
    
    # 7. 计算新的函数统计
    sensitive_function_count = len(sensitive_functions) + len(mixed_functions)
    non_sensitive_function_count = len(non_sensitive_functions) + len(mixed_functions)+len(new_common_list)
    shared_function_count = len(new_common_list)

    # 8. 收集并保存最终分区语句
    sensitive_partition_nodes = [
        node for node in all_nodes if node and (
            node['function'] in sensitive_functions or
            (node['function'] in mixed_functions and node['issensitive'])
        )
    ]
    nonsensitive_partition_nodes = [
        node for node in all_nodes if node and (
            node['function'] in non_sensitive_functions or
            node['function'] in new_common_list or
            (node['function'] in mixed_functions and not node['issensitive'])
        )
    ]

    def write_nodes_to_file(filepath, nodes):
        with open(filepath, 'w', encoding='utf-8') as f:
            for node in sorted(nodes, key=lambda x: (x['file'], x['start_line'])):
                # 构建您指定的输出格式
                output_line = (
                    f"{node['file']} + {node['function']} + "
                    f"{node['type']}:{node['start_line']}:{node['end_line']} + "
                    f"{node['quantity']}\n" 
                )
                f.write(output_line)
    
    write_nodes_to_file(sensitive_partition_file, sensitive_partition_nodes)
    write_nodes_to_file(nonsensitive_partition_file, nonsensitive_partition_nodes)
    print(f"敏感分区语句已保存到: {sensitive_partition_file}")
    print(f"非敏感分区语句已保存到: {nonsensitive_partition_file}\n")


    # 9. 将结果输出到文件和控制台
    with open(output_summary_file, 'w', encoding='utf-8') as f:
        def write_and_print(message):
            print(message)
            f.write(message + '\n')

        write_and_print("--- 分析结果摘要 ---")
        
        write_and_print("\n--- 函数数量统计 ---")
        write_and_print(f"敏感函数总数 : {sensitive_function_count}")
        write_and_print(f"非敏感函数总数 : {non_sensitive_function_count}")
        write_and_print(f"共享函数总数 : {shared_function_count}")

        # write_and_print("\n--- 各域代码大小 (size) ---")
        # write_and_print(f"敏感分区code 大小: {sdomain_size}")
        # write_and_print(f"N-domain 大小: {ndomain_size}")
        # write_and_print(f"C-domain 大小: {common_size}")
        # write_and_print(f"Mixed-domain (敏感部分) 大小: {mixed_sensitive_size}")
        # write_and_print(f"Mixed-domain (非敏感部分) 大小: {mixed_non_sensitive_size}")

        write_and_print("\n--- 敏感代码比例 ---")
        if all_codesize > 0:
            ratio = sensitive_codesize / all_codesize
            write_and_print(f"总代码大小: {all_codesize}")
            write_and_print(f"总敏感代码大小: {sensitive_codesize}")
            write_and_print(f"敏感代码所占比例: {ratio:.2%}")
        else:
            write_and_print("总代码大小为 0，无法计算比例。")

    print(f"\n分析摘要已保存到: {output_summary_file}")

def main():
    parser = argparse.ArgumentParser(description="分析代码分区结果，计算敏感代码比例和跨域调用。")
    parser.add_argument("proname", help="项目名称 (例如: chage, wget)。")
    parser.add_argument("--comm-type", default="b", choices=['b', 'u', 'noz3'], help="SO库的类型 (b, u, noz3)。")
    parser.add_argument("--quan", help="敏感信息量的限制 (例如:0, 8, 16,32, 64)，用于构成文件名。")
    
    args = parser.parse_args()
    
    calculate_and_print_stats(args.proname, args.comm_type, args.quan)

if __name__ == "__main__":
    main()
