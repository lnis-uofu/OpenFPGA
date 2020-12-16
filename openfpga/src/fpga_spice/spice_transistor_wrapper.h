#ifndef SPICE_TRANSISTOR_WRAPPER_H
#define SPICE_TRANSISTOR_WRAPPER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <map>
#include "netlist_manager.h"
#include "technology_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_spice_transistor_wrapper(NetlistManager& netlist_manager,
                                   const TechnologyLibrary& tech_lib,
                                   const std::string& submodule_dir);

int print_spice_generic_pmos_modeling(std::fstream& fp,
                                      const std::string& trans_name_postfix,
                                      const std::string& input_port_name,
                                      const std::string& gate_port_name,
                                      const std::string& output_port_name,
                                      const TechnologyLibrary& tech_lib,
                                      const TechnologyModelId& tech_model,
                                      const float& trans_width);

int print_spice_generic_nmos_modeling(std::fstream& fp,
                                      const std::string& trans_name_postfix,
                                      const std::string& input_port_name,
                                      const std::string& gate_port_name,
                                      const std::string& output_port_name,
                                      const TechnologyLibrary& tech_lib,
                                      const TechnologyModelId& tech_model,
                                      const float& trans_width);

} /* end namespace openfpga */

#endif
