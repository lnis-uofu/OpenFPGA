#ifndef CONFIGURATION_CHAIN_SDC_WRITER_H
#define CONFIGURATION_CHAIN_SDC_WRITER_H

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

void print_pnr_sdc_constrain_configurable_chain(const std::string& sdc_fname,
                                                const float& time_unit,
                                                const float& max_delay,
                                                const float& min_delay,
                                                const bool& include_time_stamp,
                                                const ModuleManager& module_manager);

} /* end namespace openfpga */

#endif
