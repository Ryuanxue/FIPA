
/*--------------------------------------------------------------------*/
/*--- A header file for all parts of the FlowCheck tool.           ---*/
/*---                                                 fc_include.h ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of FlowCheck, a heavyweight Valgrind tool for
   detecting leakage of secret information.

   Based on MemCheck, a heavyweight Valgrind tool for detecting memory
   errors.

   Copyright (C) 2000-2015 Julian Seward 
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

#ifndef __FC_INCLUDE_H
#define __FC_INCLUDE_H

#define FC_(str)    VGAPPEND(vgFlowCheck_,str)


/* This is a private header file for use only within the
   exp-flowcheck/ directory. */

/*------------------------------------------------------------*/
/*--- Tracking the heap                                    ---*/
/*------------------------------------------------------------*/

/* By default, we want at least a 16B redzone on client heap blocks
   for Memcheck.
   The default can be modified by --redzone-size. */
#define FC_MALLOC_DEFAULT_REDZONE_SZB    16
// effective redzone, as (possibly) modified by --redzone-size:
extern SizeT FC_(Malloc_Redzone_SzB);


/* For malloc()/new/new[] vs. free()/delete/delete[] mismatch checking. */
typedef
   enum {
      FC_AllocMalloc = 0,
      FC_AllocNew    = 1,
      FC_AllocNewVec = 2,
      FC_AllocCustom = 3
   }
   FC_AllocKind;
   
/* This describes a heap block. Nb: first two fields must match core's
 * VgHashNode. */
typedef
   struct _FC_Chunk {
      struct _FC_Chunk* next;
      Addr         data;            // Address of the actual block.
      SizeT        szB : (sizeof(SizeT)*8)-2; // Size requested; 30 or 62 bits.
      FC_AllocKind allockind : 2;   // Which operation did the allocation.
      ExeContext*  where[0];
      /* Variable-length array. The size depends on FC_(clo_keep_stacktraces).
         This array optionally stores the alloc and/or free stack trace. */
   }
   FC_Chunk;

/* Returns the execontext where the FC_Chunk was allocated/freed.
   Returns VG_(null_ExeContext)() if the execontext has not been recorded (due
   to FC_(clo_keep_stacktraces) and/or because block not yet freed). */
ExeContext* FC_(allocated_at) (FC_Chunk*);
ExeContext* FC_(freed_at) (FC_Chunk*);

/* Records and sets execontext according to FC_(clo_keep_stacktraces) */
void  FC_(set_allocated_at) (ThreadId, FC_Chunk*);
void  FC_(set_freed_at) (ThreadId, FC_Chunk*);

/* number of pointers needed according to FC_(clo_keep_stacktraces). */
UInt FC_(n_where_pointers) (void);

/* Memory pool.  Nb: first two fields must match core's VgHashNode. */
typedef
   struct _FC_Mempool {
      struct _FC_Mempool* next;
      Addr          pool;           // pool identifier
      SizeT         rzB;            // pool red-zone size
      Bool          is_zeroed;      // allocations from this pool are zeroed
      VgHashTable  *chunks;         // chunks associated with this pool
   }
   FC_Mempool;


void* FC_(new_block)  ( ThreadId tid,
                        Addr p, SizeT size, SizeT align,
                        Bool is_zeroed, FC_AllocKind kind,
                        VgHashTable *table);
void FC_(handle_free) ( ThreadId tid,
                        Addr p, UInt rzB, FC_AllocKind kind );

void FC_(create_mempool)  ( Addr pool, UInt rzB, Bool is_zeroed );
void FC_(destroy_mempool) ( Addr pool );
void FC_(mempool_alloc)   ( ThreadId tid, Addr pool,
                            Addr addr, SizeT size );
void FC_(mempool_free)    ( Addr pool, Addr addr );
void FC_(mempool_trim)    ( Addr pool, Addr addr, SizeT size );
void FC_(move_mempool)    ( Addr poolA, Addr poolB );
void FC_(mempool_change)  ( Addr pool, Addr addrA, Addr addrB, SizeT size );
Bool FC_(mempool_exists)  ( Addr pool );

/* Searches for a recently freed block which might bracket Addr a.
   Return the FC_Chunk* for this block or NULL if no bracketting block
   is found. */
FC_Chunk* FC_(get_freed_block_bracketting)( Addr a );

/* For efficient pooled alloc/free of the FC_Chunk. */
extern PoolAlloc* FC_(chunk_poolalloc);

/* For tracking malloc'd blocks.  Nb: it's quite important that it's a
   VgHashTable, because VgHashTable allows duplicate keys without complaint.
   This can occur if a user marks a malloc() block as also a custom block with
   MALLOCLIKE_BLOCK. */
extern VgHashTable *FC_(malloc_list);

/* For tracking memory pools. */
extern VgHashTable *FC_(mempool_list);

/* Shadow memory functions */
Bool FC_(check_mem_is_noaccess)( Addr a, SizeT len, Addr* bad_addr );
void FC_(check_mem_is_defined_secret) ( CorePart part, ThreadId tid, const HChar* s,
                                 Addr base, SizeT size );
