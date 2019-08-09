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
 * Filename:    string_token.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/08/09  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/

/************************************************************************
 * Member functions for StringToken class 
 ***********************************************************************/

#include "string.h"

#include "vtr_assert.h"

#include "string_token.h"

/************************************************************************
 * Constructors
 ***********************************************************************/
StringToken::StringToken (const std::string& data) {
  set_data(data);
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
/* Get the data string */
std::string StringToken::data() const {
  return data_;
}

/* Split the string */
std::vector<std::string> StringToken::split() {
  /* Return vector */
  std::vector<std::string> ret;

  /* Add a default delim */ 
  if (true == delims_.empty()) {
    add_default_delim();
  }
  /* Create delims */
  std::string delims;
  for (const auto& delim : delims_) {
    delims.push_back(delim);
  }
  /* Get a writable char array */
  char* tmp = new char[data_.size() + 1];
  std::copy(data_.begin(), data_.end(), tmp);
  tmp[data_.size()] = '\0';
  /* Split using strtok */
  char* result = strtok(tmp, delims.c_str());
  while (NULL != result) {
    std::string result_str(result);
    /* Store the token */
    ret.push_back(result_str);
    /* Got to next */
    result = strtok(NULL, delims.c_str());
  }

  /* Free the tmp */
  delete[] tmp;

  return ret;
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void StringToken::set_data(const std::string& data) {
  data_ = data;
  return;
}

/* Add a delima to the list */
void StringToken::add_delim(const char& delim) {
  delims_.push_back(delim);
}

/************************************************************************
 * Internal Mutators
 ***********************************************************************/
void StringToken::add_default_delim() {
  VTR_ASSERT_SAFE(true == delims_.empty());
  delims_.push_back(' ');
  return;
}

/************************************************************************
 * End of file : string_token.cpp
 ***********************************************************************/

