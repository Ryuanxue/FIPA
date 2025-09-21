/*--------------------------------------------------------------------*/
/*--- Collapse a large flow graph into a compressed one that       ---*/
/*--- allows the same flows, by adding together edges frome the    ---*/
/*--- same program location and merging nodes with a union-find    ---*/
/*--- data structure.                                              ---*/
/*---                                                       fold.c ---*/
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
#include "pub_tool_aspacemgr.h"
#include "pub_tool_hashtable.h"     // For fc_include.h
#include "pub_tool_debuginfo.h"
#include "pub_tool_machine.h"
#include "pub_tool_mallocfree.h"
#include "pub_tool_oset.h"
#include "pub_tool_poolalloc.h"
#include "pub_tool_threadstate.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_vki.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_libcfile.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_libcproc.h"
#include "pub_tool_libcassert.h"
#include "fc_include.h"
#include "fold.h"
#include "trace_runtime.h"

struct uf_object {
   struct uf_object *parent;
   UShort rank;
   Char free;
   Char mark;
};

#define NUM_TAGS 200000000
//#define NUM_TAGS 10000000 //commentd by shen
/* #define NUM_TAGS 150000 */

static int tags_left = NUM_TAGS - 2;

static struct uf_object tags[NUM_TAGS];

static struct uf_object *uf_find(struct uf_object *o) {
   struct uf_object *root, *next;

   for (root = o; root->parent != root; root = root->parent)
      ;

   for (next = o->parent; next != root; o = next, next = o->parent)
      o->parent = root;

   return root;
}

static void uf_make_set(struct uf_object *o) {
   o->parent = o;
   o->rank = 0;
}

static UWord uf_set_num(struct uf_object *o) {
   return o - tags;
}

static struct uf_object *uf_union(struct uf_object *o1, struct uf_object *o2) {
   struct uf_object *rep1 = uf_find(o1);
   struct uf_object *rep2 = uf_find(o2);

   if (rep1 == rep2)
      return rep1;

   /*VG_(printf)("Merging %d with %d\n", uf_set_num(o1), uf_set_num(o2));*/
   if (rep1->rank < rep2->rank) {
      rep1->parent = rep2;
      return rep2;
   } else {
      /* rep1->rank >= rep2->rank */
      rep2->parent = rep1;
      if (rep1->rank == rep2->rank)
	 rep1->rank++;
      return rep1;
   }
}

void fold_init(void) {
   UWord i;
   for (i = FIRST_TAG; i < NUM_TAGS; i++) {
      tags[i].free = 1;
   }
}

void fold_new_tag(ULong tag) {
   tl_assert(tag < NUM_TAGS);
   uf_make_set(&tags[tag]);
}

void mark_tag_in_use(UWord tag) {
   tl_assert(tag != 0);
   tl_assert(tag != SOURCE_TAG);
   tl_assert(tag != SINK_TAG);
   tl_assert(tag != NO_TAG);
   tl_assert(tag < NUM_TAGS);
   tags[tag].mark = 1;
}

static int next_tag = FIRST_TAG;

/* XXX This is core-only, but I get an assertion failure if I don't
   check it in the for-all-threads loop below. */
Bool VG_(is_valid_tid)(ThreadId tid);

static void gc_tags(void) {
   Word i;
   Word new_free = 0;
   struct edge_info *edge;
   for (i = 0; i < NUM_TAGS; i++) {
      tags[i].mark = 0;
   }
   mark_memory_tags();
   VG_(OSetGen_ResetIter)(edges);
   while ((edge = VG_(OSetGen_Next)(edges))) {
      struct uf_object *src_rep = uf_find(&tags[edge->src_tag]);
      struct uf_object *dest_rep = uf_find(&tags[edge->dest_tag]);
      src_rep->mark = 1;
      edge->src_tag = uf_set_num(src_rep);
      dest_rep->mark = 1;
      edge->dest_tag = uf_set_num(dest_rep);
   }
   {
      ThreadId tid;
#define GUEST_SIZE 320 /* XXX figure out how to not hard code this*/
      ULong guest_tags[GUEST_SIZE];
      for (tid = 1; tid < VG_N_THREADS; tid++) {
	 int reg;
	 if (!VG_(is_valid_tid)(tid))
	   continue;
	 VG_(get_shadow_regs_area)(tid, (UChar*)&guest_tags, 0, GUEST_SIZE, 8 * GUEST_SIZE);
	 for (reg = 0; reg < GUEST_SIZE; reg++) {
	    if (guest_tags[reg] != 0) {
	       /*VG_(printf)("Register at offset %d has tag %lld\n",
		 reg, guest_tags[reg]);*/
	       mark_tag_in_use(guest_tags[reg]);
	    }
	 }
      }
   }
   for (i = FIRST_TAG; i < NUM_TAGS; i++) {
      /*VG_(printf)("Tag %d: %s\n", i, tags[i].mark ? "in use" : "free");*/
      if (tags[i].mark) {
	 tags[i].free = 0;
      } else {
	 tags[i].free = 1;
	 new_free++;
      }
   }
   VG_(printf)("%ld tags freed by garbage collection\n", new_free);
   next_tag = FIRST_TAG;
   tags_left = new_free;
   tl_assert(tags_left);
}

