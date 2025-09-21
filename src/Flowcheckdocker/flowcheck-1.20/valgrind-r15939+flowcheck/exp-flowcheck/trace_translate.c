/*--------------------------------------------------------------------*/
/*--- VEX IR transformations related to constructing a data-flow   ---*/
/*--- graph of program execution.                                  ---*/
/*---                                            trace_translate.c ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of FlowCheck, a heavyweight Valgrind tool for
   detecting leakage of secret information.

   By Stephen McCamant, MIT CSAIL Program Analsis group, Copyright (C)
   2007-2008.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/

#include "pub_tool_basics.h"
#include "pub_tool_hashtable.h"     // For fc_include.h
#include "pub_tool_libcassert.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_poolalloc.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_machine.h"     // VG_(fnptr_to_fnentry)
#include "pub_tool_xarray.h"
#include "fc_include.h"
#include "fc_translate.h"
#include "trace_translate.h"
#include "trace_runtime.h"
#include "popcount.h"
#include "libvex_guest_offsets.h"
#include "libvex_ir.h"


/* Find the tmp currently shadowing the given original tmp.  If none
   so far exists, allocate one.  */
static IRTemp
findShadowTmp_trace(FCEnv *fce, TraceEnv *trace_env, IRTemp orig) {
   (void)trace_env;
   return findShadowTmpT(fce, orig);
}

/* assign value to tmp */
#define assign(_bb,_tmp,_expr)   \
   addStmtToIRSB((_bb), IRStmt_WrTmp((_tmp),(_expr)))

/* add stmt to a bb */
#define stmt(_bb,_stmt)    \
   addStmtToIRSB((_bb), (_stmt))

/* build various kinds of expressions */
#define binop(_op, _arg1, _arg2) IRExpr_Binop((_op),(_arg1),(_arg2))
#define unop(_op, _arg)          IRExpr_Unop((_op),(_arg))
#define mkU8(_n)                 IRExpr_Const(IRConst_U8(_n))
#define mkU16(_n)                IRExpr_Const(IRConst_U16(_n))
#define mkU32(_n)                IRExpr_Const(IRConst_U32(_n))
#define mkU64(_n)                IRExpr_Const(IRConst_U64(_n))
#define mkV128(_n)               IRExpr_Const(IRConst_V128(_n))
#define mkexpr(_tmp)             IRExpr_RdTmp((_tmp))

/* bind the given expression to a new temporary, and return the
   temporary.  This effectively converts an arbitrary expression into
   an atom. */
static IRAtom* assignNewT ( FCEnv* fce, IRType ty, IRExpr* e ) {
   return assignNew('T', fce, ty, e);
}

/* (used for sanity checks only): is this an atom which looks
   like it's from original code? */
static Bool isOriginalAtom_trace(FCEnv* fce, TraceEnv *trace_env, IRAtom* a1) {
   if (a1->tag == Iex_Const)
      return True;
   if (a1->tag == Iex_RdTmp && a1->Iex.RdTmp.tmp <  fce->sb->tyenv->types_used)
      return True;
   return False;
}

/* (used for sanity checks only): is this an atom which looks
   like it's from shadow code? */
static Bool isShadowAtom_trace(FCEnv* fce, TraceEnv *trace_env, IRAtom* a1) {
   return isShadowAtom(fce, a1);
}

static IRExpr *
shadow_Get_trace(FCEnv *fce, TraceEnv *trace_env, Int offset, IRType ty) {
   tl_assert(ty != Ity_I1);

   return IRExpr_Get( (8 * offset) +
		      FC_(shadow_guest_offset) + FC_(tags_guest_offset),
                      Ity_I64); // Tags are always 64 bits
}

static IRExpr *
shadow_GetI_trace(FCEnv *fce, TraceEnv *trace_env, IRRegArray *descr,
		  IRAtom *ix, Int bias) {
   IRType ty   = descr->elemTy;
   IRRegArray *new_descr;
   tl_assert(ty != Ity_I1);
   tl_assert(isOriginalAtom_trace(fce, trace_env, ix));
   new_descr
      = mkIRRegArray((8 * descr->base) +
		     FC_(shadow_guest_offset) + FC_(tags_guest_offset),
		     Ity_I64, descr->nElems); // Tags are always 64 bits

   return IRExpr_GetI(new_descr, ix, bias);
}

