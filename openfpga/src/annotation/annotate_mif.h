#pragma once

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "bitstream_setting.h"
#include "mif_location_map.h"
#include "mif_pipeline.h"
#include "vpr_clustering_annotation.h"
#include "vpr_placement_annotation.h"

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
                       MifPipeline& mif_pipeline,
                       const VprClusteringAnnotation& clustering_annotation,
                       const VprPlacementAnnotation& placement_annotation);

/* Concatenate every destination PB on the FPGA-top MIF location map into
 * UNIFIED storage. A location uses PHYSICAL data when the pipeline has a
 * matching PB at that grid; otherwise the slice is filled with 0. */
int aggregate_unified_mif(const BitstreamSetting& bitstream_setting,
                          MifPipeline& mif_pipeline,
                          const MifLocationMap& mif_location_map);

} /* end namespace openfpga */
