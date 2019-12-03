/***********************************/
/*  Synthesizable Verilog Dumping  */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include "spice_types.h"
#include "linkedlist.h"
#include "fpga_x2p_globals.h"
#include "verilog_global.h"

char* verilog_netlist_file_postfix = ".v";
float verilog_sim_timescale = 1e-9; // Verilog Simulation time scale (minimum time unit) : 1ns
char* verilog_timing_preproc_flag = "ENABLE_TIMING"; // the flag to enable timing definition during compilation
char* verilog_signal_init_preproc_flag = "ENABLE_SIGNAL_INITIALIZATION"; // the flag to enable signal initialization during compilation
char* verilog_formal_verification_preproc_flag = "ENABLE_FORMAL_VERIFICATION"; // the flag to enable formal verification during compilation
char* initial_simulation_flag = "INITIAL_SIMULATION"; // the flag to enable initial functional verification
char* autochecked_simulation_flag = "AUTOCHECKED_SIMULATION"; // the flag to enable autochecked functional verification
char* formal_simulation_flag = "FORMAL_SIMULATION"; // the flag to enable formal functional verification

char* default_verilog_dir_name = "syn_verilogs/";
char* default_src_dir_name = "SRC/";
char* default_lb_dir_name = "lb/";
char* default_rr_dir_name = "routing/";
char* default_submodule_dir_name = "sub_module/";
char* default_tcl_dir_name = "SCRIPTS/";
char* default_sdc_dir_name = "SDC/";
char* default_msim_dir_name = "MSIM/";
char* default_snpsfm_dir_name = "SNPS_FM/";
char* default_modelsim_dir_name = "msim_projects/";
char* default_report_timing_rpt_dir_name = "RPT/";
char* autocheck_testbench_postfix = "_autocheck";

char* modelsim_project_name_postfix = "_fpga_msim";
char* modelsim_proc_script_name_postfix = "_proc.tcl";
char* modelsim_top_script_name_postfix = "_runsim.tcl";
char* modelsim_testbench_module_postfix = "_top_tb";
char* modelsim_autocheck_testbench_module_postfix = "_autocheck_top_tb";
char* modelsim_simulation_time_unit = "ms";

char* formal_verification_top_module_postfix = "_top_formal_verification";
char* formal_verification_top_module_port_postfix = "_fm";
char* formal_verification_top_module_uut_name = "U0_formal_verification";

// Formality script generation variables
char* formality_script_name_postfix = "_formality_script.tcl";
char* formal_verification_top_postfix = "_top_formal_verification";
// End of Formality script generation variables

// Icarus variables and flag
char* icarus_simulator_flag = "ICARUS_SIMULATOR"; // the flag to enable specific Verilog code in testbenches
// End of Icarus variables and flag

char* verilog_top_postfix = "_top.v";
char* formal_verification_verilog_file_postfix = "_top_formal_verification.v"; 
char* top_testbench_verilog_file_postfix = "_top_tb.v"; /* !!! must be consist with the modelsim_testbench_module_postfix */ 
char* autocheck_top_testbench_verilog_file_postfix = "_autocheck_top_tb.v"; /* !!! must be consist with the modelsim_autocheck_testbench_module_postfix */ 
char* random_top_testbench_verilog_file_postfix = "_formal_random_top_tb.v"; 
char* blif_testbench_verilog_file_postfix = "_blif_tb.v";
char* defines_verilog_file_name = "fpga_defines.v";
char* defines_verilog_simulation_file_name = "define_simulation.v";
char* submodule_verilog_file_name = "sub_module.v";
char* logic_block_verilog_file_name = "logic_blocks.v";
char* luts_verilog_file_name = "luts.v";
char* routing_verilog_file_name = "routing.v";
char* muxes_verilog_file_name = "muxes.v";
char* local_encoder_verilog_file_name = "local_encoder.v";
char* memories_verilog_file_name = "memories.v";
char* wires_verilog_file_name = "wires.v";
char* essentials_verilog_file_name = "inv_buf_passgate.v";
char* config_peripheral_verilog_file_name = "config_peripherals.v";
char* user_defined_template_verilog_file_name = "user_defined_templates.v";

