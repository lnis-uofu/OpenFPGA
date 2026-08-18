#pragma once

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "mif_pipeline.h"
#include "vpr_clustering_annotation.h"
#include "vpr_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/* Load eblif/hex into MifPipeline and build PHYSICAL MIF.
 * Must run after repack so PhysicalPb is available.
 *
 * Aggregated physical MIF lands in MifPipeline::physical_; placement
 * coords for those segments are annotated into the same pipeline.
 *
 * EBLIF/LOGICAL keep operating pb paths to match bitstream-setting keys;
 * PHYSICAL keeps mif_address_map des_pb_type for location-map binding. */
int build_physical_mif(const BitstreamSetting& bitstream_setting,
                       MifPipeline& mif_pipeline, const AtomContext& atom_ctx,
                       const PlacementContext& place_ctx,
                       const VprClusteringAnnotation& clustering_annotation);

} /* end namespace openfpga */
