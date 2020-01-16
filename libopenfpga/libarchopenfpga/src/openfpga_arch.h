#ifndef OPENFPGA_ARCH_H
#define OPENFPGA_ARCH_H

#include "circuit_library.h"

/* A unified data structure to store circuit-level settings,
 * including circuit library, technology library and simulation parameters
 */
struct OpenFPGAArch {
  CircuitLibrary circuit_lib;
};

#endif
