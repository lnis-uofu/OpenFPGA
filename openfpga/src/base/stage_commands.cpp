#include "stage_commands.h"

#include "command_exit_codes.h"
#include "staged_flow.h"

/* Begin namespace openfpga */
namespace openfpga {

namespace {

int read_vpr_arch_command(OpenfpgaContext& openfpga_ctx, const Command& cmd,
                          const CommandContext& cmd_context) {
  const std::string file = cmd_context.option_value(cmd, cmd.option("file"));
  const std::string layout =
    cmd_context.option_value(cmd, cmd.option("layout"));
  if (!read_vpr_arch_input(openfpga_ctx.mutable_session(), file, layout)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

int read_circuit_command(OpenfpgaContext& openfpga_ctx, const Command& cmd,
                         const CommandContext& cmd_context) {
  const std::string file = cmd_context.option_value(cmd, cmd.option("file"));
  if (!read_circuit_input(openfpga_ctx.mutable_session(), file)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

int run_stage_command(OpenfpgaContext& openfpga_ctx,
                      const OpenFpgaStage& stage) {
  if (!run_flow_stage(openfpga_ctx.mutable_session(), stage)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

int pack_command(OpenfpgaContext& openfpga_ctx, const Command&,
                 const CommandContext&) {
  return run_stage_command(openfpga_ctx, OpenFpgaStage::PACKED);
}

int place_command(OpenfpgaContext& openfpga_ctx, const Command&,
                  const CommandContext&) {
  return run_stage_command(openfpga_ctx, OpenFpgaStage::PLACED);
}

int route_command(OpenfpgaContext& openfpga_ctx, const Command&,
                  const CommandContext&) {
  return run_stage_command(openfpga_ctx, OpenFpgaStage::ROUTED);
}

int analysis_command(OpenfpgaContext& openfpga_ctx, const Command&,
                     const CommandContext&) {
  return run_stage_command(openfpga_ctx, OpenFpgaStage::ANALYZED);
}

/* A command of a stage, which has no option: the settings are the
 * application options */
ShellCommandId add_stage_command(openfpga::Shell<OpenfpgaContext>& shell,
                                 const ShellCommandClassId& cmd_class,
                                 const char* name, const char* description,
                                 int (*exec)(OpenfpgaContext&, const Command&,
                                             const CommandContext&)) {
  Command shell_cmd(name);
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, description);
  shell.set_command_class(shell_cmd_id, cmd_class);
  shell.set_command_execute_function(shell_cmd_id, exec);
  return shell_cmd_id;
}

}  // End anonymous namespace

void add_stage_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  ShellCommandClassId cmd_class = shell.add_command_class("Flow stages");

  /* Command 'read_vpr_arch' */
  Command shell_cmd_arch("read_vpr_arch");
  CommandOptionId arch_opt_file = shell_cmd_arch.add_option(
    "file", true, "Path to the VPR architecture file");
  shell_cmd_arch.set_option_short_name(arch_opt_file, "f");
  shell_cmd_arch.set_option_require_value(arch_opt_file, openfpga::OPT_STRING);
  CommandOptionId arch_opt_layout = shell_cmd_arch.add_option(
    "layout", true,
    "Name of the fixed layout of the device, as in the architecture file");
  shell_cmd_arch.set_option_short_name(arch_opt_layout, "l");
  shell_cmd_arch.set_option_require_value(arch_opt_layout,
                                          openfpga::OPT_STRING);
  ShellCommandId shell_cmd_arch_id = shell.add_command(
    shell_cmd_arch,
    "Read and check the VPR architecture file, and start the flow over");
  shell.set_command_class(shell_cmd_arch_id, cmd_class);
  shell.set_command_execute_function(shell_cmd_arch_id, read_vpr_arch_command);

  /* Command 'read_circuit' */
  Command shell_cmd_circuit("read_circuit");
  CommandOptionId circuit_opt_file =
    shell_cmd_circuit.add_option("file", true, "Path to the circuit netlist");
  shell_cmd_circuit.set_option_short_name(circuit_opt_file, "f");
  shell_cmd_circuit.set_option_require_value(circuit_opt_file,
                                             openfpga::OPT_STRING);
  ShellCommandId shell_cmd_circuit_id = shell.add_command(
    shell_cmd_circuit, "Read the circuit netlist against the VPR architecture");
  shell.set_command_class(shell_cmd_circuit_id, cmd_class);
  shell.set_command_execute_function(shell_cmd_circuit_id,
                                     read_circuit_command);

  add_stage_command(shell, cmd_class, "pack",
                    "Pack the circuit into the blocks of the device",
                    pack_command);
  add_stage_command(shell, cmd_class, "place",
                    "Place the packed circuit on the device", place_command);
  add_stage_command(shell, cmd_class, "route",
                    "Route the placed circuit on the device", route_command);
  ShellCommandId analysis_cmd_id = add_stage_command(
    shell, cmd_class, "analysis",
    "Analyze the routed circuit: timing, power and the synchronization of the "
    "packing with the routing",
    analysis_command);

  /* The commands that follow 'vpr' follow the stages as well: 'analysis' makes
   * the data that 'vpr' makes. The stages that follow are checked by the
   * session, see require_stage() */
  shell.add_command_alternative(shell.command("vpr"), analysis_cmd_id);
}

} /* End namespace openfpga */
