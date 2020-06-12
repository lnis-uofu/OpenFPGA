#ifndef BUILD_MEMORY_MODULES_H
#define BUILD_MEMORY_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "decoder_library.h"
#include "circuit_library.h"
#include "mux_library.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void build_memory_modules(ModuleManager& module_manager,
                          DecoderLibrary& arch_decoder_lib,
                          const MuxLibrary& mux_lib,
                          const CircuitLibrary& circuit_lib,
                          const e_config_protocol_type& sram_orgz_type);

} /* end namespace openfpga */

#endif
