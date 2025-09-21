/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2012 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */

/* ===================================================================== */
/*
  @ORIGINAL_AUTHOR: Dongrui Zeng
*/

/* ===================================================================== */
/*! @file
 *  This file contains an ISA-portable PIN tool for tracing instructions
 */

#include "pin.H"
#include <iostream>
#include <fstream>
//#include <string>
using namespace std;

/* ===================================================================== */
/* Global Variables */
/* ===================================================================== */

std::ofstream TraceFile;

/* ===================================================================== */
/* Commandline Switches */
/* ===================================================================== */

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "callrelation.out", "specify trace file name");
KNOB<BOOL>   KnobPrintArgs(KNOB_MODE_WRITEONCE, "pintool", "a", "0", "print call arguments ");
//KNOB<BOOL>   KnobPrintArgs(KNOB_MODE_WRITEONCE, "pintool", "i", "0", "mark indirect calls ");

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This tool produces a call relation trace." << endl << endl;
    cerr << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

string invalid = "invalid";

/* ===================================================================== */
const string *Target2String(ADDRINT target)
{
  string name = RTN_FindNameByAddress(target);
    if (name == "") 
      return &invalid;
    else
      return new string(name);
}

const string *Addr2String(ADDRINT addr)
{
  IMG img = IMG_FindByAddress(addr);
  
  ADDRINT img_ent = IMG_LoadOffset(img);
  return new string(StringFromAddrint(addr - img_ent));
  /*
  SYM correct_sym = SYM_Invalid();
  for(SYM sym = IMG_RegsymHead(img); SYM_Valid(sym); sym = SYM_Next(sym)) {
    cout << StringFromAddrint(addr) << ":" << StringFromAddrint(SYM_Value(sym)) << endl;
    if(SYM_Value(sym) == addr) {
      correct_sym = sym;
      break;
    } 
    else if(SYM_Value(sym) < addr) { 
      if(SYM_Value(sym) > SYM_Value(correct_sym)) {
	correct_sym = sym;
      }
      } else {}
  }
  
  if( SYM_Valid(correct_sym) ) {
    const string& name = SYM_Name(correct_sym);
    string undecorated_name = 
      PIN_UndecorateSymbolName(name,UNDECORATION_NAME_ONLY);
    
    return new string(StringFromAddrint(addr));
  } else {
    return &invalid;
  }
  */
}

/* ===================================================================== */

VOID  do_call_args(const string *s, ADDRINT arg0)
{
    TraceFile << *s << "(" << arg0 << ",...)" << endl;
}

/* ===================================================================== */

VOID  do_call_args_indirect(ADDRINT target, BOOL taken, ADDRINT arg0)
{
    if( !taken ) return;
    
    const string *s = Target2String(target);
    do_call_args(s, arg0);

    if (s != &invalid)
        delete s;
}

/* ===================================================================== */

VOID  do_call(string *caller, ADDRINT callee_addr)
{
  //  PIN_LockClient();
  //if(IMG_IsMainExecutable(IMG_FindByAddress(caller_addr)) /*|| IMG_IsMainExecutable(IMG_FindByAddress(callee_addr))*/) {
  //const string *caller = Target2String(caller_addr);
  const string *callee = Target2String(callee_addr);
    //if(caller != &invalid) delete caller;
  TraceFile << *caller << ":" << *callee << endl;
  if(callee != &invalid) delete callee;
    //}
    //PIN_UnlockClient();
}

// VOID  do_clever_call(ADDRINT caller_addr, ADDRINT callee_addr)
// {
//   PIN_LockClient();
//   IMG caller_img = IMG_FindByAddress(caller_addr);
//   IMG callee_img = IMG_FindByAddress(callee_addr);
  
//   if( (IMG_Valid(caller_img) && IMG_IsMainExecutable(IMG_FindByAddress(caller_addr))) || 
//       (IMG_Valid(callee_img) && IMG_IsMainExecutable(IMG_FindByAddress(callee_addr))) ) {
//     const string *caller = Target2String(caller_addr);
//     const string *callee = Target2String(callee_addr);
//     // 获取call语句的源代码行号
//     string file;
//     INT32 line = -1;
//     PIN_GetSourceLocation(caller_addr, NULL, &line, &file);
//     if(caller->compare(*callee)) {
//       TraceFile << *caller << " " << *callee << " " << file << ":" << line << endl;
//     }
//     if(caller != &invalid) delete caller;
//     if(callee != &invalid) delete callee;
//   }
//   PIN_UnlockClient();
// }

