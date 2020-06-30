/******************************************************************************
 * Memember functions for data structure ModuleManager
 ******************************************************************************/
#include <string>
#include <numeric>
#include <algorithm>
#include "vtr_assert.h"

#include "circuit_library.h"
#include "module_manager.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************************
 * Public Constructors
 ******************************************************************************/

/**************************************************
 * Public Accessors : Aggregates
 *************************************************/
/* Find all the modules */
ModuleManager::module_range ModuleManager::modules() const {
  return vtr::make_range(ids_.begin(), ids_.end());
}

/* Find all the ports belonging to a module */
ModuleManager::module_port_range ModuleManager::module_ports(const ModuleId& module) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_id(module));
  return vtr::make_range(port_ids_[module].begin(), port_ids_[module].end());
}

/* Find all the nets belonging to a module */
ModuleManager::module_net_range ModuleManager::module_nets(const ModuleId& module) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_id(module));
  return vtr::make_range(net_ids_[module].begin(), net_ids_[module].end());
}

/* Find all the child modules under a parent module */
std::vector<ModuleId> ModuleManager::child_modules(const ModuleId& parent_module) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_id(parent_module));
  return children_[parent_module];
}

/* Find all the instances under a parent module */
std::vector<size_t> ModuleManager::child_module_instances(const ModuleId& parent_module, const ModuleId& child_module) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_id(parent_module));
  /* Ensure that the child module is in the child list of parent module */
  size_t child_index = children_[parent_module].size();
  for (size_t i = 0; i < children_[parent_module].size(); ++i) {
    if (child_module == children_[parent_module][i]) {
      child_index = i;
      break;
    } 
  }
  VTR_ASSERT(child_index != children_[parent_module].size());
  
  /* Create a vector, with sequentially increasing numbers */
  std::vector<size_t> instance_range(num_child_instances_[parent_module][child_index], 0);
  std::iota(instance_range.begin(), instance_range.end(), 0);

  return instance_range;
}

/* Find all the configurable child modules under a parent module */
std::vector<ModuleId> ModuleManager::configurable_children(const ModuleId& parent_module) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_id(parent_module));

  return configurable_children_[parent_module];
}

/* Find all the instances of configurable child modules under a parent module */
std::vector<size_t> ModuleManager::configurable_child_instances(const ModuleId& parent_module) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_id(parent_module));

  return configurable_child_instances_[parent_module];
}

/* Find the source ids of modules */
ModuleManager::module_net_src_range ModuleManager::module_net_sources(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_net_id(module, net));
  return vtr::make_range(net_src_ids_[module][net].begin(), net_src_ids_[module][net].end());
}

/* Find the sink ids of modules */
ModuleManager::module_net_sink_range ModuleManager::module_net_sinks(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_net_id(module, net));
  return vtr::make_range(net_sink_ids_[module][net].begin(), net_sink_ids_[module][net].end());
}

/******************************************************************************
 * Public Accessors
 ******************************************************************************/
/* Return number of modules */
size_t ModuleManager::num_modules() const {
  return ids_.size();
}

/* Return number of net of a module */
size_t ModuleManager::num_nets(const ModuleId& module) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_id(module));
  return net_ids_[module].size();
}

/* Find the name of a module */
std::string ModuleManager::module_name(const ModuleId& module_id) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_id(module_id));
  return names_[module_id];
}

/* Get the string of a module port type */
std::string ModuleManager::module_port_type_str(const enum e_module_port_type& port_type) const {
  std::array<const char*, NUM_MODULE_PORT_TYPES> MODULE_PORT_TYPE_STRING = {{"GLOBAL PORTS", "GPIN PORTS", "GPOUT PORTS", "GPIO PORTS", "INOUT PORTS", "INPUT PORTS", "OUTPUT PORTS", "CLOCK PORTS"}};
  return MODULE_PORT_TYPE_STRING[port_type];
}

/* Find a list of ports of a module by a given types */
std::vector<BasicPort> ModuleManager::module_ports_by_type(const ModuleId& module_id, const enum e_module_port_type& port_type) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_id(module_id));

  std::vector<BasicPort> ports;
  for (const auto& port : port_ids_[module_id]) {
    /* Skip unmatched ports */
    if (port_type != port_types_[module_id][port]) {
      continue;
    }
    ports.push_back(ports_[module_id][port]);
  }

  return ports;
}

