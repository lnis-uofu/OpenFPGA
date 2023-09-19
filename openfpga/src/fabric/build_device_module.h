#ifndef BUILD_DEVICE_MODULE_H
#define BUILD_DEVICE_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "fabric_key.h"
#include "fabric_tile.h"
#include "io_name_map.h"
#include "module_name_map.h"
#include "openfpga_context.h"
#include "tile_config.h"
#include "vpr_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int build_device_module_graph(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  MemoryBankShiftRegisterBanks& blwl_sr_banks, FabricTile& fabric_tile,
  ModuleNameMap& module_name_map, const OpenfpgaContext& openfpga_ctx,
  const DeviceContext& vpr_device_ctx, const bool& frame_view,
  const bool& compress_routing, const bool& duplicate_grid_pin,
  const FabricKey& fabric_key, const TileConfig& tile_config,
  const bool& group_config_block, const bool& name_module_using_index,
  const bool& generate_random_fabric_key, const bool& verbose);

} /* end namespace openfpga */

#endif
