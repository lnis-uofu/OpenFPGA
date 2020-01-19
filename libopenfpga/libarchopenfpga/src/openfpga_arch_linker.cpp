/********************************************************************
 * This file includes the functions that build links between
 * data structures inside the openfpga arch data structure
 *******************************************************************/
#include "vtr_log.h"
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
