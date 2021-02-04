# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Script Name   : run_fpga_flow.py
# Description   : This script designed to run different flows supported by
#                 OpensFPGA project.
# Args          : python3 run_fpga_flow.py --help
# Author        : Ganesh Gore
# Email         : ganesh.gore@utah.edu
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

import os
import sys
import shutil
import time
import traceback
from datetime import timedelta
import shlex
import glob
import json
import argparse
from configparser import ConfigParser, ExtendedInterpolation
import logging
from envyaml import EnvYAML
import glob
import subprocess
import threading
from string import Template
import re
import xml.etree.ElementTree as ET
from importlib import util
if util.find_spec("humanize"):
    import humanize

if sys.version_info[0] < 3:
    raise Exception("run_fpga_task script must be using Python 3")

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Initialise general paths for the script
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Copy directory where flow file exist
flow_script_dir = os.path.dirname(os.path.abspath(__file__))
# Find OpenFPGA base directory
openfpga_base_dir = os.path.abspath(
    os.path.join(flow_script_dir, os.pardir, os.pardir))
# Copy directory from where script is laucnhed
# [req to resolve relative paths provided while launching script]
launch_dir = os.getcwd()

# Path section to append in configuration file to interpolate path
task_script_dir = os.path.dirname(os.path.abspath(__file__))
script_env_vars = ({"PATH": {
    "OPENFPGA_FLOW_PATH": task_script_dir,
    "ARCH_PATH": os.path.join("${PATH:OPENFPGA_PATH}", "arch"),
    "OPENFPGA_SHELLSCRIPT_PATH": os.path.join("${PATH:OPENFPGA_PATH}", "OpenFPGAShellScripts"),
    "BENCH_PATH": os.path.join("${PATH:OPENFPGA_PATH}", "benchmarks"),
    "TECH_PATH": os.path.join("${PATH:OPENFPGA_PATH}", "tech"),
    "SPICENETLIST_PATH": os.path.join("${PATH:OPENFPGA_PATH}", "SpiceNetlists"),
    "VERILOG_PATH": os.path.join("${PATH:OPENFPGA_PATH}", "VerilogNetlists"),
    "OPENFPGA_PATH": os.path.abspath(os.path.join(task_script_dir, os.pardir,
                                                  os.pardir))}})

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Reading command-line argument
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

# Helper function to provide better alignment to help print


def formatter(prog): return argparse.HelpFormatter(prog, max_help_position=60)


parser = argparse.ArgumentParser(formatter_class=formatter)

# Mandatory arguments
parser.add_argument('arch_file', type=str)
parser.add_argument('benchmark_files', type=str, nargs='+')
# parser.add_argument('extraArgs', nargs=argparse.REMAINDER)
parser.add_argument('otherthings', nargs='*')

# Optional arguments
parser.add_argument('--top_module', type=str, default="top")
parser.add_argument('--fpga_flow', type=str, default="yosys_vpr")
parser.add_argument('--flow_config', type=str,
                    help="CAD tools path overrides default setting")
parser.add_argument('--run_dir', type=str,
                    default=os.path.join(openfpga_base_dir,  'tmp'),
                    help="Directory to store intermidiate file & final results")
parser.add_argument('--openfpga_shell_template', type=str,
                    default=os.path.join("openfpga_flow",
                                         "openfpga_shell_scripts",
                                         "example_script.openfpga"),
                    help="Sample openfpga shell script")
parser.add_argument('--openfpga_arch_file', type=str,
                    help="Openfpga architecture file for shell")
parser.add_argument('--arch_variable_file', type=str, default=None,
                    help="Openfpga architecture file for shell")
# parser.add_argument('--openfpga_sim_setting_file', type=str,
#                     help="Openfpga simulation file for shell")
# parser.add_argument('--external_fabric_key_file', type=str,
#                     help="Key file for shell")
parser.add_argument('--yosys_tmpl', type=str, default=None,
                    help="Alternate yosys template, generates top_module.blif")
parser.add_argument('--disp', action="store_true",
                    help="Open display while running VPR")
parser.add_argument('--debug', action="store_true",
                    help="Run script in debug mode")

