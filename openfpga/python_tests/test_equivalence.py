"""The flow that is run stage by stage gives the same results as the flow of
'vpr', on a set of golden designs: the files of VPR, the quality of results and
the bitstream.

The designs are the ones of the repository that need no synthesis. Each one
runs on the architectures given with it, on a fixed layout (the stage by stage
flow needs one). The pairs of architectures are in GOLDEN.
"""

import os
import re
from pathlib import Path

import pytest

FLOW = Path(__file__).resolve().parents[2] / "openfpga_flow"
UBM = FLOW / "benchmarks" / "ubm"

K6 = ("k6_frac_N10_tileable_40nm.xml", "k6_frac_N10_40nm_openfpga.xml")
K4 = ("k4_N4_tileable_40nm.xml", "k4_N4_40nm_cc_openfpga.xml")

# name: (architectures, circuit, layout, channel width)
GOLDEN = {
    "k6_and2": (K6, "simple_gates/and2/and2.blif", "2x2", 20),
    "k6_and2_or2": (K6, "simple_gates/and2_or2/and2_or2.blif", "2x2", 20),
    "k6_and2_latch": (K6, "simple_gates/and2_latch/and2_latch.blif", "2x2", 20),
    "k6_or2": (K6, "simple_gates/or2/or2.blif", "2x2", 20),
    "k6_routing_test": (K6, "interface/routing_test/routing_test.blif", "2x2", 20),
    "k4_and2_or2": (K4, "simple_gates/and2_or2/and2_or2.blif", "2x2", 20),
    "k4_routing_test": (K4, "interface/routing_test/routing_test.blif", "4x4", 30),
}

# The files VPR writes, named after the circuit
VPR_FILES = ["{}.net", "{}.place", "{}.route", "{}.net.post_routing"]

# What the logs report about the quality of the results
QOR = {
    "critical_path_delay": r"Final critical path delay \(least slack\): ([-\d.]+)",
    "wirelength": r"Total wirelength: (\d+)",
    "channel_width": r"channel width factor of (\d+)",
    "clb_blocks": r"Netlist clb blocks: (\d+)",
    "io_blocks": r"Netlist io blocks: (\d+)",
}


def qor(log):
    found = {}
    for key, pattern in QOR.items():
        found[key] = re.findall(pattern, log)
    return found


class Golden:
    """A golden design, with the flows that are run on it"""

    def __init__(self, name):
        (arch, openfpga_arch), circuit, self.layout, self.chan_width = GOLDEN[name]
        self.name = Path(circuit).stem
        self.vpr_arch = FLOW / "vpr_arch" / arch
        self.openfpga_arch = FLOW / "openfpga_arch" / openfpga_arch
        self.circuit = UBM / circuit
        self.sim_setting = FLOW / "openfpga_simulation_settings" / "fixed_sim_openfpga.xml"
        for path in (self.vpr_arch, self.openfpga_arch, self.circuit, self.sim_setting):
            if not path.exists():
                pytest.skip(f"missing {path}")

    def vpr_command(self, extra=""):
        return (
            f"vpr {self.vpr_arch} {self.circuit} --clock_modeling route "
            f"--device {self.layout} --route_chan_width {self.chan_width} "
            f"--pack --place --route --analysis {extra}"
        )

    def link(self, shell):
        shell.run(f"read_openfpga_simulation_setting -f {self.sim_setting}")
        shell.run("link_openfpga_arch --sort_gsb_chan_node_in_edges")
        shell.run("build_fabric --compress_routing")

    def bitstream(self, shell):
        shell.run("repack")
        shell.run("build_architecture_bitstream")
        shell.run("build_fabric_bitstream")
        shell.run(
            "write_fabric_bitstream --file fabric.bit --format plain_text "
            "--no_time_stamp"
        )
        return Path("fabric.bit").read_bytes()

    def run_staged(self, shell):
        """The inputs, then the stages one by one. Return the log of the stages"""
        shell.run("set_app_option -n general.clock_modeling -v route")
        shell.run(f"set_app_option -n router.route_chan_width -v {self.chan_width}")
        shell.run(f"read_vpr_arch -f {self.vpr_arch} -l {self.layout}")
        shell.run(f"read_openfpga_arch -f {self.openfpga_arch}")
        shell.run(f"read_circuit -f {self.circuit}")
        log = ""
        for stage in ("pack", "place", "route", "analysis"):
            log += shell.run(stage).log
        self.link(shell)
        return log

    def run_monolithic(self, shell, extra=""):
        shell.run(f"read_openfpga_arch -f {self.openfpga_arch}")
        log = shell.run(self.vpr_command(extra)).log
        self.link(shell)
        return log

    def files(self):
        return {
            pattern.format(self.name): Path(pattern.format(self.name)).read_bytes()
            for pattern in VPR_FILES
        }


