#!/bin/bash

set -e
source openfpga.sh
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "Basic regression tests";

echo -e "Test multiple runs of vpr"
run-task basic_tests/vpr_standalone $@

echo -e "Test source commands in openfpga shell"
run-task basic_tests/source_command/source_string $@
run-task basic_tests/source_command/source_file $@

echo -e "Testing preloading rr_graph"
run-task basic_tests/preload_rr_graph/preload_rr_graph_xml $@
run-task basic_tests/preload_rr_graph/preload_rr_graph_bin $@

echo -e "Testing testbenches using fpga core wrapper"
run-task basic_tests/full_testbench/fpga_core_wrapper $@
run-task basic_tests/full_testbench/fpga_core_wrapper_naming_rules $@
run-task basic_tests/full_testbench/fpga_core_wrapper_naming_rules_use_core_tb $@
run-task basic_tests/preconfig_testbench/fpga_core_wrapper $@
run-task basic_tests/preconfig_testbench/fpga_core_wrapper_naming_rules $@
run-task basic_tests/preconfig_testbench/fpga_core_wrapper_naming_rules_use_core_tb $@

echo -e "Testing configuration chain of a K4N4 FPGA";
run-task basic_tests/full_testbench/configuration_chain $@
run-task basic_tests/full_testbench/configuration_chain_no_time_stamp $@
run-task basic_tests/full_testbench/configuration_chain_use_reset $@
run-task basic_tests/full_testbench/configuration_chain_use_resetb $@
run-task basic_tests/full_testbench/configuration_chain_use_set $@
run-task basic_tests/full_testbench/configuration_chain_use_setb $@
run-task basic_tests/full_testbench/configuration_chain_use_set_reset $@
run-task basic_tests/full_testbench/configuration_chain_config_enable_scff $@
run-task basic_tests/full_testbench/multi_region_configuration_chain $@
run-task basic_tests/full_testbench/multi_region_configuration_chain_2clk $@
run-task basic_tests/full_testbench/multi_region_configuration_chain_3clk $@
run-task basic_tests/full_testbench/fast_configuration_chain $@
run-task basic_tests/full_testbench/fast_configuration_chain_use_set $@
run-task basic_tests/full_testbench/smart_fast_configuration_chain $@
run-task basic_tests/full_testbench/smart_fast_multi_region_configuration_chain $@
run-task basic_tests/preconfig_testbench/configuration_chain $@
run-task basic_tests/preconfig_testbench/configuration_chain_config_done_io $@
run-task basic_tests/preconfig_testbench/configuration_chain_no_time_stamp $@

echo -e "Testing fram-based configuration protocol of a K4N4 FPGA";
run-task basic_tests/full_testbench/configuration_frame $@
run-task basic_tests/full_testbench/smart_fast_configuration_frame $@
run-task basic_tests/full_testbench/fast_configuration_frame $@
run-task basic_tests/full_testbench/fast_configuration_frame_use_set $@
run-task basic_tests/full_testbench/configuration_frame_ccff $@
run-task basic_tests/full_testbench/configuration_frame_scff $@
run-task basic_tests/full_testbench/configuration_frame_use_reset $@
run-task basic_tests/full_testbench/configuration_frame_use_resetb $@
run-task basic_tests/full_testbench/configuration_frame_use_set $@
run-task basic_tests/full_testbench/configuration_frame_use_setb $@
run-task basic_tests/full_testbench/configuration_frame_use_set_reset $@
run-task basic_tests/full_testbench/multi_region_configuration_frame $@
run-task basic_tests/full_testbench/smart_fast_multi_region_configuration_frame $@
run-task basic_tests/preconfig_testbench/configuration_frame $@