static IRExpr *
make_popcount(FCEnv *fce, TraceEnv *trace_env, IRExpr *expr) {
   IRExpr *pop_expr;
   IRExpr *widened;
   IRType ty = typeOfIRExpr(fce->sb->tyenv, expr);

   if (expr->tag == Iex_Const) {
      switch (expr->Iex.Const.con->tag) {
      case Ico_U1:
	 return IRExpr_Const(IRConst_U32(expr->Iex.Const.con->Ico.U1));
      case Ico_U8:
	 {
	    UChar val = expr->Iex.Const.con->Ico.U8;
	    return IRExpr_Const(IRConst_U32(popcount8(val)));
	 }
      case Ico_U16:
	 {
	    UShort val = expr->Iex.Const.con->Ico.U16;
	    return IRExpr_Const(IRConst_U32(popcount16(val)));
	 }
      case Ico_U32:
	 {
	    UInt val = expr->Iex.Const.con->Ico.U32;
	    return IRExpr_Const(IRConst_U32(popcount32(val)));
	 }
      case Ico_U64:
	 {
	    ULong val = expr->Iex.Const.con->Ico.U64;
	    return IRExpr_Const(IRConst_U32(popcount64(val)));
	 }
    case Ico_V128: //add by liu sen
 	 { 
 	    UShort val = expr->Iex.Const.con->Ico.V128;
 	    return IRExpr_Const(IRConst_U32(8*popcount16(val)));
 	 }
      default:
	 VG_(tool_panic)("flowcheck trace:make_popcount constant");
      }
   }

   switch (ty) {
   case Ity_I1:
      pop_expr = IRExpr_Unop(Iop_1Uto32, expr);
      break;
   case Ity_I8:
      widened = assignNewT(fce, Ity_I32, IRExpr_Unop(Iop_8Uto32, expr));
      pop_expr = mkIRExprCCall(Ity_I32, 0, "FC_(helperc_POPCOUNT8)",
			       &FC_(helperc_POPCOUNT8),
			       mkIRExprVec_1(widened));
      break;
   case Ity_I16:
      widened = assignNewT(fce, Ity_I32, IRExpr_Unop(Iop_16Uto32, expr));
      pop_expr = mkIRExprCCall(Ity_I32, 0, "FC_(helperc_POPCOUNT16)",
			       &FC_(helperc_POPCOUNT16),
			       mkIRExprVec_1(widened));
      break;
   case Ity_I32:
      pop_expr = mkIRExprCCall(Ity_I32, 0, "FC_(helperc_POPCOUNT32)",
			       &FC_(helperc_POPCOUNT32), mkIRExprVec_1(expr));
      break;
   case Ity_I64:
      pop_expr = mkIRExprCCall(Ity_I32, 0, "FC_(helperc_POPCOUNT64)",
			       &FC_(helperc_POPCOUNT64), mkIRExprVec_1(expr));
      break;

   case Ity_V128:{ //add by liu sen
       IRExpr *loHalf = assignNewT(fce, Ity_I64, IRExpr_Unop(Iop_V128to64, expr));
       IRExpr *hiHalf = assignNewT(fce, Ity_I64, IRExpr_Unop(Iop_V128HIto64, expr));
       pop_expr = mkIRExprCCall(Ity_I32, 0, "FC_(helperc_POPCOUNT128)",
 			       &FC_(helperc_POPCOUNT128), mkIRExprVec_2(loHalf, hiHalf));
 	break;
    }  
   default:
      ppIRType(ty);
      VG_(tool_panic)("flowcheck trace:make_popcount");
      return 0;
   }
   return assignNewT(fce, Ity_I32, pop_expr);
}

static IRExpr *
expr2tag_Unop_trace(FCEnv *fce, TraceEnv *trace_env, IRAtom *atom) {
   IRAtom *tatom = expr2tag_trace(fce, trace_env, atom);
   IRAtom *vatom = expr2vbits(fce, atom);
   IRAtom *pop;
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom));
   tl_assert(isShadowAtom(fce, vatom));

   if (atom->tag == Iex_Const)
     return tatom;

   pop = make_popcount(fce, trace_env, vatom);

   /* Even though this helper has side-effects (i.e., sometimes printing
      an edge to the trace), it's a clean call because it's appropriate to
      optimize away if its results are not used. */
   return mkIRExprCCall(Ity_I64, 0, "FC_(helperc_UNARY_TAG)",
			&FC_(helperc_UNARY_TAG),
			mkIRExprVec_3(tatom, pop,
				      mkU64(trace_env->location++)));
}

