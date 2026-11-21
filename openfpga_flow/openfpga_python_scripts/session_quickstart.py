"""Run the flow of OpenFPGA on a small design with the Python module, and look at the results.

    python3 session_quickstart.py [--workdir DIR] [--json]

The module is the one that 'pip install .' installs, or the one that the build makes in
build/openfpga (set PYTHONPATH to it). The default design is a small one from this repository. The
flow runs in a worker process (openfpga.RemoteSession), so that whatever VPR does, this script is
safe. The files of the flow (.net, .place, .route, the checkpoint) are written in --workdir.
"""

import argparse
import json
import sys
import tempfile
import time
from pathlib import Path

try:
    import openfpga
except ImportError as error:
    sys.exit(
        "The module 'openfpga' is not installed: run 'pip install .' in the source of OpenFPGA, "
        "or set PYTHONPATH to the directory build/openfpga after a build with "
        f"-DOPENFPGA_SWIG_PYTHON=ON ({error})"
    )

FLOW = Path(__file__).resolve().parents[1]
DEFAULTS = {
    "vpr_arch": FLOW / "vpr_arch" / "k6_frac_N10_tileable_40nm.xml",
    "openfpga_arch": FLOW / "openfpga_arch" / "k6_frac_N10_40nm_openfpga.xml",
    "circuit": FLOW / "benchmarks" / "ubm" / "simple_gates" / "and2_latch" / "and2_latch.blif",
}


def parse_arguments():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--workdir", type=Path, help="where the files are written (default: new)")
    for name, path in DEFAULTS.items():
        parser.add_argument(f"--{name}", type=Path, default=path)
    parser.add_argument("--layout", default="2x2", help="fixed layout of the device")
    parser.add_argument("--channel-width", type=int, default=20)
    parser.add_argument("--in-process", action="store_true", help="no worker: a Session")
    parser.add_argument("--json", action="store_true", help="print the summary as JSON only")
    return parser.parse_args()


def main():
    arguments = parse_arguments()
    workdir = arguments.workdir or Path(tempfile.mkdtemp(prefix="openfpga_quickstart_"))
    workdir.mkdir(parents=True, exist_ok=True)
    for name in DEFAULTS:
        if not getattr(arguments, name).exists():
            sys.exit(f"The file {getattr(arguments, name)} does not exist")

    if arguments.in_process:
        # The files are written in the current directory of the process
        import os

        os.chdir(workdir)
        session = openfpga.Session(checkpoint_dir=workdir / "checkpoint")
    else:
        session = openfpga.RemoteSession(cwd=workdir, checkpoint_dir=workdir / "checkpoint")

    with session if not arguments.in_process else _nothing(session):
        # The options are typed, and have help, defaults and choices
        session.options["general.clock_modeling"] = "route"
        session.options["router.route_chan_width"] = arguments.channel_width
        session.read_vpr_arch(arguments.vpr_arch.resolve(), layout=arguments.layout)
        session.read_openfpga_arch(arguments.openfpga_arch.resolve())
        session.read_circuit(arguments.circuit.resolve())

        times = {}
        for stage in ("pack", "place", "route", "analysis"):
            started = time.monotonic()
            getattr(session, stage)()
            times[stage] = round(time.monotonic() - started, 3)

        timing = session.timing_summary()
        utilization = session.utilization()
        paths = session.critical_paths(count=3)
        congestion = session.congestion()
        summary = {
            "stages": session.state().reached,
            "seconds": times,
            "fmax_mhz": timing.setup.fmax_mhz,
            "worst_slack_ns": timing.setup.worst_negative_slack_ns,
            "device_utilization": utilization.device_utilization,
            "blocks": {
                block.name: (block.used, block.available) for block in utilization.block_types
            },
            "worst_path": [point.pin for point in paths.paths[0].points] if paths.paths else [],
            "channel_occupancy": {
                "chanx": congestion.summary.chanx.occupancy,
                "chany": congestion.summary.chany.occupancy,
            },
            "workdir": str(workdir),
        }

    if arguments.json:
        print(json.dumps(summary))
        return
    print(f"The flow ran in {workdir}")
    print("Stages reached: " + ", ".join(summary["stages"]))
    print("Seconds: " + ", ".join(f"{name} {value}" for name, value in times.items()))
    print(f"Fmax: {summary['fmax_mhz']:.1f} MHz, worst slack: {summary['worst_slack_ns']} ns")
    print(f"Device utilization: {summary['device_utilization']:.0%}")
    for name, (used, available) in summary["blocks"].items():
        print(f"  {name}: {used} of {available}")
    print("Worst path: " + " -> ".join(summary["worst_path"]))
    print(f"Checkpoint: {workdir / 'checkpoint'} (openfpga.Session.restore() goes on from it)")


class _nothing:
    """A with block that does nothing, for a Session, which needs no closing"""

    def __init__(self, session):
        self.session = session

    def __enter__(self):
        return self.session

    def __exit__(self, *exception):
        return False


if __name__ == "__main__":
    main()
