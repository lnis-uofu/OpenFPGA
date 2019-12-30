#ifndef BUILD_TOP_MODULE_UTILS_H
#define BUILD_TOP_MODULE_UTILS_H

#include <vector>
#include <string>
#include "vtr_geometry.h"
#include "vpr_types.h"
#include "sides.h"

std::string generate_grid_block_module_name_in_top_module(const std::string& prefix,
                                                          const vtr::Point<size_t>& device_size,
                                                          const std::vector<std::vector<t_grid_tile>>& grids,
                                                          const vtr::Point<size_t>& grid_coordinate);

t_rr_type find_top_module_cb_type_by_sb_side(const e_side& sb_side);

DeviceCoordinator find_top_module_gsb_coordinate_by_sb_side(const RRGSB& rr_gsb,
                                                            const e_side& sb_side);

#endif
