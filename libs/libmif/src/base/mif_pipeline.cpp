#include "mif_pipeline.h"

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "aggregate_mif_util.h"
#include "bitstream_setting_xml_constants.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "write_mif.h"

namespace openfpga {

bool MifPipeline::physical_segment_has_grid_coord(
  const MifSegmentId& segment_id) const {
  VTR_ASSERT(physical_.valid_segment_id(segment_id));
  VTR_ASSERT(physical_segment_grid_coords_.size() > size_t(segment_id));
  return physical_segment_grid_coords_[segment_id].is_valid();
}

const MifGridCoord& MifPipeline::physical_segment_grid_coord(
  const MifSegmentId& segment_id) const {
  VTR_ASSERT(physical_segment_has_grid_coord(segment_id));
  return physical_segment_grid_coords_[segment_id];
}

void MifPipeline::set_physical_segment_grid_coord(
  const MifSegmentId& segment_id, int x, int y, int z) {
  VTR_ASSERT(physical_.valid_segment_id(segment_id));
  VTR_ASSERT(physical_segment_grid_coords_.size() == physical_.num_segments());
  physical_segment_grid_coords_[segment_id] = MifGridCoord{x, y, z};
}

const MifStorage& MifPipeline::storage(Stage stage) const {
  switch (stage) {
    case Stage::HEX:
      return hex_;
    case Stage::EBLIF:
      return eblif_;
    case Stage::LOGICAL:
      return logical_;
    case Stage::PHYSICAL:
      return physical_;
    case Stage::UNIFIED:
      return unified_;
    default:
      return logical_;
  }
}

MifStorage& MifPipeline::mutable_storage(Stage stage) {
  return const_cast<MifStorage&>(
    static_cast<const MifPipeline*>(this)->storage(stage));
}

void MifPipeline::clear() {
  hex_.clear();
  eblif_.clear();
  logical_.clear();
  physical_.clear();
  unified_.clear();
  physical_segment_grid_coords_.clear();
}

void MifPipeline::clear(Stage stage) {
  mutable_storage(stage).clear();
  if (Stage::PHYSICAL == stage) {
    physical_segment_grid_coords_.clear();
  }
}

int MifPipeline::merge_to_logical(const BitstreamSetting& bitstream_setting) {
  logical_.clear();

  /* HEX: only for pb_types not configured as source="eblif". */
  for (const MifSegmentId& hex_seg : hex_.segments()) {
    const std::string& pb = hex_.physical_pb(hex_seg);
    if (bitstream_setting.pb_type_is_eblif_mif_source(pb)) {
      VTR_LOG(
        "mif_pipeline: skip hex segment for eblif mif_source pb_type '%s'\n",
        pb.c_str());
      continue;
    }
    copy_mif_segment(hex_, hex_seg, logical_);
  }

  if (eblif_.empty()) {
    return CMD_EXEC_SUCCESS;
  }

  size_t eblif_segment_count = 0;
  for (const MifSegmentId& eblif_seg : eblif_.segments()) {
    if (!bitstream_setting.pb_type_is_eblif_mif_source(
          eblif_.physical_pb(eblif_seg))) {
      continue;
    }
    ++eblif_segment_count;
    copy_mif_segment(eblif_, eblif_seg, logical_);
  }

  if (bitstream_setting.has_eblif_mif_source() && eblif_segment_count == 0) {
    VTR_LOG_ERROR(
      "mif_pipeline: eblif loaded but no segment matches any mif_source with "
      "source='%s'\n",
      XML_MIF_SOURCE_SOURCE_EBLIF);
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

int MifPipeline::decode_logical(const BitstreamSetting& bitstream_setting) {
  for (const MifSegmentId& segment_id : logical_.segments()) {
    const std::string vpr_pb_type = logical_.physical_pb(segment_id);
    const MifSourceSettingId source_id =
      bitstream_setting.find_mif_source_by_pb_type(vpr_pb_type);
    if (!source_id.is_valid()) {
      VTR_LOG_ERROR(
        "mif_pipeline: segment %zu pb_type '%s' has no mif_source\n",
        static_cast<size_t>(segment_id), vpr_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    logical_.set_segment_physical_pb(
      segment_id, bitstream_setting.mif_source_pb_type(source_id));
    const BasicPort addr_range =
      bitstream_setting.mif_source_address_range(source_id);
    const BasicPort data_range =
      bitstream_setting.mif_source_data_range(source_id);
    logical_.set_segment_addr_range(segment_id, addr_range);
    logical_.set_segment_data_width(segment_id,
                                    static_cast<int>(data_range.get_width()));

    if (logical_.raw_data(segment_id).empty()) {
      for (const MifMemoryLineId& line_id :
           logical_.segment_memory_lines(segment_id)) {
        std::string data_bits = logical_.memory_line_data(line_id);
        if (!is_valid_bit_string(data_bits) ||
            !normalize_bit_string_width(data_bits, data_range.get_width())) {
          VTR_LOG_ERROR(
            "mif_pipeline: cannot fit hex word at addr %lu into data_range "
            "width %zu for pb_type '%s'\n",
            static_cast<unsigned long>(logical_.memory_line_address(line_id)),
            data_range.get_width(),
            bitstream_setting.mif_source_pb_type(source_id).c_str());
          return CMD_EXEC_FATAL_ERROR;
        }
        logical_.set_memory_line_data(line_id, data_bits);
      }
      continue;
    }

    std::vector<std::pair<uint64_t, std::string>> words;
    if (!unpack_yosys_init_param(logical_.raw_data(segment_id),
                                 data_range.get_width(), addr_range.get_width(),
                                 words)) {
      return CMD_EXEC_FATAL_ERROR;
    }
    for (const auto& word : words) {
      logical_.create_memory_line(segment_id, word.first + addr_range.get_lsb(),
                                  word.second);
    }
    logical_.set_segment_raw_data(segment_id, std::string());
  }
  return CMD_EXEC_SUCCESS;
}

int MifPipeline::aggregate_to_physical(
  const BitstreamSetting& bitstream_setting) {
  physical_.clear();
  physical_segment_grid_coords_.clear();
  if (bitstream_setting.mif_address_map_settings().empty()) {
    VTR_LOG_ERROR("mif_pipeline: no mif_address_map in bitstream setting\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (logical_.empty()) {
    VTR_LOG("mif_pipeline: empty logical MIF storage; nothing to aggregate\n");
    return CMD_EXEC_SUCCESS;
  }

  std::map<std::string, std::map<uint64_t, std::string>> des_data_maps;
  std::map<std::string, std::map<uint64_t, std::string>> des_written_masks;

  for (const MifSegmentId& segment_id : logical_.segments()) {
    if (!logical_.has_physical_pb(segment_id)) {
      VTR_LOG_ERROR(
        "mif_pipeline: segment %zu has no pb_type from its mif_source\n",
        static_cast<size_t>(segment_id));
      return CMD_EXEC_FATAL_ERROR;
    }
    const std::string& src_pb_type = logical_.physical_pb(segment_id);
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
    const BasicPort& src_addr_range = logical_.addr_range(segment_id);
    const size_t src_word_width =
      static_cast<size_t>(logical_.data_width(segment_id));
    auto& phys_data_map = des_data_maps[des_pb_type];
    auto& phys_written_mask = des_written_masks[des_pb_type];

    VTR_LOG("mif_pipeline: segment %zu bound src='%s' -> des='%s'\n",
            static_cast<size_t>(segment_id), src_pb_type.c_str(),
            des_pb_type.c_str());

    for (const MifMemoryLineId& line_id :
         logical_.segment_memory_lines(segment_id)) {
      const uint64_t logical_addr = logical_.memory_line_address(line_id);
      const std::string& logical_data = logical_.memory_line_data(line_id);
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
    const MifSegmentId out_seg = physical_.create_segment();
    physical_segment_grid_coords_.emplace_back();
    physical_.set_segment_physical_pb(out_seg, des_pb_type);
    physical_.set_segment_data_width(
      out_seg, static_cast<int>(des_data_range.get_width()));
    physical_.set_segment_addr_range(out_seg, des_addr_range);

    for (const auto& addr_data : des_data_map.second) {
      physical_.create_memory_line(out_seg, addr_data.first, addr_data.second);
    }
  }

  if (physical_.empty()) {
    VTR_LOG_ERROR("mif_pipeline: no aggregated data produced\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

int MifPipeline::write_stage(const std::string& file_path, Stage stage) const {
  return write_mif(file_path, storage(stage));
}

} /* namespace openfpga */
