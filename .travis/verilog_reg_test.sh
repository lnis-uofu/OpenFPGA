#!/bin/bash

set -e

start_section "OpenFPGA.TaskTun" "${GREEN}..Running_Regression..${NC}"
cd ${TRAVIS_BUILD_DIR}

###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "OpenFPGA Feature Testing for Verilog-to-Verification";

echo -e "Testing Verilog generation for LUTs: a single mode LUT6 FPGA using micro benchmarks";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/lut_design/single_mode --debug --show_thread_logs

echo -e "Testing Verilog generation for LUTs: simple fracturable LUT6 ";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/lut_design/frac_lut --debug --show_thread_logs

echo -e "Testing Verilog generation for LUTs: LUT6 with intermediate buffers";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/lut_design/intermediate_buffer --debug --show_thread_logs

echo -e "Testing Verilog generation with VPR's untileable routing architecture ";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/untileable --debug --show_thread_logs

echo -e "Testing Verilog generation with hard adder chain in CLBs ";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/hard_adder --debug --show_thread_logs

echo -e "Testing Verilog generation with 16k block RAMs ";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/bram/dpram16k --debug --show_thread_logs

echo -e "Testing Verilog generation with 16k block RAMs spanning two columns ";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/bram/wide_dpram16k --debug --show_thread_logs

echo -e "Testing Verilog generation with different I/O capacities on each side of an FPGA ";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/io/multi_io_capacity --debug --show_thread_logs

echo -e "Testing Verilog generation with I/Os only on left and right sides of an FPGA ";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/io/reduced_io --debug --show_thread_logs

echo -e "Testing Verilog generation with adder chain across an FPGA";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/fabric_chain/adder_chain --debug --show_thread_logs

echo -e "Testing Verilog generation with shift register chain across an FPGA";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/fabric_chain/register_chain --debug --show_thread_logs

echo -e "Testing Verilog generation with scan chain across an FPGA";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/fabric_chain/scan_chain --debug --show_thread_logs

echo -e "Testing Verilog generation with routing mutliplexers implemented by tree structure";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/mux_design/tree_structure --debug --show_thread_logs

echo -e "Testing Verilog generation with routing mutliplexers implemented by standard cell MUX2";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/mux_design/stdcell_mux2 --debug --show_thread_logs

echo -e "Testing Verilog generation with routing mutliplexers implemented by local encoders";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/mux_design/local_encoder --debug --show_thread_logs

echo -e "Testing Verilog generation with behavioral description";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/behavioral_verilog --debug --show_thread_logs

echo -e "Testing implicit Verilog generation";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/implicit_verilog --debug --show_thread_logs

echo -e "Testing Verilog generation with flatten routing modules";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/flatten_routing --debug --show_thread_logs

echo -e "Testing Verilog generation with duplicated grid output pins";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/duplicated_grid_pin --debug --show_thread_logs

echo -e "Testing Verilog generation with spy output pads";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/spypad --debug --show_thread_logs

echo -e "Testing Secured FPGA fabrics";
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/fabric_key/generate_vanilla_key --debug --show_thread_logs
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/fabric_key/generate_random_key --debug --show_thread_logs
python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/fabric_key/load_external_key --debug --show_thread_logs

# Verify MCNC big20 benchmark suite with ModelSim 
# Please make sure you have ModelSim installed in the environment
# Otherwise, it will fail
#python3 openfpga_flow/scripts/run_fpga_task.py openfpga_shell/mcnc_big20 --debug --show_thread_logs --maxthreads 20
#python3 openfpga_flow/scripts/run_modelsim.py openfpga_shell/mcnc_big20 --run_sim 

end_section "OpenFPGA.TaskTun"
