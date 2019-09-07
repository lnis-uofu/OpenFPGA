from string import Template
import sys
import os
import argparse
import subprocess
import logging
from pprint import pprint

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Configure logging system
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
logging.basicConfig(level=logging.INFO, stream=sys.stdout,
                    format='%(levelname)s (%(threadName)10s) - %(message)s')
logger = logging.getLogger('Modelsim_run_log')

parser = argparse.ArgumentParser()
parser.add_argument('files', nargs='+')
parser.add_argument('--modelsim_template', type=str,
                    help="Modelsim verification template file")
parser.add_argument('--run_sim', action="store_true",
                    help="Execute generated script in formality")
args = parser.parse_args()


if not args.modelsim_template:
    task_script_dir = os.path.dirname(os.path.abspath(__file__))
    args.modelsim_template = os.path.join(task_script_dir, os.pardir,
                                          "misc", "modelsim_template.j2")

args.modelsim_template = os.path.abspath(args.modelsim_template)


def main():
    for eachFile in args.files:
        eachFile = os.path.abspath(eachFile)
        directory = os.path.dirname(eachFile)
        os.chdir(directory)
        with open(eachFile, 'r') as fp:
            lines = fp.read().split("\n")
            SplitL = [indx for indx, eachL in enumerate(lines) if eachL == ""]
            SplitL = list(zip([0] + SplitL[:-1], SplitL))
            for indx, eachSection in enumerate(SplitL):
                SplitL[indx] = list(filter(None, lines[slice(*eachSection)]))

        match_str = "set_user_match r:%s i:%s -type port -noninverted"
        lables = {"SOURCE_DESIGN": " ".join(SplitL[0]),
                  "SOURCE_TOP_DIR": "/WORK/" + " ".join(SplitL[1]),
                  "IMPL_DESIGN": " ".join(SplitL[2]),
                  "IMPL_TOP_DIR": "/WORK/" + " ".join(SplitL[3]),
                  "MATCH_MODUEL_LIST": "\n".join([match_str % tuple(eachPort.split()) for eachPort in SplitL[4]])
                  }

    tmpl = Template(open(args.modelsim_template, encoding='utf-8').read())
    with open("Output.tcl", 'w', encoding='utf-8') as tclout:
        tclout.write(tmpl.substitute(lables))
    if args.run_sim:
        formality_run_string = ["formality", "-file", "Output.tcl"]
        run_command("Modelsim run", "modelsim_run.log", formality_run_string)
    else:
        with open("Output.tcl", 'r', encoding='utf-8') as tclout:
            print(tclout.read())


def run_command(taskname, logfile, command, exit_if_fail=True):
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
