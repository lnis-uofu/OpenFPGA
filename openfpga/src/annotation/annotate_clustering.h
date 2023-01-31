#ifndef ANNOTATE_CLUSTERING_H
#define ANNOTATE_CLUSTERING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_clustering_annotation.h"
#include "vpr_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

bool annotate_post_routing_cluster_sync_results(
  const ClusteringContext& clustering_ctx,
  VprClusteringAnnotation& clustering_annotation);

} /* end namespace openfpga */

#endif
