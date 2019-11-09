#ifndef PNR_SDC_WRITER_H
#define PNR_SDC_WRITER_H

#include <string>
#include "vtr_geometry.h"
#include "vpr_types.h"
#include "rr_blocks.h"
#include "module_manager.h"
#include "mux_library.h"
#include "circuit_library.h"
#include "sdc_option.h"

void print_pnr_sdc(const SdcOption& sdc_options,
                   const float& critical_path_delay,
                   const std::vector<std::vector<t_grid_tile>>& grids,
                   const std::vector<t_switch_inf>& switches,
                   const DeviceRRGSB& L_device_rr_gsb,
                   const ModuleManager& module_manager,
                   const MuxLibrary& mux_lib,
                   const CircuitLibrary& circuit_lib,
                   const std::vector<CircuitPortId>& global_ports,
                   const bool& compact_routing_hierarchy);

#endif
