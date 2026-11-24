# Tcl over a Session: part of the module 'openfpga', see openfpga_python.i,
# which includes it after the Session and before the worker.
#
# Session.tcl(script) runs a Tcl script in an interpreter that the Session
# keeps, in the process of the Session: in a worker, the script runs where the
# data of VPR is, a script that does not end is stopped by the timeout of the
# call, and its variables and procedures stay for the next script until the
# worker ends. Each command of the OpenFPGA shell is a Tcl command that runs
# through Session.command(), so that what it changes is in the journal as if it
# had been run from Python.
#
# With safe=True the script runs in a safe interpreter (interp create -safe):
# no files, no programs, no exit; the commands of the shell are there except
# those that run other programs or scripts, and a path in their arguments must
# be inside the current directory (the one of the session).
import re as _tcl_re

# The commands of the shell that are not Tcl commands: exit would end the
# process that runs the Session
_TCL_LEFT_OUT = frozenset(("exit",))
# The commands that a safe interpreter does not have: they run another program,
# a script of the shell, or a script of Yosys (which can run programs)
_TCL_SAFE_REFUSED = frozenset(("exit", "ext_exec", "source", "yosys"))
# What a script prints is kept up to this size: the end of it
_TCL_OUTPUT_MAX = 1 << 20
# In a safe interpreter, the pieces of an argument that are checked as paths
_TCL_PATH_SPLIT = _tcl_re.compile(r"[,;:\s]+")

_TCL_SETUP = r"""
namespace eval ::openfpga {}

proc ::openfpga::_call {name safe args} {
    lassign [::openfpga::_run $name $safe {*}$args] status value code
    if {$status eq "ok"} {
        return $value
    }
    return -code error -errorcode [list OPENFPGA $code] $value
}

# puts to stdout and stderr is the output of the script; to another channel it
# is the one of Tcl. In a safe interpreter only stdout and stderr exist
proc ::openfpga::_puts {safe args} {
    set newline 1
    if {[lindex $args 0] eq "-nonewline"} {
        set newline 0
        set args [lrange $args 1 end]
    }
    switch [llength $args] {
        1 {
            set channel stdout
            set text [lindex $args 0]
        }
        2 {
            lassign $args channel text
        }
        default {
            return -code error {wrong # args: should be "puts ?-nonewline? ?channelId? string"}
        }
    }
    if {$channel in {stdout stderr}} {
        ::openfpga::_write $text $newline
        return
    }
    if {$safe} {
        return -code error "can not find channel named \"$channel\""
    }
    if {$newline} {
        ::openfpga::_tcl_puts $channel $text
    } else {
        ::openfpga::_tcl_puts -nonewline $channel $text
    }
}

rename ::puts ::openfpga::_tcl_puts
interp alias {} ::puts {} ::openfpga::_puts 0

# The process is the one of the session: exit would end it, and the files of
# the session are in its directory
proc ::exit {args} {
    return -code error "exit is not available: it would end the worker of the session"
}
proc ::cd {args} {
    return -code error "cd is not available: the commands run in the directory of the session"
}
"""


class TclError(RuntimeError):
    """A Tcl script that failed: the error that Tcl reported.

    Attributes:
        output: what the script printed before it failed (puts, the logs of
            the commands of the shell)
        printed: what puts printed, without the logs
        error_info: the trace of the error, as Tcl writes it (errorInfo)
        commands: the command lines of the shell that the script ran
        causes: the diagnostics of the command of the shell that failed, when
            the error comes from one (see OpenfpgaError.diagnostics)
    """

    kind = "tcl_error"
    hint = None

    def __init__(self, message, output="", error_info="", commands=(), causes=(), printed=""):
        super().__init__(message)
        self.output = output
        self.printed = printed
        self.error_info = error_info
        self.commands = list(commands)
        self.causes = list(causes)

    @property
    def diagnostics(self):
        """The error of Tcl, with the line of the script, then the diagnostics
        of the command that failed, if it was one"""
        where = {"where": _tcl_where(self.error_info)}
        return [Diagnostic("error", self.kind, str(self), hint=self.hint, data=where)] + self.causes


class TclRefused(TclError):
    """A safe interpreter refused a command or an argument of the shell"""

    kind = "tcl_refused"


