#ifndef OPENFPGA_PB_PIN_FIXUP_H
#define OPENFPGA_PB_PIN_FIXUP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "openfpga_context.h"
#include "vpr_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void update_pb_pin_with_post_routing_results(
  const DeviceContext& device_ctx, const ClusteringContext& clustering_ctx,
  const PlacementContext& placement_ctx,
  const VprRoutingAnnotation& vpr_routing_annotation,
  VprClusteringAnnotation& vpr_clustering_annotation,
  const bool& perimeter_cb,
  const bool& verbose);

} /* end namespace openfpga */

#endif
