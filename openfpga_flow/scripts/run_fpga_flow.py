import os
import sys
import shutil
import time
import shlex
import glob
import argparse
from configparser import ConfigParser, ExtendedInterpolation
import logging
import glob
import subprocess
import threading
from string import Template
import re
import xml.etree.ElementTree as ET

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
script_env_vars = {"PATH": {
    "OPENFPGA_FLOW_PATH": flow_script_dir,
    "OPENFPGA_PATH": openfpga_base_dir}}

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Reading command-line argument
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

# Helper function to provide better alignment to help print


def formatter(prog): return argparse.HelpFormatter(prog, max_help_position=60)


parser = argparse.ArgumentParser(formatter_class=formatter)

# Mandatory arguments
parser.add_argument('arch_file', type=str)
parser.add_argument('benchmark_files', type=str, nargs='+')

# Optional arguments
parser.add_argument('--top_module', type=str, default="top")
parser.add_argument('--fpga_flow', type=str, default="yosys_vpr")
parser.add_argument('--cad_tool_conf', type=str,
                    help="CAD tools path overrides default setting")
parser.add_argument('--run_dir', type=str,
                    default=os.path.join(openfpga_base_dir,  'tmp'),
                    help="Directory to store intermidiate file & final results")
parser.add_argument('--yosys_tmpl', type=str,
                    help="Alternate yosys template, generates top_module.blif")
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

# VPR - FPGA-SPICE Extension
SPParse = parser.add_argument_group('FPGA-SPICE Extension')
SPParse.add_argument('--vpr_fpga_spice', type=str,
                     help="Print SPICE netlists in VPR")
SPParse.add_argument('--vpr_fpga_spice_sim_mt_num', type=int,
                     help="Specify the option sim_mt_num of VPR FPGA SPICE")
SPParse.add_argument('--vpr_fpga_spice_print_component_tb', action='store_true',
                     help="Output component-level testbench")
SPParse.add_argument('--vpr_fpga_spice_print_grid_tb', action='store_true',
                     help="Output grid-level testbench")
