#pragma once

#include <cstdint>
#include <ostream>
#include <string>

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
 * init.hex input parsing:
 *   <data>           sequential address
 *   <addr> <data>    explicit address/data pair
 *   @<addr> <data>   address jump
 *
 * Optional comment metadata (NOT required):
 *   // depth: 8 (indexed from 0 to 7)
 *   // width: 16
 * Address/data widths for aggregation and output headers come from bitstream
 * setting (mif_source / mif_address_map), not from init.hex comments.
 *******************************************************************/
namespace openfpga {

constexpr const char* MIF_PRELOAD_MEM_TITLE =
  "Aggregated MIF for memory preloading interface";

/* Serialize aggregated preload MIF (output of aggregate_mif). */
void serialize_preload_mem(const MifStorage& storage, std::ostream& os);

int read_init_hex(const std::string& file_path, MifStorage& mif_storage);

} /* namespace openfpga */