# Blif_VPR Only flow arguments
parser.add_argument('--activity_file', type=str,
                    help="Activity file used while running yosys flow")
parser.add_argument('--base_verilog', type=str,
                    help="Original Verilog file to run verification in " +
                    "blif_VPR flow")

# ACE2 and power estimation related arguments
parser.add_argument('--K', type=int,
                    help="LUT Size, if not specified extracted from arch file")
parser.add_argument('--power', action='store_true')
parser.add_argument('--power_tech', type=str,
                    help="Power tech xml file for power calculation")
parser.add_argument('--ace_d', type=float,
                    help="Specify the default signal density of PIs in ACE2")
parser.add_argument('--ace_p', type=float,
                    help="Specify the default signal probablity of PIs in ACE2")
parser.add_argument('--black_box_ace', action='store_true')

# VPR Options
parser.add_argument('--min_route_chan_width', type=float,
                    help="Turn on min_route_chan_width")
parser.add_argument('--max_route_width_retry', type=int, default=100,
                    help="Maximum iterations to perform to reroute")
parser.add_argument('--fix_route_chan_width', type=int,
                    help="Turn on fix_route_chan_width")
parser.add_argument('--vpr_timing_pack_off', action='store_true',
                    help="Turn off the timing-driven pack for vpr")
parser.add_argument('--vpr_place_clb_pin_remap', action='store_true',
                    help="Turn on place_clb_pin_remap in VPR")
parser.add_argument('--vpr_max_router_iteration', type=int,
                    help="Specify the max router iteration in VPR")
parser.add_argument('--vpr_route_breadthfirst', action='store_true',
                    help="Use the breadth-first routing algorithm of VPR")
parser.add_argument('--vpr_use_tileable_route_chan_width', action='store_true',
                    help="Turn on the conversion to " +
                    "tileable_route_chan_width in VPR")

#  VPR - FPGA-X2P Extension
X2PParse = parser.add_argument_group('VPR-FPGA-X2P Extension')
X2PParse.add_argument('--vpr_fpga_x2p_rename_illegal_port', action='store_true',
                      help="Rename illegal ports option of VPR FPGA SPICE")
X2PParse.add_argument('--vpr_fpga_x2p_signal_density_weight', type=float,
                      help="Specify the signal_density_weight of VPR FPGA SPICE")
X2PParse.add_argument('--vpr_fpga_x2p_sim_window_size', type=float,
                      help="specify the sim_window_size of VPR FPGA SPICE")
X2PParse.add_argument('--vpr_fpga_x2p_compact_routing_hierarchy',
                      action="store_true", help="Compact_routing_hierarchy")
X2PParse.add_argument('--vpr_fpga_x2p_duplicate_grid_pin', action="store_true",
                      help="Added duplicated grid pin")

# VPR - FPGA-SPICE Extension
SPParse = parser.add_argument_group('FPGA-SPICE Extension')
SPParse.add_argument('--vpr_fpga_spice', action='store_true',
                     help="Print SPICE netlists in VPR")
SPParse.add_argument('--vpr_fpga_spice_sim_mt_num', type=int,
                     help="Specify the option sim_mt_num of VPR FPGA SPICE")
SPParse.add_argument('--vpr_fpga_spice_print_component_tb', action='store_true',
                     help="Output component-level testbench")
SPParse.add_argument('--vpr_fpga_spice_print_grid_tb', action='store_true',
                     help="Output grid-level testbench")
SPParse.add_argument('--vpr_fpga_spice_print_top_testbench', action='store_true',
                     help="Output full-chip-level testbench")
SPParse.add_argument('--vpr_fpga_spice_leakage_only', action='store_true',
                     help="Turn on leakage_only mode in VPR FPGA SPICE")
SPParse.add_argument('--vpr_fpga_spice_parasitic_net_estimation_off',
                     action='store_true',
                     help="Turn off parasitic_net_estimation in VPR FPGA SPICE")
SPParse.add_argument('--vpr_fpga_spice_testbench_load_extraction_off',
                     action='store_true',
                     help="Turn off testbench_load_extraction in VPR FPGA SPICE")
