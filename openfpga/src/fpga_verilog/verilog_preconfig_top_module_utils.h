#ifndef VERILOG_PRECONFIG_TOP_MODULE_UTILS_H
#define VERILOG_PRECONFIG_TOP_MODULE_UTILS_H

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

void print_verilog_preconfig_top_module_internal_wires(
  std::fstream &fp, const ModuleManager &module_manager,
  const ModuleId &top_module, const std::string &port_postfix);

int print_verilog_preconfig_top_module_connect_global_ports(
  std::fstream &fp, const ModuleManager &module_manager,
  const ModuleId &top_module, const PinConstraints &pin_constraints,
  const AtomContext &atom_ctx, const VprNetlistAnnotation &netlist_annotation,
  const FabricGlobalPortInfo &fabric_global_ports,
  const std::vector<std::string> &benchmark_clock_port_names,
  const std::string &port_postfix);

} /* end namespace openfpga */

#endif
