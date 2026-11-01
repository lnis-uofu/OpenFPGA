"""The queries about the design: timing_summary, critical_paths, utilization, placement,
net_route, congestion and report_cluster.

Each answer is checked against what VPR reports for the same run: its log, report_timing.setup.rpt,
the .place and .route files, the channel occupancy files and packing_pin_util.rpt.
"""

import json
import math
import re
from pathlib import Path

import pytest

SCHEMAS = Path(__file__).resolve().parents[3] / "openfpga_flow" / "schemas"
DESIGNS = {
    "and2_latch": "simple_gates/and2_latch/and2_latch.blif",
    "routing_test": "interface/routing_test/routing_test.blif",
}
KINDS = [
    "timing_summary",
    "critical_paths",
    "utilization",
    "placement",
    "net_route",
    "congestion",
    "cluster_report",
]


class Flow:
    """A design taken through the stages, with the logs of the stages"""

    def __init__(self, shell, and2_files, flow_dir, name):
        circuit = flow_dir / "benchmarks" / "ubm" / DESIGNS[name]
        if not circuit.exists():
            pytest.skip(f"missing {circuit}")
        self.shell = shell
        self.name = Path(circuit).stem
        self.files = and2_files
        self.circuit = circuit
        self.logs = {}

    def inputs(self, chan_width=20):
        shell = self.shell
        shell.set_app_option(name="general.clock_modeling", value="route")
        shell.set_app_option(name="router.route_chan_width", value=chan_width)
        shell.read_vpr_arch(file=self.files["vpr_arch"], layout="2x2")
        shell.read_openfpga_arch(file=self.files["openfpga_arch"])
        shell.read_circuit(file=self.circuit)

    def run(self, *stages):
        for stage in stages:
            self.logs[stage] = getattr(self.shell, stage)().log
        return self


@pytest.fixture(params=sorted(DESIGNS))
def flow(request, shell, and2_files, flow_dir):
    flow = Flow(shell, and2_files, flow_dir, request.param)
    flow.inputs()
    return flow


@pytest.fixture
def routed(flow):
    return flow.run("pack", "place", "route", "analysis")


@pytest.fixture(scope="module")
def validate():
    jsonschema = pytest.importorskip("jsonschema")

    def check(answer):
        schema = json.loads((SCHEMAS / f"{answer['kind']}.schema.json").read_text())
        jsonschema.Draft202012Validator.check_schema(schema)
        jsonschema.validate(answer, schema, cls=jsonschema.Draft202012Validator)

    return check


def data(result):
    return result.data["data"]


def close(actual, expected, rel=2e-5):
    return math.isclose(actual, expected, rel_tol=rel, abs_tol=1e-9)


# What VPR wrote, as numbers ----------------------------------------------------------------


def log_numbers(log, pattern):
    return [float(value) for value in re.findall(pattern, log)]


def parse_place(path):
    blocks = {}
    for line in path.read_text().splitlines():
        match = re.match(r"^(\S+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+#\d+", line)
        if match:
            name, x, y, sub, layer = match.groups()
            blocks[name] = (int(x), int(y), int(sub), int(layer))
    return blocks


def parse_route(path):
    """The nets of a .route file: name -> nodes (id, type, low corner, high corner, sink pin)"""
    nets, nodes = {}, None
    for line in path.read_text().splitlines():
        net = re.match(r"^Net \d+ \((.*)\)$", line)
        if net:
            nodes = nets.setdefault(net.group(1), [])
            continue
        node = re.match(
            r"^Node:\s+(\d+)\s+(\w+) \((\d+),(\d+),(\d+)\)(?: to \((\d+),(\d+),(\d+)\))?", line
        )
        if node and nodes is not None:
            ident, kind, x, y, layer, xh, yh, _ = node.groups()
            low = (int(x), int(y), int(layer))
            high = (int(xh), int(yh), int(layer)) if xh else low
            pin = re.search(r"Net_pin_index: (\d+)", line)
            nodes.append((int(ident), kind, low, high, int(pin.group(1)) if pin else None))
    return nets


def parse_occupancy(path):
    tiles = {}
    for line in path.read_text().splitlines():
        match = re.match(r"^\s*(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+[\d.]+\s+(\d+)\s*$", line)
        if match:
            _, x, y, used, capacity = map(int, match.groups())
            tiles[(x, y)] = (used, capacity)
    return tiles


