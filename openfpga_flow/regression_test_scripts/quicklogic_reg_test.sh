#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "QuickLogic regression tests";

# TODO: Disabled all the tests here because Quicklogic's synthesis script is not in Yosys v0.10 release. Will bring back once Quicklogic manages to merge their contribution to Yosys upstream

##echo -e "Testing yosys flow using custom ys script for running quicklogic device";
##run-task quicklogic_tests/flow_test --debug --show_thread_logs
##
##echo -e "Testing yosys flow using custom ys script for running multi-clock quicklogic device";
##run-task quicklogic_tests/counter_5clock_test --debug --show_thread_logs
##run-task quicklogic_tests/sdc_controller_test --debug --show_thread_logs
##
##echo -e "Testing yosys flow using custom ys script for adders in quicklogic device";
##run-task quicklogic_tests/lut_adder_test --debug --show_thread_logs
