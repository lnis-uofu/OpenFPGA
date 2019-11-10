#ifndef ANALYSIS_SDC_WRITER_H
#define ANALYSIS_SDC_WRITER_H

#include <string>
#include "vpr_types.h"
#include "rr_blocks.h"
#include "module_manager.h"
#include "bitstream_manager.h"

void print_analysis_sdc(const std::string& sdc_dir,
                        const float& critical_path_delay,
                        const DeviceRRGSB& L_device_rr_gsb, 
                        const std::vector<t_logical_block>& L_logical_blocks,
                        const vtr::Point<size_t>& device_size,
                        const std::vector<std::vector<t_grid_tile>>& L_grids, 
                        const std::vector<t_block>& L_blocks,
                        const ModuleManager& module_manager,
                        const CircuitLibrary& circuit_lib,
                        const std::vector<CircuitPortId>& global_ports,
                        const bool& compact_routing_hierarchy);

#endif
