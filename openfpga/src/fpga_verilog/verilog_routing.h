#ifndef VERILOG_ROUTING_H
#define VERILOG_ROUTING_H

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

void print_verilog_flatten_routing_modules(NetlistManager& netlist_manager,
                                           const ModuleManager& module_manager,
                                           const DeviceRRGSB& device_rr_gsb,
                                           const std::string& verilog_dir,
                                           const std::string& subckt_dir,
                                           const bool& use_explicit_port_map);

void print_verilog_unique_routing_modules(NetlistManager& netlist_manager,
                                          const ModuleManager& module_manager,
                                          const DeviceRRGSB& device_rr_gsb,
                                          const std::string& verilog_dir,
                                          const std::string& subckt_dir,
                                          const bool& use_explicit_port_map);

} /* end namespace openfpga */

#endif
