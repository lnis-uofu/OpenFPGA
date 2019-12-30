/********************************************************************
 * This file include top-level function of FPGA-Verilog
 ********************************************************************/
/* Standard header files */
#include <ctime>

/* External library header files */
#include "util.h"
#include "vtr_assert.h"
#include "circuit_library_utils.h"

/* FPGA-X2P header files */
#include "fpga_x2p_utils.h"
#include "rr_blocks.h"

/* FPGA-Verilog header files */
#include "verilog_global.h"
#include "verilog_submodules.h"
#include "verilog_routing.h"
#include "verilog_submodules.h"
#include "verilog_grid.h"
#include "verilog_routing.h"
#include "verilog_top_module.h"
#include "verilog_top_testbench.h"
#include "verilog_formal_random_top_testbench.h"
#include "verilog_preconfig_top_module.h"
#include "simulation_info_writer.h"
#include "verilog_auxiliary_netlists.h"

/* Header file for this source file */
#include "verilog_api.h"

/********************************************************************
 * Top-level function of FPGA-Verilog
 * This function will generate
 * 1. primitive modules required by the full fabric
 *    which are LUTs, routing multiplexer, logic gates, transmission-gates etc.
 * 2. Routing modules, which are Switch Blocks (SBs) and Connection Blocks (CBs)
 * 3. Logic block modules, which are Configuration Logic Blocks (CLBs)
 * 4. FPGA module, which are the full FPGA fabric with configuration protocol
 * 5. A wrapper module, which encapsulate the FPGA module in a Verilog module which have the same port as the input benchmark 
 * 6. Testbench, where a FPGA module is configured with a bitstream and then driven by input vectors
 * 7. Pre-configured testbench, which can skip the configuration phase and pre-configure the FPGA module. This testbench is created for quick verification and formal verification purpose.
 * 8. Verilog netlist including preprocessing flags and all the Verilog netlists that have been generated
 ********************************************************************/
void vpr_fpga_verilog(ModuleManager& module_manager,
                      const BitstreamManager& bitstream_manager,
                      const std::vector<ConfigBitId>& fabric_bitstream,
                      const MuxLibrary& mux_lib,
                      const std::vector<t_logical_block>& L_logical_blocks,
                      const vtr::Point<size_t>& device_size,
                      const std::vector<std::vector<t_grid_tile>>& L_grids, 
                      const std::vector<t_block>& L_blocks,
                      const DeviceRRGSB& L_device_rr_gsb,
                      const t_vpr_setup& vpr_setup,
                      const t_arch& Arch,
                      const std::string& circuit_name,
                      t_sram_orgz_info* sram_verilog_orgz_info) {
  /* Start time count */
  clock_t t_start = clock();

  /* 0. basic units: inverter, buffers and pass-gate logics, */
  /* Check if the routing architecture we support*/
  if (UNI_DIRECTIONAL != vpr_setup.RoutingArch.directionality) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "FPGA-Verilog only supports uni-directional routing architecture!\n");
    exit(1);
  }
  
  /* We don't support mrFPGA */
#ifdef MRFPGA_H
  if (is_mrFPGA) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "FPGA-Verilog does not support mrFPGA!\n");
    exit(1);
  }
