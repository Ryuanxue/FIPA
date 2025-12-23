/*
 * 基本块覆盖率测试工具 (Basic Block Coverage Tool) - 修正版
 * * 改进点:
 * 1. 静态扫描: 在镜像加载时发现所有潜在基本块，确保分母相对稳定
 * 2. 地址归一化: 输出相对镜像基址的偏移量(Offset)，解决ASLR问题
 */

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include "pin.H"

using std::map;
using std::set;
using std::pair;
using std::string;
using std::cerr;
using std::endl;
using std::ofstream;
using std::hex;
using std::dec;
using std::vector;

/* ===================================================================== */
/* 命令行参数配置 */
/* ===================================================================== */

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "bblcoverage.cov", "指定输出文件名");
KNOB<BOOL> KnobPid(KNOB_MODE_WRITEONCE, "pintool",
    "i", "0", "在输出文件名中添加进程ID");
KNOB<BOOL> KnobDetailedReport(KNOB_MODE_WRITEONCE, "pintool",
    "d", "1", "生成详细的覆盖率报告");

/* ===================================================================== */
/* 全局结构与变量 */
/* ===================================================================== */

// 基本块信息结构体
class BBL_INFO
{
  public:
    ADDRINT _start_offset;   // 相对镜像基址的偏移 (RVA)
    ADDRINT _end_offset;     // 结束偏移
    UINT32 _inst_count;      // 指令数量
    string _routine_name;    // 所属函数名
    string _img_name;        // 所属镜像文件名
    ADDRINT _img_base;       // 镜像加载基址(运行时)
    UINT64 _exec_count;      // 执行次数
    
    BBL_INFO() : _start_offset(0), _end_offset(0), _inst_count(0), 
                 _img_base(0), _exec_count(0) {}
    
    BBL_INFO(ADDRINT start_off, ADDRINT end_off, UINT32 count, 
             const string& rtn, const string& img, ADDRINT base)
        : _start_offset(start_off), _end_offset(end_off), _inst_count(count), 
          _routine_name(rtn), _img_name(img), _img_base(base), _exec_count(0) {}
};

// 关键：为了性能，Runtime我们用绝对地址做Key，输出时再转偏移
// 但为了处理动态加载卸载，更严谨的做法是结合 image_id，这里简化处理
map<ADDRINT, BBL_INFO> all_bbls;        // 所有发现的基本块 (Key: 运行时绝对地址)
set<ADDRINT> executed_bbls;             // 已执行的基本块地址
PIN_LOCK lock;                          // 多线程锁
ofstream outFile;

/* ===================================================================== */
/* 辅助函数 */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "基本块覆盖率测试工具 (Fixed)\n";
    cerr << "此工具统计程序执行过程中的基本块覆盖率，包含静态发现功能。\n";
    cerr << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

// 提取纯文件名
string StripPath(const string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != string::npos)
        return path.substr(pos + 1);
    return path;
}

/* ===================================================================== */
/* 核心逻辑 */
/* ===================================================================== */

// 执行计数回调
VOID PIN_FAST_ANALYSIS_CALL BBL_Count(ADDRINT bbl_addr)
{
    // 快速路径：无锁检查 (虽然不是完全线程安全，但统计误差可接受，性能优先)
    // 如果需要严格精确，可以使用原子操作
    // 这里简单使用 PIN_GetLock 保护 map写入，但 execution 标记通常读多写少
    
    // 我们只标记 "已执行"，详细计数在 map 中
    // 注意：executed_bbls 插入不是线程安全的，但在分析例程中通常没问题
    // 为安全起见，这里简化：只更新计数，最后统计
    
    // 实际生产中建议使用 per-thread storage，这里为了代码简洁直接更新
    // 假设 all_bbls 已经在 ImageLoad 或 Trace 中填充好了
    
    // 简单的原子自增 (如果编译器支持) 或 忽略竞争
    // all_bbls[bbl_addr]._exec_count++; 
    // 上面这行在map查找太慢，通常做法是在JIT时传入指针
}

// 改进的计数回调：直接传指针，极快
VOID PIN_FAST_ANALYSIS_CALL Docount(UINT64 * counter) {
    (*counter)++;
}

