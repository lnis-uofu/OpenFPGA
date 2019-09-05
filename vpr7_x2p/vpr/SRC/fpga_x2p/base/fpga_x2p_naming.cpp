/************************************************
 * This file includes functions to 
 * generate module/port names for Verilog 
 * and SPICE netlists 
 ***********************************************/
#include "vtr_assert.h"

#include "fpga_x2p_naming.h"

/************************************************
 * Generate the node name for a multiplexing structure 
 * Case 1 : If there is an intermediate buffer followed by,
 *          the node name will be mux_l<node_level>_in_buf
 * Case 1 : If there is NO intermediate buffer followed by,
 *          the node name will be mux_l<node_level>_in
 ***********************************************/
std::string generate_verilog_mux_node_name(const size_t& node_level, 
                                           const bool& add_buffer_postfix) {
  /* Generate the basic node_name */
  std::string node_name = "mux_l" + std::to_string(node_level) + "_in";

  /* Add a postfix upon requests */
  if (true == add_buffer_postfix)  {
    /* '1' indicates that the location is needed */
    node_name += "_buf";
  }

  return node_name;
}

/************************************************
 * Generate the module name for a multiplexer in Verilog format
 * Different circuit model requires different names: 
 * 1. LUTs are named as <model_name>_mux
 * 2. MUXes are named as <model_name>_size<num_inputs>
 ***********************************************/
std::string generate_verilog_mux_subckt_name(const CircuitLibrary& circuit_lib, 
                                             const CircuitModelId& circuit_model, 
                                             const size_t& mux_size, 
                                             const std::string& postfix) {
  std::string module_name = circuit_lib.model_name(circuit_model); 
  /* Check the model type and give different names */
  if (SPICE_MODEL_MUX == circuit_lib.model_type(circuit_model)) {
    module_name += "_size";
    module_name += std::to_string(mux_size);
  } else {  
    VTR_ASSERT(SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model));
    module_name += "_mux";
  }

  /* Add postfix if it is not empty */
  if (!postfix.empty()) {
    module_name += postfix;
  }

  return module_name;
}

/************************************************
 * Generate the module name of a branch for a
 * multiplexer in Verilog format
 ***********************************************/
std::string generate_verilog_mux_branch_subckt_name(const CircuitLibrary& circuit_lib, 
                                                    const CircuitModelId& circuit_model, 
                                                    const size_t& mux_size, 
                                                    const size_t& branch_mux_size, 
                                                    const std::string& postfix) {
  /* If the tgate spice model of this MUX is a MUX2 standard cell,
   * the mux_subckt name will be the name of the standard cell
   */
  CircuitModelId subckt_model = circuit_lib.pass_gate_logic_model(circuit_model);
  if (SPICE_MODEL_GATE == circuit_lib.model_type(subckt_model)) {
    VTR_ASSERT (SPICE_MODEL_GATE_MUX2 == circuit_lib.gate_type(subckt_model));
    return circuit_lib.model_name(subckt_model);
  }
  std::string branch_postfix = postfix + "_size" + std::to_string(branch_mux_size);

  return generate_verilog_mux_subckt_name(circuit_lib, circuit_model, mux_size, branch_postfix);
}
