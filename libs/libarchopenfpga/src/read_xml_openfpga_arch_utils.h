#ifndef READ_XML_OPENFPGA_ARCH_UTILS_H
#define READ_XML_OPENFPGA_ARCH_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "bitstream_setting.h"
#include "openfpga_arch.h"
#include "simulation_setting.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
std::vector<char> parse_mode_bits(pugi::xml_node& xml_mode_bits,
                                  const pugiutil::loc_data& loc_data,
                                  const std::string& mode_bit_str,
                                  const bool& accept_dont_care_bits);

#endif
