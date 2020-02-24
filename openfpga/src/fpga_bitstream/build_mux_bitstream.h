#ifndef BUILD_MUX_BITSTREAM_H
#define BUILD_MUX_BITSTREAM_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "circuit_library.h"
#include "mux_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

size_t find_mux_default_path_id(const CircuitLibrary& circuit_lib,
                                const CircuitModelId& mux_model,
                                const size_t& mux_size);

std::vector<bool> build_mux_bitstream(const CircuitLibrary& circuit_lib,
                                      const CircuitModelId& mux_model,
                                      const MuxLibrary& mux_lib,
                                      const size_t& mux_size,
                                      const int& path_id);

} /* end namespace openfpga */

#endif