/* Find a list of port ids of a module by a given types */
std::vector<ModulePortId> ModuleManager::module_port_ids_by_type(const ModuleId& module_id, const enum e_module_port_type& port_type) const {
  /* Validate the module_id */
  VTR_ASSERT(valid_module_id(module_id));

  std::vector<ModulePortId> port_ids;
  for (const auto& port : port_ids_[module_id]) {
    /* Skip unmatched ports */
    if (port_type != port_types_[module_id][port]) {
      continue;
    }
    port_ids.push_back(port_ids_[module_id][port]);
  }

  return port_ids;
}


/* Find a port of a module by a given name */
ModulePortId ModuleManager::find_module_port(const ModuleId& module_id, const std::string& port_name) const {
  /* Validate the module id */
  VTR_ASSERT(valid_module_id(module_id));

  /* Iterate over the ports of the module */
  for (const auto& port : port_ids_[module_id]) {
    if (0 == port_name.compare(ports_[module_id][port].get_name())) {
      /* Find it, return the id */
      return port; 
    }
  }
  /* Not found, return an invalid id */
  return ModulePortId::INVALID();
}

/* Find the Port information with a given port id */
BasicPort ModuleManager::module_port(const ModuleId& module_id, const ModulePortId& port_id) const {
  /* Validate the module and port id */
  VTR_ASSERT(valid_module_port_id(module_id, port_id));
  return ports_[module_id][port_id]; 
}

/* Find the module id by a given name, return invalid if not found */
ModuleId ModuleManager::find_module(const std::string& name) const {
  if (name_id_map_.find(name) != name_id_map_.end()) {
    /* Find it, return the id */
    return name_id_map_.at(name); 
  }
  /* Not found, return an invalid id */
  return ModuleId::INVALID();
}

/* Find the number of instances of a child module in the parent module */
size_t ModuleManager::num_instance(const ModuleId& parent_module, const ModuleId& child_module) const {
  size_t child_index = find_child_module_index_in_parent_module(parent_module, child_module);
  if (size_t(-1) == child_index) {
    /* Not found, return a zero */
    return 0;
  }

  return num_child_instances_[parent_module][child_index]; 
}

/* Find the instance name of a child module */
std::string ModuleManager::instance_name(const ModuleId& parent_module, const ModuleId& child_module,
                                         const size_t& instance_id) const {
  /* Validate the id of both parent and child modules */
  VTR_ASSERT ( valid_module_id(parent_module) );
  VTR_ASSERT ( valid_module_id(child_module) );

  /* Find the index of child module in the child list of parent module */
  size_t child_index = find_child_module_index_in_parent_module(parent_module, child_module);
  VTR_ASSERT (child_index < children_[parent_module].size());
  /* Ensure that instance id is valid */
  VTR_ASSERT (instance_id < num_instance(parent_module, child_module));
  return child_instance_names_[parent_module][child_index][instance_id];
}

/* Find the instance id of a given instance name */
size_t ModuleManager::instance_id(const ModuleId& parent_module, const ModuleId& child_module,
                                  const std::string& instance_name) const {
  /* Validate the id of both parent and child modules */
  VTR_ASSERT ( valid_module_id(parent_module) );
  VTR_ASSERT ( valid_module_id(child_module) );

  /* Find the index of child module in the child list of parent module */
  size_t child_index = find_child_module_index_in_parent_module(parent_module, child_module);
  VTR_ASSERT (child_index < children_[parent_module].size());

  /* Search the instance name list and try to find a match */
  for (size_t name_id = 0; name_id < child_instance_names_[parent_module][child_index].size(); ++name_id) {
    const std::string& name = child_instance_names_[parent_module][child_index][name_id];
    if (0 == name.compare(instance_name)) {
      return name_id;
    }
  }
  
  /* Not found, return an invalid name */
  return size_t(-1);
}

/* Find if a port is a wire connection */
bool ModuleManager::port_is_wire(const ModuleId& module, const ModulePortId& port) const {
  /* validate both module id and port id*/
  VTR_ASSERT(valid_module_port_id(module, port));
  return port_is_wire_[module][port];
}

