#ifndef OPENFPGA_SCALE_H
#define OPENFPGA_SCALE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

/********************************************************************
 * Function declaration
 *******************************************************************/
/* namespace openfpga begins */
namespace openfpga {

bool same_float_number(const float& a,
                       const float& b,
                       const float& epsilon);

std::string unit_to_string(const float& unit);

std::string time_unit_to_string(const float& unit, const std::string& postfix = "s");

float string_to_unit(const std::string& scale);

float string_to_time_unit(const std::string& scale); 

} /* namespace openfpga ends */

#endif
