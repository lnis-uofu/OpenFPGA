#ifndef VERILOG_FORMAL_RANDOME_TOP_TESTBENCH
#define VERILOG_FORMAL_RANDOME_TOP_TESTBENCH

#include <string>
#include "vpr_types.h"
#include "spice_types.h"

void dump_verilog_random_top_testbench(t_sram_orgz_info* cur_sram_orgz_info,
                                       char* circuit_name,
                                       const char* top_netlist_name,
                                       char* verilog_dir_path,
                                       t_syn_verilog_opts fpga_verilog_opts,
                                       t_spice verilog);

void print_verilog_random_top_testbench(const std::string& circuit_name,
                                        const std::string& verilog_fname,
                                        const std::string& verilog_dir,
                                        const std::vector<t_logical_block>& L_logical_blocks,
                                        const t_syn_verilog_opts& fpga_verilog_opts,
                                        const t_spice_params& simulation_parameters);

#endif
