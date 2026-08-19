#include "mif_vpr_placement.h"

#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "openfpga_pb_parser.h"
#include "vtr_log.h"

namespace openfpga {

int annotate_physical_mif_grid_coordinates(
  MifPipeline& mif_pipeline, const BitstreamSetting& bitstream_setting,
  const VprClusteringAnnotation& clustering_annotation,
  const VprPlacementAnnotation& placement_annotation) {
  MifStorage& physical_storage =
    mif_pipeline.mutable_storage(MifPipeline::Stage::PHYSICAL);
  if (physical_storage.empty()) {
    return CMD_EXEC_SUCCESS;
  }

  for (const auto& cluster_physical_pb : clustering_annotation.physical_pbs()) {
    const ClusterBlockId& cluster_block = cluster_physical_pb.first;
    const PhysicalPb& physical_pb = cluster_physical_pb.second;
    if (!placement_annotation.has_block_location(cluster_block)) {
      VTR_LOG_ERROR(
        "annotate_physical_mif_grid_coordinates: clustered block %zu has no "
        "placement annotation\n",
        static_cast<size_t>(cluster_block));
      return CMD_EXEC_FATAL_ERROR;
    }
    const std::array<size_t, 3>& location =
      placement_annotation.block_location(cluster_block);

    for (const PhysicalPbId& physical_pb_id : physical_pb.primitive_pbs()) {
      for (const PhysicalPb::MifDataInfo& mif_data :
           physical_pb.mif_data(physical_pb_id)) {
        const std::string& operating_pb = mif_data.operating_pb_path;
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
            "repacked pb '%s'\n",
            operating_pb.c_str());
          return CMD_EXEC_FATAL_ERROR;
        }

        if (mif_pipeline.physical_segment_has_grid_coord(segment_id)) {
          const MifGridCoord& grid =
            mif_pipeline.physical_segment_grid_coord(segment_id);
          if (grid.x != static_cast<int>(location[0]) ||
              grid.y != static_cast<int>(location[1]) ||
              grid.z != static_cast<int>(location[2])) {
            VTR_LOG_WARN(
              "annotate_physical_mif_grid_coordinates: PHYSICAL segment %zu "
              "already has grid (%d,%d,%d); skip pb '%s' at (%zu,%zu,%zu)\n",
              static_cast<size_t>(segment_id), grid.x, grid.y, grid.z,
              operating_pb.c_str(), location[0], location[1], location[2]);
          }
          continue;
        }

        mif_pipeline.set_physical_segment_grid_coord(
          segment_id, static_cast<int>(location[0]),
          static_cast<int>(location[1]), static_cast<int>(location[2]));
        VTR_LOG(
          "annotate_physical_mif_grid_coordinates: segment %zu grid "
          "(%zu,%zu,%zu) pb '%s'\n",
          static_cast<size_t>(segment_id), location[0], location[1],
          location[2], operating_pb.c_str());
      }
    }
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
