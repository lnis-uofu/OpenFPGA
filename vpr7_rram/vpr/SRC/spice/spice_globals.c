/***********************************/
/*      SPICE Modeling for VPR     */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
/* Include SPICE support headers*/
#include <stdio.h>
#include "spice_types.h"
#include "linkedlist.h"
#include "fpga_spice_globals.h"
#include "spice_globals.h"

/* Threshold of max transistor width for each transistor */
float max_width_per_trans = 5.;

char* nmos_subckt_name = "vpr_nmos";
char* pmos_subckt_name = "vpr_pmos";
char* io_nmos_subckt_name = "vpr_io_nmos";
char* io_pmos_subckt_name = "vpr_io_pmos";
char* cpt_subckt_name = "cpt";
char* mux_basis_posfix = "_basis";
char* mux_special_basis_posfix = "_special_basis";
char* nmos_pmos_spice_file_name = "nmos_pmos.sp";
char* basics_spice_file_name = "inv_buf_trans_gate.sp";
char* muxes_spice_file_name = "muxes.sp";
char* rram_veriloga_file_name = "rram_behavior.va";
char* wires_spice_file_name = "wires.sp";
char* logic_block_spice_file_name = "logic_blocks.sp";
char* luts_spice_file_name = "luts.sp";
char* routing_spice_file_name = "routing.sp";
char* meas_header_file_name = "meas_params.sp";
char* stimu_header_file_name = "stimulate_params.sp";
char* design_param_header_file_name = "design_params.sp";

/* Postfix for circuit design parameters */
char* design_param_postfix_input_buf_size = "_input_buf_size"; 
char* design_param_postfix_output_buf_size = "_output_buf_size"; 
char* design_param_postfix_pass_gate_logic_pmos_size = "_pgl_pmos_size"; 
char* design_param_postfix_pass_gate_logic_nmos_size = "_pgl_nmos_size"; 
char* design_param_postfix_wire_param_res_val = "_wire_param_res_val"; 
char* design_param_postfix_wire_param_cap_val = "_wire_param_cap_val"; 
char* design_param_postfix_rram_ron = "_rram_ron"; 
char* design_param_postfix_rram_roff = "_rram_roff"; 
char* design_param_postfix_rram_wprog_set_pmos = "_rram_wprog_set_pmos"; 
char* design_param_postfix_rram_wprog_set_nmos = "_rram_wprog_set_nmos"; 
char* design_param_postfix_rram_wprog_reset_pmos = "_rram_wprog_reset_pmos"; 
char* design_param_postfix_rram_wprog_reset_nmos = "_rram_wprog_reset_nmos"; 

/* Testbench names */
char* spice_top_testbench_postfix = "_top.sp";
char* spice_grid_testbench_postfix = "_grid_testbench.sp";
char* spice_pb_mux_testbench_postfix = "_pbmux_testbench.sp";
char* spice_cb_mux_testbench_postfix = "_cbmux_testbench.sp";
char* spice_sb_mux_testbench_postfix = "_sbmux_testbench.sp";
char* spice_cb_testbench_postfix = "_cb_testbench.sp";
char* spice_sb_testbench_postfix = "_sb_testbench.sp";
char* spice_lut_testbench_postfix = "_lut_testbench.sp";
char* spice_dff_testbench_postfix = "_dff_testbench.sp";
char* spice_hardlogic_testbench_postfix = "_hardlogic_testbench.sp";
char* spice_io_testbench_postfix = "_io_testbench.sp";
char* bitstream_spice_file_postfix = ".bitstream";

/* SRAM SPICE MODEL should be set as global*/
t_spice_model* sram_spice_model = NULL;
enum e_sram_orgz sram_spice_orgz_type = SPICE_SRAM_STANDALONE;
t_sram_orgz_info* sram_spice_orgz_info = NULL;

/* Input and Output Pad spice model. should be set as global */
t_spice_model* iopad_spice_model = NULL;

/* Global counters */
int rram_design_tech = 0;
int num_used_grid_mux_tb = 0;
int num_used_grid_tb = 0;
int num_used_cb_tb = 0;
int num_used_sb_tb = 0;
int num_used_cb_mux_tb = 0;
int num_used_sb_mux_tb = 0;
int num_used_lut_tb = 0;
int num_used_dff_tb = 0;
int num_used_hardlogic_tb = 0;
int num_used_io_tb = 0;

/* linked-list for all the testbenches */
t_llist* tb_head = NULL;
/* linked-list for heads of scan-chain */
t_llist* scan_chain_heads = NULL;

/* Name of global ports used in all netlists */
char* spice_tb_global_vdd_port_name = "gvdd";
char* spice_tb_global_gnd_port_name = "ggnd";
char* spice_tb_global_config_done_port_name = "gconfig_done";
char* spice_tb_global_set_port_name = "gset";
char* spice_tb_global_reset_port_name = "greset";
char* spice_tb_global_vdd_localrouting_port_name = "gvdd_local_interc";
char* spice_tb_global_vdd_io_port_name = "gvdd_io";
char* spice_tb_global_vdd_hardlogic_port_name = "gvdd_hardlogic";
char* spice_tb_global_vdd_sram_port_name = "gvdd_sram";
char* spice_tb_global_vdd_lut_sram_port_name = "gvdd_sram_luts";
char* spice_tb_global_vdd_localrouting_sram_port_name = "gvdd_sram_local_routing";
char* spice_tb_global_vdd_io_sram_port_name = "gvdd_sram_io";
char* spice_tb_global_vdd_hardlogic_sram_port_name = "gvdd_sram_hardlogic";
char* spice_tb_global_vdd_cb_sram_port_name = "gvdd_sram_cbs";
char* spice_tb_global_vdd_sb_sram_port_name = "gvdd_sram_sbs";
char* spice_tb_global_clock_port_name = "gclock";
char* spice_tb_global_vdd_load_port_name = "gvdd_load";
char* spice_tb_global_port_inv_postfix = "_inv";

int spice_sim_multi_thread_num = 8;

