
void verilog_generate_report_timing(t_sram_orgz_info* cur_sram_orgz_info,
                                    char* sdc_dir,
                                    t_arch arch,
                                    t_det_routing_arch* routing_arch,
                                    int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                    t_ivec*** LL_rr_node_indices,
                                    t_syn_verilog_opts fpga_verilog_opts);

