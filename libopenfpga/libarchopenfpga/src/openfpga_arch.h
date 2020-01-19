#ifndef OPENFPGA_ARCH_H
#define OPENFPGA_ARCH_H

#include <map>

#include "circuit_library.h"
#include "technology_library.h"
#include "simulation_setting.h"
#include "config_protocol.h"

/* namespace openfpga begins */
namespace openfpga {

/* A unified data structure to store circuit-level settings,
 * including circuit library, technology library and simulation parameters
 *
 * Note: 
 * Once this struct is built by function read_xml_openfpga_arch()
 * It should be READ-ONLY! Any modification should not be applied later
 * This is to keep everything well modularized
 */
struct Arch {
  /* Circuit models */
  CircuitLibrary circuit_lib;
  
  /* Technology devices */
  TechnologyLibrary tech_lib;

  /* Simulation settings */
  SimulationSetting sim_setting;

  /* Configuration protocol settings */
  ConfigProtocol config_protocol;

  /* Mapping from the names of routing switches 
   * to circuit models in circuit library 
   */
  std::map<std::string, CircuitModelId> cb_switch2circuit;
  std::map<std::string, CircuitModelId> sb_switch2circuit;

  /* Mapping from the names of routing segments
   * to circuit models in circuit library 
   */
  std::map<std::string, CircuitModelId> routing_seg2circuit;

  /* Mapping from the names of direct connection
   * to circuit models in circuit library 
   */
  std::map<std::string, CircuitModelId> direct2circuit;
};

} /* namespace openfpga ends */

#endif
