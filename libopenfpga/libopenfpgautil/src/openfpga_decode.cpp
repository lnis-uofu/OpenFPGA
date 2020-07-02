/***************************************************************************************
 * This file includes functions that are used to decode integer to binary vectors
 * or the reverse operation
 ***************************************************************************************/
#include <cmath>

/* Headers from vtrutil library */
#include "vtr_assert.h"

#include "openfpga_decode.h"

/* begin namespace openfpga */
namespace openfpga {

/******************************************************************** 
 * Convert an integer to an one-hot encoding integer array
 * For example: 
 *   Input integer: 3
 *   Binary length : 4
 *   Output:
 *     index | 0 | 1 | 2 | 3
 *     ret   | 0 | 0 | 0 | 1
 *
 * If you need all zero code, set the input integer same as the binary length
 * For example: 
 *   Input integer: 4
 *   Binary length : 4
 *   Output:
 *     index | 0 | 1 | 2 | 3
 *     ret   | 0 | 0 | 0 | 0
 *           
 ********************************************************************/
std::vector<size_t> ito1hot_vec(const size_t& in_int,
                                const size_t& bin_len) {
  /* Make sure we do not have any overflow! */
  VTR_ASSERT ( (in_int <= bin_len) );

  /* Initialize */
  std::vector<size_t> ret(bin_len, 0);

  if (bin_len == in_int) {
    return ret; /* all zero case */
  }
  ret[in_int] = 1; /* Keep a good sequence of bits */
 
  return ret;
}

/******************************************************************** 
 * Converter an integer to a binary vector 
 * For example: 
 *   Input integer: 4
 *   Binary length : 3
 *   Output:
 *     index | 0 | 1 | 2
 *     ret   | 0 | 0 | 1 
 ********************************************************************/
std::vector<size_t> itobin_vec(const size_t& in_int,
                               const size_t& bin_len) {
  std::vector<size_t> ret(bin_len, 0);

  /* Make sure we do not have any overflow! */
  VTR_ASSERT ( (in_int < pow(2., bin_len)) );
  
  size_t temp = in_int;
  for (size_t i = 0; i < bin_len; i++) {
    if (1 == temp % 2) { 
      ret[i] = 1; /* Keep a good sequence of bits */
    }
    temp = temp / 2;
  }
 
  return ret;
}

/******************************************************************** 
 * Converter an integer to a binary vector 
 * For example: 
 *   Input integer: 4
 *   Binary length : 3
 *   Output:
 *     index | 0 | 1 | 2
 *     ret   | 0 | 0 | 1 
 *
 * This function is optimized to return a vector of char
 * which has a smaller memory footprint than size_t
 ********************************************************************/
std::vector<char> itobin_charvec(const size_t& in_int,
                                 const size_t& bin_len) {
  std::vector<char> ret(bin_len, '0');

  /* Make sure we do not have any overflow! */
  VTR_ASSERT ( (in_int < pow(2., bin_len)) );
  
  size_t temp = in_int;
  for (size_t i = 0; i < bin_len; i++) {
    if (1 == temp % 2) { 
      ret[i] = '1'; /* Keep a good sequence of bits */
    }
    temp = temp / 2;
  }
 
  return ret;
}

} /* end namespace openfpga */
