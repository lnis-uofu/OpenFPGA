#ifndef VERILOG_TOP_MODULE_H
#define VERILOG_TOP_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_top_module(const ModuleManager& module_manager,
                              const std::string& verilog_dir,
                              const bool& use_explicit_mapping);

} /* end namespace openfpga */

#endif
