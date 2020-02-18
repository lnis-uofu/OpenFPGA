#ifndef BUILD_PHYSICAL_LB_RR_GRAPH_H
#define BUILD_PHYSICAL_LB_RR_GRAPH_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void build_physical_lb_rr_graphs(const DeviceContext& device_ctx,
                                 VprDeviceAnnotation& device_annotation,
                                 const bool& verbose);

} /* end namespace openfpga */

#endif
