#ifndef SPICE_ESSENTIAL_GATES_H
#define SPICE_ESSENTIAL_GATES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "netlist_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_spice_transistor_wrapper(NetlistManager& netlist_manager,
                                    const std::string& submodule_dir);

} /* end namespace openfpga */

#endif