void check_enough_tags(Word needed) {
   if (!FC_(clo_folding_level))
      return;
   /* 100000 is a fudge factor to save from figuring out all the places
      "needed" needs to be bigger. */
   if (needed + 100000 > tags_left) {
      gc_tags();
      if (needed > tags_left) {
	 VG_(printf)("Out of tag space in request for %ld tags\n", needed);
	 tl_assert(needed <= tags_left);
      }
   }
}

UWord get_free_tag(void) {
   UWord tag;
   while (next_tag < NUM_TAGS && !tags[next_tag].free)
      next_tag++;
   tl_assert(next_tag < NUM_TAGS);
   tags_left--;
   tag = next_tag++;
   uf_make_set(&tags[tag]);
   return tag;
}

void fold_new_edge(ULong src, ULong dest, int capacity,
		   struct edge_info *edge) {
   if (edge->src_tag == NO_TAG) {
      edge->src_tag = src;
   } else {
      uf_union(&tags[src], &tags[edge->src_tag]);
   }

   if (edge->dest_tag == NO_TAG) {
      edge->dest_tag = dest;
   } else {
      uf_union(&tags[dest], &tags[edge->dest_tag]);
   }

   {
      ULong old_total = edge->total_capacity;
      edge->total_capacity += capacity;
      tl_assert(edge->total_capacity >= old_total);
   }
}

static void output_folded_edges_to_fd(int fd, Bool long_form) {
   struct uf_object *source_rep = uf_find(&tags[SOURCE_TAG]);
   struct uf_object *sink_rep = uf_find(&tags[SINK_TAG]);
   struct edge_info *edge;

   VG_(OSetGen_ResetIter)(edges);
   while ((edge = VG_(OSetGen_Next)(edges))) {
      struct uf_object *src_rep = uf_find(&tags[edge->src_tag]);
      struct uf_object *dest_rep = uf_find(&tags[edge->dest_tag]);
      Long src_num, dest_num;
      if (src_rep == source_rep)
	 src_num = 0;
      else
	 src_num = uf_set_num(src_rep);
      if (dest_rep == sink_rep)
	 dest_num = -1;
      else
	 dest_num = uf_set_num(dest_rep);
      if (fd == 2) {
	 /* NB: Valgrind's "printf" goes to stderr */
	 if (long_form) {
	    VG_(printf)("%lld %lld %lld %llx:%llx %s (%s)\n",
			src_num, dest_num, edge->total_capacity,
			edge->location, edge->context,
			edge->eip_descr, edge->type_descr);
	 } else {
	    VG_(printf)("%lld %lld %lld\n", src_num, dest_num,
			edge->total_capacity);
	 }
      } else {
	 if (long_form) {
	    fdprintf(fd, "%lld %lld %lld %llx:%llx %s (%s)\n",
		     src_num, dest_num, edge->total_capacity,
		     edge->location, edge->context,
		     edge->eip_descr, edge->type_descr);
	 } else {
	    fdprintf(fd, "%lld %lld %lld\n",
		     src_num, dest_num, edge->total_capacity);
	 }
      }

   }
}

void output_folded_edges_stderr(void) {
   output_folded_edges_to_fd(2, True);
}

void run_max_flow_program(const char *fname) {
   int len;
   char *flow_fname;
   char cmd_buf[4096];
   len = VG_(strlen)(fname);
   flow_fname = VG_(malloc)("fc.rmfp", len + 6);
   VG_(strcpy)(flow_fname, fname);
   if (fname[len-2] == '.' && fname[len-1] == 'g') {
      VG_(strcpy)(flow_fname + len - 2, ".flow");
   } else {
      VG_(strcpy)(flow_fname + len, ".flow");
   }
   /* XXX avoid the shell and fixed-size buffer here */
   VG_(snprintf)(cmd_buf, sizeof cmd_buf, "%s <%s >%s",
		 FC_(clo_max_flow_program), fname, flow_fname);
   VG_(system)(cmd_buf);
   VG_(free)(flow_fname);
}

void output_folded_edges_filename(const char *fname) {
   SysRes sr;
   int fd;
   sr = VG_(open)(fname, VKI_O_WRONLY|VKI_O_CREAT|VKI_O_TRUNC,
		  0666);
   tl_assert(!sr_isError(sr));
   fd = (int) sr_Res(sr);
   output_folded_edges_to_fd(fd, True);
   VG_(close)(fd);
   if (FC_(clo_max_flow_program)) {
      run_max_flow_program(fname);
   }
}

static UInt last_millis = 0;

void run_incremental_max_flow(void) {
   int fd;
   UInt millis;
   SysRes sr;
   millis = VG_(read_millisecond_timer)();
   if (millis - last_millis > 1000) {
      char cmd_buf[4096];
      sr = VG_(open)("flowcheck.g", VKI_O_WRONLY|VKI_O_CREAT|VKI_O_TRUNC,
		     0666);
      tl_assert(!sr_isError(sr));
      fd = (int)sr_Res(sr);
      output_folded_edges_to_fd(fd, False);
      VG_(close)(fd);
      if (FC_(clo_max_flow_program)) {
	 VG_(snprintf)(cmd_buf, sizeof cmd_buf, "%s <%s >%s",
		       FC_(clo_max_flow_program), "flowcheck.g",
		       "flowcheck.flow");
	 VG_(system)(cmd_buf);
      }
      last_millis = millis;
   }
}
