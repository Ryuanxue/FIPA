#include "QPGGraph.hh"
#include "parserxml.hh"
#include <fstream>
#include <sstream>
#include <z3++.h>
#include "llvm/Support/CommandLine.h"
#include <chrono>
#include <iostream>

// 计时宏
#define TIME_START(name) auto start_##name = std::chrono::high_resolution_clock::now();
#define TIME_END(name) { \
    auto end_##name = std::chrono::high_resolution_clock::now(); \
    auto duration_##name = std::chrono::duration_cast<std::chrono::milliseconds>(end_##name - start_##name).count(); \
    std::cerr << "Time for " #name ": " << duration_##name << " ms\n"; \
}

using namespace llvm;
// 定义一个整数命令行参数，名称为 "min-quan"，默认值为 8
static cl::opt<int> MinQuan("min-quan", cl::desc("Minimum quan value"), cl::init(8));
// 定义一个浮点数命令行参数，名称为 "min-code-sz"，默认值为 0.1
static cl::opt<double> MinCodeSz("max-code-sz", cl::desc("Maxmum code size value"), cl::init(0.2));
// 定义一个字符串参数，名称为 "st-xml"，默认值为 "statement.xml"
static cl::opt<std::string> StXml("st-xml", cl::desc("Statement XML file"), cl::init("statement.xml"));//若xml文件和可执行文件在同一目录下，直接写文件名即可，否则需要写文件的绝对路径
// 定义一个字符串参数，名称为 "quanfile"，默认值为 "quanfile.txt"
static cl::opt<std::string> QuanFile("quanfile", cl::desc("Quan file"), cl::init("quanfile.txt"));//若quanfile.txt文件和可执行文件在同一目录下，直接写文件名即可，否则需要写文件的绝对路径
// 添加 trim 函数声明
void trim(std::string &s);
// 定义一个命令行参数用于指定边权重文件
static cl::opt<std::string> EdgeWeightFile("edge-weight", 
    cl::desc("Edge weight file path"), 
    cl::init("edge_weights.txt"));

// 定义一个命令行参数用于指定Z3结果输出文件
static cl::opt<std::string> Z3OutputFile("z3-output", 
    cl::desc("Z3 result output file path"), 
    cl::init("z3_result.txt"));

// 存储边权重的数据结构
typedef std::unordered_map<std::string, std::unordered_map<std::string, int>> EdgeWeightMap;

// 读取边权重文件的函数
EdgeWeightMap qg::QPgGraph::readEdgeWeights(const std::string& filename) {
    EdgeWeightMap forwardEdges;  // 正向边权重
    EdgeWeightMap backwardEdges; // 反向边权重
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        errs() << "Warning: Unable to open edge weight file: " << filename << "\n";
        return forwardEdges; // 返回空映射
    }
    
    std::string line;
    // 跳过标题行
    std::getline(file, line);
    
    // 逐行读取数据
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string source, target, weightStr;
        
        // 使用逗号分隔，读取三列数据
        if (!std::getline(iss, source, ',') || 
            !std::getline(iss, target, ',') || 
            !std::getline(iss, weightStr)) {
            errs() << "Warning: Invalid line format: " << line << "\n";
            continue;
        }
        
        // 去除可能的空白字符
        trim(source);
        trim(target);
        trim(weightStr);
        
        // 转换权重为整数
        int weight = std::stoi(weightStr);
        
        // 存储正向边
        forwardEdges[source][target] = weight;
        // 存储反向边
        backwardEdges[target][source] = weight;
        
        errs() << "Edge loaded: " << source << " -> " << target << " (weight: " << weight << ")\n";
    }
    
    errs() << "Loaded " << forwardEdges.size() << " source nodes with edge weights.\n";
    
    // 将边权重存储在类的成员变量中，以便其他函数访问
    _forward_edge_weights = forwardEdges;
    _backward_edge_weights = backwardEdges;
    
    return forwardEdges;
}

// 辅助函数：将源节点和目标节点标识符转换为边权重映射中使用的键
std::string qg::QPgGraph::nodeToEdgeKey(pdg::Node* node) {
    if (node->getNodeType() == pdg::GraphNodeType::STATEMENT) {
        return node->getStatement();
    } else {
        // 对于FUNC_ENTRY节点等，使用它们的标识符
        return node->getStatement();
    }
}

