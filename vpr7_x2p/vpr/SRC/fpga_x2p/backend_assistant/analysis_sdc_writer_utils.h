#ifndef ANALYSIS_SDC_WRITER_UTILS_H
#define ANALYSIS_SDC_WRITER_UTILS_H

#include <fstream>
#include <string>
#include <map>
#include "module_manager.h"
#include "vpr_types.h"

bool is_rr_node_to_be_disable_for_analysis(t_rr_node* cur_rr_node);

void disable_analysis_module_input_pin_net_sinks(std::fstream& fp,
                                                 const ModuleManager& module_manager,
                                                 const ModuleId& parent_module,
                                                 const std::string& parent_instance_name,
                                                 const ModulePortId& module_input_port,
                                                 const size_t& module_input_pin,
                                                 t_rr_node* input_rr_node,
                                                 const std::map<std::string, int> mux_instance_to_net_map);

void disable_analysis_module_input_port_net_sinks(std::fstream& fp,
                                                  const ModuleManager& module_manager,
                                                  const ModuleId& parent_module,
                                                  const std::string& parent_instance_name,
                                                  const ModulePortId& module_input_port,
                                                  t_rr_node* input_rr_node,
                                                  const std::map<std::string, int> mux_instance_to_net_map) ;

void disable_analysis_module_output_pin_net_sinks(std::fstream& fp,
                                                  const ModuleManager& module_manager,
                                                  const ModuleId& parent_module,
                                                  const std::string& parent_instance_name,
                                                  const ModuleId& child_module,
                                                  const size_t& child_instance,
                                                  const ModulePortId& child_module_port,
                                                  const size_t& child_module_pin,
                                                  t_rr_node* output_rr_node,
                                                  const std::map<std::string, int> mux_instance_to_net_map);

#endif
