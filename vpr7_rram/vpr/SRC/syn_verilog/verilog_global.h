/* global parameters for dumping synthesizable verilog */
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

extern t_spice_model* sram_verilog_model;
extern enum e_sram_orgz sram_verilog_orgz_type;
extern t_sram_orgz_info* sram_verilog_orgz_info;

/* Input and Output Pad spice model. should be set as global */
extern t_spice_model* inpad_verilog_model;
extern t_spice_model* outpad_verilog_model;
extern t_spice_model* iopad_verilog_model;

/* Linked-list that stores all the configuration bits */
extern t_llist* conf_bits_head;

extern int verilog_default_signal_init_value;

enum e_dump_verilog_port_type {
VERILOG_PORT_INPUT,
VERILOG_PORT_OUTPUT,
VERILOG_PORT_INOUT,
VERILOG_PORT_WIRE,
VERILOG_PORT_REG,
VERILOG_PORT_CONKT
};
