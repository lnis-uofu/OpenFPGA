#pragma once

#include <cstddef>
#include <string>

namespace openfpga {

/* Resolve the Nth AtomBlock carrying the requested eblif parameter to its
 * packed pb_type. VPR pack must already be complete. */
std::string get_mif_pb_type_from_vpr(const std::string& param_name,
                                     size_t param_index);

} /* namespace openfpga */
