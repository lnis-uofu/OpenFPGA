#pragma once

#include <map>
#include <string>

#include "aggregated_mif_storage.h"
#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Aggregate logical init.hex segments into preload .mem data per physical pb.
 *
 * For each logical segment:
 *   - resolve Verilog instance via $readmemh source file
 *   - look up index-level pb_type in instance_pb_type_path_map
 *   - apply mif_address_map address_offset / data_offset from bitstream_setting
 *
 * Segments sharing the same index-stripped pb_type are merged:
 *   phys_addr = logical_addr << address_offset
 *   phys_data[phys_addr] |= logical_data << data_offset
 */
int aggregate_mif(
  const MifStorage& logical_storage, const std::string& verilog_path,
  const std::map<std::string, std::string>& instance_pb_type_path_map,
  const BitstreamSetting& bitstream_setting,
  AggregatedMifStorage& out_aggregated_storage);

} /* namespace openfpga */