SPParse.add_argument('--vpr_fpga_spice_simulator_path', type=str,
                     help="Specify simulator path")

# VPR - FPGA-Verilog Extension
VeriPar = parser.add_argument_group('FPGA-Verilog Extension')
VeriPar.add_argument('--vpr_fpga_verilog', action='store_true',
                     help="Generator verilog of VPR FPGA SPICE")
VeriPar.add_argument('--vpr_fpga_verilog_dir', type=str,
                     help="path to store generated verilog files")
VeriPar.add_argument('--vpr_fpga_verilog_include_timing', action="store_true",
                     help="Print delay specification in Verilog files")
VeriPar.add_argument('--vpr_fpga_verilog_include_signal_init',
                     action="store_true",
                     help="Print signal initialization in Verilog files")
VeriPar.add_argument('--vpr_fpga_verilog_print_autocheck_top_testbench',
                     action="store_true", help="Print autochecked top-level " +
                     "testbench for Verilog Generator of VPR FPGA SPICE")
VeriPar.add_argument('--vpr_fpga_verilog_formal_verification_top_netlist',
                     action="store_true", help="Print formal top Verilog files")
VeriPar.add_argument('--vpr_fpga_verilog_include_icarus_simulator',
                     action="store_true", help="dd syntax and definition" +
                     " required to use Icarus Verilog simulator")
VeriPar.add_argument('--vpr_fpga_verilog_print_user_defined_template',
                     action="store_true", help="Unknown parameter")
VeriPar.add_argument('--vpr_fpga_verilog_print_report_timing_tcl',
                     action="store_true", help="Generate tcl script useful " +
                     "for timing report generation")
VeriPar.add_argument('--vpr_fpga_verilog_report_timing_rpt_path',
                     type=str, help="Specify path for report timing results")
VeriPar.add_argument('--vpr_fpga_verilog_print_sdc_pnr', action="store_true",
                     help="Generate sdc file to constraint Hardware P&R")
VeriPar.add_argument('--vpr_fpga_verilog_print_sdc_analysis',
                     action="store_true", help="Generate sdc file to do STA")
VeriPar.add_argument('--vpr_fpga_verilog_print_top_tb', action="store_true",
                     help="Print top-level testbench for Verilog Generator " +
                     "of VPR FPGA SPICE")
VeriPar.add_argument('--vpr_fpga_verilog_print_input_blif_tb',
                     action="store_true", help="Print testbench" +
                     "for input blif file in Verilog Generator")
VeriPar.add_argument('--vpr_fpga_verilog_print_simulation_ini', action="store_true",
                     help="Create simulation INI file")
VeriPar.add_argument('--vpr_fpga_verilog_explicit_mapping', action="store_true",
                     help="Explicit Mapping")

# VPR - FPGA-Bitstream Extension
BSparse = parser.add_argument_group('FPGA-Bitstream Extension')
BSparse.add_argument('--vpr_fpga_bitstream_generator', action="store_true",
                     help="Generate FPGA-SPICE bitstream")

# Regression test option
RegParse = parser.add_argument_group('Regression Test Extension')
RegParse.add_argument("--end_flow_with_test", action="store_true",
                      help="Run verification test at the end")


# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Global varaibles declaration
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Setting up print and logging system
logging.basicConfig(level=logging.INFO, stream=sys.stdout,
                    format='%(levelname)s - %(message)s')
logger = logging.getLogger('OpenFPGA_Flow_Logs')

# variable to store script_configuration and cad tool paths
config, cad_tools = None, None
ExecTime = {}

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Main program starts here
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


