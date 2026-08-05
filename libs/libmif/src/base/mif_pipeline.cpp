#include "mif_pipeline.h"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "bitstream_setting_xml_constants.h"
#include "openfpga_decode.h"
#include "vtr_log.h"
#include "write_mif.h"

namespace openfpga {

namespace {

MifStorage& stage_storage(MifPipeline::Stage stage, MifStorage& hex,
                          MifStorage& eblif, MifStorage& logical,
                          MifStorage& physical) {
  switch (stage) {
    case MifPipeline::Stage::HEX:
      return hex;
    case MifPipeline::Stage::EBLIF:
      return eblif;
    case MifPipeline::Stage::LOGICAL:
      return logical;
    case MifPipeline::Stage::PHYSICAL:
      return physical;
    default:
      return logical;
  }
}

} /* namespace */

void copy_mif_storage(const MifStorage& src, MifStorage& dest) {
  dest.clear();
  for (const MifSegmentId& seg : src.segments()) {
    const MifSegmentId new_seg = dest.create_segment();
    dest.set_segment_physical_pb(new_seg, src.physical_pb(seg));
    dest.set_segment_raw_data(new_seg, src.raw_data(seg));
    if (src.addr_range(seg).is_valid()) {
      dest.set_segment_addr_range(new_seg, src.addr_range(seg));
    }
    if (src.data_width(seg) >= 0) {
      dest.set_segment_data_width(new_seg, src.data_width(seg));
    }
    for (const MifMemoryLineId& line_id : src.segment_memory_lines(seg)) {
      dest.create_memory_line(new_seg, src.memory_line_address(line_id),
                              src.memory_line_data(line_id));
    }
  }
}

namespace {

bool address_in_range(uint64_t addr, const BasicPort& address_range) {
  return address_range.is_valid() && addr >= address_range.get_lsb() &&
         addr <= address_range.get_msb();
}

bool normalize_bit_string_width(std::string& bits, const size_t target_width) {
  if (bits.size() == target_width) {
    return true;
  }
  if (bits.size() < target_width) {
    bits.append(target_width - bits.size(), '0');
    return true;
  }
  for (size_t i = target_width; i < bits.size(); ++i) {
    if (bits[i] == '1') {
      return false;
    }
  }
  bits.resize(target_width);
  return true;
}

bool is_valid_bit_string(const std::string& bits) {
  for (const char bit : bits) {
    if (bit != '0' && bit != '1') {
      return false;
    }
  }
  return true;
}

std::string extract_mif_bits(const std::string& data, const BasicPort& bits) {
  std::string out(bits.get_width(), '0');
  for (size_t i = 0; i < bits.get_width(); ++i) {
    const size_t src = bits.get_lsb() + i;
    if (src < data.size()) {
      out[i] = data[src];
    }
  }
  return out;
}

bool unpack_yosys_init_param(
  const std::string& bits, size_t data_width, size_t depth,
  std::vector<std::pair<uint64_t, std::string>>& words) {
  words.clear();
  if (data_width == 0 || depth == 0 || bits.size() != data_width * depth) {
    VTR_LOG_ERROR(
      "mif_pipeline: INIT length %zu does not match depth %zu x width %zu\n",
      bits.size(), depth, data_width);
    return false;
  }

  for (size_t addr = 0; addr < depth; ++addr) {
    const size_t end = bits.size() - addr * data_width;
    const std::string word_bits_msb = bits.substr(end - data_width, data_width);
    bool all_undefined = true;
    std::string word_bits_lsb(data_width, '0');
    for (size_t i = 0; i < data_width; ++i) {
      const char bit = word_bits_msb[i];
      if (bit == '0' || bit == '1') {
        all_undefined = false;
        word_bits_lsb[data_width - 1 - i] = bit;
      } else if (bit != 'x' && bit != 'X' && bit != 'z' && bit != 'Z') {
        VTR_LOG_ERROR("mif_pipeline: invalid bit in INIT at address %zu\n",
                      addr);
        return false;
      }
    }
    if (!all_undefined) {
      words.emplace_back(static_cast<uint64_t>(addr), word_bits_lsb);
    }
  }
  return true;
}

bool remap_logical_word(uint64_t logical_addr, const std::string& logical_data,
                        int des_addr_offset, const BasicPort& src_mif_bits,
                        const BasicPort& des_mif_bits, size_t des_word_width,
                        std::map<uint64_t, std::string>& phys_data_map,
                        std::map<uint64_t, std::string>& phys_written_mask) {
  const uint64_t des_addr =
    static_cast<uint64_t>(static_cast<int64_t>(logical_addr) + des_addr_offset);
  const std::string extracted = extract_mif_bits(logical_data, src_mif_bits);

  auto& des_data = phys_data_map[des_addr];
  auto& des_written = phys_written_mask[des_addr];
  if (des_data.empty()) {
    des_data.assign(des_word_width, '0');
  }
  if (des_written.empty()) {
    des_written.assign(des_word_width, '0');
  }
  if (des_data.size() != des_word_width ||
      des_written.size() != des_word_width) {
    VTR_LOG_ERROR("mif_pipeline: destination word width mismatch at addr %lu\n",
                  static_cast<unsigned long>(des_addr));
    return false;
  }

  for (size_t i = 0; i < des_mif_bits.get_width(); ++i) {
    const size_t des_bit = des_mif_bits.get_lsb() + i;
    if (des_bit >= des_word_width) {
      VTR_LOG_ERROR(
        "mif_pipeline: des_mif_bits out of range for destination width %zu\n",
        des_word_width);
      return false;
    }
    const char new_bit = (i < extracted.size()) ? extracted[i] : '0';
    if (des_written[des_bit] == '1' && des_data[des_bit] != new_bit) {
      VTR_LOG_ERROR(
        "mif_pipeline: conflicting writes to des addr %lu bit %zu: "
        "existing '%c' vs new '%c' (logical addr %lu)\n",
        static_cast<unsigned long>(des_addr), des_bit, des_data[des_bit],
        new_bit, static_cast<unsigned long>(logical_addr));
      return false;
    }
    des_data[des_bit] = new_bit;
    des_written[des_bit] = '1';
  }
  return true;
}

std::vector<std::string> collect_eblif_mif_contents(
  const BitstreamSetting& bitstream_setting) {
  std::vector<std::string> contents;
  for (const MifSourceSettingId& id : bitstream_setting.mif_source_settings()) {
    if (bitstream_setting.mif_source_source(id) !=
        XML_MIF_SOURCE_SOURCE_EBLIF) {
      continue;
    }
    const std::string content = bitstream_setting.mif_source_content(id);
    bool exists = false;
    for (const std::string& existing : contents) {
      if (existing == content) {
        exists = true;
        break;
      }
    }
    if (!exists) {
      contents.push_back(content);
    }
  }
  return contents;
}

int decode_logical_segment(MifStorage& logical_storage,
                           const MifSegmentId& segment_id,
                           const BitstreamSetting& bitstream_setting) {
  const std::string vpr_pb_type = logical_storage.physical_pb(segment_id);
  const MifSourceSettingId source_id =
    bitstream_setting.find_mif_source_by_pb_type(vpr_pb_type);
  if (!source_id.is_valid()) {
    VTR_LOG_ERROR("mif_pipeline: segment %zu pb_type '%s' has no mif_source\n",
                  static_cast<size_t>(segment_id), vpr_pb_type.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  logical_storage.set_segment_physical_pb(
    segment_id, bitstream_setting.mif_source_pb_type(source_id));
  const BasicPort addr_range =
    bitstream_setting.mif_source_address_range(source_id);
  const BasicPort data_range =
    bitstream_setting.mif_source_data_range(source_id);
  logical_storage.set_segment_addr_range(segment_id, addr_range);
  logical_storage.set_segment_data_width(
    segment_id, static_cast<int>(data_range.get_width()));

  if (logical_storage.raw_data(segment_id).empty()) {
    for (const MifMemoryLineId& line_id :
         logical_storage.segment_memory_lines(segment_id)) {
      std::string data_bits = logical_storage.memory_line_data(line_id);
      if (!is_valid_bit_string(data_bits) ||
          !normalize_bit_string_width(data_bits, data_range.get_width())) {
        VTR_LOG_ERROR(
          "mif_pipeline: cannot fit hex word at addr %lu into data_range "
          "width %zu for pb_type '%s'\n",
          static_cast<unsigned long>(
            logical_storage.memory_line_address(line_id)),
          data_range.get_width(),
          bitstream_setting.mif_source_pb_type(source_id).c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      logical_storage.set_memory_line_data(line_id, data_bits);
    }
    return CMD_EXEC_SUCCESS;
  }

  std::vector<std::pair<uint64_t, std::string>> words;
  if (!unpack_yosys_init_param(logical_storage.raw_data(segment_id),
                               data_range.get_width(), addr_range.get_width(),
                               words)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  for (const auto& word : words) {
    logical_storage.create_memory_line(
      segment_id, word.first + addr_range.get_lsb(), word.second);
  }
  logical_storage.set_segment_raw_data(segment_id, std::string());
  return CMD_EXEC_SUCCESS;
}

} /* namespace */

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
    default:
      return logical_;
  }
}

MifStorage& MifPipeline::mutable_storage(Stage stage) {
  return stage_storage(stage, hex_, eblif_, logical_, physical_);
}

void MifPipeline::clear() {
  hex_.clear();
  eblif_.clear();
  logical_.clear();
  physical_.clear();
}

void MifPipeline::clear(Stage stage) { mutable_storage(stage).clear(); }

int MifPipeline::load_eblif(const std::string& eblif_path,
                            const BitstreamSetting& bitstream_setting,
                            const MifPbTypeResolver& pb_type_resolver) {
  eblif_.clear();
  if (!bitstream_setting.has_eblif_mif_source()) {
    return CMD_EXEC_SUCCESS;
  }
  if (!pb_type_resolver) {
    VTR_LOG_ERROR(
      "mif_pipeline: mif_source source='%s' requires a pb_type resolver\n",
      XML_MIF_SOURCE_SOURCE_EBLIF);
    return CMD_EXEC_FATAL_ERROR;
  }
  const std::vector<std::string> eblif_contents =
    collect_eblif_mif_contents(bitstream_setting);
  return read_mif_from_eblif(eblif_path, eblif_, pb_type_resolver,
                             eblif_contents);
}

int MifPipeline::merge_to_logical(const BitstreamSetting& bitstream_setting) {
  logical_.clear();
  copy_mif_storage(hex_, logical_);

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

    const std::string& pb = eblif_.physical_pb(eblif_seg);
    const std::string& raw = eblif_.raw_data(eblif_seg);

    MifSegmentId matched_seg;
    bool found = false;
    for (const MifSegmentId& logical_seg : logical_.segments()) {
      if (logical_.physical_pb(logical_seg) != pb) {
        continue;
      }
      matched_seg = logical_seg;
      found = true;
      break;
    }

    if (found) {
      VTR_LOG("mif_pipeline: overwrite logical pb_type '%s' from eblif\n",
              pb.c_str());
      logical_.clear_segment_memory_lines(matched_seg);
      logical_.set_segment_physical_pb(matched_seg, pb);
      logical_.set_segment_raw_data(matched_seg, raw);
    } else {
      const MifSegmentId new_seg = logical_.create_segment();
      logical_.set_segment_physical_pb(new_seg, pb);
      logical_.set_segment_raw_data(new_seg, raw);
    }
  }

  if (eblif_segment_count == 0) {
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
    const int decode_status =
      decode_logical_segment(logical_, segment_id, bitstream_setting);
    if (CMD_EXEC_SUCCESS != decode_status) {
      return decode_status;
    }
  }
  return CMD_EXEC_SUCCESS;
}

int MifPipeline::pad_logical_zeros(
  const BitstreamSetting& /*bitstream_setting*/) {
  for (const MifSegmentId& segment_id : logical_.segments()) {
    const BasicPort& addr_range = logical_.addr_range(segment_id);
    if (!addr_range.is_valid()) {
      continue;
    }
    const size_t data_width =
      static_cast<size_t>(logical_.data_width(segment_id));
    if (data_width == 0) {
      continue;
    }

    std::map<uint64_t, bool> has_addr;
    for (const MifMemoryLineId& line_id :
         logical_.segment_memory_lines(segment_id)) {
      has_addr[logical_.memory_line_address(line_id)] = true;
    }

    const std::string zero_word(data_width, '0');
    for (uint64_t addr = addr_range.get_lsb(); addr <= addr_range.get_msb();
         ++addr) {
      if (!has_addr[addr]) {
        logical_.create_memory_line(segment_id, addr, zero_word);
      }
    }
  }
  return CMD_EXEC_SUCCESS;
}

int MifPipeline::aggregate_to_physical(
  const BitstreamSetting& bitstream_setting) {
  physical_.clear();
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
      if (!address_in_range(logical_addr, src_addr_range) ||
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
        if (!address_in_range(logical_addr, rule_src_addr_range)) {
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

int MifPipeline::run(const BitstreamSetting& bitstream_setting,
                     const MifPbTypeResolver& pb_type_resolver,
                     const std::string& eblif_file_path) {
  if (bitstream_setting.has_other_mif_source() && hex_.empty() &&
      !bitstream_setting.has_eblif_mif_source()) {
    VTR_LOG_ERROR(
      "mif_pipeline: hex MIF storage is empty; source='%s' requires "
      "read_mif, or add a source='%s' mif_source\n",
      XML_MIF_SOURCE_SOURCE_OTHERS, XML_MIF_SOURCE_SOURCE_EBLIF);
    return CMD_EXEC_FATAL_ERROR;
  }

  if (bitstream_setting.has_eblif_mif_source()) {
    const std::string path =
      eblif_file_path.empty() ? find_yosys_eblif_file_path() : eblif_file_path;
    if (path.empty()) {
      return CMD_EXEC_FATAL_ERROR;
    }
    const int load_status =
      load_eblif(path, bitstream_setting, pb_type_resolver);
    if (CMD_EXEC_SUCCESS != load_status) {
      return load_status;
    }
  }

  const int merge_status = merge_to_logical(bitstream_setting);
  if (CMD_EXEC_SUCCESS != merge_status) {
    return merge_status;
  }
  if (logical_.empty()) {
    VTR_LOG("mif_pipeline: empty logical MIF storage; nothing to aggregate\n");
    return CMD_EXEC_SUCCESS;
  }

  const int decode_status = decode_logical(bitstream_setting);
  if (CMD_EXEC_SUCCESS != decode_status) {
    return decode_status;
  }

  return aggregate_to_physical(bitstream_setting);
}

int MifPipeline::write_stage(const std::string& file_path, Stage stage) const {
  return write_mif(file_path, storage(stage));
}

} /* namespace openfpga */
