#ifndef BUILD_FABRIC_TILE_H
#define BUILD_FABRIC_TILE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>

#include "device_grid.h"
#include "device_rr_gsb.h"
#include "fabric_tile.h"
#include "tile_config.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int build_fabric_tile(FabricTile& fabric_tile, const TileConfig& tile_config,
                      const DeviceGrid& grids, const RRGraphView& rr_graph,
                      const DeviceRRGSB& device_rr_gsb, const bool& verbose);

} /* end namespace openfpga */

#endif
