"""A file that the page opens in its viewer: what may be read, and what is refused"""

import json

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from starlette.testclient import TestClient  # noqa: E402

from openfpga_app import AppConfig, viewer  # noqa: E402
from openfpga_app.backend import DemoBackend  # noqa: E402
from openfpga_app.server import create_app  # noqa: E402
from test_app_device_package import LOGIC, backend, client  # noqa: E402,F401
from test_app_session import HOST, call, say, view  # noqa: E402,F401
from test_app_upload import put  # noqa: E402
from test_mcp_tools import golden  # noqa: E402,F401


def fetch(client, name):
    return client.get("/api/file", params={"name": name})


def test_an_input_by_its_name_and_a_file_that_was_uploaded(client, golden):
    shown = fetch(client, golden.circuit.name)
    assert shown.status_code == 200
    data = shown.json()["data"]
    assert data["text"] == golden.circuit.read_text()
    assert data["bytes"] == golden.circuit.stat().st_size and data["truncated"] is False

    assert put(client, "mine.blif", b".model m\n.end\n").status_code == 200
    assert fetch(client, "mine.blif").json()["data"]["text"] == ".model m\n.end\n"


def test_a_result_of_the_session_is_shown_by_the_name_that_the_sidebar_gives(client, golden):
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    assert call(say(client, f"/read_inputs circuit={golden.circuit.name}"))["ok"]
    assert call(say(client, "/run_stage stage=pack"))["ok"]
    (name,) = [n for n in view(client)["files"]["results"] if n.endswith(".net")]
    data = fetch(client, name).json()["data"]
    assert "<block" in data["text"]  # the packing is XML


def test_a_file_of_a_package_is_shown_by_its_path_from_the_folder_of_the_packages(client):
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    names = [f["name"] for f in view(client)["files"]["inputs"]]
    assert names == [LOGIC + "/arch/vpr_arch.xml", LOGIC + "/arch/openfpga_arch.xml"]
    assert "<architecture" in fetch(client, names[0]).json()["data"]["text"]
    assert fetch(client, LOGIC + "/CHECKSUMS.sha256").status_code == 200


@pytest.mark.parametrize(
    "name, status, kind",
    [
        ("../etc/passwd", 403, "path_refused"),
        ("/etc/passwd", 403, "path_refused"),
        (LOGIC + "/../../../etc/passwd", 403, "path_refused"),
        ("nope.txt", 404, "file_not_found"),
        ("", 400, "invalid_argument"),
    ],
)
def test_what_is_not_inside_the_allowed_folders_is_refused(client, name, status, kind):
    answer = fetch(client, name)
    assert answer.status_code == status, answer.text
    assert answer.json()["diagnostics"][0]["kind"] == kind


def test_a_file_that_is_not_text_is_refused_and_a_long_one_is_cut(client):
    assert put(client, "data.csv", b"a,b\0c\n").status_code == 200
    answer = fetch(client, "data.csv")
    assert answer.status_code == 400
    assert answer.json()["diagnostics"][0]["kind"] == "binary_file"
    assert put(client, "big.txt", b"x" * (viewer.MAX_SHOWN_BYTES + 10)).status_code == 200
    data = fetch(client, "big.txt").json()["data"]
    assert data["truncated"] is True and len(data["text"]) == viewer.MAX_SHOWN_BYTES
    assert data["bytes"] == viewer.MAX_SHOWN_BYTES + 10


def test_the_viewer_needs_the_token_and_the_demo_has_no_files(client):
    anonymous = TestClient(client.app, base_url="http://" + HOST)
    assert anonymous.get("/api/file", params={"name": "a.txt"}).status_code == 401
    with TestClient(
        create_app(AppConfig(port=8770, token="t"), DemoBackend(delay=0)),
        base_url="http://" + HOST,
    ) as demo:
        demo.headers["Authorization"] = "Bearer t"
        assert fetch(demo, "a.txt").status_code == 404


def save(client, name, text):
    return client.put(
        "/api/file", params={"name": name}, content=text.encode(), headers={"Content-Type": "text/plain"}
    )


def test_a_file_of_the_uploads_is_editable_and_saved_in_place(client, backend, golden):
    assert put(client, "mine.blif", b".model m\n.end\n").status_code == 200
    data = fetch(client, "mine.blif").json()["data"]
    assert data["editable"] is True and data["copyable"] is False

    saved = save(client, "mine.blif", ".model changed\n.end\n")
    assert saved.status_code == 200, saved.json()
    assert saved.json()["data"] == {
        "name": "mine.blif",
        "bytes": len(".model changed\n.end\n"),
        "kind": "circuit",  # by its first lines
        "in_use": False,
    }
    assert fetch(client, "mine.blif").json()["data"]["text"] == ".model changed\n.end\n"
    folder = backend.config.root / "s1" / "uploads"
    assert [p.name for p in folder.iterdir()] == ["mine.blif"]  # no half file is left


def test_a_file_that_the_session_read_says_so_when_it_is_saved(client, backend, golden):
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    assert put(client, "mine.blif", golden.circuit.read_bytes()).status_code == 200
    assert call(say(client, "/read_inputs circuit=mine.blif"))["ok"]
    text = golden.circuit.read_text() + "\n# a comment\n"
    answer = save(client, "mine.blif", text).json()
    assert answer["ok"] and answer["data"]["in_use"] is True and answer["data"]["kind"] == "circuit"


