/********************************************************************
 * This file includes most utilized functions that are used to
 * build module graphs
 ********************************************************************/
#include <vector>
#include "vtr_assert.h"

#include "build_module_graph_utils.h" 

/********************************************************************
 * Find input port of a buffer/inverter module
 ********************************************************************/
ModulePortId find_inverter_buffer_module_port(const ModuleManager& module_manager, 
                                              const ModuleId& module_id, 
                                              const CircuitLibrary& circuit_lib, 
                                              const CircuitModelId& model_id,
                                              const e_spice_model_port_type& port_type) {
  /* We must have a valid module id */
  VTR_ASSERT(true == module_manager.valid_module_id(module_id));
  /* Check the type of model */ 
  VTR_ASSERT(SPICE_MODEL_INVBUF == circuit_lib.model_type(model_id));

  /* Add module nets to wire to the buffer module */
  /* To match the context, Buffer should have only 2 non-global ports: 1 input port and 1 output port */
  std::vector<CircuitPortId> model_ports = circuit_lib.model_ports_by_type(model_id, port_type, true);
  VTR_ASSERT(1 == model_ports.size());

  /* Find the input and output module ports */
  ModulePortId module_port_id = module_manager.find_module_port(module_id, circuit_lib.port_lib_name(model_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(module_id, module_port_id));

  return module_port_id;  
}

/********************************************************************
 * Add inverter/buffer module to a parent module
 * and complete the wiring to the input port of inverter/buffer
 * This function will return the wire created for the output port of inverter/buffer
 *
 *  parent_module
 *  +-----------------------------------------------------------------
 *  |       
 *  |                 input_net                    output_net
 *  |                    |                           |
 *  |                    v      +---------------+    v
 *  | src_module_port --------->|  child_module |--------> 
 *  |                           +---------------+
 *
 ********************************************************************/
ModuleNetId add_inverter_buffer_child_module_and_nets(ModuleManager& module_manager,
                                                      const ModuleId& parent_module, 
                                                      const CircuitLibrary& circuit_lib, 
                                                      const CircuitModelId& model_id, 
                                                      const ModuleNetId& input_net) {
  /* We must have a valid module id */
  VTR_ASSERT(true == module_manager.valid_module_id(parent_module));

  std::string module_name = circuit_lib.model_name(model_id);
  ModuleId child_module = module_manager.find_module(module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(child_module));

  ModulePortId module_input_port_id = find_inverter_buffer_module_port(module_manager, child_module, circuit_lib, model_id, SPICE_MODEL_PORT_INPUT);
  ModulePortId module_output_port_id = find_inverter_buffer_module_port(module_manager, child_module, circuit_lib, model_id, SPICE_MODEL_PORT_OUTPUT);

  /* Port size should be 1 ! */
  VTR_ASSERT(1 == module_manager.module_port(child_module, module_input_port_id).get_width());
  VTR_ASSERT(1 == module_manager.module_port(child_module, module_output_port_id).get_width());

  /* Instanciate a child module */
  size_t child_instance = module_manager.num_instance(parent_module, child_module);
  module_manager.add_child_module(parent_module, child_module);

  /* Use the net to connect to the input net of buffer */
  module_manager.add_module_net_sink(parent_module, input_net, child_module, child_instance, module_input_port_id, 0);

  /* Create a net to bridge the input inverter and LUT MUX */ 
  ModuleNetId output_net = module_manager.create_module_net(parent_module);
  module_manager.add_module_net_source(parent_module, output_net, child_module, child_instance, module_output_port_id, 0);

  return output_net;
}

