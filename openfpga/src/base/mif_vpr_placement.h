#pragma once

#include <string>
#include <utility>
#include <vector>

#include "command_exit_codes.h"
#include "mif_storage.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"

namespace openfpga {

/* Resolve an eblif subckt to its packed operating pb_type through the output
 * net used by VPR as the AtomBlock name. VPR pack must already be complete. */
std::string get_mif_pb_type_from_vpr(
  const std::string& model_name,
  const std::vector<std::pair<std::string, std::string>>& port_connections);

/* Rewrite PHYSICAL-stage segment pb paths via VprDeviceAnnotation
 * (operating/des path -> OpenFPGA physical hierarchy path). */
int rewrite_aggregated_mif_physical_pb(
  MifStorage& physical_storage, const DeviceContext& vpr_device_ctx,
  const VprDeviceAnnotation& vpr_device_annotation);

} /* namespace openfpga */
