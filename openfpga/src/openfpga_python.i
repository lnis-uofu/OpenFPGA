/* SWIG interface file for OpenFPGA shell APIs (Python) */
%module openfpga

%include <std_string.i>
%include <exception.i>

%{
#include <exception>

#include "openfpga_shell.h"
%}

/* Never let a C++ exception unwind into the interpreter. Commands already
 * guard their own exceptions, this catches anything raised elsewhere */
%exception {
  try {
    $action
  } catch (const std::exception& exc) {
    SWIG_exception(SWIG_RuntimeError, exc.what());
  } catch (...) {
    SWIG_exception(SWIG_UnknownError, "Unknown C++ exception");
  }
}

/* start() takes a C-style argv, which is not usable from Python */
%ignore OpenfpgaShell::start;

%include "openfpga_shell.h"

%pythoncode %{
class OpenfpgaError(RuntimeError):
    """A command run by OpenfpgaShell.run() did not succeed.

    Attributes:
        command: the command line that failed
        status: status code of the command (1: fatal error, 2: minor error)
        log: everything the command printed
        error: message of the exception that stopped the command, if any
    """

    # Number of log lines quoted in the message, when it has no error lines
    LOG_TAIL_LINES = 10

    def __init__(self, command, result):
        self.command = command
        self.status = result.status
        self.log = result.log
        self.error = result.error
        detail = self.error
        if not detail:
            lines = [line for line in self.log.splitlines() if line.strip()]
            # Error messages are not always at the end of the log, as stderr
            # is not ordered against stdout. Quote them first when present
            errors = [line for line in lines if line.startswith("Error ")]
            detail = "\n".join(errors or lines[-self.LOG_TAIL_LINES:])
        message = "Command '%s' failed with status %d" % (command, self.status)
        if detail:
            message += ":\n" + detail
        super().__init__(message)


def _run(self, command, check=True):
    """Run a command and return its CommandResult (status, log, error).

    The output of the command is captured in the result, not printed.
    Raise OpenfpgaError when the command does not succeed, unless check is
    False.
    """
    result = self.execute(command)
    if check and result.status != 0:
        raise OpenfpgaError(command, result)
    return result


OpenfpgaShell.run = _run
%}
