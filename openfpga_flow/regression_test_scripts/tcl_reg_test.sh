#!/bin/bash

set -e
###############################################
# OpenFPGA Shell in Tcl
##############################################
source openfpga.sh
echo -e "Regression tests for OpenFPGA in Tcl Shell";

cd build/openfpga
${OPENFPGA_PATH}/openfpga_flow/scripts/swig_tcl_example.tcl
cd -
