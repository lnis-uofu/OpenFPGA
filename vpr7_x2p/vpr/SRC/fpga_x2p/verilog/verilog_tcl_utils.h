#ifndef VERILOG_TCL_UTILS_H
#define VERILOG_TCL_UTILS_H

/* Include header files that require by the function declared in the following lines !!!*/
#include "device_coordinator.h"
#include "rr_blocks.h"

void dump_verilog_sdc_file_header(FILE* fp,
                                  char* usage);

void dump_verilog_one_sb_chan_pin(FILE* fp, 
                                  RRSwitchBlock& rr_sb,
                                  t_rr_node* cur_rr_node,
                                  enum PORTS port_type);

void dump_verilog_one_sb_chan_pin(FILE* fp, 
                                  t_sb* cur_sb_info,
                                  t_rr_node* cur_rr_node,
                                  enum PORTS port_type);

void dump_verilog_one_sb_routing_pin(FILE* fp,
                                     RRSwitchBlock& rr_sb,
                                     t_rr_node* cur_rr_node);

void dump_verilog_one_sb_routing_pin(FILE* fp,
                                     t_sb* cur_sb_info,
                                     t_rr_node* cur_rr_node);

t_cb* get_chan_rr_node_ending_cb(t_rr_node* src_rr_node, 
                                t_rr_node* end_rr_node);

DeviceCoordinator get_chan_node_ending_sb_coordinator(t_rr_node* src_rr_node, 
                                                      t_rr_node* end_rr_node);

t_sb* get_chan_rr_node_ending_sb(t_rr_node* src_rr_node, 
                                 t_rr_node* end_rr_node);

void restore_disable_timing_one_sb_output(FILE* fp, 
                                          RRSwitchBlock& rr_sb,
                                          t_rr_node* wire_rr_node);

void restore_disable_timing_one_sb_output(FILE* fp, 
                                          t_sb* cur_sb_info,
                                          t_rr_node* wire_rr_node);

void set_disable_timing_one_sb_output(FILE* fp, 
                                      RRSwitchBlock& rr_sb,
                                      t_rr_node* wire_rr_node);

void set_disable_timing_one_sb_output(FILE* fp, 
                                      t_sb* cur_sb_info,
                                      t_rr_node* wire_rr_node);

#endif
