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
