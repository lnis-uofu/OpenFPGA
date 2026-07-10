#pragma once

#include <map>
#include <string>

#include "vpr_types.h"

namespace openfpga {

/* Analyze vpr placement context to get ram model-name -> location map */
std::map<std::string, t_pl_loc> get_instance_info_from_placement();

} /* namespace openfpga */
