#ifndef BUILD_DEVICE_MODULE_H
#define BUILD_DEVICE_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

ModuleManager build_device_module_graph(IoLocationMap& io_location_map,
                                        DecoderLibrary& decoder_lib,
                                        const OpenfpgaContext& openfpga_ctx,
                                        const DeviceContext& vpr_device_ctx,
                                        const bool& compress_routing,
                                        const bool& duplicate_grid_pin,
                                        const bool& verbose);

} /* end namespace openfpga */

#endif
