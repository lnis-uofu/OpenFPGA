"""The options of the flow of a device package (flow_defaults.vpr_options of its manifest): a
session that reads the package sets them, as the reference flow of the package gives them to
VPR. They are found by the flag of VPR of each option (AppOption.vpr_flag)."""

import hashlib
import json
import re
import shutil
from pathlib import Path

import pytest

pytest.importorskip("mcp")

from openfpga_mcp import device_packages  # noqa: E402
from openfpga_mcp.errors import PackageInvalid  # noqa: E402

PACKAGES = Path(__file__).resolve().parents[2] / "device-packages"
SCHEMAS = Path(__file__).resolve().parents[3] / "openfpga_flow" / "schemas"

# What the manifests of the repository say, as options of the flow
EXPECTED = {
    "k4_n4_dsp16_3x4": {"general.clock_modeling": "route"},
    "k6_frac_n10_2x2": {"general.clock_modeling": "route"},
    "k6_frac_n10_mem1k_3x2": {"general.constant_net_method": "route"},
}


@pytest.fixture
def session(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return openfpga_module.Session()


def changed_copy(tmp_path, name, change):
    """A copy of a package whose manifest is changed by change(manifest), with its checksum"""
    copy = tmp_path / "packages" / name
    shutil.copytree(PACKAGES / name, copy)
    path = copy / "device_package.json"
    manifest = json.loads(path.read_text())
    change(manifest)
    path.write_text(json.dumps(manifest, indent=2))
    sums = copy / "CHECKSUMS.sha256"
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    sums.write_text(
        re.sub(
            r"^\w+  device_package.json$",
            digest + "  device_package.json",
            sums.read_text(),
            flags=re.M,
        )
    )
    return device_packages.load(copy)


def test_an_option_knows_its_flag_of_vpr(openfpga_module, session):
    jsonschema = pytest.importorskip("jsonschema")
    assert session.options.get("general.device_layout").vpr_flag == "--device"
    assert session.options.get("router.route_chan_width").vpr_flag == "--route_chan_width"
    assert session.options.get("place.placer").vpr_flag is None  # an option of OpenFPGA
    raw = session.shell.run("report_app_option --json").data
    schema = json.loads((SCHEMAS / "app_options.schema.json").read_text())
    jsonschema.validate(raw, schema, cls=jsonschema.Draft202012Validator)


@pytest.mark.parametrize("name", sorted(EXPECTED))
def test_a_package_sets_the_options_of_its_flow(session, name):
    package = device_packages.load(PACKAGES / name)
    applied = device_packages.read_into(session, package)
    # The layout, the channel width and the circuit format are given by the package otherwise,
    # and agree with it: they are not told
    assert applied == {"options": EXPECTED[name], "ignored": {}}
    for option, value in EXPECTED[name].items():
        assert session.options[option] == value
    assert session.options["router.route_chan_width"] == package.channel_width
    assert session.state().inputs.vpr_arch.layout == package.layout


def test_what_is_not_set_is_told_with_the_reason(session, tmp_path):
    def change(manifest):
        manifest["flow_defaults"]["vpr_options"].update(
            device="9x9", route_chan_width=99, no_such_flag="x"
        )

    package = changed_copy(tmp_path, "k6_frac_n10_2x2", change)
    applied = device_packages.read_into(session, package)
    assert applied["options"] == {"general.clock_modeling": "route"}
    ignored = applied["ignored"]
    assert set(ignored) == {"device", "route_chan_width", "no_such_flag"}
    assert "2x2" in ignored["device"] and "20" in ignored["route_chan_width"]
    assert session.options["router.route_chan_width"] == 20


def test_a_value_that_the_flow_refuses_stops_before_the_architectures(session, tmp_path):
    def change(manifest):
        manifest["flow_defaults"]["vpr_options"]["constant_net_method"] = "nonsense"

    package = changed_copy(tmp_path, "k6_frac_n10_mem1k_3x2", change)
    with pytest.raises(PackageInvalid) as error:
        device_packages.read_into(session, package)
    assert "general.constant_net_method" in str(error.value)
    assert session.state().inputs.vpr_arch is None


def test_a_manifest_whose_options_are_not_values_does_not_load(tmp_path):
    def change(manifest):
        manifest["flow_defaults"]["vpr_options"]["clock_modeling"] = ["route"]

    with pytest.raises(PackageInvalid, match="vpr_options"):
        changed_copy(tmp_path, "k6_frac_n10_2x2", change)


@pytest.mark.parametrize("width", [None, 0, "absent"])
def test_a_package_without_a_channel_width_does_not_load(tmp_path, width):
    """The channel width of a package is read only, and the routing needs it: a package must
    give a number of tracks above 0"""

    def change(manifest):
        if width == "absent":
            del manifest["device"]["channel_width"]
        else:
            manifest["device"]["channel_width"] = width

    with pytest.raises(PackageInvalid, match="channel_width"):
        changed_copy(tmp_path, "k6_frac_n10_2x2", change)


def test_the_model_is_told_the_options_and_the_memory_reference_runs(openfpga_module, tmp_path):
    """The memory package runs its reference design with the options of its flow, through the
    tools of the model"""
    from openfpga_mcp import Config, build_server
    from test_mcp_tools import Script

    name = "k6_frac_n10_mem1k_3x2"
    manifest = json.loads((PACKAGES / name / "device_package.json").read_text())
    source = PACKAGES / name / manifest["reference"]["design"]
    script = Script(build_server(Config(root=tmp_path / "workspace", package_dirs=(PACKAGES,))))
    try:

        async def steps(call):
            sid = (await call("session_open"))["data"]["session"]
            used = await call("use_device_package", session=sid, name=name)
            await call("write_file", session=sid, name=source.name, content=source.read_text())
            top = manifest["reference"]["top"]
            synthesized = await call("synthesize", session=sid, top=top, files=source.name)
            analysis = await call("run_stage", session=sid, stage="analysis")
            return used, synthesized, analysis

        used, synthesized, analysis = script.run(steps)
    finally:
        script.close()
    assert used["data"]["options_set"] == EXPECTED[name]
    assert "options_not_set" not in used["data"]
    assert synthesized.get("ok"), synthesized
    assert analysis.get("ok"), analysis
    assert analysis["data"]["reached"][-1] == "ANALYZED"


def test_the_shell_reads_a_package_as_the_python_code_did(openfpga_module, session):
    """read_device_package is the command of the shell that Session.read_device_package and the
    MCP server run: it says what it set, and the session keeps the package"""
    name = "k6_frac_n10_mem1k_3x2"
    answer = session.read_device_package(PACKAGES / name)
    assert answer["name"] == name and answer["options_set"] == EXPECTED[name]
    assert answer["options_not_set"] == {}
    assert session.device_package() == answer
    # Reading an architecture by hand forgets the package
    session.read_openfpga_arch(str(PACKAGES / name / "arch" / "openfpga_arch.xml"))
    assert session.device_package() is None


def test_the_channel_width_of_a_package_is_read_only(openfpga_module, session):
    """The channel width is the one of the device package: it is not changed, nor reset, until
    an architecture is read by hand, which forgets the package"""
    name = "k6_frac_n10_2x2"
    session.read_device_package(PACKAGES / name)
    width = session.options["router.route_chan_width"]
    assert width > 0
    session.options["router.route_chan_width"] = width  # the value it has: nothing changes
    for change in (
        lambda: session.set_route_chan_width(width + 2),
        lambda: session.options.reset("router.route_chan_width"),
    ):
        with pytest.raises(openfpga_module.OpenfpgaError, match="read only"):
            change()
        assert session.options["router.route_chan_width"] == width
    assert session.state().reached == ["DEVICE"]
    # Another package is read with its own width
    session.read_device_package(PACKAGES / "k6_frac_n10_mem1k_3x2")
    session.read_openfpga_arch(str(PACKAGES / name / "arch" / "openfpga_arch.xml"))
    session.set_route_chan_width(width + 2)
    assert session.options["router.route_chan_width"] == width + 2


def test_a_checkpoint_and_a_journal_keep_the_package(openfpga_module, tmp_path, monkeypatch):

    monkeypatch.chdir(tmp_path)
    name = "k6_frac_n10_2x2"
    journal, checkpoint = tmp_path / "journal", tmp_path / "checkpoint"
    session = openfpga_module.Session(journal=journal)
    session.read_device_package(PACKAGES / name)
    session.options["place.seed"] = 3  # a change after the package, which a restore keeps
    session.read_circuit(str(PACKAGES / name / "reference" / "and2.blif"))
    session.pack()
    session.checkpoint(checkpoint)
    manifest = json.loads((checkpoint / "checkpoint.json").read_text())
    assert manifest["device_package"] == str(PACKAGES / name)

    restored = openfpga_module.Session.restore(checkpoint)
    assert restored.device_package()["name"] == name
    assert restored.options["place.seed"] == 3
    assert restored.options["general.clock_modeling"] == "route"
    assert restored.state().reached == ["DEVICE", "PACKED"]

    replayed = openfpga_module.Session.replay(journal)
    assert replayed.device_package()["name"] == name
    assert replayed.state().reached == ["DEVICE", "PACKED"]


@pytest.mark.parametrize("name", sorted(EXPECTED))
def test_the_answer_follows_its_schema(openfpga_module, session, name):
    jsonschema = pytest.importorskip("jsonschema")
    schema = json.loads((SCHEMAS / "device_package.schema.json").read_text())
    on_disk = session.shell.run("report_device_package --file %s" % (PACKAGES / name)).data
    jsonschema.validate(on_disk, schema, cls=jsonschema.Draft202012Validator)
    assert "options_set" not in on_disk["data"]
    session.read_device_package(PACKAGES / name)
    read = session.shell.run("report_device_package").data
    jsonschema.validate(read, schema, cls=jsonschema.Draft202012Validator)
