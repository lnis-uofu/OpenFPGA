#pragma once

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "bitstream_setting.h"
#include "mif_location_map.h"
#include "mif_pipeline.h"
#include "vpr_clustering_annotation.h"
#include "vpr_context.h"
#include "vpr_placement_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/* Register hex files (read_mif) and eblif atom->pb_type pairs, then
 * decode INIT / hex and aggregate into placed physical primitives.
 * Must run after repack. */
int build_physical_mif(const BitstreamSetting& bitstream_setting,
                       MifPipeline& mif_pipeline, const AtomContext& atom_ctx,
                       const VprClusteringAnnotation& clustering_annotation,
                       const VprPlacementAnnotation& placement_annotation);

/* Concatenate every destination PB on the FPGA-top MIF location map into
 * top_mif_. A location uses physical_mifs_ data when the pipeline has a
 * matching PB at that grid; otherwise the slice is filled with 0. */
int aggregate_unified_mif(const BitstreamSetting& bitstream_setting,
                          MifPipeline& mif_pipeline,
                          const MifLocationMap& mif_location_map);

} /* end namespace openfpga */
