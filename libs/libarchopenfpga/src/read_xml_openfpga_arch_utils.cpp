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
#include "read_xml_openfpga_arch_utils.h"

/********************************************************************
 * Parse mode_bits: convert from string to array of digits
 * We only allow the bit to either '0' or '1'
 *******************************************************************/
std::vector<size_t> parse_mode_bits(pugi::xml_node& xml_mode_bits,
                                    const pugiutil::loc_data& loc_data,
                                    const std::string& mode_bit_str) {
  std::vector<size_t> mode_bits;

  for (const char& bit_char : mode_bit_str) {
    if ('0' == bit_char) {
      mode_bits.push_back(0);
    } else if ('1' == bit_char) {
      mode_bits.push_back(1);
    } else {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_mode_bits),
                     "Unexpected '%c' character found in the mode bit '%s'! "
                     "Only allow either '0' or '1'\n",
                     bit_char, mode_bit_str.c_str());
    }
  }

  return mode_bits;
}
