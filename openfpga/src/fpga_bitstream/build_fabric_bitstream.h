#ifndef BUILD_FABRIC_BITSTREAM_H
#define BUILD_FABRIC_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "config_protocol.h"
#include "bitstream_manager.h"
#include "fabric_bitstream.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

FabricBitstream build_fabric_dependent_bitstream(const BitstreamManager& bitstream_manager,
                                                 const ModuleManager& module_manager,
                                                 const ConfigProtocol& config_protocol,
                                                 const bool& verbose);

} /* end namespace openfpga */

#endif
