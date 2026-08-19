#ifndef BUILD_FABRIC_MIF_LOCATION_MAP_H
#define BUILD_FABRIC_MIF_LOCATION_MAP_H

/********************************************************************
 * Build a fabric-side lookup:
 *   top-level MIF data port -> (physical loc, physical pb_graph_node)
 *
 * Physical MIF primitives come from VprBitstreamAnnotation, remapped
 * through VprDeviceAnnotation. Port names come from the circuit models
 * bound to those primitives.
 *******************************************************************/

#include "circuit_library.h"
#include "device_grid.h"
#include "mif_location_map.h"
#include "module_manager.h"
#include "vpr_bitstream_annotation.h"
#include "vpr_device_annotation.h"

/* begin namespace openfpga */
namespace openfpga {

MifLocationMap build_fabric_mif_location_map(
  const ModuleManager& module_manager, const DeviceGrid& grids,
  const CircuitLibrary& circuit_lib,
  const VprBitstreamAnnotation& bitstream_annotation,
  const VprDeviceAnnotation& device_annotation, const bool& tiled_fabric);

} /* end namespace openfpga */

#endif
