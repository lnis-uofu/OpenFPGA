/********************************************************************
 * This file includes the main function to build module graphs
 * for the FPGA fabric
 *******************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgashell library */
#include "build_decoder_modules.h"
#include "build_device_module.h"
#include "build_essential_modules.h"
#include "build_grid_modules.h"
#include "build_lut_modules.h"
#include "build_memory_modules.h"
#include "build_mux_modules.h"
#include "build_routing_modules.h"
#include "build_top_module.h"
#include "build_wire_modules.h"
#include "command_exit_codes.h"
#include "openfpga_naming.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * The main function to be called for building module graphs
 * for a FPGA fabric
 *******************************************************************/
int build_device_module_graph(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const OpenfpgaContext& openfpga_ctx, const DeviceContext& vpr_device_ctx,
  const bool& frame_view, const bool& compress_routing,
  const bool& duplicate_grid_pin, const FabricKey& fabric_key,
  const bool& generate_random_fabric_key, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Build fabric module graph");

  int status = CMD_EXEC_SUCCESS;

  CircuitModelId sram_model =
    openfpga_ctx.arch().config_protocol.memory_model();
  VTR_ASSERT(true ==
             openfpga_ctx.arch().circuit_lib.valid_model_id(sram_model));

  /* Add constant generator modules: VDD and GND */
  build_constant_generator_modules(module_manager);

  /* Register all the user-defined modules in the module manager
   * This should be done prior to other steps in this function,
   * because they will be instanciated by other primitive modules
   */
  build_user_defined_modules(module_manager, openfpga_ctx.arch().circuit_lib);

  /* Build elmentary modules */
  build_essential_modules(module_manager, openfpga_ctx.arch().circuit_lib);

  /* Build local encoders for multiplexers, this MUST be called before
   * multiplexer building */
  build_mux_local_decoder_modules(module_manager, openfpga_ctx.mux_lib(),
                                  openfpga_ctx.arch().circuit_lib);

  /* Build multiplexer modules */
  build_mux_modules(module_manager, openfpga_ctx.mux_lib(),
                    openfpga_ctx.arch().circuit_lib);

  /* Build LUT modules */
  build_lut_modules(module_manager, openfpga_ctx.arch().circuit_lib);

  /* Build wire modules */
  build_wire_modules(module_manager, openfpga_ctx.arch().circuit_lib);

  /* Build memory modules */
  build_memory_modules(module_manager, decoder_lib, openfpga_ctx.mux_lib(),
                       openfpga_ctx.arch().circuit_lib,
                       openfpga_ctx.arch().config_protocol.type());

  /* Build grid and programmable block modules */
  build_grid_modules(module_manager, decoder_lib, vpr_device_ctx,
                     openfpga_ctx.vpr_device_annotation(),
                     openfpga_ctx.arch().circuit_lib, openfpga_ctx.mux_lib(),
                     openfpga_ctx.arch().config_protocol.type(), sram_model,
                     duplicate_grid_pin, verbose);

  if (true == compress_routing) {
    build_unique_routing_modules(
      module_manager, decoder_lib, vpr_device_ctx,
      openfpga_ctx.vpr_device_annotation(), openfpga_ctx.device_rr_gsb(),
      openfpga_ctx.arch().circuit_lib,
      openfpga_ctx.arch().config_protocol.type(), sram_model, verbose);
  } else {
    VTR_ASSERT_SAFE(false == compress_routing);
    build_flatten_routing_modules(
      module_manager, decoder_lib, vpr_device_ctx,
      openfpga_ctx.vpr_device_annotation(), openfpga_ctx.device_rr_gsb(),
      openfpga_ctx.arch().circuit_lib,
      openfpga_ctx.arch().config_protocol.type(), sram_model, verbose);
  }

  /* Build FPGA fabric top-level module */
  status = build_top_module(
    module_manager, decoder_lib, blwl_sr_banks, openfpga_ctx.arch().circuit_lib,
    openfpga_ctx.clock_arch(), openfpga_ctx.clock_rr_lookup(),
    openfpga_ctx.vpr_device_annotation(), vpr_device_ctx.grid,
    openfpga_ctx.arch().tile_annotations, vpr_device_ctx.rr_graph,
    openfpga_ctx.device_rr_gsb(), openfpga_ctx.tile_direct(),
    openfpga_ctx.arch().arch_direct, openfpga_ctx.arch().config_protocol,
    sram_model, frame_view, compress_routing, duplicate_grid_pin, fabric_key,
    generate_random_fabric_key);

  if (CMD_EXEC_FATAL_ERROR == status) {
    return status;
  }

  /* Now a critical correction has to be done!
   * In the module construction, we always use prefix of ports because they are
   * binded to the ports in architecture description (logic blocks etc.) To
   * interface with standard cell, we should rename the ports of primitive
   * modules using lib_name instead of prefix (which have no children and are
   * probably linked to a standard cell!)
   */
  rename_primitive_module_port_names(module_manager,
                                     openfpga_ctx.arch().circuit_lib);

  return status;
}

/********************************************************************
 * The main function to be called for adding the fpga_core wrapper to a FPGA
 *fabric
 * - Rename existing fpga_top to fpga_core
 * - Create a wrapper module 'fpga_top' on the fpga_core
 *******************************************************************/
int add_fpga_core_to_device_module_graph(ModuleManager& module_manager,
                                         const std::string& core_inst_name,
                                         const bool& frame_view,
                                         const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;

  /* Execute the module graph api */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  if (!module_manager.valid_module_id(top_module)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Rename existing top module to fpga_core */
  std::string core_module_name = generate_fpga_core_module_name();
  module_manager.set_module_name(top_module, core_module_name);
  VTR_LOGV(verbose, "Rename current top-level module '%s' to '%s'\n",
           top_module_name.c_str(), core_module_name.c_str());

  /* Create a wrapper module under the existing fpga_top */
  ModuleId new_top_module = module_manager.create_wrapper_module(
    top_module, top_module_name, core_inst_name, !frame_view);
  if (!module_manager.valid_module_id(new_top_module)) {
    VTR_LOGV_ERROR(verbose,
                   "Failed to create a wrapper module '%s' on top of '%s'!\n",
                   top_module_name.c_str(), core_module_name.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  VTR_LOGV(verbose, "Created a wrapper module '%s' on top of '%s'\n",
           top_module_name.c_str(), core_module_name.c_str());

  /* Now fpga_core should be the only configurable child under the top-level
   * module */
  module_manager.add_configurable_child(new_top_module, top_module, 0);

  /* TODO: Update the fabric global ports */

  return status;
}

} /* end namespace openfpga */
