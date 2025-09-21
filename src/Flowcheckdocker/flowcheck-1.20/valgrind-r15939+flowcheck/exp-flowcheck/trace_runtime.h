/*--------------------------------------------------------------------*/
/*--- Runtime operations related to constructing a data-flow       ---*/
/*--- graph of program execution.                                  ---*/
/*---                                              trace_runtime.h ---*/
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

#ifndef TRACE_RUNTIME_H
#define TRACE_RUNTIME_H

#include "pub_tool_oset.h"

struct edge_info {
   ULong location;
   ULong context;
   UWord eip;
   char *eip_descr;
   const char *type_descr;
   UWord src_tag;
   UWord dest_tag;
   ULong total_capacity;
};

#define SOURCE_TAG  0
#define SINK_TAG    1
#define FIRST_TAG   2
#define NO_TAG     -1

OSet *edges;

void init_tag_memory(void);
void mark_memory_tags(void);

ULong get_tag(Addr addr, ULong location);
void set_tag(Addr addr, ULong tag, ULong location);

void make_freshly_tagged_input(Addr start, SizeT len, ULong location);
void make_freshly_tagged_register(ThreadId tid, OffT offset, int capacity,
				  ULong location);
void count_leaked_tags(Addr start, SizeT len, ULong location);

void trace_enter_enclose(void);
void trace_leak(ULong tag, Int width, ULong location);
void trace_leak_enclosed(ULong tag, Int width, ULong location);
void trace_enclosed_output(Addr addr, ULong location);
void trace_enclosed_output_region(Addr addr, Int size, ULong location);

void output_graph_stats(void);

ULong FC_(helperc_RESULT_TAG)  (ULong, UInt, ULong);
ULong FC_(helperc_UNARY_TAG)   (ULong, UInt, ULong);
ULong FC_(helperc_BINARY_TAGS) (ULong, ULong, UInt, UInt, ULong);
ULong FC_(helperc_TERNARY_TAGS)(ULong, ULong, ULong, UInt, UInt, UInt, ULong);
ULong FC_(helperc_FOURARY_TAGS)(ULong, ULong, ULong, ULong,
				UInt,  UInt,  UInt,  UInt, ULong);
ULong FC_(helperc_FIVEARY_TAGS)(ULong, ULong, ULong, ULong, ULong,
				UInt,  UInt,  UInt,  UInt,  UInt, ULong);

ULong FC_(helper_LOAD_TAG_1) (Addr addr, ULong addrTag, ULong location);
ULong FC_(helper_LOAD_TAG_2) (Addr addr, ULong addrTag, ULong location);
ULong FC_(helper_LOAD_TAG_4) (Addr addr, ULong addrTag, ULong location);
ULong FC_(helper_LOAD_TAG_8) (Addr addr, ULong addrTag, ULong location);
ULong FC_(helper_LOAD_TAG_16)(Addr addr, ULong addrTag, ULong location);

void FC_(helper_STORE_TAG_1) (Addr addr, ULong addrTag, ULong dataTag,
			      ULong location, Word in_rollback);
void FC_(helper_STORE_TAG_2) (Addr addr, ULong addrTag, ULong dataTag,
			      ULong location, Word in_rollback);
void FC_(helper_STORE_TAG_4) (Addr addr, ULong addrTag, ULong dataTag,
			      ULong location, Word in_rollback);
void FC_(helper_STORE_TAG_8) (Addr addr, ULong addrTag, ULong dataTag,
			      ULong location, Word in_rollback);
void FC_(helper_STORE_TAG_16)(Addr addr, ULong addrTag, ULong dataTag,
			      ULong location, Word in_rollback);

void helperc_count_senaddr(Addr data,int len,int type);//add by xue
void helperc_store_addr_val(Addr data,Addr val,int size);//add by xue
void init_addr_val_page_table(void) ;//add by xue

void FC_(helper_BRANCH_TAG)(ULong tag);

void FC_(helper_WATCH_CALL)(UWord addr);
void FC_(helper_WATCH_RET)(void);
void shadow_stack_iterate(void);
void helper_split_stack(void) ;

#define fdprintf(fd, format, args...) ({			\
         const int _buf_size = 4096;				\
	 char _buf[_buf_size];					\
	 VG_(snprintf)(_buf, _buf_size, format, ##args);	\
	 VG_(write)(fd, (void*)_buf, VG_(strlen)(_buf));	\
      })

#endif /* TRACE_RUNTIME_H */
