import os
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
import xml.etree.ElementTree as ET


flow_script_dir = os.path.dirname(os.path.abspath(__file__))
openfpga_base_dir = os.path.abspath(
    os.path.join(flow_script_dir, os.pardir, os.pardir))
default_cad_tool_conf = os.path.join(flow_script_dir, os.pardir, 'misc',
                                     'fpgaflow_default_tool_path.conf')
launch_dir = os.getcwd()

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Setting up print and logging system
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
logging.basicConfig(level=logging.INFO,
                    format='%(levelname)s (%(threadName)-9s) - %(message)s')
logger = logging.getLogger('OpenFPGA_Task_logs')

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Reading commnad-line argument
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
parser = argparse.ArgumentParser()
parser.add_argument('arch_file', type=str)
parser.add_argument('benchmark_files', type=str, nargs='+')
parser.add_argument('--top_module', type=str)
parser.add_argument('--fpga_flow', type=str, default="yosys_vpr")
parser.add_argument('--cad_tool_conf',
                    type=str,
                    default=default_cad_tool_conf,
                    help="CAD tool path and configurations")
parser.add_argument('--run_dir',
                    type=str,
                    default=os.path.join(openfpga_base_dir,  'tmp'),
                    help="Directory to store intermidiate file & final results")
args = parser.parse_args()


# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Reading CAD Tools path configuration file
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
script_env_vars = {"PATH": {
    "OPENFPGA_FLOW_PATH": flow_script_dir,
    "OPENFPGA_PATH": openfpga_base_dir}}

config = ConfigParser(interpolation=ExtendedInterpolation())
config.read_dict(script_env_vars)
config.read_file(
    open(os.path.join(args.cad_tool_conf)))
cad_tools = config["CAD_TOOLS_PATH"]


# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Main program starts here
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
def main():
    validate_command_line_arguments(args)
    prepare_run_directory(args.run_dir)
    if (args.fpga_flow == "yosys_vpr"):
        logger.info('Running "yosys_vpr" Flow')
        run_yosys_with_abc()
        exit()
    if (args.fpga_flow == "vtr"):
        run_odin2()
        run_abc_vtr()
    if (args.fpga_flow == "vtr_standard"):
        run_abc_for_standarad()
    run_ace2()
    run_vpr()
    exit()

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Subroutines starts here
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


def validate_command_line_arguments(args):
    """
    TODO :
    This funtion validates all supplied paramters
    and check for compatibility
    Chec correct flow
    Check if architecture and circuit files exist
    if argument provide relative path replace to absolute
    benchmark argument convert glob to list of files
    Dont maintain the directory strcuture
    Throw error for directory in benchmark
    """
    logger.info("Parsing commnad line arguments - Pending implementation")

    # Filter provided architecrue files
    args.arch_file = os.path.abspath(args.arch_file)
    if not os.path.isfile(args.arch_file):
        clean_up_and_exit("Architecure file not found. -%s", args.arch_file)

    # Filter provided benchmark files
    for index, everyinput in enumerate(args.benchmark_files):
        args.benchmark_files[index] = os.path.abspath(everyinput)
        for everyfile in glob.glob(args.benchmark_files[index]):
            if not os.path.isfile(everyfile):
                clean_up_and_exit(
                    "Failed to copy benchmark file-%s", args.arch_file)
    pass


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
    tmpl = Template(open(args.arch_file).read())
    arch_filename = os.path.basename(args.arch_file)
    args.arch_file = os.path.join(run_dir, "arch", arch_filename)
    with open(args.arch_file, 'w') as archfile:
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
    logger.error(msg)
    logger.error("Existing . . . . . .")
    exit()


