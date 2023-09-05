#ifndef BUILD_GRID_BITSTREAM_H
#define BUILD_GRID_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>

#include "bitstream_manager.h"
#include "circuit_library.h"
#include "device_grid.h"
#include "fabric_tile.h"
#include "module_manager.h"
#include "mux_library.h"
#include "vpr_bitstream_annotation.h"
#include "vpr_clustering_annotation.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"
#include "vpr_placement_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void build_grid_bitstream(
  BitstreamManager& bitstream_manager, const ConfigBlockId& top_block,
  const ModuleManager& module_manager, const FabricTile& fabric_tile,
  const CircuitLibrary& circuit_lib, const MuxLibrary& mux_lib,
  const DeviceGrid& grids, const size_t& layer, const AtomContext& atom_ctx,
  const VprDeviceAnnotation& device_annotation,
  const VprClusteringAnnotation& cluster_annotation,
  const VprPlacementAnnotation& place_annotation,
  const VprBitstreamAnnotation& bitstream_annotation, const bool& verbose);

} /* end namespace openfpga */

#endif
