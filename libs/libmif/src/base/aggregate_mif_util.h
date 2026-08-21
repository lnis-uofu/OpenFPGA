#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "mif_storage.h"
#include "openfpga_port.h"

namespace openfpga {

/* Copy bits [lsb:msb] from a logical MIF word (LSB at index 0). */
std::string extract_mif_bits(const std::string& data, const BasicPort& bits);

/* Copy one segment (metadata + memory lines) from src into dest. */
void copy_mif_segment(const MifStorage& src, const MifSegmentId& seg,
                      MifStorage& dest);

bool is_valid_bit_string(const std::string& bits);

bool normalize_bit_string_width(std::string& bits, size_t target_width);

bool unpack_yosys_init_param(
  const std::string& bits, size_t data_width, size_t depth,
  std::vector<std::pair<uint64_t, std::string>>& words);

/* Remap one logical word into destination physical addr/data maps. */
bool remap_logical_word(uint64_t logical_addr, const std::string& logical_data,
                        int des_addr_offset, const BasicPort& src_mif_bits,
                        const BasicPort& des_mif_bits, size_t des_word_width,
                        std::map<uint64_t, std::string>& phys_data_map,
                        std::map<uint64_t, std::string>& phys_written_mask);

} /* namespace openfpga */
