/************************************************************************
 * A header file for FabricGlobalPortList class, including critical data declaration
 * Please include this file only for using any FabricGlobalPortList data structure
 * Refer to fabric_global_port_list.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for fabric global ports to avoid illegal type casting 
 ***********************************************************************/
#ifndef FABRIC_GLOBAL_PORT_INFO_FWD_H
#define FABRIC_GLOBAL_PORT_INFO_FWD_H

#include "vtr_strong_id.h"

struct fabric_global_port_id_tag;

typedef vtr::StrongId<fabric_global_port_id_tag> FabricGlobalPortId;

/* Short declaration of class */
class FabricGlobalPortInfo;

#endif
