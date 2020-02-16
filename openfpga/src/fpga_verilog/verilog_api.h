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
#include "verilog_options.h"

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
                         const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