// 1. 静态扫描：在镜像加载时尽可能发现所有基本块
VOID ImageLoad(IMG img, VOID *v)
{
    PIN_GetLock(&lock, 1);
    
    string img_name = StripPath(IMG_Name(img));
    ADDRINT img_base = IMG_LowAddress(img);
    
    // 遍历所有 Section
    for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
    {
        // 只扫描可执行段
        if (!SEC_IsExecutable(sec)) continue;

        // 遍历所有 Routine (函数)
        for (RTN rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn))
        {
            RTN_Open(rtn);
            
            string rtn_name = RTN_Name(rtn);
            
            // 静态遍历指令来切分基本块
            // 策略：每当遇到跳转、调用、返回，或者下一条指令是跳转目标时(静态难判断目标)，切分
            // 简化策略：以 "控制流指令" 为边界
            
            UINT32 inst_count = 0;
            ADDRINT bbl_start = 0;
            bool inside_bbl = false;
            
            for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins))
            {
                ADDRINT addr = INS_Address(ins);
                
                if (!inside_bbl) {
                    bbl_start = addr;
                    inst_count = 0;
                    inside_bbl = true;
                }
                
                inst_count++;
                // 修正代码：将 INS_IsBranchOrCall 拆分为 INS_IsBranch || INS_IsCall
                if (INS_IsBranch(ins) || INS_IsCall(ins) || INS_IsRet(ins) || !INS_Valid(INS_Next(ins)))
                {
                    // 如果是控制流指令，或者最后一条指令，结束当前块
                // if (INS_IsBranchOrCall(ins) || INS_IsRet(ins) || !INS_Valid(INS_Next(ins)))
                // {
                    // 记录这个静态发现的块
                    // Key 使用绝对地址，Value 存偏移
                    if (all_bbls.find(bbl_start) == all_bbls.end()) {
                        ADDRINT start_off = bbl_start - img_base;
                        ADDRINT end_off = addr + INS_Size(ins) - img_base;
                        all_bbls[bbl_start] = BBL_INFO(start_off, end_off, inst_count, rtn_name, img_name, img_base);
                    }
                    inside_bbl = false;
                }
            }
            
            RTN_Close(rtn);
        }
    }
    
    PIN_ReleaseLock(&lock);
}

// 2. 动态 Trace：补全静态扫描可能遗漏的动态块，并插入计数桩
VOID Trace(TRACE trace, VOID *v)
{
    // 获取镜像信息
    ADDRINT trace_addr = TRACE_Address(trace);
    IMG img = IMG_FindByAddress(trace_addr);
    string img_name = IMG_Valid(img) ? StripPath(IMG_Name(img)) : "unknown";
    ADDRINT img_base = IMG_Valid(img) ? IMG_LowAddress(img) : 0;
    
    // 遍历Trace中的BBL
    for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
    {
        ADDRINT bbl_addr = BBL_Address(bbl);
        
        PIN_GetLock(&lock, 1);
        
        // 如果这个块不在我们静态扫描的列表中 (可能是动态生成的代码，或者静态分析漏掉的)
        // 我们将其加入
        if (all_bbls.find(bbl_addr) == all_bbls.end())
        {
            UINT32 inst_count = BBL_NumIns(bbl);
            ADDRINT end_addr = bbl_addr + BBL_Size(bbl); // 简化的结束地址计算
            
            string rtn_name = "unknown";
            RTN rtn = TRACE_Rtn(trace); // Trace关联的RTN通常比较准
            if (RTN_Valid(rtn)) rtn_name = RTN_Name(rtn);
            
            ADDRINT start_off = (img_base > 0) ? (bbl_addr - img_base) : bbl_addr;
            ADDRINT end_off   = (img_base > 0) ? (end_addr - img_base) : end_addr;

            all_bbls[bbl_addr] = BBL_INFO(start_off, end_off, inst_count, rtn_name, img_name, img_base);
        }
        
        // 获取计数器的指针
        BBL_INFO* info = &all_bbls[bbl_addr];
        
        PIN_ReleaseLock(&lock);
        
        // 插入快速计数调用
        // 传递 &info->_exec_count 指针，避免每次回调都查map
        BBL_InsertCall(bbl, IPOINT_BEFORE, (AFUNPTR)Docount, IARG_PTR, &(info->_exec_count), IARG_END);
    }
}

