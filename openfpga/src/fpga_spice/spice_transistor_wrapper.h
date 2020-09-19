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

} /* end namespace openfpga */

#endif
