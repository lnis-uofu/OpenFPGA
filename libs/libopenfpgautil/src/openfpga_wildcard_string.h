#ifndef OPENFPGA_WILDCARD_STRING_H
#define OPENFPGA_WILDCARD_STRING_H

/********************************************************************
 * Include header files that are required by data structure declaration
 *******************************************************************/
#include <string>
#include <vector>

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * This file includes a object that can apply wildcard characters 
 * By default it will replace any digital numbers with a '*' character 
 * Users can set the wildcard character on their needs
 *
 * Example:
 *   std::string orig_str;
 *   WildCardString wc_str(orig_str);
 *   std::string output = wc_str.data();
 *
 ***********************************************************************/

class WildCardString {
  public : /* Constructors*/
    WildCardString (const std::string& data);

  public : /* Public Accessors */ 
    std::string data() const;

  public : /* Public Mutators */ 
    /* Give a string to apply wildcards */
    void set_data(const std::string& data);

  private : /* Private Mutators */ 
    /* Use default wildcard character '*' */
    void set_default_wildcard_char();

    /* Use default sensitive words which are numbers */
    void set_default_sensitive_chars();

    /* Replace sensitive words with wildcard characters */
    void apply_wildcard_char();

    /* Remove redundant wildcard chars (which are next to each other) */
    void compress();

  private : /* Internal data */
    std::string data_; /* Lines to be splited */
    std::vector<char> sensitive_chars_;
    char wildcard_char_;
};

} /* namespace openfpga ends */

#endif
