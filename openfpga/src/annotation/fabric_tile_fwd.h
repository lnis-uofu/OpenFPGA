/**************************************************
 * This file includes only declarations for
 * the data structures for fabric tiles
 * Please refer to fabric_tiles.h for more details
 *************************************************/
#ifndef FABRIC_TILE_FWD_H
#define FABRIC_TILE_FWD_H

#include "vtr_strong_id.h"

/* begin namespace openfpga */
namespace openfpga {

/* Strong Ids for ModuleManager */
struct fabric_tile_id_tag;

typedef vtr::StrongId<fabric_tile_id_tag> FabricTileId;

class FabricTile;

} /* end namespace openfpga */

#endif
