/********************************************************************
 * Header file for circuit_library_utils.cpp
 *******************************************************************/
#ifndef CIRCUIT_LIBRARY_UTILS_H
#define CIRCUIT_LIBRARY_UTILS_H

/* Header files should be included in a sequence */
/* Standard header files required go first */

#include <vector>
#include "circuit_library.h"

std::vector<CircuitModelId> get_circuit_sram_models(const CircuitLibrary& circuit_lib,
                                                    const CircuitModelId& circuit_model);

#endif
