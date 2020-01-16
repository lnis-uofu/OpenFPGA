#ifndef OPENFPGA_TOKENIZER_H
#define OPENFPGA_TOKENIZER_H

/********************************************************************
 * Include header files that are required by data structure declaration
 *******************************************************************/
#include <string>
#include <vector>

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * This file includes a tokenizer for string objects
 * It splits a string with given delima and return a vector of tokens
 * It can accept different delima in splitting strings
 ***********************************************************************/

class StringToken {
  public : /* Constructors*/
    StringToken (const std::string& data);
  public : /* Public Accessors */ 
    std::string data() const;
    std::vector<std::string> split(const std::string& delims) const;
    std::vector<std::string> split(const char& delim) const;
    std::vector<std::string> split(const char* delim) const;
    std::vector<std::string> split(const std::vector<char>& delim) const;
    std::vector<std::string> split();
  public : /* Public Mutators */ 
    void set_data(const std::string& data);
    void add_delim(const char& delim);
    void ltrim(const std::string& sensitive_word);
    void rtrim(const std::string& sensitive_word);
    void trim();
  private : /* Private Mutators */ 
    void add_default_delim();
  private: /* Internal data */
    std::string data_; /* Lines to be splited */
    std::vector<char> delims_;
};

} /* namespace openfpga ends */

#endif
