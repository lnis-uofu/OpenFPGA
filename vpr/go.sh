#!/bin/sh
# Example of how to run fpga_spice
rm -rf spice_test
#valgrind 
./vpr ARCH/sample_arch.xml Circuits/s298_K6_N10_ace.blif --activity_file Circuits/s298_K6_N10_ace.act --nodisp --fpga_spice --spice_dir /home/xitang/backup/vpr7_fpga_spice/vpr/spice_test  --print_spice_grid_testbench --print_spice_lut_testbench --print_spice_dff_testbench --print_spice_pb_mux_testbench --print_spice_cb_mux_testbench --print_spice_sb_mux_testbench --print_spice_top_testbench #--fpga_spice_parasitic_net_estimation_off #--print_spice_grid_testbench  #--fpga_spice_leakage_only
