#ifndef BUILD_MODULE_GRAPH_H
#define BUILD_MODULE_GRAPH_H

#include <vector>
#include "vpr_types.h"
#include "rr_blocks.h"
#include "mux_library.h"
#include "module_manager.h"

ModuleManager build_device_module_graph(const t_vpr_setup& vpr_setup,
                                        const t_arch& arch,
                                        const MuxLibrary& mux_lib,
                                        const std::vector<std::vector<t_grid_tile>>& grids,
                                        const std::vector<t_switch_inf>& rr_switches,
                                        const DeviceRRGSB& L_device_rr_gsb);

#endif
