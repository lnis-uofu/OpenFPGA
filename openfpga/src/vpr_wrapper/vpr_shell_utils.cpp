#include "vpr_shell_utils.h"

#include <algorithm>

#include "command_exit_codes.h"
#include "vtr_log.h"

namespace vpr {

int validate_vpr_arch_types(
	const std::string& arch_file_name,
	const std::vector<t_physical_tile_type>& physical_tile_types,
	const std::vector<t_logical_block_type>& logical_block_types) {
	int num_inputs = 0;
	int num_outputs = 0;
	bool has_empty_physical_tile = false;
	for (const t_physical_tile_type& type : physical_tile_types) {
		if (type.is_empty()) {
			has_empty_physical_tile = true;
		}

		if (type.is_input_type) {
			++num_inputs;
		}

		if (type.is_output_type) {
			++num_outputs;
		}
	}

	bool has_empty_logical_block = false;
	int max_equivalent_tiles = 0;
	for (const t_logical_block_type& type : logical_block_types) {
		if (type.is_empty()) {
			has_empty_logical_block = true;
		}

		max_equivalent_tiles =
			std::max(max_equivalent_tiles, static_cast<int>(type.equivalent_tiles.size()));
	}

	if (!has_empty_physical_tile || !has_empty_logical_block) {
		VTR_LOG_ERROR(
			"Invalid VPR architecture '%s': missing empty physical/logical block type.\n",
			arch_file_name.c_str());
		return openfpga::CMD_EXEC_FATAL_ERROR;
	}

	if (max_equivalent_tiles <= 0) {
		VTR_LOG_ERROR("Invalid VPR architecture '%s': no equivalent tiles found.\n",
									arch_file_name.c_str());
		return openfpga::CMD_EXEC_FATAL_ERROR;
	}

	if (num_inputs == 0) {
		VTR_LOG_ERROR(
			"Invalid VPR architecture '%s': no top-level block type contains '.input' models.\n",
			arch_file_name.c_str());
		return openfpga::CMD_EXEC_FATAL_ERROR;
	}

	if (num_outputs == 0) {
		VTR_LOG_ERROR(
			"Invalid VPR architecture '%s': no top-level block type contains '.output' models.\n",
			arch_file_name.c_str());
		return openfpga::CMD_EXEC_FATAL_ERROR;
	}

	return openfpga::CMD_EXEC_SUCCESS;
}

}  // namespace vpr
