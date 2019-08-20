/************************************************
 * This files includes data structures for 
 * module management.
 * It keeps a list of modules that have been
 * generated, the port map of the modules,
 * parents and children of each modules
 * This will ease instanciation of modules 
 * with explicit port map and outputting a 
 * hierarchy of modules 
 ***********************************************/

#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <string>
#include "module_manager_fwd.h"
#include "device_port.h"

class ModuleManager {
  private: /* Internal data */
    vtr::vector<ModuleId, ModuleId> ids_; 
    vtr::vector<ModuleId, BasicPort> ports_; 
    vtr::vector<ModuleId, std::vector<ModuleId>> parents_;
    vtr::vector<ModuleId, std::vector<ModuleId>> children_;
};

#endif
