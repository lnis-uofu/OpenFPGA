# 
# Top Makefile
# ------------
#
# Top-level makefile to compile the codebase
#
# Following options are available
# 
# .. option:: BUILD_TYPE=<string>
#
#  Pick the type of compilation. Can be either ``release`` or ``debug``. By default, release mode is selected (full optimization on runtime).
#
# .. option:: CMAKE_FLAGS=<string>
#
#  Force compilation flags for CMake to generate Makefiles

BUILD_TYPE ?= release
# Convert to lower case for consistency
BUILD_TYPE := $(shell echo ${BUILD_TYPE} | tr '[:upper:]' '[lower]')
# Trim any _pgo or _strict in the build type name (since it would not match any of CMake's standard build types
CMAKE_BUILD_TYPE := $(shell echo ${BUILD_TYPE} | sed 's/_\?pgo//' | sed 's/_\?strict//')
CMAKE_BUILD_CONFIG_VAL = Release
ifeq ($(CMAKE_BUILD_TYPE), release)
CMAKE_BUILD_CONFIG_VAL = Release
else ifeq ($(CMAKE_BUILD_TYPE), debug)
CMAKE_BUILD_CONFIG_VAL = Debug
else
$(error Invalid BUILD type '$(BUILD_TYPE)'. Allowed values are 'release' or 'debug')
endif

INTERNAL_CMAKE_FLAGS =
BUILD_INSTALLER ?= ON
# Convert to lower case for consistency
#BUILD_INSTALLER := $(shell echo ${BUILD_INSTALLER} | tr '[:upper:]' '[lower]')
INTERNAL_CMAKE_FLAGS += -DOPENFPGA_WITH_INSTALLER=${BUILD_INSTALLER}

INSTALL_DOC ?= ON
# Convert to lower case for consistency
#INSTALL_DOC := $(shell echo ${INSTALL_DOC} | tr '[:upper:]' '[lower]')
INTERNAL_CMAKE_FLAGS += -DOPENFPGA_INSTALL_DOC=${INSTALL_DOC}

#Detect the operating system
UNAME_S := $(shell uname -s)
# Cmake generator has to be Ninja on MSVC
CMAKE_GEN = Unix Makefiles
ifeq ($(OS),Windows_NT)
CMAKE_GEN = Ninja
# Msys2 can still use Linux gcc
ifeq ($(MSYSTEM),MINGW64)
CMAKE_GEN = Unix Makefiles
endif
endif
ifeq ($(UNAME_S),Darwin)
CMAKE_GEN = Ninja
endif

CMAKE_BUILD_CONFIG=
CPACK_CONFIG_TYPE=
ifeq ($(CMAKE_GEN),Ninja)
CPACK_CONFIG_TYPE=-C ${CMAKE_BUILD_CONFIG_VAL}
CMAKE_BUILD_CONFIG=--config ${CMAKE_BUILD_CONFIG_VAL}
endif

# Allow users to pass parameters to cmake, without defining build types
# e.g. make CMAKE_FLAGS="-DCMAKE_CXX_COMPILER=g++-9'
override CMAKE_FLAGS := -G '${CMAKE_GEN}' -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ${INTERNAL_CMAKE_FLAGS} ${CMAKE_FLAGS}

ifeq ($(OS),Windows_NT)
# The curl path should be defined by user. Try to get one from system
CURL_PATH:=$(shell where curl.exe 2>nul | head -n 1)
# VCPKG root is a system variable env:VCPKG in power shell. User can override by using VCPKG_PATH when calling the makefile
VCPKG_CMAKE_PATH:=$(subst \,/,$(VCPKG_PATH))
override CMAKE_FLAGS := ${CMAKE_FLAGS} -DVTR_IPO_BUILD=OFF -DWITH_ABC=OFF -DOPENFPGA_WITH_SWIG=OFF -DOPENFPGA_WITH_YOSYS=OFF
# Msys2 can still use Linux gcc
ifneq ($(MSYSTEM),MINGW64)
override CMAKE_FLAGS := ${CMAKE_FLAGS} -DWGET="${CURL_PATH}" -DCMAKE_TOOLCHAIN_FILE="${VCPKG_CMAKE_PATH}/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-release -DVCPKG_MANIFEST_MODE=OFF
endif
endif

# -s : Suppress makefile output (e.g. entering/leaving directories)
# --output-sync target : For parallel compilation ensure output for each target is synchronized (make version >= 4.0)
MAKEFLAGS := -s

# Directory to build the codes
CMAKE_GOALS = all
INSTALLER_TYPE=STGZ
# Branch on OS
ifeq ($(OS),Windows_NT)
ifeq ($(MSYSTEM),MINGW64)
SOURCE_DIR := $(PWD)
else
SOURCE_DIR := $(shell powershell -NoProfile -Command "(Get-Location).Path")
SOURCE_DIR := $(subst \,/,$(SOURCE_DIR))
endif
else
SOURCE_DIR := $(PWD)
endif
BUILD_DIR ?= build

