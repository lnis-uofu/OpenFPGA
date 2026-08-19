#include "annotate_mif.h"

#include <algorithm>
#include <cstdint>
#include <string>

#include "aggregate_mif_util.h"
#include "bitstream_setting_xml_constants.h"
#include "command_exit_codes.h"
#include "openfpga_pb_parser.h"
#include "pb_type_utils.h"
#include "physical_pb.h"
#include "physical_pb_utils.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

std::string strip_quoted_init(std::string value) {
  if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
    value = value.substr(1, value.size() - 2);
  }
  return value;
}

t_pl_loc make_cluster_pl_loc(const std::array<size_t, 3>& location) {
  return t_pl_loc(static_cast<int>(location[0]), static_cast<int>(location[1]),
                  static_cast<int>(location[2]), 0);
}

t_pb_graph_node* find_pb_graph_node_by_type_path(
  const PhysicalPb& physical_pb, const std::string& pb_type_path) {
  const std::string target = strip_numeric_pb_index(pb_type_path);
  for (const PhysicalPbId& pb_id : physical_pb.primitive_pbs()) {
    const t_pb_graph_node* node = physical_pb.pb_graph_node(pb_id);
    if (nullptr == node || nullptr == node->pb_type) {
      continue;
    }
    if (strip_numeric_pb_index(
          generate_pb_type_hierarchy_path(node->pb_type)) == target) {
      return const_cast<t_pb_graph_node*>(node);
    }
  }
  return nullptr;
}

bool pb_type_paths_match(const std::string& lhs, const std::string& rhs) {
  return strip_numeric_pb_index(lhs) == strip_numeric_pb_index(rhs);
}

