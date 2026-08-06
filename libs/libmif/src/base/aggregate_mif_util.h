#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "mif_storage.h"
#include "openfpga_port.h"

namespace openfpga {

/* Copy one segment (metadata + memory lines) from src into dest. */
void copy_mif_segment(const MifStorage& src, const MifSegmentId& seg,
                      MifStorage& dest);

bool is_valid_bit_string(const std::string& bits);

/* Remap one logical word into destination physical addr/data maps. */
bool remap_logical_word(uint64_t logical_addr, const std::string& logical_data,
                        int des_addr_offset, const BasicPort& src_mif_bits,
                        const BasicPort& des_mif_bits, size_t des_word_width,
                        std::map<uint64_t, std::string>& phys_data_map,
                        std::map<uint64_t, std::string>& phys_written_mask);

} /* namespace openfpga */
