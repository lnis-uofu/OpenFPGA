#ifndef OPENFPGA_DECODE_H
#define OPENFPGA_DECODE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <stddef.h>
#include <vector>
#include <string>

/********************************************************************
 * Function declaration
 *******************************************************************/
/* namespace openfpga begins */
namespace openfpga {

/**************************************** 
 * Constants 
 */
constexpr char DONT_CARE_CHAR = 'x';

std::vector<size_t> ito1hot_vec(const size_t& in_int,
                                const size_t& bin_len);

std::vector<char> ito1hot_charvec(const size_t& in_int,
                                  const size_t& bin_len);

/******************************************************************** 
 * @brief Combine to two 1-hot codes which are in string format
 * Any unique '1' will be merged
 * For example: 
 *   Code 1: 001000110
 *   Code 2: 010001001
 *   Output: 011001111
 * @note This function requires two codes in the same length
 ********************************************************************/
std::string combine_two_1hot_str(const std::string& code1,
                                 const std::string& code2);

std::vector<size_t> itobin_vec(const size_t& in_int,
                               const size_t& bin_len);

std::vector<char> itobin_charvec(const size_t& in_int,
                                 const size_t& bin_len);

size_t bintoi_charvec(const std::vector<char>& bin);

std::vector<std::string> expand_dont_care_bin_str(const std::string& input_str);

} /* namespace openfpga ends */

#endif
