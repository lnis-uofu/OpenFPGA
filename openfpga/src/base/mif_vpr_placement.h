#pragma once

#include "mif_pipeline.h"
#include "vpr_context.h"

namespace openfpga {

class BitstreamSetting;

/* After aggregate_to_physical: stamp VPR placement (x,y,sub_tile) into
 * MifPipeline for each physical_ segment. */
int annotate_physical_mif_grid_coordinates(
  MifPipeline& mif_pipeline, const BitstreamSetting& bitstream_setting,
  const AtomContext& atom_ctx, const PlacementContext& place_ctx);

} /* namespace openfpga */
