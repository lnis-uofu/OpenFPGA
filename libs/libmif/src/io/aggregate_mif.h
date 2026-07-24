#pragma once

#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Aggregate logical MIF segments into preload .mem data per physical pb.
 *
 * Binding is bitstream-setting only (no Verilog instance / placement map):
 *   - one or more mif_address_map entries are allowed if they share des_pb_type
 *   - supported patterns:
 *       (1) mode catalog: several src modes -> one physical pb (only matching
 *           src maps are applied; unused maps are ignored)
 *       (2) sibling pack: several src halves -> same des addr, OR by bits
 *   - each logical segment is bound by physical_pb() == map.src_pb_type
 *     (set by read_mif from eblif mif_source); if only one map exists,
 *     untagged segments (e.g. init.hex) still use that map
 *   - sanity-check addresses/data against each map's src mif_source ranges
 *
 * Per logical (addr, data), each matching <map> rule remaps:
 *   des_addr = addr + des_addr_offset
 *   extract src_mif_bits from data, place into des_mif_bits (OR into des_addr)
 *
 * Output .mem header addr/data metadata is derived from all <map> rules for
 * the des_pb_type (so unused sibling halves still size the physical word):
 *   address[lsb:msb] = union of (src_addr_range + des_addr_offset)
 *   data width       = max(des_mif_bits.msb) + 1
 * src_mif_bits / des_mif_bits widths must match.
 *
 * logical_storage: from read_mif (eblif / init.hex)
 * out_aggregated_storage: aggregated preload result for write_mif
 */
int aggregate_mif(const MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage);

} /* namespace openfpga */
