#pragma once

#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

std::string default_preload_mem_file_path(const std::string& write_file_path);

/* Aggregate logical MIF into aggregated storage (logical storage unchanged). */
int aggregate_mif_storage(const MifStorage& mif_storage,
                          const BitstreamSetting& bitstream_setting,
                          MifStorage& aggregated_mif_storage);

/* Aggregate into aggregated storage and optionally write preload .mem. */
int aggregate_mif_storage_and_write_preload_mem(
  const MifStorage& mif_storage, const BitstreamSetting& bitstream_setting,
  MifStorage& aggregated_mif_storage, const std::string& mem_file_path);

} /* namespace openfpga */
