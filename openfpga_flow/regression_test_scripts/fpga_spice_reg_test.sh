#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-SPICE regression tests";

echo -e "Testing FPGA-SPICE with netlist generation";
run-task fpga_spice/generate_spice --debug --show_thread_logs
