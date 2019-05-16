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
  cmake ..
  make -j2
fi
end_section "OpenFPGA.build"

$SPACER

source .travis/regression.sh
