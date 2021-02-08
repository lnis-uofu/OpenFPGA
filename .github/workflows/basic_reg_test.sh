#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "Basic regression tests";

echo -e "Testing configuration chain of a K4N4 FPGA";
run-task basic_tests/full_testbench/configuration_chain --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_chain_use_reset --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_chain_use_resetb --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_chain_use_set --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_chain_use_setb --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_chain_use_set_reset --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_chain_config_enable_scff --debug --show_thread_logs
run-task basic_tests/full_testbench/multi_region_configuration_chain --debug --show_thread_logs
run-task basic_tests/full_testbench/fast_configuration_chain --debug --show_thread_logs
run-task basic_tests/full_testbench/fast_configuration_chain_use_set --debug --show_thread_logs
run-task basic_tests/full_testbench/smart_fast_configuration_chain --debug --show_thread_logs
run-task basic_tests/full_testbench/smart_fast_multi_region_configuration_chain --debug --show_thread_logs
run-task basic_tests/preconfig_testbench/configuration_chain --debug --show_thread_logs

echo -e "Testing fram-based configuration protocol of a K4N4 FPGA";
run-task basic_tests/full_testbench/configuration_frame --debug --show_thread_logs
run-task basic_tests/full_testbench/smart_fast_configuration_frame --debug --show_thread_logs
run-task basic_tests/full_testbench/fast_configuration_frame --debug --show_thread_logs
run-task basic_tests/full_testbench/fast_configuration_frame_use_set --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_frame_ccff --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_frame_scff --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_frame_use_reset --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_frame_use_resetb --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_frame_use_set --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_frame_use_setb --debug --show_thread_logs
run-task basic_tests/full_testbench/configuration_frame_use_set_reset --debug --show_thread_logs
run-task basic_tests/full_testbench/multi_region_configuration_frame --debug --show_thread_logs
run-task basic_tests/full_testbench/smart_fast_multi_region_configuration_frame --debug --show_thread_logs
run-task basic_tests/preconfig_testbench/configuration_frame --debug --show_thread_logs

echo -e "Testing memory bank configuration protocol of a K4N4 FPGA";
run-task basic_tests/full_testbench/memory_bank --debug --show_thread_logs
run-task basic_tests/full_testbench/memory_bank_use_reset --debug --show_thread_logs
run-task basic_tests/full_testbench/memory_bank_use_resetb --debug --show_thread_logs
run-task basic_tests/full_testbench/memory_bank_use_set --debug --show_thread_logs
run-task basic_tests/full_testbench/memory_bank_use_setb --debug --show_thread_logs
run-task basic_tests/full_testbench/memory_bank_use_set_reset --debug --show_thread_logs
run-task basic_tests/full_testbench/multi_region_memory_bank --debug --show_thread_logs
run-task basic_tests/full_testbench/fast_memory_bank --debug --show_thread_logs
run-task basic_tests/full_testbench/fast_memory_bank_use_set --debug --show_thread_logs
run-task basic_tests/full_testbench/smart_fast_memory_bank --debug --show_thread_logs
run-task basic_tests/full_testbench/smart_fast_multi_region_memory_bank --debug --show_thread_logs
run-task basic_tests/preconfig_testbench/memory_bank --debug --show_thread_logs

echo -e "Testing standalone (flatten memory) configuration protocol of a K4N4 FPGA";
run-task basic_tests/full_testbench/flatten_memory --debug --show_thread_logs
run-task basic_tests/preconfig_testbench/flatten_memory --debug --show_thread_logs

echo -e "Testing fixed device layout and routing channel width";
run-task basic_tests/fixed_device_support --debug --show_thread_logs

echo -e "Testing fabric Verilog generation only";
run-task basic_tests/generate_fabric --debug --show_thread_logs

echo -e "Testing Verilog testbench generation only";
run-task basic_tests/generate_testbench --debug --show_thread_logs

echo -e "Testing separated Verilog fabric netlists and testbench locations";
run-task basic_tests/custom_fabric_netlist_location --debug --show_thread_logs

echo -e "Testing user-defined simulation settings: clock frequency and number of cycles";
run-task basic_tests/fixed_simulation_settings --debug --show_thread_logs

echo -e "Testing Secured FPGA fabrics";
run-task basic_tests/fabric_key/generate_vanilla_key --debug --show_thread_logs
run-task basic_tests/fabric_key/generate_multi_region_vanilla_key --debug --show_thread_logs
run-task basic_tests/fabric_key/generate_random_key --debug --show_thread_logs
run-task basic_tests/fabric_key/load_external_key --debug --show_thread_logs
run-task basic_tests/fabric_key/load_external_key_cc_fpga --debug --show_thread_logs
run-task basic_tests/fabric_key/load_external_key_multi_region_cc_fpga --debug --show_thread_logs


echo -e "Testing K4 series FPGA";
echo -e "Testing K4N4 with facturable LUTs";
run-task basic_tests/k4_series/k4n4_frac_lut --debug --show_thread_logs
echo -e "Testing K4N4 with hard adders";
run-task basic_tests/k4_series/k4n4_adder --debug --show_thread_logs
echo -e "Testing K4N4 without local routing architecture";
run-task basic_tests/k4_series/k4n4_no_local_routing --debug --show_thread_logs
echo -e "Testing K4N4 with block RAM";
run-task basic_tests/k4_series/k4n4_bram --debug --show_thread_logs
echo -e "Testing K4N4 with LUTRAM";
run-task basic_tests/k4_series/k4n4_lutram --debug --show_thread_logs
echo -e "Testing K4N4 with multiple lengths of routing segments";
run-task basic_tests/k4_series/k4n4_L124 --debug --show_thread_logs
echo -e "Testing K4N4 with 32-bit fracturable multiplier";
run-task basic_tests/k4_series/k4n4_frac_mult --debug --show_thread_logs
echo -e "Testing K4N5 with pattern based local routing";
run-task basic_tests/k4_series/k4n5_pattern_local_routing --debug --show_thread_logs

echo -e "Testing different tile organizations";
echo -e "Testing tiles with pins only on top and left sides";
run-task basic_tests/tile_organization/top_left_custom_pins --debug --show_thread_logs
echo -e "Testing tiles with pins only on top and right sides";
run-task basic_tests/tile_organization/top_right_custom_pins --debug --show_thread_logs
echo -e "Testing tiles with pins only on bottom and right sides";
run-task basic_tests/tile_organization/bottom_right_custom_pins --debug --show_thread_logs
echo -e "Testing tiles with I/O in center grid";
run-task basic_tests/tile_organization/tileable_io --debug --show_thread_logs

echo -e "Testing global port definition from tiles";
run-task basic_tests/global_tile_ports/global_tile_clock --debug --show_thread_logs
run-task basic_tests/global_tile_ports/global_tile_reset --debug --show_thread_logs
run-task basic_tests/global_tile_ports/global_tile_4clock --debug --show_thread_logs

echo -e "Testing yosys flow using custom ys script for running quicklogic device";
run-task quicklogic_tests/flow_test --debug --show_thread_logs

# Repgression test to test multi-user enviroment
cp -r */*/basic_test/full_testbench/configuration_chain /tmp/
cd /tmp/ && run-task configuration_chain