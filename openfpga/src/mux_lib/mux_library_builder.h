#ifndef MUX_LIBRARY_BUILDER_H
#define MUX_LIBRARY_BUILDER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "mux_library.h"
#include "openfpga_context.h"
#include "vpr_context.h"

class RRGraphInEdges;

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

MuxLibrary build_device_mux_library(const DeviceContext& vpr_device_ctx,
                                    const OpenfpgaContext& openfpga_ctx,
                                    const RRGraphInEdges& in_edges);

} /* end namespace openfpga */

#endif
