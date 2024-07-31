#ifndef OPENFPGA_PORT_PARSER_H
#define OPENFPGA_PORT_PARSER_H

/********************************************************************
 * Include header files that are required by data structure declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "openfpga_port.h"
#include "openfpga_tokenizer.h"
#include "vtr_geometry.h"
#include "vtr_ndmatrix.h"

/************************************************************************
 * This file includes parsers for port definition in the architecture XML
 * language. Note that it is also compatiable to Verilog syntax.
 * It means we may reuse this for constructing a structural Verilog parser
 ***********************************************************************/

/* namespace openfpga begins */
namespace openfpga {

constexpr int PORT_PARSER_SUPPORT_NO_PORT_FORMAT = (1 << 0);       // (5) below
constexpr int PORT_PARSER_SUPPORT_SINGLE_INDEX_FORMAT = (1 << 1);  // (3) below
constexpr int PORT_PARSER_SUPPORT_RANGE_FORMAT = (1 << 2);  // (1) and (2) below
constexpr int PORT_PARSER_SUPPORT_ALL_FORMAT = ((1 << 3) - 1);

/************************************************************************
 * Class PortParser: single port parser
 * Supported port definition:
 * (1) <port_name>[<LSB>:<MSB>]
 * (2) <port_name>[<MSB>:<LSB>]
 * (3) <port_name>[<single_pin_index>]
 * (4) <port_name>[]  -- this is not currently supported. Two problems:
 *                       * tokenizer will error out and
 *                       * stoi cannot support empty string, and give
 *                         std::invalid_argument error
 * (5) <port_name>
 * In case 4 and 5, we will assign (-1,-1) for LSB and MSB
 ***********************************************************************/
class PortParser {
 public: /* Constructors*/
  PortParser(const std::string& data,
             const int support_format = PORT_PARSER_SUPPORT_ALL_FORMAT);

 public: /* Public Accessors */
  std::string data() const;
  BasicPort port() const;
  bool valid() const;

 public: /* Public Mutators */
  void set_support_format(const int support_format);
  void set_data(const std::string& data);

 private: /* Private Mutators */
  void parse();
  void set_default_bracket();
  void set_default_delim();
  size_t string_to_number(const std::string& str);

 private:            /* Internal data */
  std::string data_; /* Lines to be splited */
  int support_format_;
  vtr::Point<char> bracket_;
  char delim_;
  BasicPort port_;
  bool valid_;
};

/************************************************************************
 * MultiPortParser: a parser for multiple ports in one line
 ***********************************************************************/
class MultiPortParser {
 public: /* Constructors*/
  MultiPortParser(const std::string& data);

 public: /* Public Accessors */
  std::string data() const;
  std::vector<BasicPort> ports() const;

 public: /* Public Mutators */
  void set_data(const std::string& data);

 private: /* Private Mutators */
  void parse();
  void set_default_delim();
  void clear();

 private:            /* Internal data */
  std::string data_; /* Lines to be splited */
  char delim_;
  std::vector<BasicPort> ports_;
};

/************************************************************************
 * PortDelayParser: a parser for 2D delay matrix
 ***********************************************************************/
class PortDelayParser {
 public: /* Constructors*/
  PortDelayParser(const std::string& data);

 public: /* Public Accessors */
  std::string data() const;
  size_t height() const;
  size_t width() const;
  vtr::Point<size_t> delay_size() const;
  float delay(size_t x, size_t y) const;

 public: /* Public Mutators */
  void set_data(const std::string& data);

 private: /* Private Mutators */
  void parse();
  void set_default_element_delim();
  void set_default_line_delim();
  void clear();

 private:            /* Internal data */
  std::string data_; /* Lines to be splited */
  std::vector<char> element_delim_;
  std::vector<char> line_delim_;
  vtr::Matrix<float> delay_matrix_;
};

}  // namespace openfpga

#endif
