#ifndef PNR_SDC_ROUTING_WRITER_H
#define PNR_SDC_ROUTING_WRITER_H

#include <string>
#include <vector>
#include "module_manager.h"
#include "rr_blocks.h"
#include "vpr_types.h"

void print_pnr_sdc_flatten_routing_constrain_sb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager,
                                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                                       const std::vector<t_switch_inf>& switches,
                                                       const DeviceRRGSB& L_device_rr_gsb);

void print_pnr_sdc_compact_routing_constrain_sb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager,
                                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                                       const std::vector<t_switch_inf>& switches,
                                                       const DeviceRRGSB& L_device_rr_gsb);

void print_pnr_sdc_flatten_routing_constrain_cb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager, 
                                                       const DeviceRRGSB& L_device_rr_gsb,
                                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                                       const std::vector<t_switch_inf>& switches);

void print_pnr_sdc_compact_routing_constrain_cb_timing(const std::string& sdc_dir,
                                                       const ModuleManager& module_manager,
                                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                                       const std::vector<t_switch_inf>& switches,
                                                       const DeviceRRGSB& L_device_rr_gsb);

#endif
