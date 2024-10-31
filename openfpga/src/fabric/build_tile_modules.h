#ifndef BUILD_TILE_MODULES_H
#define BUILD_TILE_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>

#include "circuit_library.h"
#include "config_protocol.h"
#include "decoder_library.h"
#include "device_grid.h"
#include "device_rr_gsb.h"
#include "fabric_tile.h"
#include "module_manager.h"
#include "rr_graph_view.h"
#include "tile_annotation.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int build_tile_modules(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const FabricTile& fabric_tile, const DeviceGrid& grids,
  const VprDeviceAnnotation& vpr_device_annotation,
  const DeviceRRGSB& device_rr_gsb, const RRGraphView& rr_graph_view,
  const TileAnnotation& tile_annotation, const CircuitLibrary& circuit_lib,
  const CircuitModelId& sram_model,
  const e_config_protocol_type& sram_orgz_type,
  const bool& name_module_using_index, const bool& perimeter_cb,
  const bool& frame_view, const bool& verbose);

} /* end namespace openfpga */

#endif
