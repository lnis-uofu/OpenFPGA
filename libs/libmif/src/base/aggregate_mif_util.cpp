#include "aggregate_mif_util.h"

#include "bitstream_setting_xml_constants.h"
#include "vtr_log.h"

namespace openfpga {

namespace {

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

} /* namespace */

void copy_mif_segment(const MifStorage& src, const MifSegmentId& seg,
                      MifStorage& dest) {
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

bool address_in_range(uint64_t addr, const BasicPort& address_range) {
  return address_range.is_valid() && addr >= address_range.get_lsb() &&
         addr <= address_range.get_msb();
}

bool is_valid_bit_string(const std::string& bits) {
  for (const char bit : bits) {
    if (bit != '0' && bit != '1') {
      return false;
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

} /* namespace openfpga */
