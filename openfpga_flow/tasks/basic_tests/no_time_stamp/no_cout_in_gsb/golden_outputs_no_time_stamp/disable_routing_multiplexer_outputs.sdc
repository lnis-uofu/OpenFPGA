#############################################
#	Synopsys Design Constraints (SDC)
#	For FPGA fabric 
#	Description: Disable routing multiplexer outputs for PnR
#	Author: Xifan TANG 
#	Organization: University of Utah 
#############################################

set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/cbx_*__*_/mux_top_ipin_*/out
set_disable_timing fpga_top/cbx_*__*_/mux_bottom_ipin_*/out
set_disable_timing fpga_top/cby_*__*_/mux_left_ipin_*/out
set_disable_timing fpga_top/cby_*__*_/mux_right_ipin_*/out
set_disable_timing fpga_top/cby_*__*_/mux_left_ipin_*/out
set_disable_timing fpga_top/cby_*__*_/mux_right_ipin_*/out
set_disable_timing fpga_top/cby_*__*_/mux_left_ipin_*/out
set_disable_timing fpga_top/cby_*__*_/mux_right_ipin_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/cbx_*__*_/mux_bottom_ipin_*/out
set_disable_timing fpga_top/cbx_*__*_/mux_bottom_ipin_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_bottom_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_right_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_left_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/sb_*__*_/mux_top_track_*/out
set_disable_timing fpga_top/grid_clb_*__*_/logical_tile_clb_mode_clb__*/mux_fle_*_in_*/out
set_disable_timing fpga_top/grid_clb_*__*_/logical_tile_clb_mode_clb__*/logical_tile_clb_mode_default__fle_*/logical_tile_clb_mode_default__fle_mode_physical__fabric_*/mux_fabric_out_*/out
set_disable_timing fpga_top/grid_clb_*__*_/logical_tile_clb_mode_clb__*/logical_tile_clb_mode_default__fle_*/logical_tile_clb_mode_default__fle_mode_physical__fabric_*/logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__frac_logic_*/mux_frac_logic_out_*/out
set_disable_timing fpga_top/grid_clb_*__*_/logical_tile_clb_mode_clb__*/logical_tile_clb_mode_default__fle_*/logical_tile_clb_mode_default__fle_mode_physical__fabric_*/mux_ff_*_D_*/out
