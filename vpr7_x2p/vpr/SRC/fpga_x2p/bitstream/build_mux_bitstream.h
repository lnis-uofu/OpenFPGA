/********************************************************************
 * Header file for build_mux_bitstream.cpp
 *******************************************************************/
#ifndef BUILD_MUX_BITSTREAM_H
#define BUILD_MUX_BITSTREAM_H

#include <vector>
#include "circuit_library.h"
#include "mux_library.h"

size_t find_mux_default_path_id(const CircuitLibrary& circuit_lib,
                                const CircuitModelId& mux_model,
                                const size_t& mux_size);

std::vector<bool> build_mux_bitstream(const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& mux_model,
                                      const MuxLibrary& mux_lib,
                                      const size_t& mux_size,
                                      const int& path_id);

#endif
