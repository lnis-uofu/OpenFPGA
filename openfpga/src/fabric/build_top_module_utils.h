#ifndef BUILD_TOP_MODULE_UTILS_H
#define BUILD_TOP_MODULE_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include <string>
#include "vtr_geometry.h"
#include "device_grid.h"
#include "rr_gsb.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::string generate_grid_block_module_name_in_top_module(const std::string& prefix,
                                                          const DeviceGrid& grids,
                                                          const vtr::Point<size_t>& grid_coord);

t_rr_type find_top_module_cb_type_by_sb_side(const e_side& sb_side);

vtr::Point<size_t> find_top_module_gsb_coordinate_by_sb_side(const RRGSB& rr_gsb,
                                                             const e_side& sb_side);

} /* end namespace openfpga */

#endif
