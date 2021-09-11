#ifndef BUILD_TOP_MODULE_MEMORY_UTILS_H
#define BUILD_TOP_MODULE_MEMORY_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <vector>
#include <map>
#include "vtr_vector.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/* A data structure to store the number of configuration bits for each configurable region
 * of the top-level module.
 * For different configuration protocol, the std::pair<size_t, size_t> represents different data
 * See details in each function about how the data is organized
 */
typedef vtr::vector<ConfigRegionId, std::pair<size_t, size_t>> TopModuleNumConfigBits;

} /* end namespace openfpga */

#endif
