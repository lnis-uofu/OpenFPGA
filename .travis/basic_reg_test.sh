#!/bin/bash

set -e

start_section "OpenFPGA.TaskTun" "${GREEN}..Running_Regression..${NC}"
cd ${TRAVIS_BUILD_DIR}

###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "Basic regression tests";

echo -e "Testing configuration chain of a K4N4 FPGA";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/full_testbench/configuration_chain --debug --show_thread_logs
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/preconfig_testbench/configuration_chain --debug --show_thread_logs

echo -e "Testing fram-based configuration protocol of a K4N4 FPGA";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/full_testbench/configuration_frame --debug --show_thread_logs
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/full_testbench/fast_configuration_frame --debug --show_thread_logs
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/preconfig_testbench/configuration_frame --debug --show_thread_logs

echo -e "Testing memory bank configuration protocol of a K4N4 FPGA";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/full_testbench/memory_bank --debug --show_thread_logs
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/full_testbench/fast_memory_bank --debug --show_thread_logs
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/preconfig_testbench/memory_bank --debug --show_thread_logs

echo -e "Testing standalone (flatten memory) configuration protocol of a K4N4 FPGA";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/full_testbench/flatten_memory --debug --show_thread_logs
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/preconfig_testbench/flatten_memory --debug --show_thread_logs

echo -e "Testing fabric Verilog generation only";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/generate_fabric --debug --show_thread_logs

echo -e "Testing Verilog testbench generation only";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/generate_testbench --debug --show_thread_logs

echo -e "Testing bitstream generation only";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/generate_bitstream --debug --show_thread_logs

echo -e "Testing user-defined simulation settings: clock frequency and number of cycles";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/fixed_simulation_settings --debug --show_thread_logs

echo -e "Testing SDC generation with time units";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/sdc_time_unit --debug --show_thread_logs

end_section "OpenFPGA.TaskTun"