static IRExpr *
expr2tag_Binop_trace(FCEnv *fce, TraceEnv *trace_env, IROp op,
		      IRAtom *atom1, IRAtom *atom2) {
   IRAtom *tatom1 = expr2tag_trace(fce, trace_env, atom1);
   IRAtom *tatom2 = expr2tag_trace(fce, trace_env, atom2);
   IRAtom *vatom1 = expr2vbits(fce, atom1);
   IRAtom *vatom2 = expr2vbits(fce, atom2);
   IRAtom *pop1, *pop2;

   tl_assert(isOriginalAtom_trace(fce, trace_env, atom1));
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom2));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom1));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom2));

   if (atom1->tag == Iex_Const && atom2->tag == Iex_Const) {
      return tatom1;
   }

   pop1 = make_popcount(fce, trace_env, vatom1);
   pop2 = make_popcount(fce, trace_env, vatom2);

   /* Even though this helper has side-effects (i.e., sometimes printing
      edges to the trace), it's a clean call because it's appropriate to
      optimize away if its results are not used. */
   return mkIRExprCCall(Ity_I64, 0, "FC_(helperc_BINARY_TAGS)",
			&FC_(helperc_BINARY_TAGS),
			mkIRExprVec_5(tatom1, tatom2, pop1, pop2,
				      mkU64(trace_env->location++)));
}

static IRExpr *
expr2tag_Triop_trace(FCEnv *fce, TraceEnv *trace_env, IROp op,
		      IRAtom *atom1, IRAtom *atom2, IRAtom *atom3) {
   IRAtom* tatom1 = expr2tag_trace(fce, trace_env, atom1);
   IRAtom* tatom2 = expr2tag_trace(fce, trace_env, atom2);
   IRAtom* tatom3 = expr2tag_trace(fce, trace_env, atom3);
   IRAtom *vatom1 = expr2vbits(fce, atom1);
   IRAtom *vatom2 = expr2vbits(fce, atom2);
   IRAtom *vatom3 = expr2vbits(fce, atom3);
   IRAtom *pop1, *pop2, *pop3;

   tl_assert(isOriginalAtom_trace(fce, trace_env, atom1));
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom2));
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom3));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom1));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom2));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom3));

   pop1 = make_popcount(fce, trace_env, vatom1);
   pop2 = make_popcount(fce, trace_env, vatom2);
   pop3 = make_popcount(fce, trace_env, vatom3);

   /* Even though this helper has side-effects (i.e., sometimes printing
      edges to the trace), it's a clean call because it's appropriate to
      optimize away if its results are not used. */
   return mkIRExprCCall(Ity_I64, 0, "FC_(helperc_TERNARY_TAGS)",
			&FC_(helperc_TERNARY_TAGS),
			mkIRExprVec_7(tatom1, tatom2, tatom3,
				      pop1, pop2, pop3,
				      mkU64(trace_env->location++)));
}

static IRExpr *
expr2tag_Qop_trace(FCEnv *fce, TraceEnv *trace_env, IROp op,
		    IRAtom *atom1, IRAtom *atom2,
		    IRAtom *atom3, IRAtom *atom4) {
   IRAtom* tatom1 = expr2tag_trace(fce, trace_env, atom1);
   IRAtom* tatom2 = expr2tag_trace(fce, trace_env, atom2);
   IRAtom* tatom3 = expr2tag_trace(fce, trace_env, atom3);
   IRAtom* tatom4 = expr2tag_trace(fce, trace_env, atom4);
   IRAtom *vatom1 = expr2vbits(fce, atom1);
   IRAtom *vatom2 = expr2vbits(fce, atom2);
   IRAtom *vatom3 = expr2vbits(fce, atom3);
   IRAtom *vatom4 = expr2vbits(fce, atom4);
   IRAtom *pop1, *pop2, *pop3, *pop4;

   tl_assert(isOriginalAtom_trace(fce, trace_env, atom1));
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom2));
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom3));
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom4));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom1));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom2));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom3));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom4));

   pop1 = make_popcount(fce, trace_env, vatom1);
   pop2 = make_popcount(fce, trace_env, vatom2);
   pop3 = make_popcount(fce, trace_env, vatom3);
   pop4 = make_popcount(fce, trace_env, vatom4);

   /* Even though this helper has side-effects (i.e., sometimes printing
      edges to the trace), it's a clean call because it's appropriate to
      optimize away if its results are not used. */
   return mkIRExprCCall(Ity_I64, 0, "FC_(helperc_FOURARY_TAGS)",
			&FC_(helperc_FOURARY_TAGS),
			mkIRExprVec_9(tatom1, tatom2, tatom3, tatom4,
				      pop1, pop2, pop3, pop4,
				      mkU64(trace_env->location++)));
}

