

int rec_count_rr_graph_nodes_for_phy_pb_graph_node(t_pb_graph_node* cur_pb_graph_node);

void init_one_rr_node_pack_cost_for_phy_graph_node(INP t_pb_graph_pin* cur_pb_graph_pin,
                                                   INOUTP t_rr_graph* local_rr_graph,
                                                   int cur_rr_node_index,
                                                   enum PORTS port_type);

void init_one_rr_node_for_phy_pb_graph_node(INP t_pb_graph_pin* cur_pb_graph_pin,
                                            INOUTP t_rr_graph* local_rr_graph,
                                            int cur_rr_node_index,
                                            int phy_mode_index, 
                                            t_rr_type rr_node_type);

void connect_one_rr_node_for_phy_pb_graph_node(INP t_pb_graph_pin* cur_pb_graph_pin,
                                               INOUTP t_rr_graph* local_rr_graph,
                                               int cur_rr_node_index,
                                               int phy_mode_index, 
                                               t_rr_type rr_node_type);

int rec_init_rr_graph_for_phy_pb_graph_node(INP t_pb_graph_node* cur_pb_graph_node, 
                                            INOUTP t_rr_graph* local_rr_graph,
                                            int cur_rr_node_index);

int rec_connect_rr_graph_for_phy_pb_graph_node(INP t_pb_graph_node* cur_pb_graph_node, 
                                               INOUTP t_rr_graph* local_rr_graph,
                                               int cur_rr_node_index);

void alloc_and_load_rr_graph_for_phy_pb_graph_node(INP t_pb_graph_node* top_pb_graph_node, 
                                                  OUTP t_rr_graph* local_rr_graph);

void alloc_and_load_phy_pb_rr_graph_nets(INP t_pb* cur_op_pb,
                                         t_rr_graph* local_rr_graph,
                                         int L_num_vpack_nets, t_net* L_vpack_net);

void load_phy_pb_rr_graph_net_rr_terminals(INP t_pb* cur_op_pb,
                                           t_rr_graph* local_rr_graph);

void alloc_and_load_rr_graph_for_phy_pb(INP t_pb* cur_op_pb,
                                        INP t_phy_pb* cur_phy_pb,
                                        int L_num_vpack_nets, t_net* L_vpack_net);
