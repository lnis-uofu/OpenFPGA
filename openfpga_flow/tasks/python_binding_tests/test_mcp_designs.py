"""The designs folders: a short name is looked for under them, two files of one name are an error
that lists them, and the file list of the prompt shows only the BLIF circuits"""

import os

import pytest

from openfpga_mcp import paths
from openfpga_mcp.config import Config
from openfpga_mcp.errors import AmbiguousFile, FileNotInWorkspace, PathRefused


def write(root, name, text=".model m\n.end\n"):
    path = root / name
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)
    return path


@pytest.fixture
def designs(tmp_path):
    folder = tmp_path / "designs"
    write(folder, "top.blif")
    write(folder, "ubm/interface/routing_test/routing_test.blif")
    write(folder, "ubm/a/same.blif")
    write(folder, "ubm/b/same.blif")
    write(folder, "arch/k6.xml", "<architecture/>")
    return Config(root=tmp_path / "ws", design_dirs=(folder,)), folder


def test_a_short_name_is_found_under_the_designs_folder(designs):
    config, folder = designs
    assert paths.resolve_file(config, "", "routing_test.blif") == str(
        (folder / "ubm/interface/routing_test/routing_test.blif").resolve()
    )
    assert paths.resolve_file(config, "", "k6.xml").endswith("arch/k6.xml")  # any kind is found
    # A path with folders still works, and a file at the top is found as before
    assert paths.resolve_file(config, "", "ubm/a/same.blif").endswith("ubm/a/same.blif")
    assert paths.resolve_file(config, "", "top.blif").endswith("designs/top.blif")


def test_two_files_of_one_name_are_an_error_that_lists_them_never_a_choice(designs):
    config, _ = designs
    with pytest.raises(AmbiguousFile) as raised:
        paths.resolve_file(config, "", "same.blif")
    assert "ubm/a/same.blif" in str(raised.value) and "ubm/b/same.blif" in str(raised.value)
    assert raised.value.kind == "ambiguous_file"
    assert raised.value.hint == "Give the path, for example ubm/a/same.blif"


def test_a_name_that_is_not_there_says_so_and_the_hint_knows_the_deep_files(designs):
    config, _ = designs
    with pytest.raises(FileNotInWorkspace) as raised:
        paths.resolve_file(config, "", "routing_tes.blif")
    assert "routing_test.blif" in raised.value.hint


def test_the_search_is_inside_the_designs_folders_only(designs, tmp_path):
    config, folder = designs
    outside = write(tmp_path, "outside/x.blif")
    os.symlink(outside.parent, folder / "link")  # a link out of the folder is not followed
    os.symlink(outside, folder / "x.blif")
    for name in ("x.blif", "../outside/x.blif", "/etc/passwd"):
        with pytest.raises((FileNotInWorkspace, PathRefused)):
            paths.resolve_file(config, "", name)
    # The results of the flow are looked for in the workspace only
    with pytest.raises(FileNotInWorkspace):
        paths.resolve_file(config, "", "top.blif", workspace_only=True)


def test_the_walk_is_bounded_and_skips_hidden_folders(designs, monkeypatch):
    config, folder = designs
    write(folder, ".hidden/h.blif")
    deep = "/".join("d%d" % i for i in range(paths.DESIGN_DEPTH + 2))
    write(folder, deep + "/deep.blif")
    names = [p.name for p, _ in paths.design_files(config)]
    assert "h.blif" not in names and "deep.blif" not in names and "top.blif" in names
    monkeypatch.setattr(paths, "DESIGN_MAX_FILES", 2)
    assert len(list(paths.design_files(config))) == 2


def test_the_prompt_lists_only_the_circuits_of_the_designs_folders(openfpga_module, designs):
    from openfpga_app.session_backend import SessionBackend

    config, _ = designs
    backend = SessionBackend(config)
    try:
        lines = dict(l.split(": ", 1) for l in backend._files_listing().splitlines())
        assert set(lines) == {"circuit"}  # no architecture is listed from the designs folder
        shown = lines["circuit"].split(", ")
        # A name that is the only one of its name is short; the others are given by their path
        assert sorted(shown) == ["routing_test.blif", "top.blif", "ubm/a/same.blif", "ubm/b/same.blif"]
    finally:
        backend.close()
