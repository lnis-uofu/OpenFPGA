/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include <algorithm>
#include <map>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "vpr_utils.h"

/* Headers from openfpgashell library */
#include "build_module_graph_utils.h"
#include "build_top_module.h"
#include "build_top_module_child_fine_grained_instance.h"
#include "build_top_module_child_tile_instance.h"
#include "build_top_module_connection.h"
#include "build_top_module_directs.h"
#include "build_top_module_memory.h"
#include "build_top_module_memory_bank.h"
#include "build_top_module_utils.h"
#include "command_exit_codes.h"
#include "module_manager_memory_utils.h"
#include "module_manager_utils.h"
#include "openfpga_device_grid_utils.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"
#include "rr_gsb_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print the top-level module for the FPGA fabric in Verilog format
 * This function will
 * 1. name the top-level module
 * 2. include dependent netlists
 *    - User defined netlists
 *    - Auto-generated netlists
 * 3. Add the submodules to the top-level graph
 * 4. Add module nets to connect datapath ports
 * 5. Add module nets/submodules to connect configuration ports
 *******************************************************************/
int build_top_module(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const CircuitLibrary& circuit_lib, const ClockNetwork& clk_ntwk,
  const RRClockSpatialLookup& rr_clock_lookup,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const TileAnnotation& tile_annotation, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb, const TileDirect& tile_direct,
  const ArchDirect& arch_direct, const ConfigProtocol& config_protocol,
  const CircuitModelId& sram_model, const FabricTile& fabric_tile,
  const bool& name_module_using_index, const bool& frame_view,
  const bool& compact_routing_hierarchy, const bool& duplicate_grid_pin,
  const FabricKey& fabric_key, const bool& generate_random_fabric_key,
  const bool& group_config_block, const bool& perimeter_cb,
  const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Build FPGA fabric module");

  int status = CMD_EXEC_SUCCESS;

  /* Create a module as the top-level fabric, and add it to the module manager
   */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.add_module(top_module_name);

  /* Label module usage */
  module_manager.set_module_usage(top_module, ModuleManager::MODULE_TOP);

  size_t layer = 0;

  if (fabric_tile.empty()) {
    status = build_top_module_fine_grained_child_instances(
      module_manager, top_module, blwl_sr_banks, circuit_lib, clk_ntwk,
      rr_clock_lookup, vpr_device_annotation, grids, layer, tile_annotation,
      rr_graph, device_rr_gsb, tile_direct, arch_direct, config_protocol,
      sram_model, frame_view, compact_routing_hierarchy, duplicate_grid_pin,
      fabric_key, group_config_block, perimeter_cb, verbose);
  } else {
    /* Build the tile instances under the top module */
    status = build_top_module_tile_child_instances(
      module_manager, top_module, blwl_sr_banks, circuit_lib, clk_ntwk,
      rr_clock_lookup, vpr_device_annotation, grids, layer, tile_annotation,
      rr_graph, device_rr_gsb, tile_direct, arch_direct, fabric_tile,
      config_protocol, sram_model, fabric_key, group_config_block,
      name_module_using_index, perimeter_cb, frame_view, verbose);
  }

  if (status != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Shuffle the configurable children in a random sequence */
  if (true == generate_random_fabric_key) {
    shuffle_top_module_configurable_children(module_manager, top_module,
                                             config_protocol);
  }

  /* Build shift register bank detailed connections */
  sync_memory_bank_shift_register_banks_with_config_protocol_settings(
    module_manager, blwl_sr_banks, config_protocol, top_module, circuit_lib);

  /* Add shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  size_t module_num_shared_config_bits =
    find_module_num_shared_config_bits_from_child_modules(module_manager,
                                                          top_module);
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, top_module,
                                              module_num_shared_config_bits);
  }

  /* Add SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  TopModuleNumConfigBits top_module_num_config_bits =
    find_top_module_regional_num_config_bit(module_manager, top_module,
                                            circuit_lib, sram_model,
                                            config_protocol.type());

  if (!top_module_num_config_bits.empty()) {
    add_top_module_sram_ports(
      module_manager, top_module, circuit_lib, sram_model, config_protocol,
      const_cast<const MemoryBankShiftRegisterBanks&>(blwl_sr_banks),
      top_module_num_config_bits);
  }

  /* Add module nets to connect memory cells inside
   * This is a one-shot addition that covers all the memory modules in this pb
   * module!
   */
  if (false == frame_view) {
    if (0 < module_manager
              .configurable_children(
                top_module, ModuleManager::e_config_child_type::PHYSICAL)
              .size()) {
      add_top_module_nets_memory_config_bus(
        module_manager, decoder_lib, blwl_sr_banks, top_module, circuit_lib,
        config_protocol, circuit_lib.design_tech_type(sram_model),
        top_module_num_config_bits);
    }
  }

  /* For configuration chains, we avoid adding nets for programmable clocks if
   * there are a few */
  std::vector<std::string> global_port_blacklist;
  if (config_protocol.num_prog_clocks() > 1) {
    BasicPort prog_clk_port = config_protocol.prog_clock_port_info();
    global_port_blacklist.push_back(prog_clk_port.get_name());
    /* Add port */
    ModulePortId port_id = module_manager.add_port(
      top_module, prog_clk_port, ModuleManager::MODULE_GLOBAL_PORT);
    /* Add nets by following configurable children under different regions */
    add_top_module_nets_prog_clock(module_manager, top_module, port_id,
                                   config_protocol);
  }

  /* Add global ports to the top module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
   * @note This function is called after the
   * add_top_module_nets_memory_config_bus() because it may add some sub modules
   */
  add_module_global_ports_from_child_modules(module_manager, top_module,
                                             global_port_blacklist);

  return status;
}

} /* end namespace openfpga */
