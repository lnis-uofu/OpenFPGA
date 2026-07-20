#pragma once

#include <map>
#include <string>

#include "command_exit_codes.h"
#include "mif_address_map.h"
#include "mif_storage.h"

namespace openfpga {

/* Aggregate logical MIF segments into per-physical-pb wide memories
 * (Verilog $readmemh-compatible) using mif_address_map offsets.
 *
 * - Each input segment is associated to a Verilog instance via:
 *     instance_name = find_verilog_instance_reading_mif(verilog_path,
 *                                 mif_file_basename(segment_source_file))
 * - mif_address_map is looked up by pb_type_path of that instance.
 * - Operating pb paths sharing the same parent physical pb are merged
 *   into one output segment tagged with // PB_TYPE <physical_pb>.
 * - For each (logical_addr, logical_data):
 *     phys_addr = logical_addr << address_offset
 *     phys_data = logical_data << data_offset
 *   and phys_data words are OR-combined for identical phys_addr.
 */
int aggregate_mif(const MifStorage& logical_storage,
                   const std::string& verilog_path,
                   const std::map<std::string, std::string>&
                     instance_pb_type_path_map,
                   const MifAddressMap& mif_address_map,
                   MifStorage& out_aggregated_storage);

} /* namespace openfpga */

