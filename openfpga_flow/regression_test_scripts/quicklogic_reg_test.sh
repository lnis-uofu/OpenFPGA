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

echo -e "Testing yosys flow using custom ys script for running multi-clock quicklogic device";
run-task quicklogic_tests/counter_5clock_test --debug --show_thread_logs
run-task quicklogic_tests/sdc_controller_test --debug --show_thread_logs

echo -e "Testing yosys flow using custom ys script for adders in quicklogic device";
run-task quicklogic_tests/lut_adder_test --debug --show_thread_logs
