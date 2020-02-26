#ifndef BUILD_GRID_BITSTREAM_H
#define BUILD_GRID_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "device_grid.h"
#include "bitstream_manager.h"
#include "module_manager.h"
#include "circuit_library.h"
#include "mux_library.h"
#include "vpr_device_annotation.h"
#include "vpr_clustering_annotation.h"
#include "vpr_placement_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void build_grid_bitstream(BitstreamManager& bitstream_manager,
                          const ConfigBlockId& top_block,
                          const ModuleManager& module_manager,
                          const CircuitLibrary& circuit_lib,
                          const MuxLibrary& mux_lib,
                          const DeviceGrid& grids,
                          const VprDeviceAnnotation& device_annotation,
                          const VprClusteringAnnotation& cluster_annotation,
                          const VprPlacementAnnotation& place_annotation,
                          const bool& verbose);

} /* end namespace openfpga */

#endif
