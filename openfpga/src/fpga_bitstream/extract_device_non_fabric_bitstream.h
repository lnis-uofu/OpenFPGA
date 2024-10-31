#ifndef EXTRACT_DEVICE_NON_FABRIC_BITSTREAM_H
#define EXTRACT_DEVICE_NON_FABRIC_BITSTREAM_H

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

void extract_device_non_fabric_bitstream(const VprContext& vpr_ctx,
                                         const OpenfpgaContext& openfpga_ctx,
                                         const bool& verbose);

} /* end namespace openfpga */

#endif
