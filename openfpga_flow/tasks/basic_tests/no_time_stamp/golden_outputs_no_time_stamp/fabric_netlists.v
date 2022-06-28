//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Fabric Netlist Summary
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ------ Include defines: preproc flags -----
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/fpga_defines.v"

// ------ Include user-defined netlists -----
`include "/home/tangxifan/OpenFPGA/openfpga_flow/openfpga_cell_library/verilog/dff.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/openfpga_cell_library/verilog/gpio.v"
// ------ Include primitive module netlists -----
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/sub_module/inv_buf_passgate.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/sub_module/arch_encoder.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/sub_module/local_encoder.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/sub_module/mux_primitives.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/sub_module/muxes.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/sub_module/luts.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/sub_module/wires.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/sub_module/memories.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/sub_module/shift_register_banks.v"

// ------ Include logic block netlists -----
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/logical_tile_io_mode_physical__iopad.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/logical_tile_io_mode_io_.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__lut4.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4_mode_default__ff.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_default__fle_mode_n1_lut4__ble4.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_default__fle.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_clb_.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/grid_io_top.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/grid_io_right.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/grid_io_bottom.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/grid_io_left.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/lb/grid_clb.v"

// ------ Include routing module netlists -----
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/routing/sb_0__0_.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/routing/sb_0__1_.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/routing/sb_1__0_.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/routing/sb_1__1_.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/routing/cbx_1__0_.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/routing/cbx_1__1_.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/routing/cby_0__1_.v"
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/routing/cby_1__1_.v"

// ------ Include fabric top-level netlists -----
`include "/home/tangxifan/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/golden_outputs_no_time_stamp/fpga_top.v"

