#ifndef FPGA_X2P_RR_GRAPH_UTILS_H
#define FPGA_X2P_RR_GRAPH_UTILS_H

void init_rr_graph(INOUTP t_rr_graph* local_rr_graph);

void alloc_rr_graph_net_rr_sources_and_sinks(t_rr_graph* local_rr_graph);

void alloc_rr_graph_net_rr_terminals(t_rr_graph* local_rr_graph);

void alloc_rr_graph_route_static_structs(t_rr_graph* local_rr_graph,
                                         int heap_size);

void load_rr_graph_chan_rr_indices(t_rr_graph* local_rr_graph,
                                   INP int nodes_per_chan, INP int chan_len,
                                   INP int num_chans, INP t_rr_type type, INP t_seg_details * seg_details,
                                   INOUTP int *index);

void alloc_and_load_rr_graph_rr_node(INOUTP t_rr_graph* local_rr_graph,
                                     int local_num_rr_nodes);

void alloc_and_load_rr_graph_rr_node_indices(t_rr_graph* local_rr_graph,
                                             INP int nodes_per_chan, 
                                             INP int L_nx, INP int L_ny, t_grid_tile** L_grid, 
                                             INOUTP int *index, INP t_seg_details * seg_details);

void alloc_and_load_rr_graph_switch_inf(INOUTP t_rr_graph* local_rr_graph,
                                        int num_switch_inf,
                                        INP t_switch_inf* switch_inf);

void alloc_and_load_rr_graph_route_structs(t_rr_graph* local_rr_graph);

t_trace* alloc_rr_graph_trace_data(t_rr_graph* local_rr_graph);

t_heap * get_rr_graph_heap_head(t_rr_graph* local_rr_graph);

t_linked_f_pointer* alloc_rr_graph_linked_f_pointer(t_rr_graph* local_rr_graph);

t_heap * alloc_rr_graph_heap_data(t_rr_graph* local_rr_graph);

void add_to_rr_graph_mod_list(t_rr_graph* local_rr_graph,
                              float *fptr);

void empty_rr_graph_heap(t_rr_graph* local_rr_graph);

void reset_rr_graph_rr_node_route_structs(t_rr_graph* local_rr_graph);

t_trace* update_rr_graph_traceback(t_rr_graph* local_rr_graph,
                                   t_heap *hptr, int inet);

void reset_rr_graph_path_costs(t_rr_graph* local_rr_graph);

void alloc_rr_graph_rr_indexed_data(t_rr_graph* local_rr_graph, int L_num_rr_indexed_data);

float get_rr_graph_rr_cong_cost(t_rr_graph* local_rr_graph,
                                int rr_node_index);


void add_heap_node_to_rr_graph_heap(t_rr_graph* local_rr_graph,
                                    t_heap *hptr);

void add_node_to_rr_graph_heap(t_rr_graph* local_rr_graph,
                               int inode, float cost, int prev_node, int prev_edge,
                               float backward_path_cost, float R_upstream);

void mark_rr_graph_sinks(t_rr_graph* local_rr_graph, 
                         int inet, int start_isink, boolean* net_sink_routed);

void mark_rr_graph_ends(t_rr_graph* local_rr_graph, 
                        int inet);

void invalidate_rr_graph_heap_entries(t_rr_graph* local_rr_graph, 
                                      int sink_node, int ipin_node);

float get_rr_graph_rr_node_pack_intrinsic_cost(t_rr_graph* local_rr_graph,
                                               int inode);

void free_rr_graph_rr_nodes(t_rr_graph* local_rr_graph);

void free_rr_graph_switch_inf(INOUTP t_rr_graph* local_rr_graph);

void free_rr_graph_route_structs(t_rr_graph* local_rr_graph);

void free_rr_graph(t_rr_graph* local_rr_graph);

void free_rr_graph_heap_data(t_rr_graph* local_rr_graph,
                             t_heap *hptr);

void free_rr_graph_traceback(t_rr_graph* local_rr_graph, 
                             int inet);

void build_prev_node_list_rr_nodes(int LL_num_rr_nodes,
                                   t_rr_node* LL_rr_node);

void sort_rr_graph_drive_rr_nodes(int LL_num_rr_nodes,
                                  t_rr_node* LL_rr_node);

void alloc_and_load_prev_node_list_rr_graph_rr_nodes(t_rr_graph* local_rr_graph);

void backannotate_rr_graph_routing_results_to_net_name(t_rr_graph* local_rr_graph);

int get_rr_graph_net_vpack_net_index(t_rr_graph* local_rr_graph,
                                     int net_index);

int get_rr_graph_net_index_with_vpack_net(t_rr_graph* local_rr_graph,
                                          int vpack_net_index);

void get_chan_rr_node_start_coordinate(t_rr_node* chan_rr_node,
                                       int* x_start, int* y_start);

void get_chan_rr_node_end_coordinate(t_rr_node* chan_rr_node,
                                     int* x_end, int* y_end);

int get_rr_node_wire_length(t_rr_node* src_rr_node);

#endif
