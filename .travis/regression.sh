#!/bin/bash
# Regression test version 1.0

# Set variables
my_pwd=$PWD
fpga_flow_scripts=${my_pwd}/fpga_flow/scripts
vpr_path=${my_pwd}/vpr7_x2p/vpr
benchmark="test_modes"
include_netlists="_include_netlists.v"
compiled_file="compiled_$benchmark"
tb_formal_postfix="_top_formal_verification_random_tb"
verilog_output_dirname="${benchmark}_Verilog"
log_file="${benchmark}_sim.log"
new_reg_sh="my_regression.sh"


cd $fpga_flow_scripts

perl rewrite_path_in_file.pl -i $vpr_path/regression_verilog.sh -o $vpr_path/$new_reg_sh

cd $my_pwd

# Move to vpr folder
cd $vpr_path

# Remove former log file
rm -f $log_file
rm -f $compiled_file

# Start the script -> run the fpga generation -> run the simulation -> check the log file
source $new_reg_sh
iverilog -o $compiled_file $verilog_output_dirname/SRC/$benchmark$include_netlists -s $benchmark$tb_formal_postfix
vvp $compiled_file -j 16 >> $log_file

result=`grep "Succeed" $log_file`
if ["$result" = ""]; then
  result=`grep "Failed" $log_file`
  if ["$result" = ""]; then
    echo "Unexpected error, Verification didn't run"
    cd $my_pwd
    exit 1
  else
    echo "Verification failed"
    cd $my_pwd
    exit 2
  fi
else
  echo "Verification succeed"
  cd $my_pwd
fi
