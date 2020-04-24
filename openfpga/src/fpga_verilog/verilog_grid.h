#ifndef VERILOG_GRID_H
#define VERILOG_GRID_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "vpr_context.h"
#include "module_manager.h"
#include "netlist_manager.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_grids(NetlistManager& netlist_manager,
                         const ModuleManager& module_manager,
                         const DeviceContext& device_ctx,
                         const VprDeviceAnnotation& device_annotation,
                         const std::string& verilog_dir,
                         const std::string& subckt_dir,
                         const bool& use_explicit_mapping,
                         const bool& verbose);


} /* end namespace openfpga */

#endif
