#ifndef RR_GRAPH_H
#define RR_GRAPH_H

enum e_graph_type {
	GRAPH_GLOBAL, /* One node per channel with wire capacity > 1 and full connectivity */
	GRAPH_BIDIR, /* Detailed bidirectional graph */
	GRAPH_UNIDIR, /* Detailed unidir graph, untilable */
	/* RESEARCH TODO: Get this option debugged */
	GRAPH_UNIDIR_TILEABLE /* Detail unidir graph with wire groups multiples of 2*L */
};
typedef enum e_graph_type t_graph_type;

/* Warnings about the routing graph that can be returned.
 * This is to avoid output messages during a value sweep */
enum {
	RR_GRAPH_NO_WARN = 0x00,
	RR_GRAPH_WARN_FC_CLIPPED = 0x01,
	RR_GRAPH_WARN_CHAN_WIDTH_CHANGED = 0x02
};

void build_rr_graph(INP t_graph_type graph_type, 
		INP int L_num_types,
		INP t_type_ptr types, 
		INP int L_nx, 
		INP int L_ny,
		INP struct s_grid_tile **L_grid, 
		INP int chan_width,
		INP struct s_chan_width_dist *chan_capacity_inf,
		INP enum e_switch_block_type sb_type, 
		INP int Fs, 
		INP int num_seg_types,
		INP int num_switches, 
		INP t_segment_inf * segment_inf,
		INP int global_route_switch, 
		INP int delayless_switch,
		INP t_timing_inf timing_inf, 
		INP int wire_to_ipin_switch,
		INP enum e_base_cost_type base_cost_type, 
		INP t_direct_inf *directs, 
		INP int num_directs,
		INP boolean ignore_Fc_0,
		OUTP int *Warnings,
        /* Xifan TANG: Switch Segment Pattern Support*/
        int num_swseg_pattern,
        t_swseg_pattern_inf* swseg_patterns,
        boolean opin_to_cb_fast_edges,
        boolean opin_logic_eq_edges);

void free_rr_graph(void);

void dump_rr_graph(INP const char *file_name);
void print_rr_indexed_data(FILE * fp, int index); /* For debugging only */
void load_net_rr_terminals(t_ivec *** L_rr_node_indices);

void print_rr_node(FILE *fp, t_rr_node *L_rr_node, int inode);

int **
alloc_and_load_actual_fc(INP int L_num_types, INP t_type_ptr types,
		INP int nodes_per_chan, INP boolean is_Fc_out,
		INP enum e_directionality directionality, OUTP boolean * Fc_clipped, INP boolean ignore_Fc_0);

void rr_graph_externals(t_timing_inf timing_inf,
		t_segment_inf * segment_inf, int num_seg_types, int nodes_per_chan,
		int wire_to_ipin_switch, enum e_base_cost_type base_cost_type);

/* Xifan Tang: Functions shared by tileable rr_graph generator */

int ****alloc_and_load_pin_to_track_map(INP enum e_pin_type pin_type,
		INP int nodes_per_chan, INP int *Fc, INP t_type_ptr Type,
		INP boolean perturb_switch_pattern,
		INP enum e_directionality directionality);

struct s_ivec ***alloc_and_load_track_to_pin_lookup(
		INP int ****pin_to_track_map, INP int *Fc, INP int height,
		INP int num_pins, INP int nodes_per_chan);

boolean *
alloc_and_load_perturb_ipins(INP int nodes_per_chan, INP int L_num_types,
		INP int **Fc_in, INP int **Fc_out, INP enum e_directionality directionality);

void free_type_pin_to_track_map(int***** ipin_to_track_map,
		t_type_ptr types);

void free_type_track_to_ipin_map(struct s_ivec**** track_to_pin_map,
		t_type_ptr types, int nodes_per_chan);


#endif

