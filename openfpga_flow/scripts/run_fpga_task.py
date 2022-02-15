# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Script Name   : run_fpga_task.py
# Description   : This script designed to run openfpga_flow tasks,
#                 Opensfpga task are design to run opefpga_flow on each
#                 Combination of architecture, benchmark and script paramters
# Args          : python3 run_fpga_task.py --help
# Author        : Ganesh Gore
# Email          : ganesh.gore@utah.edu
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

import os
import sys
import shutil
import time
from datetime import timedelta
import shlex
import argparse
from configparser import ConfigParser, ExtendedInterpolation
import logging
import glob
import subprocess
import threading
import csv
from string import Template
import pprint
from importlib import util
from collections import OrderedDict

if util.find_spec("coloredlogs"):
    import coloredlogs
if util.find_spec("humanize"):
    import humanize

if sys.version_info[0] < 3:
    raise Exception("run_fpga_task script must be using Python 3")

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Configure logging system
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
LOG_FORMAT = "%(levelname)5s (%(threadName)15s) - %(message)s"
if util.find_spec("coloredlogs"):
    coloredlogs.install(level='INFO', stream=sys.stdout,
                        fmt=LOG_FORMAT)
else:
    logging.basicConfig(level=logging.INFO, stream=sys.stdout,
                        format=LOG_FORMAT)
logger = logging.getLogger('OpenFPGA_Task_logs')


# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Read commandline arguments
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
parser = argparse.ArgumentParser()
parser.add_argument('tasks', nargs='+')
parser.add_argument('--maxthreads', type=int, default=2,
                    help="Number of fpga_flow threads to run default = 2," +
                    "Typically <= Number of processors on the system")
parser.add_argument('--remove_run_dir', type=str,
                    help="Remove run dir " +
                         "'all' to remove all." +
                         "<int>,<int> to remove specific run dir" +
                         "<int>-<int> To remove range of directory")
parser.add_argument('--config', help="Override default configuration")
parser.add_argument('--test_run', action="store_true",
                    help="Dummy run shows final generated VPR commands")
parser.add_argument('--debug', action="store_true",
                    help="Run script in debug mode")
parser.add_argument('--continue_on_fail', action="store_true",
                    help="Exit script with return code")
parser.add_argument('--show_thread_logs', action="store_true",
                    help="Skips logs from running thread")
args = parser.parse_args()

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Read script configuration file
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
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
config = ConfigParser(interpolation=ExtendedInterpolation())
config.read_dict(script_env_vars)
config.read_file(open(os.path.join(task_script_dir, 'run_fpga_task.conf')))
gc = config["GENERAL CONFIGURATION"]


def main():
    validate_command_line_arguments()
    for eachtask in args.tasks:
        logger.info("Currently running task %s" % eachtask)
        eachtask = eachtask.replace("\\", "/").split("/")
        job_run_list, GeneralSection = generate_each_task_actions(eachtask)
        if args.remove_run_dir:
            continue
        eachtask = "_".join(eachtask)
        if not args.test_run:
            run_actions(job_run_list)
            if not (GeneralSection.get("fpga_flow") == "yosys"):
                collect_results(job_run_list)
        else:
            pprint.pprint(job_run_list)
    logger.info("Task execution completed")
    exit(0)

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Subroutines starts here
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


def clean_up_and_exit(msg):
    logger.error(msg)
    logger.error("Exiting . . . . . .")
    exit(1)


def validate_command_line_arguments():
    if args.debug:
        logger.info("Setting loggger in debug mode")
        logger.setLevel(logging.DEBUG)
    logger.info("Set up to run %d Parallel threads", args.maxthreads)


def remove_run_dir():
    remove_dir = []
    try:
        argval = args.remove_run_dir.lower()
        if argval == "all":
            for eachRun in glob.glob("run*"):
                remove_dir += [eachRun]
        elif "-" in argval:
            minval, maxval = map(int, argval.split("-"))
            if minval > maxval:
                raise Exception("Enter valid range to remove")
            for eachRun in glob.glob("run*"):
                if minval <= int(eachRun[-3:]) <= maxval:
                    remove_dir += [eachRun]
        elif "," in argval:
            for eachRun in argval.split(","):
                remove_dir += ["run%03d" % int(eachRun)]
        else:
            logger.error("Unknow argument to --remove_run_dir")
    except:
        logger.exception("Failed to parse remove rund_dir options")

    try:
        for eachdir in remove_dir:
            logger.info('Removing run_dir %s' % (eachdir))
            if os.path.exists('latest'):
                if eachdir == os.readlink('latest'):
                    remove_dir += ["latest"]
            shutil.rmtree(eachdir, ignore_errors=True)
    except:
        logger.exception("Failed to remove %s run directory" %
                         (eachdir or "Unknown"))


