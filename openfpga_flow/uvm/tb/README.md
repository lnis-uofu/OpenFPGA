How to run the testbench :

cd /research/ece/lnis/USERS/scharas/UVM_to_git/golden_tb_2x2
vsim &
source /research/ece/lnis/USERS/scharas/UVM_to_git/scripts/openfpga_variables.tcl
top_create_new_project
simulate_genmode

Or manually
vsim FPGA_DUT_rtl.openfpga_tb_opt +UVM_TESTNAME=<test_name>


