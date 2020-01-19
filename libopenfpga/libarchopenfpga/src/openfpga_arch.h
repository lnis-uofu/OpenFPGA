#ifndef OPENFPGA_ARCH_H
#define OPENFPGA_ARCH_H

#include "circuit_library.h"
#include "technology_library.h"
#include "simulation_setting.h"
#include "config_protocol.h"

/* namespace openfpga begins */
namespace openfpga {

/* A unified data structure to store circuit-level settings,
 * including circuit library, technology library and simulation parameters
 */
struct Arch {
  CircuitLibrary circuit_lib;
  TechnologyLibrary tech_lib;
  SimulationSetting sim_setting;
  ConfigProtocol config_protocol;
};

} /* namespace openfpga ends */

#endif
