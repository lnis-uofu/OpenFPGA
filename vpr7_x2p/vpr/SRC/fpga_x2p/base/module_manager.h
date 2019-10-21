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
#include "vtr_vector.h"
#include "module_manager_fwd.h"
#include "device_port.h"

class ModuleManager {
  public: /* Private data structures */
    enum e_module_port_type {
      MODULE_GLOBAL_PORT, /* Global inputs */
      MODULE_GPIO_PORT,   /* General-purpose IOs, which are data IOs of the fabric */
      MODULE_INOUT_PORT,  /* Normal (non-global) inout ports */
      MODULE_INPUT_PORT,  /* Normal (non-global) input ports */
      MODULE_OUTPUT_PORT, /* Normal (non-global) output ports */
      MODULE_CLOCK_PORT,  /* Nromal (non-global) clock ports*/
      NUM_MODULE_PORT_TYPES 
    };

  public: /* Public Constructors */

  public: /* Types and ranges */
    typedef vtr::vector<ModuleId, ModuleId>::const_iterator module_iterator;
    typedef vtr::vector<ModulePortId, ModulePortId>::const_iterator module_port_iterator;
    typedef vtr::vector<ModuleNetId, ModuleNetId>::const_iterator module_net_iterator;
    typedef vtr::vector<ModuleNetSrcId, ModuleNetSrcId>::const_iterator module_net_src_iterator;
    typedef vtr::vector<ModuleNetSinkId, ModuleNetSinkId>::const_iterator module_net_sink_iterator;

    typedef vtr::Range<module_iterator> module_range;
    typedef vtr::Range<module_port_iterator> module_port_range;
    typedef vtr::Range<module_net_iterator> module_net_range;
    typedef vtr::Range<module_net_src_iterator> module_net_src_range;
    typedef vtr::Range<module_net_sink_iterator> module_net_sink_range;

  public: /* Public aggregators */
    /* Find all the modules */
    module_range modules() const;
    /* Find all the ports belonging to a module */
    module_port_range module_ports(const ModuleId& module) const;
    /* Find all the nets belonging to a module */
    module_net_range module_nets(const ModuleId& module) const;
    /* Find all the child modules under a parent module */
    std::vector<ModuleId> child_modules(const ModuleId& parent_module) const;
    /* Find all the instances under a parent module */
    std::vector<size_t> child_module_instances(const ModuleId& parent_module, const ModuleId& child_module) const;
    /* Find the source ids of modules */
    module_net_src_range module_net_sources(const ModuleId& module, const ModuleNetId& net) const;
    /* Find the sink ids of modules */
    module_net_sink_range module_net_sinks(const ModuleId& module, const ModuleNetId& net) const;

  public: /* Public accessors */
    size_t num_modules() const;
    size_t num_nets(const ModuleId& module) const;
    std::string module_name(const ModuleId& module_id) const;
    std::string module_port_type_str(const enum e_module_port_type& port_type) const;
    std::vector<BasicPort> module_ports_by_type(const ModuleId& module_id, const enum e_module_port_type& port_type) const;
    std::vector<ModulePortId> module_port_ids_by_type(const ModuleId& module_id, const enum e_module_port_type& port_type) const;
    /* Find a port of a module by a given name */
    ModulePortId find_module_port(const ModuleId& module_id, const std::string& port_name) const;
    /* Find the Port information with a given port id */
    BasicPort module_port(const ModuleId& module_id, const ModulePortId& port_id) const;
    /* Find a module by a given name */
    ModuleId find_module(const std::string& name) const;
    /* Find the number of instances of a child module in the parent module */
    size_t num_instance(const ModuleId& parent_module, const ModuleId& child_module) const;
    /* Find the instance name of a child module */
    std::string instance_name(const ModuleId& parent_module, const ModuleId& child_module,
                              const size_t& instance_id) const;
    /* Find the instance id of a given instance name */
    size_t instance_id(const ModuleId& parent_module, const ModuleId& child_module,
                       const std::string& instance_name) const;
    /* Find if a port is a wire connection */
    bool port_is_wire(const ModuleId& module, const ModulePortId& port) const;
    /* Find if a port is register */
    bool port_is_register(const ModuleId& module, const ModulePortId& port) const;
    /* Return the pre-processing flag of a port */
    std::string port_preproc_flag(const ModuleId& module, const ModulePortId& port) const;
    /* Find a net from an instance of a module */
    ModuleNetId module_instance_port_net(const ModuleId& parent_module, 
                                         const ModuleId& child_module, const size_t& child_instance,
                                         const ModulePortId& child_port, const size_t& child_pin) const;
    /* Find the name of net */
    std::string net_name(const ModuleId& module, const ModuleNetId& net) const;
    /* Find the source modules of a net */
    vtr::vector<ModuleNetSrcId, ModuleId> net_source_modules(const ModuleId& module, const ModuleNetId& net) const;
    /* Find the ids of source instances of a net */
    vtr::vector<ModuleNetSrcId, size_t> net_source_instances(const ModuleId& module, const ModuleNetId& net) const;
    /* Find the source ports of a net */
    vtr::vector<ModuleNetSrcId, ModulePortId> net_source_ports(const ModuleId& module, const ModuleNetId& net) const;
    /* Find the source pin indices of a net */
    vtr::vector<ModuleNetSrcId, size_t> net_source_pins(const ModuleId& module, const ModuleNetId& net) const;

