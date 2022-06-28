#ifndef SDC_MUX_UTILS_H
#define SDC_MUX_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <string>
#include "mux_library.h"
#include "circuit_library.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_sdc_disable_routing_multiplexer_outputs(const std::string& sdc_dir,
                                                   const bool& flatten_names,
                                                   const bool& include_time_stamp,
                                                   const MuxLibrary& mux_lib,
                                                   const CircuitLibrary& circuit_lib,
                                                   const ModuleManager& module_manager,
                                                   const ModuleId& top_module);

int print_sdc_disable_routing_multiplexer_configure_ports(std::fstream& fp,
                                                          const bool& flatten_names,
                                                          const MuxLibrary& mux_lib,
                                                          const CircuitLibrary& circuit_lib,
                                                          const ModuleManager& module_manager,
                                                          const ModuleId& top_module);


} /* end namespace openfpga */

#endif
