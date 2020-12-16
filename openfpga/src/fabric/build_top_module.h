#ifndef BUILD_TOP_MODULE_H
#define BUILD_TOP_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>
#include "vtr_geometry.h"
#include "device_grid.h"
#include "tile_annotation.h"
#include "rr_graph_obj.h"
#include "device_rr_gsb.h"
#include "circuit_library.h"
#include "decoder_library.h"
#include "tile_direct.h"
#include "arch_direct.h"
#include "config_protocol.h"
#include "module_manager.h"
#include "fabric_key.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int build_top_module(ModuleManager& module_manager,
                     DecoderLibrary& decoder_lib,
                     const CircuitLibrary& circuit_lib,
                     const DeviceGrid& grids,
                     const TileAnnotation& tile_annotation,
                     const RRGraph& rr_graph,
                     const DeviceRRGSB& device_rr_gsb,
                     const TileDirect& tile_direct,
                     const ArchDirect& arch_direct,
                     const ConfigProtocol& config_protocol,
                     const CircuitModelId& sram_model,
                     const bool& frame_view,
                     const bool& compact_routing_hierarchy,
                     const bool& duplicate_grid_pin,
                     const FabricKey& fabric_key,
                     const bool& generate_random_fabric_key);

} /* end namespace openfpga */

#endif
