#!/bin/bash

set -e
source openfpga.sh
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-SDC regression tests";

echo -e "Testing SDC generation with time units";
run-task fpga_sdc/sdc_time_unit --debug --show_thread_logs
