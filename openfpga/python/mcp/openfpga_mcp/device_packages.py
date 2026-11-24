"""Device packages: one directory with everything that targets one device (docs/spec_device_package.md)

This reads the manifest of a package and checks what a session needs before it uses the package:
the format version, the name, that every path is a file inside the package, one architecture of
VPR and one of OpenFPGA, the layout that the grid names, and the checksums. It does not run the
flow and it never writes into a package.
"""

import hashlib
import json
import os
import re
import xml.etree.ElementTree as ET
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional

from .config import Config
from .errors import PackageInvalid, PackageNotFound

MANIFEST = "device_package.json"
CHECKSUMS = "CHECKSUMS.sha256"
_NAME = re.compile(r"^[a-z][a-z0-9_]*$")
_KNOWN_MAJOR = 1


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
    license: str = ""
    files: List[Dict[str, str]] = field(default_factory=list)  # role and path inside the package
    reference: Dict[str, Any] = field(default_factory=dict)

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


def _invalid(name: str, why: str) -> PackageInvalid:
    return PackageInvalid("The device package '%s' cannot be used: %s" % (name, why))


def _inside(root: Path, relative: Any, name: str) -> Path:
    if not isinstance(relative, str) or not relative or "\0" in relative:
        raise _invalid(name, "a path in the manifest is not text")
    if os.path.isabs(relative) or ".." in Path(relative).parts or "\\" in relative:
        raise _invalid(name, "the path '%s' is not inside the package" % relative)
    target = root / relative
    if target.is_symlink() or not target.is_file():
        raise _invalid(name, "the file '%s' is missing or is a link" % relative)
    if root.resolve() not in target.resolve().parents:
        raise _invalid(name, "the file '%s' leads outside the package" % relative)
    return target


def _layouts(vpr_arch: Path) -> List[str]:
    try:
        tree = ET.parse(vpr_arch)
    except (ET.ParseError, OSError) as error:
        raise PackageInvalid("The VPR architecture of the package is not readable: %s" % error)
    return [e.get("name", "") for e in tree.iter("fixed_layout")]


def load(directory: Path, verify_checksums: bool = True) -> DevicePackage:
    """The package in a directory, checked. Raises PackageInvalid with the first problem"""
    name = directory.name
    try:
        manifest = json.loads((directory / MANIFEST).read_text())
    except (OSError, ValueError) as error:
        raise _invalid(name, "its %s is not readable (%s)" % (MANIFEST, error))
    if not isinstance(manifest, dict):
        raise _invalid(name, "its %s is not an object" % MANIFEST)
    version = str(manifest.get("format_version", ""))
    if not re.fullmatch(r"\d+\.\d+", version) or int(version.split(".")[0]) > _KNOWN_MAJOR:
        raise _invalid(name, "format_version '%s' is not one that is known (1.x)" % version)
    if manifest.get("name") != name or not _NAME.match(name):
        raise _invalid(name, "its name '%s' is not the name of its directory" % manifest.get("name"))
    if not manifest.get("license"):
        raise _invalid(name, "it has no license")
    if verify_checksums:  # first: a file that was changed is the reason, whatever else follows
        _verify(directory, name)
    files = manifest.get("files")
    if not isinstance(files, list):
        raise _invalid(name, "it has no list of files")
    roles: Dict[str, List[Path]] = {}
    listed = set()
    listing: List[Dict[str, str]] = []
    for entry in files:
        if not isinstance(entry, dict):
            raise _invalid(name, "an entry of files is not an object")
        path = _inside(directory, entry.get("path"), name)
        listed.add(str(path.relative_to(directory)))
        listing.append({"role": str(entry.get("role", "")), "path": str(path.relative_to(directory))})
        roles.setdefault(str(entry.get("role", "")), []).append(path)
    for role in ("vpr_arch", "openfpga_arch"):
        if len(roles.get(role, [])) != 1:
            raise _invalid(name, "it must list exactly one file with the role %s" % role)
    present = {
        str(p.relative_to(directory)) for p in directory.rglob("*") if p.is_file() or p.is_symlink()
    } - {MANIFEST, "README.md", CHECKSUMS}
    if present != listed:
        extra = sorted(present - listed) + sorted(listed - present)
        raise _invalid(name, "the files and the manifest differ: %s" % ", ".join(extra[:3]))
    device = manifest.get("device") or {}
    grid = device.get("grid") or {}
    try:
        layout = "%dx%d" % (int(grid["width"]), int(grid["height"]))
    except (KeyError, TypeError, ValueError):
        raise _invalid(name, "device.grid needs a width and a height")
    flat_routing = device.get("flat_routing", False)
    if not isinstance(flat_routing, bool):
        raise _invalid(name, "device.flat_routing is true or false")
    vpr_arch = roles["vpr_arch"][0]
    if layout not in _layouts(vpr_arch):
        raise _invalid(
            name, "its grid is %s and the VPR architecture has no fixed layout of that name" % layout
        )
    return DevicePackage(
        name=name,
        version=str(manifest.get("version", "")),
        description=str(manifest.get("description", "")),
        path=directory,
        layout=layout,
        vpr_arch=vpr_arch,
        openfpga_arch=roles["openfpga_arch"][0],
        lut_size=device.get("lut_size"),
        channel_width=device.get("channel_width"),
        tags=[str(t) for t in device.get("tags", [])],
        flat_routing=flat_routing,
        license=str(manifest["license"]),
        files=listing,
        reference=manifest.get("reference") if isinstance(manifest.get("reference"), dict) else {},
    )


