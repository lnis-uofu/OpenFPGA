#ifndef COMMAND_EXIT_CODES_H
#define COMMAND_EXIT_CODES_H

/* Begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Exit codes to signal success/failure status of command execution
 * Depend on the exit code, OpenFPGA shell will decide to continue or abort
 ********************************************************************/

/* Never been executed */
constexpr int CMD_EXEC_NONE = -1;

/* Everything OK */
constexpr int CMD_EXEC_SUCCESS = 0;

/* Fatal error occurred, we have to abort and do not execute the rest of commands */
constexpr int CMD_EXEC_FATAL_ERROR = 1;

/* See minor errors but it will not impact the downsteam. We can continue to execute the rest of commands */
constexpr int CMD_EXEC_MINOR_ERROR = 2;


} /* End namespace openfpga */

#endif
