#ifndef BUILD_TOP_MODULE_H
#define BUILD_TOP_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>
#include "vtr_geometry.h"
#include "device_grid.h"
#include "rr_graph_obj.h"
#include "device_rr_gsb.h"
#include "circuit_library.h"
#include "tile_direct.h"
#include "arch_direct.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void build_top_module(ModuleManager& module_manager,
                      const CircuitLibrary& circuit_lib,
                      const DeviceGrid& grids,
                      const RRGraph& rr_graph,
                      const DeviceRRGSB& device_rr_gsb,
                      const TileDirect& tile_direct,
                      const ArchDirect& arch_direct,
                      const e_config_protocol_type& sram_orgz_type,
                      const CircuitModelId& sram_model,
                      const bool& compact_routing_hierarchy,
                      const bool& duplicate_grid_pin);

} /* end namespace openfpga */

#endif
