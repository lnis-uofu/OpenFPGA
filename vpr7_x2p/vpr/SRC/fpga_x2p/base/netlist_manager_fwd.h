/**************************************************
 * This file includes only declarations for 
 * the data structures for netlist managers
 * Please refer to netlist_manager.h for more details
 *************************************************/
#ifndef NETLIST_MANAGER_FWD_H
#define NETLIST_MANAGER_FWD_H

#include "vtr_strong_id.h"

/* Strong Ids for ModuleManager */
struct netlist_id_tag;
struct preprocessing_flag_id_tag;

typedef vtr::StrongId<netlist_id_tag> NetlistId;
typedef vtr::StrongId<preprocessing_flag_id_tag> PreprocessingFlagId;

class NetlistManager;

#endif 