echo -e "Testing memory bank configuration protocol of a K4N4 FPGA";
run-task basic_tests/full_testbench/memory_bank $@
run-task basic_tests/full_testbench/memory_bank_use_reset $@
run-task basic_tests/full_testbench/memory_bank_use_resetb $@
run-task basic_tests/full_testbench/memory_bank_use_set $@
run-task basic_tests/full_testbench/memory_bank_use_setb $@
run-task basic_tests/full_testbench/memory_bank_use_set_reset $@
run-task basic_tests/full_testbench/multi_region_memory_bank $@
run-task basic_tests/full_testbench/fast_memory_bank $@
run-task basic_tests/full_testbench/fast_memory_bank_use_set $@
run-task basic_tests/full_testbench/smart_fast_memory_bank $@
run-task basic_tests/full_testbench/smart_fast_multi_region_memory_bank $@
run-task basic_tests/preconfig_testbench/memory_bank $@

echo -e "Testing physical design friendly memory bank configuration protocol of a K4N4 FPGA";
run-task basic_tests/full_testbench/ql_memory_bank $@
run-task basic_tests/full_testbench/ql_memory_bank_use_wlr $@
run-task basic_tests/full_testbench/multi_region_ql_memory_bank $@
run-task basic_tests/full_testbench/ql_memory_bank_flatten $@
run-task basic_tests/full_testbench/ql_memory_bank_flatten_defined_wl $@
run-task basic_tests/full_testbench/ql_memory_bank_flatten_use_wlr $@
run-task basic_tests/full_testbench/ql_memory_bank_shift_register $@
run-task basic_tests/full_testbench/ql_memory_bank_shift_register_use_wlr $@
run-task basic_tests/full_testbench/ql_memory_bank_shift_register_multi_chain $@

echo -e "Testing simulator support";
run-task basic_tests/full_testbench/ql_memory_bank_shift_register_vcs $@

echo -e "Testing testbenches without self checking features";
run-task basic_tests/full_testbench/full_testbench_without_self_checking $@
run-task basic_tests/preconfig_testbench/preconfigured_testbench_without_self_checking $@

echo -e "Testing standalone (flatten memory) configuration protocol of a K4N4 FPGA";
run-task basic_tests/full_testbench/flatten_memory $@
run-task basic_tests/preconfig_testbench/flatten_memory $@

echo -e "Testing fixed device layout and routing channel width";
run-task basic_tests/fixed_device_support $@

echo -e "Testing fabric Verilog generation only";
run-task basic_tests/generate_fabric $@

echo -e "Testing Verilog testbench generation only";
run-task basic_tests/generate_testbench $@
run-task basic_tests/generate_template_testbench $@

echo -e "Testing separated Verilog fabric netlists and testbench locations";
run-task basic_tests/custom_fabric_netlist_location $@

echo -e "Testing user-defined simulation settings: clock frequency and number of cycles";
run-task basic_tests/fixed_simulation_settings/fixed_operating_clock_freq $@
run-task basic_tests/fixed_simulation_settings/fixed_operating_clock_freq_no_ace $@
# TODO: This feature is temporarily out of test due to the emergency in delivering netlists for multi-chain shift-register memory bank
#run-task basic_tests/fixed_simulation_settings/fixed_shift_register_clock_freq $@

echo -e "Testing Secured FPGA fabrics";
run-task basic_tests/fabric_key/generate_vanilla_key $@
run-task basic_tests/fabric_key/generate_multi_region_vanilla_key $@
run-task basic_tests/fabric_key/generate_random_key $@
run-task basic_tests/fabric_key/generate_random_key_ql_memory_bank $@
run-task basic_tests/fabric_key/load_external_key $@
run-task basic_tests/fabric_key/load_external_key_cc_fpga $@
run-task basic_tests/fabric_key/load_external_subkey_cc_fpga $@
run-task basic_tests/fabric_key/load_external_key_multi_region_cc_fpga $@
run-task basic_tests/fabric_key/load_external_key_qlbank_fpga $@
run-task basic_tests/fabric_key/load_external_key_multi_region_qlbank_fpga $@
run-task basic_tests/fabric_key/load_external_key_qlbanksr_multi_chain_fpga $@
# TODO: This feature is temporarily out of test due to the emergency in delivering netlists for multi-chain shift-register memory bank
#run-task basic_tests/fabric_key/load_external_key_multi_region_qlbanksr_fpga $@

