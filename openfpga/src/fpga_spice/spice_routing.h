#ifndef SPICE_ROUTING_H
#define SPICE_ROUTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include "mux_library.h"
#include "module_manager.h"
#include "netlist_manager.h"
#include "device_rr_gsb.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_spice_flatten_routing_modules(NetlistManager& netlist_manager,
                                         const ModuleManager& module_manager,
                                         const DeviceRRGSB& device_rr_gsb,
                                         const std::string& subckt_dir);

void print_spice_unique_routing_modules(NetlistManager& netlist_manager,
                                        const ModuleManager& module_manager,
                                        const DeviceRRGSB& device_rr_gsb,
                                        const std::string& subckt_dir);

} /* end namespace openfpga */

#endif
