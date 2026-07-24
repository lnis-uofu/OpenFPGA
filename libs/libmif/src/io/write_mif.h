#pragma once

#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Write aggregated preload MIF (from aggregate_mif) to a .mem file. */
int write_mif(const std::string& file_path,
              const MifStorage& aggregated_mif_storage);

} /* namespace openfpga */