void FC_(make_mem_noaccess)        ( Addr a, SizeT len );
void FC_(make_mem_undefined_w_otag)( Addr a, SizeT len, UInt otag );
void FC_(make_mem_undefined) 	   ( Addr a, SizeT len );
void FC_(make_mem_defined)         ( Addr a, SizeT len );
void FC_(make_mem_accessible)(Addr a, SizeT len);
void FC_(copy_address_range_state) ( Addr src, Addr dst, SizeT len );


void FC_(print_malloc_stats) ( void );
/* nr of free operations done */
SizeT FC_(get_cmalloc_n_frees) ( void );

void* FC_(malloc)               ( ThreadId tid, SizeT n );
void* FC_(__builtin_new)        ( ThreadId tid, SizeT n );
void* FC_(__builtin_vec_new)    ( ThreadId tid, SizeT n );
void* FC_(memalign)             ( ThreadId tid, SizeT align, SizeT n );
void* FC_(calloc)               ( ThreadId tid, SizeT nmemb, SizeT size1 );
void  FC_(free)                 ( ThreadId tid, void* p );
void  FC_(__builtin_delete)     ( ThreadId tid, void* p );
void  FC_(__builtin_vec_delete) ( ThreadId tid, void* p );
void* FC_(realloc)              ( ThreadId tid, void* p, SizeT new_size );
SizeT FC_(malloc_usable_size)   ( ThreadId tid, void* p );

void FC_(handle_resizeInPlace)(ThreadId tid, Addr p,
                               SizeT oldSizeB, SizeT newSizeB, SizeT rzB);


/*------------------------------------------------------------*/
/*--- Origin tracking translate-time support               ---*/
/*------------------------------------------------------------*/

/* See detailed comments in fc_machine.c. */
Int FC_(get_otrack_shadow_offset) ( Int offset, Int szB );
IRType FC_(get_otrack_reg_array_equiv_int_type) ( IRRegArray* arr );

/* Constants which are used as the lowest 2 bits in origin tags.
   
   An origin tag comprises an upper 30-bit ECU field and a lower 2-bit
   'kind' field.  The ECU field is a number given out by m_execontext
   and has a 1-1 mapping with ExeContext*s.  An ECU can be used
   directly as an origin tag (otag), but in fact we want to put
   additional information 'kind' field to indicate roughly where the
   tag came from.  This helps print more understandable error messages
   for the user -- it has no other purpose.

   Hence the following 2-bit constants are needed for 'kind' field. 

   To summarise:

   * Both ECUs and origin tags are represented as 32-bit words

   * m_execontext and the core-tool interface deal purely in ECUs.
     They have no knowledge of origin tags - that is a purely
     Memcheck-internal matter.

   * all valid ECUs have the lowest 2 bits zero and at least
     one of the upper 30 bits nonzero (see VG_(is_plausible_ECU))

   * to convert from an ECU to an otag, OR in one of the FC_OKIND_
     constants below

   * to convert an otag back to an ECU, AND it with ~3
*/

#define FC_OKIND_UNKNOWN  0  /* unknown origin */
#define FC_OKIND_HEAP     1  /* this is a heap origin */
#define FC_OKIND_STACK    2  /* this is a stack origin */
#define FC_OKIND_USER     3  /* arises from user-supplied client req */


/*------------------------------------------------------------*/
/*--- Profiling of memory events                           ---*/
/*------------------------------------------------------------*/

/* Define to collect detailed performance info. */
/* #define FC_PROFILE_MEMORY */
#ifdef FC_PROFILE_MEMORY

/* Order of enumerators does not matter. But MCPE_LAST has to be the 
   last entry in the list as it is used as an array bound. */
