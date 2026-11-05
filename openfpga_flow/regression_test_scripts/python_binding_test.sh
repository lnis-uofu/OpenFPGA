#!/bin/bash

set -e
###############################################
# OpenFPGA Shell in Python
##############################################
source openfpga.sh
echo -e "Regression tests for OpenFPGA in Python Shell";

# The module (_openfpga.so and openfpga.py) is generated under the build directory
# when OPENFPGA_SWIG_PYTHON is enabled
export OPENFPGA_PYTHON_PATH=${OPENFPGA_PYTHON_PATH:-${OPENFPGA_PATH}/build/openfpga}

if [ ! -f "${OPENFPGA_PYTHON_PATH}/openfpga.py" ]; then
  echo "Error: Python module not found in ${OPENFPGA_PYTHON_PATH}"
  echo "Build with -DOPENFPGA_SWIG_PYTHON=ON (and a Python with development headers, e.g. -DPython3_EXECUTABLE=<path>)"
  exit 1
fi

# Older environment images do not ship pytest
${PYTHON_EXEC} -m pytest --version > /dev/null 2>&1 || ${PYTHON_EXEC} -m pip install pytest

${PYTHON_EXEC} -m pytest ${OPENFPGA_PATH}/openfpga/python_tests -v