/* Find if a port is register */
bool ModuleManager::port_is_register(const ModuleId& module, const ModulePortId& port) const {
  /* validate both module id and port id*/
  VTR_ASSERT(valid_module_port_id(module, port));
  return port_is_register_[module][port];
}

/* Return the pre-processing flag of a port */
std::string ModuleManager::port_preproc_flag(const ModuleId& module, const ModulePortId& port) const {
  /* validate both module id and port id*/
  VTR_ASSERT(valid_module_port_id(module, port));
  return port_preproc_flags_[module][port];
}

/* Find a net from an instance of a module */
ModuleNetId ModuleManager::module_instance_port_net(const ModuleId& parent_module, 
                                                    const ModuleId& child_module, const size_t& child_instance,
                                                    const ModulePortId& child_port, const size_t& child_pin) const {
  /* Validate parent_module */
  VTR_ASSERT(valid_module_id(parent_module));
  
  /* Validate child_module */
  VTR_ASSERT(valid_module_id(child_module));

  /* Validate instance id */
  if (child_module == parent_module) {
    /* Assume a default instance id as zero */
    VTR_ASSERT(0 == child_instance);
  } else {
    VTR_ASSERT(child_instance < num_instance(parent_module, child_module));
  }

  /* Validate child_port */
  VTR_ASSERT(valid_module_port_id(child_module, child_port));

  /* Validate child_pin */
  VTR_ASSERT(child_pin < module_port(child_module, child_port).get_width());
  
  return net_lookup_[parent_module][child_module][child_instance][child_port][child_pin];
}

/* Find the name of net */
std::string ModuleManager::net_name(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  return net_names_[module][net];
}

/* Find the source modules of a net */
vtr::vector<ModuleNetSrcId, ModuleId> ModuleManager::net_source_modules(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  return net_src_module_ids_[module][net];
}

/* Find the ids of source instances of a net */
vtr::vector<ModuleNetSrcId, size_t> ModuleManager::net_source_instances(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  return net_src_instance_ids_[module][net];
}

/* Find the source ports of a net */
vtr::vector<ModuleNetSrcId, ModulePortId> ModuleManager::net_source_ports(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  return net_src_port_ids_[module][net];
}

/* Find the source pin indices of a net */
vtr::vector<ModuleNetSrcId, size_t> ModuleManager::net_source_pins(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  return net_src_pin_ids_[module][net];
}

/* Identify if a pin of a port in a module already exists in the net source list*/
bool ModuleManager::net_source_exist(const ModuleId& module, const ModuleNetId& net,
                                     const ModuleId& src_module, const size_t& instance_id,
                                     const ModulePortId& src_port, const size_t& src_pin) {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  /* Iterate over each source of the net.
   * If a net source has the same src_module, instance_id, src_port and src_pin,
   * we can say that the source has already been added to this net!
   */
  for (const ModuleNetSrcId& net_src : module_net_sources(module, net)) {
    if ( (src_module == net_source_modules(module, net)[net_src]) 
      && (instance_id == net_source_instances(module, net)[net_src])   
      && (src_port == net_source_ports(module, net)[net_src])   
      && (src_pin == net_source_pins(module, net)[net_src]) ) {
      return true;
    }
  }

  /* Reach here, it means nothing has been found. Return false */
  return false;
}

/* Find the sink modules of a net */
vtr::vector<ModuleNetSinkId, ModuleId> ModuleManager::net_sink_modules(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  return net_sink_module_ids_[module][net];
}

/* Find the ids of sink instances of a net */
vtr::vector<ModuleNetSinkId, size_t> ModuleManager::net_sink_instances(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  return net_sink_instance_ids_[module][net];
}

/* Find the sink ports of a net */
vtr::vector<ModuleNetSinkId, ModulePortId> ModuleManager::net_sink_ports(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  return net_sink_port_ids_[module][net];
}

/* Find the sink pin indices of a net */
vtr::vector<ModuleNetSinkId, size_t> ModuleManager::net_sink_pins(const ModuleId& module, const ModuleNetId& net) const {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  return net_sink_pin_ids_[module][net];
}

