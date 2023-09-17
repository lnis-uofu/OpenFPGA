#ifndef VERILOG_ROUTING_H
#define VERILOG_ROUTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include "device_rr_gsb.h"
#include "fabric_verilog_options.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "mux_library.h"
#include "netlist_manager.h"
#include "rr_graph_view.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_flatten_routing_modules(
  NetlistManager& netlist_manager, const ModuleManager& module_manager,
  const ModuleNameMap& module_name_map, const DeviceRRGSB& device_rr_gsb,
  const RRGraphView& rr_graph, const std::string& subckt_dir,
  const std::string& subckt_dir_name, const FabricVerilogOption& options);

void print_verilog_unique_routing_modules(NetlistManager& netlist_manager,
                                          const ModuleManager& module_manager,
                                          const ModuleNameMap& module_name_map,
                                          const DeviceRRGSB& device_rr_gsb,
                                          const std::string& subckt_dir,
                                          const std::string& subckt_dir_name,
                                          const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
