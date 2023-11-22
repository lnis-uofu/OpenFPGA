#ifndef VERILOG_MOCK_FPGA_WRAPPER_H
#define VERILOG_MOCK_FPGA_WRAPPER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "bitstream_manager.h"
#include "bus_group.h"
#include "circuit_library.h"
#include "config_protocol.h"
#include "fabric_global_port_info.h"
#include "io_location_map.h"
#include "io_name_map.h"
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

int print_verilog_mock_fpga_wrapper(
  const ModuleManager& module_manager, const FabricGlobalPortInfo& global_ports,
  const AtomContext& atom_ctx, const PlacementContext& place_ctx,
  const PinConstraints& pin_constraints, const BusGroup& bus_group,
  const IoLocationMap& io_location_map, const IoNameMap& io_name_map,
  const ModuleNameMap& module_name_map,
  const VprNetlistAnnotation& netlist_annotation,
  const std::string& circuit_name, const std::string& verilog_fname,
  const VerilogTestbenchOption& options);

} /* end namespace openfpga */

#endif