/* Identify if a pin of a port in a module already exists in the net sink list*/
bool ModuleManager::net_sink_exist(const ModuleId& module, const ModuleNetId& net,
                                     const ModuleId& sink_module, const size_t& instance_id,
                                     const ModulePortId& sink_port, const size_t& sink_pin) {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  /* Iterate over each sink of the net.
   * If a net sink has the same sink_module, instance_id, sink_port and sink_pin,
   * we can say that the sink has already been added to this net!
   */
  for (const ModuleNetSinkId& net_sink : module_net_sinks(module, net)) {
    if ( (sink_module == net_sink_modules(module, net)[net_sink]) 
      && (instance_id == net_sink_instances(module, net)[net_sink])   
      && (sink_port == net_sink_ports(module, net)[net_sink])   
      && (sink_pin == net_sink_pins(module, net)[net_sink]) ) {
      return true;
    }
  }

  /* Reach here, it means nothing has been found. Return false */
  return false;
}

/******************************************************************************
 * Private Accessors
 ******************************************************************************/
size_t ModuleManager::find_child_module_index_in_parent_module(const ModuleId& parent_module, const ModuleId& child_module) const {
  /* validate both module ids */
  VTR_ASSERT(valid_module_id(parent_module));
  VTR_ASSERT(valid_module_id(child_module));
  /* Try to find the child_module in the children list of parent_module*/
  for (size_t i = 0; i < children_[parent_module].size(); ++i) {
    if (child_module == children_[parent_module][i]) {
      /* Found, return the number of instances */
      return i; 
    }
  }
  /* Not found: return an valid value */
  return size_t(-1);
}

/******************************************************************************
 * Public Mutators
 ******************************************************************************/
/* Add a module */
ModuleId ModuleManager::add_module(const std::string& name) {
  /* Find if the name has been used. If used, return an invalid Id and report error! */
  std::map<std::string, ModuleId>::iterator it = name_id_map_.find(name);
  if (it != name_id_map_.end()) {
    return ModuleId::INVALID();
  }

  /* Create an new id */
  ModuleId module = ModuleId(ids_.size());
  ids_.push_back(module);

  /* Allocate other attributes */
  names_.push_back(name);
  parents_.emplace_back();
  children_.emplace_back();
  num_child_instances_.emplace_back();
  child_instance_names_.emplace_back();
  configurable_children_.emplace_back();
  configurable_child_instances_.emplace_back();

  port_ids_.emplace_back();
  ports_.emplace_back();
  port_types_.emplace_back();
  port_is_wire_.emplace_back();
  port_is_register_.emplace_back();
  port_preproc_flags_.emplace_back();

  net_ids_.emplace_back();
  net_names_.emplace_back();
  net_src_ids_.emplace_back();
  net_src_module_ids_.emplace_back();
  net_src_instance_ids_.emplace_back();
  net_src_port_ids_.emplace_back();
  net_src_pin_ids_.emplace_back();

  net_sink_ids_.emplace_back();
  net_sink_module_ids_.emplace_back();
  net_sink_instance_ids_.emplace_back();
  net_sink_port_ids_.emplace_back();
  net_sink_pin_ids_.emplace_back();

  /* Register in the name-to-id map */
  name_id_map_[name] = module;

  /* Build port lookup */
  port_lookup_.emplace_back();
  port_lookup_[module].resize(NUM_MODULE_PORT_TYPES);

  /* Build fast look-up for nets */
  net_lookup_.emplace_back();
  /* Reserve the instance 0 for the module */
  net_lookup_[module][module].emplace_back();

  /* Return the new id */
  return module;
}

/* Add a port to a module */
ModulePortId ModuleManager::add_port(const ModuleId& module, 
                                     const BasicPort& port_info, const enum e_module_port_type& port_type) {
  /* Validate the id of module */
  VTR_ASSERT( valid_module_id(module) );

  /* Add port and fill port attributes */
  ModulePortId port = ModulePortId(port_ids_[module].size());
  port_ids_[module].push_back(port);
  ports_[module].push_back(port_info);
  port_types_[module].push_back(port_type);
  port_is_wire_[module].push_back(false);
  port_is_register_[module].push_back(false);
  port_preproc_flags_[module].emplace_back(); /* Create an empty string for the pre-processing flags */

  /* Update fast look-up for port */
  port_lookup_[module][port_type].push_back(port);

  /* Update fast look-up for nets */
  VTR_ASSERT_SAFE(1 == net_lookup_[module][module].size());
  net_lookup_[module][module][0][port].resize(port_info.get_width(), ModuleNetId::INVALID());

  return port;
}

