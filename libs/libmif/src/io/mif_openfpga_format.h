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
 *   // addr[0:7]
 *   // Address width: 3
 *   // Data width: 32
 *   0 0x00000000138F0000
 *
 * init.hex input parsing (depth/width from comments):
 *   <data>           sequential address
 *   <addr> <data>    explicit address/data pair
 *   @<addr> <data>   address jump
 *******************************************************************/
namespace openfpga {

constexpr const char* MIF_PRELOAD_MEM_TITLE =
  "Aggregated MIF for memory preloading interface";

void serialize_preload_mem(const AggregatedMifStorage& storage,
                           std::ostream& os);

int read_init_hex(const std::string& file_path, MifStorage& mif_storage);

} /* namespace openfpga */
