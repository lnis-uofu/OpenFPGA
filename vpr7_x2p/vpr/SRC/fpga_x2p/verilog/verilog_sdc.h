
void verilog_generate_sdc_pnr(t_sram_orgz_info* cur_sram_orgz_info,
                              char* sdc_dir,
                              t_arch arch,
                              t_det_routing_arch* routing_arch,
                              int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                              t_ivec*** LL_rr_node_indices,
                              t_rr_indexed_data* LL_rr_indexed_data,
                              int LL_nx, int LL_ny, t_grid_tile** LL_grid,
                              t_syn_verilog_opts fpga_verilog_opts);

void verilog_generate_sdc_analysis(t_sram_orgz_info* cur_sram_orgz_info,
                                   char* sdc_dir,
                                   char* circuit_name,
                                   t_arch arch,
                                   t_det_routing_arch* routing_arch,
                                   int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                   t_ivec*** LL_rr_node_indices,
                                   t_rr_indexed_data* LL_rr_indexed_data,
                                   int LL_nx, int LL_ny, t_grid_tile** LL_grid,
                                   t_block* LL_block, 
                                   t_syn_verilog_opts fpga_verilog_opts);

void verilog_generate_sdc_disable_unused_sbs_muxs(FILE* fp, int LL_nx, int LL_ny);

void verilog_generate_sdc_disable_unused_cbs_muxs(FILE* fp); 

void verilog_generate_sdc_disable_unused_grids_muxs(FILE* fp,
                                                    int LL_nx, int LL_ny,
                                                    t_grid_tile** LL_grid,
                                                    t_block* LL_block);

void dump_sdc_one_clb_muxes(FILE* fp, 
                      char* grid_instance_name,
                      t_rr_graph* rr_graph, 
                      t_pb_graph_node* pb_graph_head);


void dump_sdc_rec_one_pb_muxes(FILE* fp,
                      char* grid_instance_name,
                      t_rr_graph* rr_graph,
                      t_pb_graph_node* cur_pb_graph_node);

void dump_sdc_pb_graph_node_muxes(FILE* fp,
                              char* grid_instance_name,
                              t_rr_graph* rr_graph,
							  t_pb_graph_node* cur_pb_graph_node);

void dump_sdc_pb_graph_pin_muxes (FILE* fp,
                                         char* grid_instance_name, 
                                         t_rr_graph* rr_graph, 
                                         t_pb_graph_pin pb_graph_pin); 

