#pragma once

#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Read Yosys eblif .param content into logical_storage, then aggregate it. */
int aggregate_mif(const std::string& eblif_file_path,
                  MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage);

/* Aggregate already-loaded logical MIF segments per physical pb.
 *
 * Algorithm:
 *   1. clear output
 *   2. for each des_pb_type, remap bound sources via <map> rules (OR-append)
 *   3. emit one aggregated segment per des that received data
 *
 * Per logical (addr, data), each matching <map> rule:
 *   des_addr = addr + des_addr_offset
 *   extract src_mif_bits, place into des_mif_bits (OR into des_addr)
 *
 * Header for each des is inferred from all <map> rules of that des:
 *   address[lsb:msb] = union of (src_addr_range + des_addr_offset)
 *   data width       = max(des_mif_bits.msb) + 1
 */
int aggregate_mif(const MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage);

} /* namespace openfpga */
