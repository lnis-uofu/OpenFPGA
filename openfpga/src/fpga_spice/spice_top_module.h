#ifndef SPICE_TOP_MODULE_H
#define SPICE_TOP_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "module_manager.h"
#include "netlist_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_spice_top_module(NetlistManager& netlist_manager,
                            const ModuleManager& module_manager,
                            const std::string& spice_dir);

} /* end namespace openfpga */

#endif
