#ifndef OPENFPGA_PB_PARSER_H
#define OPENFPGA_PB_PARSER_H

/********************************************************************
 * Include header files that are required by data structure declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "openfpga_tokenizer.h"
#include "vtr_geometry.h"

/************************************************************************
 * This file includes parsers for pb_type definition in the architecture XML
 * language.
 ***********************************************************************/

/* namespace openfpga begins */
namespace openfpga {

/********************************************************************
 * Strip a trailing numeric leaf index used by instance paths:
 *   foo.bar[3] -> foo.bar
 * Non-numeric brackets (e.g. mode names) are left unchanged.
 *******************************************************************/
std::string strip_numeric_pb_index(const std::string& pb_type);

/************************************************************************
 * Class PbParser: pb_type name with full hierarchy parser
 * Supported pb_type definition:
 *   1. <pb_type>[<mode>].<sub_pb_type>[<mode>] ... .<leaf_pb_type>
 *   2. <pb_type>.<sub_pb_type> ... .<leaf_pb_type>
 * where each pb_type may be specified by a mode or use the default mode
 * (mode name not given)
 ***********************************************************************/
class PbParser {
 public: /* Constructors*/
  PbParser(const std::string& data);

 public: /* Public Accessors */
  std::string data() const;
  std::string leaf() const;
  std::vector<std::string> parents() const;
  std::vector<std::string> modes() const;

 public: /* Public Mutators */
  void set_data(const std::string& data);

 private: /* Private Mutators */
  void parse();
  void set_default_bracket();
  void set_default_delim();

 private:            /* Internal data */
  std::string data_; /* Lines to be splited */
  vtr::Point<char> bracket_;
  char delim_;
  std::vector<std::string> parents_;
  std::vector<std::string> modes_;
  std::string leaf_;
};

}  // namespace openfpga

#endif
