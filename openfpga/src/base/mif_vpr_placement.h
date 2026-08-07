#pragma once

#include <string>
#include <utility>
#include <vector>

#include "mif_pipeline.h"
#include "read_mif.h"
#include "vpr_context.h"

namespace openfpga {

class BitstreamSetting;

/* Resolve an eblif subckt to its packed operating pb_type through the output
 * net used by VPR as the AtomBlock name. VPR pack must already be complete. */
std::string get_mif_pb_type_from_vpr(
  const AtomContext& atom_ctx, const DeviceContext& device_ctx,
  const std::string& model_name,
  const MifEblifPortConnections& port_connections);

/* After aggregate_to_physical: stamp VPR placement (x,y,sub_tile) into
 * MifPipeline for each physical_ segment. */
int annotate_physical_mif_grid_coordinates(
  MifPipeline& mif_pipeline, const BitstreamSetting& bitstream_setting,
  const AtomContext& atom_ctx, const PlacementContext& place_ctx);

} /* namespace openfpga */
