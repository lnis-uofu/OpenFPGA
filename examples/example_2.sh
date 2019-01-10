#!/bin/sh
# Example of how to run vpr


# The paths need to be absolute hence we modify a keyword with PWD

sed "s:OPENFPGAPATH:${PWD}/..:g" example_2_template.xml > example_2.xml

# Pack, place, and route a heterogeneous FPGA
# Packing uses the AAPack algorithm
../vpr7_x2p/vpr/vpr ./example_2.xml ./example_2.blif --full_stats --nodisp --route_chan_width 100 --fpga_spice --fpga_spice_rename_illegal_port --fpga_spice_dir ./spice_test_example_2 --fpga_spice_print_top_testbench --fpga_spice_print_grid_testbench --fpga_spice_print_cb_testbench --fpga_spice_print_sb_testbench --fpga_spice_print_lut_testbench --fpga_spice_print_hardlogic_testbench --fpga_spice_print_pb_mux_testbench --fpga_spice_print_cb_mux_testbench --fpga_spice_print_sb_mux_testbench --fpga_verilog --fpga_verilog_dir ./verilog_test_example_2


