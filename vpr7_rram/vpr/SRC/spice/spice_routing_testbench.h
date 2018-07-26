

void spice_print_cb_testbench(char* formatted_spice_dir,
                               char* circuit_name,
                               char* include_dir_path,
                               char* subckt_dir_path,
                               t_ivec*** LL_rr_node_indices,
                               int num_clocks,
                               t_arch arch,
                               boolean leakage_only);

void spice_print_sb_testbench(char* formatted_spice_dir,
                               char* circuit_name,
                               char* include_dir_path,
                               char* subckt_dir_path,
                               t_ivec*** LL_rr_node_indices,
                               int num_clocks,
                               t_arch arch,
                               boolean leakage_only);
