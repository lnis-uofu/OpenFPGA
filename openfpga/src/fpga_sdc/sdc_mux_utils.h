#ifndef SDC_MUX_UTILS_H
#define SDC_MUX_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <string>
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void rec_print_pnr_sdc_disable_routing_multiplexer_outputs(std::fstream& fp, 
                                                           const bool& flatten_names,
                                                           const ModuleManager& module_manager, 
                                                           const ModuleId& parent_module,
                                                           const ModuleId& mux_module,
                                                           const std::string& parent_module_path);


} /* end namespace openfpga */

#endif
