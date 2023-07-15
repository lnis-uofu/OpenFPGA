#ifndef BUILD_FABRIC_TILE_H
#define BUILD_FABRIC_TILE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>

#include "fabric_tile.h"
#include "tile_config.h"
#include "device_grid.h"
#include "device_rr_gsb.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int build_fabric_tile(FabricTile& fabric_tile, const TileConfig& tile_config, const DeviceGrid& grids, const DeviceRRGSB& device_rr_gsb);

} /* end namespace openfpga */

#endif