def generate_each_task_actions(taskname):
    """
    This script generates all the scripts required for each benchmark
    """

    # Check if task directory exists and consistent
    local_tasks = os.path.join(*(taskname))
    repo_tasks = os.path.join(gc["task_dir"], *(taskname))
    abs_tasks = os.path.abspath('/' + local_tasks)
    if os.path.isdir(local_tasks):
        os.chdir(local_tasks)
        curr_task_dir = os.path.abspath(os.getcwd())
    elif os.path.isdir(abs_tasks):
        curr_task_dir = abs_tasks
    elif os.path.isdir(repo_tasks):
        curr_task_dir = repo_tasks
    else:
        clean_up_and_exit("Task directory [%s] not found" % taskname +
                          " locally at [%s]" % local_tasks +
                          ", absolutely at [%s]" % abs_tasks +
                          ", or in OpenFPGA task directory [%s]" % repo_tasks)

    os.chdir(curr_task_dir)

    curr_task_conf_file = os.path.join(curr_task_dir, "config", "task.conf")
    if not os.path.isfile(curr_task_conf_file):
        clean_up_and_exit(
            "Missing configuration file for task %s" % curr_task_dir)

    if args.remove_run_dir:
        remove_run_dir()
    else:
      # Create run directory for current task run ./runxxx
      run_dirs = [int(os.path.basename(x)[-3:]) for x in glob.glob('run*[0-9]')]
      curr_run_dir = "run%03d" % (max(run_dirs+[0, ])+1)
      try:
          os.mkdir(curr_run_dir)
          if os.path.islink('latest') or os.path.exists('latest'):
              os.remove("latest")
          os.symlink(curr_run_dir, "latest")
          logger.info('Created "%s" directory for current task run' %
                      curr_run_dir)
      except:
          logger.exception("")
          logger.error("Failed to create new run directory in task directory")
      os.chdir(curr_run_dir)

    # Read task configuration file and check consistency
    task_conf = ConfigParser(allow_no_value=True,
                             interpolation=ExtendedInterpolation())
    script_env_vars['PATH']["TASK_NAME"] = "/".join(taskname)
    script_env_vars['PATH']["TASK_DIR"] = curr_task_dir
    task_conf.read_dict(script_env_vars)
    task_conf.read_file(open(curr_task_conf_file))

    required_sec = ["GENERAL", "BENCHMARKS", "ARCHITECTURES"]
    missing_section = list(set(required_sec)-set(task_conf.sections()))
    if missing_section:
        clean_up_and_exit("Missing sections %s" % " ".join(missing_section) +
                          " in task configuration file")

    # Declare varibles to access sections
    TaskFileSections = task_conf.sections()
    SynthSection = task_conf["SYNTHESIS_PARAM"]
    GeneralSection = task_conf["GENERAL"]

    # Check if specified architecture files exist
    # TODO Store it as a dictionary and take reference from the key
    archfile_list = []
    for _, arch_file in task_conf["ARCHITECTURES"].items():
        arch_full_path = arch_file
        if os.path.isfile(arch_full_path):
            archfile_list.append(arch_full_path)
        else:
            clean_up_and_exit("Architecture file not found: " +
                              "%s  " % arch_file)
    if not len(archfile_list) == len(list(set(archfile_list))):
        clean_up_and_exit("Found duplicate architectures in config file")

    # Get Flow information
    logger.info('Running "%s" flow' %
                GeneralSection.get("fpga_flow", fallback="yosys_vpr"))

    # Check if specified benchmark files exist
    benchmark_list = []
    for bech_name, each_benchmark in task_conf["BENCHMARKS"].items():
        # Declare varible to store paramteres for current benchmark
        CurrBenchPara = {}

        # Parse benchmark file
        bench_files = []
        for eachpath in each_benchmark.split(","):
            files = glob.glob(eachpath)
            if not len(files):
                clean_up_and_exit(("No files added benchmark %s" % bech_name) +
                                  " with path %s " % (eachpath))
            bench_files += files

        # Read provided benchmark configurations
        # Common configurations
        # - All the benchmarks may share the same yosys synthesis template script
        # - All the benchmarks may share the same rewrite yosys template script, which converts post-synthesis .v netlist to be compatible with .blif port definition. This is required for correct verification at the end of flows
        # - All the benchmarks may share the same routing channel width in VPR runs. This is designed to enable architecture evaluations for a fixed device model
        # - All the benchmarks may share the same options for reading verilog files
        ys_for_task_common = SynthSection.get("bench_yosys_common")
        ys_rewrite_for_task_common = SynthSection.get("bench_yosys_rewrite_common")
        chan_width_common = SynthSection.get("bench_chan_width_common")

        yosys_params = [
            "read_verilog_options",
            "yosys_args",
            "yosys_bram_map_rules",
            "yosys_bram_map_verilog",
            "yosys_cell_sim_verilog",
            "yosys_cell_sim_systemverilog",
            "yosys_cell_sim_vhdl",
            "yosys_blackbox_modules",
            "yosys_dff_map_verilog",
            "yosys_dsp_map_parameters",
            "yosys_dsp_map_verilog",
            "verific_verilog_standard",
            "verific_systemverilog_standard",
            "verific_vhdl_standard",
            "verific_include_dir",
            "verific_library_dir",
            "verific_search_lib"
        ]

        yosys_params_common = {}
        for param in yosys_params:
            yosys_params_common[param.upper()] = SynthSection.get("bench_"+param+"_common")

        # Individual benchmark configuration
        CurrBenchPara["files"] = bench_files
        CurrBenchPara["top_module"] = SynthSection.get(bech_name+"_top",
                                                       fallback="top")
        CurrBenchPara["ys_script"] = SynthSection.get(bech_name+"_yosys",
                                                      fallback=ys_for_task_common)
        CurrBenchPara["ys_rewrite_script"] = SynthSection.get(bech_name+"_yosys_rewrite",
                                                      fallback=ys_rewrite_for_task_common)
        CurrBenchPara["chan_width"] = SynthSection.get(bech_name+"_chan_width",
                                                       fallback=chan_width_common)
        CurrBenchPara["benchVariable"] = []
        for eachKey, eachValue in SynthSection.items():
            if bech_name in eachKey:
                eachKey = eachKey.replace(bech_name+"_", "").upper()
                CurrBenchPara["benchVariable"] += [f"--{eachKey}", eachValue]
        
        for param, value in yosys_params_common.items():
            if not param in CurrBenchPara["benchVariable"] and value:
                CurrBenchPara["benchVariable"] += [f"--{param}", value]

        if GeneralSection.get("fpga_flow") == "vpr_blif":
            # Check if activity file exist
            if not SynthSection.get(bech_name+"_act"):
                clean_up_and_exit("Missing argument %s" % (bech_name+"_act") +
                                  "for vpr_blif flow")
            CurrBenchPara["activity_file"] = SynthSection.get(bech_name+"_act")

            # Check if base verilog file exists
            if not SynthSection.get(bech_name+"_verilog"):
                clean_up_and_exit("Missing argument %s for vpr_blif flow" %
                                  (bech_name+"_verilog"))
            CurrBenchPara["verilog_file"] = SynthSection.get(
                bech_name+"_verilog")

        # Add script parameter list in current benchmark
        ScriptSections = [x for x in TaskFileSections if "SCRIPT_PARAM" in x]
        script_para_list = {}
        for eachset in ScriptSections:
            command = []
            for key, values in task_conf[eachset].items():
                command += ["--"+key, values] if values else ["--"+key]

            # Set label for Sript Parameters
            set_lbl = eachset.replace("SCRIPT_PARAM", "")
            set_lbl = set_lbl[1:] if set_lbl else "Common"
            script_para_list[set_lbl] = command
        CurrBenchPara["script_params"] = script_para_list

        benchmark_list.append(CurrBenchPara)

    # Count the number of duplicated top module name among benchmark
    # This is required as flow run directory names for these benchmarks are different than others
    # which are uniquified
    benchmark_top_module_count = []
    for bench in benchmark_list:
      benchmark_top_module_count.append(bench["top_module"])

    # Create OpenFPGA flow run commnad for each combination of
    # architecture, benchmark and parameters
    # Create run_job object [arch, bench, run_dir, commnad]
    flow_run_cmd_list = []
    for indx, arch in enumerate(archfile_list):
        for bench in benchmark_list:
            for lbl, param in bench["script_params"].items():
                if (benchmark_top_module_count.count(bench["top_module"]) > 1):
                  flow_run_dir = get_flow_rundir(arch, "bench" + str(benchmark_list.index(bench)) + "_" + bench["top_module"], lbl)
                else:
                  flow_run_dir = get_flow_rundir(arch, bench["top_module"], lbl)

                command = create_run_command(
                    curr_job_dir=flow_run_dir,
                    archfile=arch,
                    benchmark_obj=bench,
                    param=param,
                    task_conf=task_conf)
                flow_run_cmd_list.append({
                    "arch": arch,
                    "bench": bench,
                    "name": "%02d_%s_%s" % (indx, bench["top_module"], lbl),
                    "run_dir": flow_run_dir,
                    "commands": command + bench["benchVariable"],
                    "finished": False,
                    "status": False})

    logger.info('Found %d Architectures %d Benchmarks & %d Script Parameters' %
                (len(archfile_list), len(benchmark_list), len(ScriptSections)))
    logger.info('Created total %d jobs' % len(flow_run_cmd_list))

    return flow_run_cmd_list,GeneralSection