def parse_timing_report(path, count):
    """The first paths of report_timing.setup.rpt: start, end, slack, data points"""
    text = path.read_text()
    paths = []
    for block in re.split(r"\n#Path \d+\n", text)[1 : count + 1]:
        start = re.search(r"Startpoint: (\S+)", block).group(1)
        end = re.search(r"Endpoint\s*: (\S+)", block).group(1)
        arrival_at = block.index("data arrival time")
        points = []
        for line in block[:arrival_at].splitlines():
            match = re.match(r"^(\S+) \((\.\w+)\).*?\s+(-?[\d.]+)\s+(-?[\d.]+)$", line)
            if match:
                points.append((match.group(1), float(match.group(4))))
        slack = float(re.search(r"slack \(\w+\)\s+(-?[\d.]+)", block).group(1))
        paths.append((start, end, slack, points))
    return paths


# The answers follow the schemas ------------------------------------------------------------


def test_every_answer_follows_its_schema(routed, validate):
    shell = routed.shell
    net = next(iter(parse_route(Path(f"{routed.name}.route"))))
    answers = [
        shell.timing_summary(),
        shell.critical_paths(count=3),
        shell.utilization(),
        shell.placement(),
        shell.net_route(net=net),
        shell.congestion(),
        shell.congestion(region_size=2),
        shell.report_cluster(),
        shell.report_cluster(stats=True, query="out:*"),
    ]
    seen = set()
    for result in answers:
        validate(result.data)
        assert result.data["schema_version"] == 1
        assert result.raw_data in result.log
        seen.add(result.data["kind"])
    assert seen == set(KINDS)


def test_the_stage_of_the_envelope_is_the_one_that_is_read(routed):
    shell = routed.shell
    stages = {
        "timing_summary": "ROUTED",
        "critical_paths": "ROUTED",
        "utilization": "PACKED",
        "placement": "PLACED",
        "congestion": "ROUTED",
        "report_cluster": "PACKED",
    }
    for command, stage in stages.items():
        assert getattr(shell, command)().data["stage"] == stage, command


def test_the_same_query_gives_the_same_text(routed):
    shell = routed.shell
    for command in ("timing_summary", "critical_paths", "congestion", "placement"):
        assert getattr(shell, command)().raw_data == getattr(shell, command)().raw_data


# Timing ------------------------------------------------------------------------------------


def test_timing_summary_is_what_the_analysis_reports(routed):
    log = routed.logs["analysis"]
    setup = data(routed.shell.timing_summary())["setup"]
    hold = data(routed.shell.timing_summary())["hold"]

    (delay,) = log_numbers(log, r"Final critical path delay \(least slack\): ([-\d.e]+) ns")
    (fmax,) = log_numbers(log, r"Fmax: ([-\d.e]+) MHz")
    (swns,) = log_numbers(log, r"Final setup Worst Negative Slack \(sWNS\): ([-\d.e]+) ns")
    (stns,) = log_numbers(log, r"Final setup Total Negative Slack \(sTNS\): ([-\d.e]+) ns")
    (hwns,) = log_numbers(log, r"Final hold Worst Negative Slack \(hWNS\): ([-\d.e]+) ns")
    (htns,) = log_numbers(log, r"Final hold Total Negative Slack \(hTNS\): ([-\d.e]+) ns")
    assert close(setup["critical_path_delay_ns"], delay)
    assert close(setup["fmax_mhz"], fmax)
    assert close(setup["worst_negative_slack_ns"], swns)
    assert close(setup["total_negative_slack_ns"], stns)
    assert close(hold["worst_negative_slack_ns"], hwns)
    assert close(hold["total_negative_slack_ns"], htns)
    # The delay is what Fmax says
    assert close(setup["fmax_mhz"], 1e3 / setup["critical_path_delay_ns"])


def test_timing_summary_lists_the_clock_domains(routed):
    summary = data(routed.shell.timing_summary())
    domains = summary["clock_domains"]
    assert domains
    crit = summary["setup"]["critical_path"]
    worst = min(domains, key=lambda d: d["worst_slack_ns"])
    assert (worst["launch_clock"], worst["capture_clock"]) == (
        crit["launch_clock"],
        crit["capture_clock"],
    )
    for domain in domains:
        assert domain["intra_domain"] == (domain["launch_clock"] == domain["capture_clock"])
        assert (domain["fmax_mhz"] is not None) == domain["intra_domain"]


