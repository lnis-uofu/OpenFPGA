"""Place and route a design with several placement seeds in one shell, keep the seed with the
shortest critical path, and make the bitstream for it.

The task runner runs this script when it is the 'openfpga_shell_template' of a task. It is plain
Python: the variables that the scripts of the shell get by substitution, e.g., ${VPR_ARCH_FILE},
are environment variables here, and the seeds are the list ${OPENFPGA_PLACE_SEEDS}, e.g., 1,2,3.

The inputs are read and the design is packed once. For each seed, 'place.seed' is set and the
design is placed and routed again: changing the option drops the placement, so 'place' runs on
the packing that is kept. The critical path of each run is read with the query 'timing_summary',
which answers in JSON, and all runs are written to 'seed_sweep.json'. The best seed is then placed
and routed again; its critical path must be the one of its run in the sweep (the flow is
deterministic), and the bitstream is made from it.

Everything the commands print goes to the standard output, which the task runner keeps as the log
of the shell and reads the results of VPR from: the log ends with the run of the best seed.
"""

import json
import os
import sys

import openfpga


class EchoShell(openfpga.OpenfpgaShell):
    """A shell that prints what each command printed, and stops the flow if one fails"""

    def run(self, command, check=True):
        try:
            result = super().run(command, check)
        except openfpga.OpenfpgaError as err:
            sys.stdout.write(err.log)
            raise
        sys.stdout.write(result.log)
        return result


def place_and_route(shell, seed):
    """Place and route with a seed; return the QoR of the run"""
    shell.set_app_option(name="place.seed", value=seed)
    shell.place()
    shell.route()
    setup = shell.timing_summary().data["data"]["setup"]
    # The wires in use: the wirelength that the analysis of VPR reports
    wires = shell.congestion().data["data"]["summary"]
    return {
        "seed": seed,
        "critical_path_delay_ns": setup["critical_path_delay_ns"],
        "fmax_mhz": setup["fmax_mhz"],
        "wirelength": wires["chanx"]["used"] + wires["chany"]["used"],
    }


def main():
    env = os.environ
    seeds = [int(seed) for seed in env["OPENFPGA_PLACE_SEEDS"].split(",")]
    shell = EchoShell()

    shell.set_app_option(name="general.clock_modeling", value="route")
    shell.set_app_option(name="router.route_chan_width", value=env["OPENFPGA_VPR_ROUTE_CHAN_WIDTH"])
    shell.read_vpr_arch(file=env["VPR_ARCH_FILE"], layout=env["OPENFPGA_VPR_DEVICE_LAYOUT"])
    shell.read_openfpga_arch(file=env["OPENFPGA_ARCH_FILE"])
    shell.read_circuit(file=env["VPR_TESTBENCH_BLIF"])
    shell.pack()

    runs = [place_and_route(shell, seed) for seed in seeds]
    # The first of the seeds with the shortest critical path
    best = min(runs, key=lambda run: run["critical_path_delay_ns"])

    with open("seed_sweep.json", "w", encoding="utf-8") as report:
        json.dump({"runs": runs, "best_seed": best["seed"]}, report, indent=2)
        report.write("\n")

    final = place_and_route(shell, best["seed"])
    if final != best:
        sys.exit(f"The best seed gives another result when run again: {final} instead of {best}")
    shell.analysis()

    # The bitstream of the best run
    shell.read_openfpga_simulation_setting(file=env["OPENFPGA_SIM_SETTING_FILE"])
    shell.link_openfpga_arch(activity_file=env["ACTIVITY_FILE"], sort_gsb_chan_node_in_edges=True)
    shell.check_netlist_naming_conflict(fix=True, report="./netlist_renaming.xml")
    shell.build_fabric(compress_routing=True)
    shell.repack()
    shell.build_architecture_bitstream(write_file="fabric_independent_bitstream.xml")
    shell.build_fabric_bitstream()
    shell.write_fabric_bitstream(file="fabric_bitstream.bit", format="plain_text")


if __name__ == "__main__":
    main()