def main():
    logger.debug("Script Launched in "+os.getcwd())
    check_required_file()
    read_script_config()
    validate_command_line_arguments()
    prepare_run_directory(args.run_dir)
    if (args.fpga_flow == "yosys_vpr"):
        logger.info('Running "yosys_vpr" Flow')
        run_yosys_with_abc()
        # TODO Make it optional if activity file is provided
        if args.power:
            run_ace2()
            run_pro_blif_3arg()
            run_rewrite_verilog()
    if (args.fpga_flow == "vpr_blif"):
        collect_files_for_vpr()
    logger.info("Runing OpenFPGA Shell Engine ")
    run_openfpga_shell()
    if args.end_flow_with_test:
        run_netlists_verification()

    ExecTime["End"] = time.time()
    def timestr(x): return humanize.naturaldelta(timedelta(seconds=x)) \
        if "humanize" in sys.modules else str(int(x)) + " Sec "
    TimeInfo = ("Openfpga_flow completed, " +
                "Total Time Taken %s " %
                timestr(ExecTime["End"]-ExecTime["Start"]) +
                "VPR Time %s " %
                timestr(ExecTime["VPREnd"]-ExecTime["VPRStart"]))
    TimeInfo += ("Verification Time %s " %
                 timestr(ExecTime["VerificationEnd"] -
                         ExecTime["VerificationStart"])
                 if args.end_flow_with_test else "")
    logger.info(TimeInfo)
    exit()

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Subroutines starts here
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


def check_required_file():
    """ Function ensure existace of all required files for the script """
    files_dict = {
        "CAD TOOL PATH": os.path.join(flow_script_dir, os.pardir, 'misc',
                                      'fpgaflow_default_tool_path.conf'),
    }
    for filename, filepath in files_dict.items():
        if not os.path.isfile(filepath):
            clean_up_and_exit("Not able to locate deafult file " + filename)


def read_script_config():
    """ This fucntion reads default CAD tools path from configuration file """
    global config, cad_tools
    config = ConfigParser(interpolation=ExtendedInterpolation())
    config.read_dict(script_env_vars)
    default_cad_tool_conf = os.path.join(flow_script_dir, os.pardir, 'misc',
                                         'fpgaflow_default_tool_path.conf')
    config.read_file(open(default_cad_tool_conf))
    if args.flow_config:
        config.read_file(open(args.flow_config))
    if not "CAD_TOOLS_PATH" in config.sections():
        clean_up_and_exit("Missing CAD_TOOLS_PATH in openfpga_flow config")
    cad_tools = config["CAD_TOOLS_PATH"]

    if args.arch_variable_file:
        _, file_extension = os.path.splitext(args.arch_variable_file)
        if file_extension in [".yml", ".yaml"]:
            script_env_vars["PATH"].update(
                EnvYAML(args.arch_variable_file, include_environment=False))
        if file_extension in [".json", ]:
            with open(args.arch_variable_file, "r") as fp:
                script_env_vars["PATH"].update(json.load(fp))


def validate_command_line_arguments():
    '''
    This function validate the command line arguments
    FLOW_SCRIPT_CONFIG->valid_flows :
        Key is used to validate if the request flow is supported by the script
    CMD_ARGUMENT_DEPENDANCY :
        Validates the dependencies of the command arguments

    Checks the following file exists and replaces them with an absolute path
    - All architecture files
    - Benchmark files
    - Power tech files
    - Run directory
    - Activity file
    - Base verilog file
    '''
    logger.info("Validating commnad line arguments")

    if args.debug:
        logger.info("Setting loggger in debug mode")
        logger.setLevel(logging.DEBUG)

    # Check if flow supported
    if not args.fpga_flow in config.get("FLOW_SCRIPT_CONFIG", "valid_flows"):
        clean_up_and_exit("%s Flow not supported" % args.fpga_flow)

    # Check if argument list is consistant
    for eacharg, dependent in config.items("CMD_ARGUMENT_DEPENDANCY"):
        if getattr(args, eacharg, None):
            dependent = dependent.split(",")
            for eachdep in dependent:
                if not any([getattr(args, i, 0) for i in eachdep.split("|")]):
                    clean_up_and_exit("'%s' argument depends on (%s) argumets" %
                                      (eacharg, ", ".join(dependent).replace("|", " or ")))

    # Check if architecrue files exists
    args.arch_file = os.path.abspath(args.arch_file)
    if not os.path.isfile(args.arch_file):
        clean_up_and_exit(
            "VPR architecture file not found. -%s",
            args.arch_file)
    args.openfpga_arch_file = os.path.abspath(args.openfpga_arch_file)
    if not os.path.isfile(args.openfpga_arch_file):
        clean_up_and_exit(
            "OpenFPGA architecture file not found. -%s",
            args.openfpga_arch_file)

    # Filter provided benchmark files
    for index, everyinput in enumerate(args.benchmark_files):
        args.benchmark_files[index] = os.path.abspath(everyinput)
        if os.path.isdir(args.benchmark_files[index]):
            logger.warning("Skipping directory in bench %s" % everyinput)
            logger.warning("Directory is not support in benchmark list" +
                           "use wildcard pattern to add files")
            continue
        for everyfile in glob.glob(args.benchmark_files[index]):
            if not os.path.isfile(everyfile):
                clean_up_and_exit(
                    "Failed to copy benchmark file-%s", args.arch_file)

    # Filter provided powertech files
    if args.power_tech:
        args.power_tech = os.path.abspath(args.power_tech)
        if not os.path.isfile(args.power_tech):
            clean_up_and_exit(
                "Power Tech file not found. -%s", args.power_tech)

    # Expand run directory to absolute path
    args.run_dir = os.path.abspath(args.run_dir)
    if args.activity_file:
        args.activity_file = os.path.abspath(args.activity_file)
    if args.base_verilog:
        args.base_verilog = os.path.abspath(args.base_verilog)


