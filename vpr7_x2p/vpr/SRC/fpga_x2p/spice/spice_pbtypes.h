#ifndef SPICE_PBTYPES_H
#define SPICE_PBTYPES_H

void fprint_pb_type_ports(FILE* fp,
                          char* port_prefix,
                          int use_global_clock,
                          t_pb_type* cur_pb_type);

void fprint_spice_dangling_des_pb_graph_pin_interc(FILE* fp,
                                                   t_pb_graph_pin* des_pb_graph_pin,
                                                   t_mode* cur_mode,
                                                   enum e_spice_pin2pin_interc_type pin2pin_interc_type,
                                                   char* parent_pin_prefix);

void generate_spice_src_des_pb_graph_pin_prefix(t_pb_graph_pin* src_pb_graph_pin,
                                                t_pb_graph_pin* des_pb_graph_pin,
                                                enum e_spice_pin2pin_interc_type pin2pin_interc_type,
                                                t_interconnect* pin2pin_interc,
                                                char* parent_pin_prefix,
                                                char** src_pin_prefix,
                                                char** des_pin_prefix);

void fprintf_spice_pb_graph_pin_interc(FILE* fp,
                                       char* parent_pin_prefix,
                                       enum e_spice_pin2pin_interc_type pin2pin_interc_type,
                                       t_pb_graph_pin* des_pb_graph_pin,
                                       t_mode* cur_mode,
                                       int is_idle);

void fprint_spice_pb_graph_interc(FILE* fp, 
                                  char* pin_prefix,
                                  t_pb_graph_node* cur_pb_graph_node,
                                  t_phy_pb* cur_pb,
                                  int select_mode_index,
                                  int is_idle);

void fprint_spice_pb_graph_primitive_node(FILE* fp,
                                          char* subckt_prefix,
                                          t_pb* cur_pb,
                                          t_pb_graph_node* cur_pb_graph_node,
                                          int pb_type_index);

void fprint_pb_primitive_spice_model(FILE* fp,
                                     char* subckt_prefix,
                                     t_phy_pb* prim_pb,
                                     t_pb_graph_node* prim_pb_graph_node,
                                     int pb_index,
                                     t_spice_model* spice_model,
                                     int is_idle);

void fprint_spice_idle_pb_graph_node_rec(FILE* fp,
                                         char* subckt_prefix,
                                         t_pb_graph_node* cur_pb_graph_node,
                                         int pb_type_index);

void fprint_spice_pb_graph_node_rec(FILE* fp, 
                                    char* subckt_prefix, 
                                    t_pb* cur_pb, 
                                    t_pb_graph_node* cur_pb_graph_node,
                                    int pb_type_index);


void fprint_spice_block(FILE* fp,
                        char* subckt_name, 
                        int x,
                        int y,
                        int z,
                        t_type_ptr type_descriptor,
                        t_block* mapped_block);

void fprint_grid_pins(FILE* fp,
                      int x,
                      int y,
                      int top_level);

void fprint_io_grid_pins(FILE* fp,
                         int x,
                         int y,
                         int top_level);

char* get_grid_block_subckt_name(int x,
                                 int y,
                                 int z,
                                 char* subckt_prefix,
                                 t_block* mapped_block);

void fprint_grid_block_subckt_pins(FILE* fp,
                                   int z,
                                   t_type_ptr type_descriptor);

void fprint_io_grid_block_subckt_pins(FILE* fp,
                                      int x,
                                      int y,
                                      int z,
                                      t_type_ptr type_descriptor);

void fprint_grid_blocks(FILE* fp,
                        int ix,
                        int iy);

void fprint_spice_idle_block(FILE* fp,
                             char* subckt_name, 
                             int x,
                             int y,
                             int z,
                             t_type_ptr type_descriptor);

void generate_spice_logic_blocks(char* subckt_dir, t_arch* arch);

#endif
