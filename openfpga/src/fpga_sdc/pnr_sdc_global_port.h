#ifndef PNR_SDC_GLBOAL_PORT_H
#define PNR_SDC_GLBOAL_PORT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "module_manager.h"
#include "fabric_global_port_info.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_pnr_sdc_global_ports(const std::string& sdc_dir, 
                                const float& programming_critical_path_delay,
                                const float& operating_critical_path_delay,
                                const ModuleManager& module_manager,
                                const ModuleId& top_module,
                                const FabricGlobalPortInfo& global_ports,
                                const bool& constrain_non_clock_port);

} /* end namespace openfpga */

#endif
