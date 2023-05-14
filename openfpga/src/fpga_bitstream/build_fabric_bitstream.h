#ifndef BUILD_FABRIC_BITSTREAM_H
#define BUILD_FABRIC_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "bitstream_manager.h"
#include "circuit_library.h"
#include "config_protocol.h"
#include "fabric_bitstream.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

FabricBitstream build_fabric_dependent_bitstream(
  const BitstreamManager& bitstream_manager,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const ConfigProtocol& config_protocol, const bool& verbose);

FabricBitstream load_fabric_dependent_bitstream(
  const BitstreamManager& bitstream_manager,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const ConfigProtocol& config_protocol, const bool& verbose,
  const std::string& infile);

} /* end namespace openfpga */

#endif
