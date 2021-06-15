#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################

echo -e "Benchmark sweep tests";
run-task benchmark_sweep/signal_gen --debug --show_thread_logs