# Make the directory name unique by including the benchmark index in the list.
# This is because benchmarks may share the same top module names


def get_flow_rundir(arch, top_module, flow_params=None):
    path = [
        os.path.basename(arch).replace(".xml", ""),
        top_module,
        flow_params if flow_params else "common"
    ]
    return os.path.abspath(os.path.join(*path))


def create_run_command(curr_job_dir, archfile, benchmark_obj, param, task_conf):
    """
    Create_run_script function accepts run directory, architecture list and
    fpga_flow configuration file and prepare final executable fpga_flow script
    TODO : Replace this section after convert fpga_flow to python script
    Config file creation and bechnmark list can be skipped
    """
    # = = = = = = = = = File/Directory Consitancy Check = = = = = = = = = =
    if not os.path.isdir(gc["misc_dir"]):
        clean_up_and_exit("Miscellaneous directory does not exist")

    # = = = = = = = = = = = =  Create execution folder = = = = = = = = = = = =
    if os.path.isdir(curr_job_dir):
        question = "One the result directory already exist.\n"
        question += "%s\n" % curr_job_dir
        reply = str(input(question+' (y/n): ')).lower().strip()
        if reply[:1] in ['y', 'yes']:
            shutil.rmtree(curr_job_dir)
        else:
            logger.info("Result directory removal denied by the user")
            exit()
    os.makedirs(curr_job_dir)

    # Make execution command to run Open FPGA flow
    task_gc = task_conf["GENERAL"]
    task_OFPGAc = task_conf["OpenFPGA_SHELL"]
    command = [archfile] + benchmark_obj["files"]
    command += ["--top_module", benchmark_obj["top_module"]]
    command += ["--run_dir", curr_job_dir]

    if task_gc.get("fpga_flow"):
        command += ["--fpga_flow", task_gc.get("fpga_flow")]

    if task_gc.getboolean("verific"):
        command += ["--verific"]

    if task_gc.get("run_engine") == "openfpga_shell":
        for eachKey in task_OFPGAc.keys():
            command += [f"--{eachKey}",
                        task_OFPGAc.get(f"{eachKey}")]

    if benchmark_obj.get("activity_file"):
        command += ["--activity_file", benchmark_obj.get("activity_file")]

    if benchmark_obj.get("verilog_file"):
        command += ["--base_verilog", benchmark_obj.get("verilog_file")]

    if benchmark_obj.get("ys_script"):
        command += ["--yosys_tmpl", benchmark_obj["ys_script"]]

    if benchmark_obj.get("ys_rewrite_script"):
        command += ["--ys_rewrite_tmpl", benchmark_obj["ys_rewrite_script"]]

    if task_gc.getboolean("power_analysis"):
        command += ["--power"]
        command += ["--power_tech", task_gc.get("power_tech_file")]

    if task_gc.get("arch_variable_file"):
        command += ["--arch_variable_file", task_gc.get("arch_variable_file")]

    if task_gc.getboolean("spice_output"):
        command += ["--vpr_fpga_spice"]

    if task_gc.getboolean("verilog_output"):
        command += ["--vpr_fpga_verilog"]
        command += ["--vpr_fpga_verilog_dir", curr_job_dir]
        command += ["--vpr_fpga_x2p_rename_illegal_port"]

    # Add other paramters to pass
    command += param

    if args.debug:
        command += ["--debug"]
    return command


