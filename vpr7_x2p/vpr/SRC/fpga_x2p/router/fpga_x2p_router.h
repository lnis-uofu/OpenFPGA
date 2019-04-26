
void breadth_first_expand_rr_graph_trace_segment(t_rr_graph* local_rr_graph,
                                                 t_trace *start_ptr, 
                                                 int remaining_connections_to_sink);

void breadth_first_expand_rr_graph_neighbours(t_rr_graph* local_rr_graph,
                                              int inode, float pcost,
                                              int inet, boolean first_time);

boolean breadth_first_route_one_net_rr_graph_cluster(t_rr_graph* local_rr_graph, 
                                                     int inet);

boolean feasible_routing_rr_graph(t_rr_graph* local_rr_graph);

void pathfinder_update_rr_graph_one_cost(t_rr_graph* local_rr_graph, 
                                         t_trace *route_segment_start,
                                         int add_or_sub, float pres_fac);

void pathfinder_update_rr_graph_cost(t_rr_graph* local_rr_graph,
                                     float pres_fac, float acc_fac);

void breadth_first_add_source_to_rr_graph_heap(t_rr_graph* local_rr_graph,
                                               int src_net_index);

boolean breadth_first_route_one_net_pb_rr_graph(t_rr_graph* local_rr_graph, 
                                                     int inet);

boolean try_breadth_first_route_pb_rr_graph(t_rr_graph* local_rr_graph);
