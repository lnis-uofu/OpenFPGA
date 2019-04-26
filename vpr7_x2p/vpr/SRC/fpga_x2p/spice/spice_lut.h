


void fprint_spice_lut_subckt(FILE* fp,
                             t_spice_model* spice_model);

void generate_spice_luts(char* subckt_dir, 
                         int num_spice_model, 
                         t_spice_model* spice_models);


void fprint_pb_primitive_lut(FILE* fp,
                             char* subckt_prefix,
                             t_phy_pb* prim_phy_pb,
                             t_pb_type* prim_pb_type,
                             int index,
                             t_spice_model* spice_model);
