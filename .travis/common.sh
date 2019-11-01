# Some colors, use it like following;
# echo -e "Hello ${YELLOW}yellow${NC}"
GRAY='\033[0;30m'
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

SPACER="echo -e ${GRAY} - ${NC}"

export -f travis_nanoseconds
export -f travis_fold
export -f travis_time_start
export -f travis_time_finish

function start_section() {
  $SPACER
	travis_fold start "$1"
	travis_time_start
	echo -e "${PURPLE}OpenFPGA${NC}: - $2${NC}"
	echo -e "${GRAY}-------------------------------------------------------------------${NC}"
}

function end_section() {
	echo -e "${GRAY}-------------------------------------------------------------------${NC}"
	travis_time_finish
	travis_fold end "$1"
  $SPACER
}

# For Mac OS, we use g++ and gcc as default compilers
if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  export CC=gcc-4.9
  export CXX=g++-4.9
  # export PATH="/usr/local/opt/bison/bin:/usr/local/bin:$PATH"
  # export PATH="/usr/local/opt/qt/bin:$PATH"
  # Install header files in Mojave, if not gcc-4.9 cannot spot stdio.h
  sudo installer -pkg /Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg -target / -allowUntrusted
else
  # For linux, we use g++-8 and gcc-8 as default compilers
  export CC=gcc-8
  export CXX=g++-8
fi

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

