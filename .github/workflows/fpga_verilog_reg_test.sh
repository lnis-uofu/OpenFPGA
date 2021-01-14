#!/bin/bash

set -e

###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "FPGA-Verilog Feature Tests";

echo -e "Testing Verilog generation for LUTs: a single mode LUT6 FPGA using micro benchmarks";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/lut_design/single_mode --debug --show_thread_logs

echo -e "Testing Verilog generation for LUTs: simple fracturable LUT4 ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/lut_design/frac_lut4 --debug --show_thread_logs

echo -e "Testing Verilog generation for LUTs: native fracturable LUT4 ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/lut_design/frac_native_lut4 --debug --show_thread_logs

echo -e "Testing Verilog generation for LUTs: simple fracturable LUT4 using AND gate to switch modes";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/lut_design/frac_lut4_and_switch --debug --show_thread_logs

echo -e "Testing Verilog generation for LUTs: simple fracturable LUT6 ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/lut_design/frac_lut6 --debug --show_thread_logs

echo -e "Testing Verilog generation for LUTs: LUT6 with intermediate buffers";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/lut_design/intermediate_buffer --debug --show_thread_logs

echo -e "Testing Verilog generation with VPR's untileable routing architecture ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/untileable --debug --show_thread_logs

echo -e "Testing Verilog generation with hard adder chain in CLBs ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/hard_adder --debug --show_thread_logs

echo -e "Testing Verilog generation with 16k block RAMs ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/bram/dpram16k --debug --show_thread_logs

echo -e "Testing Verilog generation with 16k block RAMs spanning two columns ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/bram/wide_dpram16k --debug --show_thread_logs

echo -e "Testing Verilog generation with different I/O capacities on each side of an FPGA ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/io/multi_io_capacity --debug --show_thread_logs

echo -e "Testing Verilog generation with I/Os only on left and right sides of an FPGA ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/io/reduced_io --debug --show_thread_logs

echo -e "Testing Verilog generation with embedded I/Os for an FPGA ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/io/embedded_io --debug --show_thread_logs

echo -e "Testing Verilog generation with SoC I/Os for an FPGA ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/io/soc_io --debug --show_thread_logs

echo -e "Testing Verilog generation with registerable I/Os for an FPGA ";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/io/registerable_io --debug --show_thread_logs

echo -e "Testing Verilog generation with adder chain across an FPGA";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/fabric_chain/adder_chain --debug --show_thread_logs

echo -e "Testing Verilog generation with shift register chain across an FPGA";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/fabric_chain/register_chain --debug --show_thread_logs

echo -e "Testing Verilog generation with scan chain across an FPGA";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/fabric_chain/scan_chain --debug --show_thread_logs

echo -e "Testing Verilog generation with routing multiplexers implemented by tree structure";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/mux_design/tree_structure --debug --show_thread_logs

echo -e "Testing Verilog generation with routing multiplexers implemented by standard cell MUX2";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/mux_design/stdcell_mux2 --debug --show_thread_logs

echo -e "Testing Verilog generation with routing multiplexers implemented by local encoders";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/mux_design/local_encoder --debug --show_thread_logs

echo -e "Testing Verilog generation with routing multiplexers without buffers";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/mux_design/debuf_mux --debug --show_thread_logs

echo -e "Testing Verilog generation with routing multiplexers with input buffers only";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/mux_design/inbuf_only_mux --debug --show_thread_logs

echo -e "Testing Verilog generation with routing multiplexers with output buffers only";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/mux_design/outbuf_only_mux --debug --show_thread_logs

echo -e "Testing Verilog generation with routing multiplexers with constant gnd input";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/mux_design/const_input_gnd --debug --show_thread_logs

echo -e "Testing Verilog generation with routing multiplexers without constant inputs";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/mux_design/no_const_input --debug --show_thread_logs

echo -e "Testing Verilog generation with behavioral description";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/behavioral_verilog --debug --show_thread_logs

echo -e "Testing implicit Verilog generation";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/implicit_verilog --debug --show_thread_logs

echo -e "Testing Verilog generation with flatten routing modules";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/flatten_routing --debug --show_thread_logs

echo -e "Testing Verilog generation with duplicated grid output pins";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/duplicated_grid_pin --debug --show_thread_logs

echo -e "Testing Verilog generation with spy output pads";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/spypad --debug --show_thread_logs


echo -e "Testing Power-gating designs";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/power_gated_design/power_gated_inverter --show_thread_logs --debug

echo -e "Testing Depopulated crossbar in local routing";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/depopulate_crossbar --debug --show_thread_logs

echo -e "Testing Fully connected output crossbar in local routing";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/fully_connected_output_crossbar --debug --show_thread_logs

echo -e "Testing through channels in tileable routing";
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/thru_channel/thru_narrow_tile --debug --show_thread_logs
python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/thru_channel/thru_wide_tile --debug --show_thread_logs

# Verify MCNC big20 benchmark suite with ModelSim 
# Please make sure you have ModelSim installed in the environment
# Otherwise, it will fail
#python3 openfpga_flow/scripts/run_fpga_task.py fpga_verilog/mcnc_big20 --debug --show_thread_logs --maxthreads 20
#python3 openfpga_flow/scripts/run_modelsim.py mcnc_big20 --run_sim 