def test_the_files_of_the_inputs_and_the_results_are_not_edited_but_can_be_copied(client, golden):
    shown = fetch(client, golden.circuit.name).json()["data"]
    assert shown["editable"] is False and shown["copyable"] is True
    refused = save(client, golden.circuit.name, "x")
    assert refused.status_code == 400 and refused.json()["diagnostics"][0]["kind"] == "invalid_argument"
    assert fetch(client, golden.circuit.name).json()["data"]["text"] == golden.circuit.read_text()

    copied = client.post("/api/file/copy", params={"name": golden.circuit.name})
    assert copied.status_code == 200, copied.json()
    assert copied.json()["data"]["name"] == golden.circuit.name
    assert copied.json()["data"]["kind"] == "circuit"
    # The copy is the one that is found now, and it is editable; the original is the same
    assert fetch(client, golden.circuit.name).json()["data"]["editable"] is True
    assert save(client, golden.circuit.name, ".model e\n.end\n").status_code == 200
    assert golden.circuit.read_text() != ".model e\n.end\n"
    again = client.post("/api/file/copy", params={"name": golden.circuit.name})
    assert again.status_code == 400  # a copy that is there is not replaced


def test_a_file_of_a_package_or_of_another_kind_is_not_saved(client):
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    for name in (LOGIC + "/arch/vpr_arch.xml", "../x.xml", "nope.xml"):
        assert save(client, name, "<a/>").status_code in (400, 403, 404)
    assert "<architecture" in fetch(client, LOGIC + "/arch/vpr_arch.xml").json()["data"]["text"]


def test_the_text_that_is_saved_is_limited_and_utf8(client):
    assert put(client, "mine.blif", b".model m\n").status_code == 200
    big = save(client, "mine.blif", "x" * (viewer.MAX_SHOWN_BYTES + 2048))
    assert big.status_code == 413
    bad = client.put("/api/file", params={"name": "mine.blif"}, content=b"\xff\xfe")
    assert bad.status_code == 400
    assert fetch(client, "mine.blif").json()["data"]["text"] == ".model m\n"


def test_nothing_is_saved_in_the_demo():
    with TestClient(
        create_app(AppConfig(port=8770, token="t"), DemoBackend(delay=0)),
        base_url="http://" + HOST,
    ) as demo:
        demo.headers["Authorization"] = "Bearer t"
        assert save(demo, "a.blif", "x").status_code == 404
        assert demo.post("/api/file/copy", params={"name": "a.blif"}).status_code == 404


def test_the_odc_of_the_session_is_in_the_sidebar_with_its_checks_on_its_lines(
    client, golden, tmp_path
):
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    assert call(say(client, f"/read_inputs circuit={golden.circuit.name}"))["ok"]
    assert view(client)["constraints"] is None  # no ODC, and the package has no I/O map
    odc = (
        "odc_version 1\n"
        "create_clock -period 10 clk\n"
        "set_input_delay 0 -clock clk [get_ports {IN0 INX}]\n"
    )
    assert put(client, "top.odc", odc.encode()).status_code == 200
    assert call(say(client, "/set_constraint kind=odc file=top.odc"))["ok"]
    v = view(client)
    shown = v["constraints"]
    assert shown["file"] == "top.odc" and shown["commands"] == 2
    assert (shown["errors"], shown["warnings"]) == (0, 1)
    (check,) = shown["diagnostics"]
    assert (check["file"], check["line"], check["level"]) == ("top.odc", 3, "warning")
    assert shown["clocks"] == ["clk"] and shown["timing_checked"] is True
    assert shown["outputs_without_delay"] == 3
    # The files that the ODC gives VPR are not constraints of their own
    assert "odc top.odc" in v["files"]["constraints"]
    assert not any(c.startswith(("sdc ", "vpr constraints ")) for c in v["files"]["constraints"])
    assert str(tmp_path) not in json.dumps(v)

    answer = save(client, "top.odc", odc + "set_output_delay 0 -clock clk [all_outputs]\n").json()
    assert answer["ok"] and answer["data"]["in_use"] is True
    assert call(say(client, "/set_constraint kind=odc file=top.odc"))["ok"]
    assert view(client)["constraints"]["outputs_without_delay"] == 0
    assert call(say(client, "/set_constraint kind=odc clear=true"))["ok"]
    assert view(client)["constraints"] is None


def test_the_bitstream_is_a_stage_of_the_sidebar_and_its_file_is_downloaded(
    client, backend, golden
):
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    assert call(say(client, f"/read_inputs circuit={golden.circuit.name}"))["ok"]
    assert call(say(client, "/run_stage stage=bitstream"))["ok"]
    v = view(client)
    (bitstream,) = [s for s in v["stages"] if s["stage"] == "BITSTREAM"]
    assert bitstream["status"] == "reached"
    assert "bitstream" in v["reports"]
    name = "bitstream/%s.bit" % golden.circuit.stem
    assert name in v["files"]["results"]
    path = backend.config.root / "s1" / name
    got = client.get("/api/download", params={"name": name})
    assert got.status_code == 200 and got.content == path.read_bytes()
    assert "attachment" in got.headers["content-disposition"]
    assert fetch(client, name).json()["data"]["text"] == path.read_text()
    for bad in ("../x.bit", "/etc/passwd", "nope.bit"):
        assert client.get("/api/download", params={"name": bad}).status_code == 404