static
IRExpr** mkIRExprVec_11 ( IRExpr* arg1, IRExpr* arg2, IRExpr* arg3,
			  IRExpr* arg4, IRExpr* arg5, IRExpr* arg6,
			  IRExpr* arg7, IRExpr *arg8, IRExpr *arg9,
			  IRExpr *arg10,IRExpr *arg11) {
   IRExpr** vec = LibVEX_Alloc(12 * sizeof(IRExpr*));
   vec[0] = arg1;
   vec[1] = arg2;
   vec[2] = arg3;
   vec[3] = arg4;
   vec[4] = arg5;
   vec[5] = arg6;
   vec[6] = arg7;
   vec[7] = arg8;
   vec[8] = arg9;
   vec[9] = arg10;
   vec[10] = arg11;
   vec[11] = NULL;
   return vec;
}

static IRExpr *
expr2tag_5op_trace(FCEnv *fce, TraceEnv *trace_env, IROp op,
		    IRAtom *atom1, IRAtom *atom2, IRAtom *atom3,
		    IRAtom *atom4, IRAtom *atom5) {
   IRAtom* tatom1 = expr2tag_trace(fce, trace_env, atom1);
   IRAtom* tatom2 = expr2tag_trace(fce, trace_env, atom2);
   IRAtom* tatom3 = expr2tag_trace(fce, trace_env, atom3);
   IRAtom* tatom4 = expr2tag_trace(fce, trace_env, atom4);
   IRAtom* tatom5 = expr2tag_trace(fce, trace_env, atom5);
   IRAtom *vatom1 = expr2vbits(fce, atom1);
   IRAtom *vatom2 = expr2vbits(fce, atom2);
   IRAtom *vatom3 = expr2vbits(fce, atom3);
   IRAtom *vatom4 = expr2vbits(fce, atom4);
   IRAtom *vatom5 = expr2vbits(fce, atom5);
   IRAtom *pop1, *pop2, *pop3, *pop4, *pop5;

   tl_assert(isOriginalAtom_trace(fce, trace_env, atom1));
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom2));
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom3));
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom4));
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom5));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom1));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom2));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom3));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom4));
   tl_assert(isShadowAtom_trace(fce, trace_env, tatom5));

   pop1 = make_popcount(fce, trace_env, vatom1);
   pop2 = make_popcount(fce, trace_env, vatom2);
   pop3 = make_popcount(fce, trace_env, vatom3);
   pop4 = make_popcount(fce, trace_env, vatom4);
   pop5 = make_popcount(fce, trace_env, vatom5);

   /* Even though this helper has side-effects (i.e., sometimes printing
      edges to the trace), it's a clean call because it's appropriate to
      optimize away if its results are not used. */
   return mkIRExprCCall(Ity_I64, 0, "FC_(helperc_FIVEARY_TAGS)",
			&FC_(helperc_FIVEARY_TAGS),
			mkIRExprVec_11(tatom1, tatom2, tatom3, tatom4, tatom5,
				       pop1, pop2, pop3, pop4, pop5,
				       mkU64(trace_env->location++)));
}