/* File names for Report Timing */
char* trpt_sb_file_name = "report_timing_sb.tcl";
char* trpt_routing_file_name = "report_timing_routing.tcl";

/* File names for SDC*/
char* sdc_analysis_file_name = "fpga_top_analysis.sdc";
char* sdc_break_loop_file_name = "break_loop.sdc";
char* sdc_constrain_routing_chan_file_name = "routing_channels.sdc";
char* sdc_constrain_cb_file_name = "cb.sdc";
char* sdc_constrain_sb_file_name = "sb.sdc";
char* sdc_clock_period_file_name = "clb_clock.sdc";
char* sdc_constrain_pb_type_file_name = "clb_constraints.sdc";
char* sdc_break_vertical_sbs_file_name = "break_horizontal_sbs.sdc"; /* We break the vertical to read the horizontal */
char* sdc_break_horizontal_sbs_file_name = "break_vertical_sbs.sdc"; /* We break the horizontal to read the vertical */ 
char* sdc_restore_vertical_sbs_file_name = "restore_horizontal_sbs.sdc"; /* We break the vertical to read the horizontal */
char* sdc_restore_horizontal_sbs_file_name = "restore_vertical_sbs.sdc"; /* We break the horizontal to read the vertical */ 

char* verilog_mux_basis_posfix = "_basis";
char* verilog_mux_special_basis_posfix = "_special_basis";
char* verilog_mem_posfix = "_mem";
char* verilog_config_peripheral_prefix = "config_peripheral";

/* Prefix for subckt Verilog netlists */
char* grid_verilog_file_name_prefix = "grid_";
char* chanx_verilog_file_name_prefix = "chanx_";
char* chany_verilog_file_name_prefix = "chany_";
char* sb_verilog_file_name_prefix = "sb_";
char* cbx_verilog_file_name_prefix = "cbx_";
char* cby_verilog_file_name_prefix = "cby_";

/* SRAM SPICE MODEL should be set as global*/
t_spice_model* sram_verilog_model = NULL;

/* Input and Output Pad spice model. should be set as global */
t_spice_model* iopad_verilog_model = NULL;

/* Linked-list that stores all the configuration bits */
t_llist* conf_bits_head = NULL;

/* Linked-list that stores submodule Verilog file mames */
t_llist* grid_verilog_subckt_file_path_head  = NULL;
t_llist* routing_verilog_subckt_file_path_head = NULL;
t_llist* submodule_verilog_subckt_file_path_head = NULL;

int verilog_default_signal_init_value = 0;

char* top_netlist_bl_enable_port_name = "en_bl";
char* top_netlist_wl_enable_port_name = "en_wl";
char* top_netlist_bl_data_in_port_name = "data_in";
char* top_netlist_addr_bl_port_name = "addr_bl";
char* top_netlist_addr_wl_port_name = "addr_wl";
char* top_netlist_array_bl_port_name = "bl_bus";
char* top_netlist_array_wl_port_name = "wl_bus";
char* top_netlist_array_blb_port_name = "blb_bus";
char* top_netlist_array_wlb_port_name = "wlb_bus";
char* top_netlist_reserved_bl_port_postfix = "_reserved_bl";
char* top_netlist_reserved_wl_port_postfix = "_reserved_wl";
char* top_netlist_normal_bl_port_postfix = "_bl";
char* top_netlist_normal_wl_port_postfix = "_wl";
char* top_netlist_normal_blb_port_postfix = "_blb";
char* top_netlist_normal_wlb_port_postfix = "_wlb";
char* top_netlist_scan_chain_head_prefix = "cc_in";

char* top_tb_reset_port_name = "greset";
char* top_tb_set_port_name = "gset";
char* top_tb_prog_reset_port_name = "prog_reset";
char* top_tb_prog_set_port_name = "prog_set";
char* top_tb_config_done_port_name = "config_done";
char* top_tb_op_clock_port_name = "op_clock";
char* top_tb_prog_clock_port_name = "prog_clock";
char* top_tb_inout_reg_postfix = "_reg";
char* top_tb_clock_reg_postfix = "_reg";