enum {
   MCPE_LOADV8,
   MCPE_LOADV8_SLOW1,
   MCPE_LOADV8_SLOW2,
   MCPE_LOADV16,
   MCPE_LOADV16_SLOW1,
   MCPE_LOADV16_SLOW2,
   MCPE_LOADV32,
   MCPE_LOADV32_SLOW1,
   MCPE_LOADV32_SLOW2,
   MCPE_LOADV64,
   MCPE_LOADV64_SLOW1,
   MCPE_LOADV64_SLOW2,
   MCPE_LOADV_128_OR_256,
   MCPE_LOADV_128_OR_256_SLOW_LOOP,
   MCPE_LOADV_128_OR_256_SLOW1,
   MCPE_LOADV_128_OR_256_SLOW2,
   MCPE_LOADVN_SLOW,
   MCPE_LOADVN_SLOW_LOOP,
   MCPE_STOREV8,
   MCPE_STOREV8_SLOW1,
   MCPE_STOREV8_SLOW2,
   MCPE_STOREV8_SLOW3,
   MCPE_STOREV8_SLOW4,
   MCPE_STOREV16,
   MCPE_STOREV16_SLOW1,
   MCPE_STOREV16_SLOW2,
   MCPE_STOREV16_SLOW3,
   MCPE_STOREV16_SLOW4,
   MCPE_STOREV32,
   MCPE_STOREV32_SLOW1,
   MCPE_STOREV32_SLOW2,
   MCPE_STOREV32_SLOW3,
   MCPE_STOREV32_SLOW4,
   MCPE_STOREV64,
   MCPE_STOREV64_SLOW1,
   MCPE_STOREV64_SLOW2,
   MCPE_STOREV64_SLOW3,
   MCPE_STOREV64_SLOW4,
   MCPE_STOREVN_SLOW,
   MCPE_STOREVN_SLOW_LOOP,
   MCPE_MAKE_ALIGNED_WORD32_UNDEFINED,
   MCPE_MAKE_ALIGNED_WORD32_UNDEFINED_SLOW,
   MCPE_MAKE_ALIGNED_WORD64_UNDEFINED,
   MCPE_MAKE_ALIGNED_WORD64_UNDEFINED_SLOW,
   MCPE_MAKE_ALIGNED_WORD64_READABLE,
   MCPE_MAKE_ALIGNED_WORD32_NOACCESS,
   MCPE_MAKE_ALIGNED_WORD32_NOACCESS_SLOW,
   MCPE_MAKE_ALIGNED_WORD64_NOACCESS,
   MCPE_MAKE_ALIGNED_WORD64_NOACCESS_SLOW,
   MCPE_MAKE_MEM_NOACCESS,
   MCPE_MAKE_MEM_UNDEFINED,
   MCPE_MAKE_MEM_UNDEFINED_W_OTAG,
   MCPE_MAKE_MEM_DEFINED,
   MCPE_CHEAP_SANITY_CHECK,
   MCPE_EXPENSIVE_SANITY_CHECK,
   MCPE_COPY_ADDRESS_RANGE_STATE,
   MCPE_COPY_ADDRESS_RANGE_STATE_LOOP1,
   MCPE_COPY_ADDRESS_RANGE_STATE_LOOP2,
   MCPE_CHECK_MEM_IS_NOACCESS,
   MCPE_CHECK_MEM_IS_NOACCESS_LOOP,
   MCPE_IS_MEM_ADDRESSABLE,
   MCPE_IS_MEM_ADDRESSABLE_LOOP,
   MCPE_IS_MEM_DEFINED,
   MCPE_IS_MEM_DEFINED_LOOP,
   MCPE_IS_MEM_DEFINED_COMPREHENSIVE,
   MCPE_IS_MEM_DEFINED_COMPREHENSIVE_LOOP,
   MCPE_IS_DEFINED_ASCIIZ,
   MCPE_IS_DEFINED_ASCIIZ_LOOP,
   MCPE_FIND_CHUNK_FOR_OLD,
   MCPE_FIND_CHUNK_FOR_OLD_LOOP,
   MCPE_SET_ADDRESS_RANGE_PERMS,
   MCPE_SET_ADDRESS_RANGE_PERMS_SINGLE_SECMAP,
   MCPE_SET_ADDRESS_RANGE_PERMS_STARTOF_SECMAP,
   MCPE_SET_ADDRESS_RANGE_PERMS_MULTIPLE_SECMAPS,
   MCPE_SET_ADDRESS_RANGE_PERMS_DIST_SM1,
   MCPE_SET_ADDRESS_RANGE_PERMS_DIST_SM2,
   MCPE_SET_ADDRESS_RANGE_PERMS_DIST_SM1_QUICK,
   MCPE_SET_ADDRESS_RANGE_PERMS_DIST_SM2_QUICK,
   MCPE_SET_ADDRESS_RANGE_PERMS_LOOP1A,
   MCPE_SET_ADDRESS_RANGE_PERMS_LOOP1B,
   MCPE_SET_ADDRESS_RANGE_PERMS_LOOP1C,
   MCPE_SET_ADDRESS_RANGE_PERMS_LOOP8A,
   MCPE_SET_ADDRESS_RANGE_PERMS_LOOP8B,
   MCPE_SET_ADDRESS_RANGE_PERMS_LOOP64K,
   MCPE_SET_ADDRESS_RANGE_PERMS_LOOP64K_FREE_DIST_SM,
   MCPE_NEW_MEM_STACK,
   MCPE_NEW_MEM_STACK_4,
   MCPE_NEW_MEM_STACK_8,
   MCPE_NEW_MEM_STACK_12,
   MCPE_NEW_MEM_STACK_16,
   MCPE_NEW_MEM_STACK_32,
   MCPE_NEW_MEM_STACK_112,
   MCPE_NEW_MEM_STACK_128,
   MCPE_NEW_MEM_STACK_144,
   MCPE_NEW_MEM_STACK_160,
   MCPE_DIE_MEM_STACK,
   MCPE_DIE_MEM_STACK_4,
   MCPE_DIE_MEM_STACK_8,
   MCPE_DIE_MEM_STACK_12,
   MCPE_DIE_MEM_STACK_16,
   MCPE_DIE_MEM_STACK_32,
   MCPE_DIE_MEM_STACK_112,
   MCPE_DIE_MEM_STACK_128,
   MCPE_DIE_MEM_STACK_144,
   MCPE_DIE_MEM_STACK_160,
   MCPE_MAKE_STACK_UNINIT_W_O,
   MCPE_MAKE_STACK_UNINIT_NO_O,
   MCPE_MAKE_STACK_UNINIT_128_NO_O,
   MCPE_MAKE_STACK_UNINIT_128_NO_O_ALIGNED_16,
   MCPE_MAKE_STACK_UNINIT_128_NO_O_ALIGNED_8,
   MCPE_MAKE_STACK_UNINIT_128_NO_O_SLOWCASE,
   MCPE_CHECK_READABLE_LOOP,
   /* Do not add enumerators past this line. */
   MCPE_LAST
};

