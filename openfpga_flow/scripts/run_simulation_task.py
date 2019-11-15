# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Script Name   : regression.py
# Description   : This script designed to run:
#                   openfpga_flow tasks
#                   run_{simulator}.py
# Args          : python3 regression.py --help
# Author        : Aurelien Alacchi
# Email         : aurelien.alacchi@utah.edu
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

modelsim="modelsim"
vcs="vcs"
formality="formality"
modelsim_file="simulation_deck_info.ini"
ini_list=""

# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
# Parse commandline arguments
# = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
parser = argparse.ArgumentParser()
parser.add_argument('tasks', nargs='+')
parser.add_argument('--maxthreads', type=int, default=2,
                    help="Number of fpga_flow threads to run default = 2," +
                    "Typically <= Number of processors on the system")
parser.add_argument('--simulator', type=str, default=modelsim,
                    help="Simulator to use. Set at \"" + modelsim + "\" by default. Can also be \"" + vcs + "\" or \"" + formality + "\"")
args = parser.parse_args()


args.tasks=str(args.tasks).strip('[]')
#print(args.tasks)
#print(args.maxthreads)

command="python3 openfpga_flow/scripts/run_fpga_task.py " + args.tasks + " --maxthreads " + str(args.maxthreads) + " --debug --show_thread_logs"

print(command)

os.system(command)

if(args.simulator == modelsim):
	command="python3 openfpga_flow/scripts/run_modelsim.py"
	os.system("grep \"INFO - Run directory :\" openfpga_flow/tasks/" + args.tasks + "/latest/*.log > paths_ini.txt")
	arguments = " --skip_prompt --run_sim";
	fp = open("paths_ini.txt")
	line = fp.readline()
	while line:
		ini_list= ini_list + line + modelsim_file
		line = fp.readline()
	ini_list = ini_list.replace("INFO - Run directory :", "")
	ini_list = ini_list.replace("\n", "/")
	fp.close()
	print(command + ini_list + arguments)
	os.system(command + ini_list + arguments)
