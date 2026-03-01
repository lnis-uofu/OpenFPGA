#include "format_arg.h "

namespace openfpga { // begin namespace openfpga

/** @brief Initialize the options from command-line inputs and organize in the
 * format that is ready for parsing */
std::vector<std::string> format_argv(const std::string& cmd_name,
                                     int argc, const char** argv) {
  std::vector<std::string> cmd_opts;
  cmd_opts.push_back(cmd_name);
  for (int iarg = 1; iarg < argc; ++iarg) {
    cmd_opts.push_back(std::string(argv[iarg]));
  }
  return cmd_opts;
}

} // end namespace openfpga
