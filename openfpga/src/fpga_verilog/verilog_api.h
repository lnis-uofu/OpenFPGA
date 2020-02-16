#ifndef VERILOG_API_H 
#define VERILOG_API_H 

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>
#include <vector>
#include "vpr_types.h"
#include "mux_library.h"
#include "circuit_library.h"
#include "device_grid.h"
#include "device_rr_gsb.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void fpga_fabric_verilog(const ModuleManager& module_manager,
                         const CircuitLibrary& circuit_lib,
                         const MuxLibrary& mux_lib,
                         const DeviceGrid& grids, 
                         const DeviceRRGSB& device_rr_gsb,
                         const std::string& output_directory,
                         const bool& compress_routing,
                         const bool& dump_explict_verilog,
                         const bool& verbose);

} /* end namespace openfpga */

#endif