def prepare_run_directory(run_dir):
    """
    Prepares run directory to run
    1. Change current directory to run_dir
    2. Copy architecture XML file to run_dir
    3. Copy circuit files to run_dir
    """
    logger.info("Run directory : %s" % run_dir)
    if os.path.isdir(run_dir):
        no_of_files = len(next(os.walk(run_dir))[2])
        shutil.rmtree(run_dir)
    os.makedirs(run_dir)
    # Clean run_dir is created change working directory
    os.chdir(run_dir)

    # Create arch dir in run_dir and copy flattened architecture file
    os.mkdir("arch")
    tmpl = Template(
        open(args.arch_file, encoding='utf-8').read())
    arch_filename = os.path.basename(args.arch_file)
    args.arch_file = os.path.join(run_dir, "arch", arch_filename)
    with open(args.arch_file, 'w', encoding='utf-8') as archfile:
        archfile.write(tmpl.safe_substitute(script_env_vars["PATH"]))

    if (args.openfpga_arch_file):
        tmpl = Template(
            open(args.openfpga_arch_file, encoding='utf-8').read())
        arch_filename = os.path.basename(args.openfpga_arch_file)
        args.openfpga_arch_file = os.path.join(run_dir, "arch", arch_filename)
        with open(args.openfpga_arch_file, 'w', encoding='utf-8') as archfile:
            archfile.write(tmpl.safe_substitute(script_env_vars["PATH"]))

    # Sanitize provided openshell template, if provided
    if (args.openfpga_shell_template):
        if not os.path.isfile(args.openfpga_shell_template or ""):
            logger.error("Openfpga shell file - %s" %
                         args.openfpga_shell_template)
            clean_up_and_exit("Provided openfpga_shell_template" +
                              f" {args.openfpga_shell_template} file not found")
        else:
            shutil.copy(args.openfpga_shell_template,
                        args.top_module+"_template.openfpga")

    # Create benchmark dir in run_dir and copy flattern architecture file
    os.mkdir("benchmark")
    try:
        for index, eachfile in enumerate(args.benchmark_files):
            args.benchmark_files[index] = shutil.copy2(
                eachfile, os.path.join(os.curdir, "benchmark"))
    except:
        logger.exception("Failed to copy all benchmark file to run_dir")


def clean_up_and_exit(msg, clean=False):
    logger.error("Current working directory : " + os.getcwd())
    logger.error(msg)
    logger.error("Exiting . . . . . .")
    exit(1)


