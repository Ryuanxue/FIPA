/*--------------------------------------------------------------------*/
/*--- VEX IR transformations related to constructing a data-flow   ---*/
/*--- graph of program execution.                                  ---*/
/*---                                            trace_translate.h ---*/
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

#ifndef TRACE_TRANSLATE_H
#define TRACE_TRANSLATE_H

/* "TraceEnv" is a typedef to "struct _TraceEnv", but we need to
   declare that in fc_translate.h, not here, because of the two-way
   dependence between these two headers. */
struct _TraceEnv {
   /* We used to have our own tmpMap, but now that the
      Memcheck/fc_translate one supports different shadow types, we
      just piggyback on that. */

   /* Static location identifier: guest instruction (4 bytes) + stmt
      (1 bytes) + expr (1 byte) */
   Long location;
};

IRExpr *expr2tag_trace(FCEnv *fce, TraceEnv *trace_env, IRExpr *e);

void trace_translate_stmt(FCEnv *fce, TraceEnv *trace_env, IRStmt *st);

void do_shadow_jump_trace(FCEnv *fce, TraceEnv *trace_env, IRJumpKind jk,
			  IRExpr *target, IRExpr *guard);

#endif /* TRACE_TRANSLATE_H */
