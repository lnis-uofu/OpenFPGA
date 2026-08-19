#include "annotate_mif.h"

#include <algorithm>
#include <cstdint>
#include <string>

#include "bitstream_setting_xml_constants.h"
#include "command_exit_codes.h"
#include "mif_vpr_placement.h"
#include "openfpga_pb_parser.h"
#include "pb_type_utils.h"
#include "physical_pb.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

static size_t load_eblif_mif_data(
  MifPipeline& mif_pipeline,
  const VprClusteringAnnotation& clustering_annotation) {
  MifStorage& eblif_storage =
    mif_pipeline.mutable_storage(MifPipeline::Stage::EBLIF);
  eblif_storage.clear();
  size_t num_mif_data = 0;
  for (const auto& cluster_physical_pb : clustering_annotation.physical_pbs()) {
    const PhysicalPb& physical_pb = cluster_physical_pb.second;
    for (const PhysicalPbId& physical_pb_id : physical_pb.primitive_pbs()) {
      for (const PhysicalPb::MifDataInfo& mif_data :
           physical_pb.mif_data(physical_pb_id)) {
        if (mif_data.source != XML_MIF_SOURCE_SOURCE_EBLIF) {
          continue;
        }
        const MifSegmentId segment_id = eblif_storage.create_segment();
        eblif_storage.set_segment_physical_pb(segment_id,
                                              mif_data.operating_pb_path);
        eblif_storage.set_segment_raw_data(segment_id, mif_data.value);
        ++num_mif_data;
      }
    }
  }
  return num_mif_data;
}

/********************************************************************
 * Build LOGICAL then PHYSICAL MIF for the given bitstream setting.
 *
 * Aggregated physical MIF is stored as physical_ in MifPipeline.
 * Its corresponding placement coords are also stored in MifPipeline.
 *
 * Stage pb semantics:
 *   EBLIF/LOGICAL - operating pb paths (match mif_source /
 *                   mif_address_map src_pb_type strings)
 *   PHYSICAL      - aggregated des_pb_type from mif_address_map
 *
 * EBLIF values and operating pb paths were cached in PhysicalPb by repack.
 * This stage consumes only OpenFPGA clustering and placement annotations.
 *******************************************************************/