/* Set a name for a module port */
void ModuleManager::set_module_port_name(const ModuleId& module, const ModulePortId& module_port, 
                                         const std::string& port_name) {
  /* Validate the id of module port */
  VTR_ASSERT( valid_module_port_id(module, module_port) );
  
  ports_[module][module_port].set_name(port_name);
}

/* Set a name for a module */
void ModuleManager::set_module_name(const ModuleId& module, const std::string& name) {
  /* Validate the id of module */
  VTR_ASSERT( valid_module_id(module) );
  names_[module] = name;
}

/* Set a port to be a wire */
void ModuleManager::set_port_is_wire(const ModuleId& module, const std::string& port_name, const bool& is_wire) {
  /* Find the port */
  ModulePortId port = find_module_port(module, port_name);
  /* Must find something, otherwise drop an error */
  VTR_ASSERT(ModulePortId::INVALID() != port);
  port_is_wire_[module][port] = is_wire;
}

/* Set a port to be a register */
void ModuleManager::set_port_is_register(const ModuleId& module, const std::string& port_name, const bool& is_register) {
  /* Find the port */
  ModulePortId port = find_module_port(module, port_name);
  /* Must find something, otherwise drop an error */
  VTR_ASSERT(ModulePortId::INVALID() != port);
  port_is_register_[module][port] = is_register;
}

/* Set the preprocessing flag for a port */
void ModuleManager::set_port_preproc_flag(const ModuleId& module, const ModulePortId& port, const std::string& preproc_flag) {
  /* Must find something, otherwise drop an error */
  VTR_ASSERT(valid_module_port_id(module, port));
  port_preproc_flags_[module][port] = preproc_flag;
}

/* Add a child module to a parent module */
void ModuleManager::add_child_module(const ModuleId& parent_module, const ModuleId& child_module) {
  /* Validate the id of both parent and child modules */
  VTR_ASSERT ( valid_module_id(parent_module) );
  VTR_ASSERT ( valid_module_id(child_module) );

  /* Try to find if the parent module is already in the list */
  std::vector<ModuleId>::iterator parent_it = std::find(parents_[child_module].begin(), parents_[child_module].end(), parent_module);
  if (parent_it == parents_[child_module].end()) {
    /* Update the parent module of child module */
    parents_[child_module].push_back(parent_module);
  }

  std::vector<ModuleId>::iterator child_it = std::find(children_[parent_module].begin(), children_[parent_module].end(), child_module);
  if (child_it == children_[parent_module].end()) {
    /* Update the child module of parent module */
    children_[parent_module].push_back(child_module);
    num_child_instances_[parent_module].push_back(1); /* By default give one */
    /* Update the instance name list */
    child_instance_names_[parent_module].emplace_back();
    child_instance_names_[parent_module].back().emplace_back();
  } else {
    /* Increase the counter of instances */
    num_child_instances_[parent_module][child_it - children_[parent_module].begin()]++;
    child_instance_names_[parent_module][child_it - children_[parent_module].begin()].emplace_back();
  }

  /* Update fast look-up for nets */
  size_t instance_id = net_lookup_[parent_module][child_module].size();
  net_lookup_[parent_module][child_module].emplace_back();
  /* Find the ports for the child module and update the fast look-up */
  for (ModulePortId child_port : port_ids_[child_module]) {
    net_lookup_[parent_module][child_module][instance_id][child_port].resize(ports_[child_module][child_port].get_width(), ModuleNetId::INVALID());
  } 
}

/* Set the instance name of a child module */
void ModuleManager::set_child_instance_name(const ModuleId& parent_module, 
                                            const ModuleId& child_module, 
                                            const size_t& instance_id, 
                                            const std::string& instance_name) {
  /* Validate the id of both parent and child modules */
  VTR_ASSERT ( valid_module_id(parent_module) );
  VTR_ASSERT ( valid_module_id(child_module) );
  /* Ensure that the instance id is in range */
  VTR_ASSERT ( instance_id < num_instance(parent_module, child_module));
  /* Try to find the child_module in the children list of parent_module*/
  size_t child_index = find_child_module_index_in_parent_module(parent_module, child_module);
  /* We must find something! */
  VTR_ASSERT(size_t(-1) != child_index);
  /* Set the name */
  child_instance_names_[parent_module][child_index][instance_id] = instance_name;
}

