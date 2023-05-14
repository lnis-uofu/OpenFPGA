#ifndef BUILD_FABRIC_BITSTREAM_MEMORY_BANK_H
#define BUILD_FABRIC_BITSTREAM_MEMORY_BANK_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
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

void build_module_fabric_dependent_bitstream_ql_memory_bank(
  const ConfigProtocol& config_protocol, const CircuitLibrary& circuit_lib,
  const BitstreamManager& bitstream_manager, const ConfigBlockId& top_block,
  const ModuleManager& module_manager, const ModuleId& top_module,
  FabricBitstream& fabric_bitstream);

void load_module_fabric_dependent_bitstream_ql_memory_bank(
  const ConfigProtocol& config_protocol, const CircuitLibrary& circuit_lib,
  const BitstreamManager& bitstream_manager, const ConfigBlockId& top_block,
  const ModuleManager& module_manager, const ModuleId& top_module,
  FabricBitstream& fabric_bitstream, const std::string& infile);

} /* end namespace openfpga */

#endif