extern ULong FC_(event_ctr)[MCPE_LAST];

#  define PROF_EVENT(ev)                           \
   do { tl_assert((ev) >= 0 && (ev) < MCPE_LAST);  \
      FC_(event_ctr)[ev]++;                        \
   } while (False);

#else

#  define PROF_EVENT(ev)    /* */

#endif   /* FC_PROFILE_MEMORY */


/*------------------------------------------------------------*/
/*--- V and A bits (Victoria & Albert ?)                   ---*/
/*------------------------------------------------------------*/

/* The number of entries in the primary map can be altered.  However
   we hardwire the assumption that each secondary map covers precisely
   64k of address space. */
#define SM_SIZE 65536            /* DO NOT CHANGE */
#define SM_MASK (SM_SIZE-1)      /* DO NOT CHANGE */

#define V_BIT_DEFINED         0
#define V_BIT_UNDEFINED       1

#define V_BITS8_DEFINED       0
#define V_BITS8_UNDEFINED     0xFF

#define V_BITS16_DEFINED      0
#define V_BITS16_UNDEFINED    0xFFFF

#define V_BITS32_DEFINED      0
#define V_BITS32_UNDEFINED    0xFFFFFFFF

#define V_BITS64_DEFINED      0ULL
#define V_BITS64_UNDEFINED    0xFFFFFFFFFFFFFFFFULL


/*------------------------------------------------------------*/
/*--- Leak checking                                        ---*/
/*------------------------------------------------------------*/

typedef 
   enum { 
      // Nb: the order is important -- it dictates the order of loss records
      // of equal sizes.
      Reachable    =0,  // Definitely reachable from root-set.
      Possible     =1,  // Possibly reachable from root-set;  involves at
                        //   least one interior-pointer along the way.
      IndirectLeak =2,  // Leaked, but reachable from another leaked block
                        //   (be it Unreached or IndirectLeak).
      Unreached    =3,  // Not reached, ie. leaked. 
                        //   (At best, only reachable from itself via a cycle.)
  }
  Reachedness;

// Build mask to check or set Reachedness r membership
#define R2S(r) (1 << (r))
// Reachedness r is member of the Set s ?
#define RiS(r,s) ((s) & R2S(r))
// Returns a set containing all Reachedness
UInt FC_(all_Reachedness)(void);

/* For VALGRIND_COUNT_LEAKS client request */
extern SizeT FC_(bytes_leaked);
extern SizeT FC_(bytes_indirect);
extern SizeT FC_(bytes_dubious);
extern SizeT FC_(bytes_reachable);
extern SizeT FC_(bytes_suppressed);

/* For VALGRIND_COUNT_LEAK_BLOCKS client request */
extern SizeT FC_(blocks_leaked);
extern SizeT FC_(blocks_indirect);
extern SizeT FC_(blocks_dubious);
extern SizeT FC_(blocks_reachable);
extern SizeT FC_(blocks_suppressed);

typedef
   enum {
      LC_Off,
      LC_Summary,
      LC_Full,
   }
   LeakCheckMode;

typedef
   enum {
      LCD_Any,       // output all loss records, whatever the delta
      LCD_Increased, // output loss records with an increase in size or blocks
      LCD_Changed,   // output loss records with an increase or 
                     //decrease in size or blocks
   }
   LeakCheckDeltaMode;

/* When a LossRecord is put into an OSet, these elements represent the key. */
typedef
   struct _LossRecordKey {
      Reachedness  state;        // LC_Extra.state value shared by all blocks.
      ExeContext*  allocated_at; // Where they were allocated.
   } 
   LossRecordKey;

/* A loss record, used for generating err msgs.  Multiple leaked blocks can be
 * merged into a single loss record if they have the same state and similar
 * enough allocation points (controlled by --leak-resolution). */
typedef
   struct _LossRecord {
      LossRecordKey key;  // Key, when used in an OSet.
      SizeT szB;          // Sum of all FC_Chunk.szB values.
      SizeT indirect_szB; // Sum of all LC_Extra.indirect_szB values.
      UInt  num_blocks;   // Number of blocks represented by the record.
      SizeT old_szB;          // old_* values are the values found during the 
      SizeT old_indirect_szB; // previous leak search. old_* values are used to
      UInt  old_num_blocks;   // output only the changed/new loss records
   }
   LossRecord;

