#ifndef VPR_SHELL_UTILS_H
#define VPR_SHELL_UTILS_H

#include <string>
#include <vector>

#include "openfpga_context.h"
#include "shell.h"
#include "vpr_types.h"

namespace vpr {

void sync_vpr_setup_to_app_options(
	t_vpr_setup& vpr_setup, openfpga::Shell<OpenfpgaContext>& shell);

void shell_setup_packer_opts(t_vpr_setup* vpr_setup,
                             openfpga::Shell<OpenfpgaContext>* shell);

int validate_vpr_arch_types(
	const std::string& arch_file_name,
	const std::vector<t_physical_tile_type>& physical_tile_types,
	const std::vector<t_logical_block_type>& logical_block_types);

}  // namespace vpr

#endif
