#ifndef SPICE_AUXILIARY_NETLISTS_H
#define SPICE_AUXILIARY_NETLISTS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "circuit_library.h"
#include "netlist_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_spice_fabric_include_netlist(const NetlistManager& netlist_manager,
                                        const std::string& src_dir,
                                        const CircuitLibrary& circuit_lib);

} /* end namespace openfpga */

#endif 
