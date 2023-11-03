#ifndef VERILOG_TEMPLATE_TESTBENCH_H
#define VERILOG_TEMPLATE_TESTBENCH_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "io_name_map.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "verilog_testbench_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_verilog_template_testbench(const ModuleManager &module_manager,
                                     const IoNameMap &io_name_map,
                                     const ModuleNameMap &module_name_map,
                                     const std::string &verilog_fname,
                                     const VerilogTestbenchOption &options);

} /* end namespace openfpga */

#endif