/* Add a configurable child module to module
 * Note: this function should be called after add_child_module!
 * It will check if the child module does exist in the parent module
 * And the instance id is in range or not
 */
void ModuleManager::add_configurable_child(const ModuleId& parent_module, 
                                           const ModuleId& child_module, 
                                           const size_t& child_instance) {
  /* Validate the id of both parent and child modules */
  VTR_ASSERT ( valid_module_id(parent_module) );
  VTR_ASSERT ( valid_module_id(child_module) );
  /* Ensure that the instance id is in range */
  VTR_ASSERT ( child_instance < num_instance(parent_module, child_module));

  configurable_children_[parent_module].push_back(child_module);
  configurable_child_instances_[parent_module].push_back(child_instance);
}

void ModuleManager::reserve_configurable_child(const ModuleId& parent_module,
                                               const size_t& num_children) {
  VTR_ASSERT ( valid_module_id(parent_module) );
  /* Do reserve when the number of children is larger than current size of lists */
  if (num_children > configurable_children_[parent_module].size()) {
    configurable_children_[parent_module].reserve(num_children);
  }
  if (num_children > configurable_child_instances_[parent_module].size()) {
    configurable_child_instances_[parent_module].reserve(num_children);
  }
}

void ModuleManager::reserve_module_nets(const ModuleId& module,
                                        const size_t& num_nets) {
  /* Validate the module id */
  VTR_ASSERT ( valid_module_id(module) );

  net_ids_[module].reserve(num_nets);

  net_names_[module].reserve(num_nets);
  net_src_ids_[module].reserve(num_nets);
  net_src_module_ids_[module].reserve(num_nets);
  net_src_instance_ids_[module].reserve(num_nets);
  net_src_port_ids_[module].reserve(num_nets);
  net_src_pin_ids_[module].reserve(num_nets);

  net_sink_ids_[module].reserve(num_nets);
  net_sink_module_ids_[module].reserve(num_nets);
  net_sink_instance_ids_[module].reserve(num_nets);
  net_sink_port_ids_[module].reserve(num_nets);
  net_sink_pin_ids_[module].reserve(num_nets);
}

/* Add a net to the connection graph of the module */ 
ModuleNetId ModuleManager::create_module_net(const ModuleId& module) {
  /* Validate the module id */
  VTR_ASSERT ( valid_module_id(module) );

  /* Create an new id */
  ModuleNetId net = ModuleNetId(net_ids_[module].size());
  net_ids_[module].push_back(net);
  
  /* Allocate net-related data structures */
  net_names_[module].emplace_back();
  net_src_ids_[module].emplace_back();
  net_src_module_ids_[module].emplace_back();
  net_src_instance_ids_[module].emplace_back();
  net_src_port_ids_[module].emplace_back();
  net_src_pin_ids_[module].emplace_back();

  net_sink_ids_[module].emplace_back();
  net_sink_module_ids_[module].emplace_back();
  net_sink_instance_ids_[module].emplace_back();
  net_sink_port_ids_[module].emplace_back();
  net_sink_pin_ids_[module].emplace_back();

  return net;
}

/* Set the name of net */
void ModuleManager::set_net_name(const ModuleId& module, const ModuleNetId& net,
                                 const std::string& name) {
  /* Validate module net */
  VTR_ASSERT(valid_module_net_id(module, net));

  net_names_[module][net] = name;
}