// 获取两个节点间边的权重
int qg::QPgGraph::getEdgeWeight(pdg::Node* src, pdg::Node* dst, int defaultWeight) {
    std::string srcKey = nodeToEdgeKey(src);
    std::string dstKey = nodeToEdgeKey(dst);
    
    // 查找边权重
    if (_forward_edge_weights.count(srcKey) > 0 && 
        _forward_edge_weights[srcKey].count(dstKey) > 0) {
        return _forward_edge_weights[srcKey][dstKey];
    } 
    // else if (_forward_edge_weights.count(srcKey) > 0 && _backward_edge_weights.count(dstKey) > 0) 
    // {
    //     // 得到所有_forward_edge_weights[srcKey],提取边的dst，得到所有_backward_edge_weights[desKey],提取dst1
    //     // 若dst和dst1中的文件名与函数名都相同，则返回_backward_edge_weights[dstKey][dst1key]
        
    //     // 从源节点提取文件和函数信息
    //     size_t srcFirstPlus = srcKey.find(" + ");
    //     size_t srcSecondPlus = srcKey.find(" + ", srcFirstPlus + 1);
    //     if (srcFirstPlus == std::string::npos || srcSecondPlus == std::string::npos) {
    //         return defaultWeight; // 格式不符，返回默认权重
    //     }
    //     std::string srcFilePath = srcKey.substr(0, srcFirstPlus);
    //     std::string srcFuncName = srcKey.substr(srcFirstPlus + 3, srcSecondPlus - srcFirstPlus - 3);
        
    //     // 从目标节点提取文件和函数信息
    //     size_t dstFirstPlus = dstKey.find(" + ");
    //     size_t dstSecondPlus = dstKey.find(" + ", dstFirstPlus + 1);
    //     if (dstFirstPlus == std::string::npos || dstSecondPlus == std::string::npos) {
    //         return defaultWeight; // 格式不符，返回默认权重
    //     }
    //     std::string dstFilePath = dstKey.substr(0, dstFirstPlus);
    //     std::string dstFuncName = dstKey.substr(dstFirstPlus + 3, dstSecondPlus - dstFirstPlus - 3);
        
    //     // 遍历源节点的所有出边目标节点
    //     for (const auto& dstPair : _forward_edge_weights[srcKey]) {
    //         const std::string& forwardDstKey = dstPair.first;
            
    //         // 提取目标节点的文件和函数信息
    //         size_t forwardDstFirstPlus = forwardDstKey.find(" + ");
    //         size_t forwardDstSecondPlus = forwardDstKey.find(" + ", forwardDstFirstPlus + 1);
    //         if (forwardDstFirstPlus == std::string::npos || forwardDstSecondPlus == std::string::npos) {
    //             continue; // 格式不符，跳过
    //         }
    //         std::string forwardDstFilePath = forwardDstKey.substr(0, forwardDstFirstPlus);
    //         std::string forwardDstFuncName = forwardDstKey.substr(forwardDstFirstPlus + 3, forwardDstSecondPlus - forwardDstFirstPlus - 3);
            
    //         // 遍历目标节点的所有入边源节点
    //         for (const auto& srcPair : _backward_edge_weights[dstKey]) {
    //             const std::string& backwardSrcKey = srcPair.first;
                
    //             // 提取后向源节点的文件和函数信息
    //             size_t backwardSrcFirstPlus = backwardSrcKey.find(" + ");
    //             size_t backwardSrcSecondPlus = backwardSrcKey.find(" + ", backwardSrcFirstPlus + 1);
    //             if (backwardSrcFirstPlus == std::string::npos || backwardSrcSecondPlus == std::string::npos) {
    //                 continue; // 格式不符，跳过
    //             }
    //             std::string backwardSrcFilePath = backwardSrcKey.substr(0, backwardSrcFirstPlus);
    //             std::string backwardSrcFuncName = backwardSrcKey.substr(backwardSrcFirstPlus + 3, backwardSrcSecondPlus - backwardSrcFirstPlus - 3);
                
    //             // 如果文件名和函数名匹配，使用对应的权重
    //             if (forwardDstFilePath == backwardSrcFilePath && forwardDstFuncName == backwardSrcFuncName)  {
    //                 int weight = _backward_edge_weights[dstKey][backwardSrcKey];
    //                 // errs() << "找到匹配的边权重通过后向匹配: " << srcKey << " -> " << dstKey 
    //                 //        << " 对应于 " << backwardSrcKey << " -> " << dstKey 
    //                 //        << " 权重: " << weight << "\n";
    //                 return weight;
    //             }
    //         }
    //     }
        
    //     // 如果没有找到匹配项，尝试基于文件和函数名进行简单匹配
    //     for (const auto& srcPair : _backward_edge_weights[dstKey]) {
    //         const std::string& backwardSrcKey = srcPair.first;
            
    //         // 检查是否包含相同的文件名和函数名
    //         if (backwardSrcKey.find(srcFilePath) != std::string::npos && 
    //             backwardSrcKey.find(srcFuncName) != std::string::npos) {
    //             int weight = _backward_edge_weights[dstKey][backwardSrcKey];
    //             // errs() << "找到简单匹配的边权重: " << srcKey << " -> " << dstKey 
    //             //        << " 对应于 " << backwardSrcKey << " -> " << dstKey 
    //             //        << " 权重: " << weight << "\n";
    //             return weight;
    //         }
    //     }
    // }
    
    return defaultWeight; // 如果未找到，返回默认权重
}

// Generic Graph
bool qg::GenericGraph::hasNode(std::string str)
{
  return (_st_node_map.find(str) != _st_node_map.end());
}

pdg::Node* qg::GenericGraph::getNode(std::string str)
{
  if (!hasNode(str))
    return nullptr;
  return _st_node_map[str];
}


using namespace z3;

// 辅助函数：去除字符串首尾空格
void trim(std::string &s) {
    const char* whitespace = " \t\n\r\f\v";
    s.erase(0, s.find_first_not_of(whitespace));
    s.erase(s.find_last_not_of(whitespace) + 1);
}

