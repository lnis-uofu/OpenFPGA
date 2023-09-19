#ifndef BUILD_FPGA_CORE_WRAPPER_MODULE_H
#define BUILD_FPGA_CORE_WRAPPER_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "io_name_map.h"
#include "module_manager.h"
#include "module_name_map.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int add_fpga_core_to_device_module_graph(ModuleManager& module_manager,
                                         ModuleNameMap& module_name_map,
                                         const IoNameMap& io_naming,
                                         const std::string& core_inst_name,
                                         const bool& frame_view,
                                         const bool& verbose);

} /* end namespace openfpga */

#endif
