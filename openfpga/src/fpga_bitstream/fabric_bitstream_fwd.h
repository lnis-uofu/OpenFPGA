/**************************************************
 * This file includes only declarations for 
 * the data structures for fabric-dependent bitstream database
 * Please refer to fabric_bitstream.h for more details
 *************************************************/
#ifndef FABRIC_BITSTREAM_FWD_H
#define FABRIC_BITSTREAM_FWD_H

#include "vtr_strong_id.h"

/* begin namespace openfpga */
namespace openfpga {

/* Strong Ids for BitstreamContext */
struct fabric_bit_id_tag;
struct fabric_bit_region_id_tag;

typedef vtr::StrongId<fabric_bit_id_tag> FabricBitId;
typedef vtr::StrongId<fabric_bit_region_id_tag> FabricBitRegionId;

class FabricBitstream;

} /* end namespace openfpga */

#endif 
