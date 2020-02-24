#ifndef ANNOTATE_PLACEMENT_H
#define ANNOTATE_PLACEMENT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "vpr_placement_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void annotate_mapped_blocks(const DeviceContext& device_ctx, 
                            const ClusteringContext& cluster_ctx, 
                            const PlacementContext& place_ctx, 
                            VprPlacementAnnotation& place_annotation);

} /* end namespace openfpga */

#endif
