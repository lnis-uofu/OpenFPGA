/***********************************/
/*  Synthesizable Verilog Dumping  */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include "spice_types.h"
#include "linkedlist.h"
#include "fpga_spice_globals.h"
#include "verilog_global.h"

char* verilog_netlist_file_postfix = ".v";
float verilog_sim_timescale = 1e-9; // Verilog Simulation time scale (minimum time unit) : 1ns
char* verilog_timing_preproc_flag = "ENABLE_TIMING"; // the flag to enable timing definition during compilation
char* verilog_init_sim_preproc_flag = "INITIALIZATION"; // the flag to enable initialization during simulation

char* default_verilog_dir_name = "syn_verilogs/";
char* default_lb_dir_name = "lb/";
char* default_rr_dir_name = "routing/";
char* default_submodule_dir_name = "sub_module/";
char* default_modelsim_dir_name = "msim_projects/";

char* modelsim_project_name_postfix = "_fpga_msim";
char* modelsim_proc_script_name_postfix = "_proc.tcl";
char* modelsim_top_script_name_postfix = "_runsim.tcl";
char* modelsim_testbench_module_postfix = "_top_tb";
char* modelsim_auto_testbench_module_postfix = "_top_auto_tb";
char* modelsim_auto_preconf_testbench_module_postfix = "_top_auto_preconf_tb";
char* modelsim_simulation_time_unit = "ns";

char* verilog_top_postfix = "_top.v";
char* bitstream_verilog_file_postfix = ".bitstream";
char* hex_verilog_file_postfix = ".hex";
char* top_testbench_verilog_file_postfix = "_top_tb.v";
char* top_auto_testbench_verilog_file_postfix = "_top_auto_tb.v";
char* top_auto_preconf_testbench_verilog_file_postfix = "_top_auto_preconf_tb.v";
char* blif_testbench_verilog_file_postfix = "_blif_tb.v";
char* submodule_verilog_file_name = "sub_module.v";
char* logic_block_verilog_file_name = "logic_blocks.v";
char* luts_verilog_file_name = "luts.v";
char* routing_verilog_file_name = "routing.v";
char* sub_module_verilog_file_name = "sub_module.v";
char* muxes_verilog_file_name = "muxes.v";
char* wires_verilog_file_name = "wires.v";
char* essentials_verilog_file_name = "inv_buf_passgate.v";
char* decoders_verilog_file_name = "decoders.v";

char* verilog_mux_basis_posfix = "_basis";
char* verilog_mux_special_basis_posfix = "_special_basis";

/* Prefix for subckt Verilog netlists */
char* grid_verilog_file_name_prefix = "grid_";
char* chanx_verilog_file_name_prefix = "chanx_";
char* chany_verilog_file_name_prefix = "chany_";
char* sb_verilog_file_name_prefix = "sb_";
char* cbx_verilog_file_name_prefix = "cbx_";
char* cby_verilog_file_name_prefix = "cby_";

/* SRAM SPICE MODEL should be set as global*/
t_spice_model* sram_verilog_model = NULL;
enum e_sram_orgz sram_verilog_orgz_type = SPICE_SRAM_STANDALONE;
t_sram_orgz_info* sram_verilog_orgz_info = NULL;

/* Input and Output Pad spice model. should be set as global */
t_spice_model* iopad_verilog_model = NULL;

/* Linked-list that stores all the configuration bits */
t_llist* conf_bits_head = NULL;

/* Linked-list that stores submodule Verilog file mames */
t_llist* grid_verilog_subckt_file_path_head  = NULL;
t_llist* routing_verilog_subckt_file_path_head = NULL;
t_llist* submodule_verilog_subckt_file_path_head = NULL;


int verilog_default_signal_init_value = 0;
