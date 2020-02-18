/***************************************************************************************
 * This file includes functions that are used to redo packing for physical pbs
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "repack.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Top-level function to pack physical pb_graph 
 * This function will do :
 *  - create physical lb_rr_graph for each pb_graph considering physical modes only
 *    the lb_rr_graph willbe added to device annotation
 *  - annotate nets to be routed for each clustered block from operating modes of pb_graph 
 *    to physical modes of pb_graph
 *  - rerun the routing for each clustered block
 *  - store the packing results to clustering annotation
 ***************************************************************************************/
void pack_physical_pbs(const DeviceContext& device_ctx,
                       VprDeviceAnnotation& device_annotation,
                       VprClusteringAnnotation& clustering_annotation,
                       const VprRoutingAnnotation& routing_annotation,
                       const bool& verbose) {
}

} /* end namespace openfpga */
