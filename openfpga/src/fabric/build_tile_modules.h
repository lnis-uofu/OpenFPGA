#ifndef BUILD_TILE_MODULES_H
#define BUILD_TILE_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>

#include "circuit_library.h"
#include "config_protocol.h"
#include "device_grid.h"
#include "device_rr_gsb.h"
#include "fabric_tile.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int build_tile_modules(ModuleManager& module_manager,
                       const FabricTile& fabric_tile, const DeviceGrid& grids,
                       const DeviceRRGSB& device_rr_gsb,
                       const CircuitLibrary& circuit_lib,
                       const CircuitModelId& sram_model,
                       const e_config_protocol_type& sram_orgz_type,
                       const bool& verbose);

} /* end namespace openfpga */

#endif
