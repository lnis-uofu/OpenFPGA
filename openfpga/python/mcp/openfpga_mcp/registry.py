"""The sessions of a server: workers kept by id, with limits and a clean end.

Each session is a RemoteSession, a process of its own, so that an abort of VPR,
a kill or a timeout ends the session and not the server. The registry is
thread safe: the tools of the server call it from worker threads.
"""

import concurrent.futures
import dataclasses
import itertools
import json
import os
import re
import shutil
import threading
import time
from typing import Dict, List, Optional

from .config import Config
from .errors import (  # noqa: F401 - the failures are part of what the registry raises
    RegistryError,
    SessionClosed,
    SessionLimit,
    UnknownSession,
)

_NAME = re.compile(r"[A-Za-z0-9][A-Za-z0-9 _.-]{0,63}")
_DIRECTORY = re.compile(r"s\d+")  # the directories that this registry makes
JOURNAL = "journal"  # the commands of the session, as scripts that run again (get_journal, replay)
META = "session.json"  # the name of the session and what the page knows of it, kept in its directory


@dataclasses.dataclass
class Entry:
    id: str
    name: str
    session: object
    directory: str
    created: float
    last_used: float
    # What each stage printed (the last of it), for read_log
    logs: Dict[str, str] = dataclasses.field(default_factory=dict)


@dataclasses.dataclass(frozen=True)
class SessionInfo:
    id: str
    name: str
    directory: str
    running: bool
    idle_s: float
    stages: List[str]
    ended: Optional[str]


def _exit_reason(session) -> str:
    """Why a worker is not running when no call has seen it end: it was killed or it crashed
    between two calls"""
    process = session._process
    code = None if process is None else process.poll()
    if code is None:
        return "The worker is not running"
    if code < 0:
        return "The worker was ended by signal %d between two calls" % -code
    return "The worker exited with code %d between two calls" % code


