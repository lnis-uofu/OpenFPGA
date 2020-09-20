#ifndef SPICE_GRID_H
#define SPICE_GRID_H

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

void print_spice_grids(NetlistManager& netlist_manager,
                      const ModuleManager& module_manager,
                      const DeviceContext& device_ctx,
                      const VprDeviceAnnotation& device_annotation,
                      const std::string& subckt_dir,
                      const bool& verbose);


} /* end namespace openfpga */

#endif