def test_the_worst_paths_are_the_ones_of_the_timing_report(routed):
    paths = data(routed.shell.critical_paths(count=3))["paths"]
    expected = parse_timing_report(Path("report_timing.setup.rpt"), 3)
    assert len(paths) == len(expected) >= 2
    for path, (start, end, slack, points) in zip(paths, expected):
        assert path["startpoint"] == start
        assert path["endpoint"] == end
        assert path["slack_ns"] == pytest.approx(slack, abs=6e-4)
        # The data path ends the way the report does, point by point
        listed = [(p["pin"], p["arrival_ns"]) for p in path["points"]]
        assert [name for name, _ in listed] == [name for name, _ in points[-len(listed) :]]
        for (_, arrival), (_, reported) in zip(listed, points[-len(listed) :]):
            assert arrival == pytest.approx(reported, abs=6e-4)
        assert path["data_arrival_ns"] == pytest.approx(path["points"][-1]["arrival_ns"])


def test_the_paths_are_worst_first_and_made_of_steps(routed):
    paths = data(routed.shell.critical_paths(count=10))["paths"]
    slacks = [p["slack_ns"] for p in paths]
    assert slacks == sorted(slacks)
    assert [p["index"] for p in paths] == list(range(len(paths)))
    for path in paths:
        points = path["points"]
        assert points[0]["pin"] == path["startpoint"]
        assert points[-1]["pin"] == path["endpoint"]
        before = 0.0
        for point in points:
            assert point["delay_ns"] == pytest.approx(point["arrival_ns"] - before, abs=1e-9)
            before = point["arrival_ns"]
        # The slack of a path is what is required less what arrives
        assert path["slack_ns"] == pytest.approx(
            path["data_required_ns"] - path["data_arrival_ns"], abs=1e-6
        )


def test_the_number_of_paths_is_what_is_asked_for(routed):
    shell = routed.shell
    one = data(shell.critical_paths(count=1))
    assert (one["requested"], one["returned"]) == (1, 1)
    many = data(shell.critical_paths(count=100))
    assert many["requested"] == 100
    assert 1 <= many["returned"] <= 100
    assert data(shell.critical_paths())["requested"] == 5
    for bad in (0, -1, 101):
        result = shell.critical_paths(count=bad, check=False)
        assert result.status != 0 and result.data is None
    assert shell.critical_paths(count="many", check=False).status != 0


def test_timing_queries_agree_after_analysis_and_without_it(flow):
    """The analysis is not needed: the timing is analyzed from the routing"""
    flow.run("pack", "place", "route")
    before = flow.shell.timing_summary().raw_data
    flow.run("analysis")
    assert flow.shell.timing_summary().raw_data == before


# Packing -----------------------------------------------------------------------------------


def test_utilization_is_what_the_packing_reports(routed):
    log = routed.logs["pack"]
    answer = data(routed.shell.utilization())
    used = {t["name"]: t["used"] for t in answer["block_types"]}
    counted = re.findall(r"Netlist\n\t\t(\d+)\tblocks of type: (\w+)", log)
    assert {name for _, name in counted} == set(used)
    for count, name in counted:
        assert used[name] == int(count)
    # VPR reports it twice
    reported = log_numbers(log, r"Device Utilization: ([\d.]+)")
    assert reported
    assert answer["device_utilization"] == pytest.approx(reported[0], abs=0.005)
    assert answer["grid"] == {"width": 4, "height": 4}
    for block_type in answer["block_types"]:
        assert block_type["used"] <= block_type["available"]
        assert block_type["utilization"] == pytest.approx(
            block_type["used"] / block_type["available"]
        )
    assert {t["name"] for t in answer["block_types"] if t["is_io"]} == {"io"}
    # The places for blocks, as the "Architecture" lines of the resource usage of VPR count them:
    # a tile holds as many blocks as its capacity, and is counted once
    available = {t["name"]: t["available"] for t in answer["block_types"]}
    reported = {
        name: int(count)
        for count, name in re.findall(r"Architecture\n\t\t(\d+)\tblocks of type: (\w+)", log)
    }
    assert reported and available == reported
    assert available["io"] > available["clb"]


