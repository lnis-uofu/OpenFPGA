"""A session that is created from a device package: the package is checked, the two architectures
are read, and the session waits for the circuit"""

import asyncio
import json
import shutil
from pathlib import Path

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from openfpga_mcp import device_packages  # noqa: E402
from openfpga_mcp.config import Config  # noqa: E402
from openfpga_mcp.errors import PackageInvalid, PackageNotFound  # noqa: E402
from openfpga_app.session_backend import SessionBackend  # noqa: E402
from test_app_session import HOST, call, client, say, view  # noqa: E402,F401
from test_mcp_tools import golden  # noqa: E402,F401

PACKAGES = Path(__file__).resolve().parents[2] / "device-packages"
LOGIC = "k6_frac_n10_2x2"


def config(tmp_path, golden, **kwargs):
    folders = {golden.circuit.parent}
    return Config(
        root=tmp_path / "workspace",
        design_dirs=tuple(folders),
        package_dirs=(PACKAGES,),
        call_timeout_s=120,
        **kwargs,
    )


@pytest.fixture
def backend(openfpga_module, tmp_path, golden):
    backend = SessionBackend(config(tmp_path, golden))
    yield backend
    backend.close()


@pytest.fixture
def copy(tmp_path):
    """A package that a test may damage"""
    target = tmp_path / "packages"
    shutil.copytree(PACKAGES / LOGIC, target / LOGIC)
    return target / LOGIC


# The package itself ------------------------------------------------------------------------


def test_the_packages_of_the_repository_load_and_say_what_they_are(tmp_path):
    found = device_packages.discover(Config(root=tmp_path, package_dirs=(PACKAGES,)))
    assert found["problems"] == []
    names = [p.name for p in found["packages"]]
    assert names == ["k4_n4_dsp16_3x4", "k6_frac_n10_2x2", "k6_frac_n10_mem1k_3x2"]
    logic = next(p for p in found["packages"] if p.name == LOGIC)
    assert (logic.layout, logic.lut_size) == ("2x2", 6)
    assert logic.vpr_arch.name == "vpr_arch.xml" and logic.openfpga_arch.is_file()


@pytest.mark.parametrize(
    "damage, why",
    [
        (lambda d: (d / "arch/vpr_arch.xml").write_text("<changed/>"), "changed"),
        (lambda d: (d / "arch/extra.xml").write_text("x"), "does not list"),
        (lambda d: (d / "arch/simulation_setting.xml").unlink(), "not a file"),
        (lambda d: (d / "CHECKSUMS.sha256").unlink(), "CHECKSUMS"),
        (lambda d: (d / "device_package.json").write_text("{"), "not readable"),
    ],
)
def test_a_package_that_is_damaged_is_refused_with_the_reason(copy, damage, why):
    damage(copy)
    with pytest.raises(PackageInvalid, match=why):
        device_packages.load(copy)


@pytest.mark.parametrize(
    "change, why",
    [
        (lambda m: m.update(name="other"), "name"),
        (lambda m: m.update(format_version="2.0"), "format_version"),
        (lambda m: m.pop("license"), "license"),
        (lambda m: m["device"].update(grid={"width": 9, "height": 9}), "fixed layout"),
        (lambda m: m["files"].append({"role": "x_a", "path": "../x"}), "not inside"),
        (lambda m: m["files"].__setitem__(0, {"role": "vpr_arch", "path": "/etc/hosts"}), "not inside"),
    ],
)
def test_a_manifest_that_breaks_a_rule_is_refused(copy, change, why):
    path = copy / "device_package.json"
    manifest = json.loads(path.read_text())
    change(manifest)
    path.write_text(json.dumps(manifest))
    with pytest.raises(PackageInvalid, match=why):
        device_packages.load(copy, verify_checksums=False)


def test_a_damaged_package_is_listed_as_a_problem_and_the_others_still_load(tmp_path, copy):
    (copy / "arch/vpr_arch.xml").write_text("<changed/>")
    other = copy.parent / "k6_frac_n10_mem1k_3x2"
    shutil.copytree(PACKAGES / "k6_frac_n10_mem1k_3x2", other)
    found = device_packages.discover(Config(root=tmp_path, package_dirs=(copy.parent,)))
    assert [p.name for p in found["packages"]] == ["k6_frac_n10_mem1k_3x2"]
    assert found["problems"][0]["name"] == LOGIC


