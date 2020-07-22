#!/bin/bash

source .travis/common.sh
set -e

start_section "OpenFPGA.build" "${GREEN}Building..${NC}"
cd ${TRAVIS_BUILD_DIR}
mkdir build
cd build

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  cmake .. -DENABLE_VPR_GRAPHICS=off
else
  cmake .. 
fi
make -j16

# Return to upper directory
cd ${TRAVIS_BUILD_DIR}

end_section "OpenFPGA.build"
