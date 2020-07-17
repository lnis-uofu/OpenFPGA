#ifndef SPICE_SUBMODULE_H
#define SPICE_SUBMODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "netlist_manager.h"
#include "module_manager.h"
#include "openfpga_arch.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_spice_submodule(NetlistManager& netlist_manager,
                          const ModuleManager& module_manager,
                          const Arch& openfpga_arch,
                          const std::string& submodule_dir);

} /* end namespace openfpga */

#endif
