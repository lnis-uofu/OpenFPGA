#pragma once

#include <string>

namespace openfpga {

/********************************************************************
 * Find the Verilog instance (e.g. memory_0) that reads the given
 * init.hex file via $readmemh (parameter default or instance override).
 * Returns empty string if not found.
 *******************************************************************/
std::string find_verilog_instance_reading_mif(const std::string& verilog_path,
                                              const std::string& mif_file_name);

} /* namespace openfpga */
