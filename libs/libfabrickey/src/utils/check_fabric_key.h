#ifndef CHECK_FABRIC_KEY_H
#define CHECK_FABRIC_KEY_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "fabric_key.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int check_fabric_key_alias(const FabricKey& input_key, const bool& verbose);

int check_fabric_key_names_and_values(const FabricKey& input_key,
                                      const bool& verbose);

} /* end namespace openfpga */

#endif