typedef
   struct _LeakCheckParams {
      LeakCheckMode mode;
      UInt show_leak_kinds;
      UInt errors_for_leak_kinds;
      UInt heuristics;
      LeakCheckDeltaMode deltamode;
      UInt max_loss_records_output; // limit on the nr of loss records output.
      Bool requested_by_monitor_command; // True when requested by gdb/vgdb.
   }
   LeakCheckParams;

void FC_(detect_memory_leaks) ( ThreadId tid, LeakCheckParams * lcp);

// Each time a leak search is done, the leak search generation
// FC_(leak_search_gen) is incremented.
extern UInt FC_(leak_search_gen);

// maintains the lcp.deltamode given in the last call to detect_memory_leaks
extern LeakCheckDeltaMode FC_(detect_memory_leaks_last_delta_mode);

// prints the list of blocks corresponding to the given loss_record_nr slice
// (from/to) (up to maximum max_blocks)
// Returns True if loss_record_nr_from identifies a correct loss record
// from last leak search, returns False otherwise.
// Note that loss_record_nr_to can be bigger than the nr of loss records. All
// loss records after from will then be examined and maybe printed.
// If heuristics != 0, print only the loss records/blocks found via
// one of the heuristics in the set.
Bool FC_(print_block_list) ( UInt loss_record_nr_from, UInt loss_record_nr_to,
                             UInt max_blocks, UInt heuristics);

// Prints the addresses/registers/... at which a pointer to
// the given range [address, address+szB[ is found.
void FC_(who_points_at) ( Addr address, SizeT szB);

// if delta_mode == LCD_Any, prints in buf an empty string
// otherwise prints a delta in the layout  " (+%'lu)" or " (-%'lu)" 
extern HChar * FC_(snprintf_delta) (HChar * buf, Int size, 
                                    SizeT current_val, SizeT old_val, 
                                    LeakCheckDeltaMode delta_mode);


Bool FC_(is_valid_aligned_word)     ( Addr a );
Bool FC_(is_within_valid_secondary) ( Addr a );

// Prints as user msg a description of the given loss record.
void FC_(pp_LossRecord)(UInt n_this_record, UInt n_total_records,
                        LossRecord* l);
                          

/*------------------------------------------------------------*/
/*--- Errors and suppressions                              ---*/
/*------------------------------------------------------------*/

/* Did we show to the user, any errors for which an uninitialised
   value origin could have been collected (but wasn't) ?  If yes,
   then, at the end of the run, print a 1 line message advising that a
   rerun with --track-origins=yes might help. */
extern Bool FC_(any_value_errors);

/* Standard functions for error and suppressions as required by the
   core/tool iface */
Bool FC_(eq_Error)           ( VgRes res, const Error* e1, const Error* e2 );
void FC_(before_pp_Error)    ( const Error* err );
void FC_(pp_Error)           ( const Error* err );
UInt FC_(update_Error_extra) ( const Error* err );

Bool FC_(is_recognised_suppression) ( const HChar* name, Supp* su );

Bool FC_(read_extra_suppression_info) ( Int fd, HChar** buf,
                                        SizeT* nBuf, Int* lineno, Supp *su );

Bool FC_(error_matches_suppression) ( const Error* err, const Supp* su );

SizeT FC_(get_extra_suppression_info) ( const Error* err,
                                        /*OUT*/HChar* buf, Int nBuf );
SizeT FC_(print_extra_suppression_use) ( const Supp* su,
                                         /*OUT*/HChar* buf, Int nBuf );
void FC_(update_extra_suppression_use) ( const Error* err, const Supp* su );

const HChar* FC_(get_error_name) ( const Error* err );

/* Recording of errors */
void FC_(record_address_error) ( ThreadId tid, Addr a, Int szB,
                                 Bool isWrite );
void FC_(record_cond_error)    ( ThreadId tid, UInt otag );
void FC_(record_value_error)  ( ThreadId tid, Int size,
				    UInt val, UInt vbits, ULong tag,
				    ULong location, UWord origin );
void FC_(record_jump_error)    ( ThreadId tid, Addr a );

void FC_(record_free_error)            ( ThreadId tid, Addr a ); 
void FC_(record_illegal_mempool_error) ( ThreadId tid, Addr a );
void FC_(record_freemismatch_error)    ( ThreadId tid, FC_Chunk* mc );

void FC_(record_overlap_error)  ( ThreadId tid, const HChar* function,
                                  Addr src, Addr dst, SizeT szB );
void FC_(record_core_mem_error) ( ThreadId tid, const HChar* msg );
void FC_(record_regparam_error) ( ThreadId tid, const HChar* msg, UInt otag );
void FC_(record_memparam_error) ( ThreadId tid, Addr a, 
                                  Bool isAddrErr, const HChar* msg, UInt otag );
