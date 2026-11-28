/********************************************************************
 * An example of a plugin of the shell (see openfpga_plugins.h). It
 * - adds the command example_report, which says what the plugin saw;
 * - overrides write_fabric_spice, which then writes nothing;
 * - removes write_sdc_disable_timing_configure_ports;
 * - hooks read_vpr_arch (after: the file it read), read_circuit (before:
 *   it refuses a file whose name has 'refused_by_example') and every
 *   command (after: it counts them).
 * What it saw is kept in a slot of the context (extension()).
 *******************************************************************/
#include <string>

#include "command_exit_codes.h"
#include "openfpga_plugins.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

/* The data of the plugin in the context */
struct ExampleData {
  int commands = 0;
  std::string architecture;
};

/* The value of the option --file of a command, or "" */
std::string file_option(const Command& cmd, const CommandContext& context) {
  const CommandOptionId opt = cmd.option("file");
  if (CommandOptionId::INVALID() == opt || !context.option_enable(cmd, opt)) {
    return std::string();
  }
  return context.option_value(cmd, opt);
}

int example_report(const OpenfpgaContext& openfpga_ctx) {
  const ExampleData* data = openfpga_ctx.find_extension<ExampleData>();
  VTR_LOG("Commands seen by the example plugin: %d\n",
          data ? data->commands : 0);
  VTR_LOG("Architecture read: %s\n", data && !data->architecture.empty()
                                       ? data->architecture.c_str()
                                       : "none");
  return CMD_EXEC_SUCCESS;
}

} /* end anonymous namespace */

/* Declared for the list of the plugins of the build, which calls it */
void register_plugin_example(PluginApi& api);

void register_plugin_example(PluginApi& api) {
  Shell<OpenfpgaContext>& shell = api.shell();

  ShellCommandClassId cmd_class = shell.add_command_class("Example plugin");
  ShellCommandId report = shell.add_command(Command("example_report"),
                                            "Say what the example plugin saw");
  shell.set_command_class(report, cmd_class);
  shell.set_command_const_execute_function(report, example_report);

  ShellCommandId spice = shell.override_command("write_fabric_spice");
  shell.set_command_execute_function(
    spice, [](OpenfpgaContext&, const Command&, const CommandContext&) {
      VTR_LOG("No SPICE netlist is written in this build (plugin example)\n");
      return CMD_EXEC_SUCCESS;
    });

  shell.deregister_command("write_sdc_disable_timing_configure_ports");

  shell.on_after("read_vpr_arch",
                 [](OpenfpgaContext& openfpga_ctx, const Command& cmd,
                    const CommandContext& context, int status) {
                   if (CMD_EXEC_SUCCESS == status) {
                     openfpga_ctx.extension<ExampleData>().architecture =
                       file_option(cmd, context);
                   }
                 });
  shell.on_before("read_circuit", [](OpenfpgaContext&, const Command& cmd,
                                     const CommandContext& context) {
    if (std::string::npos !=
        file_option(cmd, context).find("refused_by_example")) {
      return std::string("the example plugin refuses it");
    }
    return std::string();
  });
  shell.on_after_any(
    [](OpenfpgaContext& openfpga_ctx, const Command&, const CommandContext&,
       int) { ++openfpga_ctx.extension<ExampleData>().commands; });
}

} /* end namespace openfpga */
