/********************************************************************
 * This file includes the main function to build module graphs
 * for the FPGA fabric
 *******************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

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

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * The main function to be called for building module graphs 
 * for a FPGA fabric
 *******************************************************************/
ModuleManager build_device_module_graph(const DeviceContext& vpr_device_ctx,
                                        const OpenfpgaContext& openfpga_ctx,
                                        const bool& compress_routing,
                                        const bool& duplicate_grid_pin,
                                        const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Build fabric module graph");

  /* Module manager to be built */
  ModuleManager module_manager;

  CircuitModelId sram_model = openfpga_ctx.arch().config_protocol.memory_model();  
  VTR_ASSERT(true == openfpga_ctx.arch().circuit_lib.valid_model_id(sram_model));

  /* Add constant generator modules: VDD and GND */
  build_constant_generator_modules(module_manager);

  /* Register all the user-defined modules in the module manager
   * This should be done prior to other steps in this function, 
   * because they will be instanciated by other primitive modules
   */
  build_user_defined_modules(module_manager, openfpga_ctx.arch().circuit_lib);

  /* Build elmentary modules */
  build_essential_modules(module_manager, openfpga_ctx.arch().circuit_lib);

  /* Build local encoders for multiplexers, this MUST be called before multiplexer building */
  build_mux_local_decoder_modules(module_manager, openfpga_ctx.mux_lib(), 
                                  openfpga_ctx.arch().circuit_lib);

  /* Build multiplexer modules */
  build_mux_modules(module_manager, openfpga_ctx.mux_lib(), openfpga_ctx.arch().circuit_lib);

  /* Build LUT modules */
  build_lut_modules(module_manager, openfpga_ctx.arch().circuit_lib);

  /* Build wire modules */
  build_wire_modules(module_manager, openfpga_ctx.arch().circuit_lib);

  /* Build memory modules */
  build_memory_modules(module_manager, openfpga_ctx.mux_lib(), 
                       openfpga_ctx.arch().circuit_lib,
                       openfpga_ctx.arch().config_protocol.type());

  /* Build grid and programmable block modules */
  build_grid_modules(module_manager, vpr_device_ctx,
                     openfpga_ctx.vpr_device_annotation(),
                     openfpga_ctx.arch().circuit_lib,
                     openfpga_ctx.mux_lib(),
                     openfpga_ctx.arch().config_protocol.type(),
                     sram_model, duplicate_grid_pin, verbose);

  if (true == compress_routing) {
    build_unique_routing_modules(module_manager,
                                 vpr_device_ctx,
                                 openfpga_ctx.vpr_device_annotation(),
                                 openfpga_ctx.device_rr_gsb(),
                                 openfpga_ctx.arch().circuit_lib,
                                 openfpga_ctx.arch().config_protocol.type(),
                                 sram_model, verbose);
  } else {
    VTR_ASSERT_SAFE(false == compress_routing);
    build_flatten_routing_modules(module_manager,
                                  vpr_device_ctx,
                                  openfpga_ctx.vpr_device_annotation(),
                                  openfpga_ctx.device_rr_gsb(),
                                  openfpga_ctx.arch().circuit_lib,
                                  openfpga_ctx.arch().config_protocol.type(),
                                  sram_model, verbose);
  }

  /* Build FPGA fabric top-level module */
  build_top_module(module_manager, openfpga_ctx.arch().circuit_lib, 
                   vpr_device_ctx.grid,
                   vpr_device_ctx.rr_graph,
                   openfpga_ctx.device_rr_gsb(), 
                   openfpga_ctx.tile_direct(), 
                   openfpga_ctx.arch().arch_direct, 
                   openfpga_ctx.arch().config_protocol.type(),
                   sram_model,
                   compress_routing, duplicate_grid_pin);

  /* Now a critical correction has to be done!
   * In the module construction, we always use prefix of ports because they are binded
   * to the ports in architecture description (logic blocks etc.)
   * To interface with standard cell, we should
   * rename the ports of primitive modules using lib_name instead of prefix
   * (which have no children and are probably linked to a standard cell!)
   */
  rename_primitive_module_port_names(module_manager, openfpga_ctx.arch().circuit_lib);

  return module_manager;
}

} /* end namespace openfpga */
