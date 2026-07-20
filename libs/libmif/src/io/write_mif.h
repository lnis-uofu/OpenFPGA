#pragma once

#include <string>

#include "aggregated_mif_storage.h"
#include "command_exit_codes.h"

namespace openfpga {

int write_mif(const std::string& file_path,
              const AggregatedMifStorage& aggregated_mif_storage);

} /* namespace openfpga */
