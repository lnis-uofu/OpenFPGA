/* global parameters for dumping synthesizable verilog */

extern char* verilog_netlist_file_postfix;
extern float verilog_sim_timescale;
extern char* verilog_timing_preproc_flag; // the flag to enable timing definition during compilation
extern char* verilog_init_sim_preproc_flag; // the flag to enable initialization during simulation

extern char* verilog_top_postfix;
extern char* bitstream_verilog_file_postfix;
extern char* top_testbench_verilog_file_postfix;
extern char* blif_testbench_verilog_file_postfix;
extern char* logic_block_verilog_file_name;
extern char* luts_verilog_file_name;
extern char* routing_verilog_file_name;
extern char* muxes_verilog_file_name;
extern char* wires_verilog_file_name;
extern char* essentials_verilog_file_name;
extern char* decoders_verilog_file_name;
extern char* verilog_mux_basis_posfix;
extern char* verilog_mux_special_basis_posfix;

/* Prefix for subckt Verilog netlists */
extern char* grid_verilog_file_name_prefix;
extern char* chanx_verilog_file_name_prefix;
extern char* chany_verilog_file_name_prefix;
extern char* sb_verilog_file_name_prefix;
extern char* cbx_verilog_file_name_prefix;
extern char* cby_verilog_file_name_prefix;

extern t_spice_model* sram_verilog_model;
extern enum e_sram_orgz sram_verilog_orgz_type;
extern t_sram_orgz_info* sram_verilog_orgz_info;

/* Input and Output Pad spice model. should be set as global */
extern t_spice_model* inpad_verilog_model;
extern t_spice_model* outpad_verilog_model;
extern t_spice_model* iopad_verilog_model;

/* Linked-list that stores all the configuration bits */
extern t_llist* conf_bits_head;

/* Linked-list that stores submodule Verilog file mames */
extern t_llist* grid_verilog_subckt_file_path_head;
extern t_llist* routing_verilog_subckt_file_path_head;

extern int verilog_default_signal_init_value;

enum e_dump_verilog_port_type {
VERILOG_PORT_INPUT,
VERILOG_PORT_OUTPUT,
VERILOG_PORT_INOUT,
VERILOG_PORT_WIRE,
VERILOG_PORT_REG,
VERILOG_PORT_CONKT
};
