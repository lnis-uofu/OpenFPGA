#!/bin/bash

source .travis/common.sh
set -e

$SPACER

start_section "OpenFPGA.build" "${GREEN}Building..${NC}"
if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  #make
  mkdir build
  cd build
  cmake ..
  make -j2
else 
# For linux, we enable full package compilation
  #make
  mkdir build
  cd build
  cmake --version
  cmake .. #-DCMAKE_BUILD_TYPE=debug
  make -j16
fi
end_section "OpenFPGA.build"

$SPACER

cd -
source .travis/regression.sh

#cd fpga_flow
#./regression_fpga_flow.sh
#cd -
