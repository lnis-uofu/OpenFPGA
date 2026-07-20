#pragma once

#include <map>
#include <string>
#include <utility>

#include "command_exit_codes.h"
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
 * Bind placement coordinates into mif_storage:
 * 1) find Verilog instance that $readmemh each MIF source
 * 2) look up (x, y) by instance name from inst_coord_map (placement)
 * 3) write X/Y into mif_storage segments
 *
 * Operating->physical packing (pb_type / address_offset / data_offset)
 * is handled separately via MifAddressMap during aggregation.
 *******************************************************************/
int bind_bram_to_mif_storage(
  MifStorage& mif_storage, const std::string& verilog_path,
  const std::map<std::string, std::pair<int, int>>& inst_coord_map);

} /* namespace openfpga */
