#pragma once

#include <cstddef>
#include <ostream>
#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

/********************************************************************
 * MIF text I/O for libmif.
 *
 * Two input formats are supported by read_mif(); both use // comments.
 *
 * 1) OpenFPGA custom MIF (.mif or extensionless files that match):
 *      // X <int>  // Y <int>           optional tile coordinates
 *      // ADDR_WIDTH <int>  // DATA_WIDTH <int>
 *      // Address Data
 *      0x2000 0x0000000a                 explicit address + data pair
 *      //RAM_ID <int>                    starts a new RAM block section
 *      0x2e00 0x0000038f
 *
 * 2) Verilog $readmemh init.hex (.hex or files without OpenFPGA markers):
 *      // comment
 *      <hex_data>                        sequential address, auto-increment
 *      @<hex_addr> <hex_data>            address jump
 *
 * write_mif() always emits the OpenFPGA custom MIF format.
 *******************************************************************/
namespace openfpga {

constexpr const char* MIF_DIRECTIVE_X = "X";
constexpr const char* MIF_DIRECTIVE_Y = "Y";
constexpr const char* MIF_DIRECTIVE_RAM_ID = "RAM_ID";
constexpr const char* MIF_DIRECTIVE_ADDR_WIDTH = "ADDR_WIDTH";
constexpr const char* MIF_DIRECTIVE_DATA_WIDTH = "DATA_WIDTH";
constexpr const char* MIF_DIRECTIVE_ID_WIDTH = "ID_WIDTH";

/* --- OpenFPGA MIF serialization ----------------------------------- */

void serialize_openfpga_mif(const MifStorage& storage, std::ostream& os);

/* --- OpenFPGA MIF parsing (line-at-a-time API for read_mif) ------- */

int parse_mif_line(const std::string& file_path, size_t line_no,
                   const std::string& raw_line, MifStorage& mif_storage,
                   bool& has_current_segment, MifSegmentId& current_segment_id,
                   size_t& total_words);

int finalize_openfpga_mif_parse(MifStorage& mif_storage,
                                bool has_current_segment,
                                const MifSegmentId& current_segment_id,
                                size_t total_words,
                                const std::string& file_path);

/* --- Verilog init.hex parsing ------------------------------------- */

bool is_init_hex_file(const std::string& file_path);

int read_init_hex(const std::string& file_path, MifStorage& mif_storage);

/* Test helper: parse one init.hex content line (comments already stripped). */
bool parse_init_hex_content_line(const std::string& line, uint64_t& next_addr,
                                 uint64_t& addr, uint64_t& data);

} /* namespace openfpga */
