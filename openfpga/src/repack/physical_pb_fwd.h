/**************************************************
 * This file includes only declarations for 
 * the data structures for PhysicalPb
 * Please refer to physical_pb.h for more details
 *************************************************/
#ifndef PHYSICAL_PB_FWD_H
#define PHYSICAL_PB_FWD_H

#include "vtr_strong_id.h"

/* begin namespace openfpga */
namespace openfpga {

/* Strong Ids for ModuleManager */
struct physical_pb_id_tag;

typedef vtr::StrongId<physical_pb_id_tag> PhysicalPbId;

class PhysicalPb;

} /* end namespace openfpga */

#endif 
