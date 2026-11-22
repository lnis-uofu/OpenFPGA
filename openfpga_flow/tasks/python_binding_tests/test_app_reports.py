"""The reports of the stages in the app: the view lists them and the page opens one in a tab"""

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from test_app_session import backend, client, set_up, view  # noqa: E402,F401
from test_mcp_tools import golden  # noqa: E402,F401


def test_the_view_lists_the_reports_of_the_stages_that_ran(client, golden):
    assert view(client)["reports"] == []
    set_up(client, golden, stages=("pack", "place"))
    assert view(client)["reports"] == ["device", "pack", "place"]


def test_a_report_is_a_page_with_a_policy_that_runs_no_script(client, golden):
    set_up(client, golden, stages=("pack",))
    page = client.get("/api/report", params={"stage": "pack"})
    assert page.status_code == 200 and page.headers["content-type"].startswith("text/html")
    assert "Packing report" in page.text
    policy = page.headers["content-security-policy"]
    assert "default-src 'none'" in policy and "script-src" not in policy
    assert page.headers["x-content-type-options"] == "nosniff"


@pytest.mark.parametrize("stage", ["route", "../reports/pack", "", "pack.html"])
def test_a_stage_with_no_report_or_a_name_that_is_not_a_stage_is_not_found(client, golden, stage):
    set_up(client, golden, stages=("pack",))
    assert client.get("/api/report", params={"stage": stage}).status_code == 404


def test_a_report_needs_the_token(client, golden):
    set_up(client, golden, stages=("pack",))
    del client.headers["Authorization"]
    assert client.get("/api/report", params={"stage": "pack"}).status_code == 401
