/*--------------------------------------------------------------------*/
/*--- Instrument IR to perform memory checking operations.         ---*/
/*---                                               fc_translate.h ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of FlowCheck, a heavyweight Valgrind tool for
   detecting leakage of secret information.

   Based on MemCheck, a heavyweight Valgrind tool for detecting memory
   errors.

   Copyright (C) 2000-2007 Julian Seward
      jseward@acm.org

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

typedef struct _TraceEnv TraceEnv;

/* Carries around state during flowcheck instrumentation. */
typedef
   struct _FCEnv {
      /* MODIFIED: the superblock being constructed.  IRStmts are
         added. */
      IRSB* sb;
      Bool  trace;

      /* MODIFIED: a table [0 .. #temps_in_sb-1] which gives the
         current kind and possibly shadow temps for each temp in the
         IRSB being constructed.  Note that it does not contain the
         type of each tmp.  If you want to know the type, look at the
         relevant entry in sb->tyenv.  It follows that at all times
         during the instrumentation process, the valid indices for
         tmpMap and sb->tyenv are identical, being 0 .. N-1 where N is
         total number of Orig, V- and B- temps allocated so far.

         The reason for this strange split (types in one place, all
         other info in another) is that we need the types to be
         attached to sb so as to make it possible to do
         "typeOfIRExpr(fce->bb->tyenv, ...)" at various places in the
         instrumentation process. */
      XArray* /* of TempMapEnt */ tmpMap;

      IRTemp enclosureModeTmp8;
      IRTemp enclosureModeTmp32;

      IRTemp rollbackModeTmp1;
      IRTemp rollbackModeTmp32;

      /* MODIFIED: indicates whether "bogus" literals have so far been
         found.  Starts off False, and may change to True. */
      Bool bogusLiterals;

      /* READONLY: indicates whether we should use expensive
         interpretations of integer adds, since unfortunately LLVM
         uses them to do ORs in some circumstances.  Defaulted to True
         on MacOS and False everywhere else. */
      Bool useLLVMworkarounds;

      /* READONLY: the guest layout.  This indicates which parts of
         the guest state should be regarded as 'always defined'. */
      const VexGuestLayout* layout;

      /* READONLY: the host word type.  Needed for constructing
         arguments of type 'HWord' to be passed to helper functions.
         Ity_I32 or Ity_I64 only. */
      IRType hWordTy;
      TraceEnv *trace_env;

   }
   FCEnv;


typedef  IRExpr  IRAtom;

IRExpr* expr2vbits ( struct _FCEnv* fce, IRExpr* e );
Bool isShadowAtom ( FCEnv* fce, IRAtom* a1 );
IRTemp findShadowTmpV ( FCEnv* fce, IRTemp orig );
IRTemp findShadowTmpT ( FCEnv* fce, IRTemp orig );
IRTemp newTempT( FCEnv* fce );
IRAtom* assignNew ( HChar cat, FCEnv* fce, IRType ty, IRExpr* e );

/* The offset of the shadow area, within the guest state. */
Int FC_(shadow_guest_offset);

/* Offset of the tags shadows within the shadow guest state. */
Int FC_(tags_guest_offset);
