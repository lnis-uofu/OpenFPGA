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


import os as _os
import re as _re

_WHITESPACE = _re.compile(r"\s")


def _command_value(label, value):
    """Return a value as it is written on a command line.

    The shell splits the line at spaces, except inside a pair of double
    quotes, and has no escape for a double quote: a value with a space is
    quoted, and a value that cannot be written is refused.
    """
    if isinstance(value, (bool, list, tuple, set, dict)):
        raise TypeError(
            "'%s' takes a string, a number or a path, not %s"
            % (label, type(value).__name__)
        )
    text = _os.fspath(value) if isinstance(value, _os.PathLike) else str(value)
    if not text:
        raise ValueError("'%s' has an empty value" % label)
    if '"' in text or "\n" in text or "\r" in text:
        raise ValueError(
            "'%s' has a value that cannot be written on a command line: %r"
            % (label, text)
        )
    if _WHITESPACE.search(text):
        text = '"%s"' % text
    return text


def _command_line(name, args, options):
    """Return the command line of the command 'name'.

    The positional arguments come first, then every option as '--key value'.
    An option set to True is a switch, and one set to False or None is left
    out.
    """
    words = [name]
    for arg in args:
        words.append(_command_value(name, arg))
    for key, value in options.items():
        if value is None or value is False:
            continue
        words.append("--" + key)
        if value is not True:
            words.append(_command_value(key, value))
    return " ".join(words)


def _shell_getattr(self, name):
    """Run a command of the shell as a method.

    shell.read_circuit(file="a.blif") runs 'read_circuit --file a.blif'. The
    positional arguments are written first, as in shell.vpr(arch, blif,
    pack=True), and the options follow: key=value is '--key value', key=True
    is the switch '--key', and key=False or key=None is left out. The check
    argument is the one of run(): the call raises OpenfpgaError when the
    command fails, unless check is False. It returns the CommandResult.

    Any name is taken for a command, and the shell reports one that does not
    exist when the method is called. Names that start with an underscore are
    not commands.
    """
    if name.startswith("_") or name in ("this", "thisown"):
        raise AttributeError(name)

    def command(*args, check=True, **options):
        return self.run(_command_line(name, args, options), check=check)

    command.__name__ = name
    command.__doc__ = "Run the command '%s' of the shell, see run()" % name
    return command


OpenfpgaShell.__getattr__ = _shell_getattr
%}
