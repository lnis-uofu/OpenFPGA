#ifndef BUILD_FABRIC_GLOBAL_PORT_INFO_H
#define BUILD_FABRIC_GLOBAL_PORT_INFO_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>
#include "tile_annotation.h"
#include "circuit_library.h"
#include "fabric_global_port_info.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

FabricGlobalPortInfo build_fabric_global_port_info(const ModuleManager& module_manager,
                                                   const TileAnnotation& tile_annotation,
                                                   const CircuitLibrary& circuit_lib);

} /* end namespace openfpga */

#endif
