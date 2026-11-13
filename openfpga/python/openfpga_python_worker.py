# Sessions in worker processes: part of the module 'openfpga', see
# openfpga_python.i, which includes it after the Session.
#
# VPR keeps its data in global variables, aborts on an assertion and exits the
# process on a bad option, so a Session shares its fate with the interpreter
# that runs it, and two sessions share their data. A RemoteSession runs a
# Session in a process of its own: whatever happens to the worker (abort, exit,
# kill, a command that takes too long) the interpreter of the caller goes on,
# and workers do not share anything.
import multiprocessing.connection
import signal
import socket
import subprocess
import sys
import tempfile
import threading


class WorkerError(RuntimeError):
    """Base of the failures of a worker, as opposed to the failures of a command
    (OpenfpgaError), which the worker survives.

    diagnostics is a list of Diagnostic objects, as for OpenfpgaError."""

    kind = "error"
    hint = None

    @property
    def diagnostics(self):
        return [
            Diagnostic(
                "error",
                self.kind,
                str(self).split("\n")[0],
                hint=self.hint,
                data=self._data(),
            )
        ]

    def _data(self):
        return {}


class WorkerNotRunning(WorkerError):
    """The worker is not running: it died, was cancelled or closed. restart()
    starts another one"""

    kind = "worker_not_running"
    hint = "Call restart()"


class WorkerCrashed(WorkerError):
    """The worker process ended while it ran a call, without an answer.

    Attributes:
        exitcode: the exit code of the process, negative for a signal
        signal_name: the signal that ended it (e.g., SIGABRT), or None
        log_tail: the last lines that the worker wrote on its output, outside
            of the commands
    """

    def __init__(self, message, exitcode, signal_name, log_tail):
        super().__init__(message)
        self.exitcode = exitcode
        self.signal_name = signal_name
        self.log_tail = log_tail

    kind = "worker_crashed"
    hint = (
        "restart() starts another worker, which is restored from the last "
        "checkpoint if there is one"
    )

    def _data(self):
        return {"exitcode": self.exitcode, "signal": self.signal_name}

    @property
    def diagnostics(self):
        # What the worker wrote before it ended may say why
        return super().diagnostics + parse_diagnostics(self.log_tail)


class WorkerTimeout(WorkerError):
    """A call took longer than its timeout: the worker was killed"""

    kind = "worker_timeout"
    hint = "restart() starts another worker; a longer timeout_s or a smaller step may help"


class WorkerCancelled(WorkerError):
    """cancel() was called while a call was running: the worker was killed"""

    kind = "worker_cancelled"
    hint = "restart() starts another worker"


# The failures of a command that are given back as themselves
_WIRE_ERRORS = {
    "OpenfpgaError": OpenfpgaError,
    "AnswerError": AnswerError,
    "CheckpointError": CheckpointError,
}
# Seconds between two looks at the worker while a call is running
_POLL_INTERVAL = 0.05
# Lines of the output of a worker that a failure reports
_LOG_TAIL_LINES = 20


def _error_to_wire(error):
    """An exception of the worker as something that is sent: its type, its
    message and the attributes of the failure of a command"""
    name = type(error).__name__
    if isinstance(error, OpenfpgaError):
        attributes = {key: getattr(error, key) for key in ("command", "status", "log", "error")}
    else:
        attributes = {}
    return (name, str(error), attributes)


def _error_from_wire(name, message, attributes):
    cls = _WIRE_ERRORS.get(name)
    if cls is None:
        return WorkerError("%s in the worker: %s" % (name, message))
    # The failure of a command is made from the command and its result, which
    # do not travel: it is rebuilt with the same attributes and message
    error = cls.__new__(cls)
    RuntimeError.__init__(error, message)
    for key, value in attributes.items():
        setattr(error, key, value)
    return error


def _worker_main():
    """The loop of a worker: it takes calls of the methods of a Session from the
    socket whose number is the argument, and answers them. It ends when the
    caller closes the socket, or dies"""
    connection = multiprocessing.connection.Connection(int(sys.argv[1]))
    try:
        # The worker ends with the process that started it, even in a command
        import ctypes

        ctypes.CDLL(None).prctl(1, int(signal.SIGKILL))  # PR_SET_PDEATHSIG
    except Exception:  # noqa: BLE001 - only on Linux
        pass
    session = None
    while True:
        try:
            request = connection.recv()
        except (EOFError, OSError):
            return
        operation = request[0]
        try:
            if operation == "close":
                connection.send(("ok", None))
                return
            if operation == "start":
                checkpoint_dir, restore = request[1:]
                if restore is not None:
                    session = Session.restore(restore)
                    session.checkpoint_dir = checkpoint_dir
                else:
                    session = Session(checkpoint_dir=checkpoint_dir)
                connection.send(("ok", None))
                continue
            _, name, args, kwargs = request
            if name not in _REMOTE_CALLS:
                raise AttributeError("'%s' is not a method of a Session" % name)
            target = session
            for part in name.split("."):
                target = getattr(target, part)
            connection.send(("ok", target(*args, **kwargs)))
        except BaseException as error:  # noqa: BLE001 - reported to the caller
            try:
                connection.send(("error",) + _error_to_wire(error))
            except (OSError, EOFError):
                return


