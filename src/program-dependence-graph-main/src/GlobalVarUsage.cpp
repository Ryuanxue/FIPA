// #include "llvm/IR/Module.h"
// #include "llvm/IR/Function.h"
// #include "llvm/IR/Instructions.h"
// #include "llvm/IR/GlobalVariable.h"
// #include "llvm/IR/DebugInfo.h"
// #include "llvm/IR/DebugLoc.h"
// #include "llvm/Pass.h"
// #include "llvm/Support/raw_ostream.h"

// using namespace llvm;

// namespace {
//     struct GlobalVarUsagePass : public ModulePass {
//         static char ID;
//         GlobalVarUsagePass() : ModulePass(ID) {}

//         bool runOnModule(Module &M) override {
//             // 遍历所有全局变量
//             for (GlobalVariable &GV : M.globals()) {
//                 // 输出全局变量名称
//                 errs() << "Global Variable: " << GV.getName() << "\n";
//                 //如果变量名以"."开头，则跳过
//                 if (GV.getName().startswith(".")) {
//                     continue;
//                 }
//                 //如果变量名以“__PRETTY_FUNCTION__”开头，则跳过
//                 if (GV.getName().startswith("__PRETTY_FUNCTION__")) {
//                     continue;
//                 }

//                 // 遍历全局变量的所有使用者
//                 for (User *U : GV.users()) {
//                  int temp=   analyzeUser(U, &GV);
//                  if (temp==0){
//                     errs() << "  cant judge modify or not\n";
//                  }
//                 }
//             }
//             return false; // 未修改模块
//         }

//         int analyzeUser(User *U, GlobalVariable *GV) {
//             // errs() << "  Used by: " << *U << "\n";
//             if (Instruction *I = dyn_cast<Instruction>(U)) {
//                 errs() << "  Instruction: " << *I << "\n";
//                 // 获取调试信息
//                 const DebugLoc &DL = I->getDebugLoc();
//                 if (DL) {
//                     errs() << "  Location: " << DL->getFilename() << ":" << DL.getLine() << "\n";
//                 }

//                 // 区分读和写操作
//                 if (LoadInst *LI = dyn_cast<LoadInst>(I)) {
//                     if (isGEPBasedOnGlobal(LI->getPointerOperand(), GV)) {
//                         errs() << "  Read at: " << *LI << "\n";
//                         return 1;
//                     }
//                 } else if (StoreInst *SI = dyn_cast<StoreInst>(I)) {
//                     //此处应该区分store的第一个操作数和第二个操作数，第一个操作数是存储的值，第二个操作数是存储的地址
//                     //如果第一个操作数是全局变量，则是读操作，如果第二个操作数是全局变量，则是写操作
//                     // 检查存储的目标地址（第二个操作数）是否基于全局变量
//                     if (isGEPBasedOnGlobal(SI->getPointerOperand(), GV)) {
//                         errs() << "  Write at: " << *SI << "\n";
//                         return 1;
//                     }
//                     // 检查要存储的值（第一个操作数）是否基于全局变量
//                     if (isGEPBasedOnGlobal(SI->getValueOperand(), GV)) {
//                         errs() << "  Read at: " << *SI << "\n";
//                         return 1;
//                     }
//                 } else if (CallInst *CI = dyn_cast<CallInst>(I)) {
//                     // 处理函数调用
//                             errs() << "  Write at: " << *CI << "\n";
//                             return 1;
                        
                    
//                 } else if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(I)) {
//                     // 处理指针或数组访问
//                     errs() << "  GEP at: " << *GEP << "\n";
//                     // 递归分析 GEP 的使用者
//                     for (User *GEPUser : GEP->users()) {
//                      return   analyzeUser(GEPUser, GV);
//                     }
//                 }//ret i8* getelementptr inbounds ([33 x i8], [33 x i8]* @calculate_skey_response.buf, i64 0, i64 0), !dbg !1443
//                 else if (ReturnInst *RI = dyn_cast<ReturnInst>(U)) {
//                     // 如果 User 是返回指令，递归追踪其使用者
//                     errs() << " Read at: " << *RI << "\n";
//                     return 1;

