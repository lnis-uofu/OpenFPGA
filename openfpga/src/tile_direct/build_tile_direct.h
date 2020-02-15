#ifndef BUILD_TILE_DIRECT_H
#define BUILD_TILE_DIRECT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "arch_direct.h"
#include "tile_direct.h"
#include "circuit_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

TileDirect build_device_tile_direct(const DeviceContext& device_ctx,
                                    const ArchDirect& arch_direct,
                                    const CircuitLibrary& circuit_lib);

} /* end namespace openfpga */

#endif
