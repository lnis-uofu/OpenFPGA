#ifndef ANNOTATE_SIMULATION_SETTING_H
#define ANNOTATE_SIMULATION_SETTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void annotate_simulation_setting(const AtomContext& atom_ctx, 
                                 const std::unordered_map<AtomNetId, t_net_power>& net_activity, 
                                 SimulationSetting& sim_setting);

} /* end namespace openfpga */

#endif
