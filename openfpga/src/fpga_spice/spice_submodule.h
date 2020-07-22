#ifndef SPICE_SUBMODULE_H
#define SPICE_SUBMODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "netlist_manager.h"
#include "technology_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_spice_submodule(NetlistManager& netlist_manager,
                          const TechnologyLibrary& tech_lib,
                          const std::string& submodule_dir);

} /* end namespace openfpga */

#endif
