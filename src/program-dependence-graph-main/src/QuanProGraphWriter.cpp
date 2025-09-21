#include "QuanProGraphWriter.hh"

char qg::QuanProgramGraphPrinter::ID = 0;
using namespace llvm;




static RegisterPass<qg::QuanProgramGraphPrinter>
    PDGPrinter("dot-qg",
               "Print statement-level  quantified program dependency graph of "
               "function to 'dot' file",
               false, false);