#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace openfpga {

void trim_mif_line_inplace(std::string& s);

/* Strip trailing // comments from init.hex lines. */
std::string strip_mif_line_comment(const std::string& raw_line);

/* Parse init.hex data tokens (bare hex digits use base 16, not octal). */
bool parse_mif_init_hex_value_token(const std::string& tok, uint64_t& out);

int mif_bit_width_for_max_value(uint64_t max_value);

/* Parse optional init.hex header comments, e.g. depth/width metadata. */
bool try_parse_init_hex_depth_metadata(const std::string& comment_line,
                                       int& depth, uint64_t& min_addr,
                                       uint64_t& max_addr);
bool try_parse_init_hex_width_metadata(const std::string& comment_line,
                                       int& width);

std::string mif_file_basename(const std::string& file_path);

/* Remove [index] suffixes from OpenFPGA pb_type paths, e.g.
 * dpram8x32[0]{dual}.dpram8x16[1] -> dpram8x32{dual}.dpram8x16 */
std::string strip_pb_type_indices(const std::string& indexed_pb_type);

} /* namespace openfpga */