int build_physical_mif(const BitstreamSetting& bitstream_setting,
                       MifPipeline& mif_pipeline,
                       const VprClusteringAnnotation& clustering_annotation,
                       const VprPlacementAnnotation& placement_annotation) {
  const bool has_mif_setting =
    !bitstream_setting.mif_source_settings().empty() ||
    !bitstream_setting.mif_address_map_settings().empty();
  if (!has_mif_setting) {
    return CMD_EXEC_SUCCESS;
  }

  if (bitstream_setting.has_other_mif_source() &&
      mif_pipeline.storage(MifPipeline::Stage::HEX).empty() &&
      !bitstream_setting.has_eblif_mif_source()) {
    VTR_LOG_ERROR(
      "build_physical_mif: hex MIF storage is empty; source='%s' requires "
      "read_mif, or add a source='%s' mif_source\n",
      XML_MIF_SOURCE_SOURCE_OTHERS, XML_MIF_SOURCE_SOURCE_EBLIF);
    return CMD_EXEC_FATAL_ERROR;
  }

  if (bitstream_setting.has_eblif_mif_source()) {
    if (load_eblif_mif_data(mif_pipeline, clustering_annotation) == 0) {
      VTR_LOG_ERROR(
        "build_physical_mif: no EBLIF-backed MIF data found in repacked "
        "PhysicalPb annotations\n");
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  /* merge mif collected from two sources: read_mif command and eblif source */
  int status = mif_pipeline.merge_to_logical(bitstream_setting);
  if (CMD_EXEC_SUCCESS != status) {
    return status;
  }
  if (mif_pipeline.storage(MifPipeline::Stage::LOGICAL).empty()) {
    VTR_LOG(
      "build_physical_mif: empty logical MIF storage; nothing to "
      "aggregate\n");
    return CMD_EXEC_SUCCESS;
  }

  status = mif_pipeline.decode_logical(bitstream_setting);
  if (CMD_EXEC_SUCCESS != status) {
    return status;
  }

  /*aggregate to physical_; result stored in mif_pipeline.physical_ */
  status = mif_pipeline.aggregate_to_physical(bitstream_setting);
  if (CMD_EXEC_SUCCESS != status) {
    return status;
  }

  /* Annotate placement coords into the same MifPipeline as physical_. */
  status = annotate_physical_mif_grid_coordinates(
    mif_pipeline, bitstream_setting, clustering_annotation,
    placement_annotation);
  return status;
}

int aggregate_unified_mif(const BitstreamSetting& bitstream_setting,
                          MifPipeline& mif_pipeline,
                          const MifLocationMap& mif_location_map) {
  MifStorage& unified =
    mif_pipeline.mutable_storage(MifPipeline::Stage::UNIFIED);
  unified.clear();
  if (mif_location_map.empty()) {
    VTR_LOG(
      "aggregate_unified_mif: empty MIF location map; skip FPGA-top "
      "aggregation\n");
    return CMD_EXEC_SUCCESS;
  }

  const MifStorage& physical =
    mif_pipeline.storage(MifPipeline::Stage::PHYSICAL);
  size_t num_used_slices = 0;
  size_t num_zero_slices = 0;

  for (const auto& port_entry : mif_location_map.data_port2phy_loc_map()) {
    const std::string& port_name = port_entry.first;
    const std::map<t_pl_loc, MifPortSlice>& loc_map = port_entry.second;
    if (loc_map.empty()) {
      continue;
    }

    size_t total_width = 0;
    BasicPort addr_range;
    std::map<t_pl_loc, MifSegmentId> loc_to_segment;
    for (const auto& loc_entry : loc_map) {
      const t_pl_loc& phy_loc = loc_entry.first;
      const MifPortSlice& slice = loc_entry.second;
      total_width = std::max(total_width, slice.data_offset + slice.data_width);

      std::string type_target;
      if (nullptr != slice.pb_graph_node &&
          nullptr != slice.pb_graph_node->pb_type) {
        type_target =
          generate_pb_type_hierarchy_path(slice.pb_graph_node->pb_type);
        if (!addr_range.is_valid()) {
          const MifSourceSettingId source_id =
            bitstream_setting.find_mif_source_by_pb_type(type_target);
          if (source_id.is_valid()) {
            addr_range = bitstream_setting.mif_source_address_range(source_id);
          }
        }
        type_target = strip_numeric_pb_index(type_target);
      }

      MifSegmentId segment_id = MifSegmentId::INVALID();
      for (const MifSegmentId& candidate : physical.segments()) {
        if (!mif_pipeline.physical_segment_has_grid_coord(candidate)) {
          continue;
        }
        const MifGridCoord& grid =
          mif_pipeline.physical_segment_grid_coord(candidate);
        if (grid.x != phy_loc.x || grid.y != phy_loc.y ||
            grid.z != phy_loc.sub_tile) {
          continue;
        }
        if (!type_target.empty() && strip_numeric_pb_index(physical.physical_pb(
                                      candidate)) != type_target) {
          continue;
        }
        segment_id = candidate;
        break;
      }
      if (segment_id.is_valid()) {
        loc_to_segment[phy_loc] = segment_id;
        ++num_used_slices;
      } else {
        ++num_zero_slices;
      }
    }
    if (0 == total_width) {
      continue;
    }
    if (!addr_range.is_valid()) {
      for (const MifSegmentId& segment_id : physical.segments()) {
        if (physical.addr_range(segment_id).is_valid()) {
          addr_range = physical.addr_range(segment_id);
          break;
        }
      }
    }
    if (!addr_range.is_valid()) {
      VTR_LOG_ERROR(
        "aggregate_unified_mif: no address range for FPGA-top MIF port "
        "'%s'\n",
        port_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const MifSegmentId out_seg = unified.create_segment();
    unified.set_segment_physical_pb(out_seg, port_name);
    unified.set_segment_data_width(out_seg, static_cast<int>(total_width));
    unified.set_segment_addr_range(out_seg, addr_range);

    for (size_t addr = addr_range.get_lsb(); addr <= addr_range.get_msb();
         ++addr) {
      std::string word(total_width, '0');
      for (const auto& loc_entry : loc_map) {
        const auto segment_it = loc_to_segment.find(loc_entry.first);
        if (segment_it == loc_to_segment.end()) {
          continue;
        }
        const MifPortSlice& slice = loc_entry.second;
        if (0 == slice.data_width ||
            slice.data_offset + slice.data_width > total_width) {
          continue;
        }
        for (const MifMemoryLineId& line_id :
             physical.segment_memory_lines(segment_it->second)) {
          if (physical.memory_line_address(line_id) !=
              static_cast<uint64_t>(addr)) {
            continue;
          }
          const std::string& src = physical.memory_line_data(line_id);
          const size_t copy_width = std::min(slice.data_width, src.size());
          for (size_t i = 0; i < copy_width; ++i) {
            word[slice.data_offset + i] = src[i];
          }
          break;
        }
      }
      unified.create_memory_line(out_seg, static_cast<uint64_t>(addr), word);
    }
  }

  VTR_LOG(
    "aggregate_unified_mif: %zu FPGA-top port(s), %zu placed slice(s), "
    "%zu zero-filled slice(s)\n",
    unified.num_segments(), num_used_slices, num_zero_slices);
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
