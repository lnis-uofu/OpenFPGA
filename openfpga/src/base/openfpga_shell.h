#ifndef OPENFPGA_SHELL_H
#define OPENFPGA_SHELL_H

#include <string>

#include "openfpga_context.h"
#include "shell.h"

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

 private: /* Internal data */
  openfpga::Shell<OpenfpgaContext> shell_;
  OpenfpgaContext openfpga_ctx_;
};

#endif
