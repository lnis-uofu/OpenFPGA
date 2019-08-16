import os
import sys
import shutil
import time
import shlex
import argparse
from configparser import ConfigParser, ExtendedInterpolation
import logging
import glob
import subprocess
import threading
import csv
from string import Template
import run_fpga_flow
import pprint

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Configure logging system
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
logging.basicConfig(level=logging.DEBUG, stream=sys.stdout,
                    format='%(levelname)s (%(threadName)-9s) - %(message)s')
logger = logging.getLogger('OpenFPGA_Task_logs')


# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Read commandline arguments
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
parser = argparse.ArgumentParser()
parser.add_argument('tasks', nargs='+')
parser.add_argument('--maxthreads', type=int, default=2,
                    help="Number of fpga_flow threads to run default = 2," +
                    "Typically <= Number of processors on the system")
parser.add_argument('--config', help="Override default configuration")
parser.add_argument('--test_run', action="store_true",
                    help="Dummy run shows final generated VPR commands")
args = parser.parse_args()

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Read script configuration file
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
task_script_dir = os.path.dirname(os.path.abspath(__file__))
script_env_vars = ({"PATH": {
    "OPENFPGA_FLOW_PATH": task_script_dir,
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
        job_run_list = generate_each_task_actions(eachtask)
        if not args.test_run:
            run_actions(job_run_list)
            collect_results(job_run_list)
        else:
            pprint.pprint(job_run_list)
    logger.info("Task execution completed")
    exit()

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Subroutines starts here
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


def clean_up_and_exit(msg):
    logger.error(msg)
    logger.error("Existing . . . . . .")
    exit()


def validate_command_line_arguments():
    pass


def generate_each_task_actions(taskname):
    """
    This script generates all the scripts required for each benchmark
    """

    # Check if task directory exists and consistent
    curr_task_dir = os.path.join(gc["task_dir"], taskname)
    if not os.path.isdir(curr_task_dir):
        clean_up_and_exit("Task directory [%s] not found" % curr_task_dir)
    os.chdir(curr_task_dir)

    curr_task_conf_file = os.path.join(curr_task_dir, "config", "task.conf")
    if not os.path.isfile(curr_task_conf_file):
        clean_up_and_exit(
            "Missing configuration file for task %s" % curr_task_dir)

    # Create run directory for current task run ./runxxx
    run_dirs = [int(os.path.basename(x)[-3:]) for x in glob.glob('run*[0-9]')]
    curr_run_dir = "run%03d" % (max(run_dirs+[0, ])+1)
    try:
        os.mkdir(curr_run_dir)
        if os.path.islink('latest'):
            os.unlink("latest")
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
    task_conf.read_dict(script_env_vars)
    task_conf.read_file(open(curr_task_conf_file))

    required_sec = ["GENERAL", "BENCHMARKS", "ARCHITECTURES", "POST_RUN"]
    missing_section = list(set(required_sec)-set(task_conf.sections()))
    if missing_section:
        clean_up_and_exit("Missing sections %s" % " ".join(missing_section) +
                          " in task configuration file")

    # Check if specified architecture files exist
    archfile_list = []
    for _, arch_file in task_conf["ARCHITECTURES"].items():
        arch_full_path = arch_file
        if os.path.isfile(arch_full_path):
            archfile_list.append(arch_full_path)
        else:
            clean_up_and_exit("Architecture file not found: " +
                              "%s  " % arch_file)

    # Check if specified benchmark files exist
    benchmark_list = []
    for bech_name, each_benchmark in task_conf["BENCHMARKS"].items():
        bench_files = []
        for eachpath in each_benchmark.split(","):
            files = glob.glob(eachpath)
            if not len(files):
                clean_up_and_exit(("No files added benchmark %s" % bech_name) +
                                  " with path %s " % (eachpath))
            bench_files += files

        ys_for_task = task_conf.get("SYNTHESIS_PARAM", "bench_yosys_common")
        benchmark_list.append({
            "files": bench_files,
            "top_module": task_conf.get("SYNTHESIS_PARAM", bech_name+"_top",
                                        fallback="top"),
            "ys_script": task_conf.get("SYNTHESIS_PARAM", bech_name+"_yosys",
                                       fallback=ys_for_task)
        })

    # Create OpenFPGA flow run commnad for each combination of
    # architecture, benchmark and parameters
    # Create run_job object [arch, bench, run_dir, commnad]
    flow_run_cmd_list = []
    for indx, arch in enumerate(archfile_list):
        for bench in benchmark_list:
            flow_run_dir = get_flow_rundir(arch, bench["top_module"])
            cmd = create_run_command(
                flow_run_dir, arch, bench, task_conf)
            flow_run_cmd_list.append({
                "arch": arch,
                "bench": bench,
                "name": "%s_arch%d" % (bench["top_module"], indx),
                "run_dir": flow_run_dir,
                "commands": cmd})
    return flow_run_cmd_list


def get_flow_rundir(arch, top_module, flow_params=None):
    path = [
        os.path.basename(arch).replace(".xml", ""),
        top_module,
        flow_params if flow_params else "common"
    ]
    return os.path.abspath(os.path.join(*path))


def create_run_command(curr_job_dir, archfile, benchmark_obj, task_conf):
    """
    Create_run_script function accepts run directory, architecture list and
    fpga_flow configuration file and prepare final executable fpga_flow script
    TODO : Replace this section after convert fpga_flow to python script
    Config file creation and bechnmark list can be skipped
    """
    # = = = = = = = = = File/Directory Consitancy Check = = = = = = = = = =
    if not os.path.isdir(gc["misc_dir"]):
        clean_up_and_exit("Miscellaneous directory does not exist")

    fpga_flow_script = os.path.join(gc["misc_dir"], "fpga_flow_template.sh")
    if not os.path.isfile(fpga_flow_script):
        clean_up_and_exit("Missing fpga_flow script template %s" %
                          fpga_flow_script)

    fpga_flow_conf_tmpl = os.path.join(gc["misc_dir"], "fpga_flow_script.conf")
    if not os.path.isfile(fpga_flow_conf_tmpl):
        clean_up_and_exit("fpga_flow configuration tempalte is missing %s" %
                          fpga_flow_conf_tmpl)

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
    command = [archfile] + benchmark_obj["files"]
    command += ["--top_module", benchmark_obj["top_module"]]
    command += ["--run_dir", curr_job_dir]
    if task_conf.getboolean("GENERAL", "power_analysis", fallback=False):
        command += ["--power"]
        command += ["--power_tech",
                    task_conf.get("GENERAL", "power_tech_file")]
    if task_conf.getboolean("GENERAL", "spice_output", fallback=False):
        command += ["--vpr_fpga_spice"]
    if task_conf.getboolean("GENERAL", "verilog_output", fallback=False):
        command += ["--vpr_fpga_verilog"]

    # Add other paramters to pass
    for key, values in task_conf["SCRIPT_PARAM"].items():
        command += ["--"+key, values]
    return command


def run_single_script(s, command):
    logger.debug('Added job in pool')
    with s:
        logger.debug("Running OpenFPGA flow with " + " ".join(command))
        name = threading.currentThread().getName()
        # run_fpga_flow.external_call(logger, command)
        try:
            logfile = "%s_out.log" % name
            with open(logfile, 'w+') as output:
                process = subprocess.run(["python3.5", gc["script_default"]]+command,
                                         check=True,
                                         stdout=subprocess.PIPE,
                                         stderr=subprocess.PIPE,
                                         universal_newlines=True)
                output.write(process.stdout)
        except:
            logger.exception()
        logger.info("%s Finished " % name)


def run_actions(job_run_list):
    thread_sema = threading.Semaphore(args.maxthreads)
    thred_list = []
    for index, eachjob in enumerate(job_run_list):
        t = threading.Thread(target=run_single_script,
                             name='Job_%02d' % (index+1),
                             args=(thread_sema, eachjob["commands"]))
        t.start()
        thred_list.append(t)

    for eachthread in thred_list:
        eachthread.join()


def collect_results(job_run_list):
    task_result = []
    for run in job_run_list:
        # Check if any result file exist
        if not glob.glob(os.path.join(run["run_dir"], "*.result")):
            logger.info("No result files found for %s" % run["name"])

        # Read and merge result file
        vpr_res = ConfigParser(allow_no_value=True,
                               interpolation=ExtendedInterpolation())
        vpr_res.read_file(
            open(os.path.join(run["run_dir"], "vpr_stat.result")))
        result = dict(vpr_res["RESULTS"])
        result["name"] = run["name"]
        task_result.append(result)

    pprint.pprint(task_result)

    with open("task_result.csv", 'w', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=task_result[0].keys())
        writer.writeheader()
        for eachResult in task_result:
            writer.writerow(eachResult)


if __name__ == "__main__":
    main()
