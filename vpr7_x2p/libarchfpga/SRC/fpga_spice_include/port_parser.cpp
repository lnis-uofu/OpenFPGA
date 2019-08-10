/**********************************************************
 * MIT License
 *
 * Copyright (c) 2018 LNIS - The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************/

/************************************************************************
 * Filename:    port_parser.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/08/09  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/

/************************************************************************
 * Member functions for Port parsers
 ***********************************************************************/

#include "string.h"

#include "vtr_assert.h"
#include "vtr_geometry.h"

#include "string_token.h"

#include "port_parser.h"

/************************************************************************
 * Member functions for PortParser class 
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
PortParser::PortParser (const std::string& data) {
  set_data(data);
  set_default_bracket();
  set_default_delim();
  parse();
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
/* Get the data string */
std::string PortParser::data() const {
  return data_;
}

BasicPort PortParser::port() const {
  return port_;
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
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
  /* Create a tokenizer */
  StringToken tokenizer(data_);

  /* Split the data into <port_name> and <pin_string> */
  std::vector<std::string> port_tokens = tokenizer.split(bracket_.x());
  /* Make sure we have a port name! */
  VTR_ASSERT_SAFE ((1 == port_tokens.size()) || (2 == port_tokens.size()));
  /* Store the port name! */
  port_.set_name(port_tokens[0]);

  /* If we only have one token */
  if (1 == port_tokens.size()) {
    port_.set_width(0); 
    return; /* We can finish here */
  }

  /* Chomp the ']' */
  tokenizer.set_data(port_tokens[1]);
  std::vector<std::string> pin_tokens = tokenizer.split(bracket_.y());
  /* Make sure we have a valid string! */
  VTR_ASSERT_SAFE (1 == port_tokens.size());

  /* Split the pin string now */
  tokenizer.set_data(port_tokens[0]);
  pin_tokens = tokenizer.split(delim_);

  /* Check if we have LSB and MSB or just one */
  if ( 1 == pin_tokens.size() ) {
    /* Single pin */
    port_.set_width(stoi(pin_tokens[0]), stoi(pin_tokens[0])); 
  } else if ( 2 == pin_tokens.size() ) {
    /* A number of pin */
    port_.set_width(stoi(pin_tokens[0]), stoi(pin_tokens[1])); 
  }

  /* Re-order to ensure LSB <= MSB */
  if (false == port_.is_valid()) {
    port_.revert(); 
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

/************************************************************************
 * Member functions for MultiPortParser class 
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
MultiPortParser::MultiPortParser (const std::string& data) {
  set_data(data);
  set_default_delim();
  parse();
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
/* Get the data string */
std::string MultiPortParser::data() const {
  return data_;
}

std::vector<BasicPort> MultiPortParser::ports() const {
  return ports_;
}

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
 * End of file : port_parser.cpp
 ***********************************************************************/

