"""Device packages: one directory with everything that targets one device (docs/spec_device_package.md)

The package is checked and read by the commands of the shell (read_device_package and
report_device_package, in C++): this module finds the packages in the folders of the server,
and gives what the commands answer to the tools and the page. It never writes into a package.
"""

import os
import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional

from .config import Config
from .errors import PackageInvalid, PackageNotFound

MANIFEST = "device_package.json"
_NAME = re.compile(r"^[a-z][a-z0-9_]*$")


@dataclass
class DevicePackage:
    name: str
    version: str
    description: str
    path: Path
    layout: str
    vpr_arch: Path
    openfpga_arch: Path
    lut_size: Optional[int]
    channel_width: Optional[int]
    tags: List[str] = field(default_factory=list)
    flat_routing: bool = False  # the flow was checked with router.flat_routing on this device
    # The options of VPR that the flow of the device takes (flow_defaults.vpr_options), by the
    # flag of VPR without its dashes, e.g. {"constant_net_method": "route"}
    vpr_options: Dict[str, Any] = field(default_factory=dict)
    license: str = ""
    files: List[Dict[str, str]] = field(default_factory=list)  # type and path inside the package
    reference: Dict[str, Any] = field(default_factory=dict)
    # What report_device_package answers of the package
    answer: Dict[str, Any] = field(default_factory=dict)

    def summary(self) -> Dict[str, Any]:
        """What the page shows to choose a package"""
        return {
            "name": self.name,
            "version": self.version,
            "description": self.description,
            "layout": self.layout,
            "lut_size": self.lut_size,
            "tags": list(self.tags),
            "flat_routing": self.flat_routing,
        }


_SHELL = None


def _shell():
    """A shell of this process, which checks and describes packages (it reads no flow)"""
    global _SHELL
    if _SHELL is None:
        import openfpga

        _SHELL = openfpga.OpenfpgaShell()
    return _SHELL


def _reason(error: Exception) -> str:
    """The reason that a command of the shell gave, from the failure it raised"""
    text = getattr(error, "log", None) or str(error)
    found = re.search(r"^Error \d+: (.*)$", text, re.M)
    return found.group(1).strip() if found else str(error)


def load(directory: Path, verify_checksums: bool = True) -> DevicePackage:
    """The package in a directory, checked (report_device_package --file). Raises
    PackageInvalid with the first problem"""
    import openfpga

    directory = Path(directory)
    try:
        result = _shell().report_device_package(
            file=os.fspath(directory), no_checksums=not verify_checksums
        )
    except openfpga.OpenfpgaError as error:
        raise PackageInvalid(_reason(error))
    data = result.data["data"]
    reference = data.get("reference") or {}
    return DevicePackage(
        name=data["name"],
        version=data["version"],
        description=data["description"],
        path=directory,
        layout=data["layout"],
        vpr_arch=directory / data["vpr_arch"],
        openfpga_arch=directory / data["openfpga_arch"],
        lut_size=data["lut_size"],
        channel_width=data["channel_width"],
        tags=list(data["tags"]),
        flat_routing=data["flat_routing"],
        vpr_options=dict(data["vpr_options"]),
        license=data["license"],
        files=[{"type": f["type"], "path": f["path"]} for f in data["files"]],
        reference={k: v for k, v in reference.items() if v is not None},
        answer=data,
    )


def discover(config: Config) -> Dict[str, Any]:
    """The packages in the folders of the configuration: the ones that load, and for each of the
    others why not (a package that is damaged is shown, not hidden)"""
    found: List[DevicePackage] = []
    problems: List[Dict[str, str]] = []
    for folder in config.package_dirs:
        try:
            children = sorted(p for p in folder.iterdir() if (p / MANIFEST).is_file())
        except OSError:
            continue
        for child in children:
            try:
                found.append(load(child))
            except PackageInvalid as error:
                problems.append({"name": child.name, "message": str(error)})
    return {"packages": found, "problems": problems}


def _names(config: Config) -> List[str]:
    names: List[str] = []
    for folder in config.package_dirs:
        try:
            names += [p.name for p in folder.iterdir() if (p / MANIFEST).is_file()]
        except OSError:
            continue
    return sorted(names)


def _not_found(message: str, config: Config) -> PackageNotFound:
    error = PackageNotFound(message)
    names = _names(config)
    error.hint = (
        "The packages are: %s. If the person named architecture files, give those to "
        "read_inputs instead" % ", ".join(names)
        if names
        else "There are none: if the person named architecture files, give those to read_inputs"
    )
    return error


def find(config: Config, name: str) -> DevicePackage:
    """One package by name"""
    if not isinstance(name, str) or not _NAME.match(name):
        raise _not_found("'%s' is not the name of a device package" % name, config)
    for folder in config.package_dirs:
        if (folder / name / MANIFEST).is_file():
            return load(folder / name)
    raise _not_found("There is no device package '%s'" % name, config)


def read_into(session, package: DevicePackage, corner: Optional[str] = None) -> Dict[str, Any]:
    """Make a session target the device of a package (read_device_package): the options of VPR
    of its flow (flow_defaults.vpr_options), its two architectures, and its channel width (the
    routing runs at a fixed width). Returns the options that were set, by name, and the entries
    of the manifest that were not, with the reason. A package or an option that the flow
    refuses raises PackageInvalid before the architectures are read. corner is a corner of the
    package (None: its default), whose timing values replace the variables of the architecture"""
    import openfpga

    try:
        answer = session.read_device_package(os.fspath(package.path), corner=corner)
    except openfpga.OpenfpgaError as error:
        raise PackageInvalid(_reason(error))
    return {
        "options": answer["options_set"],
        "ignored": answer["options_not_set"],
        "corner": answer.get("corner"),
    }


def flat_routing_allowed(config: Config, meta: Dict[str, Any]) -> bool:
    """Whether the device of a session supports flat routing, from what the registry keeps of
    the session (meta): a session of a device package only if its package says so, any other
    session (architectures that the person gave) yes. A package that is gone is not known to
    refuse it"""
    name = meta.get("package_name") if meta else None
    if not name:
        return True
    try:
        return find(config, name).flat_routing
    except (PackageInvalid, PackageNotFound):
        return True


def describe(package: DevicePackage) -> Dict[str, Any]:
    """Everything that a person or a model may want to know of one package: the device, the files
    (type, path inside the package, size) and the reference result, with the start of its README.
    The paths are inside the package: the folder of the package on the disk is not told"""
    return {k: v for k, v in package.answer.items() if k != "directory"}
