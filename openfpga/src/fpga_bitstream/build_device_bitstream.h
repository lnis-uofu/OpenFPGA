#ifndef BUILD_DEVICE_BITSTREAM_H
#define BUILD_DEVICE_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>

#include "openfpga_context.h"
#include "vpr_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

BitstreamManager build_device_bitstream(const VprContext& vpr_ctx,
                                        const OpenfpgaContext& openfpga_ctx,
                                        const bool& prefer_unused_mux_input,
                                        const bool& verbose);

} /* end namespace openfpga */

#endif