static IRAtom *
expr2tag_Load_trace(FCEnv *fce, TraceEnv *trace_env, IRType ty,
		     IRAtom *addr) {
   IRAtom *taddr;
   void *helper;
   const HChar *hname;
   IRTemp datatag;
   IRDirty *di;

   tl_assert(addr);
   tl_assert(isOriginalAtom_trace(fce, trace_env, addr));
   taddr = expr2tag_trace(fce, trace_env, addr);
   tl_assert(isShadowAtom_trace(fce, trace_env, taddr));

   int size=0;

   switch (ty) {
   case Ity_I8:
      helper = &FC_(helper_LOAD_TAG_1);
      hname  = "FC_(helper_LOAD_TAG_1)";
      size=1;
      break;
   case Ity_I16:
      helper = &FC_(helper_LOAD_TAG_2);
      hname  = "FC_(helper_LOAD_TAG_2)";
      size=2;
      break;
   case Ity_I32: case Ity_F32:
      helper = &FC_(helper_LOAD_TAG_4);
      hname  = "FC_(helper_LOAD_TAG_4)";
      size=4;
      break;
   case Ity_I64: case Ity_F64:
      helper = &FC_(helper_LOAD_TAG_8);
      hname  = "FC_(helper_LOAD_TAG_8)";
      size=8;
      break;
   case Ity_I128: case Ity_V128:
      helper = &FC_(helper_LOAD_TAG_16);
      hname  = "FC_(helper_LOAD_TAG_16)";
      size=16;
      break;
   default:
      ppIRType(ty);
      VG_(tool_panic)("flowcheck trace:expr2tag_Load_trace");


   }

   datatag = newTempT(fce);
   /* This is a dirty call because it can't be moved around writes to
      memory. */
   di = unsafeIRDirty_1_N(datatag, 0, hname, helper,
			  mkIRExprVec_3(addr, taddr,
					mkU64(trace_env->location++)));
   di->nFxState = 0;
   stmt(fce->sb, IRStmt_Dirty(di));


   //// add by xue
   di=unsafeIRDirty_0_N(0, "helperc_count_senaddr", &helperc_count_senaddr, //显示流
			  mkIRExprVec_3(addr, IRExpr_Const(IRConst_U32(size)), IRExpr_Const(IRConst_U32(1))));
           stmt(fce->sb, IRStmt_Dirty(di));

           //潜在流,data是地址
           
      di=unsafeIRDirty_0_N(0, "helperc_count_senaddr", &helperc_count_senaddr, //显示流
      mkIRExprVec_3(addr, IRExpr_Const(IRConst_U32(size)), IRExpr_Const(IRConst_U32(0))));
      stmt(fce->sb, IRStmt_Dirty(di));
           
   /////

   return mkexpr(datatag);
}

static IRAtom *
handleCCall_trace(FCEnv *fce, TraceEnv *trace_env, IRAtom **exprvec,
		  IRCallee* cee) {
   tl_assert(exprvec);
   tl_assert(exprvec[0]); // 0-arg clean calls don't make sense
   if (!exprvec[1]) {
      return expr2tag_Unop_trace(fce, trace_env, exprvec[0]);
   } else if (!exprvec[2]) {
      return expr2tag_Binop_trace(fce, trace_env, 0, exprvec[0], exprvec[1]);
   } else if (!exprvec[3]) {
      return expr2tag_Triop_trace(fce, trace_env, 0, exprvec[0], exprvec[1],
				   exprvec[2]);
   } else if (!exprvec[4]) {
      return expr2tag_Qop_trace(fce, trace_env, 0, exprvec[0], exprvec[1],
				 exprvec[2], exprvec[3]);
   } else if (!exprvec[5]) {
      return expr2tag_5op_trace(fce, trace_env, 0, exprvec[0], exprvec[1],
				 exprvec[2], exprvec[3], exprvec[4]);
   } else {
      VG_(tool_panic)("flowcheck trace: too many clean call args");
   }
}

