#!/bin/bash

set -e

start_section "OpenFPGA.TaskTun" "${GREEN}..Running_Regression..${NC}"
cd ${TRAVIS_BUILD_DIR}

###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-SPICE regression tests";

echo -e "Testing FPGA-SPICE with netlist generation";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_spice/generate_spice --debug --show_thread_logs

end_section "OpenFPGA.TaskTun"
