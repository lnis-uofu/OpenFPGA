/************************************************************************
 * Member functions for Pb parsers
 ***********************************************************************/
#include <cstring>

#include "vtr_assert.h"

#include "openfpga_tokenizer.h"

#include "openfpga_pb_parser.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Member functions for PbParser class 
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
PbParser::PbParser (const std::string& data) {
  set_default_bracket();
  set_default_delim();
  set_data(data);
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
/* Get the data string */
std::string PbParser::data() const {
  return data_;
}

std::string PbParser::leaf() const {
  return leaf_;
}

std::vector<std::string> PbParser::parents() const {
  return parents_;
}

std::vector<std::string> PbParser::modes() const {
  return modes_;
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void PbParser::set_data(const std::string& data) {
  data_ = data;
  parse();
}

/************************************************************************
 * Internal Mutators
 ***********************************************************************/
/* Parse the data */
void PbParser::parse() {
  /* Create a tokenizer */
  StringToken tokenizer(data_);

  /* Split the data into <pb_type>[<mode>] */
  std::vector<std::string> pb_tokens = tokenizer.split(delim_);

  /* The last pb is the leaf node.
   * It should NOT be empty and should NOT contain any brackets!  
   */
  VTR_ASSERT(0 < pb_tokens.size());
  VTR_ASSERT(false == pb_tokens.back().empty());
  VTR_ASSERT(pb_tokens.back().find(bracket_.x()) == std::string::npos);
  VTR_ASSERT(pb_tokens.back().find(bracket_.y()) == std::string::npos);
  /* Pass the check, we assign the value */
  leaf_ = pb_tokens.back();

  /* Split the rest of pb_tokens, split the pb_type and mode_name */
  for (size_t itok = 0; itok < pb_tokens.size() - 1; ++itok) {
    /* Create a tokenizer */
    StringToken pb_tokenizer(pb_tokens[itok]);
    std::vector<std::string> tokens = pb_tokenizer.split(bracket_.x());
    /* Make sure we have a port name! */
    VTR_ASSERT_SAFE ((1 == tokens.size()) || (2 == tokens.size()));
    /* Store the pb_type name */
    parents_.push_back(tokens[0]);

    /* If we only have one token, the mode name is the default mode
     * Here we use the default keyword 'default'
     * from VPR function ProcessMode() in libarchfpga
     */
    if (1 == tokens.size()) {
      modes_.push_back(std::string("default"));
      continue; /* We can finish here */
    }
   
    /* If there is a mode name, extract it */
    VTR_ASSERT_SAFE (2 == tokens.size());

    /* Chomp the ']' */
    pb_tokenizer.set_data(tokens[1]);
    std::vector<std::string> mode_tokens = pb_tokenizer.split(bracket_.y());

    /* Make sure we have only one mode name inside */
    VTR_ASSERT(1 == mode_tokens.size());
    modes_.push_back(mode_tokens[0]);
  }
}

void PbParser::set_default_bracket() {
  bracket_.set_x('[');
  bracket_.set_y(']');
}

void PbParser::set_default_delim() {
  delim_ = '.';
}

} /* namespace openfpga ends */
