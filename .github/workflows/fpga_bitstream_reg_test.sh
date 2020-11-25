#!/bin/bash

set -e

###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-Bitstream regression tests";

echo -e "Testing bitstream generation for an auto-sized device";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_bitstream/generate_bitstream/device_auto --debug --show_thread_logs


echo -e "Testing bitstream generation for an 48x48 FPGA device";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_bitstream/generate_bitstream/device_48x48 --debug --show_thread_logs

echo -e "Testing bitstream generation for an 96x96 FPGA device";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_bitstream/generate_bitstream/device_96x96 --debug --show_thread_logs

echo -e "Testing loading architecture bitstream from an external file";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_bitstream/load_external_architecture_bitstream --debug --show_thread_logs
