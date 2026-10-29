"""Run the flow of the task 'basic_tests/generate_fabric' with the Python module of OpenFPGA.

The task runner runs this script when it is the 'openfpga_shell_template' of a task. It is plain
Python: the variables that the scripts of the shell get by substitution, e.g., ${VPR_ARCH_FILE},
are environment variables here. The script can be run by hand, from the directory of the run, with
the same variables set.

Every command of the shell is a method of the shell: shell.read_circuit(file="a.blif") runs
'read_circuit --file a.blif'.

Everything the commands print goes to the standard output, which the task runner keeps as the log
of the shell and reads the results of VPR from.
"""

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


def main():
    env = os.environ
    shell = EchoShell()

    # Run VPR for the design: the whole flow is one command
    shell.vpr(
        env["VPR_ARCH_FILE"],
        env["VPR_TESTBENCH_BLIF"],
        clock_modeling="route",
        device=env["OPENFPGA_VPR_DEVICE_LAYOUT"],
        route_chan_width=env["OPENFPGA_VPR_ROUTE_CHAN_WIDTH"],
    )

    # Read the OpenFPGA architecture and the simulation settings
    shell.read_openfpga_arch(file=env["OPENFPGA_ARCH_FILE"])
    shell.read_openfpga_simulation_setting(file=env["OPENFPGA_SIM_SETTING_FILE"])

    # Annotate the OpenFPGA architecture to the data of VPR
    shell.link_openfpga_arch(activity_file=env["ACTIVITY_FILE"], sort_gsb_chan_node_in_edges=True)
    shell.check_netlist_naming_conflict(fix=True, report="./netlist_renaming.xml")

    # Build the module graph of the fabric
    shell.build_fabric(compress_routing=True)
    shell.write_fabric_hierarchy(file="./fabric_hierarchy.txt")
    shell.write_fabric_io_info(file="./fabric_io_location.xml", verbose=True)

    # Write the netlists and the constraints of the fabric
    shell.write_fabric_verilog(
        file=f"{env['OPENFPGA_VERILOG_OUTPUT_DIR']}/SRC",
        explicit_port_mapping=True,
        include_timing=True,
        print_user_defined_template=True,
        verbose=True,
    )
    shell.write_pnr_sdc(file="./SDC")
    shell.write_sdc_disable_timing_configure_ports(file="./SDC/disable_configure_ports.sdc")


if __name__ == "__main__":
    main()