VOID do_clever_call(ADDRINT caller_addr, ADDRINT callee_addr)
{
  PIN_LockClient();
  IMG caller_img = IMG_FindByAddress(caller_addr);
  IMG callee_img = IMG_FindByAddress(callee_addr);

  // 确保 caller 和 callee 都在主可执行文件中
  if (IMG_Valid(caller_img) && IMG_IsMainExecutable(caller_img) &&
      IMG_Valid(callee_img) && IMG_IsMainExecutable(callee_img)) {

    // 获取 caller 和 callee 的函数名
    const string *caller = Target2String(caller_addr);
    const string *callee = Target2String(callee_addr);

    // 获取 callsite 的源代码信息
    string file;
    INT32 line = 0;
    // 如果想要caller的行号，则用caller_addr。
    // 如果想要callee的行号，则用callee_addr。
    PIN_GetSourceLocation(caller_addr, NULL, &line, &file);

    // 如果成功获取行号，则打印文件和行号
    if (line > 0) {
      TraceFile << *caller << " " << *callee << " " << file << ":" << dec << line << endl;
    } else {
      // 调试用，如果主程序内部调用都找不到行号，可能有编译问题
      TraceFile << "Internal call line not found: " << *caller << " -> " << *callee << endl;
    }

    if (caller != &invalid) delete caller;
    if (callee != &invalid) delete callee;
  }
  PIN_UnlockClient();
}

VOID  do_read(ADDRINT curr_addr, ADDRINT read_addr)
{
  PIN_LockClient();
  IMG curr_img = IMG_FindByAddress(curr_addr);
  IMG read_img = IMG_FindByAddress(read_addr);
  
  if( (IMG_Valid(curr_img) && IMG_IsMainExecutable(curr_img)) &&
      (IMG_Valid(read_img) && IMG_IsMainExecutable(read_img)) ) {
    const string *curr_fun = Target2String(curr_addr);
    const string *read_global = Addr2String(read_addr);
    
    if(curr_fun != &invalid && read_global != &invalid ) {
      TraceFile << *read_global << " " << *curr_fun << endl;
    }
    if(curr_fun != &invalid) delete curr_fun;
    if(read_global != &invalid) delete read_global;
  }
  PIN_UnlockClient();
}

VOID  do_write(ADDRINT curr_addr, ADDRINT write_addr) 
{
  PIN_LockClient();
  IMG curr_img = IMG_FindByAddress(curr_addr);
  IMG write_img = IMG_FindByAddress(write_addr);
  
  if( (IMG_Valid(curr_img) && IMG_IsMainExecutable(curr_img)) &&
      (IMG_Valid(write_img) && IMG_IsMainExecutable(write_img)) ) {
    const string *curr_fun = Target2String(curr_addr);
    const string *write_global = Addr2String(write_addr);

    if(curr_fun != &invalid && write_global != &invalid ) {
      TraceFile << *curr_fun << " " << *write_global << endl;
    }
    if(curr_fun != &invalid) delete curr_fun;
    if(write_global != &invalid) delete write_global;
  }
  PIN_UnlockClient();
}

/* ===================================================================== */

VOID  do_call_indirect(string *caller, ADDRINT callee_addr, BOOL taken)
{
    if( !taken ) return;
    //PIN_LockClient();
    //if(IMG_IsMainExecutable(IMG_FindByAddress(caller_addr)) /*|| IMG_IsMainExecutable(IMG_FindByAddress(callee_addr))*/) {
    //const string *caller = Target2String(caller_addr);
      do_call( caller, callee_addr );
      //}
//    PIN_UnlockClient();
}

/* ===================================================================== */

VOID Instruction(INS ins, VOID *v)
{
  if( (INS_IsCall(ins) || INS_IsBranch(ins)) && !INS_IsRet(ins) ) {
    
    INS_InsertPredicatedCall(ins, IPOINT_BEFORE, AFUNPTR(do_clever_call),
			     IARG_INST_PTR, 
			     IARG_BRANCH_TARGET_ADDR, IARG_END);
    
  } else {
    if( INS_IsMemoryRead(ins) ) {
      if( !INS_HasMemoryRead2(ins) ) {// single memory read
	INS_InsertPredicatedCall(ins, IPOINT_BEFORE, AFUNPTR(do_read),
				 IARG_INST_PTR,
				 IARG_MEMORYREAD_EA,
				 IARG_END);
      } else { // double memory reads
	INS_InsertPredicatedCall(ins, IPOINT_BEFORE, AFUNPTR(do_read),
				 IARG_INST_PTR,
				 IARG_MEMORYREAD_EA,
				 IARG_END); 
	INS_InsertPredicatedCall(ins, IPOINT_BEFORE, AFUNPTR(do_read),
				 IARG_INST_PTR,
				 IARG_MEMORYREAD2_EA,
				 IARG_END); 
      }
    }
    if( INS_IsMemoryWrite(ins) ) {
      INS_InsertPredicatedCall(ins, IPOINT_BEFORE, AFUNPTR(do_write),
			       IARG_INST_PTR,
			       IARG_MEMORYWRITE_EA,
			       IARG_END);
    }
  }
}
/* ===================================================================== */

VOID Fini(INT32 code, VOID *v)
{
  //TraceFile << "# eof" << endl;
    
    TraceFile.close();
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int  main(int argc, char *argv[])
{
    
    PIN_InitSymbols();

    if( PIN_Init(argc,argv) )
    {
        return Usage();
    }
    

    TraceFile.open(KnobOutputFile.Value().c_str());

    TraceFile << hex;
    TraceFile.setf(ios::showbase);
    
    //string trace_header = string("#\n"
    //                             "# Call Trace Generated By Pin\n"
    //                             "#\n");
    

    //TraceFile.write(trace_header.c_str(),trace_header.size());

    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);

    // Never returns

    PIN_StartProgram();
    
    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
