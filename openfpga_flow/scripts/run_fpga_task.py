import os
import shutil
import time
import shlex
import argparse
from configparser import ConfigParser, ExtendedInterpolation
import logging
import glob
import subprocess
import threading
from string import Template
import pprint

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Configure logging system
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
logging.basicConfig(level=logging.INFO,
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
    task_action = []
    validate_command_line_arguments()
    for eachtask in args.tasks:
        logger.info("Currently running task %s" % eachtask)
        task_action += generate_each_task_actions(eachtask)
    pprint.pprint(task_action)
    # run_actions(task_action)
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
        clean_up_and_exit("Task directory not found")
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
        logger.exception()
        logger.error("Failed to create new run directory in task directory")

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
    benchmark_list, bench_files = [], []
    for bech_name, each_benchmark in task_conf["BENCHMARKS"].items():
        for eachpath in each_benchmark.split(","):
            bench_files += glob.glob(eachpath)
        for eachfile in bench_files:
            if not os.path.isfile(eachfile):
                clean_up_and_exit("Missing source file %s in benchmark %s " %
                                  (eachfile, bech_name))

        ys_for_task = task_conf.get("SYNTHESIS_PARAM", "bench_yosys_common")
        benchmark_list.append({
            "files": bench_files,
            "top_module": task_conf.get("SYNTHESIS_PARAM", bech_name+"_top",
                                        fallback="top"),
            "ys_script": task_conf.get("SYNTHESIS_PARAM", bech_name+"_yosys",
                                       fallback=ys_for_task)
        })

    for eachbench in benchmark_list:
        pprint.pprint(eachbench)

    # Create OpenFPGA flow run commnad for each combination of
    # architecture, benchmark and parameters
    flow_run_cmd_list = []
    print(archfile_list)
    print(benchmark_list)
    # for arch in archfile_list:
    #     for bench in benchmark_list:
    #         flow_run_dir = get_flow_rundir(arch, bench["top_module"])
    #         print(flow_run_dir)
    #         flow_run_cmd_list.append(create_run_script(
    #             gc["temp_run_dir"],
    #             arch,
    #             bench,
    #             task_conf["GENERAL"]["power_tech_file"],
    #             task_conf["SCRIPT_PARAM"]))
    return flow_run_cmd_list


def get_flow_rundir(arch, top_module, flow_params=None):
    path = [
        os.path.basename(arch).replace(".xml", ""),
        top_module,
        flow_params if flow_params else "common"
    ]
    return os.path.join(*path)


def create_run_script(curr_job_dir, archfile, benchmark_list,
                      power_tech_file, additional_fpga_flow_params):
    """
    Create_run_script function accepts run directory, architecture list and
    fpga_flow configuration file and prepare final executable fpga_flow script
    TODO : Replace this section after convert fpga_flow to python script
    Config file creation and bechnmark list can be skipped
    """
    print(archfile)
    return
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
        shutil.rmtree(curr_job_dir)
    os.makedirs(curr_job_dir)

    # Make execution command to run Open FPGA flow
    command = [archfile] + benchmark_list

    # # = = = = = = = = = = = Create config file= = = = = = = = = = = = = = = =
    # fpga_flow_conf = ConfigParser(
    #     strict=False,
    #     interpolation=ExtendedInterpolation())
    # fpga_flow_conf.read_dict(script_env_vars)
    # fpga_flow_conf.read_file(open(fpga_flow_conf_tmpl))

    # # HACK: Find better way to resolve all interpolations in the script and write back
    # for eachSection in fpga_flow_conf:
    #     for eachkey in fpga_flow_conf[eachSection].keys():
    #         fpga_flow_conf[eachSection][eachkey] = fpga_flow_conf.get(
    #             eachSection, eachkey)

    # # Update configuration file with script realated parameters
    # fpga_flow_conf["flow_conf"]["vpr_arch"] = archfile
    # fpga_flow_conf["flow_conf"]["power_tech_xml"] = power_tech_file

    # # Remove extra path section and create configuration file
    # fpga_flow_conf.remove_section("PATH")
    # with open("openfpga_job.conf", 'w') as configfile:
    #     fpga_flow_conf.write(configfile)

    # # = = = = = = = = = = = Create Benchmark List file = = = = = = = = = = = =
    # # TODO: This script strips common path from bechmark list and add
    # # only single directory and filename to benchmarklist file
    # # This can be imporoved by modifying fpga_flow script
    # with open("openfpga_benchmark_list.txt", 'w') as configfile:
    #     configfile.write("# Circuit Names, fixed routing channel width\n")
    #     for eachBenchMark in benchmark_list:
    #         configfile.write(eachBenchMark.replace(
    #             fpga_flow_conf["dir_path"]["benchmark_dir"], ""))
    #         configfile.write(",30")
    #         configfile.write("\n")

    # # = = = = = = = = = Create fpga_flow_shell Script  = = = = = = = = = = = =
    # d = {
    #     "fpga_flow_script": shlex.quote(gc["script_default"]),
    #     "conf_file": shlex.quote(os.path.join(os.getcwd(), "openfpga_job.conf")),
    #     "benchmark_list_file":  shlex.quote(os.path.join(os.getcwd(), "openfpga_benchmark_list.txt")),
    #     "csv_rpt_file": shlex.quote(os.path.join(os.getcwd(), os.path.join(gc["csv_rpt_dir"], "fpga_flow.csv"))),
    #     "verilog_output_path": shlex.quote(os.path.join(os.getcwd(), gc["verilog_output_path"])),
    #     "additional_params": " \\\n    ".join(["-%s  %s" % (key, value or "") for key, value in additional_fpga_flow_params.items()])
    # }
    # result = Template(open(fpga_flow_script).read()).substitute(d)
    # fpga_flow_script_path = os.path.join(os.getcwd(), "openfpga_flow.sh")
    # with open(fpga_flow_script_path, 'w') as configfile:
    #     configfile.write(result)
    return command


def run_single_script(s, script_path):
    logging.debug('Waiting to join the pool')
    with s:
        name = threading.currentThread().getName()
        subprocess.run(["bash", script_path], stdout=subprocess.PIPE)
        logging.info("%s Finished " % name)


def run_actions(actions):
    thread_sema = threading.Semaphore(args.maxthreads)
    thred_list = []
    for index, eachAction in enumerate(actions):
        t = threading.Thread(target=run_single_script,
                             name='benchmark_' + str(index),
                             args=(thread_sema, eachAction))
        t.start()
        thred_list.append(t)

    for eachthread in thred_list:
        eachthread.join()


if __name__ == "__main__":
    main()
