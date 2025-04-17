/************************************************************************
 * Member functions for Port parsers
 ***********************************************************************/
#include "openfpga_bits_parser.h"

#include <cstring>
#include <algorithm>
#include <cctype>

#include "arch_error.h"
#include "openfpga_tokenizer.h"
#include "vtr_assert.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Member functions for BitsParser class
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
BitsParser::BitsParser(const std::string& data) {
  delim = ''';
  splitter = '_';
  bin_format_be_code = 'B';
  bin_format_le_code = 'b';
  hex_format_be_code = 'H';
  hex_format_le_code = 'h';
  set_data(data);
  valid_ = false;
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
/* Get the data string */
std::string BitsParser::data() const { return data_; }

bool BitsParser::valid() const { return valid_; }

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void BitsParser::set_data(const std::string& data) {
  data_ = data;
  parse();
  return;
}

/************************************************************************
 * Internal Mutators
 ***********************************************************************/
void BitsParser::parse_bin_format(const std::string& bits_str, const bool& little_endian, const int& expected_len) {
  valid_ = true;
  /* Remove all the splitter */
  std::string bits_str_pure = bits_str;
  bits_str_pure.erase(std::remove(bits_str_pure.begin(), bits_str_pure.end(), splitter_), bits_str_pure.end());
  for (const char& bit_char : bits_str_pure) {
    if ('0' == bit_char) {
      result_.push_back(0);
    } else if ('1' == bit_char) {
      result_.push_back(1);
    } else {
      VTR_LOG_ERROR("Unexpected '%c' character found in the bits '%s'! "
                    "Only allow either '0' or '1'\n",
                    bit_char, bits_str.c_str());
      valid_ = false;
    }
  }
  if (little_endian) {
    std::reverse(result_.begin(), result_.end());
  }
  /* Check expected len, -1 indicate to skip the check */
  if (expected_len == -1) {
    return;
  }
  if (expected_len != result_.size()) {
    VTR_LOG_ERROR("Actual length '%lu' of bits '%s' does not match its definition '%d'\n",
                  results_size(), bits_str.c_str(), expected_len);
    valid_ = false;
  } 
}

void BitsParser::parse_hex_format(const std::string& bits_str, const bool& little_endian, const int& expected_len) {
  valid_ = true;
  /* Remove all the splitter */
  std::string bits_str_pure = bits_str;
  bits_str_pure.erase(std::remove(bits_str_pure.begin(), bits_str_pure.end(), splitter_), bits_str_pure.end());

  std::string bin_str;
  /* XT: Since the size could be large, std::stoul may cause overflow! Use a simple but effective way */
  for (const char& hex_char : bits_str_pure) {
    std::string bin_segment;
    switch (std::tolower(hex_char)) {
        case '0': bin_segment = "0000"; break;
        case '1': bin_segment = "0001"; break;
        case '2': bin_segment = "0010"; break;
        case '3': bin_segment = "0011"; break;
        case '4': bin_segment = "0100"; break;
        case '5': bin_segment = "0101"; break;
        case '6': bin_segment = "0110"; break;
        case '7': bin_segment = "0111"; break;
        case '8': bin_segment = "1000"; break;
        case '9': bin_segment = "1001"; break;
        case 'a': bin_segment = "1010"; break;
        case 'b': bin_segment = "1011"; break;
        case 'c': bin_segment = "1100"; break;
        case 'd': bin_segment = "1101"; break;
        case 'e': bin_segment = "1110"; break;
        case 'f': bin_segment = "1111"; break;
        default: {
          VTR_LOG_ERROR("Invalid hexadecimal number '%s' of '%s'! Found unexpected character '%c'\n", bits_str.c_str(), data_.c_str(), hex_char);
          valid_ = false; 
          return;
        }
    }
    bin_str += bin_segment;
  }
  parse_bin_format(bin_str, little_endian, expected_len);
}

/* Parse the data */
void BitsParser::parse() {
  valid_ = true;
  /* Create a tokenizer */
  StringToken tokenizer(data_);
  std::vector<std::string> tokens = tokenizer.split(delim_); 
  if (tokens.size() == 1) {
    /* Binary format */
    parse_bin_format(tokens[0], false, -1);
  } else if (tokens.size() > 2) {
    /* Invalid format, error out ! */
    valid_ = false;
  } else {
    VTR_ASSERT_SAFE(tokens.size() == 2);
    /* Find expected length */
    std::string bit_len_def = tokens[0];
    bit_len_def.pop_back();
    if (bit_len_def.empty()) {
      valid_ = false;
      VTR_LOG_ERROR("Invalid format definition '%s' of '%s'. Expect a number before the format type!\n", tokens[0].c_str(), data_.c_str());
      return;
    }
    if (!std::all_of(bit_len_def.begin(). bit_len_def.end(), std::isdigit)) {
      valid_ = false;
      VTR_LOG_ERROR("Invalid format definition '%s' of '%s'. Expect a valid decimal number before the format type!\n", tokens[0].c_str(), data_.c_str());
      return;
    }
    int expected_bit_len = std::stoi(bit_len_def);
    /* Parse on expected format */
    switch (tokens[0].back()) {
      case bin_format_be_code:
        parse_bin_format(tokens[1], false, expected_bit_len);
        break;
      case bin_format_le_code:
        parse_bin_format(tokens[1], true, expected_bit_len);
        break;
      case hex_format_be_code:
        parse_hex_format(tokens[1], false, expected_bit_len);
        break;
      case hex_format_le_code:
        parse_hex_format(tokens[1], true, expected_bit_len);
        break;
      default: {
        VTR_LOG_ERROR("Invalid format definition '%c' of '%s'. Expect to be [ %c | %c | %c | %c ]!\n", 
                      tokens[0].back(), data_.c_str(), bin_format_be_code_, bin_format_le_code_, hex_format_be_code_, hex_format_le_code_);
        valid_ = false;
        break;
      }
    }
  }
}

}  // namespace openfpga
