

void dump_verilog_pb_generic_primitive(t_sram_orgz_info* cur_sram_orgz_info,
                                       FILE* fp,
                                       char* subckt_prefix,
                                       t_pb_graph_node* prim_pb_graph_node,
                                       int index,
                                       t_spice_model* spice_model);

void dump_verilog_pb_primitive_lut(t_sram_orgz_info* cur_sram_orgz_info,
                                   FILE* fp,
                                   char* subckt_prefix,
                                   t_pb_graph_node* prim_pb_graph_node,
                                   int index,
                                   t_spice_model* spice_model);
