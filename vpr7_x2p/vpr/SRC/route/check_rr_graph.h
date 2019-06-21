#ifndef CHECK_RR_GRAPH_H
#define CHECK_RR_GRAPH_H

void check_rr_graph(INP const t_graph_type graph_type,
		INP t_type_ptr types,
		INP const int L_nx,
		INP const int L_ny,
		INP const int nodes_per_chan,
		INP const int Fs,
		INP const int num_seg_types,
		INP const int num_switches,
		INP const t_segment_inf * segment_inf,
		INP const int global_route_switch,
		INP const int delayless_switch,
		INP const int wire_to_ipin_switch,
		int ** Fc_in,
		int ** Fc_out);

void check_node(int inode, enum e_route_type route_type);

#endif

