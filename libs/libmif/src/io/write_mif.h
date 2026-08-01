#pragma once

#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Write all segments in storage to a MIF file (overwrite). */
int write_mif(const std::string& file_path, const MifStorage& mif_storage);

} /* namespace openfpga */
