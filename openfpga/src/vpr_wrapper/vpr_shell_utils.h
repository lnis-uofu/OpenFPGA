#ifndef VPR_SHELL_UTILS_H
#define VPR_SHELL_UTILS_H

#include <string>
#include <vector>

#include "physical_types.h"

namespace vpr {

int validate_vpr_arch_types(
	const std::string& arch_file_name,
	const std::vector<t_physical_tile_type>& physical_tile_types,
	const std::vector<t_logical_block_type>& logical_block_types);

}  // namespace vpr

#endif