IRExpr *expr2tag_trace(FCEnv *fce, TraceEnv *trace_env, IRExpr *e) {
   switch (e->tag) {
   case Iex_Get:
      return shadow_Get_trace(fce, trace_env, e->Iex.Get.offset,
			      e->Iex.Get.ty);

   case Iex_GetI:
      return shadow_GetI_trace(fce, trace_env, e->Iex.GetI.descr,
			       e->Iex.GetI.ix, e->Iex.GetI.bias);

   case Iex_RdTmp:
      return IRExpr_RdTmp( findShadowTmp_trace(fce, trace_env,
					       e->Iex.RdTmp.tmp));

   case Iex_Const:
      /* Literals are always public */
      return IRExpr_Const(IRConst_U64(0));

   case Iex_Unop:
      return expr2tag_Unop_trace(fce, trace_env, e->Iex.Unop.arg);

   case Iex_Binop:
      return expr2tag_Binop_trace(fce, trace_env, e->Iex.Binop.op,
				   e->Iex.Binop.arg1, e->Iex.Binop.arg2);

   case Iex_Triop:
      return expr2tag_Triop_trace(fce, trace_env, e->Iex.Triop.details->op,
				   e->Iex.Triop.details->arg1, e->Iex.Triop.details->arg2,
				   e->Iex.Triop.details->arg3);

   case Iex_Qop:
      return expr2tag_Qop_trace(fce, trace_env, e->Iex.Qop.details->op,
				 e->Iex.Qop.details->arg1, e->Iex.Qop.details->arg2,
				 e->Iex.Qop.details->arg3, e->Iex.Qop.details->arg4);

   case Iex_Load:
      return expr2tag_Load_trace(fce, trace_env, e->Iex.Load.ty,
				 e->Iex.Load.addr);

   case Iex_CCall:
      return handleCCall_trace(fce, trace_env, e->Iex.CCall.args,
			       e->Iex.CCall.cee);

   case Iex_ITE:
      return expr2tag_Triop_trace(fce, trace_env, 0,
				   e->Iex.ITE.cond, e->Iex.ITE.iftrue,
				   e->Iex.ITE.iffalse);

   default:
      VG_(printf)("\n");
      ppIRExpr(e);
      VG_(printf)("\n");
      VG_(tool_panic)("flowcheck trace: expr2tag_trace");
   }
}

static void do_shadow_Put_trace(FCEnv *fce, TraceEnv *trace_env, Int offset,
				IRAtom *atom) {
   IRAtom *tatom;
   IRType ty;
   tl_assert(isOriginalAtom_trace(fce, trace_env, atom));
   tatom = expr2tag_trace(fce, trace_env, atom);

   ty = typeOfIRExpr(fce->sb->tyenv, tatom);
   tl_assert(ty != Ity_I1);

   stmt(fce->sb,
	IRStmt_Put((8 * offset) +
		   FC_(shadow_guest_offset) + FC_(tags_guest_offset), tatom));
}

static void
do_shadow_PutI_trace(FCEnv *fce, TraceEnv *trace_env, IRRegArray *descr,
		     IRAtom *ix, Int bias, IRAtom *atom) {
   IRAtom *tatom;
   IRType ty;
   IRRegArray *new_descr;

   tl_assert(isOriginalAtom_trace(fce, trace_env, atom));
   tatom = expr2tag_trace(fce, trace_env, atom);
   ty = descr->elemTy;
   tl_assert(ty != Ity_I1);
   tl_assert(isOriginalAtom_trace(fce, trace_env, ix));

   new_descr =
      mkIRRegArray((8 * descr->base) +
		   FC_(shadow_guest_offset) + FC_(tags_guest_offset),
		   Ity_I64, descr->nElems); // tags are all 64 bits

   stmt(fce->sb, IRStmt_PutI(mkIRPutI(new_descr, ix, bias, tatom)));
}

