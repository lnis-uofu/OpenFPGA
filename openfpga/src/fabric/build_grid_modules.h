#ifndef BUILD_GRID_MODULES_H
#define BUILD_GRID_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "decoder_library.h"
#include "module_manager.h"
#include "mux_library.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int build_grid_modules(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const DeviceContext& device_ctx, const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib, const MuxLibrary& mux_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, const bool& duplicate_grid_pin,
  const bool& group_config_block, const bool& verbose);

} /* end namespace openfpga */

#endif