echo -e "Testing mock wrapper"
run-task basic_tests/mock_wrapper/mock_wrapper_explicit_port_mapping $@
run-task basic_tests/mock_wrapper/mock_wrapper_implicit_port_mapping $@
run-task basic_tests/mock_wrapper/mock_wrapper_pcf $@
run-task basic_tests/mock_wrapper/mock_wrapper_bgf $@

echo -e "Testing K4 series FPGA";
echo -e "Testing K4N4 with facturable LUTs";
run-task basic_tests/k4_series/k4n4_frac_lut $@
echo -e "Testing K4N4 with asynchronous reset";
run-task basic_tests/k4_series/k4n4_fracff $@
echo -e "Testing K4N4 with negative edge clocks";
run-task basic_tests/k4_series/k4n4_fracff2edge $@
echo -e "Testing K4N4 with hard adders";
run-task basic_tests/k4_series/k4n4_adder $@
echo -e "Testing K4N4 without local routing architecture";
run-task basic_tests/k4_series/k4n4_no_local_routing $@
echo -e "Testing K4N4 with block RAM";
run-task basic_tests/k4_series/k4n4_bram $@
echo -e "Testing K4N4 with LUTRAM";
run-task basic_tests/k4_series/k4n4_lutram $@
echo -e "Testing K4N4 with multiple lengths of routing segments";
run-task basic_tests/k4_series/k4n4_L124 $@
echo -e "Testing K4N4 with routing channel width distribution: x = 0.8, y = 1.0";
run-task basic_tests/k4_series/k4n4_chandistr $@
echo -e "Testing K4N4 with routing channel width distribution: x = 0.8, y = 1.0 and wire segment distribution: x=L124, Y=L12";
run-task basic_tests/k4_series/k4n4_chandistr_segdist $@
echo -e "Testing K4N4 with 32-bit fracturable multiplier";
run-task basic_tests/k4_series/k4n4_frac_mult $@
echo -e "Testing K4N5 with pattern based local routing";
run-task basic_tests/k4_series/k4n5_pattern_local_routing $@
echo -e "Testing K4N4 with custom I/O location syntax";
run-task basic_tests/k4_series/k4n4_custom_io_loc $@
run-task basic_tests/k4_series/k4n4_custom_io_loc_center $@
run-task basic_tests/k4_series/k4n4_custom_io_loc_center_height_odd $@
run-task basic_tests/k4_series/k4n4_custom_io_loc_center_width_odd $@
echo -e "Testing K4N4 with a local routing where reset can driven LUT inputs";
run-task basic_tests/k4_series/k4n4_rstOnLut $@
run-task basic_tests/k4_series/k4n4_rstOnLut_strong $@
echo -e "Testing K4N4 support clock generation by internal resources";
run-task basic_tests/k4_series/k4n4_clk_gen $@
echo -e "Testing K4N4 support reset generation by internal resources";
run-task basic_tests/k4_series/k4n4_rst_gen $@
echo -e "Testing enhanced connection blocks"
run-task basic_tests/k4_series/k4n4_ecb $@

