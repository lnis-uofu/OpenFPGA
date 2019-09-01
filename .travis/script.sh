#!/bin/bash

source .travis/common.sh
set -e

$SPACER

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
$SPACER

start_section "OpenFPGA.TaskTun" "${GREEN}..Running_Regression..${NC}"
cd -
python3 openfpga_flow/scripts/run_fpga_task.py blif_vpr_flow
end_section "OpenFPGA.TaskTun"