void FC_(record_user_error)     ( ThreadId tid, Addr a,
                                  Bool isAddrErr, UInt otag );

Bool FC_(record_leak_error)     ( ThreadId tid,
                                  UInt n_this_record,
                                  UInt n_total_records,
                                  LossRecord* lossRecord,
                                  Bool print_record,
                                  Bool count_error );

Bool FC_(record_fishy_value_error)  ( ThreadId tid, const HChar* function,
                                      const HChar *argument_name, SizeT value );

void FC_(record_revert_error)      ( ThreadId tid, Addr addr, Int ident,
				       UChar cur_value, UChar old_value,
				       Addr written_at );

void FC_(record_leaked_word)(Addr addr);


/* Leak kinds tokens to call VG_(parse_enum_set). */
extern const HChar* FC_(parse_leak_kinds_tokens);

/* prints a description of address a */
void FC_(pp_describe_addr) (Addr a);

/* Is this address in a user-specified "ignored range" ? */
Bool FC_(in_ignored_range) ( Addr a );

int FC_(count_tainted_bits)(Addr a, SizeT len);

void FC_(add_to_leaked)(int num_to_add);

/*------------------------------------------------------------*/
/*--- Client blocks                                        ---*/
/*------------------------------------------------------------*/

/* Describes a client block.  See fc_main.c.  An unused block has
   start == size == 0.  */
typedef
   struct {
      Addr        start;
      SizeT       size;
      ExeContext* where;
      HChar*      desc;
   } 
   CGenBlock;

/* Get access to the client block array. */
void FC_(get_ClientBlock_array)( /*OUT*/CGenBlock** blocks,
                                 /*OUT*/UWord* nBlocks );


/*------------------------------------------------------------*/
/*--- Command line options + defaults                      ---*/
/*------------------------------------------------------------*/

/* Allow loads from partially-valid addresses?  default: YES */
extern Bool FC_(clo_partial_loads_ok);

/* Max volume of the freed blocks queue. */
extern Long FC_(clo_freelist_vol);

/* Blocks with a size >= FC_(clo_freelist_big_blocks) will be put
   in the "big block" freed blocks queue. */
extern Long FC_(clo_freelist_big_blocks);

/* Do leak check at exit?  default: NO */
extern LeakCheckMode FC_(clo_leak_check);

/* How closely should we compare ExeContexts in leak records? default: 2 */
extern VgRes FC_(clo_leak_resolution);

/* In leak check, show loss records if their R2S(reachedness) is set.
   Default : R2S(Possible) | R2S(Unreached). */
extern UInt FC_(clo_show_leak_kinds);

/* In leak check, a loss record is an error if its R2S(reachedness) is set.
   Default : R2S(Possible) | R2S(Unreached). */
extern UInt FC_(clo_errors_for_leak_kinds);

/* Various leak check heuristics which can be activated/deactivated. */
typedef 
   enum {
      LchNone                =0,
      // no heuristic.
      LchStdString           =1,
      // Consider interior pointer pointing at the array of char in a
      // std::string as reachable.
      LchLength64            =2,
      // Consider interior pointer pointing at offset 64bit of a block as
      // reachable, when the first 8 bytes contains the block size - 8.
      // Such length+interior pointers are used by e.g. sqlite3MemMalloc.
      // On 64bit platforms LchNewArray will also match these blocks.
      LchNewArray            =3,
      // Consider interior pointer pointing at second word of a new[] array as
      // reachable. Such interior pointers are used for arrays whose elements
      // have a destructor.
      LchMultipleInheritance =4,
      // Conside interior pointer pointing just after what looks a vtable
      // as reachable.
  }
  LeakCheckHeuristic;

// Nr of heuristics, including the LchNone heuristic.
#define N_LEAK_CHECK_HEURISTICS 5

// Build mask to check or set Heuristic h membership
#define H2S(h) (1 << (h))
// Heuristic h is member of the Set s ?
#define HiS(h,s) ((s) & H2S(h))

/* Heuristics set to use for the leak search.
   Default : all heuristics. */
extern UInt FC_(clo_leak_check_heuristics);

/* Assume accesses immediately below %esp are due to gcc-2.96 bugs.
 * default: NO */
extern Bool FC_(clo_workaround_gcc296_bugs);

/* Fill malloc-d/free-d client blocks with a specific value?  -1 if
   not, else 0x00 .. 0xFF indicating the fill value to use.  Can be
   useful for causing programs with bad heap corruption to fail in
   more repeatable ways.  Note that malloc-filled and free-filled
   areas are still undefined and noaccess respectively.  This merely
   causes them to contain the specified values. */
extern Int FC_(clo_malloc_fill);
extern Int FC_(clo_free_fill);

/* Which stack trace(s) to keep for malloc'd/free'd client blocks?
   For each client block, the stack traces where it was allocated
   and/or freed are optionally kept depending on FC_(clo_keep_stacktraces). */
