/******************************************************************************
 * This files includes memeber functions for data structure NetlistManager
 ******************************************************************************/
#include <algorithm>

#include "vtr_assert.h"
#include "netlist_manager.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************************
 * Public aggregators
 ******************************************************************************/
/* Find all the netlists */
NetlistManager::netlist_range NetlistManager::netlists() const {
  return vtr::make_range(netlist_ids_.begin(), netlist_ids_.end());
}

/* Find all the modules that are included in a netlist */
std::vector<ModuleId> NetlistManager::netlist_modules(const NetlistId& netlist) const {
  VTR_ASSERT(true == valid_netlist_id(netlist));
  return included_module_ids_[netlist];
}


/******************************************************************************
 * Public accessors
 ******************************************************************************/
/* Find the name of a netlist */
std::string NetlistManager::netlist_name(const NetlistId& netlist) const {
  VTR_ASSERT(true == valid_netlist_id(netlist));
  return netlist_names_[netlist];
}

/* Find a netlist by its name */
NetlistId NetlistManager::find_netlist(const std::string& netlist_name) const {
  if (name_id_map_.find(netlist_name) != name_id_map_.end()) {
    /* Found, return the id */
    return name_id_map_.at(netlist_name);
  } 
  /* Not found, return an invalid id */
  return NetlistId::INVALID();
}

NetlistManager::e_netlist_type NetlistManager::netlist_type(const NetlistId& netlist) const {
  VTR_ASSERT(true == valid_netlist_id(netlist));
  return netlist_types_[netlist];
}

/* Find if a module belongs to a netlist */
bool NetlistManager::is_module_in_netlist(const NetlistId& netlist, const ModuleId& module) const {
  VTR_ASSERT(true == valid_netlist_id(netlist));

  for (const ModuleId& included_module : included_module_ids_[netlist]) {
    /* Already in the netlist, return true */
    if (module == included_module) {
      return true;  
    }
  }
 
  /* Not in the netlist, return false */
  return false;
}

/* Find the netlist that a module belongs to */
NetlistId NetlistManager::find_module_netlist(const ModuleId& module) const {
  /* Find if the module has been added to a netlist. If used, return false! */
  /* Not found, return an invalid value */
  if ( module_netlist_map_.end()
    != module_netlist_map_.find(module)) {
    return NetlistId::INVALID();
  }
  return module_netlist_map_.at(module);
}


/* Find all the preprocessing flags that are included in a netlist */
std::vector<std::string> NetlistManager::netlist_preprocessing_flags(const NetlistId& netlist) const {
  VTR_ASSERT(true == valid_netlist_id(netlist));

  std::vector<std::string> flags; 

  for (const PreprocessingFlagId& flag_id : included_preprocessing_flag_ids_[netlist]) {
    VTR_ASSERT(true == valid_preprocessing_flag_id(flag_id));
    flags.push_back(preprocessing_flag_names_[flag_id]);
  }

  return flags;
}

/******************************************************************************
 * Public mutators
 ******************************************************************************/
/* Add a netlist to the library */
NetlistId NetlistManager::add_netlist(const std::string& name) {
  /* Find if the name has been used. If used, return an invalid Id! */
  std::map<std::string, NetlistId>::iterator it = name_id_map_.find(name);
  if (it !=  name_id_map_.end()) {
    return NetlistId::INVALID();
  }

  /* Create a new id */
  NetlistId netlist = NetlistId(netlist_ids_.size());
  netlist_ids_.push_back(netlist);

  /* Allocate related attributes */
  netlist_names_.push_back(name);
  netlist_types_.push_back(NUM_NETLIST_TYPES);
  included_module_ids_.emplace_back();
  included_preprocessing_flag_ids_.emplace_back();

  /* Register in the name-to-id map */
  name_id_map_[name] = netlist;  

  return netlist;
} 

void NetlistManager::set_netlist_type(const NetlistId& netlist,
                                      const e_netlist_type& type) {
  VTR_ASSERT(true == valid_netlist_id(netlist));
  netlist_types_[netlist] = type;
}

/* Add a module to a netlist in the library */
bool NetlistManager::add_netlist_module(const NetlistId& netlist, const ModuleId& module) {
  VTR_ASSERT(true == valid_netlist_id(netlist));

  /* Find if the module already in the netlist */
  std::vector<ModuleId>::iterator module_it = std::find(included_module_ids_[netlist].begin(), included_module_ids_[netlist].end(), module);
  if (module_it != included_module_ids_[netlist].end()) {
    /* Already in the netlist, nothing to do */
    return true;
  }
  /* Try to register it in module-to-netlist map */
  /* Find if the module has been added to a netlist. If used, return false! */
  std::map<ModuleId, NetlistId>::iterator map_it = module_netlist_map_.find(module);
  if (map_it != module_netlist_map_.end()) {
    return false;
  }

  /* Does not exist! Should add it to the list */
  included_module_ids_[netlist].push_back(module);
  /* Register it in module-to-netlist map */
  module_netlist_map_[module] = netlist;
  return true;
}

/* Add a pre-processing flag to a netlist */
void NetlistManager::add_netlist_preprocessing_flag(const NetlistId& netlist, const std::string& preprocessing_flag) {
  VTR_ASSERT(true == valid_netlist_id(netlist));

  PreprocessingFlagId flag = PreprocessingFlagId(preprocessing_flag_ids_.size());
 
  /* Find if the module already in the netlist */
  for (const PreprocessingFlagId& id : preprocessing_flag_ids_) {
    if (0 != preprocessing_flag.compare(preprocessing_flag_names_[id])) {
      continue;
    }
    /* Already in the list of pre-processing flags, push it ot the  */
    flag = id;
    break;
  }

  /* Update the list if we need */
  if (flag == PreprocessingFlagId(preprocessing_flag_ids_.size())) {
    preprocessing_flag_ids_.push_back(flag);
    preprocessing_flag_names_.push_back(preprocessing_flag);
  }

  /* Check if the flag is already in the netlist */
  std::vector<PreprocessingFlagId>::iterator it = std::find(included_preprocessing_flag_ids_[netlist].begin(), included_preprocessing_flag_ids_[netlist].end(), flag);
  if (it == included_preprocessing_flag_ids_[netlist].end()) {
    /* Not in the list, we add it */
    included_preprocessing_flag_ids_[netlist].push_back(flag);
  }
}

/******************************************************************************
 * Public validators/invalidators
 ******************************************************************************/
bool NetlistManager::valid_netlist_id(const NetlistId& netlist) const {
  return (size_t(netlist) < netlist_ids_.size()) && (netlist == netlist_ids_[netlist]);
}

/******************************************************************************
 * Private validators/invalidators
 ******************************************************************************/
bool NetlistManager::valid_preprocessing_flag_id(const PreprocessingFlagId& flag) const {
  return (size_t(flag) < preprocessing_flag_ids_.size()) && (flag == preprocessing_flag_ids_[flag]);
}

void NetlistManager::invalidate_name2id_map() {
  name_id_map_.clear();
}

void NetlistManager::invalidate_module2netlist_map() {
  module_netlist_map_.clear();
}

} /* end namespace openfpga */
