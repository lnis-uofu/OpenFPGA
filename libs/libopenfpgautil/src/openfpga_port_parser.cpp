/************************************************************************
 * Member functions for Port parsers
 ***********************************************************************/
#include "openfpga_port_parser.h"

#include <cstring>

#include "arch_error.h"
#include "openfpga_tokenizer.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Member functions for PortParser class
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
PortParser::PortParser(const std::string& data, const int support_format) {
  set_default_bracket();
  set_default_delim();
  set_support_format(support_format);
  set_data(data);
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
/* Get the data string */
std::string PortParser::data() const { return data_; }

BasicPort PortParser::port() const { return port_; }

bool PortParser::valid() const { return valid_; }

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void PortParser::set_support_format(const int support_format) {
  VTR_ASSERT((support_format & PORT_PARSER_SUPPORT_ALL_FORMAT) != 0);
  VTR_ASSERT((support_format & ~PORT_PARSER_SUPPORT_ALL_FORMAT) == 0);
  support_format_ = support_format;
  return;
}

void PortParser::set_data(const std::string& data) {
  data_ = data;
  parse();
  return;
}

/************************************************************************
 * Internal Mutators
 ***********************************************************************/
/* Parse the data */
void PortParser::parse() {
  valid_ = true;

  /* Create a tokenizer */
  StringToken tokenizer(data_);

  /* Split the data into <port_name> and <pin_string> */
  std::vector<std::string> port_tokens = tokenizer.split(bracket_.x());
  /* Make sure we have a port name! */
  VTR_ASSERT_SAFE((1 == port_tokens.size()) || (2 == port_tokens.size()));

  /* Store the port name! */
  port_.set_name(port_tokens[0]);

  /* If we only have one token */
  if (1 == port_tokens.size()) {
    // there is no [
    valid_ = (support_format_ & PORT_PARSER_SUPPORT_NO_PORT_FORMAT) != 0;
    port_.set_width(1);
    return; /* We can finish here */
  }

  /* Chomp the ']' */
  tokenizer.set_data(port_tokens[1]);
  std::vector<std::string> pin_tokens = tokenizer.split(bracket_.y());
  /* Make sure we have a valid string! */
  VTR_ASSERT_SAFE(1 == port_tokens.size());

  /* Split the pin string now */
  tokenizer.set_data(pin_tokens[0]);
  pin_tokens = tokenizer.split(delim_);
  VTR_ASSERT_SAFE((1 == pin_tokens.size()) || (2 == pin_tokens.size()));

  /* Check if we have LSB and MSB or just one */
  if (1 == pin_tokens.size()) {
    /* Single pin */
    valid_ = (support_format_ & PORT_PARSER_SUPPORT_SINGLE_INDEX_FORMAT) != 0;
    size_t temp = string_to_number(pin_tokens[0]);
    port_.set_width(temp, temp);
  } else if (2 == pin_tokens.size()) {
    /* A number of pins.
     * Note that we always use the LSB for token[0] and MSB for token[1]
     */
    valid_ = (support_format_ & PORT_PARSER_SUPPORT_RANGE_FORMAT) != 0;
    size_t temp0 = string_to_number(pin_tokens[0]);
    size_t temp1 = string_to_number(pin_tokens[1]);
    if (temp1 < temp0) {
      port_.set_width(temp1, temp0);
    } else {
      port_.set_width(temp0, temp1);
    }
  }

  return;
}

void PortParser::set_default_bracket() {
  bracket_.set_x('[');
  bracket_.set_y(']');
  return;
}

void PortParser::set_default_delim() {
  delim_ = ':';
  return;
}

/*
  Make sure string is not empty and is all digit before stoi
*/
size_t PortParser::string_to_number(const std::string& str) {
  bool bad_format = str.empty();
  for (auto& chr : str) {
    if (!std::isdigit(chr)) {
      bad_format = true;
      break;
    }
  }
  if (bad_format) {
    archfpga_throw(__FILE__, __LINE__,
                   "Invalid string '%s' to call std::stoi()", str.c_str());
  }
  return (size_t)(std::stoi(str));
}

/************************************************************************
 * Member functions for MultiPortParser class
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
MultiPortParser::MultiPortParser(const std::string& data) {
  set_default_delim();
  set_data(data);
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
/* Get the data string */
std::string MultiPortParser::data() const { return data_; }

std::vector<BasicPort> MultiPortParser::ports() const { return ports_; }

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void MultiPortParser::set_data(const std::string& data) {
  data_ = data;
  parse();
  return;
}

/************************************************************************
 * Internal Mutators
 ***********************************************************************/
/* Split the data line into fragments and parse one by one */
void MultiPortParser::parse() {
  /* Clear content */
  clear();

  /* Create a tokenizer */
  StringToken tokenizer(data_);

  /* Split the data into <port_name> and <pin_string> */
  std::vector<std::string> port_tokens = tokenizer.split(delim_);

  /* Use PortParser for each token */
  for (const auto& port : port_tokens) {
    PortParser port_parser(port);
    /* Get the port name, LSB and MSB */
    ports_.push_back(port_parser.port());
  }

  return;
}

void MultiPortParser::set_default_delim() {
  delim_ = ' ';
  return;
}

void MultiPortParser::clear() {
  ports_.clear();
  return;
}

/************************************************************************
 * Member functions for PortDelayParser class
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
PortDelayParser::PortDelayParser(const std::string& data) {
  set_default_element_delim();
  set_default_line_delim();
  set_data(data);
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
/* Get the data string */
std::string PortDelayParser::data() const { return data_; }

/* Get the size of delay matrix [height, width]*/
size_t PortDelayParser::height() const { return delay_matrix_.dim_size(0); }

size_t PortDelayParser::width() const { return delay_matrix_.dim_size(1); }

vtr::Point<size_t> PortDelayParser::delay_size() const {
  vtr::Point<size_t> matrix_size(height(), width());
  return matrix_size;
}

float PortDelayParser::delay(size_t x, size_t y) const {
  /* Make sure x and y are in range */
  VTR_ASSERT_SAFE((x < width()) && (y < height()));
  return delay_matrix_[x][y];
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void PortDelayParser::set_data(const std::string& data) {
  data_ = data;
  parse();
  return;
}

/************************************************************************
 * Internal Mutators
 ***********************************************************************/
/* Split the data line into fragments and parse one by one */
void PortDelayParser::parse() {
  /* Clear content */
  clear();

  /* Create a tokenizer */
  StringToken delay_tokenizer(data_);
  /* Ensure a clean start! Trim whitespace at the beginning and end of the
   * string */
  delay_tokenizer.trim();

  /* Split the data into different lines */
  std::vector<std::string> delay_lines = delay_tokenizer.split(line_delim_);

  /* The number of lines is actually the height of delay matrix */
  size_t matrix_height = delay_lines.size();
  size_t matrix_width = 0;

  /* Visit each line and split with element_delim */
  for (const auto& line : delay_lines) {
    /* Create a tokenizer for each line  */
    StringToken line_tokenizer(line);
    std::vector<std::string> delay_elements =
      line_tokenizer.split(element_delim_);
    /* Get maximum number of length, which is the width of delay matrix  */
    matrix_width = std::max(matrix_width, delay_elements.size());
  }

  /* Resize matrix */
  delay_matrix_.resize({matrix_height, matrix_width});

  /* Fill matrix */
  for (const auto& line : delay_lines) {
    /* Create a tokenizer for each line  */
    StringToken line_tokenizer(line);
    std::vector<std::string> delay_elements =
      line_tokenizer.split(element_delim_);
    /* Get maximum number of length, which is the width of delay matrix  */
    for (const auto& element : delay_elements) {
      delay_matrix_[size_t(&line - &delay_lines[0])]
                   [size_t(&element - &delay_elements[0])] = stof(element);
    }
  }

  return;
}

void PortDelayParser::set_default_element_delim() {
  /* Ensure a clean start */
  element_delim_.clear();
  element_delim_.push_back(' ');
  element_delim_.push_back('\t');
  return;
}

void PortDelayParser::set_default_line_delim() {
  /* Ensure a clean start */
  line_delim_.clear();
  line_delim_.push_back('\n');
  line_delim_.push_back('\r');
  return;
}

void PortDelayParser::clear() {
  delay_matrix_.clear();
  return;
}

}  // namespace openfpga
