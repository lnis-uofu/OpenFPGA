#pragma once

#include <cstddef>
#include <ostream>
#include <string>

#include "mif_storage.h"

/********************************************************************
 * OpenFPGA custom MIF text format (example):
    // line comments
    // Comments are allowed using double slashes
    00 // Address 0x00: Data value 0
    01 // Address 0x01: Data value 1    
    02 // Address 0x02: Data value 2
    03 // Address 0x03: Data value 3
    0A // Address 0x04: Data value 10
    0B // Address 0x05: Data value 11
    0C // Address 0x06: Data value 12
    @0A FF // Jump to Address 0x0A and set value to 255
    @0B FE // Address 0x0B: Data value 254
 *******************************************************************/
namespace openfpga {

constexpr const char* MIF_DIRECTIVE_X = "X";
constexpr const char* MIF_DIRECTIVE_Y = "Y";
constexpr const char* MIF_DIRECTIVE_RAM_ID = "RAM_ID";
constexpr const char* MIF_DIRECTIVE_ADDR_WIDTH = "ADDR_WIDTH";
constexpr const char* MIF_DIRECTIVE_DATA_WIDTH = "DATA_WIDTH";
constexpr const char* MIF_DIRECTIVE_ID_WIDTH = "ID_WIDTH";

void serialize_openfpga_mif(const MifStorage& storage, std::ostream& os);

/* Parse one raw MIF input line (comments stripped inside). */
int parse_mif_line(const std::string& file_path, size_t line_no,
                   const std::string& raw_line, MifStorage& mif_storage,
                   bool& has_current_segment, MifSegmentId& current_segment_id,
                   size_t& total_words);

/* Validate parse results and drop a trailing empty segment if needed. */
int finalize_openfpga_mif_parse(MifStorage& mif_storage,
                                bool has_current_segment,
                                const MifSegmentId& current_segment_id,
                                size_t total_words,
                                const std::string& file_path);

} /* namespace openfpga */
