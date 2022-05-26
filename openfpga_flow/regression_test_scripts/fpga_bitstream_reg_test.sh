#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-Bitstream regression tests";

echo -e "Testing bitstream generation for an auto-sized device";
run-task fpga_bitstream/generate_bitstream/configuration_chain/device_auto $@
run-task fpga_bitstream/generate_bitstream/ql_memory_bank_shift_register/device_auto $@

echo -e "Testing bitstream generation for an 48x48 FPGA device";
run-task fpga_bitstream/generate_bitstream/configuration_chain/device_48x48 $@
run-task fpga_bitstream/generate_bitstream/ql_memory_bank_shift_register/device_48x48 $@

echo -e "Testing bitstream generation for an 96x96 FPGA device";
run-task fpga_bitstream/generate_bitstream/configuration_chain/device_96x96 $@
run-task fpga_bitstream/generate_bitstream/ql_memory_bank_shift_register/device_96x96 $@

echo -e "Testing loading architecture bitstream from an external file";
run-task fpga_bitstream/load_external_architecture_bitstream $@

echo -e "Testing repacker capability in identifying wire LUTs";
run-task fpga_bitstream/repack_wire_lut $@

echo -e "Testing overloading default paths for programmable interconnect when generating bitstream";
run-task fpga_bitstream/overload_mux_default_path $@

echo -e "Testing outputting I/O mapping result to file";
run-task fpga_bitstream/write_io_mapping $@

echo -e "Testing report bitstream distribution to file";
run-task fpga_bitstream/report_bitstream_distribution/default_depth $@
run-task fpga_bitstream/report_bitstream_distribution/custom_depth $@

echo -e "Testing bitstream file with don't care bits";
run-task fpga_bitstream/dont_care_bits/ql_memory_bank_flatten $@
run-task fpga_bitstream/dont_care_bits/ql_memory_bank_shift_register $@
