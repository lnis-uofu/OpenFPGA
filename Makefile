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

BUILD_INSTALLER ?= ON
# Convert to lower case for consistency
#BUILD_INSTALLER := $(shell echo ${BUILD_INSTALLER} | tr '[:upper:]' '[lower]')
CMAKE_FLAGS += -DOPENFPGA_WITH_INSTALLER=${BUILD_INSTALLER}

INSTALL_DOC ?= ON
# Convert to lower case for consistency
INSTALL_DOC := $(shell echo ${INSTALL_DOC} | tr '[:upper:]' '[lower]')
CMAKE_FLAGS +=  -DOPENFPGA_INSTALL_DOC=${INSTALL_DOC}

# Allow users to pass parameters to cmake, without defining build types
# e.g. make CMAKE_FLAGS="-DCMAKE_CXX_COMPILER=g++-9'
override CMAKE_FLAGS := -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ${CMAKE_FLAGS}

# -s : Suppress makefile output (e.g. entering/leaving directories)
# --output-sync target : For parallel compilation ensure output for each target is synchronized (make version >= 4.0)
MAKEFLAGS := -s

# Directory to build the codes
SOURCE_DIR :=${PWD}
BUILD_DIR ?= build
CMAKE_GOALS = all
INSTALLER_TYPE=STGZ

# Find CMake command from system variable, otherwise use a default one
ifeq ($(origin CMAKE_COMMAND),undefined)
CMAKE_COMMAND := cmake
else
CMAKE_COMMAND := ${CMAKE_COMMAND}
endif

# Define executables
PYTHON_EXEC ?= python3
CLANG_FORMAT_EXEC ?= clang-format-14
XML_FORMAT_EXEC ?= xmllint
PYTHON_FORMAT_EXEC ?= black

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
	echo "cd ${BUILD_DIR} && ${CMAKE_COMMAND} ${CMAKE_FLAGS} ${SOURCE_DIR}" && \
	cd ${BUILD_DIR} && ${CMAKE_COMMAND} ${CMAKE_FLAGS} ${SOURCE_DIR}

compile: | prebuild
# Compile the code base. By default, all the targets will be compiled
# Following options are available
# .. option:: CMAKE_GOALS
#
#   Define the target for cmake to compile. for example, ``cmake_goals=openfpga`` indicates that only openfpga binary will be compiled 
	echo "Building target(s): ${CMAKE_GOALS}"
	@+${MAKE} -C ${BUILD_DIR} ${CMAKE_GOALS}

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
	cpack --config ${BUILD_DIR}/CPackConfig.cmake -G ${INSTALLER_TYPE}

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
