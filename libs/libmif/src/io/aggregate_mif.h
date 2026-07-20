#pragma once

#include <map>
#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "aggregated_mif_storage.h"
#include "mif_address_map.h"
#include "mif_storage.h"

namespace openfpga {

MifAddressMap mif_address_map_from_bitstream_setting(
  const BitstreamSetting& bitstream_setting);

/* Aggregate logical init.hex segments into preload .mem data per physical pb.
 *
 * For each logical segment:
 *   - resolve Verilog instance via $readmemh source file
 *   - look up index-level pb_type in instance_pb_type_path_map
 *   - apply mif_address_map address_offset / data_offset
 *
 * Segments sharing the same index-stripped pb_type are merged:
 *   phys_addr = logical_addr << address_offset
 *   phys_data[phys_addr] |= logical_data << data_offset
 */
int aggregate_mif(const MifStorage& logical_storage,
                   const std::string& verilog_path,
                   const std::map<std::string, std::string>&
                     instance_pb_type_path_map,
                   const MifAddressMap& mif_address_map,
                   AggregatedMifStorage& out_aggregated_storage);

} /* namespace openfpga */
