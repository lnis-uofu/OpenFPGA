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


class FixedWidthOnly(RegistryError):
    """The flow routes at a fixed channel width, and runs the stages one by one: a route with no
    width (VPR would search for the smallest) and the one-shot vpr command are refused"""

    kind = "chan_width_required"
    hint = (
        "set_constraint kind=chan_width width=N sets the width (a device package gives its own); "
        "get_device says the width of the device"
    )


class ChannelWidthFixed(RegistryError):
    """The channel width of a session of a device package is the one of its package, and is read
    only"""

    kind = "chan_width_fixed"
    hint = (
        "The device package gives the channel width: route with it (get_device says it); a design "
        "that does not route needs another placement, or another package"
    )


class FlatRoutingUnsupported(RegistryError):
    """Flat routing (router.flat_routing) is asked of a device whose package does not say that
    it supports it"""

    kind = "flat_routing_unsupported"
    hint = (
        "set_option name=router.flat_routing value=false; list_device_packages says which "
        "packages support flat routing"
    )


class BitstreamUnsupported(RegistryError):
    """The bitstream is asked of a device whose package says that it cannot make one
    (device.bitstream false)"""

    kind = "bitstream_unsupported"
    hint = "Say so: the device of this package makes no bitstream; the stages up to analysis run"
