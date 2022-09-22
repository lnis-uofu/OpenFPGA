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
    VTR_LOG("Invalid memory model name '%s' defined in <configuration_protocol>!",
            openfpga_arch.config_protocol.memory_model_name().c_str());
    exit(1);
  }

  openfpga_arch.config_protocol.set_memory_model(config_memory_model); 

  /* Optional: we need to bind the memory model for BL/WL protocols */
  if (!openfpga_arch.config_protocol.bl_memory_model_name().empty()) {
    CircuitModelId bl_memory_model = openfpga_arch.circuit_lib.model(openfpga_arch.config_protocol.bl_memory_model_name());
    /* Error out if the circuit model id is invalid */
    if (CircuitModelId::INVALID() == bl_memory_model) {
      VTR_LOG("Invalid bl memory model name '%s' defined in <configuration_protocol>!",
              openfpga_arch.config_protocol.bl_memory_model_name().c_str());
      exit(1);
    }
    openfpga_arch.config_protocol.set_bl_memory_model(bl_memory_model); 
  }

  if (!openfpga_arch.config_protocol.wl_memory_model_name().empty()) {
    CircuitModelId wl_memory_model = openfpga_arch.circuit_lib.model(openfpga_arch.config_protocol.wl_memory_model_name());
    /* Error out if the circuit model id is invalid */
    if (CircuitModelId::INVALID() == wl_memory_model) {
      VTR_LOG("Invalid wl memory model name '%s' defined in <configuration_protocol>!",
              openfpga_arch.config_protocol.wl_memory_model_name().c_str());
      exit(1);
    }
    openfpga_arch.config_protocol.set_wl_memory_model(wl_memory_model); 
  }
}

/********************************************************************
 * Link the circuit model of circuit library
 * to these device model defined in technology library
 *******************************************************************/
void bind_circuit_model_to_technology_model(openfpga::Arch& openfpga_arch) {
  /* Ensure a clean start */
  openfpga_arch.circuit_tech_binding.clear();

  for (const CircuitModelId& circuit_model : openfpga_arch.circuit_lib.models()) {
    const std::string device_model_name = openfpga_arch.circuit_lib.device_model_name(circuit_model);
    if (true == device_model_name.empty()) {
      continue;
    }
    /* Try to find the device model name in technology library */
    TechnologyModelId tech_model = openfpga_arch.tech_lib.model(device_model_name);
    if (false == openfpga_arch.tech_lib.valid_model_id(tech_model)) {
      VTR_LOG("Invalid device model name '%s' defined in circuit model '%s'!",
              device_model_name.c_str(),
              openfpga_arch.circuit_lib.model_name(circuit_model).c_str());
      exit(1);
    }
    /* Create binding */
    openfpga_arch.circuit_tech_binding[circuit_model] = tech_model;
  }
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
