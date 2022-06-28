#ifndef VERILOG_PRECONFIG_TOP_MODULE_H
#define VERILOG_PRECONFIG_TOP_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include <string>
#include "circuit_library.h"
#include "vpr_context.h"
#include "module_manager.h"
#include "bitstream_manager.h"
#include "pin_constraints.h"
#include "io_location_map.h"
#include "fabric_global_port_info.h"
#include "config_protocol.h"
#include "bus_group.h"
#include "vpr_netlist_annotation.h"
#include "verilog_testbench_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_verilog_preconfig_top_module(const ModuleManager& module_manager,
                                       const BitstreamManager& bitstream_manager,
                                       const ConfigProtocol &config_protocol,
                                       const CircuitLibrary& circuit_lib,
                                       const FabricGlobalPortInfo &global_ports,
                                       const AtomContext& atom_ctx,
                                       const PlacementContext& place_ctx,
                                       const PinConstraints& pin_constraints,
                                       const BusGroup& bus_group,
                                       const IoLocationMap& io_location_map,
                                       const VprNetlistAnnotation& netlist_annotation,
                                       const std::string& circuit_name,
                                       const std::string& verilog_fname,
                                       const VerilogTestbenchOption& options);

} /* end namespace openfpga */

#endif
