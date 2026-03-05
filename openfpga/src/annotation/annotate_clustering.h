#pragma once

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_clustering_annotation.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int annotate_post_routing_cluster_sync_results(
  const ClusteringContext& clustering_ctx,
  VprClusteringAnnotation& clustering_annotation);

int annotate_cluster_physical_equivalent_sites(
  const DeviceGrid& grids, const ClusteringContext& clustering_ctx,
  const PlacementContext& place_ctx,
  const VprDeviceAnnotation& device_annotation,
  VprClusteringAnnotation& clustering_annotation, const bool& verbose);

} /* end namespace openfpga */
