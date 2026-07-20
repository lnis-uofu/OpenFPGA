#pragma once

#include <map>
#include <string>

#include "aggregated_mif_storage.h"
#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

std::string default_preload_mem_file_path(const std::string& write_file_path);

/* Aggregate logical MIF into aggregated storage (logical storage unchanged). */
int aggregate_mif_storage(
  const MifStorage& mif_storage, const BitstreamSetting& bitstream_setting,
  const std::map<std::string, std::string>& instance_pb_type_path_map,
  const std::string& verilog_path,
  AggregatedMifStorage& aggregated_mif_storage);

/* Aggregate into aggregated storage and optionally write preload .mem. */
int aggregate_mif_storage_and_write_preload_mem(
  const MifStorage& mif_storage, const BitstreamSetting& bitstream_setting,
  const std::map<std::string, std::string>& instance_pb_type_path_map,
  const std::string& verilog_path, AggregatedMifStorage& aggregated_mif_storage,
  const std::string& mem_file_path);

} /* namespace openfpga */
