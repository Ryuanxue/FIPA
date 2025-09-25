import sys
import subprocess
import time

proname = sys.argv[1]
min_quan = sys.argv[2] if "min-quan" in sys.argv[2] else sys.argv[3]
max_code_sz = sys.argv[3] if "max-code-sz" in sys.argv[3] else sys.argv[2]
so_type = "b"  # 默认类型

# 支持可选参数 --so-type
for arg in sys.argv:
    if arg.startswith("--so-type="):
        so_type = arg.split("=")[1]

so_map = {
    "b": "libpdg_b.so",
    "u": "libpdg_u.so",
    "noz3": "libpdg_noz3.so"
}
so_file = so_map.get(so_type, "libpdg_b.so")

# 从min_quan中提取=后面的值，转换为整数
min_quan = int(min_quan.split("=")[1])

# 从max_code_sz中提取=后面的值，转换为浮点数
max_code_sz = float(max_code_sz.split("=")[1])
# 超时时间（30分钟）
timeout = 1200*60

# 循环执行命令，直到找到解或超时
start_time = time.time()
while True:
    print(max_code_sz)

    # 构造命令
    command = f"opt-12 -load ./{so_file} --dot-qg --min-quan={min_quan} --max-code-sz={max_code_sz:.3f} " \
            f"--st-xml=../partitioned_software/{proname}/2_flowcheck_result/{proname}_statement.xml " \
            f"--quanfile=../partitioned_software/{proname}/2_flowcheck_result/{proname}_quanfile.txt " \
            f"--edge-weight=../partitioned_software/{proname}/2_flowcheck_result/{proname}_edge_weights.txt " \
            f"../partitioned_software/{proname}/2_flowcheck_result/{proname}.bc"

    print(command)
    # 执行命令
    process = subprocess.Popen(command, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    try:
        # 设置超时时间为30分钟（1800秒）
        process.wait(timeout=timeout)
    except subprocess.TimeoutExpired:
        print("命令执行超时，停止执行。")
        process.kill()  # 终止命令
        break

    # # 检查执行时间是否超时
    # if time.time() - start_time > timeout:
    #     print("执行超时，停止执行。")
    #     break

    # 读取 z3_result.txt 文件
    try:
        with open("z3_result.txt", "r") as f:
            lines = f.readlines()
            last_line = lines[-1].strip() if lines else ""  # 确保文件不为空
            if last_line != "无解":
                print("找到解，程序结束。")
                break
            else:
                # 增加 max_code_sz 并继续执行
                max_code_sz += 0.01
                print(f"无解，增加 max_code_sz 至 {max_code_sz:.3f}，重新执行命令。")
    except FileNotFoundError:
        print("z3_result.txt 文件未找到，程序结束。")
        break
#移动文件z3_result.txt到flowcheck_result文件夹改命为wget_z3_result.txt
subprocess.run(f"mv z3_result.txt ../partitioned_software/{proname}/3_qg_bi_z3_result/{proname}_z3_result.txt",shell=True)
