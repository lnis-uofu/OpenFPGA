/********************************************************************
 * Add basic commands to the OpenFPGA shell interface, including:
 * - exit
 * - version
 * - help
 *******************************************************************/
#include "openfpga_basic.h"

#include "command_exit_codes.h"
#include "openfpga_title.h"

/* begin namespace openfpga */
namespace openfpga {

int source_existing_command(openfpga::Shell<OpenfpgaContext>* shell,
                            OpenfpgaContext& openfpga_ctx, const Command& cmd,
                            const CommandContext& cmd_context) {
  CommandOptionId opt_file = cmd.option("from_file");
  CommandOptionId opt_batch_mode = cmd.option("batch_mode");
  CommandOptionId opt_ss = cmd.option("command_stream");

  bool is_cmd_file = cmd_context.option_enable(cmd, opt_file);
  std::string cmd_ss = cmd_context.option_value(cmd, opt_ss);

  int status = CMD_EXEC_SUCCESS;

  /* If a file is specified, run script mode of the shell, otherwise,  */
  if (is_cmd_file) {
    shell->run_script_mode(cmd_ss.c_str(), openfpga_ctx,
                           cmd_context.option_enable(cmd, opt_batch_mode));
  } else {
    /* Split the string with ';' and run each command */
    /* Remove the space at the end of the line
     * So that we can check easily if there is a continued line in the end
     */
    StringToken cmd_ss_tokenizer(cmd_ss);

    for (std::string cmd_part : cmd_ss_tokenizer.split(";")) {
      StringToken cmd_part_tokenizer(cmd_part);
      cmd_part_tokenizer.rtrim(std::string(" "));
      std::string single_cmd_line = cmd_part_tokenizer.data();

      if (!single_cmd_line.empty()) {
        status = shell->execute_command(single_cmd_line.c_str(), openfpga_ctx);

        /* Check the execution status of the command,
         * if fatal error happened, we should abort immediately
         */
        if (CMD_EXEC_FATAL_ERROR == status) {
          return CMD_EXEC_FATAL_ERROR;
        }
      }
    }
  }

  return CMD_EXEC_SUCCESS;
}

/** Call an external command using system call */
int call_external_command(const Command& cmd,
                          const CommandContext& cmd_context) {
  CommandOptionId opt_ss = cmd.option("command");

  std::string cmd_ss = cmd_context.option_value(cmd, opt_ss);

  /* First test if command processor is available and then execute */
  if (!system(NULL)) {
    VTR_LOG("Processer is not available");
    return CMD_EXEC_FATAL_ERROR;
  }

  // Refer https://pubs.opengroup.org/onlinepubs/009695399/functions/system.html
  // Refer
  // https://pubs.opengroup.org/onlinepubs/009695399/functions/waitpid.html
  int status = system(cmd_ss.c_str());
  if (WIFEXITED(status)) {
    // This is normal program exit, WEXITSTATUS() will help you shift the status
    // accordingly (status >> 8)
    // Becareful if the final status is 2 or beyond, program will not error
    // as it is treated as CMD_EXEC_MINOR_ERROR
    return WEXITSTATUS(status);
  }
  // Program maybe terminated because of various killed or stopped signal
  return CMD_EXEC_FATAL_ERROR;
}

} /* end namespace openfpga */
