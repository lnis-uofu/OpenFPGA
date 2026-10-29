#include "openfpga_shell.h"

#include <cctype>
#include <exception>

#include "app_options_commands.h"
#include "basic_command.h"
#include "command_echo.h"
#include "command_exit_codes.h"
#include "command_parser.h"
#include "openfpga_bitstream_command.h"
#include "openfpga_context.h"
#include "openfpga_sdc_command.h"
#include "openfpga_setup_command.h"
#include "openfpga_spice_command.h"
#include "openfpga_title.h"
#include "openfpga_verilog_command.h"
#include "output_capture.h"
#ifdef OPENFPGA_INCLUDE_YOSYS_COMMAND
#include "openfpga_yosys_command.h"
#endif
#include "stage_commands.h"
#include "vpr_command.h"
#include "vtr_log.h"

OpenfpgaShell::OpenfpgaShell() {
  shell_.set_name("OpenFPGA");
  shell_.add_title(create_openfpga_title().c_str());

  /* Add vpr commands */
  openfpga::add_vpr_commands(shell_);

#ifdef OPENFPGA_INCLUDE_YOSYS_COMMAND
  /* Add yosys commands */
  openfpga::add_openfpga_yosys_commands(shell_);
#endif

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

  /* Add commands to set and report the application options */
  openfpga::add_app_options_commands(shell_);

  /* Add the commands of the flow that is run stage by stage */
  openfpga::add_stage_commands(shell_);

  /* Add basic commands: exit, help, etc.
   * Note:
   * This MUST be the last command group to be added!
   */
  openfpga::add_basic_commands(shell_);
}

/* Return true if the command line calls the built-in command 'exit' */
static bool is_exit_command(const char* cmd_line) {
  const char* cur = cmd_line;
  while (*cur != '\0' && std::isspace(static_cast<unsigned char>(*cur))) {
    ++cur;
  }
  std::string name;
  while (*cur != '\0' && !std::isspace(static_cast<unsigned char>(*cur))) {
    name.push_back(*cur);
    ++cur;
  }
  return name == "exit";
}

int OpenfpgaShell::execute_guarded(const char* cmd_line, std::string& error) {
  if (nullptr == cmd_line) {
    error = "Empty command line";
    VTR_LOG_ERROR("%s\n", error.c_str());
    return openfpga::CMD_EXEC_FATAL_ERROR;
  }

  try {
    return shell_.execute_command(cmd_line, openfpga_ctx_);
  } catch (const std::exception& exc) {
    error = exc.what();
  } catch (...) {
    error = "Unknown exception";
  }
  VTR_LOG_ERROR("%s\n", error.c_str());
  return openfpga::CMD_EXEC_FATAL_ERROR;
}

int OpenfpgaShell::run_command(const char* cmd_line) {
  std::string error;
  return execute_guarded(cmd_line, error);
}

CommandResult OpenfpgaShell::execute(const char* cmd_line) {
  CommandResult result;

  if (nullptr != cmd_line && is_exit_command(cmd_line)) {
    result.status = openfpga::CMD_EXEC_FATAL_ERROR;
    result.error =
      "The command 'exit' is not available here: it would terminate the host "
      "process";
    return result;
  }

  openfpga::OutputCapture capture;
  result.status = execute_guarded(cmd_line, result.error);
  result.log = capture.stop();
  return result;
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