static void
do_shadow_Store_trace(FCEnv *fce, TraceEnv *trace_env, IRAtom *addr,
		      IRAtom *data) {
   IRAtom *taddr, *tdata;
   void *helper;
   const HChar *hname;
   IRDirty *di;
   IRType ty;

   tl_assert(isOriginalAtom_trace(fce, trace_env, addr));
   taddr = expr2tag_trace(fce, trace_env, addr);
   tl_assert(isShadowAtom_trace(fce, trace_env, taddr));

   tl_assert(isOriginalAtom_trace(fce, trace_env, data));
   tdata = expr2tag_trace(fce, trace_env, data);
   tl_assert(isShadowAtom_trace(fce, trace_env, tdata));

   ty = typeOfIRExpr(fce->sb->tyenv, data);
   int size=0;//add by xue
   if(ty==Ity_I32)
  {
   di=unsafeIRDirty_0_N(0, "helperc_store_addr_val", &helperc_store_addr_val, //显示流
			  mkIRExprVec_3(addr, data,IRExpr_Const(IRConst_U32(4))));
           stmt(fce->sb, IRStmt_Dirty(di));
  }

   switch (ty) {
   case Ity_I8:
      helper = &FC_(helper_STORE_TAG_1);
      hname  = "FC_(helper_STORE_TAG_1)";
      size=1;
      break;
   case Ity_I16:
      helper = &FC_(helper_STORE_TAG_2);
      hname  = "FC_(helper_STORE_TAG_2)";
      size=2;
      break;
   case Ity_I32: case Ity_F32:
      helper = &FC_(helper_STORE_TAG_4);
      hname  = "FC_(helper_STORE_TAG_4)";
      size=4;
      break;
   case Ity_I64: case Ity_F64:
      helper = &FC_(helper_STORE_TAG_8);
      hname  = "FC_(helper_STORE_TAG_8)";
      size=8;
      break;
   case Ity_I128: case Ity_V128:
      helper = &FC_(helper_STORE_TAG_16);
      hname  = "FC_(helper_STORE_TAG_16)";
      size=16;
      break;
   default:
      ppIRType(ty);
      VG_(tool_panic)("flowcheck trace:do_shadow_Store_trace");
   }

   di = unsafeIRDirty_0_N(0, hname, helper,
			  mkIRExprVec_5(addr, taddr, tdata,
					mkU64(trace_env->location++),
					IRExpr_RdTmp(fce->rollbackModeTmp32)));
   di->nFxState = 0;
   stmt(fce->sb, IRStmt_Dirty(di));
  
  //// add by xue

  
  


   di=unsafeIRDirty_0_N(0, "helperc_count_senaddr", &helperc_count_senaddr, //显示流
			  mkIRExprVec_3(addr, IRExpr_Const(IRConst_U32(size)), IRExpr_Const(IRConst_U32(1))));
           stmt(fce->sb, IRStmt_Dirty(di));

   //潜在流,data是地址
   if(ty==Ity_I32){
   di=unsafeIRDirty_0_N(0, "helperc_count_senaddr", &helperc_count_senaddr, //潜在流
   mkIRExprVec_3(addr, data, IRExpr_Const(IRConst_U32(0))));
   stmt(fce->sb, IRStmt_Dirty(di));}
           
   /////
}

static void
do_shadow_cond_trace(FCEnv *fce, TraceEnv *trace_env, IRExpr *guard) {
   if (guard->tag == Iex_Const)
      return;

#if 0
   IRDirty *di;
   IRAtom *tguard;

   /* Don't do this here, piggyback on complainIfUndefined instead */
   tguard = expr2tag_trace(fce, trace_env, guard);
   /* This needs to be dirty so it doesn't get optimized away. */
   di = unsafeIRDirty_0_N(0, "FC_(helper_BRANCH_TAG)", &FC_(helper_BRANCH_TAG),
			  mkIRExprVec_1(tguard));
   di->nFxState = 0;
   stmt(fce->sb, IRStmt_Dirty(di));
#endif
}

static void
do_shadow_Dirty_trace(FCEnv *fce, TraceEnv *trace_env, IRDirty *d) {
   IRExpr **exprvec = d->args;
   IRExpr *tresult;
   IRTemp dst;

   do_shadow_cond_trace(fce, trace_env, d->guard);
   if (d->tmp == IRTemp_INVALID)
      return;

   if (!exprvec[0]) {
      tresult = IRExpr_Const(IRConst_U64(0));
   } else if (!exprvec[1]) {
      tresult = expr2tag_Unop_trace(fce, trace_env, exprvec[0]);
   } else if (!exprvec[2]) {
      tresult = expr2tag_Binop_trace(fce, trace_env, 0, exprvec[0],
				     exprvec[1]);
   } else if (!exprvec[3]) {
      tresult = expr2tag_Triop_trace(fce, trace_env, 0, exprvec[0],
				     exprvec[1], exprvec[2]);
   } else if (!exprvec[4]) {
     tresult = expr2tag_Qop_trace(fce, trace_env, 0, exprvec[0], exprvec[1],
				   exprvec[2], exprvec[3]);
   } else if (!exprvec[5]) {
      tresult = expr2tag_5op_trace(fce, trace_env, 0, exprvec[0], exprvec[1],
				   exprvec[2], exprvec[3], exprvec[4]);
   } else {
      VG_(tool_panic)("flowcheck trace: too many dirty call args");
   }

   dst = findShadowTmp_trace(fce, trace_env, d->tmp);
   assign(fce->sb, dst, tresult);
}

