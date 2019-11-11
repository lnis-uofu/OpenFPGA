#ifndef ANALYSIS_SDC_ROUTING_WRITER_H
#define ANALYSIS_SDC_ROUTING_WRITER_H

#include <fstream>
#include <vector>
#include "module_manager.h"
#include "rr_blocks.h"
#include "vpr_types.h"

void print_analysis_sdc_disable_unused_cbs(std::fstream& fp,
                                           const std::vector<std::vector<t_grid_tile>>& grids,
                                           const std::vector<t_switch_inf>& rr_switches,
                                           t_rr_node* L_rr_node,
                                           const ModuleManager& module_manager, 
                                           const CircuitLibrary& circuit_lib,
                                           const DeviceRRGSB& L_device_rr_gsb,
                                           const bool& compact_routing_hierarchy);
#endif
