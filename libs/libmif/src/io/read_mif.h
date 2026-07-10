#pragma once

#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Read an Verilog init.hex memory initialization file into storage. */
int read_mif(const std::string& file_path, MifStorage& mif_storage);

} /* namespace openfpga */
