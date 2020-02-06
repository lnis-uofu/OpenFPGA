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

void annotate_rr_node_nets(const ClusteringContext& vpr_clustering_ctx,
                           const RoutingContext& vpr_routing_ctx,
                           VprRoutingAnnotation& vpr_routing_annotation);

} /* end namespace openfpga */

#endif