    /* Find the sink modules of a net */
    vtr::vector<ModuleNetSinkId, ModuleId> net_sink_modules(const ModuleId& module, const ModuleNetId& net) const;
    /* Find the ids of sink instances of a net */
    vtr::vector<ModuleNetSinkId, size_t> net_sink_instances(const ModuleId& module, const ModuleNetId& net) const;
    /* Find the sink ports of a net */
    vtr::vector<ModuleNetSinkId, ModulePortId> net_sink_ports(const ModuleId& module, const ModuleNetId& net) const;
    /* Find the sink pin indices of a net */
    vtr::vector<ModuleNetSinkId, size_t> net_sink_pins(const ModuleId& module, const ModuleNetId& net) const;
  private: /* Private accessors */
    size_t find_child_module_index_in_parent_module(const ModuleId& parent_module, const ModuleId& child_module) const;
  public: /* Public mutators */
    /* Add a module */
    ModuleId add_module(const std::string& name);
    /* Add a port to a module */
    ModulePortId add_port(const ModuleId& module, 
                          const BasicPort& port_info, const enum e_module_port_type& port_type);
    /* Set a name for a module */
    void set_module_name(const ModuleId& module, const std::string& name);
    /* Set a port to be a wire */
    void set_port_is_wire(const ModuleId& module, const std::string& port_name, const bool& is_wire);
    /* Set a port to be a register */
    void set_port_is_register(const ModuleId& module, const std::string& port_name, const bool& is_register);
    /* Set the preprocessing flag for a port */
    void set_port_preproc_flag(const ModuleId& module, const ModulePortId& port, const std::string& preproc_flag);
    /* Add a child module to a parent module */
    void add_child_module(const ModuleId& parent_module, const ModuleId& child_module);
    /* Set the instance name of a child module */
    void set_child_instance_name(const ModuleId& parent_module, const ModuleId& child_module, const size_t& instance_id, const std::string& instance_name);
    /* Add a net to the connection graph of the module */ 
    ModuleNetId create_module_net(const ModuleId& module);
    /* Set the name of net */
    void set_net_name(const ModuleId& module, const ModuleNetId& net,
                      const std::string& name);
    /* Add a source to a net in the connection graph */
    ModuleNetSrcId add_module_net_source(const ModuleId& module, const ModuleNetId& net,
                                         const ModuleId& src_module, const size_t& instance_id,
                                         const ModulePortId& src_port, const size_t& src_pin);
    /* Add a sink to a net in the connection graph */
    ModuleNetSinkId add_module_net_sink(const ModuleId& module, const ModuleNetId& net,
                                        const ModuleId& sink_module, const size_t& instance_id,
                                        const ModulePortId& sink_port, const size_t& sink_pin);
  public: /* Public validators/invalidators */
    bool valid_module_id(const ModuleId& module) const;
    bool valid_module_port_id(const ModuleId& module, const ModulePortId& port) const;
    bool valid_module_net_id(const ModuleId& module, const ModuleNetId& net) const;
  private: /* Private validators/invalidators */
    void invalidate_name2id_map();
    void invalidate_port_lookup();
    void invalidate_net_lookup();
  private: /* Internal data */
    /* Module-level data */
    vtr::vector<ModuleId, ModuleId> ids_;                                  /* Unique identifier for each Module */
    vtr::vector<ModuleId, std::string> names_;                                  /* Unique identifier for each Module */
    vtr::vector<ModuleId, std::vector<ModuleId>> parents_;                 /* Parent modules that include the module */
    vtr::vector<ModuleId, std::vector<ModuleId>> children_;                /* Child modules that this module contain */
    vtr::vector<ModuleId, std::vector<size_t>> num_child_instances_;          /* Number of children instance in each child module */
    vtr::vector<ModuleId, std::vector<std::vector<std::string>>> child_instance_names_;          /* Number of children instance in each child module */