typedef
   enum {                 // keep alloc stack trace ?  keep free stack trace ?
      KS_none,            // never                     never
      KS_alloc,           // always                    never
      KS_free,            // never                     always
      KS_alloc_then_free, // when still malloc'd       when free'd
      KS_alloc_and_free,  // always                    always
   }
   KeepStacktraces;
extern KeepStacktraces FC_(clo_keep_stacktraces);

/* Indicates the level of instrumentation/checking done by Memcheck.

   1 = No undefined value checking, Addrcheck-style behaviour only:
       only address checking is done.  This is faster but finds fewer
       errors.  Note that although Addrcheck had 1 bit per byte
       overhead vs the old Memcheck's 9 bits per byte, with this mode
       and compressed V bits, no memory is saved with this mode --
       it's still 2 bits per byte overhead.  This is a little wasteful
       -- it could be done with 1 bit per byte -- but lets us reuse
       the many shadow memory access functions.  Note that in this
       mode neither the secondary V bit table nor the origin-tag cache
       are used.

   2 = Address checking and Undefined value checking are performed,
       but origins are not tracked.  So the origin-tag cache is not
       used in this mode.  This setting is the default and corresponds
       to the "normal" Memcheck behaviour that has shipped for years.

   3 = Address checking, undefined value checking, and origins for
       undefined values are tracked.

   The default is 2.
*/
extern Int FC_(clo_fc_level);

/* Should we show mismatched frees?  Default: YES */
extern Bool FC_(clo_show_mismatched_frees);

/* Should we use expensive definedness checking for add/sub and compare
   operations? Default: NO */
extern Bool FC_(clo_expensive_definedness_checks);


/* Treat data read from non-world-readable files as tainted?
 * default: NO */
extern Bool FC_(clo_private_files_are_secret);

/* Treat data that's unpredictable, like /dev/urandom, the PID, and
 * the time of day, as `secret' (for use in testing RNGs, etc.)?
 * default: NO */
extern Bool FC_(clo_unpredictable_is_secret);

/* Treat data read from the standard input as tainted?
 * default: NO */
extern Bool FC_(clo_stdin_is_secret);

/* Report leaks with a backtrace and other Memcheck-like details?
 * default: NO */
extern Bool  FC_(clo_detailed_leak_report);

/* Abort execution after this many bits have been leaked
 * default: effectively infinite */
extern Long  FC_(clo_max_bits_leaked);

/* Output a graph showing all the operations on secrets along with
 * edge capacities?
 * default: NO */
extern Bool  FC_(clo_trace_secret_graph);

/* Mark all bits generated inside an enclosure region as secret?
 * default: NO */
extern Bool  FC_(clo_taint_all_enclosed);

/* Print a warning message every time an enclosure region reverts a *
 * memory location on exit?
 * default: NO */
extern Bool  FC_(clo_trace_reverts);

/* Taint the modified bits every time an enclosure region reverts a *
 * memory location on exit?
 * default: NO */
extern Bool  FC_(clo_taint_reverts);

extern Int  FC_(clo_revert_notbelow);

/* To what extent should the flow graph be collapsed by combining
   similar edges? Currently defined values:

   0   - do not collapse at all (edges will be output incrementally)
   10  - collapse edges with the same source location and call stack
         (fully context sensitive)
   50  - collapse edges with the same source location and caller
         (one level context sensitive)
   100 - collapse edges with the same source location, no matter
         what call stack (context insensitive)
   default: 10 */
extern Int FC_(clo_folding_level);

/* Run the maximum flow computation after each output? */
extern Bool FC_(clo_incremental);

/* File to store the produced graph in */
extern const char *FC_(clo_graph_file);
extern const char *FC_(project_name);//by xuerao
extern Bool          FC_(secret_start);

/* Maximum flow program to pass it to */
extern const char *FC_(clo_max_flow_program);

/*------------------------------------------------------------*/
/*--- Variables                                            ---*/
/*------------------------------------------------------------*/

extern Long FC_(total_bits_leaked);
extern int  FC_(enclosure_mode);
extern int  FC_(enclosure_level);

/*------------------------------------------------------------*/
/*--- Rollback                                             ---*/
/*------------------------------------------------------------*/

extern int  FC_(rollback_mode);

extern Bool FC_(is_escaping)(Addr addr);

/*------------------------------------------------------------*/
/*--- Instrumentation                                      ---*/
/*------------------------------------------------------------*/

/* Functions defined in fc_main.c */

/* For the fail_w_o functions, the UWord arg is actually the 32-bit
   origin tag and should really be UInt, but to be simple and safe
   considering it's called from generated code, just claim it to be a
   UWord. */

void FC_(helperc_value_checkN_fail_w_o) ( HWord, UInt,
							UInt, ULong, ULong, UWord );
void FC_(helperc_value_check8_fail_w_o) ( ULong, ULong, UWord );
void FC_(helperc_value_check4_fail_w_o) ( UInt, UInt, ULong,
							   ULong, UWord );
void FC_(helperc_value_check1_fail_w_o) ( UChar, UChar,
							   ULong, ULong, UWord);
