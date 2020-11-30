
OBJS += techlibs/quicklogic/synth_quicklogic.o


$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/ap3_ffs_map.v))
$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/ap3_openfpga_ffs_map.v))
$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/ap3_lut_map.v))
$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/ap3_latches_map.v))
$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/ap3_cells_map.v))
$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/ap3_arith_map.v))
$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/ap3_io_map.v))
$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/ap3_io_map_dbuff.v))
$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/cells_sim.v))
$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/ap3_cells_sim.v))

$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/abc9_model.v))
$(eval $(call add_share_file,share/quicklogic,techlibs/quicklogic/ap3_lutdefs.txt))

