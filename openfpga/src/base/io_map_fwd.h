/**************************************************
 * This file includes only declarations for 
 * the data structures for IoMap
 * Please refer to io_map.h for more details
 *************************************************/
#ifndef IO_MAP_FWD_H
#define IO_MAP_FWD_H

#include "vtr_strong_id.h"

/* begin namespace openfpga */
namespace openfpga {

/* Strong Ids */
struct io_map_id_tag;

typedef vtr::StrongId<io_map_id_tag> IoMapId;

class IoMap;

} /* end namespace openfpga */

#endif 
