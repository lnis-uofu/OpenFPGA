#pragma once

#include <map>
#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Aggregate logical init.hex segments into preload .mem data per physical pb.
 *
 * For each logical segment:
 *   - resolve Verilog instance via $readmemh source file
 *   - look up src_pb_type in instance_pb_type_path_map
 *   - resolve mif_address_map by src_pb_type and group by des_pb_type
 *   - sanity-check every logical address against mif_source.address_range
 *     (and optional init.hex depth comment range if present)
 *   - sanity-check data against mif_source.data_range width
 *     (init.hex width comments are optional and not required)
 *
 * Output .mem header addr/data metadata is derived from <map> rules for the
 * des_pb_type:
 *   addr[min:max]  = union of (src_addr_range + des_addr_offset)
 *   data width     = max(des_mif_bits.msb) + 1
 *   address width  = bit width of max mapped address
 * src_mif_bits / des_mif_bits widths must match.
 *
 * Segments sharing the same des_pb_type are merged at the same logical
 * address. When a leaf RAM index is present in the src path, slices are
 * appended by index. Full map-rule remapping of addr/data bits is left to the
 * later bitstream stage.
 *
 * logical_storage: from read_mif (init.hex)
 * out_aggregated_storage: aggregated preload result for write_mif
 */
int aggregate_mif(
  const MifStorage& logical_storage, const std::string& verilog_path,
  const std::map<std::string, std::string>& instance_pb_type_path_map,
  const BitstreamSetting& bitstream_setting,
  MifStorage& out_aggregated_storage);

} /* namespace openfpga */
