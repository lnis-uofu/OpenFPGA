#ifndef VERILOG_GRID_H
#define VERILOG_GRID_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "fabric_verilog_options.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "netlist_manager.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_grids(
  NetlistManager& netlist_manager, const ModuleManager& module_manager,
  const ModuleNameMap& module_name_map, const DeviceContext& device_ctx,
  const VprDeviceAnnotation& device_annotation, const std::string& subckt_dir,
  const std::string& subckt_dir_name, const FabricVerilogOption& options,
  const bool& verbose);

} /* end namespace openfpga */

#endif
