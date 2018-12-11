void dump_verilog_top_auto_testbench(char* circuit_name,
                                char* top_netlist_name,
                                int num_clock,
                                t_syn_verilog_opts syn_verilog_opts,
                                t_spice verilog,
								char* postfix);


void dump_verilog_top_auto_preconf_testbench(char* circuit_name,
                                			char* top_netlist_name,
                                			int num_clock,
                                			t_syn_verilog_opts syn_verilog_opts,
                                			t_spice verilog,
											char* postfix,
											char* hex_file_path);

void dump_fpga_spice_hex(char* hex_file_path, 
						 char* chomped_circuit_name, 
						 t_sram_orgz_info* sram_verilog_orgz_info);
