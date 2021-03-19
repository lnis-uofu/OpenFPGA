#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "VTR benchmark regression tests";
run-task benchmark_sweep/vtr_benchmarks --debug --show_thread_logs
