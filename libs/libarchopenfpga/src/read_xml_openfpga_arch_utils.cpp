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
#include "openfpga_bit_parser.h"

/********************************************************************
 * Parse mode_bits: convert from string to array of digits
 * We only allow the bit to either '0' or '1'
 *******************************************************************/
static std::vector<size_t> parse_mode_bits_bin_format(pugi::xml_node& xml_mode_bits,
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

/********************************************************************
 * Parse mode_bits: convert from string to array of digits
 * We only allow the bit to either '0' or '1'
 *******************************************************************/
std::vector<size_t> parse_mode_bits(pugi::xml_node& xml_mode_bits,
                                    const pugiutil::loc_data& loc_data,
                                    const std::string& mode_bit_str) {
  openfpga::BitsParser bit_parser(mode_bit_str);
  if (!bit_parser.status())
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_mode_bits),
                   "Invalid format of mode bit '%s'! Expect to be binary or HEX format. For example, 1100 or 4b'1100 or 4h'A\n",
                   mode_bit_str.c_str());
  }
  return bit_parser.data();
}

  /* Check the head, if starts with an apostrophy, consider the format. Otherwise, following binary format as default */
  std::string format_delim = "'";
  openfpga::StringToken str_tok(mode_bit_str);
  std::vector<std::string> tokens = str_tok.split(format_delim); 
  if (tokens.size() == 1) {
    /* Binary format */
    return parse_mode_bits_bin_format(xml_mode_bits, loc_data, mode_bit_str);
  } else if (tokens.size() > 2) {
    /* Invalid format, error out ! */
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_mode_bits),
                   "Invalid format of mode bit '%s'! Expect to be binary or HEX format. For example, 1100 or 4b'1100 or 4h'A\n",
                   mode_bit_str.c_str());
  } else {
    VTR_ASSERT_SAFE(tokens.size() == 2);
    /* Parse the format */
    std::string bin_format = "b";
    std::string bin_format = "b";
    std::string fmt_type = tokens[0].back();
    if (fmt_type == std::string("b")) 
  }
  
  return mode_bits;
}