void
do_shadow_jump_trace(FCEnv *fce, TraceEnv *trace_env, IRJumpKind jk,
		     IRExpr *target, IRExpr *guard) {
   IRDirty *di;
   if (jk == Ijk_Call) {
      di = unsafeIRDirty_0_N(0, "FC_(helper_WATCH_CALL)",
			     &FC_(helper_WATCH_CALL), mkIRExprVec_1(target));
      di->guard = guard;
      stmt(fce->sb, IRStmt_Dirty(di));
   } else if (jk == Ijk_Ret) {
      di = unsafeIRDirty_0_N(0, "FC_(helper_WATCH_RET)",
			     &FC_(helper_WATCH_RET), mkIRExprVec_0());
      di->guard = guard;
      stmt(fce->sb, IRStmt_Dirty(di));
   }
}

void trace_translate_stmt(FCEnv *fce, TraceEnv *trace_env,
			  IRStmt *st) {
   ////////// add by xue
   IRDirty *di;
   di = unsafeIRDirty_0_N(0, "helper_split_stack)",
			     &helper_split_stack, mkIRExprVec_0());
      stmt(fce->sb, IRStmt_Dirty(di));
   //////////
   switch (st->tag) {
   case Ist_WrTmp:
      {
	 IRExpr *data = st->Ist.WrTmp.data;
	 IRTemp result = st->Ist.WrTmp.tmp;
	 IRTemp tresult = findShadowTmp_trace(fce, trace_env, result);
	 IRTemp vresult = findShadowTmpV(fce, result);
	 IRAtom *tdata = assignNewT(fce, Ity_I64,
				    expr2tag_trace(fce, trace_env, data));
	 IRAtom *pop = make_popcount(fce, trace_env, IRExpr_RdTmp(vresult));
	 assign(fce->sb, tresult,
		mkIRExprCCall(Ity_I64, 0, "FC_(helperc_RESULT_TAG)",
			      &FC_(helperc_RESULT_TAG),
			      mkIRExprVec_3(tdata, pop,
					    mkU64(trace_env->location++))));
      }
      break;

   case Ist_Put:
      do_shadow_Put_trace(fce, trace_env, st->Ist.Put.offset,
			  st->Ist.Put.data);
      break;

   case Ist_PutI:
      do_shadow_PutI_trace(fce, trace_env, st->Ist.PutI.details->descr, st->Ist.PutI.details->ix,
			   st->Ist.PutI.details->bias, st->Ist.PutI.details->data);
      break;

   case Ist_Store:
      do_shadow_Store_trace(fce, trace_env, st->Ist.Store.addr,
			    st->Ist.Store.data);
      break;

   case Ist_Exit:
      do_shadow_cond_trace(fce, trace_env, st->Ist.Exit.guard);
      do_shadow_jump_trace(fce, trace_env, st->Ist.Exit.jk,
			   IRExpr_Const(st->Ist.Exit.dst),
			   st->Ist.Exit.guard);
      break;

   case Ist_Dirty:
      do_shadow_Dirty_trace(fce, trace_env, st->Ist.Dirty.details);
      break;
   ///////////nvd add by liusen
 	case Ist_CAS:
 	{
 	// trace load
 		IRType tyy = typeOfIRExpr(fce->sb->tyenv, st->Ist.CAS.details->addr);
 		IRAtom *tdata = expr2tag_Load_trace(fce, trace_env, tyy, st->Ist.CAS.details->addr);
 
 	 IRTemp tresult = findShadowTmp_trace(fce, trace_env, st->Ist.CAS.details->oldLo);
 	 IRTemp vresult = findShadowTmpV(fce, st->Ist.CAS.details->oldLo);
 
 	IRAtom *pop = make_popcount(fce, trace_env, IRExpr_RdTmp(vresult));
 	 assign(fce->sb, tresult,
 		mkIRExprCCall(Ity_I64, 0, "FC_(helperc_RESULT_TAG)",
 			      &FC_(helperc_RESULT_TAG),
 			      mkIRExprVec_3(tdata, pop,
 					    mkU64(trace_env->location++))));
	// trace store 
 		do_shadow_Store_trace(fce, trace_env, st->Ist.CAS.details->addr, st->Ist.CAS.details->dataLo);
 	}
 		break;
   
   case Ist_IMark:
   case Ist_NoOp:
   case Ist_AbiHint:
   case Ist_MBE:
      break;

   default:
      VG_(printf)("\n");
      ppIRStmt(st);
      VG_(printf)("\n");
      VG_(tool_panic)("flowcheck trace: unhandled IRStmt");
   }
}

