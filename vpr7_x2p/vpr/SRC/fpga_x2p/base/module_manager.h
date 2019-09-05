/******************************************************************************
 * This files includes data structures for module management.
 * It keeps a list of modules that have been generated, the port map of the modules,
 * parents and children of each modules. This will ease instanciation of modules 
 * with explicit port map and outputting a hierarchy of modules 
 *
 * Module includes the basic information:
 * 1. unique identifier
 * 2. module name: which should be unique 
 * 3. port list: basic information of all the ports belonging to the module
 * 4. port types: types of each port, which will matter how we output the ports
 * 5. parent modules: ids of parent modules
 * 6. children modules: ids of child modules 
 ******************************************************************************/

#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <string>
#include <map>
#include "module_manager_fwd.h"
#include "device_port.h"

class ModuleManager {
  public: /* Private data structures */
    enum e_module_port_type {
      MODULE_GLOBAL_PORT,
      MODULE_INOUT_PORT,
      MODULE_INPUT_PORT,
      MODULE_OUTPUT_PORT,
      MODULE_CLOCK_PORT,
      NUM_MODULE_PORT_TYPES 
    };
  public: /* Public Constructors */
  public: /* Public accessors */
    size_t num_modules() const;
    std::string module_name(const ModuleId& module_id) const;
    std::string module_port_type_str(const enum e_module_port_type& port_type) const;
    std::vector<BasicPort> module_ports_by_type(const ModuleId& module_id, const enum e_module_port_type& port_type) const;
    /* Find a port of a module by a given name */
    ModulePortId find_module_port(const ModuleId& module_id, const std::string& port_name) const;
    /* Find the Port information with a given port id */
    BasicPort module_port(const ModuleId& module_id, const ModulePortId& port_id) const;
    /* Find a module by a given name */
    ModuleId find_module(const std::string& name) const;
    /* Find the number of instances of a child module in the parent module */
    size_t num_instance(const ModuleId& parent_module, const ModuleId& child_module) const;
  public: /* Public mutators */
    /* Add a module */
    ModuleId add_module(const std::string& name);
    /* Add a port to a module */
    ModulePortId add_port(const ModuleId& module, 
                          const BasicPort& port_info, const enum e_module_port_type& port_type);
    /* Add a child module to a parent module */
    void add_child_module(const ModuleId& parent_module, const ModuleId& child_module);
  private: /* Private validators/invalidators */
    bool valid_module_id(const ModuleId& module) const;
    bool valid_module_port_id(const ModuleId& module, const ModulePortId& port) const;
    void invalidate_name2id_map();
    void invalidate_port_lookup();
  private: /* Internal data */
    vtr::vector<ModuleId, ModuleId> ids_;                                  /* Unique identifier for each Module */
    vtr::vector<ModuleId, std::string> names_;                                  /* Unique identifier for each Module */
    vtr::vector<ModuleId, std::vector<ModuleId>> parents_;                 /* Parent modules that include the module */
    vtr::vector<ModuleId, std::vector<ModuleId>> children_;                /* Child modules that this module contain */
    vtr::vector<ModuleId, std::vector<size_t>> num_child_instances_;          /* Number of children instance in each child module */

    vtr::vector<ModuleId, vtr::vector<ModulePortId, ModulePortId>> port_ids_;    /* List of ports for each Module */ 
    vtr::vector<ModuleId, vtr::vector<ModulePortId, BasicPort>> ports_;    /* List of ports for each Module */ 
    vtr::vector<ModuleId, vtr::vector<ModulePortId, enum e_module_port_type>> port_types_; /* Type of ports */ 

    /* fast look-up for module */
    std::map<std::string, ModuleId> name_id_map_;
    /* fast look-up for ports */
    typedef vtr::vector<ModuleId, std::vector<std::vector<ModulePortId>>> PortLookup;
    mutable PortLookup port_lookup_; /* [module_ids][port_types][port_ids] */ 
};

#endif
