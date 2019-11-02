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
echo -e "Testing single-mode architectures";
python3 openfpga_flow/scripts/run_fpga_task.py single_mode
python3 openfpga_flow/scripts/run_fpga_task.py single_mode --remove_run_dir all
#python3 openfpga_flow/scripts/run_fpga_task.py s298

echo -e "Testing multi-mode architectures";
python3 openfpga_flow/scripts/run_fpga_task.py blif_vpr_flow --maxthreads 2
python3 openfpga_flow/scripts/run_fpga_task.py blif_vpr_flow --remove_run_dir all

echo -e "Testing compact routing techniques";
python3 openfpga_flow/scripts/run_fpga_task.py compact_routing
python3 openfpga_flow/scripts/run_fpga_task.py compact_routing --remove_run_dir all

echo -e "Testing tileable architectures";
python3 openfpga_flow/scripts/run_fpga_task.py tileable_routing
python3 openfpga_flow/scripts/run_fpga_task.py tileable_routing --remove_run_dir all

echo -e "Testing Verilog generation with explicit port mapping ";
python3 openfpga_flow/scripts/run_fpga_task.py explicit_verilog
python3 openfpga_flow/scripts/run_fpga_task.py explicit_verilog --remove_run_dir all

end_section "OpenFPGA.TaskTun"
