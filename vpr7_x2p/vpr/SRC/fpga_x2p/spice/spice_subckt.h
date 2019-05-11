
int generate_spice_nmos_pmos(char* subckt_dir,
                             t_spice_tech_lib tech_lib);

int generate_spice_basics(char* subckt_dir,
                          t_arch arch);

void generate_spice_subckt_tapbuf(FILE* fp, 
                                  t_spice_model_buffer* output_buf);

void generate_spice_subckts(char* subckt_dir,
                            t_arch* arch,
                            t_det_routing_arch* routing_arch,
                            boolean compact_routing_hierarchy);
