#!/bin/bash

set -e
source openfpga.sh

###############################################
# OpenFPGA Shell with VPR8
##############################################

run_openfpga_command() {
    local command="$1"
    ${OPENFPGA_PATH}/build/openfpga/openfpga -x "${command}; exit;"
    if [ $? -ne 0 ]; then
        echo "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        echo "Command '${command}' failed in OpenFPGA Shell with VPR8"
        echo "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
    fi
}

echo -e "Testing app options commands in OpenFPGA Shell with VPR8"

run_openfpga_command "show_vpr_setup;"

run_openfpga_command "\
set_app_option -n atom.const_gen_inference -v comb; \
show_vpr_setup;"


run_openfpga_command "
vpr ${OPENFPGA_PATH}/openfpga_flow/vpr_arch/k4_frac_N4_tileable_40nm_custrrg.xml \
${OPENFPGA_PATH}/openfpga_flow/benchmarks/micro_benchmark/and2/and2.blif \
--device FPGA44_CUSTOM_GRAPH --route_chan_width 100"


run_openfpga_command "\
read_vpr_arch -f ${OPENFPGA_PATH}/openfpga_flow/vpr_arch/k4_frac_N4_tileable_40nm_custrrg.xml -l FPGA44_CUSTOM_GRAPH;
read_circuit -f ${OPENFPGA_PATH}/openfpga_flow/benchmarks/micro_benchmark/and2/and2.blif;
pack;"