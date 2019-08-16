
void dump_verilog_top_testbench_global_ports(FILE* fp, t_llist* head,
                                             enum e_dump_verilog_port_type dump_port_type);

void dump_verilog_top_testbench_global_ports_stimuli(FILE* fp, t_llist* head);

void dump_verilog_top_testbench_call_top_module(t_sram_orgz_info* cur_sram_orgz_info,
                                                FILE* fp,
                                                char* circuit_name,
                                                bool is_explicit_mapping);

void dump_verilog_top_testbench_stimuli(t_sram_orgz_info* cur_sram_orgz_info, 
                                        FILE* fp,
                                        t_spice verilog);

void dump_verilog_top_testbench(t_sram_orgz_info* cur_sram_orgz_info,
                                char* circuit_name,
                                char* top_netlist_name,
                                char* verilog_dir_path,
                                t_spice verilog);

void dump_verilog_input_blif_testbench(char* circuit_name,
                                       char* top_netlist_name,
                                       char* verilog_dir_path,
                                       t_spice verilog);
