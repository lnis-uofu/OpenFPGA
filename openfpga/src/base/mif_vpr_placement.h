#pragma once

#include "mif_pipeline.h"
#include "vpr_context.h"

namespace openfpga {

class BitstreamSetting;
class VprClusteringAnnotation;

/* After aggregate_to_physical: stamp VPR placement (x,y,sub_tile) into
 * MifPipeline for each physical_ segment. PhysicalPb confirms that each
 * AtomBlock belongs to a repacked physical instance. */
int annotate_physical_mif_grid_coordinates(
  MifPipeline& mif_pipeline, const BitstreamSetting& bitstream_setting,
  const AtomContext& atom_ctx, const PlacementContext& place_ctx,
  const VprClusteringAnnotation& clustering_annotation);

} /* namespace openfpga */
