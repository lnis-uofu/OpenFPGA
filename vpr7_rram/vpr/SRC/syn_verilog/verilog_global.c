/***********************************/
/*  Synthesizable Verilog Dumping  */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include "spice_types.h"
#include "linkedlist.h"
#include "fpga_spice_globals.h"
#include "verilog_global.h"

char* verilog_top_postfix = "_top.v";
char* bitstream_verilog_file_postfix = ".bitstream";
char* top_testbench_verilog_file_postfix = "_top_tb.v";
char* blif_testbench_verilog_file_postfix = "_blif_tb.v";
char* logic_block_verilog_file_name = "logic_blocks.v";
char* luts_verilog_file_name = "luts.v";
char* routing_verilog_file_name = "routing.v";
char* muxes_verilog_file_name = "muxes.v";
char* wires_verilog_file_name = "wires.v";
char* essentials_verilog_file_name = "inv_buf_passgate.v";
char* decoders_verilog_file_name = "decoders.v";

char* verilog_mux_basis_posfix = "_basis";
char* verilog_mux_special_basis_posfix = "_special_basis";

/* SRAM SPICE MODEL should be set as global*/
t_spice_model* sram_verilog_model = NULL;
enum e_sram_orgz sram_verilog_orgz_type = SPICE_SRAM_STANDALONE;
t_sram_orgz_info* sram_verilog_orgz_info = NULL;

/* Input and Output Pad spice model. should be set as global */
t_spice_model* iopad_verilog_model = NULL;

/* Linked-list that stores all the configuration bits */
t_llist* conf_bits_head = NULL;

int verilog_default_signal_init_value = 0;
