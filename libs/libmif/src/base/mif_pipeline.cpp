#include "mif_pipeline.h"

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "aggregate_mif_util.h"
#include "vtr_log.h"

namespace openfpga {

const std::map<std::string, std::string>& MifPipeline::hex() const {
  return hex_;
}

std::map<std::string, std::string>& MifPipeline::mutable_hex() { return hex_; }

const std::map<AtomBlockId, std::string>& MifPipeline::eblif() const {
  return eblif_;
}

std::map<AtomBlockId, std::string>& MifPipeline::mutable_eblif() {
  return eblif_;
}

const MifStorage& MifPipeline::top_mif() const { return top_mif_; }

MifStorage& MifPipeline::mutable_top_mif() { return top_mif_; }

const std::map<t_pl_loc, std::map<t_pb_graph_node*, MifStorage>>&
MifPipeline::physical_mifs() const {
  return physical_mifs_;
}

std::map<t_pl_loc, std::map<t_pb_graph_node*, MifStorage>>&
MifPipeline::mutable_physical_mifs() {
  return physical_mifs_;
}

const MifStorage& MifPipeline::physical_mif(
  const t_pl_loc& phy_loc, t_pb_graph_node* pb_graph_node) const {
  static const MifStorage empty;
  auto loc_it = physical_mifs_.find(phy_loc);
  if (loc_it == physical_mifs_.end()) {
    return empty;
  }
  const auto& by_node = loc_it->second;
  auto node_it = by_node.find(pb_graph_node);
  if (node_it != by_node.end()) {
    return node_it->second;
  }
  /* One primitive at this loc: accept even if graph-node pointers differ. */
  if (1 == by_node.size()) {
    return by_node.begin()->second;
  }
  return empty;
}

void MifPipeline::clear() {
  hex_.clear();
  eblif_.clear();
  physical_mifs_.clear();
  top_mif_.clear();
}

MifStorage MifPipeline::copy_all_physical_mifs() const {
  MifStorage combined;
  for (const auto& loc_entry : physical_mifs_) {
    for (const auto& node_entry : loc_entry.second) {
      const MifStorage& storage = node_entry.second;
      for (const MifSegmentId& segment_id : storage.segments()) {
        copy_mif_segment(storage, segment_id, combined);
      }
    }
  }
  return combined;
}

int MifPipeline::decode_storage(
  MifStorage& storage, const BitstreamSetting& bitstream_setting) const {
  for (const MifSegmentId& segment_id : storage.segments()) {
    const std::string vpr_pb_type = storage.physical_pb(segment_id);
    const MifSourceSettingId source_id =
      bitstream_setting.find_mif_source_by_pb_type(vpr_pb_type);
    if (!source_id.is_valid()) {
      VTR_LOG_ERROR(
        "mif_pipeline: segment %zu pb_type '%s' has no mif_source\n",
        static_cast<size_t>(segment_id), vpr_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const BasicPort addr_range =
      bitstream_setting.mif_source_address_range(source_id);
    const BasicPort data_range =
      bitstream_setting.mif_source_data_range(source_id);
    storage.set_segment_addr_range(segment_id, addr_range);
    storage.set_segment_data_width(segment_id,
                                   static_cast<int>(data_range.get_width()));

    if (storage.raw_data(segment_id).empty()) {
      for (const MifMemoryLineId& line_id :
           storage.segment_memory_lines(segment_id)) {
        std::string data_bits = storage.memory_line_data(line_id);
        if (!is_valid_bit_string(data_bits) ||
            !normalize_bit_string_width(data_bits, data_range.get_width())) {
          VTR_LOG_ERROR(
            "mif_pipeline: cannot fit hex word at addr %lu into data_range "
            "width %zu for pb_type '%s'\n",
            static_cast<unsigned long>(storage.memory_line_address(line_id)),
            data_range.get_width(),
            bitstream_setting.mif_source_pb_type(source_id).c_str());
          return CMD_EXEC_FATAL_ERROR;
        }
        storage.set_memory_line_data(line_id, data_bits);
      }
      continue;
    }

    std::vector<std::pair<uint64_t, std::string>> words;
    if (!unpack_yosys_init_param(storage.raw_data(segment_id),
                                 data_range.get_width(), addr_range.get_width(),
                                 words)) {
      return CMD_EXEC_FATAL_ERROR;
    }
    for (const auto& word : words) {
      storage.create_memory_line(segment_id, word.first + addr_range.get_lsb(),
                                 word.second);
    }
    storage.set_segment_raw_data(segment_id, std::string());
  }
  return CMD_EXEC_SUCCESS;
}

int MifPipeline::aggregate_logical_into_physical(
  const MifStorage& logical, const BitstreamSetting& bitstream_setting,
  MifStorage& dest) const {
  if (bitstream_setting.mif_address_map_settings().empty()) {
    VTR_LOG_ERROR("mif_pipeline: no mif_address_map in bitstream setting\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (logical.empty()) {
    return CMD_EXEC_SUCCESS;
  }

  std::map<std::string, std::map<uint64_t, std::string>> des_data_maps;
  std::map<std::string, std::map<uint64_t, std::string>> des_written_masks;

  for (const MifSegmentId& dest_seg : dest.segments()) {
    auto& phys_data_map = des_data_maps[dest.physical_pb(dest_seg)];
    auto& phys_written_mask = des_written_masks[dest.physical_pb(dest_seg)];
    for (const MifMemoryLineId& line_id : dest.segment_memory_lines(dest_seg)) {
      const std::string& bits = dest.memory_line_data(line_id);
      phys_data_map[dest.memory_line_address(line_id)] = bits;
      phys_written_mask[dest.memory_line_address(line_id)] =
        std::string(bits.size(), '1');
    }
  }

  for (const MifSegmentId& segment_id : logical.segments()) {
    if (!logical.has_physical_pb(segment_id)) {
      VTR_LOG_ERROR(
        "mif_pipeline: segment %zu has no pb_type from its mif_source\n",
        static_cast<size_t>(segment_id));
      return CMD_EXEC_FATAL_ERROR;
    }
    const std::string& src_pb_type = logical.physical_pb(segment_id);
    const MifAddressMapSettingId map_id =
      bitstream_setting.find_mif_address_map_by_src_pb_type(src_pb_type);
    if (!map_id.is_valid()) {
      VTR_LOG_ERROR(
        "mif_pipeline: segment %zu pb_type '%s' has no mif_address_map\n",
        static_cast<size_t>(segment_id), src_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const std::string des_pb_type =
      bitstream_setting.mif_address_map_des_pb_type(map_id);
    const MifSourceSettingId des_source_id =
      bitstream_setting.find_mif_source_by_pb_type(des_pb_type);
    if (!des_source_id.is_valid()) {
      VTR_LOG_ERROR(
        "mif_pipeline: des_pb_type '%s' has no matching mif_source\n",
        des_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    const size_t des_word_width =
      bitstream_setting.mif_source_data_range(des_source_id).get_width();
    const BasicPort& src_addr_range = logical.addr_range(segment_id);
    const size_t src_word_width =
      static_cast<size_t>(logical.data_width(segment_id));
    auto& phys_data_map = des_data_maps[des_pb_type];
    auto& phys_written_mask = des_written_masks[des_pb_type];

    VTR_LOG("mif_pipeline: segment %zu bound src='%s' -> des='%s'\n",
            static_cast<size_t>(segment_id), src_pb_type.c_str(),
            des_pb_type.c_str());

    for (const MifMemoryLineId& line_id :
         logical.segment_memory_lines(segment_id)) {
      const uint64_t logical_addr = logical.memory_line_address(line_id);
      const std::string& logical_data = logical.memory_line_data(line_id);
      if (!src_addr_range.is_valid() ||
          logical_addr < src_addr_range.get_lsb() ||
          logical_addr > src_addr_range.get_msb() ||
          logical_data.size() != src_word_width ||
          !is_valid_bit_string(logical_data)) {
        VTR_LOG_ERROR(
          "mif_pipeline: invalid address/data in segment %zu for "
          "mif_source pb_type '%s'\n",
          static_cast<size_t>(segment_id), src_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }

      bool matched = false;
      for (const MifAddressMapRuleId& rule_id :
           bitstream_setting.mif_address_map_rules(map_id)) {
        const BasicPort rule_src_addr_range =
          bitstream_setting.mif_address_map_rule_src_addr_range(rule_id);
        if (!rule_src_addr_range.is_valid() ||
            logical_addr < rule_src_addr_range.get_lsb() ||
            logical_addr > rule_src_addr_range.get_msb()) {
          continue;
        }
        matched = true;
        if (!remap_logical_word(
              logical_addr, logical_data,
              bitstream_setting.mif_address_map_rule_des_addr_offset(rule_id),
              bitstream_setting.mif_address_map_rule_src_mif_bits(rule_id),
              bitstream_setting.mif_address_map_rule_des_mif_bits(rule_id),
              des_word_width, phys_data_map, phys_written_mask)) {
          return CMD_EXEC_FATAL_ERROR;
        }
      }
      if (!matched) {
        VTR_LOG_ERROR(
          "mif_pipeline: logical addr %lu is not covered by any map rule "
          "for src_pb_type '%s'\n",
          static_cast<unsigned long>(logical_addr), src_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  dest.clear();
  for (const auto& des_data_map : des_data_maps) {
    if (des_data_map.second.empty()) {
      continue;
    }
    const std::string& des_pb_type = des_data_map.first;
    const MifSourceSettingId des_source_id =
      bitstream_setting.find_mif_source_by_pb_type(des_pb_type);
    if (!des_source_id.is_valid()) {
      VTR_LOG_ERROR(
        "mif_pipeline: des_pb_type '%s' has no matching mif_source\n",
        des_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    const BasicPort des_addr_range =
      bitstream_setting.mif_source_address_range(des_source_id);
    const BasicPort des_data_range =
      bitstream_setting.mif_source_data_range(des_source_id);
    const MifSegmentId out_seg = dest.create_segment();
    dest.set_segment_physical_pb(out_seg, des_pb_type);
    dest.set_segment_data_width(out_seg,
                                static_cast<int>(des_data_range.get_width()));
    dest.set_segment_addr_range(out_seg, des_addr_range);

    for (const auto& addr_data : des_data_map.second) {
      dest.create_memory_line(out_seg, addr_data.first, addr_data.second);
    }
  }

  if (dest.empty()) {
    VTR_LOG_ERROR("mif_pipeline: no aggregated data produced\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