def run_yosys_with_abc():
    """
    Execute yosys with ABC and optional blackbox support
    """
    tree = ET.parse(args.arch_file)
    root = tree.getroot()
    try:
        lut_size = max([int(pb_type.find("input").get("num_pins"))
                        for pb_type in root.iter("pb_type")
                        if pb_type.get("class") == "lut"])
        logger.info("Extracted lut_size size from arch XML = %s", lut_size)
        logger.info("Running Yosys with lut_size = %s", lut_size)
    except:
        logger.exception("Failed to extract lut_size from XML file")
        clean_up_and_exit("")
    args.K = lut_size
    # Yosys script parameter mapping
    ys_params = {
        "READ_VERILOG_FILE": " \n".join([
            "read_verilog -nolatches " + shlex.quote(eachfile)
            for eachfile in args.benchmark_files]),
        "TOP_MODULE": args.top_module,
        "LUT_SIZE": lut_size,
        "OUTPUT_BLIF": args.top_module+"_yosys_out.blif",
    }
    yosys_template = args.yosys_tmpl if args.yosys_tmpl else os.path.join(
        cad_tools["misc_dir"], "ys_tmpl_yosys_vpr_flow.ys")
    tmpl = Template(open(yosys_template, encoding='utf-8').read())
    with open("yosys.ys", 'w') as archfile:
        archfile.write(tmpl.safe_substitute(ys_params))

    run_command("Run yosys", "yosys_output.log",
                [cad_tools["yosys_path"], 'yosys.ys'])


def run_odin2():
    pass


def run_abc_vtr():
    pass


def run_abc_for_standarad():
    pass


def run_ace2():
    if args.black_box_ace:
        with open(args.top_module+'_yosys_out.blif', 'r') as fp:
            blif_lines = fp.readlines()

        with open(args.top_module+'_bb.blif', 'w') as fp:
            for eachline in blif_lines:
                if ".names" in eachline:
                    input_nets = eachline.split()[1:]
                    if len(input_nets)-1 > args.K:
                        logger.error("One module in blif have more inputs" +
                                     " than K value")
                    # Map CEll to each logic in blif
                    map_nets = (input_nets[:-1] + ["unconn"]*args.K)[:args.K]
                    map_nets = ["I[%d]=%s" % (indx, eachnet)
                                for indx, eachnet in enumerate(map_nets)]
                    map_nets += ["O[0]=%s\n" % input_nets[-1]]
                    fp.write(".subckt CELL ")
                    fp.write(" ".join(map_nets))
                else:
                    fp.write(eachline)

            declar_input = " ".join(["I[%d]" % i for i in range(args.K)])
            model_tmpl = "\n" + \
                ".model CELL\n" + \
                ".inputs " + declar_input + " \n" + \
                ".outputs O[0]\n" + \
                ".blackbox\n" + \
                ".end\n"
            fp.write(model_tmpl)
    # Prepare ACE run command
    command = [
        "-b", args.top_module +
        ('_bb.blif' if args.black_box_ace else "_yosys_out.blif"),
        "-o", args.top_module+"_ace_out.act",
        "-n", args.top_module+"_ace_out.blif",
        "-c", "clk",
    ]
    command += ["-d", "%.4f" % args.ace_d] if args.ace_d else [""]
    command += ["-p", "%.4f" % args.ace_d] if args.ace_p else [""]
    try:
        filename = args.top_module + '_ace2_output.txt'
        with open(filename, 'w+') as output:
            process = subprocess.run([cad_tools["ace_path"]] + command,
                                     check=True,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE,
                                     universal_newlines=True)
            output.write(process.stdout)
            if process.returncode:
                logger.info("ACE2 failed with returncode %d",
                            process.returncode)
                raise subprocess.CalledProcessError(0, command)
    except:
        logger.exception("Failed to run ACE2")
        clean_up_and_exit("")
    logger.info("ACE2 output is written in file %s" % filename)


def run_pro_blif_3arg():
    command = [
        "-i", args.top_module+"_ace_out.blif",
        "-o", args.top_module+".blif",
        "-initial_blif", args.top_module+'_yosys_out.blif',
    ]
    try:
        filename = args.top_module+'_blif_3args_output.txt'
        with open(filename, 'w+') as output:
            process = subprocess.run(["perl", cad_tools["pro_blif_path"]] +
                                     command,
                                     check=True,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE,
                                     universal_newlines=True)
            output.write(process.stdout)
            if process.returncode:
                logger.info("blif_3args script failed with returncode %d",
                            process.returncode)
    except:
        logger.exception("Failed to run blif_3args")
        clean_up_and_exit("")
    logger.info("blif_3args output is written in file %s" % filename)


