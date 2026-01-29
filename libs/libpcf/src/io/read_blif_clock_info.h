#pragma once

#include <string>
#include <vector>

std::vector<std::string> read_blif_clock_info(const char* arch_fname,
                                              const char* blif_fname,
                                              const std::string& blif_ffmt);