/* ===================================================================== */
/* 报告生成 */
/* ===================================================================== */

VOID GenerateCoverageReport()
{
    UINT32 total_bbls = all_bbls.size();
    UINT32 executed_count = 0;
    
    // 统计执行数
    for (map<ADDRINT, BBL_INFO>::iterator it = all_bbls.begin(); it != all_bbls.end(); ++it) {
        if (it->second._exec_count > 0) executed_count++;
    }

    double coverage_rate = total_bbls > 0 ? (double)executed_count / total_bbls * 100.0 : 0.0;
    
    outFile << "===============================================\n";
    outFile << "           基本块覆盖率报告 (Fixed)\n";
    outFile << "===============================================\n";
    outFile << "总基本块数量: " << total_bbls << "\n";
    outFile << "执行基本块数量: " << executed_count << "\n";
    outFile << "覆盖率: " << coverage_rate << "%\n";
    outFile << "===============================================\n\n";
    
    // 按镜像统计
    map<string, pair<UINT32, UINT32> > img_stats;
    
    // 排序输出需要，收集到vector
    vector<BBL_INFO> sorted_bbls;
    
    for (map<ADDRINT, BBL_INFO>::iterator it = all_bbls.begin(); it != all_bbls.end(); ++it)
    {
        BBL_INFO& info = it->second;
        img_stats[info._img_name].first++;
        if (info._exec_count > 0) img_stats[info._img_name].second++;
        sorted_bbls.push_back(info);
    }
    
    outFile << "按镜像文件统计:\n";
    outFile << "镜像文件名\t\t总基本块\t执行基本块\t覆盖率\n";
    outFile << "---------------------------------------------------------------\n";
    
    for (map<string, pair<UINT32, UINT32> >::iterator it = img_stats.begin(); it != img_stats.end(); ++it)
    {
        double img_coverage = it->second.first > 0 ? 
            (double)it->second.second / it->second.first * 100.0 : 0.0;
        
        outFile << it->first << "\t\t" 
                << it->second.first << "\t\t"
                << it->second.second << "\t\t"
                << img_coverage << "%\n";
    }
    
    if (KnobDetailedReport.Value())
    {
        outFile << "\n===============================================\n";
        outFile << "              详细基本块信息\n";
        outFile << "  (地址已转换为: ImageName + Offset)\n";
        outFile << "===============================================\n";
        outFile << "地址偏移\t大小\t指令数\t执行次数\t函数名\t\t镜像\n";
        outFile << "-------------------------------------------------------------------------\n";
        
        // 为了美观，按镜像名然后按地址排序
        // 简单的 Lambda 排序 (C++11)
        // std::sort(sorted_bbls.begin(), sorted_bbls.end(), ...); 
        // 既然你是Pin工具，假设支持C++ STL
        
        for(size_t i=0; i<sorted_bbls.size(); ++i) {
            const BBL_INFO& info = sorted_bbls[i];
            
            outFile << "0x" << hex << info._start_offset << dec << "\t"
                    << (info._end_offset - info._start_offset) << "\t"
                    << info._inst_count << "\t"
                    << info._exec_count << "\t\t"
                    << info._routine_name << "\t\t"
                    << info._img_name << "\t"
                    << (info._exec_count > 0 ? "COVERED" : "NOT_COVERED") << "\n";
        }
    }
}

VOID Fini(INT32 code, VOID *v)
{
    GenerateCoverageReport();
    outFile.close();
}

int main(int argc, char *argv[])
{
    PIN_InitSymbols(); // 重要：必须初始化符号，否则 ImageLoad 找不到函数名
    if (PIN_Init(argc, argv)) return Usage();
    PIN_InitLock(&lock);
    
    string fileName = KnobOutputFile.Value();
    if (KnobPid.Value()) fileName += "." + decstr(getpid());
    outFile.open(fileName.c_str());
    
    // 注册 ImageLoad 用于静态扫描
    IMG_AddInstrumentFunction(ImageLoad, 0);
    // 注册 Trace 用于动态插桩
    TRACE_AddInstrumentFunction(Trace, 0);
    
    PIN_AddFiniFunction(Fini, 0);
    
    PIN_StartProgram();
    return 0;
}