def test_the_clusters_are_the_ones_of_the_netlist(routed):
    log = routed.logs["pack"]
    answer = data(routed.shell.report_cluster())
    by_type = {}
    for cluster in answer["clusters"]:
        by_type[cluster["type"]] = by_type.get(cluster["type"], 0) + 1
    (clb,) = log_numbers(log, r"Netlist clb blocks: (\d+)")
    (io,) = log_numbers(log, r"Netlist io blocks: (\d+)")
    assert by_type == {"clb": int(clb), "io": int(io)}
    assert answer["total"] == answer["matched"] == answer["returned"] == len(answer["clusters"])
    assert not answer["truncated"] and "stats" not in answer


def test_cluster_pin_utilization_is_the_one_of_the_pin_report(routed):
    stats = data(routed.shell.report_cluster(stats=True))["stats"]
    report = Path("packing_pin_util.rpt").read_text()
    for entry in stats["by_type"]:
        section = report.split(f"Type: {entry['type']}\n")[1].split("Type: ")[0]
        inputs = re.search(r"Input Pin Usage:.*?Avg: [\d.]+ \(([\d.]+)\)", section, re.S)
        outputs = re.search(r"Output Pin Usage:.*?Avg: [\d.]+ \(([\d.]+)\)", section, re.S)
        assert entry["average_input_pin_utils"] == pytest.approx(float(inputs.group(1)), abs=0.006)
        assert entry["average_output_pin_utils"] == pytest.approx(
            float(outputs.group(1)), abs=0.006
        )
    assert stats["num_clusters"] == sum(e["count"] for e in stats["by_type"])


def test_the_cluster_query_filters_by_name(routed):
    shell = routed.shell
    everything = data(shell.report_cluster())
    outputs = data(shell.report_cluster(query="out:*"))
    assert outputs["query"] == "out:*"
    assert 0 < outputs["matched"] < everything["total"]
    assert all(c["name"].startswith("out:") for c in outputs["clusters"])
    assert outputs["total"] == everything["total"]
    exact = data(shell.report_cluster(query=everything["clusters"][0]["name"]))
    assert exact["matched"] == 1
    assert data(shell.report_cluster(query="no_such_cluster*"))["clusters"] == []
    # '?' and '[]' are shell patterns too
    assert data(shell.report_cluster(query="out:?*"))["matched"] == outputs["matched"]


def test_a_list_is_cut_at_the_limit(routed):
    shell = routed.shell
    cut = data(shell.report_cluster(limit=2))
    assert cut["returned"] == len(cut["clusters"]) == 2
    assert cut["truncated"] and cut["matched"] > 2
    cut = data(shell.placement(limit=2))
    assert cut["returned"] == len(cut["blocks"]) == 2
    assert cut["truncated"] and cut["total"] > 2
    for command in ("report_cluster", "placement"):
        assert getattr(shell, command)(limit=0, check=False).status != 0


# Placement ---------------------------------------------------------------------------------


def test_placement_is_the_place_file(routed):
    answer = data(routed.shell.placement())
    listed = {b["name"]: (b["x"], b["y"], b["sub_tile"], b["layer"]) for b in answer["blocks"]}
    assert listed == parse_place(Path(f"{routed.name}.place"))
    assert answer["total"] == answer["returned"] == len(listed)
    types = {b["name"]: b["type"] for b in answer["blocks"]}
    assert set(types.values()) == {"io", "clb"}


def test_placement_filters_by_type_and_prefix(routed):
    shell = routed.shell
    everything = data(shell.placement())
    ios = data(shell.placement(type="io"))
    assert ios["filter"] == {"type": "io", "prefix": None}
    assert {b["type"] for b in ios["blocks"]} == {"io"}
    assert ios["total"] == sum(1 for b in everything["blocks"] if b["type"] == "io")
    outs = data(shell.placement(prefix="out:"))
    assert outs["filter"]["prefix"] == "out:"
    assert all(b["name"].startswith("out:") for b in outs["blocks"]) and outs["total"] > 0
    both = data(shell.placement(type="io", prefix="out:"))
    assert both["total"] == outs["total"]
    assert data(shell.placement(type="clb", prefix="out:"))["blocks"] == []
    assert data(shell.placement(type="dsp"))["total"] == 0


# Routing -----------------------------------------------------------------------------------


