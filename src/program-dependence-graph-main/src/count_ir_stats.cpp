#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Pass.h"
#include <set>
#include <string>
#include <utility>
#include <map>
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;

namespace {
  struct IRStatsPass : public ModulePass {
    static char ID;
    IRStatsPass() : ModulePass(ID) {}

    bool runOnModule(Module &M) override {
      unsigned instructionCount = 0;
      unsigned functionCount = 0;
      unsigned globalVarCount = 0;
      unsigned totalFunctionLines = 0;

      // Count global variables
      for (GlobalVariable &GV : M.globals()) {
        //打印所有全局变量的名称
        
        //如果变量名以"."开头，则跳过, 跳过stdout和stderr
        if (GV.getName().startswith(".")) {
          continue;
        }
        if (GV.getName().startswith("__PRETTY_FUNCTION__")) {
          continue;
        }
        errs() << "Global Variable: " << GV.getName() << "\n";
        globalVarCount++;
      }

      // Count functions and instructions
      for (Function &F : M) {
        if (!F.isDeclaration()) { // Exclude function declarations
            //排除函数体为空的函数
            if (F.empty()) {
              continue;
            }
            //打印函数名
            errs() << "Function: " << F.getName() << "\n";
            functionCount++;

            unsigned startLine = 0;
            unsigned endLine = 0;

            if (DISubprogram *SP = F.getSubprogram()) {
                startLine = SP->getLine();
                endLine = startLine; // Initialize endLine with startLine
            }

            for (BasicBlock &BB : F) {
                for (Instruction &I : BB) {
                    instructionCount++;
                    if (const DebugLoc &DL = I.getDebugLoc()) {
                        if (unsigned line = DL.getLine()) {
                            if (line > endLine) {
                                endLine = line;
                            }
                        }
                    }
                }
            }

            if (startLine != 0) {
                errs() << "  Source line range: " << startLine << " - " << endLine << "\n";
                totalFunctionLines += (endLine - startLine + 2);
            } else {
                errs() << "  Source line information not available.\n";
            }
        }
      }

      // Output results
      errs() << "LLVM IR Statistics:\n";
      errs() << "Number of Instructions: " << instructionCount << "\n";
      errs() << "Number of Functions: " << functionCount << "\n";
      errs() << "Number of Global Variables: " << globalVarCount << "\n";
      errs() << "Total Lines of Function Source Code: " << totalFunctionLines << "\n";

      return false; // Pass does not modify the IR
    }
  };
}

char IRStatsPass::ID = 0;
static RegisterPass<IRStatsPass> X("irstats", "Count LLVM IR instructions, functions, and global variables", false, false);