def strip_child_logger_info(line):
    try:
        logtype, message = line.split(" - ", 1)
        lognumb = {"CRITICAL": 50, "ERROR": 40, "WARNING": 30,
                   "INFO": 20, "DEBUG": 10, "NOTSET": 0}
        logger.log(lognumb[logtype.strip().upper()], message)
    except:
        logger.info(line)


def run_single_script(s, eachJob, job_list):
    with s:
        thread_name = threading.currentThread().getName()
        eachJob["starttime"] = time.time()
        try:
            logfile = "%s_out.log" % thread_name
            with open(logfile, 'w+') as output:
                output.write("* "*20 + '\n')
                output.write("RunDirectory : %s\n" % os.getcwd())
                command = [os.getenv('PYTHON_EXEC', gc["python_path"]), gc["script_default"]] + \
                    eachJob["commands"]
                output.write(" ".join(command) + '\n')
                output.write("* "*20 + '\n')
                logger.debug("Running OpenFPGA flow with [%s]" % command)
                process = subprocess.Popen(command,
                                           stdout=subprocess.PIPE,
                                           stderr=subprocess.STDOUT,
                                           universal_newlines=True)
                for line in process.stdout:
                    if args.show_thread_logs:
                        strip_child_logger_info(line[:-1])
                    sys.stdout.buffer.flush()
                    output.write(line)
                process.wait()
                if process.returncode:
                    raise subprocess.CalledProcessError(0, " ".join(command))
                eachJob["status"] = True
        except:
            logger.exception("Failed to execute openfpga flow - " +
                             eachJob["name"])
            if not args.continue_on_fail:
                os._exit(1)
        eachJob["endtime"] = time.time()
        timediff = timedelta(seconds=(eachJob["endtime"]-eachJob["starttime"]))
        timestr = humanize.naturaldelta(timediff) if "humanize" in sys.modules \
            else str(timediff)
        logger.info("%s Finished with returncode %d, Time Taken %s " %
                    (thread_name, process.returncode, timestr))
        eachJob["finished"] = True
        no_of_finished_job = sum([not eachJ["finished"] for eachJ in job_list])
        logger.info("***** %d runs pending *****" % (no_of_finished_job))


