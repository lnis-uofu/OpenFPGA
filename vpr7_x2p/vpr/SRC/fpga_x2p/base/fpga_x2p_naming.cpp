/************************************************
 * This file includes functions to 
 * generate module/port names for Verilog 
 * and SPICE netlists 
 ***********************************************/
#include "vtr_assert.h"

#include "fpga_x2p_naming.h"

/************************************************
 * Generate the module name for a multiplexer in Verilog format
 ***********************************************/
std::string generate_verilog_mux_subckt_name(const CircuitLibrary& circuit_lib, 
                                             const CircuitModelId& circuit_model, 
                                             const size_t& mux_size, 
                                             const std::string& postfix) {
  std::string module_name = circuit_lib.model_name(circuit_model); 
  module_name += "_size";
  module_name += std::to_string(mux_size);
  module_name += postfix;

  return module_name;
}

/************************************************
 * Generate the module name of a branch for a
 * multiplexer in Verilog format
 ***********************************************/
std::string generate_verilog_mux_branch_subckt_name(const CircuitLibrary& circuit_lib, 
                                                    const CircuitModelId& circuit_model, 
                                                    const size_t& mux_size, 
                                                    const std::string& postfix) {
  /* If the tgate spice model of this MUX is a MUX2 standard cell,
   * the mux_subckt name will be the name of the standard cell
   */
  CircuitModelId subckt_model = circuit_lib.pass_gate_logic_model(circuit_model);
  if (SPICE_MODEL_GATE == circuit_lib.model_type(subckt_model)) {
    VTR_ASSERT (SPICE_MODEL_GATE_MUX2 == circuit_lib.gate_type(subckt_model));
    return circuit_lib.model_name(subckt_model);
  }

  return generate_verilog_mux_subckt_name(circuit_lib, circuit_model, mux_size, postfix);
}
