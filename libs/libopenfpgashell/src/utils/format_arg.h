#pragma once

#include <string>
#include <vector>

namespace openfpga {  // begin namespace openfpga

/** @brief Initialize the options from command-line inputs and organize in the
 * format that is ready for parsing */
std::vector<std::string> format_argv(const std::string& cmd_name, int argc,
                                     const char** argv);

}  // end namespace openfpga
