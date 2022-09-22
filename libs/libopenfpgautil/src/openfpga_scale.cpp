/********************************************************************
 * This file includes functions that convert time/resistance/capacitance
 * units to string or vice versa
 *******************************************************************/
#include <cmath>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_scale.h" 

namespace openfpga {

/* A small ratio for float number comparison 
 * If the float number B is in the range of the referance A +/- epsilon 
 * we regard A == B
 *   A - A * EPSILON <= B <= A + A * EPSILON 
 */
#define EPSILON_RATIO 1e-3

bool same_float_number(const float& a,
                       const float& b,
                       const float& epsilon) {
  /* Always use a positive epsilon */
  if ( (a - a * std::abs(epsilon) <= b) 
    && (b <= a + a * std::abs(epsilon)) ) {
    return true;
  }
 
  return false;
}

/********************************************************************
 * Convert numeric unit to string:
 *   - 1e12  -> T
 *   - 1e9   -> B
 *   - 1e6   -> M
 *   - 1e3   -> k
 *   - 1.    ->  
 *   - 1e-3  -> m
 *   - 1e-6  -> u 
 *   - 1e-9  -> n
 *   - 1e-12 -> p
 *   - 1e-15 -> f 
 *   - 1e-18 -> a 
 *******************************************************************/
std::string unit_to_string(const float& unit) {
  if (true == same_float_number(unit, 1., EPSILON_RATIO)) {
    return std::string();
  /* Larger than 1 unit */
  } else if (true == same_float_number(unit, 1e3, EPSILON_RATIO)) {
    return std::string("k");
  } else if (true == same_float_number(unit, 1e6, EPSILON_RATIO)) {
    return std::string("M");
  } else if (true == same_float_number(unit, 1e9, EPSILON_RATIO)) {
    return std::string("B");
  } else if (true == same_float_number(unit, 1e12, EPSILON_RATIO)) {
    return std::string("T");
  /* Less than 1 unit */
  } else if (true == same_float_number(unit, 1e-3, EPSILON_RATIO)) {
    return std::string("m");
  } else if (true == same_float_number(unit, 1e-6, EPSILON_RATIO)) {
    return std::string("u");
  } else if (true == same_float_number(unit, 1e-9, EPSILON_RATIO)) {
    return std::string("n");
  } else if (true == same_float_number(unit, 1e-12, EPSILON_RATIO)) {
    return std::string("p");
  } else if (true == same_float_number(unit, 1e-15, EPSILON_RATIO)) {
    return std::string("f");
  } else if (true == same_float_number(unit, 1e-18, EPSILON_RATIO)) {
    return std::string("a");
  }

  /* Invalid unit report error */
  VTR_LOGF_ERROR(__FILE__, __LINE__,
                 "Invalid unit %g!\nAcceptable units are [1e12|1e9|1e6|1e3|1|1e-3|1e-6|1e-9|1e-12|1e-15|1e-18]\n",
                 unit); 
  exit(1);
}

/********************************************************************
 * Convert numeric time unit to string
 * e.g. 1e-12 -> ps
 *******************************************************************/
std::string time_unit_to_string(const float& unit, const std::string& postfix) {
  /* For larger than 1 unit, we do not accept */
  if (1e6 < unit) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid time unit %g!\nAcceptable units are [1e6|1e3|1|1e-3|1e-6|1e-9|1e-12|1e-15|1e-18]\n",
                   unit); 
    exit(1);
  }

  return unit_to_string(unit) + postfix;
}

/********************************************************************
 * Convert string unit to numeric:
 *   - T  -> 1e12
 *   - B  -> 1e9 
 *   - M  -> 1e6
 *   - k  -> 1e3
 *   - "" -> 1.  
 *   - m  -> 1e-3
 *   - u  -> 1e-6 
 *   - n  -> 1e-9
 *   - p  -> 1e-12
 *   - f  -> 1e-15
 *   - a  -> 1e-18
 *******************************************************************/
float string_to_unit(const std::string& scale) {
  if (true == scale.empty()) {
    return 1.;
  /* Larger than 1 unit */
  } else if (std::string("T") == scale) {
    return 1e12;
  } else if (std::string("B") == scale) {
    return 1e9;
  } else if (std::string("M") == scale) {
    return 1e6;
  } else if (std::string("k") == scale) {
    return 1e3;
  /* Less than 1 unit */
  } else if (std::string("m") == scale) {
    return 1e-3;
  } else if (std::string("u") == scale) {
    return 1e-6;
  } else if (std::string("n") == scale) {
    return 1e-9;
  } else if (std::string("p") == scale) {
    return 1e-12;
  } else if (std::string("f") == scale) {
    return 1e-15;
  } else if (std::string("a") == scale) {
    return 1e-18;
  }

  /* Invalid unit report error */
  VTR_LOGF_ERROR(__FILE__, __LINE__,
                 "Invalid unit %s!\nAcceptable units are [a|f|p|n|u|k|M|B|T] or empty\n",
                 scale.c_str()); 
  exit(1);
}

/********************************************************************
 * Convert string time unit to numeric
 * e.g. ps -> 1e-12
 *******************************************************************/
float string_to_time_unit(const std::string& scale) {
  if ( (1 != scale.length())
    && (2 != scale.length()) ) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Time unit (='%s') must contain only one or two characters!\n",
                   scale.c_str());
  } 
  /* The last character must be 's' */
  if ('s' != scale.back()) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Time unit (='%s') must end with 's'!\n",
                   scale.c_str());
  }

  float unit = 1.;
  VTR_ASSERT ( (1 == scale.length())
            || (2 == scale.length()) );
  if (2 == scale.length()) {
    unit = string_to_unit(scale.substr(0, 1));
  }

  /* For larger than 1 unit, we do not accept */
  if (1e6 < unit) {
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid time unit %g!\nAcceptable units are [1e6|1e3|1|1e-3|1e-6|1e-9|1e-12|1e-15|1e-18]\n",
                   unit); 
    exit(1);
  }

  return unit;
}

} /* namespace openfpga ends */
