#ifndef BUILD_ROUTING_MODULES_H
#define BUILD_ROUTING_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "vpr_device_annotation.h"
#include "device_rr_gsb.h"
#include "mux_library.h"
#include "decoder_library.h"
#include "circuit_library.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void build_flatten_routing_modules(ModuleManager& module_manager,
                                   DecoderLibrary& decoder_lib,
                                   const DeviceContext& device_ctx,
                                   const VprDeviceAnnotation& device_annotation,
                                   const DeviceRRGSB& device_rr_gsb,
                                   const CircuitLibrary& circuit_lib,
                                   const e_config_protocol_type& sram_orgz_type,
                                   const CircuitModelId& sram_model,
                                   const bool& verbose);

void build_unique_routing_modules(ModuleManager& module_manager,
                                  DecoderLibrary& decoder_lib,
                                  const DeviceContext& device_ctx,
                                  const VprDeviceAnnotation& device_annotation,
                                  const DeviceRRGSB& device_rr_gsb,
                                  const CircuitLibrary& circuit_lib,
                                  const e_config_protocol_type& sram_orgz_type,
                                  const CircuitModelId& sram_model,
                                  const bool& verbose); 

} /* end namespace openfpga */

#endif
