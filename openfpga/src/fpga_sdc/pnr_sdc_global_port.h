#ifndef PNR_SDC_GLBOAL_PORT_H
#define PNR_SDC_GLBOAL_PORT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "module_manager.h"
#include "fabric_global_port_info.h"
#include "simulation_setting.h"
#include "pnr_sdc_option.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_pnr_sdc_global_ports(const PnrSdcOption& options,
                                const ModuleManager& module_manager,
                                const ModuleId& top_module,
                                const FabricGlobalPortInfo& global_ports,
                                const SimulationSetting& sim_setting);

} /* end namespace openfpga */

#endif
