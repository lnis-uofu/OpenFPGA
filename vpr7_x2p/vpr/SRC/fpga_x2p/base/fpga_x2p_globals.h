#ifndef FPGA_X2P_GLOBALS_H 
#define FPGA_X2P_GLOBALS_H 

#include "rr_blocks.h"

/* global parameters for FPGA-SPICE tool suites */

extern t_spice_model* fpga_spice_sram_model;
extern enum e_sram_orgz fpga_spice_sram_orgz_type;

/* Input and Output Pad spice model. should be set as global */
extern t_spice_model* fpga_spice_inpad_model;
extern t_spice_model* fpga_spice_outpad_model;
extern t_spice_model* fpga_spice_iopad_model;

/* Number of configuration bits of each switch block */
extern int** num_conf_bits_sb;
/* Number of configuration bits of each Connection Box CHANX */
extern int** num_conf_bits_cbx;
/* Number of configuration bits of each Connection Box CHANY */
extern int** num_conf_bits_cby;

/* Prefix of global input, output and inout of a I/O pad */
extern char* gio_input_prefix;
extern char* gio_output_prefix;
extern char* gio_inout_prefix;

extern int default_signal_init_value;
extern boolean run_parasitic_net_estimation;
extern boolean run_testbench_load_extraction;

/* Linked list for global ports */
extern t_llist* global_ports_head;

/* Linked list for verilog and spice syntax char */
extern t_llist* reserved_syntax_char_head;
 
extern char* renaming_report_postfix;
extern char* fpga_spice_bitstream_output_file_postfix;
extern char* fpga_spice_bitstream_logic_block_log_file_postfix;
extern char* fpga_spice_bitstream_routing_log_file_postfix;

extern DeviceRRChan device_rr_chan;

#endif
