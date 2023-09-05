/***************************************************************************************
 * This file includes functions that are used to decode integer to binary
 *vectors or the reverse operation
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "openfpga_decode.h"

#include "vtr_assert.h"

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
std::vector<size_t> ito1hot_vec(const size_t& in_int, const size_t& bin_len) {
  /* Make sure we do not have any overflow! */
  VTR_ASSERT((in_int <= bin_len));

  /* Initialize */
  std::vector<size_t> ret(bin_len, 0);

  if (bin_len == in_int) {
    return ret; /* all zero case */
  }
  ret[in_int] = 1; /* Keep a good sequence of bits */

  return ret;
}

/********************************************************************
 * Convert an integer to an one-hot encoding character array
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
std::vector<char> ito1hot_charvec(const size_t& in_int, const size_t& bin_len,
                                  const char& default_bit) {
  /* Make sure we do not have any overflow! */
  VTR_ASSERT((in_int <= bin_len));

  /* Initialize */
  std::vector<char> ret(bin_len, default_bit);

  if (bin_len == in_int) {
    return ret; /* all zero case */
  }
  ret[in_int] = '1'; /* Keep a good sequence of bits */

  return ret;
}

void replace_str_bits(std::string& str_to_convert, const char& bit_in_place,
                      const char& bit_to_replace) {
  for (char& bit : str_to_convert) {
    if (bit_in_place == bit) {
      bit = bit_to_replace;
    }
  }
}

std::string combine_two_1hot_str(const std::string& code1,
                                 const std::string& code2) {
  VTR_ASSERT(code1.length() == code2.length());
  std::string ret = code1;
  for (size_t ichar = 0; ichar < code2.length(); ichar++) {
    VTR_ASSERT('0' == code2[ichar] || '1' == code2[ichar] ||
               'x' == code2[ichar]);
    if ('1' == code2[ichar] || '0' == code2[ichar]) {
      ret[ichar] = code2[ichar];
    }
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
 *  ToDo: Need to revisit and change all the feature that call to this function
 *            Had studied the code, should be safe to make the change
 *            Apparently we only want to store 0 or 1 (binary vector)
 *            Yet we store it in a vector on size_t (8 Bytes)
 *            We are using 8x memory that we supposed to
 *            uint8_t is good enough
 *            Not a bug, but is a serious unoptimized issue
 ********************************************************************/
std::vector<size_t> itobin_vec(const size_t& in_int, const size_t& bin_len) {
  /* bin_len must be in valid range*/
  VTR_ASSERT(bin_len > 0 && bin_len <= 64);
  std::vector<size_t> ret(bin_len, 0);

  /* Make sure we do not have any overflow! */
  /* If the length is 64 bits, in_int can be any number since this is the max
     bit length, and BTW pow(2, 64) itself should be zero (from 64bits size_t
     perspective). Once we fix the bintoi_charvec() bug, without this change, it
     will cause assertion */
  if (bin_len < 64) {
    VTR_ASSERT(in_int < (size_t(1) << bin_len));
  }

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
std::vector<char> itobin_charvec(const size_t& in_int, const size_t& bin_len) {
  /* bin_len must be in valid range*/
  VTR_ASSERT(bin_len > 0 && bin_len <= 64);
  std::vector<char> ret(bin_len, '0');

  /* Make sure we do not have any overflow! */
  /* If the length is 64 bits, in_int can be any number since this is the max
     bit length, and BTW pow(2, 64) itself should be zero (from 64bits size_t
     perspective). Once we fix the bintoi_charvec() bug, without this change, it
     will cause assertion */
  if (bin_len < 64) {
    VTR_ASSERT(in_int < (size_t(1) << bin_len));
  }

  size_t temp = in_int;
  for (size_t i = 0; i < bin_len; i++) {
    if (1 == temp % 2) {
      ret[i] = '1'; /* Keep a good sequence of bits */
    }
    temp = temp / 2;
  }

  return ret;
}

/********************************************************************
 * Converter a binary vector to an integer
 * For example:
 *   Binary length : 3
 *   Input:
 *     index | 0 | 1 | 2
 *     ret   | 0 | 0 | 1
 *
 *   Output integer: 4
 *
 * This function is optimized to return a vector of char
 * which has a smaller memory footprint than size_t
 ********************************************************************/
size_t bintoi_charvec(const std::vector<char>& bin) {
  /* bin.size() must be in valid range*/
  VTR_ASSERT(bin.size() > 0 && bin.size() <= 64);

  size_t ret = 0;

  for (size_t i = 0; i < bin.size(); ++i) {
    if ('1' == bin[i]) {
      ret |= ((size_t)(1) << i);
    }
  }

  return ret;
}

/********************************************************************
 * Expand all the don't care bits in a string
 * A don't care 'x' can be decoded to either '0' or '1'
 * For example:
 *    input:  0x1x
 *    output: 0010
 *            0100
 *            0101
 *            0011
 *
 * Return all the strings
 ********************************************************************/
std::vector<std::string> expand_dont_care_bin_str(
  const std::string& input_str) {
  std::vector<std::string> ret;

  /* If the input is don't care free, we can retrun */
  bool has_dont_care = false;
  for (const char& bit : input_str) {
    if (DONT_CARE_CHAR == bit) {
      has_dont_care = true;
      break;
    }
  }

  if (false == has_dont_care) {
    ret.push_back(input_str);
    return ret;
  }

  /* Recusively expand all the don't bits */
  for (size_t i = 0; i < input_str.length(); ++i) {
    if (DONT_CARE_CHAR == input_str[i]) {
      std::string temp_input_str = input_str;
      /* Flip to '0' and go recursively */
      temp_input_str[i] = '0';
      for (const std::string& expanded_str :
           expand_dont_care_bin_str(temp_input_str)) {
        ret.push_back(expanded_str);
      }
      /* Flip to '1' and go recursively */
      temp_input_str[i] = '1';
      for (const std::string& expanded_str :
           expand_dont_care_bin_str(temp_input_str)) {
        ret.push_back(expanded_str);
      }
      break;
    }
  }

  return ret;
}

} /* end namespace openfpga */
