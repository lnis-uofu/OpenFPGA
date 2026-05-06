//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Fabric Netlist Summary
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ------ Include defines: preproc flags -----
`include "fpga_defines.v"

// ------ Include user-defined netlists -----
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/openfpga_cell_library/verilog/dff.v"
`include "/localproj/ggore/OpenFPGA-QL/openfpga_flow/openfpga_cell_library/verilog/gpio.v"
// ------ Include primitive module netlists -----
`include "sub_module/inv_buf_passgate.v"
`include "sub_module/arch_encoder.v"
`include "sub_module/local_encoder.v"
`include "sub_module/mux_primitives.v"
`include "sub_module/muxes.v"
`include "sub_module/luts.v"
`include "sub_module/wires.v"
`include "sub_module/memories.v"
`include "sub_module/shift_register_banks.v"

// ------ Include logic block netlists -----
`include "lb/logical_tile_io_mode_physical__iopad.v"
`include "lb/logical_tile_io_mode_io_.v"
`include "lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_mode_default__frac_lut4.v"
`include "lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic.v"
`include "lb/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__ff.v"
`include "lb/logical_tile_clb_mode_default__fle_mode_physical__fabric.v"
`include "lb/logical_tile_clb_mode_default__fle.v"
`include "lb/logical_tile_clb_mode_clb_.v"
`include "lb/grid_clb.v"
`include "lb/grid_clb_tile.v"
`include "lb/grid_io_left_tile.v"
`include "lb/grid_io_right_tile.v"
`include "lb/grid_io_top_tile.v"
`include "lb/grid_io_bottom_tile.v"

// ------ Include routing module netlists -----
`include "routing/sb_0__0_.v"
`include "routing/sb_0__1_.v"
`include "routing/sb_0__6_.v"
`include "routing/sb_1__0_.v"
`include "routing/sb_1__1_.v"
`include "routing/sb_1__2_.v"
`include "routing/sb_1__5_.v"
`include "routing/sb_1__6_.v"
`include "routing/sb_2__1_.v"
`include "routing/sb_2__2_.v"
`include "routing/sb_2__5_.v"
`include "routing/sb_2__6_.v"
`include "routing/sb_5__1_.v"
`include "routing/sb_5__2_.v"
`include "routing/sb_5__5_.v"
`include "routing/sb_6__0_.v"
`include "routing/sb_6__1_.v"
`include "routing/sb_6__2_.v"
`include "routing/sb_6__6_.v"

// ------ Include tile module netlists -----

// ------ Include fabric top-level netlists -----
`include "fpga_top.v"

