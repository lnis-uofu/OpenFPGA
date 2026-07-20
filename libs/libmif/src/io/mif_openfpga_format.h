#pragma once

#include <cstdint>
#include <ostream>
#include <string>

#include "aggregated_mif_storage.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

/********************************************************************
 * Aggregated preload .mem output (Verilog $readmemh compatible):
 *   // Aggregated MIF for memory preloading interface
 *   // Address width: 6
 *   // Data width: 32
 *   0 0x00000000138F0000
 *
 * init.hex input parsing (depth/width from comments).
 *******************************************************************/
namespace openfpga {

constexpr const char* MIF_PRELOAD_MEM_TITLE =
  "Aggregated MIF for memory preloading interface";

void serialize_preload_mem(const AggregatedMifStorage& storage,
                           std::ostream& os);

int read_init_hex(const std::string& file_path, MifStorage& mif_storage);

bool parse_init_hex_content_line(const std::string& line, uint64_t& next_addr,
                                 uint64_t& addr, uint64_t& data);

} /* namespace openfpga */
