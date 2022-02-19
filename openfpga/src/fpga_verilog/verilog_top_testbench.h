#ifndef VERILOG_TOP_TESTBENCH
#define VERILOG_TOP_TESTBENCH

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "module_manager.h"
#include "bitstream_manager.h"
#include "fabric_bitstream.h"
#include "circuit_library.h"
#include "config_protocol.h"
#include "vpr_context.h"
#include "pin_constraints.h"
#include "io_location_map.h"
#include "fabric_global_port_info.h"
#include "vpr_netlist_annotation.h"
#include "simulation_setting.h"
#include "memory_bank_shift_register_banks.h"
#include "verilog_testbench_options.h"
#include "bus_group.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_verilog_full_testbench(const ModuleManager& module_manager,
                                 const BitstreamManager& bitstream_manager,
                                 const FabricBitstream& fabric_bitstream,
                                 const MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                 const CircuitLibrary& circuit_lib,
                                 const ConfigProtocol& config_protocol,
                                 const FabricGlobalPortInfo& global_ports,
                                 const AtomContext& atom_ctx,
                                 const PlacementContext& place_ctx,
                                 const PinConstraints& pin_constraints,
                                 const BusGroup& bus_group,
                                 const std::string& bitstream_file,
                                 const IoLocationMap& io_location_map,
                                 const VprNetlistAnnotation& netlist_annotation,
                                 const std::string& circuit_name,
                                 const std::string& verilog_fname,
                                 const SimulationSetting& simulation_parameters,
                                 const VerilogTestbenchOption& options);

} /* end namespace openfpga */

#endif
