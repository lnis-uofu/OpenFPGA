#!/bin/sh
# Example of how to run vpr

# Pack, place, and route a heterogeneous FPGA
# Packing uses the AAPack algorithm
./vpr picorv/arch.xml picorv/picorv.blif --full_stats --nodisp --activity_file picorv/picorv.act --route_chan_width 30 --fpga_spice --fpga_spice_rename_illegal_port --fpga_spice_dir ./spice_test_picorv --fpga_spice_print_top_testbench --fpga_spice_print_grid_testbench --fpga_spice_print_cb_testbench --fpga_spice_print_sb_testbench --fpga_spice_print_lut_testbench --fpga_spice_print_hardlogic_testbench --fpga_spice_print_pb_mux_testbench --fpga_spice_print_cb_mux_testbench --fpga_spice_print_sb_mux_testbench --fpga_verilog --fpga_verilog_dir ./verilog_test_picorv --fpga_verilog_print_top_testbench