SPParse.add_argument('--vpr_fpga_spice_print_top_tb', action='store_true',
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
VeriPar.add_argument('--vpr_fpga_verilog_print_modelsim_autodeck', type=str,
                     help="Print modelsim " +
                     "simulation script", metavar="<modelsim.ini_path>")
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
# Varible to store logger instance
logger = None
# arguments are parsed at the end of the script depending upon whether script
# is called externally or as a standalone
args = None
# variable to store script_configuration and cad tool paths
config, cad_tools = None, None

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
        run_rewrite_verilog()
    if args.power:
        run_ace2()
        run_pro_blif_3arg()
    if (args.fpga_flow == "vpr_blif"):
        collect_files_for_vpr()
    # if (args.fpga_flow == "vtr"):
    #     run_odin2()
    #     run_abc_vtr()
    # if (args.fpga_flow == "vtr_standard"):
    #     run_abc_for_standarad()
    run_vpr()
    if args.end_flow_with_test:
        run_netlists_verification()
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
    if args.cad_tool_conf:
        config.read_file(open(args.cad_tool_conf))
    if not "CAD_TOOLS_PATH" in config.sections():
        clean_up_and_exit("Missing CAD_TOOLS_PATH in openfpga_flow config")
    cad_tools = config["CAD_TOOLS_PATH"]


def validate_command_line_arguments():
    """
    TODO :
    This funtion validates all supplied paramters
    """
    logger.info("Validating commnad line arguments")

    if args.debug:
        logger.info("Setting loggger in debug mode")
        logger.setLevel(logging.DEBUG)

    # Check if flow supported
    if not args.fpga_flow in config.get("FLOW_SCRIPT_CONFIG", "valid_flows"):
        clean_up_and_exit("%s Flow not supported"%args.fpga_flow)

    # Check if argument list is consistant
    for eacharg, dependent in config.items("CMD_ARGUMENT_DEPENDANCY"):
        if getattr(args, eacharg, None):
            dependent = dependent.split(",")
            for eachdep in dependent:
                if not any([getattr(args, i, 0) for i in eachdep.split("|")]):
                    clean_up_and_exit("'%s' argument depends on (%s) argumets"%
                        (eacharg, ", ".join(dependent).replace("|", " or ")))

    # Filter provided architecrue files
    args.arch_file = os.path.abspath(args.arch_file)
    if not os.path.isfile(args.arch_file):
        clean_up_and_exit("Architecure file not found. -%s", args.arch_file)

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


def ask_user_quetion(condition, question):
    if condition:
        reply = str(input(question+' (y/n): ')).lower().strip()
        if reply[:1] in ['n', 'no']:
            return False
        elif reply[:1] in ['y', 'yes']:
            return True
        else:
            return ask_user_quetion(question, condition)
    return True


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
        if not ask_user_quetion((no_of_files > 100),
                                ("[run_dir:%s] already exist and contains %d " +
                                 "files script will remove all the file, " +
                                 "continue? ") % (run_dir, no_of_files)):
            clean_up_and_exit("Aborted by user")
        else:
            shutil.rmtree(run_dir)
    os.makedirs(run_dir)
    # Clean run_dir is created change working directory
    os.chdir(run_dir)

    # Create arch dir in run_dir and copy flattern architecrture file
    os.mkdir("arch")
    tmpl = Template(
        open(args.arch_file, encoding='utf-8').read())
    arch_filename = os.path.basename(args.arch_file)
    args.arch_file = os.path.join(run_dir, "arch", arch_filename)
    with open(args.arch_file, 'w', encoding='utf-8') as archfile:
        archfile.write(tmpl.substitute(script_env_vars["PATH"]))

    # Create benchmark dir in run_dir and copy flattern architecrture file
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
    yosys_template = os.path.join(
        cad_tools["misc_dir"], "ys_tmpl_yosys_vpr_flow.ys")
    tmpl = Template(open(yosys_template, encoding='utf-8').read())
    with open("yosys.ys", 'w') as archfile:
        archfile.write(tmpl.substitute(ys_params))
    try:
        with open('yosys_output.txt', 'w+') as output:
            process = subprocess.run([cad_tools["yosys_path"], 'yosys.ys'],
                                     check=True,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE,
                                     universal_newlines=True)
            output.write(process.stdout)
            if process.returncode:
                logger.info("Yosys failed with returncode %d",
                            process.returncode)
    except:
        logger.exception("Failed to run yosys")
        clean_up_and_exit("")
    logger.info("Yosys output is written in file yosys_output.txt")


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


def run_vpr():
    if not args.fix_route_chan_width:
    # Run Standard VPR Flow
    min_channel_width = run_standard_vpr(
        args.top_module+".blif",
        -1,
        args.top_module+"_min_chan_width_vpr.txt")
    logger.info("Standard VPR flow routed with minimum %d Channels" %
                min_channel_width)

    # Minimum routing channel width
    if (args.min_route_chan_width):
        min_channel_width *= args.min_route_chan_width
        min_channel_width = int(min_channel_width)
        min_channel_width += 1 if (min_channel_width % 2) else 0
        logger.info(("Trying to route using %d channels" % min_channel_width) +
                    " (Slack of %d%%)" % ((args.min_route_chan_width-1)*100))

        while(1):
            res = run_standard_vpr(args.top_module+".blif",
                                   int(min_channel_width),
                                args.top_module+"_reroute_vpr.txt",
                                route_only=True)

            if res:
                logger.info("Routing with channel width=%d successful" %
                            min_channel_width)
                break
            elif args.max_route_width_retry < (min_channel_width-2):
                clean_up_and_exit("Failed to route within maximum " +
                                  "iteration of channel width")
            else:
                logger.info("Unable to route using channel width %d" %
                            min_channel_width)
            min_channel_width += 2

        extract_vpr_stats(args.top_module+"_reroute_vpr.txt")

    # Fixed routing channel width
    elif args.fix_route_chan_width:
        min_channel_width = run_standard_vpr(
            args.top_module+".blif",
            args.fix_route_chan_width,
            args.top_module+"_fr_chan_width.txt")
        logger.info("Fixed routing channel successfully routed with %d width" %
                    min_channel_width)
        extract_vpr_stats(args.top_module+"_fr_chan_width.txt")
    else:
        extract_vpr_stats(args.top_module+"_min_chan_width.txt")
    if args.power:
        extract_vpr_stats(logfile=args.top_module+".power",
                          r_filename="vpr_power_stat",
                          parse_section="power")


def run_standard_vpr(bench_blif, fixed_chan_width, logfile, route_only=False):
    command = [cad_tools["vpr_path"],
               args.arch_file,
               bench_blif,
               "--net_file", args.top_module+"_vpr.net",
               "--place_file", args.top_module+"_vpr.place",
               "--route_file", args.top_module+"_vpr.route",
               "--full_stats", "--nodisp",
               ]
    if route_only:
        command += ["--route"]
    # Power options
    if args.power:
        command += ["--power",
                    "--activity_file", args.top_module+"_ace_out.act",
                    "--tech_properties", args.power_tech]
    # packer options
    if args.vpr_timing_pack_off:
        command += ["--timing_driven_clustering", "off"]
    #  channel width option
    if fixed_chan_width >= 0:
        command += ["--route_chan_width", "%d" % fixed_chan_width]
    if args.vpr_use_tileable_route_chan_width:
        command += ["--use_tileable_route_chan_width"]

    # FPGA_Spice Options
    if (args.power and args.vpr_fpga_spice):
        command += ["--fpga_spice"]
        if args.vpr_fpga_x2p_signal_density_weight:
            command += ["--fpga_x2p_signal_density_weight",
                        args.vpr_fpga_x2p_signal_density_weight]
        if args.vpr_fpga_x2p_sim_window_size:
            command += ["--fpga_x2p_sim_window_size",
                        args.vpr_fpga_x2p_sim_window_size]
        if args.vpr_fpga_x2p_compact_routing_hierarchy:
            command += ["--fpga_x2p_compact_routing_hierarchy"]

        if args.vpr_fpga_spice_sim_mt_num:
            command += ["--fpga_spice_sim_mt_num",
                        args.vpr_fpga_spice_sim_mt_num]
        if args.vpr_fpga_spice_simulator_path:
            command += ["--fpga_spice_simulator_path",
                        args.vpr_fpga_spice_simulator_path]
        if args.vpr_fpga_spice_print_component_tb:
            command += ["--fpga_spice_print_lut_testbench",
                        "--fpga_spice_print_hardlogic_testbench",
                        "--fpga_spice_print_pb_mux_testbench",
                        "--fpga_spice_print_cb_mux_testbench",
                        "--fpga_spice_print_sb_mux_testbench"
                        ]
        if args.vpr_fpga_spice_print_grid_tb:
            command += ["--fpga_spice_print_grid_testbench",
                        "--fpga_spice_print_cb_testbench",
                        "--fpga_spice_print_sb_testbench"
                        ]
        if args.vpr_fpga_spice_print_top_tb:
            command += ["--fpga_spice_print_top_testbench"]
        if args.vpr_fpga_spice_leakage_only:
            command += ["--fpga_spice_leakage_only"]
        if args.vpr_fpga_spice_parasitic_net_estimation_off:
            command += ["--fpga_spice_parasitic_net_estimation", "off"]
        if args.vpr_fpga_spice_testbench_load_extraction_off:
            command += ["--fpga_spice_testbench_load_extraction", "off"]

    # FPGA Verilog options
    if (args.power and args.vpr_fpga_verilog):
        command += ["--fpga_verilog"]
        if args.vpr_fpga_verilog_dir:
            command += ["--fpga_verilog_dir", args.vpr_fpga_verilog_dir]
        if args.vpr_fpga_verilog_print_top_tb:
            command += ["--fpga_verilog_print_top_testbench"]
        if args.vpr_fpga_verilog_print_input_blif_tb:
            command += ["--fpga_verilog_print_input_blif_testbench"]
        if args.vpr_fpga_verilog_print_autocheck_top_testbench:
            command += ["--fpga_verilog_print_autocheck_top_testbench",
                        args.top_module+"_output_verilog.v"]
        if args.vpr_fpga_verilog_include_timing:
            command += ["--fpga_verilog_include_timing"]
        if args.vpr_fpga_verilog_explicit_mapping:
            command += ["--fpga_verilog_explicit_mapping"]
        if args.vpr_fpga_verilog_include_signal_init:
            command += ["--fpga_verilog_include_signal_init"]
        if args.vpr_fpga_verilog_formal_verification_top_netlist:
            command += ["--fpga_verilog_print_formal_verification_top_netlist"]
        if args.vpr_fpga_verilog_print_modelsim_autodeck:
            command += ["--fpga_verilog_print_modelsim_autodeck",
                        args.vpr_fpga_verilog_print_modelsim_autodeck]
        if args.vpr_fpga_verilog_include_icarus_simulator:
            command += ["--fpga_verilog_include_icarus_simulator"]
        if args.vpr_fpga_verilog_print_report_timing_tcl:
            command += ["--fpga_verilog_print_report_timing_tcl"]
        if args.vpr_fpga_verilog_report_timing_rpt_path:
            command += ["--fpga_verilog_report_timing_rpt_path",
                        args.vpr_fpga_verilog_report_timing_rpt_path]
        if args.vpr_fpga_verilog_print_sdc_pnr:
            command += ["--fpga_verilog_print_sdc_pnr"]
        if args.vpr_fpga_verilog_print_user_defined_template:
            command += ["--fpga_verilog_print_user_defined_template"]
        if args.vpr_fpga_verilog_print_sdc_analysis:
            command += ["--fpga_verilog_print_sdc_analysis"]

    # FPGA Bitstream Genration options
    if args.vpr_fpga_verilog_print_sdc_analysis:
        command += ["--fpga_bitstream_generator"]

    if args.vpr_fpga_x2p_rename_illegal_port or \
            args.vpr_fpga_spice or \
            args.vpr_fpga_verilog:
        command += ["--fpga_x2p_rename_illegal_port"]

    # Other VPR options
    if args.vpr_place_clb_pin_remap:
        command += ["--place_clb_pin_remap"]
    if args.vpr_route_breadthfirst:
        command += ["--router_algorithm", "breadth_first"]
    if args.vpr_max_router_iteration:
        command += ["--max_router_iterations", args.vpr_max_router_iteration]

    chan_width = None
    try:
        with open(logfile, 'w+') as output:
            output.write(" ".join(command)+"\n")
            process = subprocess.run(command,
                                     check=True,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE,
                                     universal_newlines=True)
            for line in process.stdout.split('\n'):
                if "Best routing" in line:
                    chan_width = int(re.search(
                        r"channel width factor of ([0-9]+)", line).group(1))
                if "Circuit successfully routed" in line:
                    chan_width = int(re.search(
                        r"a channel width factor of ([0-9]+)", line).group(1))
            output.write(process.stdout)
            if process.returncode:
                logger.info("Standard VPR run failed with returncode %d",
                            process.returncode)
    except (Exception, subprocess.CalledProcessError) as e:
        logger.exception("Failed to run VPR")
        process_failed_vpr_run(e.output)
        clean_up_and_exit("")
    logger.info("VPR output is written in file %s" % logfile)
    return chan_width


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
                        mult = {"m":1E-3, "u":1E-6, "n":1E-9,
                        "K":1E-3, "M":1E-6, "G":1E-9,}.get(match.group(2)[0], 1)
                    except:
                        mult = 1
                    extract_val = float(match.group(1))*mult
                else:
                    extract_val = match.group(1)
            except:
                logger.exception("Filter failed")
                extract_val= "Filter Failed"
            resultDict[name] = extract_val

    dummyparser = ConfigParser()
    dummyparser.read_dict({"RESULTS": resultDict})

    with open(r_filename+'.result', 'w') as configfile:
        dummyparser.write(configfile)
    logger.info("%s result extracted in file %s" %
    (parse_section,r_filename+'.result'))


def run_rewrite_verilog():
    # Rewrite the verilog after optimization
    script_cmd = [
        "read_blif %s" % args.top_module+".blif",
        "write_verilog %s" % args.top_module+"_output_verilog.v"
    ]
    command = [cad_tools["yosys_path"], "-p", "; ".join(script_cmd)]
    try:
        with open('yosys_rewrite_veri_output.txt', 'w+') as output:
            process = subprocess.run(command,
                                     check=True,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE,
                                     universal_newlines=True)
            output.write(process.stdout)
            if process.returncode:
                logger.info("Rewrite veri yosys run failed with returncode %d",
                            process.returncode)
    except:
        logger.exception("Failed to run VPR")
        clean_up_and_exit("")
    logger.info("Yosys output is written in file yosys_rewrite_veri_output.txt")


def run_netlists_verification():
    compiled_file = "compiled_"+args.top_module
    # include_netlists = args.top_module+"_include_netlists.v"
    tb_top_formal = args.top_module+"_top_formal_verification_random_tb"
    tb_top_autochecked = args.top_module+"_autocheck_top_tb"
    # netlists_path = args.vpr_fpga_verilog_dir_val+"/SRC/"

    command = [cad_tools["iverilog_path"]]
    command += ["-o", compiled_file]
    command += ["./SRC/%s_include_netlists.v" %
                args.top_module]
    command += ["-s"]
    if args.vpr_fpga_verilog_formal_verification_top_netlist:
        command += [tb_top_formal]
    else:
        command += [tb_top_autochecked]
    run_command("iverilog_verification", "iverilog_output.txt", command)

    vvp_command = ["vvp", compiled_file]
    output = run_command("vvp_verification", "vvp_sim_output.txt", vvp_command)
    if "Succeed" in output:
        logger.info("VVP Simulation Successful")
    else:
        logger.info(str(output).split("\n")[-1])


def run_command(taskname, logfile, command, exit_if_fail=True):
    logger.info("Launching %s " % taskname)
    try:
        with open(logfile, 'w+') as output:
            output.write(" ".join(command)+"\n")
            process = subprocess.run(command,
                                     check=True,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE,
                                     universal_newlines=True)
            output.write(process.stdout)
            if process.returncode:
                logger.error("%s run failed with returncode %d" %
                             (taskname, process.returncode))
    except (Exception, subprocess.CalledProcessError) as e:
        logger.exception("failed to execute %s" % taskname)
        process_failed_vpr_run(e.output)
        print(e.output)
        if exit_if_fail:
            clean_up_and_exit("Failed to run %s task" % taskname)
        return None
    logger.info("%s is written in file %s" % (taskname, logfile))
    return process.stdout


def process_failed_vpr_run(vpr_output):
    for line in vpr_output.split("\n"):
        if "error" in line.lower():
            logger.error("-->>" + line)


if __name__ == "__main__":
    # Setting up print and logging system
    logging.basicConfig(level=logging.DEBUG, stream=sys.stdout,
                        format='%(levelname)s - %(message)s')
    logger = logging.getLogger('OpenFPGA_Flow_Logs')

    # Parse commandline argument
    args = parser.parse_args()
    main()
