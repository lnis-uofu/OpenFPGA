#!/bin/bash
echo "#################################################"
echo "The current shell environment is the following:"
echo $0
echo "#################################################"

# Example of how to run vprset circuit_name = pip_add
#set circuit_name = pip_add
circuit_name=sync_4bits_add
circuit_blif=${PWD}/Circuits/${circuit_name}.blif
arch_file=${PWD}/ARCH/k6_N10_scan_chain_ptm45nm_TT.xml
arch_file_template=${PWD}/ARCH/k6_N10_sram_chain_HC_template.xml
circuit_act=${PWD}/Circuits/${circuit_name}.act
circuit_verilog=${PWD}/Circuits/${circuit_name}.v
spice_output=${PWD}/spice_demo 
verilog_output=${PWD}/verilog_demo 
modelsim_ini=/uusoc/facility/cad_tools/Mentor/modelsim10.7b/modeltech/modelsim.ini
openfpga_path=${PWD}/../..

# Make sure a clean start
rm -rf ${spice_output}
rm -rf ${verilog_output}

echo "*******************************"
echo "THIS SCRIPT NEEDS TO BE SOURCED"
echo "source ./go.sh"
echo "*******************************"

sed "s:OPENFPGAPATH:${openfpga_path}:g" ${arch_file_template} > ${arch_file} 

# Pack, place, and route a heterogeneous FPGA
# Packing uses the AAPack algorithm
./vpr ${arch_file} ${circuit_blif} --full_stats --nodisp --activity_file ${circuit_act} --route_chan_width 30 --fpga_spice --fpga_spice_rename_illegal_port --fpga_spice_dir ${spice_output} --fpga_spice_print_top_testbench
