#ifndef VERILOG_TESTBENCH_IO_CONNECTION_H
#define VERILOG_TESTBENCH_IO_CONNECTION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "bus_group.h"
#include "fabric_global_port_info.h"
#include "io_location_map.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "pin_constraints.h"
#include "verilog_testbench_options.h"
#include "vpr_context.h"
#include "vpr_netlist_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_verilog_testbench_io_connection(
  const ModuleManager& module_manager, const FabricGlobalPortInfo& global_ports,
  const AtomContext& atom_ctx, const PlacementContext& place_ctx,
  const PinConstraints& pin_constraints, const BusGroup& bus_group,
  const IoLocationMap& io_location_map, const ModuleNameMap& module_name_map,
  const VprNetlistAnnotation& netlist_annotation,
  const std::string& circuit_name, const std::string& verilog_fname,
  const VerilogTestbenchOption& options);

} /* end namespace openfpga */

#endif
