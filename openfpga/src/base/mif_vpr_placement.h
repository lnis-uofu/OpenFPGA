#pragma once

#include "mif_pipeline.h"
#include "vpr_clustering_annotation.h"
#include "vpr_placement_annotation.h"

namespace openfpga {

class BitstreamSetting;

/* After aggregate_to_physical: stamp VPR placement (x,y,sub_tile) into
 * MifPipeline for each physical_ segment using OpenFPGA annotations. */
int annotate_physical_mif_grid_coordinates(
  MifPipeline& mif_pipeline, const BitstreamSetting& bitstream_setting,
  const VprClusteringAnnotation& clustering_annotation,
  const VprPlacementAnnotation& placement_annotation);

} /* namespace openfpga */
