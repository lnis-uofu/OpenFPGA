python3.5 openfpga_flow/scripts/run_fpga_flow.py \
./openfpga_flow/arch/winbond90/k6_N10_rram_memory_bank_SC_winbond90.xml \
./openfpga_flow/benchmarks/MCNC_Verilog/s298/s298.v \
--top_module s298 \
--power \
--power_tech ./openfpga_flow/tech/winbond90nm/winbond90nm_power_properties.xml \
--min_route_chan_width 1.3 \
--vpr_fpga_verilog \
--vpr_fpga_verilog_dir ./SRC \
--vpr_fpga_x2p_rename_illegal_port \
--vpr_fpga_verilog_print_autocheck_top_testbench


# \
# --end_flow_with_test \
# --vpr_fpga_verilog_print_autocheck_top_testbench \
# --vpr_fpga_verilog_include_icarus_simulator \
# --vpr_fpga_verilog_formal_verification_top_netlist


# '/research/ece/lnis/USERS/alacchi/Ganesh/OpenFPGA/vpr7_x2p/vpr/vpr',
# '/research/ece/lnis/USERS/alacchi/Ganesh/OpenFPGA/tmp/arch/k6_N10_rram_memory_bank_SC_winbond90.xml', 's298_ace_corrected_out.blif'
# '--net_file'
# 's298_vpr.net'
# '--place_file'
# 's298_vpr.place'
# '--route_file'
# 's298_vpr.route'
# '--full_stats'
# '--nodisp'
# '--power'
# '--activity_file'
# 's298_ace_out.act'
# '--tech_properties'
# '/research/ece/lnis/USERS/alacchi/Ganesh/OpenFPGA/openfpga_flow/tech/winbond90nm/winbond90nm_power_properties.xml'
# '--fpga_verilog'
# '--fpga_verilog_dir'
# './SRC'
# '--fpga_verilog_print_autocheck_top_testbench'
# 's298_output_verilog.v'
# '--fpga_verilog_print_formal_verification_top_netlist'
# '--fpga_verilog_include_icarus_simulator'
# '--fpga_x2p_rename_illegal_port'