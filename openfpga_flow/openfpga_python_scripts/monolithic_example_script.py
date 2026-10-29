"""Run the flow of the task 'basic_tests/generate_fabric' with the Python module of OpenFPGA.

The task runner runs this script when it is the 'openfpga_shell_template' of a task. It is plain
Python: the variables that the scripts of the shell get by substitution, e.g., ${VPR_ARCH_FILE},
are environment variables here. The script can be run by hand, from the directory of the run, with
the same variables set.

Everything the commands print goes to the standard output, which the task runner keeps as the log
of the shell and reads the results of VPR from.
"""

import os
import sys

import openfpga


def run(shell, command):
    """Run a command, print what it printed, and stop the flow if it failed"""
    try:
        result = shell.run(command)
    except openfpga.OpenfpgaError as err:
        sys.stdout.write(err.log)
        raise
    sys.stdout.write(result.log)
    return result


def main():
    env = os.environ
    shell = openfpga.OpenfpgaShell()

    # Run VPR for the design: the whole flow is one command
    run(
        shell,
        f"vpr {env['VPR_ARCH_FILE']} {env['VPR_TESTBENCH_BLIF']} --clock_modeling route "
        f"--device {env['OPENFPGA_VPR_DEVICE_LAYOUT']} "
        f"--route_chan_width {env['OPENFPGA_VPR_ROUTE_CHAN_WIDTH']}",
    )

    # Read the OpenFPGA architecture and the simulation settings
    run(shell, f"read_openfpga_arch -f {env['OPENFPGA_ARCH_FILE']}")
    run(shell, f"read_openfpga_simulation_setting -f {env['OPENFPGA_SIM_SETTING_FILE']}")

    # Annotate the OpenFPGA architecture to the data of VPR
    run(
        shell,
        f"link_openfpga_arch --activity_file {env['ACTIVITY_FILE']} --sort_gsb_chan_node_in_edges",
    )
    run(shell, "check_netlist_naming_conflict --fix --report ./netlist_renaming.xml")

    # Build the module graph of the fabric
    run(shell, "build_fabric --compress_routing")
    run(shell, "write_fabric_hierarchy --file ./fabric_hierarchy.txt")
    run(shell, "write_fabric_io_info --file ./fabric_io_location.xml --verbose")

    # Write the netlists and the constraints of the fabric
    run(
        shell,
        f"write_fabric_verilog --file {env['OPENFPGA_VERILOG_OUTPUT_DIR']}/SRC "
        "--explicit_port_mapping --include_timing --print_user_defined_template --verbose",
    )
    run(shell, "write_pnr_sdc --file ./SDC")
    run(shell, "write_sdc_disable_timing_configure_ports --file ./SDC/disable_configure_ports.sdc")


if __name__ == "__main__":
    main()
