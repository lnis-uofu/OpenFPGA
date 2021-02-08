#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-SDC regression tests";

echo -e "Testing SDC generation with time units";
run-task fpga_sdc/sdc_time_unit --debug --show_thread_logs


# Repgression test to test multi-user enviroment
# TODO : While restructuring regression test files
#        move this to separate file
cp -r */*/fpga_sdc/sdc_time_unit /tmp/
cd /tmp/ && run-task sdc_time_unit