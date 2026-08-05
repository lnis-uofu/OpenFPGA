#pragma once

#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

constexpr const char* K_PRELOAD_MEM_TITLE =
  "Aggregated MIF for memory preloading interface";

/* Write aggregated preload MIF (from MifPipeline PHYSICAL stage) to a .mem
 * file. */
int write_mif(const std::string& file_path,
              const MifStorage& aggregated_mif_storage);

} /* namespace openfpga */