class SessionRegistry:
    def __init__(self, config: Config, clock=time.monotonic):
        self.config = config
        self._clock = clock
        self._lock = threading.RLock()
        self._entries: Dict[str, Entry] = {}
        self._gone: Dict[str, str] = {}
        self._numbers = itertools.count(1)
        # Every worker is started from this one thread, which lives as long as the registry. A
        # worker is ended by the system when the thread that started it ends (see
        # PR_SET_PDEATHSIG in the worker), and the threads that run the tools come and go
        self._spawner = concurrent.futures.ThreadPoolExecutor(
            max_workers=1, thread_name_prefix="openfpga-spawn"
        )
        config.root.mkdir(parents=True, exist_ok=True)

    # Sessions --------------------------------------------------------------------

    def open(self, name: Optional[str] = None) -> Entry:
        """Start a session in a directory of its own"""
        import openfpga

        if name is not None and not _NAME.fullmatch(name):
            raise ValueError(
                "A session name is 1 to 64 letters, digits, spaces, '_', '.' or '-', "
                "and starts with a letter or a digit"
            )
        with self._lock:
            self._reap()
            if len(self._entries) >= self.config.max_sessions:
                raise SessionLimit(
                    "%d sessions are open, which is the limit" % self.config.max_sessions
                )
            number = next(self._numbers)
            sid = "s%d" % number
            directory = self.config.root / sid
            # A directory of an earlier server is not reused: its files are not ours
            while directory.exists():
                number = next(self._numbers)
                sid = "s%d" % number
                directory = self.config.root / sid
            session = self.spawn(
                lambda: openfpga.RemoteSession(
                    cwd=directory,
                    checkpoint_dir=directory / "checkpoints",
                    timeout_s=self.config.call_timeout_s,
                    journal=directory / JOURNAL,
                    memory_limit_mb=self.config.worker_memory_mb,
                )
            )
            now = self._clock()
            entry = Entry(sid, name or sid, session, sid, now, now)
            self._entries[sid] = entry
            self.update_meta(entry, name=entry.name)
            return entry

    # What a session keeps in its directory, and sessions that a server finds there ----------

    def read_meta(self, directory: str) -> Dict:
        try:
            data = json.loads((self.config.root / directory / META).read_text())
        except (OSError, ValueError):
            return {}
        return data if isinstance(data, dict) else {}

    def update_meta(self, entry: Entry, **fields) -> None:
        """Keep the name of a session and what the page knows of it (the device package), so that
        the session can be opened again after a restart. A disk that fails does not stop a flow"""
        try:
            meta = {**self.read_meta(entry.directory), **fields}
            target = self.config.root / entry.directory / META
            temporary = target.with_name(".%s.part" % META)
            temporary.write_text(json.dumps(meta))
            os.replace(temporary, target)
        except OSError:
            pass

    def saved(self) -> List[Dict]:
        """The sessions of the workspace that no worker holds and that have a checkpoint: the
        ones that an earlier server made, and the ones that were closed. Newest first"""
        with self._lock:
            live = {e.directory for e in self._entries.values()}
        found = []
        try:
            children = [p for p in self.config.root.iterdir() if _DIRECTORY.fullmatch(p.name)]
        except OSError:
            return []
        for path in children:
            if path.name in live or path.is_symlink():
                continue
            try:
                manifest = json.loads((path / "checkpoints" / "checkpoint.json").read_text())
                modified = (path / "checkpoints" / "checkpoint.json").stat().st_mtime
            except (OSError, ValueError):
                continue
            meta = self.read_meta(path.name)
            found.append(
                {
                    "id": path.name,
                    "name": meta.get("name") or path.name,
                    "stages": manifest.get("stages", []),
                    "device_package": meta.get("device_package"),
                    "modified": modified,
                }
            )
        return sorted(found, key=lambda s: -s["modified"])

    def adopt(self, directory: str) -> Entry:
        """Start a worker for a session of an earlier server (or one that was closed), restored
        from its last checkpoint, and hold it under the id of its directory"""
        import openfpga

        if not _DIRECTORY.fullmatch(directory or ""):
            raise UnknownSession("'%s' is not a session of this workspace" % directory)
        path = self.config.root / directory
        checkpoint = path / "checkpoints"
        if path.is_symlink() or not (checkpoint / "checkpoint.json").is_file():
            raise UnknownSession("Session %s has no checkpoint to open it from" % directory)
        with self._lock:
            self._reap()
            if any(e.directory == directory for e in self._entries.values()):
                raise ValueError("Session %s is open already" % directory)
            if len(self._entries) >= self.config.max_sessions:
                raise SessionLimit(
                    "%d sessions are open, which is the limit" % self.config.max_sessions
                )
            session = self.spawn(
                lambda: openfpga.RemoteSession.restore(
                    checkpoint,
                    cwd=path,
                    checkpoint_dir=checkpoint,
                    timeout_s=self.config.call_timeout_s,
                    journal=path / JOURNAL,
                    memory_limit_mb=self.config.worker_memory_mb,
                )
            )
            now = self._clock()
            meta = self.read_meta(directory)
            entry = Entry(directory, meta.get("name") or directory, session, directory, now, now)
            self._entries[directory] = entry
            self._gone.pop(directory, None)
            return entry

    def get(self, sid: str) -> Entry:
        """The session, and it counts as used now"""
        with self._lock:
            self._reap()
            entry = self._entries.get(sid)
            if entry is None:
                if sid in self._gone:
                    raise SessionClosed("Session %s was closed: %s" % (sid, self._gone[sid]))
                raise UnknownSession("There is no session %s" % sid)
            entry.last_used = self._clock()
            return entry

    def restart(self, entry: Entry):
        """Start another worker for a session, restored from its last checkpoint. Returns the
        state of the session"""
        return self.spawn(entry.session.restart)

    def close(self, sid: str, delete_files: bool = False) -> None:
        """End a session. Its directory stays, unless delete_files"""
        with self._lock:
            entry = self.get(sid)
            del self._entries[sid]
            self._gone[sid] = "it was closed"
        self._end(entry, delete_files)

    def list(self) -> List[SessionInfo]:
        with self._lock:
            self._reap()
            now = self._clock()
            entries = list(self._entries.values())
        # Outside the lock of the registry: asking a worker for its state waits for the call
        # that it is in, if any, and other tools must not wait for that
        return [self._info(entry, now) for entry in entries]

    def close_all(self) -> None:
        """End every worker: at the exit of the server"""
        with self._lock:
            entries = list(self._entries.values())
            self._entries.clear()
        for entry in entries:
            self._end(entry, False)

    def directory_bytes(self, entry: Entry) -> int:
        """What the files of a session take. Links are not followed"""
        total = 0
        stack = [str(self.config.root / entry.directory)]
        while stack:
            try:
                with os.scandir(stack.pop()) as items:
                    for item in items:
                        if item.is_symlink():
                            continue
                        if item.is_dir(follow_symlinks=False):
                            stack.append(item.path)
                        else:
                            total += item.stat(follow_symlinks=False).st_size
            except OSError:
                pass
        return total

    def spawn(self, function):
        return self._spawner.submit(function).result()

    # Inside ----------------------------------------------------------------------

    def _info(self, entry: Entry, now: float) -> SessionInfo:
        session = entry.session
        running = session.running
        stages: List[str] = []
        ended = None
        if running:
            # Ask only when no call is running, so that a long stage does not block the list
            if not session._lock.locked():
                try:
                    stages = list(session.state().reached)
                except Exception:  # a worker that ends now is reported by the next call
                    pass
        else:
            ended = session._ended or _exit_reason(session)
        return SessionInfo(
            entry.id,
            entry.name,
            entry.directory,
            running,
            round(now - entry.last_used, 1),
            stages,
            ended,
        )

    def _reap(self):
        """Close the sessions that were idle for too long"""
        now = self._clock()
        for sid, entry in list(self._entries.items()):
            if now - entry.last_used > self.config.idle_timeout_s:
                # A call that is running makes the session busy, not idle
                if entry.session._lock.locked():
                    continue
                del self._entries[sid]
                self._gone[sid] = "it was idle for more than %g s" % self.config.idle_timeout_s
                self._end(entry, False)

    def _end(self, entry: Entry, delete_files: bool):
        try:
            entry.session.cancel()
            entry.session.close()
        finally:
            if delete_files:
                shutil.rmtree(self.config.root / entry.directory, ignore_errors=True)
