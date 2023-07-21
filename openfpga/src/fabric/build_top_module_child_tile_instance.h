#ifndef BUILD_TOP_MODULE_CHILD_TILE_INSTANCE_H
#define BUILD_TOP_MODULE_CHILD_TILE_INSTANCE_H

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

int build_top_module_tile_child_instances(
  ModuleManager& module_manager, const ModuleId& top_module,
  MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const CircuitLibrary& circuit_lib, const DeviceGrid& grids,
  const FabricTile& fabric_tile, const ConfigProtocol& config_protocol,
  const FabricKey& fabric_key, const bool& frame_view,
  const bool& verbose);

} /* end namespace openfpga */

#endif
