#ifndef CONFIGURE_PORT_SDC_WRITER_H
#define CONFIGURE_PORT_SDC_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_sdc_disable_timing_configure_ports(const std::string& sdc_fname,
                                             const bool& flatten_names,
                                             const MuxLibrary& mux_lib,
                                             const CircuitLibrary& circuit_lib,
                                             const ModuleManager& module_manager);

} /* end namespace openfpga */

#endif
