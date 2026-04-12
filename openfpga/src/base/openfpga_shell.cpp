#include "openfpga_shell.h"

#include <map>

#include "ShowSetup.h"
#include "app_options_commands.h"
#include "basic_command.h"
#include "command_echo.h"
#include "command_parser.h"
#include "openfpga_bitstream_command.h"
#include "openfpga_context.h"
#include "openfpga_sdc_command.h"
#include "openfpga_setup_command.h"
#include "openfpga_spice_command.h"
#include "openfpga_title.h"
#include "openfpga_verilog_command.h"
#include "vpr_command.h"
#include "yosys_command.h"

OpenfpgaShell::OpenfpgaShell() {
  sync_vpr_setup_to_app_options(openfpga_ctx_.mutable_vpr_setup(), shell_);
  ShowSetup(openfpga_ctx_.mutable_vpr_setup());

  shell_.set_name("OpenFPGA");
  shell_.add_title(create_openfpga_title().c_str());

  /* Add vpr commands */
  openfpga::add_vpr_commands(shell_);

  /* Add yosys commands */
  openfpga::add_yosys_commands(shell_);

  /* Add openfpga setup commands */
  openfpga::add_openfpga_setup_commands(shell_);

  /* Add openfpga verilog commands */
  openfpga::add_openfpga_verilog_commands(shell_);

  /* Add openfpga bitstream commands */
  openfpga::add_openfpga_bitstream_commands(shell_);

  /* Add openfpga SPICE commands */
  openfpga::add_openfpga_spice_commands(shell_);

  /* Add openfpga sdc commands */
  openfpga::add_openfpga_sdc_commands(shell_);

  // Add app options commands
  openfpga::add_app_options_commands(shell_);

  /* Add basic commands: exit, help, etc.
   * Note:
   * This MUST be the last command group to be added!
   */
  openfpga::add_basic_commands(shell_);
}

void OpenfpgaShell::sync_vpr_setup_to_app_options(
  t_vpr_setup vpr_setup, openfpga::Shell<OpenfpgaContext>& shell) {

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
  setupvpr_from_ofshell(&vpr_setup);
}

void OpenfpgaShell::setupvpr_from_ofshell(t_vpr_setup* vpr_setup) {
  // Sync the options in VPR setup to the app options in the shell

  // Setup netlist options
  auto NetlistOpts = vpr_setup->NetlistOpts;
  auto ShellNetlistOpts = shell_.app_options_.atom_netlist;
  auto PackerOpts = vpr_setup->PackerOpts;
  auto ShellPackerOpts = shell_.app_options_.clustering;
  auto ShellGeneralOpts = shell_.app_options_.general;

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
  PackerOpts.output_file = ShellPackerOpts.net_file.string_value;
  PackerOpts.circuit_file_name = ShellPackerOpts.circuit_file.string_value;
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
  PackerOpts.pack_verbosity = ShellPackerOpts.pack_verbosity.int_value;
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

int OpenfpgaShell::run_command(const char* cmd_line) {
  return shell_.execute_command(cmd_line, openfpga_ctx_);
}

void OpenfpgaShell::reset() {
  /* TODO: reset the shell status */
  /* TODO: reset the data storage */
}

int OpenfpgaShell::start(int argc, char** argv) {
  reset();

  /* Create the command to launch shell in different modes */
  openfpga::Command start_cmd("OpenFPGA");
  /* Add options to openfpga shell interface */
  /* '--interactive', -i': launch the interactive mode */
  openfpga::CommandOptionId opt_interactive = start_cmd.add_option(
    "interactive", false, "Launch OpenFPGA in interactive mode");
  start_cmd.set_option_short_name(opt_interactive, "i");

  /* '--file', -f': launch the script mode */
  openfpga::CommandOptionId opt_script_mode =
    start_cmd.add_option("file", false, "Launch OpenFPGA in script mode");
  start_cmd.set_option_require_value(opt_script_mode, openfpga::OPT_STRING);
  start_cmd.set_option_short_name(opt_script_mode, "f");

  /* '--execute', -x': execute command line(s), separated by ';' */
  openfpga::CommandOptionId opt_exec_mode = start_cmd.add_option(
    "execute", false, "Execute OpenFPGA command line(s), separated by ';'");
  start_cmd.set_option_require_value(opt_exec_mode, openfpga::OPT_STRING);
  start_cmd.set_option_short_name(opt_exec_mode, "x");

  /* '--batch_execution': execute the script in batch mode.
   * Will exit immediately when fatal errors occurred
   */
  openfpga::CommandOptionId opt_batch_exec =
    start_cmd.add_option("batch_execution", false,
                         "Launch OpenFPGA in batch  mode when running scripts");
  start_cmd.set_option_short_name(opt_batch_exec, "batch");

  /* '--version', -v': print version information */
  openfpga::CommandOptionId opt_version =
    start_cmd.add_option("version", false, "Show OpenFPGA version");
  start_cmd.set_option_short_name(opt_version, "v");

  /* '--help', -h': print help desk */
  openfpga::CommandOptionId opt_help =
    start_cmd.add_option("help", false, "Help desk");
  start_cmd.set_option_short_name(opt_help, "h");

  /* Parse the option, to avoid issues, we use the command name to replace the
   * argv[0] */
  std::vector<std::string> cmd_opts;
  cmd_opts.push_back(start_cmd.name());
  for (int iarg = 1; iarg < argc; ++iarg) {
    cmd_opts.push_back(std::string(argv[iarg]));
  }

  openfpga::CommandContext start_cmd_context(start_cmd);
  if (false == parse_command(cmd_opts, start_cmd, start_cmd_context)) {
    /* Parse fail: Echo the command */
    openfpga::print_command_options(start_cmd);
  } else {
    /* Parse succeed. Branch on options */
    /* Show version */
    if (true == start_cmd_context.option_enable(start_cmd, opt_version)) {
      print_openfpga_version_info();
      return 0;
    }
    /* Start a shell */
    if (true == start_cmd_context.option_enable(start_cmd, opt_interactive)) {
      shell_.run_interactive_mode(openfpga_ctx_);
      return shell_.exit_code();
    }

    if (true == start_cmd_context.option_enable(start_cmd, opt_exec_mode)) {
      shell_.run_execute_mode(
        start_cmd_context.option_value(start_cmd, opt_exec_mode).c_str(),
        openfpga_ctx_);
      return shell_.exit_code();
    }

    if (true == start_cmd_context.option_enable(start_cmd, opt_script_mode)) {
      shell_.run_script_mode(
        start_cmd_context.option_value(start_cmd, opt_script_mode).c_str(),
        openfpga_ctx_,
        start_cmd_context.option_enable(start_cmd, opt_batch_exec));
      return shell_.exit_code();
    }
    /* Reach here there is something wrong, show the help desk */
    openfpga::print_command_options(start_cmd);
  }

  /* Reach here, it means shell execution has critical errors.
   * Return a code with fatal errors
   */
  return 1;
}
