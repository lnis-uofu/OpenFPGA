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
#include "io_location_map.h"
#include "fabric_global_port_info.h"
#include "vpr_netlist_annotation.h"
#include "simulation_setting.h"
#include "verilog_testbench_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_top_testbench(const ModuleManager& module_manager,
                                 const BitstreamManager& bitstream_manager,
                                 const FabricBitstream& fabric_bitstream,
                                 const CircuitLibrary& circuit_lib,
                                 const ConfigProtocol& config_protocol,
                                 const FabricGlobalPortInfo& global_ports,
                                 const AtomContext& atom_ctx,
                                 const PlacementContext& place_ctx,
                                 const IoLocationMap& io_location_map,
                                 const VprNetlistAnnotation& netlist_annotation,
                                 const std::string& circuit_name,
                                 const std::string& verilog_fname,
                                 const SimulationSetting& simulation_parameters,
                                 const VerilogTestbenchOption& options);

} /* end namespace openfpga */

#endif
