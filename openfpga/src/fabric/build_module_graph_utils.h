#ifndef BUILD_MODULE_GRAPH_UTILS_H
#define BUILD_MODULE_GRAPH_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "vtr_geometry.h" 

#include "circuit_library.h" 
#include "openfpga_side_manager.h" 

#include "vpr_types.h" 
#include "module_manager.h" 

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

ModulePortId find_inverter_buffer_module_port(const ModuleManager& module_manager, 
                                              const ModuleId& module_id, 
                                              const CircuitLibrary& circuit_lib, 
                                              const CircuitModelId& model_id,
                                              const e_circuit_model_port_type& port_type);

ModuleNetId add_inverter_buffer_child_module_and_nets(ModuleManager& module_manager,
                                                      const ModuleId& parent_module, 
                                                      const CircuitLibrary& circuit_lib, 
                                                      const CircuitModelId& model_id, 
                                                      const ModuleNetId& input_net);

} /* end namespace openfpga */

#endif
