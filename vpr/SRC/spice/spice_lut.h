

void stats_lut_spice_mux(t_llist** muxes_head,
                         t_spice_model* spice_model);

char* complete_truth_table_line(int lut_size,
                                char* input_truth_table_line);

void configure_lut_sram_bits_per_line_rec(int** sram_bits, 
                                          int lut_size,
                                          char* truth_table_line,
                                          int start_point);

int* generate_lut_sram_bits(int truth_table_len,
                            char** truth_table,
                            int lut_size);

void fprint_spice_lut_subckt(FILE* fp,
                             t_spice_model spice_model);

void generate_spice_luts(char* subckt_dir, 
                         int num_spice_model, 
                         t_spice_model* spice_models);

char** assign_lut_truth_table(t_logical_block* mapped_logical_block,
                              int* truth_table_length);

int get_lut_output_init_val(t_logical_block* lut_logical_block);

void fprint_pb_primitive_lut(FILE* fp,
                             char* subckt_prefix,
                             t_logical_block* mapped_logical_block,
                             t_pb_graph_node* cur_pb_graph_node,
                             int index,
                             t_spice_model* spice_model);