def test_an_unknown_package_says_which_there_are(tmp_path):
    with pytest.raises(PackageNotFound) as raised:
        device_packages.find(Config(root=tmp_path, package_dirs=(PACKAGES,)), "nope")
    assert "k6_frac_n10_2x2" in raised.value.hint
    with pytest.raises(PackageNotFound):
        device_packages.find(Config(root=tmp_path, package_dirs=(PACKAGES,)), "../etc")


# The session --------------------------------------------------------------------------------


def test_the_page_lists_the_packages(client):
    answer = client.get("/api/device-packages").json()
    assert answer["ok"]
    assert [p["name"] for p in answer["data"]["packages"]] == [
        "k4_n4_dsp16_3x4",
        "k6_frac_n10_2x2",
        "k6_frac_n10_mem1k_3x2",
    ]
    first = answer["data"]["packages"][1]
    assert first["layout"] == "2x2" and first["lut_size"] == 6 and first["description"]


def test_a_session_from_a_package_reads_the_architectures_and_waits_for_the_circuit(
    client, backend, golden
):
    answer = client.post("/api/session/new", json={"device_package": LOGIC}).json()
    assert answer["ok"], answer
    shown = view(client)
    assert shown["session"]["name"] == LOGIC
    assert shown["session"]["device_package"]["name"] == LOGIC
    labels = [f["label"] for f in shown["files"]["inputs"]]
    assert labels == ["vpr arch", "openfpga arch"]  # no circuit: it stops there
    assert shown["files"]["inputs"][0]["extra"] == "layout 2x2"
    assert all(s["status"] == "not_reached" for s in shown["stages"])
    # Nothing is written in the package
    assert [p for p in (PACKAGES / LOGIC).rglob("*") if p.suffix in (".net", ".place", ".route")] == []

    # The person gives the circuit: only the circuit is needed now
    found = say(client, f"/read_inputs circuit={golden.circuit.name}")
    assert call(found)["ok"], call(found)
    shown = view(client)
    assert [f["label"] for f in shown["files"]["inputs"]] == ["vpr arch", "openfpga arch", "circuit"]
    assert [s["status"] for s in shown["stages"]][0] == "reached"
    assert call(say(client, "/run_stage stage=pack"))["ok"]


def test_a_package_that_cannot_be_used_makes_no_session(client, backend):
    answer = client.post("/api/session/new", json={"device_package": "nope"})
    assert not answer.json()["ok"]
    assert answer.json()["diagnostics"][0]["kind"] == "package_not_found"
    assert view(client)["session"] is None
    assert client.post("/api/session/new", json={"device_package": 3}).status_code == 400


def test_a_package_that_is_changed_makes_no_session(openfpga_module, tmp_path, golden, copy):
    (copy / "arch/vpr_arch.xml").write_text("<changed/>")
    backend = SessionBackend(
        Config(root=tmp_path / "ws", package_dirs=(copy.parent,), call_timeout_s=120)
    )
    try:
        answer = asyncio.run(backend.new_session(device_package=LOGIC))
        assert answer["diagnostics"][0]["kind"] == "package_invalid"
        assert asyncio.run(backend.view())["session"] is None
        assert backend.registry.list() == []
    finally:
        backend.close()


def test_read_inputs_without_architectures_and_without_a_package_says_what_is_missing(
    client, golden
):
    assert call(say(client, "/session_open"))["ok"]
    found = say(client, f"/read_inputs circuit={golden.circuit.name}")
    done = call(found)
    assert not done["ok"]
    assert "no architecture has been read" in json.dumps(done)
    found = say(client, f"/read_inputs circuit={golden.circuit.name} vpr_arch={golden.vpr_arch.name}")
    # The files are looked up before the arguments are checked (the layout message is tested in
    # test_mcp_synth.py): a name that is in no designs folder is reported first
    assert "in the workspace or in the designs folders" in json.dumps(call(found))


