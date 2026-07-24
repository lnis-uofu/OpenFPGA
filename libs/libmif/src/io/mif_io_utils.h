#pragma once

#include <cstdint>
#include <string>

namespace openfpga {

/* Strip trailing // comments from init.hex lines. */
std::string strip_mif_line_comment(const std::string& raw_line);

/* Parse init.hex data tokens (bare hex digits use base 16, not octal). */
bool parse_mif_init_hex_value_token(const std::string& tok, uint64_t& out);

int mif_bit_width_for_max_value(uint64_t max_value);

/* Parse optional init.hex depth header comments.
 * These comments are NOT required. On success, min_addr/max_addr are the
 * inclusive address range (from "indexed from A to B", or [0, depth-1]
 * when only depth is given). */
bool try_parse_init_hex_depth_metadata(const std::string& comment_line,
                                       int& depth, uint64_t& min_addr,
                                       uint64_t& max_addr);

/* Parse one init.hex data line. Supported forms:
 *   <data>           sequential address (next_addr)
 *   <addr> <data>    explicit address/data pair
 *   @<addr> <data>   address jump (same as explicit pair)
 */
bool parse_init_hex_line(const std::string& line, uint64_t& next_addr,
                         uint64_t& addr, uint64_t& data);

} /* namespace openfpga */
