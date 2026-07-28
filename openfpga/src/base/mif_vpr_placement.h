#pragma once

#include <string>
#include <utility>
#include <vector>

namespace openfpga {

/* Resolve an eblif subckt to its packed pb_type through the output net used
 * by VPR as the AtomBlock name. VPR pack must already be complete. */
std::string get_mif_pb_type_from_vpr(
  const std::string& model_name,
  const std::vector<std::pair<std::string, std::string>>& port_connections);

} /* namespace openfpga */
