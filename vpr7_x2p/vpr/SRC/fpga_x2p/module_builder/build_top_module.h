/********************************************************************
 * Header file for build_top_module.cpp
 *******************************************************************/
#ifndef BUILD_TOP_MODULE_H
#define BUILD_TOP_MODULE_H

#include <string>
#include "vtr_geometry.h"
#include "vpr_types.h"
#include "spice_types.h"
#include "rr_blocks.h"
#include "circuit_library.h"
#include "module_manager.h"

void build_top_module(ModuleManager& module_manager,
                      const CircuitLibrary& circuit_lib,
                      const vtr::Point<size_t>& device_size,
                      const std::vector<std::vector<t_grid_tile>>& grids,
                      const DeviceRRGSB& L_device_rr_gsb,
                      const std::vector<t_clb_to_clb_directs>& clb2clb_directs,
                      const e_sram_orgz& sram_orgz_type,
                      const CircuitModelId& sram_model,
                      const bool& compact_routing_hierarchy,
                      const bool& duplicate_grid_pin);

#endif
