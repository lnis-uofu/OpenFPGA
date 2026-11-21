"""The queries about the design: timing_summary, critical_paths, utilization, placement,
net_route, congestion and report_cluster.

Each answer is checked against what VPR reports for the same run: its log, report_timing.setup.rpt,
the .place and .route files, the channel occupancy files and packing_pin_util.rpt.
"""

import fnmatch
import json
import math
import re
import xml.etree.ElementTree as ET
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
    "find",
    "block_info",
    "net_info",
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
                points.append((match.group(1), float(match.group(4)), float(match.group(3))))
        # The clock to the block that captures the data comes after the arrival of the data
        capture = []
        required = block[arrival_at:]
        for line in required[: required.index("data required time")].splitlines():
            match = re.match(r"^(\S+) \((\.\w+)\).*?\s+(-?[\d.]+)\s+(-?[\d.]+)$", line)
            if match:
                capture.append((match.group(1), float(match.group(4)), float(match.group(3))))
        slack = float(re.search(r"slack \(\w+\)\s+(-?[\d.]+)", block).group(1))
        paths.append((start, end, slack, points, capture))
    return paths


# The answers follow the schemas ------------------------------------------------------------


def test_every_answer_follows_its_schema(routed, validate):
    shell = routed.shell
    net = next(iter(parse_route(Path(f"{routed.name}.route"))))
    block = data(shell.find(pattern="*", kind="block"))["blocks"]["items"][0]["name"]
    answers = [
        shell.timing_summary(),
        shell.critical_paths(count=3),
        shell.utilization(),
        shell.placement(),
        shell.net_route(net=net),
        shell.congestion(),
        shell.congestion(region_size=2),
        shell.congestion(hotspots=3),
        shell.report_cluster(),
        shell.report_cluster(stats=True, query="out:*"),
        shell.find(pattern="*"),
        shell.block_info(block=block),
        shell.net_info(net=net),
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
    # The pairs add up to the sTNS of VPR, and a pair has no more than the worst of its paths
    log = routed.logs["analysis"]
    (tns,) = log_numbers(log, r"Final setup Total Negative Slack \(sTNS\): ([-\d.e]+) ns")
    assert close(sum(d["total_negative_slack_ns"] for d in domains), tns)
    for domain in domains:
        assert domain["total_negative_slack_ns"] <= 0
        assert domain["total_negative_slack_ns"] <= min(0, domain["worst_slack_ns"]) + 1e-9
        assert domain["intra_domain"] == (domain["launch_clock"] == domain["capture_clock"])
        assert (domain["fmax_mhz"] is not None) == domain["intra_domain"]


def test_the_worst_paths_are_the_ones_of_the_timing_report(routed):
    paths = data(routed.shell.critical_paths(count=3))["paths"]
    expected = parse_timing_report(Path("report_timing.setup.rpt"), 3)
    assert len(paths) == len(expected) >= 2
    for path, (start, end, slack, points, capture) in zip(paths, expected):
        assert path["startpoint"] == start
        assert path["endpoint"] == end
        assert path["slack_ns"] == pytest.approx(slack, abs=6e-4)
        # The report lists the clock to the launching block, then the data path: point by point,
        # with the time since the point before
        listed = path["clock_launch_path"] + path["points"]
        assert [p["pin"] for p in listed] == [name for name, _, _ in points]
        for point, (_, arrival, delay) in zip(listed, points):
            assert point["arrival_ns"] == pytest.approx(arrival, abs=6e-4)
            assert point["delay_ns"] == pytest.approx(delay, abs=6e-4)
        assert path["data_arrival_ns"] == pytest.approx(path["points"][-1]["arrival_ns"])
        assert [p["pin"] for p in path["clock_capture_path"]] == [name for name, _, _ in capture]
        for point, (_, arrival, delay) in zip(path["clock_capture_path"], capture):
            assert point["arrival_ns"] == pytest.approx(arrival, abs=6e-4)
            assert point["delay_ns"] == pytest.approx(delay, abs=6e-4)


def test_the_paths_are_worst_first_and_made_of_steps(routed):
    paths = data(routed.shell.critical_paths(count=10))["paths"]
    slacks = [p["slack_ns"] for p in paths]
    assert slacks == sorted(slacks)
    assert [p["index"] for p in paths] == list(range(len(paths)))
    for path in paths:
        points = path["points"]
        assert points[0]["pin"] == path["startpoint"]
        assert points[-1]["pin"] == path["endpoint"]
        # Each point is the time since the point before it, the clock path included
        before = 0.0
        for point in path["clock_launch_path"] + points:
            assert point["delay_ns"] == pytest.approx(point["arrival_ns"] - before, abs=1e-9)
            before = point["arrival_ns"]
        # The slack of a path is what is required less what arrives
        assert path["slack_ns"] == pytest.approx(
            path["data_required_ns"] - path["data_arrival_ns"], abs=1e-6
        )


def test_the_clock_paths_are_there_for_the_flip_flops(routed):
    paths = data(routed.shell.critical_paths(count=100))["paths"]
    for path in paths:
        launch, capture = path["clock_launch_path"], path["clock_capture_path"]
        # The clock is routed in this flow: it goes from its input to the pin of the flip-flop
        launched_by_clock = path["startpoint"].endswith(".Q[0]")
        captured_by_clock = path["endpoint"].endswith(".D[0]")
        assert bool(launch) == launched_by_clock
        assert bool(capture) == captured_by_clock
        if launch:
            assert launch[-1]["pin"] == path["startpoint"].replace(".Q[0]", ".clk[0]")
            # The data leaves the flip-flop when the clock is there
            assert path["points"][0]["arrival_ns"] >= launch[-1]["arrival_ns"]
            assert path["points"][0]["delay_ns"] >= 0
        if capture:
            assert capture[-1]["pin"] == path["endpoint"].replace(".D[0]", ".clk[0]")
    assert any(p["clock_launch_path"] for p in paths)


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


# Find --------------------------------------------------------------------------------------


def test_find_lists_the_blocks_where_the_place_file_has_them(routed):
    answer = data(routed.shell.find(pattern="*"))
    assert answer["kind"] is None and answer["pattern"] == "*"
    blocks = answer["blocks"]
    assert blocks["total"] == blocks["matched"] == blocks["returned"] == len(blocks["items"])
    assert not blocks["truncated"]
    placed = {b["name"]: (b["x"], b["y"], b["sub_tile"], b["layer"]) for b in blocks["items"]}
    assert placed == parse_place(Path(f"{routed.name}.place"))
    assert all(b["placed"] for b in blocks["items"])
    types = {b["name"]: b["type"] for b in data(routed.shell.placement())["blocks"]}
    assert {b["name"]: b["type"] for b in blocks["items"]} == types


def test_find_has_no_location_before_the_placement(flow):
    flow.run("pack")
    blocks = data(flow.shell.find(pattern="*", kind="block"))["blocks"]
    assert blocks["items"]
    for block in blocks["items"]:
        assert block["placed"] is False
        assert (block["x"], block["y"], block["sub_tile"], block["layer"]) == (None,) * 4


def test_find_nets_are_the_nets_that_vpr_routes(routed):
    shell = routed.shell
    nets = data(shell.find(pattern="*", kind="net"))["nets"]
    routed_nets = parse_route(Path(f"{routed.name}.route"))
    # A net that is ignored (a clock) is not routed
    assert {n["name"] for n in nets["items"] if not n["ignored"]} == set(routed_nets)
    assert nets["total"] == nets["matched"] == len(nets["items"])
    for net in nets["items"]:
        if not net["ignored"]:
            assert net["fanout"] == data(shell.net_route(net=net["name"]))["num_sinks"]
        assert net["fanout"] >= 1


def test_find_matches_names_like_a_shell_does(routed):
    shell = routed.shell
    names = [b["name"] for b in data(shell.find(pattern="*"))["blocks"]["items"]]
    nets = [n["name"] for n in data(shell.find(pattern="*"))["nets"]["items"]]
    for pattern in ("out:*", "?*:*", "[io]*", names[0], "no_such_name*"):
        found = data(shell.find(pattern=pattern))
        assert [b["name"] for b in found["blocks"]["items"]] == [
            n for n in names if fnmatch.fnmatchcase(n, pattern)
        ]
        assert [n["name"] for n in found["nets"]["items"]] == [
            n for n in nets if fnmatch.fnmatchcase(n, pattern)
        ]
    assert data(shell.find(pattern="out:*"))["blocks"]["matched"] > 0


def test_find_looks_for_one_kind_and_stops_at_the_limit(routed):
    shell = routed.shell
    blocks = data(shell.find(pattern="*", kind="block"))
    assert blocks["kind"] == "block" and blocks["nets"] is None
    nets = data(shell.find(pattern="*", kind="net"))
    assert nets["kind"] == "net" and nets["blocks"] is None
    cut = data(shell.find(pattern="*", limit=1))
    for part in (cut["blocks"], cut["nets"]):
        assert part["returned"] == len(part["items"]) == 1
        assert part["truncated"] and part["matched"] > 1
    for bad in ({"kind": "pin"}, {"limit": 0}):
        assert shell.find(**{"pattern": "*", **bad}, check=False).status != 0
    assert shell.run("find", check=False).status != 0


def test_find_needs_the_packing(flow):
    refused(flow.shell.find(pattern="*", check=False), "PACKED", "find")


# Block and net information -----------------------------------------------------------------


def packed_blocks(path):
    """The clusters of a .net file: name -> (input nets, atoms), the atoms being the primitives
    in it, as name -> path"""
    root = ET.parse(path).getroot()
    clusters = {}
    for cluster in root.findall("block"):
        inputs = {}
        for port in cluster.find("inputs") or []:
            for bit, net in enumerate(port.text.split()):
                if net != "open":
                    inputs[(port.get("name"), bit)] = net
        atoms = {}

        def walk(block, parents):
            mode = block.get("mode")
            leaf = not block.findall("block")
            here = block.get("instance") + (f"[{mode}]" if mode and not leaf else "")
            if not parents:
                # The cluster has no index in the answer
                here = here.split("[")[0] + (f"[{mode}]" if mode else "")
            if leaf and block.get("name") != "open":
                atoms[block.get("name")] = "/".join(parents + [here])
            for child in block.findall("block"):
                walk(child, parents + [here])

        walk(cluster, [])
        clusters[cluster.get("name")] = (inputs, atoms)
    return clusters


def circuit_atoms(blif):
    """The names of the atoms of a circuit: its inputs, its outputs (named out:...), and the
    outputs of its .names and .latch"""
    names = set()
    text = blif.read_text().replace("\\\n", " ")
    for line in text.splitlines():
        words = line.split()
        if not words:
            continue
        if words[0] == ".inputs":
            names.update(words[1:])
        elif words[0] == ".outputs":
            names.update("out:" + w for w in words[1:])
        elif words[0] == ".names":
            names.add(words[-1])
        elif words[0] == ".latch":
            names.add(words[2])
    return names


def test_a_block_is_what_the_packed_netlist_says(routed):
    shell = routed.shell
    packed = packed_blocks(Path(f"{routed.name}.net"))
    assert packed
    for name, (inputs, atoms) in packed.items():
        answer = data(shell.block_info(block=name))
        assert answer["name"] == name and answer["placed"]
        pins = answer["pins"]
        listed = {
            (p["port"], p["bit"]): p["net"] for p in pins if p["direction"] in ("input", "clock")
        }
        # Every input of the cluster, as the file lists them (the clocks are not in its inputs)
        assert {k: v for k, v in listed.items() if k in inputs} == inputs, name
        assert all(p["net"] for p in pins)
        assert {a["name"]: a["path"] for a in answer["atoms"]} == atoms, name


def test_the_atoms_of_the_blocks_are_the_atoms_of_the_circuit(routed):
    shell = routed.shell
    seen = []
    for name in packed_blocks(Path(f"{routed.name}.net")):
        seen += [a["name"] for a in data(shell.block_info(block=name))["atoms"]]
    assert len(seen) == len(set(seen))
    # VPR sweeps what is not used (a net that nothing reads), so not every name of the file is there
    assert set(seen) <= circuit_atoms(routed.circuit)


def test_the_atoms_have_the_model_of_their_primitive(routed):
    models = {}
    for name in packed_blocks(Path(f"{routed.name}.net")):
        for atom in data(routed.shell.block_info(block=name))["atoms"]:
            primitive = atom["path"].split("/")[-1].split("[")[0]
            models.setdefault(primitive, set()).add(atom["model"])
    expected = {"lut": {".names"}, "ff": {".latch"}, "inpad": {".input"}, "outpad": {".output"}}
    assert models and all(expected[primitive] == found for primitive, found in models.items())
    assert {"inpad", "outpad"} <= set(models)


def test_a_block_has_its_place_once_placed(flow):
    shell = flow.shell
    flow.run("pack")
    name = data(shell.find(pattern="*", kind="block"))["blocks"]["items"][0]["name"]
    before = data(shell.block_info(block=name))
    assert before["placed"] is False and before["location"] is None
    flow.run("place")
    after = data(shell.block_info(block=name))
    assert after["placed"] is True
    placed = parse_place(Path(f"{flow.name}.place"))[name]
    location = after["location"]
    assert (location["x"], location["y"], location["sub_tile"], location["layer"]) == placed
    assert {k: v for k, v in after.items() if k not in ("placed", "location")} == {
        k: v for k, v in before.items() if k not in ("placed", "location")
    }


def test_block_info_refuses_what_it_cannot_answer(routed, flow):
    shell = routed.shell
    missing = shell.block_info(block="no_such_block", check=False)
    assert missing.status != 0 and missing.data is None
    assert "'no_such_block' does not exist" in missing.log
    assert shell.run("block_info", check=False).status != 0


def test_a_net_is_seen_from_its_blocks_and_from_itself(routed):
    shell = routed.shell
    nets = data(shell.find(pattern="*", kind="net"))["nets"]["items"]
    # What each block says of its pins is what the net says of its blocks
    drivers, sinks = {}, {}
    for block in data(shell.find(pattern="*", kind="block"))["blocks"]["items"]:
        for pin in data(shell.block_info(block=block["name"]))["pins"]:
            where = (block["name"], pin["port"], pin["bit"])
            (drivers if pin["direction"] == "output" else sinks).setdefault(pin["net"], []).append(
                where
            )
    for net in nets:
        answer = data(shell.net_info(net=net["name"]))
        assert answer["name"] == net["name"]
        assert answer["fanout"] == net["fanout"] == len(answer["sinks"])
        assert answer["ignored"] == net["ignored"]
        as_tuple = lambda pin: (pin["block"], pin["port"], pin["bit"])
        assert drivers[net["name"]] == [as_tuple(answer["driver"])]
        assert sorted(sinks[net["name"]]) == sorted(as_tuple(p) for p in answer["sinks"])


def test_the_bounding_box_is_the_one_of_the_placed_blocks(routed):
    shell = routed.shell
    placed = parse_place(Path(f"{routed.name}.place"))
    for net in data(shell.find(pattern="*", kind="net"))["nets"]["items"]:
        answer = data(shell.net_info(net=net["name"]))
        blocks = [answer["driver"]["block"]] + [s["block"] for s in answer["sinks"]]
        xs = [placed[b][0] for b in blocks]
        ys = [placed[b][1] for b in blocks]
        box = answer["bounding_box"]
        assert (box["xmin"], box["ymin"], box["xmax"], box["ymax"]) == (
            min(xs),
            min(ys),
            max(xs),
            max(ys),
        )
        assert box["half_perimeter"] == max(xs) - min(xs) + max(ys) - min(ys)


def test_the_routing_of_a_net_adds_up_to_the_wirelength_of_vpr(routed):
    shell = routed.shell
    routes = parse_route(Path(f"{routed.name}.route"))
    total = 0
    for net in data(shell.find(pattern="*", kind="net"))["nets"]["items"]:
        answer = data(shell.net_info(net=net["name"]))
        if net["name"] not in routes:
            assert answer["routing"] is None
            continue
        nodes = {}
        for ident, kind, low, high, _ in routes[net["name"]]:
            nodes[ident] = (kind, low, high)
        wires = {i: v for i, v in nodes.items() if v[0] in ("CHANX", "CHANY")}
        length = sum(h[0] - l[0] + h[1] - l[1] + 1 for _, l, h in wires.values())
        assert answer["routing"] == {
            "nodes": len(nodes),
            "wires": len(wires),
            "wirelength": length,
        }
        total += length
    assert total == int(re.search(r"Total wirelength: (\d+)", routed.logs["analysis"]).group(1))


def test_a_net_on_the_critical_path_is_critical(routed):
    shell = routed.shell
    path = data(shell.critical_paths(count=1))["paths"][0]
    known = {n["name"] for n in data(shell.find(pattern="*", kind="net"))["nets"]["items"]}
    on_path = [p["net"] for p in path["points"] if p["net"] in known]
    assert on_path
    for name in on_path:
        timing = data(shell.net_info(net=name))["timing"]
        assert timing["criticality"] == pytest.approx(1.0, abs=1e-4), name
        assert timing["slack_ns"] == pytest.approx(path["slack_ns"], abs=1e-3), name
    # No net is slower than the worst path, and none is more critical than 1
    for net in data(shell.find(pattern="*", kind="net"))["nets"]["items"]:
        timing = data(shell.net_info(net=net["name"]))["timing"]
        if timing is not None:
            assert 0 <= timing["criticality"] <= 1 + 1e-6
            assert timing["slack_ns"] >= path["slack_ns"] - 1e-3


def test_a_net_tells_what_the_stages_so_far_know(flow):
    shell = flow.shell
    flow.run("pack")
    name = data(shell.find(pattern="*", kind="net"))["nets"]["items"][0]["name"]
    answer = data(shell.net_info(net=name))
    assert (answer["bounding_box"], answer["routing"], answer["timing"]) == (None, None, None)
    flow.run("place")
    answer = data(shell.net_info(net=name))
    assert answer["bounding_box"] is not None
    assert (answer["routing"], answer["timing"]) == (None, None)
    flow.run("route")
    answer = data(shell.net_info(net=name))
    assert answer["routing"] is not None and answer["timing"] is not None


def test_a_net_that_vpr_does_not_route_is_ignored(shell, and2_files, flow_dir):
    flow = Flow(shell, and2_files, flow_dir, "and2_latch")
    flow.inputs()
    shell.set_app_option(name="general.clock_modeling", value="ideal")
    flow.run("pack", "place", "route")
    clock = data(shell.net_info(net="clk"))
    assert clock["ignored"] is True
    assert clock["routing"] is None
    assert data(shell.net_info(net="a"))["ignored"] is False


def test_net_info_refuses_what_it_cannot_answer(routed):
    shell = routed.shell
    missing = shell.net_info(net="no_such_net", check=False)
    assert missing.status != 0 and missing.data is None
    assert "'no_such_net' does not exist" in missing.log
    assert shell.run("net_info", check=False).status != 0


def test_the_information_needs_the_packing(flow):
    refused(flow.shell.block_info(block="x", check=False), "PACKED", "block_info")
    refused(flow.shell.net_info(net="x", check=False), "PACKED", "net_info")


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


def crossings(route_file):
    """The nets that cross each segment of a channel, from the .route file: (channel, x, y) -> nets"""
    found = {}
    for name, nodes in parse_route(route_file).items():
        for _, kind, low, high, _ in nodes:
            if kind not in ("CHANX", "CHANY"):
                continue
            for x in range(low[0], high[0] + 1):
                for y in range(low[1], high[1] + 1):
                    found.setdefault((kind.lower(), x, y), set()).add(name)
    return found


def test_the_hotspots_are_the_fullest_segments_of_the_channels(routed):
    answer = data(routed.shell.congestion(hotspots=1000))
    spots = answer["hotspots"]
    files = {
        "chanx": parse_occupancy(Path("chanx_occupancy.txt")),
        "chany": parse_occupancy(Path("chany_occupancy.txt")),
    }
    # Every segment in use is there, with what VPR wrote for it
    in_use = {
        (channel, x, y): value
        for channel, tiles in files.items()
        for (x, y), value in tiles.items()
        if value[0] > 0
    }
    assert {(s["channel"], s["x"], s["y"]) for s in spots} == set(in_use)
    assert len(spots) == len(in_use)
    for spot in spots:
        used, capacity = in_use[(spot["channel"], spot["x"], spot["y"])]
        assert (spot["used"], spot["capacity"]) == (used, capacity)
        assert spot["occupancy"] == pytest.approx(used / capacity)
        assert spot["overused_nodes"] == 0
    # The fullest first
    keys = [(s["occupancy"], s["used"]) for s in spots]
    assert keys == sorted(keys, reverse=True)
    # The regions of one tile say the same
    assert spots[0]["occupancy"] == max(
        max(r["chanx"]["occupancy"], r["chany"]["occupancy"]) for r in answer["regions"]
    )


def test_the_hotspots_name_the_nets_that_cross_them(routed):
    spots = data(routed.shell.congestion(hotspots=1000))["hotspots"]
    expected = crossings(Path(f"{routed.name}.route"))
    assert {(s["channel"], s["x"], s["y"]) for s in spots} == set(expected)
    for spot in spots:
        nets = expected[(spot["channel"], spot["x"], spot["y"])]
        assert spot["num_nets"] == len(nets)
        assert set(spot["nets"]) == nets and len(spot["nets"]) == len(nets)
        # Each net has at least one wire there
        assert spot["used"] >= spot["num_nets"]


def test_the_hotspots_are_as_many_as_asked(routed):
    shell = routed.shell
    everything = data(shell.congestion(hotspots=1000))["hotspots"]
    for count in (1, 3):
        assert data(shell.congestion(hotspots=count))["hotspots"] == everything[:count]
    assert "hotspots" not in data(shell.congestion())
    assert "hotspots" not in data(shell.congestion(region_size=2))
    # The rest of the answer does not change
    plain = data(shell.congestion())
    asked = data(shell.congestion(hotspots=2))
    assert {k: v for k, v in asked.items() if k != "hotspots"} == plain
    for bad in (0, -1):
        assert shell.congestion(hotspots=bad, check=False).status != 0
    assert shell.run("congestion --hotspots many", check=False).status != 0


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
