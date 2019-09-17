# python3 openfpga_flow/scripts/run_fpga_flow.py \
# ./openfpga_flow/arch/template/k6_N10_sram_chain_HC_template.xml \
# ./openfpga_flow/benchmarks/MCNC_Verilog/s298/s298.v \
# --top_module s298 \
# --power \
# --power_tech ./openfpga_flow/tech/PTM_22nm/22nm.xml \
# --min_route_chan_width 1.3 \
# --vpr_fpga_verilog \
# --vpr_fpga_verilog_dir . \
# --vpr_fpga_x2p_rename_illegal_port \
# --end_flow_with_test \
# --vpr_fpga_verilog_include_icarus_simulator \
# --vpr_fpga_verilog_formal_verification_top_netlist \
# --vpr_fpga_verilog_include_timing \
# --vpr_fpga_verilog_include_signal_init \
# --vpr_fpga_verilog_print_autocheck_top_testbench

# Test popular multi-mode architecture
python3 openfpga_flow/scripts/run_fpga_flow.py \
./openfpga_flow/arch/template/k6_N10_sram_chain_HC_template.xml \
./openfpga_flow/benchmarks/Test_Modes/test_modes.blif \
--fpga_flow vpr_blif \
--top_module test_modes \
--activity_file ./openfpga_flow/benchmarks/Test_Modes/test_modes.act \
--base_verilog ./openfpga_flow/benchmarks/Test_Modes/test_modes.v \
--power \
--power_tech ./openfpga_flow/tech/PTM_45nm/45nm.xml \
#--fix_route_chan_width 300 \
--min_route_chan_width 1.3 \
--vpr_fpga_verilog \
--vpr_fpga_verilog_dir . \
--vpr_fpga_x2p_rename_illegal_port \
--vpr_fpga_verilog_include_icarus_simulator \
--vpr_fpga_verilog_formal_verification_top_netlist \
--vpr_fpga_verilog_include_timing \
--vpr_fpga_verilog_include_signal_init \
--vpr_fpga_verilog_print_autocheck_top_testbench \
--debug \
--vpr_fpga_bitstream_generator \
--vpr_fpga_verilog_print_user_defined_template \
--vpr_fpga_verilog_print_report_timing_tcl \
--vpr_fpga_verilog_print_sdc_pnr \
--vpr_fpga_verilog_print_sdc_analysis \
--vpr_fpga_x2p_compact_routing_hierarchy \
--end_flow_with_test

# Test Standard cell MUX2
python3 openfpga_flow/scripts/run_fpga_flow.py \
./openfpga_flow/arch/template/k8_N10_sram_chain_FC_template.xml \
./openfpga_flow/benchmarks/Test_Modes/test_modes.blif \
--fpga_flow vpr_blif \
--top_module test_modes \
--activity_file ./openfpga_flow/benchmarks/Test_Modes/test_modes.act \
--base_verilog ./openfpga_flow/benchmarks/Test_Modes/test_modes.v \
--power \
--power_tech ./openfpga_flow/tech/PTM_45nm/45nm.xml \
#--fix_route_chan_width 300 \
--min_route_chan_width 1.3 \
--vpr_fpga_verilog \
--vpr_fpga_verilog_dir . \
--vpr_fpga_x2p_rename_illegal_port \
--vpr_fpga_verilog_include_icarus_simulator \
--vpr_fpga_verilog_formal_verification_top_netlist \
--vpr_fpga_verilog_include_timing \
--vpr_fpga_verilog_include_signal_init \
--vpr_fpga_verilog_print_autocheck_top_testbench \
--debug \
--vpr_fpga_bitstream_generator \
--vpr_fpga_verilog_print_user_defined_template \
--vpr_fpga_verilog_print_report_timing_tcl \
--vpr_fpga_verilog_print_sdc_pnr \
--vpr_fpga_verilog_print_sdc_analysis \
--vpr_fpga_x2p_compact_routing_hierarchy \
--end_flow_with_test

# Test local encoder feature
python3 openfpga_flow/scripts/run_fpga_flow.py \
./openfpga_flow/arch/template/k6_N10_sram_chain_HC_local_encoder_template.xml \
./openfpga_flow/benchmarks/Test_Modes/test_modes.blif \
--fpga_flow vpr_blif \
--top_module test_modes \
--activity_file ./openfpga_flow/benchmarks/Test_Modes/test_modes.act \
--base_verilog ./openfpga_flow/benchmarks/Test_Modes/test_modes.v \
--power \
--power_tech ./openfpga_flow/tech/PTM_45nm/45nm.xml \
--fix_route_chan_width 300 \
--vpr_fpga_verilog \
--vpr_fpga_verilog_dir . \
--vpr_fpga_x2p_rename_illegal_port \
--vpr_fpga_verilog_include_icarus_simulator \
--vpr_fpga_verilog_formal_verification_top_netlist \
--vpr_fpga_verilog_include_timing \
--vpr_fpga_verilog_include_signal_init \
--vpr_fpga_verilog_print_autocheck_top_testbench \
--debug \
--vpr_fpga_bitstream_generator \
--vpr_fpga_verilog_print_user_defined_template \
--vpr_fpga_verilog_print_report_timing_tcl \
--vpr_fpga_verilog_print_sdc_pnr \
--vpr_fpga_verilog_print_sdc_analysis \
--vpr_fpga_x2p_compact_routing_hierarchy \
--end_flow_with_test

# Test tileable routing feature
#python3 openfpga_flow/scripts/run_fpga_flow.py \
#./openfpga_flow/arch/template/k6_N10_sram_chain_HC_tileable_template.xml \
#./openfpga_flow/benchmarks/Test_Modes/test_modes.blif \
#--fpga_flow vpr_blif \
#--top_module test_modes \
#--activity_file ./openfpga_flow/benchmarks/Test_Modes/test_modes.act \
#--base_verilog ./openfpga_flow/benchmarks/Test_Modes/test_modes.v \
#--power \
#--power_tech ./openfpga_flow/tech/PTM_45nm/45nm.xml \
##--fix_route_chan_width 300 \
#--min_route_chan_width 1.3 \
#--vpr_fpga_verilog \
#--vpr_fpga_verilog_dir . \
#--vpr_fpga_x2p_rename_illegal_port \
#--vpr_fpga_verilog_include_icarus_simulator \
#--vpr_fpga_verilog_formal_verification_top_netlist \
#--vpr_fpga_verilog_include_timing \
#--vpr_fpga_verilog_include_signal_init \
#--vpr_fpga_verilog_print_autocheck_top_testbench \
#--debug \
#--vpr_fpga_bitstream_generator \
#--vpr_fpga_verilog_print_user_defined_template \
#--vpr_fpga_verilog_print_report_timing_tcl \
#--vpr_fpga_verilog_print_sdc_pnr \
#--vpr_fpga_verilog_print_sdc_analysis \
#--vpr_fpga_x2p_compact_routing_hierarchy \
#--vpr_use_tileable_route_chan_width \
#--end_flow_with_test

