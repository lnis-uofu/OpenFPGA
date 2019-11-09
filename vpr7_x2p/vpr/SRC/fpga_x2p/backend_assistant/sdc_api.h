#ifndef SDC_API_H
#define SDC_API_H

#include <vector>
#include "sdc_option.h"
#include "circuit_library.h"
#include "mux_library.h"
#include "module_manager.h"

void fpga_sdc_generator(const SdcOption& sdc_options,
                        const float& critical_path_delay,
                        const MuxLibrary& mux_lib,
                        const CircuitLibrary& circuit_lib,
                        const ModuleManager& module_manager,
                        const std::vector<CircuitPortId>& global_ports);

#endif
