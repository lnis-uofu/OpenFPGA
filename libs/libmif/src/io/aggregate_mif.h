#pragma once

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Complete and aggregate logical MIF storage.
 *
 * Algorithm:
 *   1. clear the aggregated output and validate address-map availability
 *   2. bind each logical segment's VPR pb_type to its mif_source
 *   3. persist source address range/data width in logical_storage
 *   4. decode raw eblif INIT into logical memory lines and clear the raw data
 *   5. remap logical memory lines through <map> rules, detecting conflicts
 *   6. emit one aggregated segment for each destination pb_type
 *
 * Per logical (addr, data), each matching <map> rule:
 *   des_addr = addr + des_addr_offset
 *   extract src_mif_bits, place into des_mif_bits (OR into des_addr)
 *
 * Source and destination dimensions come from their mif_source definitions.
 * Map rules only determine address translation and data-bit placement.
 * logical_storage is updated in place with decoded source metadata and words.
 */
int aggregate_mif(MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage);

} /* namespace openfpga */
