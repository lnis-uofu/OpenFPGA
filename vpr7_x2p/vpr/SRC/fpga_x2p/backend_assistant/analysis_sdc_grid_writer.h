#ifndef ANALYSIS_SDC_GRID_WRITER_H
#define ANALYSIS_SDC_GRID_WRITER_H

#include <fstream>
#include <vector>
#include "vtr_geometry.h"
#include "vpr_types.h"
#include "module_manager.h"

void print_analysis_sdc_disable_unused_grids(std::fstream& fp, 
                                             const vtr::Point<size_t>& device_size,
                                             const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                             const std::vector<t_block>& L_blocks,
                                             const ModuleManager& module_manager);

#endif
