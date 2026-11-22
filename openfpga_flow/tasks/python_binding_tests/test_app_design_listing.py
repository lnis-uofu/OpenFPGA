"""The designs that the model is told about: the directories that have a manifest design.json"""

import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))

from openfpga_mcp.config import Config  # noqa: E402
from openfpga_mcp.paths import design_directories  # noqa: E402


def _design(folder: Path, name: str, top: str, files):
    (folder / "rtl").mkdir(parents=True)
    for f in files:
        (folder / "rtl" / f).write_text("module %s; endmodule\n" % top)
    (folder / "design.json").write_text(
        json.dumps(
            {
                "format_version": "1.0",
                "name": name,
                "top": top,
                "files": [{"role": "rtl", "path": "rtl/" + f} for f in files],
            }
        )
    )


def _config(tmp_path, *folders):
    return Config(root=tmp_path / "workspace", design_dirs=tuple(folders), call_timeout_s=60)


def test_design_directories_are_the_folders_with_a_manifest(tmp_path):
    designs = tmp_path / "designs"
    _design(designs / "group" / "alpha", "alpha", "alpha_top", ["a.v", "b.v"])
    (designs / "group" / "loose").mkdir(parents=True)
    (designs / "group" / "loose" / "x.v").write_text("module x; endmodule\n")
    found = design_directories(_config(tmp_path, designs))
    assert [d["name"] for d in found] == ["alpha"]
    assert found[0]["top"] == "alpha_top" and found[0]["rtl"] == ["a.v", "b.v"]
    assert found[0]["relative"] == "group/alpha"


def test_a_manifest_that_is_not_an_object_is_skipped(tmp_path):
    designs = tmp_path / "designs"
    (designs / "bad").mkdir(parents=True)
    (designs / "bad" / "design.json").write_text("[1, 2]")
    (designs / "worse").mkdir()
    (designs / "worse" / "design.json").write_text("{not json")
    assert design_directories(_config(tmp_path, designs)) == []


def test_the_prompt_lists_the_designs_with_top_and_files_and_not_their_circuits(
    openfpga_module, tmp_path
):
    from openfpga_app.session_backend import SessionBackend

    designs = tmp_path / "designs"
    _design(designs / "alpha", "alpha", "alpha_top", ["a.v"])
    (designs / "alpha" / "inside.blif").write_text(".model inside\n.end\n")
    (designs / "plain.blif").write_text(".model plain\n.inputs a\n.outputs b\n.names a b\n1 1\n.end\n")
    backend = SessionBackend(_config(tmp_path, designs))
    try:
        listing = backend._files_listing()
    finally:
        backend.close()
    assert "synthesize(design=name)" in listing
    assert "alpha (top alpha_top)" in listing
    assert "plain.blif" in listing and "inside.blif" not in listing


def test_the_view_has_the_design_directories_with_no_session(openfpga_module, tmp_path):
    from openfpga_app.session_backend import SessionBackend

    designs = tmp_path / "designs"
    _design(designs / "group" / "alpha", "alpha", "alpha_top", ["a.v", "b.v"])
    backend = SessionBackend(_config(tmp_path, designs))
    try:
        view = backend._build_view()
    finally:
        backend.close()
    assert view["session"] is None
    assert view["designs"] == [
        {"name": "alpha", "top": "alpha_top", "path": "group/alpha", "files": ["a.v", "b.v"], "netlists": [], "description": None}
    ]
