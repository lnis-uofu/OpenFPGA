#ifndef OPENFPGA_DECODE_H
#define OPENFPGA_DECODE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <stddef.h>
#include <stdint.h>

#include <string>
#include <vector>

/********************************************************************
 * Function declaration
 *******************************************************************/
/* namespace openfpga begins */
namespace openfpga {

/****************************************
 * Constants
 */
constexpr const char DONT_CARE_CHAR = 'x';

std::vector<size_t> ito1hot_vec(const size_t& in_int, const size_t& bin_len);

std::vector<char> ito1hot_charvec(const size_t& in_int, const size_t& bin_len,
                                  const char& default_bit = '0');

/** @brief Replace the characters in a string with a given replacement
 *  @param str_to_convert the input and output string
 *  @param bit_in_place the charater to be replaced
 *  @param bit_to_replace the charater to replace the bit_in_place
 */
void replace_str_bits(std::string& str_to_convert, const char& bit_in_place,
                      const char& bit_to_replace);

/********************************************************************
 * @brief Combine to two 1-hot codes which are in string format
 * Any unique '1' or '0' will be merged
 * @note Where there are both '0' or '1' in the code1 and code2,
 *       code2 bits will overwrite
 * For example:
 *   Code 1: xx1x0x110
 *   Code 2: 01xx01xx1
 *   Output: 011x01111
 * @note This function requires two codes in the same length
 ********************************************************************/
std::string combine_two_1hot_str(const std::string& code1,
                                 const std::string& code2);

std::vector<size_t> itobin_vec(const size_t& in_int, const size_t& bin_len);

std::vector<char> itobin_charvec(const size_t& in_int, const size_t& bin_len);

size_t bintoi_charvec(const std::vector<char>& bin);

std::vector<std::string> expand_dont_care_bin_str(const std::string& input_str);

/* Number of hex digits needed to cover width_bits (ceil(width/4)). */
int hex_digits_for_width(const int& width_bits);

/********************************************************************
 * Convert a hex digit string (optional 0x prefix) into a bit string with
 * LSB at index 0. width_bits selects how many bits to produce (zero-extend
 * or truncate high bits that must be 0 when truncating).
 * Returns empty string on invalid input.
 ********************************************************************/
std::string hex_to_bit_string(const std::string& hex_digits,
                              const size_t& width_bits);

/********************************************************************
 * Format a bit string (LSB at index 0) as zero-padded uppercase hex
 * (Verilog $readmemh style, no 0x prefix). width_bits selects digit count;
 * if <= 0, bits.size() is used.
 ********************************************************************/
std::string format_hex_word(const std::string& bits_lsb0,
                            const int& width_bits);

/********************************************************************
 * Resize an LSB-at-index-0 bit string to target_width (zero-extend, or
 * truncate high bits that must be 0). Requires only '0'/'1'.
 ********************************************************************/
bool normalize_bit_string_width(std::string& bits, size_t target_width);

}  // namespace openfpga

#endif
