"""
读取.txt文件，文件中的内容是flowcheck的输出结果，大体格式如下：
Function exit: get_warc_log_fp end
Clearing sensitive tags from stack: 0xFEFFC994 to 0xFEFFC998
Function entry: rpl_fflush
callpair: caller:logflush,callee:rpl_fflush, location:0x15DF82: rpl_fflush (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/lib/fflush.c:130)
 ]
Function exit: rpl_fflush end
Clearing sensitive tags from stack: 0xFEFFC96C to 0xFEFFC988
Function exit: logflush end
Clearing sensitive tags from stack: 0xFEFFC988 to 0xFEFFC9B8
Function exit: log_vprintf_internal end
Clearing sensitive tags from stack: 0xFEFFC9B8 to 0xFEFFCA78
Function exit: logprintf end
Clearing sensitive tags from stack: 0xFEFFCA78 to 0xFEFFCAC8
Function entry: sockaddr_set_data
callpair: caller:connect_to_ip,callee:sockaddr_set_data, location:0x10DF85: sockaddr_set_data (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:79)
 ]
Function exit: sockaddr_set_data end
Clearing sensitive tags from stack: 0xFEFFCAAC to 0xFEFFCAD8
call_stack_funname:[ main retrieve_url http_loop gethttp establish_connection connect_to_host connect_to_ip ]
nosenseinfo:0x10E63B: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB0C
nosenseinfo:0x10E63B: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB0D
nosenseinfo:0x10E63B: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB0E
nosenseinfo:0x10E63B: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB0F
call_stack_funname:[ main retrieve_url http_loop gethttp establish_connection connect_to_host connect_to_ip ]
nosenseinfo:0x10E641: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB0C
nosenseinfo:0x10E641: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB0D
nosenseinfo:0x10E641: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB0E
nosenseinfo:0x10E641: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB0F
call_stack_funname:[ main retrieve_url http_loop gethttp establish_connection connect_to_host connect_to_ip ]
nosenseinfo:0x10E647: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB24
nosenseinfo:0x10E647: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB25
nosenseinfo:0x10E647: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB26
nosenseinfo:0x10E647: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301),0xFEFFCB27
call_stack_funname:[ main retrieve_url http_loop gethttp establish_connection connect_to_host connect_to_ip ]
hassenseinfo:0x10E64D: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:302),0xFEFFCB08
hassenseinfo:0x10E64D: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:302),0xFEFFCB09
hassenseinfo:0x10E64D: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:302),0xFEFFCB0A
hassenseinfo:0x10E64D: connect_to_ip (/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:302),0xFEFFCB0B
以上只是一部分内容，我主要关注四种信息，call_stack_funname, nosenseinfo, hassenseinfo, Function entry
读取每一行，若是call_stack_funname，输出到文件，规则为判断当前的call_stack_funname与上一次的是否相同，若不同，则更新当前的call_stack_funname，输出到文件
若是nosenseinfo，以内容(/Desktop/IF-driver-partition/partitioned_software/wget-1.20/1_sense-annotation-code/wget-1.20/src/connect.c:301)为key,0xFEFFCB0C为value，存入字典,若key已存在，则将value加入到该key对应的value列表中，若value已存在，则不加入,前提是没有遇到Function entry，当遇到Function entry或Function exit时，将字典中的内容存入文件，并清空字典
若是hassenseinfo,同nosenseinfo处理
若是Function entry或Function exit,将字典中的内容存入文件，同时将当前行内容存入文件

"""
import os
import re
import sys

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

# --- 主程序入口 ---
if __name__ == "__main__":
    # 检查命令行参数
    if len(sys.argv) < 2:
        print("用法: python refactor_flowcheck_output.py <项目名称>")
        sys.exit(1)
        
    proname = sys.argv[1]
    # 根据你的描述，输入文件名似乎是 output1.txt
    input_filename = f"../examples/{proname}/output/temp/{proname}output1.fc"
    output_filename = f"../examples/{proname}/output/temp/senseAddress.txt"
    
    process_log_file(input_filename, output_filename)
