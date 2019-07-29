#! /bin/bash
# Exit if error occurs
set -e
# Make sure a clear start
default_task='lattice_benchmark'
pwd_path="$PWD"
task_name=${1:-$default_task} # run task defined in argument else run default task
config_file="$PWD/configs/${task_name}.conf"
bench_txt="$PWD/benchmarks/List/${task_name}.txt"
rpt_file="$PWD/csv_rpts/fpga_spice/${task_name}.csv"
task_file="$PWD/vpr_fpga_spice_task_lists/${task_name}"

verilog_path="${PWD}/regression_${task_name}"

config_file_final=$(echo ${config_file/.conf/_final.conf})

# List of argument passed to FPGA flow
vpr_config_flags=(
    '-N 10'
    '-K 6'
    '-ace_d 0.5'
    '-multi_thread 1'
    '-vpr_fpga_x2p_rename_illegal_port'
    '-vpr_fpga_verilog'
    '-vpr_fpga_bitstream_generator'
    '-vpr_fpga_verilog_print_autocheck_top_testbench'
    '-vpr_fpga_verilog_include_timing'
    '-vpr_fpga_verilog_include_signal_init'
    '-vpr_fpga_verilog_formal_verification_top_netlist'
    '-fix_route_chan_width'
    '-vpr_fpga_verilog_include_icarus_simulator'
    '-power'
)
# vpr_config_flags+=("$@") # Append provided arguments

#=============== Argument Sanity Check =====================
#Check if script running in correct (OpenFPGA/fpga_flow) folder
if [[ $pwd_path != *"OpenFPGA/fpga_flow"* ]]; then
    echo "Error : Execute script from OpenFPGA/fpga_flow project folder"
    exitflag=1
fi

#Check if fconfig and benchmark_list file exists
for filepath in $config_file $bench_txt; do
    if [ ! -f $filepath ]; then
        echo "$filepath File not found!"
        exitflag=1
    fi
done
if [ -n "$exitflag" ]; then
    echo "Terminating script . . . . . . "
    exit 1
fi
#=======================================================
#======== Replace variables in config file =============

#Extract OpenFPGA Project Path and Escape
OPENFPGAPATHKEYWORD=$(echo "$(echo $pwd_path | sed 's/.OpenFPGA.*$//')/OpenFPGA" | sed 's/\//\\\//g')

# Create final config file with replaced keywords replaced variables
sed 's/OPENFPGAPATHKEYWORD/'"${OPENFPGAPATHKEYWORD}"'/g' $config_file >$config_file_final

#==================Clean result, change directory and execute ===============
cd ${pwd_path}/scripts

# perl fpga_flow.pl -conf ${config_file_final} -benchmark ${bench_txt} -rpt ${rpt_file} -vpr_fpga_verilog_dir $verilog_path $(echo "${vpr_config_flags[@]}")

perl fpga_flow.pl -conf ${config_file_final} -benchmark ${bench_txt} -rpt ${rpt_file} -N 10 -K 6 -ace_d 0.5 -multi_thread 1 -vpr_fpga_x2p_rename_illegal_port -vpr_fpga_verilog -vpr_fpga_verilog_dir $verilog_path -vpr_fpga_bitstream_generator -vpr_fpga_verilog_print_autocheck_top_testbench -vpr_fpga_verilog_include_timing -vpr_fpga_verilog_include_signal_init -vpr_fpga_verilog_formal_verification_top_netlist -fix_route_chan_width -vpr_fpga_verilog_include_icarus_simulator -power

echo "Netlists successfully generated and simulated"
exit 0
