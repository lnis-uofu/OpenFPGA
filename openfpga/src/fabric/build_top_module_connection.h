#ifndef BUILD_TOP_MODULE_CONNECTION_H
#define BUILD_TOP_MODULE_CONNECTION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "vtr_geometry.h"
#include "vtr_ndmatrix.h"
#include "device_grid.h"
#include "rr_graph_obj.h"
#include "device_rr_gsb.h"
#include "tile_annotation.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void add_top_module_nets_connect_grids_and_gsbs(ModuleManager& module_manager, 
                                                const ModuleId& top_module, 
                                                const DeviceGrid& grids,
                                                const vtr::Matrix<size_t>& grid_instance_ids,
                                                const RRGraph& rr_graph,
                                                const DeviceRRGSB& device_rr_gsb,
                                                const vtr::Matrix<size_t>& sb_instance_ids,
                                                const std::map<t_rr_type, vtr::Matrix<size_t>>& cb_instance_ids,
                                                const bool& compact_routing_hierarchy,
                                                const bool& duplicate_grid_pin);

int add_top_module_global_ports_from_grid_modules(ModuleManager& module_manager,
                                                  const ModuleId& top_module,
                                                  const TileAnnotation& tile_annotation,
                                                  const DeviceGrid& grids,
                                                  const vtr::Matrix<size_t>& grid_instance_ids);

} /* end namespace openfpga */

#endif
