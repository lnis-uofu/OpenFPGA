#ifndef SDC_API_H
#define SDC_API_H

#include <vector>
#include "sdc_option.h"
#include "circuit_library.h"
#include "mux_library.h"
#include "module_manager.h"

void fpga_sdc_generator(const SdcOption& sdc_options,
                        const float& critical_path_delay,
                        const std::vector<t_switch_inf>& rr_switches,
                        const DeviceRRGSB& L_device_rr_gsb,
                        const std::vector<t_logical_block>& L_logical_blocks,
                        const vtr::Point<size_t>& device_size,
                        const std::vector<std::vector<t_grid_tile>>& L_grids, 
                        const std::vector<t_block>& L_blocks,
                        const ModuleManager& module_manager,
                        const MuxLibrary& mux_lib,
                        const CircuitLibrary& circuit_lib,
                        const std::vector<CircuitPortId>& global_ports,
                        const bool& compact_routing_hierarchy);

#endif