//                 }
//             }else if (ConstantExpr *CE = dyn_cast<ConstantExpr>(U)) {
//                 // 如果 User 是常量表达式，递归追踪其使用者
//                 errs() << "  ConstantExpr: " << *CE << "\n";
//                 for (User *CEUser : CE->users()) {
//                    return analyzeUser(CEUser, GV);
//                 }
//             }else{
//                 errs() << "  Other in : " << *U << "\n";
//             }
            
//             return 0;
//         }

//         bool isGEPBasedOnGlobal(Value *V, GlobalVariable *GV) {
//             errs() << "    Check: " << *V << "\n";
//             if (V == GV) {
//                 return true; // 直接使用全局变量
//             }

//             // 处理 GetElementPtrInst
//             if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(V)) {
//                 return isGEPBasedOnGlobal(GEP->getPointerOperand(), GV); // 递归检查 GEP 的操作数
//             }

//             // 处理 ConstantExpr
//             else if (ConstantExpr *CE = dyn_cast<ConstantExpr>(V)) {
//                 errs() << "    ConstantExpr: " << *CE << "\n";
//                 if (CE->getOpcode() == Instruction::GetElementPtr) {
//                     // 递归检查 GEP 操作数
//                     return isGEPBasedOnGlobal(CE->getOperand(0), GV);
//                 }
//                 //%struct.sockaddr* bitcast (%struct.sockaddr_in* @bindport.srv to %struct.sockaddr*)
//                 else if (CE->getOpcode() == Instruction::BitCast) {
//                     return isGEPBasedOnGlobal(CE->getOperand(0), GV); // 递归检查 bitcast 操作数
//                 }
//             }
//             else if (BitCastInst *BC = dyn_cast<BitCastInst>(V)) {
//                 errs() << "    BitCast: " << *BC << "\n";
//                 return isGEPBasedOnGlobal(BC->getOperand(0), GV); // 递归检查 bitcast 操作数
//             }else{
//                 errs() << "    Other: " << *V << "\n";  
//             }

//     return false;
// }
//     };
// }

// char GlobalVarUsagePass::ID = 0;
// static RegisterPass<GlobalVarUsagePass> X("global-var-usage", "Global Variable Usage Pass", false, false);


//================================================================================================
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h" // 添加此头文件以使用 cl::opt
#include <map>
#include <set>

using namespace llvm;

namespace {
    // 添加一个命令行选项来指定输出文件名
    static cl::opt<std::string> GvuOutputFile(
        "gvu-output", 
        cl::desc("Global Var Usage XML output file path"), 
        cl::init("global_var_usage.xml"));

    struct GlobalVarUsagePass : public ModulePass {
        static char ID;
        GlobalVarUsagePass() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            for (GlobalVariable &GV : M.globals()) {
                if (GV.getName().startswith(".") || GV.getName().startswith("__PRETTY_FUNCTION__")) {
                    continue;
                }
                globalDeclLocations[&GV] = getDeclLocation(&GV);
            }

            for (GlobalVariable &GV : M.globals()) {
                if (GV.getName().startswith(".") || GV.getName().startswith("__PRETTY_FUNCTION__")) {
                    continue;
                }
                for (User *U : GV.users()) {
                    analyzeUser(U, &GV);
                }
            }

            generateXML();
            return false;
        }

