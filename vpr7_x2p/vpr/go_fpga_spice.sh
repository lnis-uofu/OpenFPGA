#! /bin/csh -f
# Example of how to run vpr

# Set variables
# For FPGA-Verilog ONLY 
set spice_output_dirname = sram_fpga_hetero
set spice_output_dirpath = $PWD
# VPR critical inputs
#set arch_xml_file = ARCH/k6_N10_MD_tsmc40nm_chain_TT.xml 
#set arch_xml_file = ARCH/ed_dev.xml 
set arch_xml_file = ARCH/k6_N10_SC_tsmc40nm_chain_TT.xml 
#set arch_xml_file = ARCH/k6_N10_SC_tsmc40nm_chain_TT_yosys.xml 
#set arch_xml_file = ARCH/k6_N10_sram_chain_SC_gf130_2x2.xml
set blif_file = Circuits/s298_prevpr.blif 
set act_file = Circuits/s298_prevpr.act 
#set blif_file = Circuits/simple_gates_prevpr.blif 
#set act_file = Circuits/simple_gates_prevpr.act 
set vpr_route_chan_width = 100

# Step A: Make sure a clean start 
# Recompile if needed
#make clean
#make -j32
# Remove previous designs
rm -rf $spice_output_dirpath/$spice_output_dirname

# Run VPR  
#valgrind 
./vpr $arch_xml_file $blif_file --full_stats --nodisp --activity_file $act_file --fpga_spice --fpga_spice_dir $spice_output_dirpath/$spice_output_dirname --fpga_x2p_rename_illegal_port --fpga_spice_print_top_testbench --fpga_spice_print_lut_testbench --fpga_spice_print_hardlogic_testbench --fpga_spice_print_io_testbench --fpga_spice_print_pb_mux_testbench --fpga_spice_print_cb_mux_testbench --fpga_spice_print_sb_mux_testbench --fpga_spice_print_cb_testbench --fpga_spice_print_sb_testbench --fpga_spice_print_grid_testbench



