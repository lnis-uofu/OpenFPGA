#!/bin/bash
# Example of how to run vpr

# Set variables
# For FPGA-Verilog ONLY 
benchmark="test_modes"
verilog_output_dirname="${benchmark}_Verilog"
verilog_output_dirpath="$PWD"
modelsim_ini_file="/uusoc/facility/cad_tools/Mentor/modelsim10.7b/modeltech/modelsim.ini"
# VPR critical inputs
arch_xml_file="OPENFPGAPATHKEYWORD/fpga_flow/arch/template/k6_N10_sram_chain_HC_template.xml"
blif_file="OPENFPGAPATHKEYWORD/fpga_flow/benchmarks/Blif/Test_Modes/$benchmark.blif" 
act_file="OPENFPGAPATHKEYWORD/fpga_flow/benchmarks/Blif/Test_Modes/$benchmark.act "
verilog_reference="OPENFPGAPATHKEYWORD/fpga_flow/benchmarks/Verilog/Test_Modes/$benchmark.v"
vpr_route_chan_width="200"

# Step A: Make sure a clean start 
# Recompile if needed
#make clean
#make -j32
# Remove previous designs
rm -rf $verilog_output_dirpath/$verilog_output_dirname

# Run VPR  
#valgrind 
./vpr $arch_xml_file $blif_file --full_stats --nodisp --activity_file $act_file --fpga_verilog --fpga_verilog_dir $verilog_output_dirpath/$verilog_output_dirname --fpga_x2p_rename_illegal_port --fpga_bitstream_generator --fpga_verilog_print_top_testbench --fpga_verilog_print_input_blif_testbench --fpga_verilog_include_timing --fpga_verilog_include_signal_init --fpga_verilog_print_modelsim_autodeck $modelsim_ini_file --fpga_verilog_print_formal_verification_top_netlist --fpga_verilog_print_autocheck_top_testbench $verilog_reference --fpga_verilog_print_user_defined_template --route_chan_width $vpr_route_chan_width --fpga_verilog_include_icarus_simulator --fpga_verilog_print_report_timing_tcl --power --tech_properties OPENFPGAPATHKEYWORD/fpga_flow/tech/PTM_45nm/45nm.xml --fpga_verilog_print_sdc_pnr #--fpga_verilog_print_sdc_analysis



