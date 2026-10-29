"""Run the flow stage by stage with the Python module of OpenFPGA, and read the results between
the stages.

The task runner runs this script when it is the 'openfpga_shell_template' of a task. It is plain
Python: the variables that the scripts of the shell get by substitution, e.g., ${VPR_ARCH_FILE},
are environment variables here. The script can be run by hand, from the directory of the run, with
the same variables set. Every command of the shell is a method of the shell:
shell.read_circuit(file="a.blif") runs 'read_circuit --file a.blif'.

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
    shell = EchoShell()

    # The settings of the stages are application options
    shell.set_app_option(name="general.clock_modeling", value="route")
    shell.set_app_option(name="router.route_chan_width", value=env["OPENFPGA_VPR_ROUTE_CHAN_WIDTH"])

    # The inputs: the device is made once the three of them are read
    shell.read_vpr_arch(file=env["VPR_ARCH_FILE"], layout=env["OPENFPGA_VPR_DEVICE_LAYOUT"])
    shell.read_openfpga_arch(file=env["OPENFPGA_ARCH_FILE"])
    shell.read_circuit(file=env["VPR_TESTBENCH_BLIF"])

    # The stages, one at a time. The results of each one can be read before the next runs
    qor = {}
    for stage in ("pack", "place", "route", "analysis"):
        qor[stage] = read_results(stage, getattr(shell, stage)().log)
    with open("stage_qor.json", "w", encoding="utf-8") as report:
        json.dump(qor, report, indent=2)
        report.write("\n")

    # Link the OpenFPGA architecture to the routed design, and build the fabric
    shell.read_openfpga_simulation_setting(file=env["OPENFPGA_SIM_SETTING_FILE"])
    shell.link_openfpga_arch(activity_file=env["ACTIVITY_FILE"], sort_gsb_chan_node_in_edges=True)
    shell.check_netlist_naming_conflict(fix=True, report="./netlist_renaming.xml")
    shell.build_fabric(compress_routing=True)

    # The bitstream follows the routing: repack, then the bitstream of the architecture and the fabric
    shell.repack()
    shell.build_architecture_bitstream(write_file="fabric_independent_bitstream.xml")
    shell.build_fabric_bitstream()
    shell.write_fabric_bitstream(file="fabric_bitstream.bit", format="plain_text")

    # Write the netlists of the fabric
    shell.write_fabric_verilog(
        file=f"{env['OPENFPGA_VERILOG_OUTPUT_DIR']}/SRC",
        explicit_port_mapping=True,
        include_timing=True,
        print_user_defined_template=True,
        verbose=True,
    )


if __name__ == "__main__":
    main()