        int analyzeUser(User *U, GlobalVariable *GV) {
            if (Instruction *I = dyn_cast<Instruction>(U)) {
                Function *F = I->getFunction();
                if (F) {
                    functionToGlobals[F].insert(GV);
                }

                if (LoadInst *LI = dyn_cast<LoadInst>(I)) {
                    if (isGEPBasedOnGlobal(LI->getPointerOperand(), GV)) {
                        return 1;
                    }
                } else if (StoreInst *SI = dyn_cast<StoreInst>(I)) {
                    if (isGEPBasedOnGlobal(SI->getPointerOperand(), GV)) {
                        globalWriteStatus[GV] = true;
                        return 1;
                    }
                    if (isGEPBasedOnGlobal(SI->getValueOperand(), GV)) {
                        return 1;
                    }
                } else if (CallInst *CI = dyn_cast<CallInst>(I)) {
                    globalWriteStatus[GV] = true;
                    return 1;
                } else if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(I)) {
                    for (User *GEPUser : GEP->users()) {
                        return analyzeUser(GEPUser, GV);
                    }
                } else if (ReturnInst *RI = dyn_cast<ReturnInst>(I)) {
                    return 1;
                }
            } else if (ConstantExpr *CE = dyn_cast<ConstantExpr>(U)) {
                for (User *CEUser : CE->users()) {
                    return analyzeUser(CEUser, GV);
                }
            }
            return 0;
        }

        bool isGEPBasedOnGlobal(Value *V, GlobalVariable *GV) {
            if (V == GV) {
                return true;
            }
            if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(V)) {
                return isGEPBasedOnGlobal(GEP->getPointerOperand(), GV);
            }
            if (ConstantExpr *CE = dyn_cast<ConstantExpr>(V)) {
                if (CE->getOpcode() == Instruction::GetElementPtr) {
                    return isGEPBasedOnGlobal(CE->getOperand(0), GV);
                } else if (CE->getOpcode() == Instruction::BitCast) {
                    return isGEPBasedOnGlobal(CE->getOperand(0), GV);
                }
            }
            if (BitCastInst *BC = dyn_cast<BitCastInst>(V)) {
                return isGEPBasedOnGlobal(BC->getOperand(0), GV);
            }
            return false;
        }

        std::string getDeclLocation(GlobalVariable *GV) {
            if (MDNode *DbgNode = GV->getMetadata("dbg")) {
                if (DIGlobalVariableExpression *DIGVE = dyn_cast<DIGlobalVariableExpression>(DbgNode)) {
                    DIGlobalVariable *DIGV = DIGVE->getVariable();
                    if (DIGV) {
                        return DIGV->getFilename().str() + ":" + std::to_string(DIGV->getLine());
                    }
                }
            }
            return "unknown";
        }
       

        void generateXML() {
            std::error_code EC;
            // 使用命令行参数指定的文件名
            raw_fd_ostream xmlFile(GvuOutputFile, EC);
            if (EC) {
                errs() << "Could not open file: " << EC.message() << "\n";
                return;
            }

            xmlFile << "<root>\n";
            for (const auto &pair : functionToGlobals) {
                Function *F = pair.first;
                const std::set<GlobalVariable*> &globals = pair.second;
                xmlFile << "  <fun-gval-use funname=\"" << F->getName() << "\" gvallist=\"";
                for (auto it = globals.begin(); it != globals.end(); ++it) {
                    if (it != globals.begin()) xmlFile << ",";
                    xmlFile << (*it)->getName();
                }
                xmlFile << "\"/>\n";
            }
            for (const auto &pair : globalDeclLocations) {
                GlobalVariable *GV = pair.first;
                std::string rwproperty = globalWriteStatus[GV] ? "not-read-only" : "read-only";
                xmlFile << "  <gval valname=\"" << GV->getName() << "\" rwproperty=\"" << rwproperty 
                        << "\" decllocation=\"" << pair.second << "\"/>\n";
            }
            xmlFile << "</root>\n";
            xmlFile.close();
        }

    private:
        std::map<Function*, std::set<GlobalVariable*>> functionToGlobals;
        std::map<GlobalVariable*, bool> globalWriteStatus;
        std::map<GlobalVariable*, std::string> globalDeclLocations;
    };
}

char GlobalVarUsagePass::ID = 0;
static RegisterPass<GlobalVarUsagePass> X("global-var-usage", "Global Variable Usage Pass", false, false);