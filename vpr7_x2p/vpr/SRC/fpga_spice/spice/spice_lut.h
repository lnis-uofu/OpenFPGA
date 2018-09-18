


void fprint_spice_lut_subckt(FILE* fp,
                             t_spice_model spice_model);

void generate_spice_luts(char* subckt_dir, 
                         int num_spice_model, 
                         t_spice_model* spice_models);


void fprint_pb_primitive_lut(FILE* fp,
                             char* subckt_prefix,
                             t_pb* prim_pb,
                             t_logical_block* mapped_logical_block,
                             t_pb_graph_node* cur_pb_graph_node,
                             int index,
                             t_spice_model* spice_model);
