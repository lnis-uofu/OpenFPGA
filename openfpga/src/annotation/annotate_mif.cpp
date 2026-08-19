#include "annotate_mif.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>

#include "bitstream_setting_xml_constants.h"
#include "command_exit_codes.h"
#include "openfpga_pb_parser.h"
#include "pb_type_utils.h"
#include "physical_pb.h"
#include "physical_pb_utils.h"
#include "read_mif.h"
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

const std::string* find_hex_file_path(
  const std::map<std::string, std::string>& hex_map,
  const std::string& operating_pb_path) {
  for (const auto& hex_entry : hex_map) {
    if (pb_type_paths_match(hex_entry.first, operating_pb_path)) {
      return &hex_entry.second;
    }
  }
  return nullptr;
}

void bind_decoded_segments_to_pb(MifStorage& storage,
                                 const std::string& pb_type) {
  for (const MifSegmentId& segment_id : storage.segments()) {
    storage.set_segment_physical_pb(segment_id, pb_type);
  }
}

int load_decoded_hex_file(MifPipeline& mif_pipeline,
                          const BitstreamSetting& bitstream_setting,
                          const std::string& file_path,
                          const std::string& pb_type,
                          std::map<std::string, MifStorage>& decoded_hex,
                          MifStorage& decoded) {
  auto cache_it = decoded_hex.find(file_path);
  if (cache_it != decoded_hex.end()) {
    decoded = cache_it->second;
    bind_decoded_segments_to_pb(decoded, pb_type);
    return CMD_EXEC_SUCCESS;
  }
  const int read_status = read_mif_from_init_hex(file_path, decoded, pb_type);
  if (CMD_EXEC_SUCCESS != read_status) {
    return read_status;
  }
  const int decode_status =
    mif_pipeline.decode_storage(decoded, bitstream_setting);
  if (CMD_EXEC_SUCCESS != decode_status) {
    return decode_status;
  }
  decoded_hex[file_path] = decoded;
  bind_decoded_segments_to_pb(decoded, pb_type);
  return CMD_EXEC_SUCCESS;
}

int decode_mif_instance(MifPipeline& mif_pipeline, const AtomContext& atom_ctx,
                        const BitstreamSetting& bitstream_setting,
                        const PhysicalPb::MifDataInfo& mif_data,
                        std::map<std::string, MifStorage>& decoded_hex,
                        MifStorage& decoded, size_t& num_eblif,
                        size_t& num_others) {
  if (!mif_data.atom_block) {
    VTR_LOG_ERROR(
      "build_physical_mif: MIF annotation for pb '%s' has no AtomBlock "
      "id\n",
      mif_data.operating_pb_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  if (mif_data.source == XML_MIF_SOURCE_SOURCE_EBLIF) {
    mif_pipeline.mutable_eblif()[mif_data.atom_block] =
      mif_data.operating_pb_path;
    std::string value;
    if (!read_atom_block_field(atom_ctx, mif_data.atom_block, mif_data.selector,
                               value)) {
      VTR_LOG_ERROR(
        "build_physical_mif: MIF field '%s' was not found on AtomBlock "
        "'%s' for pb '%s'\n",
        mif_data.selector.c_str(),
        atom_ctx.netlist().block_name(mif_data.atom_block).c_str(),
        mif_data.operating_pb_path.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    value = strip_quoted_init(value);
    const MifSegmentId segment_id = decoded.create_segment();
    decoded.set_segment_physical_pb(segment_id, mif_data.operating_pb_path);
    decoded.set_segment_raw_data(segment_id, value);
    const int decode_status =
      mif_pipeline.decode_storage(decoded, bitstream_setting);
    if (CMD_EXEC_SUCCESS != decode_status) {
      return decode_status;
    }
    ++num_eblif;
    return CMD_EXEC_SUCCESS;
  }

  if (mif_data.source != XML_MIF_SOURCE_SOURCE_OTHERS) {
    return CMD_EXEC_SUCCESS;
  }

  const std::string* hex_path =
    find_hex_file_path(mif_pipeline.hex(), mif_data.operating_pb_path);
  if (nullptr == hex_path) {
    VTR_LOG_ERROR(
      "build_physical_mif: no hex file registered for pb_type '%s'; "
      "run read_mif --pb_type\n",
      mif_data.operating_pb_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  const int read_status =
    load_decoded_hex_file(mif_pipeline, bitstream_setting, *hex_path,
                          mif_data.operating_pb_path, decoded_hex, decoded);
  if (CMD_EXEC_SUCCESS != read_status) {
    return read_status;
  }
  ++num_others;
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
      "build_physical_mif: no hex file registered; source='%s' requires "
      "read_mif, or add a source='%s' mif_source\n",
      XML_MIF_SOURCE_SOURCE_OTHERS, XML_MIF_SOURCE_SOURCE_EBLIF);
    return CMD_EXEC_FATAL_ERROR;
  }

  mif_pipeline.mutable_eblif().clear();
  mif_pipeline.mutable_physical_mifs().clear();
  std::map<std::string, MifStorage> decoded_hex;
  size_t num_eblif = 0;
  size_t num_others = 0;
  for (const auto& cluster_physical_pb : clustering_annotation.physical_pbs()) {
    const ClusterBlockId& cluster_block = cluster_physical_pb.first;
    const PhysicalPb& physical_pb = cluster_physical_pb.second;
    bool loc_ready = false;
    t_pl_loc loc;

    for (const PhysicalPbId& physical_pb_id : physical_pb.primitive_pbs()) {
      for (const PhysicalPb::MifDataInfo& mif_data :
           physical_pb.mif_data(physical_pb_id)) {
        if (mif_data.source == XML_MIF_SOURCE_SOURCE_NONE) {
          continue;
        }
        if (!loc_ready) {
          if (!placement_annotation.has_block_location(cluster_block)) {
            VTR_LOG_ERROR(
              "build_physical_mif: clustered block %zu has no placement "
              "annotation\n",
              static_cast<size_t>(cluster_block));
            return CMD_EXEC_FATAL_ERROR;
          }
          loc = make_cluster_pl_loc(
            placement_annotation.block_location(cluster_block));
          loc_ready = true;
        }

        MifStorage decoded;
        const int decode_status = decode_mif_instance(
          mif_pipeline, atom_ctx, bitstream_setting, mif_data, decoded_hex,
          decoded, num_eblif, num_others);
        if (CMD_EXEC_SUCCESS != decode_status) {
          return decode_status;
        }
        if (decoded.empty()) {
          continue;
        }

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
        const int status = mif_pipeline.aggregate_logical_into_physical(
          decoded, bitstream_setting, dest);
        if (CMD_EXEC_SUCCESS != status) {
          return status;
        }
      }
    }
  }

  if (bitstream_setting.has_eblif_mif_source() && 0 == num_eblif) {
    VTR_LOG_ERROR(
      "build_physical_mif: no EBLIF-backed MIF data found in repacked "
      "PhysicalPb annotations\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (bitstream_setting.has_other_mif_source() && 0 == num_others) {
    VTR_LOG_ERROR(
      "build_physical_mif: no hex-backed MIF data found in repacked "
      "PhysicalPb annotations\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (mif_pipeline.physical_mifs().empty()) {
    VTR_LOG("build_physical_mif: no MIF data to aggregate\n");
    return CMD_EXEC_SUCCESS;
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
