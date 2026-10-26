#ifndef OPENFPGA_SHELL_H
#define OPENFPGA_SHELL_H

#include <string>

#include "openfpga_context.h"
#include "shell.h"

/********************************************************************
 * Outcome of a command executed through OpenfpgaShell::execute()
 * - status: 0 when the command succeeds, otherwise a non-zero code
 *   (1: fatal error, 2: minor error)
 * - log: everything the command printed on stdout and stderr
 * - error: message of the exception that stopped the command, if any.
 *   Empty when the command ended without throwing
 *******************************************************************/
struct CommandResult {
  int status = 0;
  std::string log;
  std::string error;
};

/********************************************************************
 * This is a shell class built on top of the general-purpose shell
 * It is dedicated for the usage of OpenFPGA engines
 * - It includes the data storage required
 * - It includes all the commands available in OpenFPGA
 *******************************************************************/
class OpenfpgaShell {
 public: /* Contructors */
  OpenfpgaShell();

 public: /* Mutators */
  /* Execute a specific command with options in a line, which is available in
   * the shell Note that running a command will be based on the current status
   * of data storage. The data storage is impacted by previous commands that
   * have been run. If you want a clear start, please call API reset() before
   * running a command. Running a command may cause modification to data
   * storage. */
  int run_command(const char* cmd_line);

  /* Same as run_command(), but returns the output of the command together
   * with its status, instead of printing it. Intended for embedding, where
   * the caller needs the output of each command separately.
   * Note that capturing is process-wide, see OutputCapture.
   * The command 'exit' is refused, as it would terminate the host process.
   * Exceptions raised by a command never leave this function: they are
   * reported in the error field, with a fatal status. */
  CommandResult execute(const char* cmd_line);

  /* Start the shell with options. For detailed usage, please refer to online
   * documentation about openfpgashell Depending on the options, a shell may run
   * in
   * - an interactive mode, where users can type in commands and get results
   * or
   * - an batch mode, where users provide a script contains a set of commands to
   * execute
   * TODO: the shell always has a clean start (refreshed data storage).
   */
  int start(int argc, char** argv);

  /* Reset the data storage and shell status, to ensure a clean start */
  void reset();

 private: /* Internal functions */
  /* Run a command and turn any exception into a fatal status, whose message
   * is reported through error. The message is also printed as an error */
  int execute_guarded(const char* cmd_line, std::string& error);

 private: /* Internal data */
  openfpga::Shell<OpenfpgaContext> shell_;
  OpenfpgaContext openfpga_ctx_;
};

#endif
