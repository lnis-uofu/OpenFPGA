#ifndef VERILOG_API_H
#define VERILOG_API_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>
#include <vector>

#include "bitstream_manager.h"
#include "bus_group.h"
#include "circuit_library.h"
#include "config_protocol.h"
#include "decoder_library.h"
#include "device_rr_gsb.h"
#include "fabric_bitstream.h"
#include "fabric_global_port_info.h"
#include "fabric_verilog_options.h"
#include "io_location_map.h"
#include "memory_bank_shift_register_banks.h"
#include "module_manager.h"
#include "mux_library.h"
#include "netlist_manager.h"
#include "pin_constraints.h"
#include "simulation_setting.h"
#include "verilog_testbench_options.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"
#include "vpr_netlist_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void fpga_fabric_verilog(
  ModuleManager& module_manager, NetlistManager& netlist_manager,
  const MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const CircuitLibrary& circuit_lib, const MuxLibrary& mux_lib,
  const DecoderLibrary& decoder_lib, const DeviceContext& device_ctx,
  const VprDeviceAnnotation& device_annotation,
  const DeviceRRGSB& device_rr_gsb, const FabricVerilogOption& options);

int fpga_verilog_full_testbench(
  const ModuleManager& module_manager,
  const BitstreamManager& bitstream_manager,
  const FabricBitstream& fabric_bitstream,
  const MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const AtomContext& atom_ctx, const PlacementContext& place_ctx,
  const PinConstraints& pin_constraints, const BusGroup& bus_group,
  const std::string& bitstream_file, const IoLocationMap& io_location_map,
  const FabricGlobalPortInfo& fabric_global_port_info,
  const VprNetlistAnnotation& netlist_annotation,
  const CircuitLibrary& circuit_lib,
  const SimulationSetting& simulation_parameters,
  const ConfigProtocol& config_protocol, const VerilogTestbenchOption& options);

int fpga_verilog_preconfigured_fabric_wrapper(
  const ModuleManager& module_manager,
  const BitstreamManager& bitstream_manager, const AtomContext& atom_ctx,
  const PlacementContext& place_ctx, const PinConstraints& pin_constraints,
  const BusGroup& bus_group, const IoLocationMap& io_location_map,
  const FabricGlobalPortInfo& fabric_global_port_info,
  const VprNetlistAnnotation& netlist_annotation,
  const CircuitLibrary& circuit_lib, const ConfigProtocol& config_protocol,
  const VerilogTestbenchOption& options);

int fpga_verilog_mock_fpga_wrapper(
  const ModuleManager& module_manager, const AtomContext& atom_ctx,
  const PlacementContext& place_ctx, const PinConstraints& pin_constraints,
  const BusGroup& bus_group, const IoLocationMap& io_location_map,
  const FabricGlobalPortInfo& fabric_global_port_info,
  const VprNetlistAnnotation& netlist_annotation,
  const VerilogTestbenchOption& options);

int fpga_verilog_preconfigured_testbench(
  const ModuleManager& module_manager, const AtomContext& atom_ctx,
  const PinConstraints& pin_constraints, const BusGroup& bus_group,
  const FabricGlobalPortInfo& fabric_global_port_info,
  const VprNetlistAnnotation& netlist_annotation,
  const SimulationSetting& simulation_setting,
  const VerilogTestbenchOption& options);

int fpga_verilog_simulation_task_info(
  const ModuleManager& module_manager,
  const BitstreamManager& bitstream_manager, const AtomContext& atom_ctx,
  const PlacementContext& place_ctx, const IoLocationMap& io_location_map,
  const SimulationSetting& simulation_setting,
  const ConfigProtocol& config_protocol, const VerilogTestbenchOption& options);

} /* end namespace openfpga */

#endif
