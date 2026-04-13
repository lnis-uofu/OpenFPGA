#include "vpr_shell_utils.h"

#include <algorithm>

#include "command_exit_codes.h"
#include "openfpga_context.h"
#include "shell.h"
#include "ShowSetup.h"
#include "vpr_types.h"
#include "vtr_log.h"

namespace vpr {

void sync_vpr_setup_to_app_options(
  t_vpr_setup& vpr_setup, openfpga::Shell<OpenfpgaContext>& shell) {
  vpr_setup.TimingEnabled =
    shell.app_options_.general.timing_analysis.bool_value;
  vpr_setup.device_layout =
    shell.app_options_.general.device_layout.string_value;
  vpr_setup.constant_net_method = static_cast<e_constant_net_method>(
    shell.app_options_.general.constant_net_method.to_enum());
  vpr_setup.clock_modeling = static_cast<e_clock_modeling>(
    shell.app_options_.general.clock_modeling.to_enum());
  vpr_setup.two_stage_clock_routing =
    shell.app_options_.general.two_stage_clock_routing.bool_value;
  vpr_setup.exit_before_pack = false;
  vpr_setup.num_workers = shell.app_options_.general.num_workers.int_value;

  /* = = = = = = = = = = = = = = = = = =
   * TODO: Separate supress warning and errors to different options, and support
   * more flexible configuration (e.g. specify a log file to output the
   * supressed warnings/errors)
   * = = = = = = = = = = = = = = = = = = */
  /*
   * Initialize the functions names for which VPR_ERRORs
   * are demoted to VTR_LOG_WARNs
   */
  for (const std::string& func_name :
       vtr::StringToken(shell.app_options_.general.disable_errors.string_value)
         .split(":")) {
    map_error_activation_status(func_name);
  }

  /*
   * Initialize the functions names for which
   * warnings are being suppressed
   */
  std::vector<std::string> split_warning_option =
    vtr::StringToken(shell.app_options_.general.suppress_warnings.string_value)
      .split(",");
  std::string warn_log_file;
  std::string warn_functions;
  // If no log file name is provided, the specified warning
  // to suppress are not output anywhere.
  if (split_warning_option.size() == 1) {
    warn_functions = split_warning_option[0];
  } else if (split_warning_option.size() == 2) {
    warn_log_file = split_warning_option[0];
    warn_functions = split_warning_option[1];
  }
  /* = = = = = = = = = = = = = = = = = = */
  setupvpr_from_ofshell(&vpr_setup, &shell);
  ShowSetup(vpr_setup);
}

void setupvpr_from_ofshell(
  t_vpr_setup* vpr_setup, openfpga::Shell<OpenfpgaContext>* shell) {
  // Sync the options in VPR setup to the app options in the shell

  // Setup netlist options
  auto NetlistOpts = vpr_setup->NetlistOpts;
  auto ShellNetlistOpts = shell->app_options_.atom_netlist;
  auto PackerOpts = vpr_setup->PackerOpts;
  auto ShellPackerOpts = shell->app_options_.clustering;
  auto ShellGeneralOpts = shell->app_options_.general;
  auto ShellFilenameOpts = shell->app_options_.filename;

  // Filenames
  vpr_setup->FileNameOpts.NetFile = ShellFilenameOpts.net_file.string_value;
  vpr_setup->FileNameOpts.CircuitFile = ShellFilenameOpts.circuit_file.string_value;

  vpr_setup->RoutingArch.write_rr_graph_filename =
    ShellFilenameOpts.write_rr_graph_file.string_value;
  vpr_setup->RoutingArch.read_rr_graph_filename =
    ShellFilenameOpts.read_rr_graph_file.string_value;
  vpr_setup->RoutingArch.read_rr_edge_override_filename =
    ShellFilenameOpts.read_rr_edge_override_file.string_value;

  // init global variables

  NetlistOpts.const_gen_inference = static_cast<e_const_gen_inference>(
    ShellNetlistOpts.const_gen_inference.to_enum());
  NetlistOpts.absorb_buffer_luts =
    ShellNetlistOpts.absorb_buffer_luts.bool_value;
  NetlistOpts.sweep_dangling_primary_ios =
    ShellNetlistOpts.sweep_dangling_primary_ios.bool_value;
  NetlistOpts.sweep_dangling_nets =
    ShellNetlistOpts.sweep_dangling_nets.bool_value;
  NetlistOpts.sweep_dangling_blocks =
    ShellNetlistOpts.sweep_dangling_blocks.bool_value;
  NetlistOpts.sweep_constant_primary_outputs =
    ShellNetlistOpts.sweep_constant_primary_outputs.bool_value;

  // Setup packer options
  PackerOpts.output_file = ShellFilenameOpts.net_file.string_value;
  PackerOpts.circuit_file_name = ShellFilenameOpts.circuit_file.string_value;
  PackerOpts.doPacking = e_stage_action::DO;

  PackerOpts.allow_unrelated_clustering = static_cast<e_unrelated_clustering>(
    ShellPackerOpts.allow_unrelated_clustering.to_enum());
  PackerOpts.connection_driven =
    ShellPackerOpts.connection_driven_clustering.bool_value;
  PackerOpts.timing_driven =
    ShellPackerOpts.timing_driven_clustering.bool_value;
  PackerOpts.cluster_seed_type =
    static_cast<e_cluster_seed>(ShellPackerOpts.cluster_seed_type.to_enum());
  PackerOpts.timing_gain_weight =
    ShellPackerOpts.timing_gain_weight.float_value;
  PackerOpts.connection_gain_weight =
    ShellPackerOpts.connection_gain_weight.float_value;
  PackerOpts.pack_verbosity = 0;
  PackerOpts.memoize_cluster_packings =
    ShellPackerOpts.memoize_cluster_packings.bool_value;
  PackerOpts.enable_pin_feasibility_filter =
    ShellPackerOpts.enable_clustering_pin_feasibility_filter.bool_value;
  PackerOpts.balance_block_type_utilization =
    static_cast<e_balance_block_type_util>(
      ShellPackerOpts.balance_block_type_utilization.to_enum());
  PackerOpts.target_external_pin_util =
    vtr::StringToken(ShellPackerOpts.target_external_pin_util.string_value)
      .split(" ");
  PackerOpts.target_device_utilization =
    ShellGeneralOpts.target_device_utilization.float_value;
  PackerOpts.prioritize_transitive_connectivity =
    ShellPackerOpts.pack_prioritize_transitive_connectivity.bool_value;
  PackerOpts.high_fanout_threshold =
    vtr::StringToken(ShellPackerOpts.pack_high_fanout_threshold.string_value)
      .split(" ");
  PackerOpts.transitive_fanout_threshold =
    ShellPackerOpts.pack_transitive_fanout_threshold.int_value;
  PackerOpts.feasible_block_array_size =
    ShellPackerOpts.pack_feasible_block_array_size.int_value;

  PackerOpts.device_layout = ShellGeneralOpts.device_layout.string_value;

  PackerOpts.timing_update_type = static_cast<e_timing_update_type>(
    ShellGeneralOpts.timing_update_type.to_enum());
}

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
