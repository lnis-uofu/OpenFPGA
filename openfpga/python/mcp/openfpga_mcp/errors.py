"""The failures that a tool reports as diagnostics. kind names what happened, as the kinds of
diagnostics of the flow do, and hint what to try"""

from typing import Optional


class RegistryError(Exception):
    """Base of the failures that the registry reports. kind names what happened, as the kinds
    of diagnostics do"""

    kind = "error"
    hint: Optional[str] = None


class UnknownSession(RegistryError):
    kind = "unknown_session"
    hint = "session_list gives the open sessions, session_open starts another"


class SessionClosed(RegistryError):
    """The session was open and the server closed it (idle for too long)"""

    kind = "session_closed"
    hint = "session_open starts another session"


class SessionLimit(RegistryError):
    kind = "too_many_sessions"
    hint = "session_close one that is not needed, or wait until one is closed for being idle"


class InvalidArgument(RegistryError):
    kind = "invalid_argument"


class PathRefused(RegistryError):
    """A file name that is not inside the workspace or the folders of the inputs"""

    kind = "path_refused"
    hint = "Give a name relative to the workspace, or to a folder of the inputs: no '..'"


class FileNotInWorkspace(RegistryError):
    kind = "file_not_found"


class OptionNotAllowed(RegistryError):
    kind = "option_not_allowed"


class SessionFull(RegistryError):
    kind = "session_full"
    hint = "session_close ends the session; its files stay in the workspace"


class WrongFileKind(RegistryError):
    """A file that is given for one input but is another kind of file"""

    kind = "wrong_file_kind"
