/******************************************************************************
 * This files includes declarations for most utilized functions 
 * for data structures for module management.
 ******************************************************************************/

#ifndef MODULE_MANAGER_UTILS_H
#define MODULE_MANAGER_UTILS_H

/* Include other header files which are dependency on the function declared below */
#include <vector>
#include "device_port.h"
#include "spice_types.h"
#include "vpr_types.h"
#include "circuit_library.h"
#include "module_manager.h"

ModuleId add_circuit_model_to_module_manager(ModuleManager& module_manager, 
                                             const CircuitLibrary& circuit_lib, const CircuitModelId& circuit_model,
                                             const std::string& module_name);

ModuleId add_circuit_model_to_module_manager(ModuleManager& module_manager, 
                                             const CircuitLibrary& circuit_lib, const CircuitModelId& circuit_model);

void add_reserved_sram_ports_to_module_manager(ModuleManager& module_manager, 
                                               const ModuleId& module_id,
                                               const size_t& port_size);

void add_formal_verification_sram_ports_to_module_manager(ModuleManager& module_manager, 
                                                          const ModuleId& module_id,
                                                          const CircuitLibrary& circuit_lib,
                                                          const CircuitModelId& sram_model,
                                                          const std::string& preproc_flag,
                                                          const size_t& port_size);

void add_sram_ports_to_module_manager(ModuleManager& module_manager, 
                                      const ModuleId& module_id,
                                      const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& sram_model,
                                      const e_sram_orgz sram_orgz_type,
                                      const size_t& num_config_bits);

void add_primitive_pb_type_ports_to_module_manager(ModuleManager& module_manager, 
                                                   const ModuleId& module_id,
                                                   t_pb_type* cur_pb_type);

void add_pb_type_ports_to_module_manager(ModuleManager& module_manager, 
                                         const ModuleId& module_id,
                                         t_pb_type* cur_pb_type);

bool module_net_is_local_wire(const ModuleManager& module_manager, 
                              const ModuleId& module_id, const ModuleNetId& module_net);

bool module_net_include_local_short_connection(const ModuleManager& module_manager, 
                                               const ModuleId& module_id, const ModuleNetId& module_net);

void add_primitive_pb_type_module_nets(ModuleManager& module_manager,
                                       const ModuleId& pb_type_module,
                                       const ModuleId& child_module,
                                       const size_t& child_instance_id,
                                       const CircuitLibrary& circuit_lib,
                                       t_pb_type* cur_pb_type);

void add_module_nets_between_logic_and_memory_sram_bus(ModuleManager& module_manager,
                                                       const ModuleId& parent_module,
                                                       const ModuleId& logic_module,
                                                       const size_t& logic_instance_id,
                                                       const ModuleId& memory_module,
                                                       const size_t& memory_instance_id, 
                                                       const CircuitLibrary& circuit_lib,
                                                       const CircuitModelId& logic_model);

void add_module_nets_memory_config_bus(ModuleManager& module_manager,
                                       const ModuleId& parent_module,
                                       const std::vector<ModuleId>& memory_modules,
                                       const std::vector<size_t>& memory_instances,
                                       const e_sram_orgz& sram_orgz_type, 
                                       const e_spice_model_design_tech& mem_tech);

size_t find_module_num_shared_config_bits(const ModuleManager& module_manager,
                                          const ModuleId& module_id);

size_t find_module_num_config_bits(const ModuleManager& module_manager,
                                   const ModuleId& module_id,
                                   const CircuitLibrary& circuit_lib,
                                   const CircuitModelId& sram_model,
                                   const e_sram_orgz& sram_orgz_type);

void add_module_global_ports_from_child_modules(ModuleManager& module_manager, 
                                                const ModuleId& module_id);

void add_module_gpio_ports_from_child_modules(ModuleManager& module_manager, 
                                              const ModuleId& module_id);

size_t find_module_num_shared_config_bits_from_child_modules(ModuleManager& module_manager, 
                                                             const ModuleId& module_id);

size_t find_module_num_config_bits_from_child_modules(ModuleManager& module_manager, 
                                                      const ModuleId& module_id,
                                                      const CircuitLibrary& circuit_lib,
                                                      const CircuitModelId& sram_model,
                                                      const e_sram_orgz& sram_orgz_type);

#endif
