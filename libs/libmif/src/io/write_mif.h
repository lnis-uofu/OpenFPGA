#pragma once

#include <set>
#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

constexpr const char* K_PRELOAD_MEM_TITLE =
  "Aggregated MIF for memory preloading interface";

/* Write FPGA-top unified MIF (MifPipeline::top_mif_) to a .mem file.
 * allowed_ports empty: write every segment. Otherwise only segments whose
 * physical_pb (top-level port name) is in the set.
 * include_time_stamp writes Version/Date comments; disable via --no_time_stamp.
 */
int write_mif(const std::string& file_path,
              const MifStorage& aggregated_mif_storage,
              const std::set<std::string>& allowed_ports = {},
              const bool& include_time_stamp = true,
              const bool& verbose = false);

} /* namespace openfpga */
