/**************************************************
 * This file includes only declarations for 
 * the data structures for module managers
 * Please refer to module_manager.h for more details
 *************************************************/
#ifndef MODULE_MANAGER_FWD_H
#define MODULE_MANAGER_FWD_H

#include "vtr_strong_id.h"

/* Strong Ids for MUXes */
struct module_id_tag;

typedef vtr::StrongId<module_id_tag> ModuleId;

class ModuleManager;

#endif 
