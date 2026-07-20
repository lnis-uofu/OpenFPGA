#pragma once

#include <cstddef>
#include <ostream>
#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

/********************************************************************
 * OpenFPGA custom MIF text format (example):
 *   // line comments
 *   // X <int>  // Y <int>  (optional tile coordinates)
 *   // ADDR_WIDTH <int>  // DATA_WIDTH <int>
 *   // Address Data
 *   0x2000 0x0000000a
 *   //RAM_ID <int>  (starts a new RAM block section)
 *   0x2e00 0x0000038f
 *
 * Verilog $readmemh init.hex parsing lives in the same translation unit.
 *******************************************************************/
namespace openfpga {

constexpr const char* MIF_DIRECTIVE_X = "X";
constexpr const char* MIF_DIRECTIVE_Y = "Y";
constexpr const char* MIF_DIRECTIVE_PB_TYPE = "PB_TYPE";
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

int read_init_hex(const std::string& file_path, MifStorage& mif_storage);

bool is_init_hex_file(const std::string& file_path);

bool parse_init_hex_content_line(const std::string& line, uint64_t& next_addr,
                                 uint64_t& addr, uint64_t& data);

} /* namespace openfpga */
