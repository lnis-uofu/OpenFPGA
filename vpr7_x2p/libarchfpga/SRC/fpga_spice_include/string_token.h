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
 * Filename:    string_token.h
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/08/09  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/

/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef STRING_TOKEN_H
#define STRING_TOKEN_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <string>
#include <vector>


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

#endif

/************************************************************************
 * End of file : string_token.h
 ***********************************************************************/

