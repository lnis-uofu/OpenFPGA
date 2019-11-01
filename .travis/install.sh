#!/bin/bash

source .travis/common.sh
set -e

# Install necessary package which is not available on Travis CI 
export DEPS_DIR="${HOME}/deps"
mkdir -p ${DEPS_DIR} && cd ${DEPS_DIR}

# Install CMake
if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
  export CMAKE_URL="https://cmake.org/files/v3.13/cmake-3.13.0-rc3-Linux-x86_64.tar.gz"
  mkdir cmake && travis_retry wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake
  export PATH=${DEPS_DIR}/cmake/bin:${PATH}
  echo ${PATH}
else
  brew install cmake || brew upgrade cmake
fi

cmake --version

# Install latest iVerilog. Since no deb is provided, compile from source codes
if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
  export IVERILOG_URL="https://github.com/steveicarus/iverilog/archive/v10_3.tar.gz"
  mkdir iverilog-10.3 && travis_retry wget --no-check-certificate --quiet -O - ${IVERILOG_URL} | tar --strip-components=1 -xz -C iverilog-10.3
  cd iverlog-10.3
  sh autoconf.sh --prefix=${DEPS_DIR}/iverilog-10.3/bin
  ./configure --prefix=${DEPS_DIR}/iverilog-10.3/bin
  make -j4
  make check
  make install --prefix=${HOME}/iverilog-10.3/bin
  export PATH=${DEPS_DIR}/iverilog-10.3/bin:${PATH}
  echo ${PATH}
fi

iverilog --version

# Go back to home directory
cd -


# Git repo fixup
#start_section "environment.git" "Setting up ${YELLOW}git checkout${NC}"
#set -x
#git fetch --tags
#git submodule update --recursive --init
#git submodule foreach git submodule update --recursive --init
#set +x
#end_section "environment.git"

$SPACER

