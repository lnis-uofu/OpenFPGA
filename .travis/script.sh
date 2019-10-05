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
python3 openfpga_flow/scripts/run_fpga_task.py blif_vpr_flow tileable_routing explicit_verilog --maxthreads 2
end_section "OpenFPGA.TaskTun"
