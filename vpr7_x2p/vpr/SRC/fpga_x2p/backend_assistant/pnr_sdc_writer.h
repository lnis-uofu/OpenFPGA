#ifndef PNR_SDC_WRITER_H
#define PNR_SDC_WRITER_H

#include <string>
#include "vtr_geometry.h"
#include "vpr_types.h"
#include "rr_blocks.h"
#include "module_manager.h"
#include "sdc_option.h"

void print_pnr_sdc(const SdcOption& sdc_options,
                   const float& critical_path_delay,
                   const CircuitLibrary& circuit_lib,
                   const ModuleManager& module_manager,
                   const std::vector<CircuitPortId>& global_ports);

#endif