/* Add a source to a net in the connection graph */
ModuleNetSrcId ModuleManager::add_module_net_source(const ModuleId& module, const ModuleNetId& net,
                                                    const ModuleId& src_module, const size_t& instance_id,
                                                    const ModulePortId& src_port, const size_t& src_pin) {
  /* Validate the module and net id */
  VTR_ASSERT(valid_module_net_id(module, net));

  /* Create a new id for src node */
  ModuleNetSrcId net_src = ModuleNetSrcId(net_src_ids_[module][net].size());
  net_src_ids_[module][net].push_back(net_src);

  /* Validate the source module */
  VTR_ASSERT(valid_module_id(src_module));
  net_src_module_ids_[module][net].push_back(src_module);

  /* if it has the same id as module, our instance id will be by default 0 */
  size_t src_instance_id = instance_id;
  if (src_module == module) {
    src_instance_id = 0;
    net_src_instance_ids_[module][net].push_back(src_instance_id);
  } else {
    /* Check the instance id of the src module */
    VTR_ASSERT (src_instance_id < num_instance(module, src_module));
    net_src_instance_ids_[module][net].push_back(src_instance_id);
  } 

  /* Validate the port exists in the src module */
  VTR_ASSERT(valid_module_port_id(src_module, src_port));
  net_src_port_ids_[module][net].push_back(src_port);

  /* Validate the pin id is in the range of the port width */
  VTR_ASSERT(src_pin < module_port(src_module, src_port).get_width());
  net_src_pin_ids_[module][net].push_back(src_pin);

  /* Update fast look-up for nets */
  net_lookup_[module][src_module][src_instance_id][src_port][src_pin] = net;

  return net_src;
}

/* Add a sink to a net in the connection graph */
ModuleNetSinkId ModuleManager::add_module_net_sink(const ModuleId& module, const ModuleNetId& net,
                                                   const ModuleId& sink_module, const size_t& instance_id,
                                                   const ModulePortId& sink_port, const size_t& sink_pin) {
  /* Validate the module and net id */
  VTR_ASSERT(valid_module_net_id(module, net));

  /* Create a new id for sink node */
  ModuleNetSinkId net_sink = ModuleNetSinkId(net_sink_ids_[module][net].size());
  net_sink_ids_[module][net].push_back(net_sink);

  /* Validate the source module */
  VTR_ASSERT(valid_module_id(sink_module));
  net_sink_module_ids_[module][net].push_back(sink_module);

  /* if it has the same id as module, our instance id will be by default 0 */
  size_t sink_instance_id = instance_id;
  if (sink_module == module) {
    sink_instance_id = 0;
    net_sink_instance_ids_[module][net].push_back(sink_instance_id);
  } else {
    /* Check the instance id of the src module */
    VTR_ASSERT (sink_instance_id < num_instance(module, sink_module));
    net_sink_instance_ids_[module][net].push_back(sink_instance_id);
  } 

  /* Validate the port exists in the sink module */
  VTR_ASSERT(valid_module_port_id(sink_module, sink_port));
  net_sink_port_ids_[module][net].push_back(sink_port);

  /* Validate the pin id is in the range of the port width */
  VTR_ASSERT(sink_pin < module_port(sink_module, sink_port).get_width());
  net_sink_pin_ids_[module][net].push_back(sink_pin);

  /* Update fast look-up for nets */
  net_lookup_[module][sink_module][sink_instance_id][sink_port][sink_pin] = net;

  return net_sink;
}

/******************************************************************************
 * Public Deconstructor
 ******************************************************************************/
void ModuleManager::clear_configurable_children(const ModuleId& parent_module) {
  VTR_ASSERT(valid_module_id(parent_module));

  configurable_children_[parent_module].clear();
  configurable_child_instances_[parent_module].clear();
}

/******************************************************************************
 * Private validators/invalidators
 ******************************************************************************/
bool ModuleManager::valid_module_id(const ModuleId& module) const {
  return ( size_t(module) < ids_.size() ) && ( module == ids_[module] ); 
}

bool ModuleManager::valid_module_port_id(const ModuleId& module, const ModulePortId& port) const {
  if (false == valid_module_id(module)) {
    return false;
  }
  return ( size_t(port) < port_ids_[module].size() ) && ( port == port_ids_[module][port] ); 
}

bool ModuleManager::valid_module_net_id(const ModuleId& module, const ModuleNetId& net) const {
  if (false == valid_module_id(module)) {
    return false;
  }
  return ( size_t(net) < net_ids_[module].size() ) && ( net == net_ids_[module][net] ); 
}

void ModuleManager::invalidate_name2id_map() {
  name_id_map_.clear();
}

void ModuleManager::invalidate_port_lookup() {
  port_lookup_.clear();
}

void ModuleManager::invalidate_net_lookup() {
  net_lookup_.clear();
}

} /* end namespace openfpga */
