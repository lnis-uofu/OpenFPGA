from string import Template
import sys
import os
import pprint
import argparse
import subprocess
import logging
from pprint import pprint
from configparser import ConfigParser

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Configure logging system
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
logging.basicConfig(level=logging.INFO, stream=sys.stdout,
                    format='%(levelname)s (%(threadName)10s) - %(message)s')
logger = logging.getLogger('Modelsim_run_log')

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Parse commandline arguments
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
parser = argparse.ArgumentParser()
parser.add_argument('files', nargs='+')
parser.add_argument('--formality_template', type=str,
                    help="Modelsim verification template file")
parser.add_argument('--run_sim', action="store_true",
                    help="Execute generated script in formality")
args = parser.parse_args()

# Consider default formality script template
if not args.formality_template:
    task_script_dir = os.path.dirname(os.path.abspath(__file__))
    args.formality_template = os.path.join(task_script_dir, os.pardir,
                                           "misc", "formality_template.tcl")

args.formality_template = os.path.abspath(args.formality_template)


def main():
    for eachFile in args.files:
        eachFile = os.path.abspath(eachFile)
        pDir = os.path.dirname(eachFile)
        os.chdir(pDir)

        config = ConfigParser()
        config.read(eachFile)

        port_map = ("set_user_match r:%s/%%s i:/WORK/%%s -type port -noninverted" % (
            "/WORK/" + config["BENCHMARK_INFO"]["src_top_module"]
        ))
        cell_map = ("set_user_match r:%s/%%s i:/WORK/%%s -type cell -noninverted" % (
            "/WORK/" + config["BENCHMARK_INFO"]["src_top_module"]
        ))

        lables = {
            "SOURCE_DESIGN_FILES": config["BENCHMARK_INFO"]["benchmark_netlist"],
            "SOURCE_TOP_MODULE": "/WORK/" + config["BENCHMARK_INFO"]["src_top_module"],

            "IMPL_DESIGN_FILES": " ".join(
                [val for key, val in config["FPGA_INFO"].items()
                 if "impl_netlist_" in key]),
            "IMPL_TOP_DIR": "/WORK/" + config["FPGA_INFO"]["impl_top_module"],

            "PORT_MAP_LIST": "\n".join([port_map %
                                        ele for ele in
                                        config["PORT_MATCHING"].items()]),
            "REGISTER_MAP_LIST": "\n".join([cell_map %
                                            ele for ele in
                                            config["REGISTER_MATCH"].items()]),
        }

    tmpl = Template(open(args.formality_template, encoding='utf-8').read())
    with open(os.path.join(pDir, "Output.tcl"), 'w', encoding='utf-8') as tclout:
        tclout.write(tmpl.substitute(lables))
    if args.run_sim:
        formality_run_string = ["formality", "-file", "Output.tcl"]
        run_command("Formality Run", "formality_run.log", formality_run_string)
    else:
        with open("Output.tcl", 'r', encoding='utf-8') as tclout:
            print(tclout.read())


def run_command(taskname, logfile, command, exit_if_fail=True):
    os.chdir(os.pardir)
    logger.info("Launching %s " % taskname)
    with open(logfile, 'w+') as output:
        try:
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
            return None
    logger.info("%s is written in file %s" % (taskname, logfile))
    return process.stdout


if __name__ == "__main__":
    main()
