/******************************************************************************
 * This files includes data structures for netlist management.
 * It keeps a list of netlists that have been created
 * Each netlist includes a list of ids of modules that are stored in ModuleManager
 *
 * When we want to dump out a netlist in Verilog/SPICE format,
 * the netlist manager can generate the dependency on other netlists 
 * This can help us tracking the dependency and generate `include` files easily
 *
 * Cross-reference:
 *
 *   +---------+               +---------+      
 *   |         |   ModuleId    |         |
 *   | Netlist |-------------->| Module  |
 *   | Manager |               | Manager |
 *   |         |               |         |
 *   +---------+               +---------+
 *   
 ******************************************************************************/
#ifndef NETLIST_MANAGER_H
#define NETLIST_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include "vtr_vector.h"
#include "netlist_manager_fwd.h"
#include "module_manager.h"

class NetlistManager {
  public: /* Types and ranges */
    typedef vtr::vector<NetlistId, NetlistId>::const_iterator netlist_iterator;

    typedef vtr::Range<netlist_iterator> netlist_range;

  public: /* Public aggregators */
    /* Find all the netlists */
    netlist_range netlists() const;
    /* Find all the modules that are included in a netlist */
    std::vector<ModuleId> netlist_modules(const NetlistId& netlist) const;
    /* Find all the preprocessing flags that are included in a netlist */
    std::vector<std::string> netlist_preprocessing_flags(const NetlistId& netlist) const;

  public: /* Public accessors */
    /* Find the name of a netlist */
    std::string netlist_name(const NetlistId& netlist) const;
    /* Find a netlist by its name */
    NetlistId find_netlist(const std::string& netlist_name) const;
    /* Find if a module belongs to a netlist */
    bool is_module_in_netlist(const NetlistId& netlist, const ModuleId& module) const;
    /* Find the netlist that a module belongs to */
    NetlistId find_module_netlist(const ModuleId& module) const;

  public: /* Public mutators */
    /* Add a netlist to the library */
    NetlistId add_netlist(const std::string& name);
    /* Add a module to a netlist in the library */
    bool add_netlist_module(const NetlistId& netlist, const ModuleId& module);
    /* Add a pre-processing flag to a netlist */
    void add_netlist_preprocessing_flag(const NetlistId& netlist, const std::string& preprocessing_flag);

  public: /* Public validators/invalidators */
    bool valid_netlist_id(const NetlistId& netlist) const;

  private: /* Private validators/invalidators */
    bool valid_preprocessing_flag_id(const PreprocessingFlagId& flag) const;
    void invalidate_name2id_map();
    void invalidate_module2netlist_map();

  private: /* Internal data */
    vtr::vector<NetlistId, NetlistId> netlist_ids_;
    vtr::vector<NetlistId, std::string> netlist_names_;
    vtr::vector<NetlistId, std::vector<ModuleId>> included_module_ids_;
    vtr::vector<NetlistId, std::vector<PreprocessingFlagId>> included_preprocessing_flag_ids_;

    vtr::vector<PreprocessingFlagId, PreprocessingFlagId> preprocessing_flag_ids_;
    vtr::vector<PreprocessingFlagId, std::string> preprocessing_flag_names_;

    /* fast look-up for netlist */
    std::map<std::string, NetlistId> name_id_map_;
    /* fast look-up for modules in netlists */
    std::map<ModuleId, NetlistId> module_netlist_map_;
};

#endif

