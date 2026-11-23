"""openfpga-app tcl: Tcl in a terminal, over the session of a running app or a session of its own.

Attached to an app (started with --console), a script runs in the current session of the app, in the
same Tcl interpreter as the Tcl mode of its console: what it does is on the page, in the journal and
in what the model is told. With --local DIR there is no app: the session is a worker of this
command, in DIR.

    openfpga-app tcl --root WORKSPACE            # the app that runs on WORKSPACE
    openfpga-app tcl --url http://localhost:8770/?token=...
    openfpga-app tcl --local run1 flow.tcl       # no app: a session in run1
"""

import argparse
import json
import os
import sys
import urllib.error
import urllib.parse
import urllib.request
from pathlib import Path
from typing import Callable, Optional, Tuple

# The file of a workspace where a running app writes its address and token (see cli.py)
ADDRESS_FILE = ".openfpga-app.json"

Outcome = Tuple[bool, str, str]  # succeeded, what it printed, its result


class Unavailable(Exception):
    """The app cannot be reached, or does not run Tcl"""


def write_address(root: Path, url: str, token: str) -> Optional[Path]:
    """Where a terminal finds the running app of a workspace: a file only its owner reads"""
    path = Path(root) / ADDRESS_FILE
    try:
        path.parent.mkdir(parents=True, exist_ok=True)
        descriptor = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_TRUNC, 0o600)
        with os.fdopen(descriptor, "w") as handle:
            json.dump({"url": url, "token": token, "pid": os.getpid()}, handle)
    except OSError:
        return None
    return path


def remove_address(path: Optional[Path]) -> None:
    if path is not None:
        try:
            path.unlink()
        except OSError:
            pass


def _address(args) -> Tuple[str, str]:
    """The base address and the token of the app: --url, $OPENFPGA_APP_URL, or the file of --root"""
    url = args.url or os.environ.get("OPENFPGA_APP_URL")
    if url:
        parts = urllib.parse.urlsplit(url)
        token = dict(urllib.parse.parse_qsl(parts.query)).get("token") or os.environ.get(
            "OPENFPGA_APP_TOKEN"
        )
        if not token:
            raise Unavailable("The address has no token: give the one that the app printed")
        return "%s://%s" % (parts.scheme or "http", parts.netloc), token
    if args.root is not None:
        path = Path(args.root) / ADDRESS_FILE
        try:
            found = json.loads(path.read_text())
        except (OSError, ValueError):
            raise Unavailable(
                "No app runs on %s (no %s): start it with --root %s --console"
                % (args.root, ADDRESS_FILE, args.root)
            )
        return found["url"], found["token"]
    raise Unavailable("Say which app: --root WORKSPACE or --url ADDRESS, or --local DIR for no app")


class _App:
    """The Tcl of the console of a running app"""

    def __init__(self, base: str, token: str):
        self.base = base.rstrip("/")
        self.token = token

    def _post(self, path: str, data) -> dict:
        request = urllib.request.Request(
            self.base + path,
            data=json.dumps(data).encode(),
            headers={"Authorization": "Bearer " + self.token, "Content-Type": "application/json"},
            method="POST",
        )
        try:
            with urllib.request.urlopen(request) as response:
                return json.loads(response.read().decode())
        except urllib.error.HTTPError as error:
            text = error.read().decode(errors="replace")
            try:
                text = json.loads(text).get("error", text)
            except (ValueError, AttributeError):
                pass
            if error.code == 401:
                text = "the token is not the one of the app"
            raise Unavailable("The app refused (%d): %s" % (error.code, text))
        except urllib.error.URLError as error:
            raise Unavailable("The app does not answer at %s: %s" % (self.base, error.reason))

    def open(self, session: str) -> None:
        answer = self._post("/api/session/open", {"session": session})
        if not answer.get("ok", True):
            message = (answer.get("diagnostics") or [{}])[0].get("message", answer)
            raise Unavailable("Cannot open %s: %s" % (session, message))

    def run(self, script: str) -> Outcome:
        answer = self._post("/api/console", {"command": script, "mode": "tcl"})
        return bool(answer.get("ok")), answer.get("output", ""), answer.get("result", "")

    def close(self) -> None:
        pass