def test_net_route_is_the_route_file(routed):
    shell = routed.shell
    nets = parse_route(Path(f"{routed.name}.route"))
    assert nets
    for name, expected in nets.items():
        answer = data(shell.net_route(net=name))
        assert answer["net"] == name
        nodes = answer["nodes"]
        # The file writes the node a branch starts from again, for each branch
        unique = []
        for entry in expected:
            if entry[0] not in [e[0] for e in unique]:
                unique.append(entry)
        expected = unique
        assert [n["rr_node"] for n in nodes] == [e[0] for e in expected], name
        for node, (_, kind, low, high, pin) in zip(nodes, expected):
            assert node["type"] == kind
            assert (node["xlow"], node["ylow"], node["layer"]) == low
            assert (node["xhigh"], node["yhigh"]) == high[:2]
            assert node["sink_pin"] == pin
        assert answer["num_sinks"] == sum(1 for n in nodes if n["sink_pin"])


def test_net_route_is_a_tree(routed):
    shell = routed.shell
    for name in parse_route(Path(f"{routed.name}.route")):
        nodes = data(shell.net_route(net=name))["nodes"]
        assert [n["index"] for n in nodes] == list(range(len(nodes)))
        assert nodes[0]["parent"] is None and nodes[0]["type"] == "SOURCE"
        # The node that drives another one comes before it
        assert all(n["parent"] is not None and n["parent"] < n["index"] for n in nodes[1:])
        driven = {n["parent"] for n in nodes[1:]}
        for node in nodes:
            if node["type"] == "SINK":
                assert node["index"] not in driven


def test_net_route_refuses_what_it_cannot_answer(routed):
    shell = routed.shell
    missing = shell.net_route(net="no_such_net", check=False)
    assert missing.status != 0 and missing.data is None
    assert "'no_such_net' does not exist" in missing.log
    assert shell.execute("net_route").status != 0


def test_a_net_that_vpr_does_not_route_has_no_route(shell, and2_files, flow_dir):
    """An ideal clock is not routed"""
    flow = Flow(shell, and2_files, flow_dir, "and2_latch")
    flow.inputs()
    shell.set_app_option(name="general.clock_modeling", value="ideal")
    flow.run("pack", "place", "route")
    clock = shell.net_route(net="clk", check=False)
    assert clock.status != 0 and clock.data is None
    assert "has no routing" in clock.log
    assert shell.net_route(net="a").data["kind"] == "net_route"


def test_congestion_is_the_channel_occupancy_of_vpr(routed):
    answer = data(routed.shell.congestion())
    x = parse_occupancy(Path("chanx_occupancy.txt"))
    y = parse_occupancy(Path("chany_occupancy.txt"))
    summary = answer["summary"]
    assert summary["chanx"]["used"] == sum(used for used, _ in x.values())
    assert summary["chanx"]["capacity"] == sum(cap for _, cap in x.values())
    assert summary["chany"]["used"] == sum(used for used, _ in y.values())
    assert summary["chany"]["capacity"] == sum(cap for _, cap in y.values())
    # One region per tile: the same as the file, tile by tile
    assert answer["region_size"] == 1
    for region in answer["regions"]:
        tile = (region["x_low"], region["y_low"])
        assert tile == (region["x_high"], region["y_high"])
        assert (region["chanx"]["used"], region["chanx"]["capacity"]) == x[tile]
        assert (region["chany"]["used"], region["chany"]["capacity"]) == y[tile]
    listed = {(r["x_low"], r["y_low"]) for r in answer["regions"]}
    assert listed == {t for t in set(x) | set(y) if x.get(t, (0, 0))[0] + y.get(t, (0, 0))[0]}
    assert summary["overused_nodes"] == 0
    # The wires in use are the wirelength of the routing
    (wirelength,) = log_numbers(routed.logs["analysis"], r"Total wirelength: (\d+)")
    assert summary["chanx"]["used"] + summary["chany"]["used"] == wirelength
    assert answer["channel_width"] == {"x": 20, "y": 20}


def test_regions_add_up_to_the_same_channels(routed):
    shell = routed.shell
    fine = data(shell.congestion())
    for size in (2, 3, 10):
        coarse = data(shell.congestion(region_size=size))
        assert coarse["region_size"] == size
        assert coarse["summary"] == fine["summary"]
        for channel in ("chanx", "chany"):
            assert sum(r[channel]["used"] for r in coarse["regions"]) == (
                fine["summary"][channel]["used"]
            )
        for region in coarse["regions"]:
            assert region["x_low"] % size == 0 and region["y_low"] % size == 0
            assert region["x_high"] <= 3 and region["y_high"] <= 3
            for channel in ("chanx", "chany"):
                usage = region[channel]
                expected = usage["used"] / usage["capacity"] if usage["capacity"] else 0
                assert usage["occupancy"] == pytest.approx(expected)
    for bad in (0, -2):
        assert shell.congestion(region_size=bad, check=False).status != 0