#endif
  
  /* Verilog generator formally starts*/
  vpr_printf(TIO_MESSAGE_INFO, 
             "\nFPGA-Verilog starts...\n");

  /* Format the directory paths */
  std::string chomped_parent_dir = find_path_dir_name(circuit_name);
  std::string chomped_circuit_name = find_path_file_name(circuit_name);

  std::string verilog_dir_formatted;
  if (NULL != vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.syn_verilog_dump_dir) {
    verilog_dir_formatted = format_dir_path(std::string(vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.syn_verilog_dump_dir));
  } else { 
    verilog_dir_formatted = format_dir_path(format_dir_path(chomped_parent_dir) + std::string(default_verilog_dir_name));
  }

  /* Create directories */
  create_dir_path(verilog_dir_formatted.c_str());

  /* SRC directory to contain all the netlists */
  std::string src_dir_path = format_dir_path(verilog_dir_formatted + std::string(default_src_dir_name)); 
  create_dir_path(src_dir_path.c_str());

  /* Sub directory under SRC directory to contain all the primitive block netlists */
  std::string submodule_dir_path = src_dir_path + std::string(default_submodule_dir_name);
  create_dir_path(submodule_dir_path.c_str());

  /* Sub directory under SRC directory to contain all the logic block netlists */
  std::string lb_dir_path = src_dir_path + std::string(default_lb_dir_name);
  create_dir_path(lb_dir_path.c_str());

  /* Sub directory under SRC directory to contain all the routing block netlists */
  std::string rr_dir_path = src_dir_path + std::string(default_rr_dir_name);
  create_dir_path(rr_dir_path.c_str());

  /* Ensure all the SRAM port is using the correct circuit model */
  config_circuit_models_sram_port_to_default_sram_model(Arch.spice->circuit_lib, Arch.sram_inf.verilog_sram_inf_orgz->circuit_model); 

  /* Print Verilog files containing preprocessing flags */
  print_verilog_preprocessing_flags_netlist(std::string(src_dir_path),
                                            vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts);

  print_verilog_simulation_preprocessing_flags(std::string(src_dir_path),
                                               vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts);

  /* Generate primitive Verilog modules, which are corner stones of FPGA fabric
   * Note that this function MUST be called before Verilog generation of
   * core logic (i.e., logic blocks and routing resources) !!!
   * This is because that this function will add the primitive Verilog modules to 
   * the module manager.
   * Without the modules in the module manager, core logic generation is not possible!!!
   */
  print_verilog_submodules(module_manager, mux_lib, sram_verilog_orgz_info, src_dir_path.c_str(), submodule_dir_path.c_str(), 
                           Arch, vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts);

  /* Generate routing blocks */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.compact_routing_hierarchy) {
    print_verilog_unique_routing_modules(module_manager, L_device_rr_gsb,  
                                         vpr_setup.RoutingArch,
                                         src_dir_path, rr_dir_path,
                                         TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.dump_explicit_verilog);
  } else {
    VTR_ASSERT(FALSE == vpr_setup.FPGA_SPICE_Opts.compact_routing_hierarchy);
    print_verilog_flatten_routing_modules(module_manager, L_device_rr_gsb, 
                                          vpr_setup.RoutingArch,
                                          src_dir_path, rr_dir_path,
                                          TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.dump_explicit_verilog);
  }

  /* Generate grids */
  print_verilog_grids(module_manager, 
                      src_dir_path, lb_dir_path,
                      TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.dump_explicit_verilog);

  /* Generate FPGA fabric */
  print_verilog_top_module(module_manager, 
                           std::string(vpr_setup.FileNameOpts.ArchFile), 
                           src_dir_path,
                           TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.dump_explicit_verilog);

  /* Collect global ports from the circuit library
   * TODO: move outside this function 
   */
  std::vector<CircuitPortId> global_ports = find_circuit_library_global_ports(Arch.spice->circuit_lib);

  /* Generate wrapper module for FPGA fabric (mapped by the input benchmark and pre-configured testbench for verification */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_formal_verification_top_netlist) {
    std::string formal_verification_top_netlist_file_path = src_dir_path + chomped_circuit_name 
                                                          + std::string(formal_verification_verilog_file_postfix);
    print_verilog_preconfig_top_module(module_manager, bitstream_manager,
                                       Arch.spice->circuit_lib, global_ports, L_logical_blocks,
                                       device_size, L_grids, L_blocks,
                                       std::string(chomped_circuit_name), formal_verification_top_netlist_file_path,
                                       std::string(src_dir_path));

    /* Generate top-level testbench using random vectors */
    std::string random_top_testbench_file_path = src_dir_path + chomped_circuit_name 
                                               + std::string(random_top_testbench_verilog_file_postfix);
    print_verilog_random_top_testbench(chomped_circuit_name, random_top_testbench_file_path, 
                                       src_dir_path, L_logical_blocks,  
                                       vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts, Arch.spice->spice_params);
  }
 
  /* Generate exchangeable files which contains simulation settings */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_simulation_ini) {
    std::string simulation_ini_file_name;
    if (NULL != vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.simulation_ini_path) {
      simulation_ini_file_name = std::string(vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.simulation_ini_path);
    }
    print_verilog_simulation_info(simulation_ini_file_name,
                                  format_dir_path(chomped_parent_dir),
                                  chomped_circuit_name,
                                  src_dir_path,
                                  bitstream_manager.bits().size(),
                                  Arch.spice->spice_params.meas_params.sim_num_clock_cycle,
                                  Arch.spice->spice_params.stimulate_params.prog_clock_freq,
                                  Arch.spice->spice_params.stimulate_params.op_clock_freq);
  }

  /* Generate full testbench for verification, including configuration phase and operating phase */
  if (TRUE == vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.print_autocheck_top_testbench) {
    std::string autocheck_top_testbench_file_path = src_dir_path + chomped_circuit_name
                                                  + std::string(autocheck_top_testbench_verilog_file_postfix);
    print_verilog_top_testbench(module_manager, bitstream_manager, fabric_bitstream,
                                sram_verilog_orgz_info->type,
                                Arch.spice->circuit_lib, global_ports,
                                L_logical_blocks, device_size, L_grids, L_blocks,
                                chomped_circuit_name,
                                autocheck_top_testbench_file_path,
                                src_dir_path,
                                Arch.spice->spice_params);
  }

  /* Generate a Verilog file including all the netlists that have been generated */
  std::string ref_verilog_benchmark_file_name;
  if (NULL != vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.reference_verilog_benchmark_file) {
    ref_verilog_benchmark_file_name = std::string(vpr_setup.FPGA_SPICE_Opts.SynVerilogOpts.reference_verilog_benchmark_file);
  }
  print_include_netlists(src_dir_path,
                         chomped_circuit_name,
                         ref_verilog_benchmark_file_name,
                         Arch.spice->circuit_lib);

  /* Given a brief stats on how many Verilog modules have been written to files */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Outputted %lu Verilog modules in total\n", 
             module_manager.num_modules());  

  /* End time count */
  clock_t t_end = clock();
 
  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "FPGA-Verilog took %g seconds\n", 
             run_time_sec);  
}

