/***********************************/
/*  Synthesizable Verilog Dumping  */
/*       Xifan TANG, EPFL/LSI      */
/***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>

/* Include vpr structs*/
#include "util.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "globals.h"
#include "rr_graph.h"
#include "vpr_utils.h"
#include "path_delay.h"
#include "stats.h"

/* Include FPGA-SPICE utils */
#include "read_xml_spice_util.h"
#include "linkedlist.h"
#include "fpga_spice_utils.h"
#include "fpga_spice_backannotate_utils.h"
#include "fpga_spice_globals.h"
#include "fpga_spice_bitstream.h"
#include "verilog_modelsim_autodeck.h"

/* Include SynVerilog headers */
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_submodules.h"
#include "verilog_decoder.h"
#include "verilog_pbtypes.h"
#include "verilog_routing.h"
#include "verilog_top_netlist.h"
#include "verilog_autocheck_tb.h"


/***** Subroutines *****/
/* Alloc array that records Configuration bits for :
 * (1) Switch blocks
 * (2) Connection boxes
 * TODO: Can be improved in alloc strategy to be more memory efficient!
 */
static 
void alloc_global_routing_conf_bits() {
  int i;
  
  /* Alloc array for Switch blocks */
  num_conf_bits_sb = (int**)my_malloc((nx+1)*sizeof(int*));
  for (i = 0; i < (nx + 1); i++) {
    num_conf_bits_sb[i] = (int*)my_calloc((ny+1), sizeof(int));
  }

  /* Alloc array for Connection blocks */
  num_conf_bits_cbx = (int**)my_malloc((nx+1)*sizeof(int*));
  for (i = 0; i < (nx + 1); i++) {
    num_conf_bits_cbx[i] = (int*)my_calloc((ny+1), sizeof(int));
  }

  num_conf_bits_cby = (int**)my_malloc((nx+1)*sizeof(int*));
  for (i = 0; i < (nx + 1); i++) {
    num_conf_bits_cby[i] = (int*)my_calloc((ny+1), sizeof(int));
  }

  return;
}

static 
void free_global_routing_conf_bits() {
  int i;

  /* Free array for Switch blocks */
  for (i = 0; i < (nx + 1); i++) {
    my_free(num_conf_bits_sb[i]);
  }
  my_free(num_conf_bits_sb);
  
  /* Free array for Connection box */
  for (i = 0; i < (nx + 1); i++) {
    my_free(num_conf_bits_cbx[i]);
  }
  my_free(num_conf_bits_cbx);

  for (i = 0; i < (nx + 1); i++) {
    my_free(num_conf_bits_cby[i]);
  }
  my_free(num_conf_bits_cby);

  return;
}
 
