#pragma once

#include <map>
#include <string>
#include <utility>

#include "command_exit_codes.h"
#include "memory_address_map.h"
#include "mif_storage.h"

namespace openfpga {

/********************************************************************
 * Find the Verilog instance (e.g. memory_0) that reads the given
 * MIF/hex file via $readmemh (parameter default or instance override).
 * Returns empty string if not found.
 *******************************************************************/
std::string find_verilog_instance_reading_mif(const std::string& verilog_path,
                                              const std::string& mif_file_name);

/********************************************************************
 * Bind BRAM info into mif_storage:
 * 1) find Verilog instance that $readmemh each MIF source
 * 2) look up (x, y) by instance name from inst_coord_map (placement)
 * 3) look up that (x, y) in memory_address_map for ram_id / id_width /
 *    addr_width / data_width; error if (x, y) is missing
 * 4) write X/Y/RAM_ID/widths into mif_storage segments
 *******************************************************************/
int bind_bram_to_mif_storage(
  MifStorage& mif_storage, const std::string& verilog_path,
  const std::map<std::string, std::pair<int, int>>& inst_coord_map,
  const MemoryAddressMap& memory_address_map);

} /* namespace openfpga */
