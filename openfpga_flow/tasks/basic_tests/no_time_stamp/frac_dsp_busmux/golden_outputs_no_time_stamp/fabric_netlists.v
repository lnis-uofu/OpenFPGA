//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Fabric Netlist Summary
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ------ Include defines: preproc flags -----
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/fpga_defines.v"

// ------ Include user-defined netlists -----
`include "/home/xifan/github/OpenFPGA/openfpga_flow/openfpga_cell_library/verilog/dff.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/openfpga_cell_library/verilog/latch.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/openfpga_cell_library/verilog/gpio.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/openfpga_cell_library/verilog/adder.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/openfpga_cell_library/verilog/dpram1k.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/openfpga_cell_library/verilog/mult_32x32.v"
// ------ Include primitive module netlists -----
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/sub_module/inv_buf_passgate.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/sub_module/arch_encoder.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/sub_module/local_encoder.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/sub_module/mux_primitives.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/sub_module/muxes.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/sub_module/luts.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/sub_module/wires.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/sub_module/memories.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/sub_module/shift_register_banks.v"

// ------ Include logic block netlists -----
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_io_mode_physical__iopad.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_io_mode_io_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_default__fle.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_clb_mode_clb_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_memory_mode_mem_128x8_dp__mem_128x8_dp.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_memory_mode_memory_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/logical_tile_mult_32_mode_mult_32_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/grid_io_top.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/grid_io_right.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/grid_io_bottom.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/grid_io_left.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/grid_clb.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/grid_memory.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/lb/grid_mult_32.v"

// ------ Include routing module netlists -----
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_0__0_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_0__1_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_0__4_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_1__0_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_1__1_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_1__2_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_1__4_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_2__0_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_2__1_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_2__2_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_2__4_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_3__0_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_3__1_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_3__4_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_4__0_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_4__1_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/sb_4__4_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cbx_1__0_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cbx_1__1_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cbx_1__4_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cbx_2__0_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cbx_2__2_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cbx_2__4_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cbx_4__0_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cbx_4__4_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cby_0__1_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cby_1__1_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cby_1__2_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cby_2__1_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cby_2__2_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cby_3__1_.v"
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/routing/cby_4__1_.v"

// ------ Include tile module netlists -----

// ------ Include fabric top-level netlists -----
`include "/home/xifan/github/OpenFPGA/openfpga_flow/tasks/basic_tests/no_time_stamp/frac_dsp_busmux/golden_outputs_no_time_stamp/fpga_top.v"

