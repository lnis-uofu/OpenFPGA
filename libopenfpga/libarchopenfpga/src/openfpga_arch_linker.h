#ifndef OPENFPGA_ARCH_LINKER_H
#define OPENFPGA_ARCH_LINKER_H

#include "openfpga_arch.h"

void link_config_protocol_to_circuit_library(openfpga::Arch& openfpga_arch);

void config_circuit_models_sram_port_to_default_sram_model(CircuitLibrary& circuit_lib,
                                                           const CircuitModelId& default_sram_model);

#endif
