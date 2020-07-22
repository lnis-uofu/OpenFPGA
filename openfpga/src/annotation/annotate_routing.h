#ifndef ANNOTATE_ROUTING_H
#define ANNOTATE_ROUTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "openfpga_context.h"
#include "vpr_routing_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void annotate_rr_node_nets(const DeviceContext& device_ctx,
                           const ClusteringContext& clustering_ctx,
                           const RoutingContext& routing_ctx,
                           VprRoutingAnnotation& vpr_routing_annotation,
                           const bool& verbose);

void annotate_rr_node_previous_nodes(const DeviceContext& device_ctx,
                                     const ClusteringContext& clustering_ctx,
                                     const RoutingContext& routing_ctx,
                                     VprRoutingAnnotation& vpr_routing_annotation,
                                     const bool& verbose);

} /* end namespace openfpga */

#endif
