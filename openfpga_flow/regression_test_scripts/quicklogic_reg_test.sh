#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "QuickLogic regression tests";

echo -e "Testing yosys flow using custom ys script for running quicklogic device";
run-task quicklogic_tests/flow_test --debug --show_thread_logs
