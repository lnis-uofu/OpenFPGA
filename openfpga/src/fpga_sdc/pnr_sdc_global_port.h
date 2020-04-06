#ifndef PNR_SDC_GLBOAL_PORT_H
#define PNR_SDC_GLBOAL_PORT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "circuit_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_pnr_sdc_global_ports(const std::string& sdc_dir, 
                                const float& programming_critical_path_delay,
                                const float& operating_critical_path_delay,
                                const CircuitLibrary& circuit_lib,
                                const std::vector<CircuitPortId>& global_ports,
                                const bool& constrain_non_clock_port);

} /* end namespace openfpga */

#endif
