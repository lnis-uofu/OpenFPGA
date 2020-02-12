/********************************************************************
 * This file includes the functions that build links between
 * data structures inside the openfpga arch data structure
 *******************************************************************/
#include "vtr_log.h"
#include "vtr_assert.h"

#include "openfpga_arch_linker.h"

/********************************************************************
 * Link the circuit model inside configuration protocol
 * to these circuit models defined in circuit library
 *******************************************************************/
void link_config_protocol_to_circuit_library(openfpga::Arch& openfpga_arch) {
  CircuitModelId config_memory_model = openfpga_arch.circuit_lib.model(openfpga_arch.config_protocol.memory_model_name());

  /* Error out if the circuit model id is invalid */
  if (CircuitModelId::INVALID() == config_memory_model) {
    VTR_LOG("Invalid memory model name (=%s) defined in <configuration_protocol>!",
            openfpga_arch.config_protocol.memory_model_name().c_str());
    exit(1);
  }

  openfpga_arch.config_protocol.set_memory_model(config_memory_model); 
}

/********************************************************************
 * Link the circuit model of SRAM ports of each circuit model
 * to a default SRAM circuit model.
 * This function aims to ease the XML writing, allowing users to skip
 * the circuit model definition for SRAM ports that are used by default
 * TODO: Maybe deprecated as we prefer strict definition 
 *******************************************************************/
void config_circuit_models_sram_port_to_default_sram_model(CircuitLibrary& circuit_lib,
                                                           const CircuitModelId& default_sram_model) {
  VTR_ASSERT(CircuitModelId::INVALID() != default_sram_model);

  for (const auto& model : circuit_lib.models()) {
    for (const auto& port : circuit_lib.model_ports(model)) {
      /* Bypass non SRAM ports */
      if (CIRCUIT_MODEL_PORT_SRAM != circuit_lib.port_type(port)) {
        continue;
      }

      /* Write for the default SRAM SPICE model! */
      circuit_lib.set_port_tri_state_model_id(port, default_sram_model);

      /* Only show warning when we try to override the given spice_model_name ! */ 
      if (true == circuit_lib.port_tri_state_model_name(port).empty()) { 
        VTR_LOG("Use the default configurable memory model '%s' for circuit model '%s' port '%s')\n",
                circuit_lib.model_name(default_sram_model).c_str(),
                circuit_lib.model_name(model).c_str(),
                circuit_lib.port_prefix(port).c_str());
        continue;
      }

      /* Give a warning !!! */
      if (circuit_lib.model_name(default_sram_model) != circuit_lib.port_tri_state_model_name(port)) {
        VTR_LOG_WARN("Overwrite SRAM circuit model for circuit model port (name:%s, port:%s) to be the correct one (name:%s)!\n",
                     circuit_lib.model_name(model).c_str(),
                     circuit_lib.port_prefix(port).c_str(),
                     circuit_lib.model_name(default_sram_model).c_str());
      }
    }
  }
  /* Rebuild the submodels for circuit_library
   * because we have created links for ports 
   */
  circuit_lib.build_model_links();
}