/* Top-level function*/
void vpr_dump_syn_verilog(t_vpr_setup vpr_setup,
                          t_arch Arch,
                          char* circuit_name) {
  /* Timer */
  clock_t t_start;
  clock_t t_end;
  float run_time_sec;

  int num_clocks = Arch.spice->spice_params.stimulate_params.num_clocks;
  /* int vpr_crit_path_delay = Arch.spice->spice_params.stimulate_params.vpr_crit_path_delay; */
  
  /* Directory paths */
  char* verilog_dir_formatted = NULL;
  char* submodule_dir_path= NULL;
  char* lb_dir_path = NULL;
  char* rr_dir_path = NULL;
  char* top_netlist_file = NULL;
  char* top_netlist_path = NULL;
  char* bitstream_file_name = NULL;
  char* bitstream_file_path = NULL;
  char* hex_file_name = NULL;
  char* hex_file_path = NULL;
  char* top_testbench_file_name = NULL;
  char* top_auto_testbench_file_name = NULL;
  char* top_auto_preconf_testbench_file_name = NULL;
  char* top_testbench_file_path = NULL;
  char* top_auto_testbench_file_path = NULL;
  char* top_auto_preconf_testbench_file_path = NULL;
  char* blif_testbench_file_name = NULL;
  char* blif_testbench_file_path = NULL;

  char* chomped_parent_dir = NULL;
  char* chomped_circuit_name = NULL;

  boolean tb_preconf = TRUE;

  /* Check if the routing architecture we support*/
  if (UNI_DIRECTIONAL != vpr_setup.RoutingArch.directionality) {
    vpr_printf(TIO_MESSAGE_ERROR, "FPGA synthesizable Verilog dumping only support uni-directional routing architecture!\n");
    exit(1);
  }
  
  /* We don't support mrFPGA */
#ifdef MRFPGA_H
  if (is_mrFPGA) {
    vpr_printf(TIO_MESSAGE_ERROR, "FPGA synthesizable verilog dumping do not support mrFPGA!\n");
    exit(1);
  }
#endif
  
  assert ( TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.dump_syn_verilog);

  /* VerilogGenerator formally starts*/
  vpr_printf(TIO_MESSAGE_INFO, "\nFPGA synthesizable verilog generator starts...\n");
 
  /* Start time count */
  t_start = clock();

  /* Format the directory paths */
  split_path_prog_name(circuit_name, '/', &chomped_parent_dir, &chomped_circuit_name);

  if (NULL != vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.syn_verilog_dump_dir) {
    verilog_dir_formatted = format_dir_path(vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.syn_verilog_dump_dir);
  } else { 
    verilog_dir_formatted = format_dir_path(my_strcat(format_dir_path(chomped_parent_dir),default_verilog_dir_name));
  }
  /* lb directory */
  (lb_dir_path) = my_strcat(verilog_dir_formatted, default_lb_dir_name);
  /* routing resources directory */
  (rr_dir_path) = my_strcat(verilog_dir_formatted, default_rr_dir_name);
  /* submodule_dir_path */
  (submodule_dir_path) = my_strcat(verilog_dir_formatted, default_submodule_dir_name);
  /* Top netlists dir_path */
  top_netlist_file = my_strcat(chomped_circuit_name, verilog_top_postfix);
  top_netlist_path = my_strcat(verilog_dir_formatted, top_netlist_file);
  bitstream_file_name = my_strcat(chomped_circuit_name, bitstream_verilog_file_postfix);
  bitstream_file_path = my_strcat(verilog_dir_formatted, bitstream_file_name);
  hex_file_name = my_strcat(chomped_circuit_name, hex_verilog_file_postfix);
  hex_file_path = my_strcat(verilog_dir_formatted, hex_file_name);
  top_testbench_file_name = my_strcat(chomped_circuit_name, top_testbench_verilog_file_postfix);
  top_testbench_file_path = my_strcat(verilog_dir_formatted, top_testbench_file_name);
  top_auto_testbench_file_name = my_strcat(chomped_circuit_name, top_auto_testbench_verilog_file_postfix);
  top_auto_testbench_file_path = my_strcat(verilog_dir_formatted, top_auto_testbench_file_name);
  top_auto_preconf_testbench_file_name = my_strcat(chomped_circuit_name, top_auto_preconf_testbench_verilog_file_postfix);
  top_auto_preconf_testbench_file_path = my_strcat(verilog_dir_formatted, top_auto_preconf_testbench_file_name);
  blif_testbench_file_name = my_strcat(chomped_circuit_name, blif_testbench_verilog_file_postfix);
  blif_testbench_file_path = my_strcat(verilog_dir_formatted, blif_testbench_file_name);
  
  /* Create directories */
  create_dir_path(verilog_dir_formatted);
  create_dir_path(lb_dir_path);
  create_dir_path(rr_dir_path);
  create_dir_path(submodule_dir_path);

  /* assign the global variable of SRAM model */
  assert(NULL != Arch.sram_inf.verilog_sram_inf_orgz); /* Check !*/
  sram_verilog_model = Arch.sram_inf.verilog_sram_inf_orgz->spice_model;
  sram_verilog_orgz_type = Arch.sram_inf.verilog_sram_inf_orgz->type;
  /* initialize the SRAM organization information struct */
  sram_verilog_orgz_info = alloc_one_sram_orgz_info();
  init_sram_orgz_info(sram_verilog_orgz_info, sram_verilog_orgz_type, sram_verilog_model, nx + 2, ny + 2);
  /* Check all the SRAM port is using the correct SRAM SPICE MODEL */
  config_spice_models_sram_port_spice_model(Arch.spice->num_spice_model, 
                                            Arch.spice->spice_models,
                                            Arch.sram_inf.verilog_sram_inf_orgz->spice_model);

  /* Assign global variables of input and output pads */
  iopad_verilog_model = find_iopad_spice_model(Arch.spice->num_spice_model, Arch.spice->spice_models);
  assert(NULL != iopad_verilog_model);

  /* zero the counter of each spice_model */
  zero_spice_models_cnt(Arch.spice->num_spice_model, Arch.spice->spice_models);

  /* Initialize the user-defined verilog netlists to be included */
  init_list_include_verilog_netlists(Arch.spice);

  /* Initial global variables about configuration bits */
  alloc_global_routing_conf_bits();

  /* Initialize the number of configuration bits of all the grids */
  vpr_printf(TIO_MESSAGE_INFO, "Count the number of configuration bits, IO pads in each logic block...\n");
  /* init_grids_num_conf_bits(sram_verilog_orgz_type); */
  init_grids_num_conf_bits(sram_verilog_orgz_info);
  init_grids_num_iopads();
  /* init_grids_num_mode_bits(); */

  /* Dump routing resources: switch blocks, connection blocks and channel tracks */
  dump_verilog_routing_resources(rr_dir_path, Arch, &vpr_setup.RoutingArch,
                                 num_rr_nodes, rr_node, rr_node_indices);

  /* Dump logic blocks */
  dump_verilog_logic_blocks(lb_dir_path, &Arch);

  /* Dump decoder modules only when memory bank is required */
  switch(sram_verilog_orgz_type) {
  case SPICE_SRAM_STANDALONE:
  case SPICE_SRAM_SCAN_CHAIN:
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Dump verilog decoder */
    dump_verilog_decoder(submodule_dir_path);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,"(File:%s,[LINE%d])Invalid type of SRAM organization in Verilog Generator!\n",
               __FILE__, __LINE__);
    exit(1);
  }
 
  /* Dump internal structures of submodules */
  dump_verilog_submodules(submodule_dir_path, Arch, &vpr_setup.RoutingArch, vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.include_timing, vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.init_sim);

  /* Dump top-level verilog */
  dump_verilog_top_netlist(chomped_circuit_name, top_netlist_path, lb_dir_path, rr_dir_path, 
                           num_rr_nodes, rr_node, rr_node_indices, num_clocks, *(Arch.spice));

  /* Dump SDC constraints */
  // dump_verilog_sdc_file();
  
  /* dump verilog testbench only for top-level */
  if (( TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_top_tb) || ( TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_top_auto_tb)){ 
    if ( TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_top_tb){
		dump_verilog_top_testbench(chomped_circuit_name, top_testbench_file_path, num_clocks, 
                               vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts, *(Arch.spice));
	}
	// AA: to generate autocheck testbench but only one bitstream
	if ( TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_top_auto_tb) { 				
	    dump_verilog_top_auto_testbench(chomped_circuit_name, top_auto_testbench_file_path, num_clocks, 
                               			vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts, *(Arch.spice),
										modelsim_auto_testbench_module_postfix);
	}
	// AA: to generate autocheck preconfigured testbench 
	if (( TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_top_auto_tb) && (tb_preconf)) { 
	    dump_verilog_top_auto_preconf_testbench(chomped_circuit_name, 
												top_auto_preconf_testbench_file_path,
												num_clocks, 
                               					vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts, 
												*(Arch.spice),
												modelsim_auto_preconf_testbench_module_postfix,
												hex_file_path);
		dump_fpga_spice_hex(hex_file_path, chomped_circuit_name, sram_verilog_orgz_info);
	}
    /* Dump bitstream file */
    dump_fpga_spice_bitstream(bitstream_file_path, chomped_circuit_name, sram_verilog_orgz_info);
  }


  /* Output Modelsim Autodeck scripts */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_modelsim_autodeck) {
    dump_verilog_modelsim_autodeck(sram_verilog_orgz_info, *(Arch.spice),
                                   Arch.spice->spice_params.meas_params.sim_num_clock_cycle,
                                   verilog_dir_formatted, chomped_circuit_name,
                                   vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.modelsim_ini_path,
                                   vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.include_timing,
                                   vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.init_sim,
								   vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_top_tb,
								   vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_top_auto_tb,
								   tb_preconf);
  }

  /* dump verilog testbench only for input blif */
  if ( TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_input_blif_tb) { 
    dump_verilog_input_blif_testbench(chomped_circuit_name, blif_testbench_file_path, num_clocks, 
                                      vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts, *(Arch.spice));
  }  

  /* End time count */
  t_end = clock();
 
  run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, "Synthesizable verilog dumping took %g seconds\n", run_time_sec);  

  /* Free global array */
  free_global_routing_conf_bits();

  /* Free sram_orgz_info */
  free_sram_orgz_info(sram_verilog_orgz_info,
                      sram_verilog_orgz_info->type,
                      nx + 2, ny + 2);
  /* Free */
  my_free(verilog_dir_formatted);
  my_free(lb_dir_path);
  my_free(rr_dir_path);
  my_free(top_netlist_file);
  my_free(top_netlist_path);
  my_free(submodule_dir_path);

  return;
}
