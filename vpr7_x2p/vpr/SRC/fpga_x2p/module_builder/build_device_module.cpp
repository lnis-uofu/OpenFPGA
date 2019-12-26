/********************************************************************
 * This file includes the main function to build module graphs
 * for the FPGA fabric
 *******************************************************************/
#include <vector>
#include <time.h>
#include <unistd.h>

#include "vtr_assert.h"
#include "util.h"
#include "spice_types.h"
#include "fpga_x2p_utils.h"

#include "build_essential_modules.h"
#include "build_decoder_modules.h"
#include "build_mux_modules.h"
#include "build_lut_modules.h"
#include "build_wire_modules.h"
#include "build_memory_modules.h"
#include "build_grid_modules.h"
#include "build_routing_modules.h"
#include "build_top_module.h"
#include "build_device_module.h"

/********************************************************************
 * The main function to be called for building module graphs 
 * for a FPGA fabric
 *******************************************************************/
ModuleManager build_device_module_graph(const t_vpr_setup& vpr_setup,
                                        const t_arch& arch,
                                        const MuxLibrary& mux_lib,
                                        const vtr::Point<size_t>& device_size,
                                        const std::vector<std::vector<t_grid_tile>>& grids,
                                        const std::vector<t_switch_inf>& rr_switches,
                                        const std::vector<t_clb_to_clb_directs>& clb2clb_directs,
                                        const DeviceRRGSB& L_device_rr_gsb) {
  /* Check if the routing architecture we support*/
  if (UNI_DIRECTIONAL != vpr_setup.RoutingArch.directionality) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "FPGA X2P only supports uni-directional routing architecture!\n");
    exit(1);
  }

  /* We don't support mrFPGA */
#ifdef MRFPGA_H
  if (is_mrFPGA) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "FPGA X2P does not support mrFPGA!\n");
    exit(1);
  }
#endif

  /* Module Graph builder formally starts*/
  vpr_printf(TIO_MESSAGE_INFO, 
             "\nStart building module graphs for FPGA fabric...\n");

  /* Module manager to be built */
  ModuleManager module_manager;

  /* Start time count */
  clock_t t_start = clock();

  /* Assign the SRAM model applied to the FPGA fabric */
  VTR_ASSERT(NULL != arch.sram_inf.verilog_sram_inf_orgz); /* Check !*/
  t_spice_model* mem_model = arch.sram_inf.verilog_sram_inf_orgz->spice_model;
  /* initialize the SRAM organization information struct */
  CircuitModelId sram_model = arch.spice->circuit_lib.model(mem_model->name);  
  VTR_ASSERT(CircuitModelId::INVALID() != sram_model);

  /* TODO: This should be moved to FPGA-X2P setup 
   * Check all the SRAM port is using the correct SRAM circuit model 
   */
  config_spice_models_sram_port_spice_model(arch.spice->num_spice_model, 
                                            arch.spice->spice_models,
                                            arch.sram_inf.verilog_sram_inf_orgz->spice_model);
  config_circuit_models_sram_port_to_default_sram_model(arch.spice->circuit_lib, sram_model); 

  /* Add constant generator modules: VDD and GND */
  build_constant_generator_modules(module_manager);

  /* Register all the user-defined modules in the module manager
   * This should be done prior to other steps in this function, 
   * because they will be instanciated by other primitive modules
   */
  build_user_defined_modules(module_manager, arch.spice->circuit_lib);

  /* Build elmentary modules */
  build_essential_modules(module_manager, arch.spice->circuit_lib);

  /* Build local encoders for multiplexers, this MUST be called before multiplexer building */
  build_mux_local_decoder_modules(module_manager, mux_lib, arch.spice->circuit_lib);

  /* Build multiplexer modules */
  build_mux_modules(module_manager, mux_lib, arch.spice->circuit_lib);

  /* Build LUT modules */
  build_lut_modules(module_manager, arch.spice->circuit_lib);

  /* Build wire modules */
  build_wire_modules(module_manager, arch.spice->circuit_lib);

  /* Build memory modules */
  build_memory_modules(module_manager, mux_lib, arch.spice->circuit_lib,
                       arch.sram_inf.verilog_sram_inf_orgz->type);

  /* Build grid and programmable block modules */
  build_grid_modules(module_manager, arch.spice->circuit_lib, mux_lib,  
                     arch.sram_inf.verilog_sram_inf_orgz->type, sram_model,
                     TRUE == vpr_setup.FPGA_SPICE_Opts.duplicate_grid_pin);

  if (TRUE == vpr_setup.FPGA_SPICE_Opts.compact_routing_hierarchy) {
    build_unique_routing_modules(module_manager, L_device_rr_gsb, arch.spice->circuit_lib, 
                                 arch.sram_inf.verilog_sram_inf_orgz->type, sram_model, 
                                 vpr_setup.RoutingArch, rr_switches);
  } else {
    VTR_ASSERT(FALSE == vpr_setup.FPGA_SPICE_Opts.compact_routing_hierarchy);
    build_flatten_routing_modules(module_manager, L_device_rr_gsb, arch.spice->circuit_lib, 
                                  arch.sram_inf.verilog_sram_inf_orgz->type, sram_model, 
                                  vpr_setup.RoutingArch, rr_switches);
  }


  /* Build FPGA fabric top-level module */
  build_top_module(module_manager, arch.spice->circuit_lib, 
                   device_size, grids, L_device_rr_gsb, 
                   clb2clb_directs, 
                   arch.sram_inf.verilog_sram_inf_orgz->type, sram_model, 
                   TRUE == vpr_setup.FPGA_SPICE_Opts.compact_routing_hierarchy);

  /* Now a critical correction has to be done!
   * In the module construction, we always use prefix of ports because they are binded
   * to the ports in architecture description (logic blocks etc.)
   * To interface with standard cell, we should
   * rename the ports of primitive modules using lib_name instead of prefix
   * (which have no children and are probably linked to a standard cell!)
   */
  rename_primitive_module_port_names(module_manager, arch.spice->circuit_lib);

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "Building module graphs took %g seconds\n", 
             run_time_sec);  

  return module_manager;
}
