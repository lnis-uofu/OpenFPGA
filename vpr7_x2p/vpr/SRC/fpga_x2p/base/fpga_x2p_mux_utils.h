#ifndef FPGA_X2P_MUX_UTILS_H
#define FPGA_X2P_MUX_UTILS_H

int determine_num_sram_bits_mux_basis_subckt(t_spice_model* mux_spice_model,
                                             int mux_size,
                                             int num_input_per_level,
                                             boolean special_basis);

int determine_tree_mux_level(int mux_size);

int determine_num_input_basis_multilevel_mux(int mux_size,
                                             int mux_level);

int tree_mux_last_level_input_num(int num_level,
                                  int mux_size);

int multilevel_mux_last_level_input_num(int num_level, int num_input_per_unit,
                                        int mux_size);

int* decode_onelevel_mux_sram_bits(int fan_in,
                                   int mux_level,
                                   int path_id);

int* decode_multilevel_mux_sram_bits(int fan_in,
                                     int mux_level,
                                     int path_id);

int* decode_tree_mux_sram_bits(int fan_in,
                               int mux_level,
                               int path_id);

int get_mux_default_path_id(t_spice_model* mux_spice_model,
                            int mux_size, int path_id);

int get_mux_full_input_size(t_spice_model* mux_spice_model,
                            int mux_size);

void decode_cmos_mux_sram_bits(t_spice_model* mux_spice_model,
                               int mux_size, int path_id, 
                               int* bit_len, int** conf_bits, int* mux_level);

void decode_one_level_4t1r_mux(int path_id, 
                                       int bit_len, int* conf_bits); 

void decode_rram_mux(t_spice_model* mux_spice_model,
                             int mux_size, int path_id,
                             int* bit_len, int** conf_bits, int* mux_level);

void init_spice_mux_arch(t_spice_model* spice_model,
                         t_spice_mux_arch* spice_mux_arch,
                         int mux_size);

int find_spice_mux_arch_special_basis_size(t_spice_mux_arch spice_mux_arch);

t_llist* search_mux_linked_list(t_llist* mux_head,
                                int mux_size,
                                t_spice_model* spice_model);

void check_and_add_mux_to_linked_list(t_llist** muxes_head,
                                      int mux_size,
                                      t_spice_model* spice_model);

void free_muxes_llist(t_llist* muxes_head);

void stats_lut_spice_mux(t_llist** muxes_head,
                         t_spice_model* spice_model);


void stats_spice_muxes_routing_arch(t_llist** muxes_head,
                                    int num_switch,
                                    t_switch_inf* switches,
                                    t_spice* spice,
                                    t_det_routing_arch* routing_arch);

void stats_mux_spice_model_pb_type_rec(t_llist** muxes_head,
                                       t_pb_type* cur_pb_type);

void stats_mux_spice_model_pb_node_rec(t_llist** muxes_head,
                                       t_pb_graph_node* cur_pb_node);

t_llist* stats_spice_muxes(int num_switch,
                           t_switch_inf* switches,
                           t_spice* spice,
                           t_det_routing_arch* routing_arch);


#endif
