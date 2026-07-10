#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace openfpga {

void trim_mif_line_inplace(std::string& s);

/* Strip // comments; keep // directive lines intact for OpenFPGA MIF parsing. */
std::string strip_mif_line_comment(const std::string& raw_line);

bool parse_mif_u64_token(const std::string& tok, uint64_t& out);

/* Parse init.hex data tokens (bare hex digits use base 16, not octal). */
bool parse_mif_init_hex_value_token(const std::string& tok, uint64_t& out);

int mif_bit_width_for_max_value(uint64_t max_value);

bool path_has_file_extension(const std::string& file_path,
                             const char* extension);

/* Return filename component of a path (no directory). */
std::string mif_file_basename(const std::string& file_path);

} /* namespace openfpga */
