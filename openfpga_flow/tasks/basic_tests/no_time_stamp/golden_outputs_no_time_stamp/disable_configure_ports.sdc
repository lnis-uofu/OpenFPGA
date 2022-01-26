#############################################
#	Synopsys Design Constraints (SDC)
#	For FPGA fabric 
#	Description: Disable configuration outputs of all the programmable cells for PnR
#	Author: Xifan TANG 
#	Organization: University of Utah 
#############################################

set_disable_timing fpga_top/grid_clb_*__*_/logical_tile_clb_mode_clb__*/logical_tile_clb_mode_default__fle_*/logical_tile_clb_mode_default__fle_mode_n*_lut*__ble*_*/logical_tile_clb_mode_default__fle_mode_n*_lut*__ble*_mode_default__lut*_*/lut*_*_/sram
set_disable_timing fpga_top/grid_clb_*__*_/logical_tile_clb_mode_clb__*/logical_tile_clb_mode_default__fle_*/logical_tile_clb_mode_default__fle_mode_n*_lut*__ble*_*/logical_tile_clb_mode_default__fle_mode_n*_lut*__ble*_mode_default__lut*_*/lut*_*_/sram_inv
set_disable_timing fpga_top/cbx_*__*_/mux_bottom_ipin_*/sram
set_disable_timing fpga_top/cbx_*__*_/mux_top_ipin_*/sram
set_disable_timing fpga_top/cbx_*__*_/mux_bottom_ipin_*/sram
set_disable_timing fpga_top/cbx_*__*_/mux_top_ipin_*/sram
set_disable_timing fpga_top/cby_*__*_/mux_left_ipin_*/sram
set_disable_timing fpga_top/cby_*__*_/mux_right_ipin_*/sram
set_disable_timing fpga_top/cby_*__*_/mux_left_ipin_*/sram
set_disable_timing fpga_top/cby_*__*_/mux_right_ipin_*/sram
set_disable_timing fpga_top/cbx_*__*_/mux_bottom_ipin_*/sram_inv
set_disable_timing fpga_top/cbx_*__*_/mux_top_ipin_*/sram_inv
set_disable_timing fpga_top/cbx_*__*_/mux_bottom_ipin_*/sram_inv
set_disable_timing fpga_top/cbx_*__*_/mux_top_ipin_*/sram_inv
set_disable_timing fpga_top/cby_*__*_/mux_left_ipin_*/sram_inv
set_disable_timing fpga_top/cby_*__*_/mux_right_ipin_*/sram_inv
set_disable_timing fpga_top/cby_*__*_/mux_left_ipin_*/sram_inv
set_disable_timing fpga_top/cby_*__*_/mux_right_ipin_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram_inv
set_disable_timing fpga_top/grid_clb_*__*_/logical_tile_clb_mode_clb__*/logical_tile_clb_mode_default__fle_*/logical_tile_clb_mode_default__fle_mode_n*_lut*__ble*_*/mux_ble*_out_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram
set_disable_timing fpga_top/grid_clb_*__*_/logical_tile_clb_mode_clb__*/logical_tile_clb_mode_default__fle_*/logical_tile_clb_mode_default__fle_mode_n*_lut*__ble*_*/mux_ble*_out_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/sram_inv
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/sram_inv
set_disable_timing fpga_top/grid_clb_*__*_/logical_tile_clb_mode_clb__*/mux_fle_*_in_*/sram
set_disable_timing fpga_top/grid_clb_*__*_/logical_tile_clb_mode_clb__*/mux_fle_*_in_*/sram_inv
set_disable_timing fpga_top/grid_io_top_*__*_/logical_tile_io_mode_io__*/logical_tile_io_mode_physical__iopad_*/GPIO_*_/DIR
set_disable_timing fpga_top/grid_io_right_*__*_/logical_tile_io_mode_io__*/logical_tile_io_mode_physical__iopad_*/GPIO_*_/DIR
set_disable_timing fpga_top/grid_io_bottom_*__*_/logical_tile_io_mode_io__*/logical_tile_io_mode_physical__iopad_*/GPIO_*_/DIR
set_disable_timing fpga_top/grid_io_left_*__*_/logical_tile_io_mode_io__*/logical_tile_io_mode_physical__iopad_*/GPIO_*_/DIR
