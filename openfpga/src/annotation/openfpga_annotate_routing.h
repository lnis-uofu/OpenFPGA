#ifndef OPENFPGA_ANNOTATE_ROUTING_H
#define OPENFPGA_ANNOTATE_ROUTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "openfpga_context.h"
#include "vpr_clustering_annotation.h"
#include "vpr_context.h"
#include "vpr_routing_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

vtr::vector<RRNodeId, ClusterNetId> annotate_rr_node_global_net(
  const DeviceContext& device_ctx, const ClusteredNetlist& cluster_nlist,
  const PlacementContext& placement_ctx,
  const VprClusteringAnnotation& clustering_annotation, const bool& verbose);

void annotate_vpr_rr_node_nets(const DeviceContext& device_ctx,
                               const ClusteringContext& clustering_ctx,
                               VprRoutingAnnotation& vpr_routing_annotation,
                               const bool& verbose);

void annotate_rr_node_previous_nodes(
  const DeviceContext& device_ctx, const ClusteringContext& clustering_ctx,
  VprRoutingAnnotation& vpr_routing_annotation, const bool& verbose);

} /* end namespace openfpga */

#endif