# Find CMake command from system variable, otherwise use a default one
#Check for the cmake executable
CMAKE_COMMAND := $(shell command -v cmake 2> /dev/null)
ifeq ($(OS),Windows_NT)
ifneq ($(MSYSTEM),MINGW64)
CMAKE_COMMAND := cmake.exe
endif
endif

# Define executables
PYTHON_EXEC ?= python3
CLANG_FORMAT_EXEC ?= clang-format-14
XML_FORMAT_EXEC ?= xmllint
PYTHON_FORMAT_EXEC ?= black

# Extract the -j value from MAKEFLAGS if present, otherwise default to empty (CMake will auto-detect)
# This handles formats like '-j 4' or '-j4'
JOBS_FLAGS := $(shell echo "$(MAKEFLAGS)" | grep -oE '\-j[0-9]*' | sed 's/\-j/--parallel /')

# Put it first so that "make" without argument is like "make help".
export COMMENT_EXTRACT

# Put it first so that "make" without argument is like "make help".
help:
	@${PYTHON_EXEC} -c "$$COMMENT_EXTRACT"

.PHONY: help

checkout: 
# Update all the submodules
	git submodule init
	git submodule update --init --recursive

prebuild:
# Run cmake to generate Makefile under the build directory, before compilation
	@mkdir -p ${BUILD_DIR} && \
	echo "${CMAKE_COMMAND} -S ${SOURCE_DIR} -B ${BUILD_DIR} ${CMAKE_FLAGS}" && \
	${CMAKE_COMMAND} -S ${SOURCE_DIR} -B ${BUILD_DIR} ${CMAKE_FLAGS}

compile: | prebuild
# Compile the code base. By default, all the targets will be compiled
# Following options are available
# .. option:: CMAKE_GOALS
#
#   Define the target for cmake to compile. for example, ``cmake_goals=openfpga`` indicates that only openfpga binary will be compiled 
	echo "Building target(s): ${CMAKE_GOALS}" && \
	echo "${CMAKE_COMMAND} ${CMAKE_BUILD_CONFIG} --build ${BUILD_DIR} --target ${CMAKE_GOALS} ${JOB_FLAGS}" && \
	${CMAKE_COMMAND} ${CMAKE_BUILD_CONFIG} --build ${BUILD_DIR} --target ${CMAKE_GOALS} ${JOB_FLAGS}

list_cmake_targets: | prebuild
# Show the targets available to be built, which can be specified through ``CMAKE_GOALS`` when compile
	cd ${BUILD_DIR} && make help && cd -

all: checkout
# A shortcut command to run checkout and compile in serial
	@+${MAKE} compile

installer:
# Create the package for users to install on their computer with pre-built binaries
#
# Following options are available
# .. option:: INSTALLER_TYPE
#
#   Define the type of installer, can be [STGZ|DEB|IFW] (default: STGZ). for example, ``INSTALLER_TYPE=DEB`` indicates to create a DEB package
	cpack ${CPACK_BUILD_CONFIG} --config ${BUILD_DIR}/CPackConfig.cmake -G ${INSTALLER_TYPE}

format-cpp:
# Format all the C/C++ files under this project, excluding submodules
	for f in `find libs openfpga -iname *.cpp -o -iname *.hpp -o -iname *.c -o -iname *.h`; \
	do \
	${CLANG_FORMAT_EXEC} --style=file -i $${f} || exit 1; \
	done

format-xml:
# Format all the XML files under this project, excluding submodules
	for f in `find openfpga_flow/vpr_arch openfpga_flow/openfpga_arch -iname *.xml`; \
	do \
	XMLLINT_INDENT="  " && ${XML_FORMAT_EXEC} --format $${f} --output $${f} || exit 1; \
	done

format-py:
# Format all the python scripts under this project, excluding submodules
	for f in `find openfpga_flow/scripts -iname *.py`; \
	do \
	${PYTHON_FORMAT_EXEC} $${f} --line-length 100 || exit 1; \
	done

format-all: format-cpp format-xml format-py
# Format all the C/C++, XML and Python codes

clean:
# Remove current build results
	rm -rf ${BUILD_DIR} yosys/install

# Functions to extract comments from Makefiles
define COMMENT_EXTRACT
import re
with open ('Makefile', 'r' ) as f:
    matches = re.finditer('^([a-zA-Z-_]*):.*\n#(.*)', f.read(), flags=re.M)
    for _, match in enumerate(matches, start=1):
        header, content = match[1], match[2]
        print(f"  {header:10} {content}")
endef
