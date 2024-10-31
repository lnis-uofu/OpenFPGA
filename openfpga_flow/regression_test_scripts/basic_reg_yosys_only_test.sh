#!/bin/bash

set -e
source openfpga.sh
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "Basic regression tests for Yosys-only flow";

echo -e "Testing configuration chain of a K4N4 FPGA";
run-task basic_tests/yosys_only $@

# Repgression test to test multi-user enviroment
cp -r */*/basic_tests/yosys_only /tmp/
cd /tmp/ && run-task yosys_only $@
cd -
