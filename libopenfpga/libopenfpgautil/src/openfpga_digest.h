#ifndef OPENFPGA_DIGEST_H
#define OPENFPGA_DIGEST_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>

/********************************************************************
 * Function declaration
 *******************************************************************/
/* namespace openfpga begins */
namespace openfpga {

bool valid_file_stream(std::fstream& fp);

void check_file_stream(const char* fname, 
                       std::fstream& fp);

std::string format_dir_path(const std::string& dir_path_to_format);

std::string find_path_file_name(const std::string& file_name);

std::string find_path_dir_name(const std::string& file_name);

bool create_dir_path(const char* dir_path);

} /* namespace openfpga ends */

#endif
