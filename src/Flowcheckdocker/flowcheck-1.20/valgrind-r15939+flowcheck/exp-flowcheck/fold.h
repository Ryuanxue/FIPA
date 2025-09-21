#ifndef FOLD_H
#define FOLD_H

struct edge_info;

void fold_init(void);

void fold_new_tag(ULong tag);

UWord get_free_tag(void);
void check_enough_tags(Word needed);
void mark_tag_in_use(UWord tag);

void fold_new_edge(ULong src, ULong dest, int capacity,
		   struct edge_info *edge);
void output_folded_edges_stderr(void);
void output_folded_edges_filename(const char *);
void run_max_flow_program(const char *fname);
void run_incremental_max_flow(void);

#endif
