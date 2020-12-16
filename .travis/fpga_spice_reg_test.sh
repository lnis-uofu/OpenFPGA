#!/bin/bash

set -e

start_section "OpenFPGA.TaskTun" "${GREEN}..Running_Regression..${NC}"
cd ${TRAVIS_BUILD_DIR}

source .github/workflows/fpga_spice_reg_test.sh

end_section "OpenFPGA.TaskTun"
