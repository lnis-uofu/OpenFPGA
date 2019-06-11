#!/bin/bash
# Regression test version 1.0

# Set variables
benchmark="test_modes"
include_netlists="_include_netlists.v"
compiled_file="compiled_$benchmark"
tb_formal_postfix="_top_formal_verification_random_tb"
verilog_output_dirname="${benchmark}_Verilog"
log_file="${benchmark}_sim.log"

cd fpga_flow/scripts

perl rewrite_path_in_file.pl -i ../arch/template/k6_N10_sram_chain_HC_template.xml
perl rewrite_path_in_file.pl -i ../../vpr7_x2p/vpr/regression_verilog.sh
perl rewrite_path_in_file.pl -i ../../vpr7_x2p/vpr/VerilogNetlists/ff.v

cd -

# Move to vpr folder
cd vpr7_x2p/vpr

# Remove former log file
rm -f $log_file
rm -f $compiled_file

# Start the script -> run the fpga generation -> run the simulation -> check the log file
source regression_verilog.sh
iverilog -o $compiled_file $verilog_output_dirname/SRC/$benchmark$include_netlists -s $benchmark$tb_formal_postfix
vvp $compiled_file -j 16 >> $log_file

result=`grep "Succeed" $log_file`
if ["$result" = ""]; then
  result=`grep "Failed" $log_file`
  if ["$result" = ""]; then
    echo "Unexpected error, Verification didn't run"
    cd -
    exit 1
  else
    echo "Verification failed"
    cd -
    exit 2
  fi
else
  echo "Verification succeed"
  cd -
fi
