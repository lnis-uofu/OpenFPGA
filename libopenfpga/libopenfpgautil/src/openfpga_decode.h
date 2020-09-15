#ifndef OPENFPGA_DECODE_H
#define OPENFPGA_DECODE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <stddef.h>
#include <vector>

/********************************************************************
 * Function declaration
 *******************************************************************/
/* namespace openfpga begins */
namespace openfpga {

std::vector<size_t> ito1hot_vec(const size_t& in_int,
                                const size_t& bin_len);

std::vector<size_t> itobin_vec(const size_t& in_int,
                               const size_t& bin_len);

std::vector<char> itobin_charvec(const size_t& in_int,
                                 const size_t& bin_len);

size_t bintoi_charvec(const std::vector<char>& bin);

} /* namespace openfpga ends */

#endif