    /* Port-level data */
    vtr::vector<ModuleId, vtr::vector<ModulePortId, ModulePortId>> port_ids_;    /* List of ports for each Module */ 
    vtr::vector<ModuleId, vtr::vector<ModulePortId, BasicPort>> ports_;    /* List of ports for each Module */ 
    vtr::vector<ModuleId, vtr::vector<ModulePortId, enum e_module_port_type>> port_types_; /* Type of ports */ 
    vtr::vector<ModuleId, vtr::vector<ModulePortId, bool>> port_is_wire_; /* If the port is a wire, use for Verilog port definition. If enabled: <port_type> reg <port_name>  */ 
    vtr::vector<ModuleId, vtr::vector<ModulePortId, bool>> port_is_register_; /* If the port is a register, use for Verilog port definition. If enabled: <port_type> reg <port_name>  */ 
    vtr::vector<ModuleId, vtr::vector<ModulePortId, std::string>> port_preproc_flags_; /* If a port is available only when a pre-processing flag is enabled. This is to record the pre-processing flags */ 

    /* Graph-level data: 
     * We use nets to model the connection between pins of modules and instances.  
     * To avoid large memory footprint, we do NOT create pins,   
     * To enable fast look-up on pins, we create a fast look-up
     */
    vtr::vector<ModuleId, vtr::vector<ModuleNetId, ModuleNetId>> net_ids_;    /* List of nets for each Module */ 
    vtr::vector<ModuleId, vtr::vector<ModuleNetId, std::string>> net_names_;    /* Name of net */ 

    vtr::vector<ModuleId, vtr::vector<ModuleNetId, vtr::vector<ModuleNetSrcId, ModuleNetSrcId>>> net_src_ids_;  /* Unique id of the source that drive the net */ 
    vtr::vector<ModuleId, vtr::vector<ModuleNetId, vtr::vector<ModuleNetSrcId, ModuleId>>> net_src_module_ids_;  /* Pin ids that drive the net */ 
    vtr::vector<ModuleId, vtr::vector<ModuleNetId, vtr::vector<ModuleNetSrcId, size_t>>> net_src_instance_ids_;  /* Pin ids that drive the net */ 
    vtr::vector<ModuleId, vtr::vector<ModuleNetId, vtr::vector<ModuleNetSrcId, ModulePortId>>> net_src_port_ids_;  /* Pin ids that drive the net */ 
    vtr::vector<ModuleId, vtr::vector<ModuleNetId, vtr::vector<ModuleNetSrcId, size_t>>> net_src_pin_ids_;  /* Pin ids that drive the net */ 


    vtr::vector<ModuleId, vtr::vector<ModuleNetId, vtr::vector<ModuleNetSinkId, ModuleNetSinkId>>> net_sink_ids_;  /* Unique ids of the sink that the net drives */ 
    vtr::vector<ModuleId, vtr::vector<ModuleNetId, vtr::vector<ModuleNetSinkId, ModuleId>>> net_sink_module_ids_;  /* Pin ids that the net drives */ 
    vtr::vector<ModuleId, vtr::vector<ModuleNetId, vtr::vector<ModuleNetSinkId, size_t>>> net_sink_instance_ids_;  /* Pin ids that drive the net */ 
    vtr::vector<ModuleId, vtr::vector<ModuleNetId, vtr::vector<ModuleNetSinkId, ModulePortId>>> net_sink_port_ids_;  /* Pin ids that drive the net */ 
    vtr::vector<ModuleId, vtr::vector<ModuleNetId, vtr::vector<ModuleNetSinkId, size_t>>> net_sink_pin_ids_;  /* Pin ids that drive the net */ 

    /* fast look-up for module */
    std::map<std::string, ModuleId> name_id_map_;
    /* fast look-up for ports */
    typedef vtr::vector<ModuleId, std::vector<std::vector<ModulePortId>>> PortLookup;
    mutable PortLookup port_lookup_; /* [module_ids][port_types][port_ids] */ 

    /* fast look-up for nets */
    typedef vtr::vector<ModuleId, std::map<ModuleId, std::vector<std::map<ModulePortId, std::vector<ModuleNetId>>>>> NetLookup;
    mutable NetLookup net_lookup_; /* [module_ids][module_ids][instance_ids][port_ids][pin_ids] */ 
};

#endif
