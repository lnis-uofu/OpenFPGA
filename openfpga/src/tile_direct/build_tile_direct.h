#ifndef BUILD_TILE_DIRECT_H
#define BUILD_TILE_DIRECT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "arch_direct.h"
#include "circuit_library.h"
#include "tile_direct.h"
#include "vpr_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

TileDirect build_device_tile_direct(const DeviceContext& device_ctx,
                                    const ArchDirect& arch_direct,
                                    const bool& verbose);

} /* end namespace openfpga */

#endif
