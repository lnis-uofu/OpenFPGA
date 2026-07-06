#############################################
#	Synopsys Design Constraints (SDC)
#	For FPGA fabric 
#	Description: Timing constraints for Grid logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core in PnR
#	Author: Xifan TANG 
#	Organization: University of Utah 
#############################################

#############################################
#	Define time unit 
#############################################
set_units -time s

set_max_delay -from fpga_top/grid_clb/logical_tile_ckbuf_mode_ckbuf__0/logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core_0/logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core/ckbuf_core_in[0] -to fpga_top/grid_clb/logical_tile_ckbuf_mode_ckbuf__0/logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core_0/logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core/ckbuf_core_out[0] 1.999999992e-11
set_min_delay -from fpga_top/grid_clb/logical_tile_ckbuf_mode_ckbuf__0/logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core_0/logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core/ckbuf_core_in[0] -to fpga_top/grid_clb/logical_tile_ckbuf_mode_ckbuf__0/logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core_0/logical_tile_ckbuf_mode_ckbuf_physical_mode__ckbuf_core/ckbuf_core_out[0] 1.999999992e-11
