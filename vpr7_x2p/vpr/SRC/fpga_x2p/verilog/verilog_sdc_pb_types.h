
void sdc_dump_annotation(char* from_path, // includes the cell
						char* to_path,
						FILE* fp,
						t_pb_graph_edge* cur_edge);

void dump_sdc_pb_graph_pin_interc(t_sram_orgz_info* cur_sram_orgz_info,
                                      FILE* fp,
                                      enum e_spice_pin2pin_interc_type pin2pin_interc_type,
                                      t_pb_graph_pin* des_pb_graph_pin,
                                      t_mode* cur_mode,
                                      char* instance_name); 

void dump_sdc_pb_graph_port_interc(t_sram_orgz_info* cur_sram_orgz_info,
                                       FILE* fp,
                                       t_pb_graph_node* cur_pb_graph_node,
                                       enum e_spice_pb_port_type pb_port_type,
                                       t_mode* cur_mode, 
                                       char* instance_name); 

void sdc_dump_cur_node_constraints(t_sram_orgz_info* cur_sram_orgz_info,
							  FILE*  fp,
							  t_pb_graph_node* cur_pb_graph_node,
							  int select_mode_index, 
                              char* instance_name); 

void sdc_rec_dump_child_pb_graph_node(t_sram_orgz_info* cur_sram_orgz_info,
									 FILE* fp,
									 t_pb_graph_node* cur_pb_graph_node, 
                                     char* instance_name); 

void sdc_dump_all_pb_graph_nodes(FILE* fp,
							t_sram_orgz_info* cur_sram_orgz_info,
							int type_descriptor_mode,
                            char* instance_name); 

void dump_sdc_physical_blocks(t_sram_orgz_info* cur_sram_orgz_info,
							char* sdc_path,
							int type_descriptor_mode, 
                            char* instance_name); 

void verilog_generate_sdc_constrain_pb_types(t_sram_orgz_info* cur_sram_orgz_info,
                                             char* sdc_dir);
