
enum e_spice_mux_tb_type {
  SPICE_CB_MUX_TB, SPICE_SB_MUX_TB, SPICE_PB_MUX_TB 
};

void spice_print_mux_testbench(char* formatted_spice_dir,
                                char* circuit_name,
                                char* include_dir_path,
                                char* subckt_dir_path,
                                t_ivec*** LL_rr_node_indices,
                                int num_clock,
                                t_arch arch,
                                enum e_spice_mux_tb_type mux_tb_type,
                                boolean leakage_only);

/* useful subroutines */
void fprint_spice_mux_testbench_pb_graph_pin_inv_loads_rec(FILE* fp, 
                                                           int grid_x, int grid_y,
                                                           t_pb_graph_pin* src_pb_graph_pin, 
                                                           t_pb* src_pb, 
                                                           char* outport_name,
                                                           boolean consider_parent_node,
                                                           t_ivec*** LL_rr_node_indices);

