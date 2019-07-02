
void encode_decoder_addr(int input,
                         int decoder_size, char* addr);

void dump_fpga_spice_bitstream(char* bitstream_file_name, 
                               char* circuit_name,
                               t_sram_orgz_info* cur_sram_orgz_info);

void vpr_fpga_generate_bitstream(t_vpr_setup vpr_setup,
                                 t_arch Arch,
                                 char* circuit_name,
                                 char* bitstream_file_path,
                                 t_sram_orgz_info** cur_sram_orgz_info);

void vpr_fpga_bitstream_generator(t_vpr_setup vpr_setup,
                                  t_arch Arch,
                                  char* circuit_name,
                                  t_sram_orgz_info** cur_sram_orgz_info);