void FC_(helperc_value_check0_fail_w_o) ( ULong, ULong, UWord );

/* And call these ones instead to report an uninitialised value error
   but with no origin available. */
void FC_(helperc_value_checkN_fail_no_o) ( HWord, UInt,
							UInt, ULong, ULong );
void FC_(helperc_value_check8_fail_no_o) ( ULong, ULong);
void FC_(helperc_value_check4_fail_no_o) ( UInt, UInt, ULong,
							   ULong );
void FC_(helperc_value_check1_fail_no_o) ( UChar, UChar,
							   ULong, ULong);
void FC_(helperc_value_check0_fail_no_o) ( ULong, ULong );

/* V-bits load/store helpers */
VG_REGPARM(1) void FC_(helperc_STOREV64be) ( Addr, ULong );
VG_REGPARM(1) void FC_(helperc_STOREV64le) ( Addr, ULong );
VG_REGPARM(2) void FC_(helperc_STOREV32be) ( Addr, UWord );
VG_REGPARM(2) void FC_(helperc_STOREV32le) ( Addr, UWord );
VG_REGPARM(2) void FC_(helperc_STOREV16be) ( Addr, UWord );
VG_REGPARM(2) void FC_(helperc_STOREV16le) ( Addr, UWord );
VG_REGPARM(2) void FC_(helperc_STOREV8)    ( Addr, UWord );

VG_REGPARM(2) void  FC_(helperc_LOADV256be) ( /*OUT*/V256*, Addr );
VG_REGPARM(2) void  FC_(helperc_LOADV256le) ( /*OUT*/V256*, Addr );
VG_REGPARM(2) void  FC_(helperc_LOADV128be) ( /*OUT*/V128*, Addr );
VG_REGPARM(2) void  FC_(helperc_LOADV128le) ( /*OUT*/V128*, Addr );
VG_REGPARM(1) ULong FC_(helperc_LOADV64be)  ( Addr );
VG_REGPARM(1) ULong FC_(helperc_LOADV64le)  ( Addr );
VG_REGPARM(1) UWord FC_(helperc_LOADV32be)  ( Addr );
VG_REGPARM(1) UWord FC_(helperc_LOADV32le)  ( Addr );
VG_REGPARM(1) UWord FC_(helperc_LOADV16be)  ( Addr );
VG_REGPARM(1) UWord FC_(helperc_LOADV16le)  ( Addr );
VG_REGPARM(1) UWord FC_(helperc_LOADV8)     ( Addr );

VG_REGPARM(1) void FC_(save_for_rollback8) (Addr location);
VG_REGPARM(1) void FC_(save_for_rollback16)(Addr location);
VG_REGPARM(1) void FC_(save_for_rollback32)(Addr location);
VG_REGPARM(1) void FC_(save_for_rollback64)(Addr location);
VG_REGPARM(1) void FC_(save_for_rollback128)(Addr location);

VG_REGPARM(3)
void FC_(helperc_MAKE_STACK_UNINIT_w_o) ( Addr base, UWord len, Addr nia );

VG_REGPARM(2)
void FC_(helperc_MAKE_STACK_UNINIT_no_o) ( Addr base, UWord len );

VG_REGPARM(1)
void FC_(helperc_MAKE_STACK_UNINIT_128_no_o) ( Addr base );

/* Origin tag load/store helpers */
VG_REGPARM(2) void  FC_(helperc_b_store1) ( Addr a, UWord d32 );
VG_REGPARM(2) void  FC_(helperc_b_store2) ( Addr a, UWord d32 );
VG_REGPARM(2) void  FC_(helperc_b_store4) ( Addr a, UWord d32 );
VG_REGPARM(2) void  FC_(helperc_b_store8) ( Addr a, UWord d32 );
VG_REGPARM(2) void  FC_(helperc_b_store16)( Addr a, UWord d32 );
VG_REGPARM(2) void  FC_(helperc_b_store32)( Addr a, UWord d32 );
VG_REGPARM(1) UWord FC_(helperc_b_load1) ( Addr a );
VG_REGPARM(1) UWord FC_(helperc_b_load2) ( Addr a );
VG_REGPARM(1) UWord FC_(helperc_b_load4) ( Addr a );
VG_REGPARM(1) UWord FC_(helperc_b_load8) ( Addr a );
VG_REGPARM(1) UWord FC_(helperc_b_load16)( Addr a );
VG_REGPARM(1) UWord FC_(helperc_b_load32)( Addr a );

/* Functions defined in fc_translate.c */
IRSB* FC_(instrument) ( VgCallbackClosure* closure,
                        IRSB* bb_in, 
                        const VexGuestLayout* layout, 
                        const VexGuestExtents* vge,
                        const VexArchInfo* archinfo_host,
                        IRType gWordTy, IRType hWordTy );

IRSB* FC_(final_tidy) ( IRSB* );

#endif /* ndef __FC_INCLUDE_H */

/*--------------------------------------------------------------------*/
/*--- end                                                          ---*/
/*--------------------------------------------------------------------*/
