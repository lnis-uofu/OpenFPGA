
void dump_verilog_decoder_memory_bank_ports(t_sram_orgz_info* cur_sram_orgz_info, 
                                            FILE* fp, 
                                            enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_config_peripherals(t_sram_orgz_info* cur_sram_orgz_info,
                                     char* verilog_dir_path,
                                     char* submodule_dir);
