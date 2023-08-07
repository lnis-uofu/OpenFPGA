/******************************************************************************
 * This files includes declarations for most utilized functions
 * for data structures for module management.
 ******************************************************************************/
#ifndef MODULE_MANAGER_MEMORY_UTILS_H
#define MODULE_MANAGER_MEMORY_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <tuple>
#include <vector>

/* Headers from readarch library */
#include "physical_types.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"

/* Headers from readarchopenfpga library */
#include "circuit_library.h"
#include "circuit_types.h"
#include "config_protocol.h"
#include "decoder_library.h"
#include "fabric_key.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int load_submodules_memory_modules_from_fabric_key(
  ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const ConfigProtocol& config_protocol, const FabricKey& fabric_key,
  const bool& group_config_block);

} /* end namespace openfpga */

#endif
