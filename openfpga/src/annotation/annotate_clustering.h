#ifndef ANNOTATE_CLUSTERING_H
#define ANNOTATE_CLUSTERING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "vpr_clustering_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void annotate_post_routing_cluster_sync_results(const DeviceContext& device_ctx, 
                                                const ClusteringContext& cluster_ctx, 
                                                VprClusteringAnnotation& cluster_annotation);

} /* end namespace openfpga */

#endif
