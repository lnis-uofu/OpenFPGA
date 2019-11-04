#ifndef VERILOG_AUTOCHECK_TOP_TESTBENCH_H
#define VERILOG_AUTOCHECK_TOP_TESTBENCH_H


void dump_verilog_autocheck_top_testbench(t_sram_orgz_info* cur_sram_orgz_info,
                                          char* circuit_name,
                                          const char* top_netlist_name,
                                          char* verilog_dir_path,
                                          t_syn_verilog_opts fpga_verilog_opts,
                                          t_spice verilog);

#endif
