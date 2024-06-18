#!/bin/bash

set -e
source openfpga.sh
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-SDC regression tests";

echo -e "Testing SDC generation with time units";
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_as $@
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_fs $@
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_ps $@
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_ns $@
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_us $@
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_ms $@
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_default $@
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_ks $@
run-task fpga_sdc/sdc_time_unit/sdc_time_unit_Ms $@
