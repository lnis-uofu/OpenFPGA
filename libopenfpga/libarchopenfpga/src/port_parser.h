/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef PORT_PARSER_H
#define PORT_PARSER_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <string>
#include <vector>

#include "vtr_ndmatrix.h"
#include "vtr_geometry.h"

#include "device_port.h"
#include "string_token.h"

/************************************************************************
 * This file includes parsers for port definition in the architecture XML
 * language. Note that it is also compatiable to Verilog syntax.
 * It means we may reuse this for constructing a structural Verilog parser
 ***********************************************************************/

/************************************************************************
 * Class PortParser: single port parser
 * Supported port definition:
 * 1. <port_name>[<LSB>:<MSB>]
 * 2. <port_name>[<MSB>:<LSB>]
 * 3. <port_name>[<single_pin_index>]
 * 4. <port_name>[]  
 * 5. <port_name>  
 * In case 4 and 5, we will assign (-1,-1) for LSB and MSB
 ***********************************************************************/
class PortParser{
  public : /* Constructors*/
    PortParser (const std::string& data);
  public : /* Public Accessors */ 
    std::string data() const;
    BasicPort port() const;
  public : /* Public Mutators */ 
    void set_data(const std::string& data);
  private : /* Private Mutators */ 
    void parse();
    void set_default_bracket();
    void set_default_delim();
  private: /* Internal data */
    std::string data_; /* Lines to be splited */
    vtr::Point<char> bracket_;
    char delim_;
    BasicPort port_;
};

/************************************************************************
 * MultiPortParser: a parser for multiple ports in one line 
 ***********************************************************************/
class MultiPortParser {
  public : /* Constructors*/
    MultiPortParser (const std::string& data);
  public : /* Public Accessors */ 
    std::string data() const;
    std::vector<BasicPort> ports() const;
  public : /* Public Mutators */ 
    void set_data(const std::string& data);
  private : /* Private Mutators */ 
    void parse();
    void set_default_delim();
    void clear();
  private: /* Internal data */
    std::string data_; /* Lines to be splited */
    char delim_;
    std::vector<BasicPort> ports_;
};

/************************************************************************
 * PortDelayParser: a parser for 2D delay matrix
 ***********************************************************************/
class PortDelayParser {
  public : /* Constructors*/
    PortDelayParser (const std::string& data);
  public : /* Public Accessors */ 
    std::string data() const;
    size_t height() const;
    size_t width() const;
    vtr::Point<size_t> delay_size() const;
    float delay(size_t x, size_t y) const;
  public : /* Public Mutators */ 
    void set_data(const std::string& data);
  private : /* Private Mutators */ 
    void parse();
    void set_default_element_delim();
    void set_default_line_delim();
    void clear();
  private: /* Internal data */
    std::string data_; /* Lines to be splited */
    std::vector<char> element_delim_;
    std::vector<char> line_delim_;
    vtr::Matrix<float> delay_matrix_;
};


#endif

/************************************************************************
 * End of file : port_parser.h
 ***********************************************************************/

