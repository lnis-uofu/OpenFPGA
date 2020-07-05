#ifndef SPICE_API_H 
#define SPICE_API_H 

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>
#include <vector>
#include "netlist_manager.h"
#include "module_manager.h"
#include "fabric_spice_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void fpga_fabric_spice(const ModuleManager& module_manager,
                       NetlistManager& netlist_manager,
                       const FabricSpiceOption& options);

} /* end namespace openfpga */

#endif