def run_yosys_with_abc():
    # Extract lut Input size from architecture file
    tree = ET.parse(args.arch_file)
    root = tree.getroot()
    try:
        lut_size = max([int(pb_type.find("input").get("num_pins"))
                        for pb_type in root.iter("pb_type")
                        if pb_type.get("class") == "lut"])
        logger.info("Running Yosys with lut_size = %s", lut_size)
    except:
        logger.exception("Failed to extract lut_size from XML file")
        clean_up_and_exit("")

    # Yosys script parameter mapping
    ys_params = {
        "READ_VERILOG_FILE": " \n".join([
            "read_verilog -nolatches " + shlex.quote(eachfile)
            for eachfile in args.benchmark_files]),
        "TOP_MODULE": args.top_module,
        "LUT_SIZE": lut_size,
        "OUTPUT_BLIF": args.top_module+".blif",
    }
    yosys_template = os.path.join(
        cad_tools["misc_dir"], "ys_tmpl_yosys_vpr_flow.ys")
    tmpl = Template(open(yosys_template).read())
    with open("yosys.ys", 'w') as archfile:
        archfile.write(tmpl.substitute(ys_params))

    try:
        with open('yosys_output.txt', 'w+') as output:
            process = subprocess.run([cad_tools["yosys_path"], 'yosys1.ys'],
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
    logger.info("Yosys output written in file yosys_output.txt")


def run_odin2():
    pass


def run_abc_vtr():
    pass


def run_abc_for_standarad():
    pass


def run_ace2():

    pass


def run_vpr():
    pass
# def generate_single_task_actions(taskname):
#     """
#     This script generates all the scripts required for each benchmark
#     """
#     curr_task_dir=os.path.join(gc["task_dir"], taskname)
#     if not os.path.isdir(curr_task_dir):
#         clean_up_and_exit("Task directory not found")
#     os.chdir(curr_task_dir)

#     curr_task_conf_file=os.path.join(curr_task_dir, "config", "task.conf")
#     if not os.path.isfile(curr_task_conf_file):
#         clean_up_and_exit(
#             "Missing configuration file for task %s" % curr_task_dir)

#     task_conf=ConfigParser(allow_no_value = True,
#                              interpolation = ExtendedInterpolation())
#     task_conf.optionxform=str
#     task_conf.read_dict(script_env_vars)
#     task_conf.read_file(open(curr_task_conf_file))
#     # Check required sections in config file
#     required_sec=["GENERAL", "BENCHMARKS", "ARCHITECTURES", "POST_RUN"]
#     missing_section=list(set(required_sec)-set(task_conf.sections()))
#     if missing_section:
#         clean_up_and_exit(
#             "Missing section %s in task configuration file" % " ".join(missing_section))

#     benchmark_list=[]
#     for _, bench_file in task_conf["BENCHMARKS"].items():
#         if(glob.glob(bench_file)):
#             benchmark_list.append(bench_file)
#         else:
#             logger.warning(
#                 "File Not Found: Skipping %s benchmark " % bench_file)

#     # Check if all benchmark/architecture files exits
#     archfile_list=[]
#     for _, arch_file in task_conf["ARCHITECTURES"].items():
#         arch_full_path=arch_file
#         if os.path.isfile(arch_full_path):
#             archfile_list.append(arch_full_path)
#         else:
#             logger.warning(
#                 "File Not Found: Skipping %s architecture " % arch_file)

#     script_list=[]
#     for eacharch in archfile_list:
#         script_list.append(create_run_script(gc["temp_run_dir"],
#                                              eacharch,
#                                              benchmark_list,
#                                              task_conf["GENERAL"]["power_tech_file"],
#                                              task_conf["SCRIPT_PARAM"]))
#     return script_list


# def create_run_script(task_run_dir, archfile, benchmark_list, power_tech_file, additional_fpga_flow_params):
#     """
#     Create_run_script function accespts run directory, architecture list and
#     fpga_flow configuration file and prepare final executable fpga_flow script
#     TODO : Replace this section after convert fpga_flow to python script
#     Config file creation and bechnmark list can be skipped
#     """
#     # = = = = = = = = = File/Directory Consitancy Check = = = = = = = = = =
#     if not os.path.isdir(gc["misc_dir"]):
#         clean_up_and_exit("Miscellaneous directory does not exist")

#     fpga_flow_script=os.path.join(gc["misc_dir"], "fpga_flow_template.sh")
#     if not os.path.isfile(fpga_flow_script):
#         clean_up_and_exit("Missing fpga_flow script template %s" %
#                           fpga_flow_script)

#     fpga_flow_conf_tmpl=os.path.join(gc["misc_dir"], "fpga_flow_script.conf")
#     if not os.path.isfile(fpga_flow_conf_tmpl):
#         clean_up_and_exit("fpga_flow configuration tempalte is missing %s" %
#                           fpga_flow_conf_tmpl)

#     # = = = = = = = = = = = =  Create execution folder = = = = = = = = = = = =
#     # TODO : this directory should change as <architecture>/<benchmark>/{conf_opt}
#     curr_job_dir=os.path.join(task_run_dir, "tmp")
#     if os.path.isdir(curr_job_dir):
#         shutil.rmtree(curr_job_dir)
#     os.makedirs(curr_job_dir)
#     os.chdir(curr_job_dir)

#     # = = = = = = = = = = = Create config file= = = = = = = = = = = = = = = =
#     fpga_flow_conf=ConfigParser(
#         strict=False,
#         interpolation=ExtendedInterpolation())
#     fpga_flow_conf.read_dict(script_env_vars)
#     fpga_flow_conf.read_file(open(fpga_flow_conf_tmpl))

#     # HACK: Find better way to resolve all interpolations in the script and write back
#     for eachSection in fpga_flow_conf:
#         for eachkey in fpga_flow_conf[eachSection].keys():
#             fpga_flow_conf[eachSection][eachkey] = fpga_flow_conf.get(
#                 eachSection, eachkey)

#     # Update configuration file with script realated parameters
#     fpga_flow_conf["flow_conf"]["vpr_arch"] = archfile
#     fpga_flow_conf["flow_conf"]["power_tech_xml"] = power_tech_file

#     # Remove extra path section and create configuration file
#     fpga_flow_conf.remove_section("PATH")
#     with open("openfpga_job.conf", 'w') as configfile:
#         fpga_flow_conf.write(configfile)

#     # = = = = = = = = = = = Create Benchmark List file = = = = = = = = = = = =
#     # TODO: This script strips common path from bechmark list and add
#     # only single directory and filename to benchmarklist file
#     # This can be imporoved by modifying fpga_flow script
#     with open("openfpga_benchmark_list.txt", 'w') as configfile:
#         configfile.write("# Circuit Names, fixed routing channel width\n")
#         for eachBenchMark in benchmark_list:
#             configfile.write(eachBenchMark.replace(
#                 fpga_flow_conf["dir_path"]["benchmark_dir"], ""))
#             configfile.write(",30")
#             configfile.write("\n")

#     # = = = = = = = = = Create fpga_flow_shell Script  = = = = = = = = = = = =
#     d = {
#         "fpga_flow_script": shlex.quote(gc["script_default"]),
#         "conf_file": shlex.quote(os.path.join(os.getcwd(), "openfpga_job.conf")),
#         "benchmark_list_file":  shlex.quote(os.path.join(os.getcwd(), "openfpga_benchmark_list.txt")),
#         "csv_rpt_file": shlex.quote(os.path.join(os.getcwd(), os.path.join(gc["csv_rpt_dir"], "fpga_flow.csv"))),
#         "verilog_output_path": shlex.quote(os.path.join(os.getcwd(), gc["verilog_output_path"])),
#         "additional_params": " \\\n    ".join(["-%s  %s" % (key, value or "") for key, value in additional_fpga_flow_params.items()])
#     }
#     result = Template(open(fpga_flow_script).read()).substitute(d)
#     fpga_flow_script_path = os.path.join(os.getcwd(), "openfpga_flow.sh")
#     with open(fpga_flow_script_path, 'w') as configfile:
#         configfile.write(result)
#     return fpga_flow_script_path


# def run_single_script(s, script_path):
#     logging.debug('Waiting to join the pool')
#     with s:
#         name = threading.currentThread().getName()
#         subprocess.run(["bash", script_path], stdout=subprocess.PIPE)
#         logging.info("%s Finished " % name)


# def run_actions(actions):
#     thread_sema = threading.Semaphore(args.maxthreads)
#     thred_list = []
#     for index, eachAction in enumerate(actions):
#         t = threading.Thread(target=run_single_script,
#                              name='benchmark_' + str(index),
#                              args=(thread_sema, eachAction))
#         t.start()
#         thred_list.append(t)

#     for eachthread in thred_list:
#         eachthread.join()


if __name__ == "__main__":
    main()