def collect_files_for_vpr():
    # Sanitize provided Benchmark option
    if len(args.benchmark_files) > 1:
        logger.error("Expecting Single Benchmark Blif file.")
    if not os.path.isfile(args.benchmark_files[0] or ""):
        clean_up_and_exit("Provided Blif file not found")
    shutil.copy(args.benchmark_files[0], args.top_module+".blif")

    # Sanitize provided Activity file option
    if not os.path.isfile(args.activity_file or ""):
        logger.error("Activity File - %s" % args.activity_file)
        clean_up_and_exit("Provided activity file not found")
    shutil.copy(args.activity_file, args.top_module+"_ace_out.act")

    # Sanitize provided Benchmark option
    if not os.path.isfile(args.base_verilog or ""):
        logger.error("Base Verilog File - %s" % args.base_verilog)
        clean_up_and_exit("Provided base_verilog file not found")
    shutil.copy(args.base_verilog, args.top_module+"_output_verilog.v")


def run_openfpga_shell():
    ExecTime["VPRStart"] = time.time()
    # bench_blif, fixed_chan_width, logfile, route_only=False
    tmpl = Template(open(args.top_module+"_template.openfpga",
                         encoding='utf-8').read())

    path_variables = script_env_vars["PATH"]
    path_variables["VPR_ARCH_FILE"] = args.arch_file
    path_variables["OPENFPGA_ARCH_FILE"] = args.openfpga_arch_file
    path_variables["VPR_TESTBENCH_BLIF"] = args.top_module+".blif"
    path_variables["ACTIVITY_FILE"] = args.top_module+"_ace_out.act"
    path_variables["REFERENCE_VERILOG_TESTBENCH"] = args.top_module + \
        "_output_verilog.v"

    for indx in range(0, len(OpenFPGAArgs), 2):
        tmpVar = OpenFPGAArgs[indx][2:].upper()
        path_variables[tmpVar] = OpenFPGAArgs[indx+1]

    with open(args.top_module+"_run.openfpga", 'w', encoding='utf-8') as archfile:
        archfile.write(tmpl.safe_substitute(path_variables))
    command = [cad_tools["openfpga_shell_path"], "-batch", "-f",
               args.top_module+"_run.openfpga"]
    run_command("OpenFPGA Shell Run", "openfpgashell.log", command)
    ExecTime["VPREnd"] = time.time()
    extract_vpr_stats("vpr_stdout.log")


def extract_vpr_stats(logfile, r_filename="vpr_stat", parse_section="vpr"):
    section = "DEFAULT_PARSE_RESULT_POWER" if parse_section == "power" \
        else "DEFAULT_PARSE_RESULT_VPR"
    vpr_log = open(logfile).read()
    resultDict = {}
    for name, value in config.items(section):
        reg_string, filt_function = value.split(",")
        match = re.search(reg_string[1:-1], vpr_log)
        if match:
            try:
                if "lambda" in filt_function.strip():
                    eval("ParseFunction = "+filt_function.strip())
                    extract_val = ParseFunction(**match.groups())
                elif filt_function.strip() == "int":
                    extract_val = int(match.group(1))
                elif filt_function.strip() == "float":
                    extract_val = float(match.group(1))
                elif filt_function.strip() == "str":
                    extract_val = str(match.group(1))
                elif filt_function.strip() == "scientific":
                    try:
                        mult = {"m": 1E-3, "u": 1E-6, "n": 1E-9,
                                "K": 1E-3, "M": 1E-6, "G": 1E-9, }.get(match.group(2)[0], 1)
                    except:
                        mult = 1
                    extract_val = float(match.group(1))*mult
                else:
                    extract_val = match.group(1)
            except:
                logger.exception("Filter failed")
                extract_val = "Filter Failed"
            resultDict[name] = extract_val

    dummyparser = ConfigParser()
    dummyparser.read_dict({"RESULTS": resultDict})

    with open(r_filename+'.result', 'w') as configfile:
        dummyparser.write(configfile)
    logger.info("%s result extracted in file %s" %
                (parse_section, r_filename+'.result'))


