#!/bin/sh
# Example of how to run vprset circuit_name = pip_add
#set circuit_name = pip_add
set circuit_name = sync_4bits_add
set arch_file = ${PWD}/ARCH/k6_N10_scan_chain_tsmc40nm_TT.xml
set circuit_blif = ${PWD}/Circuits/${circuit_name}.blif
set circuit_act = ${PWD}/Circuits/${circuit_name}.act
set circuit_verilog = ${PWD}/Circuits/${circuit_name}.v
set spice_output = ${PWD}/spice_demo 
set verilog_output = ${PWD}/verilog_demo 
set modelsim_ini = /uusoc/facility/cad_tools/Mentor/modelsim10.7b/modeltech/modelsim.ini

# Make sure a clean start
rm -rf ${spice_output}
rm -rf ${verilog_output}

# Pack, place, and route a heterogeneous FPGA
# Packing uses the AAPack algorithm
./vpr ${arch_file} ${circuit_blif} --full_stats --nodisp --activity_file ${circuit_act} --route_chan_width 30 --fpga_spice --fpga_spice_rename_illegal_port --fpga_spice_dir ${spice_output} --fpga_spice_print_top_testbench --fpga_spice_print_grid_testbench --fpga_spice_print_cb_testbench --fpga_spice_print_sb_testbench --fpga_spice_print_lut_testbench --fpga_spice_print_hardlogic_testbench --fpga_spice_print_pb_mux_testbench --fpga_spice_print_cb_mux_testbench --fpga_spice_print_sb_mux_testbench --fpga_verilog --fpga_verilog_dir ${verilog_output} --fpga_verilog_print_top_testbench --fpga_verilog_print_top_auto_testbench ${circuit_verilog} --fpga_verilog_print_modelsim_autodeck --fpga_verilog_modelsim_ini_path ${modelsim_ini} --fpga_verilog_include_timing --fpga_verilog_init_sim

