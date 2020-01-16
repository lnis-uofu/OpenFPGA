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

} /* namespace openfpga ends */

#endif
