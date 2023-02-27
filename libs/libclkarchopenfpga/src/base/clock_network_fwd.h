/************************************************************************
 * A header file for ClockNetwork class, including critical data declaration
 * Please include this file only for using any PinConstraints data structure
 * Refer to clock_network.h for more details
 ***********************************************************************/

/************************************************************************
 * Create strong id for ClockNetwork to avoid illegal type casting
 ***********************************************************************/
#ifndef CLOCK_NETWORK_FWD_H
#define CLOCK_NETWORK_FWD_H

#include "vtr_strong_id.h"

namespace openfpga {  // Begin namespace openfpga

struct clock_level_id_tag;
struct clock_tree_id_tag;
struct clock_tree_pin_id_tag;
struct clock_spine_id_tag;
struct clock_switch_point_id_tag;

typedef vtr::StrongId<clock_level_id_tag> ClockLevelId;
typedef vtr::StrongId<clock_tree_id_tag> ClockTreeId;
typedef vtr::StrongId<clock_tree_pin_id_tag> ClockTreePinId;
typedef vtr::StrongId<clock_spine_id_tag> ClockSpineId;
typedef vtr::StrongId<clock_switch_point_id_tag> ClockSwitchPointId;

/* Short declaration of class */
class ClockNetwork;

}  // End of namespace openfpga

#endif
