/***********************************/
/*      SPICE Modeling for VPR     */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
/* Include SPICE support headers*/
#include <stdio.h>
#include "spice_types.h"
#include "linkedlist.h"
#include "spice_globals.h"

char* nmos_subckt_name = "vpr_nmos";
char* pmos_subckt_name = "vpr_pmos";
char* cpt_subckt_name = "cpt";
char* mux_basis_posfix = "_basis";
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

/* Testbench names */
char* spice_top_testbench_postfix = "_top.sp";
char* spice_grid_testbench_postfix = "_grid_testbench.sp";
char* spice_pb_mux_testbench_postfix = "_pbmux_testbench.sp";
char* spice_cb_mux_testbench_postfix = "_cbmux_testbench.sp";
char* spice_sb_mux_testbench_postfix = "_sbmux_testbench.sp";
char* spice_lut_testbench_postfix = "_lut_testbench.sp";
char* spice_dff_testbench_postfix = "_dff_testbench.sp";

/* SRAM SPICE MODEL should be set as global*/
t_spice_model* sram_spice_model = NULL;
/* Global counters */
int rram_design_tech = 0;
int num_used_grid_tb = 0;
int num_used_cb_tb = 0;
int num_used_sb_tb = 0;
int num_used_lut_tb = 0;
int num_used_dff_tb = 0;
/* Default init_value */
int default_signal_init_value = 0;
/* Default do parasitic net estimation !!!*/
int run_parasitic_net_estimation = 1;

t_llist* tb_head = NULL;