def _verify(directory: Path, name: str) -> None:
    sums = directory / CHECKSUMS
    if not sums.is_file():
        raise _invalid(name, "it has no %s" % CHECKSUMS)
    seen = set()
    for line in sums.read_text().splitlines():
        digest, _, relative = line.partition("  ")
        target = directory / relative
        if not relative or ".." in Path(relative).parts or not target.is_file():
            raise _invalid(name, "%s names '%s', which is not a file" % (CHECKSUMS, relative))
        if hashlib.sha256(target.read_bytes()).hexdigest() != digest:
            raise _invalid(name, "the file '%s' was changed (checksum)" % relative)
        seen.add(relative)
    missing = {
        str(p.relative_to(directory))
        for p in directory.rglob("*")
        if p.is_file() and p.name != CHECKSUMS
    } - seen
    if missing:
        raise _invalid(name, "%s does not list '%s'" % (CHECKSUMS, sorted(missing)[0]))


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


def read_into(session, package: DevicePackage) -> None:
    """Make a session target the device of a package: its two architectures, and its channel
    width (the routing runs at a fixed width)"""
    session.read_vpr_arch(str(package.vpr_arch), layout=package.layout)
    session.read_openfpga_arch(str(package.openfpga_arch))
    if package.channel_width:
        session.set_route_chan_width(package.channel_width)


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


_README_CHARS = 1200


def describe(package: DevicePackage) -> Dict[str, Any]:
    """Everything that a person or a model may want to know of one package: the device, the files
    (role, path inside the package, size) and the reference result, with the start of its README.
    The paths are inside the package: the folder of the package on the disk is not told"""
    files = []
    for entry in package.files:
        size = (package.path / entry["path"]).stat().st_size
        files.append({**entry, "bytes": size})
    readme = package.path / "README.md"
    text = readme.read_text(errors="replace") if readme.is_file() else ""
    expected = package.reference.get("expected") if package.reference else None
    return {
        **package.summary(),
        "license": package.license,
        "channel_width": package.channel_width,
        "vpr_arch": str(package.vpr_arch.relative_to(package.path)),
        "openfpga_arch": str(package.openfpga_arch.relative_to(package.path)),
        "files": files,
        "reference": {
            "design": package.reference.get("design"),
            "top": package.reference.get("top"),
            "expected": expected,
        }
        if package.reference
        else None,
        "readme": text[:_README_CHARS],
        "readme_cut": len(text) > _README_CHARS,
    }
