/***************************************************************************************
 * Header file for build_decoder_modules.cpp
 ***************************************************************************************/
#ifndef BUILD_DECODER_MODULES_H
#define BUILD_DECODER_MODULES_H

#include "module_manager.h"
#include "mux_library.h"
#include "circuit_library.h"

void build_mux_local_decoder_modules(ModuleManager& module_manager,
                                     const MuxLibrary& mux_lib,
                                     const CircuitLibrary& circuit_lib);

#endif
