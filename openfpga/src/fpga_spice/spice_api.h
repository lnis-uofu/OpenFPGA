#ifndef SPICE_API_H 
#define SPICE_API_H 

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <string>
#include <vector>
#include "netlist_manager.h"
#include "module_manager.h"
#include "openfpga_arch.h"
#include "mux_library.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"
#include "device_rr_gsb.h"
#include "fabric_spice_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int fpga_fabric_spice(const ModuleManager& module_manager,
                      NetlistManager& netlist_manager,
                      const Arch& openfpga_arch,
                      const MuxLibrary& mux_lib,
                      const DeviceContext &device_ctx,
                      const VprDeviceAnnotation &device_annotation,
                      const DeviceRRGSB &device_rr_gsb,
                      const FabricSpiceOption& options);

} /* end namespace openfpga */

#endif
