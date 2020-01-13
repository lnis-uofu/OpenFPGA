#ifndef CIRCUIT_SETTINGS_H
#define CIRCUIT_SETTINGS_H

#include "circuit_library.h"

/* A unified data structure to store circuit-level settings,
 * including circuit library, technology library and simulation parameters
 */
struct CircuitSettings {
  CircuitLibrary circuit_lib;
};

#endif
