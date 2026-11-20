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


class AmbiguousFile(RegistryError):
    """A short name that more than one file in the designs folders has"""

    kind = "ambiguous_file"


class PathRefused(RegistryError):
    """A file name that is not inside the workspace or the designs folders"""

    kind = "path_refused"
    hint = "Give a name relative to the workspace, or to a designs folder: no '..'"


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


class FileExists(RegistryError):
    kind = "file_exists"
    hint = "Give another name, or overwrite=true to replace the file that was written before"


class SessionNotEmpty(RegistryError):
    """A device package is not read over files that a session has read: that drops its stages"""

    kind = "session_not_empty"
    hint = "session_open starts a session that is empty, and use_device_package reads a package there"


class PackageNotFound(RegistryError):
    kind = "package_not_found"


class PackageInvalid(RegistryError):
    """A device package that cannot be used: the manifest, a path, a file or a checksum"""

    kind = "package_invalid"
    hint = "Check the package against docs/spec_device_package.md"