int load_logical_mifs(MifPipeline& mif_pipeline, const AtomContext& atom_ctx,
                      const VprClusteringAnnotation& clustering_annotation,
                      const BitstreamSetting& bitstream_setting) {
  mif_pipeline.mutable_logical_mifs().clear();
  size_t num_eblif = 0;
  for (const auto& cluster_physical_pb : clustering_annotation.physical_pbs()) {
    const PhysicalPb& physical_pb = cluster_physical_pb.second;
    for (const PhysicalPbId& physical_pb_id : physical_pb.primitive_pbs()) {
      for (const PhysicalPb::MifDataInfo& mif_data :
           physical_pb.mif_data(physical_pb_id)) {
        if (mif_data.source != XML_MIF_SOURCE_SOURCE_EBLIF) {
          continue;
        }
        if (!mif_data.atom_block) {
          VTR_LOG_ERROR(
            "build_physical_mif: MIF annotation for pb '%s' has no AtomBlock "
            "pointer\n",
            mif_data.operating_pb_path.c_str());
          return CMD_EXEC_FATAL_ERROR;
        }
        std::string value;
        if (!read_atom_block_field(atom_ctx, mif_data.atom_block,
                                   mif_data.selector, value)) {
          VTR_LOG_ERROR(
            "build_physical_mif: MIF field '%s' was not found on AtomBlock "
            "'%s' for pb '%s'\n",
            mif_data.selector.c_str(),
            atom_ctx.netlist().block_name(mif_data.atom_block).c_str(),
            mif_data.operating_pb_path.c_str());
          return CMD_EXEC_FATAL_ERROR;
        }
        value = strip_quoted_init(value);

        MifStorage& logical =
          mif_pipeline.mutable_logical_mifs()[mif_data.atom_block];
        const MifSegmentId segment_id = logical.create_segment();
        logical.set_segment_physical_pb(segment_id, mif_data.operating_pb_path);
        logical.set_segment_raw_data(segment_id, value);
        ++num_eblif;
      }
    }
  }

  if (bitstream_setting.has_eblif_mif_source() && 0 == num_eblif) {
    VTR_LOG_ERROR(
      "build_physical_mif: no EBLIF-backed MIF data found in repacked "
      "PhysicalPb annotations\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  for (auto& logical_entry : mif_pipeline.mutable_logical_mifs()) {
    const int status =
      mif_pipeline.decode_storage(logical_entry.second, bitstream_setting);
    if (CMD_EXEC_SUCCESS != status) {
      return status;
    }
  }
  return CMD_EXEC_SUCCESS;
}

int aggregate_hex_segment_into(MifPipeline& mif_pipeline,
                               const BitstreamSetting& bitstream_setting,
                               const std::string& operating_pb_path,
                               MifStorage& dest) {
  const MifStorage& hex = mif_pipeline.hex();
  bool matched = false;
  for (const MifSegmentId& hex_seg : hex.segments()) {
    if (!pb_type_paths_match(hex.physical_pb(hex_seg), operating_pb_path)) {
      continue;
    }
    if (bitstream_setting.pb_type_is_eblif_mif_source(
          hex.physical_pb(hex_seg))) {
      continue;
    }
    MifStorage one_seg;
    copy_mif_segment(hex, hex_seg, one_seg);
    const int status = mif_pipeline.aggregate_logical_into_physical(
      one_seg, bitstream_setting, dest);
    if (CMD_EXEC_SUCCESS != status) {
      return status;
    }
    matched = true;
  }
  if (!matched) {
    VTR_LOG_ERROR(
      "build_physical_mif: no hex MIF segment matches pb_type '%s'\n",
      operating_pb_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

} /* namespace */

int build_physical_mif(const BitstreamSetting& bitstream_setting,
                       MifPipeline& mif_pipeline, const AtomContext& atom_ctx,
                       const VprClusteringAnnotation& clustering_annotation,
                       const VprPlacementAnnotation& placement_annotation) {
  const bool has_mif_setting =
    !bitstream_setting.mif_source_settings().empty() ||
    !bitstream_setting.mif_address_map_settings().empty();
  if (!has_mif_setting) {
    return CMD_EXEC_SUCCESS;
  }

  if (bitstream_setting.has_other_mif_source() && mif_pipeline.hex().empty() &&
      !bitstream_setting.has_eblif_mif_source()) {
    VTR_LOG_ERROR(
      "build_physical_mif: hex MIF storage is empty; source='%s' requires "
      "read_mif, or add a source='%s' mif_source\n",
      XML_MIF_SOURCE_SOURCE_OTHERS, XML_MIF_SOURCE_SOURCE_EBLIF);
    return CMD_EXEC_FATAL_ERROR;
  }

  if (!mif_pipeline.hex().empty()) {
    const int hex_status = mif_pipeline.decode_storage(
      mif_pipeline.mutable_hex(), bitstream_setting);
    if (CMD_EXEC_SUCCESS != hex_status) {
      return hex_status;
    }
  }

  const int logical_status = load_logical_mifs(
    mif_pipeline, atom_ctx, clustering_annotation, bitstream_setting);
  if (CMD_EXEC_SUCCESS != logical_status) {
    return logical_status;
  }

  if (mif_pipeline.logical_mifs().empty() && mif_pipeline.hex().empty()) {
    VTR_LOG(
      "build_physical_mif: empty logical MIF storage; nothing to "
      "aggregate\n");
    return CMD_EXEC_SUCCESS;
  }

  mif_pipeline.mutable_physical_mifs().clear();
  for (const auto& cluster_physical_pb : clustering_annotation.physical_pbs()) {
    const ClusterBlockId& cluster_block = cluster_physical_pb.first;
    const PhysicalPb& physical_pb = cluster_physical_pb.second;
    if (!placement_annotation.has_block_location(cluster_block)) {
      VTR_LOG_ERROR(
        "build_physical_mif: clustered block %zu has no placement "
        "annotation\n",
        static_cast<size_t>(cluster_block));
      return CMD_EXEC_FATAL_ERROR;
    }
    const t_pl_loc loc =
      make_cluster_pl_loc(placement_annotation.block_location(cluster_block));

    for (const PhysicalPbId& physical_pb_id : physical_pb.primitive_pbs()) {
      for (const PhysicalPb::MifDataInfo& mif_data :
           physical_pb.mif_data(physical_pb_id)) {
        std::string dest_pb_type = mif_data.operating_pb_path;
        const MifAddressMapSettingId map_id =
          bitstream_setting.find_mif_address_map_by_src_pb_type(
            mif_data.operating_pb_path);
        if (map_id.is_valid()) {
          dest_pb_type = bitstream_setting.mif_address_map_des_pb_type(map_id);
        }
        t_pb_graph_node* dest_node =
          find_pb_graph_node_by_type_path(physical_pb, dest_pb_type);
        if (nullptr == dest_node) {
          dest_node = const_cast<t_pb_graph_node*>(
            physical_pb.pb_graph_node(physical_pb_id));
        }
        if (nullptr == dest_node) {
          VTR_LOG_ERROR(
            "build_physical_mif: no physical pb_graph_node for pb '%s'\n",
            mif_data.operating_pb_path.c_str());
          return CMD_EXEC_FATAL_ERROR;
        }

        MifStorage& dest = mif_pipeline.mutable_physical_mifs()[loc][dest_node];
        int status = CMD_EXEC_SUCCESS;
        if (mif_data.source == XML_MIF_SOURCE_SOURCE_EBLIF) {
          auto logical_it =
            mif_pipeline.logical_mifs().find(mif_data.atom_block);
          if (logical_it == mif_pipeline.logical_mifs().end()) {
            VTR_LOG_ERROR(
              "build_physical_mif: AtomBlock for pb '%s' has no logical "
              "MIF\n",
              mif_data.operating_pb_path.c_str());
            return CMD_EXEC_FATAL_ERROR;
          }
          status = mif_pipeline.aggregate_logical_into_physical(
            logical_it->second, bitstream_setting, dest);
        } else {
          status = aggregate_hex_segment_into(mif_pipeline, bitstream_setting,
                                              mif_data.operating_pb_path, dest);
        }
        if (CMD_EXEC_SUCCESS != status) {
          return status;
        }
      }
    }
  }

  if (mif_pipeline.physical_mifs().empty()) {
    VTR_LOG_ERROR("build_physical_mif: no aggregated physical MIF produced\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

int aggregate_unified_mif(const BitstreamSetting& bitstream_setting,
                          MifPipeline& mif_pipeline,
                          const MifLocationMap& mif_location_map) {
  MifStorage& top_mif = mif_pipeline.mutable_top_mif();
  top_mif.clear();
  if (mif_location_map.empty()) {
    VTR_LOG(
      "aggregate_unified_mif: empty MIF location map; skip FPGA-top "
      "aggregation\n");
    return CMD_EXEC_SUCCESS;
  }

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
    std::map<t_pl_loc, const MifStorage*> loc_to_storage;
    for (const auto& loc_entry : loc_map) {
      const t_pl_loc& phy_loc = loc_entry.first;
      const MifPortSlice& slice = loc_entry.second;
      total_width = std::max(total_width, slice.data_offset + slice.data_width);

      if (nullptr != slice.pb_graph_node &&
          nullptr != slice.pb_graph_node->pb_type) {
        const std::string type_target =
          generate_pb_type_hierarchy_path(slice.pb_graph_node->pb_type);
        if (!addr_range.is_valid()) {
          const MifSourceSettingId source_id =
            bitstream_setting.find_mif_source_by_pb_type(type_target);
          if (source_id.is_valid()) {
            addr_range = bitstream_setting.mif_source_address_range(source_id);
          }
        }
      }

      const MifStorage& physical =
        mif_pipeline.physical_mif(phy_loc, slice.pb_graph_node);
      if (!physical.empty()) {
        loc_to_storage[phy_loc] = &physical;
        ++num_used_slices;
      } else {
        ++num_zero_slices;
      }
    }
    if (0 == total_width) {
      continue;
    }
    if (!addr_range.is_valid()) {
      for (const auto& loc_entry : loc_to_storage) {
        const MifStorage& storage = *loc_entry.second;
        for (const MifSegmentId& segment_id : storage.segments()) {
          if (storage.addr_range(segment_id).is_valid()) {
            addr_range = storage.addr_range(segment_id);
            break;
          }
        }
        if (addr_range.is_valid()) {
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

    const MifSegmentId out_seg = top_mif.create_segment();
    top_mif.set_segment_physical_pb(out_seg, port_name);
    top_mif.set_segment_data_width(out_seg, static_cast<int>(total_width));
    top_mif.set_segment_addr_range(out_seg, addr_range);

    for (size_t addr = addr_range.get_lsb(); addr <= addr_range.get_msb();
         ++addr) {
      std::string word(total_width, '0');
      for (const auto& loc_entry : loc_map) {
        const auto storage_it = loc_to_storage.find(loc_entry.first);
        if (storage_it == loc_to_storage.end()) {
          continue;
        }
        const MifPortSlice& slice = loc_entry.second;
        if (0 == slice.data_width ||
            slice.data_offset + slice.data_width > total_width) {
          continue;
        }
        const MifStorage& physical = *storage_it->second;
        bool copied = false;
        for (const MifSegmentId& segment_id : physical.segments()) {
          for (const MifMemoryLineId& line_id :
               physical.segment_memory_lines(segment_id)) {
            if (physical.memory_line_address(line_id) !=
                static_cast<uint64_t>(addr)) {
              continue;
            }
            const std::string& src = physical.memory_line_data(line_id);
            const size_t copy_width = std::min(slice.data_width, src.size());
            for (size_t i = 0; i < copy_width; ++i) {
              word[slice.data_offset + i] = src[i];
            }
            copied = true;
            break;
          }
          if (copied) {
            break;
          }
        }
      }
      top_mif.create_memory_line(out_seg, static_cast<uint64_t>(addr), word);
    }
  }

  VTR_LOG(
    "aggregate_unified_mif: %zu FPGA-top port(s), %zu placed slice(s), "
    "%zu zero-filled slice(s)\n",
    top_mif.num_segments(), num_used_slices, num_zero_slices);
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
