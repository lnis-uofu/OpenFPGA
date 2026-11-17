"""A file that the page opens in its viewer: what may be read, and what is refused"""

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