# The stages --------------------------------------------------------------------------------


def refused(result, stage, command):
    assert result.status != 0 and result.data is None
    assert f"Cannot run '{command}'" in result.log and f"the stage {stage}" in result.log


def test_a_query_needs_the_stage_that_it_reads(flow):
    shell = flow.shell
    # Nothing has run: everything is refused, with the stage that is missing
    refused(shell.utilization(check=False), "PACKED", "utilization")
    refused(shell.report_cluster(check=False), "PACKED", "report_cluster")
    flow.run("pack")
    assert shell.utilization().data["kind"] == "utilization"
    assert shell.report_cluster().data["kind"] == "cluster_report"
    refused(shell.placement(check=False), "PLACED", "placement")
    refused(shell.timing_summary(check=False), "ROUTED", "timing_summary")
    flow.run("place")
    assert shell.placement().data["kind"] == "placement"
    refused(shell.congestion(check=False), "ROUTED", "congestion")
    refused(shell.critical_paths(check=False), "ROUTED", "critical_paths")
    refused(shell.net_route(net="x", check=False), "ROUTED", "net_route")
    flow.run("route")
    assert shell.timing_summary().data["kind"] == "timing_summary"
    assert shell.congestion().data["kind"] == "congestion"


def test_an_option_that_drops_a_stage_refuses_its_queries(routed):
    shell = routed.shell
    shell.set_app_option(name="place.seed", value=3)
    refused(shell.placement(check=False), "PLACED", "placement")
    refused(shell.timing_summary(check=False), "ROUTED", "timing_summary")
    # The packing is kept
    assert shell.utilization().data["kind"] == "utilization"
    routed.run("place", "route")
    assert shell.placement().data["kind"] == "placement"


def test_the_answers_follow_the_stage_that_runs_again(routed):
    shell = routed.shell
    before = shell.placement().raw_data
    shell.set_app_option(name="place.seed", value=3)
    routed.run("place", "route")
    after = shell.placement().raw_data
    assert after != before
    # What is answered is the placement that is in the files
    answer = data(shell.placement())
    listed = {b["name"]: (b["x"], b["y"], b["sub_tile"], b["layer"]) for b in answer["blocks"]}
    assert listed == parse_place(Path(f"{routed.name}.place"))


def test_the_queries_do_not_change_the_flow(routed):
    shell = routed.shell
    before = [
        getattr(shell, command)().raw_data
        for command in ("timing_summary", "placement", "congestion", "utilization")
    ]
    shell.critical_paths(count=5)
    shell.report_cluster(stats=True)
    # The stages are all still valid, and the answers are the same
    after = [
        getattr(shell, command)().raw_data
        for command in ("timing_summary", "placement", "congestion", "utilization")
    ]
    assert after == before
    assert shell.report_app_option(name="place.seed").data is None


def test_queries_work_after_the_vpr_command(shell, and2_files, flow_dir):
    """The flow of 'vpr' has no stages: the queries read the data of VPR"""
    circuit = flow_dir / "benchmarks" / "ubm" / DESIGNS["and2_latch"]
    if not circuit.exists():
        pytest.skip(f"missing {circuit}")
    shell.read_openfpga_arch(file=and2_files["openfpga_arch"])
    shell.vpr(
        and2_files["vpr_arch"],
        circuit,
        clock_modeling="route",
        device="2x2",
        route_chan_width=20,
        pack=True,
        place=True,
        route=True,
        analysis=True,
    )
    assert data(shell.timing_summary())["setup"]["critical_path_delay_ns"] > 0
    assert data(shell.placement())["total"] > 0
    assert data(shell.congestion())["summary"]["chanx"]["used"] > 0
    assert data(shell.utilization())["block_types"]


def test_the_options_of_the_queries_are_typed(routed):
    shell = routed.shell
    # The shell checks that a number is one
    assert shell.critical_paths(count="x", check=False).status != 0
    assert shell.congestion(region_size="x", check=False).status != 0
    # An option that does not exist
    assert shell.timing_summary(verbose=True, check=False).status != 0
