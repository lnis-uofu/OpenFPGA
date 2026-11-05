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
            errors = self._error_lines(lines)
            detail = "\n".join(errors or lines[-self.LOG_TAIL_LINES:])
        message = "Command '%s' failed with status %d" % (command, self.status)
        if detail:
            message += ":\n" + detail
        super().__init__(message)

    # Lines of a VPR error after "Error <n>:" (Type, File, Line, Message)
    VPR_ERROR_LINES = 4

    @classmethod
    def _error_lines(cls, lines):
        """The lines that start with "Error ". VPR writes its errors over
        several lines, with "Error <n>:" alone and the text on a "Message:"
        line after it: that text is added to the error line."""
        errors = []
        for index, line in enumerate(lines):
            if not line.startswith("Error "):
                continue
            if line.rstrip().endswith(":"):
                following = lines[index + 1:index + 1 + cls.VPR_ERROR_LINES]
                for next_line in following:
                    if next_line.startswith("Message:"):
                        line = line.rstrip() + " " + next_line[len("Message:"):].strip()
                        break
            errors.append(line)
        return errors


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
