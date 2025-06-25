#ifndef BUILD_FABRIC_BITSTREAM_H
#define BUILD_FABRIC_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>

#include "bitstream_manager.h"
#include "circuit_library.h"
#include "config_protocol.h"
#include "fabric_bitstream.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "bitstream_reorder_map.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

FabricBitstream build_fabric_dependent_bitstream(
  const BitstreamManager& bitstream_manager,
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const CircuitLibrary& circuit_lib, const ConfigProtocol& config_protocol,
  const bool& verbose);

FabricBitstream build_fabric_dependent_bitstream_with_reorder(
  const BitstreamManager& bitstream_manager,
  const FabricBitstream& original_fabric_bitstream,
  const ModuleManager& module_manager, const ModuleNameMap& module_name_map,
  const BitstreamReorderMap& bitstream_reorder_map,
  const std::string& output_file_name,
  const bool& verbose);

} /* end namespace openfpga */

#endif
