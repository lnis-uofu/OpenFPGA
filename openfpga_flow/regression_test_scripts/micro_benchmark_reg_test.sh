#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "Micro benchmark regression tests";
# TODO: enable this when flow-run script support same top module name in one task configuration file
#run-task benchmark_sweep/counter --debug --show_thread_logs
run-task benchmark_sweep/mac_units --debug --show_thread_logs