echo -e "Testing different tile organizations";
echo -e "Testing tiles with pins only on top and left sides";
run-task basic_tests/tile_organization/top_left_custom_pins $@
echo -e "Testing tiles with pins only on top and right sides";
run-task basic_tests/tile_organization/top_right_custom_pins $@
echo -e "Testing tiles with pins only on bottom and right sides";
run-task basic_tests/tile_organization/bottom_right_custom_pins $@
echo -e "Testing tiles with I/O in center grid";
run-task basic_tests/tile_organization/tileable_io $@
echo -e "Testing tiles with I/O consisting of subtiles";
run-task basic_tests/tile_organization/io_subtile $@
run-task basic_tests/tile_organization/io_subtile_strong $@
echo -e "Testing tile grouping on a homogeneous FPGA fabric (Full testbench)";
run-task basic_tests/tile_organization/homo_fabric_tile $@
echo -e "Testing tile grouping on a homogeneous FPGA fabric (Preconfigured testbench)";
run-task basic_tests/tile_organization/fabric_tile_global_tile_clock_io_subtile $@
run-task basic_tests/tile_organization/homo_fabric_tile_preconfig $@
run-task basic_tests/tile_organization/homo_fabric_tile_2x2_preconfig $@
run-task basic_tests/tile_organization/homo_fabric_tile_4x4_preconfig $@
run-task basic_tests/tile_organization/homo_fabric_tile_global_tile_clock $@
run-task basic_tests/tile_organization/homo_fabric_tile_adder_chain $@
run-task basic_tests/tile_organization/homo_fabric_tile_clkntwk $@
run-task basic_tests/tile_organization/hetero_fabric_tile $@
run-task basic_tests/tile_organization/homo_fabric_tile_ecb_2x2_preconfig $@

echo -e "Testing group config block";
run-task basic_tests/group_config_block/group_config_block_homo_full_testbench $@
run-task basic_tests/group_config_block/group_config_block_homo_Lshape_full_testbench $@
run-task basic_tests/group_config_block/group_config_block_homo_fabric_tile $@
run-task basic_tests/group_config_block/group_config_block_homo_fabric_tile_Lshape $@
run-task basic_tests/group_config_block/group_config_block_homo_fabric_tile_core_wrapper $@
run-task basic_tests/group_config_block/group_config_block_hetero_fabric_tile $@
run-task basic_tests/group_config_block/group_config_block_hetero_fabric_tile_Lshape $@
run-task basic_tests/group_config_block/group_config_block_homo_fabric_tile_global_tile_clock_io_subtile $@

echo -e "Module naming";
run-task basic_tests/module_naming/using_index $@
run-task basic_tests/module_naming/renaming_rules $@
run-task basic_tests/module_naming/renaming_rules_strong $@
run-task basic_tests/module_naming/renaming_rules_on_indexed_names $@

echo -e "Testing global port definition from tiles";
run-task basic_tests/global_tile_ports/global_tile_clock $@
run-task basic_tests/global_tile_ports/global_tile_clock_subtile $@
run-task basic_tests/global_tile_ports/global_tile_clock_subtile_port_merge $@
run-task basic_tests/global_tile_ports/global_tile_clock_subtile_port_merge_fabric_tile_group_config $@
run-task basic_tests/global_tile_ports/global_tile_reset $@
run-task basic_tests/global_tile_ports/global_tile_4clock --default_tool_path ${OPENFPGA_PATH}/openfpga_flow/misc/fpgaflow_default_tool_path_timing.conf $@
run-task basic_tests/global_tile_ports/global_tile_4clock_pin $@

echo -e "Testing programmable clock architecture";
run-task basic_tests/clock_network/homo_1clock_2layer $@
run-task basic_tests/clock_network/homo_1clock_2layer_full_tb $@
run-task basic_tests/clock_network/homo_2clock_2layer $@

echo -e "Testing configuration chain of a K4N4 FPGA using .blif generated by yosys+verific";
run-task basic_tests/verific_test $@

echo -e "Testing explicit multi verilog files";
run-task basic_tests/explicit_multi_verilog_files $@

echo -e "Test the remove of runtime directories"
clear-task-run basic_tests/explicit_multi_verilog_files $@