class _Local:
    """A session of its own, in a worker: the full interpreter, as in the console of the app"""

    def __init__(self, directory: Path):
        import openfpga

        self.openfpga = openfpga
        directory = Path(directory).resolve()
        self.session = openfpga.RemoteSession(
            cwd=directory,
            checkpoint_dir=directory / "checkpoints",
            journal=directory / "journal",
        )

    def run(self, script: str) -> Outcome:
        try:
            done = self.session.tcl(script)
        except self.openfpga.TclError as error:
            return False, (error.output or "") + (error.error_info or str(error)), ""
        except self.openfpga.WorkerError as error:
            self.session.restart()
            return False, "%s: the worker was started again, from the last checkpoint" % error, ""
        except KeyboardInterrupt:
            self.session.cancel()
            self.session.restart()
            return False, "Interrupted: the worker was started again, from the last checkpoint", ""
        return True, done.output, done.result

    def close(self) -> None:
        self.session.close()


def _show(outcome: Outcome) -> None:
    ok, output, result = outcome
    output = output.rstrip("\n")
    if output:
        print(output, file=sys.stdout if ok else sys.stderr)
    if ok and result:
        print(result)


def _completer() -> Callable[[str], bool]:
    """Whether some lines are a whole Tcl script, or need more (an open brace, a quote)"""
    try:
        import _tkinter

        tcl = _tkinter.create(None, "complete", "Tk", False, False, False, False, None)
        return lambda text: tcl.call("info", "complete", text + "\n") == "1"
    except Exception:  # noqa: BLE001 - no Tcl here: count the braces

        def balanced(text: str) -> bool:
            depth = 0
            for char in text:
                depth += {"{": 1, "}": -1, "[": 1, "]": -1}.get(char, 0)
            return depth <= 0

        return balanced


def repl(runner, prompt: str = "openfpga% ") -> int:
    """Read scripts from the terminal until end of file: a line that leaves a brace open goes on
    on the next. exit or quit alone leave"""
    try:
        import readline  # noqa: F401 - history and editing of the lines
    except ImportError:
        pass
    complete = _completer()
    print("Tcl over OpenFPGA: the commands of the shell are Tcl commands. exit leaves.")
    while True:
        try:
            text = input(prompt)
            while not complete(text):
                text += "\n" + input("> ")
        except EOFError:
            print()
            return 0
        except KeyboardInterrupt:
            print()
            continue
        if text.strip() in ("exit", "quit"):
            return 0
        if text.strip():
            _show(runner.run(text))


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        prog="openfpga-app tcl",
        description="Tcl in a terminal, over the session of a running app (started with "
        "--console) or, with --local, a session of its own",
    )
    where = parser.add_mutually_exclusive_group()
    where.add_argument("--root", type=Path, help="the workspace of the running app")
    where.add_argument("--url", help="the address that the app printed, with its token")
    where.add_argument("--local", type=Path, metavar="DIR", help="no app: a session in DIR")
    parser.add_argument(
        "--session", help="the session of the app to use (default: its current one)"
    )
    parser.add_argument("-c", dest="command", metavar="SCRIPT", help="run this script and end")
    parser.add_argument("file", nargs="?", type=Path, help="a Tcl script to run, then end")
    args = parser.parse_args(argv)
    try:
        if args.local is not None:
            if args.session:
                parser.error("--session is for an app: --local has a session of its own")
            try:
                runner = _Local(args.local)
            except ImportError as error:
                print("The module openfpga is not found: %s" % error, file=sys.stderr)
                return 2
        else:
            runner = _App(*_address(args))
            if args.session:
                runner.open(args.session)
    except Unavailable as error:
        print(error, file=sys.stderr)
        return 2
    try:
        if args.command is not None or args.file is not None:
            try:
                script = args.command if args.command is not None else args.file.read_text()
            except OSError as error:
                print("Cannot read %s: %s" % (args.file, error.strerror or error), file=sys.stderr)
                return 2
            outcome = runner.run(script)
            _show(outcome)
            return 0 if outcome[0] else 1
        return repl(runner)
    except Unavailable as error:
        print(error, file=sys.stderr)
        return 2
    finally:
        runner.close()
