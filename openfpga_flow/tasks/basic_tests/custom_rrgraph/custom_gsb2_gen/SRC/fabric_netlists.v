//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Fabric Netlist Summary
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ------ Include defines: preproc flags -----
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/fpga_defines.v"

// ------ Include user-defined netlists -----
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/openfpga_cell_library/verilog/dff.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/openfpga_cell_library/verilog/gpio.v"
// ------ Include primitive module netlists -----
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/sub_module/inv_buf_passgate.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/sub_module/arch_encoder.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/sub_module/local_encoder.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/sub_module/mux_primitives.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/sub_module/muxes.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/sub_module/luts.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/sub_module/wires.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/sub_module/memories.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/sub_module/shift_register_banks.v"

// ------ Include logic block netlists -----
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/logical_tile_io_mode_physical__iopad.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/logical_tile_io_mode_io_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/logical_tile_clb_mode_default__fle_mode_physical__fabric.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/logical_tile_clb_mode_default__fle.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/logical_tile_clb_mode_clb_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/grid_clb.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/grid_clb_tile.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/grid_io_left_tile.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/grid_io_right_tile.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/grid_io_top_tile.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/lb/grid_io_bottom_tile.v"

// ------ Include routing module netlists -----
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_0__0_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_0__1_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_0__6_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_1__0_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_1__1_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_1__2_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_1__5_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_1__6_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_2__1_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_2__2_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_2__6_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_5__1_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_5__2_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_6__0_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_6__1_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_6__2_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/sb_6__6_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/cbx_1__0_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/cbx_2__1_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/cbx_2__2_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/cbx_6__2_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/cby_0__1_.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/routing/cby_1__2_.v"

// ------ Include tile module netlists -----

// ------ Include fabric top-level netlists -----
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/tasks/basic_tests/custom_rrgraph/custom_gsb2_gen/SRC/fpga_top.v"

