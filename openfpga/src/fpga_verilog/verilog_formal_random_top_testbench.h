#ifndef VERILOG_FORMAL_RANDOM_TOP_TESTBENCH
#define VERILOG_FORMAL_RANDOM_TOP_TESTBENCH

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "bus_group.h"
#include "fabric_global_port_info.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "pin_constraints.h"
#include "simulation_setting.h"
#include "verilog_testbench_options.h"
#include "vpr_context.h"
#include "vpr_netlist_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_random_top_testbench(
  const std::string& circuit_name, const std::string& verilog_fname,
  const AtomContext& atom_ctx, const VprNetlistAnnotation& netlist_annotation,
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const FabricGlobalPortInfo& global_ports,
  const PinConstraints& pin_constraints, const BusGroup& bus_group,
  const SimulationSetting& simulation_parameters,
  const VerilogTestbenchOption& options);

} /* end namespace openfpga */

#endif