def test_the_model_is_told_to_give_only_the_circuit(openfpga_module, tmp_path, golden):
    from openfpga_app.agent.scripted import ScriptedModel, say as say_
    from test_app_agent import _collect

    model = ScriptedModel([say_("Understood.")])
    backend = SessionBackend(config(tmp_path, golden), model=model)
    try:
        assert asyncio.run(backend.new_session(device_package=LOGIC))["ok"]
        asyncio.run(_collect(backend.chat("What next?")))
        user = [m for m in model.requests[-1] if m["role"] == "user"][-1]["content"]
        assert "created session s1 from the device package k6_frac_n10_2x2" in user
        assert "no circuit yet" in user and "only circuit=" in user
    finally:
        backend.close()


def test_the_demo_has_no_packages_and_no_session_to_create():
    from starlette.testclient import TestClient

    from openfpga_app import AppConfig
    from openfpga_app.backend import DemoBackend
    from openfpga_app.server import create_app

    with TestClient(
        create_app(AppConfig(port=8770, token="t"), DemoBackend(delay=0)),
        base_url="http://" + HOST,
    ) as client:
        client.headers["Authorization"] = "Bearer t"
        assert client.get("/api/device-packages").json()["data"]["packages"] == []
        answer = client.post("/api/session/new", json={"device_package": LOGIC}).json()
        assert answer["diagnostics"][0]["kind"] == "demo"


# A circuit that is uploaded is read ---------------------------------------------------------


def put(client, name, data, overwrite=False):
    query = "name=%s%s" % (name, "&overwrite=1" if overwrite else "")
    return client.post(
        "/api/upload?" + query, content=data, headers={"Content-Type": "application/octet-stream"}
    )


def test_a_circuit_that_is_uploaded_is_read_when_the_session_waits_for_one(client, golden):
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    answer = put(client, "and2.blif", golden.circuit.read_bytes())
    assert answer.status_code == 200
    assert answer.json()["data"]["loaded"] is True
    shown = view(client)
    assert [f["label"] for f in shown["files"]["inputs"]] == ["vpr arch", "openfpga arch", "circuit"]
    assert shown["files"]["inputs"][2]["name"].endswith("uploads/and2.blif")
    assert shown["stages"][0]["status"] == "reached"  # DEVICE
    assert call(say(client, "/run_stage stage=pack"))["ok"]

    # A circuit that is there is not replaced by another upload: the stages would be dropped
    second = put(client, "other.blif", golden.circuit.read_bytes())
    assert second.json()["data"]["loaded"] is False and "load_error" not in second.json()["data"]
    assert view(client)["files"]["inputs"][2]["name"].endswith("and2.blif")
    assert view(client)["stages"][1]["status"] == "reached"  # PACKED is still there


def test_what_is_not_a_circuit_or_has_no_architectures_is_only_uploaded(client, golden):
    assert put(client, "notes.txt", b"hello").json()["data"]["loaded"] is False  # opens a session
    assert [f["label"] for f in view(client)["files"]["inputs"]] == []
    answer = put(client, "and2.blif", golden.circuit.read_bytes())
    assert answer.json()["data"]["loaded"] is False  # the session has no architectures
    assert view(client)["files"]["inputs"] == []


def test_a_circuit_that_cannot_be_read_is_uploaded_and_the_reason_is_given(client):
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    bad = b".model m\n.inputs a\n.outputs b\n.names a b c d e f g h i j k l m n o p\n1 1\n.end\n"
    answer = put(client, "bad.blif", bad)
    assert answer.status_code == 200
    data = answer.json()["data"]
    assert data["loaded"] is False
    assert data.get("load_error"), data
    assert [f["label"] for f in view(client)["files"]["inputs"]] == ["vpr arch", "openfpga arch"]


def test_the_model_is_told_that_the_circuit_was_read(openfpga_module, tmp_path, golden):
    from openfpga_app.agent.scripted import ScriptedModel, say as say_
    from test_app_agent import _collect

    model = ScriptedModel([say_("Understood.")])
    backend = SessionBackend(config(tmp_path, golden), model=model)
    try:
        assert asyncio.run(backend.new_session(device_package=LOGIC))["ok"]
        data = asyncio.run(backend.upload("and2.blif", golden.circuit.read_bytes(), False))["data"]
        assert data["loaded"]
        asyncio.run(_collect(backend.chat("What next?")))
        user = [m for m in model.requests[-1] if m["role"] == "user"][-1]["content"]
        assert "uploaded and2.blif (circuit) to session s1, and it was read as the circuit" in user
    finally:
        backend.close()
