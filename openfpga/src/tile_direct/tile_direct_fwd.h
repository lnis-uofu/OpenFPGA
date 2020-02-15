/**************************************************
 * This file includes only declarations for 
 * the data structures for TileDirect
 * Please refer to tile_direct.h for more details
 *************************************************/
#ifndef TILE_DIRECT_FWD_H
#define TILE_DIRECT_FWD_H

#include "vtr_strong_id.h"

/* begin namespace openfpga */
namespace openfpga {

/* Strong Ids for ModuleManager */
struct tile_direct_id_tag;

typedef vtr::StrongId<tile_direct_id_tag> TileDirectId;

class TileDirect;

} /* end namespace openfpga */

#endif 
