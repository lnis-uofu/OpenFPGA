#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "Basic regression tests for Yosys-only flow";

echo -e "Testing configuration chain of a K4N4 FPGA";
run-task basic_tests/verific_test --debug --show_thread_logs

# Repgression test to test multi-user enviroment
cp -r */*/basic_tests/full_testbench/configuration_chain /tmp/
cd /tmp/ && run-task configuration_chain --debug --show_thread_logs
