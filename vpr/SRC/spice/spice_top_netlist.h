void fprint_spice_top_netlist(char* circuit_name,
                              char* top_netlist_name,
                              char* include_dir_path,
                              char* subckt_dir_path,
                              t_ivec*** LL_rr_node_indices,
                              int num_clock,
                              t_spice spice,
                              boolean leakage_only);
