/************************************************************************
 * A header file for BusGroup class, including critical data declaration
 * Please include this file only for using any PinConstraints data structure
 * Refer to bus_group.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for BusGroup to avoid illegal type casting 
 ***********************************************************************/
#ifndef BUS_GROUP_FWD_H
#define BUS_GROUP_FWD_H

#include "vtr_strong_id.h"

namespace openfpga { // Begin namespace openfpga

struct bus_group_id_tag;
struct bus_pin_id_tag;

typedef vtr::StrongId<bus_group_id_tag> BusGroupId;
typedef vtr::StrongId<bus_pin_id_tag> BusPinId;

/* Short declaration of class */
class BusGroup;

} // End of namespace openfpga

#endif