void qg::QPgGraph::build(llvm::Module &M)
{
    // 创建XMLParser对象并解析文件
    // string filePath = "test.xml";
    std::string filePath = StXml;
    XMLParser parser(filePath);

    // 根据parser.getfunMap()中的每个函数的行号范围，统计每个行号范围对应的llvm ir指令的数量
    for (const auto& funcPair : parser.getfunMap()) {
        // funcPair.first 是函数名
        // funcPair.second 是包含语句信息的列表，每个语句信息包含了行号范围
        // cout << "Function: " << funcPair.first << endl;

        // 创建函数入口节点
        pdg::Node* entrynode = new Node(funcPair.first, pdg::GraphNodeType::FUNC_ENTRY);
        _st_node_map.insert(std::pair<std::string, pdg::Node*>(funcPair.first, entrynode));
        addNode(*entrynode);

        

        // 遍历模块中的每个函数，找到与funcPair.first匹配的函数
        //对每个函数创建一个entrynode，创建statement节点，statement节点的key为文件路径+函数名+语句类型+行号范围，value为statement节点的指针,统计每个statement节点的指令数量
        //将statement节点添加到st_node_map中，最后一个statement节点添加到fun_lastnode_map中
        for (auto& F : M) {
            pdg::Node* prenode = NULL;
            // pdg::Node* firstnode = NULL; // 新增：记录函数第一个节点
            if (F.getName() == funcPair.first) {
                // errs() << "create statement node: " << F.getName() << "\n";
                // 遍历函数中的每条指令
                for (auto& BB : F) {
                    for (auto& I : BB) {
                        // 获取指令所在的行号
                        if (const llvm::DebugLoc& debugLoc = I.getDebugLoc()) {
                            unsigned line = debugLoc.getLine();
                            // 将指令行号与当前函数语句映射中的行号范围比较
                            for (const auto& stmt : funcPair.second) {
                                // 解析stmt以获取行号范围
                                std::string stmtDec = stmt;
                                std::string stmtType = stmtDec.substr(0, stmtDec.find(":"));
                                std::string stmtRange = stmtDec.substr(stmtDec.find(":") + 1);
        
                                int startLine = std::stoi(stmtRange.substr(0, stmtRange.find(":")));
                                int endLine = std::stoi(stmtRange.substr(stmtRange.find(":") + 1));
        
                                // 如果行号在范围内，则计数
                                // errs() << "  check linerange" << startLine << "  " << endLine << "\n";
                                if (line >= startLine && line <= endLine) { // 修正：确保行号在范围内
                                    // cout << "  " << stmt << endl;
                                    pdg::Node* stnode = nullptr;
        
                                    // 查找是否已经存在stnode
                                    auto it = _st_node_map.find(stmt);
                                    if (it != _st_node_map.end()) {
                                        stnode = it->second;
                                    } else {
                                        // 创建新的stnode
                                        stnode = new Node(stmt, pdg::GraphNodeType::STATEMENT);
                                        _st_node_map.insert(std::pair<std::string, pdg::Node*>(stmt, stnode));
                                        addNode(*stnode);
                                        // errs() << "create statement node: " << stmt << "\n";
                                        // 添加CSE边，仅在节点首次创建时
                                        entrynode->addNeighbor(*stnode, pdg::EdgeType::TYPE_CSE);
        
                                        // 添加SSE边
                                        if (prenode) {
                                            prenode->addNeighbor(*stnode, pdg::EdgeType::TYPE_SSE);
                                        }

                                        // // 记录第一个节点
                                        // if (!firstnode) {
                                        //     firstnode = stnode;
                                        // }
                                        prenode = stnode;
                                    }
        
                                    // 增加stnode的指令计数
                                    int wscode = stnode->get_wscode();
                                    wscode += 1;
                                    stnode->set_wscode(wscode);
                                }
                            }
                        }
                    }
                }
                // _fun_lastnode_map[F.getName().str()]=prenode;
                //  // 新增：记录函数的第一个节点
                //  if (firstnode) {
                //     _fun_firstnode_map[F.getName().str()] = firstnode;
                // }
               

                //遍历parser.getfunMap()[F.getName().str()]中的每个语句，若前后两个语句之间不存在SSE边，则添加一条SSE边
                // 遍历函数中的每条语句，添加SSE边
                for (auto it = funcPair.second.begin(); it != funcPair.second.end(); ++it) {
                    std::string currentStmt = *it;
                    // 查找当前语句对应的节点
                    auto currentIt = _st_node_map.find(currentStmt);
                    if (currentIt != _st_node_map.end()) {
                        pdg::Node* currentNode = currentIt->second;

                        // 如果不是第一个语句，检查前一个语句
                        if (it != funcPair.second.begin()) {
                            auto prevIt = std::prev(it);
                            std::string prevStmt = *prevIt;
                            auto prevItNode = _st_node_map.find(prevStmt);
                            if (prevItNode != _st_node_map.end()) {
                                pdg::Node* prevNode = prevItNode->second;

                                // 检查是否已经存在SSE边
                                if (!prevNode->hasOutNeighborWithEdgeType(*currentNode, pdg::EdgeType::TYPE_SSE)) {
                                    // 添加SSE边
                                    prevNode->addNeighbor(*currentNode, pdg::EdgeType::TYPE_SSE);
                                }
                            }
                        }
                    }else{
                        //添加节点ADD
                        pdg::Node* stnode = new Node(currentStmt, pdg::GraphNodeType::STATEMENT);
                        _st_node_map.insert(std::pair<std::string, pdg::Node*>(currentStmt, stnode));
                        addNode(*stnode);
                    }
                    
                    
                 
                }
                std::string firstStmt = parser.getfunMap()[F.getName().str()].front();
                if (_st_node_map.find(firstStmt) != _st_node_map.end()) {
                    _fun_firstnode_map[F.getName().str()] = _st_node_map[firstStmt];
                }

                //lastnode
                std::string lastStmt = parser.getfunMap()[F.getName().str()].back();
                if (_st_node_map.find(lastStmt) != _st_node_map.end()) {
                    _fun_lastnode_map[F.getName().str()] = _st_node_map[lastStmt];
                } else {
                    // 如果没有找到lastStmt对应的节点，输出警告
                    errs() << "Warning: Last statement node for function " << F.getName().str() << " not found.\n";
                }

            }
        }
    }


    // 添加call edge边,从call site到callee函数entry node节点的边
    PDGCallGraph &call_g = PDGCallGraph::getInstance();
    if (!call_g.isBuild())
        call_g.build(M);

    // 使用call_g.Get_File_Line_FunMap函数获得file_line_funMap
    auto file_line_funMap = call_g.Get_File_Line_FunMap();

    // 根据file_line_funMap每个key中的文件，函数，行号信息判断此key属于哪个节点
    for (const auto& fileFunPair : file_line_funMap) {
        std::string key = fileFunPair.first;
        std::string calleeFuncName = fileFunPair.second;
        
        // 找到对应的节点
        /**
         * 此处并不是简单到_st_node_map中去查找key,因为key的形式为/home/raoxue/Desktop/IF-driver-partition/my_examp/test.c + main + 57
         * 而_st_node_map中的stament节点的key为/home/raoxue/Desktop/IF-driver-partition/my_examp/test.c + main + DeclStmt:29:29的格式，
         * 所以要先判断两者的key中filepath和fun是否相等，进而判断前者的行号是否在后者的行号范围内，进而根据后者的key查找对应的node

         * */
        // 解析key，提取文件路径，函数名和行号
        size_t firstPlusPos = key.find(" + ");
        size_t secondPlusPos = key.find(" + ", firstPlusPos + 1);
        std::string filepath = key.substr(0, firstPlusPos);
        std::string funname = key.substr(firstPlusPos + 3, secondPlusPos - firstPlusPos - 3);
        unsigned lineNumber = std::stoi(key.substr(secondPlusPos + 3));

        // 遍历_st_node_map来查找合适的节点
        pdg::Node* callSiteNode = nullptr;
        for (auto &it : _st_node_map) {
            const std::string& stmtKey = it.first;
            pdg::Node* stmtNode = it.second;

            // 输出调试信息
            // errs() << "Statement Key: " << stmtKey << "\n";

            // 解析_stmt_node_map中的key，提取文件路径，函数名和语句行号范围
            size_t stmtFirstPlusPos = stmtKey.find(" + ");
            size_t stmtSecondPlusPos = stmtKey.find(" + ", stmtFirstPlusPos + 1);
            std::string stmtFilepath = stmtKey.substr(0, stmtFirstPlusPos);
            std::string stmtFunname = stmtKey.substr(stmtFirstPlusPos + 3, stmtSecondPlusPos - stmtFirstPlusPos - 3);
            std::string stmtRange = stmtKey.substr(stmtSecondPlusPos + 3);

            // 检查文件路径和函数名是否匹配
            //处处filepath,stmtFilepath,funname,stmtFunname
            
            if (filepath == stmtFilepath && funname == stmtFunname) {
            //     errs() << "filepath: " << filepath << ", stmtFilepath: " << stmtFilepath << "\n";
            // errs() << "funname: " << funname << ", stmtFunname: " << stmtFunname << "\n";
                // 解析行号范围
                size_t colonPos = stmtRange.find(":");
                if (colonPos != std::string::npos) {
                    int startLine = std::stoi(stmtRange.substr(colonPos + 1, stmtRange.find(":", colonPos + 1) - colonPos - 1));
                    int endLine = std::stoi(stmtRange.substr(stmtRange.find_last_of(":") + 1));

                    // 检查行号是否在范围内
                    if (lineNumber >= startLine && lineNumber <= endLine) {
                        callSiteNode = stmtNode;
                        break;
                    }
                }
            }
        }

        // 如果找到了callSiteNode，则添加边
        if (callSiteNode) {
            // 查找callee函数入口节点
            auto calleeIt = _st_node_map.find(calleeFuncName);
            if (calleeIt != _st_node_map.end()) {
                pdg::Node* calleeEntryNode = calleeIt->second;

                // 从callSiteNode添加一条到calleeEntryNode的边
                callSiteNode->addNeighbor(*calleeEntryNode, pdg::EdgeType::CONTROLDEP_CALLINV);

                //添加ret边
                auto &tempmap=getFunLastNodeMap();
                // for(auto &it: tempmap)
                // {
                //   errs()<<it.first<<"  lastnode\n";
                // }
                // errs()<<calleeFuncName<<"  calleefunn\n";
                pdg::Node* lastnode=tempmap[calleeFuncName];
                if (lastnode==NULL)
                {
                  errs()<<"lastnode is null\n";
                }
                // errs()<<lastnode->getStatement()<<"statement\n";
                //  errs()<<calleeFuncName<<"  calleefunn\n";
                lastnode->addNeighbor(*callSiteNode, pdg::EdgeType::CONTROLDEP_CALLRET);
                // errs()<<calleeFuncName<<"  calleefunn\n";

            }
        }
  }

    /**解析quanfile.txt文件，其中的内容为 
     * /home/raoxue/Desktop/IF-driver-partition/my_examp/test.c + main + BinaryOperator:39:39 + 8
/home/raoxue/Desktop/IF-driver-partition/my_examp/test.c + main + BinaryOperator:40:40 + 8
/home/raoxue/Desktop/IF-driver-partition/my_examp/test.c + main + IfStmt:41:56 + 36
形式的，其中，每一行最后一个加号之前的内容对应stmt，最后一个加号后的数字为stmt对应的敏感信息量，使用Node上的get_wquan()和set_wquan(int)函数进行添加敏感信息量
     * 
    */
    // 解析quanfile.txt文件,添加敏感信息量

    // std::ifstream quanFile("quanfile.txt");
    std::ifstream quanFile(QuanFile);
    if (!quanFile) {
        cerr << "Error: Unable to open quanfile.txt" << endl;
        return;
    }

    /**解析quanfile.txt,构造一个映射，其key为以下代码中的key,值为wquan,然后遍历map，根据map的key查找node，获得node的入边入度，根据入边获得入度的节点
    从入度节点上通过node->getStatement()获得值，查找此值是否在map中
    */
    std::map<std::string, int> wquan_map; // 存储敏感信息量映射
    std::string line;

    while (std::getline(quanFile, line)) {
        std::istringstream iss(line);
        std::string filePath, functionName, stmt;
        int wquan;

        // 假设每行的格式固定
        std::getline(iss, filePath, '+');
        std::getline(iss, functionName, '+');
        std::getline(iss, stmt, '+');
        iss >> wquan;

        // 去除开头和结尾的空格
            trim(filePath);
            trim(functionName);
            trim(stmt);
            // trim(sensitiveValueStr);

        // 创建key并查找节点
        std::string key = filePath +" + " +functionName + " + "+stmt;
         wquan_map[key] = wquan;
    }
    
    // 遍历 wquan_map 并更新 _st_node_map 中的节点信息
    for (const auto& pair : wquan_map) {
        auto it = _st_node_map.find(pair.first);
        if (it != _st_node_map.end()) {
            pdg::Node* stnode = it->second;
            int maxInQuan = 0;
            maxInQuan = pair.second;

            // // 如果 maxInQuan 仍然是 0，设置为当前节点的敏感信息量
            // if (maxInQuan == 0) {
            //     for (auto* inNeighbor : stnode->getInNeighbors()) {
            //     //如果inNeighbor的类型为pdg::GraphNodeType::FUNC_ENTRY，再一次获得此节点的入度节点
            //     if (inNeighbor->getNodeType() == pdg::GraphNodeType::FUNC_ENTRY) {
            //         // 如果是 FUNC_ENTRY 类型，获取其入度节点
            //         for (auto* innerInNeighbor : inNeighbor->getInNeighbors()) {
            //             std::string nodeKey = innerInNeighbor->getStatement();
            //             auto eit = wquan_map.find(nodeKey);
            //             if (eit != wquan_map.end()) {
            //                 if (eit->second > maxInQuan) {
            //                     maxInQuan = eit->second;
            //                 }
            //             }
            //         }
            //     }else {
            //         // 否则直接获取当前入度节点的敏感信息量
            //         std::string nodeKey = inNeighbor->getStatement();
            //         auto eit = wquan_map.find(nodeKey);
            //         if (eit != wquan_map.end()) {
            //             if (eit->second > maxInQuan) {
            //                 maxInQuan = eit->second;
            //             }
            //         }
            //     }
            // }
            // }
             // 设置节点的敏感信息量
            stnode->set_wquan(maxInQuan);
        }
    }

    quanFile.close();

    // 读取边权重文件
    readEdgeWeights(EdgeWeightFile);
}



