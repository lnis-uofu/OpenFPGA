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
 *   - group by index-stripped pb_type from mif_address_map
 *
 * Segments sharing the same physical pb are merged at the same logical
 * address by RAM slice index (last numeric [N] in the pb_type path). Slice [0]
 * occupies the MSB, higher indices append toward the LSB:
 *   bit_shift = (max_slice_index - slice_index) * slice_data_width
 *   aggregated_data[logical_addr] |= logical_data << bit_shift
 *
 * address_offset / data_offset in mif_address_map are NOT used here for either
 * dual (width packing) or dualA (depth packing). Both modes merge slices by RAM
 * index only. Offsets apply in the later bitstream stage when mapping
 * aggregated data onto physical configuration address/data ports.
 *
 * Slices without MIF contribute zero at each address. aggregated_data_width is
 * (max_slice_index + 1) * slice_data_width, inferred from mif_address_map.
 */
int aggregate_mif(
  const MifStorage& logical_storage, const std::string& verilog_path,
  const std::map<std::string, std::string>& instance_pb_type_path_map,
  const BitstreamSetting& bitstream_setting,
  AggregatedMifStorage& out_aggregated_storage);

} /* namespace openfpga */
