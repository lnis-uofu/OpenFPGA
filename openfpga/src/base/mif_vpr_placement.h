#pragma once

#include <map>
#include <string>

#include "vpr_types.h"

namespace openfpga {

struct MifPlacementInfo {
  t_pl_loc location;
  std::string pb_type_path;
};

/* Analyze VPR context to get ram instance-name -> placement/pb_type info */
std::map<std::string, MifPlacementInfo> get_instance_info_from_placement();

} /* namespace openfpga */
