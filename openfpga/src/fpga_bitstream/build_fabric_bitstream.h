#ifndef BUILD_FABRIC_BITSTREAM_H
#define BUILD_FABRIC_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "bitstream_manager.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::vector<ConfigBitId> build_fabric_dependent_bitstream(const BitstreamManager& bitstream_manager,
                                                          const ModuleManager& module_manager);

} /* end namespace openfpga */

#endif
