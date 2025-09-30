#pragma once

/********************************************************************
 * Include header files that are required by data structure declaration
 *******************************************************************/
#include <string>
#include <vector>

/************************************************************************
 * This file includes parsers for bit data in the architecture XML
 * language. Note that it is also compatiable to Verilog syntax.
 * It means we may reuse this for constructing a structural Verilog parser
 ***********************************************************************/

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Class BitParser:
 * Supported data format:
 * - Binary format:
 *     Native: 11000111
 *     With splitter: 1100_0111
 *     Header in big endian: 8B'11000111
 *     Header in big endian with splitter : 8B'1100_0111
 *     Header in little endian with splitter:  8b'11100011
 *     Header in little endian:  8b'1110_0011
 *   where
 *    - 8 represents the number of bits
 *    - B represent the big endian
 *    - b represents the little endian
 * - Hexadecimal format: 8H'A7 or 8h'E3
 *   where
 *    - 8 represents the number of bits
 *    - H represent the big endian
 *    - h represents the little endian
 ***********************************************************************/
class BitsParser {
 public: /* Constructors*/
  BitsParser(const std::string& data, const bool& accept_dont_care_bits);

 public: /* Public Accessors */
  std::string data() const;
  bool valid() const;
  std::vector<char> result() const;

 public: /* Public Mutators */
  void set_data(const std::string& data);

 private: /* Private Mutators */
  void parse();
  void parse_bin_format(const std::string& bits_str, const bool& little_endian,
                        const int& expected_len);
  void parse_hex_format(const std::string& bits_str, const bool& little_endian,
                        const int& expected_len);

 private:            /* Internal data */
  std::string data_; /* Lines to be splited */
  char delim_;
  char splitter_;
  char bin_format_be_code_;
  char hex_format_be_code_;
  char bin_format_le_code_;
  char hex_format_le_code_;
  std::vector<char> result_;
  bool valid_;
  bool accept_dont_care_bits_;
};

}  // namespace openfpga
