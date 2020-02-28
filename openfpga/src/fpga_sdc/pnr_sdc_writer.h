#ifndef PNR_SDC_WRITER_H
#define PNR_SDC_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "vpr_context.h"
#include "vpr_device_annotation.h"
#include "device_rr_gsb.h"
#include "module_manager.h"
#include "mux_library.h"
#include "circuit_library.h"
#include "pnr_sdc_option.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_pnr_sdc(const PnrSdcOption& sdc_options,
                   const float& critical_path_delay,
                   const DeviceContext& device_ctx,
                   const VprDeviceAnnotation& device_annotation,
                   const DeviceRRGSB& device_rr_gsb,
                   const ModuleManager& module_manager,
                   const MuxLibrary& mux_lib,
                   const CircuitLibrary& circuit_lib,
                   const std::vector<CircuitPortId>& global_ports,
                   const bool& compact_routing_hierarchy);

} /* end namespace openfpga */

#endif
