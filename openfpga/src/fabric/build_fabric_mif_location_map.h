#ifndef BUILD_FABRIC_MIF_LOCATION_MAP_H
#define BUILD_FABRIC_MIF_LOCATION_MAP_H

/********************************************************************
 * Build a fabric-side lookup from VPR grid (x, y, z) to each physical
 * MIF instance's bit offset on a top-level MIF data bus.
 *
 * Offset order follows module_graph GPIN concatenation (io_children),
 * matching how is_mif_data_bus ports are wired at fabric top.
 *
 * Independent of annotate_mif / MifPipeline data flow.
 *******************************************************************/

#include "bitstream_setting.h"
#include "circuit_library.h"
#include "device_grid.h"
#include "mif_location_map.h"
#include "module_manager.h"

/* begin namespace openfpga */
namespace openfpga {

MifLocationMap build_fabric_mif_location_map(
  const ModuleManager& module_manager, const DeviceGrid& grids,
  const CircuitLibrary& circuit_lib, const BitstreamSetting& bitstream_setting,
  const bool& tiled_fabric);

} /* end namespace openfpga */

#endif
