/*--------------------------------------------------------------------*/
/*--- Count how many of the bits in a value are set to 1.          ---*/
/*---                                                   popcount.c ---*/
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
#include "pub_tool_tooliface.h"     // For fc_include.h
#include "pub_tool_poolalloc.h"     // For fc_include.h
#include "fc_include.h"
#include "popcount.h"

static const char popcount_table[] = {
    0,1,1,2, 1,2,2,3, 1,2,2,3, 2,3,3,4, 1,2,2,3, 2,3,3,4, 2,3,3,4, 3,4,4,5,
    1,2,2,3, 2,3,3,4, 2,3,3,4, 3,4,4,5, 2,3,3,4, 3,4,4,5, 3,4,4,5, 4,5,5,6,
    1,2,2,3, 2,3,3,4, 2,3,3,4, 3,4,4,5, 2,3,3,4, 3,4,4,5, 3,4,4,5, 4,5,5,6,
    2,3,3,4, 3,4,4,5, 3,4,4,5, 4,5,5,6, 3,4,4,5, 4,5,5,6, 4,5,5,6, 5,6,6,7,

    1,2,2,3, 2,3,3,4, 2,3,3,4, 3,4,4,5, 2,3,3,4, 3,4,4,5, 3,4,4,5, 4,5,5,6,
    2,3,3,4, 3,4,4,5, 3,4,4,5, 4,5,5,6, 3,4,4,5, 4,5,5,6, 4,5,5,6, 5,6,6,7,
    2,3,3,4, 3,4,4,5, 3,4,4,5, 4,5,5,6, 3,4,4,5, 4,5,5,6, 4,5,5,6, 5,6,6,7,
    3,4,4,5, 4,5,5,6, 4,5,5,6, 5,6,6,7, 4,5,5,6, 5,6,6,7, 5,6,6,7, 6,7,7,8
};

int popcount8(UChar val) {
    return popcount_table[val];
}

int popcount16(UShort val) {
    return popcount_table[ val        & 0xff] +
	   popcount_table[(val >>  8) & 0xff];
}

int popcount32(UInt val) {
    return popcount_table[ val        & 0xff] +
	   popcount_table[(val >>  8) & 0xff] +
	   popcount_table[(val >> 16) & 0xff] +
	   popcount_table[(val >> 24) & 0xff];
}

int popcount64(ULong val) {
    return popcount_table[ val        & 0xff] +
	   popcount_table[(val >>  8) & 0xff] +
	   popcount_table[(val >> 16) & 0xff] +
	   popcount_table[(val >> 24) & 0xff] +
	   popcount_table[(val >> 32) & 0xff] +
	   popcount_table[(val >> 40) & 0xff] +
	   popcount_table[(val >> 48) & 0xff] +
	   popcount_table[(val >> 56) & 0xff];
}

Int FC_(helperc_POPCOUNT8) (UInt  val) {
    return popcount8(val & 0xff);
}

Int FC_(helperc_POPCOUNT16)(UInt  val) {
    return popcount16(val & 0xffff);
}

Int FC_(helperc_POPCOUNT32)(UInt  val) {
    return popcount32(val);
}

Int FC_(helperc_POPCOUNT64)(ULong val) {
    return popcount64(val);
}

Int FC_(helperc_POPCOUNT128)(ULong val1, ULong val2) { //add by shen
    return popcount64(val1) + popcount64(val2);
}
