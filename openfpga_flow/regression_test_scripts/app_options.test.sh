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
        echo "Command '${command}' failed in OpenFPGA Shell with VPR8"
        exit 1
    fi
}

echo -e "Testing app options commands in OpenFPGA Shell with VPR8"

run_openfpga_command "report_app_option"

run_openfpga_command "\
set_app_option -n atom.const_gen_inference -v comb_seq; \
report_app_option -n atom.const_gen_inference"


run_openfpga_command "\
read_vpr_arch -f ${OPENFPGA_PATH}/openfpga_flow/vpr_arch/k4_frac_N4_tileable_40nm_custrrg.xml"