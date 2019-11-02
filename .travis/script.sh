#!/bin/bash

source .travis/common.sh
set -e

start_section "OpenFPGA.build" "${GREEN}Building..${NC}"
mkdir build
cd build

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  cmake .. -DCMAKE_BUILD_TYPE=debug -DENABLE_VPR_GRAPHICS=off
else
  cmake .. -DCMAKE_BUILD_TYPE=debug
fi
 make -j16
end_section "OpenFPGA.build"


start_section "OpenFPGA.TaskTun" "${GREEN}..Running_Regression..${NC}"
cd -
./vpr7_x2p/vpr/vpr ./openfpga_flow/arch/template/k4_N4_sram_chain_FC_behavioral_verilog_template.xml ./openfpga_flow/benchmarks/test_modes/k4_N4/K4N4_test_modes.blif --net_file ./openfpga_flow/benchmarks/test_modes/k4_N4/K4n4_test_vpr.net --place_file ./openfpga_flow/benchmarks/test_modes/k4_N4/K4n4_test_vpr.place --route_file ./openfpga_flow/benchmarks/test_modes/k4_N4/K4n4_test_vpr.route --full_stats --nodisp --activity_file ./openfpga_flow/benchmarks/test_modes/k4_N4/K4N4_test_modes.act --power --tech_properties ./openfpga_flow/tech/PTM_45nm/45nm.xml --fpga_x2p_compact_routing_hierarchy --fpga_verilog --fpga_verilog_dir ./verilog --fpga_verilog_print_autocheck_top_testbench K4n4_test_output_verilog.v --fpga_verilog_include_timing --fpga_verilog_explicit_mapping --fpga_verilog_include_signal_init --fpga_verilog_print_formal_verification_top_netlist --fpga_verilog_include_icarus_simulator --fpga_verilog_print_report_timing_tcl --fpga_verilog_print_sdc_pnr --fpga_verilog_print_user_defined_template --fpga_verilog_print_sdc_analysis --fpga_bitstream_generator --fpga_x2p_rename_illegal_port

echo -e "Testing single-mode architectures";
python3 openfpga_flow/scripts/run_fpga_task.py single_mode --debug --show_thread_logs
#python3 openfpga_flow/scripts/run_fpga_task.py s298 

echo -e "Testing multi-mode architectures";
python3 openfpga_flow/scripts/run_fpga_task.py blif_vpr_flow --maxthreads 4 --debug --show_thread_logs

echo -e "Testing compact routing techniques";
python3 openfpga_flow/scripts/run_fpga_task.py compact_routing --debug --show_thread_logs

echo -e "Testing tileable architectures";
python3 openfpga_flow/scripts/run_fpga_task.py tileable_routing --debug --show_thread_logs

echo -e "Testing Verilog generation with explicit port mapping ";
python3 openfpga_flow/scripts/run_fpga_task.py explicit_verilog --debug --show_thread_logs

end_section "OpenFPGA.TaskTun"
