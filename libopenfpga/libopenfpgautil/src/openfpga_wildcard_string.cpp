/************************************************************************
 * Member functions for WildCardString class 
 ***********************************************************************/
#include <cstring>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "openfpga_wildcard_string.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
WildCardString::WildCardString(const std::string& data) {
  set_data(data);
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
std::string WildCardString::data() const {
  return data_;
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void WildCardString::set_data(const std::string& data) {
  data_ = data;

  set_default_wildcard_char(); 
  set_default_sensitive_chars(); 
  apply_wildcard_char(); 
  compress();
}

/************************************************************************
 * Internal Mutators
 ***********************************************************************/
void WildCardString::set_default_wildcard_char() {
  wildcard_char_ = '*';
}

void WildCardString::set_default_sensitive_chars() {
  sensitive_chars_.clear();
  sensitive_chars_.reserve(10);
  sensitive_chars_.push_back('0');
  sensitive_chars_.push_back('1');
  sensitive_chars_.push_back('2');
  sensitive_chars_.push_back('3');
  sensitive_chars_.push_back('4');
  sensitive_chars_.push_back('5');
  sensitive_chars_.push_back('6');
  sensitive_chars_.push_back('7');
  sensitive_chars_.push_back('8');
  sensitive_chars_.push_back('9');
}

void WildCardString::apply_wildcard_char() {
  /* Step by step:
   * For each sensitive character,
   * replace all of its occurance in the string data_ with wildcard character
   */
  for (const char& char_to_replace : sensitive_chars_) {
    size_t cur_pos = 0;
    std::string::size_type found;
    while (std::string::npos != (found = data_.find_first_of(char_to_replace, cur_pos))) {
      data_.replace(found, 1, 1, wildcard_char_);
      cur_pos = found + 1;
    }
  }
}

void WildCardString::compress() {
  for (std::string::size_type i = 0; i < data_.size(); ++i) {
    /* Care only wildcard character */
    if (wildcard_char_ != data_[i]) {
      continue;
    }

    /* Finish if this is the end of string */
    if (data_.size() - 1 == i) {
      break;
    }

    /* Try to find the next element and see if the same as wild card 
     * Keep erase the next element until we have a non-wildcard character
     */
    while (data_[i] == data_[i + 1]) {
      /* Erase the next element */
      data_.erase(i + 1, 1);

      /* Finish if this is the end of string */
      if (data_.size() - 1 == i) {
        break;
      }
    }
  }
}

} /* namespace openfpga ends */
