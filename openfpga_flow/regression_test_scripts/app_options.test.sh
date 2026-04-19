#!/bin/bash

source openfpga.sh

###############################################
# OpenFPGA Shell with VPR8
##############################################

run_tcl_command() {
    local test_name="$1"
    local command="$2"
    mkdir -p ${OPENFPGA_PATH}/tmp/${test_name}
    (cd ${OPENFPGA_PATH}/tmp/${test_name} && ${OPENFPGA_PATH}/build/openfpga/openfpga -batch -x "${command};" 2>&1 > /dev/null)
    # (cd ${OPENFPGA_PATH}/tmp/${test_name} && ${OPENFPGA_PATH}/build/openfpga/openfpga -batch -x "${command};")
    if [ $? -ne 0 ]; then
        echo "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        echo "Test '${test_name}' failed"
        echo "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
    else
        echo "Test '${test_name}' passed"
    fi
}

rm -rf ${OPENFPGA_PATH}/tmp

echo -e "Testing app options commands"

# Test 1: show_vpr_setup
run_tcl_command "test1" "show_vpr_setup;"

# Test 2: report_app_option
run_tcl_command "test2" "report_app_option -n atom.const_gen_inference;"

# Test 3: set_app_option and show_vpr_setup
run_tcl_command "test3" "
    set_app_option -n atom.const_gen_inference -v comb_seq;
    report_app_option -n atom.const_gen_inference;
    show_vpr_setup;"

# Test 4: run a complete flow from openfpga shell
run_tcl_command "test4" "
    read_vpr_arch -f ${OPENFPGA_PATH}/openfpga_flow/vpr_arch/k4_frac_N4_tileable_40nm_custrrg.xml -l FPGA44_CUSTOM_GRAPH;
    read_circuit -f ${OPENFPGA_PATH}/openfpga_flow/benchmarks/micro_benchmark/and2/and2.blif;
    pack;
    place;
    route;" | tee openfpga_shell_output.log


# Test 5: run a complete flow from openfpga shell
run_tcl_command "test5" "
    set_app_option -n router.flat_routing -v true;
    read_vpr_arch -f ${OPENFPGA_PATH}/openfpga_flow/vpr_arch/k4_frac_N4_tileable_40nm_custrrg.xml -l FPGA44_CUSTOM_GRAPH;
    read_circuit -f ${OPENFPGA_PATH}/openfpga_flow/benchmarks/micro_benchmark/and2/and2.blif;
    pack;
    place;
    route;
    analysis;" | tee openfpga_shell_output.log


# Test 6: run a complete flow from openfpga shell
run_tcl_command "test6" "
    set_app_option -n router.flat_routing -v true;
    read_vpr_arch -f ${OPENFPGA_PATH}/openfpga_flow/vpr_arch/k4_frac_N4_tileable_40nm_custrrg.xml -l FPGA44_CUSTOM_GRAPH;
    read_circuit -f ${OPENFPGA_PATH}/openfpga_flow/benchmarks/micro_benchmark/and2/and2.blif;
    pack;
    report_cluster --query a " | tee openfpga_shell_output.log


# run_tcl_command "
#     vpr ${OPENFPGA_PATH}/openfpga_flow/vpr_arch/k4_frac_N4_tileable_40nm_custrrg.xml \
#     ${OPENFPGA_PATH}/openfpga_flow/benchmarks/micro_benchmark/and2/and2.blif \
#     --device FPGA44_CUSTOM_GRAPH --route_chan_width 100" | tee openfpga_shell_output1.log