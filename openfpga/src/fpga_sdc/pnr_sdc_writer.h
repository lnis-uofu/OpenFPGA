#ifndef PNR_SDC_WRITER_H
#define PNR_SDC_WRITER_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "circuit_library.h"
#include "device_rr_gsb.h"
#include "fabric_global_port_info.h"
#include "fabric_tile.h"
#include "module_manager.h"
#include "mux_library.h"
#include "pnr_sdc_option.h"
#include "simulation_setting.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_pnr_sdc(
  const PnrSdcOption& sdc_options, const DeviceContext& device_ctx,
  const VprDeviceAnnotation& device_annotation, const FabricTile& fabric_tile,
  const DeviceRRGSB& device_rr_gsb, const ModuleManager& module_manager,
  const MuxLibrary& mux_lib, const CircuitLibrary& circuit_lib,
  const FabricGlobalPortInfo& global_ports,
  const SimulationSetting& sim_setting, const bool& compact_routing_hierarchy);

} /* end namespace openfpga */

#endif
