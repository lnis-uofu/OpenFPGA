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

/* Bind source metadata, decode raw INIT into logical storage, then aggregate
 * logical MIF segments per physical pb.
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
 * Destination address range and data width come directly from its mif_source.
 * Map rules only determine where each logical address/data slice is placed.
 */
int aggregate_mif(MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage);

} /* namespace openfpga */