def run_rewrite_verilog():
    # Rewrite the verilog after optimization
    script_cmd = [
        "read_blif %s" % args.top_module+".blif",
        "write_verilog %s" % args.top_module+"_output_verilog.v"
    ]
    command = [cad_tools["yosys_path"], "-p", "; ".join(script_cmd)]
    run_command("Yosys", "yosys_rewrite.log", command)


def run_netlists_verification(exit_if_fail=True):
    ExecTime["VerificationStart"] = time.time()
    compiled_file = "compiled_"+args.top_module
    # include_netlists = args.top_module+"_include_netlists.v"
    tb_top_formal = args.top_module+"_top_formal_verification_random_tb"
    tb_top_autochecked = args.top_module+"_autocheck_top_tb"
    # netlists_path = args.vpr_fpga_verilog_dir_val+"/SRC/"

    command = [cad_tools["iverilog_path"]]
    command += ["-o", compiled_file]
    fpga_define_file = "./SRC/define_simulation.v"
    fpga_define_file_bk = "./SRC/define_simulation.v.bak"
    shutil.copy(fpga_define_file, fpga_define_file_bk)
    with open(fpga_define_file, "r") as fp:
        fpga_defines = fp.readlines()

    command += ["./SRC/%s_include_netlists.v" % args.top_module]
    command += ["-s"]
    if args.vpr_fpga_verilog_formal_verification_top_netlist:
        command += [tb_top_formal]
    else:
        command += [tb_top_autochecked]
        with open(fpga_define_file, "w") as fp:
            for eachLine in fpga_defines:
                if not (("ENABLE_FORMAL_VERIFICATION" in eachLine) or
                        "FORMAL_SIMULATION" in eachLine):
                    fp.write(eachLine)
    run_command("iverilog_verification", "iverilog_output.txt", command)

    vvp_command = ["vvp", compiled_file]
    output = run_command("vvp_verification", "vvp_sim_output.txt", vvp_command)
    if "Succeed" in output:
        logger.info("VVP Simulation Successful")
    else:
        logger.error(str(output).split("\n")[-1])
        if exit_if_fail:
            clean_up_and_exit("Failed to run VVP verification")
    ExecTime["VerificationEnd"] = time.time()


def run_command(taskname, logfile, command, exit_if_fail=True):
    logger.info("Launching %s " % taskname)
    with open(logfile, 'w') as output:
        try:
            output.write(" ".join(command)+"\n")
            process = subprocess.run(command,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE,
                                     universal_newlines=True)
            output.write(process.stdout)
            output.write(process.stderr)
            output.write(str(process.returncode))
            if "openfpgashell" in logfile:
                filter_openfpga_output(process.stdout)
            if process.returncode:
                logger.error("%s run failed with returncode %d" %
                             (taskname, process.returncode))
                logger.error("command %s" % " ".join(command))
                filter_failed_process_output(process.stderr)
                if exit_if_fail:
                    clean_up_and_exit("Failed to run %s task" % taskname)
        except Exception:
            logger.exception("%s failed to execute" % (taskname))
            traceback.print_exc(file=output)
            if exit_if_fail:
                clean_up_and_exit("Failed to run %s task" % taskname)
    logger.info("%s is written in file %s" % (taskname, logfile))
    return process.stdout


def filter_openfpga_output(vpr_output):
    stdout = iter(vpr_output.split("\n"))
    try:
        for i in range(50):
            if "Version:" in next(stdout):
                logger.info("OpenFPGAShell %s %s" %
                            (next(stdout), next(stdout)))
                break
    except StopIteration:
        pass


def filter_failed_process_output(vpr_output):
    for line in vpr_output.split("\n"):
        if "error" in line.lower():
            logger.error("-->>" + line)


if __name__ == "__main__":
    ExecTime["Start"] = time.time()
    # args = parser.parse_args()
    args, OpenFPGAArgs = parser.parse_known_args()
    main()
