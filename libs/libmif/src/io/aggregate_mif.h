#pragma once

#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Aggregate logical init.hex segments into preload .mem data per physical pb.
 *
 * Binding is bitstream-setting only (no Verilog instance / placement map):
 *   - requires exactly one mif_address_map (strategy A)
 *   - all logical segments use that map's src_pb_type / des_pb_type
 *   - sanity-check addresses/data against mif_source ranges
 *
 * Output .mem header addr/data metadata is derived from <map> rules for the
 * des_pb_type:
 *   addr[min:max]  = union of (src_addr_range + des_addr_offset)
 *   data width     = max(des_mif_bits.msb) + 1
 *   address width  = bit width of max mapped address
 * src_mif_bits / des_mif_bits widths must match.
 *
 * Full map-rule remapping of addr/data bits is left to the later bitstream
 * stage.
 *
 * logical_storage: from read_mif (init.hex)
 * out_aggregated_storage: aggregated preload result for write_mif
 */
int aggregate_mif(const MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage);

} /* namespace openfpga */
