#!/bin/bash

set -e

###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-SDC regression tests";

echo -e "Testing SDC generation with time units";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_sdc/sdc_time_unit --debug --show_thread_logs
