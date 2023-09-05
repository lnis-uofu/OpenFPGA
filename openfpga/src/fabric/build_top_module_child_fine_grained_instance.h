#ifndef BUILD_TOP_MODULE_CHILD_FINE_GRAINED_INSTANCE_H
#define BUILD_TOP_MODULE_CHILD_FINE_GRAINED_INSTANCE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>

#include "arch_direct.h"
#include "circuit_library.h"
#include "clock_network.h"
#include "config_protocol.h"
#include "decoder_library.h"
#include "device_grid.h"
#include "device_rr_gsb.h"
#include "fabric_key.h"
#include "fabric_tile.h"
#include "memory_bank_shift_register_banks.h"
#include "module_manager.h"
#include "rr_clock_spatial_lookup.h"
#include "rr_graph_view.h"
#include "tile_annotation.h"
#include "tile_direct.h"
#include "vpr_device_annotation.h"
#include "vtr_geometry.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int build_top_module_fine_grained_child_instances(
  ModuleManager& module_manager, const ModuleId& top_module,
  MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const CircuitLibrary& circuit_lib, const ClockNetwork& clk_ntwk,
  const RRClockSpatialLookup& rr_clock_lookup,
  const VprDeviceAnnotation& vpr_device_annotation, const DeviceGrid& grids,
  const size_t& layer, const TileAnnotation& tile_annotation,
  const RRGraphView& rr_graph, const DeviceRRGSB& device_rr_gsb,
  const TileDirect& tile_direct, const ArchDirect& arch_direct,
  const ConfigProtocol& config_protocol, const CircuitModelId& sram_model,
  const bool& frame_view, const bool& compact_routing_hierarchy,
  const bool& duplicate_grid_pin, const FabricKey& fabric_key,
  const bool& group_config_block);

} /* end namespace openfpga */

#endif
