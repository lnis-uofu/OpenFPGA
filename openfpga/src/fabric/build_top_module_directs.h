#ifndef BUILD_TOP_MODULE_DIRECTS_H
#define BUILD_TOP_MODULE_DIRECTS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "vtr_geometry.h"
#include "vtr_ndmatrix.h"
#include "arch_direct.h"
#include "tile_direct.h"
#include "vpr_device_annotation.h"
#include "device_grid.h"
#include "module_manager.h"
#include "circuit_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void add_top_module_nets_tile_direct_connections(ModuleManager& module_manager, 
                                                 const ModuleId& top_module, 
                                                 const CircuitLibrary& circuit_lib, 
                                                 const VprDeviceAnnotation& vpr_device_annotation,
                                                 const DeviceGrid& grids,
                                                 const vtr::Matrix<size_t>& grid_instance_ids,
                                                 const TileDirect& tile_direct,
                                                 const ArchDirect& arch_direct);

} /* end namespace openfpga */

#endif