class _RemoteOptions:
    """The options of a RemoteSession: the same as Options"""

    def __init__(self, remote):
        self._remote = remote

    def get(self, name):
        return self._remote.call("options.get", name)

    def list(self):
        return self._remote.call("options.list")

    def changed(self):
        return self._remote.call("options.changed")

    def set(self, name, value):
        self._remote.call("options.set", name, value)

    def reset(self, name):
        self._remote.call("options.reset", name)

    def __getitem__(self, name):
        return self.get(name).value

    def __setitem__(self, name, value):
        self.set(name, value)

    def __iter__(self):
        return (option.name for option in self.list())

    def __contains__(self, name):
        return name in set(self)

    def __len__(self):
        return len(self.list())


# What a RemoteSession forwards: the methods of a Session that give a value that
# can travel, and not its shell
_REMOTE_METHODS = frozenset(
    name
    for name, member in vars(Session).items()
    if callable(member) and not name.startswith("_") and name not in ("restore", "replay")
)

# The calls that a worker takes: the methods, and the ones of the options
_REMOTE_CALLS = _REMOTE_METHODS | frozenset(
    "options." + name
    for name, member in vars(Options).items()
    if callable(member) and not name.startswith("_")
)

_WORKER_CODE = "import openfpga; openfpga._worker_main()"