@dataclasses.dataclass(frozen=True)
class TclResult:
    """A Tcl script that ended: its result (the value of its last command, as
    Tcl writes it), what it printed (output: puts and the logs of the commands
    of the shell, in their order; printed: puts alone), the command lines of
    the shell that it ran, and its time in seconds"""

    result: str
    output: str
    commands: List[str]
    elapsed_s: float
    printed: str = ""


def _tcl_where(error_info):
    """The line of the trace of Tcl that says where the error is in the script"""
    for line in (error_info or "").splitlines():
        found = _tcl_re.search(r"\(script line (\d+)\)", line)
        if found:
            return int(found.group(1))
    return None


def _to_tcl(tcl, value):
    """The answer of a query as a Tcl value: an object is a dict, a list is a
    list, true and false are 1 and 0, and null is the empty string"""
    if isinstance(value, dict):
        return tcl.call(
            "list",
            *[part for key, item in value.items() for part in (str(key), _to_tcl(tcl, item))],
        )
    if isinstance(value, (list, tuple)):
        return tcl.call("list", *[_to_tcl(tcl, item) for item in value])
    if isinstance(value, bool):
        return "1" if value else "0"
    if value is None:
        return ""
    if isinstance(value, float):
        return repr(value)
    return str(value)


class _TclHost:
    """The Tcl interpreters of a Session: one for the person, and a safe one,
    made when a safe script first runs. They are used from one thread"""

    def __init__(self, session):
        try:
            import _tkinter
        except ImportError:
            raise TclError(
                "Tcl is not available: this Python has no tkinter (the module _tkinter), "
                "which brings the Tcl library"
            )
        self._TclLibraryError = _tkinter.TclError
        self._session = session
        # Results as text, not Tcl objects; no Tk
        self._tcl = _tkinter.create(None, "openfpga", "Tk", False, False, False, False, None)
        self._output = []
        self._size = 0
        self._printed = []
        self._printed_size = 0
        self._commands = []
        self._causes = []
        self._sandbox = None
        self._fixed_width = False  # of the script that runs: see Session.fixed_width_refusal
        self._flat_routing = True
        self.commands = [name for name in session.shell_commands() if name not in _TCL_LEFT_OUT]
        self._tcl.eval("namespace eval ::openfpga {}")
        self._tcl.createcommand("::openfpga::_run", self._run)
        self._tcl.createcommand("::openfpga::_write", self._write)
        self._tcl.eval(_TCL_SETUP)
        for name in self.commands:
            self._tcl.call(
                "interp", "alias", "", "::openfpga::" + name, "", "::openfpga::_call", name, "0"
            )
            # A command of Tcl keeps its name (source): the one of the shell is openfpga::source
            if not self._tcl.eval("info commands ::%s" % name):
                self._tcl.call(
                    "interp", "alias", "", "::" + name, "", "::openfpga::_call", name, "0"
                )

    def _make_sandbox(self):
        name = "openfpga_safe"
        self._tcl.call("interp", "create", "-safe", name)
        self._tcl.call("interp", "eval", name, "rename puts {}")
        self._tcl.call("interp", "alias", name, "puts", "", "::openfpga::_puts", "1")
        for command in self.commands:
            # The refused ones are there too, to say that they are refused
            if not self._tcl.call("interp", "eval", name, "info commands ::%s" % command):
                self._tcl.call(
                    "interp", "alias", name, command, "", "::openfpga::_call", command, "1"
                )
        return name

    # The calls from Tcl ----------------------------------------------------------

    def _emit(self, text, printed=False):
        if not text:
            return
        self._output, self._size = _keep(self._output, self._size, text)
        if printed:
            self._printed, self._printed_size = _keep(self._printed, self._printed_size, text)

    def _write(self, text, newline="1"):
        self._emit(text + ("\n" if newline == "1" else ""), printed=True)
        return ""

    def _answer(self, status, value, code=""):
        return self._tcl.call("list", status, value, code)

    def _run(self, name, safe, *words):
        """A command of the shell, from Tcl: a list of the status (ok or
        error), the value (the answer of a query, or the message of the error)
        and the kind of the error. It never raises: an exception in a command
        of Tcl that Python makes is not one that Tcl can catch"""
        try:
            args, options = _split_words(words)
            refusal = _safe_refusal(name, args, options) if safe == "1" else None
            if refusal is None and self._fixed_width:
                refusal = self._session.fixed_width_refusal(name, self._flat_routing)
            if refusal:
                self._causes = [Diagnostic("error", TclRefused.kind, refusal)]
                return self._answer("error", refusal, TclRefused.kind)
            line = _command_line(name, args, options)
            self._commands.append(line)
            try:
                _, result = self._session._run_command(name, args, options)
            except OpenfpgaError as error:
                self._emit(_with_newline(error.log))
                errors = error.errors
                message = errors[0].message if errors else str(error).splitlines()[0]
                self._causes = list(error.diagnostics)
                return self._answer("error", message, errors[0].kind if errors else "error")
            answer = result.data
            if answer is not None and "data" in answer:
                # The value is the answer: its JSON, which the log has too, is not printed
                return self._answer("ok", _to_tcl(self._tcl, answer["data"]))
            self._emit(_with_newline(result.log))
            return self._answer("ok", "")
        except (
            Exception
        ) as error:  # noqa: BLE001 - becomes an error of Tcl, which a script can catch
            self._causes = []
            return self._answer("error", "%s: %s" % (type(error).__name__, error), "python")

    # A script --------------------------------------------------------------------

    def run(self, script, safe, fixed_width=False, flat_routing=True):
        if not isinstance(script, str):
            raise TypeError("A Tcl script is text, not %s" % type(script).__name__)
        self._fixed_width = bool(fixed_width)
        self._flat_routing = bool(flat_routing)
        self._output, self._size, self._commands, self._causes = [], 0, [], []
        self._printed, self._printed_size = [], 0
        started = time.monotonic()
        # The script runs in catch, at the global level, which gives the line of an error
        run = self._tcl.call("list", "catch", script, "::_openfpga_result", "::_openfpga_options")
        if safe:
            if self._sandbox is None:
                self._sandbox = self._make_sandbox()

            def tcl(text):
                return self._tcl.call("interp", "eval", self._sandbox, text)

        else:
            tcl = self._tcl.eval
        try:
            status = tcl(run)
            result = tcl("set ::_openfpga_result")
            options = tcl("set ::_openfpga_options")
            tcl("unset -nocomplain ::_openfpga_result ::_openfpga_options")
        except (
            self._TclLibraryError
        ) as error:  # a script that broke catch or set, in its interpreter
            raise TclError(
                str(error), self._text(), "", self._commands, printed=self._text(self._printed)
            )
        if status == "1":

            def option(name):
                return (
                    self._tcl.call("dict", "get", options, name)
                    if self._tcl.call("dict", "exists", options, name) == "1"
                    else ""
                )

            info, code, line = option("-errorinfo"), option("-errorcode"), option("-errorline")
            if line and info:
                info += "\n    (script line %s)" % line
            cls = TclRefused if code == "OPENFPGA %s" % TclRefused.kind else TclError
            causes = self._causes if code.startswith("OPENFPGA ") else []
            raise cls(result, self._text(), info, self._commands, causes, self._text(self._printed))
        # 0 ok, 2 return; break and continue out of a loop end the script as well
        return TclResult(
            result,
            self._text(),
            list(self._commands),
            time.monotonic() - started,
            self._text(self._printed),
        )

    def _text(self, parts=None):
        return "".join(self._output if parts is None else parts)[-_TCL_OUTPUT_MAX:]


def _keep(parts, size, text):
    """Add text to the parts of an output, and keep the end of it when it grows too long"""
    parts.append(text)
    size += len(text)
    if size > 2 * _TCL_OUTPUT_MAX:
        kept = "".join(parts)[-_TCL_OUTPUT_MAX:]
        return [kept], len(kept)
    return parts, size


def _with_newline(text):
    return text + "\n" if text and not text.endswith("\n") else text or ""


def _safe_refusal(name, args, options):
    """Why a safe interpreter does not run a command, or None: a command that
    runs other programs, or a path out of the current directory (absolute, with
    .., or a link that leads out)"""
    if name in _TCL_SAFE_REFUSED:
        return "%s is not available in a safe interpreter" % name
    base = os.path.realpath(os.getcwd())
    values = list(args) + [value for value in options.values() if value is not True]
    for value in values:
        text = os.fspath(value) if isinstance(value, os.PathLike) else str(value)
        for piece in [text] + _TCL_PATH_SPLIT.split(text):
            if not piece:
                continue
            real = os.path.realpath(os.path.join(base, piece))
            if real != base and not real.startswith(base + os.sep):
                return (
                    "'%s' is out of the directory of the session: a safe interpreter only "
                    "uses files in it" % piece
                )
    return None
