#! /bin/bash
# Exit if error occurs
set -e
# Make sure a clear start
pwd_path="$PWD"
task_name="k6_N10_regression_TT"
config_file="$PWD/configs/regression/${task_name}.conf" 
bench_txt="$PWD/benchmarks/List/mcnc_benchmark.txt"
rpt_file="$PWD/csv_rpts/fpga_spice/${task_name}.csv"
task_file="$PWD/vpr_fpga_spice_task_lists/${task_name}"

verilog_path="${PWD}/regression_MCNC"
modelsim_ini_path="/uusoc/facility/cad_tools/Mentor/modelsim10.7b/modeltech/modelsim.ini"

# FPGA-SPICE
rm -rf ${pwd_path}/results
cd ${pwd_path}/scripts

# SRAM FPGA
# TT case 
perl fpga_flow.pl -conf ${config_file} -benchmark ${bench_txt} -rpt ${rpt_file} -N 10 -K 6 -ace_d 0.5 -power -remove_designs -multi_thread 1 -vpr_fpga_x2p_rename_illegal_port -vpr_fpga_verilog -vpr_fpga_verilog_dir $verilog_path -vpr_fpga_bitstream_generator -vpr_fpga_verilog_print_autocheck_top_testbench -vpr_fpga_verilog_print_modelsim_autodeck $modelsim_ini_path -vpr_fpga_verilog_include_timing -vpr_fpga_verilog_include_signal_init -vpr_fpga_verilog_formal_verification_top_netlist -fix_route_chan_width

rm -rf ${pwd_path}/results
rm -rf $verilog_path
cd ${pwd_path}

exit 0
