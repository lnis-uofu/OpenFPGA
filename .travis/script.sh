#!/bin/bash

source .travis/common.sh
set -e

start_section "OpenFPGA.build" "${GREEN}Building..${NC}"
mkdir build
cd build

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  cmake .. -DCMAKE_BUILD_TYPE=debug -DENABLE_VPR_GRAPHICS=off
else
  cmake .. -DCMAKE_BUILD_TYPE=debug
fi
 make -j16
end_section "OpenFPGA.build"


start_section "OpenFPGA.TaskTun" "${GREEN}..Running_Regression..${NC}"
cd -

###############################################
# OpenFPGA with VPR7
# TO BE DEPRECATED
##############################################
echo -e "Testing single-mode architectures";
python3 openfpga_flow/scripts/run_fpga_task.py single_mode --debug --show_thread_logs
#python3 openfpga_flow/scripts/run_fpga_task.py s298 

echo -e "Testing multi-mode architectures";
python3 openfpga_flow/scripts/run_fpga_task.py multi_mode --maxthreads 4 --debug --show_thread_logs

echo -e "Testing compact routing techniques";
python3 openfpga_flow/scripts/run_fpga_task.py compact_routing --debug --show_thread_logs

echo -e "Testing tileable architectures";
python3 openfpga_flow/scripts/run_fpga_task.py tileable_routing --debug --show_thread_logs

echo -e "Testing Verilog generation with explicit port mapping ";
python3 openfpga_flow/scripts/run_fpga_task.py explicit_verilog --debug --show_thread_logs

echo -e "Testing Verilog generation with grid pin duplication ";
python3 openfpga_flow/scripts/run_fpga_task.py duplicate_grid_pin --debug --show_thread_logs

###############################################
# OpenFPGA Shell with VPR8
# (Will replace all the old tests)
##############################################
echo -e "Testing OpenFPGA Shell";

echo -e "Testing Verilog generation with simple fracturable LUT6 ";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/frac_lut --debug --show_thread_logs

end_section "OpenFPGA.TaskTun"
