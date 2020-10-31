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

std::vector<size_t> itobin_vec(const size_t& in_int,
                               const size_t& bin_len);

std::vector<char> itobin_charvec(const size_t& in_int,
                                 const size_t& bin_len);

size_t bintoi_charvec(const std::vector<char>& bin);

std::vector<std::string> expand_dont_care_bin_str(const std::string& input_str);

} /* namespace openfpga ends */

#endif
