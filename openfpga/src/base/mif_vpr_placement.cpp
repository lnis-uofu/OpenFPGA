#include "mif_vpr_placement.h"

#include <algorithm>
#include <string>

#include "aggregate_mif_util.h"
#include "atom_netlist.h"
#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "openfpga_pb_parser.h"
#include "physical_pb.h"
#include "vpr_clustering_annotation.h"
#include "vpr_types.h"
#include "vtr_log.h"

namespace openfpga {

static PhysicalPbId find_physical_mif_instance(const PhysicalPb& physical_pb,
                                               const AtomBlockId& atom_block) {
  for (const PhysicalPbId& physical_pb_id : physical_pb.primitive_pbs()) {
    const std::vector<AtomBlockId> atom_blocks =
      physical_pb.atom_blocks(physical_pb_id);
    if (std::find(atom_blocks.begin(), atom_blocks.end(), atom_block) !=
        atom_blocks.end()) {
      return physical_pb_id;
    }
  }
  return PhysicalPbId::INVALID();
}

int annotate_physical_mif_grid_coordinates(
  MifPipeline& mif_pipeline, const BitstreamSetting& bitstream_setting,
  const AtomContext& atom_ctx, const PlacementContext& place_ctx,
  const VprClusteringAnnotation& clustering_annotation) {
  MifStorage& physical_storage =
    mif_pipeline.mutable_storage(MifPipeline::Stage::PHYSICAL);
  if (physical_storage.empty()) {
    return CMD_EXEC_SUCCESS;
  }

  const AtomPBBimap& atom_pb_bimap = atom_ctx.lookup().atom_pb_bimap();

  for (const AtomBlockId atom_block : atom_ctx.netlist().blocks()) {
    const AtomBlockType block_type = atom_ctx.netlist().block_type(atom_block);
    if ((AtomBlockType::INPAD == block_type) ||
        (AtomBlockType::OUTPAD == block_type)) {
      continue;
    }

    const t_pb* leaf_pb = atom_pb_bimap.atom_pb(atom_block);
    if (leaf_pb == nullptr) {
      continue;
    }

    const std::string operating_pb = generate_mif_pb_path(leaf_pb);
    if (operating_pb.empty()) {
      continue;
    }
    if (!bitstream_setting.find_mif_source_by_pb_type(operating_pb)
           .is_valid()) {
      continue;
    }

    std::string target_pb = operating_pb;
    const MifAddressMapSettingId map_id =
      bitstream_setting.find_mif_address_map_by_src_pb_type(operating_pb);
    if (map_id.is_valid()) {
      target_pb = bitstream_setting.mif_address_map_des_pb_type(map_id);
    }
    const std::string type_target = strip_numeric_pb_index(target_pb);

    MifSegmentId segment_id = MifSegmentId::INVALID();
    for (const MifSegmentId& candidate : physical_storage.segments()) {
      if (strip_numeric_pb_index(physical_storage.physical_pb(candidate)) ==
          type_target) {
        segment_id = candidate;
        break;
      }
    }
    if (!segment_id.is_valid()) {
      VTR_LOG_ERROR(
        "annotate_physical_mif_grid_coordinates: no PHYSICAL segment for "
        "packed pb '%s' (AtomBlock '%s')\n",
        operating_pb.c_str(),
        atom_ctx.netlist().block_name(atom_block).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const ClusterBlockId cluster_blk = atom_ctx.lookup().atom_clb(atom_block);
    if (!cluster_blk.is_valid()) {
      VTR_LOG_ERROR(
        "annotate_physical_mif_grid_coordinates: AtomBlock '%s' has no "
        "cluster placement\n",
        atom_ctx.netlist().block_name(atom_block).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const PhysicalPb physical_pb =
      clustering_annotation.physical_pb(cluster_blk);
    const PhysicalPbId physical_pb_id =
      find_physical_mif_instance(physical_pb, atom_block);
    if (!physical_pb_id.is_valid()) {
      VTR_LOG_ERROR(
        "annotate_physical_mif_grid_coordinates: AtomBlock '%s' has no "
        "repacked PhysicalPb instance; run this after repack\n",
        atom_ctx.netlist().block_name(atom_block).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const t_pl_loc& pl_loc = place_ctx.block_locs()[cluster_blk].loc;
    if (mif_pipeline.physical_segment_has_grid_coord(segment_id)) {
      const MifGridCoord& grid =
        mif_pipeline.physical_segment_grid_coord(segment_id);
      if (grid.x != pl_loc.x || grid.y != pl_loc.y ||
          grid.z != pl_loc.sub_tile) {
        VTR_LOG_WARN(
          "annotate_physical_mif_grid_coordinates: PHYSICAL segment %zu "
          "already has grid (%d,%d,%d); skip AtomBlock '%s' at (%d,%d,%d)\n",
          static_cast<size_t>(segment_id), grid.x, grid.y, grid.z,
          atom_ctx.netlist().block_name(atom_block).c_str(), pl_loc.x, pl_loc.y,
          pl_loc.sub_tile);
        continue;
      }
      continue;
    }

    mif_pipeline.set_physical_segment_grid_coord(segment_id, pl_loc.x, pl_loc.y,
                                                 pl_loc.sub_tile);
    VTR_LOG(
      "annotate_physical_mif_grid_coordinates: segment %zu grid (%d,%d,%d) "
      "pb '%s', PhysicalPb '%s'\n",
      static_cast<size_t>(segment_id), pl_loc.x, pl_loc.y, pl_loc.sub_tile,
      operating_pb.c_str(), physical_pb.name(physical_pb_id).c_str());
  }

  for (const MifSegmentId& segment_id : physical_storage.segments()) {
    if (!mif_pipeline.physical_segment_has_grid_coord(segment_id)) {
      VTR_LOG_WARN(
        "annotate_physical_mif_grid_coordinates: PHYSICAL segment %zu "
        "(pb '%s') has no packed MIF instance placement\n",
        static_cast<size_t>(segment_id),
        physical_storage.physical_pb(segment_id).c_str());
    }
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