class RemoteSession:
    """A Session that runs in a process of its own.

    It has the methods of Session (pack(), place(), timing_summary(), options,
    ...), and each one runs in the worker, gives its result back, and raises its
    failure: an OpenfpgaError is the one of the command, as with a Session. The
    failures of the worker itself are WorkerError:

    - WorkerCrashed: the process ended during a call (abort of VPR, an
      exit on a bad option, a kill by the system). It reports the exit code
      and the signal
    - WorkerTimeout: the call took longer than timeout_s, and the worker was
      killed
    - WorkerCancelled: cancel() was called, e.g., by another thread, and the
      worker was killed
    - WorkerNotRunning: a call after one of them, or after close()

    The caller is never affected. restart() starts another worker, and with
    checkpoint_dir the new worker is restored from the last checkpoint, which
    the worker makes after each stage, so that a flow loses the stage that it
    was running and nothing else.

        with openfpga.RemoteSession(cwd="run1", checkpoint_dir="run1.ckpt") as session:
            session.read_vpr_arch("arch.xml", layout="2x2")
            ...
            try:
                session.route(timeout_s=600)
            except openfpga.WorkerError:
                session.restart()   # at the end of place
                session.route()

    cwd is the directory the worker runs in, where VPR writes its files: it is
    created if it does not exist. Calls are made one at a time, and cancel() is
    the only method that can be called while another thread is in a call.
    """

    def __init__(self, cwd=None, checkpoint_dir=None, timeout_s=None, _restore=None):
        self.cwd = None if cwd is None else os.fspath(cwd)
        self.checkpoint_dir = None if checkpoint_dir is None else os.fspath(checkpoint_dir)
        self.timeout_s = timeout_s
        self.options = _RemoteOptions(self)
        self._lock = threading.Lock()
        self._process = None
        self._connection = None
        self._log = None
        self._ended = "The worker is not started"
        self._cancelled = False
        self._start(_restore)

    @classmethod
    def restore(cls, directory, **arguments):
        """A RemoteSession whose worker is restored from the checkpoint in a
        directory, see Session.restore()"""
        return cls(_restore=os.fspath(directory), **arguments)

    # The worker ------------------------------------------------------------------

    @property
    def pid(self):
        """The process number of the worker, None if it is not running"""
        return self._process.pid if self.running else None

    @property
    def running(self):
        return self._process is not None and self._process.poll() is None

    def _start(self, restore):
        if self.cwd is not None:
            os.makedirs(self.cwd, exist_ok=True)
        ours, theirs = socket.socketpair()
        self._log = tempfile.TemporaryFile(mode="w+b")
        module_dir = os.path.dirname(os.path.abspath(__file__))
        env = dict(os.environ)
        env["PYTHONPATH"] = os.pathsep.join(
            [module_dir] + ([env["PYTHONPATH"]] if env.get("PYTHONPATH") else [])
        )
        try:
            self._process = subprocess.Popen(
                [sys.executable, "-c", _WORKER_CODE, str(theirs.fileno())],
                pass_fds=[theirs.fileno()],
                cwd=self.cwd,
                env=env,
                stdin=subprocess.DEVNULL,
                stdout=self._log,
                stderr=subprocess.STDOUT,
            )
        finally:
            theirs.close()
        self._connection = multiprocessing.connection.Connection(ours.detach())
        self._cancelled = False
        self._ended = None
        try:
            self._request(("start", self.checkpoint_dir, restore), self.timeout_s)
        except BaseException:
            self._kill()
            raise

    def _kill(self):
        if self._process is not None and self._process.poll() is None:
            self._process.kill()
        if self._process is not None:
            self._process.wait()
        if self._connection is not None:
            self._connection.close()
            self._connection = None

    def _log_tail(self):
        try:
            self._log.flush()
            self._log.seek(0)
            lines = self._log.read().decode(errors="replace").splitlines()
        except (OSError, ValueError):
            return ""
        return "\n".join(lines[-_LOG_TAIL_LINES:])

    def _died(self):
        """The failure for a worker that ended without answering"""
        self._process.wait()
        code = self._process.returncode
        name = None
        if code < 0:
            try:
                name = signal.Signals(-code).name
            except ValueError:
                name = "signal %d" % -code
        how = "was ended by %s" % name if name else "exited with code %d" % code
        tail = self._log_tail()
        self._ended = "The worker %s while it ran a call" % how
        message = self._ended + (":\n" + tail if tail else "")
        return WorkerCrashed(message, code, name, tail)

    def _request(self, request, timeout_s):
        """Send a request and wait for its answer. Return the value, or raise
        the failure. The worker is killed on a timeout or a cancel"""
        deadline = None if timeout_s is None else time.monotonic() + timeout_s
        try:
            self._connection.send(request)
        except (OSError, EOFError):
            raise self._cancelled_or_died()
        while True:
            try:
                ready = self._connection.poll(_POLL_INTERVAL)
            except (OSError, EOFError):
                raise self._cancelled_or_died()
            if ready:
                try:
                    reply = self._connection.recv()
                except (OSError, EOFError):
                    raise self._cancelled_or_died()
                break
            if self._process.poll() is not None:
                raise self._cancelled_or_died()
            if deadline is not None and time.monotonic() > deadline:
                self._kill()
                self._ended = "The worker was killed: a call took more than %g s" % timeout_s
                raise WorkerTimeout("A call took more than %g s: the worker was killed" % timeout_s)
        if reply[0] == "ok":
            return reply[1]
        raise _error_from_wire(*reply[1:])

    def _cancelled_or_died(self):
        if self._cancelled:
            self._kill()
            self._ended = "The worker was cancelled"
            return WorkerCancelled("The call was cancelled: the worker was killed")
        return self._died()

    # The calls -------------------------------------------------------------------

    def call(self, name, *args, timeout_s=None, **kwargs):
        """Call a method of the Session of the worker by name, e.g.
        call("options.set", "place.seed", 3), with the same arguments. The
        timeout of the call is timeout_s, or the one of the object"""
        with self._lock:
            if self._ended is not None:
                raise WorkerNotRunning(self._ended + ": use restart()")
            try:
                return self._request(
                    ("call", name, args, kwargs),
                    timeout_s if timeout_s is not None else self.timeout_s,
                )
            except (WorkerCrashed, WorkerTimeout, WorkerCancelled):
                self._kill()
                raise

    def __getattr__(self, name):
        if name in _REMOTE_METHODS:

            def method(*args, **kwargs):
                return self.call(name, *args, **kwargs)

            method.__name__ = name
            method.__doc__ = getattr(Session, name).__doc__
            return method
        raise AttributeError("'RemoteSession' has no attribute '%s'" % name)

    def cancel(self):
        """Kill the worker, and so end the call that another thread is in (it
        raises WorkerCancelled). A cancel with no call running ends the worker
        as well. It can be called from any thread"""
        self._cancelled = True
        process = self._process
        if process is not None and process.poll() is None:
            process.kill()

    def restart(self):
        """Start another worker, in the place of the one that ended or the one
        that is running. If there is a checkpoint in checkpoint_dir, the new
        worker is restored from it. Return the state of its session, see
        Session.state()"""
        with self._lock:
            self._kill()
            restore = None
            if self.checkpoint_dir is not None and os.path.exists(
                os.path.join(self.checkpoint_dir, _CHECKPOINT_MANIFEST)
            ):
                restore = self.checkpoint_dir
            self._start(restore)
        return self.state()

    def close(self):
        """End the worker. It is done once: another close() does nothing"""
        with self._lock:
            if self._ended is None and self.running:
                try:
                    self._request(("close",), 5)
                except (WorkerError, OSError):
                    pass
            self._kill()
            self._ended = self._ended or "The worker is closed"

    def __enter__(self):
        return self

    def __exit__(self, *exception):
        self.close()

    def __del__(self):
        try:
            self._kill()
        except Exception:  # noqa: BLE001 - at the end of the interpreter
            pass