@pytest.fixture(params=sorted(GOLDEN))
def golden(request):
    return Golden(request.param)


@pytest.fixture
def two_shells(openfpga_module, tmp_path):
    """Two shells, each in its own directory: the data of a flow is not
    released, so each flow has its own"""
    staged_dir = tmp_path / "staged"
    monolithic_dir = tmp_path / "monolithic"
    staged_dir.mkdir()
    monolithic_dir.mkdir()
    shells = (openfpga_module.OpenfpgaShell(), openfpga_module.OpenfpgaShell())
    yield shells, (staged_dir, monolithic_dir)
    for shell in shells:
        shell.reset()


def in_dir(directory, function, *args):
    cwd = os.getcwd()
    os.chdir(directory)
    try:
        return function(*args)
    finally:
        os.chdir(cwd)


def test_staged_flow_equals_the_vpr_flow(golden, two_shells):
    (staged, monolithic), (staged_dir, monolithic_dir) = two_shells

    staged_log = in_dir(staged_dir, golden.run_staged, staged)
    staged_files = in_dir(staged_dir, golden.files)
    staged_bits = in_dir(staged_dir, golden.bitstream, staged)

    monolithic_log = in_dir(monolithic_dir, golden.run_monolithic, monolithic)
    monolithic_files = in_dir(monolithic_dir, golden.files)
    monolithic_bits = in_dir(monolithic_dir, golden.bitstream, monolithic)

    assert staged_files == monolithic_files
    staged_qor = qor(staged_log)
    assert staged_qor == qor(monolithic_log)
    # The comparison means something only if the logs report the results
    assert staged_qor["wirelength"], "no wirelength in the log"
    assert staged_qor["critical_path_delay"] or "latch" in golden.name
    assert staged_bits == monolithic_bits
    assert len(staged_bits) > 0


@pytest.mark.parametrize("name", ["k6_routing_test", "k4_routing_test"])
def test_option_changed_after_the_bitstream_equals_the_vpr_flow(name, two_shells):
    """Place again with another seed once the bitstream is made: what follows is
    made again, and is what 'vpr' makes with that seed"""
    golden = Golden(name)
    (staged, monolithic), (staged_dir, monolithic_dir) = two_shells

    first_files = {}

    def scenario():
        golden.run_staged(staged)
        golden.bitstream(staged)
        first_files.update(golden.files())
        staged.run("set_app_option -n place.seed -v 3")
        log = ""
        for stage in ("place", "route", "analysis"):
            log += staged.run(stage).log
        golden.link(staged)
        return log, golden.files(), golden.bitstream(staged)

    staged_log, staged_files, staged_bits = in_dir(staged_dir, scenario)

    def reference():
        log = golden.run_monolithic(monolithic, "--seed 3")
        return log, golden.files(), golden.bitstream(monolithic)

    monolithic_log, monolithic_files, monolithic_bits = in_dir(
        monolithic_dir, reference
    )

    assert staged_files == monolithic_files
    assert qor(staged_log) == qor(monolithic_log)
    assert staged_bits == monolithic_bits
    # The scenario means something only if the seed changes the placement
    place = f"{golden.name}.place"
    assert first_files[place] != staged_files[place]
