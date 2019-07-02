

void fpga_spice_generate_bitstream_pb_generic_primitive(FILE* fp,
                                                        t_phy_pb* prim_phy_pb,
                                                        t_pb_type* prim_pb_type,
                                                        t_sram_orgz_info* cur_sram_orgz_info);

void fpga_spice_generate_bitstream_pb_primitive_lut(FILE* fp,
                                                    t_phy_pb* prim_phy_pb,
                                                    t_pb_type* prim_pb_type,
                                                    t_sram_orgz_info* cur_sram_orgz_info);
