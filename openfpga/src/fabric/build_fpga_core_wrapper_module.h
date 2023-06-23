#ifndef BUILD_FPGA_CORE_WRAPPER_MODULE_H
#define BUILD_FPGA_CORE_WRAPPER_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "module_manager.h"
#include "io_name_map.h"
#include <string>

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int add_fpga_core_to_device_module_graph(ModuleManager& module_manager,
                                         const IoNameMap& io_naming,
                                         const std::string& core_inst_name,
                                         const bool& frame_view,
                                         const bool& verbose);

} /* end namespace openfpga */

#endif
