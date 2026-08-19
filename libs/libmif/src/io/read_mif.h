#pragma once

#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Read Verilog-style init.hex into one logical segment (shell read_mif). */
int read_mif_from_init_hex(const std::string& file_path,
                           MifStorage& mif_storage, const std::string& pb_type);

} /* namespace openfpga */