bool qg::QUANProgramGraph::runOnModule(Module &M) {
    _QG = &QPgGraph::getInstance();
    if (!_QG->isBuild()) {
    _QG->build(M);
  }

  //根据scode和wquan权重，对QG中的节点进行二元划分，分为敏感域SD,和不敏感域ND,满足以下条件
    //1. SD中节点的scode之和所占比例小于等于m
    //2. 若quan小于k，则必须在SD中

    // 定义节点数量
    //节点数量是QG中所有类型为STATEMENT的节点数量
    //通过遍历_QG中的节点，统计类型为STATEMENT的节点数量
    //StatementNodeMap &getStNodeMap() { return _st_node_map; }
   
    //通过getStateemnt()函数获得节点的statement，将节点映射到一个有序的map中，比如序号0对应于特定的节点，序号1对应于特定的节点，以此类推
    int N = 0;
    std::map<int, pdg::Node*> nodeMap;
    int index = 0;
    for (auto &it : _QG->getStNodeMap()) {
        if (it.second->getNodeType() == pdg::GraphNodeType::STATEMENT) {
            N++;
            nodeMap[index] = it.second;
            index++;
        }
    }

    // N=8000;
    
     //按序构造代码量的数组和信息量权重数组，如// 节点的 sz 和 quan 权重
    //int sz[N] = {3, 5, 2, 4, 1};
    //int quan[N] = {2, 6, 4, 1, 3};
    //数组中的每个值都是序号对应的节点的代码量和信息量权重
    errs()<<"节点数量"<<N<<"\n";
    int sz[N];
    int quan[N];
    for (int i = 0; i < N; ++i) {
        sz[i] = nodeMap[i]->get_wscode();
        quan[i] = nodeMap[i]->get_wquan();
    }

    
    // 定义问题的参数
    // double m = 0.9;  // sz 权重总和的百分比
    // int k = 8;       // quan 小于 k 的节点必须在 SD 类中
    //将m和k改为从解析命令行参数获得
    double m = MinCodeSz;  // sz 权重总和的百分比
    int k = MinQuan;       // quan 小于 k 的节点必须在 SD 类中



  



    //==========================================


     // 创建 Z3 上下文和求解器
context c;
optimize opt(c);

// 在定义total_sz之后添加
int total_sz = 0;
for (int i = 0; i < N; ++i) {
    total_sz += sz[i];
}

// 定义二元变量 x_i（使用布尔变量，因为你的约束是 0 或 1）
expr_vector x(c);
for (int i = 0; i < N; ++i) {
    x.push_back(c.bool_const(("x_" + std::to_string(i)).c_str())); // 创建布尔变量 x_0, x_1, ...
}

// 构造 sensitive_sz_expr
TIME_START(sensitive_sz);
int term_count = 0;
expr sensitive_sz_expr = c.bv_val(0, 32);
expr_vector terms(c);
for (int i = 0; i < N; ++i) {
    if (sz[i] != 0) {
        terms.push_back(ite(x[i], c.bv_val(sz[i], 32), c.bv_val(0, 32)));
        term_count++;
    }
}
std::cerr << "Number of terms in sensitive_sz_expr: " << term_count << "\n";
for (unsigned i = 0; i < terms.size(); ++i) {
    sensitive_sz_expr = sensitive_sz_expr + terms[i];
}
opt.add(sensitive_sz_expr <= c.bv_val(static_cast<int>(m * total_sz), 32));
TIME_END(sensitive_sz);

// 同时添加最小化敏感域代码量的目标
// opt.minimize(sensitive_sz_expr);



std::vector<bool> fixed(N, false);
int constraint_count = 1;
for (int i = 0; i < N; ++i) {
    if (quan[i] > k) {
        fixed[i] = true;
        // 修复：使用 c.bool_val(true) 代替 true
        opt.add(x[i] == c.bool_val(true));
        constraint_count++;
    }
}
std::cerr << "Initial constraints added: " << constraint_count << "\n";

// 辅助函数：在nodeMap中查找节点索引, 使用哈希表加速节点查找
std::unordered_map<pdg::Node*, int> node_to_index;
for (int i = 0; i < N; ++i) {
    node_to_index[nodeMap[i]] = i;
}
auto findNodeIndex = [&](pdg::Node* targetNode) -> int {
    auto it = node_to_index.find(targetNode);
    return it != node_to_index.end() ? it->second : -1;
};

// 提取函数名和行号，用于排序
auto extractFunctionAndLine = [&](int nodeIdx) -> std::pair<std::string, int> {
    std::string stmt = nodeMap[nodeIdx]->getStatement();
    // 假设格式为 "filepath + function + StmtType:startLine:endLine"
    size_t firstPlus = stmt.find(" + ");
    size_t secondPlus = stmt.find(" + ", firstPlus + 1);
    if (firstPlus == std::string::npos || secondPlus == std::string::npos) {
        std::cerr << "Warning: Invalid statement format for node " << nodeIdx << "\n";
        return {"Unknown", 0}; // 默认值
    }

    

    std::string functionName = stmt.substr(firstPlus + 3, secondPlus - firstPlus - 3);
    
    size_t lastColon = stmt.rfind(":");
    size_t prevColon = stmt.rfind(":", lastColon - 1);
    if (lastColon == std::string::npos || prevColon == std::string::npos) {
        std::cerr << "Warning: Invalid line number format for node " << nodeIdx << "\n";
        return {functionName, 0}; // 默认值
    }

    int startLine = std::stoi(stmt.substr(prevColon + 1, lastColon - prevColon - 1));
    return {functionName, startLine};
};


// 缓存节点函数名和行号，避免重复调用 extractFunctionAndLine
std::unordered_map<int, std::string> node_func_names;
std::unordered_map<int, int> node_lines;
for (int i = 0; i < N; ++i) {
    node_func_names[i] = extractFunctionAndLine(i).first;
}

// 预计算每个函数的节点集合
std::unordered_map<std::string, std::vector<int>> func_nodes;
for (int i = 0; i < N; ++i) {
    func_nodes[node_func_names[i]].push_back(i);
}

TIME_START(total_switches);
int switch_term_count = 0;
int total_edges = 0, sse_edges = 0, callinv_edges = 0;
expr total_switches = c.bv_val(0, 32);
expr_vector switch_terms(c);
int newN = 1000;
// for (int i = 0; i < N; ++i) {
// // for (int i = 0; i < newN; ++i) {
//     pdg::Node* node = nodeMap[i];
//     for (auto& neighbor : node->getOutEdgeSet()) {
//         total_edges++;  
//         if (neighbor->getEdgeType() == pdg::EdgeType::TYPE_SSE) {
//             sse_edges++;
//             int j = findNodeIndex(neighbor->getDstNode());
//             if (j == -1) continue;
//             int edgeWeight = _QG->getEdgeWeight(node, neighbor->getDstNode());
//             switch_term_count++;

//             if (edgeWeight == 0) {
//                 //将i和j分配在同一个域中
//                 // opt.add(x[i] == x[j]);
//                 //将j分配到0
//                 // opt.add(x[j] == c.bool_val(0));
//                 continue;
//             }
//             switch_terms.push_back(ite(x[i] != x[j], c.bv_val(edgeWeight, 32), c.bv_val(0, 32)));
//             auto [srcFunc, srcLine] = extractFunctionAndLine(i);
//             auto [dstFunc, dstLine] = extractFunctionAndLine(j);
//             std::cerr << "SSE_Edge: " << srcFunc << ":" << srcLine << " -> " 
//                               << dstFunc << ":" << dstLine  
//                               << ", Weight: " << edgeWeight 
//                               << ", Domains: " << x[i] << " -> " << x[j] << "\n";
//         }
//         else if (neighbor->getEdgeType() == pdg::EdgeType::CONTROLDEP_CALLINV) {
//             auto [srcFunc, srcLine] = extractFunctionAndLine(i);
//             // if (srcFunc=="telrcv") {
//             //     continue;
//             // }

//             callinv_edges++;
//             // if (callinv_edges >50) {
//             //     continue;
//             // }
//             pdg::Node* entrynode = neighbor->getDstNode();

//             std::string calleeFuncName = entrynode->getStatement();
//             if (srcFunc=="auth_check"){
//                 std::cerr << "auth_check callinv edge: " << calleeFuncName << "\n";
//             }
//             pdg::Node* firstnode = _QG->getFunFirstNodeMap()[calleeFuncName];
//             if (firstnode) {
//                 if (srcFunc=="auth_check"){
//                     std::cerr << "auth_check callinv edge1: " << calleeFuncName << "\n";
//                 }
//                 int j = findNodeIndex(firstnode);
//                 if (j != -1) {
//                     if (srcFunc=="auth_check"){
//                         std::cerr << "auth_check callinv edge2: " << calleeFuncName << "\n";
//                     }
//                     switch_term_count++;
//                     int edgeWeight = _QG->getEdgeWeight(node, firstnode);
//                     // if (srcFunc=="auth_check"){
//                     //     std::cerr << "auth_check callinv edge3: " << edgeWeight << "\n";
//                     //     std:cerr <<"node: "<<node->getStatement()<<"\n";
//                     //     std::cerr<<"firstnode: "<<firstnode->getStatement()<<"\n";
//                     // }
//                     // if (srcFunc=="handle_read" || srcFunc=="really_start_request" || srcFunc=="main") 
//                     // edgeWeight = 0;
//                     if (edgeWeight == 0) {
//                         //将i和j分配在同一个域中
//                         // opt.add(x[i] == x[j]);
//                         //将j分配到0
//                         // opt.add(x[j] == c.bool_val(0));
//                         continue;
//                     }
//                     switch_terms.push_back(ite(x[i] != x[j], c.bv_val(edgeWeight, 32), c.bv_val(0, 32)));
//                     //如果x[i]的值是1，则x[j]的值也为1
                    
//                     // opt.add(implies(x[i], x[j]));
//                     // std::string calleeFuncName = node_func_names[j];
//                     // for (int k : func_nodes[calleeFuncName]) {
//                     //     opt.add(implies(x[j], x[k]));
//                     // }


//                     //请输出此处边的src信息和dst信息和权重
//                     if (srcFunc=="auth_check"){
//                         std::cerr << "auth_check callinv edge2: " << calleeFuncName << "\n";
//                     }
                    
//                     auto [dstFunc, dstLine] = extractFunctionAndLine(j);
//                     std::cerr << "CALLINV_Edge: " << srcFunc << ":" << srcLine << " -> " 
//                               << dstFunc << ":" << dstLine  
//                               << ", Weight: " << edgeWeight 
//                               << ", Domains: " << x[i] << " -> " << x[j] << "\n";
                   
                    
//                 }
//             }
//         }
//     }
// }
// 在构建边界切换约束的循环中添加单向通信约束
// 找到计算switch_terms的位置

for (int i = 0; i < N; ++i) {
    pdg::Node* node = nodeMap[i];
    for (auto& neighbor : node->getOutEdgeSet()) {
        total_edges++;  
        if (neighbor->getEdgeType() == pdg::EdgeType::TYPE_SSE) {
            sse_edges++;
            int j = findNodeIndex(neighbor->getDstNode());
            if (j == -1) continue;
            
            // 添加单向通信约束：不允许从敏感域到不敏感域的边
            // 即如果x[i] == 1（敏感域）且有边到j，则x[j]必须也是1（敏感域）
            
            // opt.add(implies(x[i], x[j]));
            
            int edgeWeight = _QG->getEdgeWeight(node, neighbor->getDstNode());
            switch_term_count++;

            if (edgeWeight == 0) {
                continue;
            }
            
            // 由于添加了约束，域切换只会发生在非敏感域到敏感域的方向
            // 原来的写法可以保留，但实际上只有 x[i]=0 && x[j]=1 的情况会计入边权重
            switch_terms.push_back(ite(x[i] != x[j], c.bv_val(edgeWeight, 32), c.bv_val(0, 32)));
            
            auto [srcFunc, srcLine] = extractFunctionAndLine(i);
            auto [dstFunc, dstLine] = extractFunctionAndLine(j);
            std::cerr << "SSE_Edge: " << srcFunc << ":" << srcLine << " -> " 
                              << dstFunc << ":" << dstLine  
                              << ", Weight: " << edgeWeight 
                              << ", Domains: " << x[i] << " -> " << x[j] << "\n";
        }
        else if (neighbor->getEdgeType() == pdg::EdgeType::CONTROLDEP_CALLINV) {
            // 对于函数调用边也添加同样的约束
            auto [srcFunc, srcLine] = extractFunctionAndLine(i);
            
            callinv_edges++;
            pdg::Node* entrynode = neighbor->getDstNode();
            std::string calleeFuncName = entrynode->getStatement();
            
            pdg::Node* firstnode = _QG->getFunFirstNodeMap()[calleeFuncName];
            if (firstnode) {
                int j = findNodeIndex(firstnode);
                if (j != -1) {
                    // 添加函数调用的单向通信约束
                    // opt.add(implies(x[i], x[j]));

                    switch_term_count++;
                    int edgeWeight = _QG->getEdgeWeight(node, firstnode);
                    if (edgeWeight == 0) {
                        continue;
                    }
                    
                    switch_terms.push_back(ite(x[i] != x[j], c.bv_val(edgeWeight, 32), c.bv_val(0, 32)));
                    
                    auto [dstFunc, dstLine] = extractFunctionAndLine(j);
                    std::cerr << "CALLINV_Edge: " << srcFunc << ":" << srcLine << " -> " 
                              << dstFunc << ":" << dstLine  
                              << ", Weight: " << edgeWeight 
                              << ", Domains: " << x[i] << " -> " << x[j] << "\n";
                }
            }
        }
    }
}
std::cerr << "Total nodes: " << N << "\n";
std::cerr << "Total edges: " << total_edges << "\n";
std::cerr << "SSE edges: " << sse_edges << "\n";
std::cerr << "CALLINV edges: " << callinv_edges << "\n";
std::cerr << "Number of terms in total_switches: " << switch_term_count << "\n";
for (unsigned i = 0; i < switch_terms.size(); ++i) {
    total_switches = total_switches + switch_terms[i];
}
opt.minimize(total_switches);
TIME_END(total_switches);


// 将求解得到的中间结果输出到文件中
std::ofstream out(Z3OutputFile);
if (!out.is_open()) {
    std::cerr << "Error: Cannot open " << Z3OutputFile << " for writing!\n";
    return 1;
}

// 查找多个解，最多20个
std::vector<std::vector<int>> solutions;
std::vector<double> solution_ratios; // 存储每个解的sz_ratio
const int MAX_FIND_SOLUTIONS = 20;  // 最多寻找的解数量
const int MAX_OUTPUT_SOLUTIONS = 5;  // 最优输出的解数量

// 在设置并行参数时添加
Z3_global_param_set("parallel.enable", "true");
Z3_global_param_set("parallel.max_threads", "32"); // 根据CPU核心数调整
Z3_global_param_set("parallel.threads.max", "32"); // 同样调整
Z3_global_param_set("sat.threads", "32"); // 设置SAT求解器的线程数
// Z3_global_param_set("smt.threads", "8"); // 设置SMT求解器的线程数
TIME_START(solve);
while (opt.check() == sat && solutions.size() < MAX_FIND_SOLUTIONS) {
    model m = opt.get_model();
    std::vector<int> solution;
    for (int i = 0; i < N; ++i) {
        // 从布尔变量提取值（0 或 1）
        solution.push_back(m.eval(x[i]).is_true() ? 1 : 0);
    }
    solutions.push_back(solution);

    // 计算当前解的total_switches值
    int current_switches = 0;
    std::vector<std::pair<int, int>> boundary_edges; // 存储边界边信息 (源节点索引, 目标节点索引)

    // 检查所有边，计算边界边的数量和权重总和
    for (int i = 0; i < N; ++i) {
        pdg::Node* node = nodeMap[i];
        for (auto& neighbor : node->getOutEdgeSet()) {
            if (neighbor->getEdgeType() == pdg::EdgeType::TYPE_SSE) {
                int j = findNodeIndex(neighbor->getDstNode());
                if (j == -1) continue;
                // 检查是否为边界边：x[i] != x[j]
                if (solution[i] != solution[j]) {
                    int edgeWeight = _QG->getEdgeWeight(node, neighbor->getDstNode());
                    current_switches += edgeWeight;
                    boundary_edges.emplace_back(i, j);
                }
            }else if (neighbor->getEdgeType() == pdg::EdgeType::CONTROLDEP_CALLINV) {
                pdg::Node* entrynode = neighbor->getDstNode();
                std::string calleeFuncName = entrynode->getStatement();
                pdg::Node* firstnode = _QG->getFunFirstNodeMap()[calleeFuncName];
                if (firstnode) {
                    int j = findNodeIndex(firstnode);
                    if (j != -1) {
                        // 检查是否为边界边：x[i] != x[j]
                        if (solution[i] != solution[j]) {
                            int edgeWeight = _QG->getEdgeWeight(node, firstnode);
                            current_switches += edgeWeight;
                            boundary_edges.emplace_back(i, j);
                        }
                    }
                }
            }
        }
    }

    // 输出到文件
    out << "Solution " << solutions.size() << ":\n";
    out << "Total Switches: " << current_switches << "\n";
    out << "Boundary Edges (" << boundary_edges.size() << "):\n";
    // 继续输出边界边详细信息（可选）
    for (const auto& [src, dst] : boundary_edges) {
        auto [srcFunc, srcLine] = extractFunctionAndLine(src);
        auto [dstFunc, dstLine] = extractFunctionAndLine(dst);
        int weight = _QG->getEdgeWeight(nodeMap[src], nodeMap[dst]);
        out << "  " << srcFunc << ":" << srcLine << " -> " 
            << dstFunc << ":" << dstLine << ", Weight: " << weight << 
            ", Domains: " << solution[src] << " -> " << solution[dst] << "\n";
    }
    out << "\n";

    // 分离敏感和非敏感节点
    std::vector<int> S_nodes, N_nodes;
    int S_sz_total = 0;
    for (int i = 0; i < N; ++i) {
        if (solution[i] == 1) {
            S_nodes.push_back(i);
            S_sz_total += sz[i];
        } else {
            N_nodes.push_back(i);
        }
    }

    

    // 按函数名和行号排序敏感节点
    std::sort(S_nodes.begin(), S_nodes.end(), [&](int a, int b) {
        auto [funcA, lineA] = extractFunctionAndLine(a);
        auto [funcB, lineB] = extractFunctionAndLine(b);
        if (funcA != funcB) return funcA < funcB;
        return lineA < lineB;
    });

    // 按函数名和行号排序非敏感节点
    std::sort(N_nodes.begin(), N_nodes.end(), [&](int a, int b) {
        auto [funcA, lineA] = extractFunctionAndLine(a);
        auto [funcB, lineB] = extractFunctionAndLine(b);
        if (funcA != funcB) return funcA < funcB;
        return lineA < lineB;
    });

    // 输出到文件
    out << "Solution " << solutions.size() << ":\n";
    out << "Sensitive Nodes: \n";
    std::string currentFunction = "";
    for (int i = 0; i < S_nodes.size(); ++i) {
        int nodeIdx = S_nodes[i];
        auto [funcName, _] = extractFunctionAndLine(nodeIdx);

        if (funcName != currentFunction) {
            out << "\n-- Function: " << funcName << " --\n";
            currentFunction = funcName;
        }

        out << nodeIdx << ": " << nodeMap[nodeIdx]->getStatement()
            << ": sz = " << sz[nodeIdx] << ", quan = " << quan[nodeIdx] << "\n";
    }

    out << "\nNon-sensitive Nodes: \n";
    currentFunction = "";
    for (int i = 0; i < N_nodes.size(); ++i) {
        int nodeIdx = N_nodes[i];
        auto [funcName, _] = extractFunctionAndLine(nodeIdx);

        if (funcName != currentFunction) {
            out << "\n-- Function: " << funcName << " --\n";
            currentFunction = funcName;
        }

        out << nodeIdx << ": " << nodeMap[nodeIdx]->getStatement()
            << ": sz = " << sz[nodeIdx] << ", quan = " << quan[nodeIdx] << "\n";
    }

    out << "\nSensitive Nodes Total sz: " << S_sz_total << "\n";
    double sz_ratio = static_cast<double>(S_sz_total) / total_sz;
    solution_ratios.push_back(sz_ratio);
    out << "Sensitive Nodes Total sz Ratio: " << sz_ratio << "\n\n";
    out.flush();

    // 添加阻塞子句
    TIME_START(block_clause);
    expr_vector block_clause(c);
    for (int i = 0; i < N; ++i) {
        // 将 solution[i] 转换为 Z3 布尔值
        bool sol_value = (solution[i] == 1);
        expr z3_sol = c.bool_val(sol_value); // 创建 Z3 布尔表达式
        block_clause.push_back(x[i] != z3_sol); // 比较 Z3 表达式
    }
    opt.add(mk_or(block_clause)); // 添加阻塞子句
    TIME_END(block_clause);
}
TIME_END(solve);

// 按 sz_ratio 从小到大排序解
std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;

for (size_t i = 0; i < solutions.size(); ++i) {
    pq.push({solution_ratios[i], static_cast<int>(i)});
}

// 输出最优的 5 个解
std::cout << "\n最优的 " << std::min(MAX_OUTPUT_SOLUTIONS, static_cast<int>(solutions.size())) << " 个解（按 sz_ratio 从小到大排序）:\n";
out << "\n最优的 " << std::min(MAX_OUTPUT_SOLUTIONS, static_cast<int>(solutions.size())) << " 个解（按 sz_ratio 从小到大排序）:\n";

int count = 0;
while (!pq.empty() && count < MAX_OUTPUT_SOLUTIONS) {
    auto [ratio, index] = pq.top();
    pq.pop();

    std::cout << "解 " << count + 1 << " (sz_ratio = " << ratio << "): ";
    out << "解 " << count + 1 << " (sz_ratio = " << ratio << "): ";
    
    for (int val : solutions[index]) {
        std::cout << val << " ";
        out << val << " ";
    }
    std::cout << std::endl;
    out << std::endl;

    // 输出详细信息到文件
    std::vector<int> S_nodes, N_nodes;
    int S_sz_total = 0;
    for (int i = 0; i < N; ++i) {
        if (solutions[index][i] == 1) {
            S_nodes.push_back(i);
            S_sz_total += sz[i];
        } else {
            N_nodes.push_back(i);
        }
    }

    out << "  Sensitive Nodes: ";
    for (int node : S_nodes) out << node << " ";
    out << std::endl;

    out << "  Non-sensitive Nodes: ";
    for (int node : N_nodes) out << node << " ";
    out << std::endl;

    out << "  Sensitive Nodes Total sz: " << S_sz_total << std::endl;
    out << "  Sensitive Nodes Total sz Ratio: " << ratio << std::endl << std::endl;

    count++;
}

// 如果没有解，输出无解
if (solutions.empty()) {
    out << "无解\n";
    std::cout << "无解\n";
}


}
char qg::QUANProgramGraph::ID = 0;
static RegisterPass<qg::QUANProgramGraph>
    PDG("bqg", "Program Dependency Graph Construction", false, true);

