#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-Bitstream regression tests";

echo -e "Testing bitstream generation for an auto-sized device";
run-task fpga_bitstream/generate_bitstream/device_auto --debug --show_thread_logs


echo -e "Testing bitstream generation for an 48x48 FPGA device";
run-task fpga_bitstream/generate_bitstream/device_48x48 --debug --show_thread_logs

echo -e "Testing bitstream generation for an 96x96 FPGA device";
run-task fpga_bitstream/generate_bitstream/device_96x96 --debug --show_thread_logs

echo -e "Testing loading architecture bitstream from an external file";
run-task fpga_bitstream/load_external_architecture_bitstream --debug --show_thread_logs

echo -e "Testing repacker capability in identifying wire LUTs";
run-task fpga_bitstream/repack_wire_lut --debug --show_thread_logs
