#ifndef VERILOG_MODULE_WRITER_H
#define VERILOG_MODULE_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>

#include "fabric_verilog_options.h"
#include "module_manager.h"
#include "verilog_port_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void write_verilog_module_to_file(std::fstream& fp,
                                  const ModuleManager& module_manager,
                                  const ModuleId& module_id,
                                  const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
