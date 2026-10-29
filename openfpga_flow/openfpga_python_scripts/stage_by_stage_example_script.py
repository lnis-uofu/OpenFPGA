"""Run the flow stage by stage with the Python module of OpenFPGA, and read the results between
the stages.

The task runner runs this script when it is the 'openfpga_shell_template' of a task. It is plain
Python: the variables that the scripts of the shell get by substitution, e.g., ${VPR_ARCH_FILE},
are environment variables here. The script can be run by hand, from the directory of the run, with
the same variables set.

The inputs are read first, then 'pack', 'place', 'route' and 'analysis' run one at a time, with the
application options as their settings. The results of the stages are written to 'stage_qor.json',
and the bitstream is made from the routed design.

Everything the commands print goes to the standard output, which the task runner keeps as the log
of the shell and reads the results of VPR from.
"""

import json
import os
import re
import sys

import openfpga

# What the log of a stage reports, as the pattern of the value
RESULTS = {
    "place": {"estimated_critical_path_delay_ns": r"critical path delay.*?: ([-\d.]+) ns"},
    "route": {
        "channel_width": r"channel width factor of (\d+)",
        "wirelength": r"Total wirelength: (\d+)",
    },
    "analysis": {"critical_path_delay_ns": r"Final critical path delay \(least slack\): ([-\d.]+)"},
}


def run(shell, command):
    """Run a command, print what it printed, and stop the flow if it failed"""
    try:
        result = shell.run(command)
    except openfpga.OpenfpgaError as err:
        sys.stdout.write(err.log)
        raise
    sys.stdout.write(result.log)
    return result


def read_results(stage, log):
    """The values that the log of a stage reports: the last one of each"""
    found = {}
    for name, pattern in RESULTS.get(stage, {}).items():
        values = re.findall(pattern, log)
        if values:
            found[name] = float(values[-1]) if "." in values[-1] else int(values[-1])
    return found


def main():
    env = os.environ
    shell = openfpga.OpenfpgaShell()

    # The settings of the stages are application options
    run(shell, "set_app_option -n general.clock_modeling -v route")
    run(
        shell,
        f"set_app_option -n router.route_chan_width -v {env['OPENFPGA_VPR_ROUTE_CHAN_WIDTH']}",
    )

    # The inputs: the device is made once the three of them are read
    run(shell, f"read_vpr_arch -f {env['VPR_ARCH_FILE']} -l {env['OPENFPGA_VPR_DEVICE_LAYOUT']}")
    run(shell, f"read_openfpga_arch -f {env['OPENFPGA_ARCH_FILE']}")
    run(shell, f"read_circuit -f {env['VPR_TESTBENCH_BLIF']}")

    # The stages, one at a time. The results of each one can be read before the next runs
    qor = {}
    for stage in ("pack", "place", "route", "analysis"):
        qor[stage] = read_results(stage, run(shell, stage).log)
    with open("stage_qor.json", "w", encoding="utf-8") as report:
        json.dump(qor, report, indent=2)
        report.write("\n")

    # Link the OpenFPGA architecture to the routed design, and build the fabric
    run(shell, f"read_openfpga_simulation_setting -f {env['OPENFPGA_SIM_SETTING_FILE']}")
    run(
        shell,
        f"link_openfpga_arch --activity_file {env['ACTIVITY_FILE']} --sort_gsb_chan_node_in_edges",
    )
    run(shell, "check_netlist_naming_conflict --fix --report ./netlist_renaming.xml")
    run(shell, "build_fabric --compress_routing")

    # The bitstream follows the routing: repack, then the bitstream of the architecture and the fabric
    run(shell, "repack")
    run(shell, "build_architecture_bitstream --write_file fabric_independent_bitstream.xml")
    run(shell, "build_fabric_bitstream")
    run(shell, "write_fabric_bitstream --file fabric_bitstream.bit --format plain_text")

    # Write the netlists of the fabric
    run(
        shell,
        f"write_fabric_verilog --file {env['OPENFPGA_VERILOG_OUTPUT_DIR']}/SRC "
        "--explicit_port_mapping --include_timing --print_user_defined_template --verbose",
    )


if __name__ == "__main__":
    main()
