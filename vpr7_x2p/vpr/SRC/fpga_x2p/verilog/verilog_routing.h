/***********************************************
 * Header file for verilog_routing.cpp
 **********************************************/
#ifndef VERILOG_ROUTING_H
#define VERILOG_ROUTING_H

/* Include other header files which are dependency on the function declared below */
#include "mux_library.h"
#include "module_manager.h"
#include "rr_blocks.h"

void dump_verilog_routing_chan_subckt(t_sram_orgz_info* cur_sram_orgz_info,
                                      char* verilog_dir,
                                      char* subckt_dir,
                                      int x,  int y,
                                      t_rr_type chan_type, 
                                      int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                      t_ivec*** LL_rr_node_indices,
                                      t_rr_indexed_data* LL_rr_indexed_data,
                                      int num_segment, t_segment_inf* segments,
                                      t_syn_verilog_opts fpga_verilog_opts);

void dump_verilog_grid_side_pin_with_given_index(FILE* fp, t_rr_type pin_type,
                                                 int pin_index, int side,
                                                 int x, int y,
                                                 int unique_x, int unique_y, /* If explicit, needs the coordinates of the mirror*/
                                                 boolean dump_port_type,
                                                 bool is_explicit_mapping);

void dump_verilog_grid_side_pins(FILE* fp,
                                 t_rr_type pin_type, int x, int y, int side,
                                 boolean dump_port_type);

void dump_verilog_switch_box_chan_port(FILE* fp,
                                       t_sb* cur_sb_info, 
                                       int chan_side,
                                       t_rr_node* cur_rr_node,
                                       enum PORTS cur_rr_node_direction);

void dump_verilog_switch_box_short_interc(FILE* fp, 
                                          t_sb* cur_sb_info,
                                          int chan_side,
                                          t_rr_node* cur_rr_node,
                                          int actual_fan_in,
                                          t_rr_node* drive_rr_node,
                                          bool is_explicit_mapping);

void dump_verilog_switch_box_mux(t_sram_orgz_info* cur_sram_orgz_info,
                                 FILE* fp, 
                                 t_sb* cur_sb_info, 
                                 int chan_side,
                                 t_rr_node* cur_rr_node,
                                 int mux_size,
                                 t_rr_node** drive_rr_nodes,
                                 int switch_index,
                                 bool is_explicit_mapping);

int count_verilog_switch_box_interc_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                              t_sb cur_sb_info, int chan_side, 
                                              t_rr_node* cur_rr_node);

int count_verilog_switch_box_interc_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                       t_sb cur_sb_info, int chan_side, 
                                                       t_rr_node* cur_rr_node);

void dump_verilog_switch_box_interc(t_sram_orgz_info* cur_sram_orgz_info,
                                    FILE* fp, 
                                    t_sb* cur_sb_info,
                                    int chan_side,
                                    t_rr_node* cur_rr_node,
                                    bool is_explicit_mapping);

int count_verilog_switch_box_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                      t_sb cur_sb_info);

int count_verilog_switch_box_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                      t_sb cur_sb_info);

void dump_verilog_routing_switch_box_subckt(t_sram_orgz_info* cur_sram_orgz_info,
                                            char* verilog_dir, char* subckt_dir, 
                                            t_sb* cur_sb_info, 
                                            int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                            t_ivec*** LL_rr_node_indices,
                                            t_syn_verilog_opts fpga_verilog_opts,
                                            boolean compact_routing_hierarchy,
                                            bool is_explicit_mapping);


void dump_verilog_connection_box_short_interc(FILE* fp,
                                              t_cb* cur_cb_info,
                                              t_rr_node* src_rr_node,
                                              bool is_explicit_mapping);

void dump_verilog_connection_box_mux(t_sram_orgz_info* cur_sram_orgz_info,
                                     FILE* fp,
                                     t_cb* cur_cb_info,
                                     t_rr_node* src_rr_node,
                                     bool is_explicit_mapping);

void dump_verilog_connection_box_interc(t_sram_orgz_info* cur_sram_orgz_info,
                                        FILE* fp,
                                        t_cb* cur_cb_info,
                                        t_rr_node* src_rr_node,
                                        bool is_explicit_mapping);


int count_verilog_connection_box_interc_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                  t_rr_node* cur_rr_node);

int count_verilog_connection_box_one_side_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                             const RRGSB& rr_gsb, enum e_side cb_side);

int count_verilog_connection_box_interc_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                           t_rr_node* cur_rr_node);

int count_verilog_connection_box_one_side_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                    const RRGSB& rr_gsb, enum e_side cb_side);

int count_verilog_connection_box_one_side_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                    int num_ipin_rr_nodes,
                                                    t_rr_node** ipin_rr_node);

int count_verilog_connection_box_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                    const RRGSB& rr_gsb, t_rr_type cb_type);

int count_verilog_connection_box_one_side_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                             int num_ipin_rr_nodes,
                                                             t_rr_node** ipin_rr_node);

int count_verilog_connection_box_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                           const RRGSB& rr_gsb, t_rr_type cb_type);

int count_verilog_connection_box_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                           t_cb* cur_cb_info);

int count_verilog_connection_box_reserved_conf_bits(t_sram_orgz_info* cur_sram_orgz_info,
                                                    t_cb* cur_cb_info);

void dump_verilog_routing_connection_box_subckt(t_sram_orgz_info* cur_sram_orgz_info,
                                                char* verilog_dir, char* subckt_dir, 
                                                t_cb* cur_cb_info,
                                                boolean compact_routing_hierarchy,
                                                bool is_explicit_mapping);

void print_verilog_routing_resources(ModuleManager& module_manager,
                                     t_sram_orgz_info* cur_sram_orgz_info,
                                     char* verilog_dir,
                                     char* subckt_dir,
                                     const t_arch& arch,
                                     const t_det_routing_arch& routing_arch,
                                     int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                     t_ivec*** LL_rr_node_indices,
                                     t_rr_indexed_data* LL_rr_indexed_data,
                                     const t_fpga_spice_opts& FPGA_SPICE_Opts);

void print_verilog_flatten_routing_modules(ModuleManager& module_manager,
                                           const DeviceRRGSB& L_device_rr_gsb,
                                           const t_det_routing_arch& routing_arch,
                                           const std::string& verilog_dir,
                                           const std::string& subckt_dir,
                                           const bool& use_explicit_port_map);

void print_verilog_unique_routing_modules(ModuleManager& module_manager,
                                          const DeviceRRGSB& L_device_rr_gsb,
                                          const t_det_routing_arch& routing_arch,
                                          const std::string& verilog_dir,
                                          const std::string& subckt_dir,
                                          const bool& use_explicit_port_map);

#endif
