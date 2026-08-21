#include "aggregate_mif_util.h"

#include "vtr_log.h"

namespace openfpga {

/* Slice logical-word bits selected by XML src_mif_bits.
 * data is LSB-at-index-0; missing high bits are padded with '0'. */
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

bool is_valid_bit_string(const std::string& bits) {
  for (const char bit : bits) {
    if (bit != '0' && bit != '1') {
      return false;
    }
  }
  return true;
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

} /* namespace openfpga */
