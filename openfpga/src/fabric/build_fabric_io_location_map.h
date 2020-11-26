#ifndef BUILD_FABRIC_IO_LOCATION_MAP_H
#define BUILD_FABRIC_IO_LOCATION_MAP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>
#include "device_grid.h"
#include "io_location_map.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

IoLocationMap build_fabric_io_location_map(const ModuleManager& module_manager,
                                           const DeviceGrid& grids);

} /* end namespace openfpga */

#endif
