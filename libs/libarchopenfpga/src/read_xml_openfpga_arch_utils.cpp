/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML modeling OpenFPGA architecture to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "openfpga_bits_parser.h"
#include "read_xml_openfpga_arch_utils.h"

/********************************************************************
 * Parse mode_bits: convert from string to array of digits
 * We only allow the bit to either '0' or '1'
 *******************************************************************/
std::vector<char> parse_mode_bits(pugi::xml_node& xml_mode_bits,
                                  const pugiutil::loc_data& loc_data,
                                  const std::string& mode_bit_str,
                                  const bool& accept_dont_care_bits) {
  /* Return if the input is empty */
  openfpga::BitsParser bits_parser(mode_bit_str, accept_dont_care_bits);
  if (!bits_parser.valid()) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_mode_bits),
                   "Invalid format of mode bit '%s'!\n", mode_bit_str.c_str());
  }
  return bits_parser.result();
}
