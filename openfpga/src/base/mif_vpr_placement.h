#pragma once

#include <map>
#include <string>

namespace openfpga {

/* Map Verilog atom instance name -> index-level pb_type path from VPR
 * placement. */
std::map<std::string, std::string> get_instance_pb_type_path_from_placement();

} /* namespace openfpga */
