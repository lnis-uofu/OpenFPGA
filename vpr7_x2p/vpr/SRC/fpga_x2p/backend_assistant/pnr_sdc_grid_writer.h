#ifndef PNR_SDC_GRID_WRITER_H
#define PNR_SDC_GRID_WRITER_H

#include <string>
#include <vector>
#include "vpr_types.h"

void print_pnr_sdc_constrain_grid_timing(const std::string& sdc_dir,
                                         const ModuleManager& module_manager);

#endif