def run_actions(job_list):
    thread_sema = threading.Semaphore(args.maxthreads)
    thread_list = []
    for _, eachjob in enumerate(job_list):
        t = threading.Thread(target=run_single_script, name=eachjob["name"],
                             args=(thread_sema, eachjob, job_list))
        t.start()
        thread_list.append(t)
    for eachthread in thread_list:
        eachthread.join()


def collect_results(job_run_list):
    task_result = []
    for run in job_run_list:
        if not run["status"]:
            logger.warning("Skipping %s run", run["name"])
            continue
        # Check if any result file exist
        if not glob.glob(os.path.join(run["run_dir"], "*.result")):
            logger.info("No result files found for %s" % run["name"])

        # Read and merge result file
        vpr_res = ConfigParser(allow_no_value=True,
                               interpolation=ExtendedInterpolation())
        vpr_res.read_file(
            open(os.path.join(run["run_dir"], "vpr_stat.result")))
        result = OrderedDict()
        result["name"] = run["name"]
        result["TotalRunTime"] = int(run["endtime"]-run["starttime"])
        result.update(vpr_res["RESULTS"])
        task_result.append(result)
        colnames = []
        for eachLbl in task_result:
            colnames.extend(eachLbl.keys())
    if len(task_result):
        with open("task_result.csv", 'w', newline='') as csvfile:
            writer = csv.DictWriter(
                csvfile, extrasaction='ignore', fieldnames=list(set(colnames)))
            writer.writeheader()
            for eachResult in task_result:
                writer.writerow(eachResult)


if __name__ == "__main__":
    main()
