#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-SDC regression tests";

echo -e "Testing SDC generation with time units";
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_as --debug --show_thread_logs
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_fs --debug --show_thread_logs
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_ps --debug --show_thread_logs
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_ns --debug --show_thread_logs
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_us --debug --show_thread_logs
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_ms --debug --show_thread_logs
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_default --debug --show_thread_logs
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_ks --debug --show_thread_logs
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_Ms --debug --show_thread_logs