echo -e "Testing write GSB to files";
run-task basic_tests/write_gsb/write_gsb_to_xml $@
run-task basic_tests/write_gsb/write_gsb_to_xml_exclude_rr_info $@
run-task basic_tests/write_gsb/write_gsb_to_xml_exclude_cbx $@
run-task basic_tests/write_gsb/write_gsb_to_xml_exclude_cby $@
run-task basic_tests/write_gsb/write_gsb_to_xml_exclude_cbx_cby $@
run-task basic_tests/write_gsb/write_gsb_to_xml_exclude_sb $@
run-task basic_tests/write_gsb/write_gsb_to_xml_exclude_sb_cbx $@
run-task basic_tests/write_gsb/write_gsb_to_xml_exclude_sb_cby $@
run-task basic_tests/write_gsb/write_gsb_to_xml_include_sb_cbx_cby $@
run-task basic_tests/write_gsb/write_gsb_to_xml_include_single_cbx $@
run-task basic_tests/write_gsb/write_gsb_to_xml_include_single_cby $@
run-task basic_tests/write_gsb/write_gsb_to_xml_include_single_sb $@
run-task basic_tests/write_gsb/write_gsb_to_xml_compress_routing $@
run-task basic_tests/write_gsb/write_unique_gsb_to_xml $@
run-task basic_tests/write_gsb/write_unique_gsb_to_xml_compress_routing $@

echo -e "Testing fabric pin physical location file"
run-task basic_tests/write_fabric_pin_phy_loc/write_fabric_pin_phy_loc_default $@
run-task basic_tests/write_fabric_pin_phy_loc/write_fabric_pin_phy_loc_for_tiles $@
run-task basic_tests/write_fabric_pin_phy_loc/write_fabric_pin_phy_loc_show_invalid_sides $@
run-task basic_tests/write_fabric_pin_phy_loc/write_fabric_pin_phy_loc_wildcards $@

echo -e "Testing bus group features";
run-task basic_tests/bus_group/preconfig_testbench_explicit_mapping $@
run-task basic_tests/bus_group/preconfig_testbench_implicit_mapping $@
run-task basic_tests/bus_group/full_testbench_explicit_mapping $@
run-task basic_tests/bus_group/full_testbench_implicit_mapping $@
run-task basic_tests/bus_group/auto_gen_bus_group $@

echo -e "Testing fix pins features";
run-task basic_tests/io_constraints/fix_pins $@
run-task basic_tests/io_constraints/example_pcf $@
run-task basic_tests/io_constraints/empty_pcf $@
run-task basic_tests/io_constraints/pcf_ql_style $@

echo -e "Testing project templates";
run-task template_tasks/fabric_netlist_gen_template $@
run-task template_tasks/fabric_verification_template $@
run-task template_tasks/frac-lut-arch-explore_template $@
run-task template_tasks/vtr_benchmarks_template $@

echo -e "Testing create tsk from template and run task"
create-task _task_copy basic_tests/generate_fabric
run-task _task_copy $@

echo -e "Testing output files without time stamp";
run-task basic_tests/no_time_stamp/device_1x1 $@
run-task basic_tests/no_time_stamp/device_4x4 $@
run-task basic_tests/no_time_stamp/no_cout_in_gsb $@
run-task basic_tests/no_time_stamp/dump_waveform $@
# Run git-diff to ensure no changes on the golden netlists
# Switch to root path in case users are running the tests in another location
cd ${OPENFPGA_PATH}
pwd
git config --global --add safe.directory ${OPENFPGA_PATH}
git log
if git diff --name-status --exit-code -- ':openfpga_flow/tasks/basic_tests/no_time_stamp/*/golden_outputs_no_time_stamp/**'; then
  echo -e "Golden netlist remain unchanged"
else
  echo -e "Detect changes in golden netlists";
  git diff -- ':openfpga_flow/tasks/basic_tests/no_time_stamp/*/golden_outputs_no_time_stamp/**';
  exit 1;
fi
cd -

# Repgression test to test multi-user enviroment
# Note: Keep this task as the last one!!!
cp -r */*/basic_tests/full_testbench/configuration_chain /tmp/
cd /tmp/ && run-task configuration_chain $@
cd -
