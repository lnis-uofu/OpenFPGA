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

ModuleManager build_device_module_graph(const DeviceContext& vpr_device_ctx,
                                        const OpenfpgaContext& openfpga_ctx,
                                        const bool& duplicate_grid_pin,
                                        const bool& verbose);

} /* end namespace openfpga */

#endif
