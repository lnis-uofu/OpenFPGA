#ifndef OPENFPGA_PHYSICAL_TILE_UTILS_H
#define OPENFPGA_PHYSICAL_TILE_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include <string>
#include "physical_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

float find_physical_tile_pin_Fc(t_physical_tile_type_ptr type,
                                const int& pin);


} /* end namespace openfpga */

#endif
