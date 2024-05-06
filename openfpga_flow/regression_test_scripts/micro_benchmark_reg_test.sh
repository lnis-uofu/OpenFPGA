#!/bin/bash

set -e
source openfpga.sh
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "Micro benchmark regression tests";
run-task benchmark_sweep/counter8 $@
run-task benchmark_sweep/counter8_full_testbench $@
run-task benchmark_sweep/counter128 $@
run-task benchmark_sweep/mac_units $@

# Verify MCNC big20 benchmark suite with ModelSim
# Please make sure you have ModelSim installed in the environment
# Otherwise, it will fail
run-task benchmark_sweep/mcnc_big20 $@
#python3 openfpga_flow/scripts/run_modelsim.py mcnc_big20 --run_sim

run-task benchmark_sweep/signal_gen $@
